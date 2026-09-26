

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <functional>
#include <cassert>
#include <climits>
#include <iomanip>
#include <numeric>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
#define allof(obj) (obj).begin(), (obj).end()
#define range(i, l, r) for(int i=l;i<r;i++)
#define unique_elem(obj) obj.erase(std::unique(allof(obj)), obj.end())
#define bit_subset(i, S) for(int i=S, zero_cnt=0;(zero_cnt+=i==S)<2;i=(i-1)&S)
#define bit_kpop(i, n, k) for(int i=(1<<k)-1,x_bit,y_bit;i<(1<<n);x_bit=(i&-i),y_bit=i+x_bit,i=(!i?(1<<n):((i&~y_bit)/x_bit>>1)|y_bit))
#define bit_kth(i, k) ((i >> k)&1)
#define bit_highest(i) (i?63-__builtin_clzll(i):-1)
#define bit_lowest(i) (i?__builtin_ctzll(i):-1)
#define sleepms(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
using ll = long long;
using ld = long double;
using ul = uint64_t;
using pi = std::pair<int, int>;
using pl = std::pair<ll, ll>;
using namespace std;

template<typename F, typename S>
std::ostream &operator << (std::ostream &dest, const std::pair<F, S> &p) {
    dest << p.first << ' ' << p.second;
    return dest;
}

template<typename A, typename B>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t);
    return dest;
}

template<typename A, typename B, typename C>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B, C> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t);
    return dest;
}

template<typename A, typename B, typename C, typename D>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B, C, D> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t) << ' ' << std::get<3>(t);
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::vector<std::vector<T>> &v) {
    int sz = v.size();
    if (!sz) return dest;
    for (int i = 0; i < sz; i++) {
        int m = v[i].size();
        for (int j = 0; j < m; j++) dest << v[i][j] << (i != sz - 1 && j == m - 1 ? '\n' : ' ');
    }
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::vector<T> &v) {
    int sz = v.size();
    if (!sz) return dest;
    for (int i = 0; i < sz - 1; i++) dest << v[i] << ' ';
    dest << v[sz - 1];
    return dest;
}

template<typename T, size_t sz>
std::ostream &operator << (std::ostream &dest, const std::array<T, sz> &v) {
    if (!sz) return dest;
    for (int i = 0; i < sz - 1; i++) dest << v[i] << ' ';
    dest << v[sz - 1];
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::set<T> &v) {
    for (auto itr = v.begin(); itr != v.end();) {
        dest << *itr;
        itr++;
        if (itr != v.end()) dest << ' ';
    }
    return dest;
}

template<typename T, typename E>
std::ostream &operator << (std::ostream &dest, const std::map<T, E> &v) {
    for (auto itr = v.begin(); itr != v.end(); ) {
        dest << '(' << itr->first << ", " << itr->second << ')';
        itr++;
        if (itr != v.end()) dest << '\n';
    }
    return dest;
}

template<typename T>
vector<T> make_vec(size_t sz, T val) { return std::vector<T>(sz, val); }

template<typename T, typename... Tail>
auto make_vec(size_t sz, Tail ...tail) {
    return std::vector<decltype(make_vec<T>(tail...))>(sz, make_vec<T>(tail...));
}

template<typename T>
vector<T> read_vec(size_t sz) {
    std::vector<T> v(sz);
    for (int i = 0; i < (int)sz; i++) std::cin >> v[i];
    return v;
}

template<typename T, typename... Tail>
auto read_vec(size_t sz, Tail ...tail) {
    auto v = std::vector<decltype(read_vec<T>(tail...))>(sz);
    for (int i = 0; i < (int)sz; i++) v[i] = read_vec<T>(tail...);
    return v;
}

// x / y以上の最小の整数
ll ceil_div(ll x, ll y) {
    assert(y > 0);
    return (x + (x > 0 ? y - 1 : 0)) / y;
}

// x / y以下の最大の整数
ll floor_div(ll x, ll y) {
    assert(y > 0);
    return (x + (x > 0 ? 0 : -y + 1)) / y;
}

void io_init() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
}


void solveC() {
    long long x, m;
    std::cin >> x >> m;
    long long ans = 0;
    int b = bit_highest(x) + 1;
    // yが[1, 2^b)の場合
    for (int y = 1; y < (1 << b); y++) {
        if (y > m) break;
        int z = x ^ y;
        if (z % x == 0 || z % y == 0) ans++;
    }

    if (m < (1 << b)) {
        std::cout << ans << '\n';
        return;
    }

    // mod x
    auto dp = make_vec<long long>(61, x, 0LL);
    dp[b - 1][0] = 1;
    for (int i = b; i <= 60; i++) {
        ll k = (1LL << b) % x;
        for (int j = 0; j < x; j++) {
            dp[i][j] += dp[i - 1][j];
            dp[i][j] += dp[i - 1][j - k < 0 ? j - k + x : j - k];
        }
    }
    std::vector<long long> table(x, 0LL);
    long long sum = 0;
    for (int i = 60; i >= b; i--) {
        if (!((m >> i) & 1)) continue;
        for (int j = 0; j < x; j++) {
            table[(j + sum >= x ? j + sum - x : j + sum)] += dp[i - 1][j];
        }
        sum += (1LL << i) % x;
        sum %= x;
    }

    // y >= 2^bの場合, x^2 < 2 * yなのでxの倍数の可能性のみ
    for (int i = 0; i < (1 << b); i++) {
        int low_bit = (x ^ i) % x;
        if (low_bit != 0) low_bit = x - low_bit;
        if (low_bit == 0) {
            ans += table[low_bit] - 1;
        } else {
            ans += table[low_bit];
        }
        if (low_bit == sum) {
            if ((m & ((1 << b) - 1)) >= i) {
                ans++;
            }
        }
    }
    std::cout << ans << '\n';
}




#include <type_traits>








// @param m `1 <= m`
constexpr long long safe_mod(long long x, long long m){
  x %= m;
  if (x < 0) x += m;
  return x;
}

// x^n mod m
// @param n `0 <= n`
// @param m `1 <= m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

constexpr __uint128_t pow_mod64_constexpr(__int128_t x, __uint128_t n, unsigned long long m) {
    if (m == 1) return 0;
    __uint128_t r = 1;
    if (x >= m) x %= m;
    if (x < 0) x += m;
    while (n) {
        if (n & 1) r = (r * x) % m;
        x = (x * x) % m;
        n >>= 1;
    }
    return r;
}



constexpr bool miller_rabin32_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) { 
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}

template<int n>
constexpr bool miller_rabin32 = miller_rabin32_constexpr(n);




// -10^18 <= _a, _b <= 10^18
long long gcd(long long _a, long long _b) {
    long long a = abs(_a), b = abs(_b);
    if (a == 0) return b;
    if (b == 0) return a;
    int shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do{
        b >>= __builtin_ctzll(b);
        if(a > b) std::swap(a, b);
        b -= a;
    } while (b);
    return a << shift;
}

// 最大でa*b
// -10^18 <= a, b <= 10^18
// a, bは負でもいいが非負の値を返す
__int128_t lcm(long long a, long long b) {
    a = abs(a), b = abs(b);
    long long g = gcd(a, b);
    if (!g) return 0;
    return __int128_t(a) * b / g;
}

// {x, y, gcd(a, b)} s.t. ax + by = gcd(a, b)
// g >= 0
std::tuple<long long, long long, long long> extgcd(long long a, long long b) {
    long long x, y;
    for (long long u = y = 1, v = x = 0; a;) {
        long long q = b / a;
        std::swap(x -= q * u, u);
        std::swap(y -= q * v, v);
        std::swap(b -= q * a, a);
    }
    // x + k * (b / g), y - k * (a / g) も条件を満たす(kは任意の整数)
    return {x, y, b};
}

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;
    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}



template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
struct modint32_static {
    using mint = modint32_static;
  public:
    static constexpr int mod() { return m; }
    
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }
  
    modint32_static(): _v(0) {}
    
    template <class T>
    modint32_static(T v) { 
        long long x = v % (long long)umod();
        if (x < 0) x += umod();
        _v = x;
    }

    unsigned int val() const { return _v; }
    
    mint& operator ++ () {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator -- () {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }
    mint operator ++ (int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator -- (int) {
        mint result = *this;
        --*this;
        return result;
    }
    mint& operator += (const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator -= (const mint& rhs) {
        _v -= rhs._v;
        if (_v >= umod()) _v += umod();
        return *this;
    }
    mint& operator *= (const mint& rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        _v = (unsigned int)(z % umod());
        return *this;
    }
    mint& operator /= (const mint& rhs) { return *this = *this * rhs.inv(); }
    mint operator + () const { return *this; }
    mint operator - () const { return mint() - *this; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        if (prime) {
            assert(_v);
            return pow(umod() - 2);
        } else {
            auto eg = inv_gcd(_v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }
    friend mint operator + (const mint& lhs, const mint& rhs) { return mint(lhs) += rhs; }
    friend mint operator - (const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs; }
    friend mint operator * (const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs; }
    friend mint operator / (const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs; }
    friend bool operator == (const mint& lhs, const mint& rhs) { return lhs._v == rhs._v; }
    friend bool operator != (const mint& lhs, const mint& rhs) { return lhs._v != rhs._v; }
  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
    static constexpr bool prime = miller_rabin32<m>;
};

template<int m>
std::ostream &operator<<(std::ostream &dest, const modint32_static<m> &a) {
    dest << a.val();
    return dest;
}

using modint998244353 = modint32_static<998244353>;
using modint1000000007 = modint32_static<1000000007>;


std::ostream &operator<<(std::ostream &dest, __int128_t value) {
  std::ostream::sentry s(dest);
  if (s) {
    __uint128_t tmp = value < 0 ? -value : value;
    char buffer[128];
    char *d = std::end(buffer);
    do {
      --d;
      *d = "0123456789"[tmp % 10];
      tmp /= 10;
    } while (tmp != 0);
    if (value < 0) {
      --d;
      *d = '-';
    }
    int len = std::end(buffer) - d;
    if (dest.rdbuf()->sputn(d, len) != len) {
      dest.setstate(std::ios_base::badbit);
    }
  }
  return dest;
}

__int128 parse(string &s) {
  __int128 ret = 0;
  for (int i = 0; i < s.length(); i++)
    if ('0' <= s[i] && s[i] <= '9')
      ret = 10 * ret + s[i] - '0';
  return ret;
}

/*
1*2^64 18446744073709551616
29505444490 18446744057541225032
29505444491 18446744087046669523
2*2^64 36893488147419103232
42038977030 36893488113593000470
42038977031 36893488155631977501
3*2^64 55340232221128654848
51709137940 55340232192303923849
51709137941 55340232244013061790
4*2^64 73786976294838206464
59889909838 73786976273265210003
59889909839 73786976333155119842
5*2^64 92233720368547758080
67115811582 92233720359243360654
67115811583 92233720426359172237
6*2^64 110680464442257309696
73661647780 110680464437376944828
73661647781 110680464511038592609
7*2^64 129127208515966861312
79691274348 129127208447381912454
79691274349 129127208527073186803
8*2^64 147573952589676412928
85311533310 147573952563371882853
85311533311 147573952648683416164
9*2^64 166020696663385964544
90596891746 166020696631522213306
90596891747 166020696722119105053
10*2^64 184467440737095516160
95601307336 184467440710950310454
95601307337 184467440806551617791
*/

// N <= 10^11
template<typename T>
struct multiplicative_function_small {
  private:
    // [i] = [1, n]の素数の和が(i+1)*2^64以上になる最小のn
    static constexpr std::array<long long, 11> psum_overflow_n = {29505444491, 42038977031, 51709137941, 59889909839, 
        67115811583, 73661647781, 79691274349, 85311533311, 90596891747, 95601307337, 100365890297};
    uint64_t n;
    int sq;
    std::vector<bool> is_prime;
    std::vector<uint32_t> cnt_prime, primes;
    std::vector<uint64_t> Q;

    static std::vector<uint64_t> enumerate_quotients(uint64_t x) {
        assert(x >= 0);
        if (x == 0) return {};
        uint64_t sq = sqrtl(x);
        std::vector<uint64_t> ans(sq);
        std::iota(ans.begin(), ans.end(), 1);
        if (x / sq == sq) sq--;
        for (long long i = sq; i >= 1; i--) ans.push_back(x / i);
        return ans;
    }
  
  public:
    multiplicative_function_small(uint64_t _n) : n(_n), sq(sqrt(n)), is_prime(sq + 1, true), cnt_prime(sq + 1) {
        assert(_n <= 100'000'000'000);
        is_prime[0] = is_prime[1] = false;
        cnt_prime[0] = cnt_prime[1] = 0;
        for (int p = 2; p <= sq; p++) {
            cnt_prime[p] = cnt_prime[p - 1] + is_prime[p];
            if (!is_prime[p]) continue;
            primes.push_back(p);
            for (int i = 2 * p; i <= sq; i += p) is_prime[i] = false;
        }
        Q = enumerate_quotients(n);
    }

    std::vector<uint64_t> quotients() const {
        return Q;
    }

    // res[i] := [0, Q[i]の素数の数
    std::vector<uint32_t> counting_primes_quotients() const {
        assert(n >= 0);
        if (n == 0) return {};
        std::vector<uint32_t> Fprime = cnt_prime;
        int W = Q.size();
        std::vector<uint32_t> f(W);
        for (int i = 0; i < W; i++) f[i] = Q[i] - 1;
        for (int p = 2; p <= sq; p++) {
            if (!is_prime[p]) continue;
            long long p2 = (long long)p * p;
            if (p2 <= sq) {
                for (int i = W - 1, j = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    while (j >= 0 && Q[j] * p > Q[i]) j--;
                    f[i] -= f[j] - Fprime[p - 1];
                }
            } else {
                for (int i = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    long long x = Q[i] / p;
                    int j = (x <= sq ? x - 1 : W - n /x);
                    f[i] -= f[j] - Fprime[p - 1];
                }
            }
        }
        return f;
    }

    // res[i] := [0, Q[i]の素数の和
    std::vector<__uint128_t> summation_primes_quotients() const {
        assert(n >= 0);
        if (n == 0) return {};
        std::vector<uint64_t> Fprime(sq + 1, 0);
        for (int p = 2; p <= sq; p++) {
            Fprime[p] = Fprime[p - 1] + (is_prime[p] ? p : 0);
        }
        int W = Q.size();
        std::vector<uint64_t> f(W);
        for (int i = 0; i < W; i++) f[i] = (__uint128_t(Q[i]) * (Q[i] + 1)) / 2 - 1;
        for (int p = 2; p <= sq; p++) {
            if (!is_prime[p]) continue;
            long long p2 = (long long)p * p;
            if (p2 <= sq) {
                for (int i = W - 1, j = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    while (j >= 0 && Q[j] * p > Q[i]) j--;
                    f[i] -= p * (f[j] - Fprime[p - 1]);
                }
            } else {
                for (int i = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    long long x = Q[i] / p;
                    int j = (x <= sq ? x - 1 : W - n / x);
                    f[i] -= p * (f[j] - Fprime[p - 1]);
                }
            }
        }

        std::vector<__uint128_t> res(W);
        for (int i = 0; i < W; i++) {
            res[i] = f[i];
            int j = 0;
            while (Q[i] >= psum_overflow_n[j++]) res[i] += __uint128_t(1) << 64;
        }
        return res;
    }
    
    // res[i] := [0, Q[i]の素数pのa + bpの和
    std::vector<T> a_bp_quotients(T a, T b) const {
        assert(n >= 0);
        if (n == 0) return {};
        std::vector<uint64_t> Fprime(sq + 1, 0);
        std::vector<uint32_t> Gprime(sq + 1, 0);

        for (int p = 2; p <= sq; p++) {
            Fprime[p] = Fprime[p - 1] + (is_prime[p] ? p : 0);
            Gprime[p] = Gprime[p - 1] + (is_prime[p] ? 1 : 0);
        }
        int W = Q.size();
        std::vector<uint64_t> f(W);
        std::vector<uint32_t> g(W);
        for (int i = 0; i < W; i++) {
            f[i] = (__uint128_t(Q[i]) * (Q[i] + 1)) / 2 - 1;
            g[i] = Q[i] - 1;
        }
        for (int p = 2; p <= sq; p++) {
            if (!is_prime[p]) continue;
            long long p2 = (long long)p * p;
            if (p2 <= sq) {
                for (int i = W - 1, j = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    while (j >= 0 && Q[j] * p > Q[i]) j--;
                    f[i] -= p * (f[j] - Fprime[p - 1]);
                    g[i] -= (g[j] - Gprime[p - 1]);
                }
            } else {
                for (int i = W - 1; i >= 0 && Q[i] >= p2; i--) {
                    long long x = Q[i] / p;
                    int j = (x <= sq ? x - 1 : W - n / x);
                    f[i] -= p * (f[j] - Fprime[p - 1]);
                    g[i] -= (g[j] - Gprime[p - 1]);
                }
            }
        }
        std::vector<T> res(W);
        for (int i = 0; i < W; i++) {
            T s = f[i];
            int j = 0;
            while (Q[i] >= psum_overflow_n[j++]) s += T(1LL << 32) * T(1LL << 32);
            res[i] = s * b + g[i] * a;
        }
        return res;
    }

    long long counting_primes() const {
        if (n == 0) return 0;
        return counting_primes_quotients().back();
    }

    T summation_primes() const {
        if (n == 0) return 0;
        return summation_primes_quotients().back();
    }

    T totient_sum_primes() const {
        if (n == 0) return 0;
        return a_bp_quotients(-1, 1).back();
    }
    
    /*
    T totient_sum() const {
        auto Fprime = a_bp_quotients(-1, 1);
        int W = Q.size();

        auto dfs = [&](auto &&dfs, int L, long long x, long long pp) -> T {
            T res = 0;
            int last_p = (L == 0 ? 0 : primes[L - 1]);
            T f;
            long long mullimit = n / x, y = mullimit;
            int idx = (y <= sq ? y - 1 : W - x);
            if (last_p == 0) {
                f = 1;
                res += Fprime[idx];
            } else {
                f = (last_p - 1) * pp;
                res += (last_p - 1) * pp * last_p;
                if ((long long)last_p * last_p <= mullimit) res += dfs(dfs, L, x * last_p, pp * last_p);
                if (idx >= last_p) res += f * (Fprime[idx] - Fprime[last_p - 1]);
            }
            for (int i = L; i < int(primes.size()); i++) {
                int p = primes[i];
                if ((long long)p * p > mullimit) break;
                res += f * dfs(dfs, i + 1, x * p, 1);
            }
            return res;
        };
        return dfs(dfs, 0, 1, 0) + 1;
    }
    */

    // f(p^e) = ae + bpのsum
    T ae_bp(T a, T b) const {
        auto Fprime = a_bp_quotients(a, b);
        int W = Q.size();

        auto dfs = [&](auto &&dfs, int L, long long x, T f) -> T {
            T res = 0;
            int last_p = (L == 0 ? 0 : primes[L - 1]);
            long long y = n / x;
            int idx = (y <= sq ? y - 1 : W - x);
            if (last_p == 0) {
                res += Fprime[idx];
            } else {
                res += f + a;
                if ((long long)last_p * last_p <= y) res += dfs(dfs, L, x * last_p, f + a);
                if (idx >= last_p) res += f * (Fprime[idx] - Fprime[last_p - 1]);
            }
            for (int i = L; i < int(primes.size()); i++) {
                int p = primes[i];
                if ((long long)p * p > y) break;
                res += f * dfs(dfs, i + 1, x * p, a + b * p);
            }
            return res;
        };
        return dfs(dfs, 0, 1, 1) + 1;
    }
};

int main() {
    io_init();
    int T;
    std::cin >> T;
    for (int i = 0; i < T; i++) {
        long long N, a, b;
        std::cin >> N >> a >> b;
        multiplicative_function_small<modint32_static<469762049>> f(N);
        std::cout << f.ae_bp(a, b) << '\n';
    }
}
