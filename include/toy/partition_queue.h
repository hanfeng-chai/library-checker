#pragma once
#include <toy/array.h>
#include <toy/buffer.h>
namespace toy {
// Ordered buckets are split only when an extremum is requested. Constant runs
// remain implicit, including when later insertions add different values.
struct PartitionQueue {
    struct Bucket {
        Buffer<i32> values;
        i64 lower = 0;
        usize copies = 0;
        i32 repeated = 0;
        bool empty() const { return !values.n && !copies; }
        void push(i32 x) {
            if (copies && x == repeated) { ++copies; return; }
            if (values.n == values.capacity) {
                values.reserve(std::max<usize>(32, 2 * values.n));
                if (!values.n) fill(values.p, values.p + 32, 0);
            }
            values[values.n++] = x;
        }
    };
    Array<Bucket> buckets{64};
    usize first = 0, count = 1, total;
    u64 state = 0x7ae8950f213bc4d1ull;
    inline static constexpr auto shuffle = [] {
        array<array<i32, 8>, 256> table{};
        for (int mask = 0; mask < 256; ++mask) { int used = 0; for (int j = 0; j < 8; ++j) if (mask >> j & 1) table[mask][used++] = j; }
        return table;
    }();
    Bucket& at(usize i) { return buckets[(first + i) & (buckets.n - 1)]; }
    explicit PartitionQueue(Buffer<i32> values = {}) : total(values.n) {
        values.reserve(std::max<usize>(32, values.n));
        if (values.n < 32) fill(values.p + values.n, values.p + 32, 0);
        at(0).values = std::move(values);
    }
    void room() {
        if (count < buckets.n) return;
        Array<Bucket> next(2 * buckets.n); for (usize i = 0; i < count; ++i) next[i] = std::move(at(i));
        swap(buckets.p, next.p); swap(buckets.n, next.n); first = 0;
    }
    u32 random() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return state; }
    template<bool Maximum> void split() {
        room(); usize index = Maximum ? count - 1 : 0; Bucket high = std::move(at(index)), low;
        low.lower = high.lower; usize n = high.values.n; low.values = Buffer<i32>(n); fill(low.values.p, low.values.p + 32, 0);
        auto sample = [&] { return high.values[(u64(random()) * n) >> 32]; };
        i32 a = sample(), b = sample(), c = sample(), pivot = std::max(std::min(a, b), std::min(std::max(a, b), c));
        usize l = 0, h = 0, i = 0; auto p = _mm256_set1_epi32(pivot);
        // Permute selected lanes to the front. Full stores are safe: output
        // positions never exceed the current input block, already in registers.
        for (; i + 8 <= n; i += 8) {
            auto x = _mm256_loadu_si256((const __m256i*)(high.values.p + i));
            u32 mask = _mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(p, x)));
            auto left = _mm256_permutevar8x32_epi32(x, _mm256_loadu_si256((const __m256i*)shuffle[mask].data()));
            auto right = _mm256_permutevar8x32_epi32(x, _mm256_loadu_si256((const __m256i*)shuffle[mask ^ 255].data()));
            _mm256_storeu_si256((__m256i*)(low.values.p + l), left); _mm256_storeu_si256((__m256i*)(high.values.p + h), right);
            usize take = popcount(mask); l += take; h += 8 - take;
        }
        for (; i < n; ++i) { i32 x = high.values[i]; if (x < pivot) low.values[l++] = x; else high.values[h++] = x; }
        low.values.n = l; high.values.n = h; high.lower = pivot;
        if (l) {
            if (high.copies && high.repeated < pivot) { low.repeated = high.repeated; low.copies = exchange(high.copies, 0); }
        } else {
            usize kept = 0, equal = 0;
            for (usize j = 0; j < h; ++j) { i32 x = high.values[j]; if (x == pivot) ++equal; else high.values[kept++] = x; }
            high.values.n = kept; high.lower = i64(pivot) + 1; low.repeated = pivot; low.copies = equal;
            if (!kept && high.copies && high.repeated < pivot) {
                low.repeated = high.repeated; low.copies = high.copies;
                high.repeated = pivot; high.copies = equal; high.lower = pivot;
            } else if (high.copies && high.repeated <= pivot) {
                if (high.repeated == pivot) low.copies += high.copies;
                else {
                    low.repeated = high.repeated; low.copies = high.copies; low.values.n = equal;
                    fill(low.values.p, low.values.p + equal, pivot);
                }
                high.copies = 0;
            }
        }
        if (!low.values.n) low.values = Buffer<i32>{};
        if (!high.values.n) high.values = Buffer<i32>{};
        if (low.empty()) { at(index) = std::move(high); return; }
        if (high.empty()) { at(index) = std::move(low); return; }
        if constexpr (Maximum) { at(index) = std::move(low); at(count++) = std::move(high); }
        else { at(0) = std::move(high); first = (first - 1) & (buckets.n - 1); at(0) = std::move(low); ++count; }
    }
    template<bool Maximum> Bucket& prepare() {
        while (at(Maximum ? count - 1 : 0).values.n > 32) split<Maximum>();
        return at(Maximum ? count - 1 : 0);
    }
    template<bool Maximum> static usize extreme(const Buffer<i32>& values) {
        i32 identity = Maximum ? numeric_limits<i32>::min() : numeric_limits<i32>::max();
        auto best = _mm256_set1_epi32(identity), indices = _mm256_setzero_si256();
        auto index = _mm256_setr_epi32(0,1,2,3,4,5,6,7), size = _mm256_set1_epi32(values.n), step = _mm256_set1_epi32(8);
        // Every allocated bucket has 32 initialized prefix cells. Mask the
        // inactive tail and carry the winning index beside each lane's value.
        for (usize i = 0; i < values.n; i += 8, index = _mm256_add_epi32(index, step)) {
            auto x = _mm256_blendv_epi8(_mm256_set1_epi32(identity), _mm256_loadu_si256((const __m256i*)(values.p + i)), _mm256_cmpgt_epi32(size, index));
            auto take = Maximum ? _mm256_cmpgt_epi32(x, best) : _mm256_cmpgt_epi32(best, x);
            indices = _mm256_blendv_epi8(indices, index, take); best = Maximum ? _mm256_max_epi32(best, x) : _mm256_min_epi32(best, x);
        }
        auto reduce = [](__m128i a, __m128i b) { if constexpr (Maximum) return _mm_max_epi32(a, b); else return _mm_min_epi32(a, b); };
        auto x = reduce(_mm256_castsi256_si128(best), _mm256_extracti128_si256(best, 1));
        x = reduce(x, _mm_shuffle_epi32(x, 0x4e)); x = reduce(x, _mm_shuffle_epi32(x, 0xb1));
        u32 matches = _mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpeq_epi32(best, _mm256_set1_epi32(_mm_cvtsi128_si32(x)))));
        return _mm_cvtsi128_si32(_mm256_castsi256_si128(_mm256_permutevar8x32_epi32(indices, _mm256_set1_epi32(countr_zero(matches)))));
    }
    template<bool Maximum> static pair<i32, usize> select(const Bucket& b) {
        if (!b.values.n) return {b.repeated, 0};
        usize i = extreme<Maximum>(b.values);
        if (b.copies && (Maximum ? b.repeated >= b.values[i] : b.repeated <= b.values[i])) return {b.repeated, b.values.n};
        return {b.values[i], i};
    }
    usize size() const { return total; }
    void push(i32 x) {
        usize l = 0, r = count;
        while (l + 1 < r) { usize m = (l + r) / 2; if (x < at(m).lower) r = m; else l = m; }
        at(l).push(x); ++total;
    }
    template<bool Maximum> i32 pop() {
        auto& b = prepare<Maximum>(); auto [answer, index] = select<Maximum>(b);
        if (index == b.values.n) --b.copies; else b.values[index] = b.values[--b.values.n];
        if (--total && b.empty()) {
            at(Maximum ? count - 1 : 0) = Bucket{};
            if constexpr (!Maximum) first = (first + 1) & (buckets.n - 1);
            --count;
        }
        return answer;
    }
    i32 min() { return select<false>(prepare<false>()).first; }
    i32 max() { return select<true>(prepare<true>()).first; }
    i32 pop_min() { return pop<false>(); }
    i32 pop_max() { return pop<true>(); }
};
}
