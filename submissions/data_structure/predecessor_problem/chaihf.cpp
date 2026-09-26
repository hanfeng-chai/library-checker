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

namespace {

u64 root;
u64 lv18[39];
u64 lv12[2442];
u64 leaf[156250];

void set(u32 x) noexcept {
  leaf[x >> 6] |= 1ull << (x & 63);
  lv12[x >> 12] |= 1ull << ((x >> 6) & 63);
  lv18[x >> 18] |= 1ull << ((x >> 12) & 63);
  root |= 1ull << (x >> 18);
}

void unset(u32 x) noexcept {
  if (!(leaf[x >> 6] &= ~(1ull << (x & 63))))
    if (!(lv12[x >> 12] &= ~(1ull << ((x >> 6) & 63))))
      if (!(lv18[x >> 18] &= ~(1ull << ((x >> 12) & 63))))
        root &= ~(1ull << (x >> 18));
}

bool get(u32 x) noexcept { return leaf[x >> 6] & (1ull << (x & 63)); }

u32 nxt(u32 x) noexcept {
  if (u64 tmp = leaf[x >> 6] & (-1ull << (x & 63)); !tmp) {
    if (u64 tmp = lv12[x >> 12] & (-2ull << ((x >> 6) & 63)); !tmp) {
      if (u64 tmp = lv18[x >> 18] & (-2ull << ((x >> 12) & 63)); !tmp) {
        if (u64 tmp = root & (-2ull << (x >> 18)); !tmp) {
          return -1;
        } else {
          u32 ans = __builtin_ctzll(tmp);
          ans = ans << 6 | __builtin_ctzll(lv18[ans]);
          ans = ans << 6 | __builtin_ctzll(lv12[ans]);
          ans = ans << 6 | __builtin_ctzll(leaf[ans]);
          return ans;
        }
      } else {
        u32 ans = (x >> 18) << 6 | __builtin_ctzll(tmp);
        ans = ans << 6 | __builtin_ctzll(lv12[ans]);
        ans = ans << 6 | __builtin_ctzll(leaf[ans]);
        return ans;
      }
    } else {
      u32 ans = (x >> 12) << 6 | __builtin_ctzll(tmp);
      ans = ans << 6 | __builtin_ctzll(leaf[ans]);
      return ans;
    }
  } else {
    u32 ans = (x >> 6) << 6 | __builtin_ctzll(tmp);
    return ans;
  }
}

u32 pre(u32 x) noexcept {
  if (u64 tmp = leaf[x >> 6] & ~(-2ull << (x & 63)); !tmp) {
    if (u64 tmp = lv12[x >> 12] & ~(-1ull << ((x >> 6) & 63)); !tmp) {
      if (u64 tmp = lv18[x >> 18] & ~(-1ull << ((x >> 12) & 63)); !tmp) {
        if (u64 tmp = root & ~(-1ull << (x >> 18)); !tmp) {
          return -1;
        } else {
          u32 ans = 63 - __builtin_clzll(tmp);
          ans = ans << 6 | 63 - __builtin_clzll(lv18[ans]);
          ans = ans << 6 | 63 - __builtin_clzll(lv12[ans]);
          ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
          return ans;
        }
      } else {
        u32 ans = (x >> 18) << 6 | 63 - __builtin_clzll(tmp);
        ans = ans << 6 | 63 - __builtin_clzll(lv12[ans]);
        ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
        return ans;
      }
    } else {
      u32 ans = (x >> 12) << 6 | 63 - __builtin_clzll(tmp);
      ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
      return ans;
    }
  } else {
    u32 ans = (x >> 6) << 6 | 63 - __builtin_clzll(tmp);
    return ans;
  }
}

} // namespace

int main() {
  rd rd;
  wt wt;
  int n = rd.uh();
  int q = rd.uh();
#ifdef LOCAL
  root = 0;
  std::memset(lv18, 0, sizeof(lv18));
  std::memset(lv12, 0, sizeof(lv12));
  std::memset(leaf, 0, sizeof(leaf));
#endif
  int i = 0;
  for (; i + 64 < n; i += 64) {
    u32 a = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
        _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(rd.p + i)),
        _mm256_set1_epi8('1')));
    u32 b = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
        _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(rd.p + i + 32)),
        _mm256_set1_epi8('1')));
    leaf[i >> 6] = a | u64(b) << 32;
  }
  for (; i < n; ++i) leaf[i >> 6] |= u64(rd.p[i] - '0') << (i & 63);
  for (int i = 0; i <= (n - 1) / 64; ++i)
    lv12[i >> 6] |= u64(!!leaf[i]) << (i & 63);
  for (int i = 0; i <= (n - 1) / 64 / 64; ++i)
    lv18[i >> 6] |= u64(!!lv12[i]) << (i & 63);
  for (int i = 0; i <= (n - 1) / 64 / 64 / 64; ++i)
    root |= u64(!!lv18[i]) << (i & 63);
  rd.p += n + 1;
  while (q--) {
    let t = rd.u1();
    let k = rd.uh();
    if (t == 0) {
      set(k);
    }
    if (t == 1) {
      unset(k);
    }
    if (t == 2) {
      let x = get(k);
      wt.uw(x);
    }
    if (t == 3) {
      let x = nxt(k);
      if (~x) {
        wt.uw(x);
      } else {
        wt.puts(" -1", 3);
      }
    }
    if (t == 4) {
      let x = pre(k);
      if (~x) {
        wt.uw(x);
      } else {
        wt.puts(" -1", 3);
      }
    }
  }
  return 0;
}
