#include <algorithm>
#include <stack>
#include <cctype>
#include <array>
#include <bitset>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <cstdio>
#include <string>
#include <cmath>
#include <queue>
#include <numeric>
#include <unordered_map>
#include <limits>
#include <concepts>
#include <istream>
#include <utility>
#include <deque>
#include <iostream>
#include <set>
#include <optional>
#include <fstream>
#include <ostream>
#include <type_traits>
#include <cassert>
#include <random>
#include <cstdint>
#include <bit>
#include <iterator>
#include <functional>
#include <map>

// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_primes

#ifndef KK2_MATH_PRIME_TABLE_HPP
#define KK2_MATH_PRIME_TABLE_HPP 1

#ifndef KK2_MATH_ISPRIME_TABLE_HPP
#define KK2_MATH_ISPRIME_TABLE_HPP 1


#ifndef KK2_DATA_STRUCTURE_MY_BITSET_HPP
#define KK2_DATA_STRUCTURE_MY_BITSET_HPP 1


#ifndef KK2_BIT_BITCOUNT_HPP
#define KK2_BIT_BITCOUNT_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
    typename std::conditional<is_signed_int128<T>::value,
                              make_unsigned_int128<T>,
                              typename std::conditional<std::is_signed<T>::value,
                                                        std::make_unsigned<T>,
                                                        std::common_type<T>>::type>::type;

#else

template <typename T> using is_integral = std::enable_if_t<std::is_integral<T>::value>;
template <typename T> using is_signed = std::enable_if_t<std::is_signed<T>::value>;
template <typename T> using is_unsigned = std::enable_if_t<std::is_unsigned<T>::value>;
template <typename T> using to_unsigned = std::make_unsigned<T>;

#endif // _MSC_VER

template <typename T> using is_integral_t = std::enable_if_t<is_integral<T>::value>;
template <typename T> using is_signed_t = std::enable_if_t<is_signed<T>::value>;
template <typename T> using is_unsigned_t = std::enable_if_t<is_unsigned<T>::value>;

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP

namespace kk2 {

template <Integral T> constexpr int ctz(T x) {
    assert(x != T(0));

    if constexpr (sizeof(T) <= 4) {
        return __builtin_ctz(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_ctzll(x);
    } else {
        if (x & 0xffffffffffffffff)
            return __builtin_ctzll((unsigned long long)(x & 0xffffffffffffffff));
        return 64 + __builtin_ctzll((unsigned long long)(x >> 64));
    }
}

template <Integral T> constexpr int lsb(T x) {
    assert(x != T(0));

    return ctz(x);
}

template <Integral T> constexpr int clz(T x) {
    assert(x != T(0));

    if constexpr (sizeof(T) <= 4) {
        return __builtin_clz(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_clzll(x);
    } else {
        if (x >> 64) return __builtin_clzll((unsigned long long)(x >> 64));
        return 64 + __builtin_clzll((unsigned long long)(x & 0xffffffffffffffff));
    }
}

template <Integral T> constexpr int msb(T x) {
    assert(x != T(0));

    return sizeof(T) * 8 - 1 - clz(x);
}

template <Integral T> constexpr int popcount(T x) {

    if constexpr (sizeof(T) <= 4) {
        return __builtin_popcount(x);
    } else if constexpr (sizeof(T) <= 8) {
        return __builtin_popcountll(x);
    } else {
        return __builtin_popcountll((unsigned long long)(x >> 64))
               + __builtin_popcountll((unsigned long long)(x & 0xffffffffffffffff));
    }
}

}; // namespace kk2

#endif // KK2_BIT_BITCOUNT_HPP
#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1


namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T>
using is_standard_istream = typename std::conditional<std::is_same<T, std::istream>::value
                                                          || std::is_same<T, std::ifstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T>
using is_standard_ostream = typename std::conditional<std::is_same<T, std::ostream>::value
                                                          || std::is_same<T, std::ofstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T>
using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

template <class T>
concept StandardInputStream = is_standard_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept StandardOutputStream = is_standard_ostream<std::remove_cvref_t<T>>::value;

template <class T>
concept InputStream = is_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept OutputStream = is_ostream<std::remove_cvref_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

template <class Accessor> struct MonotoneRankRange {
    Accessor _accessor;

    struct StrideRange {
        Accessor _accessor;
        int _start, _end, _step;

        struct Iterator {
            using value_type = int;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            using reference = int;
            using pointer = void;

            int rank, end, step;
            mutable typename Accessor::MonotoneCursor cursor;

            Iterator(int rank_, int end_, int step_, const Accessor &accessor)
                : rank(rank_),
                  end(end_),
                  step(step_),
                  cursor(accessor.monotone_cursor()) {}

            int operator*() const { return cursor[rank]; }

            Iterator &operator++() {
                rank = step < end - rank ? rank + step : end;
                return *this;
            }

            Iterator operator++(int) {
                Iterator result = *this;
                ++*this;
                return result;
            }

            bool operator==(const Iterator &other) const { return rank == other.rank; }
        };

        Iterator begin() const { return Iterator(_start, _end, _step, _accessor); }
        Iterator end() const { return Iterator(_end, _end, _step, _accessor); }

        int size() const {
            if (_start == _end) return 0;
            return (_end - _start - 1) / _step + 1;
        }

        std::vector<int> to_vec() const {
            std::vector<int> result;
            result.reserve(size());
            for (int value : *this) result.push_back(value);
            return result;
        }
    };

    auto begin() const { return stride(0, 1).begin(); }
    auto end() const { return stride(0, 1).end(); }
    int size() const { return _accessor.size(); }

    int operator[](int rank) const { return _accessor[rank]; }

    auto monotone_cursor() const { return _accessor.monotone_cursor(); }

    StrideRange stride(int start, int step) const { return stride(start, step, size()); }

    StrideRange stride(int start, int step, int end) const {
        assert(0 <= start && start <= end && end <= size() && step > 0);
        return StrideRange{_accessor, start, end, step};
    }

    std::vector<int> to_vec() const {
        std::vector<int> result;
        result.reserve(size());
        for (int value : *this) result.push_back(value);
        return result;
    }
};

struct DynamicBitSet {
    struct RankSelect;

    using T = DynamicBitSet;
    using UInt = std::uint64_t;
    constexpr static int BLOCK_SIZE = sizeof(UInt) * 8;
    constexpr static int BLOCK_SIZE_LOG = __builtin_ctz(BLOCK_SIZE);
    constexpr static int BLOCK_MASK = BLOCK_SIZE - 1;
    constexpr static UInt ONE = 1;
    int n;
    std::vector<UInt> block;

    DynamicBitSet(int n_ = 0, bool x = 0) : n(n_) {
        UInt val = x ? -1 : 0;
        block.assign((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG, val);
        if (n & BLOCK_MASK) block.back() >>= BLOCK_SIZE - (n & BLOCK_MASK);
        // fit the last block
    }

    DynamicBitSet(const std::string &s) : n(s.size()) {
        block.resize((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        set(s);
    }

    inline int size() const { return n; }

    int word_count() const { return block.size(); }

    UInt &word(int i) {
        assert(0 <= i && i < word_count());
        return block[i];
    }

    const UInt &word(int i) const {
        assert(0 <= i && i < word_count());
        return block[i];
    }

    UInt *data() { return block.data(); }

    const UInt *data() const { return block.data(); }

    T &clear_unused_bits() {
        if ((n & BLOCK_MASK) && !block.empty()) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &inplace_combine_top(const T &rhs) {
        if (this == &rhs) {
            T copy = rhs;
            return inplace_combine_top(copy);
        }
        int old_n = n;
        n += rhs.n;
        block.resize((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        int offset = old_n & BLOCK_MASK;
        int word_offset = old_n >> BLOCK_SIZE_LOG;
        if (offset == 0) {
            std::copy(rhs.block.begin(), rhs.block.end(), block.begin() + word_offset);
        } else {
            for (int i = 0; i < rhs.word_count(); ++i) {
                block[word_offset + i] |= rhs.block[i] << offset;
                if (word_offset + i + 1 < word_count()) {
                    block[word_offset + i + 1] = rhs.block[i] >> (BLOCK_SIZE - offset);
                }
            }
        }
        return *this;
    }

    T combine_top(const T &rhs) const { return T(*this).inplace_combine_top(rhs); }

    T &inplace_combine_bottom(const T &rhs) {
        T result = rhs;
        result.inplace_combine_top(*this);
        *this = std::move(result);
        return *this;
    }

    T combine_bottom(const T &rhs) const { return T(*this).inplace_combine_bottom(rhs); }

    void set(int i, bool x = true) {
        assert(0 <= i && i < n);
        if (x) block[i >> BLOCK_SIZE_LOG] |= ONE << (i & BLOCK_MASK);
        else block[i >> BLOCK_SIZE_LOG] &= ~(ONE << (i & BLOCK_MASK));
    }

    void reset(int i) { set(i, false); }

    T &set_all(bool x = true) {
        std::fill(block.begin(), block.end(), x ? ~UInt(0) : UInt(0));
        if (x && (n & BLOCK_MASK)) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &reset_all() { return set_all(false); }

    void set(const std::string &s) {
        assert((int)s.size() == n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            int r = n - (i << BLOCK_SIZE_LOG), l = std::max(0, r - BLOCK_SIZE);
            block[i] = 0;
            for (int j = l; j < r; j++) block[i] = (block[i] << 1) | (s[j] - '0');
        }
    }

    void set_reversed(const std::string &s) {
        assert((int)s.size() == n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            int l = i << BLOCK_SIZE_LOG, r = std::min(n, l + BLOCK_SIZE);
            block[i] = 0;
            for (int j = r - 1; j >= l; --j) block[i] = (block[i] << 1) | (s[j] - '0');
        }
    }

    struct BitReference {
        std::vector<UInt> &block;
        int idx;

      public:
        BitReference(std::vector<UInt> &block_, int idx_) : block(block_), idx(idx_) {}

        operator bool() const { return (block[idx >> BLOCK_SIZE_LOG] >> (idx & BLOCK_MASK)) & 1; }

        template <InputStream IStream> friend IStream &operator>>(IStream &is, BitReference a) {
            bool c;
            is >> c;
            a = c;
            return is;
        }

        BitReference &operator=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            else block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            else block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator&=(bool x) {
            if (!x) block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator&=(const BitReference &other) {
            if (!other) block[idx >> BLOCK_SIZE_LOG] &= ~(ONE << (idx & BLOCK_MASK));
            return *this;
        }

        BitReference &operator|=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator|=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] |= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator^=(bool x) {
            if (x) block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator^=(const BitReference &other) {
            if (other) block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &flip() {
            block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        BitReference &operator~() {
            block[idx >> BLOCK_SIZE_LOG] ^= ONE << (idx & BLOCK_MASK);
            return *this;
        }

        bool val() const { return (block[idx >> BLOCK_SIZE_LOG] >> (idx & BLOCK_MASK)) & 1; }
    };

    BitReference operator[](int i) {
        assert(0 <= i && i < n);
        return BitReference(block, i);
    }

    bool operator[](int i) const {
        assert(0 <= i && i < n);
        return (block[i >> BLOCK_SIZE_LOG] >> (i & BLOCK_MASK)) & 1;
    }

    bool is_pinned(int i) const {
        assert(0 <= i && i < n);
        return (block[i >> BLOCK_SIZE_LOG] >> (i & BLOCK_MASK)) & 1;
    }

    T &operator=(const std::string &s) {
        set(s);
        return *this;
    }

    T &flip() {
        for (UInt &x : block) x = ~x;
        if (n & BLOCK_MASK) block.back() &= (ONE << (n & BLOCK_MASK)) - 1;
        return *this;
    }

    T &flip(int i) {
        assert(0 <= i && i < n);
        block[i >> BLOCK_SIZE_LOG] ^= ONE << (i & BLOCK_MASK);
        return *this;
    }

    int ctz() const { return find_next(0); }

    int clz() const {
        int last = find_prev(n - 1);
        return last == -1 ? n : n - 1 - last;
    }

    int find_next(int i) const {
        if (i < 0) i = 0;
        if (i >= n) return n;
        int j = i >> BLOCK_SIZE_LOG;
        UInt bits = block[j] & (~UInt(0) << (i & BLOCK_MASK));
        while (true) {
            if (bits) return std::min(n, j * BLOCK_SIZE + (int)std::countr_zero(bits));
            if (++j == word_count()) return n;
            bits = block[j];
        }
    }

    int find_next_zero(int i) const {
        if (i < 0) i = 0;
        if (i >= n) return n;
        int j = i >> BLOCK_SIZE_LOG;
        UInt bits = ~block[j] & (~UInt(0) << (i & BLOCK_MASK));
        while (true) {
            if (bits) return std::min(n, j * BLOCK_SIZE + (int)std::countr_zero(bits));
            if (++j == word_count()) return n;
            bits = ~block[j];
        }
    }

    int find_prev(int i) const {
        if (i >= n) i = n - 1;
        if (i < 0) return -1;
        int j = i >> BLOCK_SIZE_LOG;
        int offset = i & BLOCK_MASK;
        UInt bits = block[j] & (~UInt(0) >> (BLOCK_MASK - offset));
        while (true) {
            if (bits) return j * BLOCK_SIZE + (BLOCK_MASK - std::countl_zero(bits));
            if (j-- == 0) return -1;
            bits = block[j];
        }
    }

    int popcount() const {
        int res = 0;
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            res += kk2::popcount(block[i]);
        }
        return res;
    }

    T &operator~() { return flip(); }

    T &operator&=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] &= other.block[i];
        }
        return *this;
    }

    T &operator|=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] |= other.block[i];
        }
        return *this;
    }

    T &operator^=(const T &other) {
        assert(n == other.n);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            block[i] ^= other.block[i];
        }
        return *this;
    }

    T &inplace_or_repeated(const T &pattern) {
        assert(pattern.n > 0 && (pattern.n & BLOCK_MASK) == 0);
        int pattern_words = pattern.word_count();
        for (int begin = 0; begin < word_count(); begin += pattern_words) {
            int size = std::min(pattern_words, word_count() - begin);
            for (int i = 0; i < size; ++i) block[begin + i] |= pattern.block[i];
        }
        return clear_unused_bits();
    }

    friend T operator&(const T &lhs, const T &rhs) { return T(lhs) &= rhs; }

    friend T operator|(const T &lhs, const T &rhs) { return T(lhs) |= rhs; }

    friend T operator^(const T &lhs, const T &rhs) { return T(lhs) ^= rhs; }

    friend bool operator==(const T &lhs, const T &rhs) {
        if (lhs.n != rhs.n) return false;
        for (int i = 0; i < (lhs.n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            if (lhs.block[i] != rhs.block[i]) return false;
        }
        return true;
    }

    friend bool operator!=(const T &lhs, const T &rhs) { return !(lhs == rhs); }

    operator bool() const {
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            if (block[i]) return true;
        }
        return false;
    }

    std::string to_string(UInt x) const { return std::bitset<BLOCK_SIZE>(x).to_string(); }

    std::string to_string() const {
        std::vector<std::string> tmp((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            tmp[i] = to_string(block[i]);
        }
        if (n & BLOCK_MASK) {
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
            tmp.back().resize(n & BLOCK_MASK);
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
        }
        std::string res;
        for (int i = (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i--;) { res += tmp[i]; }
        return res;
    }

    std::string to_reversed_string() const {
        std::vector<std::string> tmp((n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG);
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            tmp[i] = to_string(block[i]);
        }
        if (n & BLOCK_MASK) {
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
            tmp.back().resize(n & BLOCK_MASK);
            std::reverse(std::begin(tmp.back()), std::end(tmp.back()));
        }
        std::string res;
        for (int i = 0; i < (n + BLOCK_SIZE - 1) >> BLOCK_SIZE_LOG; i++) {
            std::reverse(std::begin(tmp[i]), std::end(tmp[i]));
            res += tmp[i];
        }
        return res;
    }

    template <OutputStream OStream> friend OStream &operator<<(OStream &os, const T &bs) {
        return os << bs.to_string();
    }

    template <InputStream IStream> friend IStream &operator>>(IStream &is, T &bs) {
        std::string s;
        is >> s;
        bs.set_reversed(s);
        return is;
    }
};

struct DynamicBitSet::RankSelect {
  private:
    DynamicBitSet _bits;
    std::vector<int> _prefix;

    UInt selected_word(int word) const { return _bits.word(word); }

  public:
    RankSelect() : _prefix(1) {}

    explicit RankSelect(DynamicBitSet bits, bool value = true)
        : _bits(std::move(bits)),
          _prefix(_bits.word_count() + 1) {
        for (int word = 0; word < _bits.word_count(); ++word) {
            UInt selected = value ? _bits.word(word) : ~_bits.word(word);
            if (word + 1 == _bits.word_count() && (_bits.size() & BLOCK_MASK)) {
                selected &= (ONE << (_bits.size() & BLOCK_MASK)) - 1;
            }
            _bits.word(word) = selected;
            _prefix[word + 1] = _prefix[word] + std::popcount(selected);
        }
    }

    int bit_size() const { return _bits.size(); }

    bool contains(int index) const {
        assert(0 <= index && index < bit_size());
        return _bits[index];
    }

    int rank(int end) const {
        assert(0 <= end && end <= bit_size());
        int word = end >> BLOCK_SIZE_LOG;
        int result = _prefix[word];
        if (end & BLOCK_MASK) {
            UInt mask = (ONE << (end & BLOCK_MASK)) - 1;
            result += std::popcount(selected_word(word) & mask);
        }
        return result;
    }

    int select(int rank) const {
        assert(0 <= rank && rank < size());
        int word =
            (int)(std::upper_bound(_prefix.begin(), _prefix.end(), rank) - _prefix.begin()) - 1;
        UInt selected = selected_word(word);
        int local_rank = rank - _prefix[word];
        while (local_rank--) selected &= selected - 1;
        return word * BLOCK_SIZE + std::countr_zero(selected);
    }

    int find_next(int index) const { return _bits.find_next(index); }

    struct MonotoneCursor {
      private:
        const RankSelect *_index;
        int _last_rank = -1;
        int _word = 0;

      public:
        explicit MonotoneCursor(const RankSelect &index) : _index(&index) {}

        int operator[](int rank) {
            assert(0 <= rank && _last_rank <= rank && rank < _index->size());
            _last_rank = rank;
            while (_index->_prefix[_word + 1] <= rank) ++_word;
            UInt selected = _index->selected_word(_word);
            int local_rank = rank - _index->_prefix[_word];
            while (local_rank--) selected &= selected - 1;
            return _word * BLOCK_SIZE + std::countr_zero(selected);
        }
    };

    int size() const { return _prefix.back(); }

    int operator[](int rank) const { return select(rank); }

    MonotoneCursor monotone_cursor() const & { return MonotoneCursor(*this); }
    MonotoneCursor monotone_cursor() const && = delete;

    struct RangeAccessor {
        const RankSelect *index;

        using MonotoneCursor = RankSelect::MonotoneCursor;

        int size() const { return index->size(); }
        int operator[](int rank) const { return (*index)[rank]; }
        MonotoneCursor monotone_cursor() const { return index->monotone_cursor(); }
    };

    using Range = MonotoneRankRange<RangeAccessor>;

    Range range() const & { return Range{RangeAccessor{this}}; }
    Range range() const && = delete;

    auto begin() const { return range().begin(); }
    auto end() const { return range().end(); }
    auto stride(int start, int step) const & { return range().stride(start, step); }
    auto stride(int start, int step, int end) const & { return range().stride(start, step, end); }
    auto stride(int, int) const && = delete;
    auto stride(int, int, int) const && = delete;
    std::vector<int> to_vec() const { return range().to_vec(); }
};

} // namespace kk2

#endif // KK2_DATA_STRUCTURE_MY_BITSET_HPP
#ifndef KK2_MATH_SQRT_FLOOR_HPP
#define KK2_MATH_SQRT_FLOOR_HPP 1


#ifndef KK2_MATH_FRAC_FLOOR_HPP
#define KK2_MATH_FRAC_FLOOR_HPP 1


namespace kk2 {

// floor(x) = ceil(x) - 1 (for all x not in Z) ...(1)
// floor(x) = -ceil(-x)   (for all x)          ...(2)

// return floor(a / b)
template <typename T, typename U> constexpr T fracfloor(T a, U b) {
    assert(b != 0);
    if (a % b == 0) return a / b;
    if (a >= 0) return a / b;

    // floor(x) = -ceil(-x)      by (2)
    //          = -floor(-x) - 1 by (1)
    return -((-a) / b) - 1;
}

// return ceil(a / b)
template <typename T, typename U> constexpr T fracceil(T a, U b) {
    assert(b != 0);
    if (a % b == 0) return a / b;
    if (a >= 0) return a / b + 1;

    // ceil(x) = -floor(-x)      by (2)
    return -((-a) / b);
}

} // namespace kk2

#endif // KK2_MATH_FRAC_FLOOR_HPP

namespace kk2 {

template <typename T> T sqrt_floor(T n) {
    assert(n >= 0);
    if (n == T(0)) return 0;
    T x = std::sqrt(n);
    if (x == T(0)) ++x;
    while (x > kk2::fracfloor(n, x)) --x;
    while (x + 1 <= kk2::fracfloor(n, x + 1)) ++x;
    return x;
}

template <typename T> T sqrt_ceil(T n) {
    assert(n >= 0);
    if (n <= T(1)) return n;
    T x = std::sqrt(n);
    if (x == T(0)) ++x;
    while (x < kk2::fracceil(n, x)) ++x;
    while (x - 1 >= kk2::fracceil(n, x - 1)) --x;
    return x;
}

} // namespace kk2

#endif // KK2_MATH_SQRT_FLOOR_HPP

namespace kk2 {

struct IsPrimeTable {
  private:
    static constexpr std::array<int, 9> _small_primes{2, 3, 5, 7, 11, 13, 17, 19, 23};
    static inline int _n = 1;
    static inline int _wheel = 1;
    static inline int _residue_count = 0;
    static inline int _wheel_prime_count = 0;
    static inline std::vector<int> _coprimes{};
    static inline std::vector<int> _residue_index{};
    static inline DynamicBitSet::RankSelect _prime_candidates{};

    template <int Wheel, int ResidueCount>
    static void set_offsets(int p,
                            const std::vector<int> &coprimes,
                            const std::vector<int> &residue_index,
                            int *offsets) {
        for (int r = 0; r < ResidueCount; ++r) {
            long long x = 1LL * p * coprimes[r];
            offsets[r] = x / Wheel * ResidueCount + residue_index[x % Wheel];
        }
    }

    static int candidate_value(int index) {
        return index / _residue_count * _wheel + _coprimes[index % _residue_count];
    }

    static int candidate_end(int n) {
        return n / _wheel * _residue_count
               + (int)(std::upper_bound(_coprimes.begin(), _coprimes.end(), n % _wheel)
                       - _coprimes.begin());
    }

    static int candidate_rank(int end) { return _prime_candidates.rank(end); }

  public:
    IsPrimeTable() = delete;

    static void set_upper(int m) {
        if (m <= _n && _residue_count != 0) return;
        int next_n = std::max({m, 2 * _n, 60});

        int sqrt_n = sqrt_floor(next_n);
        int wheel = 1;
        int wheel_prime_count = 0;
        // A moderately larger wheel pays off by reducing the number of sieve candidates.
        while (wheel_prime_count < (int)_small_primes.size()
               && 4LL * wheel * _small_primes[wheel_prime_count] <= 7LL * sqrt_n) {
            wheel *= _small_primes[wheel_prime_count];
            ++wheel_prime_count;
        }

        std::vector<bool> iscoprime(wheel, true);
        for (int i = 0; i < wheel_prime_count; ++i) {
            for (int j = _small_primes[i]; j < wheel; j += _small_primes[i]) iscoprime[j] = false;
        }
        std::vector<int> residue_index(wheel, -1);
        int residue_count = 0;
        for (int i = 1; i < wheel; ++i) {
            if (iscoprime[i]) residue_index[i] = residue_count++;
        }
        std::vector<int> coprimes(residue_count);
        for (int i = 1; i < wheel; ++i) {
            if (residue_index[i] != -1) coprimes[residue_index[i]] = i;
        }

        auto val = [&](int i) {
            return i / residue_count * wheel + coprimes[i % residue_count];
        };

        int candidate_count = (next_n + wheel - 1) / wheel * residue_count;
        while (candidate_count > 1 && val(candidate_count - 1) > next_n) --candidate_count;
        DynamicBitSet composite(candidate_count);
        std::uint64_t *composite_data = composite.data();
        auto set_composite = [&](long long i) {
            composite_data[i >> 6] |= std::uint64_t(1) << (i & 63);
        };
        {
            std::vector<bool> base_isprime(sqrt_n + 1, true);
            base_isprime[0] = base_isprime[1] = false;
            std::vector<int> base_primes;
            for (int p = 2; p <= sqrt_n; ++p) {
                if (!base_isprime[p]) continue;
                if (p > _small_primes[wheel_prime_count - 1]) base_primes.push_back(p);
                if (1LL * p * p <= sqrt_n) {
                    for (int q = p * p; q <= sqrt_n; q += p) base_isprime[q] = false;
                }
            }
            constexpr int dense_limit = 112;
            constexpr int dense_mask_bytes = 1 << 20;
            int dense_count = 0;
            if (wheel == 30030) {
                std::vector<int> offsets(residue_count);
                while (dense_count < (int)base_primes.size()
                       && base_primes[dense_count] <= dense_limit) {
                    ++dense_count;
                }
                for (int group_begin = 0; group_begin < dense_count;) {
                    int product = 1;
                    int group_end = group_begin;
                    while (group_end < dense_count
                           && 1LL * product * base_primes[group_end] * residue_count
                                  <= 8LL * dense_mask_bytes) {
                        product *= base_primes[group_end++];
                    }
                    if (group_end == group_begin) product = base_primes[group_end++];
                    int mask_word_count = product * residue_count / 64;
                    DynamicBitSet mask(mask_word_count * 64);
                    for (int k = group_begin; k < group_end; ++k) {
                        int p = base_primes[k];
                        set_offsets<30030, 5760>(p, coprimes, residue_index, offsets.data());
                        int period = p * residue_count;
                        for (int base = 0; base < product * residue_count; base += period) {
                            for (int offset : offsets) {
                                int index = base + offset;
                                mask.word(index >> 6) |= std::uint64_t(1) << (index & 63);
                            }
                        }
                    }
                    composite.inplace_or_repeated(mask);
                    for (int k = group_begin; k < group_end; ++k) {
                        int p = base_primes[k];
                        int index = p / wheel * residue_count + residue_index[p % wheel];
                        composite.word(index >> 6) &= ~(std::uint64_t(1) << (index & 63));
                    }
                    group_begin = group_end;
                }
            }
            int sparse_end = base_primes.size();
            while (sparse_end > dense_count && 1LL * base_primes[sparse_end - 1] * wheel > next_n) {
                int p = base_primes[--sparse_end];
                int begin = residue_index[p % wheel];
                int end = std::upper_bound(coprimes.begin(), coprimes.end(), next_n / p)
                          - coprimes.begin();
                for (int r = begin; r < end; ++r) {
                    long long x = 1LL * p * coprimes[r];
                    set_composite(x / wheel * residue_count + residue_index[x % wheel]);
                }
            }
            struct SieveState {
                int residue;
                long long base, step;
            };
            int batch_size = wheel == 30030 ? 384 : 128;
            constexpr int segment_size = 1 << 21;
            // Keep one candidate segment hot while marking it with a batch of primes.
            std::vector<int> all_offsets((std::size_t)batch_size * residue_count);
            std::vector<SieveState> states(batch_size);
            for (int batch_begin = dense_count; batch_begin < sparse_end;
                 batch_begin += batch_size) {
                int size = std::min(batch_size, sparse_end - batch_begin);
                for (int k = 0; k < size; ++k) {
                    int p = base_primes[batch_begin + k];
                    int *offsets = all_offsets.data() + (std::size_t)k * residue_count;
                    switch (wheel) {
                        case 6:
                            set_offsets<6, 2>(p, coprimes, residue_index, offsets);
                            break;
                        case 30:
                            set_offsets<30, 8>(p, coprimes, residue_index, offsets);
                            break;
                        case 210:
                            set_offsets<210, 48>(p, coprimes, residue_index, offsets);
                            break;
                        case 2310:
                            set_offsets<2310, 480>(p, coprimes, residue_index, offsets);
                            break;
                        case 30030:
                            set_offsets<30030, 5760>(p, coprimes, residue_index, offsets);
                            break;
                        default:
                            for (int r = 0; r < residue_count; ++r) {
                                long long x = 1LL * p * coprimes[r];
                                offsets[r] = x / wheel * residue_count + residue_index[x % wheel];
                            }
                    }
                    int i = p / wheel * residue_count + residue_index[p % wheel];
                    int block = i / residue_count;
                    states[k] = {i % residue_count,
                                 1LL * p * block * residue_count,
                                 1LL * p * residue_count};
                }
                for (int segment_begin = 0; segment_begin < candidate_count;
                     segment_begin += segment_size) {
                    int segment_end = std::min(candidate_count, segment_begin + segment_size);
                    for (int k = 0; k < size; ++k) {
                        SieveState &state = states[k];
                        int *offsets = all_offsets.data() + (std::size_t)k * residue_count;
                        while (state.base + offsets[residue_count - 1] < segment_end) {
                            int r = state.residue;
                            for (; r + 4 <= residue_count; r += 4) {
                                set_composite(state.base + offsets[r]);
                                set_composite(state.base + offsets[r + 1]);
                                set_composite(state.base + offsets[r + 2]);
                                set_composite(state.base + offsets[r + 3]);
                            }
                            for (; r < residue_count; ++r) {
                                set_composite(state.base + offsets[r]);
                            }
                            state.residue = 0;
                            state.base += state.step;
                        }
                        while (state.residue < residue_count
                               && state.base + offsets[state.residue] < segment_end) {
                            set_composite(state.base + offsets[state.residue]);
                            ++state.residue;
                        }
                        if (state.residue == residue_count) {
                            state.residue = 0;
                            state.base += state.step;
                        }
                    }
                }
            }
        }

        composite.set(0);
        _n = next_n;
        _wheel = wheel;
        _residue_count = residue_count;
        _wheel_prime_count = wheel_prime_count;
        _coprimes = std::move(coprimes);
        _residue_index = std::move(residue_index);
        _prime_candidates = DynamicBitSet::RankSelect(std::move(composite), false);
    }

  private:
    struct PrimeAccessor {
        struct MonotoneCursor {
          private:
            int _size, _prefix_count;
            int _last_rank = -1;
            DynamicBitSet::RankSelect::MonotoneCursor _cursor;

          public:
            MonotoneCursor(int size, int prefix_count)
                : _size(size),
                  _prefix_count(prefix_count),
                  _cursor(_prime_candidates.monotone_cursor()) {}

            int operator[](int rank) {
                assert(0 <= rank && _last_rank <= rank && rank < _size);
                _last_rank = rank;
                if (rank < _prefix_count) {
                    assert(rank < (int)_small_primes.size());
                    return _small_primes[rank];
                }

                return candidate_value(_cursor[rank - _prefix_count]);
            }
        };

        int _size, _prefix_count;

        int size() const { return _size; }

        int operator[](int rank) const {
            assert(0 <= rank && rank < _size);
            if (rank < _prefix_count) return _small_primes[rank];
            return candidate_value(_prime_candidates[rank - _prefix_count]);
        }

        MonotoneCursor monotone_cursor() const { return MonotoneCursor(_size, _prefix_count); }
    };

  public:
    static auto primes(int n) {
        using Range = MonotoneRankRange<PrimeAccessor>;
        if (n >= _n) set_upper(n);
        if (_residue_count == 0)
            return Range{
                PrimeAccessor{0, 0}
            };
        int prefix_count =
            (int)(std::upper_bound(
                      _small_primes.begin(), _small_primes.begin() + _wheel_prime_count, n)
                  - _small_primes.begin());
        int end = candidate_end(n);
        return Range{
            PrimeAccessor{prefix_count + candidate_rank(end), prefix_count}
        };
    }

    static auto primes() { return primes(_n); }

    static bool isprime(int n) {
        assert(n > 0);
        if (n >= _n) set_upper(n);
        for (int i = 0; i < _wheel_prime_count; ++i) {
            if (n == _small_primes[i]) return true;
        }
        int residue = _residue_index[n % _wheel];
        if (residue == -1) return false;
        int index = n / _wheel * _residue_count + residue;
        return _prime_candidates.contains(index);
    }
};

} // namespace kk2

#endif // KK2_MATH_ISPRIME_TABLE_HPP

namespace kk2 {

struct PrimeTable {
    PrimeTable() = delete;

    static void set_upper(int m) { IsPrimeTable::set_upper(m); }

    static auto primes() { return IsPrimeTable::primes(); }

    static auto primes(int n) { return IsPrimeTable::primes(n); }
};

} // namespace kk2

#endif // KK2_MATH_PRIME_TABLE_HPP
#ifndef KK2_TEMPLATE_TEMPLATE_HPP
#define KK2_TEMPLATE_TEMPLATE_HPP 1


#ifndef KK2_TEMPLATE_CONSTANT_HPP
#define KK2_TEMPLATE_CONSTANT_HPP 1

#ifndef KK2_TEMPLATE_TYPE_ALIAS_HPP
#define KK2_TEMPLATE_TYPE_ALIAS_HPP 1


using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;

using pi = std::pair<int, int>;
using pl = std::pair<i64, i64>;
using pil = std::pair<int, i64>;
using pli = std::pair<i64, int>;

template <class T> using vc = std::vector<T>;
template <class T> using vvc = std::vector<vc<T>>;
template <class T> using vvvc = std::vector<vvc<T>>;
template <class T> using vvvvc = std::vector<vvvc<T>>;

template <class T> using pq = std::priority_queue<T>;
template <class T> using pqi = std::priority_queue<T, std::vector<T>, std::greater<T>>;

#endif // KK2_TEMPLATE_TYPE_ALIAS_HPP

template <class T> constexpr T infty = 0;
template <> constexpr int infty<int> = (1 << 30) - 123;
template <> constexpr i64 infty<i64> = (1ll << 62) - (1ll << 31);
template <> constexpr i128 infty<i128> = (i128(1) << 126) - (i128(1) << 63);
template <> constexpr u32 infty<u32> = infty<int>;
template <> constexpr u64 infty<u64> = infty<i64>;
template <> constexpr u128 infty<u128> = infty<i128>;
template <> constexpr double infty<double> = infty<i64>;
template <> constexpr long double infty<long double> = infty<i64>;

constexpr int mod = 998244353;
constexpr int modu = 1e9 + 7;
constexpr long double PI = 3.14159265358979323846;

#endif // KK2_TEMPLATE_CONSTANT_HPP
#ifndef KK2_TEMPLATE_FASTIO_HPP
#define KK2_TEMPLATE_FASTIO_HPP 1


#ifndef KK2_TYPE_TRAITS_INTERGRAL_HPP
#define KK2_TYPE_TRAITS_INTERGRAL_HPP 1


namespace kk2 {

#ifndef _MSC_VER

template <typename T>
using is_signed_int128 = typename std::conditional<std::is_same<T, __int128_t>::value
                                                       or std::is_same<T, __int128>::value,
                                                   std::true_type,
                                                   std::false_type>::type;

template <typename T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value
                                  or std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value or is_signed_int128<T>::value
                                  or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_signed = typename std::conditional<std::is_signed<T>::value or is_signed_int128<T>::value,
                                            std::true_type,
                                            std::false_type>::type;

template <typename T>
using is_unsigned =
    typename std::conditional<std::is_unsigned<T>::value or is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value, __uint128_t, unsigned __int128>;

template <typename T>
using to_unsigned =
    typename std::conditional<is_signed_int128<T>::value,
                              make_unsigned_int128<T>,
                              typename std::conditional<std::is_signed<T>::value,
                                                        std::make_unsigned<T>,
                                                        std::common_type<T>>::type>::type;

#else

template <typename T> using is_integral = std::enable_if_t<std::is_integral<T>::value>;
template <typename T> using is_signed = std::enable_if_t<std::is_signed<T>::value>;
template <typename T> using is_unsigned = std::enable_if_t<std::is_unsigned<T>::value>;
template <typename T> using to_unsigned = std::make_unsigned<T>;

#endif // _MSC_VER

template <typename T> using is_integral_t = std::enable_if_t<is_integral<T>::value>;
template <typename T> using is_signed_t = std::enable_if_t<is_signed<T>::value>;
template <typename T> using is_unsigned_t = std::enable_if_t<is_unsigned<T>::value>;

template <class T>
concept Integral = is_integral<std::remove_cv_t<T>>::value;

template <class T>
concept SignedIntegral = is_signed<std::remove_cv_t<T>>::value;

template <class T>
concept UnsignedIntegral = is_unsigned<std::remove_cv_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_INTERGRAL_HPP
#ifndef KK2_TYPE_TRAITS_IO_HPP
#define KK2_TYPE_TRAITS_IO_HPP 1


namespace kk2 {

namespace type_traits {

struct istream_tag {};
struct ostream_tag {};

} // namespace type_traits

template <typename T>
using is_standard_istream = typename std::conditional<std::is_same<T, std::istream>::value
                                                          || std::is_same<T, std::ifstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T>
using is_standard_ostream = typename std::conditional<std::is_same<T, std::ostream>::value
                                                          || std::is_same<T, std::ofstream>::value,
                                                      std::true_type,
                                                      std::false_type>::type;
template <typename T> using is_user_defined_istream = std::is_base_of<type_traits::istream_tag, T>;
template <typename T> using is_user_defined_ostream = std::is_base_of<type_traits::ostream_tag, T>;

template <typename T>
using is_istream =
    typename std::conditional<is_standard_istream<T>::value || is_user_defined_istream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T>
using is_ostream =
    typename std::conditional<is_standard_ostream<T>::value || is_user_defined_ostream<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <typename T> using is_istream_t = std::enable_if_t<is_istream<T>::value>;
template <typename T> using is_ostream_t = std::enable_if_t<is_ostream<T>::value>;

template <class T>
concept StandardInputStream = is_standard_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept StandardOutputStream = is_standard_ostream<std::remove_cvref_t<T>>::value;

template <class T>
concept InputStream = is_istream<std::remove_cvref_t<T>>::value;

template <class T>
concept OutputStream = is_ostream<std::remove_cvref_t<T>>::value;

} // namespace kk2

#endif // KK2_TYPE_TRAITS_IO_HPP

namespace kk2 {

namespace fastio {

struct Scanner : type_traits::istream_tag {
  private:
    static constexpr size_t INPUT_BUF = 1 << 17;
    size_t pos = 0, end = 0;
    bool is_eof = false;
    static char buf[INPUT_BUF];
    FILE *fp;

  public:
    Scanner() : fp(stdin) {}

    Scanner(const char *file) : fp(fopen(file, "r")) {}

    ~Scanner() {
        if (fp != stdin) fclose(fp);
    }

    char now() {
        if (is_eof) return '\0';
        if (pos == end) {
            end = fread(buf, 1, INPUT_BUF, fp);
            if (end != INPUT_BUF) buf[end] = '\0';
            if (end == 0) is_eof = true;
            pos = 0;
        }
        return buf[pos];
    }

    void skip_space() {
        while (isspace(now())) ++pos;
    }

    template <UnsignedIntegral T> T next_unsigned_integral() {
        skip_space();
        T res{};
        while (isdigit(now())) {
            res = res * 10 + (now() - '0');
            ++pos;
        }
        return res;
    }

    template <SignedIntegral T> T next_signed_integral() {
        skip_space();
        if (now() == '-') {
            ++pos;
            return T(-next_unsigned_integral<typename to_unsigned<T>::type>());
        } else return (T)next_unsigned_integral<typename to_unsigned<T>::type>();
    }

    char next_char() {
        skip_space();
        auto res = now();
        ++pos;
        return res;
    }

    std::string next_string() {
        skip_space();
        std::string res;
        while (true) {
            char c = now();
            if (isspace(c) or c == '\0') break;
            res.push_back(now());
            ++pos;
        }
        return res;
    }

    template <UnsignedIntegral T> Scanner &operator>>(T &x) {
        x = next_unsigned_integral<T>();
        return *this;
    }

    template <SignedIntegral T> Scanner &operator>>(T &x) {
        x = next_signed_integral<T>();
        return *this;
    }

    Scanner &operator>>(char &x) {
        x = next_char();
        return *this;
    }

    Scanner &operator>>(std::string &x) {
        x = next_string();
        return *this;
    }
};

struct endl_struct_t {};

struct Printer : type_traits::ostream_tag {
  private:
    static char helper[10000][5];
    static char leading_zero[10000][5];
    constexpr static size_t OUTPUT_BUF = 1 << 17;
    static char buf[OUTPUT_BUF];
    size_t pos = 0;
    FILE *fp;

    template <class T> static constexpr void div_mod(T &a, T &b, T mod) {
        a = b / mod;
        b -= a * mod;
    }

    static void init() {
        buf[0] = '\0';
        for (size_t i = 0; i < 10000; ++i) {
            leading_zero[i][0] = i / 1000 + '0';
            leading_zero[i][1] = i / 100 % 10 + '0';
            leading_zero[i][2] = i / 10 % 10 + '0';
            leading_zero[i][3] = i % 10 + '0';
            leading_zero[i][4] = '\0';

            size_t j = 0;
            if (i >= 1000) helper[i][j++] = i / 1000 + '0';
            if (i >= 100) helper[i][j++] = i / 100 % 10 + '0';
            if (i >= 10) helper[i][j++] = i / 10 % 10 + '0';
            helper[i][j++] = i % 10 + '0';
            helper[i][j] = '\0';
        }
    }

  public:
    Printer() : fp(stdout) { init(); }

    Printer(const char *file) : fp(fopen(file, "w")) { init(); }

    ~Printer() {
        write();
        if (fp != stdout) fclose(fp);
    }

    void write() {
        fwrite(buf, 1, pos, fp);
        pos = 0;
    }

    void flush() {
        write();
        fflush(fp);
    }

    void put_char(char c) {
        if (pos == OUTPUT_BUF) write();
        buf[pos++] = c;
    }

    void put_cstr(const char *s) {
        while (*s) put_char(*(s++));
    }

    void put_u32(uint32_t x) {
        uint32_t y;
        if (x >= 100000000) { // 10^8
            div_mod<uint32_t>(y, x, 100000000);
            put_cstr(helper[y]);
            div_mod<uint32_t>(y, x, 10000);
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= 10000) { // 10^4
            div_mod<uint32_t>(y, x, 10000);
            put_cstr(helper[y]);
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i32(int32_t x) {
        if (x < 0) {
            put_char('-');
            put_u32(-x);
        } else put_u32(x);
    }

    void put_u64(uint64_t x) {
        uint64_t y;
        if (x >= 1000000000000ull) { // 10^12
            div_mod<uint64_t>(y, x, 1000000000000ull);
            put_u32(y);
            div_mod<uint64_t>(y, x, 100000000ull);
            put_cstr(leading_zero[y]);
            div_mod<uint64_t>(y, x, 10000ull);
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= 10000ull) { // 10^4
            div_mod<uint64_t>(y, x, 10000ull);
            put_u32(y);
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i64(int64_t x) {
        if (x < 0) {
            put_char('-');
            put_u64(-x);
        } else put_u64(x);
    }

    void put_u128(__uint128_t x) {
        constexpr static __uint128_t pow10_10 = 10000000000ull;
        constexpr static __uint128_t pow10_20 = pow10_10 * pow10_10;

        __uint128_t y;
        if (x >= pow10_20) { // 10^20
            div_mod<__uint128_t>(y, x, pow10_20);
            put_u64(uint64_t(y));
            div_mod<__uint128_t>(y, x, __uint128_t(10000000000000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(1000000000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(100000000ull));
            put_cstr(leading_zero[y]);
            div_mod<__uint128_t>(y, x, __uint128_t(10000ull));
            put_cstr(leading_zero[y]);
            put_cstr(leading_zero[x]);
        } else if (x >= __uint128_t(10000)) { // 10^4
            div_mod<__uint128_t>(y, x, __uint128_t(10000));
            put_u64(uint64_t(y));
            put_cstr(leading_zero[x]);
        } else put_cstr(helper[x]);
    }

    void put_i128(__int128_t x) {
        if (x < 0) {
            put_char('-');
            put_u128(-x);
        } else put_u128(x);
    }

    template <UnsignedIntegral T> Printer &operator<<(T x) {
        if constexpr (sizeof(T) <= 4) put_u32(x);
        else if constexpr (sizeof(T) <= 8) put_u64(x);
        else put_u128(x);
        return *this;
    }

    template <SignedIntegral T> Printer &operator<<(T x) {
        if constexpr (sizeof(T) <= 4) put_i32(x);
        else if constexpr (sizeof(T) <= 8) put_i64(x);
        else put_i128(x);
        return *this;
    }

    Printer &operator<<(char x) {
        put_char(x);
        return *this;
    }

    Printer &operator<<(const std::string &x) {
        for (char c : x) put_char(c);
        return *this;
    }

    Printer &operator<<(const char *x) {
        put_cstr(x);
        return *this;
    }

    // std::cout << std::endl; は関数ポインタを渡しているらしい
    Printer &operator<<(endl_struct_t) {
        put_char('\n');
        flush();
        return *this;
    }
};

char Scanner::buf[Scanner::INPUT_BUF];
char Printer::buf[Printer::OUTPUT_BUF];
char Printer::helper[10000][5];
char Printer::leading_zero[10000][5];

} // namespace fastio

#if defined(INTERACTIVE) || defined(USE_STDIO)
auto &kin = std::cin;
auto &kout = std::cout;
auto (*kendl)(std::ostream &) = std::endl<char, std::char_traits<char>>;
#else
fastio::Scanner kin;
fastio::Printer kout;
fastio::endl_struct_t kendl;
#endif

} // namespace kk2

#endif // KK2_TEMPLATE_FASTIO_HPP
#ifndef KK2_TEMPLATE_IO_UTIL_HPP
#define KK2_TEMPLATE_IO_UTIL_HPP 1



// なんかoj verifyはプロトタイプ宣言が落ちる

namespace impl {

struct read {
    template <class IStream, class T> inline static void all_read(IStream &is, T &x) { is >> x; }

    template <class IStream, class T, class U>
    inline static void all_read(IStream &is, std::pair<T, U> &p) {
        all_read(is, p.first);
        all_read(is, p.second);
    }

    template <class IStream, class T> inline static void all_read(IStream &is, std::vector<T> &v) {
        for (T &x : v) all_read(is, x);
    }

    template <class IStream, class T, size_t F>
    inline static void all_read(IStream &is, std::array<T, F> &a) {
        for (T &x : a) all_read(is, x);
    }
};

struct write {
    template <class OStream, class T> inline static void all_write(OStream &os, const T &x) {
        os << x;
    }

    template <class OStream, class T, class U>
    inline static void all_write(OStream &os, const std::pair<T, U> &p) {
        all_write(os, p.first);
        all_write(os, ' ');
        all_write(os, p.second);
    }

    template <class OStream, class T>
    inline static void all_write(OStream &os, const std::vector<T> &v) {
        for (int i = 0; i < (int)v.size(); ++i) {
            if (i) all_write(os, ' ');
            all_write(os, v[i]);
        }
    }

    template <class OStream, class T, size_t F>
    inline static void all_write(OStream &os, const std::array<T, F> &a) {
        for (int i = 0; i < (int)F; ++i) {
            if (i) all_write(os, ' ');
            all_write(os, a[i]);
        }
    }
};

} // namespace impl

template <kk2::InputStream IStream, class T, class U>
IStream &operator>>(IStream &is, std::pair<T, U> &p) {
    impl::read::all_read(is, p);
    return is;
}

template <kk2::InputStream IStream, class T> IStream &operator>>(IStream &is, std::vector<T> &v) {
    impl::read::all_read(is, v);
    return is;
}

template <kk2::InputStream IStream, class T, size_t F>
IStream &operator>>(IStream &is, std::array<T, F> &a) {
    impl::read::all_read(is, a);
    return is;
}

template <kk2::OutputStream OStream, class T, class U>
OStream &operator<<(OStream &os, const std::pair<T, U> &p) {
    impl::write::all_write(os, p);
    return os;
}

template <kk2::OutputStream OStream, class T>
OStream &operator<<(OStream &os, const std::vector<T> &v) {
    impl::write::all_write(os, v);
    return os;
}

template <kk2::OutputStream OStream, class T, size_t F>
OStream &operator<<(OStream &os, const std::array<T, F> &a) {
    impl::write::all_write(os, a);
    return os;
}

#endif // KK2_TEMPLATE_IO_UTIL_HPP
#ifndef KK2_TEMPLATE_MACROS_HPP
#define KK2_TEMPLATE_MACROS_HPP 1

#define rep1(a) for (long long _ = 0; _ < (long long)(a); ++_)
#define rep2(i, a) for (long long i = 0; i < (long long)(a); ++i)
#define rep3(i, a, b) for (long long i = (a); i < (long long)(b); ++i)
#define repi2(i, a) for (long long i = (a) - 1; i >= 0; --i)
#define repi3(i, a, b) for (long long i = (a) - 1; i >= (long long)(b); --i)
#define overload3(a, b, c, d, ...) d
#define rep(...) overload3(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define repi(...) overload3(__VA_ARGS__, repi3, repi2, rep1)(__VA_ARGS__)

#define fi first
#define se second
#define all(p) begin(p), end(p)

#endif // KK2_TEMPLATE_MACROS_HPP

using kk2::kendl;
using kk2::kin;
using kk2::kout;

void Yes(bool b = 1) { kout << (b ? "Yes\n" : "No\n"); }
void No(bool b = 1) { kout << (b ? "No\n" : "Yes\n"); }
void YES(bool b = 1) { kout << (b ? "YES\n" : "NO\n"); }
void NO(bool b = 1) { kout << (b ? "NO\n" : "YES\n"); }
void yes(bool b = 1) { kout << (b ? "yes\n" : "no\n"); }
void no(bool b = 1) { kout << (b ? "no\n" : "yes\n"); }
template <class T, class S> inline bool chmax(T &a, const S &b) { return (a < b ? a = b, 1 : 0); }
template <class T, class S> inline bool chmin(T &a, const S &b) { return (a > b ? a = b, 1 : 0); }

#endif // KK2_TEMPLATE_TEMPLATE_HPP
using namespace std;

int main() {
    int n, a, b;
    kin >> n >> a >> b;

    auto primes = kk2::PrimeTable::primes(n);
    int pi_n = (int)primes.size();
    auto selected_primes = primes.stride(b, a);
    kout << pi_n << " " << selected_primes.size() << kendl;
    bool first = true;
    for (int p : selected_primes) {
        if (!first) kout << " ";
        first = false;
        kout << p;
    }
    kout << kendl;

    return 0;
}
// Author: kk2
// converted by https://github.com/kk2a/cpp-bundle
// 2026-09-20 06:41:39
