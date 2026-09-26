// https://judge.yosupo.jp/problem/tree_diameter

#pragma GCC optimize(3)
#include <bits/stdc++.h>

namespace debug {

}  // namespace debug

#ifdef LOCAL
#define CHECK(expr) assert(expr)
#else
#define CHECK(expr) void(0)
#endif

#ifdef __unix__
#ifndef DISABLE_MMAP
#define ENABLE_MMAP
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#endif

namespace fast_io {

template <uint32_t BufSize>
struct FastInput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;
  size_t map_size;

  explicit FastInput(FILE* _file = stdin) : file(_file) {
#ifdef ENABLE_MMAP
    struct stat st;
    int fd = fileno(file);
    fstat(fd, &st);
    map_size = st.st_size;
    cur = buf = static_cast<char*>(
        mmap(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, 0));
    end = cur + map_size;
#else
    cur = buf = new char[BufSize + 64];
    end = buf + fread(buf, 1, BufSize, file);
    memset(const_cast<char*>(end), 0, 64);
#endif
  }

#ifdef ENABLE_MMAP
  ~FastInput() { munmap(buf, map_size); }
#else
  ~FastInput() { delete[] buf; }
#endif

  void ensure() {
#ifndef ENABLE_MMAP
    int rem = end - cur;
    if (rem >= 40) [[likely]] return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, 64);
#endif
  }

  void skip_space() {
    ensure();
    while (*cur < 33) [[unlikely]] {
      ++cur;
      ensure();
    }
  }

  template <std::unsigned_integral T>
  T read_small() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint32_t v;
    memcpy(&v, cur, 4);
    v ^= 0x30303030;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ff;
      v = (v * 100 + (v >> 16)) & 0xffff;
      x = x * 10000 + v, cur += 4;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <std::signed_integral T>
  T read_small() {
    using U = std::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read_small<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <std::unsigned_integral T>
    requires(sizeof(T) < 8 && !std::same_as<T, bool>)
  T read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint64_t v;
    memcpy(&v, cur, 8);
    v ^= 0x3030303030303030ull;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffffull;
      v = (v * 10000 + (v >> 32)) & 0xffffffffull;
      x = x * 100000000 + v, cur += 8;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <std::unsigned_integral T>
    requires(sizeof(T) == 8)
  uint64_t read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    union {
      char ch[16];
      uint64_t v[2];
    };
    memcpy(ch, cur, 16);
    uint64_t a = v[0] ^ 0x3030303030303030ull;
    uint64_t b = v[1] ^ 0x3030303030303030ull;

    uint64_t x = 0;
    if (all_digits(a)) {
      a = (a * 10 + (a >> 8)) & 0xff00ff00ff00ffull;
      a = (a * 100 + (a >> 16)) & 0xffff0000ffffull;
      a = (a * 10000 + (a >> 32)) & 0xffffffffull;
      x = a, cur += 8;
      if (all_digits(b)) {
        b = (b * 10 + (b >> 8)) & 0xff00ff00ff00ffull;
        b = (b * 100 + (b >> 16)) & 0xffff0000ffffull;
        b = (b * 10000 + (b >> 32)) & 0xffffffffull;
        x = a * 100000000 + b, cur += 8;
      }
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <std::signed_integral T>
    requires(!std::same_as<T, char>)
  T read() {
    using U = std::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <typename T>
    requires(std::same_as<T, bool>)
  bool read() {
    ensure();
    CHECK(*cur == '0' || *cur == '1');
    bool x = *cur & 1;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(std::same_as<T, char>)
  char read() {
    ensure();
    char x = *cur;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(std::same_as<T, std::string>)
  std::string read() {
    ensure();
    CHECK(*cur > 32);

#ifdef ENABLE_MMAP
    char* first = cur;
    while (*cur > 32) ++cur;
    std::string s(first, cur);
    ++cur;
    return s;
#else
    std::string s;
    while (true) {
      char* last = cur;
      while (last < end && *last > 32) ++last;
      if (last < end) {
        s.append(cur, last);
        cur = last + 1;
        return s;
      } else {
        s.append(cur, last);
        cur = end;
        ensure();
      }
    }
#endif
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    x = read<T>();
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) {
      *s++ = *cur++;
      ensure();
    }
    *s = 0, ++cur;
    return *this;
  }

 private:
  constexpr bool all_digits(uint32_t v) { return !(v & 0xf0f0f0f0); }
  constexpr bool all_digits(uint64_t v) { return !(v & 0xf0f0f0f0f0f0f0f0ull); }
};

struct EndLine {
} endl;

template <uint32_t BufSize>
struct FastOutput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;

  explicit FastOutput(FILE* _file = stdout) : file(_file) {
    cur = buf = new char[BufSize];
    end = buf + BufSize;
  }

  template <uint32_t N = BufSize>
  void flush() {
    if (end - cur < N) [[unlikely]] {
      fwrite(buf, 1, cur - buf, file);
      cur = buf;
    }
  }

  ~FastOutput() {
    flush();
    delete[] buf;
  }

  template <std::unsigned_integral T>
    requires(sizeof(T) < 8)
  void write(T x) {
    if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(x);
    } else {
      print<0>(x);
    }
  }

  template <std::unsigned_integral T>
    requires(sizeof(T) == 8)
  void write(T x) {
    if (x > 9'999'999'999'999'999ull) {
      print<4>(x);
    } else if (x > 999'999'999'999ull) [[likely]] {
      print<3>(x);
    } else if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(static_cast<uint32_t>(x));
    } else {
      print<0>(static_cast<uint32_t>(x));
    }
  }

  template <std::unsigned_integral T>
  FastOutput& operator<<(T x) {
    flush<20>();
    write(x);
    return *this;
  }

  template <std::signed_integral T>
  FastOutput& operator<<(T x) {
    using U = std::make_unsigned_t<T>;

    flush<20>();
    *cur = '-';
    cur += (x < 0);
    write(x < 0 ? -static_cast<U>(x) : static_cast<U>(x));
    return *this;
  }

  FastOutput& operator<<(bool x) {
    flush<1>();
    *cur++ = x + '0';
    return *this;
  }

  FastOutput& operator<<(char x) {
    flush<1>();
    *cur++ = x;
    return *this;
  }

  FastOutput& operator<<(const char* s) {
    uint32_t len = strlen(s);
    if (len > BufSize) [[unlikely]] {
      flush();
      do {
        fwrite(s, 1, BufSize, file);
        s += BufSize;
        len -= BufSize;
      } while (len > BufSize);
    }
    if (end - cur < len) [[unlikely]] flush();
    memcpy(cur, s, len);
    cur += len;
    return *this;
  }

  FastOutput& operator<<(char* s) {
    return *this << const_cast<const char*>(s);
  }

  FastOutput& operator<<(const std::string& s) {
    return *this << s.c_str();
  }

  FastOutput& operator<<(const EndLine& end_line) {
    flush<1>();
    *cur++ = '\n';
    flush();
    return *this;
  }

 private:
  static constexpr auto LUT = [] {
    std::array<std::array<char, 4>, 10000> a, b;

    for (int i = 0; i < 10000; ++i) {
      b[i][0] = '0' + i / 1000;
      b[i][1] = '0' + i / 100 % 10;
      b[i][2] = '0' + i / 10 % 10;
      b[i][3] = '0' + i % 10;

      int j = 0;
      if (i >= 1000) a[i][j++] = b[i][0];
      if (i >= 100) a[i][j++] = b[i][1];
      if (i >= 10) a[i][j++] = b[i][2];
      a[i][j] = b[i][3];
    }

    return std::make_pair(a, b);
  }();

  template <int N, std::unsigned_integral T>
  void print(T x) {
    if constexpr (N == 0) {
      memcpy(cur, &LUT.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    } else {
      print<N - 1>(x / 10000);
      memcpy(cur, &LUT.second[x % 10000], 4);
      cur += 4;
    }
  }
};

template <uint32_t InputBufSize, uint32_t OutputBufSize>
struct FastIO {
  FastInput<InputBufSize>* in;
  FastOutput<OutputBufSize>* out;

  FastIO() : in(nullptr), out(nullptr) {}

  ~FastIO() {
    if (in != nullptr) delete in;
    if (out != nullptr) {
      out->flush();
      delete out;
    }
  }

  void init(FILE* input_file = stdin, FILE* output_file = stdout) {
    in = new FastInput<InputBufSize>(input_file);
    out = new FastOutput<OutputBufSize>(output_file);
  }

  void flush() { out->flush(); }

  template <typename T>
  FastIO& operator>>(T& x) {
    *in >> x;
    return *this;
  }

  template <typename T>
  FastIO& operator<<(const T& x) {
    *out << x;
    return *this;
  }

  FastIO& operator<<(const EndLine& x) {
    *out << x;
    return *this;
  }
};

}  // namespace fast_io

using fast_io::FastIO;

// Based on https://codeforces.com/blog/entry/135239
template <bool weighted = false, typename T = int>
struct XorLinkedTree {
  uint32_t n = 0;
  std::vector<uint32_t> deg;
  std::vector<uint32_t> link;
  std::vector<T> val;

  XorLinkedTree() = default;
  explicit XorLinkedTree(uint32_t _n) { init(_n); };

  void init(uint32_t _n) {
    n = _n;
    deg.assign(n, 0);
    link.assign(n, 0);
    if constexpr (weighted) val.assign(n, 0);
  }

  void add_edge(uint32_t u, uint32_t v) requires(!weighted) {
    CHECK(u < n && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v;
    ++deg[v], link[v] ^= u;
  }

  void add_edge(uint32_t u, uint32_t v, T w) requires(weighted) {
    CHECK(u < n && v < n);
    CHECK(u != v);
    ++deg[u], link[u] ^= v, val[u] ^= w;
    ++deg[v], link[v] ^= u, val[v] ^= w;
  }

  template <typename F>
  void build(uint32_t root, F&& func) {
    CHECK(root < n);
    ++deg[root];

    for (uint32_t i = 0; i < n; ++i) {
      uint32_t u = i;
      while (deg[u] == 1) {
        uint32_t v = link[u];
        if constexpr (weighted) {
          T w = val[u];
          func(u, v, w);
          val[v] ^= w;
        } else {
          func(u, v);
        }
        deg[u] = 0;
        --deg[v], link[v] ^= u;
        u = v;
      }
    }
  }
};

using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

void solve_main() {
  int n;
  io >> n;
  XorLinkedTree<true, uint32_t> t(n);

  for (int i = 0; i < n - 1; ++i) {
    uint32_t u = io.in->read_small<uint32_t>();
    uint32_t v = io.in->read_small<uint32_t>();
    uint32_t w = io.in->read<uint32_t>();
    t.add_edge(u, v, w);
  }

  vector<uint64_t> mx(n, 0);
  vector<uint32_t> fa(n, 0);
  auto pos = views::iota(0, n) | ranges::to<vector<uint32_t>>();

  uint64_t ans = 0;
  uint32_t l = 0, r = 0, lca = 0;

  t.build(0, [&](uint32_t u, uint32_t v, uint32_t w) -> void {
    if (u == 0) return;
    fa[u] = v;

    uint64_t new_mx = mx[u] + w;
    uint64_t new_ans = new_mx + mx[v];

    if (new_ans > ans) {
      ans = new_ans;
      l = pos[u], r = pos[v], lca = v;
    }

    if (new_mx > mx[v]) {
      mx[v] = new_mx;
      pos[v] = pos[u];
    }
  });

  uint32_t cnt = 0;

  while (l != lca) {
    pos[cnt] = l, ++cnt;
    l = fa[l];
  }

  pos[cnt] = lca;
  uint32_t mid = ++cnt;

  while (r != lca) {
    pos[cnt] = r, ++cnt;
    r = fa[r];
  }

  io << ans << ' ' << cnt << '\n';

  for (int i = 0; i < mid; ++i) io << pos[i] << ' ';
  for (int i = cnt - 1; i >= mid; --i) io << pos[i] << ' ';
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  // cin.tie(nullptr)->sync_with_stdio(false);
  io.init();

  int T;
  // cin >> T;
  // io >> T;
  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}