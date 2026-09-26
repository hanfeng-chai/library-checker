#include <cstdint>
#include <cmath>
#include <numeric>
#include <utility>
#include <limits>
#include <cassert>
#include <bit>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <immintrin.h>

#pragma GCC target("avx","avx2")

using namespace std;

#if defined(__clang__)
#define ASSUME(expr) __builtin_assume(expr)
#elif defined(__GNUC__)
#define ASSUME(expr) if (expr) {} else { __builtin_unreachable(); }
#elif defined(_MSC_VER)
#define ASSUME(expr) __assume(expr)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define FORCE_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline
#endif

template<typename T>
struct MultiplicationTraits;

template<>
struct MultiplicationTraits<uint16_t> {
    using long_type = uint32_t;
    static constexpr uint16_t extlo(uint32_t num) noexcept { return uint16_t(num); }
    static constexpr uint16_t exthi(uint32_t num) noexcept { return uint16_t(num >> 16); }
    static constexpr uint32_t merge(uint16_t numhi, uint16_t numlo) noexcept { return uint32_t(numhi) << 16 | uint32_t(numlo); }
    static constexpr uint16_t mullo(uint16_t lhs, uint16_t rhs) noexcept { return lhs * rhs; }
    static constexpr uint16_t mulhi(uint16_t lhs, uint16_t rhs) noexcept { return uint16_t((uint32_t(lhs) * uint32_t(rhs)) >> 16); }
    static constexpr uint32_t mul(uint16_t lhs, uint16_t rhs) noexcept { return uint32_t(lhs) * uint32_t(rhs); }
    static constexpr uint16_t div(uint32_t lhs, uint16_t rhs) noexcept { return divmod(lhs, rhs).first; }
    static constexpr uint16_t mod(uint32_t lhs, uint16_t rhs) noexcept { return divmod(lhs, rhs).second; }
    static constexpr std::pair<uint16_t, uint16_t> divmod(uint32_t lhs, uint16_t rhs) noexcept {
        #if (defined(__GNUC__) || defined(__clang__)) && (defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86))
            if (!std::is_constant_evaluated()) {
                uint16_t q, r;
                __asm__("divw %[den]" : "=a"(q), "=d"(r) : "a"(uint16_t(lhs)), "d"(uint16_t(lhs >> 16)), [den] "r"(rhs));
                return std::make_pair(q, r);
            }
        #endif
        return std::make_pair(uint16_t(lhs / rhs), uint16_t(lhs % rhs));
    }
};

template<>
struct MultiplicationTraits<uint32_t> {
    using long_type = uint64_t;
    static constexpr uint32_t extlo(uint64_t num) noexcept { return uint32_t(num); }
    static constexpr uint32_t exthi(uint64_t num) noexcept { return uint32_t(num >> 32); }
    static constexpr uint64_t merge(uint32_t numhi, uint32_t numlo) noexcept { return uint64_t(numhi) << 32 | uint64_t(numlo); }
    static constexpr uint32_t mullo(uint32_t lhs, uint32_t rhs) noexcept { return lhs * rhs; }
    static constexpr uint32_t mulhi(uint32_t lhs, uint32_t rhs) noexcept { return uint32_t((uint64_t(lhs) * uint64_t(rhs)) >> 32); }
    static constexpr uint64_t mul(uint32_t lhs, uint32_t rhs) noexcept { return uint64_t(lhs) * uint64_t(rhs); }
    static constexpr uint32_t div(uint64_t lhs, uint32_t rhs) noexcept { return divmod(lhs, rhs).first; }
    static constexpr uint32_t mod(uint64_t lhs, uint32_t rhs) noexcept { return divmod(lhs, rhs).second; }
    static constexpr std::pair<uint32_t, uint32_t> divmod(uint64_t lhs, uint32_t rhs) noexcept {
        #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
            if (!std::is_constant_evaluated()) {
                uint32_t q, r;
                #if defined(__GNUC__) || defined(__clang__)
                    __asm__("divl %[den]" : "=a"(q), "=d"(r) : "a"(uint32_t(lhs)), "d"(uint32_t(lhs >> 32)), [den] "r"(rhs));
                    return std::make_pair(q, r);
                #elif defined(_MSC_VER)
                    q = _udiv64(lhs, rhs, &r);
                    return std::make_pair(q, r);
                #endif
            }
        #endif
        return std::make_pair(uint32_t(lhs / rhs), uint32_t(lhs % rhs));
    }
};

template<>
struct MultiplicationTraits<uint64_t> {
    static constexpr uint64_t mullo(uint64_t lhs, uint64_t rhs) noexcept { return lhs * rhs; }
#ifdef __GNUC__
    using long_type = __uint128_t;
    static constexpr uint64_t extlo(long_type num) noexcept { return uint64_t(num); }
    static constexpr uint64_t exthi(long_type num) noexcept { return uint64_t(num >> 64); }
    static constexpr long_type merge(uint64_t numhi, uint64_t numlo) noexcept { return long_type(numhi) << 64 | long_type(numlo); }
    static constexpr uint64_t mulhi(uint64_t lhs, uint64_t rhs) noexcept { return uint64_t((long_type(lhs) * long_type(rhs)) >> 64); }
    static constexpr long_type mul(uint64_t lhs, uint64_t rhs) noexcept { return long_type(lhs) * long_type(rhs); }
    static constexpr uint64_t div(long_type lhs, uint64_t rhs) noexcept { return uint64_t(lhs / long_type(rhs)); }
    static constexpr uint64_t mod(long_type lhs, uint64_t rhs) noexcept { return uint64_t(lhs % long_type(rhs)); }
    static constexpr std::pair<uint64_t, uint64_t> divmod(long_type lhs, uint64_t rhs) noexcept { return std::make_pair(div(lhs, rhs), mod(lhs, rhs)); }
#else
    struct long_type { uint64_t hi, lo; };
    static constexpr uint64_t extlo(long_type num) noexcept { return num.lo; }
    static constexpr uint64_t exthi(long_type num) noexcept { return num.hi; }
    static constexpr long_type merge(uint64_t numhi, uint64_t numlo) noexcept { return long_type{numhi, numlo}; }
    static constexpr uint64_t mulhi(uint64_t lhs, uint64_t rhs) noexcept {
    #if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
        if (!std::is_constant_evaluated()) {
            return __umulh(lhs, rhs);
        }
    #endif
        const uint64_t l0 = lhs & 0xffffffff, l1 = lhs >> 32;
        const uint64_t r0 = rhs & 0xffffffff, r1 = rhs >> 32;
        const uint64_t t = l1 * r0 + ((l0 * r0) >> 32);
        return l1 * r1 + (t >> 32) + (((t & 0xffffffff) + l0 * r1) >> 32);
    }
    static constexpr long_type mul(uint64_t lhs, uint64_t rhs) noexcept {
    #if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
        if (!std::is_constant_evaluated()) {
            uint64_t hi;
            const uint64_t lo = _umul128(lhs, rhs, &hi);
            return long_type{hi, lo};
        }
    #endif
        return long_type{mulhi(lhs, rhs), mullo(lhs, rhs)};
    }
    static constexpr uint64_t div(long_type lhs, uint64_t rhs) noexcept { return divmod(lhs, rhs).first; }
    static constexpr uint64_t mod(long_type lhs, uint64_t rhs) noexcept { return divmod(lhs, rhs).second; }
    static constexpr std::pair<uint64_t, uint64_t> divmod(long_type lhs, uint64_t rhs) noexcept {
    #if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
        if (!std::is_constant_evaluated()) {
            uint64_t r;
            const uint64_t q = _udiv128(lhs.hi, lhs.lo, rhs, &r);
            return std::make_pair(q, r);
        }
    #endif
        return inner_div_mod(lhs.hi, lhs.lo, rhs);
    }
private:
    static constexpr std::pair<uint64_t, uint64_t> inner_div_mod(uint64_t numhi, uint64_t numlo, uint64_t den) noexcept {
        const unsigned shift = std::countl_zero(den);
        numhi = numhi << shift | numlo >> (63 - shift) >> 1;
        numlo <<= shift;
        den <<= shift;
        const uint64_t num1 = numlo >> 32, num0 = numlo & 0xffffffff;
        const uint64_t den1 = den >> 32, den0 = den & 0xffffffff;
        const auto div = [=] (uint64_t num12, uint64_t num0) {
            const uint64_t est_q = num12 / den1, est_r = num12 % den1;
            const uint64_t val1 = est_q * den0, val2 = est_r << 32 | num0;
            return est_q - (val1 > val2) * (1 + (val1 - val2 > den));
        };
        const uint64_t q1 = inner_div(numhi, num1, den, den1, den0);
        const uint64_t t = (numhi << 32) + num1 - q1 * den;
        const uint64_t q0 = inner_div(t, num0, den, den1, den0);
        return {q1 << 32 | q0, ((t << 32) + num0 - q0 * den) >> shift};
    }
    static constexpr uint64_t inner_div(uint64_t num12, uint64_t num0, uint64_t den, uint32_t den1, uint32_t den0) {
        const uint64_t est_q = num12 / den1, est_r = num12 % den1;
        const uint64_t val1 = est_q * den0, val2 = est_r << 32 | num0;
        return est_q - (val1 > val2) * (1 + (val1 - val2 > den));
    }
#endif
};

template<typename T, typename Traits = MultiplicationTraits<T>>
class Divisor {
    T _inv;
    uint8_t _delta, _shift;
public:
    constexpr Divisor(T d = 1) {
        if (d == 0) throw std::domain_error("dividing zero is invalid.");
        const uint8_t log_d = std::numeric_limits<T>::digits - 1 - std::countl_zero(d);
        if (d == 1) {
            _inv = std::numeric_limits<T>::max();
            _delta = 1;
            _shift = 0;
        } else if (((d - 1) & d) == 0) {
            _inv = T(1) << (std::numeric_limits<T>::digits - log_d);
            _delta = 0;
            _shift = 0;
        } else {
            const auto [q, r] = Traits::divmod(Traits::merge(T(1) << log_d, T(0)), d);
            if (d - r < (T(1) << log_d)) {
                _inv = q + 1;
                _delta = 0;
            } else {
                _inv = q;
                _delta = 1;
            }
            _shift = log_d;
        }
    }
    constexpr friend T operator/(T lhs, Divisor rhs) {
        assert(lhs != std::numeric_limits<T>::max());
        return Traits::mulhi(lhs + rhs._delta, rhs._inv) >> rhs._shift;
    }
};

template<typename T, typename Traits = MultiplicationTraits<T>>
class ExtendedDivisor : public Divisor<T, Traits> {
    T _den;
public:
    constexpr ExtendedDivisor(T d = 1) : _den(d), Divisor<T, Traits>(d) {}
    constexpr T value() const noexcept { return _den; }
    constexpr operator T() const noexcept { return _den; }
    constexpr std::pair<T, T> operator()(T num) const { const T q = num / *this; return {q, num - Traits::mullo(q, _den)}; }
    constexpr friend T operator%(T lhs, ExtendedDivisor rhs) { return rhs(lhs).second; }
    friend std::pair<T, T> divmod(T lhs, ExtendedDivisor rhs) { return rhs(lhs); }
};

template<typename T, typename Reducer, unsigned W = 16>
class WideTree {
public:
    using size_type = unsigned;
    template<typename Op>
    WideTree(size_type n, Op op, Reducer reducer = {})
        : _reducer(reducer), _n(n)
        , _h((std::bit_width(n) + S - 1) / S)
    {
        _offset[0] = 0;
        for (size_type i = 0, j = n;i < _h;++i, j >>= S)
            _offset[i + 1] = _offset[i] + (j & -B) + B;
        _tree = static_cast<T*>(operator new(_offset[_h] * sizeof(T), std::align_val_t(A)));
        op(_tree);
        auto cur = _tree + _n;
        try {
            for (size_type i = _n;i < _offset[1];++i, ++cur)
                new(cur) T(_reducer.zero());
            for (size_type i = 1, j = n >> S;i < _h;++i, j >>= S) {
                const auto end = _tree + _offset[i] + j, end0 = _tree + _offset[i + 1];
                auto pre = _tree + _offset[i - 1];
                for (;cur != end;++cur, pre += B)
                    new(cur) T(_reducer.template reduce_bounded<B>(pre, B));
                for (;cur != end0;++cur)
                    new(cur) T(_reducer.zero());
            }
        } catch (...) {
            operator delete(_tree, std::align_val_t(A));
            std::destroy(_tree, cur);
            throw;
        }
    }
    ~WideTree() { operator delete(_tree, std::align_val_t(A)); std::destroy_n(_tree, _offset[_h]); }
    size_type size() const noexcept { return _n; }
    T get(size_type idx) const { return _tree[idx]; }
    T query(size_type idx) const {
        ASSUME(_h <= H);
        auto ans = _reducer.vec_zero();
        #pragma GCC unroll 64
        for (size_type i = 0;i < _h;++i)
            ans = _reducer.vec_add(ans, _reducer.template partial_reduce_bounded<B>(_tree + _offset[i] + (idx >> (i * S) & -B), (idx >> (i * S) & (B - 1))));
        return _reducer.final_reduce(ans);
    }
    void update(size_type idx, T delta) {
        ASSUME(_h <= H);
        #pragma GCC unroll 64
        for (size_type i = 0;i < _h;++i)
            _tree[_offset[i] + (idx >> (i * S))] = _reducer.add(_tree[_offset[i] + (idx >> (i * S))], delta);
    }
private:
    static constexpr size_type A = Reducer::ALIGNMENT_REQUIREMENT, B = std::max(size_type(A / sizeof(T)), W);
    static constexpr size_type S = std::bit_width(B - 1), H = (std::numeric_limits<size_type>::digits + S - 1) / S;
    static_assert(((B - 1) & B) == 0);
    T* _tree;
    size_type _n, _h, _offset[H + 1];
    Reducer _reducer;
};

template<typename T>
struct DefaultRing {
    using value_type = T;
    T zero() const { return 0; }
    T one() const { return 1; }
    T neg(T x) const { return -x; }
    T add(T x, T y) const { return x + y; }
    T sub(T x, T y) const { return x - y; }
    T mul(T x, T y) const { return x * y; }
    bool eq(T x, T y) const { return x == y; }
    bool neq(T x, T y) const { return x != y; }
};

template<typename Ring>
struct ReducerAdapter : Ring {
    using typename Ring::value_type;
    static constexpr unsigned ALIGNMENT_REQUIREMENT = alignof(value_type), B = 1;
    value_type vec_zero() const noexcept { return this->zero(); }
    value_type vec_add(value_type x, value_type y) const noexcept { return this->add(x, y); }
    value_type partial_reduce(const value_type* ptr, unsigned len) const noexcept {
        return std::reduce(ptr, ptr + len, this->zero(), [this] (value_type x, value_type y) { return this->add(x, y); });
    }
    template<unsigned W> value_type partial_reduce_bounded(const value_type* ptr, unsigned len) const noexcept { return partial_reduce(ptr, len); }
    value_type final_reduce(value_type x) const noexcept { return x; }
    value_type reduce(const value_type* ptr, unsigned len) const noexcept { return final_reduce(partial_reduce(ptr, len)); }
    template<unsigned W> value_type reduce_bounded(const value_type* ptr, unsigned len) const noexcept { return final_reduce(partial_reduce_bounded<W>(ptr, len)); }
};

template<typename T>
using DefaultReducer = ReducerAdapter<DefaultRing<T>>;

template<typename T = uint64_t, typename U = uint32_t, typename D = Divisor<T>>
class MultiplicativeSum {
public:
    using full_type = T;
    using half_type = U;

    template<typename Ring, typename Reducer>
    struct solver_type {
    public:
        using value_type = typename Ring::value_type;
        half_type len() const noexcept { return _len; }
        half_type len_s() const noexcept { return _n2; }
        half_type len_l() const noexcept { return _r_n2; }
        half_type idx(full_type num) const noexcept { return num <= _n2 ? idx_s(num) : idx_l(_n / num); }
        half_type ridx(full_type num) const noexcept { return num <= _r_n2 ? idx_l(num) : idx_s(_n / num); }
        half_type pi(half_type n) const noexcept { return std::distance(_primes, std::upper_bound(_primes, _primes + _pi_n2, n)); }

        template<typename OutR, typename SumF>
        void init_block(OutR&& block_out, SumF sum_f) const {
            for (half_type i = 1;i <= _n2;++i) block_out[idx_s(i)] = sum_f(i);
            for (half_type i = _r_n2;i > 0;--i) block_out[idx_l(i)] = sum_f(div(_n, i));
        }

        template<typename R, typename Fpp>
        value_type mul_powerful_single(const R& block, Fpp fpp) const { return mul_powerful_single(block, 1, fpp); }
        template<typename R, typename Fpp>
        value_type mul_powerful_iterative_single(const R& block, Fpp fpp) const { return mul_powerful_iterative_single(block, 1, fpp); }
        template<typename R, typename Fpp>
        value_type mul_powerful_single(const R& block, half_type ridx, Fpp fpp) const { return dfs_powerful(block, ridx, 0, 1, _n, _ring.one(), fpp); }
        template<typename R, typename Fpp>
        value_type mul_powerful_iterative_single(const R& block, half_type ridx, Fpp fpp) const { return dfs_powerful_iterative(block, ridx, 0, 1, _n, _ring.one(), fpp); }

        template<typename R, typename OutR, typename Fpp>
        void mul_powerful(const R& block, OutR&& block_out, Fpp fpp) const {
            std::vector<value_type> block_pn(_len, _ring.zero());
            collect_powerful(block_pn, 0, 1, _n, _ring.one(), fpp);
            mul_block_pn(block, block_pn, block_out);
        }

        template<typename R, typename OutR, typename Fpp>
        void mul_powerful_iterative(const R& block, OutR&& block_out, Fpp fpp) const {
            std::vector<value_type> block_pn(_len, _ring.zero());
            collect_powerful_iterative(block_pn, 0, 1, _n, _ring.one(), fpp);
            mul_block_pn(block, block_pn, block_out);
        }

        template<typename R, typename Fp>
        void eliminate_small(R&& block, half_type pi_r, Fp fp) const {
            for (half_type i = 0;i < pi_r;++i) {
                const half_type p = _primes[i];
                const full_type n_d_p = div(_n, p);
                const half_type bound1 = div(_r_n2, p), bound2 = _r_n2;
                const value_type val = fp(p);
                for (half_type j = 1;j <= bound1;++j)
                    dec(block[idx_l(j)], val, block[idx_l(j * p)]);
                for (half_type j = bound1 + 1;j <= bound2;++j)
                    dec(block[idx_l(j)], val, block[idx_s(div(n_d_p, j))]);
                for (half_type j = _n2;j >= p;--j)
                    dec(block[idx_s(j)], val, block[idx_s(div(j, p))]);
            }
        }

        template<typename R, typename Fp>
        void attach_small(R&& block, half_type pi_r, Fp fp) const {
            for (half_type i = pi_r;i-- > 0;) {
                const half_type p = _primes[i];
                const full_type n_d_p = div(_n, p);
                const half_type bound1 = div(_r_n2, p), bound2 = _r_n2;
                const value_type val = fp(p);
                for (half_type j = p;j <= _n2;++j)
                    inc(block[idx_s(j)], val, block[idx_s(div(j, p))]);
                for (half_type j = bound2;j > bound1;--j)
                    inc(block[idx_l(j)], val, block[idx_s(div(n_d_p, j))]);
                for (half_type j = bound1;j > 0;--j)
                    inc(block[idx_l(j)], val, block[idx_l(j * p)]);
            }
        }

        template<typename R, typename Fp>
        void attach_small_square_free(R&& block, half_type pi_r, Fp fp) const {
            for (half_type i = pi_r;i-- > 0;) {
                const half_type p = _primes[i];
                const full_type n_d_p = div(_n, p);
                const half_type bound1 = div(_r_n2, p), bound2 = _r_n2;
                const value_type val = fp(p);
                for (half_type j = 1;j <= bound1;++j)
                    inc(block[idx_l(j)], val, block[idx_l(j * p)]);
                for (half_type j = bound1 + 1;j <= bound2;++j)
                    inc(block[idx_l(j)], val, block[idx_s(div(n_d_p, j))]);
                for (half_type j = _n2;j >= p;--j)
                    inc(block[idx_s(j)], val, block[idx_s(div(j, p))]);
            }
        }

        template<typename Tree, unsigned MaxDepth, typename R, typename Fp>
        void attach_medium_small(R&& block, half_type pi_l, half_type pi_r, half_type r_tree_bound, Fp fp) const {
            const half_type tree_bound = div(_n, r_tree_bound + 1), len_tree = _len - r_tree_bound;
            Tree tree(len_tree, [&block, len_tree, this] (value_type* ptr) {
                value_type pre = _ring.zero();
                value_type* cur = ptr;
                try {
                    for (half_type i = 0;i < len_tree;pre = block[i++], ++cur)
                        ::new(cur) value_type(sub(block[i], pre));
                } catch (...) {
                    std::destroy(ptr, cur);
                    throw;
                }
            }, _reducer);
            for (half_type i = pi_r;i-- > pi_l;) {
                const half_type p = _primes[i];
                const full_type p2 = full_type(p) * full_type(p), n_d_p = div(_n, p), n_d_p2 = div(n_d_p, p);
                const half_type bound1 = std::min(half_type(div(_r_n2, p)), r_tree_bound), bound2 = std::min(n_d_p2, full_type(r_tree_bound));
                const half_type idx_p2 = p2 <= _n2 ? idx_s(p2) : std::min(idx_l(n_d_p2), len_tree);
                const auto access = [&block, &tree, len_tree, idx_p2] (half_type idx) { return idx_p2 <= idx && idx < len_tree ? tree.query(idx + 1) : block[idx]; };
                const value_type val = fp(p), pre = block[idx_s(p - 1)];
                dfs<false, MaxDepth>(tree, i, p, val, tree_bound, fp);
                for (half_type j = bound2;j > bound1;--j)
                    inc(block[idx_l(j)], val, sub(access(idx_s(div(n_d_p, j))), pre));
                for (half_type j = bound1;j > 0;--j)
                    inc(block[idx_l(j)], val, sub(access(idx_l(j * p)), pre));
                
            }
            value_type sum = _ring.zero();
            for (half_type i = 0;i < len_tree;++i)
                block[i] = sum = add(sum, tree.get(i));
        }

        template<typename Tree, unsigned MaxDepth, typename R, typename Fp>
        void attach_medium_small_square_free(R&& block, half_type pi_l, half_type pi_r, half_type r_tree_bound, Fp fp) const {
            const half_type tree_bound = div(_n, r_tree_bound + 1), len_tree = _len - r_tree_bound;
            Tree tree(len_tree, [&block, len_tree, this] (value_type* ptr) {
                value_type pre = _ring.zero();
                value_type* cur = ptr;
                try {
                    for (half_type i = 0;i < len_tree;pre = block[i++], ++cur)
                        ::new(cur) value_type(sub(block[i], pre));
                } catch (...) {
                    std::destroy(ptr, cur);
                    throw;
                }
            }, _reducer);
            for (half_type i = pi_r;i-- > pi_l;) {
                const half_type p = _primes[i];
                const full_type p2 = full_type(p) * full_type(p), n_d_p = div(_n, p), n_d_p2 = div(n_d_p, p);
                const half_type bound1 = std::min(half_type(div(_r_n2, p)), r_tree_bound), bound2 = std::min(n_d_p2, full_type(r_tree_bound));
                const half_type idx_p2 = p2 <= _n2 ? idx_s(p2) : std::min(idx_l(n_d_p2), len_tree);
                const auto access = [&block, &tree, len_tree, idx_p2] (half_type idx) { return idx_p2 <= idx && idx < len_tree ? tree.query(idx + 1) : block[idx]; };
                const value_type val = fp(p), pre = block[idx_s(p)];
                for (half_type j = 1;j <= bound1;++j)
                    inc(block[idx_l(j)], val, sub(access(idx_l(j * p)), pre));
                for (half_type j = bound1 + 1;j <= bound2;++j)
                    inc(block[idx_l(j)], val, sub(access(idx_s(div(n_d_p, j))), pre));
                dfs<true, MaxDepth>(tree, i + 1, p, val, tree_bound, fp);
            }
            value_type sum = _ring.zero();
            for (half_type i = 0;i < len_tree;++i)
                block[i] = sum = add(sum, tree.get(i));
        }

        template<bool SquareFree = false, typename OutR, typename LargeR, typename Div3, typename Fp>
        void cal_medium_large_invertible(OutR&& block_out, const LargeR& block_large, half_type pi_l, half_type pi_n4, Div3 div3, Fp fp) const {
            block_out[0] = _ring.one();
            for (half_type i = pi_l;i < _pi_n2;++i)
                block_out[idx_s(_primes[i])] = fp(_primes[i]);
            for (half_type i = 1;i < _n2;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = pi_n4, r = _pi_n2;i < _pi_n2;++i) {
                const half_type p = _primes[i];
                const full_type n_d_p = div(_n, p);
                while (r > 0 && full_type(r - 1) * full_type(_primes[r - 1]) > n_d_p)
                    --r;
                const value_type val = fp(p);
                for (half_type j = i + SquareFree;j < r;++j)
                    inc(block_out[idx_l(hdiv(n_d_p, _primes[j]))], val, fp(_primes[j]));
                const half_type bound = std::min(_primes[std::max(r, i + SquareFree)], _n2 + 1);
                if (p <= _r_n2) inc(block_out[idx_l(hdiv(_r_n2, p))], val, block_out[idx_s(_n2)]);
                if (bound <= n_d_p) dec(block_out[idx_l(hdiv(n_d_p, bound))], val, block_out[idx_s(bound - 1)]);
            }
            for (half_type i = _r_n2 - 1;i > 0;--i)
                inc(block_out[idx_l(i)], block_out[idx_l(i + 1)]);
            for (half_type i = pi_n4, r = _pi_n2;i < _pi_n2;++i) {
                const half_type p = _primes[i];
                const full_type n_d_p = div(_n, p);
                while (r > 0 && full_type(r - 1) * full_type(_primes[r - 1]) > n_d_p)
                    --r;
                const half_type bound = std::min(_primes[std::max(r, i + SquareFree)], _n2 + 1);
                const half_type bound1 = div(_r_n2, p), bound2 = div(n_d_p, bound);
                const value_type val = fp(p);
                for (half_type j = bound2;j > bound1;--j)
                    inc(block_out[idx_l(j)], val, block_out[idx_s(div(n_d_p, j))]);
                for (half_type j = bound1;j > 0;--j)
                    inc(block_out[idx_l(j)], val, block_large[_r_n2 - j * p]);
            }
            const half_type bound_p = _primes[pi_n4], bound_z = div(div(div(_n, bound_p), bound_p), bound_p);
            for (half_type i = 1;i <= bound_z;++i) {
                const full_type n_d_z = div(_n, i);
                const half_type bound1 = pi(div(_r_n2, i)), bound2 = pi(floor(sqrt(div(n_d_z, bound_p) + 0.5))), bound3 = pi(floor(cbrt(n_d_z + 0.5)));
                value_type sum = _ring.zero();
                for (half_type j = pi_n4;j < bound1;++j)
                    inc(sum, mul(fp(_primes[j]), block_out[idx_l(i * _primes[j])]));
                if constexpr (SquareFree) {
                    for (half_type j = pi_n4;j < bound2;++j)
                        dec(sum, mul(fp(_primes[j]), mul(fp(_primes[j]), sub(block_out[idx_s(div(div(n_d_z, _primes[j]), _primes[j]))], block_out[idx_s(bound_p - 1)]))));
                } else {
                    for (half_type j = pi_n4;j < bound2;++j)
                        inc(sum, mul(fp(_primes[j]), mul(fp(_primes[j]), sub(block_out[idx_s(div(div(n_d_z, _primes[j]), _primes[j]))], block_out[idx_s(bound_p - 1)]))));
                }
                for (half_type j = pi_n4;j < bound3;++j)
                    inc(sum, mul(fp(_primes[j]), mul(fp(_primes[j]), fp(_primes[j]))));
                inc(block_out[idx_l(i)], div3(sum));
            }
            for (half_type i = 1;i <= _r_n2;++i)
                inc(block_out[idx_l(i)], add(block_large[_r_n2 - i], block_out[idx_s(_n2)]));
        }

        template<typename OutR, typename Div3, typename Fp>
        void cal_medium_large_invertible(OutR&& block_out, half_type pi_l, half_type pi_n4, Div3 div3, Fp fp) const {
            struct Zeros { Ring _ring; value_type operator[](half_type) const { return _ring.zero(); } };
            return cal_medium_large_invertible(block_out, Zeros{_ring}, pi_l, pi_n4, div3, fp);
        }
        
        template<typename R, typename OutLargeR, typename SumF>
        void eliminate_nonlarge(const R& block, OutLargeR&& block_large_out, SumF sum_f) const {
            for (half_type i = _r_n2;i > 0;--i) {
                value_type ans = sub(sum_f(div(_n, i)), block[idx_l(i)]);
                value_type pre = block[0], cur = _ring.zero();
                for (half_type j = 2;i * j <= _r_n2;++j, pre = cur)
                    dec(ans, block_large_out[_r_n2 - i * j], sub(cur = block[idx_s(j)], pre));
                block_large_out[_r_n2 - i] = ans;
            }
        }

        template<typename OutLargeR, typename Div3, typename SumF, typename Fp>
        void sum_large_invertible(OutLargeR&& block_large_out, Div3 div3, SumF sum_f, Fp fp, double alpha = 1.0, double beta = 1.0) const {
            const half_type r_tree_bound = std::min(half_type(floor(pow(_n, 3.0 / 8) / alpha)), _r_n2), tree_bound = div(_n, r_tree_bound + 1);
            const half_type bound_n8 = std::max(half_type(div(tree_bound, _n2 + 1)), half_type(pow(tree_bound + 0.5, 1.0 / 5)));
            const half_type n8 = std::max(half_type(floor(pow(_n + 0.5, 1.0 / 8) * beta)), bound_n8);
            const half_type pi_n8 = pi(n8), n4 = floor(pow(_n + 0.5, 1.0 / 4)), pi_n4 = pi(n4);
            std::vector<value_type> block(_len, _ring.zero());
            cal_medium_large_invertible(block.data(), pi_n8, pi_n4, div3, fp);
            attach_medium_small<WideTree<value_type, Reducer>, 3>(block, pi_n8, pi_n4, r_tree_bound, fp);
            attach_small(block.data(), pi_n8, fp);
            eliminate_nonlarge(block.data(), block_large_out, sum_f);
        }

        template<typename LargeR, typename OutR, typename Div3, typename Fp>
        void attach_nonlarge_invertible(const LargeR& block_large, OutR&& block_out, Div3 div3, Fp fp, double alpha = 1.0, double beta = 1.0) const {
            const half_type r_tree_bound = std::min(half_type(floor(pow(_n, 3.0 / 8) / alpha)), _r_n2), tree_bound = div(_n, r_tree_bound + 1);
            const half_type bound_n8 = std::max(half_type(div(tree_bound, _n2 + 1)), half_type(pow(tree_bound + 0.5, 1.0 / 5)));
            const half_type n8 = std::max(half_type(floor(pow(_n + 0.5, 1.0 / 8) * beta)), bound_n8);
            const half_type pi_n8 = pi(n8), n4 = floor(pow(_n + 0.5, 1.0 / 4)), pi_n4 = pi(n4);
            cal_medium_large_invertible<false>(block_out, block_large, pi_n8, pi_n4, div3, fp);
            attach_medium_small<WideTree<value_type, Reducer>, 3>(block_out, pi_n8, pi_n4, r_tree_bound, fp);
            attach_small(block_out, pi_n8, fp);
        }

        template<typename LargeR, typename OutR, typename Div3, typename Fp>
        void attach_nonlarge_square_free_invertible(const LargeR& block_large, OutR&& block_out, Div3 div3, Fp fp, double alpha = 1.0, double beta = 1.0) const {
            const half_type r_tree_bound = std::min(half_type(floor(pow(_n, 3.0 / 8) / alpha)), _r_n2), tree_bound = div(_n, r_tree_bound + 1);
            const half_type bound_n8 = std::max(half_type(div(tree_bound, _n2 + 1)), half_type(pow(tree_bound + 0.5, 1.0 / 5)));
            const half_type n8 = std::max(half_type(floor(pow(_n + 0.5, 1.0 / 8) * beta)), bound_n8);
            const half_type pi_n8 = pi(n8), n4 = floor(pow(_n + 0.5, 1.0 / 4)), pi_n4 = pi(n4);
            cal_medium_large_invertible<true>(block_out, block_large, pi_n8, pi_n4, div3, fp);
            attach_medium_small_square_free<WideTree<value_type, Reducer>, 3>(block_out, pi_n8, pi_n4, r_tree_bound, fp);
            attach_small_square_free(block_out, pi_n8, fp);
        }

        template<typename R>
        std::vector<std::pair<half_type, value_type>> collect_nonzero(const R& block) const {
            std::vector<std::pair<half_type, value_type>> ans;
            value_type pre = _ring.zero(), cur = _ring.zero();
            for (half_type i = 1;i <= _n2;++i, pre = cur)
                if (_ring.neq(cur = block[idx_s(i)], pre))
                    ans.emplace_back(i, _ring.sub(cur, pre));
            ans.emplace_back(_n2 + 1, _ring.zero());
            return ans;
        }

        template<typename R1, typename R2, typename PairR1, typename PairR2, typename OutR>
        void mul_sparse(const R1& block1, const R2& block2, const PairR1&& pairs1, const PairR2& pairs2, half_type cnt1, half_type cnt2, OutR&& block_out) const {
            for (half_type i = 0, l = 0, r = cnt2;i < cnt1;++i) {
                const auto [x, fx] = pairs1[i];
                while (pairs2[l].first < x) ++l;
                const full_type n_d_x = div(_n, x);
                while (r > l && full_type(r - 1) * full_type(pairs2[r - 1].first) > n_d_x) --r;
                mul_sparse_small(x, n_d_x, fx, l, std::max(l, r), pairs2, block2, block_out);
            }
            for (half_type i = 0, l = 0, r = cnt1;i < cnt2;++i) {
                const auto [y, fy] = pairs2[i];
                while (pairs1[l].first <= y) ++l;
                const full_type n_d_y = div(_n, y);
                while (r > l && full_type(r - 1) * full_type(pairs1[r - 1].first) > n_d_y) --r;
                mul_sparse_small(y, n_d_y, fy, l, std::max(l, r), pairs1, block1, block_out);
            }
            for (half_type i = 1;i < _len;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = 0, l = 0, r = cnt2;i < cnt1;++i) {
                const auto [x, fx] = pairs1[i];
                while (pairs2[l].first < x) ++l;
                const full_type n_d_x = div(_n, x);
                while (r > l && full_type(r - 1) * full_type(pairs2[r - 1].first) > n_d_x) --r;
                mul_sparse_large(x, n_d_x, fx, pairs2[std::max(l, r)].first, block2, block_out);
            }
            for (half_type i = 0, l = 0, r = cnt1;i < cnt2;++i) {
                const auto [y, fy] = pairs2[i];
                while (pairs1[l].first <= y) ++l;
                const full_type n_d_y = div(_n, y);
                while (r > l && full_type(r - 1) * full_type(pairs1[r - 1].first) > n_d_y) --r;
                mul_sparse_large(y, n_d_y, fy, pairs1[std::max(l, r)].first, block1, block_out);
            }
        }

        template<typename R1, typename R2, typename OutR>
        void mul_sparse(const R1& block1, const R2& block2, OutR&& block_out) const {
            const auto pairs1 = collect_nonzero(block1), pairs2 = collect_nonzero(block2);
            mul_sparse(block1, block2, pairs1.data(), pairs2.data(), pairs1.size() - 1, pairs2.size() - 1, block_out);
        }

        template<typename R, typename PairR, typename OutR>
        void sqr_sparse(const R& block, const PairR& pairs, half_type cnt, OutR&& block_out) const {
            for (half_type i = 0, r = cnt;i < cnt;++i) {
                const auto [x, fx] = pairs[i];
                const full_type n_d_x = div(_n, x);
                while (r > i && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_x) --r;
                mul_sparse_small(x, n_d_x, add(fx, fx), i + 1, std::max(i + 1, r), pairs, block, block_out);
                inc(block_out[full_type(x) * full_type(x) <= _n2 ? idx_s(x * x) : idx_l(hdiv(n_d_x, x))], fx, fx);
            }
            for (half_type i = 1;i < _len;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = 0, r = cnt;i < cnt;++i) {
                const auto [x, fx] = pairs[i];
                const full_type n_d_x = div(_n, x);
                while (r > i && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_x) --r;
                mul_sparse_large(x, n_d_x, add(fx, fx), pairs[std::max(i + 1, r)].first, block, block_out);
            }
        }

        template<typename R, typename OutR>
        void sqr_sparse(const R& block, OutR&& block_out) const {
            const auto pairs = collect_nonzero(block);
            sqr_sparse(block, pairs.data(), pairs.size() - 1, block_out);
        }

        template<typename R1, typename R2, typename PairR2, typename OutR>
        void div_sparse(const R1& block1, const R2& block2, const PairR2& pairs2, half_type cnt2, OutR&& block_out) const {
            std::vector<std::pair<half_type, value_type>> pairs;
            block_out[0] = block1[0];
            for (half_type i = 1;i < _n2;++i)
                block_out[i] = sub(block1[i], block1[i - 1]);
            value_type sum = _ring.zero();
            for (half_type i = 1;i <= _n2;++i) {
                const value_type val = block_out[idx_s(i)];
                block_out[idx_s(i)] = sum = add(sum, val);
                if (_ring.eq(val, _ring.zero())) continue;
                pairs.emplace_back(i, val);
                for (half_type j = 1;j < cnt2 && i * pairs2[j].first <= _n2;++j)
                    dec(block_out[idx_s(i * pairs2[j].first)], val, pairs2[j].second);
            }
            const half_type cnt = pairs.size();
            pairs.emplace_back(_n2 + 1, _ring.zero());
            for (half_type i = 0, l = 0, l0 = cnt2, r = cnt2;i < cnt;++i) {
                const auto [x, fx] = pairs[i];
                while (pairs2[l].first <= x) ++l;
                const full_type n_d_x = div(_n, x);
                while (l0 > l && full_type(x) * full_type(pairs2[l0 - 1].first) > _n2) --l0;
                while (r > l && full_type(r - 1) * full_type(pairs2[r - 1].first) > n_d_x) --r;
                for (half_type j = std::max(l, l0);j < r;++j)
                    inc(block_out[idx_l(hdiv(n_d_x, pairs2[j].first))], fx, pairs2[j].second);
                r = std::max(l, r);
                if (r > 0 && pairs2[r].first <= n_d_x) dec(block_out[idx_l(hdiv(n_d_x, pairs2[r].first))], fx, block2[idx_s(pairs2[r - 1].first)]);
            }
            for (half_type i = 0, l = 0, l0 = cnt, r = cnt, bound_z = _r_n2;i < cnt2;++i) {
                const auto [y, fy] = pairs2[i];
                while (pairs[l].first < y) ++l;
                const full_type n_d_y = div(_n, y);
                const half_type bound_y = div(n_d_y, bound_z + 1);
                while (l0 > l && full_type(y) * full_type(pairs[l0 - 1].first) > _n2) --l0;
                while (r > l && pairs[r - 1].first > bound_y && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_y) --r;
                for (half_type j = std::max(l, l0);j < r;++j)
                    inc(block_out[idx_l(hdiv(n_d_y, pairs[j].first))], fy, pairs[j].second);
                r = std::max(l, r);
                if (r > 0 && pairs[r].first <= n_d_y) dec(block_out[idx_l(hdiv(n_d_y, pairs[r].first))], fy, block_out[idx_s(pairs[r - 1].first)]);
                bound_z = div(n_d_y, pairs[r].first);
            }
            inc(block_out[_n2], block1[idx_s(_n2)]);
            for (half_type i = _n2 + 1;i < _len;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = 0, l = 0, r = cnt2;i < cnt;++i) {
                const auto [x, fx] = pairs[i];
                while (pairs2[l].first <= x) ++l;
                const full_type n_d_x = div(_n, x);
                while (r > l && full_type(r - 1) * full_type(pairs2[r - 1].first) > n_d_x) --r;
                mul_sparse_large(x, n_d_x, fx, pairs2[std::max(l, r)].first, block2, block_out);
            }
            for (half_type i = _r_n2, cnt_y = 0, l = 0, r = cnt, bound_z = _r_n2;i > 0;--i) {
                while (i <= bound_z) {
                    const half_type y = pairs2[++cnt_y].first;
                    while (pairs[l].first < y) ++l;
                    const full_type n_d_y = div(_n, y);
                    const half_type bound_y = div(n_d_y, bound_z + 1);
                    while (r > l && pairs[r - 1].first > bound_y && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_y) --r;
                    r = std::max(l, r);
                    bound_z = div(n_d_y, pairs[r].first);
                }
                const full_type n_d_z = div(_n, i);
                value_type ans = sub(block1[idx_l(i)], block_out[idx_l(i)]);
                for (half_type j = 1;j < cnt_y;++j) {
                    const auto [y, fy] = pairs2[j];
                    dec(ans, fy, block_out[full_type(i) * full_type(y) <= _r_n2 ? idx_l(i * y) : idx_s(hdiv(n_d_z, y))]);
                }
                block_out[idx_l(i)] = ans;
            }
        }

        template<typename R1, typename R2, typename OutR>
        void div_sparse(const R1& block1, const R2& block2, OutR&& block_out) const {
            const auto pairs2 = collect_nonzero(block2);
            div_sparse(block1, block2, pairs2, pairs2.size() - 1, block_out);
        }

        template<typename R, typename PairR, typename OutR>
        void inv_sparse(const R& block, const PairR& pairs, half_type cnt, OutR&& block_out) const {
            struct Ones { Ring _ring; value_type operator[](half_type idx) const { return _ring.one(); } };
            return div_sparse(Ones{_ring}, block, pairs, cnt, block_out);
        }

        template<typename R, typename OutR>
        void inv_sparse(const R& block, OutR&& block_out) const {
            const auto pairs = collect_nonzero(block);
            return inv_sparse(block, pairs, pairs.size() - 1, block_out);
        }

    private:
        friend class MultiplicativeSum;
        const D* _divisors;
        const half_type* _primes;
        full_type _n;
        half_type _n2, _r_n2, _len, _pi_n2;
        Ring _ring;
        Reducer _reducer;
        full_type div(full_type num, half_type den) const { return num / _divisors[den - 1]; }
        half_type hdiv(full_type num, half_type den) const { return MultiplicationTraits<half_type>::div(num, den); }
        half_type idx_s(half_type idx) const noexcept { assert(idx <= _n2); return idx - 1; }
        half_type idx_l(half_type idx) const noexcept { assert(idx <= _r_n2); return _len - idx; }
        value_type add(value_type lhs, value_type rhs) const { return _ring.add(lhs, rhs); }
        value_type sub(value_type lhs, value_type rhs) const { return _ring.sub(lhs, rhs); }
        value_type mul(value_type lhs, value_type rhs) const { return _ring.mul(lhs, rhs); }
        void inc(value_type& ans, value_type delta) const { ans = _ring.add(ans, delta); }
        void dec(value_type& ans, value_type delta) const { ans = _ring.sub(ans, delta); }
        void inc(value_type& ans, value_type lhs, value_type rhs) const { ans = _ring.add(ans, _ring.mul(lhs, rhs)); }
        void dec(value_type& ans, value_type lhs, value_type rhs) const { ans = _ring.sub(ans, _ring.mul(lhs, rhs)); }

        solver_type(full_type n, const D* divisors, const half_type* primes, half_type pi_bound, Ring ring = {}, Reducer reducer = {})
            : _n(n), _divisors(divisors), _primes(primes), _ring(ring), _reducer(reducer), _n2(floor(sqrt(n + 0.5))), _r_n2(div(n, _n2 + 1)), _len(_n2 + _r_n2), _pi_n2(pi_bound) { _pi_n2 = pi(_n2); }

        template<typename PairR, typename R, typename OutR>
        void mul_sparse_small(half_type x, full_type n_d_x, value_type fx, half_type l, half_type r, const PairR& pairs, const R& block, OutR&& block_out) const {
            for (half_type j = l;j < r;++j) {
                const auto [y, fy] = pairs[j];
                inc(block_out[full_type(x) * full_type(y) <= _n2 ? idx_s(x * y) : idx_l(hdiv(n_d_x, y))], fx, fy);
            }
            if (r > 0 && pairs[r].first <= n_d_x) dec(block_out[idx_l(hdiv(n_d_x, pairs[r].first))], fx, block[idx_s(pairs[r - 1].first)]);
        }

        template<typename R, typename OutR>
        void mul_sparse_large(half_type x, full_type n_d_x, value_type fx, half_type bound, const R& block, OutR&& block_out) const {
            const half_type bound1 = div(_r_n2, x), bound2 = div(n_d_x, bound);
            for (half_type j = bound2;j > bound1;--j)
                inc(block_out[idx_l(j)], fx, block[idx_s(div(n_d_x, j))]);
            for (half_type j = bound1;j > 0;--j)
                inc(block_out[idx_l(j)], fx, block[idx_l(x * j)]);
        }

        template<bool SquareFree, unsigned MaxDepth, unsigned Depth = 0, typename Tree, typename Fp>
        void dfs(Tree& tree, half_type pid, full_type num, value_type val, half_type tree_bound, Fp fp) const {
            if constexpr (Depth < MaxDepth) {
                for (half_type i = pid;i < _pi_n2 && num * _primes[i] <= tree_bound;++i) {
                    const value_type nxt_val = mul(val, fp(_primes[i]));
                    const full_type nxt_num = num * _primes[i];
                    tree.update(nxt_num <= _n2 ? idx_s(nxt_num) : idx_l(_n / nxt_num), nxt_val);
                    dfs<SquareFree, MaxDepth, Depth + 1>(tree, i + SquareFree, nxt_num, nxt_val, tree_bound, fp);
                }
            }
        }

        template<typename R, typename Fpp>
        value_type dfs_powerful(const R& block, half_type ridx, half_type pid, full_type num, full_type n_d_num, value_type val, Fpp fpp) const {
            return dfs_powerful_iterative(block, ridx, pid, num, n_d_num, val, [fpp] (unsigned p) {
                return [fpp, p] (full_type pp, unsigned e) { return fpp(pp, p, e); };
            });
        }

        template<typename R, typename Fpp>
        value_type dfs_powerful_iterative(const R& block, half_type ridx, half_type pid, full_type num, full_type n_d_num, value_type val, Fpp fpp) const {
            value_type ans = mul(val, block[full_type(ridx) * full_type(_n2 + 1) <= n_d_num ? idx_l(ridx * num) : idx_s(div(n_d_num, ridx))]);
            for (half_type i = pid;i < _pi_n2;++i) {
                const half_type p = _primes[i];
                full_type pp = full_type(p) * full_type(p);
                if (pp > n_d_num) break;
                unsigned e = 1;
                full_type t = div(n_d_num, p);
                auto it = fpp(p);
                do {
                    ++e;
                    t = div(t, p);
                    inc(ans, dfs_powerful_iterative(block, ridx, i + 1, num * pp, t, mul(val, it(pp, e)), fpp));
                } while ((pp *= p) <= n_d_num);
            }
            return ans;
        }

        template<typename OutR, typename Fpp>
        void collect_powerful(OutR&& block_out, half_type pid, full_type num, full_type n_d_num, value_type val, Fpp fpp) const {
            collect_powerful_iterative(block_out, pid, num, n_d_num, val, [fpp] (unsigned p) {
                return [fpp, p] (full_type pp, unsigned e) { return fpp(pp, p, e); };
            });
        }

        template<typename OutR, typename Fpp>
        void collect_powerful_iterative(OutR&& block_out, half_type pid, full_type num, full_type n_d_num, value_type val, Fpp fpp) const {
            inc(block_out[num <= _n2 ? idx_s(num) : idx_l(n_d_num)], val);
            for (half_type i = pid;i < _pi_n2;++i) {
                const half_type p = _primes[i];
                full_type pp = full_type(p) * full_type(p);
                if (pp > n_d_num) break;
                unsigned e = 1;
                full_type t = div(n_d_num, p);
                auto it = fpp(p);
                do {
                    ++e;
                    t = div(t, p);
                    collect_powerful_iterative(block_out, i + 1, num * pp, t, mul(val, it(pp, e)), fpp);
                } while ((pp *= p) <= n_d_num);
            }
        }

        template<typename R, typename PnR, typename OutR>
        void mul_block_pn(const R& block, PnR&& block_pn, OutR&& block_out) const {
            for (half_type i = 1;i <= _n2;++i) {
                const value_type val = block_pn[idx_s(i)];
                if (_ring.eq(val, _ring.zero())) continue;
                const full_type n_d_x = div(_n, i);
                const half_type bound1 = div(_n2, i), bound2 = floor(sqrt(n_d_x));
                value_type pre = _ring.zero(), cur = _ring.zero();
                for (half_type j = 1;j <= bound1;++j, pre = cur)
                    inc(block_out[idx_s(i * j)], val, sub(cur = block[idx_s(j)], pre));
                for (half_type j = bound1 + 1;j <= bound2;++j, pre = cur)
                    inc(block_out[idx_l(div(n_d_x, j))], val, sub(cur = block[idx_s(j)], pre));
                dec(block_out[idx_l(div(n_d_x, bound2 + 1))], val, pre);
            }
            for (half_type i = 1;i < _len;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = 1;i <= _n2;++i) {
                const value_type val = block_pn[idx_s(i)];
                if (_ring.eq(val, _ring.zero())) continue;
                const full_type n_d_x = div(_n, i);
                const half_type bound = floor(sqrt(n_d_x)), bound1 = div(n_d_x, bound + 1), bound2 = div(_r_n2, i);
                for (half_type j = bound1;j > bound2;--j)
                    inc(block_out[idx_l(j)], val, block[idx_s(div(n_d_x, j))]);
                for (half_type j = bound2;j > 0;--j)
                    inc(block_out[idx_l(j)], val, block[idx_l(i * j)]);
            }
            for (half_type i = _n2 + 1;i < _len;++i)
                inc(block_pn[i], block_pn[i - 1]);
            value_type pre = _ring.zero();
            for (half_type i = 1;i <= _r_n2;pre = block[idx_s(i++)]) {
                const value_type val = sub(block[idx_s(i)], pre);
                const half_type bound = div(_r_n2, i);
                for (half_type j = 1;j <= bound;++j)
                    inc(block_out[idx_l(j)], val, block_pn[idx_l(i * j)]);
            }
        }

        template<typename R, typename PnR, typename PairR, typename OutR>
        void mul_block_pn_sparse(const R& block, PnR&& block_pn, const PairR& pairs, half_type cnt, OutR&& block_out) const {
            for (half_type i = 1, r = cnt;i <= _n2;++i) {
                const value_type val = block_pn[idx_s(i)];
                if (_ring.eq(val, _ring.zero())) continue;
                const full_type n_d_x = div(_n, i);
                while (r > 0 && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_x) --r;
                mul_sparse_small(i, n_d_x, val, 0, r, pairs, block, block_out);
            }
            for (half_type i = 1;i < _len;++i)
                inc(block_out[i], block_out[i - 1]);
            for (half_type i = 1, r = cnt;i <= _n2;++i) {
                const value_type val = block_pn[idx_s(i)];
                if (_ring.eq(val, _ring.zero())) continue;
                const full_type n_d_x = div(_n, i);
                while (r > 0 && full_type(r - 1) * full_type(pairs[r - 1].first) > n_d_x) --r;
                mul_sparse_large(i, n_d_x, val, pairs[r].first, block, block_out);
            }
            for (half_type i = _n2 + 1;i < _len;++i)
                inc(block_pn[i], block_pn[i - 1]);
            for (half_type i = 0;i < cnt;++i) {
                const auto [y, fx] = pairs[i];
                for (half_type j = 1;j * y <= _r_n2;++j)
                    inc(block_out[idx_l(j)], fx, block_pn[idx_l(j * y)]);
            }
        }
    };

    MultiplicativeSum() : _primes{2, 3, 5} {}
    MultiplicativeSum(half_type n) : MultiplicativeSum() { prepare(n); }

    const std::vector<D>& divisors() const noexcept { return _divisors; }
    const std::vector<half_type>& primes() const noexcept { return _primes; }
    half_type pi(half_type n) const { return std::distance(_primes.begin(), std::upper_bound(_primes.begin(), _primes.end(), n)); }

    void prepare(half_type n) {
        if (n < _divisors.size()) return;
        _primes.pop_back();
        std::vector<bool> composite((n + 1) / 2 - 1);
        const auto sieve = [n, &composite] (half_type p) {
            if (full_type(p) * full_type(p) > n) return;
            for (half_type i = p * p;i <= n;i += 2 * p)
                composite[i / 2 - 1] = true;
        };
        for (half_type i = 1;i < _primes.size();++i)
            sieve(_primes[i]);
        for (half_type i = _primes.back() + 2;i <= n;i += 2) {
            if (composite[i / 2 - 1]) continue;
            _primes.push_back(i);
            sieve(i);
        }
        _primes.push_back(n + 1);
        for (half_type i = _divisors.size();i <= n;++i)
            _divisors.emplace_back(i + 1);
    }

    template<typename Ring, typename Reducer = ReducerAdapter<Ring>>
    solver_type<Ring, Reducer> solver(full_type n, Ring ring, Reducer reducer = {}) {
        const half_type bound = floor(sqrt(n + 0.5));
        prepare(bound);
        return solver_type<Ring, Reducer>(n, _divisors.data(), _primes.data(), pi(bound), ring, reducer);
    }

private:
    std::vector<D> _divisors;
    std::vector<half_type> _primes;
};

constexpr unsigned MOD = 469762049, INV2 = (MOD + 1) / 2, INV3 = (MOD + 1) / 3;

struct ModRing {
    using value_type = unsigned;
    struct MulExpr {
        unsigned _val1, _val2;
        uint64_t reduce() const noexcept { return uint64_t(_val1) * uint64_t(_val2); }
        operator unsigned() const noexcept { return reduce() % MOD; }
    };
    value_type zero() const { return 0; }
    value_type one() const { return 1; }
    value_type neg(value_type x) const { return x != 0 ? MOD - x : 0; }
    value_type add(value_type x, value_type y) const { const value_type t = x + y; return t < MOD ? t : t - MOD; }
    value_type sub(value_type x, value_type y) const { const value_type t = x - y; return t < MOD ? t : t + MOD; }
    value_type add(value_type x, MulExpr y) const { return (x + y.reduce()) % MOD; }
    value_type add(MulExpr x, value_type y) const { return (x.reduce() + y) % MOD; }
    value_type add(MulExpr x, MulExpr y) const { return (x.reduce() + y.reduce()) % MOD; }
    value_type sub(value_type x, MulExpr y) const { return (x + MulExpr{MOD - y._val1, y._val2}.reduce()) % MOD; }
    value_type sub(MulExpr x, value_type y) const { return (x.reduce() + MOD - y) % MOD; }
    MulExpr mul(value_type x, value_type y) const { return MulExpr{x, y}; }
    bool eq(value_type x, value_type y) const { return x == y; }
    bool neq(value_type x, value_type y) const { return x != y; }
};

template<typename T, size_t B>
struct PrefixTable {
    T masks[B + 1][B] = {};
    constexpr PrefixTable() {
        for (size_t i = 1;i <= B;++i)
            for (size_t j = 0;j < i;++j)
                masks[i][j] = static_cast<T>(-1);
    }
};

struct ModReducer64Avx2 {
    static constexpr unsigned ALIGNMENT_REQUIREMENT = 32, B = 8;
    unsigned zero() const noexcept { return 0; }
    unsigned add(unsigned x, unsigned y) const noexcept { const unsigned t = x + y; return t < MOD ? t : t - MOD; }
    FORCE_INLINE __m256i vec_zero() const noexcept { return _mm256_set1_epi32(0); }
    FORCE_INLINE __m256i vec_add(__m256i x, __m256i y) const noexcept {
        const __m256i ans = _mm256_add_epi32(x, y);
        return _mm256_min_epu32(ans, _mm256_sub_epi32(ans, _mm256_set1_epi32(MOD)));
    }
    #define load(ptr) _mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))
    template<unsigned W>
    FORCE_INLINE __m256i partial_reduce_bounded(const unsigned* ptr, unsigned len) const noexcept {
        static constexpr PrefixTable<unsigned, W> TABLE = {};
        __m256i ans = _mm256_set1_epi32(0);
        for (unsigned i = 0;i < W;i += B)
            ans = vec_add(ans, _mm256_and_si256(load(ptr + i), load(TABLE.masks[len] + i)));
        return ans;
    }
    FORCE_INLINE __m256i partial_reduce(const unsigned* ptr, unsigned len) const noexcept {
        static constexpr PrefixTable<unsigned, B> TABLE = {};
        __m256i ans = _mm256_and_si256(load(ptr + (len & -B)), load(TABLE.masks[len % B]));
        for (unsigned i = 0;i < len / B;++i, ptr += B)
            ans = vec_add(ans, load(ptr));
        return ans;
    }
    #undef load
    FORCE_INLINE unsigned final_reduce(__m256i x) const noexcept {
        const __m128i lo  = _mm256_castsi256_si128(x);
        const __m128i hi = _mm256_extracti128_si256(x, 1);
        const __m128i est_sum2 = _mm_add_epi32(lo, hi), sum2 = _mm_min_epu32(est_sum2, _mm_sub_epi32(est_sum2, _mm_set1_epi32(MOD)));
        return add(add(_mm_extract_epi32(sum2, 0), _mm_extract_epi32(sum2, 1)), add(_mm_extract_epi32(sum2, 2), _mm_extract_epi32(sum2, 3)));
    }
    FORCE_INLINE unsigned reduce(const unsigned* ptr, unsigned len) const noexcept {
        return final_reduce(partial_reduce(ptr, len));
    }
    template<unsigned W>
    FORCE_INLINE unsigned reduce_bounded(const unsigned* ptr, unsigned len) const noexcept {
        return final_reduce(partial_reduce_bounded<W>(ptr, len));
    }
};

ModRing ring;

MultiplicativeSum<> ms;

int main() {
    unsigned t;
    cin >> t;
    for (;t > 0;--t) {
        uint64_t n;
        unsigned a, b;
        cin >> n >> a >> b;
        if (n == 1) { cout << "1\n"; continue; }
        const auto s = ms.solver(n, ring, ModReducer64Avx2{});
        const auto div3 = [] (unsigned x) { return uint64_t(x) * INV3 % MOD; };
        vector<unsigned> buf(s.len());
        const auto id0 = buf.data(), id1 = buf.data() + s.len_l();
        s.sum_large_invertible(id0, div3, [] (uint64_t n) { return n % MOD; }, [] (unsigned p) { return 1; });
        s.sum_large_invertible(id1, div3, [] (uint64_t n) { return (n % MOD + 1) * (n % MOD) % MOD * INV2 % MOD; }, [] (unsigned p) { return p; });
        const auto g_l = id1;
        for (unsigned i = 0;i < s.len_l();++i)
            g_l[i] = ring.add(ring.mul(a, id0[i]), ring.mul(b, id1[i]));
        vector<unsigned> g(s.len());
        s.attach_nonlarge_square_free_invertible(g_l, g, div3, [=] (unsigned p) { return ring.add(a, ring.mul(b, p)); });
        const unsigned ans = s.mul_powerful_iterative_single(g, [=] (unsigned p) {
            unsigned pre = 0;
            return [=] (uint64_t pp, unsigned e) mutable {
                return pre = ring.sub(ring.add(ring.mul(a, e), ring.mul(b, p)), ring.mul(ring.add(a, ring.mul(b, p)), pre));
            };
        });
        cout << ans << "\n";
    }
    return 0;
}