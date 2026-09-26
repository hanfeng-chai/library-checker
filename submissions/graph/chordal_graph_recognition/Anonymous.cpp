#line 1 "test\\cpv\\library-checker-graph\\chordal_graph_recognition.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/chordal_graph_recognition/"

#line 2 "src\\graph\\chordal\\lib.hpp"

#line 2 "src\\util\\alias\\others\\lib.hpp"

#line 2 "src\\util\\consts\\lib.hpp"

#line 2 "src\\util\\alias\\num\\lib.hpp"

#line 2 "src\\util\\util\\lib.hpp"
// https://github.com/Tiphereth-A/CP-lib
#include <bits/extc++.h>
// clang-format off
namespace tifa_libs {

#define CEXP constexpr
#define CEXPE constexpr explicit
#define CR const&
#define TPN typename
#define NE noexcept
#define CNE const noexcept
#define cT_(...) std::conditional_t<sizeof(__VA_ARGS__) <= sizeof(size_t) * 2, __VA_ARGS__, __VA_ARGS__ CR>
#define flt_(T, i, l, r, ...) for (T i = (l), i##e = (r)__VA_OPT__(, ) __VA_ARGS__; i < i##e; ++i)
#define retif_(cond, if_true, ...) if cond return if_true __VA_OPT__(; else return __VA_ARGS__)
#ifdef ONLINE_JUDGE
#undef assert
#define assert(x) 42
#endif

using namespace std::ranges;
using namespace std::literals;

template <class T>
CEXP T abs(T x) NE { retif_((x < 0), -x, x); }

}  // namespace tifa_libs
// clang-format on
#line 4 "src\\util\\alias\\num\\lib.hpp"

namespace tifa_libs {

#define mk_(w, t) \
  using w = t;    \
  CEXP w operator""_##w(unsigned long long x) NE { return (w)x; }
mk_(i8, int8_t) mk_(u8, uint8_t) mk_(i16, int16_t) mk_(u16, uint16_t) mk_(i32, int32_t) mk_(u32, uint32_t) mk_(i64, int64_t) mk_(u64, uint64_t) mk_(isz, ptrdiff_t) mk_(usz, size_t);
#undef mk_
using i128 = __int128_t;
using u128 = __uint128_t;
using f32 = float;
using f64 = double;
using f128 = long double;

}  // namespace tifa_libs
#line 4 "src\\util\\consts\\lib.hpp"
// clang-format off
namespace tifa_libs {
using std::numbers::pi_v;
template <std::floating_point FP>
inline FP eps_v = std::sqrt(std::numeric_limits<FP>::epsilon());
template <std::floating_point FP>
CEXP void set_eps(FP v) NE { eps_v<FP> = v; }
CEXP u32 TIME = ((__TIME__[0] & 15) << 20) | ((__TIME__[1] & 15) << 16) | ((__TIME__[3] & 15) << 12) | ((__TIME__[4] & 15) << 8) | ((__TIME__[6] & 15) << 4) | (__TIME__[7] & 15);
CEXP auto STR2U16 = [] { std::array<u32, 65536> table{}; table.fill(-1_u32); flt_ (u32, i, 48, 58) flt_ (u32, j, 48, 58) table[i << 8 | j] = (j & 15) * 10 + (i & 15); return table; }();

inline const auto fn_0 = [](auto&&...) NE {};
inline const auto fn_is0 = [](auto x) NE { return x == 0; };
}  // namespace tifa_libs
// clang-format on
#line 4 "src\\util\\alias\\others\\lib.hpp"

namespace tifa_libs {

template <class T>
struct chash {
  CEXP static u64 C = u64(pi_v<f128> * 2e18) | 71;
  CEXP u64 operator()(T x) CNE { return __builtin_bswap64(((u64)x ^ TIME) * C); }
};
// clang-format off
using strn = std::string;
using strnv = std::string_view;
template <class T, T v> using ic = std::integral_constant<T, v>;
template <class T> using ptt = std::pair<T, T>;
template <class T> struct edge_t { T w; u32 u, v; CEXP auto operator<=>(edge_t CR) const = default; };
template <class T> struct pt3 { T _0, _1, _2; CEXP auto operator<=>(pt3 CR) const = default; };
template <class T> struct pt4 { T _0, _1, _2, _3; CEXP auto operator<=>(pt4 CR) const = default; };
template <class T> using alc = std::pmr::polymorphic_allocator<T>;
template <class E> using itl = std::initializer_list<E>;
template <class T> using vec = std::vector<T>;
template <class T> using vvec = vec<vec<T>>;
template <class T> using v3ec = vec<vvec<T>>;
template <class T> using vecpt = vec<ptt<T>>;
template <class T> using vvecpt = vvec<ptt<T>>;
template <class T> using ptvec = ptt<vec<T>>;
template <class T> using ptvvec = ptt<vvec<T>>;
template <class T, usz ext = std::dynamic_extent> using spn = std::span<T const, ext>;
template <class T, usz N> using arr = std::array<T, N>;
template <class U, class T> using vecp = vec<std::pair<U, T>>;
template <class U, class T> using vvecp = vvec<std::pair<U, T>>;
#ifdef PB_DS_ASSOC_CNTNR_HPP
template <class T, class C = std::less<T>> using set = __gnu_pbds::tree<T, __gnu_pbds::null_type, C>;
template <class K, class V, class C = std::less<K>> using map = __gnu_pbds::tree<K, V, C>;
// hset<u64> s({}, {}, {}, {}, {1<<16});
template <class T, class HF = chash<T>> using hset = __gnu_pbds::gp_hash_table<T, __gnu_pbds::null_type, HF>;
// hmap<u64, int> s({}, {}, {}, {}, {1<<16});
template <class K, class V, class HF = chash<K>> using hmap = __gnu_pbds::gp_hash_table<K, V, HF>;
#else
using std::set, std::map;
template <class T, class HF = chash<T>> using hset = std::unordered_set<T, HF>;
template <class K, class V, class HF = chash<K>> using hmap = std::unordered_map<K, V, HF>;
#endif
#ifdef PB_DS_PRIORITY_QUEUE_HPP
template <class T, class C = std::less<T>> using pq = __gnu_pbds::priority_queue<T, C>;
#else
template <class T, class C = std::less<T>> using pq = std::priority_queue<T, vec<T>, C>;
#endif
template <class T> using pqg = pq<T, std::greater<T>>;
// clang-format on
#define mk_(V, A, T) using V##A = V<T>;
#define mk(A, T) mk_(edge_t, A, T) mk_(ptt, A, T) mk_(pt3, A, T) mk_(pt4, A, T) mk_(vec, A, T) mk_(vvec, A, T) mk_(v3ec, A, T) mk_(vecpt, A, T) mk_(vvecpt, A, T) mk_(ptvec, A, T) mk_(ptvvec, A, T) mk_(spn, A, T) mk_(itl, A, T)
mk(b, bool) mk(i, i32) mk(u, u32) mk(ii, i64) mk(uu, u64);
#undef mk
#undef mk_

}  // namespace tifa_libs
#line 2 "src\\graph\\ds\\graph_c\\lib.hpp"

#line 2 "src\\util\\traits\\others\\lib.hpp"
// clang-format off
#line 4 "src\\util\\traits\\others\\lib.hpp"

namespace tifa_libs {

//! only for template without non-type argument
template <class, template <class...> class> CEXP bool specialized_from_v = false;
template <template <class...> class T, class... Args> CEXP bool specialized_from_v<T<Args...>, T> = true;
static_assert(specialized_from_v<vecu, std::vector>);
template <class T> concept container_c = common_range<T> && !std::is_array_v<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, strn> && !std::same_as<std::remove_cvref_t<T>, strnv>;
template <class T> concept istream_c = std::derived_from<T, std::istream> || std::derived_from<T, std::wistream> || requires(T is) { is.peek(); };
template <class T> concept ostream_c = std::derived_from<T, std::ostream> || std::derived_from<T, std::wostream> || requires(T os) { os.flush(); };

}  // namespace tifa_libs
// clang-format on
#line 4 "src\\graph\\ds\\graph_c\\lib.hpp"

namespace tifa_libs {
namespace graph_info_impl_ {
struct graph_info_tag_base {};
template <class... Info>
requires(std::derived_from<Info, graph_info_tag_base> && ...)
struct graph_tag_base : Info... {
  CEXP graph_tag_base(auto&&... args) NE : Info(std::forward<decltype(args)>(args)...)... {}

 protected:
  CEXP void add_arc(auto&&... args) NE { (Info::add_arc(std::forward<decltype(args)>(args)...), ...); }
  CEXP void del_arc(auto&&... args) NE { (Info::del_arc(std::forward<decltype(args)>(args)...), ...); }
};
template <class T, class = std::is_void<T>::type>
struct E;
template <class T>
struct E<T, std::false_type> {
  u32 to;
  T cost;
  CEXP E() = default;
  CEXP E(u32 v, cT_(T) c) NE : to(v), cost(c) {}
  CEXP operator u32() CNE { return to; }
};
template <class T>
struct E<T, std::true_type> {
  u32 to;
  CEXP E() = default;
  CEXP E(u32 v) NE : to(v) {}
  CEXP operator u32() CNE { return to; }
};
}  // namespace graph_info_impl_

namespace graph_impl_ {
template <class etag_t>
struct graph : etag_t {
  using Et = etag_t::val_t;

  CEXP graph() = default;
  CEXPE graph(u32 n, auto&&... e_args) NE : etag_t(n, std::forward<decltype(e_args)>(e_args)...) {}

  CEXP void add_edge(u32 u, u32 v, auto&&... args) NE { etag_t::add_arc(u, v, std::forward<decltype(args)>(args)...), etag_t::add_arc(v, u, std::forward<decltype(args)>(args)...); }
  template <class F>
  CEXP void foreach(u32 u, F&& f) CNE {
    if CEXP (std::is_void_v<Et>)
      for (auto v : (*this)[u]) f(v);
    else
      for (auto [v, w] : (*this)[u]) f(v, w);
  }
};
}  // namespace graph_impl_

// clang-format off
#define CONCEPT_GRAPH(name, base)                                                         \
template <class T> concept name##_c = specialized_from_v<T, base>;                     \
template <class T> concept u##name##_c = name##_c<T> && std::same_as<TPN T::Et, void>; \
template <class T> concept w##name##_c = name##_c<T> && !std::same_as<TPN T::Et, void>
// clang-format on
CONCEPT_GRAPH(graph, graph_impl_::graph);

}  // namespace tifa_libs
#line 5 "src\\graph\\chordal\\lib.hpp"

namespace tifa_libs {

template <graph_c G>  // prefer alist
class chordal {
  G CR g;
  vecu deg;

 public:
  vecu peo, rnk;

  // @param g simple UNDIRECTED graph
  //! g[i] MUST be sorted
  CEXPE chordal(G CR g) NE : g(g), deg(g.vsize()), peo(g.vsize()), rnk(g.vsize()) {
    const u32 n = g.vsize();
    vecu l(n * 2 + 1), r, idx(n);
    std::iota(begin(l), end(l), 0), r = l;
    auto ins = [&](u32 i, u32 j) NE { r[l[i] = l[j]] = i, r[l[j] = i] = j; };
    auto del = [&](u32 i) NE { r[l[i]] = r[i], l[r[i]] = l[i]; };
    flt_ (u32, i, 0, n) ins(i, n);
    u32 li = n;
    flt_ (u32, i, 0, n) {
      ++li;
      while (l[li] == li) --li;
      u32 v = l[li];
      for (idx[v] = -1_u32, del(v); u32 to : g[v])
        if (~idx[to]) ++deg[to], del(to), ins(to, n + (++idx[to]));
      peo[i] = v;
    }
    reverse(peo);
    flt_ (u32, i, 0, n) rnk[peo[i]] = i;
  }

  template <bool find_indcycle = false>
  std::conditional_t<find_indcycle, std::optional<vecu>, bool> is_chordal_graph() CNE {
    std::optional<vecu> ret;
    auto has_edge = [&](u32 u, u32 v) {
      return std::ranges::binary_search(g[u], v, {}, [](auto CR e) { return (u32)e; });
    };
    for (u32 u : peo) {
      vecu s;
      for (s.reserve(g.deg_out(u)); auto v : g[u])
        if (rnk[u] < rnk[v])
          if (s.push_back(v); rnk[s.back()] < rnk[s[0]]) swap(s[0], s.back());
      flt_ (u32, j, 1, (u32)s.size())
        if (!has_edge(s[0], s[j])) {
          if CEXP (!find_indcycle) return false;
          else {
            u32 x = s[j], y = s[0], z = u;
            vecu pre(peo.size(), -1_u32);
            std::queue<u32> q({x});
            while (!q.empty()) {
              u32 t = q.front();
              if (q.pop(); has_edge(t, y)) {
                pre[y] = t;
                ret.emplace({y});
                while (ret->back() != x) ret->push_back(pre[ret->back()]);
                ret->push_back(z);
                return ret;
              }
              for (u32 u : g[t])
                if (u != z && !has_edge(u, z) && !~pre[u]) pre[u] = t, q.push(u);
            }
          }
        }
    }
    if CEXP (find_indcycle) return ret;
    else return true;
  }
  // @return {x}, which $\{x\}+N(x)$ be a maximal clique
  CEXP vecu maximal_cliques() CNE {
    vecu fst(peo.size(), -1_u32), n(peo.size()), res;
    for (u32 u : peo)
      for (auto v : g[u])
        if (rnk[u] < rnk[(u32)v])
          if (++n[u]; !~fst[u] || rnk[(u32)v] < rnk[fst[u]]) fst[u] = (u32)v;
    for (vecb vis(peo.size()); u32 u : peo) {
      if (vis[u]) continue;
      res.push_back(u), vis[fst[u]] = n[u] > n[fst[u]];
    }
    return res;
  }
  CEXP u32 chromatic_number() CNE { return max(deg) + 1; }
  CEXP vecu max_independent_set() CNE {
    vecu res;
    for (vecb vis(peo.size()); u32 u : peo) {
      if (vis[u]) continue;
      for (res.push_back(u); u32 v : g[u]) vis[v] = true;
    }
    return res;
  }
};

}  // namespace tifa_libs
#line 2 "src\\graph\\ds\\alist\\lib.hpp"

#line 4 "src\\graph\\ds\\alist\\lib.hpp"

namespace tifa_libs {
namespace alist_impl_ {
template <class T, class... Info>
class alist_tag : public graph_info_impl_::graph_tag_base<Info...> {
  using base_t = graph_info_impl_::graph_tag_base<Info...>;
  vvec<graph_info_impl_::E<T>> g;

 protected:
  using val_t = T;
  CEXPE alist_tag(u32 n) NE : base_t(n), g(n) {}

 public:
  CEXP void build() CNE {}
  CEXP void add_arc(u32 u, auto&&... args) NE { base_t::add_arc(u, std::forward<decltype(args)>(args)...), g[u].emplace_back(std::forward<decltype(args)>(args)...); }
  CEXP u32 vsize() CNE { return (u32)g.size(); }
  CEXP u32 deg_out(u32 u) CNE { return (u32)g[u].size(); }

  CEXP auto CR operator[](u32 u) CNE { return g[u]; }
  CEXP auto& operator[](u32 u) NE { return g[u]; }
};
}  // namespace alist_impl_
template <class Et = void, class... Info>
using alist = graph_impl_::graph<alist_impl_::alist_tag<Et, Info...>>;

}  // namespace tifa_libs
#line 2 "src\\io\\container\\lib.hpp"

#line 4 "src\\io\\container\\lib.hpp"

namespace tifa_libs {

auto& operator>>(tifa_libs::istream_c auto& is, tifa_libs::container_c auto& x) NE {
  for (auto& i : x) is >> i;
  return is;
}
auto& operator<<(tifa_libs::ostream_c auto& os, tifa_libs::container_c auto CR x) NE {
  if (begin(x) == end(x)) [[unlikely]]
    return os;
  auto it = begin(x);
  for (os << *it++; it != end(x); ++it) os << ' ' << *it;
  return os;
}

}  // namespace tifa_libs
#line 6 "test\\cpv\\library-checker-graph\\chordal_graph_recognition.cpp"

using namespace tifa_libs;
int main() {
  std::cin.tie(nullptr)->std::ios::sync_with_stdio(false);
  u32 n, m;
  std::cin >> n >> m;
  alist g(n);
  for (u32 i = 0, u, v; i < m; ++i) {
    std::cin >> u >> v;
    g.add_arc(u, v);
    g.add_arc(v, u);
  }
  flt_(u32,i,0,n) std::ranges::sort(g[i]);
  chordal chd(g);
  if (auto res = chd.template is_chordal_graph<true>(); res) std::cout << "NO\n"
                                                                       << res->size() << '\n'
                                                                       << res.value() << '\n';
  else std::cout << "YES\n"
                 << chd.peo << '\n';
  return 0;
}
