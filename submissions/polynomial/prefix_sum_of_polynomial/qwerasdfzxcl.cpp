#include <bits/stdc++.h>

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <queue>

#include <atcoder/convolution>

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
#include <bit>

#include <atcoder/modint>

namespace atcoder::internal{

// @param m `1 <= m`
// @return x mod m
constexpr __int128 safe_mod_i128(__int128 x, __int128 m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr __int128 pow_mod_i128_constexpr(__int128 x, __int128 n, long long m) {
    if (m == 1) return 0;
    unsigned long long _m = (unsigned long long)(m);
    unsigned __int128 r = 1;
    unsigned __int128 y = safe_mod_i128(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

} // namespace atcoder::internal

namespace atcoder::math{

template <class T, internal::is_modint<T>* = nullptr>
struct comb_info {
    inline static std::vector<T> fact = {};
    inline static std::vector<T> ifact = {};

    static void init(int n){
        n = std::min((unsigned long long)n, (unsigned long long)T::mod()-1);
        if (fact.empty()) fact = {T(1)}, ifact = {T(1)};
        int m = (int)fact.size();
        if (n < m) return;

        fact.resize(n+1);
        for (int i = m; i <= n; ++i) fact[i] = fact[i-1] * i;

        ifact.resize(n+1);
        ifact[n] = fact[n].inv();
        for (int i = n; i > m; --i) ifact[i-1] = ifact[i] * i;
    }

    static T inv(T n){
        return (n == 0 || (int)n.val() >= (int)fact.size()) ? n.inv() : ifact[n.val()] * fact[n.val()-1];
    }

    static T binom(int n, int r){
        if (r < 0 || r > n) return T(0);
        return fact[n] * ifact[r] * ifact[n-r];
    }
};

// y^2 = x mod P
// P is a prime
// O(log^2 P)
// https://judge.yosupo.jp/submission/326038
template <class T, internal::is_modint_t<T>* = nullptr>
std::pair<bool, T> sqrt(T x){
    long long P = T::mod();
    if (x == 0) return {true, T(0)};
    if (x.pow((P-1)/2) != 1) return {false, T(0)};
    if(P % 4 == 3) return {true, x.pow((P+1)/4)};
    long long s = P - 1;
    T n = 2;
    long long r = 0, m = 0;
    while(~s & 1) r++, s >>= 1;
    while(n.pow((P-1)/2) != -1) n++;
    T y = x.pow((s+1)/2), b = x.pow(s), g = n.pow(s);
    for(;; r=m){
        T t = b;
        for (m=0; m<r && t!=1; m++) t = t * t;
        if (m == 0) return {true, y};
        T gs = g.pow(1LL << (r-m-1));
        g = gs * gs;
        y = y * gs;
        b = b * g;
    }
    return {false, T(0)};
}

// https://judge.yosupo.jp/submission/326209
constexpr bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    
    if (n <= std::numeric_limits<int>::max()){
        long long d = n - 1;
        while (d % 2 == 0) d /= 2;
        constexpr long long bases[3] = {2, 7, 61};
        for (long long a : bases) {
            long long t = d;
            long long y = internal::pow_mod_constexpr(a, t, n);
            while (t != n - 1 && y != 1 && y != n - 1) {
                y = y * y % n;
                t <<= 1;
            }
            if (y != n - 1 && t % 2 == 0) {
                return false;
            }
        }
    }

    else{
        __int128 d = n - 1;
        while (d % 2 == 0) d /= 2;
        // https://miller-rabin.appspot.com/
        constexpr __int128 bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
        for (__int128 a : bases) {
            __int128 t = d;
            __int128 y = internal::pow_mod_i128_constexpr(a, t, n);
            while (t != n - 1 && y != 1 && y != n - 1) {
                y = y * y % n;
                t <<= 1;
            }
            if (y != n - 1 && t % 2 == 0) {
                return false;
            }
        }
    }
    
    return true;
}

// returns array of (p, e)
// https://judge.yosupo.jp/submission/326216
template<bool sorted = true>
std::vector<std::pair<long long, int>> factorize(long long n_in) {
    using u64 = unsigned long long;
    using u128 = unsigned __int128;

    std::vector<std::pair<long long, int>> acc;

    if (n_in == 0) return {};

    u64 n = (n_in < 0) ? (u64)(-(__int128)n_in) : (u64)n_in;

    if ((n & 1ULL) == 0) {
        int z = std::countr_zero(n);
        acc.push_back({2LL, z});
        n >>= z;
    }

    auto upd = [&](const u64& x) {
        for (auto &pe : acc) {
            if ((u64)pe.first == x) { ++pe.second; return; }
        }
        acc.push_back({x, 1});
    };

    struct FastMod {
        u64 mod;
        explicit FastMod(u64 m) : mod(m) {}
        inline u64 add(u64 a, u64 b) const { a += b; if (a >= mod) a -= mod; return a; }
        inline u64 mul(u64 a, u64 b) const { return (u128)a * b % mod; }
        inline u64 mul_add(u64 a, u64 b, u64 c) const {
            u64 t = (u128)a * b % mod;
            t += c; if (t >= mod) t -= mod;
            return t;
        }
    };

    auto Pollard_Rho = [&](const u64 &m) -> u64 {
        if ((m & 1ULL) == 0) return 2ULL;
        if (is_prime((long long)m)) return m;

        const FastMod Mod(m);
        const u64 C1 = 1, C2 = 2;
        const u64 M = 600;

        u64 Z1 = 1, Z2 = 2;
        u64 found = 0;

        auto find_once = [&]() {
            u64 z1 = Z1 % m, z2 = Z2 % m;
            for (u64 k = M; ; k <<= 1) {
                const u64 x1 = z1 + m, x2 = z2 + m;
                for (u64 j = 0; j < k; j += M) {
                    const u64 y1 = z1, y2 = z2;
                    u64 q1 = 1, q2 = 1;

                    z1 = Mod.mul_add(z1, z1, C1);
                    z2 = Mod.mul_add(z2, z2, C2);

                    for (u64 i = 0; i < M; ++i) {
                        u64 t1 = x1 - z1; if (t1 >= m) t1 -= m;
                        u64 t2 = x2 - z2; if (t2 >= m) t2 -= m;
                        z1 = Mod.mul_add(z1, z1, C1);
                        z2 = Mod.mul_add(z2, z2, C2);
                        q1 = Mod.mul(q1, t1);
                        q2 = Mod.mul(q2, t2);
                    }
                    q1 = Mod.mul(q1, (x1 - z1) >= m ? (x1 - z1 - m) : (x1 - z1));
                    q2 = Mod.mul(q2, (x2 - z2) >= m ? (x2 - z2 - m) : (x2 - z2));

                    u64 q3 = Mod.mul(q1, q2);
                    u64 g3 = std::gcd(m, q3);
                    if (g3 == 1) continue;
                    if (g3 != m) { found = g3; return; }

                    u64 g1 = std::gcd(m, q1);
                    u64 g2 = std::gcd(m, q2);
                    u64 C = (g1 != 1 ? C1 : C2);
                    u64 x = (g1 != 1 ? x1 : x2);
                    u64 z = (g1 != 1 ? y1 : y2);
                    u64 g = (g1 != 1 ? g1 : g2);

                    if (g == m) {
                        do {
                            z = Mod.mul_add(z, z, C);
                            u64 diff = x - z; if (diff >= m) diff -= m;
                            g = std::gcd(m, diff);
                        } while (g == 1);
                    }
                    if (g != m) { found = g; return; }

                    Z1 += 2; Z2 += 2;
                    return;
                }
            }
        };

        do { find_once(); } while (!found);
        return found;
    };

    auto DFS = [&](auto &&self, const u64 &x) -> void {
        if (x == 1) return;
        if (is_prime((long long)x)) { upd(x); return; }
        u64 d = x;
        while (d == x) d = Pollard_Rho(x);
        self(self, d);
        self(self, x / d);
    };

    if (n > 1) DFS(DFS, n);
    if constexpr (sorted) std::sort(acc.begin(), acc.end());
    return acc;
}

// primitive root modulo p
// if p is not a prime -> return -1
long long primitive_root(long long p){
    if (!is_prime(p)) return -1;
    if (p == 2) return 1;
    
    auto factors = factorize(p-1);
    for (long long i=2;;i++){
        bool flag = true;
        for (auto &[q, e]:factors) if (internal::pow_mod_i128_constexpr(i, (p-1) / q, p) == 1){
            flag = false;
            break;
        }
        if (flag) return i;
    }
    
    return 0;
}

// f(array<long long, 2>) -> bool
// f(0/1) = true, f(1/0) = false
// ret[0]: maximum x s.t. f(x) is true
// ret[1]: minimum x s.t. f(x) is false
// n: limit of numerator and denominator
// O(log n)
std::array<std::array<long long, 2>, 2> rational_search(long long n, const auto &f){
    using Frac = std::array<long long, 2>;
    Frac L = {0, 1}, R = {1, 0};
    
    auto chk_L = [&](Frac L, Frac R, long long x){
        return (__int128)L[0]*x + R[0] <= n && (__int128)L[1]*x + R[1] <= n && !f(Frac{L[0]*x + R[0], L[1]*x + R[1]});
    };
    auto chk_R = [&](Frac L, Frac R, long long x){
        return L[0] + (__int128)R[0]*x <= n && L[1] + (__int128)R[1]*x <= n && f(Frac{L[0] + R[0]*x, L[1] + R[1]*x});
    };

    auto exp_search = [&](Frac L, Frac R, const auto &ok){
        if (!ok(L, R, 1)) return 0ll;
        long long l = 1, r = 2;
        while(ok(L, R, r)) l <<= 1, r <<= 1;
        while(r-l > 1){
            long long mid = (l+r)>>1;
            if (ok(L, R, mid)) l = mid;
            else r = mid;
        }
        return l;
    };

    while(true){
        long long x = exp_search(L, R, chk_L);
        if (x) R = Frac{L[0]*x + R[0], L[1]*x + R[1]};
        long long y = exp_search(L, R, chk_R);
        if (y) L = Frac{L[0] + R[0]*y, L[1] + R[1]*y};
        if (!x && !y) break;
    }

    return {L, R};
}

// Stern-Brocot tree
// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
// https://judge.yosupo.jp/submission/326297
namespace SBT{

// v = a/b
// path from 1/1 to a/b
// ret[i] < 0: left, ret[i] > 0: right
// O(log max(a, b))
std::vector<long long> encode(std::array<long long, 2> v){
    using Frac = std::array<long long, 2>;
    using T = __int128;
    Frac L = {0, 1}, R = {1, 0};
    
    auto chk_L = [&](Frac L, Frac R, long long x){
        T a = (T)L[0] * x + R[0];
        T b = (T)L[1] * x + R[1];
        return b * v[0] < a * v[1];
    };
    auto chk_R = [&](Frac L, Frac R, long long x){
        T a = L[0] + (T)R[0] * x;
        T b = L[1] + (T)R[1] * x;
        return a * v[1] < b * v[0];
    };

    auto exp_search = [&](Frac L, Frac R, const auto &ok){
        if (!ok(L, R, 1)) return 0ll;
        long long l = 1, r = 2;
        while(ok(L, R, r)) l <<= 1, r <<= 1;
        while(r-l > 1){
            long long mid = (l+r)>>1;
            if (ok(L, R, mid)) l = mid;
            else r = mid;
        }
        return l;
    };

    std::vector<long long> ret;

    while(true){
        long long x = exp_search(L, R, chk_L);
        if (x) {ret.push_back(-x); R = Frac{L[0]*x + R[0], L[1]*x + R[1]};}
        long long y = exp_search(L, R, chk_R);
        if (y) {ret.push_back(y); L = Frac{L[0] + R[0]*y, L[1] + R[1]*y};}
        if (!x && !y) break;
    }

    return ret;
}

// decode path to a/b
// O(|path|)
std::array<long long, 2> decode(const std::vector<long long> &path){
    using Frac = std::array<long long, 2>;
    Frac L = {0, 1}, R = {1, 0};

    for (auto &x:path){
        if (x < 0) R = Frac{L[0]*(-x) + R[0], L[1]*(-x) + R[1]};
        else L = Frac{L[0] + R[0]*x, L[1] + R[1]*x};
    }

    return {L[0] + R[0], L[1] + R[1]};
}

long long len(const std::vector<long long> &path){
    long long ret = 0;
    for (auto &x:path) ret += std::abs(x);
    return ret;
}

std::vector<long long> lcp(const std::vector<long long> &pu, const std::vector<long long> &pv){
    std::vector<long long> p;
    for (int i=0;i<std::min((int)pu.size(), (int)pv.size());i++){
        if (pu[i] == pv[i]) {p.push_back(pu[i]); continue;}
        else if (pu[i] > 0 && pv[i] > 0) p.push_back(std::min(pu[i], pv[i]));
        else if (pu[i] < 0 && pv[i] < 0) p.push_back(std::max(pu[i], pv[i]));
        break;
    }
    return p;
}

std::pair<bool, std::vector<long long>> prefix(const std::vector<long long> &path, long long k){
    std::vector<long long> ret;
    for (int i=0;i<(int)path.size() && k>0;i++){
        if (path[i] < 0){
            long long t = std::min(k, -path[i]);
            ret.push_back(-t);
            k -= t;
        }
        else{
            long long t = std::min(k, path[i]);
            ret.push_back(t);
            k -= t;
        }
    }

    if (k > 0) return {false, {}};
    return {true, ret};
}

// lca of u = a/b and v = c/d
// O(log max(a, b, c, d))
std::array<long long, 2> lca(const std::array<long long, 2> &u, const std::array<long long, 2> &v){
    return decode(lcp(encode(u), encode(v)));
}

// ancestor of v = a/b with depth k
// fail -> ret = {-1, -1}
// O(log max(a, b))
std::array<long long, 2> ancestor(const std::array<long long, 2> &v, long long k){
    auto [flag, path] = prefix(encode(v), k);
    if (!flag) return {-1, -1};
    return decode(path);
}

// subtree range of v = a/b
// ret = {L, R}
// O(log max(a, b))
std::array<std::array<long long, 2>, 2> range(const std::array<long long, 2> &v){
    using Frac = std::array<long long, 2>;
    using T = __int128;
    Frac L = {0, 1}, R = {1, 0};
    
    auto chk_L = [&](Frac L, Frac R, long long x){
        T a = (T)L[0] * x + R[0];
        T b = (T)L[1] * x + R[1];
        return b * v[0] < a * v[1];
    };
    auto chk_R = [&](Frac L, Frac R, long long x){
        T a = L[0] + (T)R[0] * x;
        T b = L[1] + (T)R[1] * x;
        return a * v[1] < b * v[0];
    };

    auto exp_search = [&](Frac L, Frac R, const auto &ok){
        if (!ok(L, R, 1)) return 0ll;
        long long l = 1, r = 2;
        while(ok(L, R, r)) l <<= 1, r <<= 1;
        while(r-l > 1){
            long long mid = (l+r)>>1;
            if (ok(L, R, mid)) l = mid;
            else r = mid;
        }
        return l;
    };

    std::vector<long long> ret;

    while(true){
        long long x = exp_search(L, R, chk_L);
        if (x) {R = Frac{L[0]*x + R[0], L[1]*x + R[1]};}
        long long y = exp_search(L, R, chk_R);
        if (y) {L = Frac{L[0] + R[0]*y, L[1] + R[1]*y};}
        if (!x && !y) break;
    }

    return {L, R};
}

} // namespace SBT

// min((Ax + B) mod M) (0 <= x < N)
// O(log M)
// https://judge.yosupo.jp/submission/326265
long long min_mod_linear(long long N, long long M, long long A, long long B){
    using T = __int128;

    assert(M >= 1 && N >= 1);
    if (!(0 <= A && A < M)) A = (A % M + M) % M;
    if (!(0 <= B && B < M)) B = (B % M + M) % M;
    if (N >= M) N = M;

    T xlim = N, ans = M - 1;
    
    auto go = [&](const std::array<T, 2> &a, const std::array<T, 2> &s, T xlim) -> std::array<T, 2> {
        T q = (xlim - s[0] - 1) / a[0];
        ans = std::min(ans, s[1]);
        ans = std::min(ans, s[1] + a[1] * q);
        return {s[0] + a[0] * q, s[1] + a[1] * q};
    };

    auto solve = [&](auto &&self, const std::array<T, 2> &a, const std::array<T, 2> &b, const std::array<T, 2> &s) -> std::array<T, 2> {
        ans = std::min(ans, s[1]);
        if (a[1] == 0) return go(a, s, xlim);
        if (b[1] == 0) return go(b, s, xlim);

        if (a[1] <= (-b[1])){
            T t = 0, q = (-b[1]) / a[1];
            if (s[1] < (-b[1])) t = (- b[1] - s[1] + a[1] - 1) / a[1];
            if (s[0] + a[0] * t + b[0] >= xlim) return go(a, s, std::min(xlim, s[0] + a[0] * t + 1));

            std::array<T, 2> na = a;
            std::array<T, 2> nb = {a[0] * q + b[0], -((-b[1]) % a[1])};
            std::array<T, 2> ns = {s[0] + a[0] * t + b[0], s[1] + a[1] * t + b[1]};
            
            auto ret = self(self, na, nb, ns);
            return go(a, ret, std::min(xlim, ret[0] + a[0] * q + 1));
        }

        else{
            T t = 0, q = a[1] / (-b[1]);
            if (s[1] >= -b[1]) t = s[1] / (-b[1]);
            if (s[0] + b[0] * t >= xlim) return go(b, s, std::min(xlim, s[0] + b[0] * t + 1));

            std::array<T, 2> na = {a[0] + b[0] * q, a[1] % (-b[1])};
            std::array<T, 2> nb = b;
            std::array<T, 2> ns = {s[0] + b[0] * t, s[1] + b[1] * t};

            auto ret = self(self, na, nb, ns);
            if (ret[0] + a[0] >= xlim) return ret;
            ret[0] += a[0], ret[1] += a[1];
            return go(b, ret, std::min(xlim, ret[0] + b[0] * q + 1));
        }
    };

    solve(solve, {1, A}, {0, -M}, {0, B});

    return ans;
}

} // namespace atcoder::math


namespace atcoder{

// https://judge.yosupo.jp/submission/327685
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> convolution_bad(const std::vector<T>& a,
                              const std::vector<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};

    static constexpr unsigned long long MOD1 = 754974721;  // 2^24
    static constexpr unsigned long long MOD2 = 167772161;  // 2^25
    static constexpr unsigned long long MOD3 = 469762049;  // 2^26

    const unsigned long long P = (unsigned long long)T::mod();
    const unsigned long long M1P  = MOD1 % P;
    const unsigned long long M2P  = MOD2 % P;
    const unsigned long long M12P = (unsigned long long)((__uint128_t)M1P * M2P % P);

    static constexpr unsigned long long i12 =
        internal::inv_gcd(MOD1 % MOD2, MOD2).second;
    static constexpr unsigned long long i13 =
        internal::inv_gcd(MOD1 % MOD3, MOD3).second;
    static constexpr unsigned long long i23 =
        internal::inv_gcd(MOD2 % MOD3, MOD3).second;
        
    static constexpr int MAX_AB_BIT = 24;
    static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1, "MOD1 isn't enough to support an array length of 2^24.");
    static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1, "MOD2 isn't enough to support an array length of 2^24.");
    static_assert(MOD3 % (1ull << MAX_AB_BIT) == 1, "MOD3 isn't enough to support an array length of 2^24.");
    assert(n + m - 1 <= (1 << MAX_AB_BIT));

    std::vector<long long> A(a.size()), B(b.size());
    for (int i=0;i<(int)a.size();i++) A[i] = a[i].val();
    for (int i=0;i<(int)b.size();i++) B[i] = b[i].val();

    auto c1 = convolution<MOD1>(A, B);
    auto c2 = convolution<MOD2>(A, B);
    auto c3 = convolution<MOD3>(A, B);

    std::vector<T> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        unsigned long long r1 = (unsigned long long)(c1[i] % MOD1);
        unsigned long long r2 = (unsigned long long)(c2[i] % MOD2);
        unsigned long long r3 = (unsigned long long)(c3[i] % MOD3);

        unsigned long long v1 = r1;

        unsigned long long v2 =
            (unsigned long long)((__uint128_t)((r2 + MOD2 - (v1 % MOD2)) % MOD2) * i12 % MOD2);

        unsigned long long t =
            (unsigned long long)((__uint128_t)((r3 + MOD3 - (v1 % MOD3)) % MOD3) * i13 % MOD3);
        unsigned long long v3 =
            (unsigned long long)((__uint128_t)((t + MOD3 - (v2 % MOD3)) % MOD3) * i23 % MOD3);

        unsigned long long ans =
            (unsigned long long)(
                ( (__uint128_t)(v1 % P)
                + (__uint128_t)(v2 % P) * M1P
                + (__uint128_t)(v3 % P) * M12P ) % P
            );

        c[i] = T::raw(ans);
    }

    return c;
}

} // namespace atcoder

namespace atcoder::poly{

// f(x)
template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
T eval(const std::vector<T> &f, T x){
    T ret = T(0), cur = T(1);
    for (auto &c:f){
        ret += c * cur;
        cur *= x;
    }
    return ret;
}

template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
void reverse_inplace(std::vector<T> &f){
    std::reverse(f.begin(), f.end());
}

// [x^0, ..., x^(n-1)] x^(n-1) f(1/x)
template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
std::vector<T> reverse(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    if (n == 0) return {};
    assert(n >= 0);

    std::vector<T> ret = std::vector<T>(f.begin(), f.begin() + std::min(n, (int)f.size()));
    ret.resize(n);
    std::reverse(ret.begin(), ret.end());
    return ret;
}

// f + g mod x^n
template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
std::vector<T> add(const std::vector<T> &f, const std::vector<T> &g, int n = -1) {
    if (n == -1) n = std::max(f.size(), g.size());
    if (n == 0) return {};
    assert(n >= 0);

    std::vector<T> ret(n, T(0));
    for (int i=0;i<std::min(n, (int)std::max(f.size(), g.size()));i++){
        if (i < (int)f.size()) ret[i] += f[i];
        if (i < (int)g.size()) ret[i] += g[i];
    }
    
    return ret;
}

// f - g mod x^n
template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
std::vector<T> sub(const std::vector<T> &f, const std::vector<T> &g, int n = -1) {
    if (n == -1) n = std::max(f.size(), g.size());
    if (n == 0) return {};
    assert(n >= 0);

    std::vector<T> ret(n, T(0));
    for (int i=0;i<std::min(n, (int)std::max(f.size(), g.size()));i++){
        if (i < (int)f.size()) ret[i] += f[i];
        if (i < (int)g.size()) ret[i] -= g[i];
    }
    
    return ret;
}

// f * g mod x^n
// Uses NTT when possible, otherwise falls back to convolution_bad(NTT x 3 + CRT).
// O(n log n)
template <class T, atcoder::internal::is_modint_t<T>* = nullptr>
std::vector<T> multiply(const std::vector<T> &f, const std::vector<T> &g, int n = -1) {
    if (n == -1) {
        // full convolution
        int need = (int)f.size() + (int)g.size() - 1;
        if (need <= 0) return {};
        if constexpr (internal::is_static_modint<T>::value) {
            unsigned long long L = internal::bit_ceil((unsigned int)need);
            if (((unsigned long long)T::mod() - 1) % L == 0) {
                return convolution(f, g);
            }
        }
        return convolution_bad(f, g);
    } else {
        assert(n >= 0);
        std::vector<T> F(f.begin(), f.begin() + std::min(n, (int)f.size()));
        std::vector<T> G(g.begin(), g.begin() + std::min(n, (int)g.size()));

        int need = (int)F.size() + (int)G.size() - 1;
        if (need <= 0) return std::vector<T>(n);

        std::vector<T> ret;
        if constexpr (atcoder::internal::is_static_modint<T>::value) {
            unsigned long long L = atcoder::internal::bit_ceil((unsigned int)need);
            if (((unsigned long long)T::mod() - 1) % L == 0) {
                ret = convolution(F, G);
            } else {
                ret = convolution_bad(F, G);
            }
        } else {
            ret = convolution_bad(F, G);
        }
        ret.resize(n, T(0));
        return ret;
    }
}

// f * g mod x^n (exact value in long long)
// O(n log n)
std::vector<long long> multiply_ll(const std::vector<long long> &f, const std::vector<long long> &g, int n = -1){
    if (n == -1) return convolution_ll(f, g);
    assert(n >= 0);

    std::vector<long long> F(f.begin(), f.begin() + std::min(n, (int)f.size()));
    std::vector<long long> G(g.begin(), g.begin() + std::min(n, (int)g.size()));
    auto ret = convolution_ll(F, G);
    ret.resize(n);
    return ret;
}

// f(x_0, x_1, ..., x_{k-1}) * g(x_0, x_1, ..., x_{k-1}) mod (x_0^{d_0}, x_1^{d_1}, ..., x_{k-1}^{d_{k-1}})
// d = [d_0, d_1, ..., d_{k-1}], d[i] >= 2, k >= 1
// n = \prod d_i, O(k n log n + k^2 n)
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> multiply_kd(const std::vector<T> &f, const std::vector<T> &g, const std::vector<int> &d){
    assert(!d.empty()); assert(f.size() == g.size());
    for (int i=0,cur=d[0] ; i<(int)d.size() ; cur *= d[++i]){
        assert(d[i] >= 2);
        if (i+1 == (int)d.size()) assert(cur == (int)f.size());
        else assert((long long)cur * d[i+1] <= std::numeric_limits<int>::max());
    }

    int n = f.size(), k = d.size();
    std::vector<int> I(f.size());

    for (int i=0, p=d[0] ; i<k-1 ; p *= d[++i]){
        for (int j=0, v=0 ; j<n ; j += p, v++){
            if (v == k) v = 0;
            for (int t=j ; t<j+p ; t++){
                I[t] += v;
                if (I[t] >= k) I[t] -= k;
            }
        }
    }

    int sz = 1;
    while(sz < n) sz *= 2;
    sz *= 2;

    std::vector<std::vector<T>> F(k, std::vector<T>(sz)), G(k, std::vector<T>(sz));
    
    for (int i=0;i<n;i++) F[I[i]][i] = f[i];
    for (int i=0;i<n;i++) G[I[i]][i] = g[i];

    for (int i=0;i<k;i++) internal::butterfly(F[i]);
    for (int i=0;i<k;i++) internal::butterfly(G[i]);

    std::vector<std::vector<T>> H(k, std::vector<T>(sz));
    for (int i=0;i<k;i++){
        for (int j=0;j<k;j++){
            int p = (i+j) % k;
            for (int t=0;t<sz;t++) H[p][t] += F[i][t] * G[j][t];
        }
    }

    for (int i=0;i<k;i++) internal::butterfly_inv(H[i]);
    
    T ninv = T(sz).inv();
    std::vector<T> ret(n);
    for (int i=0;i<n;i++) ret[i] = H[I[i]][i] * ninv;
    return ret;
}

// f^(-1) mod x^n
// O(n log n)
// https://judge.yosupo.jp/submission/325710
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> inverse(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    assert(n >= 1 && f[0] != 0);

    std::vector<T> g = {f[0].inv()}; int sz = 1;
    
    while(sz < n){
        auto h = multiply(f, g, sz * 2);
        for (auto &x:h) x = -x;
        h[0] += 2;
        g = multiply(g, h, sz * 2);
        sz *= 2;
    }

    g.resize(n);
    return g;
}

// f = g * q + r, returns {q, r} (deg r < deg g)
// n = deg f, O(n log n)
// https://judge.yosupo.jp/submission/325714
template <class T, internal::is_modint_t<T>* = nullptr>
std::pair<std::vector<T>, std::vector<T>> divide(const std::vector<T> &f, const std::vector<T> &g){
    assert(f.empty() || f.back() != 0);
    assert(!g.empty() && g.back() != 0);
    
    int n = f.size(), m = g.size();
    if (n < m) return {{}, f};
    
    auto fR = reverse(f), gR = reverse(g);

    auto Q = reverse(multiply(fR, inverse(gR, n-m+1), n-m+1));
    while(!Q.empty() && Q.back() == 0) Q.pop_back();

    auto R = multiply(g, Q, m);
    for (int i=0;i<m;i++) R[i] = f[i] - R[i];
    while(!R.empty() && R.back() == 0) R.pop_back();

    return {Q, R};
}

// x^k mod f
// n = deg f, O(n log n log k)
// https://judge.yosupo.jp/submission/325722
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> kitamasa(const std::vector<T> &f, long long k){
    assert(!f.empty() && f.back() != 0);
    assert(k >= 0);
    
    std::vector<T> ret = {1}, base = {0, 1};
    while(k){
        if (k&1) ret = divide(multiply(ret, base), f).second;
        base = divide(multiply(base, base), f).second;
        k >>= 1;
    }
    
    return ret;
}

template <class T, internal::is_modint_t<T>* = nullptr>
void deriv_inplace(std::vector<T> &f){
    for (int i=0;i+1<(int)f.size();i++) f[i] = f[i+1] * (i+1);
    if (!f.empty()) f.pop_back();
}

// df / dx mod x^n
// O(n)
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> deriv(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    assert(n >= 0);

    std::vector<T> ret(f.begin(), f.begin() + std::min(n+1, (int)f.size()));
    ret.resize(n+1);
    deriv_inplace(ret);
    return ret;
}

template <class T, internal::is_modint_t<T>* = nullptr>
void integ_inplace(std::vector<T> &f, int c = 0){
    math::comb_info<T>::init(f.size());
    f.push_back(0);
    for (int i=(int)f.size()-1;i-1>=0;i--) f[i] = f[i-1] * math::comb_info<T>::inv(i);
    f[0] = c;
}

// (int f(x) dx) + c mod x^n
// O(n)
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> integ(const std::vector<T> &f, int n = -1, int c = 0){
    if (n == -1) n = f.size();
    if (n == 0) return {};
    assert(n >= 0);

    std::vector<T> ret(f.begin(), f.begin() + std::min(n-1, (int)f.size()));
    ret.resize(n-1);
    integ_inplace(ret, c);
    return ret;
}

// log f(x) mod x^n
// O(n log n)
// https://judge.yosupo.jp/submission/325965
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> log(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    assert(!f.empty() && f[0] == 1);
    assert(n >= 0);

    auto ret = multiply(deriv(f, n), inverse(f, n), n);
    return integ(ret, n);
}

// exp f(x) mod x^n
// O(n log n)
// https://judge.yosupo.jp/submission/325970
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> exp(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    assert(f.empty() || f[0] == 0);
    assert(n >= 0);

    std::vector<T> ret = {T(1)};
    int sz = 1;

    while(sz < n){
        auto g = log(ret, sz * 2);
        for (int i=0;i<sz*2;i++){
            if (i < (int)f.size()) g[i] = f[i] - g[i];
            else g[i] = -g[i];
        }
        g[0] += 1;

        ret = multiply(ret, g, sz * 2);
        sz *= 2;
    }

    ret.resize(n);
    return ret;
}

// f^k mod x^n
// O(n log n)
// https://judge.yosupo.jp/submission/325973
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> pow(const std::vector<T> &f, long long k, int n = -1){
    if (n == -1) n = f.size();
    assert(k >= 0);
    assert(n >= 0);

    if (k == 0){
        std::vector<T> ret(n);
        ret[0] = 1;
        return ret;
    }

    int p = -1;
    for (int i=0;i<std::min(n, (int)f.size());i++) if (f[i] != 0) {p = i; break;}
    if (p == -1) return std::vector<T>(n);
    if ((__int128)p * k >= n) return std::vector<T>(n);

    int m = n - p * k;

    std::vector<T> g(f.begin() + p, f.begin() + std::min(p+m, (int)f.size()));
    g.resize(m);

    T g0_k = g[0].pow(k), g0_inv = g[0].inv();
    for (auto &x:g) x *= g0_inv;

    g = log(g, m);
    for (auto &x:g) x *= k;
    g = exp(g, m);
    for (auto &x:g) x *= g0_k;

    g.insert(g.begin(), n - m, T(0));
    return g;
}

// g^2 = f mod x^n
// O(n log n)
// https://judge.yosupo.jp/submission/326084
template <class T, internal::is_modint_t<T>* = nullptr>
std::pair<bool, std::vector<T>> sqrt(const std::vector<T> &f, int n = -1){
    if (n == -1) n = f.size();
    assert(n >= 0);

    int p = -1;
    for (int i=0;i<std::min(n, (int)f.size());i++) if (f[i] != 0) {p = i; break;}
    if (p == -1) return {true, std::vector<T>(n)};
    if (p % 2 == 1) return {false, {}};

    auto [flag, g0_sqrt] = math::sqrt(f[p]);
    if (!flag) return {false, {}};

    int m = n - p / 2;

    std::vector<T> g(f.begin() + p, f.begin() + std::min(p+m, (int)f.size()));
    g.resize(m);

    T g0_inv = g[0].inv();
    for (auto &x:g) x *= g0_inv;

    std::vector<T> ret = {T(1)};
    int sz = 1;
    T inv2 = T(2).inv();

    while(sz < m){
        auto h = multiply(g, inverse(ret, sz*2), sz*2);
        std::swap(ret, h);
        for (int i=0;i<sz;i++) ret[i] += h[i];
        for (auto &x:ret) x *= inv2;
        sz *= 2;
    }

    ret.resize(m);

    for (auto &x:ret) x *= g0_sqrt;
    ret.insert(ret.begin(), n - m, T(0));
    return {true, ret};
}

// [x^l ... x^(r-1)] 1 / f
// n = deg f, m = r-l, O(n log (n+m) log r + m log (n+m))
// https://judge.yosupo.jp/submission/326705
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> bostan_mori(const std::vector<T> &f, long long l, long long r){
    assert(!f.empty() && f[0] != 0);
    assert(0 <= l && l <= r);
    if (l == r) return {};
    if (l <= (int)f.size()){
        auto ret = inverse(f, r);
        return std::vector<T>(ret.begin() + l, ret.end());
    }

    int dege = ((int)f.size() + 1) / 2, dego = (int)f.size() / 2; // |e| = ceil(|f|/2), |o| = floor(|f|/2)
    std::vector<T> e(dege), o(dego); 
    for (int i=0;i<(int)f.size();i++) ((i&1) ? o : e)[i/2] = f[i];

    auto ee = multiply(e, e, (int)f.size()), oo = multiply(o, o, (int)f.size()-1);
    
    std::vector<T> v(f.size()); // |v| = |f|
    v[0] = ee[0];
    for (int i=1;i<(int)f.size();i++) v[i] = ee[i] - oo[i-1];
    
    auto g = bostan_mori(v, l/2 - dege + 1, (r+1)/2); // after log (n+m) step, r-l = |f|

    auto re = multiply(g, e), ro = multiply(g, o);
    std::vector<T> ret(r-l);
    for (long long i=l;i<r;i++){
        if (i&1) ret[i-l] = -ro[i/2 - l/2 + dege - 1];
        else ret[i-l] = re[i/2 - l/2 + dege - 1];
    }

    return ret;
}

// f(a), f(ar), ..., f(ar^(m-1))
// n = deg f, O((n+m) log (n+m))
// https://judge.yosupo.jp/submission/327691
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> chirp_z(const std::vector<T> &f, T r, int m = -1, T a = T(1)){
    if (m == -1) m = f.size(); 
    assert(m >= 0);
    if (m == 0) return {};
    if (f.empty()) return std::vector<T>(m, T(0));

    if (r == 0){
        std::vector<T> ret(m, f[0]);
        ret[0] = eval(f, a);
        return ret;
    }

    int n = f.size();

    // ik = i(i+1)/2 - (k-i)(k-i-1)/2 + k(k-1)/2

    std::vector<T> F(n);
    T cura = T(1), curr = T(1), curr2 = T(1);
    for (int i=0;i<n;i++){
        F[i] = f[i] * cura * curr;
        cura *= a;
        curr2 *= r; curr *= curr2;
    }

    std::vector<T> G(m+n-1);
    T rinv = r.inv();
    curr = T(1), curr2 = T(1);
    for (int i=0;i<m;i++){
        G[i+n-1] = curr;
        curr *= curr2; curr2 *= rinv;
    }

    curr = T(1), curr2 = T(1);
    for (int i=0;i<n;i++){
        G[-i+n-1] = curr;
        curr2 *= rinv; curr *= curr2;
    }

    auto H = multiply(F, G);
    std::vector<T> ret(m);
    curr = T(1), curr2 = T(1);
    for (int i=0;i<m;i++){
        ret[i] = H[i+n-1] * curr;
        curr *= curr2; curr2 *= r;
    }

    return ret;
}

// product of a[0], a[1], ..., a[n-1]
// n = size(a), m = \sum deg a[i], O((n+m) log^2 (n+m))
// https://judge.yosupo.jp/submission/327713
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> product(const std::vector<std::vector<T>> &a){
    if (a.empty()) return {1};
    if (a.size() == 1) return a[0];

    int n = a.size();
    std::vector<std::vector<T>> b;
    std::priority_queue<std::array<int, 2>, std::vector<std::array<int, 2>>, std::greater<std::array<int, 2>>> pq;
    
    for (int i=0;i<n;i++) pq.push({(int)a[i].size(), i});
    while(pq.size() >= 2){
        auto [_, i] = pq.top(); pq.pop();
        auto [__, j] = pq.top(); pq.pop();

        auto &f = (i < n) ? a[i] : b[i-n];
        auto &g = (j < n) ? a[j] : b[j-n];

        b.push_back(multiply(f, g));

        if (i >= n) std::vector<T>().swap(b[i-n]);
        if (j >= n) std::vector<T>().swap(b[j-n]);

        pq.push({(int)b.back().size(), n + (int)b.size() - 1});
    }

    return b.back();
}

// f(x[0]), f(x[1]), ..., f(x[m-1])
// n = deg f, m = size(x), O((n+m) log (n+m) + m log^2 m)
// https://judge.yosupo.jp/submission/327962
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> eval(const std::vector<T> &f, const std::vector<T> &x){
    if (x.empty()) return {};
    if (x.size() == 1) return {eval(f, x[0])};

    int n = f.size(), m = x.size(), sz = internal::bit_ceil(x.size());
    std::vector<std::vector<T>> tree(sz * 2);

    auto init = [&](auto &&self, int i, int l, int r) -> void {
        if (r-l == 1) {tree[i] = {T(1), -x[l]}; return;}
        int m = (l+r) >> 1;
        self(self, i<<1, l, m); self(self, i<<1|1, m, r);
        tree[i] = multiply(tree[i<<1], tree[i<<1|1]);
    };
    
    std::vector<T> ret(m);

    auto dnc = [&](auto &&self, int i, int l, int r, const std::vector<T> &q) -> void {
        if (r-l == 1) {ret[l] = q[0]; return;}
        
        int m = (l+r) >> 1, len = r-l;
        auto ql = multiply(q, tree[i<<1|1]); ql = std::vector<T>(ql.begin() + len - (m-l), ql.begin() + len);
        auto qr = multiply(q, tree[i<<1]); qr = std::vector<T>(qr.begin() + len - (r-m), qr.begin() + len);
        
        self(self, i<<1, l, m, ql);
        self(self, i<<1|1, m, r, qr);
    };
    
    init(init, 1, 0, m);
    
    auto Q = multiply(reverse(f), inverse(tree[1], n), n);
    if (n >= m) Q = std::vector<T>(Q.end()-m, Q.end());
    else Q.insert(Q.begin(), m-n, T(0));

    dnc(dnc, 1, 0, m, Q);

    return ret;
}

// given f(x[i]) = y[i], returns f
// n = size(x) = size(y), O(n log^2 n)
// x[i] are distinct
// https://judge.yosupo.jp/submission/327964
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> interpolate(const std::vector<T> &x, const std::vector<T> &y){
    assert(x.size() == y.size());
    if (x.empty()) return {};
    if (x.size() == 1) return {y[0]};
    auto X = x;
    sort(X.begin(), X.end(), [](auto a, auto b){return a.val() < b.val();});
    assert(unique(X.begin(), X.end()) == X.end());

    int n = x.size(), sz = internal::bit_ceil(x.size());
    std::vector<std::vector<T>> tree(sz * 2);

    auto init = [&](auto &&self, int i, int l, int r) -> void {
        if (r-l == 1) {tree[i] = {T(1), -x[l]}; return;}
        int m = (l+r) >> 1;
        self(self, i<<1, l, m); self(self, i<<1|1, m, r);
        tree[i] = multiply(tree[i<<1], tree[i<<1|1]);
    };

    std::vector<T> c(n);

    auto dnc1 = [&](auto &&self, int i, int l, int r, const std::vector<T> &q) -> void {
        if (r-l == 1) {c[l] = y[l] / q[0]; return;}
        
        int m = (l+r) >> 1, len = r-l;
        auto ql = multiply(q, tree[i<<1|1]); ql = std::vector<T>(ql.begin() + len - (m-l), ql.begin() + len);
        auto qr = multiply(q, tree[i<<1]); qr = std::vector<T>(qr.begin() + len - (r-m), qr.begin() + len);
        
        self(self, i<<1, l, m, ql);
        self(self, i<<1|1, m, r, qr);
    };

    auto dnc2 = [&](auto &&self, int i, int l, int r) -> std::vector<T> {
        if (r-l == 1) {return {c[l]};}

        int m = (l+r) >> 1;
        auto L = self(self, i<<1, l, m);
        auto R = self(self, i<<1|1, m, r);

        return add(multiply(L, tree[i<<1|1]), multiply(R, tree[i<<1]));
    };

    init(init, 1, 0, n);

    auto P = reverse(tree[1]);
    deriv_inplace(P);
    reverse_inplace(P);
    P = multiply(P, inverse(tree[1], n), n);

    dnc1(dnc1, 1, 0, n, P);

    return reverse(dnc2(dnc2, 1, 0, n));
}

// g(x) = f(x+c)
// n = deg f, O(n log n)
// https://judge.yosupo.jp/submission/328273
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> shift(const std::vector<T> &f, T c){
    if (f.empty()) return {};
    if (c == 0) return f;
    
    int n = f.size();
    math::comb_info<T>::init(n);
    std::vector<T> F(n), G(n);
    T cur = 1;

    for (int i=0;i<n;i++) F[i] = f[i] * math::comb_info<T>::fact[i];
    for (int i=0;i<n;i++,cur*=c) G[n-1-i] = cur * math::comb_info<T>::ifact[i];

    auto H = multiply(F, G);
    std::vector<T> ret(n);
    for (int i=0;i<n;i++) ret[i] = H[n-1+i] * math::comb_info<T>::ifact[i];
    return ret;
}

// given f(0), f(1), ..., f(n-1), returns f(c), f(c+1), ..., f(c+m-1)
// O((n+m) log (n+m))
// https://judge.yosupo.jp/submission/328283
template <class T, internal::is_modint_t<T>* = nullptr>
std::vector<T> shift_of_sampling_points(const std::vector<T> &y, int m, T c){
    if (y.empty()) return std::vector<T>(m, T(0));
    if (m == 0) return {};
    assert(m >= 0);

    int n = y.size();
    math::comb_info<T>::init(n);
    std::vector<T> prf(n+m), iprf(n+m);
    
    prf[0] = (c-n == 0) ? T(1) : (c-n);
    for (int i=1;i<n+m;i++){
        T x = c+i-n;
        if (x == 0) x = 1;
        prf[i] = prf[i-1] * x;
    }

    iprf.back() = prf.back().inv();
    for (int i=n+m-2;i>=0;i--){
        T x = c+i-n+1;
        if (x == 0) x = 1;
        iprf[i] = iprf[i+1] * x;
    }

    std::vector<T> F(n), G(n+m-1);

    T sgn = 1;
    for (int i=n-1;i>=0;i--,sgn=-sgn) F[i] = y[i] * math::comb_info<T>::ifact[n-1-i] * math::comb_info<T>::ifact[i] * sgn;
    for (int i=0;i<n+m-1;i++){
        T x = c+i-n+1;
        G[i] = (x == 0) ? T(0) : (prf[i] * iprf[i+1]);
    }

    auto H = multiply(F, G);
    std::vector<T> ret(m);

    for (int i=0;i<m;i++){
        int p = (c+i).val();
        if (0 <= p && p < n) ret[i] = y[p];
        else ret[i] = H[i+n-1] * prf[i+n] * iprf[i];
    }

    return ret;
}

} // namespace atcoder::poly


using namespace std;
using namespace atcoder::poly;
using ll = long long;
using mint = atcoder::modint998244353;

mint fact[1<<20], factINV[1<<20];

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<mint> f(n);
    for (int i=0;i<n;i++) {int x; cin >> x; f[i] = x;}

    fact[0] = 1;
    for (int i=1;i<=n;i++) fact[i] = fact[i-1] * i;
    factINV[n] = fact[n].inv();
    for (int i=n-1;i>=0;i--) factINV[i] = factINV[i+1] * (i+1);

    for (int i=0;i<n;i++) f[i] *= fact[i];
    reverse(f.begin(), f.end());

    vector<mint> bum(n);
    for (int i=1;i<=n;i++) bum[n-i] = factINV[i];
    reverse(bum.begin(), bum.end());

    auto g = multiply(f, inverse(bum));
    g.resize(n);
    g.push_back(0);
    reverse(g.begin(), g.end());

    for (int i=0;i<=n;i++) printf("%d ", (int)(g[i] * factINV[i]).val());
    printf("\n");
}
