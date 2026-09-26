#include<bits/stdc++.h>

using namespace std;

using int64 = long long;
//const int mod = 1e9 + 7;
const int mod = 998244353;

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


template< typename T1, typename T2 >
ostream &operator<<(ostream &os, const pair< T1, T2 > &p) {
  os << p.first << " " << p.second;
  return os;
}

template< typename T1, typename T2 >
istream &operator>>(istream &is, pair< T1, T2 > &p) {
  is >> p.first >> p.second;
  return is;
}

template< typename T >
ostream &operator<<(ostream &os, const vector< T > &v) {
  for(int i = 0; i < (int) v.size(); i++) {
    os << v[i] << (i + 1 != v.size() ? " " : "");
  }
  return os;
}

template< typename T >
istream &operator>>(istream &is, vector< T > &v) {
  for(T &in: v) is >> in;
  return is;
}

template< typename T1, typename T2 >
inline bool chmax(T1 &a, T2 b) { return a < b && (a = b, true); }

template< typename T1, typename T2 >
inline bool chmin(T1 &a, T2 b) { return a > b && (a = b, true); }

template< typename T = int64 >
vector< T > make_v(size_t a) {
  return vector< T >(a);
}

template< typename T, typename... Ts >
auto make_v(size_t a, Ts... ts) {
  return vector< decltype(make_v< T >(ts...)) >(a, make_v< T >(ts...));
}

template< typename T, typename V >
typename enable_if< is_class< T >::value == 0 >::type fill_v(T &t, const V &v) {
  t = v;
}

template< typename T, typename V >
typename enable_if< is_class< T >::value != 0 >::type fill_v(T &t, const V &v) {
  for(auto &e: t) fill_v(e, v);
}

template< typename F >
struct FixPoint : F {
  FixPoint(F &&f) : F(forward< F >(f)) {}

  template< typename... Args >
  decltype(auto) operator()(Args &&... args) const {
    return F::operator()(*this, forward< Args >(args)...);
  }
};

template< typename F >
inline decltype(auto) MFP(F &&f) {
  return FixPoint< F >{forward< F >(f)};
}


#line 1 "structure/others/binary-indexed-tree.cpp"

/**
 * @brief Binary-Indexed-Tree(BIT)
 * @docs docs/binary-indexed-tree.md
 */
template< typename T >
struct BinaryIndexedTree {
private:
  int n;
  vector< T > data;

public:
  BinaryIndexedTree() = default;

  explicit BinaryIndexedTree(int n) : n(n) {
    data.assign(n + 1, 0);
  }

  explicit BinaryIndexedTree(const vector< T > &v) :
      BinaryIndexedTree((int) v.size()) {
    build(v);
  }

  void build(const vector< T > &v) {
    assert(n == (int) v.size());
    for(int i = 1; i <= n; i++) data[i] = v[i - 1];
    for(int i = 1; i <= n; i++) {
      int j = i + (i & -i);
      if(j <= n) data[j] += data[i];
    }
  }

  void apply(int k, const T &x) {
    for(++k; k <= n; k += k & -k) data[k] += x;
  }

  T prod(int r) const {
    T ret = T();
    for(; r > 0; r -= r & -r) ret += data[r];
    return ret;
  }

  T prod(int l, int r) const {
    return prod(r) - prod(l);
  }

  int lower_bound(T x) const {
    int i = 0;
    for(int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
      if(i + k <= n && data[i + k] < x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }

  int upper_bound(T x) const {
    int i = 0;
    for(int k = 1 << (__lg(n) + 1); k > 0; k >>= 1) {
      if(i + k <= n && data[i + k] <= x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }
};

#line 2 "other/static-point-add-rectangle-sum.cpp"

/**
 * @brief Static Point Add Rectangle Sum
 */
template< typename T, typename C >
struct StaticPointAddRectangleSum {
  using BIT = BinaryIndexedTree< C >;

  static_assert(is_integral< T >::value, "template parameter T must be integral type");

  struct Point {
    T x, y;
    C w;
  };

  struct Query {
    T l, d, r, u;
  };

  vector< Point > points;
  vector< Query > queries;

  StaticPointAddRectangleSum() = default;

  StaticPointAddRectangleSum(int n, int q) {
    points.reserve(n);
    queries.reserve(q);
  }

  void add_point(T x, T y, C w) {
    points.emplace_back(Point{x, y, w});
  }

  // tatal weight of [l, r) * [d, u) points
  void add_query(T l, T d, T r, T u) {
    queries.emplace_back(Query{l, d, r, u});
  }

  vector< C > calculate_queries() {
    int n = (int) points.size();
    int q = (int) queries.size();

    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
      return a.y < b.y;
    });
    vector< T > ys;
    ys.reserve(n);
    for(Point &p: points) {
      if(ys.empty() or ys.back() != p.y) ys.emplace_back(p.y);
      p.y = (int) ys.size() - 1;
    }
    ys.shrink_to_fit();

    struct Q {
      T x, d, u;
      bool type;
      int idx;
    };
    vector< Q > qs;
    qs.reserve(q + q);
    for(int i = 0; i < q; i++) {
      auto &query = queries[i];
      int d = lower_bound(ys.begin(), ys.end(), query.d) - ys.begin();
      int u = lower_bound(ys.begin(), ys.end(), query.u) - ys.begin();
      qs.emplace_back(Q{query.l, d, u, false, i});
      qs.emplace_back(Q{query.r, d, u, true, i});
    }
    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
      return a.x < b.x;
    });
    sort(qs.begin(), qs.end(), [](const Q &a, const Q &b) {
      return a.x < b.x;
    });
    vector< C > ans(q);
    int j = 0;
    BIT bit(ys.size());
    for(auto &query: qs) {
      while(j < n and points[j].x < query.x) {
        bit.apply(points[j].y, points[j].w);
        ++j;
      }
      if(query.type) ans[query.idx] += bit.prod(query.d, query.u);
      else ans[query.idx] -= bit.prod(query.d, query.u);
    }
    return ans;
  }
};


/**
 * @brief Printer(高速出力)
 */
struct Printer {
public:
  explicit Printer(FILE *fp) : fp(fp) {}

  ~Printer() { flush(); }

  template< bool f = false, typename T, typename... E >
  void write(const T &t, const E &... e) {
    if(f) write_single(' ');
    write_single(t);
    write< true >(e...);
  }

  template< typename... T >
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

  template< bool f = false >
  void write() {}

  void write_single(const char &t) {
    if(st + 1 >= line + line_size) flush();
    *st++ = t;
  }

  template< typename T, enable_if_t< is_integral< T >::value, int > = 0 >
  void write_single(T s) {
    if(st + int_digits >= line + line_size) flush();
    st += to_chars(st, st + int_digits, s).ptr - st;
  }

  void write_single(const string &s) {
    for(auto &c: s) write_single(c);
  }

  void write_single(const char *s) {
    while(*s != 0) write_single(*s++);
  }

  template< typename T >
  void write_single(const vector< T > &s) {
    for(size_t i = 0; i < s.size(); i++) {
      if(i) write_single(' ');
      write_single(s[i]);
    }
  }
};

/**
 * @brief Scanner(高速入力)
 */
struct Scanner {
public:

  explicit Scanner(FILE *fp) : fp(fp) {}

  template< typename T, typename... E >
  void read(T &t, E &... e) {
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

  static inline bool is_space(char c) {
    return c <= ' ';
  }

  void reread() {
    ptrdiff_t len = ed - st;
    memmove(line, st, len);
    char *tmp = line + len;
    ed = tmp + fread(tmp, 1, line_size - len, fp);
    *ed = 0;
    st = line;
  }

  void skip_space() {
    while(true) {
      if(st == ed) reread();
      while(*st && is_space(*st)) ++st;
      if(st != ed) return;
    }
  }

  template< typename T, enable_if_t< is_integral< T >::value, int > = 0 >
  void read_single(T &s) {
    skip_space();
    if(st + int_digits >= ed) reread();
    bool neg = false;
    if(is_signed< T >::value && *st == '-') {
      neg = true;
      ++st;
    }
    typename make_unsigned< T >::type y = *st++ - '0';
    while(*st >= '0') {
      y = 10 * y + *st++ - '0';
    }
    s = (neg ? -y : y);
  }

  template< typename T, enable_if_t< is_same< T, string >::value, int > = 0 >
  void read_single(T &s) {
    s = "";
    skip_space();
    while(true) {
      char *base = st;
      while(*st && !is_space(*st)) ++st;
      s += string(base, st);
      if(st != ed) return;
      reread();
    }
  }

  template< typename T >
  void read_single(vector< T > &s) {
    for(auto &d: s) read(d);
  }
};

int main() {
  int N, Q;
  Scanner in(stdin);
  Printer out(stdout);
  in.read(N, Q);
  StaticPointAddRectangleSum< int, int64 > spars(N, Q);
  for(int i = 0; i < N; i++) {
    int x, y, z;
    in.read(x, y, z);
    spars.add_point(x, y, z);
  }
  for(int i = 0; i < Q; i++) {
    int l, d, r, u;
    in.read(l, d, r, u);
    spars.add_query(l, d, r, u);
  }
  for(auto &&ans: spars.calculate_queries()) {
    out.writeln(ans);
  }
}