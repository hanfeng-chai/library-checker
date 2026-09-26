#line 2 "1-Core\\01-template.hpp"

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

#define fi    first
#define se    second
#define pb    push_back
using uint = uint32_t;
using lng = int64_t;    using ulng = uint64_t;
using lll = __int128_t; using ulll = __uint128_t;
template<typename T> 
using indexed_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

constexpr int INF32 = 0x3f3f3f3f;
constexpr lng INF64 = 0x3f3f3f3f3f3f3f3f;

template<typename T> 
constexpr inline bool chmax(T &a, const T &b) { return a < b ? a = b, 1 : 0; }
template<typename T> 
constexpr inline bool chmin(T &a, const T &b) { return a > b ? a = b, 1 : 0; }
#line 3 "1-Core\\03-modint.hpp"

// T: O(1) or O(log(n)), M: O(1)
template<int MOD>
struct ModInt {
    static_assert(MOD > 0);
    static constexpr bool isPrime() {
        if (MOD <= 1) { return false; }
        if (MOD == 2 || MOD == 7 || MOD == 61) { return true; }
        if (!(MOD & 1)) { return false; }
        int s = __builtin_ctz(uint(MOD - 1)); lng d = lng(MOD - 1) >> s;
        for (lng a : {2, 7, 61}) {
            lng x = 1;
            for (lng b = d; b > 0; b >>= 1) {
                if (b & 1) { x = x * a % MOD; }
                a = a * a % MOD;}
            if (x == 1 || x == MOD - 1) { continue; }
            bool is_comp = true;
            for (int j = 1; j < s; j++) {
                x = x * x % MOD;
                if (x == MOD - 1) { is_comp = false; break; }
                if (x == 1) { return false; }}
            if (is_comp) { return false; }}
        return true;}
    static constexpr bool is_prime = ModInt::isPrime();

    int n;
    template<typename T = int>
    requires std::is_integral_v<T> || std::is_same_v<T, lll> || std::is_same_v<T, ulll>
    constexpr ModInt(T N = 0) {
        if constexpr (std::is_signed_v<T> || std::is_same_v<T, lll>) { n = N % MOD; n += (n < 0) * MOD; }
        else { n = N % MOD; }}
    
    explicit constexpr operator int() const { return n; }
    static constexpr int mod() { return MOD; }
    static constexpr ModInt init(int N) { ModInt res; res.n = N; return res; }
    template<typename T> 
    requires std::is_integral_v<T> || std::is_same_v<T, lll> || std::is_same_v<T, ulll>
    constexpr ModInt &operator=(T N) {
        if constexpr (std::is_signed_v<T> || std::is_same_v<T, lll>) { n = N % MOD; n += (n < 0) * MOD; }
        else { n = N % MOD; }
        return *this;}
    
    constexpr ModInt &operator++() { n++; n -= (n == MOD) * MOD; return *this; }
    constexpr ModInt &operator--() { n += (n == 0) * MOD; n--; return *this; }
    constexpr ModInt operator++(int) { ModInt res = *this; ++*this; return res; }
    constexpr ModInt operator--(int) { ModInt res = *this; --*this; return res; }
    constexpr ModInt &operator+=(ModInt o) { n += o.n - (n >= MOD - o.n) * MOD; return *this; }
    constexpr ModInt &operator-=(ModInt o) { n -= o.n - (n < o.n) * MOD; return *this; }
    constexpr ModInt &operator*=(ModInt o) { n = ulng(n) * uint(o.n) % uint(MOD); return *this; }
    constexpr ModInt &operator/=(ModInt o) { return *this *= inv(o); }

    constexpr ModInt operator+() const { return *this; }
    constexpr ModInt operator-() const { return init((n != 0) * (MOD - n)); }
    friend constexpr ModInt operator+(ModInt a, ModInt b) { a += b; return a; }
    friend constexpr ModInt operator-(ModInt a, ModInt b) { a -= b; return a; }
    friend constexpr ModInt operator*(ModInt a, ModInt b) { a *= b; return a; }
    friend constexpr ModInt operator/(ModInt a, ModInt b) { a /= b; return a; }

    // T: O(log(n))
    friend constexpr ModInt inv(ModInt a) {
        assert(a != 0);
        if constexpr (is_prime) { return pow(a, MOD - 2); }
        int n = a.n, b = MOD, x = 1, y = 0;
        while (b > 0) {
            int q = n / b;
            n = std::exchange(b, n - q * b);
            x = std::exchange(y, x - q * y);}
        assert(n == 1);
        return init(x + (x < 0) * MOD);}
    // T: O(log(n))
    friend constexpr ModInt pow(ModInt a, lng b) {
        ulng ub = b < 0 ? -ulng(b) : ulng(b);
        if (b < 0) { a = inv(a); }
        ModInt res = 1;
        for (; ub > 0; ub >>= 1, a *= a) { if (ub & 1) { res *= a; } }
        return res;}
    // T: O(log(n)) average
    friend constexpr ModInt sqrt(ModInt a) {
        assert(is_prime);
        if (a == 0 || a == 1 || MOD == 2) { return a; }
        if constexpr (MOD % 4 == 3) {
            ModInt res = pow(a, (lng(MOD) + 1) / 4);
            if (res * res != a) { return -1; }
            return init(min(res.n, MOD - res.n));}
        if (pow(a, (MOD - 1) / 2) != 1) { return -1; }
        ModInt b = 1;
        while (pow(b * b - a, (MOD - 1) / 2) == 1) { b++; }
        struct Node {
            ModInt x, y, w;
            constexpr Node(ModInt X = 0, ModInt Y = 0, ModInt W = 0) : x(X), y(Y), w(W) {}
            constexpr Node operator*(const Node &o) const { 
                return Node(x * o.x + y * o.y * w, x * o.y + y * o.x, w); }};
        Node res(1, 0, b * b - a), cur(b, 1, b * b - a);
        for (lng p = (lng(MOD) + 1) / 2; p > 0; p >>= 1, cur = cur * cur) {
            if (p & 1) { res = res * cur; }}
        return init(min(res.x.n, MOD - res.x.n));}

    explicit constexpr operator bool() const { return n != 0; }
    constexpr bool operator!() const { return n == 0; }
    friend constexpr auto operator<=>(const ModInt &a, const ModInt &b) = default;

    friend istream &operator>>(istream &is, ModInt &a) { lng b; if (is >> b) { a = b; } return is; }
    friend ostream &operator<<(ostream &os, ModInt a) { return os << a.n; }
};
using mint = ModInt<998'244'353>;
#line 3 "1-Core\\07-matrix.hpp"

#ifdef __AVX2__
#include <immintrin.h>
namespace FastMatMul {
    static constexpr int BASE = 64;
    
    struct Pool {
        uint *buf; uint *ptr;
        Pool(size_t sz) {
            buf = (uint*)(_mm_malloc(4 * sizeof(uint) * sz * sz, 32)); ptr = buf;}
        ~Pool() { _mm_free(buf); }
        
        uint *alloc(uint n) { n = 8 * ((n + 7) / 8); uint *res = ptr; ptr += n; return res; }
        void free(uint n) { ptr -= 8 * ((n + 7) / 8); }};

    inline __m256i redMont(__m256i a, __m256i b, __m256i vinv, __m256i vmod) {
        __m256i qa = _mm256_mul_epu32(_mm256_mul_epu32(a, vinv), vmod);
        __m256i qb = _mm256_mul_epu32(_mm256_mul_epu32(b, vinv), vmod);
        return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a, qa), 32),
                                  _mm256_add_epi64(b, qb), 0xAA);}
    inline __m256i mulMont(__m256i a, __m256i b, __m256i vinv, __m256i vmod) {
        return redMont(_mm256_mul_epu32(a, b),
                       _mm256_mul_epu32(_mm256_srli_epi64(a, 32), b), vinv, vmod);}
    inline __m256i shrink32(__m256i a, __m256i vmod) {
        return _mm256_min_epu32(a, _mm256_sub_epi32(a, vmod));}
    inline void naiveMont(const uint *a, const uint *b, uint *c, uint mod) {
        uint inv = mod;
        for (int i = 0; i < 5; i++) { inv *= 2 - mod * inv; }
        inv = -inv;
        uint r2 = uint(-ulng(mod) % mod);
        const __m256i vmod = _mm256_set1_epi32(mod), v2xmod = _mm256_set1_epi32(2 * mod);
        const __m256i vinv = _mm256_set1_epi32(inv), vr2 = _mm256_set1_epi32(r2);
        const __m256i vhi = _mm256_set1_epi64x(ulng(1) << 63);
        #define FM8(f) f(0) f(1) f(2) f(3) f(4) f(5) f(6) f(7)
        #define FM_INIT(p)   __m256i lo##p = _mm256_setzero_si256(), hi##p = _mm256_setzero_si256();
        #define FM_WORK(p)   __m256i va##p = _mm256_set1_epi32(a[BASE * (i + p) + l]); \
                             lo##p = _mm256_add_epi64(lo##p, _mm256_mul_epu32(va##p, vb)); \
                             hi##p = _mm256_add_epi64(hi##p, _mm256_mul_epu32(va##p, vbhi));
        #define FM_SHRINK(p) lo##p = _mm256_sub_epi64(lo##p, _mm256_min_epu32(v2xmod, _mm256_and_si256(vhi, lo##p))); \
                             hi##p = _mm256_sub_epi64(hi##p, _mm256_min_epu32(v2xmod, _mm256_and_si256(vhi, hi##p)));
        #define FM_STORE(p)  __m256i x##p = shrink32(shrink32(redMont(lo##p, hi##p, vinv, vmod), v2xmod), vmod); \
                             x##p = shrink32(mulMont(x##p, vr2, vinv, vmod), vmod); \
                             _mm256_store_si256((__m256i*)(c + BASE * (i + p) + j), x##p);
        for (int i = 0; i < BASE; i += 8) {
            for (int j = 0; j < BASE; j += 8) {
                FM8(FM_INIT)
                for (int k = 0; k < BASE; k += 8) {
                    for (int l = k; l < k + 8; l++) {
                        __m256i vb = _mm256_load_si256((const __m256i*)(b + BASE * l + j));
                        __m256i vbhi = _mm256_srli_epi64(vb, 32);
                        FM8(FM_WORK)}
                    FM8(FM_SHRINK)}
                FM8(FM_STORE)}}
        #undef FM_STORE
        #undef FM_SHRINK
        #undef FM_WORK
        #undef FM_INIT
        #undef FM8
    }

    inline __m256i shrink(__m256i a, __m256i vmod8) {
        __m256i msk = _mm256_cmpgt_epi64(_mm256_sub_epi64(a, vmod8), _mm256_setzero_si256());
        return _mm256_sub_epi64(a, _mm256_and_si256(msk, vmod8));}
    inline void naiveWide(const uint *a, const uint *b, uint *c, uint mod) {
        if (mod == 1) { std::fill_n(c, BASE * BASE, 0U); return; }
        ulng lim = 2 + 2 * (mod <= 1518500249U) + 4 * (mod <= 1073741823U);
        alignas(32) uint64_t bb[BASE * BASE];
        for (int i = 0; i < BASE * BASE; ++i) { bb[i] = b[i]; }
        alignas(32) uint64_t cc[BASE * BASE] = {0};
        alignas(32) __m256i tmp[256];
        __m256i *vb = (__m256i*)(bb); __m256i *vc = (__m256i*)(cc);
        const __m256i vlim = _mm256_set1_epi64x(lim * mod * mod);
        for (int j = 0; j < BASE; j += 32) {
            for (int k0 = 0; k0 < BASE; k0 += 32) {
                for (int k = k0; k < k0 + 32; ++k) {
                    __m256i *row = vb + ((BASE / 4) * k + (j / 4));
                    for (int x = 0; x < 8; ++x) { tmp[8 * (k - k0) + x] = _mm256_load_si256(row + x); }}
                for (int i = 0; i < BASE; ++i) {
                    __m256i *out = vc + ((BASE * i + j) / 4); __m256i t[8];
                    for (int x = 0; x < 8; ++x) { t[x] = _mm256_load_si256(out + x); }
                    const uint *ca = a + BASE * i + k0;
                    for (int k = 0; k < 32; ++k) {
                        __m256i ak = _mm256_set1_epi32(ca[k]);
                        #pragma GCC unroll 8
                        for (int x = 0; x < 8; ++x) {
                            t[x] = _mm256_add_epi64(t[x], _mm256_mul_epu32(ak, tmp[8 * k + x]));}
                        if ((k & (lim - 1)) == lim - 1) {
                            #pragma GCC unroll 8
                            for (int x = 0; x < 8; ++x) { t[x] = shrink(t[x], vlim); }}}
                    for (int x = 0; x < 8; ++x) { _mm256_store_si256(out + x, t[x]); }}}}
        ulng imod = -1ULL / mod + 1;
        for (int i = 0; i < BASE * BASE; ++i) {
            ulng q = ulng((ulll(cc[i]) * imod) >> 64), r = cc[i] - q * mod;
            if (mod <= r) { r += mod; }
            c[i] = uint(r);}}

    inline void addMat(const uint *a, const uint *b, uint *c, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod);
        for (int i = 0; i < n; i += 8) {
            __m256i s = _mm256_add_epi32(_mm256_load_si256((const __m256i*)(a + i)), 
                                         _mm256_load_si256((const __m256i*)(b + i)));
            _mm256_store_si256((__m256i*)(c + i), _mm256_min_epu32(s, _mm256_sub_epi32(s, vmod)));}}
    inline void addMatTo(uint *a, const uint *b, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod);
        for (int i = 0; i < n; i += 8) {
            __m256i s = _mm256_add_epi32(_mm256_load_si256((const __m256i*)(a + i)),
                                         _mm256_load_si256((const __m256i*)(b + i)));
            _mm256_store_si256((__m256i*)(a + i), _mm256_min_epu32(s, _mm256_sub_epi32(s, vmod)));}}
    inline void subMat(const uint *a, const uint *b, uint *c, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod);
        for (int i = 0; i < n; i += 8) {
            __m256i s = _mm256_sub_epi32(_mm256_load_si256((const __m256i*)(a + i)),
                                         _mm256_load_si256((const __m256i*)(b + i)));
            _mm256_store_si256((__m256i*)(c + i), _mm256_min_epu32(s, _mm256_add_epi32(s, vmod)));}}
    inline void subMatTo(uint *a, const uint *b, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod);
        for (int i = 0; i < n; i += 8) {
            __m256i s = _mm256_sub_epi32(_mm256_load_si256((const __m256i*)(a + i)), 
                                         _mm256_load_si256((const __m256i*)(b + i)));
            _mm256_store_si256((__m256i*)(a + i), _mm256_min_epu32(s, _mm256_add_epi32(s, vmod)));}}
    inline void packRec(uint *a, const uint *b, int n, int m) {
        if (n == BASE) {
            for (int i = 0; i < BASE; ++i) {
                std::memcpy(a + BASE * i, b + i * m, sizeof(uint) * BASE);}
            return;}
        int hf = n >> 1, sz = hf * hf;
        packRec(a,          b,               hf, m);
        packRec(a + sz,     b + hf,          hf, m);
        packRec(a + 2 * sz, b + hf * m,      hf, m);
        packRec(a + 3 * sz, b + hf * m + hf, hf, m);}
    inline void unpackRec(uint *a, const uint *b, int n, int m) {
        if (n == BASE) {
            for (int i = 0; i < BASE; ++i) {
                std::memcpy(a + i * m, b + BASE * i, sizeof(uint) * BASE);}
            return;}
        int hf = n >> 1, sz = hf * hf;
        unpackRec(a,               b,          hf, m);
        unpackRec(a + hf,          b + sz,     hf, m);
        unpackRec(a + hf * m,      b + 2 * sz, hf, m);
        unpackRec(a + hf * m + hf, b + 3 * sz, hf, m);}
    inline void strassen(const uint *a, const uint *b, uint *c, int n, uint mod, Pool &pool) {
        if (n == BASE) {
            if ((mod & 1) && mod <= 1073741823U) { naiveMont(a, b, c, mod); }
            else { naiveWide(a, b, c, mod); }
            return;}
        std::memset(c, 0, sizeof(uint) * n * n);
        int hf = n >> 1, sz = hf * hf;
        const uint *a0 = a, *a1 = a + sz, *a2 = a + 2 * sz, *a3 = a + 3 * sz;
        const uint *b0 = b, *b1 = b + sz, *b2 = b + 2 * sz, *b3 = b + 3 * sz;
        uint *c0 = c, *c1 = c + sz, *c2 = c + 2 * sz, *c3 = c + 3 * sz;
        uint *tmp_a = pool.alloc(sz), *tmp_b = pool.alloc(sz), *p = pool.alloc(sz);
        addMat(a0, a3, tmp_a, sz, mod); addMat(b0, b3, tmp_b, sz, mod);
        strassen(tmp_a, tmp_b, p, hf, mod, pool);
        std::memcpy(c0, p, sizeof(uint) * sz); std::memcpy(c3, p, sizeof(uint) * sz);
        addMat(a2, a3, tmp_a, sz, mod);
        strassen(tmp_a, b0, p, hf, mod, pool);
        addMatTo(c2, p, sz, mod); subMatTo(c3, p, sz, mod);
        subMat(b1, b3, tmp_b, sz, mod);
        strassen(a0, tmp_b, p, hf, mod, pool);
        addMatTo(c1, p, sz, mod); addMatTo(c3, p, sz, mod);
        subMat(b2, b0, tmp_b, sz, mod);
        strassen(a3, tmp_b, p, hf, mod, pool);
        addMatTo(c0, p, sz, mod); addMatTo(c2, p, sz, mod);
        addMat(a0, a1, tmp_a, sz, mod);
        strassen(tmp_a, b3, p, hf, mod, pool);
        addMatTo(c1, p, sz, mod); subMatTo(c0, p, sz, mod);
        subMat(a2, a0, tmp_a, sz, mod); addMat(b0, b1, tmp_b, sz, mod);
        strassen(tmp_a, tmp_b, p, hf, mod, pool);
        addMatTo(c3, p, sz, mod);
        subMat(a1, a3, tmp_a, sz, mod); addMat(b2, b3, tmp_b, sz, mod);
        strassen(tmp_a, tmp_b, p, hf, mod, pool);
        addMatTo(c0, p, sz, mod);
        pool.free(3 * sz);}
}

namespace FastGaussian {
    template<typename T>
    constexpr uint getMod() { return uint(int(T(-1))) + 1; }
    template<typename T>
    constexpr uint getModInv() {
        uint mod = getMod<T>(), imod = mod;
        for (int i = 0; i < 5; ++i) { imod *= 2 - mod * imod; }
        return -imod;}
    
    template<typename T>
    inline void addRow(T *a, const T *b, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod); 
        int i = 0;
        for (; i + 7 < n; i += 8) {
            __m256i s = _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)(a + i)), 
                                         _mm256_loadu_si256((const __m256i*)(b + i)));
            _mm256_storeu_si256((__m256i*)(a + i), _mm256_min_epu32(s, _mm256_sub_epi32(s, vmod)));}
        for (; i < n; ++i) { a[i] += b[i]; }}
    template<typename T>
    inline void subRow(T *__restrict a, const T *__restrict b, int n, uint mod) {
        __m256i vmod = _mm256_set1_epi32(mod); 
        int i = 0;
        for (; i + 7 < n; i += 8) {
            __m256i s = _mm256_sub_epi32(_mm256_loadu_si256((const __m256i*)(a + i)), 
                                         _mm256_loadu_si256((const __m256i*)(b + i)));
            _mm256_storeu_si256((__m256i*)(a + i), _mm256_min_epu32(s, _mm256_add_epi32(s, vmod)));}
        for (; i < n; ++i) { a[i] -= b[i]; }}
    template<typename T>
    inline void mulRow(T *__restrict t, T a, int m) {
        if (a == T(0)) { std::fill_n(t, m, T(0)); return; }
        if (a == T(1)) { return; }
        if constexpr (requires { T::is_prime; } && sizeof(T) == 4) {
            const uint MOD = getMod<T>();
            if (MOD % 2 == 0 || m < 8) {
                #pragma GCC ivdep
                #pragma GCC unroll 8
                for (int j = 0; j < m; ++j) { t[j] *= a; }
                return;}
            const uint IMOD = getModInv<T>();
            uint mul = uint(int(a));
            __m256i vmod32 = _mm256_set1_epi32(MOD);
            __m256i vmod64 = _mm256_set1_epi64x(MOD);
            __m256i vimod = _mm256_set1_epi32(IMOD);
            __m256i vb = _mm256_set1_epi64x(uint((1ULL << 32) % MOD * mul % MOD));
            int j = 0;
            for (; j + 7 < m; j += 8) {
                __m256i va = _mm256_loadu_si256((const __m256i*)(t + j));
                __m256i pe = _mm256_mul_epu32(va, vb);
                __m256i po = _mm256_mul_epu32(_mm256_srli_epi64(va, 32), vb);
                __m256i qe = _mm256_mul_epu32(_mm256_mullo_epi32(pe, vimod), vmod64);
                __m256i qo = _mm256_mul_epu32(_mm256_mullo_epi32(po, vimod), vmod64);
                __m256i re = _mm256_srli_epi64(_mm256_add_epi64(pe, qe), 32);
                __m256i ro = _mm256_srli_epi64(_mm256_add_epi64(po, qo), 32);
                __m256i res = _mm256_blend_epi32(re, _mm256_slli_epi64(ro, 32), 0xAA);
                res = _mm256_min_epu32(res, _mm256_sub_epi32(res, vmod32));
                _mm256_storeu_si256((__m256i*)(t + j), res);}
            for (; j < m; ++j) { t[j] *= a; }}
        else if constexpr (std::is_floating_point_v<T>) {
            int j = 0;
            if constexpr (sizeof(T) == 8) {
                __m256d va = _mm256_set1_pd(a);
                for (; j + 3 < m; j += 4) {
                    _mm256_storeu_pd(t + j, _mm256_mul_pd(_mm256_loadu_pd(t + j), va));}}
            else if constexpr (sizeof(T) == 4) {
                __m256 va = _mm256_set1_ps(a);
                for (; j + 7 < m; j += 8) {
                    _mm256_storeu_ps(t + j, _mm256_mul_ps(_mm256_loadu_ps(t + j), va));}}
            for (; j < m; ++j) { t[j] *= a; }}}
    template<typename T>
    inline void elimRow(T *__restrict t, const T *__restrict s, T a, int m) {
        if (a == T(0)) { return; }
        if constexpr (requires { T::is_prime; } && sizeof(T) == 4) {
            const uint MOD = getMod<T>();
            if (MOD % 2 == 0 || m < 8) { 
                #pragma GCC ivdep
                #pragma GCC unroll 8
                for (int j = 0; j < m; ++j) { t[j] -= s[j] * a; }
                return;}
            const uint IMOD = getModInv<T>();
            uint mul = MOD - uint(int(a));
            __m256i vmod32 = _mm256_set1_epi32(MOD);
            __m256i vmod64 = _mm256_set1_epi64x(MOD);
            __m256i vimod = _mm256_set1_epi32(IMOD);
            __m256i vb = _mm256_set1_epi64x(uint((1ULL << 32) % MOD * mul % MOD));
            int j = 0;
            for (; j + 7 < m; j += 8) {
                __m256i va = _mm256_loadu_si256((const __m256i*)(s + j));
                __m256i pe = _mm256_mul_epu32(va, vb);
                __m256i po = _mm256_mul_epu32(_mm256_srli_epi64(va, 32), vb);
                __m256i qe = _mm256_mul_epu32(_mm256_mullo_epi32(pe, vimod), vmod64);
                __m256i qo = _mm256_mul_epu32(_mm256_mullo_epi32(po, vimod), vmod64);
                __m256i re = _mm256_srli_epi64(_mm256_add_epi64(pe, qe), 32);
                __m256i ro = _mm256_srli_epi64(_mm256_add_epi64(po, qo), 32);
                __m256i res = _mm256_blend_epi32(re, _mm256_slli_epi64(ro, 32), 0xAA);
                res = _mm256_min_epu32(res, _mm256_sub_epi32(res, vmod32));
                __m256i vt = _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)(t + j)), res);
                vt = _mm256_min_epu32(vt, _mm256_sub_epi32(vt, vmod32));
                _mm256_storeu_si256((__m256i*)(t + j), vt);}
            for (; j < m; ++j) { t[j] -= s[j] * a; }}
        else if constexpr (std::is_floating_point_v<T>) {
            int j = 0;
            if constexpr (sizeof(T) == 8) { 
                __m256d va = _mm256_set1_pd(a);
                for (; j + 3 < m; j += 4) {
                    __m256d vt = _mm256_loadu_pd(t + j);
                    __m256d vs = _mm256_loadu_pd(s + j);
                    #ifdef __FMA__
                    _mm256_storeu_pd(t + j, _mm256_fnmadd_pd(vs, va, vt));
                    #else
                    _mm256_storeu_pd(t + j, _mm256_sub_pd(vt, _mm256_mul_pd(vs, va)));
                    #endif
                }} 
            else if constexpr (sizeof(T) == 4) { 
                __m256 va = _mm256_set1_ps(a);
                for (; j + 7 < m; j += 8) {
                    __m256 vt = _mm256_loadu_ps(t + j);
                    __m256 vs = _mm256_loadu_ps(s + j);
                    #ifdef __FMA__
                    _mm256_storeu_ps(t + j, _mm256_fnmadd_ps(vs, va, vt));
                    #else
                    _mm256_storeu_ps(t + j, _mm256_sub_ps(vt, _mm256_mul_ps(vs, va)));
                    #endif
                }}
            for (; j < m; ++j) { t[j] -= s[j] * a; }}}
}
#endif

// T: O(n^2), M: O(n^2)
template<typename T>
struct Matrix {
    static constexpr bool is_mint = requires { T::is_prime; };
    static constexpr bool is_field = [] {
        if constexpr (requires { std::bool_constant<T::is_prime>{}; }) {
            return T::is_prime;}
        else { return is_mint || std::is_floating_point_v<T>; }}();
    static constexpr bool is_mint32 = is_mint && sizeof(T) == 4 &&
        requires(T x) { int(x); { x.n } -> std::same_as<int&>; };
    // T: O(1)
    static void requireField() { if constexpr (is_mint) { assert(T::is_prime); } }

    int n, m;
    vector<T> v;
    Matrix(int N = 0, int M = 0) : n(N), m(M) {
        assert(n >= 0 && m >= 0); v.resize(n * m);}
    Matrix(int N, int M, vector<T> a) : n(N), m(M), v(std::move(a)) {
        assert(n >= 0 && m >= 0 && v.size() == n * m);}
    // T: O(n)
    Matrix(vector<T> a, bool axis = 0) :
        n(axis ? int(a.size()) : 1), m(axis ? 1 : int(a.size())), v(std::move(a)) {}
    Matrix(const vector<vector<T>> &a) :
        n(int(a.size())), m(a.empty() ? 0 : int(a[0].size())), v(n * m) {
        for (int i = 0; i < n; i++) { 
            assert(a[i].size() == m);
            std::copy(a[i].begin(), a[i].end(), v.begin() + i * m);}}
    static Matrix eye(int N, int M) {
        Matrix res(N, M);
        for (int i = 0; i < min(N, M); i++) { res[i][i] = T(1); }      
        return res;}
    
    // T: O(1)
    [[gnu::noinline]] static T *emptyRow() { static T empty{}; return &empty; }
    // T: O(1)
    T *operator[](int i) { return m ? v.data() + i * m : emptyRow(); }
    // T: O(1)
    const T *operator[](int i) const { return m ? v.data() + i * m : emptyRow(); }
    // T: O(1)
    int size() const { return n; }
    // T: O(1)
    int area() const { return n * m; }

    Matrix &operator+=(const Matrix &o) {
        assert(n == o.n && m == o.m);
        #ifdef __AVX2__
        if constexpr (is_mint32) {
            FastGaussian::addRow(v.data(), o.v.data(), n * m, uint(int(T(-1))) + 1);
            return *this;}
        #endif
        for (int i = 0; i < n * m; i++) { v[i] += o.v[i]; }
        return *this;}
    Matrix &operator-=(const Matrix &o) {
        assert(n == o.n && m == o.m);
        if (this == &o) { std::fill(v.begin(), v.end(), T(0)); return *this; }
        #ifdef __AVX2__
        if constexpr (is_mint32) {
            FastGaussian::subRow(v.data(), o.v.data(), n * m, uint(int(T(-1))) + 1);
            return *this;}
        #endif
        for (int i = 0; i < n * m; i++) { v[i] -= o.v[i]; }
        return *this;}
    Matrix &operator*=(T o) {
        #ifdef __AVX2__
        if constexpr (is_mint32 || std::is_floating_point_v<T>) {
            FastGaussian::mulRow(v.data(), o, n * m); return *this;}
        #endif
        for (int i = 0; i < n * m; i++) { v[i] *= o; }
        return *this;}
    Matrix &operator*=(const vector<T> &o) { return *this = *this * o; }
    // T: O(n^3)
    Matrix &operator*=(const Matrix &o) { return *this = *this * o; }
    Matrix &operator/=(T o) {
        if constexpr (is_mint || std::is_floating_point_v<T>) { return *this *= T(1) / o; }
        for (int i = 0; i < n * m; i++) { v[i] /= o; }
        return *this;}
    // T: O(n^3)
    Matrix &operator/=(const Matrix &o) requires is_field { return *this = *this / o; }
    Matrix &operator%=(T o) {
        for (int i = 0; i < n * m; i++) { v[i] %= o; }
        return *this;}

    Matrix operator+() const { return *this; }
    Matrix operator-() const {
        Matrix res(n, m);
        for (int i = 0; i < n * m; i++) { res.v[i] = -v[i]; }
        return res;}
    friend Matrix operator+(Matrix a, const Matrix &b) { a += b; return a; }
    friend Matrix operator+(const Matrix &a, Matrix &&b) { b += a; return std::move(b); }
    friend Matrix operator-(Matrix a, const Matrix &b) { a -= b; return a; }
    friend Matrix operator-(const Matrix &a, Matrix &&b) {
        assert(a.n == b.n && a.m == b.m);
        for (int i = 0; i < a.n * a.m; i++) { b.v[i] = a.v[i] - b.v[i]; }
        return std::move(b);}
    friend Matrix operator*(Matrix a, T b) { a *= b; return a; }
    friend Matrix operator*(T b, Matrix a) { a *= b; return a; }
    // T: O(n^2)
    friend Matrix operator*(const vector<T> &a, const Matrix &b) {
        assert(a.size() == b.n);
        Matrix res(1, b.m);
        if constexpr (is_mint) {
            vector<ulll> sum(b.m);
            for (int i = 0; i < b.n; i++) {
                if (a[i] == T(0)) { continue; }
                for (int j = 0; j < b.m; j++) { sum[j] += ulll(int(a[i])) * int(b[i][j]); }}
            uint mod = uint(int(T(-1))) + 1;
            for (int j = 0; j < b.m; j++) { res[0][j] = T(sum[j] % mod); }}
        else {
            for (int i = 0; i < b.n; i++) {
                if (a[i] == T(0)) { continue; }
                for (int j = 0; j < b.m; j++) { res[0][j] += a[i] * b[i][j]; }}}
        return res;}
    // T: O(n^2)
    friend Matrix operator*(const Matrix &a, const vector<T> &b) {
        assert(a.m == b.size());
        Matrix res(a.n, 1);
        if constexpr (is_mint) {
            uint mod = uint(int(T(-1))) + 1;
            for (int i = 0; i < a.n; i++) {
                ulll sum = 0;
                for (int j = 0; j < a.m; j++) { sum += ulll(int(a[i][j])) * int(b[j]); }
                res[i][0] = T(sum % mod);}}
        else {
            for (int i = 0; i < a.n; i++) {
                for (int j = 0; j < a.m; j++) { res[i][0] += a[i][j] * b[j]; }}}
        return res;}
    // T: O(n^3)
    friend Matrix operator*(const Matrix &a, const Matrix &b) {
        assert(a.m == b.n);
        Matrix res(a.n, b.m);
        #ifdef __AVX2__
        if constexpr (is_mint32) {
            const uint MOD = uint(int(T(-1))) + 1;
            int mx = max({a.n, a.m, b.m}), sz = FastMatMul::BASE;
            while (sz < mx) { sz <<= 1; }
            long double fast = 1.L * FastMatMul::BASE * FastMatMul::BASE * FastMatMul::BASE;
            for (int i = FastMatMul::BASE; i < sz; i <<= 1) { fast *= 7; }
            bool use_fast = MOD > 1 && (MOD != 2 || sz >= 256) && mx >= FastMatMul::BASE &&
                            (7.L - (MOD == 2) * 5.L) * a.n * a.m * b.m >= fast;
            if (use_fast) {
                int cnt = 0;
                for (const T &x : a.v) { cnt += int(x) != 0; }
                use_fast = (7.L - (MOD == 2) * 5.L) * cnt * b.m >= fast;}
            if (use_fast) {
                uint *rm = (uint*)(_mm_malloc(3 * sizeof(uint) * sz * sz, 32));
                uint *a_rm = rm, *b_rm = rm + sz * sz, *c_rm = rm + 2 * sz * sz;
                std::memset(a_rm, 0, sizeof(uint) * sz * sz);
                std::memset(b_rm, 0, sizeof(uint) * sz * sz);
                for (int i = 0; i < a.n; ++i) { std::memcpy(a_rm + i * sz, a[i], sizeof(uint) * a.m); }
                for (int i = 0; i < b.n; ++i) { std::memcpy(b_rm + i * sz, b[i], sizeof(uint) * b.m); }
                if (sz == FastMatMul::BASE) {
                    if ((MOD & 1) && MOD <= 1073741823U) { FastMatMul::naiveMont(a_rm, b_rm, c_rm, MOD); }
                    else { FastMatMul::naiveWide(a_rm, b_rm, c_rm, MOD); }}
                else {
                    FastMatMul::Pool pool(sz);
                    uint *a_rc = pool.alloc(sz * sz), *b_rc = pool.alloc(sz * sz), *c_rc = pool.alloc(sz * sz);
                    FastMatMul::packRec(a_rc, a_rm, sz, sz);
                    FastMatMul::packRec(b_rc, b_rm, sz, sz);
                    FastMatMul::strassen(a_rc, b_rc, c_rc, sz, MOD, pool);
                    FastMatMul::unpackRec(c_rm, c_rc, sz, sz);}
                for (int i = 0; i < res.n; ++i) {
                    std::memcpy(static_cast<void*>(res[i]), c_rm + i * sz, sizeof(uint) * res.m);}
                _mm_free(rm);
                return res;}}
        #endif
        constexpr int BASE = 64; 
        for (int i0 = 0; i0 < a.n; i0 += BASE) {
            for (int k0 = 0; k0 < a.m; k0 += BASE) {
                for (int j0 = 0; j0 < b.m; j0 += BASE) {
        int i_mx = min(i0 + BASE, a.n);
        int k_mx = min(k0 + BASE, a.m);
        int j_mx = min(j0 + BASE, b.m);
        for (int i = i0; i < i_mx; i++) {
            T *res_i = res[i];
            for (int k = k0; k < k_mx; k++) {
                if (a[i][k] == T(0)) { continue; }
                T a_ik = a[i][k]; const T *b_k = b[k];
                #pragma GCC ivdep
                for (int j = j0; j < j_mx; j++) { res_i[j] += a_ik * b_k[j]; }}}}}}
        return res;}
    friend Matrix operator/(Matrix a, T b) { a /= b; return a; }
    // T: O(n^3)
    friend Matrix operator/(const Matrix &a, const Matrix &b) requires is_field {
        assert(a.m == b.n);
        Matrix b_inv = inv(b);
        if (b.n && b_inv.n == 0) { return Matrix(); }
        return a * b_inv;}
    friend Matrix operator%(Matrix a, T b) { a %= b; return a; }
    
    friend Matrix concat(const Matrix &a, const Matrix &b, bool axis = 0) {
        Matrix res;
        if (axis == 0) {
            assert(a.n == b.n);
            res = Matrix(a.n, a.m + b.m);
            for (int i = 0; i < a.n; i++) {
                std::copy_n(a[i], a.m, res[i]);
                std::copy_n(b[i], b.m, res[i] + a.m);}}
        else {
            assert(a.m == b.m);
            res = Matrix(a.n + b.n, a.m);
            std::copy(a.v.begin(), a.v.end(), res.v.begin());
            std::copy(b.v.begin(), b.v.end(), res.v.begin() + a.v.size());}
        return res;}
    // T: O(1)
    friend Matrix cross(const Matrix &a, const Matrix &b, bool axis = 0) {
        assert((a.n == 1 || a.m == 1) && max(a.n, a.m) == 3);
        assert((b.n == 1 || b.m == 1) && max(b.n, b.m) == 3);
        T x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
        T y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
        T z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
        return Matrix(vector<T>{x, y, z}, axis);}
    // T: O(n^3) or O(n^3 * log(n))
    friend T det(Matrix a) {
        assert(a.n == a.m);
        vector<T*> rows(a.n);
        for (int i = 0; i < a.n; i++) { rows[i] = a[i]; }
        bool chk = !std::is_floating_point_v<T>;
        if constexpr (is_mint) { chk = !T::is_prime; }
        if constexpr (requires { std::declval<T>().is_inf; }) { chk = true; }
        if (chk) {
            T res = 1;
            for (int i = 0; i < a.n; i++) {
                int piv = i;
                while (piv < a.n && isNil(rows[piv][i])) { piv++; }
                if (piv == a.n) { return T(0); }
                if (piv != i) { std::swap(rows[i], rows[piv]); res = -res; }
                for (int k = i + 1; k < a.n; k++) {
                    while (!isNil(rows[k][i])) {
                        T q; T *a_i = rows[i]; T *a_k = rows[k];
                        if constexpr (is_mint) { q = T(a_i[i].n / a_k[i].n); }
                        else { q = a_i[i] / a_k[i]; }
                        #ifdef __AVX2__
                        if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                            FastGaussian::elimRow(a_i + i, a_k + i, q, a.n - i);} 
                        else 
                        #endif 
                        {   for (int j = i; j < a.n; j++) { a_i[j] -= a_k[j] * q; } }
                        std::swap(rows[i], rows[k]);
                        res = -res;}}
                res *= rows[i][i];}
            return res;}
        T res = 1;
        for (int i = 0; i < a.n; i++) {
            int piv = i;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = i + 1; p < a.n; p++) {
                    if (abs(rows[p][i]) > abs(rows[piv][i])) { piv = p; }}
                if (isNil(rows[piv][i])) { piv = a.n; }}
            else { while (piv < a.n && isNil(rows[piv][i])) { piv++; } }
            if (piv == a.n) { return T(0); }
            if (i != piv) { std::swap(rows[i], rows[piv]); res = -res; }
            res *= rows[i][i]; T d = T(1) / rows[i][i];
            for (int k = i + 1; k < a.n; k++) {
                if (isNil(rows[k][i])) { continue; }
                T a_ki = rows[k][i] * d;
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(rows[k] + i + 1, rows[i] + i + 1, a_ki, a.n - i - 1);}
                else
                #endif 
                {   for (int j = i + 1; j < a.n; j++) { rows[k][j] -= rows[i][j] * a_ki; } }}}
        return res;}
    // T: O(n)
    friend T dot(const Matrix &a, const Matrix &b) {
        assert((a.n == 1 || a.m == 1) && (b.n == 1 || b.m == 1));
        assert(a.area() == b.area());
        if constexpr (is_mint) {
            ulll res = 0;
            for (int i = 0; i < a.area(); i++) { res += ulll(int(a.v[i])) * int(b.v[i]); }
            return T(res % (uint(int(T(-1))) + 1));}
        else {
            T res = 0;
            for (int i = 0; i < a.area(); i++) { res += a.v[i] * b.v[i]; }
            return res;}}
    // T: O(n^3)
    friend Matrix inv(Matrix a) requires is_field { 
        requireField(); assert(a.n == a.m);
        int n = a.n; Matrix b = eye(n, n);
        vector<T*> ar(n), br(n);
        for (int i = 0; i < n; i++) { ar[i] = a[i]; br[i] = b[i]; }
        for (int c = 0; c < n; c++) {
            int piv = c;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = c + 1; p < n; p++) {
                    if (abs(ar[p][c]) > abs(ar[piv][c])) { piv = p; }}
                if (isNil(ar[piv][c])) { piv = n; }}
            else { while (piv < n && isNil(ar[piv][c])) { piv++; } }
            if (piv == n) { return Matrix(); }
            std::swap(ar[c], ar[piv]);
            std::swap(br[c], br[piv]);
            T d = T(1) / ar[c][c]; ar[c][c] = T(1);
            #ifdef __AVX2__
            if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                FastGaussian::mulRow(ar[c] + c + 1, d, n - c - 1);
                FastGaussian::mulRow(br[c], d, n);}
            else
            #endif
            {   for (int j = c + 1; j < n; j++) { ar[c][j] *= d; }
                for (int j = 0; j < n; j++) { br[c][j] *= d; }}
            for (int i = c + 1; i < n; i++) {
                if (isNil(ar[i][c])) { continue; }
                T q = ar[i][c]; ar[i][c] = T(0);
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(ar[i] + c + 1, ar[c] + c + 1, q, n - c - 1);
                    FastGaussian::elimRow(br[i], br[c], q, n);}
                else
                #endif
                {   for (int j = c + 1; j < n; j++) { ar[i][j] -= ar[c][j] * q; }
                    for (int j = 0; j < n; j++) { br[i][j] -= br[c][j] * q; }}}}
        for (int c = n - 1; c >= 0; c--) {
            for (int i = 0; i < c; i++) {
                if (isNil(ar[i][c])) { continue; }
                T q = ar[i][c];
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(br[i], br[c], q, n);}
                else
                #endif
                {   for (int j = 0; j < n; j++) { br[i][j] -= br[c][j] * q; }}}}
        Matrix res(n, n);
        for (int i = 0; i < n; i++) { std::copy_n(br[i], n, res[i]); }
        return res;}
    // T: O(n^3 * log(n))
    friend Matrix pow(const Matrix &a, lng b) {
        assert(a.n == a.m && b >= 0);
        if (b == 0) { return eye(a.n, a.n); }
        ulng ub = b;
        auto getWin = [&](int i, int w) {
            int l = max(i - w + 1, 0); l += std::countr_zero(ub >> l);
            int p = int((ub >> l) & ((ulng(1) << (i - l + 1)) - 1)) >> 1;
            return pair(l, p);};
        int hi = std::bit_width(ub) - 1, bw = 1;
        int L = hi + std::popcount(ub) - 1, P = 0;
        for (int w = 2; w <= 5; w++) {
            auto [l, p] = getWin(hi, w);
            int cl = l, cp = p;
            for (int i = l - 1; i >= 0;) {
                if (!((ub >> i) & 1)) { i--; continue; }
                std::tie(l, p) = getWin(i, w);
                cl++; cp = max(cp, p); i = l - 1;}
            cl += (cp > 0) * (cp + 1);
            if (cl < L) { bw = w; L = cl; P = cp; }}
        if (bw == 1) {
            Matrix res = a;
            for (int i = hi - 1; i >= 0; i--) {
                res *= res;
                if ((ub >> i) & 1) { res *= a; }}
            return res;}
        array<Matrix, 16> odd{a};
        Matrix a_sq = a * a;
        for (int i = 1; i <= P; i++) { odd[i] = odd[i - 1] * a_sq; }
        auto [l, p] = getWin(hi, bw);
        Matrix res = odd[p];
        for (int i = l - 1; i >= 0;) {
            if (!((ub >> i) & 1)) { res *= res; i--; continue; }
            std::tie(l, p) = getWin(i, bw);
            for (int j = l; j <= i; j++) { res *= res; }
            res *= odd[p]; i = l - 1;}
        return res;}
    // T: O(n^3)
    friend Matrix solveLin(Matrix a, Matrix b, Matrix *basis = nullptr) requires is_field {
        requireField(); assert(a.n == b.n);
        if (basis) { *basis = Matrix(); }
        int r = 0; vector<int> pivs(a.m, -1);
        for (int c = 0; r < a.n && c < a.m; c++) {
            int piv = r;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = r + 1; p < a.n; p++) {
                    if (abs(a[p][c]) > abs(a[piv][c])) { piv = p; }}
                if (isNil(a[piv][c])) { piv = a.n; }} 
            else { while (piv < a.n && isNil(a[piv][c])) { piv++; } }
            if (piv == a.n) { continue; }
            if (r != piv) {
                std::swap_ranges(a[r], a[r] + a.m, a[piv]);
                std::swap_ranges(b[r], b[r] + b.m, b[piv]);}
            T d = T(1) / a[r][c]; a[r][c] = T(1);
            #ifdef __AVX2__
            if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                FastGaussian::mulRow(a[r] + c + 1, d, a.m - c - 1);
                FastGaussian::mulRow(b[r], d, b.m);}
            else
            #endif
            {   for (int j = c + 1; j < a.m; j++) { a[r][j] *= d; }
                for (int j = 0; j < b.m; j++) { b[r][j] *= d; }}
            for (int i = 0; i < a.n; i++) {
                if (i == r || isNil(a[i][c])) { continue; }
                T a_ic = a[i][c]; a[i][c] = T(0);
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(a[i] + c + 1, a[r] + c + 1, a_ic, a.m - c - 1);
                    FastGaussian::elimRow(b[i], b[r], a_ic, b.m);} 
                else
                #endif 
                {   for (int j = c + 1; j < a.m; j++) { a[i][j] -= a[r][j] * a_ic; }
                    for (int j = 0; j < b.m; j++) { b[i][j] -= b[r][j] * a_ic; }}}
            pivs[c] = r++;}
        for (int i = r; i < a.n; i++) {
            for (int j = 0; j < b.m; j++) {
                if (!isNil(b[i][j])) { return Matrix(); }}}
        if (basis) {
            *basis = Matrix(a.m - r, a.m);
            int i = 0;
            for (int c = 0; c < a.m; c++) {
                if (pivs[c] != -1) { continue; }
                (*basis)[i][c] = T(1);
                for (int j = 0; j < a.m; j++) {
                    if (pivs[j] != -1) { (*basis)[i][j] = -a[pivs[j]][c]; }}
                i++;}}
        Matrix res(a.m, b.m);
        for (int c = 0; c < a.m; c++) {
            if (pivs[c] != -1) { std::copy_n(b[pivs[c]], b.m, res[c]); }}
        return res;}
    // T: O(n^3)
    friend pair<Matrix, Matrix> solveSpace(Matrix a, Matrix b) requires is_field {
        Matrix basis;
        Matrix res = solveLin(std::move(a), std::move(b), &basis);
        return {std::move(res), std::move(basis)};}
    // T: O(n)
    friend T trc(const Matrix &a) {
        assert(a.n == a.m);
        T res = 0;
        for (int i = 0; i < a.n; i++) { res += a[i][i]; }
        return res;}
    // T: O(n^2)
    friend Matrix trp(const Matrix &a) {
        Matrix res(a.m, a.n);
        constexpr int BASE = 16;
        if (min(a.n, a.m) < 64) {
            for (int i = 0; i < a.m; i++) {
                for (int j = 0; j < a.n; j++) { res[i][j] = a[j][i]; }}}
        else {
            for (int i0 = 0; i0 < a.n; i0 += BASE) {
                for (int j0 = 0; j0 < a.m; j0 += BASE) {
                    int i_mx = min(i0 + BASE, a.n), j_mx = min(j0 + BASE, a.m);
                    for (int j = j0; j < j_mx; j++) {
                        for (int i = i0; i < i_mx; i++) { res[j][i] = a[i][j]; }}}}}
        return res;}

    // T: O(n^3)
    friend Matrix adj(const Matrix &a0) requires is_field {
        requireField(); assert(a0.n == a0.m);
        int n = a0.n;
        if (n == 0) { return Matrix(); }
        if (n == 1) { return Matrix(vector<T>{T(1)}, 0); }
        Matrix a = a0, b = eye(n, n);
        int r = 0; T pdet = 1; vector<int> pivs(n, -1);
        for (int c = 0; c < n; c++) {
            int piv = r;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = r + 1; p < a.n; p++) {
                    if (abs(a[p][c]) > abs(a[piv][c])) { piv = p; }}
                if (isNil(a[piv][c])) { piv = a.n; }} 
            else { while (piv < a.n && isNil(a[piv][c])) { piv++; } }
            if (piv == n) { continue; }
            if (r != piv) {
                std::swap_ranges(a[r], a[r] + n, a[piv]);
                std::swap_ranges(b[r], b[r] + n, b[piv]);
                pdet = -pdet;}
            pdet *= a[r][c]; T d = T(1) / a[r][c]; a[r][c] = T(1);
            #ifdef __AVX2__
            if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                FastGaussian::mulRow(a[r] + c + 1, d, n - c - 1);
                FastGaussian::mulRow(b[r], d, n);}
            else
            #endif
            {   for (int j = c + 1; j < n; j++) { a[r][j] *= d; }
                for (int j = 0; j < n; j++) { b[r][j] *= d; }}
            for (int i = 0; i < n; i++) {
                if (i == r || isNil(a[i][c])) { continue; }
                T a_ic = a[i][c]; a[i][c] = T(0);
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(a[i] + c + 1, a[r] + c + 1, a_ic, n - c - 1);
                    FastGaussian::elimRow(b[i], b[r], a_ic, n);} 
                else
                #endif 
                {   for (int j = c + 1; j < n; j++) { a[i][j] -= a[r][j] * a_ic; }
                    for (int j = 0; j < n; j++) { b[i][j] -= b[r][j] * a_ic; }}}
            pivs[c] = r++;}
        if (r == n) {
            Matrix res(n, n);
            for (int c = 0; c < n; c++) { std::copy_n(b[pivs[c]], n, res[c]); }
            res *= pdet;
            return res;}
        if (r < n - 1) { return Matrix(n, n); }
        int p = 0;
        while (pivs[p] != -1) { p++; }
        vector<T> u(n); u[p] = T(1);
        for (int c = 0; c < n; c++) {
            if (pivs[c] != -1) { u[c] = -a[pivs[c]][p]; }}
        if ((n - 1 + p) & 1) { pdet = -pdet; }
        Matrix res(n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) { res[i][j] = u[i] * b[n - 1][j] * pdet; }}
        return res;}
    // T: O(n^3)
    friend vector<T> charPoly(Matrix a) requires is_field {
        requireField(); assert(a.n == a.m);
        for (int i = 0; i < a.n - 2; i++) {
            int piv = i + 1;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = i + 2; p < a.n; p++) {
                    if (abs(a[p][i]) > abs(a[piv][i])) { piv = p; }}
                if (isNil(a[piv][i])) { piv = a.n; }} 
            else { while (piv < a.n && isNil(a[piv][i])) { piv++; } }
            if (piv == a.n) { continue; }
            if (piv != i + 1) {
                std::swap_ranges(a[i + 1], a[i + 1] + a.n, a[piv]);
                for (int j = 0; j < a.n; j++) { std::swap(a[j][i + 1], a[j][piv]); }}
            T d = T(1) / a[i + 1][i];
            for (int j = i + 2; j < a.n; j++) {
                if (isNil(a[j][i])) { continue; }
                T a_ji = a[j][i] * d;
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(a[j] + i, a[i + 1] + i, a_ji, a.n - i);} 
                else
                #endif 
                {   for (int k = i; k < a.n; k++) { a[j][k] -= a_ji * a[i + 1][k]; } }
                for (int k = 0; k < a.n; k++) { a[k][i + 1] += a_ji * a[k][j]; }}}
        vector<vector<T>> res(a.n + 1); res[0] = {T(1)};
        for (int i = 1; i <= a.n; i++) {
            res[i].assign(i + 1, T(0));
            for (int j = 0; j < i; j++) { res[i][j + 1] = res[i - 1][j]; }
            #ifdef __AVX2__
            if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                FastGaussian::elimRow(res[i].data(), res[i - 1].data(), a[i - 1][i - 1], i);} 
            else
            #endif 
            {   for (int j = 0; j < i; j++) { res[i][j] -= a[i - 1][i - 1] * res[i - 1][j]; } }
            T p = 1;
            for (int j = i - 2; j >= 0; j--) {
                p *= a[j + 1][j]; T a_ji = a[j][i - 1] * p;
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(res[i].data(), res[j].data(), a_ji, j + 1);} 
                else
                #endif 
                {   for (int k = 0; k <= j; k++) { res[i][k] -= a_ji * res[j][k]; } }}}
        return res[a.n];}
    // T: O(n^4 + n^2 * 2^(n/2)), M: O(n^4)
    friend T hafnian(const Matrix &a) {
        assert(a.n == a.m && !(a.n & 1));
        if (a.n == 0) { return T(1); }
        int n = a.n / 2; ulll mod = 0;
        if constexpr (is_mint) { mod = int(T(-1)) + 1; }
        vector<int> offs(n + 2);
        for (int d = 1; d <= n; d++) {
            offs[d + 1] = offs[d] + d * (2 * d - 1) * (n - d + 1);}
        vector<T> v(offs[n + 1], T(0));
        auto getPtr = [&](int d, int i, int j) -> T* {
            return v.data() + offs[d] + (i * (i - 1) / 2 + j) * (n - d + 1);};
        for (int i = 0; i < a.n; i++) {
            for (int j = 0; j < i; j++) { getPtr(n, i, j)[0] = a[i][j]; }}
        vector<T> work(n * (n + 1));
        auto solve = [&](auto &&solve, int m, T *res) {
            if (m == 2) {
                res[0] = T(0);
                std::copy_n(getPtr(1, 1, 0), n, res + 1);
                return;}
            int d = m / 2;
            for (int i = 0; i < m - 2; i++) {
                for (int j = 0; j < i; j++) {
                    T *cur = getPtr(d, i, j);
                    T *nxt = getPtr(d - 1, i, j);
                    std::copy_n(cur, n - d + 1, nxt);
                    nxt[n - d + 1] = T(0);}}
            solve(solve, m - 2, res);
            for (int i = 0; i < m - 2; i++) {
                for (int j = 0; j < i; j++) {
                    T *a1 = getPtr(d, m - 2, i);
                    T *a2 = getPtr(d, m - 1, i);
                    T *b1 = getPtr(d, m - 1, j);
                    T *b2 = getPtr(d, m - 2, j);
                    T *nxt = getPtr(d - 1, i, j);
                    for (int k = 0; k <= n - d; k++) {
                        if constexpr (is_mint) {
                            ulll sum = 0;
                            for (int l = 0; l <= k; l++) {
                                sum += ulll(int(a1[l])) * int(b1[k - l]) +
                                       ulll(int(a2[l])) * int(b2[k - l]);}
                            nxt[k + 1] += T(sum % mod);} 
                        else {
                            T sum = 0;
                            for (int l = 0; l <= k; l++) {
                                sum += a1[l] * b1[k - l] + a2[l] * b2[k - l];}
                            nxt[k + 1] += sum;}}}}
            T *p = getPtr(d, m - 1, m - 2);
            T *tmp = work.data() + (d - 2) * (n + 1);
            solve(solve, m - 2, tmp);
            res[d - 1] = T(0);
            for (int i = d; i <= n; i++) {
                if constexpr (is_mint) {
                    ulll sum = int(tmp[i]);
                    for (int j = d - 1; j < i; j++) {
                        sum += ulll(int(tmp[j])) * int(p[i - j - 1]);}
                    res[i] = T(sum % mod) - res[i];}
                else {
                    T sum = tmp[i];
                    for (int j = d - 1; j < i; j++) {
                        sum += tmp[j] * p[i - j - 1];}
                    res[i] = sum - res[i];}}};
        T *res = work.data() + (n - 1) * (n + 1);
        solve(solve, a.n, res);
        return res[n];}
    // T: O(n^3)
    friend Matrix intscSpace(const Matrix &a, const Matrix &b) requires is_field { // Row-major
        assert(a.m == b.m);
        if constexpr (is_mint) {
            if (T::mod() == 2 && a.m <= 32) {
                array<ulng, 64> rows{}; int r = 0;
                auto add = [&](ulng x) {
                    for (int i = 0; i < r; i++) { x = min(x, x ^ rows[i]); }
                    if (x) { rows[r++] = x; }};
                for (int i = 0; i < a.n; i++) {
                    ulng x = 0;
                    for (int j = 0; j < a.m; j++) { x |= ulng(int(a[i][j])) << j; }
                    add((x << a.m) | x);}
                for (int i = 0; i < b.n; i++) {
                    ulng x = 0;
                    for (int j = 0; j < a.m; j++) { x |= ulng(int(b[i][j])) << j; }
                    add(x << a.m);}
                ulng msk = (ulng(1) << a.m) - 1;
                int n = 0;
                for (int i = 0; i < r; i++) { n += !(rows[i] >> a.m) && (rows[i] & msk); }
                Matrix res(n, a.m); n = 0;
                for (int i = 0; i < r; i++) {
                    if ((rows[i] >> a.m) || !(rows[i] & msk)) { continue; }
                    for (int j = 0; j < a.m; j++) { res[n][j] = (rows[i] >> j) & 1; }
                    n++;}
                return res;}}
        Matrix c(a.n + b.n, 2 * a.m);
        for (int i = 0; i < a.n; i++) {
            std::copy_n(a[i], a.m, c[i]);
            std::copy_n(a[i], a.m, c[i] + a.m);}
        for (int i = 0; i < b.n; i++) { std::copy_n(b[i], a.m, c[a.n + i]); }
        c = rref<1>(std::move(c));
        int n = 0;
        for (int i = 0; i < c.n; i++) {
            bool chk = true;
            for (int j = 0; j < a.m; j++) {
                if (!isNil(c[i][j])) { chk = false; break; }}
            if (!chk) { continue; }
            for (int j = 0; j < a.m; j++) {
                if (!isNil(c[i][a.m + j])) { n++; break; }}}
        Matrix res(n, a.m); n = 0;
        for (int i = 0; i < c.n; i++) {
            bool l = true, r = false;
            for (int j = 0; j < a.m; j++) {
                l &= isNil(c[i][j]); r |= !isNil(c[i][a.m + j]);}
            if (l && r) { std::copy_n(c[i] + a.m, a.m, res[n++]); }}
        return res;}
    // T: O(n^3)
    friend Matrix ker(Matrix a) requires is_field {
        int n = a.n; Matrix res;
        solveLin(std::move(a), Matrix(n, 0), &res);
        return res;}
    // T: O(n^3)
    friend int rnk(Matrix a) requires is_field {
        requireField();
        if (min(a.n, a.m) <= 1) {
            for (const T &x : a.v) { if (!isNil(x)) { return 1; } }
            return 0;}
        vector<T*> rows(a.n);
        for (int i = 0; i < a.n; i++) { rows[i] = a[i]; }
        int r = 0;
        for (int c = 0; r < a.n && c < a.m; c++) {
            int piv = r;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = r + 1; p < a.n; p++) {
                    if (abs(rows[p][c]) > abs(rows[piv][c])) { piv = p; }}
                if (isNil(rows[piv][c])) { piv = a.n; }}
            else { while (piv < a.n && isNil(rows[piv][c])) { piv++; } }
            if (piv == a.n) { continue; }
            std::swap(rows[r], rows[piv]);
            T d = T(1) / rows[r][c];
            for (int i = r + 1; i < a.n; i++) {
                if (isNil(rows[i][c])) { continue; }
                T q = rows[i][c] * d; rows[i][c] = T(0);
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(rows[i] + c + 1, rows[r] + c + 1, q, a.m - c - 1);}
                else
                #endif
                {   for (int j = c + 1; j < a.m; j++) { rows[i][j] -= rows[r][j] * q; } }}
            r++;}
        return r;}
    // T: O(n^3)
    template<bool opt = 0>
    friend Matrix rref(Matrix a) requires is_field {
        requireField();
        int r = 0;
        for (int c = 0; r < a.n && c < a.m; c++) {
            int piv = r;
            if constexpr (std::is_floating_point_v<T>) {
                for (int p = r + 1; p < a.n; p++) {
                    if (abs(a[p][c]) > abs(a[piv][c])) { piv = p; }}
                if (isNil(a[piv][c])) { piv = a.n; }} 
            else { while (piv < a.n && isNil(a[piv][c])) { piv++; } }
            if (piv == a.n) { continue; }
            if (r != piv) { std::swap_ranges(a[r], a[r] + a.m, a[piv]); }
            T d = T(1) / a[r][c]; a[r][c] = T(1);
            #ifdef __AVX2__
            if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                FastGaussian::mulRow(a[r] + c + 1, d, a.m - c - 1);}
            else
            #endif
            {   for (int j = c + 1; j < a.m; j++) { a[r][j] *= d; }}
            for (int i = opt ? r + 1 : 0; i < a.n; i++) {
                if constexpr (opt) { if (isNil(a[i][c])) { continue; } }
                else { if (i == r || isNil(a[i][c])) { continue; } }
                T a_ic = a[i][c]; a[i][c] = T(0);
                #ifdef __AVX2__
                if constexpr (is_mint32 || std::is_floating_point_v<T>) {
                    FastGaussian::elimRow(a[i] + c + 1, a[r] + c + 1, a_ic, a.m - c - 1);} 
                else
                #endif 
                {   for (int j = c + 1; j < a.m; j++) { a[i][j] -= a[r][j] * a_ic; } }}
            r++;}
        return a;}
    
    // T: O(1)
    static bool isNil(const T &x) {
        if constexpr (std::is_floating_point_v<T>) { return abs(x) < 1e-9; }
        else { return x == T(0); }}
    friend bool operator==(const Matrix &a, const Matrix &b) = default;

    friend ostream &operator<<(ostream &os, const Matrix &a) {
        if (a.n == 0 || a.m == 0) { return os << "[]"; }
        os << "[";
        for (int i = 0; i < a.n; i++) {
            os << "[";
            for (int j = 0; j < a.m; j++) { os << a[i][j] << (j < a.m - 1 ? ", " : ""); }
            os << "]" << (i < a.n - 1 ? ", " : "");}
        return os << "]";}
};
#line 3 "X-Tests Abbreviated\\Yosupo-LinAlg-13-Hafnian.cpp"

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); 

    int n;
    cin >> n;
    vector a(n, vector<mint>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) 
            cin >> a[i][j];
    }

    Matrix<mint> A(a);

    cout << hafnian(A) << '\n';

    return 0;
}
