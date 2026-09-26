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

constexpr int N = 1e6;
constexpr int P = 998244353;

struct node {
  u32 a, b;
  node operator+(node t) {
    return {u32(u64(a) * t.a % P), u32((u64(a) * t.b + b) % P)};
  }
  u32 operator+(u32 x) { return (u64(a) * x + b) % P; }
} a[N];

} // namespace

int main() {
  rd rd;
  wt wt;
  int n = rd.uh();
  int q = rd.uh();
  for (int i = 0; i < n; ++i) a[n + n - 1 - i] = {rd.uw(), rd.uw()};
  for (int i = n - 1; i >= 1; --i) a[i] = a[i * 2] + a[i * 2 + 1];
  while (q--) {
    let t = rd.u1();
    if (t == 0) {
      int k = n + n - 1 - rd.uh();
      a[k] = {rd.uw(), rd.uw()};
      for (k /= 2; k > 0; k /= 2) a[k] = a[k * 2] + a[k * 2 + 1];
    }
    if (t == 1) {
      int r = n + n - rd.uh();
      int l = n + n - 1 - rd.uh();
      u32 x = rd.uw();
      int k = log(l ^ r);
      int R = r >> k;
      for (r = r >> __builtin_ctz(r) ^ 1; r > R; r = r >> __builtin_ctz(r) ^ 1)
        x = a[r] + x;
      for (int t = ~l & ~(-1 << k), i; t > 0; t -= 1 << i) {
        i = log(t);
        x = a[l >> i ^ 1] + x;
      }
      wt.uw(x);
    }
  }
  return 0;
}
