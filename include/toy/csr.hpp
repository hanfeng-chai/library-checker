#pragma once

#include <bits/extc++.h>

namespace toy {

template <class T>
class CsrBuckets {
    std::vector<std::size_t> offsets;
    std::vector<T> values;

  public:
    template <class Key>
    CsrBuckets(std::size_t bucket_count, std::vector<T> input, Key key)
        : offsets(bucket_count + 1), values(input.size()) {
        for (const T &value : input) ++offsets[key(value) + 1];
        std::partial_sum(offsets.begin(), offsets.end(), offsets.begin());
        std::vector<std::size_t> cursor = offsets;
        for (T &value : input) values[cursor[key(value)]++] = std::move(value);
    }

    std::span<const T> operator[](std::size_t bucket) const {
        const T *first = values.empty() ? nullptr : values.data() + offsets[bucket];
        return {first, offsets[bucket + 1] - offsets[bucket]};
    }
};

} // namespace toy
