#line 1 "Count_Points_in_Triangles.cpp"
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
      std::ostream, std::string, std::vector, std::tuple, std::cerr;
using std::cin, std::cout, std::swap, std::iota, std::endl, std::prev,
      std::next, std::min, std::max, std::tie, std::move, std::reverse, std::copy,
      std::gcd, std::lcm, std::abs, std::sin, std::cos, std::atan2, std::acos;

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
using u16 = uint16_t;
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
#define VEC(T, a, n) vc<T> a(n); IN(a)
#define VVEC(T, a, n, m) vvc<T> a(n, vc<T>(m)); IN(a)

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

TE<TN T> constexpr static inline int popcount(T x) {
  using U = std::make_unsigned_t<T>;
  return std::__popcount(static_cast<U>(x));
}
TE<TN T> constexpr static inline int pc(T x) { return popcount(x); }
TE<TN T> constexpr static inline ll len(const T &a) { return a.size(); }
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
inline Z LB(const Z &a, Z x) { return std::lower_bound(all(a), x); }
inline Z UB(const Z &a, Z x) { return std::upper_bound(all(a), x); }
int lb(const Z &a, Z x) { return LB(a, x) - a.begin(); }
int ub(const Z &a, Z x) { return UB(a, x) - a.begin(); }

TE<bool ck = true> ll bina(const Z &F, ll L, ll R) {
  if constexpr (ck) assert(F(L));
  while (abs(L - R) > 1) {
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

template <typename T>
inline void sh(vc<T> &a, int N) {
  a.resize(N, T(0));
}
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
#line 4 "Count_Points_in_Triangles.cpp"
// #include "YRS/IO/fast_io.hpp"
// #include "YRS/random/rng.hpp"
#line 2 "YRS/ge/exp/count_p_in_tri.hpp"

#line 2 "YRS/ge/basic/point.hpp"

template <typename T>
struct point {
  using P = point;
  T x, y;
  point(T x = 0, T y = 0) : x(x), y(y) {}

  P &operator+=(P p) { return x += p.x, y += p.y, *this; }
  P &operator-=(P p) { return x -= p.x, y -= p.y, *this; }
  P &operator*=(T w) { return x *= w, y *= w, *this; }
  P &operator/=(T w) { return x /= w, y /= w, *this; }

  P operator+(P p) const { return P(*this) += p; }
  P operator-(P p) const { return P(*this) -= p; }
  P operator*(T p) const { return P(*this) *= p; }
  P operator/(T p) const { return P(*this) /= p; }

  bool operator<(P p) const { return x != p.x ? x < p.x : y < p.y; }
  bool operator>(P p) const { return x != p.x ? x > p.x : y > p.y; }
  bool operator==(P p) const { return x == p.x and y == p.y; }
  bool operator!=(P p) const { return x != p.x or y != p.y; }

  P operator-() const { return P{-x, -y}; }

  T dot(P p) const { return x * p.x + y * p.y; }
  T det(P p) const { return x * p.y - y * p.x; }

  T square() const { return x * x + y * y; }

  ld length() const { return sqrtl(square()); }

  // [0, 2pi) 与 x轴 的夹角
  ld angle() const {
    ld ang = atan2(y, x);
    return ang < 0 ? ang + pi * 2 : ang;
  }

  static ld angle(P A, P B, P C) { // ∠ABC 只求劣角
    ld ang = atan2((A - B).det(C - B), (A - B).dot(C - B));
    return abs(ang);
  }

  P rotate(double ang) const { // float 逆时针
    ld C = cos(ang), S = sin(ang);
    return P{C * x - S * y, S * x + C * y};
  }
  P rol90(bool ccw = true) const { return ccw ? P{-y, x} : P{y, -x}; }

  P nor() const { return (*this) / length(); }

  P div(P p) const {
    T d = p.square();
    return P{(x * p.x + y * p.y) / d, (y * p.x - x * p.y) / d};
  }
};

template <typename T>
istream &operator>>(istream &I, point<T> &p) {
  return I >> p.x >> p.y;
}
template <typename T>
ostream &operator<<(ostream &I, const point<T> &p) {
  return I << p.x << ' ' << p.y;
}

// A -> B -> C 逆时针为 +1，顺时针为 -1。
template <typename T>
int ccw(point<T> a, point<T> b, point<T> c) {
  T x = (b - a).det(c - a);
  return x > 0 ? 1 : x < 0 ? -1 : 0;
}

#ifdef FIO
template <typename T>
void rd(point<T> &p) {
  T x, y;
  IN(x, y);
  p = {x, y};
}
template <typename T>
void wt(point<T> x) {
  wt(x.x);
  wt(' ');
  wt(x.y);
}
#endif
#line 2 "YRS/ge/basic/angle_sort.hpp"

#line 4 "YRS/ge/basic/angle_sort.hpp"

template <typename T>
int ang_st(point<T> p) {
  if (p.y) return p.y > 0 ? 1 : -1;
  return p.x > 0 ? -1 : p.x < 0 ? 1 : 0;
}

template <typename T>
int ang_cmp(point<T> L, point<T> R) {
  int a = ang_st(L), b = ang_st(R);
  if (a != b) return a < b ? -1 : 1;
  T t = L.det(R);
  return t > 0 ? -1 : t < 0 ? 1 : 0;
}

// 即使值全是整数 用浮点数还是会出问题
template <typename T>
vc<int> angle_sort(const vc<point<T>> &a) {
  const int N = len(a);
  vc<int> I(N);
  iota(all(I), 0);
  sort(I, [&](int x, int y) { return ang_cmp(a[x], a[y]) == -1; });
  return I;
}
#line 5 "YRS/ge/exp/count_p_in_tri.hpp"

struct count_p_in_tri {
  struct BIT {
    int N;
    vc<int> a;
    BIT(int N) : N(N), a(N) {}

    void add(int i) {
      for (++i; i <= N; i += i & -i) ++a[i - 1];
    }
    int prod(int i) {
      int s = 0;
      for (; i > 0; i -= i & -i) s += a[i - 1];
      return s;
    }
    int prod(int l, int r) { return prod(r) - prod(l); }
  };
  using P = point<ll>;
  static constexpr int lm = 1'000'000'000 + 10;
  vc<P> a, b;
  vc<int> id, c;
  vc<vc<int>> seg, tr;

  count_p_in_tri(const vc<P> &a, const vc<P> &b) : a(a), b(b) {
    build();
  }
  void build() {
    P bs = {-lm, -lm + 1145141919};
    for (P &x : a) x -= bs;
    for (P &x : b) x -= bs;
    int N = len(a), M = len(b);
    vc<int> I = angle_sort(a);
    a = rearrange(a, I);
    id.resize(N);
    FOR(i, N) id[I[i]] = i;
    I = angle_sort(b);
    b = rearrange(b, I);
    c.assign(N, 0);
    seg.assign(N, vc<int>(N));
    tr.assign(N, vc<int>(N));

    FOR(i, N) FOR(k, M) if (a[i] == b[k]) ++c[i];
    int m = 0;
    FOR(j, N) {
      while (m < M and a[j].det(b[m]) < 0) ++m;
      vc<P> C(M);
      FOR(k, m) C[k] = b[k] - a[j];
      vc<int> I(m);
      iota(all(I), 0);
      sort(I, [&](int i, int k) { return C[i].det(C[k]) > 0; });
      C = rearrange(C, I);
      vc<int> rk(m);
      FOR(k, m) rk[I[k]] = k;
      BIT bit(m);
      int k = m;
      for (int i = j; i--; ) {
        while (k > 0 and a[i].det(b[k - 1]) > 0) bit.add(rk[--k]);
        P p = a[i] - a[j];
        int l = bina(
            [&](int i) { return i == 0 ? 1 : C[i - 1].det(p) > 0; }, 0, m + 1);
        int r = bina(
            [&](int i) { return i == 0 ? 1 : C[i - 1].det(p) >= 0; }, 0, m + 1);
        seg[i][j] += bit.prod(l, r);
        tr[i][j] += bit.prod(l);
      }
    }
  }

  int operator()(int i, int j, int k) {
    i = id[i], j = id[j], k = id[k];
    if (i > j) swap(i, j);
    if (j > k) swap(j, k);
    if (i > j) swap(i, j);
    ll d = (a[j] - a[i]).det(a[k] - a[i]);
    if (d == 0) return 0;
    if (d > 0) return tr[i][j] + tr[j][k] - tr[i][k] - seg[i][k];
    int x = tr[i][k] - tr[i][j] - tr[j][k];
    return x - seg[i][j] - seg[j][k] - c[j];
  }
  int operator()(int i, int k) {
    i = id[i], k = id[k];
    if (i > k) swap(i, k);
    return seg[i][k];
  }
};
#line 7 "Count_Points_in_Triangles.cpp"

#define tests 0
#define fl 0
#define DB 10
using P = point<ll>;
void Yorisou() {
  INT(N);
  VEC(P, a, N);
  INT(M);
  VEC(P, b, M);
  count_p_in_tri g(a, b);
  INT(Q);
  FOR(Q) {
    INT(i, k, j);
    print(g(i, k, j));
  }
}
#line 1 "YRS/aa/main.hpp"
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int T = 1;
  if (fl) cerr.tie(0);
  if (tests and not fl) IN(T);
  for (int i = 0; i < T or fl; ++i) {
    Yorisou();
    if (fl and i % DB == 0) cerr << "Case: " << i << '\n';
  }
  return 0;
}
#line 25 "Count_Points_in_Triangles.cpp"
