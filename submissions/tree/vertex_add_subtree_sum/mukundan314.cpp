// IO from https://judge.yosupo.jp/submission/278243 (chaihf)

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

const int N = 500'000 + 1;

typedef uint64_t v4ll __attribute__((vector_size(32)));

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/
template <int N, int b = 4> struct WideSegmentTree {
  constexpr static int B = 1 << b;

  struct Mask {
    alignas(64) uint64_t mask[B][B];

    constexpr Mask() : mask{} {
      for (int k = 0; k < B; k++)
        for (int i = 0; i < B; i++)
          mask[k][i] = (i > k ? -1 : 0);
    }
  };

  constexpr static Mask M{};

  constexpr static int height(int n) { return (n <= B ? 1 : height(n / B) + 1); }

  constexpr static int offset(int h) {
    int s = 0, n = N;
    while (h--) {
      n = (n + B - 1) / B;
      s += n * B;
    }
    return s;
  }

  constexpr static int H = height(N);

  alignas(64) uint64_t data[offset(H)];

  inline void build() {
    for (int i = 1; i < N; i++)
      data[offset(0) + i] += data[offset(0) + i - 1];
  }

  inline uint64_t sum(int k) const {
    uint64_t res = 0;
    for (int h = H - 1; h >= 0; h--)
      res += data[offset(h) + (k >> (h * b))];
    return res;
  }

  inline void add(int k, uint64_t _x) {
    v4ll x = _x + v4ll{};
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v4ll *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v4ll *)M.mask[p & (B - 1)];
      for (int i = 0; i < B / 4; i++)
        l[i] += x & m[i];
    }
  }
};

WideSegmentTree<N> s;
uint32_t a[N], P[N], dp[N], dfs[N], en[N];
 
int main() {
  rd rd;
  wt wt;

  uint32_t n = rd.uh();
  uint32_t q = rd.uh();

  for (int i = 0; i < n; ++i) a[i] = rd.uw();
  for (int u = 1; u < n; ++u) P[u] = rd.uh();
  
  for (int u = n - 1; u > 0; --u) {
    uint32_t v = P[u];
    dp[v] += dp[u] + 1;
  }

  for (int u = 1; u < n; ++u) {
    uint32_t v = P[u];
    dp[u] = std::exchange(dp[v], dp[v] - dp[u] - 1);
  }
 
  for (int i = 0; i < n; ++i) dfs[dp[i]] = i;
 
  auto &st = dp;
  for (int i = 0; i < n; ++i) en[i] = st[i];
  for (int i = n - 1; i > 0; --i) {
    uint32_t u = dfs[i], v = P[u];
    en[v] = std::max(en[v], en[u]);
  }

  for (int v = 0; v < n; ++v)
    s.data[s.offset(0) + st[v] + 1] = a[v];
  s.build();

  for (int i = 0; i < q; ++i) {
    if (rd.u1() == 0) {
      uint32_t v = rd.uh();
      s.add(st[v], rd.uw());
    } else {
      uint32_t v = rd.uh();
      wt.ud(s.sum(en[v] + 1) - s.sum(st[v]));
    }
  }
}
