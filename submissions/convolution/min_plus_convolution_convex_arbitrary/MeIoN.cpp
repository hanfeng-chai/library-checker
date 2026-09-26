#line 1 "Min_Plus_Convolution_Convex_and_Arbitrary.cpp"
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
using std::cin, std::cout, std::swap, std::iota, std::endl, std::prev, std::next, std::min, std::max, std::tie, std::move;

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

TE<TN T> void reverse(T &a) { std::reverse(all(a)); }
TE<TN T> T reversed(T a) { return std::reverse(all(a)), a; }
TE<TN T> void sort(T &a) { std::sort(all(a)); }
TE<TN T> T sorted(T a) { return std::sort(all(a)), a; }
TE<TN T> void sort(T &a, Z cmp) { std::sort(all(a), cmp); }
TE<TN T> T sorted(T a, Z cmp) { return std::sort(all(a), cmp), a; }
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

namespace fast_io {
static constexpr uint sz = 1 << 17;
char ibuf[sz];
char obuf[sz];
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
  pir = pir - pil + fread(ibuf + pir - pil, 1, sz - pir + pil, stdin);
  pil = 0;
  if (pir < sz) ibuf[pir++] = '\n';
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
  if (por == sz) flush();
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
  if (por > sz - 100) flush();
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
}
#define IN(...) fast_io::read(__VA_ARGS__)
#define print(...) fast_io::println(__VA_ARGS__)
#define FLUSH() fast_io::flush()
#line 5 "Min_Plus_Convolution_Convex_and_Arbitrary.cpp"
// #include "YRS/random/rng.hpp"
#line 2 "YRS/conv/min_add_convolution.hpp"

// C: convex

// O(N + M)
template <typename T>
vc<T> min_add_conv_CC(vc<T> &a, vc<T> &b) {
  int N = len(a), M = len(b);
  if (N == 0 or M == 0) return {};
  vc<T> c(N + M - 1, inf<T>);
  c[0] = a[0] + b[0];
  int i = 0, k = 0;
  FOR(t, i + k + 1, N + M - 1) {
    if (k == M - 1 or (i != N - 1 and a[i + 1] + b[k] < a[i] + b[k + 1])) 
      chmin(c[t], a[++i] + b[k]);
    else
      chmin(c[t], a[i] + b[++k]);
  }
  return c;
}

template <typename T>
vc<T> min_add_conv_CN(vc<T> &a, vc<T> &b) {
  int N = len(a), M = len(b), sz = N + M - 1;
  vc<T> c(sz, inf<T>);
  vc<int> id(sz + 1);
  c[0] = a[0] + b[0];
  id.back() = M - 1;
  int d = 1;
  while (d < sz) d <<= 1;
  for (int q = d >> 1; q; q >>= 1) {
    for (int h = q; h < sz; h += q * 2) {
      int l = h - q, r = min(h + q, sz);
      id[h] = id[l];
      FOR(t, id[l], id[r] + 1) 
        if (t <= h and h - t < N and chmin(c[h], b[t] + a[h - t])) id[h] = t;
    }
  }
  return c;
}

template <typename T, bool a, bool b> 
vc<T> min_add_convolution(vc<T> &A, vc<T> &B) {
  static_assert(a or b);
  if constexpr (a and b) return min_add_conv_CC(A, B);
  else if constexpr (a) return min_add_conv_CN(A, B);
  else if constexpr (b) return min_add_conv_CN(B, A);
}
#line 7 "Min_Plus_Convolution_Convex_and_Arbitrary.cpp"

#define tests 0
#define fl 0
#define DB 10
void Yorisou() {
  INT(N, M);
  VEC(int, a, N);
  VEC(int, b, M);
  print(min_add_convolution<int, 1, 0>(a, b));
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
#line 18 "Min_Plus_Convolution_Convex_and_Arbitrary.cpp"
