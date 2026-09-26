// BEGIN: main.cpp
#line 1 "main.cpp"
#define PROBLEM \
  "https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary"
// BEGIN: my_template.hpp
#line 1 "my_template.hpp"
#if defined(LOCAL)
#include <my_template_compiled.hpp>
#else
#if defined(__GNUC__)
#include <bits/allocator.h>
#pragma GCC optimize("Ofast,unroll-loops")
// 環境によってはコンパイル成功かつ実行時エラー
// #pragma GCC target("avx2,popcnt")
#endif
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i128 = __int128;
using u128 = unsigned __int128;
using f128 = __float128;

template <class T>
constexpr T infty = 0;
template <>
constexpr int infty<int> = 1'010'000'000;
template <>
constexpr ll infty<ll> = 2'020'000'000'000'000'000;
template <>
constexpr u32 infty<u32> = infty<int>;
template <>
constexpr u64 infty<u64> = infty<ll>;
template <>
constexpr i128 infty<i128> = i128(infty<ll>) * 2'000'000'000'000'000'000;
template <>
constexpr double infty<double> = numeric_limits<double>::infinity();
template <>
constexpr long double infty<long double> =
    numeric_limits<long double>::infinity();

using pi = pair<ll, ll>;
using vi = vector<ll>;
template <class T>
using vc = vector<T>;
template <class T>
using vvc = vector<vc<T>>;
template <class T>
using vvvc = vector<vvc<T>>;
template <class T>
using vvvvc = vector<vvvc<T>>;
template <class T>
using pq_max = priority_queue<T>;
template <class T>
using pq_min = priority_queue<T, vector<T>, greater<T>>;

#define vv(type, name, h, ...) \
  vector<vector<type>> name(h, vector<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...)   \
  vector<vector<vector<type>>> name( \
      h, vector<vector<type>>(w, vector<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...)       \
  vector<vector<vector<vector<type>>>> name( \
      a, vector<vector<vector<type>>>(       \
             b, vector<vector<type>>(c, vector<type>(__VA_ARGS__))))

// https://trap.jp/post/1224/
#define FOR1(a) for (ll _ = 0; _ < ll(a); ++_)
#define FOR2(i, a) for (ll i = 0; i < ll(a); ++i)
#define FOR3(i, a, b) for (ll i = a; i < ll(b); ++i)
#define FOR4(i, a, b, c) for (ll i = a; i < ll(b); i += (c))
#define FOR1_R(a) for (ll i = (a) - 1; i >= ll(0); --i)
#define FOR2_R(i, a) for (ll i = (a) - 1; i >= ll(0); --i)
#define FOR3_R(i, a, b) for (ll i = (b) - 1; i >= ll(a); --i)
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

#define all(x) (x).begin(), (x).end()
#define len(x) ll(x.size())
#define elif else if

#define eb emplace_back
#define mp make_pair
#define mt make_tuple
#define fi first
#define se second

#define stoi stoll

int popcnt(int x) { return __builtin_popcount(x); }
int popcnt(u32 x) { return __builtin_popcount(x); }
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt(u64 x) { return __builtin_popcountll(x); }
int popcnt_sgn(int x) { return (__builtin_parity(unsigned(x)) & 1 ? -1 : 1); }
int popcnt_sgn(u32 x) { return (__builtin_parity(x) & 1 ? -1 : 1); }
int popcnt_sgn(ll x) { return (__builtin_parityll(x) & 1 ? -1 : 1); }
int popcnt_sgn(u64 x) { return (__builtin_parityll(x) & 1 ? -1 : 1); }
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 1, 2)
int topbit(int x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
int topbit(u32 x) { return (x == 0 ? -1 : 31 - __builtin_clz(x)); }
int topbit(ll x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
int topbit(u64 x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 0, 2)
int lowbit(int x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
int lowbit(u32 x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
int lowbit(ll x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
int lowbit(u64 x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }

template <typename T>
T kth_bit(int k) {
  return T(1) << k;
}
template <typename T>
bool has_kth_bit(T x, int k) {
  return x >> k & 1;
}

template <typename UINT>
struct all_bit {
  struct iter {
    UINT s;
    iter(UINT s) : s(s) {}
    int operator*() const { return lowbit(s); }
    iter &operator++() {
      s &= s - 1;
      return *this;
    }
    bool operator!=(const iter) const { return s != 0; }
  };
  UINT s;
  all_bit(UINT s) : s(s) {}
  iter begin() const { return iter(s); }
  iter end() const { return iter(0); }
};

template <typename UINT>
struct all_subset {
  static_assert(is_unsigned<UINT>::value);
  struct iter {
    UINT s, t;
    bool ed;
    iter(UINT s) : s(s), t(s), ed(0) {}
    UINT operator*() const { return s ^ t; }
    iter &operator++() {
      (t == 0 ? ed = 1 : t = (t - 1) & s);
      return *this;
    }
    bool operator!=(const iter) const { return !ed; }
  };
  UINT s;
  all_subset(UINT s) : s(s) {}
  iter begin() const { return iter(s); }
  iter end() const { return iter(0); }
};

template <typename T>
T floor(T a, T b) {
  return a / b - (a % b && (a ^ b) < 0);
}
template <typename T>
T ceil(T x, T y) {
  return floor(x + y - 1, y);
}
template <typename T>
T bmod(T x, T y) {
  return x - y * floor(x, y);
}
template <typename T>
pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return {q, x - q * y};
}

constexpr ll TEN[] = {
    1LL,
    10LL,
    100LL,
    1000LL,
    10000LL,
    100000LL,
    1000000LL,
    10000000LL,
    100000000LL,
    1000000000LL,
    10000000000LL,
    100000000000LL,
    1000000000000LL,
    10000000000000LL,
    100000000000000LL,
    1000000000000000LL,
    10000000000000000LL,
    100000000000000000LL,
    1000000000000000000LL,
};

template <typename T, typename U>
T SUM(const U &A) {
  return std::accumulate(A.begin(), A.end(), T{});
}

#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
template <class C, class T>
inline long long LB(const C &c, const T &x) {
  return lower_bound(c.begin(), c.end(), x) - c.begin();
}
template <class C, class T>
inline long long UB(const C &c, const T &x) {
  return upper_bound(c.begin(), c.end(), x) - c.begin();
}
#define UNIQUE(x) \
  sort(all(x)), x.erase(unique(all(x)), x.end()), x.shrink_to_fit()

template <typename T>
T POP(deque<T> &que) {
  T a = que.front();
  que.pop_front();
  return a;
}
template <class T, class Container, class Compare>
T POP(priority_queue<T, Container, Compare> &que) {
  T a = que.top();
  que.pop();
  return a;
}
template <typename T>
T POP(vc<T> &que) {
  T a = que.back();
  que.pop_back();
  return a;
}

template <typename F>
ll binary_search(F check, ll ok, ll ng, bool check_ok = true) {
  if (check_ok) assert(check(ok));
  while (llabs(ok - ng) > 1) {
    auto x = (ng + ok) / 2;
    (check(x) ? ok : ng) = x;
  }
  return ok;
}
template <typename F>
double binary_search_real(F check, double ok, double ng, int iter = 100) {
  FOR(iter) {
    double x = (ok + ng) / 2;
    (check(x) ? ok : ng) = x;
  }
  return (ok + ng) / 2;
}

template <class T, class S>
inline bool chmax(T &a, const S &b) {
  T c = max<T>(a, b);
  bool changed = (c != a);
  a = c;
  return changed;
}
template <class T, class S>
inline bool chmin(T &a, const S &b) {
  T c = min<T>(a, b);
  bool changed = (c != a);
  a = c;
  return changed;
}

// ? は -1
vc<int> s_to_vi(const string &S, char first_char) {
  vc<int> A(S.size());
  FOR(i, S.size()) { A[i] = (S[i] != '?' ? S[i] - first_char : -1); }
  return A;
}

template <typename T, typename U>
vc<T> cumsum(const vc<U> &A, int off = 1) {
  int N = A.size();
  vc<T> B(N + 1);
  FOR(i, N) { B[i + 1] = B[i] + A[i]; }
  if (off == 0) B.erase(B.begin());
  return B;
}

// stable sort
template <typename T>
vc<int> argsort(const vc<T> &A) {
  vc<int> ids(len(A));
  iota(all(ids), 0);
  sort(all(ids),
       [&](int i, int j) { return (A[i] == A[j] ? i < j : A[i] < A[j]); });
  return ids;
}

// A[I[0]], A[I[1]], ...
template <typename T>
vc<T> rearrange(const vc<T> &A, const vc<int> &I) {
  vc<T> B(len(I));
  FOR(i, len(I)) B[i] = A[I[i]];
  return B;
}

template <typename T, typename... Vectors>
void concat(vc<T> &first, const Vectors &...others) {
  vc<T> &res = first;
  (res.insert(res.end(), others.begin(), others.end()), ...);
}
#endif
// END: my_template.hpp
#line 4 "main.cpp"
// BEGIN: other/io.hpp
#line 1 "other/io.hpp"
#define FASTIO

// https://judge.yosupo.jp/submission/21623
namespace fastio {
static constexpr uint32_t SZ = 1 << 17;
char ibuf[SZ];
char obuf[SZ];
char out[100];
// pointer of ibuf, obuf
uint32_t pil = 0, pir = 0, por = 0;

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
  memmove(ibuf, ibuf + pil, pir - pil);
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
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (c == '-') {
      minus = 1, c = ibuf[pil++];
    }
  }
  x = 0;
  while ('0' <= c) {
    x = x * 10 + (c & 15), c = ibuf[pil++];
  }
  if constexpr (is_signed<T>::value || is_same_v<T, i128>) {
    if (minus) x = -x;
  }
}

template <class T>
enable_if_t<is_integral_v<T> || is_same_v<T, i128> || is_same_v<T, u128>> rd(
    T &x) {
  rd_integer(x);
}

template <class T>
enable_if_t<is_floating_point_v<T> || is_same_v<T, f128>> rd(T &x) {
  rd_real(x);
}

template <class T, class U>
void rd(pair<T, U> &p) {
  rd(p.first), rd(p.second);
}
template <size_t N = 0, typename T>
void rd_tuple(T &t) {
  if constexpr (N < tuple_size<T>::value) {
    auto &x = get<N>(t);
    rd(x);
    rd_tuple<N + 1>(t);
  }
}
template <class... T>
void rd(tuple<T...> &tpl) {
  rd_tuple(tpl);
}

template <size_t N = 0, typename T>
void rd(array<T, N> &x) {
  for (auto &d : x) rd(d);
}
template <class T>
void rd(vc<T> &x) {
  for (auto &d : x) rd(d);
}

void read() {}
template <class H, class... T>
void read(H &h, T &...t) {
  rd(h), read(t...);
}

inline void wt_range(const char *s, size_t n) {
  size_t i = 0;
  while (i < n) {
    if (por == SZ) flush();
    size_t chunk = min(n - i, (size_t)(SZ - por));
    memcpy(obuf + por, s + i, chunk);
    por += chunk;
    i += chunk;
  }
}

void wt(const char c) {
  if (por == SZ) flush();
  obuf[por++] = c;
}
void wt(const char *s) { wt_range(s, strlen(s)); }
void wt(const string &s) { wt_range(s.data(), s.size()); }

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
inline void wt_real(T x) {
  static char buf[1000];
  int n = std::snprintf(buf, sizeof(buf), "%.15f", (double)x);
  wt_range(buf, (size_t)n);
}

template <class T>
enable_if_t<is_integral_v<T> || is_same_v<T, i128> || is_same_v<T, u128>> wt(
    T x) {
  wt_integer(x);
}

template <class T>
enable_if_t<is_floating_point_v<T> || is_same_v<T, f128>> wt(T x) {
  wt_real(x);
}

inline void wt(bool b) { wt(static_cast<char>('0' + (b ? 1 : 0))); }

template <class T, class U>
void wt(const pair<T, U> &val) {
  wt(val.first);
  wt(' ');
  wt(val.second);
}
template <size_t N = 0, typename T>
void wt_tuple(const T &t) {
  if constexpr (N < tuple_size<T>::value) {
    if constexpr (N > 0) wt(' ');
    wt(get<N>(t));
    wt_tuple<N + 1>(t);
  }
}
template <class... T>
void wt(const tuple<T...> &tpl) {
  wt_tuple(tpl);
}
template <class T, size_t S>
void wt(const array<T, S> &val) {
  auto n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt(' ');
    wt(val[i]);
  }
}
template <class T>
void wt(const vector<T> &val) {
  auto n = val.size();
  for (size_t i = 0; i < n; i++) {
    if (i) wt(' ');
    wt(val[i]);
  }
}

void print() { wt('\n'); }
template <class Head, class... Tail>
void print(Head &&head, Tail &&...tail) {
  wt(head);
  if (sizeof...(Tail)) wt(' ');
  print(forward<Tail>(tail)...);
}

// gcc expansion. called automaticall after main.
void __attribute__((destructor)) _d() { flush(); }
}  // namespace fastio
using fastio::flush;
using fastio::print;
using fastio::read;

#if defined(LOCAL)
#define HDR "[DEBUG:", __func__, __LINE__, "]"
#define SHOW(...)                                                         \
  SHOW_IMPL(__VA_ARGS__, SHOW8, SHOW7, SHOW6, SHOW5, SHOW4, SHOW3, SHOW2, \
            SHOW1)                                                        \
  (__VA_ARGS__)
#define SHOW_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define SHOW1(x) print(HDR, #x, "=", (x)), flush()
#define SHOW2(x, y) print(HDR, #x, "=", (x), #y, "=", (y)), flush()
#define SHOW3(x, y, z) \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z)), flush()
#define SHOW4(x, y, z, w) \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w)), flush()
#define SHOW5(x, y, z, w, v)                                                  \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
        (v)),                                                                 \
      flush()
#define SHOW6(x, y, z, w, v, u)                                               \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
        (v), #u, "=", (u)),                                                   \
      flush()
#define SHOW7(x, y, z, w, v, u, t)                                            \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
        (v), #u, "=", (u), #t, "=", (t)),                                     \
      flush()
#define SHOW8(x, y, z, w, v, u, t, s)                                         \
  print(HDR, #x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
        (v), #u, "=", (u), #t, "=", (t), #s, "=", (s)),                       \
      flush()
#else
#define SHOW(...)
#endif

#define INT(...)   \
  int __VA_ARGS__; \
  read(__VA_ARGS__)
#define LL(...)   \
  ll __VA_ARGS__; \
  read(__VA_ARGS__)
#define U32(...)   \
  u32 __VA_ARGS__; \
  read(__VA_ARGS__)
#define U64(...)   \
  u64 __VA_ARGS__; \
  read(__VA_ARGS__)
#define STR(...)      \
  string __VA_ARGS__; \
  read(__VA_ARGS__)
#define CHAR(...)   \
  char __VA_ARGS__; \
  read(__VA_ARGS__)
#define DBL(...)      \
  double __VA_ARGS__; \
  read(__VA_ARGS__)

#define VEC(type, name, size) \
  vector<type> name(size);    \
  read(name)
#define VV(type, name, h, w)                     \
  vector<vector<type>> name(h, vector<type>(w)); \
  read(name)

void YES(bool t = 1) { print(t ? "YES" : "NO"); }
void NO(bool t = 1) { YES(!t); }
void Yes(bool t = 1) { print(t ? "Yes" : "No"); }
void No(bool t = 1) { Yes(!t); }
void yes(bool t = 1) { print(t ? "yes" : "no"); }
void no(bool t = 1) { yes(!t); }
void YA(bool t = 1) { print(t ? "YA" : "TIDAK"); }
void TIDAK(bool t = 1) { YA(!t); }
// END: other/io.hpp
#line 5 "main.cpp"

// BEGIN: convex/minplus_convolution.hpp
#line 1 "convex/minplus_convolution.hpp"
// BEGIN: convex/monotone_minima.hpp
#line 1 "convex/monotone_minima.hpp"

// 各行の最適列を求める
// better(i,j,k): 行 i において列 k が列 j より良いとき true
// 適用条件：better によって選ばれる最適列 opt[i] が i について広義単調増加
template <typename F>
vc<int> monotone_minima(int H, int W, F better) {
  if (H == 0) return {};
  assert(H > 0 && W > 0);

  vc<int> idx(H + 1);
  idx[0] = 0;
  FOR(y, 1, W) {
    if (better(0, idx[0], y)) idx[0] = y;
  }
  idx[H] = W - 1;

  int d = 1;
  while (d < H) d <<= 1;
  for (int q = d >> 1; q > 0; q >>= 1) {
    for (int h = q; h < H; h += q << 1) {
      int l = h - q;
      int r = min(h + q, H);
      int best = idx[l];
      for (int y = idx[l] + 1; y <= idx[r]; ++y) {
        if (better(h, best, y)) best = y;
      }
      idx[h] = best;
    }
  }
  idx.pop_back();
  return idx;
}// END: convex/monotone_minima.hpp
#line 2 "convex/minplus_convolution.hpp"

template <typename T>
vc<T> minplus_convolution_convex_convex(vc<T>& A, vc<T>& B) {
  int n = len(A), m = len(B);
  if (n == 0 && m == 0) return {};
  vc<T> C(n + m - 1, infty<T>);
  while (n > 0 && A[n - 1] == infty<T>) --n;
  while (m > 0 && B[m - 1] == infty<T>) --m;
  if (n == 0 || m == 0) return C;
  int a = 0, b = 0;
  while (a < n && A[a] == infty<T>) ++a;
  while (b < m && B[b] == infty<T>) ++b;
  C[a + b] = A[a] + B[b];
  for (int i = a + b + 1; i < n + m - 1; ++i) {
    if (b == m - 1 || (a != n - 1 && A[a + 1] + B[b] < A[a] + B[b + 1])) {
      chmin(C[i], A[++a] + B[b]);
    } else {
      chmin(C[i], A[a] + B[++b]);
    }
  }
  return C;
}

template <typename T>
vc<T> minplus_convolution_arbitrary_convex(vc<T>& A, vc<T>& B) {
  int n = len(A), m0 = len(B);
  if (n == 0 || m0 == 0) return {};
  vc<T> C(n + m0 - 1, infty<T>);
  int m = m0;
  while (m > 0 && B[m - 1] == infty<T>) --m;
  if (m == 0) return C;
  int b = 0;
  while (b < m && B[b] == infty<T>) ++b;

  int z = n + m - b - 1;
  vc<int> idx(z + 1);
  C[b] = A[0] + B[b];
  idx[0] = 0, idx[z] = n - 1;

  int d = 1;
  while (d < z) d <<= 1;
  for (int q = d >> 1; q > 0; q >>= 1) {
    for (int h = q; h < z; h += q << 1) {
      int l = h - q;
      int r = min(h + q, z);
      idx[h] = idx[l];
      for (int j = idx[l]; j <= idx[r]; ++j) {
        if (j <= h && h - j < m - b && C[b + h] > A[j] + B[b + h - j]) {
          C[b + h] = A[j] + B[b + h - j];
          idx[h] = j;
        }
      }
    }
  }
  return C;
}

template <typename T, bool convA, bool convB>
vc<T> minplus_convolution(vc<T>& A, vc<T>& B) {
  static_assert(convA || convB);
  if constexpr (convA && convB) return minplus_convolution_convex_convex(A, B);
  if constexpr (convA && !convB)
    return minplus_convolution_arbitrary_convex(B, A);
  if constexpr (convB && !convA)
    return minplus_convolution_arbitrary_convex(A, B);
  return {};
}
// END: convex/minplus_convolution.hpp
#line 7 "main.cpp"

void solve() {
  LL(N, M);
  VEC(ll, A, N);
  VEC(ll, B, M);
  print(minplus_convolution<ll, 1, 0>(A, B));
}

signed main() {
  solve();
  return 0;
}
// END: main.cpp
