#pragma once

#include <toy/affine.hpp>

#include <immintrin.h>

namespace toy {

template <uint32_t Mod>
struct Montgomery32 {
    static_assert(Mod & 1);
    static_assert(Mod < (uint32_t(1) << 30));

    static consteval uint32_t inverse() {
        uint32_t result = 1;
        for (int i = 0; i < 5; ++i) result *= 2u + result * Mod;
        return result;
    }

    static constexpr uint32_t modulus_twice = 2 * Mod;
    static constexpr uint32_t inverse_modulus = inverse();
    static constexpr uint32_t radix = (uint64_t(1) << 32) % Mod;
    static constexpr uint32_t radix_squared = (uint64_t)radix * radix % Mod;

    static uint32_t add(uint32_t left, uint32_t right) {
        uint32_t sum = left + right;
        return std::min(sum, sum - modulus_twice);
    }

    static uint32_t reduce(uint64_t value) {
        uint32_t factor = (uint32_t)value * inverse_modulus;
        return (value + (uint64_t)factor * Mod) >> 32;
    }

    static uint32_t multiply(uint32_t left, uint32_t right) {
        return reduce((uint64_t)left * right);
    }

    static uint32_t transform(uint32_t value) { return multiply(value, radix_squared); }
};

template <uint32_t Mod>
struct Montgomery32x8 {
    using Scalar = Montgomery32<Mod>;

    static __m256i modulus() { return _mm256_set1_epi32(Mod); }
    static __m256i modulus_twice() { return _mm256_set1_epi32(Scalar::modulus_twice); }
    static __m256i inverse_modulus() { return _mm256_set1_epi32(Scalar::inverse_modulus); }

    static __m256i add(__m256i left, __m256i right) {
        __m256i sum = _mm256_add_epi32(left, right);
        return _mm256_min_epu32(sum, _mm256_sub_epi32(sum, modulus_twice()));
    }

    static __m256i reduce(__m256i even_products, __m256i odd_products) {
        __m256i inverse = inverse_modulus();
        __m256i mod = modulus();
        __m256i even_factors = _mm256_mul_epu32(even_products, inverse);
        __m256i odd_factors = _mm256_mul_epu32(odd_products, inverse);
        __m256i even = _mm256_add_epi64(even_products, _mm256_mul_epu32(even_factors, mod));
        __m256i odd = _mm256_add_epi64(odd_products, _mm256_mul_epu32(odd_factors, mod));
        return _mm256_blend_epi32(_mm256_bsrli_epi128(even, 4), odd, 0b10101010);
    }

    template <bool BroadcastRight = false>
    static __m256i multiply(__m256i left, __m256i right) {
        __m256i even = _mm256_mul_epu32(left, right);
        __m256i odd = _mm256_mul_epu32(_mm256_bsrli_epi128(left, 4),
                                       BroadcastRight ? right : _mm256_bsrli_epi128(right, 4));
        return reduce(even, odd);
    }

    static __m256i transform(__m256i value) {
        return multiply<true>(value, _mm256_set1_epi32(Scalar::radix_squared));
    }

    static __m256i normalize(__m256i value) {
        value = multiply<true>(value, _mm256_set1_epi32(1));
        return _mm256_min_epu32(value, _mm256_sub_epi32(value, modulus()));
    }
};

template <uint32_t Mod, std::size_t Capacity, std::size_t MaxQueries, unsigned BranchBits = 3>
class WideAffinePointTree {
    static constexpr uint32_t branch = uint32_t(1) << BranchBits;
    static_assert(branch == 8);
    static_assert(MaxQueries > 0);

    using Scalar = Montgomery32<Mod>;
    using Simd = Montgomery32x8<Mod>;

    static consteval unsigned tree_height() {
        std::size_t count = Capacity;
        unsigned result = 1;
        while (count > branch) {
            count /= branch;
            ++result;
        }
        return result;
    }

    static constexpr std::size_t level_offset(unsigned level) {
        std::size_t result = 0;
        std::size_t count = Capacity;
        while (level--) {
            count = (count + branch - 1) / branch;
            result += count * branch;
        }
        return result;
    }

    static constexpr unsigned height = tree_height();
    static constexpr std::size_t storage_size = level_offset(height);
    static constexpr std::size_t padded_queries =
        (MaxQueries + branch - 1) & ~(std::size_t(branch) - 1);

    struct Masks {
        alignas(32) uint32_t left[branch][branch]{};
        alignas(32) uint32_t right[branch][branch]{};
        alignas(32) uint32_t inside[branch][branch][branch]{};

        consteval Masks() {
            for (uint32_t begin = 1; begin < branch; ++begin)
                for (uint32_t i = begin; i < branch; ++i) left[begin][i] = uint32_t(-1);
            for (uint32_t end = 0; end < branch; ++end)
                for (uint32_t i = 0; i < end; ++i) right[end][i] = uint32_t(-1);
            for (uint32_t begin = 0; begin < branch; ++begin)
                for (uint32_t end = 0; end < branch; ++end)
                    for (uint32_t i = begin; i < end; ++i) inside[begin][end][i] = uint32_t(-1);
        }
    };

    inline static constexpr Masks masks{};

    alignas(64) std::array<uint32_t, storage_size> multiplier;
    alignas(64) std::array<uint32_t, storage_size> addition;
    alignas(64) std::array<std::array<uint32_t, padded_queries>, height> result_multiplier;
    alignas(64) std::array<std::array<uint32_t, padded_queries>, height> result_addition;
    std::size_t result_count = 0;

    static void apply_block(uint32_t *multipliers, uint32_t *additions, __m256i multiplier_value,
                            __m256i addition_value) {
        __m256i old_multiplier = _mm256_load_si256(reinterpret_cast<const __m256i *>(multipliers));
        __m256i old_addition = _mm256_load_si256(reinterpret_cast<const __m256i *>(additions));
        __m256i new_multiplier = Simd::template multiply<true>(old_multiplier, multiplier_value);
        __m256i new_addition = Simd::add(
            Simd::template multiply<true>(old_addition, multiplier_value), addition_value);
        _mm256_store_si256(reinterpret_cast<__m256i *>(multipliers), new_multiplier);
        _mm256_store_si256(reinterpret_cast<__m256i *>(additions), new_addition);
    }

    static void apply_masked_block(uint32_t *multipliers, uint32_t *additions,
                                   __m256i multiplier_value, __m256i addition_value,
                                   const uint32_t *mask_values) {
        __m256i mask = _mm256_load_si256(reinterpret_cast<const __m256i *>(mask_values));
        __m256i old_multiplier = _mm256_load_si256(reinterpret_cast<const __m256i *>(multipliers));
        __m256i old_addition = _mm256_load_si256(reinterpret_cast<const __m256i *>(additions));
        __m256i new_multiplier = Simd::template multiply<true>(old_multiplier, multiplier_value);
        __m256i new_addition = Simd::add(
            Simd::template multiply<true>(old_addition, multiplier_value), addition_value);
        _mm256_store_si256(reinterpret_cast<__m256i *>(multipliers),
                           _mm256_blendv_epi8(old_multiplier, new_multiplier, mask));
        _mm256_store_si256(reinterpret_cast<__m256i *>(additions),
                           _mm256_blendv_epi8(old_addition, new_addition, mask));
    }

    void push(unsigned level, uint32_t index) {
        std::size_t node = level_offset(level) + index;
        uint32_t multiplier_value = std::exchange(multiplier[node], Scalar::transform(1));
        uint32_t addition_value = std::exchange(addition[node], 0);
        apply_block(multiplier.data() + level_offset(level - 1) + index * branch,
                    addition.data() + level_offset(level - 1) + index * branch,
                    _mm256_set1_epi32(multiplier_value), _mm256_set1_epi32(addition_value));
    }

  public:
    WideAffinePointTree() {
        std::fill(multiplier.begin(), multiplier.end(), Scalar::transform(1));
        std::fill(addition.begin(), addition.end(), 0);
    }

    void reset(const std::vector<uint32_t> &values) {
        assert(values.size() <= Capacity);
        constexpr std::size_t leaves = level_offset(1);
        std::fill(addition.begin(), addition.begin() + leaves, 0);
        std::copy(values.begin(), values.end(), addition.begin());
        for (std::size_t i = 0; i < leaves; i += branch) {
            __m256i current =
                _mm256_load_si256(reinterpret_cast<const __m256i *>(addition.data() + i));
            _mm256_store_si256(reinterpret_cast<__m256i *>(addition.data() + i),
                               Simd::transform(current));
        }
        result_count = 0;
    }

    void apply(uint32_t left, uint32_t right, Affine<Mod> function) {
        uint32_t transformed_a = Scalar::transform(function.a);
        uint32_t transformed_b = Scalar::transform(function.b);
        __m256i multiplier_value = _mm256_set1_epi32(transformed_a);
        __m256i addition_value = _mm256_set1_epi32(transformed_b);

        for (unsigned level = height - 1; level; --level) {
            push(level, left >> (BranchBits * level));
            push(level, right >> (BranchBits * level));
        }
        for (unsigned level = 0; level < height; ++level) {
            std::size_t offset = level_offset(level);
            if ((left >> BranchBits) >= (right >> BranchBits)) {
                uint32_t block_begin = left & ~(branch - 1);
                apply_masked_block(multiplier.data() + offset + block_begin,
                                   addition.data() + offset + block_begin, multiplier_value,
                                   addition_value,
                                   masks.inside[left & (branch - 1)][right & (branch - 1)]);
                return;
            }

            uint32_t left_block = left & ~(branch - 1);
            uint32_t right_block = right & ~(branch - 1);
            apply_masked_block(multiplier.data() + offset + left_block,
                               addition.data() + offset + left_block, multiplier_value,
                               addition_value, masks.left[left & (branch - 1)]);
            apply_masked_block(multiplier.data() + offset + right_block,
                               addition.data() + offset + right_block, multiplier_value,
                               addition_value, masks.right[right & (branch - 1)]);
            left = (left + branch - 1) >> BranchBits;
            right >>= BranchBits;
        }
    }

    void collect(uint32_t index) {
        assert(result_count < MaxQueries);
        result_addition[0][result_count] = addition[index];
        for (unsigned level = 1; level < height; ++level) {
            std::size_t node = level_offset(level) + (index >> (BranchBits * level));
            result_multiplier[level][result_count] = multiplier[node];
            result_addition[level][result_count] = addition[node];
        }
        ++result_count;
    }

    std::span<const uint32_t> resolve() {
        std::size_t padded_count = (result_count + branch - 1) & ~(std::size_t(branch) - 1);
        for (std::size_t i = result_count; i < padded_count; ++i) {
            result_addition[0][i] = 0;
            for (unsigned level = 1; level < height; ++level) {
                result_multiplier[level][i] = Scalar::transform(1);
                result_addition[level][i] = 0;
            }
        }
        for (std::size_t i = 0; i < result_count; i += branch) {
            __m256i value =
                _mm256_load_si256(reinterpret_cast<const __m256i *>(result_addition[0].data() + i));
            for (unsigned level = 1; level < height; ++level) {
                __m256i current_multiplier = _mm256_load_si256(
                    reinterpret_cast<const __m256i *>(result_multiplier[level].data() + i));
                __m256i current_addition = _mm256_load_si256(
                    reinterpret_cast<const __m256i *>(result_addition[level].data() + i));
                value = Simd::add(Simd::multiply(value, current_multiplier), current_addition);
            }
            _mm256_store_si256(reinterpret_cast<__m256i *>(result_addition[0].data() + i),
                               Simd::normalize(value));
        }
        return {result_addition[0].data(), result_count};
    }
};

} // namespace toy
