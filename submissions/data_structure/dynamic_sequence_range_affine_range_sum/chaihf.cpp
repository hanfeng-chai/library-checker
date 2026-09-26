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

constexpr int N = 1000003;
constexpr u32 P = 998244353;

struct Node {
  u32 val;
  u32 sum;
  u32 a;
  u32 b;
  int rev;
  int size;
  int l, r;
} node[N];

def maintain(int x) noexcept -> void {
  node[x].size = node[node[x].l].size + 1 + node[node[x].r].size;
  node[x].sum = (node[node[x].l].sum + node[x].val + node[node[x].r].sum) % P;
}

def build(int l, int r) noexcept -> int {
  if (l > r) return 0;
  int m = (l + r) / 2;
  node[m].l = build(l, m - 1);
  node[m].r = build(m + 1, r);
  maintain(m);
  return m;
}

def reverse(int x) noexcept -> void { node[x].rev ^= 1; }

def affine(int x, u64 a, u64 b) noexcept -> void {
  if (x == 0) return;
  node[x].a = (a * node[x].a) % P;
  node[x].b = (a * node[x].b + b) % P;
  node[x].val = (a * node[x].val + b) % P;
  node[x].sum = (a * node[x].sum + b * node[x].size) % P;
}

def pushdown(int x) noexcept -> void {
  if (node[x].rev) {
    std::swap(node[x].l, node[x].r);
    node[node[x].l].rev ^= 1;
    node[node[x].r].rev ^= 1;
    node[x].rev = 0;
  }
  if ((node[x].a != 1) | (node[x].b != 0)) {
    affine(node[x].l, node[x].a, node[x].b);
    affine(node[x].r, node[x].a, node[x].b);
    node[x].a = 1;
    node[x].b = 0;
  }
}

def zig(int &x) noexcept -> void {
  int l = node[x].l;
  node[x].l = node[l].r;
  maintain(x);
  node[l].r = x;
  x = l;
}

def zag(int &x) noexcept -> void {
  int r = node[x].r;
  node[x].r = node[r].l;
  maintain(x);
  node[r].l = x;
  x = r;
}

def splay(int &x, int k) noexcept -> void {
  pushdown(x);
  if (int &l = node[x].l, &r = node[x].r, size = node[l].size; k == size) {
    return;
  } else if (k < size) {
    pushdown(l);
    if (int &ll = node[l].l, &lr = node[l].r, size = node[ll].size; k == size) {
      zig(x);
    } else if (k < size) {
      splay(ll, k);
      zig(x);
      zig(x);
    } else {
      splay(lr, k - size - 1);
      zag(l);
      zig(x);
    }
  } else {
    pushdown(r);
    k -= size + 1;
    if (int &rl = node[r].l, &rr = node[r].r, size = node[rl].size; k == size) {
      zag(x);
    } else if (k < size) {
      splay(rl, k);
      zig(r);
      zag(x);
    } else {
      splay(rr, k - size - 1);
      zag(x);
      zag(x);
    }
  }
}

} // namespace

int main() {
  rd rd;
  wt wt;
  int n = rd.uh();
  int q = rd.uh();
  for (int i = 1; i <= n + q + 1; ++i) node[i] = Node{.a = 1, .size = 1};
  for (int i = 2; i <= n + 1; ++i) node[i].val = rd.uw();
  int x = build(1, n += 2);
  while (q--) {
    let t = rd.u1();
    if (t == 0) {
      ++n;
      int k = rd.uh();
      node[n].val = node[n].sum = rd.uw();
      splay(x, k);
      splay(node[x].r, 0);
      node[node[x].r].l = n;
    }
    if (t == 1) {
      int k = rd.uh();
      splay(x, k);
      splay(node[x].r, 1);
      node[node[x].r].l = 0;
    }
    if (t == 2) {
      int l = rd.uh();
      int r = rd.uh();
      splay(x, l);
      splay(node[x].r, r - l);
      reverse(node[node[x].r].l);
    }
    if (t == 3) {
      int l = rd.uh();
      int r = rd.uh();
      splay(x, l);
      splay(node[x].r, r - l);
      int a = rd.uw();
      int b = rd.uw();
      affine(node[node[x].r].l, a, b);
    }
    if (t == 4) {
      int l = rd.uh();
      int r = rd.uh();
      splay(x, l);
      splay(node[x].r, r - l);
      wt.ud(node[node[node[x].r].l].sum);
    }
  }
  return 0;
}
