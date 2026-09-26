#pragma GCC target("abm,movbe,bmi,bmi2,lzcnt,popcnt,avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("omit-frame-pointer")
#define NDEBUG 1
#include <immintrin.h>
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <chrono>
#include <climits>
#include <cmath>
#include <compare>
#include <complex>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <new>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
namespace algo::detail {
struct modnum_base {};
struct monostate {};
template <typename Tp>
concept integer = std::integral<Tp> && !(std::same_as<Tp, bool>);
template <typename Tp>
concept signed_integer = integer<Tp> && (Tp(-1) < Tp(0));
template <typename Tp>
concept unsigned_integer = integer<Tp> && (Tp(-1) > Tp(0));
template <typename Tp>
concept modular_integer = std::is_base_of_v<modnum_base, Tp>;
template <typename From, typename To>
concept sign_compatible_with =
    integer<From> && ((std::is_signed_v<From> && std::is_signed_v<To>) ||
                      (std::is_unsigned_v<From> && std::is_unsigned_v<To>));
template <typename Tp>
concept arithmetic = integer<Tp> || (std::is_floating_point_v<Tp>);
template <typename Tp>
concept qword_fittable = sizeof(Tp) <= 8UL;
template <typename Tp>
concept dword_fittable = qword_fittable<Tp> && sizeof(Tp) <= 4UL;
template <typename Tp>
concept word_fittable = dword_fittable<Tp> && sizeof(Tp) <= 2UL;
template <typename Tp>
concept byte_fittable = word_fittable<Tp> && sizeof(Tp) == 1UL;
template <typename Tp>
struct imul_result;
template <>
struct imul_result<int64_t> {
  using type = __int128_t;
};
template <>
struct imul_result<uint64_t> {
  using type = __uint128_t;
};
template <>
struct imul_result<int32_t> {
  using type = int64_t;
};
template <>
struct imul_result<uint32_t> {
  using type = uint64_t;
};
template <>
struct imul_result<int16_t> {
  using type = int32_t;
};
template <>
struct imul_result<uint16_t> {
  using type = uint32_t;
};
template <>
struct imul_result<int8_t> {
  using type = int16_t;
};
template <>
struct imul_result<uint8_t> {
  using type = uint16_t;
};
template <typename Tp>
using imul_result_t = typename imul_result<Tp>::type;
template <typename Sig>
struct function_traits;
template <typename Ret, typename... Args>
struct function_traits<Ret(Args...)> {
  using return_type = Ret;
  template <typename Fn>
  static constexpr bool same_as =
      std::is_invocable_r_v<return_type, Fn, Args...>;
};
template <typename Fn, typename Sig>
concept function = (function_traits<Sig>::template same_as<Fn>);
template <typename IIter, typename Value>
concept input_iterator = std::same_as<std::iter_value_t<IIter>, Value>;
}
namespace algo::detail {
inline void unreachable [[gnu::noreturn]] () { __builtin_unreachable(); }
constexpr void assume [[gnu::always_inline]] (bool expr) {
  if (!expr) { __builtin_unreachable(); }
}
}
namespace algo::detail {
template <typename Tp>
constexpr int count_lz(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return std::countl_zero<Up>(n);
}
template <typename Tp>
constexpr int count_tz(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return std::countr_zero<Up>(n);
}
template <typename Tp>
constexpr int bit_width(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return std::bit_width<Up>(n);
}
template <typename Tp>
constexpr Tp floor_pow2(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return static_cast<Tp>(std::bit_floor<Up>(n));
}
template <typename Tp>
constexpr Tp ceil_pow2(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return static_cast<Tp>(std::bit_ceil<Up>(n));
}
template <typename Tp>
constexpr int floor_log2(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  constexpr int Nd = std::numeric_limits<Up>::digits;
  return Nd - 1 - std::countl_zero<Up>(n);
}
template <typename Tp>
constexpr int ceil_log2(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  constexpr int Nd = std::numeric_limits<Up>::digits;
  if (n == 0) { return 0; }
  return Nd - std::countl_zero<Up>(n - 1);
}
template <typename Tp>
constexpr Tp blsr(Tp n) {
  if (std::is_constant_evaluated()) { return n & (n - 1); }
  if constexpr (dword_fittable<Tp>) {
    return static_cast<Tp>(_blsr_u32(n));
  } else if constexpr (qword_fittable<Tp>) {
    return static_cast<Tp>(_blsr_u64(n));
  } else {
    return n & (n - 1);
  }
}
template <typename Tp>
constexpr Tp blsi(Tp n) {
  if (std::is_constant_evaluated()) { return n & -n; }
  if constexpr (dword_fittable<Tp>) {
    return static_cast<Tp>(_blsi_u32(n));
  } else if constexpr (qword_fittable<Tp>) {
    return static_cast<Tp>(_blsi_u64(n));
  } else {
    return n & -n;
  }
}
template <typename Tp>
constexpr int popcount(Tp n) {
  using Up = std::make_unsigned_t<Tp>;
  return std::popcount<Up>(n);
}
}
namespace algo {
template <detail::integer Tp>
constexpr int count_lz(Tp n) {
  return detail::count_lz(n);
}
template <detail::integer Tp>
constexpr int count_tz(Tp n) {
  return detail::count_tz(n);
}
template <detail::integer Tp>
constexpr int bit_width(Tp n) {
  return detail::bit_width(n);
}
template <detail::integer Tp>
constexpr Tp floor_pow2(Tp n) {
  return detail::floor_pow2(n);
}
template <detail::integer Tp>
constexpr Tp ceil_pow2(Tp n) {
  return detail::ceil_pow2(n);
}
template <detail::integer Tp>
constexpr int floor_log2(Tp n) {
  return detail::floor_log2(n);
}
template <detail::integer Tp>
constexpr int ceil_log2(Tp n) {
  return detail::ceil_log2(n);
}
template <detail::integer Tp>
constexpr Tp blsr(Tp n) {
  return detail::blsr(n);
}
template <detail::integer Tp>
constexpr Tp blsi(Tp n) {
  return detail::blsi(n);
}
template <detail::integer Tp>
constexpr int popcount(Tp n) {
  return detail::popcount(n);
}
}
#define PRINT(...) (void)0
#define DEBUG(x) (void)0
namespace algo::detail {
template <typename Derived, typename Node>
class basic_lct {
public:
  basic_lct() noexcept = default;
  explicit basic_lct(int n, const Node& def = Node{}) noexcept
      : tree_(n + 1, def) {}
  void reroot(int u) { do_reroot(u + 1); }
  void link(int v, int u) { do_link(v + 1, u + 1); }
  void cut(int v, int u) { do_cut(v + 1, u + 1); }
  int parent(int u) { return get_parent(u + 1) - 1; }
  int root(int u) { return get_root(u + 1) - 1; }
  int lca(int v, int u) { return get_lca(v + 1, u + 1) - 1; }
  bool connected(int v, int u) { return reachable(v + 1, u + 1); }
  int num_nodes() const { return static_cast<int>(tree_.size()) - 1; }
  int null() const { return Null - 1; }
protected:
  void do_reroot(int u) {
    access(u);
    tree_[u].flip ^= true;
    push(u);
  }
  void do_link(int v, int u) {
    do_reroot(u);
    access(v);
    tree_[v].children[1] = u;
    tree_[u].parent = v;
    self()->fix_link(v, u);
    update(v);
  }
  void do_cut(int v, int u) {
    do_reroot(u);
    access(v);
    if (tree_[v].children[0] == u) {
      tree_[v].children[0] = Null;
      tree_[u].parent = Null;
      self()->fix_cut(v, u);
      update(v);
    }
  }
  int get_parent(int u) {
    access(u);
    return tree_[u].children[0];
  }
  int get_root(int u) {
    access(u);
    while (tree_[u].children[0] != Null) {
      u = tree_[u].children[0];
      push(u);
    }
    splay(u);
    return u;
  }
  int get_lca(int v, int u) {
    if (u == v) { return u; }
    access(v);
    int w = Null, z = u;
    do {
      splay(z);
      toggle(z, w);
      w = z, z = tree_[z].parent;
    } while (z != Null);
    splay(u);
    return tree_[v].parent != Null ? w : Null;
  }
  bool reachable(int v, int u) { return get_lca(v, u) != Null; }
  void access(int u) {
    splay(u);
    toggle(u, Null);
    while (tree_[u].parent != Null) {
      const int w = tree_[u].parent;
      splay(w);
      toggle(w, u);
      rotate(u);
    }
  }
  int side(int u) const {
    const int p = tree_[u].parent;
    if (tree_[p].children[0] == u) { return 0; }
    if (tree_[p].children[1] == u) { return 1; }
    return -1;
  }
  bool is_root(int u) const { return side(u) < 0; }
  void toggle(int v, int u) {
    const int c = tree_[v].children[1];
    self()->fix_remove(v, c);
    self()->fix_attach(v, u);
    tree_[v].children[1] = u;
    tree_[u].parent = v;
    update(v);
  }
  void rotate(int u) {
    const int v = tree_[u].parent;
    const int w = tree_[v].parent;
    const int du = side(u);
    const int dv = side(v);
    const int b = tree_[u].children[!du];
    self()->fix_rotate(v, u, b);
    attach(v, b, du);
    attach(u, v, !du);
    if (dv != -1) { attach(w, u, dv); }
    tree_[u].parent = w;
  }
  void attach(int v, int u, bool d) {
    tree_[v].children[d] = u;
    tree_[u].parent = v;
    update(v);
  }
  void update(int u) { self()->push_up(u); }
  void push(int u) { self()->push_down(u); }
  void splay(int u) {
    push(u);
    while (!is_root(u) && !is_root(tree_[u].parent)) {
      const int v = tree_[u].parent;
      const int w = tree_[v].parent;
      push(w);
      push(v);
      push(u);
      rotate(side(v) == side(u) ? v : u);
      rotate(u);
    }
    if (!is_root(u)) {
      push(tree_[u].parent);
      push(u);
      rotate(u);
    }
  }
  void push_down(int u) {}
  void push_up(int) {}
  void fix_link(int, int) {}
  void fix_cut(int, int) {}
  void fix_rotate(int, int, int) {}
  void fix_remove(int, int) {}
  void fix_attach(int, int) {}
  const Derived* self() const { return static_cast<const Derived*>(this); }
  Derived* self() { return static_cast<Derived*>(this); }
  static constexpr int Null = 0;
  std::vector<Node> tree_;
};
}
namespace algo::link_cut::vertex_set {
namespace impl {
template <typename Group>
struct subtree_aggregation_node {
  Group sub;
  Group self;
  Group vsub;
  int children[2]{0};
  int parent{0};
  bool flip{0};
};
}
template <std::semiregular Group, detail::function<Group(Group, Group)> BinOp,
          detail::function<Group(void)> Id, detail::function<Group(Group)> Inv>
class subtree_aggregation_handler
    : public detail::basic_lct<
          subtree_aggregation_handler<Group, BinOp, Id, Inv>,
          impl::subtree_aggregation_node<Group>> {
  using self_type = subtree_aggregation_handler;
  using node_type = impl::subtree_aggregation_node<Group>;
  using base_type = detail::basic_lct<self_type, node_type>;
  friend class detail::basic_lct<self_type, node_type>;
public:
  subtree_aggregation_handler() = default;
  subtree_aggregation_handler(int n, BinOp binop, Id id, Inv inv) noexcept
      : base_type{n, {id(), id(), id()}},
        binop_{std::move(binop)},
        id_{std::move(id)},
        inv_{std::move(inv)} {}
  subtree_aggregation_handler(int n, const Group& def, BinOp binop, Id id,
                              Inv inv) noexcept
      : base_type{n, {def, def, id()}},
        binop_{std::move(binop)},
        id_{std::move(id)},
        inv_{std::move(inv)} {
    tree_[Null].sub = id_();
    tree_[Null].self = id_();
  }
  template <detail::input_iterator<Group> IIter>
  subtree_aggregation_handler(IIter first, IIter last, BinOp binop, Id id,
                              Inv inv) noexcept
      : base_type{static_cast<int>(std::distance(first, last))},
        binop_{std::move(binop)},
        id_{std::move(id)},
        inv_{std::move(inv)} {
    std::transform(first, last, tree_.begin() + 1, [this](const Group& x) {
      return node_type{x, x, id_()};
    });
    tree_[Null].sub = id_();
    tree_[Null].self = id_();
    tree_[Null].vsub = id_();
  }
  Group query_subtree(int u) { return get_subtree(u + 1); }
  void set(int u, Group new_val) { do_set(u + 1, std::move(new_val)); }
  Group get(int u) const { return do_get(u + 1); }
private:
  Group get_subtree(int u) {
    this->access(u);
    return binop_(tree_[u].self, tree_[u].vsub);
  }
  void do_set(int u, Group new_val) {
    this->access(u);
    tree_[u].self = std::move(new_val);
    this->update(u);
  }
  Group do_get(int u) const { return tree_[u].self; }
  void push_down(int u) {
    if (tree_[u].flip) {
      std::swap(tree_[u].children[0], tree_[u].children[1]);
      const int l = tree_[u].children[0];
      const int r = tree_[u].children[1];
      tree_[u].flip ^= true;
      tree_[l].flip ^= true;
      tree_[r].flip ^= true;
    }
  }
  void push_up(int u) {
    const int l = tree_[u].children[0];
    const int r = tree_[u].children[1];
    Group sub = binop_(tree_[l].sub, tree_[r].sub);
    tree_[u].sub = binop_(binop_(tree_[u].vsub, tree_[u].self), std::move(sub));
  }
  void fix_remove(int v, int c) {
    tree_[v].vsub = binop_(tree_[v].vsub, tree_[c].sub);
  }
  void fix_attach(int v, int u) {
    tree_[v].vsub = binop_(tree_[v].vsub, inv_(tree_[u].sub));
  }
  using base_type::Null;
  using base_type::tree_;
  [[no_unique_address]] BinOp binop_;
  [[no_unique_address]] Id id_;
  [[no_unique_address]] Inv inv_;
};
}
namespace algo::link_cut::vertex_set {
template <std::semiregular Group, detail::function<Group(Group, Group)> BinOp,
          detail::function<Group(void)> Id, detail::function<Group(Group)> Inv>
auto make_subtree_aggregation_handler(int n, BinOp binop, Id id, Inv inv)
    -> subtree_aggregation_handler<Group, BinOp, Id, Inv> {
  return {n, std::move(binop), std::move(id), std::move(inv)};
}
template <std::semiregular Group, detail::function<Group(Group, Group)> BinOp,
          detail::function<Group(void)> Id, detail::function<Group(Group)> Inv>
auto make_subtree_aggregation_handler(int n, const Group& def, BinOp binop,
                                      Id id, Inv inv)
    -> subtree_aggregation_handler<Group, BinOp, Id, Inv> {
  return {n, def, std::move(binop), std::move(id), std::move(inv)};
}
template <std::semiregular Group, detail::function<Group(Group, Group)> BinOp,
          detail::function<Group(void)> Id, detail::function<Group(Group)> Inv,
          detail::input_iterator<Group> IIter>
auto make_subtree_aggregation_handler(IIter first, IIter last, BinOp binop,
                                      Id id, Inv inv)
    -> subtree_aggregation_handler<Group, BinOp, Id, Inv> {
  return {first, last, std::move(binop), std::move(id), std::move(inv)};
}
}
namespace algo::detail {
template <typename Tp>
constexpr Tp sqrt(Tp n) {
  if (n == 0) { return 0; }
  Tp ans = 0;
  for (int i = floor_log2(n) >> 1; i >= 0; --i) {
    const Tp tmp = (n - ans * ans) >> i;
    if ((Tp(1) << i) + (ans << 1) <= tmp) { ans |= Tp(1) << i; }
  }
  return ans;
}
template <typename Tp>
constexpr Tp abs(Tp n) {
  return n < 0 ? -n : n;
}
template <typename Tp>
constexpr Tp floor_div(Tp x, Tp y) {
  if constexpr (std::is_signed_v<Tp>) {
    return x / y - ((x ^ y) < 0 && x % y != 0);
  } else {
    return x / y;
  }
}
template <typename Tp>
constexpr Tp ceil_div(Tp x, Tp y) {
  if constexpr (std::is_signed_v<Tp>) {
    return y > 0 ? floor_div(x + y - 1, y) : -floor_div(x, -y);
  } else {
    return floor_div(x + y - 1, y);
  }
}
template <typename Tp>
constexpr int alt(Tp n) {
  return -(n & 1) | 1;
}
}
namespace algo {
template <detail::integer Tp>
constexpr Tp sqrt(Tp n) {
  return detail::sqrt(n);
}
template <detail::integer Tp>
constexpr Tp abs(Tp n) {
  return detail::abs(n);
}
template <detail::integer T1, detail::sign_compatible_with<T1> T2>
constexpr std::common_type_t<T1, T2> floor_div(T1 x, T2 y) {
  using Tp = std::common_type_t<T1, T2>;
  return detail::floor_div<Tp>(x, y);
}
template <detail::integer T1, detail::sign_compatible_with<T1> T2>
constexpr std::common_type_t<T1, T2> ceil_div(T1 x, T2 y) {
  using Tp = std::common_type_t<T1, T2>;
  return detail::ceil_div<Tp>(x, y);
}
template <std::integral Tp>
constexpr int alt(Tp n) {
  return detail::alt(n);
}
template <detail::integer T1, detail::sign_compatible_with<T1> T2,
          detail::sign_compatible_with<T2> T3,
          detail::sign_compatible_with<T3> T4>
constexpr auto floor_sum(T1 a, T2 b, T3 c, T4 n)
    -> std::common_type_t<T1, T2, T3, T4> {
  detail::assume(n >= 0);
  detail::assume(c > 0);
  detail::assume(a >= 0);
  using Tp = std::common_type_t<T1, T2, T3, T4>;
  Tp res = 0, a0 = a, b0 = b, c0 = c, n0 = n;
  bool neg = false;
  while (a0 != 0) {
    if (a0 < c0 && b0 < c0) {
      const Tp m = (a0 * n0 + b0) / c0;
      const Tp tmp = a0;
      res += alt(neg) * m * n0;
      b0 = c0 - b0 - 1, a0 = c0, c0 = tmp, n0 = m - 1;
      neg ^= true;
    } else {
      const Tp tmp = (n0 * (n0 + 1) >> 1) * (a0 / c0) + (n0 + 1) * (b0 / c0);
      res += alt(neg) * tmp;
      a0 %= c0, b0 %= c0;
    }
  }
  res += alt(neg) * (b0 / c0) * (n0 + 1);
  return res;
}
}
namespace views = std::views;
namespace chrono = std::chrono;
using std::array;
using std::bitset;
using std::cin;
using std::complex;
using std::cout;
using std::deque;
using std::endl;
using std::gcd;
using std::lcm;
using std::map;
using std::max;
using std::min;
using std::move;
using std::multimap;
using std::multiset;
using std::nullopt;
using std::optional;
using std::pair;
using std::queue;
using std::set;
using std::span;
using std::string;
using std::string_view;
using std::swap;
using std::tie;
using std::to_string;
using std::tuple;
using std::vector;
using wint = int64_t;
using uint = uint32_t;
using bint = uint8_t;
template <typename Tp>
using stack = std::stack<Tp, std::vector<Tp>>;
template <typename Key, typename Value, typename Hash = std::hash<Key>>
using umap = std::unordered_map<Key, Value, Hash>;
template <typename Key, typename Hash = std::hash<Key>>
using uset = std::unordered_set<Key, Hash>;
template <typename Tp, typename Comp = std::less<Tp>>
using max_heap = std::priority_queue<Tp, std::vector<Tp>, Comp>;
template <typename Tp, typename Comp = std::greater<Tp>>
using min_heap = std::priority_queue<Tp, std::vector<Tp>, Comp>;
#define INF 0x3f3f3f3f
#define MOD 998244353
#define MAXN 200001
#define MULTI 0
namespace {
}
namespace fastio {
static constexpr int SZ = 1 << 17;
char ibuf[SZ], obuf[SZ];
int pil = 0, pir = 0, por = 0;
struct Pre {
  char num[40000];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i * 4 + j] = n % 10 + '0';
        n /= 10;
      }
    }
  }
} constexpr pre;
inline void load() {
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
}
inline void flush() {
  fwrite(obuf, 1, por, stdout);
  por = 0;
}
inline void rd(char& c) { c = ibuf[pil++]; }
template <typename T>
inline void rd(T& x) {
  if (pil + 32 > pir) load();
  char c;
  do c = ibuf[pil++];
  while (c < '-');
  bool minus = 0;
  if (c == '-') {
    minus = 1;
    c = ibuf[pil++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = ibuf[pil++];
  }
  if (minus) x = -x;
}
inline void rd() {}
template <typename Head, typename... Tail>
inline void rd(Head& head, Tail&... tail) {
  rd(head);
  rd(tail...);
}
inline void wt(char c) { obuf[por++] = c; }
template <typename T>
inline void wt(T x) {
  if (por > SZ - 32) flush();
  if (!x) {
    obuf[por++] = '0';
    return;
  }
  if (x < 0) {
    obuf[por++] = '-';
    x = -x;
  }
  int i = 12;
  char buf[16];
  while (x >= 10000) {
    memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
    x /= 10000;
    i -= 4;
  }
  int d = x < 100 ? (x < 10 ? 1 : 2) : (x < 1000 ? 3 : 4);
  memcpy(obuf + por, pre.num + x * 4 + 4 - d, d);
  por += d;
  memcpy(obuf + por, buf + i + 4, 12 - i);
  por += 12 - i;
}
inline void wt() {}
template <typename Head, typename... Tail>
inline void wt(Head head, Tail... tail) {
  wt(head);
  wt(tail...);
}
template <typename T>
inline void wtn(T x) {
  wt(x, '\n');
}
struct Dummy {
  Dummy() { atexit(flush); }
} dummy;
}
using fastio::rd;
using fastio::wt;
using fastio::wtn;
void solve() noexcept {
  int n, q;
  rd(n, q);
  vector<wint> vec(n);
  for (auto& x : vec) { rd(x); }
  auto lct = algo::link_cut::vertex_set::make_subtree_aggregation_handler<wint>(
      vec.begin(), vec.end(), [](wint x, wint y) { return x + y; },
      [] { return 0; }, [](wint x) { return -x; });
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    rd(u, v);
    lct.link(u, v);
  }
  while (q--) {
    if (int t; rd(t), t == 0) {
      int u, v, w, x;
      rd(u, v, w, x);
      lct.cut(u, v);
      lct.link(w, x);
    } else if (t == 1) {
      int p, x;
      rd(p, x);
      lct.set(p, lct.get(p) + x);
    } else {
      int v, p;
      rd(v, p);
      lct.reroot(p);
      wtn(lct.query_subtree(v));
    }
  }
}
int main() {
  solve();
  return 0;
}
