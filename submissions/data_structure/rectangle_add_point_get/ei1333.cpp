#include <bits/stdc++.h>

using namespace std;

using int64 = long long;

const int64 infll = (1LL << 62) - 1;
const int inf = (1 << 30) - 1;

struct IoSetup {
  IoSetup() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(10);
    cerr << fixed << setprecision(10);
  }
} iosetup;

template<typename T1, typename T2>
ostream &operator<<(ostream &os, const pair<T1, T2> &p) {
  os << p.first << " " << p.second;
  return os;
}

template<typename T1, typename T2>
istream &operator>>(istream &is, pair<T1, T2> &p) {
  is >> p.first >> p.second;
  return is;
}

template<typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  for (int i = 0; i < (int) v.size(); i++) {
    os << v[i] << (i + 1 != v.size() ? " " : "");
  }
  return os;
}

template<typename T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &in : v) is >> in;
  return is;
}

template<typename T1, typename T2>
inline bool chmax(T1 &a, T2 b) {
  return a < b && (a = b, true);
}

template<typename T1, typename T2>
inline bool chmin(T1 &a, T2 b) {
  return a > b && (a = b, true);
}

template<typename T = int64>
vector<T> make_v(size_t a) {
  return vector<T>(a);
}

template<typename T, typename... Ts>
auto make_v(size_t a, Ts... ts) {
  return vector<decltype(make_v<T>(ts...))>(a, make_v<T>(ts...));
}

template<typename T, typename V>
typename enable_if<is_class<T>::value == 0>::type fill_v(T &t, const V &v) {
  t = v;
}

template<typename T, typename V>
typename enable_if<is_class<T>::value != 0>::type fill_v(T &t, const V &v) {
  for (auto &e : t) fill_v(e, v);
}

template<typename F>
struct FixPoint : F {
  explicit FixPoint(F &&f) : F(forward<F>(f)) {}

  template<typename... Args>
  decltype(auto) operator()(Args &&...args) const {
    return F::operator()(*this, forward<Args>(args)...);
  }
};

template<typename F>
inline decltype(auto) MFP(F &&f) {
  return FixPoint<F>{forward<F>(f)};
}

#line 1 "structure/others/binary-indexed-tree.hpp"
/**
 * @brief Binary-Indexed-Tree(BIT)
 *
 */
template <typename T>
struct BinaryIndexedTree {
 private:
  int n;
  vector<T> data;

 public:
  BinaryIndexedTree() = default;

  explicit BinaryIndexedTree(int n) : n(n) { data.assign(n + 1, T()); }

  explicit BinaryIndexedTree(const vector<T> &v)
      : BinaryIndexedTree((int)v.size()) {
    build(v);
  }

  void build(const vector<T> &v) {
    assert(n == (int)v.size());
    for (int i = 1; i <= n; i++) data[i] = v[i - 1];
    for (int i = 1; i <= n; i++) {
      int j = i + (i & -i);
      if (j <= n) data[j] += data[i];
    }
  }

  void apply(int k, const T &x) {
    for (++k; k <= n; k += k & -k) data[k] += x;
  }

  T prod(int r) const {
    T ret = T();
    for (; r > 0; r -= r & -r) ret += data[r];
    return ret;
  }

  T prod(int l, int r) const { return prod(r) - prod(l); }

  int lower_bound(T x) const {
    int i = 0;
    for (int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
      if (i + k <= n && data[i + k] < x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }

  int upper_bound(T x) const {
    int i = 0;
    for (int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
      if (i + k <= n && data[i + k] <= x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }
};

template <typename T, typename C>
struct StaticRectangleAddPointGet {
 private:
  using BIT = BinaryIndexedTree<C>;

  static_assert(is_integral<T>::value,
                "template parameter T must be integral type");

  struct Rectangle {
    T l, d, r, u;
    C w;
  };

  struct Query {
    T x, y;
  };

  vector<Rectangle> rectangles;
  vector<Query> queries;

 public:
  StaticRectangleAddPointGet() = default;

  explicit StaticRectangleAddPointGet(int n, int q) {
    rectangles.reserve(n);
    queries.reserve(q);
  }

  void add_rectangle(T l, T d, T r, T u, C w) {
    rectangles.emplace_back(Rectangle{l, d, r, u, w});
  }

  void add_query(T x, T y) {
    queries.emplace_back(Query{x, y});
  }

  vector<C> calculate_queries() const {
    int n = (int)rectangles.size();
    int q = (int)queries.size();
    vector<C> ans(q);
    if (rectangles.empty() or queries.empty()) {
      return ans;
    }

    vector<T> ys;
    ys.reserve(q);
    for(int i = 0; i < q; i++) {
      ys.emplace_back(queries[i].y);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    struct R {
      T x;
      int d, u;
      bool type;
      C w;
    };
    vector<R> rs;
    rs.reserve(n + n);
    for (int i = 0; i < n; i++) {
      auto &rect = rectangles[i];
      int d = lower_bound(ys.begin(), ys.end(), rect.d) - ys.begin();
      int u = lower_bound(ys.begin(), ys.end(), rect.u) - ys.begin();
      rs.emplace_back(R{rect.l, d, u, false, rect.w});
      rs.emplace_back(R{rect.r, d, u, true, rect.w});
    }
    sort(rs.begin(), rs.end(),
         [](const R &a, const R &b) { return a.x < b.x; });

    vector< int > qs(q);
    iota(qs.begin(), qs.end(), 0);
    sort(qs.begin(), qs.end(),
         [&](int a, int b) { return queries[a].x < queries[b].x; });

    int j = 0;
    BIT bit(ys.size());
    for (auto &i : qs) {
      while (j < n + n and rs[j].x <= queries[i].x) {
        if(rs[j].type) {
          bit.apply(rs[j].d, -rs[j].w);
          bit.apply(rs[j].u, +rs[j].w);
        } else {
          bit.apply(rs[j].d, rs[j].w);
          bit.apply(rs[j].u, -rs[j].w);
        }
        ++j;
      }
      int y = lower_bound(ys.begin(), ys.end(), queries[i].y) - ys.begin();
      ans[i] = bit.prod(y + 1);
    }
    return ans;
  }
};

template <typename T, typename C>
struct DynamicRectangleAddPointGet {
 private:
  using StaticPointGetSolver = StaticRectangleAddPointGet<T, C>;

  static_assert(is_integral<T>::value,
                "template parameter T must be integral type");

  struct Rectangle {
    T l, d, r, u;
    C w;
  };

  struct Query {
    T x, y;
  };

  vector<variant<Rectangle, Query> > queries;

 public:
  DynamicRectangleAddPointGet() = default;

  explicit DynamicRectangleAddPointGet(int q) {
    queries.reserve(q);
  }

  void add_rectangle(T l, T d, T r, T u, C w) {
    queries.emplace_back(Rectangle{l, d, r, u, w});
  }

  void add_query(T x, T y) {
    queries.emplace_back(Query{x, y});
  }

  vector<C> calculate_queries() const {
    int q = (int)queries.size();
    vector<int> rev(q);
    int sz = 0;
    for (int i = 0; i < q; i++) {
      if (holds_alternative<Query>(queries[i])) {
        rev[i] = sz++;
      }
    }
    vector<C> ans(sz);
    queue<pair<int, int> > range;
    range.emplace(0, q);
    while (not range.empty()) {
      auto [l, r] = range.front();
      range.pop();
      int m = (l + r) >> 1;
      StaticPointGetSolver solver;
      for (int k = l; k < m; k++) {
        if (holds_alternative<Rectangle>(queries[k])) {
          auto &rect = get<Rectangle>(queries[k]);
          solver.add_rectangle(rect.l, rect.d, rect.r, rect.u, rect.w);
        }
      }
      for (int k = m; k < r; k++) {
        if (holds_alternative<Query>(queries[k])) {
          auto &query = get<Query>(queries[k]);
          solver.add_query(query.x, query.y);
        }
      }
      auto sub = solver.calculate_queries();
      for (int k = m, t = 0; k < r; k++) {
        if (holds_alternative<Query>(queries[k])) {
          ans[rev[k]] += sub[t++];
        }
      }
      if (l + 1 < m) range.emplace(l, m);
      if (m + 1 < r) range.emplace(m, r);
    }
    return ans;
  }
};

#line 1 "other/scanner.hpp"
/**
 * @brief Scanner(高速入力)
 */
struct Scanner {
 public:
  explicit Scanner(FILE *fp) : fp(fp) {}

  template <typename T, typename... E>
  void read(T &t, E &...e) {
    read_single(t);
    read(e...);
  }

 private:
  static constexpr size_t line_size = 1 << 16;
  static constexpr size_t int_digits = 20;
  char line[line_size + 1] = {};
  FILE *fp = nullptr;
  char *st = line;
  char *ed = line;

  void read() {}

  static inline bool is_space(char c) { return c <= ' '; }

  void reread() {
    ptrdiff_t len = ed - st;
    memmove(line, st, len);
    char *tmp = line + len;
    ed = tmp + fread(tmp, 1, line_size - len, fp);
    *ed = 0;
    st = line;
  }

  void skip_space() {
    while (true) {
      if (st == ed) reread();
      while (*st && is_space(*st)) ++st;
      if (st != ed) return;
    }
  }

  template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
  void read_single(T &s) {
    skip_space();
    if (st + int_digits >= ed) reread();
    bool neg = false;
    if (is_signed<T>::value && *st == '-') {
      neg = true;
      ++st;
    }
    typename make_unsigned<T>::type y = *st++ - '0';
    while (*st >= '0') {
      y = 10 * y + *st++ - '0';
    }
    s = (neg ? -y : y);
  }

  template <typename T, enable_if_t<is_same<T, string>::value, int> = 0>
  void read_single(T &s) {
    s = "";
    skip_space();
    while (true) {
      char *base = st;
      while (*st && !is_space(*st)) ++st;
      s += string(base, st);
      if (st != ed) return;
      reread();
    }
  }

  template <typename T>
  void read_single(vector<T> &s) {
    for (auto &d : s) read(d);
  }
};

#line 1 "other/printer.hpp"
/**
 * @brief Printer(高速出力)
 */
struct Printer {
 public:
  explicit Printer(FILE *fp) : fp(fp) {}

  ~Printer() { flush(); }

  template <bool f = false, typename T, typename... E>
  void write(const T &t, const E &...e) {
    if (f) write_single(' ');
    write_single(t);
    write<true>(e...);
  }

  template <typename... T>
  void writeln(const T &...t) {
    write(t...);
    write_single('\n');
  }

  void flush() {
    fwrite(line, 1, st - line, fp);
    st = line;
  }

 private:
  FILE *fp = nullptr;
  static constexpr size_t line_size = 1 << 16;
  static constexpr size_t int_digits = 20;
  char line[line_size + 1] = {};
  char *st = line;

  template <bool f = false>
  void write() {}

  void write_single(const char &t) {
    if (st + 1 >= line + line_size) flush();
    *st++ = t;
  }

  template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
  void write_single(T s) {
    if (st + int_digits >= line + line_size) flush();
    st += to_chars(st, st + int_digits, s).ptr - st;
  }

  void write_single(const string &s) {
    for (auto &c : s) write_single(c);
  }

  void write_single(const char *s) {
    while (*s != 0) write_single(*s++);
  }

  template <typename T>
  void write_single(const vector<T> &s) {
    for (size_t i = 0; i < s.size(); i++) {
      if (i) write_single(' ');
      write_single(s[i]);
    }
  }
};

int main() {
  Scanner in(stdin);
  Printer out(stdout);
  int N, Q;
  in.read(N, Q);
  DynamicRectangleAddPointGet< int, int64 > sum(N + Q);
  for(int i = 0; i < N; i++) {
    int l, d, r, u, w;
    in.read(l, d, r, u, w);
    sum.add_rectangle(l, d, r, u, w);
  }
  for(int i = 0; i < Q; i++) {
    int t;
    in.read(t);
    if(t == 0) {
      int l, d, r, u, w;
      in.read(l, d, r, u, w);
      sum.add_rectangle(l, d, r, u, w);
    } else {
      int x, y;
      in.read(x, y);
      sum.add_query(x, y);
    }
  }
  for(auto& ans : sum.calculate_queries()) {
    out.writeln(ans);
  }
}