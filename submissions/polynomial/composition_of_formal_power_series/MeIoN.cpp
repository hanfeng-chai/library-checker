#line 1 "Composition_of_Formal_Power_Series.cpp"
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
#line 2 "YRS/IO/fast_io.hpp"

#define FIO

static constexpr uint SZ = 1 << 17;
char ibuf[SZ];
char obuf[SZ];
char out[100];
// pointer of ibuf, obuf
uint pil = 0, pir = 0, por = 0;

struct Pre {
  char num[10000][4];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i][j] = n % 10 | '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

inline void load() {
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
  if (pir < SZ) ibuf[pir++] = '\n';
}

inline void flush() {
  fwrite(obuf, 1, por, stdout);
  por = 0;
}
void rd(char &c) {
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (isspace(c));
}

void rd(string &x) {
  x.clear();
  char c;
  do {
    if (pil + 1 > pir) load();
    c = ibuf[pil++];
  } while (isspace(c));
  do {
    x += c;
    if (pil == pir) load();
    c = ibuf[pil++];
  } while (!isspace(c));
}

template <typename T>
void rd_real(T &x) {
  string s;
  rd(s);
  x = stod(s);
}

template <typename T>
void rd_integer(T &x) {
  if (pil + 100 > pir) load();
  char c;
  do c = ibuf[pil++];
  while (c < '-');
  bool minus = 0;
  if constexpr (std::is_signed<T>::value || std::is_same_v<T, i128>) {
    if (c == '-') {
      minus = 1, c = ibuf[pil++];
    }
  }
  x = 0;
  while ('0' <= c) {
    x = x * 10 + (c & 15), c = ibuf[pil++];
  }
  if constexpr (std::is_signed<T>::value || std::is_same_v<T, i128>) {
    if (minus) x = -x;
  }
}

void rd(int16_t &x) { rd_integer(x); }
void rd(uint16_t &x) { rd_integer(x); }
void rd(int &x) { rd_integer(x); }
void rd(long &x) { rd_integer(x); }
void rd(ll &x) { rd_integer(x); }
void rd(i128 &x) { rd_integer(x); }
void rd(uint &x) { rd_integer(x); }
void rd(ull &x) { rd_integer(x); }
void rd(u128 &x) { rd_integer(x); }
void rd(double &x) { rd_real(x); }
void rd(long double &x) { rd_real(x); }
void rd(f128 &x) { rd_real(x); }

template <typename T, typename U>
void rd(pair<T, U> &p) {
  return rd(p.first), rd(p.second);
}
template <size_t N = 0, typename T>
void rd_tuple(T &t) {
  if constexpr (N < std::tuple_size<T>::value) {
    Z &x = std::get<N>(t);
    rd(x);
    rd_tuple<N + 1>(t);
  }
}
template <typename... T>
void rd(std::tuple<T...> &tpl) {
  rd_tuple(tpl);
}

template <size_t N = 0, typename T>
void rd(array<T, N> &x) {
  for (Z &d : x) rd(d);
}
template <typename T>
void rd(vc<T> &x) {
  for (Z &d : x) rd(d);
}

void read() {}
template <typename H, typename... T>
void read(H &h, T &...t) {
  rd(h), read(t...);
}

void wt(const char c) {
  if (por == SZ) flush();
  obuf[por++] = c;
}
void wt(const string s) {
  for (char c : s) wt(c);
}
void wt(const char *s) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) wt(s[i]);
}

template <typename T>
void wt_integer(T x) {
  if (por > SZ - 100) flush();
  if (x < 0) {
    obuf[por++] = '-', x = -x;
  }
  int outi;
  for (outi = 96; x >= 10000; outi -= 4) {
    memcpy(out + outi, pre.num[x % 10000], 4);
    x /= 10000;
  }
  if (x >= 1000) {
    memcpy(obuf + por, pre.num[x], 4);
    por += 4;
  } else if (x >= 100) {
    memcpy(obuf + por, pre.num[x] + 1, 3);
    por += 3;
  } else if (x >= 10) {
    int q = (x * 103) >> 10;
    obuf[por] = q | '0';
    obuf[por + 1] = (x - q * 10) | '0';
    por += 2;
  } else
    obuf[por++] = x | '0';
  memcpy(obuf + por, out + outi + 4, 96 - outi);
  por += 96 - outi;
}

template <typename T>
void wt_real(T x) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(10) << double(x);
  std::string s = oss.str();
  wt(s);
}

void wt(int x) { wt_integer(x); }
void wt(long x) { wt_integer(x); }
void wt(ll x) { wt_integer(x); }
void wt(i128 x) { wt_integer(x); }
void wt(uint x) { wt_integer(x); }
void wt(ull x) { wt_integer(x); }
void wt(u128 x) { wt_integer(x); }
void wt(double x) { wt_real(x); }
void wt(long double x) { wt_real(x); }
void wt(f128 x) { wt_real(x); }

template <typename T, typename U>
void wt(const pair<T, U> &val) {
  wt(val.first);
  wt(' ');
  wt(val.second);
}
template <size_t N = 0, typename T>
void wt_tuple(const T &t) {
  if constexpr (N < std::tuple_size<T>::value) {
    if constexpr (N > 0) {
      wt(' ');
    }
    const Z x = std::get<N>(t);
    wt(x);
    wt_tuple<N + 1>(t);
  }
}
template <typename... T>
void wt(tuple<T...> &tpl) {
  wt_tuple(tpl);
}
template <typename T, size_t S>
void wt(const array<T, S> &val) {
  Z n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt(' ');
    wt(val[i]);
  }
}
template <typename T>
void wt(const vc<T> &a) {
  int N = len(a);
  FOR(i, N) {
    if (i) wt(' ');
    wt(a[i]);
  }
}
template <typename T>
void wt(const vc<vc<T>> &v) {
  int N = len(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}
template <typename T, const size_t s>
void wt(const vc<array<T, s>> &v) {
  int N = len(v);
  FOR(i, N) {
    wt(v[i]);
    if (i + 1 != N) wt('\n');
  }
}

// gcc expansion. called automaticall after main.
void __attribute__((destructor)) _d() { flush(); }

void println() { wt('\n'); }
template <typename Head, typename... Tail>
void println(Head &&head, Tail &&...tail) {
  wt(head);
  if (sizeof...(Tail)) wt(' ');
  println(std::forward<Tail>(tail)...);
}

#define IN(...) read(__VA_ARGS__)
#define print(...) println(__VA_ARGS__)
#define FLUSH() flush()
#line 5 "Composition_of_Formal_Power_Series.cpp"
// #include "YRS/random/rng.hpp"
#line 2 "YRS/po/poly_comp.hpp"

#line 2 "YRS/po/poly_taylor_shift.hpp"

#line 2 "YRS/po/convolution.hpp"

#line 2 "YRS/po/c/ntt.hpp"

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
#line 2 "YRS/mod/powertable.hpp"

#line 2 "YRS/pr/primtable.hpp"

template <typename T = int>
vc<T> primtable(int LIM) {
  ++LIM;
  constexpr int S = 32768;
  static int N = 2;
  static vc<T> primes = {2}, sieve(S + 1);

  if (N < LIM) {
    N = LIM;
    primes = {2}, sieve.assign(S + 1, 0);
    const int R = LIM / 2;
    primes.reserve(int(LIM / std::log(LIM) * 1.1));
    vc<PII> cp;
    for (int i = 3; i <= S; i += 2) {
      if (not sieve[i]) {
        cp.ep(i, i * i / 2);
        for (int j = i * i; j <= S; j += 2 * i) sieve[j] = 1;
      }
    }
    for (int L = 1; L <= R; L += S) {
      array<bool, S> f {};
      for (Z &[p, id] : cp)
        for (int i = id; i < S + L; id = (i += p)) f[i - L] = 1;
      for (int i = 0; i < min(S, R - L); ++i)
        if (not f[i]) primes.ep((L + i) * 2 + 1);
    }
  }
  int k = lb(primes, LIM + 1);
  return {primes.begin(), primes.begin() + k};
}
#line 4 "YRS/mod/powertable.hpp"

// https://codeforces.com/contest/1194/problem/F

// x^0, ..., x^N
template <typename mint>
vc<mint> power_table_1(mint x, int N) {
  vc<mint> dp(N + 1, 1);
  FOR(i, N) dp[i + 1] = dp[i] * x;
  return dp;
}

// 0^x, ..., N^x
template <typename mint>
vc<mint> power_table_2(mint x, ll N) {
  vc<mint> dp(N + 1, 1);
  dp[0] = mint(0).pow(x);
  for (int p : primtable(N)) {
    if (p > N) break;
    mint xp = mint(p).pow(x);
    ll pp = p;
    while (pp < N + 1) {
      ll i = pp;
      while (i < N + 1) dp[i] *= xp, i += pp;
      pp *= p;
    }
  }
  return dp;
}
#line 5 "YRS/po/poly_taylor_shift.hpp"

// f(x) -> f(x + c) 左移
template <typename mint>
vc<mint> poly_taylor_shift(vc<mint> f, mint c) {
  if (c == mint(0)) return f;
  int N = len(f);
  FOR(i, N) f[i] *= fact<mint>(i);
  Z b = power_table_1(c, N);
  FOR(i, N) b[i] *= fact_inv<mint>(i);
  reverse(all(f));
  f = convolution(f, b);
  f.resize(N);
  reverse(all(f));
  FOR(i, N) f[i] *= fact_inv<mint>(i);
  return f;
}
#line 2 "YRS/po/c/transposed_ntt.hpp"

template <typename mint>
void transposed_ntt(vc<mint> &a, bool in) {
  static_assert(mint::can_ntt());
  constexpr int p = mint::ntt_info().fi;
  constexpr uint mod = mint::get_mod();
  static array<mint, 30> r, ir, rt, irt, rat, irat;

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
      rt[i] = r[i + 2] * s;
      irt[i] = ir[i + 2] * in;
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
  assert(N == 1 << n);
  if (not in) {
    int sz = n;
    while (sz > 0) {
      if (sz == 1) {
        int p = 1 << (n - sz);
        mint c = 1;
        FOR(s, 1 << (sz - 1)) {
          int of = s << (n - sz + 1);
          FOR(i, p) {
            ull l = a[i + of].val, r = a[i + of + p].val;
            a[i + of] = l + r, a[i + of + p] = (mod + l - r) * c.val;
          }
          c *= rt[topbit(~s & -~s)];
        }
        --sz;
      } else {
        int p = 1 << (n - sz);
        mint c = 1, in = r[2];
        FOR(s, 1 << (sz - 2)) {
          int of = s << (n - sz + 2);
          mint r2 = c * c, r3 = r2 * c;
          FOR(i, p) {
            ull a0 = a[i + of + 0 * p].val;
            ull a1 = a[i + of + 1 * p].val;
            ull a2 = a[i + of + 2 * p].val;
            ull a3 = a[i + of + 3 * p].val;
            ull x = (mod + a2 - a3) * in.val % mod;
            a[i + of] = a0 + a1 + a2 + a3;
            a[i + of + 1 * p] = (a0 + mod - a1 + x) * c.val;
            a[i + of + 2 * p] = (a0 + a1 + 2 * mod - a2 - a3) * r2.val;
            a[i + of + 3 * p] = (a0 + 2 * mod - a1 - x) * r3.val;
          }
          c *= rat[topbit(~s & -~s)];
        }
        sz -= 2;
      }
    }
  } else {
    mint c = mint(1) / mint(len(a));
    FOR(i, len(a)) a[i] *= c;
    int sz = 0;
    while (sz < n) {
      if (sz == n - 1) {
        int p = 1 << (n - sz - 1);
        mint c = 1;
        FOR(s, 1 << sz) {
          int of = s << (n - sz);
          FOR(i, p) {
            mint l = a[i + of], r = a[i + of + p] * c;
            a[i + of] = l + r, a[i + of + p] = l - r;
          }
          c *= irt[topbit(~s & -~s)];
        }
        ++sz;
      } else {
        int p = 1 << (n - sz - 2);
        mint c = 1, in = ir[2];
        FOR(s, 1 << sz) {
          mint r2 = c * c, r3 = r2 * c;
          int of = s << (n - sz);
          FOR(i, p) {
            ull m2 = ull(mod) * mod;
            ull a0 = a[i + of].val;
            ull a1 = ull(a[i + of + p].val) * c.val;
            ull a2 = ull(a[i + of + 2 * p].val) * r2.val;
            ull a3 = ull(a[i + of + 3 * p].val) * r3.val;
            ull t = (a1 + m2 - a3) % mod * in.val;
            ull na = m2 - a2;
            a[i + of] = a0 + a1 + a2 + a3;
            a[i + of + 1 * p] = a0 + a2 + (2 * m2 - a1 - a3);
            a[i + of + 2 * p] = a0 + na + t;
            a[i + of + 3 * p] = a0 + na + m2 - t;
          }
          c *= irat[topbit(~s & -~s)];
        }
        sz += 2;
      }
    }
  }
}
#line 5 "YRS/po/poly_comp.hpp"

// f(g(x)) O(N^2)  2e4 1700ms  8e3 266ms
template <typename mint>
vc<mint> poly_comp_slow(vc<mint> &f, vc<mint> &g) {
  assert(len(f) == len(g));
  int N = len(g), M = len(f), k = 1;
  while (k * k < N) ++k;
  vc<vc<mint>> pw(k + 1);
  pw[0] = {1}, pw[1] = g;
  FOR(i, 2, k + 1) {
    pw[i] = convolution(pw[i - 1], pw[1]);
    pw[i].resize(N);
  }
  vc<vc<mint>> pww(k + 1);
  pww[0] = {1}, pww[1] = pw[k];
  FOR(i, 2, k + 1) {
    pww[i] = convolution(pww[i - 1], pww[1]);
    pww[i].resize(N);
  }

  vc<mint> res(N);
  FOR(i, k + 1) {
    vc<mint> a(N);
    FOR(j, k) if (k * i + j < M) {
      mint c = f[k * i + j];
      FOR(d, len(pw[j])) a[d] += pw[j][d] * c;
    }
    a = convolution(a, pww[i]);
    a.resize(N);
    FOR(d, N) res[d] += a[d];
  }
  return res;
}

// O(Nlog^2N)  2e4 93ms  8e3 18ms
template <typename mint>
vc<mint> poly_comp_ntt(vc<mint> f, vc<mint> g) {
  assert(len(f) == len(g));
  if (f.empty()) return {};
  int sz = len(f), N = 1;
  while (N < sz) N <<= 1;
  f.resize(N), g.resize(N);
  
  vc<mint> w(N);
  vc<int> b(N);
  int log = topbit(N);
  FOR(i, N) b[i] = (b[i >> 1] >> 1) + ((i & 1) << (log - 1));
  constexpr int t = mint::ntt_info().fi;
  constexpr mint r = mint::ntt_info().se;
  mint dw = r.inv().pow((1 << t) / (N << 1)), s = 1;
  for (int i : b) w[i] = s, s *= dw;

  Z dfs = [&](Z &dfs, int N, int k, vc<mint> &a) -> vc<mint> {
    if (N == 1) {
      reverse(all(f));
      transposed_ntt(f, 1);
      mint c = mint(1) / mint(k);
      for (Z &x : f) x *= c;
      vc<mint> p(k << 2);
      FOR(i, k) p[i << 1] = f[i];
      return p;
    }

    Z db = [&](vc<mint> &a, int l, int r, bool t) -> void {
      mint z = w[k >> 1].inv();
      vc<mint> f(k);
      if (not t) {
        FOR(i, l, r) {
          FOR(j, k) f[j] = a[2 * N * j + i];
          ntt(f, 1);
          mint r = 1;
          FOR(j, 1, k) r *= z, f[j] *= r;
          ntt(f, 0);
          FOR(j, k) a[2 * N * (k + j) + i] = f[j];
        }
      } else {
        FOR(i, l, r) {
          FOR(j, k) f[j] = a[2 * N * (k + j) + i];
          transposed_ntt(f, 0);
          mint r = 1;
          FOR(j, 1, k) r *= z, f[j] *= r;
          transposed_ntt(f, 1);
          FOR(j, k) a[2 * N * j + i] += f[j];
        }
      }
    };

    Z fft = [&](vc<mint> &a, int l, int r, bool t) -> void {
      vc<mint> f(N << 1);
      if (not t) {
        FOR(j, l, r) {
          move(a.begin() + 2 * N * j, a.begin() + 2 * N * (j + 1), f.begin());
          ntt(f, 0);
          move(all(f), a.begin() + 2 * N * j);
        }
      } else {
        FOR(j, l, r) {
          move(a.begin() + 2 * N * j, a.begin() + 2 * N * (j + 1), f.begin());
          transposed_ntt(f, 0);
          move(all(f), a.begin() + 2 * N * j);
        }
      }
    };

    if (N <= k) db(a, 1, N, 0), fft(a, 0, k << 1, 0);
    if (N > k) fft(a, 0, k, 0), db(a, 0, N << 1, 0);

    FOR(i, 2 * N * k) a[i] += 1;
    FOR(i, 2 * N * k, 4 * N * k) a[i] -= 1;
    
    vc<mint> nx(4 * N * k), F(N << 1), G(N << 1), f(N), g(N);
    FOR(j, k << 1) {
      move(a.begin() + 2 * N * j, a.begin() + 2 * N * j + 2 * N, G.begin());
      FOR(i, N) g[i] = G[i << 1] * G[i << 1 | 1];
      ntt(g, 1);
      move(g.begin(), g.begin() + N / 2, nx.begin() + N * j);
    }
    FOR(j, k << 2) nx[N * j] = 0;

    vc<mint> p = dfs(dfs, N >> 1, k << 1, nx);
    FOR_R(j, k << 1) {
      move(p.begin() + N * j, p.begin() + N * j + N / 2, f.begin());
      move(a.begin() + 2 * N * j, a.begin() + 2 * N * j + 2 * N, G.begin());
      fill(f.begin() + N / 2, f.end(), mint(0));
      transposed_ntt(f, 1);
      FOR(i, N) {
        f[i] *= w[i];
        F[i << 1] = G[i << 1 | 1] * f[i], F[i << 1 | 1] = -G[i << 1] * f[i];
      }
      move(all(F), p.begin() + 2 * N * j);
    }
    if (N <= k) fft(p, 0, k << 1, 1), db(p, 0, N, 1);
    if (N > k) db(p, 0, N << 1, 1), fft(p, 0, k, 1);
    return p;
  };

  vc<mint> a(N << 2);
  FOR(i, N) a[i] = -g[i];
  vc<mint> p = dfs(dfs, N, 1, a);
  p.resize(N);
  reverse(all(p));
  p.resize(sz);
  return p;
}

// 2e4 624ms  8e3 266ms  8e3 128ms
template <typename mint>
vc<mint> poly_comp_mtt(vc<mint> f, vc<mint> g) {
  constexpr uint M[3]{167'772'161, 469'762'049, 754'974'721};
  using M0 = modint<M[0]>;
  using M1 = modint<M[1]>;
  using M2 = modint<M[2]>;

  Z dfs = [&](Z &dfs, int N, int k, vc<mint> a) -> vc<mint> {
    if (N == 1) {
      vc<mint> p(k << 1);
      reverse(all(f));
      FOR(i, k) p[i << 1] = f[i];
      return p;
    }
    int sz = 4 * N * k;
    vc<M0> Q0(sz), R0(sz), p0(sz);
    vc<M1> Q1(sz), R1(sz), p1(sz);
    vc<M2> Q2(sz), R2(sz), p2(sz);
    FOR(i, sz / 2) {
      Q0[i] = a[i].val, R0[i] = i & 1 ? (-a[i]).val : a[i].val;
      Q1[i] = a[i].val, R1[i] = i & 1 ? (-a[i]).val : a[i].val;
      Q2[i] = a[i].val, R2[i] = i & 1 ? (-a[i]).val : a[i].val;
    }
    ntt(Q0, 0), ntt(Q1, 0), ntt(Q2, 0);
    ntt(R0, 0), ntt(R1, 0), ntt(R2, 0);
    FOR(i, sz) Q0[i] *= R0[i], Q1[i] *= R1[i], Q2[i] *= R2[i];
    ntt(Q0, 1), ntt(Q1, 1), ntt(Q2, 1);
    vc<mint> qq(sz);
    FOR(i, sz)
      qq[i] = CRT3<mint, M[0], M[1], M[2]>(Q0[i].val, Q1[i].val, Q2[i].val);
    FOR(i, 0, sz / 2, 2) qq[sz / 2 + i] += a[i] + a[i];
    vc<mint> nx(sz / 2);
    FOR(j, k << 1) FOR(i, N >> 1) nx[N * j + i] = qq[2 * N * j + 2 * i];

    vc<mint> nxp = dfs(dfs, N >> 1, k << 1, nx), pq(sz);
    FOR(j, k << 1) FOR(i, N >> 1) pq[2 * N * j + 2 * i + 1] += nxp[N * j + i];

    vc<mint> p(sz / 2);
    FOR(i, sz / 2) p[i] += pq[sz / 2  + i];
    FOR(i, sz) p0[i] += pq[i].val, p1[i] += pq[i].val, p2[i] += pq[i].val;
    transposed_ntt(p0, 1), transposed_ntt(p1, 1), transposed_ntt(p2, 1);
    FOR(i, sz) p0[i] *= R0[i], p1[i] *= R1[i], p2[i] *= R2[i];
    transposed_ntt(p0, 0), transposed_ntt(p1, 0), transposed_ntt(p2, 0);
    FOR(i, sz / 2)
      p[i] += CRT3<mint, M[0], M[1], M[2]>(p0[i].val, p1[i].val, p2[i].val);
    return p;
  };
  assert(len(f) == len(g));
  int N = 1, sz = len(f), k = 1;
  while (N < sz) N <<= 1;
  f.resize(N), g.resize(N);
  vc<mint> a(N << 1);
  FOR(i, N) a[i] = -g[i];
  vc<mint> p = dfs(dfs, N, k, a);
  vc<mint> r(N);
  FOR(i, N) r[i] = p[i];
  reverse(all(r));
  r.resize(sz);
  return r;
}

template <typename mint>
vc<mint> poly_comp(vc<mint> f, vc<mint> g) {
  assert(len(f) == len(g));
  if (f.empty()) return {};
  if (g[0] != mint(0)) f = poly_taylor_shift(f, g[0]), g[0] = 0;
  if constexpr (mint::can_ntt()) return poly_comp_ntt(f, g);
  return poly_comp_mtt(f, g);
}
#line 7 "Composition_of_Formal_Power_Series.cpp"

#define tests 0
#define fl 0
#define DB 10
using mint = M99;
void Yorisou() {
  INT(N);
  VEC(mint, a, N);
  VEC(mint, b, N);
  print(poly_comp(a, b));
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
#line 19 "Composition_of_Formal_Power_Series.cpp"
