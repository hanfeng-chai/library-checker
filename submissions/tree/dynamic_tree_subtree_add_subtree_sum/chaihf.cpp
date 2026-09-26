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

constexpr int N = 200001;

struct Node {
  i64 sum;
  i64 sum_;
  i64 add;
  int size;
  int size_;
  int p;
  int ch[2];
  i8 rev;
  i8 k;
} node[N];

def reverse(int x) noexcept -> void { node[x].rev ^= 1; }

def pushdown(int x) noexcept -> void {
  if (node[x].rev) {
    std::swap(node[x].ch[0], node[x].ch[1]);
    node[node[x].ch[0]].k = 0;
    node[node[x].ch[1]].k = 1;
    reverse(node[x].ch[0]);
    reverse(node[x].ch[1]);
    node[x].rev = 0;
  }
}

def maintain(int x) noexcept -> void {
  node[x].size =
      node[node[x].ch[0]].size + 1 + node[node[x].ch[1]].size + node[x].size_;
  node[x].sum = node[node[x].ch[0]].sum + node[node[x].ch[1]].sum +
                node[x].sum_ + node[x].size * node[x].add;
}

def rotateup(int x) noexcept -> void {
  int p = node[x].p;
  int g = node[p].p;
  int k = node[x].k;
  int t = node[p].k;
  i64 b = node[x].add;
  i64 a = node[p].add;
  node[x].add = a + b;
  node[p].add = -b;
  node[node[x].ch[k ^ 1]].add += b;
  node[node[x].ch[k ^ 1]].sum += b * node[node[x].ch[k ^ 1]].size;
  node[node[x].ch[k ^ 1]].p = p;
  node[node[x].ch[k ^ 1]].k = k;
  node[p].ch[k] = node[x].ch[k ^ 1];
  node[p].p = x;
  node[p].k = k ^ 1;
  node[x].ch[k ^ 1] = p;
  node[x].p = g;
  node[x].k = t;
  if (t != -1) {
    node[g].ch[t] = x;
  }
  maintain(p);
}

def is_root(int x) noexcept -> bool { return node[x].k == -1; }

def splay(int x) noexcept -> void {
  pushdown(x);
  while (!is_root(x)) {
    if (int p = node[x].p; is_root(p)) {
      pushdown(p);
      pushdown(x);
      rotateup(x);
    } else {
      int g = node[p].p;
      pushdown(g);
      pushdown(p);
      pushdown(x);
      if (node[x].k == node[p].k) {
        rotateup(p);
        rotateup(x);
      } else {
        rotateup(x);
        rotateup(x);
      }
    }
  }
}

def access(int x) noexcept -> void {
  splay(x);
  node[node[x].ch[1]].k = -1;
  node[x].sum_ += node[node[x].ch[1]].sum;
  node[x].size_ += node[node[x].ch[1]].size;
  node[x].ch[1] = 0;
  maintain(x);
  while (int p = node[x].p) {
    splay(p);
    node[node[p].ch[1]].k = -1;
    node[p].sum_ += node[node[p].ch[1]].sum;
    node[p].size_ += node[node[p].ch[1]].size;
    node[p].sum_ -= node[x].sum;
    node[p].size_ -= node[x].size;
    node[p].ch[1] = x;
    node[x].k = 1;
    rotateup(x);
    maintain(x);
  }
}

int head[N];
struct {
  int to;
  int next;
} edge[N * 2];

def build(int u, int p) noexcept -> void {
  for (int e = head[u]; e; e = edge[e].next) {
    int v = edge[e].to;
    if (v != p) {
      build(v, u);
      node[v + 1].p = u + 1;
      node[u + 1].sum_ += node[v + 1].sum;
      node[u + 1].size_ += node[v + 1].size;
    }
  }
  node[u + 1].sum = node[u + 1].sum_;
  node[u + 1].size = node[u + 1].size_ + 1;
}

} // namespace

int main() {
  rd rd;
  wt wt;
  int n = rd.uh();
  int q = rd.uh();
#ifdef LOCAL
  std::memset(head, 0, 4 * n);
#endif
  for (int i = 1; i <= n; ++i) node[i] = {.sum_ = rd.uw(), .k = -1};
  for (int i = 1; i != n; ++i) {
    int u = rd.uh();
    int v = rd.uh();
    edge[i * 2 | 0] = {v, head[u]}, head[u] = i * 2 | 0;
    edge[i * 2 | 1] = {u, head[v]}, head[v] = i * 2 | 1;
  }
  build(0, 0);
  while (q--) {
    let t = rd.u1();
    if (t == 0) {
      int u = rd.uh() + 1;
      int v = rd.uh() + 1;
      access(u);
      reverse(u);
      access(v);
      node[node[v].ch[0]].p = 0;
      node[node[v].ch[0]].k = -1;
      node[node[v].ch[0]].add += node[v].add;
      node[v].ch[0] = 0;
      int w = rd.uh() + 1;
      int x = rd.uh() + 1;
      access(w);
      reverse(w);
      access(x);
      node[w].p = x;
      node[w].add -= node[x].add;
      node[w].sum -= node[x].add * node[w].size;
      node[x].sum_ += node[w].sum;
      node[x].size_ += node[w].size;
    }
    if (t == 1) {
      int u = rd.uh() + 1;
      int p = rd.uh() + 1;
      i64 x = rd.uw();
      access(u);
      reverse(u);
      access(p);
      node[u].add += x;
      node[u].sum += x * node[u].size;
    }
    if (t == 2) {
      int u = rd.uh() + 1;
      int p = rd.uh() + 1;
      access(u);
      reverse(u);
      access(p);
      i64 ans = node[u].sum + node[u].size * node[p].add;
      wt.ud(ans);
    }
  }
  return 0;
}
