// https://judge.yosupo.jp/problem/range_set_range_composite
//
// f_i(x) = a_i x + b_i; queries: assign c x + d on [l, r), and evaluate f_{r-1}(...f_l(x)) mod p.
//
// PROTOTYPE of a wide range-assignment tree (TWideAssignmentTree below) whose tags point at
// per-assignment power records (TPowerStore), written here to measure the
// idea on the judge before it becomes a proust engine. The tree is generic over the operation like
// proust's engines (FromRepeated, Merge), plus an optional MergeMany hook folding a run of results
// at once -- here with AVX2 -- and falls back to a Merge chain without it.
//
// This file is not submitted as is: bundle.py inlines the library into submit.cpp.

// The judge compiles without -DNDEBUG; the library's asserts are for development only
// (stress.sh keeps them alive with -DKEEP_ASSERTS).
#if !defined(NDEBUG) && !defined(KEEP_ASSERTS)
#define NDEBUG
#endif

#include <algorithm>
#include <cassert>
#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <span>
#include <vector>

#if defined(__AVX2__)
#include <immintrin.h>
#endif

#include <sys/stat.h>
#include <unistd.h>


namespace {
    constexpr uint32_t MOD = 998244353;
    constexpr uint32_t TWICE_MOD = 2 * MOD;

    // Montgomery arithmetic with R = 2^32 and lazy residues: every stored coefficient is some value
    // in [0, 2p) congruent to the real one, and is only brought to [0, p) on output. A multiplier A
    // is kept in Montgomery form (A R mod p) and a translation B in plain form, so that the
    // reduction of A * B is B scaled by A, plain again -- no conversion inside a composition.
    constexpr uint32_t NEG_INVERSE = [] {
        uint32_t inverse = MOD;  // Newton's iteration: each step doubles the number of correct bits
        for (int step = 0; step < 4; ++step) {
            inverse *= 2 - MOD * inverse;
        }
        return -inverse;
    }();
    static_assert(MOD * NEG_INVERSE == static_cast<uint32_t>(-1));
    static_assert(MOD < (1u << 30));  // keeps every bound below within 32 bits
    constexpr uint32_t R2 = static_cast<uint32_t>((static_cast<unsigned __int128>(1) << 64) % MOD);

    // t R^{-1} mod p, for t < 2^32 p; the result is below t / 2^32 + p.
    inline uint32_t Reduce(const uint64_t t) noexcept {
        const uint32_t m = static_cast<uint32_t>(t) * NEG_INVERSE;
        return static_cast<uint32_t>((t + static_cast<uint64_t>(m) * MOD) >> 32);
    }

    // [0, 4p) -> [0, 2p), branchless: when x < 2p the subtraction wraps around and min drops it.
    inline uint32_t Fold(const uint32_t x) noexcept {
        return std::min(x, x - TWICE_MOD);
    }

    inline uint32_t ToMontgomery(const uint32_t x) noexcept {
        return Reduce(static_cast<uint64_t>(x) * R2);
    }

    // x -> A x + B, with A in Montgomery form and B plain, both in [0, 2p).
    struct TAffine {
        uint32_t A;
        uint32_t B;
    };

    // The assigned value: A in the high half, B in the low one. A >= 1 by the constraints, so its
    // Montgomery form is non-zero too, zero never encodes a real function and serves as the
    // "no lazy tag" sentinel, saving a flag per node.
    using TPacked = uint64_t;

    inline TAffine Unpack(const TPacked value) noexcept {
        return {static_cast<uint32_t>(value >> 32), static_cast<uint32_t>(value)};
    }

    inline TPacked Pack(const TAffine f) noexcept {
        return (static_cast<uint64_t>(f.A) << 32) | f.B;
    }

    // Converts input coefficients (a, b < p).
    inline TAffine FromInput(const uint32_t a, const uint32_t b) noexcept {
        return {ToMontgomery(a), b};
    }

    inline TAffine Identity() noexcept {
        return {ToMontgomery(1), 0};
    }

    // Applies `first`, then `second`: second(first(x)).
    //   A = A1 A2: both Montgomery, products < 4p^2, so the reduction stays below 2p.
    //   B = A2 B1 + B2: one reduction for both terms, as B2 enters pre-multiplied by R
    //   (B2 * 2^32 reduces to B2 itself); the sum stays below 4p, folded back to 2p.
    inline TAffine Compose(const TAffine first, const TAffine second) noexcept {
        const uint32_t a = Reduce(static_cast<uint64_t>(first.A) * second.A);
        const uint32_t b = Fold(Reduce(static_cast<uint64_t>(second.A) * first.B + (static_cast<uint64_t>(second.B) << 32)));
        return {a, b};
    }

    // f(x) in [0, p), for x < p.
    inline uint32_t Evaluate(const TAffine f, const uint32_t x) noexcept {
        uint32_t y = Fold(Reduce(static_cast<uint64_t>(f.A) * x + (static_cast<uint64_t>(f.B) << 32)));
        return y >= MOD ? y - MOD : y;
    }

    // Powers of assigned functions, kept per assignment instead of looked up by value.
    //
    // An assignment of f over [l, r) first creates a record f, f^2, f^4, ... f^(2^(w-1)) with
    // w = bit_width(r - l), and hands the engine the record's handle as its value. Every power the
    // engine can later ask for that value -- a covered slot, a tag pushed into a child, a tagged
    // boundary slot in a query -- has a length of at most r - l, so f^k is then a plain read of the
    // record plus one composition per further set bit of k: no hashing, no rebuilding.
    //
    // Records live in one array, in handle order. When it fills up, the records still referenced by
    // a tag are slid down in place (handles stay valid, only their offsets change), the rest is
    // dropped. Handle 0 is "no tag".
    class TPowerStore {
    public:
        static void Reserve(const size_t handles) {
            Offsets.reserve(handles + 1);
            Lengths.reserve(handles + 1);
            // Capacity is when to compact; the array itself grows in Create whenever a record does
            // not fit. Live records are few (tens of thousands of steps at N = 5 * 10^5).
            Capacity = INITIAL_CAPACITY;
            Steps.resize(Capacity + MAX_STEPS);
            Offsets.assign(1, 0);
            Lengths.assign(1, 0);
            Used = 0;
        }

        static bool Full() noexcept {
            return Used + MAX_STEPS > Capacity;
        }

        // A record for f with powers up to f^(2^(count - 1)), count in 1..MAX_STEPS.
        static uint32_t Create(TAffine f, const uint32_t count) {
            if (Used + count > Steps.size()) {
                Steps.resize(std::max(2 * Steps.size(), Used + count));
            }
            const uint32_t handle = static_cast<uint32_t>(Offsets.size());
            Offsets.push_back(static_cast<uint32_t>(Used));
            Lengths.push_back(static_cast<uint8_t>(count));
            TAffine* steps = Steps.data() + Used;
            steps[0] = f;
            for (uint32_t step = 1; step < count; ++step) {
                f = Compose(f, f);
                steps[step] = f;
            }
            Used += count;
            return handle;
        }

        static TAffine Power(const uint32_t handle, const size_t length) noexcept {
            if (length == 0) {
                return Identity();
            }
            const TAffine* steps = Steps.data() + Offsets[handle];
            assert(static_cast<size_t>(std::bit_width(length)) <= Lengths[handle]);
            // Powers of one function commute, so the set bits can be taken in any order.
            size_t rest = length;
            TAffine power = steps[std::countr_zero(rest)];
            rest &= rest - 1;
            while (rest) {
                power = Compose(power, steps[std::countr_zero(rest)]);
                rest &= rest - 1;
            }
            return power;
        }

        // Keeps exactly the records `forEachLive(mark)` marks. Marks go to a bitmap, scanned word by
        // word in handle (and thus offset) order, so every record only ever slides down.
        template<typename TForEachLive>
        static void Compact(TForEachLive&& forEachLive) {
            LiveBits.assign((Offsets.size() + 63) / 64, 0);
            forEachLive([](const uint32_t handle) noexcept { LiveBits[handle >> 6] |= uint64_t(1) << (handle & 63); });
            size_t write = 0;
            for (size_t word = 0; word < LiveBits.size(); ++word) {
                for (uint64_t bits = LiveBits[word]; bits; bits &= bits - 1) {
                    const size_t handle = word * 64 + static_cast<size_t>(std::countr_zero(bits));
                    std::memmove(Steps.data() + write, Steps.data() + Offsets[handle], Lengths[handle] * sizeof(TAffine));
                    Offsets[handle] = static_cast<uint32_t>(write);
                    write += Lengths[handle];
                }
            }
            Used = write;
            // Mostly live: grow, so that compactions stay rare compared with the records created.
            if (Used > Capacity / 2) {
                Capacity *= 2;
            }
        }

    private:
#ifndef POWER_STORE_LOG
#define POWER_STORE_LOG 18
#endif
        static constexpr size_t INITIAL_CAPACITY = size_t(1) << POWER_STORE_LOG;  // overridden in stress tests to compact often
        static constexpr uint32_t MAX_STEPS = 32;

        inline static std::vector<TAffine> Steps;
        inline static std::vector<uint32_t> Offsets;
        inline static std::vector<uint8_t> Lengths;
        inline static std::vector<uint64_t> LiveBits;
        inline static size_t Used = 0;
        inline static size_t Capacity = 0;
    };

#if defined(__AVX2__) && !defined(SCALAR_FOLD)
    // The composition of eight functions as a balanced tree: 8 -> 4 -> 2 -> 1, each level composing
    // adjacent pairs in parallel, instead of a chain of seven dependent compositions.
    //
    // Multipliers and translations are held in separate registers, one function per 32-bit lane.
    // A level reads each pair (2k, 2k + 1) from the two halves of 64-bit lane k, which is exactly
    // what _mm256_mul_epu32 multiplies, and leaves the composed pair in the low half of that lane.
    class TEightFold {
    public:
        // `count` in 1..8 functions stored as 64-bit words, the multiplier in the high half if
        // `MultiplierHigh` (TPacked) or in the low one (TAffine); missing ones are identities and
        // are not read.
        template<bool MultiplierHigh>
        static TAffine Fold(const void* data, const size_t count) noexcept {
            const TAffine one = Identity();
            const uint64_t identityWord = MultiplierHigh
                ? (static_cast<uint64_t>(one.A) << 32) | one.B
                : (static_cast<uint64_t>(one.B) << 32) | one.A;
            const __m256i lanes = _mm256_setr_epi64x(0, 1, 2, 3);
            const __m256i identity = _mm256_set1_epi64x(static_cast<long long>(identityWord));
            const __m256i lowMask = _mm256_cmpgt_epi64(_mm256_set1_epi64x(static_cast<long long>(count)), lanes);
            const __m256i highMask = _mm256_cmpgt_epi64(_mm256_set1_epi64x(static_cast<long long>(count) - 4), lanes);
            const auto* words = static_cast<const long long*>(data);
            const __m256i low = _mm256_blendv_epi8(identity, _mm256_maskload_epi64(words, lowMask), lowMask);
            const __m256i high = _mm256_blendv_epi8(identity, _mm256_maskload_epi64(words + 4, highMask), highMask);

            // Split into [lows of 4 words, highs of 4 words] per register, then regroup into the
            // eight low halves and the eight high halves.
            const __m256i split = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
            const __m256i lowSplit = _mm256_permutevar8x32_epi32(low, split);
            const __m256i highSplit = _mm256_permutevar8x32_epi32(high, split);
            const __m256i lowHalves = _mm256_permute2x128_si256(lowSplit, highSplit, 0x20);
            const __m256i highHalves = _mm256_permute2x128_si256(lowSplit, highSplit, 0x31);
            __m256i a = MultiplierHigh ? highHalves : lowHalves;
            __m256i b = MultiplierHigh ? lowHalves : highHalves;

            ComposePairs(a, b);  // 4 functions, in the low halves of the 64-bit lanes
            const __m256i gather = _mm256_setr_epi32(0, 2, 4, 6, 0, 2, 4, 6);
            a = _mm256_permutevar8x32_epi32(a, gather);
            b = _mm256_permutevar8x32_epi32(b, gather);
            ComposePairs(a, b);  // 2 functions
            a = _mm256_permutevar8x32_epi32(a, gather);
            b = _mm256_permutevar8x32_epi32(b, gather);
            ComposePairs(a, b);  // 1 function, in lane 0

            return {
                static_cast<uint32_t>(_mm_cvtsi128_si32(_mm256_castsi256_si128(a))),
                static_cast<uint32_t>(_mm_cvtsi128_si32(_mm256_castsi256_si128(b))),
            };
        }

    private:
        // The vector form of Reduce: for each 64-bit lane t, t R^{-1} mod p in its low half.
        static __m256i Reduce(const __m256i t) noexcept {
            const __m256i m = _mm256_mul_epu32(t, _mm256_set1_epi32(static_cast<int>(NEG_INVERSE)));
            return _mm256_srli_epi64(_mm256_add_epi64(t, _mm256_mul_epu32(m, _mm256_set1_epi32(static_cast<int>(MOD)))), 32);
        }

        static __m256i FoldTwice(const __m256i x) noexcept {
            return _mm256_min_epu32(x, _mm256_sub_epi32(x, _mm256_set1_epi32(static_cast<int>(TWICE_MOD))));
        }

        // The scalar Compose(f_2k, f_2k+1) for every 64-bit lane k at once.
        static void ComposePairs(__m256i& a, __m256i& b) noexcept {
            const __m256i secondA = _mm256_srli_epi64(a, 32);
            const __m256i secondBShifted = _mm256_and_si256(b, _mm256_set1_epi64x(static_cast<long long>(0xFFFFFFFF00000000ull)));
            a = Reduce(_mm256_mul_epu32(a, secondA));
            b = FoldTwice(Reduce(_mm256_add_epi64(_mm256_mul_epu32(b, secondA), secondBShifted)));
        }
    };

    // Composition of results [from, to), in order.
    inline TAffine FoldResults(const TAffine* data, const size_t from, const size_t to) noexcept {
        if (to - from < 3) {
            TAffine result = data[from];
            for (size_t index = from + 1; index < to; ++index) {
                result = Compose(result, data[index]);
            }
            return result;
        }
        TAffine result = TEightFold::Fold<false>(data + from, std::min<size_t>(8, to - from));
        for (size_t index = from + 8; index < to; index += 8) {
            result = Compose(result, TEightFold::Fold<false>(data + index, std::min<size_t>(8, to - index)));
        }
        return result;
    }
#endif

    struct TCompositionOperation {
        template<typename TValue, typename TOperationResult>
        static TOperationResult FromRepeated(const TValue& value, const size_t times) noexcept {
            return TPowerStore::Power(value, times);
        }

        template<typename TOperationResult>
        static TOperationResult Merge(const TOperationResult& lhs, const TOperationResult& rhs) noexcept {
            return Compose(lhs, rhs);
        }

#if defined(__AVX2__) && !defined(SCALAR_FOLD)
        // Optional: the composition of results [from, to), equal to Merge applied left to right.
        template<typename TOperationResult>
        static TOperationResult MergeMany(const TOperationResult* data, const size_t from, const size_t to) noexcept {
            return FoldResults(data, from, to);
        }
#endif
    };

    template<typename TOperation, typename TOperationResult>
    concept HasMergeMany = requires(const TOperationResult* data, size_t from, size_t to) {
        { TOperation::template MergeMany<TOperationResult>(data, from, to) } -> std::same_as<TOperationResult>;
    };

    // A range-assignment tree with 2^BranchingLog children per node.
    //
    // Level 1 nodes hold one result per element. A level h >= 2 node has B slots, each covering a
    // level h - 1 child of span B^(h - 1): the slot keeps that child's result and, if the whole child
    // was assigned a value it has not been told about yet, that value as a tag (`NoTag` otherwise).
    // Every fully covered slot has a power-of-two length, so f^length is one ladder step.
    //
    // Assignment writes covered slots directly and descends into at most two boundary children per
    // level, pushing their tag down first; queries never push and answer a tagged boundary slot with
    // f^length. The last node of a level may have slots past the end: they hold an arbitrary valid
    // result and are never reached, as every operation stays within [0, Length).
    template<typename TValue, typename TOperationResult, typename TOperation, size_t BranchingLog, TValue NoTag>
    class TWideAssignmentTree {
    public:
        void InitFrom(std::span<const TValue> values) {
            std::vector<TOperationResult> results(values.size());
            for (size_t index = 0; index < values.size(); ++index) {
                results[index] = One(values[index]);
            }
            InitFromResults(results);
        }

        // Builds from per-element results, as FromRepeated(value, 1) would give them.
        void InitFromResults(std::span<const TOperationResult> results) {
            Length = results.size();
            const TOperationResult filler = results[0];

            const size_t leaves = (Length + MASK) >> BranchingLog;
            Leaves.assign(leaves, TLeafNode{});
            for (size_t index = 0; index < leaves * B; ++index) {
                Leaves[index >> BranchingLog].Results[index & MASK] = index < Length ? results[index] : filler;
            }

            Inner.clear();
            Height = 1;
            for (size_t below = leaves; below > 1;) {
                const size_t count = (below + MASK) >> BranchingLog;
                std::vector<TInnerNode> level(count);
                for (size_t child = 0; child < count * B; ++child) {
                    TInnerNode& node = level[child >> BranchingLog];
                    node.Results[child & MASK] = child < below ? FoldNode(Height, child) : filler;
                    node.Tags[child & MASK] = NoTag;
                }
                Inner.push_back(std::move(level));
                ++Height;
                below = count;
            }
        }

        void SetRange(const size_t left, const size_t right, const TValue& value) {
            if (left >= right) {
                return;
            }
            if (Height == 1) {
                std::fill(Leaves[0].Results + left, Leaves[0].Results + right, One(value));
                return;
            }
            AssignIn(Height, 0, left, right, value);
        }

        // Calls `visit(tag)` for every tag slot of every node, including tags hidden below another
        // tag (stale, never read again) -- marking those too only keeps a few more records alive.
        template<typename TVisit>
        void ForEachTag(TVisit&& visit) const {
            for (const auto& level : Inner) {
                for (const TInnerNode& node : level) {
                    for (const TValue& tag : node.Tags) {
                        visit(tag);
                    }
                }
            }
        }

        TOperationResult ComputeRange(const size_t left, const size_t right) const {
            return QueryIn(Height, 0, left, right);
        }

    private:
        static constexpr size_t B = size_t(1) << BranchingLog;
        static constexpr size_t MASK = B - 1;

        struct alignas(64) TLeafNode {
            TOperationResult Results[B];
        };

        struct alignas(64) TInnerNode {
            TOperationResult Results[B];
            TValue Tags[B];
        };

        std::vector<TLeafNode> Leaves;               // level 1
        std::vector<std::vector<TInnerNode>> Inner;  // Inner[h - 2] is level h
        unsigned Height = 1;                         // the root's level
        size_t Length = 0;

        static TOperationResult One(const TValue& value) noexcept {
            return TOperation::template FromRepeated<TValue, TOperationResult>(value, 1);
        }

        static TOperationResult Fold(const TOperationResult* results, const size_t from, const size_t to) noexcept {
            if constexpr (HasMergeMany<TOperation, TOperationResult>) {
                return TOperation::template MergeMany<TOperationResult>(results, from, to);
            } else {
                TOperationResult result = results[from];
                for (size_t index = from + 1; index < to; ++index) {
                    result = TOperation::Merge(result, results[index]);
                }
                return result;
            }
        }

        TOperationResult* ResultsOf(const unsigned level, const size_t index) noexcept {
            return level == 1 ? Leaves[index].Results : Inner[level - 2][index].Results;
        }

        TOperationResult FoldNode(const unsigned level, const size_t index) noexcept {
            return Fold(ResultsOf(level, index), 0, B);
        }

        // The whole node at (level, index) becomes `value`: what a tag on its parent slot means.
        void Overwrite(const unsigned level, const size_t index, const TValue& value) noexcept {
            if (level == 1) {
                std::fill(Leaves[index].Results, Leaves[index].Results + B, One(value));
                return;
            }
            TInnerNode& node = Inner[level - 2][index];
            const TOperationResult power = TOperation::template FromRepeated<TValue, TOperationResult>(value, size_t(1) << ((level - 1) * BranchingLog));
            std::fill(node.Results, node.Results + B, power);
            std::fill(node.Tags, node.Tags + B, value);
        }

        // Assigns [low, high), relative to the node, inside the level >= 2 node.
        void AssignIn(const unsigned level, const size_t index, const size_t low, const size_t high, const TValue& value) noexcept {
            TInnerNode& node = Inner[level - 2][index];
            const unsigned shift = (level - 1) * BranchingLog;
            const size_t span = size_t(1) << shift;
            const size_t first = low >> shift;
            const size_t last = (high - 1) >> shift;
            const size_t coveredFrom = (low + span - 1) >> shift;
            const size_t coveredTo = high >> shift;

            if (coveredFrom < coveredTo) {
                const TOperationResult power = TOperation::template FromRepeated<TValue, TOperationResult>(value, span);
                std::fill(node.Results + coveredFrom, node.Results + coveredTo, power);
                std::fill(node.Tags + coveredFrom, node.Tags + coveredTo, value);
            }
            if (first == last) {
                if (coveredFrom >= coveredTo) {
                    Descend(level, index, first, low - first * span, high - first * span, value);
                }
                return;
            }
            if (low & (span - 1)) {
                Descend(level, index, first, low & (span - 1), span, value);
            }
            if (high & (span - 1)) {
                Descend(level, index, last, 0, high & (span - 1), value);
            }
        }

        // Assigns [low, high) inside the child in `slot` of the level >= 2 node, then refreshes the slot.
        void Descend(const unsigned level, const size_t index, const size_t slot, const size_t low, const size_t high, const TValue& value) noexcept {
            TInnerNode& node = Inner[level - 2][index];
            const size_t child = index * B + slot;
            if (node.Tags[slot] != NoTag) {
                Overwrite(level - 1, child, node.Tags[slot]);
                node.Tags[slot] = NoTag;
            }
            if (level - 1 == 1) {
                std::fill(Leaves[child].Results + low, Leaves[child].Results + high, One(value));
            } else {
                AssignIn(level - 1, child, low, high, value);
            }
            node.Results[slot] = FoldNode(level - 1, child);
        }

        TOperationResult QueryIn(const unsigned level, const size_t index, const size_t low, const size_t high) const noexcept {
            if (level == 1) {
                return Fold(Leaves[index].Results, low, high);
            }
            const TInnerNode& node = Inner[level - 2][index];
            const unsigned shift = (level - 1) * BranchingLog;
            const size_t span = size_t(1) << shift;
            const auto part = [&](const size_t slot, const size_t from, const size_t to) {
                if (from == 0 && to == span) {
                    return node.Results[slot];
                }
                if (node.Tags[slot] != NoTag) {
                    return TOperation::template FromRepeated<TValue, TOperationResult>(node.Tags[slot], to - from);
                }
                return QueryIn(level - 1, index * B + slot, from, to);
            };

            const size_t first = low >> shift;
            const size_t last = (high - 1) >> shift;
            if (first == last) {
                return part(first, low - first * span, high - first * span);
            }
            const size_t coveredFrom = (low + span - 1) >> shift;
            const size_t coveredTo = high >> shift;
            bool hasResult = false;
            TOperationResult result{};
            const auto append = [&](const TOperationResult& next) {
                result = hasResult ? TOperation::Merge(result, next) : next;
                hasResult = true;
            };
            if (low & (span - 1)) {
                append(part(first, low & (span - 1), span));
            }
            if (coveredFrom < coveredTo) {
                append(Fold(node.Results, coveredFrom, coveredTo));
            }
            if (high & (span - 1)) {
                append(part(last, 0, high & (span - 1)));
            }
            return result;
        }
    };

#ifndef BRANCHING_LOG
#define BRANCHING_LOG 4
#endif

    using TEngine = TWideAssignmentTree<uint32_t, TAffine, TCompositionOperation, BRANCHING_LOG, uint32_t(0)>;

    // ---- I/O: the whole input is read at once, the whole output is written at once ----

    // Up to eight decimal digits, one per byte, the most significant one in the lowest byte, as
    // values 0..9: three multiply-and-shift rounds pair them into 2-, 4- and 8-digit numbers.
    inline uint32_t ParseEightDigits(uint64_t digits) noexcept {
        digits = (digits * 10 + (digits >> 8)) & 0x00FF00FF00FF00FFull;
        digits = (digits * 100 + (digits >> 16)) & 0x0000FFFF0000FFFFull;
        return static_cast<uint32_t>((digits * 10000 + (digits >> 32)) & 0xFFFFFFFFull);
    }

    class TReader {
    public:
        // Reads all of stdin. A regular file gets a buffer of exactly its size, a pipe one that grows
        // as needed; neither is zero-filled beyond the padding, as every fresh page touched for
        // nothing is a page fault -- a few milliseconds per tens of megabytes.
        TReader() {
            size_t size = 0;
            struct stat status;
            if (::fstat(STDIN_FILENO, &status) == 0 && S_ISREG(status.st_mode)) {
                const size_t capacity = static_cast<size_t>(status.st_size);
                Buffer = std::make_unique_for_overwrite<char[]>(capacity + PADDING);
                size = ReadAll(Buffer.get(), capacity);
            } else {
                size_t capacity = size_t(1) << 20;
                Buffer = std::make_unique_for_overwrite<char[]>(capacity + PADDING);
                while (true) {
                    size += ReadAll(Buffer.get() + size, capacity - size);
                    if (size < capacity) {
                        break;
                    }
                    auto grown = std::make_unique_for_overwrite<char[]>(2 * capacity + PADDING);
                    std::memcpy(grown.get(), Buffer.get(), size);
                    Buffer = std::move(grown);
                    capacity *= 2;
                }
            }
            // Zero padding: every number can be loaded as a whole 8-byte word, and the parse stops
            // at a zero byte like at any other separator.
            std::memset(Buffer.get() + size, 0, PADDING);
            Position = Buffer.get();
        }

        // The input holds only digits and whitespace, so a byte is a digit iff its high nibble is 3.
        uint32_t Next() noexcept {
            while (static_cast<unsigned char>(*Position) < '0') {
                ++Position;
            }
            uint64_t word;
            std::memcpy(&word, Position, sizeof(word));
            const uint64_t nonDigits = (word & 0xF0F0F0F0F0F0F0F0ull) ^ 0x3030303030303030ull;
            const uint64_t digits = word - 0x3030303030303030ull;  // garbage above the number is shifted out
            if (nonDigits) {
                const unsigned length = static_cast<unsigned>(std::countr_zero(nonDigits)) >> 3;  // 1..7
                Position += length;
                return ParseEightDigits(digits << (64 - 8 * length));
            }

            uint32_t value = ParseEightDigits(digits);
            Position += 8;
            while (static_cast<unsigned char>(*Position) >= '0') {
                value = value * 10 + static_cast<uint32_t>(*Position - '0');
                ++Position;
            }
            return value;
        }

    private:
        static constexpr size_t PADDING = 16;
        std::unique_ptr<char[]> Buffer;
        const char* Position;

        // Up to `length` bytes of stdin, fewer only at its end.
        static size_t ReadAll(char* destination, const size_t length) noexcept {
            size_t done = 0;
            while (done < length) {
                const ssize_t chunk = ::read(STDIN_FILENO, destination + done, length - done);
                if (chunk <= 0) {
                    break;
                }
                done += static_cast<size_t>(chunk);
            }
            return done;
        }
    };

    // "0000" .. "9999".
    constexpr auto FOUR_DIGITS = [] {
        std::array<std::array<char, 4>, 10000> table{};
        for (unsigned value = 0; value < table.size(); ++value) {
            table[value] = {
                static_cast<char>('0' + value / 1000),
                static_cast<char>('0' + value / 100 % 10),
                static_cast<char>('0' + value / 10 % 10),
                static_cast<char>('0' + value % 10),
            };
        }
        return table;
    }();

    // Output through a small fixed buffer: memory proportional to the answer count would be
    // touched page by page for nothing.
    class TWriter {
    public:
        ~TWriter() {
            Flush();
        }

        // For value < 10^9, which every answer modulo p is.
        void Line(uint32_t value) noexcept {
            if (Position + MAX_LINE > Buffer + CAPACITY) [[unlikely]] {
                Flush();
            }
            if (value >= 100000000) {
                *Position++ = static_cast<char>('0' + value / 100000000);
                value %= 100000000;
                PutFour(value / 10000);
                PutFour(value % 10000);
            } else if (value >= 10000) {
                PutLeading(value / 10000);
                PutFour(value % 10000);
            } else {
                PutLeading(value);
            }
            *Position++ = '\n';
        }

    private:
        static constexpr size_t CAPACITY = size_t(1) << 16;
        static constexpr size_t MAX_LINE = 11;  // nine digits and a newline, with a byte to spare
        char Buffer[CAPACITY];
        char* Position = Buffer;

        void Flush() noexcept {
            const char* from = Buffer;
            while (from < Position) {
                const ssize_t written = ::write(STDOUT_FILENO, from, static_cast<size_t>(Position - from));
                if (written <= 0) {
                    break;
                }
                from += written;
            }
            Position = Buffer;
        }

        void PutFour(const uint32_t group) noexcept {
            std::memcpy(Position, FOUR_DIGITS[group].data(), 4);
            Position += 4;
        }

        // A group without its leading zeros.
        void PutLeading(const uint32_t group) noexcept {
            const unsigned length = 1 + (group >= 10) + (group >= 100) + (group >= 1000);
            std::memcpy(Position, FOUR_DIGITS[group].data() + (4 - length), length);
            Position += length;
        }
    };
} // namespace

int main() {
    TReader in;
    const uint32_t n = in.Next();
    const uint32_t q = in.Next();

    TPowerStore::Reserve(q);
    std::vector<TAffine> initial(n);
    for (uint32_t i = 0; i < n; ++i) {
        const uint32_t a = in.Next();
        const uint32_t b = in.Next();
        initial[i] = FromInput(a, b);
    }
    TEngine engine;
    engine.InitFromResults(initial);

    TWriter out;
    for (uint32_t i = 0; i < q; ++i) {
        const uint32_t type = in.Next();
        const uint32_t l = in.Next();
        const uint32_t r = in.Next();
        if (type == 0) {
            const uint32_t c = in.Next();
            const uint32_t d = in.Next();
            if (l < r) {
                if (TPowerStore::Full()) {
                    TPowerStore::Compact([&](auto mark) { engine.ForEachTag([&](const uint32_t tag) { if (tag) mark(tag); }); });
                }
                engine.SetRange(l, r, TPowerStore::Create(FromInput(c, d), static_cast<uint32_t>(std::bit_width(r - l))));
            }
        } else {
            const uint32_t x = in.Next();
            if (l == r) {
                out.Line(x);  // the empty composition is the identity
            } else {
                out.Line(Evaluate(engine.ComputeRange(l, r), x));
            }
        }
    }
    return 0;
}
