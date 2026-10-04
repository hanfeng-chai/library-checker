#pragma once
#include <toy/convolution_integer.h>
#include <toy/fft_convolution.h>
namespace toy {
// Packed real convolution: even coefficients are real, odd coefficients imaginary.
struct IntegerFFT {
    struct Fixed {
        Buffer<fft_detail::C4> spectrum;
        Buffer<u32> input;
        bool cyclic;
    };
    std::optional<fft_detail::FFT> plan;
    Buffer<fft_detail::C4> x, y;
    usize capacity = 0;
    static void prepare(Buffer<fft_detail::C4> &out, std::span<const u32> input) {
        using namespace fft_detail;
        auto order = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
        usize blocks = (input.size() + 7) / 8;
        for (usize i = 0; i < blocks; ++i) {
            alignas(32) u32 tail[8]{};
            __m256i values;
            if (8 * i + 8 <= input.size())
                values = _mm256_loadu_si256((const __m256i *)(input.data() + 8 * i));
            else {
                memcpy(tail, input.data() + 8 * i, (input.size() - 8 * i) * 4);
                values = _mm256_load_si256((const __m256i *)tail);
            }
            values = _mm256_permutevar8x32_epi32(values, order);
            out[i] = {_mm256_cvtepi32_pd(_mm256_castsi256_si128(values)),
                      _mm256_cvtepi32_pd(_mm256_extracti128_si256(values, 1))};
        }
        memset(out.p + blocks, 0, (out.n - blocks) * sizeof(C4));
    }
    void ensure(usize n) {
        if (capacity < n) {
            plan.emplace(n);
            x = fft_detail::storage<fft_detail::C4>(n);
            y = fft_detail::storage<fft_detail::C4>(n);
            capacity = n;
        }
        x.n = y.n = n;
    }
    Fixed fixed(Buffer<u32> input, usize size, bool cyclic = false) {
        ensure(size / 8);
        auto spectrum = fft_detail::storage<fft_detail::C4>(size / 8);
        prepare(spectrum, input);
        plan->template transform<false, 256>(spectrum);
        return {std::move(spectrum), std::move(input), cyclic};
    }
    Buffer<u64> operator()(std::span<const u32> a, const Fixed &b) {
        return (*this)(a, std::span<const u32>(b.input), &b);
    }
    Buffer<u64> operator()(std::span<const u32> a, std::span<const u32> b,
                           const Fixed *fixed = nullptr) {
        using namespace fft_detail;
        if (a.empty() || b.empty()) return {};
        if (!fixed && std::bit_ceil(a.size() + b.size() - 1) > (1 << 20))
            return convolution_integer(a, b);
        usize count = fixed && fixed->cyclic ? fixed->spectrum.n * 8 : a.size() + b.size() - 1;
        usize size = fixed ? fixed->spectrum.n * 8 : std::max<usize>(32, std::bit_ceil(count)),
              n = size / 8;
        ensure(n);
        const auto &fft = *plan;
        bool square = !fixed && a.data() == b.data() && a.size() == b.size();
        prepare(x, a);
        fft.transform<false, 256>(x);
        if (!square && !fixed) {
            prepare(y, b);
            fft.transform<false, 256>(y);
        }
        const auto &rhs = fixed ? fixed->spectrum : square ? x : y;
        auto conjugate = [](C4 v) { return C4{v.x, -v.y}; };
        auto scale = _mm256_set1_pd(1. / n);
        auto product = [&](C4 a, C4 b, Point w) {
            b.x *= scale;
            b.y *= scale;
            auto aw = a * C4::splat(w), c = a * FFT::lane<0>(b);
            c = madd(FFT::rotated<1>(a, aw), FFT::lane<1>(b), c);
            c = madd(FFT::rotated<2>(a, aw), FFT::lane<2>(b), c);
            return madd(FFT::rotated<3>(a, aw), FFT::lane<3>(b), c);
        };
        for (usize i = 0; i < n; ++i) {
            usize j = i ? i ^ (std::bit_floor(i) - 1) : 0;
            if (i > j) continue;
            auto difference_a = x[i] - conjugate(x[j]), difference_b = rhs[i] - conjugate(rhs[j]);
            C4 odd_a{difference_a.y * _mm256_set1_pd(.5), difference_a.x * _mm256_set1_pd(-.5)};
            C4 odd_b{difference_b.y * _mm256_set1_pd(.5), difference_b.x * _mm256_set1_pd(-.5)};
            auto w = fft.root[i] * fft.root[i];
            auto oo = product(odd_a, odd_b, w);
            auto correction = oo + FFT::rotated<1>(oo, oo * C4::splat(w));
            x[i] = product(x[i], rhs[i], w) + correction;
            if (i != j) x[j] = product(x[j], rhs[j], w.conj()) + conjugate(correction);
        }
        fft.transform<true, 256>(x);
        usize blocks = (count + 7) / 8;
        auto result = fft_detail::storage<u64>(blocks * 8);
        result.n = count;
        auto bias = _mm256_set1_pd(0x1p52), zero = _mm256_setzero_pd();
        auto integer_bias = _mm256_set1_epi64x(0x4330000000000000ll);
        for (usize i = 0; i < blocks; ++i) {
            // Nonnegative coefficients below 2^52: adding 2^52 rounds to integer,
            // and the mantissa bits are that integer. Clamp harmless negative noise.
            auto even = _mm256_sub_epi64(
                _mm256_castpd_si256(_mm256_add_pd(_mm256_max_pd(x[i].x, zero), bias)),
                integer_bias);
            auto odd = _mm256_sub_epi64(
                _mm256_castpd_si256(_mm256_add_pd(_mm256_max_pd(x[i].y, zero), bias)),
                integer_bias);
            auto lo = _mm256_unpacklo_epi64(even, odd), hi = _mm256_unpackhi_epi64(even, odd);
            _mm256_store_si256((__m256i *)(result.p + 8 * i),
                               _mm256_permute2x128_si256(lo, hi, 0x20));
            _mm256_store_si256((__m256i *)(result.p + 8 * i + 4),
                               _mm256_permute2x128_si256(lo, hi, 0x31));
        }
        return result;
    }
};
inline Buffer<u64> convolution_fft_integer(std::span<const u32> a, std::span<const u32> b) {
    IntegerFFT fft;
    return fft(a, b);
}
} // namespace toy
