#line 1 "Compositional_Inverse_of_Formal_Power_Series.cpp"
#define YRSD
#line 2 "YRS/all.hpp"

#line 2 "YRS/aa/head.hpp"

#include <iostream>
#include <algorithm>

#include <array>
#include <bitset>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <tuple>

#include <bit>
#include <chrono>
#include <functional>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <limits>

#define TE template
#define TN typename
#define Z auto
#define ep emplace_back
#define eb emplace
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define OV4(a, b, c, d, e, ...) e
#define FOR1(a) for (int _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (int i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (int i = (a); i < (b); i += (c))
#define FOR(...) OV4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR1_R(a) for (int _ = (a) - 1; _ >= 0; --_)
#define FOR2_R(i, a) for (int i = (a) - 1; i >= 0; --i)
#define FOR3_R(i, a, b) for (int i = (b) - 1; i >= (a); --i)
#define FOR4_R(i, a, b, c) for (int i = (b) - 1; i >= (a); i -= (c))
#define FOR_R(...) OV4(__VA_ARGS__, FOR4_R, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define FOR_subset(t, s) for (int t = (s); t > -1; t = (t == 0 ? -1 : (t - 1) & s))

using std::array, std::bitset, std::deque, std::greater, std::less, std::map, 
      std::multiset, std::pair, std::priority_queue, std::set, std::istream, 
      std::ostream, std::string, std::vector, std::tuple, std::function, std::cerr;
using std::cin, std::cout, std::swap, std::iota, std::endl, std::prev,
      std::next, std::min, std::max, std::tie, std::move, std::reverse;

TE<TN T> using vc = vector<T>;
TE<TN T> using vvc = vector<vc<T>>;
TE<TN T> using T1 = tuple<T>;
TE<TN T> using T2 = tuple<T, T>;
TE<TN T> using T3 = tuple<T, T, T>;
TE<TN T> using T4 = tuple<T, T, T, T>;
TE<TN T> using max_heap = priority_queue<T>;
TE<TN T> using min_heap = priority_queue<T, vector<T>, greater<T>>;
using u8 = unsigned char; using uint = unsigned int; using ll = long long;      using ull = unsigned long long;
using ld = long double;   using i128 = __int128;     using u128 = __uint128_t;  using f128 = __float128;
using PII = pair<int, int>;   using PLL = pair<ll, ll>;

#ifdef YRSD
constexpr bool dbg = 1;
#else
constexpr bool dbg = 0;
#endif
#line 2 "YRS/IO/IO.hpp"

istream &operator>>(istream &I, i128 &x) {
  static string s;
  I >> s;
  int f = s[0] == '-';
  x = 0;
  const int N = (int)s.size();
  FOR(i, f, N) x = x * 10 + s[i] - '0';
  if (f) x = -x;
  return I;
}
ostream &operator<<(ostream &O, i128 x) {
  static string s;
  s.clear();
  bool f = x < 0;
  if (f) x = -x;
  while (x) s += '0' + x % 10, x /= 10;
  if (s.empty()) s += '0';
  if (f) s += '-';
  return std::reverse(all(s)), O << s;
}
istream &operator>>(istream &I, f128 &x) {
  static string s;
  return I >> s, x = std::stold(s), I;
}
ostream &operator<<(ostream &O, const f128 x) { return O << ld(x); }
TE<TN... S> istream &operator>>(istream &I, tuple<S...> &t) {
  return std::apply([&I](Z &...args) { ((I >> args), ...); }, t), I;
}
TE<TN T, TN U> istream &operator>>(istream &I, pair<T, U> &x) {
  return I >> x.fi >> x.se;
}
TE<TN T, TN U> ostream &operator<<(ostream &O, const pair<T, U> &x) {
  return O << x.fi << ' ' << x.se;
}
TE<TN V> 
requires requires(V &c) { std::begin(c); std::end(c); } and 
                          (not std::is_same_v<std::decay_t<V>, string>)
istream &operator>>(istream &I, V &c) {
  for (Z &e : c) I >> e;
  return I;
}
TE<TN V> requires requires(const V &c) { std::begin(c); std::end(c); } and 
  (not std::is_same_v<std::decay_t<V>, const char*>) and 
  (not std::is_same_v<std::decay_t<V>, string>) and 
  (not std::is_array_v<std::remove_reference_t<V>> or 
   not std::is_same_v<std::remove_extent_t<std::remove_reference_t<V>>, char>)
ostream &operator<<(ostream &O, const V &c) {
  if (c.empty()) return O;
  Z it = c.begin();
  O << *it++;
  std::for_each(it, c.end(), [&O](const Z &e) { O << ' ' << e; });
  return O;
}
bool IN() { return true; }
TE<TN T, TN... S> bool IN(T &x, S &...y) {
  if (not(cin >> x)) return false;
  return IN(y...);
}
void print() { cout << '\n'; }
TE<TN T, TN... S> void print(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
  print(std::forward<S>(y)...);
}
void put() { cout << ' '; }
TE<TN T, TN... S> void put(T &&x, S &&...y) {
  cout << x;
  if constexpr (sizeof...(S)) cout << ' ';
  put(std::forward<S>(y)...);
}

#define INT(...)  int    __VA_ARGS__; IN(__VA_ARGS__)
#define LL(...)   ll     __VA_ARGS__; IN(__VA_ARGS__)
#define ULL(...)  ull    __VA_ARGS__; IN(__VA_ARGS__)
#define I128(...) i128   __VA_ARGS__; IN(__VA_ARGS__)
#define STR(...)  string __VA_ARGS__; IN(__VA_ARGS__)
#define CH(...)   char   __VA_ARGS__; IN(__VA_ARGS__)
#define REAL(...) RE     __VA_ARGS__; IN(__VA_ARGS__)
#define VEC(T, a, n) vector<T> a(n);  IN(a)
#define VVEC(T, a, n, m) vector a(n, vector<T>(m)); IN(a)

void YES(bool o = 1) { print(o ? "YES" : "NO"); }
void Yes(bool o = 1) { print(o ? "Yes" : "No"); }
void yes(bool o = 1) { print(o ? "yes" : "no"); }
void NO(bool o = 1) { YES(not o); }
void No(bool o = 1) { Yes(not o); }
void no(bool o = 1) { yes(not o); }
void ALICE(bool o = 1) { print(o ? "ALICE" : "BOB"); }
void Alice(bool o = 1) { print(o ? "Alice" : "Bob"); }
void alice(bool o = 1) { print(o ? "alice" : "bob"); }
void BOB(bool o = 1) { ALICE(not o); }
void Bob(bool o = 1) { Alice(not o); }
void bob(bool o = 1) { alice(not o); }
void POSSIBLE(bool o = 1) { print(o ? "POSSIBLE" : "IMPOSSIBLE"); }
void Possible(bool o = 1) { print(o ? "Possible" : "Impossible"); }
void possible(bool o = 1) { print(o ? "possible" : "impossible"); }
void IMPOSSIBLE(bool o = 1) { POSSIBLE(not o); }
void Impossible(bool o = 1) { Possible(not o); }
void impossible(bool o = 1) { possible(not o); }
void TAK(bool o = 1) { print(o ? "TAK" : "NIE"); }
void NIE(bool o = 1) { TAK(not o); }
#line 5 "YRS/all.hpp"

constexpr ld pi = 3.141592653589793L;
TE<TN T> constexpr T inf = std::numeric_limits<T>::max();
TE<> constexpr i128 inf<i128> = i128(std::numeric_limits<ll>::max()) * 2'000'000'000'000'000'000;
TE<TN T, TN U> constexpr pair<T, U> inf<pair<T, U>> = {inf<T>, inf<U>};

TE<TN T> constexpr static int popcount(T x) {
  using U = std::make_unsigned_t<T>;
  return std::__popcount(static_cast<U>(x));
}
TE<TN T> constexpr static int pc(T x) { return popcount(x); }
TE<TN T> constexpr static ll len(const T &a) { return a.size(); }
TE<TN T> constexpr static string to_s(T x) { return std::to_string(x); }

TE<TN T> void reverse(T &a) { reverse(all(a)); }
TE<TN T> void sort(T &a) { std::sort(all(a)); }
TE<TN T> void sort(T &a, Z cmp) { std::sort(all(a), cmp); }
TE<TN T> void unique(T &a) {
  std::sort(all(a));
  a.erase(std::unique(all(a)), a.end());
}
TE<TN T> vc<int> inverse(const vc<T> &A) {
  int N = len(A);
  vc<int> B(N, -1);
  FOR(i, N) if (A[i] != -1) B[A[i]] = i;
  return B;
}

Z QMAX(const Z &A) { return *std::max_element(all(A)); }
Z QMIN(const Z &A) { return *std::min_element(all(A)); }
constexpr bool chmax(Z &a, const Z &b) { return (a < b ? a = b, true : false); }
constexpr bool chmin(Z &a, const Z &b) { return (a > b ? a = b, true : false); }

TE<TN T, TN U> constexpr static pair<T, U> operator-(const pair<T, U> &p) {
  return pair<T, U>(-p.fi, -p.se);
}

TE<TN T> vc<int> argsort(const T &A) {
  vc<int> I(A.size());
  iota(all(I), 0);
  std::sort(all(I), [&](int i, int k) { return A[i] < A[k] or (A[i] == A[k] and i < k); });
  return I;
}
TE<TN T> vc<T> rearrange(const vc<T> &A, const vc<int> &I) {
  int N = len(I);
  vc<T> B(N);
  FOR(i, N) B[i] = A[I[i]];
  return B;
}
TE<bool off = 1, TN T> vc<T> pre_sum(const vc<T> &v) {
  int N = v.size();
  vc<T> A(N + 1);
  FOR(i, N) A[i + 1] = A[i] + v[i];
  if constexpr (off == 0) A.erase(A.begin());
  return A;
}

TE<TN T = int> vc<T> s_to_vec(const string &s, char off) {
  int N = len(s);
  vc<T> A(N);
  FOR(i, N) A[i] = (s[i] != '?' ? s[i] - off : -1);
  return A;
}

TE<TN T> constexpr static int topbit(T x) {
  if (x == 0) return - 1;
  if constexpr (sizeof(T) <= 4) return 31 - __builtin_clz(x);
  else return 63 - __builtin_clzll(x);
}
TE<TN T> constexpr static int lowbit(T x) {
  if (x == 0) return -1;
  if constexpr (sizeof(T) <= 4) return __builtin_ctz(x);
  else return __builtin_ctzll(x);
}

TE<TN T> constexpr T floor(T x, T y) { return x / y - (x % y and (x ^ y) < 0); }
TE<TN T> constexpr T ceil(T x, T y) { return floor(x + y - 1, y); }
TE<TN T> pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return pair{q, x - q * y};
}
TE<TN T = ll> T SUM(const Z &v) { return std::accumulate(all(v), T(0)); }
Z LB(const Z &a, Z x) { return std::lower_bound(all(a), x); }
Z UB(const Z &a, Z x) { return std::upper_bound(all(a), x); }
int lower_bound(const Z &a, Z x) { return LB(a, x) - a.begin(); }
int upper_bound(const Z &a, Z x) { return UB(a, x) - a.begin(); }
int lb(const Z &a, Z x) { return LB(a, x) - a.begin(); }
int ub(const Z &a, Z x) { return UB(a, x) - a.begin(); }

TE<bool ck = true> ll bina(const Z &F, ll L, ll R) {
  if constexpr (ck) assert(F(L));
  while (std::abs(L - R) > 1) {
    ll x = (R + L) >> 1;
    (F(x) ? L : R) = x;
  }
  return L;
}
TE<TN T> T bina_real(const Z &F, T L, T R, int c = 100) {
  while (c--) {
    T m = (L + R) / 2;
    (F(m) ? L : R) = m;
  }
  return (L + R) / 2;
}

TE<TN T> Z pop(T &s) {
  if constexpr (requires { s.back(); }) {
    Z x = s.back();
    return s.pop_back(), x;
  } else {
    Z x = s.top();
    return s.pop(), x;
  }
}
void setp(int x) { cout << std::fixed << std::setprecision(x); }
#line 1 "YRS/debug.hpp"
#ifdef YRSD
void DBG() { cerr << ']' << std::endl; }
TE<TN T, TN... S> void DBG(T &&x, S &&...y) {
  cerr << x;
  if constexpr (sizeof...(S)) cerr << ", ";
  DBG(std::forward<S>(y)...);
}
void DBG_ERR() { cerr << std::endl; }
TE<TN T, TN... S> void DBG_ERR(T &&x, S &&...y) {
  cerr << x;
  if constexpr (sizeof...(S)) cerr << ", ";
  DBG_ERR(std::forward<S>(y)...);
}
#define debug(...) cerr << '[' << __LINE__ << ']' << ": [" #__VA_ARGS__ "] = [", DBG(__VA_ARGS__)
#define err(...) cerr << '[' << __LINE__ << ']' << ": ", DBG_ERR(__VA_ARGS__)
#define asser assert
#else
#define debug(...) void(0721)
#define err(...) void(0721)
#define asser(...) void(0721)
#endif
#line 4 "Compositional_Inverse_of_Formal_Power_Series.cpp"
// #include "YRS/IO/fast_io.hpp"
// #include "YRS/random/rng.hpp"
#line 2 "YRS/po/comp_inv.hpp"

#line 2 "YRS/po/c/diff.hpp"

#line 2 "YRS/mod/modint.hpp"

#line 2 "YRS/mod/modint_common.hpp"

template <class T>
concept is_mint = requires(T x) {
  { T::get_mod() };
  { T::gen(0ull) } -> std::same_as<T>;
  x.val;
};

template <typename mint>
mint inv(int n) {
  static constexpr int mod = mint::get_mod();
  static vector<mint> dat = {0, 1};
  assert(0 <= n);
  if (n >= mod) n %= mod;
  while (len(dat) <= n) {
    int k = len(dat);
    Z q = (mod + k - 1) / k;
    int r = k * q - mod;
    dat.ep(dat[r] * mint(q));
  }
  return dat[n];
}
template <typename mint>
mint fact(int n) {
  static constexpr int mod = mint::get_mod();
  static vector<mint> dat = {1, 1};
  assert(0 <= n);
  if (n >= mod) return 0;
  while (len(dat) <= n) {
    int k = len(dat);
    dat.ep(dat[k - 1] * mint(k));
  }
  return dat[n];
}

template <typename mint>
mint fact_inv(int n) {
  static vector<mint> dat = {1, 1};
  if (n < 0) return mint(0);
  while (len(dat) <= n)
    dat.ep(dat[len(dat) - 1] * inv<mint>(len(dat)));
  return dat[n];
}

template <typename mint, typename... Ts>
mint fact_invs(Ts... xs) {
  return(mint(1) * ... * fact_inv<mint>(xs));
}

template <typename mint, typename Head, typename... Tail>
mint multinomial(Head&& head, Tail&&... tail) {
  return fact<mint>(head) * fact_invs<mint>(std::forward<Tail>(tail)...);
}

template <typename mint>
mint C_dense(int n, int k) {
  assert(n >= 0);
  if (k < 0 or n < k) return 0;
  static vector<vector<mint>> C;
  static int H = 0, W = 0;
  Z calc = [&](int i, int j) -> mint {
    if (i == 0) return(j == 0 ? mint(1) : mint(0));
    return C[i - 1][j] + (j ? C[i - 1][j - 1] : 0);
  };
  if (W <= k) {
    for (int i = 0; i < H; ++i) {
      C[i].resize(k + 1);
      for (int j = W; j < k + 1; ++j) {
        C[i][j] = calc(i, j);
      }
    }
    W = k + 1;
  }
  if (H <= n) {
    C.resize(n + 1);
    for (int i = H; i < n + 1; ++i) {
      C[i].resize(W);
      for (int j = 0; j < W; ++j) {
        C[i][j] = calc(i, j);
      }
    }
    H = n + 1;
  }
  return C[n][k];
}

template <typename mint, bool large = false, bool dense = false>
mint C(ll n, ll k) {
  assert(n >= 0);
  if (k < 0 or n < k) return 0;
  if constexpr (dense) return C_dense<mint>(n, k);
  if constexpr (not large) return multinomial<mint>(n, k, n - k);
  k = std::min(k, n - k);
  mint x(1);
  for (int i = 0; i < k; ++i) x *= mint(n - i);
  return x * fact_inv<mint>(k);
}

template <typename mint, bool large = false>
mint C_inv(ll n, ll k) {
  assert(n >= 0);
  assert(0 <= k and k <= n);
  if (not large) return fact_inv<mint>(n) * fact<mint>(k) * fact<mint>(n - k);
  return mint(1) / C<mint, 1>(n, k);
}

// [x^d](1-x)^{-n}
template <typename mint, bool large = false, bool dense = false>
mint C_negative(ll n, ll d) {
  assert(n >= 0);
  if (d < 0) return mint(0);
  if (n == 0) {
    return(d == 0 ? mint(1) : mint(0));
  }
  return C<mint, large, dense>(n + d - 1, d);
}
#line 4 "YRS/mod/modint.hpp"

template <int mod>
struct modint {
  static constexpr uint umod = uint(mod);
  static_assert(umod < uint(1) << 31);
  uint val;

  static constexpr modint raw(uint v) {
    modint x;
    x.val = v;
    return x;
  }

  static constexpr modint gen(uint x) {
    modint s;
    s.val = x;
    return s;
  }

  constexpr modint() : val(0) {}
  constexpr modint(uint x) : val(x % umod) {}
  constexpr modint(ull x) : val(x % umod) {}
  constexpr modint(u128 x) : val(x % umod) {}
  constexpr modint(int x) : val((x %= mod) < 0 ? x + mod : x) {}
  constexpr modint(ll x) : val((x %= mod) < 0 ? x + mod : x) {}
  constexpr modint(i128 x) : val((x %= mod) < 0 ? x + mod : x) {}

  bool operator<(const modint &p) const { return val < p.val; }

  constexpr modint &operator+=(const modint &p) {
    if ((val += p.val) >= umod) val -= umod;
    return *this;
  }
  constexpr modint &operator-=(const modint &p) {
    if ((val += umod - p.val) >= umod) val -= umod;
    return *this;
  }
  constexpr modint &operator*=(const modint &p) {
    val = ull(val) * p.val % umod;
    return *this;
  }
  constexpr modint &operator/=(const modint &p) {
    *this *= p.inv();
    return *this;
  }
  constexpr modint operator-() const { return modint::gen(val ? mod - val : uint(0)); }
  constexpr modint operator+(const modint &p) const { return modint(*this) += p; }
  constexpr modint operator-(const modint &p) const { return modint(*this) -= p; }
  constexpr modint operator*(const modint &p) const { return modint(*this) *= p; }
  constexpr modint operator/(const modint &p) const { return modint(*this) /= p; }
  bool operator==(const modint &p) const { return val == p.val; }
  bool operator!=(const modint &p) const { return val != p.val; }

  friend istream &operator>>(istream &is, modint &p) {
    ll x;
    is >> x;
    p = x;
    return is;
  }
  friend ostream &operator<<(ostream &os, modint p) { return os << p.val; }

  constexpr modint inv() const {
    int a = val, b = mod, x = 1, y = 0, t;
    while (b > 0) {
      t = a / b;
      swap(a -= t * b, b);
      swap(x -= t * y, y);
    }
    return modint(x);
  }

  constexpr modint pow(ll k) const {
    modint r(1), a(val);
    for (; k; k >>= 1, a *= a)
      if (k & 1) r *= a;
    return r;
  }

  static constexpr int get_mod() { return mod; }

  static constexpr PII ntt_info() {
    if constexpr (mod == 120586241) return {20, 74066978};
    if (mod == 167772161) return {25, 17};
    if (mod == 469762049) return {26, 30};
    if (mod == 754974721) return {24, 362};
    if (mod == 880803841) return {23, 211};
    if (mod == 943718401) return {22, 663003469};
    if (mod == 998244353) return {23, 31};
    if (mod == 1004535809) return {21, 582313106};
    if (mod == 1012924417) return {21, 368093570};
    return {-1, -1};
  }
  
  static constexpr bool can_ntt() { return ntt_info().fi != -1; }
};
using M99 = modint<998244353>;
using M17 = modint<1000000007>;

#ifdef FIO
template <int mod>
void rd(modint<mod> &x) {
  LL(y);
  x = y;
}
template <int mod>
void wt(modint<mod> x) {
  wt(x.val);
}
#endif
#line 4 "YRS/po/c/diff.hpp"

template <typename mint>
vc<mint> diff(const vc<mint> &f) {
  int N = len(f);
  if (N <= 1) return {};
  vc<mint> g(N + 1);
  FOR(i, N + 1) g[i] = f[i + 1] * mint(i + 1);
  return g;
}
#line 2 "YRS/po/fps_div.hpp"

#line 2 "YRS/po/c/count_terms.hpp"

// 非 0 数量
template<typename mint>
int count_terms(const vc<mint> &f){
  int t = 0, N = len(f);
  FOR(i, N) if(f[i] != mint(0)) ++t;
  return t;
}
#line 2 "YRS/po/fps_inv.hpp"

#line 2 "YRS/po/convolution.hpp"

#line 2 "YRS/po/c/ntt.hpp"

#line 4 "YRS/po/c/ntt.hpp"

template <typename mint>
void ntt(vc<mint> &a, bool in) {
  asser(mint::can_ntt());
  constexpr int p = mint::ntt_info().fi;
  constexpr uint mod = mint::get_mod();
  static array<mint, 30> r, ir, ra, ira, rat, irat;
  assert(p != -1 and len(a) <= (1 << max(0, p)));
  static bool ok = 0;
  if (not ok) {
    ok = 1;
    r[p] = mint::ntt_info().se;
    ir[p] = mint(1) / r[p];
    FOR_R(i, p) {
      r[i] = r[i + 1] * r[i + 1];
      ir[i] = ir[i + 1] * ir[i + 1];
    }
    mint s = 1, in = 1;
    FOR(i, p - 1) {
      ra[i] = r[i + 2] * s;
      ira[i] = ir[i + 2] * in;
      s *= ir[i + 2];
      in *= r[i + 2];
    }
    s = 1, in = 1;
    FOR(i, p - 2) {
      rat[i] = r[i + 3] * s;
      irat[i] = ir[i + 3] * in;
      s *= ir[i + 3];
      in *= r[i + 3];
    }
  }

  int N = len(a), n = topbit(N);
  if (not in) {
    int sz = 0;
    while (sz < n) {
      if (n - sz == 1) {
        int p = 1 << (n - sz - 1);
        mint c = 1;
        FOR(s, 1 << sz) {
          int of = s << (n - sz);
          FOR(i, p) {
            mint l = a[i + of], r = a[i + of + p] * c;
            a[i + of] = l + r, a[i + of + p] = l - r;
          }
          c *= ra[topbit(~s & -~s)];
        }
        ++sz;
      } else {
        int p = 1 << (n - sz - 2);
        mint c = 1, in = r[2];
        FOR(s, 1 << sz) {
          mint r2 = c * c, r3 = r2 * c;
          int of = s << (n - sz);
          FOR(i, p) {
            constexpr ull m2 = ull(mod) * mod;
            ull a0 = a[i + of].val, a1 = ull(a[i + of + p].val) * c.val;
            ull a2 = ull(a[i + of + 2 * p].val) * r2.val;
            ull a3 = ull(a[i + of + 3 * p].val) * r3.val;
            ull t = (a1 + m2 - a3) % mod * in.val;
            ull na = m2 - a2;
            a[i + of] = a0 + a1 + a2 + a3;
            a[i + of + p] = a0 + a2 + m2 * 2 - a1 - a3;
            a[i + of + 2 * p] = a0 + na + t;
            a[i + of + 3 * p] = a0 + na + m2 - t;
          }
          c *= rat[topbit(~s & -~s)];
        }
        sz += 2;
      }
    }
  } else {
    mint c = mint(1) / mint(len(a));
    FOR(i, len(a)) a[i] *= c;
    int sz = n;
    while (sz) {
      if (sz == 1) {
        int p = 1 << (n - sz);
        mint c = 1;
        FOR(s, 1 << (sz - 1)) {
          int of = s << (n - sz + 1);
          FOR(i, p) {
            ull l = a[i + of].val, r = a[i + of + p].val;
            a[i + of] = l + r;
            a[i + of + p] = (mod + l - r) * c.val;
          }
          c *= ira[topbit(~s & -~s)];
        }
        --sz;
      } else {
        int p = 1 << (n - sz);
        mint c = 1, in = ir[2];
        FOR(s, 1 << (sz - 2)) {
          mint r2 = c * c, r3 = r2 * c;
          int of = s << (n - sz + 2);
          FOR(i, p) {
            ull a0 = a[i + of].val, a1 = a[i + of + p].val;
            ull a2 = a[i + of + 2 * p].val;
            ull a3 = a[i + of + 3 * p].val;
            ull x = (mod + a2 - a3) * in.val % mod;
            a[i + of] = a0 + a1 + a2 + a3;
            a[i + of + p] = (a0 + mod - a1 + x) * c.val;
            a[i + of + 2 * p] = (a0 + a1 + 2 * mod - a2 - a3) * r2.val;
            a[i + of + 3 * p] = (a0 + 2 * mod - a1 - x) * r3.val;
          }
          c *= irat[topbit(~s & -~s)];
        }
        sz -= 2;
      }
    }
  }
}
#line 2 "YRS/mod/crt3.hpp"

constexpr uint pow_constexpr(ull a, ull b, uint mod) {
  a %= mod;
  ull res = 1;
  FOR(32) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod, b >>= 1;
  }
  return res;
}

template <typename T, uint p0, uint p1>
T CRT2(ull a0, ull a1) {
  static_assert(p0 < p1);
  static constexpr ull x0_1 = pow_constexpr(p0, p1 - 2, p1);
  ull c = (a1 - a0 + p1) * x0_1 % p1;
  return a0 + c * p0;
}

template <typename T, uint p0, uint p1, uint p2>
T CRT3(ull a0, ull a1, ull a2) {
  static_assert(p0 < p1 and p1 < p2);
  static constexpr ull x1 = pow_constexpr(p0, p1 - 2, p1);
  static constexpr ull x2 = pow_constexpr(ull(p0) * p1 % p2, p2 - 2, p2);
  static constexpr ull p01 = ull(p0) * p1;
  ull c = (a1 - a0 + p1) * x1 % p1;
  ull ans_1 = a0 + c * p0;
  c = (a2 - ans_1 % p2 + p2) * x2 % p2;
  return T(ans_1) + T(c) * T(p01);
}

template <typename T, uint p0, uint p1, uint p2, uint p3>
T CRT4(ull a0, ull a1, ull a2, ull a3) {
  static_assert(p0 < p1 and p1 < p2 and p2 < p3);
  static constexpr ull x1 = pow_constexpr(p0, p1 - 2, p1);
  static constexpr ull x2 = pow_constexpr(ull(p0) * p1 % p2, p2 - 2, p2);
  static constexpr ull x3 = pow_constexpr(ull(p0) * p1 % p3 * p2 % p3, p3 - 2, p3);
  static constexpr ull p01 = ull(p0) * p1;
  ull c = (a1 - a0 + p1) * x1 % p1;
  ull ans_1 = a0 + c * p0;
  c = (a2 - ans_1 % p2 + p2) * x2 % p2;
  u128 ans_2 = ans_1 + c * static_cast<u128>(p01);
  c = (a3 - ans_2 % p3 + p3) * x3 % p3;
  return T(ans_2) + T(c) * T(p01) * T(p2);
}

template <typename T, uint p0, uint p1, uint p2, uint p3, uint p4>
T CRT5(ull a0, ull a1, ull a2, ull a3, ull a4) {
  static_assert(p0 < p1 and p1 < p2 and p2 < p3 and p3 < p4);
  static constexpr ull x1 = pow_constexpr(p0, p1 - 2, p1);
  static constexpr ull x2 = pow_constexpr(ull(p0) * p1 % p2, p2 - 2, p2);
  static constexpr ull x3 = pow_constexpr(ull(p0) * p1 % p3 * p2 % p3, p3 - 2, p3);
  static constexpr ull x4 = pow_constexpr(ull(p0) * p1 % p4 * p2 % p4 * p3 % p4, p4 - 2, p4);
  static constexpr ull p01 = ull(p0) * p1;
  static constexpr ull p23 = ull(p2) * p3;
  ull c = (a1 - a0 + p1) * x1 % p1;
  ull ans_1 = a0 + c * p0;
  c = (a2 - ans_1 % p2 + p2) * x2 % p2;
  u128 ans_2 = ans_1 + c * static_cast<u128>(p01);
  c = static_cast<ull>(a3 - ans_2 % p3 + p3) * x3 % p3;
  u128 ans_3 = ans_2 + static_cast<u128>(c * p2) * p01;
  c = static_cast<ull>(a4 - ans_3 % p4 + p4) * x4 % p4;
  return T(ans_3) + T(c) * T(p01) * T(p23);
}
#line 5 "YRS/po/convolution.hpp"

template <typename mint>
vc<mint> conv_ntt(vc<mint> a, vc<mint> b) {
  static_assert(mint::can_ntt());
  if (a.empty() or b.empty()) return {};
  int N = len(a), M = len(b), sz = 1;
  while (sz < N + M - 1) sz <<= 1;
  a.resize(sz), b.resize(sz);
  bool ok = a == b;
  ntt(a, 0);
  if (ok) b = a;
  else ntt(b, 0);
  FOR(i, sz) a[i] *= b[i];
  ntt(a, 1);
  a.resize(N + M - 1);
  return a;
}

template <typename mint>
vc<mint> conv_mtt(const vc<mint> &a, const vc<mint> &b) {
  int N = len(a), M = len(b);
  if (not N or not M) return {};
  static constexpr int p0 = 167772161;
  static constexpr int p1 = 469762049;
  static constexpr int p2 = 754974721;
  using M0 = modint<p0>;
  using M1 = modint<p1>;
  using M2 = modint<p2>;
  vc<M0> a0(N), b0(M);
  vc<M1> a1(N), b1(M);
  vc<M2> a2(N), b2(M);
  FOR(i, N) a0[i] = a[i].val, a1[i] = a[i].val, a2[i] = a[i].val;
  FOR(i, M) b0[i] = b[i].val, b1[i] = b[i].val, b2[i] = b[i].val;
  vc<M0> c0 = conv_ntt<M0>(a0, b0);
  vc<M1> c1 = conv_ntt<M1>(a1, b1);
  vc<M2> c2 = conv_ntt<M2>(a2, b2);
  vc<mint> c(len(c0));
  FOR(i, N + M - 1) 
    c[i] = CRT3<mint, p0, p1, p2>(c0[i].val, c1[i].val, c2[i].val);
  return c;
}

template <typename mint>
vc<mint> convolution(const vc<mint> &a, const vc<mint> &b) {
  int N = len(a), M = len(b);
  if (not N or not M) return {};
  if constexpr (mint::can_ntt()) return conv_ntt(a, b);
  return conv_mtt(a, b);
}
#line 5 "YRS/po/fps_inv.hpp"

// O(NK)
template <typename mint>
vc<mint> fps_inv_sparse(const vc<mint> &f) {
  int N = len(f);
  vc<pair<int, mint>> dat;
  FOR(i, 1, N) if (f[i] != mint(0)) dat.ep(i, f[i]);
  vc<mint> g(N);
  mint t = mint(1) / f[0];
  g[0] = t;
  FOR(i, 1, N) {
    mint s = 0;
    for (Z &&[x, y] : dat) {
      if (x > i) break;
      s -= y * g[i - x];
    }
    g[i] = s * t;
  }
  return g;
}

template <typename mint>
vc<mint> fps_inv_dense_ntt(const vc<mint> &F) {
  vc<mint> G{mint(1) / F[0]};
  int N = len(F), n = 1;
  G.reserve(N);
  while (n < N) {
    vc<mint> f(n << 1), g(n << 1);
    int L = min(N, n << 1);
    FOR(i, L) f[i] = F[i];
    FOR(i, n) g[i] = G[i];
    ntt(f, 0), ntt(g, 0);
    FOR(i, n << 1) f[i] *= g[i];
    ntt(f, 1);
    FOR(i, n) f[i] = 0;
    ntt(f, 0);
    FOR(i, n << 1) f[i] *= g[i];
    ntt(f, 1);
    FOR(i, n, L) G.ep(-f[i]);
    n <<= 1;
  }
  return G;
}

template <typename mint>
vc<mint> fps_inv_dense(const vc<mint> &F) {
  if constexpr (mint::can_ntt()) return fps_inv_dense_ntt(F);
  int N = len(F);
  vc<mint> R = {mint(1) / F[0]}, p;
  int n = 1;
  while (n < N) {
    p = convolution(R, R);
    p.resize(n << 1);
    vc<mint> f = {F.begin(), F.begin() + min(n << 1, N)};
    p = convolution(p, f);
    R.resize(n << 1);
    FOR(i, n + n) R[i] = R[i] + R[i] - p[i];
    n <<= 1;
  }
  R.resize(N);
  return R;
}

template <typename mint>
vc<mint> fps_inv(const vc<mint> &f) {
  assert(f[0] != mint(0));
  int sz = count_terms(f), c = mint::can_ntt() ? 160 : 820;
  return sz <= c ? fps_inv_sparse(f) : fps_inv_dense(f);
}
#line 5 "YRS/po/fps_div.hpp"

template <typename mint>
vc<mint> fps_div_sprase(vc<mint> f, vc<mint> &g) {
  if (g[0] != mint(1)) {
    mint c = g[0].inv();
    for (Z &x : f) x *= c;
    for (Z &x : g) x *= c;
  }
  vc<pair<int, mint>> dat;
  int N = len(g);
  FOR(i, 1, N) if (g[i] != mint(0)) dat.ep(i, -g[i]);
  N = len(f);
  FOR(i, N) for (Z[x, y] : dat) if (i >= x) f[i] += y * f[i - x];
  return f;
}

// f/g 截断的商
template <typename mint>
vc<mint> fps_div(vc<mint> f, vc<mint> g) {
  if (count_terms(f) < 100 and 0) return fps_div_sprase(f, g);
  int N = len(f);
  g.resize(N);
  g = fps_inv<mint>(g);
  f = convolution(f, g);
  f.resize(N);
  return f;
}
#line 2 "YRS/po/fps_pow.hpp"

#line 2 "YRS/po/fps_exp.hpp"

#line 2 "YRS/po/c/inte.hpp"

#line 4 "YRS/po/c/inte.hpp"

// 不定积分
template <typename mint>
vc<mint> inte(const vc<mint> &f) {
  int N = len(f);
  vc<mint> g(N + 1);
  FOR(i, 1, N + 1) g[i] = f[i - 1] * inv<mint>(i);
  return g;
}

// 定积分
template <typename mint>
mint inte(const vc<mint> &f, mint l, mint r) {
  mint s = 0, L = 1, R = 1;
  int N = len(f);
  FOR(i, N) {
    L *= l, R *= r;
    s += inv<mint>(i + 1) * f[i] * (L - R);
  }
  return s;
}
#line 7 "YRS/po/fps_exp.hpp"

template <typename mint>
vc<mint> fps_exp_sparse(const vc<mint> &f) {
  int N = len(f);
  if (N == 0) return {mint(1)};
  assert(f[0] == 0);
  vc<pair<int, mint>> dat;
  FOR(i, 1, N) if (f[i] != mint(0)) dat.ep(i - 1, f[i] * mint(i));
  vc<mint> F(N);
  F[0] = 1;
  FOR(i, 1, N) {
    mint s = 0;
    for (Z [x, y] : dat) {
      if (x > i - 1) break;
      s += y * F[i - 1 - x];
    }
    F[i] = s * inv<mint>(i);
  }
  return F;
}

template <typename mint>
vc<mint> fps_exp_ntt(const vc<mint> &f) {
  int N = len(f);
  assert(N > 0 and f[0] == mint(0));
  vc<mint> b = {1, (1 < N ? f[1] : 0)}, c = {1}, z1, z2 = {1, 1};
  while (len(b) < N) {
    int m = len(b);
    Z y = b;
    y.resize(m << 1);
    ntt(y, 0);
    z1 = z2;
    vc<mint> z(m);
    FOR(i, m) z[i] = y[i] * z1[i];
    ntt(z, 1);
    FOR(i, m >> 1) z[i] = 0;
    ntt(z, 0);
    FOR(i, m) z[i] *= -z1[i];
    ntt(z, 1);
    c.insert(c.end(), z.begin() + m / 2, z.end());
    z2 = c;
    z2.resize(m << 1);
    ntt(z2, 0);

    vc<mint> x(f.begin(), f.begin() + m);
    FOR(i, len(x) - 1) x[i] = x[i + 1] * mint(i + 1);
    x.back() = 0;
    ntt(x, 0);
    FOR(i, m) x[i] *= y[i];
    ntt(x, 1);

    FOR(i, m - 1) x[i] -= b[i + 1] * mint(i + 1);

    x.resize(m << 1);
    FOR(i, m - 1) x[m + i] = x[i], x[i] = 0;
    ntt(x, 0);
    FOR(i, m << 1) x[i] *= z2[i];
    ntt(x, 1);
    FOR_R(i, len(x) - 1) x[i + 1] = x[i] * inv<mint>(i + 1);
    x[0] = 0;

    FOR(i, m, min(N, m << 1)) x[i] += f[i];
    FOR(i, m) x[i] = 0;
    ntt(x, 0);
    FOR(i, m << 1) x[i] *= y[i];
    ntt(x, 1);
    b.insert(b.end(), x.begin() + m, x.end());
  }
  b.resize(N);
  return b;
}

template <typename mint>
vc<mint> fps_exp_dense(const vc<mint> &e) {
  if constexpr (mint::can_ntt()) return fps_exp_ntt(e);
  vc<mint> h = e;
  int N = len(h);
  assert(N > 0 and h[0] == mint(0));
  int log = 0;
  while (1 << log < N) ++log;
  h.resize(1 << log);
  Z dh = diff(h);
  vc<mint> f = {1}, g = {1};
  int m = 1;
  vc<mint> p;
  FOR(log) {
    p = convolution(f, g);
    p.resize(m);
    p = convolution(p, g);
    p.resize(m);
    g.resize(m);
    FOR(i, m) g[i] += g[i] - p[i];
    p = {dh.begin(), dh.begin() + m - 1};
    p = convolution(f, p);
    p.resize(m + m - 1);
    FOR(i, m + m - 1) p[i] = -p[i];
    FOR(i, m - 1) p[i] += mint(i + 1) * f[i + 1];
    p = convolution(p, g);
    p.resize(m + m - 1);
    FOR(i, m - 1) p[i] += dh[i];
    p = inte(p);
    FOR(i, m + m) p[i] = h[i] - p[i];
    p[0] += mint(1);
    f = convolution(f, p);
    f.resize(m << 1);
    m <<= 1;
  }
  f.resize(N);
  return f;
}

template <typename mint>
vc<mint> fps_exp(vc<mint> &f) {
  int n = count_terms(f), t = mint::can_ntt() ? 320 : 3000;
  return n <= t ? fps_exp_sparse(f) : fps_exp_dense(f);
}
#line 2 "YRS/po/fps_log.hpp"

#line 5 "YRS/po/fps_log.hpp"

template <typename mint>
vc<mint> fps_log_sparse(const vc<mint> &a) {
  int N = len(a);
  vc<pair<int, mint>> dat;
  FOR(i, 1, N) if (a[i] != mint(0)) dat.ep(i, a[i]);
  vc<mint> f(N), g(N - 1);
  FOR(i, N - 1) {
    mint s = a[i + 1] * mint(i + 1);
    for (Z &&[x, y] : dat) {
      if (x > i) break;
      s -= y * g[i - x];
    }
    g[i] = s;
    f[i + 1] = s * inv<mint>(i + 1);
  }
  return f;
}

template <typename mint>
vc<mint> fps_log_dense(const vc<mint> &f) {
  assert(f[0] == mint(1));
  const int N = len(f);
  vc<mint> df = f;
  FOR(i, N) df[i] *= mint(i);
  df.erase(df.begin());
  vc<mint> inf = fps_inv(f), g = convolution(df, inf);
  g.resize(N - 1);
  g.insert(g.begin(), 0);
  FOR(i, N) g[i] *= inv<mint>(i);
  return g;
}

template <typename mint>
vc<mint> fps_log(const vc<mint> &f) {
  assert(f[0] == mint(1));
  int n = count_terms(f), t = mint::can_ntt() ? 200 : 1200;
  return n <= t ? fps_log_sparse(f) : fps_log_dense(f);
}
#line 5 "YRS/po/fps_pow.hpp"

template <typename mint>
vc<mint> fps_pw_sparse(const vc<mint> &f, mint k) {
  int N = len(f);
  assert(N == 0 or f[0] == mint(1));
  vc<pair<int, mint>> dat;
  FOR(i, 1, N) if (f[i] != mint(0)) dat.ep(i, f[i]);
  vc<mint> g(N);
  g[0] = 1;
  FOR(i, N - 1) {
    mint &s = g[i + 1];
    for (Z &&[x, y] : dat) {
      if (x > i + 1) break;
      mint t = y * g[i - x + 1];
      s += t * (k * mint(x) - mint(i - x + 1));
    }
    s *= inv<mint>(i + 1);
  }
  return g;
}

template <typename mint>
vc<mint> fps_pw_dense(const vc<mint> &f, mint k) {
  assert(f[0] == mint(1));
  Z g = fps_log(f);
  int N = len(f);
  FOR(i, N) g[i] *= k;
  return fps_exp_dense(g);
}

template <typename mint>
vc<mint> fps_pw(const vc<mint> &f, mint k) {
  int n = count_terms(f), t = mint::can_ntt() ? 100 : 1300;
  return n <= t ? fps_pw_sparse(f, k) : fps_pw_dense(f, k);
}

template <typename mint>
vc<mint> fps_pow(const vc<mint> &f, ll k) {
  assert(0 <= k);
  int N = len(f);
  if (k == 0) {
    vc<mint> g(N);
    g[0] = 1;
    return g;
  }
  if (f[0] == mint(1)) return fps_pw(f, mint(k));
  int d = N;
  FOR_R(i, N) if (f[i] != mint(0)) d = i;
  if (d >= ceil<ll>(N, k)) return vc<mint>(N);
  int of = d * k;
  mint c = f[d], in = mint(1) / c;
  vc<mint> g(N - of);
  FOR(i, N - of) g[i] = f[d + i] * in;
  g = fps_pw(g, mint(k));
  vc<mint> r(N);
  c = c.pow(k);
  N = len(g);
  FOR(i, N) r[of + i] = g[i] * c;
  return r;
}
#line 2 "YRS/po/pow_projection.hpp"

#line 4 "YRS/po/pow_projection.hpp"

template <typename mint>
vc<mint> pow_projection_0_ntt(vc<mint> wt, vc<mint> f, int m) {
  assert(len(f) == len(wt) and f[0] == mint(0));
  int sz = len(f), N = 1;
  while (N < sz) N <<= 1;
  for (Z &x : f) x = -x;
  f.resize(N), wt.resize(N);
  reverse(all(wt));
  vc<mint> &P = wt, &Q = f;
  P.resize(N << 2), Q.resize(N << 2);

  vc<mint> w(N);
  vc<int> b(N);
  int log = topbit(N);
  FOR(i, N) b[i] = (b[i >> 1] >> 1) + ((i & 1) << (log - 1));
  constexpr int t = mint::ntt_info().fi;
  constexpr mint r = mint::ntt_info().se;
  mint dw = r.inv().pow((1 << t) / (N << 1)), s = 1;
  for (int i : b) w[i] = s, s *= dw;

  int k = 1;
  while (N > 1) {
    Z db = [&](vc<mint> &a, int l, int r) -> void {
      mint z = w[k >> 1].inv();
      vc<mint> f(k);
      FOR(i, l, r) {
        FOR(j, k) f[j] = a[2 * N * j + i];
        ntt(f, 1);
        mint r = 1;
        FOR(j, 1, k) r *= z, f[j] *= r;
        ntt(f, 0);
        FOR(j, k) a[2 * N * (k + j) + i] = f[j];
      }
    };

    Z fft = [&](vc<mint> &a, int l, int r) -> void {
      vc<mint> f(N << 1);
      FOR(j, l, r) {
        move(a.begin() + 2 * N * j, a.begin() + 2 * N * (j + 1), f.begin());
        ntt(f, 0);
        move(all(f), a.begin() + 2 * N * j);
      }
    };

    if (N <= k) {
      db(P, 0, N), db(Q, 1, N);
      fft(P, 0, k << 1), fft(Q, 0, k << 1);
    } else {
      fft(P, 0, k), fft(Q, 0, k);
      db(P, 0, N << 1), db(Q, 0, N << 1);
    }
    FOR(i, 2 * N * k) Q[i] += 1;
    FOR(i, 2 * N * k, 4 * N * k) Q[i] -= 1;

    vc<mint> F(N << 1), G(N << 1), f(N), g(N);
    FOR(j, k << 1) {
      move(P.begin() + 2 * N * j, P.begin() + 2 * N * j + 2 * N, F.begin());
      move(Q.begin() + 2 * N * j, Q.begin() + 2 * N * j + 2 * N, G.begin());
      FOR(i, N) {
        f[i] = w[i] * (F[i << 1] * G[i << 1 | 1] - F[i << 1 | 1] * G[i << 1]);
        g[i] = G[i << 1] * G[i << 1 | 1];
      }
      ntt(f, 1), ntt(g, 1);
      fill(f.begin() + N / 2, f.end(), mint(0));
      fill(g.begin() + N / 2, g.end(), mint(0));
      move(all(f), P.begin() + N * j);
      move(all(g), Q.begin() + N * j);
    }
    fill(P.begin() + 2 * N * k, P.end(), mint(0));
    fill(Q.begin() + 2 * N * k, Q.end(), mint(0));
    FOR(j, k << 2) Q[N * j] = 0;
    N >>= 1, k <<= 1;
  }
  FOR(i, k) P[i] = P[i << 1];
  P.resize(k);
  mint c = mint(1) / mint(k);
  for (Z &x : P) x *= c;
  ntt(P, 1);
  reverse(all(P));
  P.resize(m + 1);
  return P;
}

template <typename mint>
vc<mint> pow_projection_0_mtt(vc<mint> wt, vc<mint> f, int m) {
  assert(len(f) == len(wt) and f[0] == mint(0));
  int N = 1, sz = len(f);
  while (N < sz) N <<= 1;
  f.resize(N), wt.resize(N);
  reverse(all(wt));

  constexpr uint M[]{167'772'161, 469'762'049, 754'974'721};
  using M0 = modint<M[0]>;
  using M1 = modint<M[1]>;
  using M2 = modint<M[2]>;
  vc<M0> w0(N << 1);
  vc<M1> w1(N << 1);
  vc<M2> w2(N << 1);

  vc<int> b(N << 1);
  int log = topbit(N << 1);
  FOR(i, N << 1) b[i] = (b[i >> 1] >> 1) + ((i & 1) << (log - 1));
  {
    int t = M0::ntt_info().fi;
    M0 r = M0::ntt_info().se, dw = r.inv().pow((1 << t) / (N << 2)), s = 1;
    for (int i : b) w0[i] = s, s *= dw;
  }
  {
    int t = M1::ntt_info().fi;
    M1 r = M1::ntt_info().se, dw = r.inv().pow((1 << t) / (N << 2)), s = 1;
    for (int i : b) w1[i] = s, s *= dw;
  }
  {
    int t = M2::ntt_info().fi;
    M2 r = M2::ntt_info().se, dw = r.inv().pow((1 << t) / (N << 2)), s = 1;
    for (int i : b) w2[i] = s, s *= dw;
  }

  int k = 1;
  vc<mint> P(N << 1), Q(N << 1);
  FOR(i, N) P[i] = wt[i], Q[i] = -f[i];
  while (N > 1) {
    int sz = 2 * N * k;
    vc<M0> P0(sz << 1), Q0(sz << 1);
    vc<M1> P1(sz << 1), Q1(sz << 1);
    vc<M2> P2(sz << 1), Q2(sz << 1);
    FOR(i, sz) P0[i] = P[i].val, Q0[i] = Q[i].val;
    FOR(i, sz) P1[i] = P[i].val, Q1[i] = Q[i].val;
    FOR(i, sz) P2[i] = P[i].val, Q2[i] = Q[i].val;
    Q0[sz] = 1, Q1[sz] = 1, Q2[sz] = 1;
    ntt(P0, 0), ntt(P1, 0), ntt(P2, 0);
    ntt(Q0, 0), ntt(Q1, 0), ntt(Q2, 0);
    FOR(i, sz) {
      P0[i] = inv<M0>(2) * w0[i] *
              (P0[i << 1] * Q0[i << 1 | 1] - P0[i << 1 | 1] * Q0[i << 1]);
      P1[i] = inv<M1>(2) * w1[i] *
              (P1[i << 1] * Q1[i << 1 | 1] - P1[i << 1 | 1] * Q1[i << 1]);
      P2[i] = inv<M2>(2) * w2[i] *
              (P2[i << 1] * Q2[i << 1 | 1] - P2[i << 1 | 1] * Q2[i << 1]);
      Q0[i] = Q0[i << 1] * Q0[i << 1 | 1];
      Q1[i] = Q1[i << 1] * Q1[i << 1 | 1];
      Q2[i] = Q2[i << 1] * Q2[i << 1 | 1];
    }
    P0.resize(sz), P1.resize(sz), P2.resize(sz);
    Q0.resize(sz), Q1.resize(sz), Q2.resize(sz);
    ntt(P0, 1), ntt(P1, 1), ntt(P2, 1);
    ntt(Q0, 1), ntt(Q1, 1), ntt(Q2, 1);

    constexpr i128 K = u128(M[0]) * M[1] * M[2];
    Z ke = [&](M0 a, M1 b, M2 c) -> mint {
      i128 x = CRT3<u128, M[0], M[1], M[2]>(a.val, b.val, c.val);
      i128 y = K - x;
      return x < y ? mint(x) : -mint(y);
    };

    fill(all(P), mint(0)), fill(all(Q), mint(0));
    FOR(j, k << 1) FOR(i, N >> 1) {
      int k = N * j + i;
      P[k] = ke(P0[k], P1[k], P2[k]);
      Q[k] = ke(Q0[k], Q1[k], Q2[k]);
    }
    Q[0] = 0;
    N >>= 1, k <<= 1;
  }
  vc<mint> F(k);
  FOR(i, k) F[i] = P[i << 1];
  reverse(all(F));
  F.resize(m + 1);
  return F;
}

// sum_j wt[j] [x^j] f^i  i=0,1,...,m
template <typename mint>
vc<mint> pow_projection(vc<mint> wt, vc<mint> f, int m) {
  assert(len(f) == len(wt));
  if (f.empty()) return vc<mint>(m + 1, mint(0));
  if (f[0] != mint(0)) {
    mint c = f[0];
    f[0] = 0;
    vc<mint> a = pow_projection(wt, f, m);
    FOR(i, m + 1) a[i] *= fact_inv<mint>(i);
    vc<mint> b(m + 1);
    mint pw = 1;
    FOR(i, m + 1) b[i] = pw * fact_inv<mint>(i), pw *= c;
    a = convolution(a, b);
    a.resize(m + 1);
    FOR(i, m + 1) a[i] *= fact<mint>(i);
    return a;
  }
  if constexpr (mint::can_ntt()) return pow_projection_0_ntt(wt, f, m);
}
#line 7 "YRS/po/comp_inv.hpp"

template <typename mint>
vc<mint> comp_inv(vc<mint> f) {
  int N = len(f) - 1;
  if (N == -1) return {};
  assert(f[0] == mint(0));
  if (N == 0) return f;
  assert(f[1] != mint(0));
  mint in = f[1].inv();
  for (Z &x : f) x *= in;
  vc<mint> wt(N + 1);
  wt[N] = 1;
  vc<mint> a = pow_projection(wt, f, N), g(N);
  FOR(i, 1, N + 1) g[N - i] = mint(N) * a[i] * inv<mint>(i);
  g = fps_pw(g, -inv<mint>(N));
  g.insert(g.begin(), 0);
  mint pw = 1;
  N = len(g);
  FOR(i, N) g[i] *= pw, pw *= in;
  return g;
}

// 给定 G -> F(G)、G -> DF(G)
// 计算到 len(G) 为止。算到 len(F) 也可以。
// 计算量大致与多项式复合相同。
template <typename mint, typename F1, typename F2>
vc<mint> comp_inv(const vc<mint> &f, F1 comp_f, F2 comp_df) {
  int N = len(f);
  assert(N <= 0 or f[0] == mint(0));
  assert(N <= 1 or f[1] != mint(0));

  vc<mint> g(2);
  g[1] = mint(1) / f[1];
  while (len(g) < N) {
    int n = len(g);
    // g:= g(x)-(F(g(x))-x)/DF(g(x))
    vc<mint> GG = comp_df(g);
    g.resize(n << 1);
    vc<mint> G = comp_f(g);
    G = {G.begin() + n, G.end()};
    G = fps_div(G, GG);
    FOR(i, n) g[n + i] -= G[i];
  }
  g.resize(N);
  return g;
}
#line 7 "Compositional_Inverse_of_Formal_Power_Series.cpp"

#define tests 0
#define fl 0
#define DB 10
using mint = M99;
void Yorisou() {
  INT(N);
  VEC(mint, f, N);
  print(comp_inv(f));
}
#line 1 "YRS/aa/main.hpp"
int main() {
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int T = 1;
  if (fl) cerr.tie(0);
  if (tests and not fl) IN(T);
  for (int i = 0; i < T or fl; ++i) {
    Yorisou();
    if (fl and i % DB == 0) cerr << "Case: " << i << '\n';
  }
  return 0;
}
#line 18 "Compositional_Inverse_of_Formal_Power_Series.cpp"
