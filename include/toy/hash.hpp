#pragma once

#include <bits/extc++.h>

namespace toy {

template<std::unsigned_integral Key, std::unsigned_integral Value>
class IntegerHashMap {
    std::vector<Key> keys;
    std::vector<Value> values;
    std::vector<uint64_t> occupied;
    std::size_t mask;
    unsigned shift;

    bool used(std::size_t index) const {
        return occupied[index / 64] >> (index % 64) & 1;
    }

    std::size_t locate(Key key) const {
        constexpr uint64_t multiplier = 11'995'408'973'635'179'863ULL;
        std::size_t index = uint64_t(key) * multiplier >> shift;
        while (used(index) && keys[index] != key)
            index = (index + 1) & mask;
        return index;
    }

public:
    explicit IntegerHashMap(std::size_t capacity)
        : keys(std::bit_ceil(std::max<std::size_t>(4, capacity + 1))),
          values(keys.size()), occupied((keys.size() + 63) / 64),
          mask(keys.size() - 1), shift(64 - std::bit_width(mask)) {}

    void set(Key key, Value value) {
        std::size_t index = locate(key);
        keys[index] = key;
        values[index] = value;
        occupied[index / 64] |= 1ULL << (index % 64);
    }

    Value get(Key key) const {
        std::size_t index = locate(key);
        return used(index) ? values[index] : 0;
    }

    Value exchange(Key key, Value value) {
        std::size_t index = locate(key);
        Value previous = used(index) ? values[index] : 0;
        keys[index] = key;
        values[index] = value;
        occupied[index / 64] |= 1ULL << (index % 64);
        return previous;
    }

    Value add(Key key, Value delta) {
        std::size_t index = locate(key);
        if (!used(index)) {
            keys[index] = key;
            values[index] = 0;
            occupied[index / 64] |= 1ULL << (index % 64);
        }
        return values[index] += delta;
    }
};

using U32HashMap = IntegerHashMap<uint32_t, uint32_t>;
using U64HashMap = IntegerHashMap<uint64_t, uint64_t>;

} // namespace toy
