#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cfenv>
#include <cfloat>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <streambuf>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <bit>
#include <compare>
#include <concepts>
#include <numbers>
#include <ranges>
#include <span>

//#define int ll
#define INT128_MAX (__int128)(((unsigned __int128) 1 << ((sizeof(__int128) * __CHAR_BIT__) - 1)) - 1)
#define INT128_MIN (-INT128_MAX - 1)

#define pb push_back
#define eb emplace_back
#define clock chrono::steady_clock::now().time_since_epoch().count()

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream& os, const pair<T1, T2> pr) {
  return os << pr.first << ' ' << pr.second;
}
template<class T, size_t N>
ostream& operator<<(ostream& os, const array<T, N> &arr) {
  for(size_t i = 0; T x : arr) {
    os << x;
    if (++i != N) os << ' ';
  }
  return os;
}
template<class T>
ostream& operator<<(ostream& os, const vector<T> &vec) {
  for(size_t i = 0; T x : vec) {
    os << x;
    if (++i != size(vec)) os << ' ';
  }
  return os;
}
template<class T>
ostream& operator<<(ostream& os, const set<T> &s) {
  for(size_t i = 0; T x : s) {
    os << x;
    if (++i != size(s)) os << ' ';
  }
  return os;
}
template<class T1, class T2>
ostream& operator<<(ostream& os, const map<T1, T2> &m) {
  for(size_t i = 0; pair<T1, T2> x : m) {
    os << x;
    if (++i != size(m)) os << ' ';
  }
  return os;
}

#ifdef DEBUG
#define dbg(...) cerr << '(', _do(#__VA_ARGS__), cerr << ") = ", _do2(__VA_ARGS__)
template<typename T> void _do(T &&x) { cerr << x; }
template<typename T, typename ...S> void _do(T &&x, S&&...y) { cerr << x << ", "; _do(y...); }
template<typename T> void _do2(T &&x) { cerr << x << endl; }
template<typename T, typename ...S> void _do2(T &&x, S&&...y) { cerr << x << ", "; _do2(y...); }
#else
#define dbg(...)
#endif

using ll = long long;
using ull = unsigned long long;
using ldb = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
//#define double ldb

template<typename T> using min_heap = priority_queue<T, vector<T>, greater<T>>;
template<typename T> using max_heap = priority_queue<T>;

template<ranges::forward_range rng, class T = ranges::range_value_t<rng>, class OP = plus<T>>
void pSum(rng &&v) {
  if (!v.empty())
    for(T p = v[0]; T &x : v | views::drop(1))
      x = p = OP()(p, x);
}
template<ranges::forward_range rng, class T = ranges::range_value_t<rng>, class OP>
void pSum(rng &&v, OP op) {
  if (!v.empty())
    for(T p = v[0]; T &x : v | views::drop(1))
      x = p = op(p, x);
}

template<ranges::forward_range rng>
void Unique(rng &v) {
  ranges::sort(v);
  v.resize(unique(v.begin(), v.end()) - v.begin());
}

template<ranges::random_access_range rng>
rng invPerm(rng p) {
  rng ret = p;
  for(int i = 0; i < ssize(p); i++)
    ret[p[i]] = i;
  return ret;
}

template<ranges::random_access_range rng, ranges::random_access_range rng2>
rng Permute(rng v, rng2 p) {
  rng ret = v;
  for(int i = 0; i < ssize(p); i++)
    ret[p[i]] = v[i];
  return ret;
}

template<bool directed>
vector<vector<int>> readGraph(int n, int m, int base) {
  vector<vector<int>> g(n);
  for(int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    u -= base, v -= base;
    g[u].emplace_back(v);
    if constexpr (!directed)
      g[v].emplace_back(u);
  }
  return g;
}

template<class T>
void setBit(T &msk, int bit, bool x) {
  msk = (msk & ~(T(1) << bit)) | (T(x) << bit);
}
template<class T> void flipBit(T &msk, int bit) { msk ^= T(1) << bit; }
template<class T> bool getBit(T msk, int bit) { return msk >> bit & T(1); }

template<class T>
T floorDiv(T a, T b) {
  if (b < 0) a *= -1, b *= -1;
  return a >= 0 ? a / b : (a - b + 1) / b;
}
template<class T>
T ceilDiv(T a, T b) {
  if (b < 0) a *= -1, b *= -1;
  return a >= 0 ? (a + b - 1) / b : a / b;
}

template<class T> bool chmin(T &a, T b) { return a > b ? a = b, 1 : 0; }
template<class T> bool chmax(T &a, T b) { return a < b ? a = b, 1 : 0; }

template<class T>
struct fenwickTree {
  const int size;
  vector<T> data;

  fenwickTree(int _size) : size(_size + 1), data(_size + 1) {}
  fenwickTree(vector<T> &init) : size(ssize(init) + 1), data(ssize(init) + 1) {
    partial_sum(init.begin(), init.end(), data.begin() + 1);
    for(int i = size - 1; i > 0; i--)
      data[i] -= data[i - (i & (-i))];
  }

  void add(int i, T d) {
    for(i += 1; i < size; i += i & (-i))
      data[i] += d;
  }

  T query(int i) {
    T res = T(0);
    for(i += 1; i > 0; i -= i & (-i))
      res += data[i];
    return res;
  }

  T query(int l, int r) { //query [l, r)
    return query(r - 1) - query(l - 1);
  }
};

//#include<ds/fenwickTree.cpp>

template<class T>
struct point_set_range_frequency {
  vector<T> a;
  int n, now = 0;
  struct Query {
    T x;
    int t, l, r, id;
  };
  vector<Query> q;
  struct Modification {
    T x;
    int t, i;
    bool add;
  };
  vector<Modification> m;

  point_set_range_frequency(vector<T> &_a)
    : a(_a), n(ssize(a)), m(n) {
    for(int i = 0; i < n; i++)
      m[i] = {a[i], 0, i, true};
  };

  void modify(int i, T x) {
    m.push_back({a[i], now, i, false});
    m.push_back({a[i] = x, now, i, true});
  }

  void query(int l, int r, T x) {
    q.push_back({x, now++, l, r, (int)size(q)});
  }

  vector<int> solve() {
    for(int i = 0; i < n; i++)
      m.push_back({a[i], now, i, false});
    ranges::sort(q, {}, [&](auto &e) { return pair(e.x, e.t); });
    ranges::sort(m, {}, [&](auto &e) { return pair(e.x, e.t); });
    vector<int> ans(ssize(q));
    fenwickTree<int> ft(n);
    for(int i = 0; auto [x, t, l, r, id] : q) {
      while(i < ssize(m) and pair(m[i].x, m[i].t) <= pair(x, t))
        ft.add(m[i].i, m[i].add ? 1 : -1), i++;
      ans[id] = ft.query(l, r);
    }
    return ans;
  }
};

template<class M, M(*id)(), M(*op)(const M&, const M&)>
struct segmentTree {
  int size;
  vector<M> data;

  segmentTree(int _size) : size(_size), data(2 * size, id()) {}
  segmentTree(vector<M> init) : size(ssize(init)), data(2 * size, id()) {
    ranges::copy(init, data.begin() + size);
    for(int i = size - 1; i > 0; i--)
      data[i] = op(data[i << 1], data[i << 1 | 1]);
  }

  void set(int i, M x) {
    data[i += size] = x;
    while(i >>= 1)
      data[i] = op(data[i << 1], data[i << 1 | 1]);
  }

  M get(int i) { return data[i + size]; }

  M query(int l, int r) {
    M L = id(), R = id();
    for(l += size, r += size; l < r; l >>= 1, r >>= 1) {
      if (l & 1) L = op(L, data[l++]);
      if (r & 1) R = op(data[--r], R);
    }
    return op(L, R);
  }

  //return first j in [i, size) s.t. f(op([l, j])) is true,
  //assume f(id()) is false.
  int firstTrue(int i, function<bool(const M&)> f) {
    vector<int> idL, idR;
    for(int l = i + size, r = size << 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) idL.emplace_back(l++);
      if (r & 1) idR.emplace_back(--r);
    }
    idL.insert(idL.end(), idR.rbegin(), idR.rend());
    M pre = id();
    int v = -1;
    for(int j : idL) {
      if (f(op(pre, data[j]))) {
        v = j;
        break;
      } else {
        pre = op(pre, data[j]);
      }
    }
    if (v == -1) return size;
    while(v < size) {
      if (f(op(pre, data[v << 1])))
        v = v << 1;
      else
        pre = op(pre, data[v << 1]), v = v << 1 | 1;
    }
    return v - size;
  }

  int lastTrue(int i, function<bool(const M&)> f) {
    vector<int> idL, idR;
    for(int l = size, r = (i + 1) + size; l < r; l >>= 1, r >>= 1) {
      if (l & 1) idL.emplace_back(l++);
      if (r & 1) idR.emplace_back(--r);
    }
    idR.insert(idR.end(), idL.rbegin(), idL.rend());
    M suf = id();
    int v = -1;
    for(int j : idR) {
      if (f(op(data[j], suf))) {
        v = j;
        break;
      } else {
        suf = op(data[j], suf);
      }
    }
    if (v == -1) return -1;
    while(v < size) {
      if (f(op(data[v << 1 | 1], suf)))
        v = v << 1 | 1;
      else
        suf = op(data[v << 1 | 1], suf), v = v << 1;
    }
    return v - size;
  }
};

struct M { int x, f; };
M op(const M &a, const M &b) { return a.x == b.x ? M{a.x, a.f + b.f} : a.f > b.f ? M{a.x, a.f - b.f} : M{b.x, b.f - a.f}; }
M id() { return M{0, 0}; }

signed main() {
  ios::sync_with_stdio(false), cin.tie(NULL);

  int n, q; cin >> n >> q;
  vector<int> a(n);
  for(int &x : a) cin >> x;
  vector<array<int, 3>> qry(q);
  for(auto &[x, y, z] : qry) cin >> x >> y >> z;

  vector<M> init(n);
  for(int i = 0; i < n; i++)
    init[i] = {a[i], 1};
  segmentTree<M, id, op> st(init);
  point_set_range_frequency<int> ds(a);
  vector<pii> ans;
  for(auto [op, y, z] : qry) {
    if (op == 0) {
      ds.modify(y, z), st.set(y, M{z, 1});
    } else {
      int val = st.query(y, z).x;
      ans.eb(val, z - y), ds.query(y, z, val);
    }
  }

  auto f = ds.solve();
  for(int i = 0; i < ssize(ans); i++)
    cout << (2 * f[i] > ans[i].second ? ans[i].first : -1) << '\n';

  return 0;
}
