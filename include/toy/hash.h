#pragma once
#include <toy/buffer.h>

namespace toy {
// Empty is reserved. Capacity bounds the number of distinct inserted keys.
template<class Key, class Value, Key Empty = Key(-1)> struct HashMap {
    struct Entry { Key key; Value value; };
    Buffer<Entry> table;
    usize mask;
    unsigned shift;
    explicit HashMap(usize capacity) : table(bit_ceil(max<usize>(4, 2 * capacity + 1))), mask(table.n - 1), shift(64 - countr_zero(table.n)) {
        for (usize i = 0; i < table.n; ++i) table[i].key = Empty;
    }
    usize bucket(Key key) const { return ((u64(key) ^ (u64(key) >> 32)) * 11995408973635179863ull) >> shift; }
    usize locate(Key key) const {
        usize i = bucket(key);
        while (table[i].key != Empty && table[i].key != key) i = (i + 1) & mask;
        return i;
    }
    Value* find(Key key) {
        auto& e = table[locate(key)]; return e.key == Empty ? nullptr : &e.value;
    }
    Value get(Key key) const { const auto& e = table[locate(key)]; return e.key == Empty ? Value{} : e.value; }
    Value& operator[](Key key) {
        auto& e = table[locate(key)]; if (e.key == Empty) e.key = key, e.value = Value{}; return e.value;
    }
};
}
