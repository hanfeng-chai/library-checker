#ifndef __clang__
#define NDEBUG
#pragma GCC optimize("Ofast")
#endif

#ifdef LOCAL
import std;
#define prelude import std
#else
#include <bits/stdc++.h>
#define prelude
#endif

#include <cassert>
#ifdef NDEBUG
#define toy_assert(cond) [[assume(cond)]]
#else
#define toy_assert(cond) assert(cond)
#endif

#define def auto
#define let const auto
#define fun constexpr auto

namespace toy {

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using i128 = __int128;
using isize = std::ptrdiff_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using u128 = unsigned __int128;
using usize = std::size_t;

using f32 = float;
using f64 = double;
using f80 = long double;

template <typename T> fun ensure(T &&t) noexcept -> decltype(auto) {
  toy_assert(t);
  return std::forward<T>(t);
}

} // namespace toy
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace toy;
prelude;

struct rd {
  static fun all_digit(u32 x) noexcept -> bool {
    x ^= 0x30303030;
    x &= 0xf0f0f0f0;
    return !x;
  }

  static fun all_digit(u64 x) noexcept -> bool {
    x ^= 0x3030303030303030;
    x &= 0xf0f0f0f0f0f0f0f0;
    return !x;
  }

  char *p, *l, *r;

  rd() noexcept {
    struct stat st;
    fstat(0, &st);
    p = l = r = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    r += st.st_size;
  }
  ~rd() noexcept { munmap(l, r - l); }

  def skip(int n) noexcept -> void { p += n; }

  def ch() noexcept -> u8 {
    u8 x = *p++;
    return ++p, x;
  }

  def u1() noexcept -> u32 {
    u32 x = *p++ - '0';
    return ++p, x;
  }

  def ub() noexcept -> u32 {
    u32 x = *p++ - '0';
    for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
    return ++p, x;
  }

  def uh() noexcept -> u32 {
    u32 x{};
    u32 a;
    std::memcpy(&a, p, sizeof(a));
    if (all_digit(a)) {
      a ^= 0x30303030;
      a = (a * 10 + (a >> 8)) & 0x00ff00ff;
      a = (a * 100 + (a >> 16)) & 0x0000ffff;
      x = a, p += sizeof(a);
    }
    for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
    return ++p, x;
  }

  def uw() noexcept -> u32 {
    u32 x{};
    u64 a;
    std::memcpy(&a, p, sizeof(a));
    if (all_digit(a)) {
      a ^= 0x3030303030303030;
      a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
      a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
      a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
      x = u32(a), p += sizeof(a);
    }
    for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
    return ++p, x;
  }

  def ud() noexcept -> u64 {
    u64 x{};
    union {
      u128 ch;
      u64 d[2];
    };
    std::memcpy(&ch, p, sizeof(ch));
    u64 a = d[0], b = d[1];
    if (all_digit(a) && all_digit(b)) {
      a ^= 0x3030303030303030;
      b ^= 0x3030303030303030;
      a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
      b = (b * 10 + (b >> 8)) & 0x00ff00ff00ff00ff;
      a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
      b = (b * 100 + (b >> 16)) & 0x0000ffff0000ffff;
      a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
      b = (b * 10000 + (b >> 32)) & 0x00000000ffffffff;
      x = a * 100000000 + b, p += sizeof(ch);
    }
    for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
    return ++p, x;
  }

  def i1() noexcept -> i32 { return *p == '-' ? ++p, -u1() : u1(); }
  def ib() noexcept -> i32 { return *p == '-' ? ++p, -ub() : ub(); }
  def ih() noexcept -> i32 { return *p == '-' ? ++p, -uh() : uh(); }
  def iw() noexcept -> i32 { return *p == '-' ? ++p, -uw() : uw(); }
  def id() noexcept -> i64 { return *p == '-' ? ++p, -ud() : ud(); }
};

struct wt {
  static fun low = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
      res[i][0] = '0' + i / 1000 % 10;
      res[i][1] = '0' + i / 100 % 10;
      res[i][2] = '0' + i / 10 % 10;
      res[i][3] = '0' + i % 10;
    }
    return res;
  }();
  static fun pos = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
      res[i][0] = '0' + i / 1000 % 10;
      res[i][1] = '0' + i / 100 % 10;
      res[i][2] = '0' + i / 10 % 10;
      res[i][3] = '0' + i % 10;
      if (i < 1000) res[i][0] = ' ';
      if (i < 100) res[i][1] = ' ';
      if (i < 10) res[i][2] = ' ';
    }
    return res;
  }();

  inline static char buf[1 << 20];
  static fun sentinel = buf + sizeof(buf) - 50;
  char *p = buf;
  ~wt() noexcept { flush(); }
  void flush() noexcept { write(1, buf, p - buf), p = buf; }
  void print_low(u64 x) noexcept { std::memcpy(p, &low[x], 4), p += 4; }
  void print_pos(u64 x) noexcept { std::memcpy(p, &pos[x], 4), p += 4; }

  void puts(const char *src, int n) noexcept {
    if (sentinel < p + n) flush();
    std::memcpy(p, src, n), p += n;
  }

  void u1(u8 x) noexcept {
    toy_assert(x < 10);
    *p++ = ' ';
    *p++ = '0' + x;
  }

  void uw(u32 x) noexcept {
    if (sentinel < p) flush();
    *p++ = ' ';
    if (x > 9999'9999) {
      print_pos(x / 10000 / 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    } else if (x > 9999) {
      print_pos(x / 10000);
      print_low(x % 10000);
    } else {
      print_pos(x);
    }
  }

  void ud(u64 x) noexcept {
    if (sentinel < p) flush();
    *p++ = ' ';
    if (x > 9999'9999'9999'9999) {
      print_pos(x / 10000 / 10000 / 10000 / 10000);
      print_low(x / 10000 / 10000 / 10000 % 10000);
      print_low(x / 10000 / 10000 % 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    } else if (x > 9999'9999'9999) {
      print_pos(x / 10000 / 10000 / 10000);
      print_low(x / 10000 / 10000 % 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    } else if (x > 9999'9999) {
      print_pos(x / 10000 / 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    } else if (x > 9999) {
      print_pos(x / 10000);
      print_low(x % 10000);
    } else {
      print_pos(x);
    }
  }
};

#ifdef LOCAL
#include <cstdio>
int main(int argc, char *argv[]) {
  extern int Main();
  if (argc < 2) return Main();
  std::string last = argv[argc - 1];
  if (last.ends_with("out")) {
    int half = (argc - 1) / 2;
    for (int i = 0; i < half; ++i) {
      freopen(argv[1 + i], "r", stdin);
      freopen(argv[1 + half + i], "w", stdout);
      ensure(!Main());
    }
  } else {
    for (int i = 1; i < argc; ++i) {
      freopen(argv[i], "r", stdin);
      ensure(!Main());
    }
  }
  return 0;
}
#define main Main
#endif
prelude;

namespace toy {

using std::bit_cast;
using std::byteswap;
template <std::integral T> fun bit_ceil(T t) noexcept {
  return std::bit_ceil(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun bit_floor(T t) noexcept {
  return std::bit_floor(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun bit_width(T t) noexcept {
  return std::bit_width(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun countl_one(T t) noexcept {
  return std::countl_one(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun countl_zero(T t) noexcept {
  return std::countl_zero(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun countr_one(T t) noexcept {
  return std::countr_one(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun countr_zero(T t) noexcept {
  return std::countr_zero(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun has_single_bit(T t) noexcept {
  return std::has_single_bit(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun popcount(T t) noexcept {
  return std::popcount(std::make_unsigned_t<T>(t));
}
template <std::integral T> [[nodiscard]] fun log(T t) noexcept {
  return std::bit_width(std::make_unsigned_t<T>(ensure(t))) - 1;
}
template <std::integral T> [[nodiscard]] fun rotl(T t, int cnt) noexcept {
  return std::rotl(std::make_unsigned_t<T>(t), cnt);
}
template <std::integral T> [[nodiscard]] fun rotr(T t, int cnt) noexcept {
  return std::rotr(std::make_unsigned_t<T>(t), cnt);
}

} // namespace toy
prelude;

namespace {

constexpr int N = 5e5;

int head[N];
struct {
  int to;
  int next;
} edge[N * 2];
int p[N];
int l[N];
int r[N];
int a[N];
u64 b[N + 1];
u64 c[N + 2];

} // namespace

int main() {
  rd rd;
  wt wt;
  int n = rd.uh();
  int q = rd.uh();
#ifdef LOCAL
  std::memset(head, 0, 4 * n);
  std::memset(c, 0, 8 * n + 16);
#endif
  for (int i = 0; i < n; ++i) a[i] = rd.uw();
  for (int i = 1; i < n; ++i) {
    int u = rd.uh();
    int v = rd.uh();
    edge[i * 2 | 0] = {v, head[u]}, head[u] = i * 2 | 0;
    edge[i * 2 | 1] = {u, head[v]}, head[v] = i * 2 | 1;
  }
  p[0] = -1, l[0] = 1, b[1] = c[1] = a[0];
  for (int u = 0, i = 1; u >= 0;) {
    if (int e = head[u]; e) {
      def[v, x] = edge[e];
      head[u] = x;
      if (v == p[u]) continue;
      p[v] = u;
      l[v] = ++i;
      c[i] += b[i] = a[v];
      u = v;
    } else {
      c[r[u] = i + 1] -= a[u];
      u = p[u];
    }
  }
  u64 sum = 0;
  for (int i = 1; i <= n; ++i) c[i] = sum += c[i];
  for (int i = n; i >= 1; --i) c[i] -= c[i - (i & -i)];
  int val[n + 1];
  for (int i = 1; i < n; ++i) val[l[i]] = l[p[i]];
  ++n;
  int m = n / 64 + 1;
  int len = log(m) + 1;
  int table[len][m];
  int suf[n];
  int pre[n];
  for (int min, i = 0; i < n; ++i) {
    int id = i & 63;
    int value = pre[i] = val[i];
    suf[i] = min = id ? std::min(min, value) : value;
    if (id == 63) table[0][i / 64] = suf[i];
  }
  for (int min, i = n - 2; i >= 0; --i) {
    int id = ~i & 63;
    int value = pre[i];
    pre[i] = min = id ? std::min(min, value) : value;
  }
  for (int i = 1; i < len; ++i) {
    for (int j = 0, k = 1 << (i - 1); k < m; ++j, ++k) {
      table[i][j] = std::min(table[i - 1][j], table[i - 1][k]);
    }
  }
  while (q--) {
    let t = rd.u1();
    if (t == 0) {
      int k = rd.uh();
      int x = rd.uw();
      int u = l[k];
      int v = r[k];
      b[u] += x;
      for (; u <= n; u += u & -u) c[u] += x;
      for (; v <= n; v += v & -v) c[v] -= x;
    }
    if (t == 1) {
      int u = l[rd.uh()];
      int v = l[rd.uh()];
      if (u == v) {
        wt.ud(b[u]);
        continue;
      }
      int l = std::min(u, v) + 1;
      int r = std::max(u, v);
      int L = l / 64;
      int R = r / 64;
      int w;
      if (L < R - 1) {
        int p = pre[l];
        int s = suf[r];
        w = std::min(p, s);
        int k = log(R - L - 1);
        int a = table[k][L + 1];
        int b = table[k][R - (1 << k)];
        int tmp = std::min(a, b);
        w = std::min(w, tmp);
      } else if (L == R - 1) {
        int p = pre[l];
        int s = suf[r];
        w = std::min(p, s);
      } else {
        w = val[l];
        for (int i = l + 1; i <= r; ++i) w = std::min(w, val[i]);
      }
      u64 sum = b[w];
      --l;
      for (; l; l -= l & -l) sum += c[l];
      for (; r; r -= r & -r) sum += c[r];
      for (; w; w -= w & -w) sum -= c[w] * 2;
      wt.ud(sum);
    }
  }
  return 0;
}
