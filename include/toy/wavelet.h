#pragma once
#include <toy/radix_sort.h>

namespace toy {
// Compressed values; one interleaved bitmap/prefix record per 64 positions.
struct WaveletMatrix {
    struct Block { u64 bits; u32 before, padding; };
    Buffer<u32> values, zeros;
    Buffer<Block> rows;
    usize height, stride;
    explicit WaveletMatrix(span<const u32> input) : values(input.size()) {
        struct Item { u32 value, index; }; Buffer<Item> ordered(input.size());
        for (u32 i = 0; i < input.size(); ++i) ordered[i] = {input[i], i};
        radix_sort(span(ordered.p, ordered.n), [](Item x) { return x.value; });
        Buffer<u32> current(input.size()), next(input.size()), ones(input.size()); u32 count = 0;
        for (usize i = 0; i < ordered.n; ++i) {
            auto x = ordered[i]; if (!i || x.value != ordered[i - 1].value) values[count++] = x.value;
            current[x.index] = count - 1;
        }
        values.n = count; ordered = {};
        height = count <= 1 ? 0 : bit_width(count - 1); stride = (input.size() + 63) / 64 + 1;
        zeros = Buffer<u32>(height); rows = Buffer<Block>(height * stride);
        for (usize level = 0; level < height; ++level) {
            unsigned bit = height - 1 - level; Block* row = rows.p + level * stride;
            u32 zero_count = 0, one_count = 0, before = 0;
            for (usize start = 0; start < current.n; start += 64) {
                u64 bits = 0; usize length = min<usize>(64, current.n - start);
                for (usize j = 0; j < length; ++j) {
                    u32 x = current[start + j], one = (x >> bit) & 1;
                    bits |= u64(one) << j; next[zero_count] = x; ones[one_count] = x;
                    zero_count += one ^ 1; one_count += one;
                }
                row[start / 64] = {bits, before, 0}; before += popcount(bits);
            }
            row[stride - 1] = {0, before, 0}; zeros[level] = zero_count;
            if (one_count) memcpy(next.p + zero_count, ones.p, one_count * 4);
            swap(current, next);
        }
    }
    static u32 rank(const Block* row, u32 end) {
        auto block = row[end / 64]; return block.before + popcount(block.bits & ((u64(1) << (end & 63)) - 1));
    }
    u32 kth(u32 l, u32 r, u32 k) const {
        u32 result = 0;
        for (usize level = 0; level < height; ++level) {
            const auto* row = rows.p + level * stride;
            u32 a = rank(row, l), b = rank(row, r), z = r - l - b + a;
            bool one = k >= z; u32 mask = -u32(one);
            l = (l - a) ^ (((l - a) ^ (zeros[level] + a)) & mask);
            r = (r - b) ^ (((r - b) ^ (zeros[level] + b)) & mask);
            k -= z & mask; result = 2 * result + one;
        }
        return values[result];
    }
    Buffer<u32> kth_batch(Buffer<u32> left, Buffer<u32> right, Buffer<u32> index) const {
        Buffer<u32> result(left.n); fill(result.p, result.p + result.n, 0u);
        for (usize level = 0; level < height; ++level) {
            const auto* row = rows.p + level * stride;
            for (usize i = 0; i < left.n; ++i) {
                u32 l = left[i], r = right[i], a = rank(row, l), b = rank(row, r), z = r - l - b + a;
                u32 one = index[i] >= z, mask = -one;
                left[i] = (l - a) ^ (((l - a) ^ (zeros[level] + a)) & mask);
                right[i] = (r - b) ^ (((r - b) ^ (zeros[level] + b)) & mask);
                index[i] -= z & mask; result[i] = 2 * result[i] + one;
            }
        }
        for (usize i = 0; i < result.n; ++i) result[i] = values[result[i]];
        return result;
    }
};
}
