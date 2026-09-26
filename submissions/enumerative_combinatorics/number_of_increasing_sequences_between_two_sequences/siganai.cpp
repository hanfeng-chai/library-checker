#line 1 "main.cpp"
#include<bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include <debug.hpp>
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) (static_cast<void>(0))
#endif
//#pragma GCC target("avx,avx2")
//#pragma GCC optimize("O3")
//#pragma GCC optimize("unroll-loops")
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pll = pair<ll,ll>;
using pii = pair<int,int>;
using vi = vector<int>;
using vvi = vector<vi>;
using vvvi = vector<vvi>;
using vl = vector<ll>;
using vvl = vector<vl>;
using vvvl = vector<vvl>;
using vul = vector<ull>;
using vpii = vector<pii>;
using vvpii = vector<vpii>;
using vpll = vector<pll>;
using vvpll = vector<vpll>;
using vs = vector<string>;
template<class T> using pq = priority_queue<T,vector<T>, greater<T>>;
#define overload4(_1, _2, _3, _4, name, ...) name
#define overload3(a,b,c,name,...) name
#define rep1(n) for (ll UNUSED_NUMBER = 0; UNUSED_NUMBER < (n); ++UNUSED_NUMBER)
#define rep2(i, n) for (ll i = 0; i < (n); ++i)
#define rep3(i, a, b) for (ll i = (a); i < (b); ++i)
#define rep4(i, a, b, c) for (ll i = (a); i < (b); i += (c))
#define rep(...) overload4(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)
#define rrep1(n) for(ll i = (n) - 1;i >= 0;i--)
#define rrep2(i,n) for(ll i = (n) - 1;i >= 0;i--)
#define rrep3(i,a,b) for(ll i = (b) - 1;i >= (a);i--)
#define rrep4(i,a,b,c) for(ll i = (a) + (((b)-(a)-1) / (c) - (((b)-(a)-1) % (c) && (((b)-(a)-1) ^ c) < 0)) * (c);i >= (a);i -= c)
#define rrep(...) overload4(__VA_ARGS__, rrep4, rrep3, rrep2, rrep1)(__VA_ARGS__)
#define all1(i) begin(i) , end(i)
#define all2(i,a) begin(i) , begin(i) + a
#define all3(i,a,b) begin(i) + a , begin(i) + b
#define all(...) overload3(__VA_ARGS__, all3, all2, all1)(__VA_ARGS__)
#define sum(...) accumulate(all(__VA_ARGS__),0LL)
template<class T> bool chmin(T &a, const T &b){ if(a > b){ a = b; return 1; } else return 0; }
template<class T> bool chmax(T &a, const T &b){ if(a < b){ a = b; return 1; } else return 0; }
template<class T> auto min(const T& a){return *min_element(all(a));}
template<class T> auto max(const T& a){return *max_element(all(a));}
template<class... Ts> void in(Ts&... t);
#define INT(...) int __VA_ARGS__; in(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__; in(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; in(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; in(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__; in(__VA_ARGS__)
#define LD(...) ld __VA_ARGS__; in(__VA_ARGS__)
#define VEC(type, name, size) vector<type> name(size); in(name)
#define VV(type, name, h, w) vector<vector<type>> name(h, vector<type>(w)); in(name)
ll intpow(ll a, ll b){ll ans = 1; while(b){if(b & 1) ans *= a; a *= a; b /= 2;} return ans;}
ll modpow(ll a, ll b, ll p){ ll ans = 1 % p; a %= p;if(a < 0) a += p;while(b){ if(b & 1) (ans *= a) %= p; (a *= a) %= p; b /= 2; } return ans; }
bool is_clamp(ll val,ll low,ll high) {return low <= val && val < high;}
void Yes() {cout << "Yes\n";return;}
void No() {cout << "No\n";return;}
void YES() {cout << "YES\n";return;}
void NO() {cout << "NO\n";return;}
template <typename U,typename T>
U floor(U a, T b) {return a / b - (a % b && (a ^ b) < 0);}
// ceil(x,y) = floor(x+y-1,y)なのでx+y-1がoverflowする可能性あり
template <typename U,typename T>
U ceil(U x, T y) {return floor(x + y - 1, y);}
template <typename U,typename T>
T bmod(U x, T y) {return x - y * floor(x, y);}
template <typename U,typename T>
pair<U, T> divmod(U x, T y) {U q = floor(x, y);return {q, x - q * y};}
namespace IO{
#define VOID(a) decltype(void(a))
struct setting{ setting(){cin.tie(nullptr); ios::sync_with_stdio(false);fixed(cout); cout.precision(15);}} setting;
template<int I> struct P : P<I-1>{};
template<> struct P<0>{};
template<class T> void i(T& t){ i(t, P<3>{}); }
void i(vector<bool>::reference t, P<3>){ int a; i(a); t = a; }
template<class T> auto i(T& t, P<2>) -> VOID(cin >> t){ cin >> t; }
template<class T> auto i(T& t, P<1>) -> VOID(begin(t)){ for(auto&& x : t) i(x); }
template<class T, size_t... idx> void ituple(T& t, index_sequence<idx...>){in(get<idx>(t)...);}
template<class T> auto i(T& t, P<0>) -> VOID(tuple_size<T>{}){ituple(t, make_index_sequence<tuple_size<T>::value>{});} 
#undef VOID
}
#define unpack(a) (void)initializer_list<int>{(a, 0)...}
template<class... Ts> void in(Ts&... t){ unpack(IO :: i(t)); }
#undef unpack
constexpr long double PI = 3.141592653589793238462643383279L;
template <class F> struct REC {
    F f;
    REC(F &&f_) : f(forward<F>(f_)) {}
    template <class... Args> auto operator()(Args &&...args) const { return f(*this, forward<Args>(args)...); }};

constexpr int mod = 998244353;
//constexpr int mod = 1000000007;
#line 2 "misc/fastio.hpp"

#line 105 "main.cpp"
#include <type_traits>
#line 107 "main.cpp"

using namespace std;

#line 2 "internal/internal-type-traits.hpp"

#line 4 "internal/internal-type-traits.hpp"
using namespace std;

namespace internal {
template <typename T>
using is_broadly_integral =
    typename conditional_t<is_integral_v<T> || is_same_v<T, __int128_t> ||
                               is_same_v<T, __uint128_t>,
                           true_type, false_type>::type;

template <typename T>
using is_broadly_signed =
    typename conditional_t<is_signed_v<T> || is_same_v<T, __int128_t>,
                           true_type, false_type>::type;

template <typename T>
using is_broadly_unsigned =
    typename conditional_t<is_unsigned_v<T> || is_same_v<T, __uint128_t>,
                           true_type, false_type>::type;

#define ENABLE_VALUE(x) \
  template <typename T> \
  constexpr bool x##_v = x<T>::value;

ENABLE_VALUE(is_broadly_integral);
ENABLE_VALUE(is_broadly_signed);
ENABLE_VALUE(is_broadly_unsigned);
#undef ENABLE_VALUE

#define ENABLE_HAS_TYPE(var)                                   \
  template <class, class = void>                               \
  struct has_##var : false_type {};                            \
  template <class T>                                           \
  struct has_##var<T, void_t<typename T::var>> : true_type {}; \
  template <class T>                                           \
  constexpr auto has_##var##_v = has_##var<T>::value;

#define ENABLE_HAS_VAR(var)                                     \
  template <class, class = void>                                \
  struct has_##var : false_type {};                             \
  template <class T>                                            \
  struct has_##var<T, void_t<decltype(T::var)>> : true_type {}; \
  template <class T>                                            \
  constexpr auto has_##var##_v = has_##var<T>::value;

}  // namespace internal
#line 12 "misc/fastio.hpp"

namespace fastio {
static constexpr int SZ = 1 << 17;
static constexpr int offset = 64;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre {
  char num[40000];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i * 4 + j] = n % 10 + '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

void load() {
  int len = in_right - in_left;
  memmove(inbuf, inbuf + in_left, len);
  in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
  in_left = 0;
}
void flush() {
  fwrite(outbuf, 1, out_right, stdout);
  out_right = 0;
}
void skip_space() {
  if (in_left + offset > in_right) load();
  while (inbuf[in_left] <= ' ') in_left++;
}

void single_read(char& c) {
  if (in_left + offset > in_right) load();
  skip_space();
  c = inbuf[in_left++];
}
void single_read(string& S) {
  skip_space();
  while (true) {
    if (in_left == in_right) load();
    int i = in_left;
    for (; i != in_right; i++) {
      if (inbuf[i] <= ' ') break;
    }
    copy(inbuf + in_left, inbuf + i, back_inserter(S));
    in_left = i;
    if (i != in_right) break;
  }
}
template <typename T,
          enable_if_t<internal::is_broadly_integral_v<T>>* = nullptr>
void single_read(T& x) {
  if (in_left + offset > in_right) load();
  skip_space();
  char c = inbuf[in_left++];
  [[maybe_unused]] bool minus = false;
  if constexpr (internal::is_broadly_signed_v<T>) {
    if (c == '-') minus = true, c = inbuf[in_left++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = inbuf[in_left++];
  }
  if constexpr (internal::is_broadly_signed_v<T>) {
    if (minus) x = -x;
  }
}
void rd() {}
template <typename Head, typename... Tail>
void rd(Head& head, Tail&... tail) {
  single_read(head);
  rd(tail...);
}

void single_write(const char& c) {
  if (out_right > SZ - offset) flush();
  outbuf[out_right++] = c;
}
void single_write(const bool& b) {
  if (out_right > SZ - offset) flush();
  outbuf[out_right++] = b ? '1' : '0';
}
void single_write(const string& S) {
  flush(), fwrite(S.data(), 1, S.size(), stdout);
}
void single_write(const char* p) { flush(), fwrite(p, 1, strlen(p), stdout); }
template <typename T,
          enable_if_t<internal::is_broadly_integral_v<T>>* = nullptr>
void single_write(const T& _x) {
  if (out_right > SZ - offset) flush();
  if (_x == 0) {
    outbuf[out_right++] = '0';
    return;
  }
  T x = _x;
  if constexpr (internal::is_broadly_signed_v<T>) {
    if (x < 0) outbuf[out_right++] = '-', x = -x;
  }
  constexpr int buffer_size = sizeof(T) * 10 / 4;
  char buf[buffer_size];
  int i = buffer_size;
  while (x >= 10000) {
    i -= 4;
    memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
    x /= 10000;
  }
  if (x < 100) {
    if (x < 10) {
      outbuf[out_right] = '0' + x;
      ++out_right;
    } else {
      uint32_t q = (uint32_t(x) * 205) >> 11;
      uint32_t r = uint32_t(x) - q * 10;
      outbuf[out_right] = '0' + q;
      outbuf[out_right + 1] = '0' + r;
      out_right += 2;
    }
  } else {
    if (x < 1000) {
      memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
      out_right += 3;
    } else {
      memcpy(outbuf + out_right, pre.num + (x << 2), 4);
      out_right += 4;
    }
  }
  memcpy(outbuf + out_right, buf + i, buffer_size - i);
  out_right += buffer_size - i;
}
void wt() {}
template <typename Head, typename... Tail>
void wt(const Head& head, const Tail&... tail) {
  single_write(head);
  wt(std::forward<const Tail>(tail)...);
}
template <typename... Args>
void wtn(const Args&... x) {
  wt(std::forward<const Args>(x)...);
  wt('\n');
}

struct Dummy {
  Dummy() { atexit(flush); }
} dummy;

}  // namespace fastio
using fastio::rd;
using fastio::skip_space;
using fastio::wt;
using fastio::wtn;
#line 2 "library/modint/LazyMontgomeryModint.hpp"
template <uint32_t mod>
struct LazyMontgomeryModInt {
    using mint = LazyMontgomeryModInt;
    using i32 = int32_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    static constexpr u32 get_r() {
        u32 ret = mod;
        for (i32 i = 0; i < 4; ++i) ret *= 2 - mod * ret;
        return ret;
    }
    static constexpr u32 r = get_r();
    static constexpr u32 n2 = -u64(mod) % mod;
    static_assert(r * mod == 1);
    static_assert(mod < (1 << 30));
    static_assert((mod & 1) == 1);
    u32 a;
    constexpr LazyMontgomeryModInt() : a(0) {}
    constexpr LazyMontgomeryModInt(const int64_t &b)
      : a(reduce(u64(b % mod + mod) * n2)){};

    static constexpr u32 reduce(const u64 &b) {
        return (b + u64(u32(b) * u32(-r)) * mod) >> 32;
    }
    constexpr mint &operator+=(const mint &b) {
        if (i32(a += b.a - 2 * mod) < 0) a += 2 * mod;
        return *this;
    }
    constexpr mint &operator-=(const mint &b) {
        if (i32(a -= b.a) < 0) a += 2 * mod;
        return *this;
    }
    constexpr mint &operator*=(const mint &b) {
        a = reduce(u64(a) * b.a);
        return *this;
    }
    constexpr mint &operator/=(const mint &b) {
        *this *= b.inverse();
        return *this;
    }
    constexpr mint operator+(const mint &b) const { return mint(*this) += b; }
    constexpr mint operator-(const mint &b) const { return mint(*this) -= b; }
    constexpr mint operator*(const mint &b) const { return mint(*this) *= b; }
    constexpr mint operator/(const mint &b) const { return mint(*this) /= b; }
    constexpr bool operator==(const mint &b) const {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr bool operator!=(const mint &b) const {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr mint operator-() const { return mint() - mint(*this); }
    constexpr mint pow(u64 n) const {
        mint ret(1), mul(*this);
        while (n > 0) {
        if (n & 1) ret *= mul;
        mul *= mul;
        n >>= 1;
        }
        return ret;
    }
    constexpr mint inverse() const { return pow(mod - 2); }
    friend ostream &operator<<(ostream &os, const mint &b) {
        return os << b.get();
    }
    friend istream &operator>>(istream &is, mint &b) {
        int64_t t;
        is >> t;
        b = LazyMontgomeryModInt<mod>(t);
        return (is);
    }
    constexpr u32 get() const {
        u32 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }
    static constexpr u32 get_mod() { return mod; }
};
#line 315 "main.cpp"
using mint = LazyMontgomeryModInt<mod>;
using vm = vector<mint>;
using vvm = vector<vm>;
using vvvm = vector<vvm>;
#line 2 "library/modulo/binomial.hpp"

template <typename T>
struct Binomial {
  vector<T> f, g, h;
  Binomial(int MAX = 0) {
    assert(T::get_mod() != 0 && "Binomial<mint>()");
    f.resize(1, T{1});
    g.resize(1, T{1});
    h.resize(1, T{1});
    while (MAX >= (int)f.size()) extend();
  }
  void extend() {
    int n = f.size();
    int m = n * 2;
    f.resize(m);
    g.resize(m);
    h.resize(m);
    for (int i = n; i < m; i++) f[i] = f[i - 1] * T(i);
    g[m - 1] = f[m - 1].inverse();
    h[m - 1] = g[m - 1] * f[m - 2];
    for (int i = m - 2; i >= n; i--) {
      g[i] = g[i + 1] * T(i + 1);
      h[i] = g[i] * f[i - 1];
    }
  }
  T fac(int i) {
    if (i < 0) return T(0);
    while (i >= (int)f.size()) extend();
    return f[i];
  }

  T finv(int i) {
    if (i < 0) return T(0);
    while (i >= (int)g.size()) extend();
    return g[i];
  }

  T inv(int i) {
    if (i < 0) return -inv(-i);
    while (i >= (int)h.size()) extend();
    return h[i];
  }

  T C(int n, int r) {
    if (n < 0 || n < r || r < 0) return T(0);
    return fac(n) * finv(n - r) * finv(r);
  }

  inline T operator()(int n, int r) { return C(n, r); }

  template <typename I>
  T multinomial(const vector<I>& r) {
    static_assert(is_integral<I>::value == true);
    int n = 0;
    for (auto& x : r) {
      if (x < 0) return T(0);
      n += x;
    }
    T res = fac(n);
    for (auto& x : r) res *= finv(x);
    return res;
  }

  template <typename I>
  T operator()(const vector<I>& r) {
    return multinomial(r);
  }

  T C_naive(int n, int r) {
    if (n < 0 || n < r || r < 0) return T(0);
    T ret = T(1);
    r = min(r, n - r);
    for (int i = 1; i <= r; ++i) ret *= inv(i) * (n--);
    return ret;
  }

  T P(int n, int r) {
    if (n < 0 || n < r || r < 0) return T(0);
    return fac(n) * finv(n - r);
  }

  T H(int n, int r) {
    if (n < 0 || r < 0) return T(0);
    return r == 0 ? 1 : C(n + r - 1, r);
  }
};
#line 2 "library/ntt/ntt.hpp"
template<typename mint>
struct NTT{
    static constexpr uint32_t get_pr() {
        uint32_t _mod = mint::get_mod();
        using u64 = uint64_t;
        u64 ds[32] = {};
        int idx = 0;
        u64 m = _mod - 1;
        for(u64 i = 2;i * i <= m; ++i) {
            if(m % i == 0) {
                ds[idx++] = i;
                while(m % i == 0) m /= i;
            }
        }
        if (m != 1) ds[idx++] = m;
        uint32_t _pr = 2;
        while(1) {
            int flg = 1;
            for(int i = 0;i < idx; ++i) {
                u64 a = _pr, b = (_mod - 1) / ds[i],r = 1;
                while(b) {
                    if(b & 1) r = r * a % _mod;
                    a = a * a % _mod;
                    b >>= 1;
                }
                if(r == 1) {
                    flg = 0;
                    break;
                }
            }
            if (flg == 1) break;
            ++_pr;
        }
        return _pr;
    };
    static constexpr uint32_t mod = mint::get_mod();
    static constexpr uint32_t pr = get_pr();
    static constexpr int level = __builtin_ctzll(mod - 1);
    mint dw[level], dy[level];
    void setwy(int k) {
        mint w[level],y[level];
        w[k - 1] = mint(pr).pow((mod - 1) / (1 << k));
        y[k - 1] = w[k - 1].inverse();
        for(int i = k - 2;i > 0; --i) w[i] = w[i+1] * w[i+1],y[i] = y[i+1] * y[i+1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        for(int i = 3;i < k;++i) {
            dw[i] = dw[i-1] * y[i-2] * w[i];
            dy[i] = dy[i-1] * w[i-2] * y[i];
        }
    }
    NTT() {setwy(level);}
    void fft4(vector<mint> &a,int k) {
        if((int)a.size() <= 1) return;
        if(k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        if (k & 1) {
            int v = 1 << (k - 1);
            for(int j = 0;j < v; ++j) {
                mint ajv = a[j + v];
                a[j + v] = a[j] - ajv;
                a[j] += ajv;
            }
        }
        int u = 1 << (2 + (k & 1));
        int v = 1 << (k - 2 - (k & 1));
        mint one = mint(1);
        mint imag = dw[1];
        while(v) {
            {
                int j0 = 0,j1 = v;
                int j2 = j1 + v;
                int j3 = j2 + v;
                for(;j0 < v; ++j0,++j1,++j2,++j3) {
                    mint t0 = a[j0], t1 = a[j1],t2 = a[j2],t3 = a[j3];
                    mint t0p2 = t0 + t2,t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2,t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            mint ww = one,xx = one * dw[2],wx = one;
            for(int jh = 4;jh < u;) {
                ww = xx * xx,wx = ww * xx;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for(;j0 < je;++j0,++j2) {
                    mint t0 = a[j0], t1 = a[j0 + v] * xx, t2 = a[j2] * ww,t3 = a[j2 + v] * wx;
                    mint t0p2 = t0 + t2,t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2,t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j0 + v] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j2 + v] = t0m2 - t1m3;
                }
                xx *= dw[__builtin_ctzll((jh += 4))];
            }
            u <<= 2;
            v >>= 2;
        }
    }
    void ifft4(vector<mint> &a,int k) {
        if((int)a.size() <= 1) return;
        if(k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        int u = 1 << (k - 2);
        int v = 1;
        mint one = mint(1);
        mint imag = dy[1];
        while(u) {
            {
                int j0 = 0,j1 = v;
                int j2 = j1 + v;
                int j3 = j2 + v;
                for(;j0 < v;++j0,++j1,++j2,++j3) {
                    mint t0 = a[j0],t1 = a[j1],t2 = a[j2],t3 = a[j3];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                    a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                    a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
                }
            }
            mint ww = one,xx = one * dy[2],yy = one;
            u <<= 2;
            for(int jh = 4;jh < u;) {
                ww = xx * xx,yy = xx * imag;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for(;j0 < je;++j0,++j2) {
                    mint t0 = a[j0], t1 = a[j0 + v], t2 = a[j2], t3 = a[j2 + v];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j0 + v] = t0m1 + t2m3, a[j2 + v] = (t0m1 - t2m3) * ww;       
                }
                xx *= dy[__builtin_ctzll(jh += 4)];
            }
            u >>= 4;
            v <<= 2;
        }
        if(k & 1) {
            u = 1 << (k - 1);
            for(int j = 0;j < u;++j) {
                mint ajv = a[j] - a[j+u];
                a[j] += a[j+u];
                a[j+u] = ajv;
            }
        }
    }
    void ntt(vector<mint> &a) {
        if((int)a.size() <= 1) return;
        fft4(a,__builtin_ctz(a.size()));
    }
    void intt(vector<mint> &a) {
        if((int)a.size() <= 1) return;
        ifft4(a,__builtin_ctz(a.size()));
        mint iv = mint(a.size()).inverse();
        for(auto &x:a) x *= iv;
    }
    vector<mint> multiply(const vector<mint> &a,const vector<mint> &b) {
        int l = a.size() + b.size() - 1;
        if(min<int>(a.size(),b.size()) <= 40) {
            vector<mint> s(l);
            for(int i = 0;i < (int)a.size();++i) for(int j = 0;j < (int)b.size();++j) s[i+j] += a[i] * b[j];
            return s;
        }
        int k = 2, M = 4;
        while(M < l) M <<= 1, ++k;
        //setwy(k);
        vector<mint> s(M), t(M);
        for(int i = 0;i < (int)a.size();++i) s[i] = a[i];
        for(int i = 0;i < (int)b.size();++i) t[i] = b[i];
        fft4(s,k);
        fft4(t,k);
        for(int i = 0;i < M;++i) s[i] *= t[i];
        ifft4(s,k);
        s.resize(l);
        mint invm = mint(M).inverse();
        for(int i = 0;i < l;++i) s[i] *= invm;
        return s;
    }
    void ntt_doubling(vector<mint> &a) {
        int M = (int)a.size();
        auto b = a;
        intt(b);
        mint r = 1, zeta = mint(pr).pow((mint::get_mod() - 1) / (M << 1));
        for(int i = 0;i < M;++i) b[i] *= r,r *= zeta;
        ntt(b);
        copy(begin(b),end(b),back_inserter(a));
    }
};
#line 321 "main.cpp"
NTT<mint> ntt;
template<class mint>
pair<vector<mint>, vector<mint>> count_square(vector<mint> L, vector<mint> D,Binomial<mint> &C){
    assert(!L.empty() && !D.empty());
    int N = L.size();
    int M = D.size();
    if (min(N, M) <= 400){
        int sw = 0;
        if (N > M) swap(N, M), swap(L, D), sw = 1;
        vector<mint> R(N);
        for (int i = 0; i < N; i++){
            D[0] += L[i];
            for (int j = 1; j < M; j++) D[j] += D[j - 1];
            R[i] = D.back();
        }
        if (sw) swap(R, D);
        return {R, D};
    }
    auto cyclic_convolution = [&](vector<mint> f,vector<mint> g) {
        ntt.ntt(f);
        ntt.ntt(g);
        for(int i = 0;i < (int)f.size();i++) f[i] *= g[i];
        ntt.intt(f);
        return f;
    };
    vector<mint> R(N), U(M);
    int z = 0;
    while ((1 << z) < (N + M - 1)) z++;
    // 左から右
    {
        vector<mint> tmp(N);
        for (int i = 0; i < N; i++) tmp[i] = C.C(M - 1 + i, i);
        tmp = ntt.multiply(tmp, L);
        for (int i = 0; i < N; i++) R[i] += tmp[i];
    }
    // 左から上
    {
        vector<mint> tmp(1 << z);
        for (int i = 0; i < N; i++) L[i] *= C.finv(N - 1 - i);
        for (int i = 0; i < N + M - 1; i++) tmp[i] = C.fac(i);
        L.resize(1 << z,0);
        tmp = cyclic_convolution(tmp,L);
        for (int i = 0; i < M; i++) U[i] += tmp[N - 1 + i] * C.finv(i);
    }
    // 下から上
    {
        vector<mint> tmp(M);
        for (int i = 0; i < M; i++) tmp[i] = C.C(N - 1 + i, i);
        tmp = ntt.multiply(tmp, D);
        for (int i = 0; i < M; i++) U[i] += tmp[i];
    }
    // 下から右
    {
        vector<mint> tmp(1 << z);
        for (int i = 0; i < M; i++) D[i] *= C.finv(M - i - 1);
        for (int i = 0; i < N + M - 1; i++) tmp[i] = C.fac(i);
        D.resize(1 << z,0);
        tmp = cyclic_convolution(tmp, D);
        for (int i = 0; i < N; i++) R[i] += tmp[M - 1 + i] * C.finv(i);
    }
    return {R, U};
}
template<class mint>
/*
 * g(A, x) を
 * 0 <= B[i] < A[i] かつ B[i] = x を満たす
 * 広義単調増加列 B の数とする
 * res[x] = sum C[i] * g(A[i:N], x)
 * を返す
 */
vector<mint> count_increase_sequences_with_upper_bounds(Binomial<mint> &BC, vector<int> A, vector<mint> C){
    int N = A.size();
    assert((int)C.size() == N);
    assert(N);
    for (int i = (int)(A.size()) - 1; i > 0; i--) A[i - 1] = min(A[i - 1], A[i]);
    if (A.back() == 0) return {};
    if (min(A.back(), N) <= 400){
        vector<mint> dp(0);
        dp.reserve(A.back());
        for (int i = 0; i < N; i++){
            dp.resize(A[i], 0);
            if (A[i]) dp[0] += C[i];
            for (int j = 1; j < (int)dp.size(); j++){
                dp[j] += dp[j - 1];
            }
        }
        return dp;
    }
    if (N == 1){
        vector<mint> res(A[0]);
        for (int i = 0; i < A[0]; i++) res[i] = C[0];
        return res;
    }
    int m = N / 2;
    vector<int> LA(m), RA(N - m);
    vector<mint> LC(m), RC(N - m);
    for (int i = 0; i < m; i++){
        LA[i] = A[i];
        LC[i] = C[i];
    }
    for (int i = 0; i < N - m; i++){
        RA[i] = A[i + m] - A[m - 1];
        RC[i] = C[i + m];
    }
    vector<mint> res;
    res.reserve(A.back());
    auto L = count_increase_sequences_with_upper_bounds(BC,LA,LC);
    if (!L.empty()){
        auto [R, U] = count_square(L,RC,BC);
        for (int i = 0; i < (int)R.size(); i++) res.push_back(R[i]);
        swap(U, RC);
    }
    auto R = count_increase_sequences_with_upper_bounds(BC,RA,RC);
    for (auto x : R) res.push_back(x);
    return res;
}

template<class mint>
/*
 * f(a, b) を X[0] = a, X[N - 1] = b であるような、A, B に挟まれたものとする
 * 長さ B[N - 1] - A[N - 1] を返す
 * res[b - A.back()] = sum C[a - A[0]] * f(a, b)
 * A, B は広義単調増加が嬉しい
 * C は空ならば、全て 1 であるとする。
 * そうでないなら、|C| = B[0] - A[0] でないといけない
 */
vector<mint> count_increase_sequences_with_upper_lower_bounds(Binomial<mint> &BC,vector<int> A, vector<int> B,vector<mint> C = {}){
    int N = A.size();
    assert(A.size() == B.size());
    for (int i = 0; i < N - 1; i++){
        A[i + 1] = max(A[i], A[i + 1]);
    }
    for (int i = N - 1; i > 0; i--){
        B[i - 1] = min(B[i], B[i - 1]);
    }
    if (A.back() >= B.back()) return {};
    // A[0] == 0 にする
    vector<mint> res(B.back() - A.back(), 0);
    {
        int tmp = A[0];
        for (int i = 0; i < N; i++){
            A[i] -= tmp;
            B[i] -= tmp;
            if (A[i] >= B[i]) return res;
        }
    }
    if (C.empty()){
        C.resize(B[0] - A[0], 1);
    }
    else assert((int)(C.size()) == B[0] - A[0]);
    int l = 0;
    while (B[l] <= A.back()){
        for (int i = (int)(C.size()) - 1; i > 0; i--) C[i] -= C[i - 1];
        int nl = l;
        while (A[nl] < B[l]) nl++;
        vector<int> tmp(B[l] - A[l]);
        tmp[0] = 1;
        for (int i = l; i < nl; i++){
            tmp[A[i] - A[l]]++;
        }
        for (int i = 1; i < B[l] - A[l]; i++) tmp[i] += tmp[i - 1];
        auto X = count_increase_sequences_with_upper_bounds(BC,tmp,C);
        vector<int> nB(nl - l + 1);
        for (int i = l; i <= nl; i++){
            nB[i - l] = B[i] - B[l];
        }
        auto Y = count_increase_sequences_with_upper_bounds(BC,nB,X);
        C.resize(B[nl] - A[nl]);
        for (int i = 0; i < B[nl] - A[nl]; i++){
            C[i] = Y[i + A[nl] - B[l]];
        }
        l = nl;
    }
    // A を揃えてしまえ
    {
        int a = A[l];
        for (int i = l; i < N; i++){
            A[i] -= a;
            B[i] -= a;
        }
    }
    for (int i = (int)(C.size()) - 1; i > 0; i--) C[i] -= C[i - 1];
    vector<mint> D(N - l, 0);
    if (A.back() != 0){
        vector<mint> L(A.back());
        for (int i = 0; i < (int)L.size(); i++) L[i] = C[i];
        vector<int> tmp(L.size());
        tmp[0] = 1;
        for (int i = l; i < N; i++){
            if (A[i] < (int)tmp.size()) tmp[A[i]]++;
        }
        for (int i = 1; i < (int)tmp.size(); i++){
            tmp[i] += tmp[i - 1];
        }
        auto nD = count_increase_sequences_with_upper_bounds(BC,tmp,L);
        for (int i = 0; i < (int)nD.size(); i++) D[i] = nD[i];
    }
    for (int i = A.back(); i < B[l]; i++) C[i - A.back()] = C[i];
    C.resize(B[l] - A.back());
    auto [R, U] = count_square(C,D,BC);
    res = R;
    vector<int> nB(N - l);
    for (int i = 0; i < N - l; i++) nB[i] = B[i + l] - B[l];
    R = count_increase_sequences_with_upper_bounds(BC,nB,U);
    for (auto x : R) res.push_back(x);
    return res;
}

void solve() {
    int n,m;
    rd(n,m);
    vi a(n),b(n);
    rep(i,n) rd(a[i]);
    rep(i,n) rd(b[i]);
    Binomial<mint> C;
    auto tmp = count_increase_sequences_with_upper_lower_bounds<mint>(C,a,b);
    mint ans;
    rep(i,tmp.size()) ans += tmp[i];
    wtn(ans.get());
}

int main() {
    //INT(TT);
    int TT = 1;
    rep(i,TT) solve();
}
