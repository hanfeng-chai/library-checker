#pragma once

#include <bits/extc++.h>

namespace toy {

template <unsigned KeyBits = 32, unsigned RadixBits = 16, std::random_access_iterator Iterator,
          class Key>
void radix_sort_u32(Iterator first, Iterator last, Key key) {
    static_assert(1 <= RadixBits && RadixBits <= 16);
    static_assert(RadixBits < KeyBits && KeyBits <= 2 * RadixBits);
    constexpr uint32_t bucket_count = uint32_t(1) << RadixBits;
    constexpr uint32_t mask = bucket_count - 1;
    using Value = std::iter_value_t<Iterator>;

    std::size_t count = last - first;
    if (count < bucket_count / 8) {
        std::stable_sort(first, last, [&](const Value &left, const Value &right) {
            return key(left) < key(right);
        });
        return;
    }

    std::array<uint32_t, bucket_count> low_count{}, high_count{};
    for (const Value &value : std::ranges::subrange(first, last)) {
        uint32_t current = key(value);
        if constexpr (KeyBits < 32) assert(current < (uint32_t(1) << KeyBits));
        ++low_count[current & mask];
        ++high_count[(current >> RadixBits) & mask];
    }
    for (uint32_t i = 1; i < bucket_count; ++i) {
        low_count[i] += low_count[i - 1];
        high_count[i] += high_count[i - 1];
    }

    std::vector<Value> temporary(count);
    for (std::size_t i = count; i--;) {
        uint32_t current = key(first[i]);
        temporary[--low_count[current & mask]] = std::move(first[i]);
    }
    for (std::size_t i = count; i--;) {
        uint32_t current = key(temporary[i]);
        first[--high_count[(current >> RadixBits) & mask]] = std::move(temporary[i]);
    }
}

template <unsigned KeyBits = 64, unsigned RadixBits = 16, std::random_access_iterator Iterator,
          class Key>
void radix_sort_u64(Iterator first, Iterator last, Key key) {
    static_assert(1 <= RadixBits && RadixBits <= 16);
    static_assert(RadixBits < KeyBits && KeyBits <= 64);
    constexpr uint32_t bucket_count = uint32_t(1) << RadixBits;
    constexpr uint64_t mask = bucket_count - 1;
    constexpr unsigned passes = (KeyBits + RadixBits - 1) / RadixBits;
    using Value = std::iter_value_t<Iterator>;

    std::size_t count = last - first;
    if (count < bucket_count / 8) {
        std::stable_sort(first, last, [&](const Value &left, const Value &right) {
            return key(left) < key(right);
        });
        return;
    }

    std::vector<Value> temporary(count);
    bool in_temporary = false;
    for (unsigned pass = 0; pass < passes; ++pass) {
        unsigned shift = pass * RadixBits;
        std::array<uint32_t, bucket_count> frequencies{};
        if (!in_temporary) {
            for (std::size_t i = 0; i < count; ++i) ++frequencies[key(first[i]) >> shift & mask];
        } else {
            for (const Value &value : temporary) ++frequencies[key(value) >> shift & mask];
        }
        for (uint32_t i = 1; i < bucket_count; ++i) frequencies[i] += frequencies[i - 1];
        if (!in_temporary) {
            for (std::size_t i = count; i--;)
                temporary[--frequencies[key(first[i]) >> shift & mask]] = std::move(first[i]);
        } else {
            for (std::size_t i = count; i--;)
                first[--frequencies[key(temporary[i]) >> shift & mask]] = std::move(temporary[i]);
        }
        in_temporary = !in_temporary;
    }
    if (in_temporary) std::move(temporary.begin(), temporary.end(), first);
}

} // namespace toy
