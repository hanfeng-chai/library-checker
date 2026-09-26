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

namespace toy
{

  using i32 = std::int32_t;
  using i64 = std::int64_t;

  using u8 = std::uint8_t;
  using u16 = std::uint16_t;
  using u32 = std::uint32_t;
  using u64 = std::uint64_t;
  using u128 = unsigned __int128;
  using usize = std::size_t;

  using f32 = float;
  using f64 = double;
  using f80 = long double;

  template <typename T>
  fun ensure(T &&t) noexcept -> decltype(auto)
  {
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

struct rd
{
  static fun all_digit(u32 x) noexcept -> bool
  {
    x ^= 0x30303030;
    x &= 0xf0f0f0f0;
    return !x;
  }

  static fun all_digit(u64 x) noexcept -> bool
  {
    x ^= 0x3030303030303030;
    x &= 0xf0f0f0f0f0f0f0f0;
    return !x;
  }

  char *p, *l, *r;

  rd() noexcept
  {
    struct stat st;
    fstat(0, &st);
    p = l = r = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    r += st.st_size;
  }
  ~rd() noexcept { munmap(l, r - l); }

  def skip(int n) noexcept -> void { p += n; }

  def ch() noexcept -> u8
  {
    u8 x = *p++;
    return ++p, x;
  }

  def u1() noexcept -> u32
  {
    u32 x = *p++ - '0';
    return ++p, x;
  }

  def ub() noexcept -> u32
  {
    u32 x = *p++ - '0';
    for (; *p >= '0'; ++p)
      x = x * 10 + *p - '0';
    return ++p, x;
  }

  def uh() noexcept -> u32
  {
    u32 x{};
    u32 a;
    std::memcpy(&a, p, sizeof(a));
    if (all_digit(a))
    {
      a ^= 0x30303030;
      a = (a * 10 + (a >> 8)) & 0x00ff00ff;
      a = (a * 100 + (a >> 16)) & 0x0000ffff;
      x = a, p += sizeof(a);
    }
    for (; *p >= '0'; ++p)
      x = x * 10 + *p - '0';
    return ++p, x;
  }

  def uw() noexcept -> u32
  {
    u32 x{};
    u64 a;
    std::memcpy(&a, p, sizeof(a));
    if (all_digit(a))
    {
      a ^= 0x3030303030303030;
      a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
      a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
      a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
      x = u32(a), p += sizeof(a);
    }
    for (; *p >= '0'; ++p)
      x = x * 10 + *p - '0';
    return ++p, x;
  }

  def ud() noexcept -> u64
  {
    u64 x{};
    union
    {
      u128 ch;
      u64 d[2];
    };
    std::memcpy(&ch, p, sizeof(ch));
    u64 a = d[0], b = d[1];
    if (all_digit(a) && all_digit(b))
    {
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
    for (; *p >= '0'; ++p)
      x = x * 10 + *p - '0';
    return ++p, x;
  }

  def i1() noexcept -> i32 { return *p == '-' ? ++p, -u1() : u1(); }
  def ib() noexcept -> i32 { return *p == '-' ? ++p, -ub() : ub(); }
  def ih() noexcept -> i32 { return *p == '-' ? ++p, -uh() : uh(); }
  def iw() noexcept -> i32 { return *p == '-' ? ++p, -uw() : uw(); }
  def id() noexcept -> i64 { return *p == '-' ? ++p, -ud() : ud(); }
};

struct wt
{
  static fun low = []
  {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i)
    {
      res[i][0] = '0' + i / 1000 % 10;
      res[i][1] = '0' + i / 100 % 10;
      res[i][2] = '0' + i / 10 % 10;
      res[i][3] = '0' + i % 10;
    }
    return res;
  }();
  static fun pos = []
  {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i)
    {
      res[i][0] = '0' + i / 1000 % 10;
      res[i][1] = '0' + i / 100 % 10;
      res[i][2] = '0' + i / 10 % 10;
      res[i][3] = '0' + i % 10;
      if (i < 1000)
        res[i][0] = ' ';
      if (i < 100)
        res[i][1] = ' ';
      if (i < 10)
        res[i][2] = ' ';
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

  void puts(const char *src, int n) noexcept
  {
    if (sentinel < p + n)
      flush();
    std::memcpy(p, src, n), p += n;
  }

  void u1(u8 x) noexcept
  {
    toy_assert(x < 10);
    *p++ = ' ';
    *p++ = '0' + x;
  }

  void uw(u32 x) noexcept
  {
    if (sentinel < p)
      flush();
    *p++ = ' ';
    if (x > 9999'9999)
    {
      print_pos(x / 10000 / 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    }
    else if (x > 9999)
    {
      print_pos(x / 10000);
      print_low(x % 10000);
    }
    else
    {
      print_pos(x);
    }
  }

  void ud(u64 x) noexcept
  {
    if (sentinel < p)
      flush();
    *p++ = ' ';
    if (x > 9999'9999'9999'9999)
    {
      print_pos(x / 10000 / 10000 / 10000 / 10000);
      print_low(x / 10000 / 10000 / 10000 % 10000);
      print_low(x / 10000 / 10000 % 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    }
    else if (x > 9999'9999'9999)
    {
      print_pos(x / 10000 / 10000 / 10000);
      print_low(x / 10000 / 10000 % 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    }
    else if (x > 9999'9999)
    {
      print_pos(x / 10000 / 10000);
      print_low(x / 10000 % 10000);
      print_low(x % 10000);
    }
    else if (x > 9999)
    {
      print_pos(x / 10000);
      print_low(x % 10000);
    }
    else
    {
      print_pos(x);
    }
  }
};
#include <algorithm>
#include <bit>
#include <cstring>
#include <iostream>
using namespace std;
const uint32_t N = 5e5 + 1, K = 64;
namespace BlockCatTree
{
  uint32_t a[N], prefixInBlock[N], suffixInBlock[N], ct[bit_width(N / K)][N / K];
  inline void init(uint32_t n)
  {
    memcpy(prefixInBlock, a, sizeof(uint32_t) * n);
    for (uint32_t l = 0; l < n; l += K)
      for (uint32_t i = l + 1, r = min(n, l + K); i < r; ++i)
        prefixInBlock[i] = min(prefixInBlock[i - 1], prefixInBlock[i]);
    memcpy(suffixInBlock, a, sizeof(uint32_t) * n);
    for (uint32_t l = 0; l < n; l += K)
      for (uint32_t i = min(n, l + K) - 1; i > l; --i)
        suffixInBlock[i - 1] = min(suffixInBlock[i], suffixInBlock[i - 1]);
    uint32_t block = n / K;
    for (uint32_t i = 0; i < block; ++i)
      ct[0][i] = suffixInBlock[i * K];
    for (uint32_t i = 1; i < bit_width(block); ++i)
      memcpy(ct[i], ct[0], sizeof(uint32_t) * block);
    for (uint32_t dep = 1; dep < bit_width(block); ++dep)
      for (uint32_t l = 0; l < block; l += (1 << (dep + 1)))
      {
        for (uint32_t i = l + (1 << dep) - 1; i > l; --i)
          ct[dep][i - 1] = min(ct[dep][i], ct[dep][i - 1]);
        for (uint32_t i = l + (1 << dep), r = min(l + (1 << (dep + 1)), block) - 1; i < r; ++i)
          ct[dep][i + 1] = min(ct[dep][i], ct[dep][i + 1]);
      }
  }
  inline uint32_t query(uint32_t l, uint32_t r)
  {
    uint32_t bl = l / K, br = r / K;
    if (bl == br)
      return *min_element(a + l, a + r + 1);
    uint32_t res = min(suffixInBlock[l], prefixInBlock[r]);
    if (bl + 1 == br)
      return res;
    ++bl, --br;
    uint32_t dep = (bl == br ? 0 : bit_width(bl ^ br) - 1);
    return min({res, ct[dep][bl], ct[dep][br]});
  }
}
uint32_t fa[N];
namespace DFNLCA
{
  uint32_t path[N], dfn[N];
  inline void init(int n)
  {
    for (uint32_t i = n - 1; i > 0; --i)
      dfn[fa[i]] += dfn[i] + 1;
    for (uint32_t i = 1; i < n; ++i)
    {
      uint32_t t = dfn[i] + 1;
      dfn[i] = dfn[fa[i]];
      dfn[fa[i]] -= t;
    }
    for (int i = 0; i < n; ++i)
      path[dfn[i]] = i;
    for (int i = 1; i < n; ++i)
      BlockCatTree::a[i - 1] = fa[path[i]];
    BlockCatTree::init(n - 1);
  }
  inline int lca(int u, int v)
  {
    if (u == v)
      return u;
    int l = dfn[u], r = dfn[v];
    if (l > r)
      swap(l, r);
    return BlockCatTree::query(l, r - 1);
  }
}
int main()
{
  rd rd;
  wt wt;
  uint32_t n = rd.uh(), m = rd.uh();
  for (uint32_t i = 1; i < n; ++i)
    fa[i] = rd.uh();
  DFNLCA::init(n);
  for (uint32_t i = 0; i < m; ++i)
  {
    uint32_t u = rd.uh(), v = rd.uh();
    wt.uw(DFNLCA::lca(u, v));
  }
  return 0;
}