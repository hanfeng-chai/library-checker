// https://judge.yosupo.jp/problem/range_affine_range_sum

#include <bits/stdc++.h>
#include <cassert>

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>

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
  void write_w4(T x) {
    print<0>(x);
  }

  template <std::unsigned_integral T>
  void write_w8(T x) {
    if (x > 9999) {
      print<1>(x);
    } else {
      print<0>(x);
    }
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
    } else if (x > 999'999'999'999ull) {
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
    flush<20>();
    *cur++ = x + '0';
    return *this;
  }

  FastOutput& operator<<(char x) {
    flush<20>();
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
    flush<20>();
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

using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 19> io;

constexpr int P = 998244353;

inline constexpr uint32_t reduce(uint32_t x) {
  return std::min(x, x - P);
}

struct RangeAffineRangeSum {
  struct Info {
    uint32_t sum, len;
  };

  struct Tag {
    uint32_t b, c;
  };

  static constexpr Info id() noexcept { return {0, 0}; };
  static constexpr Tag tag_id() noexcept { return {1, 0}; };

  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {reduce(lhs.sum + rhs.sum), lhs.len + rhs.len};
  }

  static constexpr void compose(Tag& lhs, const Tag& rhs) {
    lhs.b = static_cast<uint64_t>(lhs.b) * rhs.b % P;
    lhs.c = (static_cast<uint64_t>(lhs.c) * rhs.b + rhs.c) % P;
  }

  static constexpr void apply(Info& info, const Tag& tag) {
    info.sum = (static_cast<uint64_t>(tag.b) * info.sum +
                static_cast<uint64_t>(tag.c) * info.len) % P;
  }
};

struct AffineSumTree {
  using Tag = RangeAffineRangeSum::Tag;
  static constexpr uint32_t empty_b = 0x80000001u;

  uint32_t n = 1, m;
  int h;
  vector<uint32_t> sum;
  vector<Tag> tag;

  explicit AffineSumTree(uint32_t size) : m(size) {
    while (n < m) n <<= 1;
    h = bit_width(n) - 1;
    sum.assign(n << 1, 0);
    tag.assign(n << 1, {empty_b, 0});
  }

  static void compose(Tag& lhs, const Tag& rhs) {
    lhs.b = static_cast<uint64_t>(lhs.b) * rhs.b % P;
    lhs.c = (static_cast<uint64_t>(lhs.c) * rhs.b + rhs.c) % P;
  }

  void apply_node(uint32_t x, const Tag& v, uint32_t len) {
    sum[x] = (static_cast<uint64_t>(v.b) * sum[x] +
              static_cast<uint64_t>(v.c) * len) % P;
    tag[x].b = static_cast<uint64_t>(tag[x].b & 0x7fffffffu) * v.b % P;
    tag[x].c = (static_cast<uint64_t>(tag[x].c) * v.b + v.c) % P;
  }

  void push_down(uint32_t x, uint32_t child_len) {
    if (!(tag[x].b >> 31)) {
      apply_node(x << 1, tag[x], child_len);
      apply_node(x << 1 | 1, tag[x], child_len);
      tag[x] = {empty_b, 0};
    }
  }

  void push_up(uint32_t x) {
    sum[x] = reduce(sum[x << 1] + sum[x << 1 | 1]);
  }

  void apply(uint32_t l, uint32_t r, const Tag& v) {
    if (l == r) return;
    l += n, r += n;
    int w = bit_width(l ^ r) - 1;
    int cl = countr_zero(l);
    int cr = countr_zero(r);

    for (int i = h; i > cl; --i) push_down(l >> i, 1u << (i - 1));
    for (int i = max(cl, w); i > cr; --i) {
      push_down(r >> i, 1u << (i - 1));
    }

    uint32_t L = l, R = r;
    int k = bit_width(--L ^ R) - 1;

    uint32_t x = L;
    L = ~L & ((1u << k) - 1);
    while (L) {
      int i = countr_zero(L);
      L ^= 1u << i;
      apply_node(x >> i ^ 1, v, 1u << i);
    }

    x = R;
    R &= (1u << k) - 1;
    while (R) {
      int i = bit_width(R) - 1;
      R ^= 1u << i;
      apply_node(x >> i ^ 1, v, 1u << i);
    }

    for (int i = cl + 1; i <= w; ++i) push_up(l >> i);
    for (int i = cr + 1; i <= w; ++i) push_up(r >> i);
    for (l >>= w + 1; l; l >>= 1) push_up(l);
  }

  uint32_t prod(uint32_t l, uint32_t r) const {
    if (l == r) return 0;
    uint32_t left = 0, right = 0, left_len = 0, right_len = 0, len = 1;
    l += n - 1, r += n;
    while ((l ^ r) != 1) {
      if (~l & 1) {
        left = reduce(left + sum[l ^ 1]);
        left_len += len;
      }
      if (r & 1) {
        right = reduce(sum[r ^ 1] + right);
        right_len += len;
      }
      l >>= 1, r >>= 1;
      len <<= 1;
      left = (static_cast<uint64_t>(tag[l].b & 0x7fffffffu) * left +
              static_cast<uint64_t>(tag[l].c) * left_len) % P;
      right = (static_cast<uint64_t>(tag[r].b & 0x7fffffffu) * right +
               static_cast<uint64_t>(tag[r].c) * right_len) % P;
    }

    uint32_t total = reduce(left + right);
    uint32_t total_len = left_len + right_len;
    while (l >>= 1) {
      total = (static_cast<uint64_t>(tag[l].b & 0x7fffffffu) * total +
               static_cast<uint64_t>(tag[l].c) * total_len) % P;
    }
    return total;
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  AffineSumTree t(n);
  for (uint32_t i = 0; i < t.m; ++i) t.sum[t.n + i] = io.in->read<uint32_t>();
  for (uint32_t i = t.n; --i; ) t.push_up(i);

  while (q--) {
    bool op = io.in->read<bool>();
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    if (op == 0) {
      uint32_t b, c;
      io >> b >> c;
      t.apply(l, r, {b, c});
    } else {
      io << t.prod(l, r) << '\n';
    }
  }
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