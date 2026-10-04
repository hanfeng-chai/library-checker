#pragma once
#include <toy/buffer.h>
namespace toy {
// Monotone u64 priorities. Equal keys share bucket zero; other buckets hold
// contiguous records, so redistribution reads memory sequentially.
template <class Key = u64>
struct IntegerHeap {
    struct Item {
        Key key;
        u32 value;
    };
    struct Result {
        u64 key;
        u32 value;
    };
    std::array<Buffer<Item>, sizeof(Key) * 8> bucket;
    Buffer<u32> zero;
    std::array<Item, 32> small;
    u64 last = 0, nonempty = 0;
    u32 count = 0;
    bool tiny = true;
    bool empty() const { return !count; }
    void clear() {
        for (auto &a : bucket) a.n = 0;
        zero.n = 0;
        last = nonempty = count = 0;
        tiny = true;
    }
    template <class T>
    static void append(Buffer<T> &a, T x) {
        if (a.n == a.capacity) a.reserve(std::max<usize>(32, a.capacity * 2));
        a.p[a.n++] = x;
    }
    void put(Item x) {
        if (x.key == Key(last))
            append(zero, x.value);
        else {
            u32 b = std::bit_width(Key(x.key ^ Key(last))) - 1;
            append(bucket[b], x);
            nonempty |= 1ull << b;
        }
    }
    void push(u64 key, u32 value) {
        if (tiny) {
            if (count < small.size()) {
                small[count++] = {Key(key), value};
                return;
            }
            for (auto x : small) put(x);
            tiny = false;
        }
        put({Key(key), value});
        ++count;
    }
    Result pop() {
        if (tiny) {
            u32 i = 0;
            for (u32 j = 1; j < count; ++j)
                if (Key(small[j].key - Key(last)) < Key(small[i].key - Key(last))) i = j;
            auto x = small[i];
            small[i] = small[--count];
            last += Key(x.key - Key(last));
            return {last, x.value};
        }
        if (!zero.n) {
            auto &a = bucket[std::countr_zero(nonempty)];
            nonempty &= nonempty - 1;
            if (a.n == 1) {
                auto x = a[0];
                a.n = 0;
                last += Key(x.key - Key(last));
                --count;
                return {last, x.value};
            }
            if constexpr (sizeof(Key) == 8) {
                last = ~0ull;
                for (auto x : std::span(a.p, a.n)) last = std::min(last, x.key);
            } else {
                Key delta = ~Key(0);
                for (auto x : std::span(a.p, a.n)) delta = std::min(delta, Key(x.key - Key(last)));
                last += delta;
            }
            for (auto x : std::span(a.p, a.n)) put(x);
            a.n = 0;
        }
        --count;
        return {last, zero[--zero.n]};
    }
};
using RadixHeap = IntegerHeap<u64>;
// All live keys must be in [last,last+2^31). u64 priorities wrap through u32
// bucket keys, halving the stored record size. Dijkstra weights <2^31 suffice.
using BoundedRadixHeap = IntegerHeap<u32>;
} // namespace toy
