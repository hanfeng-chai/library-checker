#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

#if defined(__SIZEOF_INT128__) && !defined(ATL_NO_INT128)
#define ATL_HAS_INT128 1
#else
#define ATL_HAS_INT128 0
#endif

namespace atl {

	using int32 = std::int32_t;

	using uint8 = std::uint8_t;

	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

}

namespace atl::detail {

}

namespace atl::traits {

	template<typename Tp, typename... Types>
	struct is_one_of : public std::disjunction<std::is_same<Tp, Types>...> {};

}

namespace atl::traits {

	template<typename T>
	struct is_signed_integral
		: public is_one_of<std::remove_cv_t<T>, signed char, signed short, signed int, signed long, signed long long
#if ATL_HAS_INT128
						   ,
						   int128
#endif
						   > {
	};

	template<typename T>
	constexpr bool is_signed_integral_v = is_signed_integral<T>::value;

	template<typename T>
	struct is_unsigned_integral
		: public is_one_of<std::remove_cv_t<T>, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#if ATL_HAS_INT128
						   ,
						   uint128
#endif
						   > {
	};

	template<typename T>
	constexpr bool is_unsigned_integral_v = is_unsigned_integral<T>::value;

	template<typename T>
	struct is_integral : public std::bool_constant<is_signed_integral_v<T> || is_unsigned_integral_v<T>> {};

	template<typename T>
	constexpr bool is_integral_v = is_integral<T>::value;

	namespace detail {

		template<typename From, typename To>
		struct copy_cv {
			using type = To;
		};

		template<typename From, typename To>
		struct copy_cv<From const, To> {
			using type = To const;
		};

		template<typename From, typename To>
		struct copy_cv<From volatile, To> {
			using type = To volatile;
		};

		template<typename From, typename To>
		struct copy_cv<From const volatile, To> {
			using type = To const volatile;
		};

		template<typename From, typename To>
		using copy_cv_t = typename copy_cv<From, To>::type;
	}

#if ATL_HAS_INT128

#endif

	template<typename T>
	struct make_unsigned : std::make_unsigned<T> {};

#if ATL_HAS_INT128
	template<typename T>
		requires std::is_same_v<std::remove_cv_t<T>, int128>
	struct make_unsigned<T> {
		using type = detail::copy_cv_t<T, uint128>;
	};

	template<typename T>
		requires std::is_same_v<std::remove_cv_t<T>, uint128>
	struct make_unsigned<T> {
		using type = T;
	};
#endif

	template<typename T>
		requires std::is_enum_v<T>
	struct make_unsigned<T>;

	template<typename T>
	using make_unsigned_t = typename make_unsigned<T>::type;
}

namespace atl {

	template<typename T>
	concept integral = traits::is_integral_v<T>;

	template<typename T>
	concept signed_integral = traits::is_signed_integral_v<T>;

	template<typename T>
	concept unsigned_integral = traits::is_unsigned_integral_v<T>;
}

namespace atl::graph {

	using vertex_id = uint32;

	template<typename Weight = void>
	struct edge {
	};

	template<typename Weight = void>
	struct arc {
	};

	template<>
	struct arc<void> {
		vertex_id from, to;
	};

	template<typename E>
	struct graph_traits {};

	namespace detail {
		template<typename E>
		concept traits_target = requires(E const &e) {
			{ graph_traits<E>::target(e) } -> std::convertible_to<vertex_id>;
		};

		template<typename E>
		concept member_target = requires(E const &e) {
			{ e.to } -> std::convertible_to<vertex_id>;
		};

		template<typename E>
		concept traits_source = requires(E const &e) {
			{ graph_traits<E>::source(e) } -> std::convertible_to<vertex_id>;
		};

		template<typename E>
		concept member_source = requires(E const &e) {
			{ e.from } -> std::convertible_to<vertex_id>;
		};

		template<typename E>
		concept traits_weight = requires(E const &e) { graph_traits<E>::weight(e); };

		template<typename E>
		concept member_weight = requires(E const &e) { e.weight; };

	}

	template<typename E>
		requires(std::integral<E> || detail::traits_target<E> || detail::member_target<E>)
	constexpr vertex_id target(E const &e) {
		if constexpr (std::integral<E>)
			return vertex_id(e);
		else if constexpr (detail::traits_target<E>)
			return graph_traits<E>::target(e);
		else
			return vertex_id(e.to);
	}

	template<typename E>
		requires(detail::traits_source<E> || detail::member_source<E>)
	constexpr vertex_id source(E const &e) {
		if constexpr (detail::traits_source<E>)
			return graph_traits<E>::source(e);
		else
			return vertex_id(e.from);
	}

	template<typename E>
		requires(detail::traits_weight<E> || detail::member_weight<E>)
	constexpr decltype(auto) weight(E const &e) {
		if constexpr (detail::traits_weight<E>)
			return graph_traits<E>::weight(e);
		else
			return (e.weight);
	}

	template<typename E>
	concept adjacency_edge = requires(E const &e) { graph::target(e); };

	template<typename E>
	concept list_edge = requires(E const &e) {
		graph::source(e);
		graph::target(e);
	};

	template<typename E>
	concept weighted_list_edge = list_edge<E> && requires(E const &e) { graph::weight(e); };

	namespace detail {

		template<typename G>
		concept member_graph = requires(G const &g, vertex_id u) {
			{ g.vertex_count() } -> std::convertible_to<std::size_t>;
			{ g.out_edges(u) } -> std::ranges::input_range;
		};

		template<typename G>
		concept range_graph = std::ranges::random_access_range<G> && std::ranges::sized_range<G>
						   && std::ranges::input_range<std::ranges::range_reference_t<G const>>;
	}

	template<typename G>
		requires(detail::member_graph<G> || detail::range_graph<G>)
	constexpr std::size_t vertex_count(G const &g) {
		if constexpr (detail::member_graph<G>)
			return g.vertex_count();
		else
			return std::ranges::size(g);
	}

	template<typename G>
		requires(detail::member_graph<G> || detail::range_graph<G>)
	constexpr decltype(auto) out_edges(G const &g, vertex_id u) {
		if constexpr (detail::member_graph<G>)
			return g.out_edges(u);
		else
			return std::ranges::begin(g)[u];
	}

	template<typename G>
	using out_edges_t = std::remove_cvref_t<decltype(graph::out_edges(std::declval<G const &>(), vertex_id{}))>;

	template<typename G>
	concept adjacency_graph = requires(G const &g, vertex_id u) {
		{ graph::vertex_count(g) } -> std::convertible_to<std::size_t>;
		{ graph::out_edges(g, u) } -> std::ranges::input_range;
		requires adjacency_edge<std::ranges::range_value_t<std::remove_cvref_t<decltype(graph::out_edges(g, u))>>>;
	};

	template<typename EL>
	concept edge_list = std::ranges::input_range<EL> && list_edge<std::ranges::range_value_t<EL>>;

}

#ifdef ATL_ASSERT
#define atl_assert(condition)                                                                             \
	do {                                                                                                  \
		if (!(condition)) {                                                                               \
			std::fprintf(stderr, "atl_assert: (%s), file %s, line %d\n", #condition, __FILE__, __LINE__); \
			std::abort();                                                                                 \
		}                                                                                                 \
	} while (0)
#else
#define atl_assert(condition) (void)0
#endif

namespace atl::graph {

	namespace detail {

		template<adjacency_graph G>
		inline constexpr bool edges_by_ref =
			std::is_lvalue_reference_v<decltype(graph::out_edges(std::declval<G const &>(), vertex_id{}))>;

		template<typename G>
		concept scannable_graph =
			adjacency_graph<G>
			&& ((edges_by_ref<G> && std::ranges::forward_range<out_edges_t<G>>)
				|| (std::ranges::random_access_range<out_edges_t<G>>
					&& std::ranges::sized_range<out_edges_t<G>>));

	}

}

namespace atl::graph {

	struct centroid_pair_vertex {
		vertex_id vertex, parent;
		uint32 depth;
	};

	namespace detail {

		template<bool WantEdges, adjacency_graph G, typename Emit>
		void centroid_bisect_engine(G const &g, Emit &&emit) {
			using Edge = std::ranges::range_value_t<out_edges_t<G>>;
			std::size_t const n = graph::vertex_count(g);
			if (n == 0) return;

			struct frame {
				std::vector<vertex_id> v;
				std::vector<uint32> par;
				std::vector<Edge> pedge;
			};

			std::vector<uint32> sz(n), off(n + 1), kid(n), kcur(n), cpar(n), cdep(n),
				col(n), nmap(n);
			std::vector<Edge> cedge;
			if constexpr (WantEdges) cedge.resize(n);
			std::vector<uint32> order;
			order.reserve(n);
			std::vector<centroid_pair_vertex> comp;
			std::vector<Edge> ecomp;

			std::vector<frame> stack, freelist;
			auto const take_frame = [&]() -> frame {
				if (freelist.empty()) return frame{};
				frame f = std::move(freelist.back());
				freelist.pop_back();
				f.v.clear();
				f.par.clear();
				if constexpr (WantEdges) f.pedge.clear();
				return f;
			};

			{
				std::vector<uint8> seen(n, 0);
				std::vector<uint32> gloc(n);
				for (uint32 s = 0; s < n; ++s) {
					if (seen[s]) continue;
					frame f;
					f.v.push_back(s);
					f.par.push_back(0);
					if constexpr (WantEdges) f.pedge.push_back(Edge{});
					seen[s] = 1;
					gloc[s] = 0;
					for (std::size_t i = 0; i < f.v.size(); ++i)
						for (auto const &e : graph::out_edges(g, f.v[i])) {
							uint32 const w = graph::target(e);
							if (seen[w]) continue;
							seen[w] = 1;
							gloc[w] = uint32(f.v.size());
							f.v.push_back(w);
							f.par.push_back(uint32(i));
							if constexpr (WantEdges) f.pedge.push_back(e);
						}
					stack.push_back(std::move(f));
				}
			}

			while (!stack.empty()) {
				frame fr = std::move(stack.back());
				stack.pop_back();
				std::size_t const m = fr.v.size();
				if (m <= 2) {
					freelist.push_back(std::move(fr));
					continue;
				}
				auto const &par = fr.par;

				for (std::size_t i = 0; i < m; ++i) sz[i] = 1;
				for (std::size_t i = m; i-- > 1;) sz[par[i]] += sz[i];
				uint32 c = 0, cs = uint32(m) + 1;
				for (uint32 i = 0; i < m; ++i)
					if (std::size_t(sz[i]) * 2 >= m && sz[i] < cs) cs = sz[i], c = i;

				for (std::size_t i = 0; i <= m; ++i) off[i] = 0;
				for (std::size_t i = 1; i < m; ++i) ++off[par[i] + 1];
				for (std::size_t i = 0; i < m; ++i) off[i + 1] += off[i];
				for (std::size_t i = 0; i < m; ++i) kcur[i] = off[i];
				for (uint32 i = 1; i < m; ++i) kid[kcur[par[i]]++] = i;

				order.clear();
				cpar[c] = c;
				cdep[c] = 0;
				order.push_back(c);
				for (std::size_t i = 0; i < order.size(); ++i) {
					uint32 const u = order[i];
					for (uint32 k = off[u]; k < off[u + 1]; ++k) {
						uint32 const w = kid[k];
						if (w == cpar[u]) continue;
						cpar[w] = u;
						cdep[w] = cdep[u] + 1;
						if constexpr (WantEdges) cedge[w] = fr.pedge[w];
						order.push_back(w);
					}
					uint32 const p0 = par[u];
					if (u != 0 && p0 != cpar[u]) {
						cpar[p0] = u;
						cdep[p0] = cdep[u] + 1;
						if constexpr (WantEdges) cedge[p0] = fr.pedge[u];
						order.push_back(p0);
					}
				}

				std::size_t const half = (m - 1) / 2;
				uint32 const up = (c == 0) ? uint32(-1) : par[c];
				std::size_t take = 0;
				for (std::size_t i = 1; i < m; ++i) {
					uint32 const u = order[i];
					if (cpar[u] == c) {
						std::size_t const bs = (u == up) ? std::size_t(m) - sz[c] : sz[u];
						if (take + bs <= half)
							col[u] = 0, take += bs;
						else
							col[u] = 1;
					} else
						col[u] = col[cpar[u]];
				}

				comp.clear();
				comp.push_back({fr.v[c], fr.v[c], 0});
				if constexpr (WantEdges) {
					ecomp.clear();
					ecomp.push_back(Edge{});
				}
				frame child[2] = {take_frame(), take_frame()};
				for (auto &ch : child) {
					ch.v.push_back(fr.v[c]);
					ch.par.push_back(0);
					if constexpr (WantEdges) ch.pedge.push_back(Edge{});
				}
				nmap[c] = 0;
				std::size_t n0 = 0;
				for (uint32 pass = 0; pass < 2; ++pass) {
					frame &ch = child[pass];
					for (std::size_t i = 1; i < m; ++i) {
						uint32 const u = order[i];
						if (col[u] != pass) continue;
						comp.push_back({fr.v[u], fr.v[cpar[u]], cdep[u]});
						nmap[u] = uint32(ch.v.size());
						ch.v.push_back(fr.v[u]);
						ch.par.push_back(nmap[cpar[u]]);
						if constexpr (WantEdges) {
							ecomp.push_back(cedge[u]);
							ch.pedge.push_back(cedge[u]);
						}
					}
					if (pass == 0) n0 = comp.size() - 1;
				}
				if constexpr (WantEdges)
					emit(fr.v[c], std::span<centroid_pair_vertex const>(comp), n0,
						 std::span<Edge const>(ecomp));
				else
					emit(fr.v[c], std::span<centroid_pair_vertex const>(comp), n0,
						 std::span<Edge const>{});
				stack.push_back(std::move(child[0]));
				stack.push_back(std::move(child[1]));
				freelist.push_back(std::move(fr));
			}
		}
	}

}

namespace atl::graph::experimental {

	template<adjacency_graph G, typename F>
	void centroid_bisect_split(G const &g, F f) {
		using Edge = std::ranges::range_value_t<out_edges_t<G>>;
		using cpv = centroid_pair_vertex;
		constexpr bool weighted = std::invocable<F &, vertex_id, std::span<cpv const>,
												 std::size_t, std::span<Edge const>>;

		graph::detail::centroid_bisect_engine<weighted>(
			g, [&](vertex_id c, std::span<cpv const> comp, std::size_t n0,
				   std::span<Edge const> ec) {
				if constexpr (weighted)
					f(c, comp, n0, ec);
				else
					f(c, comp, n0);
			});
	}
}

namespace atl::graph {
	namespace detail {

		template<list_edge A>
		struct adjacent_of {
			using type = vertex_id;
		};

		template<weighted_list_edge A>
		struct adjacent_of<A> {
			using type = edge<std::remove_cvref_t<decltype(graph::weight(std::declval<A const &>()))>>;
		};

		template<typename A>
		using adjacent_of_t = typename adjacent_of<A>::type;

		template<adjacency_edge E, list_edge A>
		constexpr E adjacent_from(A const &a) {
			if constexpr (std::integral<E>)
				return E(graph::target(a));
			else
				return E{graph::target(a), graph::weight(a)};
		}
	}

	template<adjacency_edge E>
	class csr {
		std::vector<uint32> offset_;
		std::vector<E> edges_;

		template<bool BothDirections, edge_list R, typename Proj>
			requires std::convertible_to<std::invoke_result_t<Proj const &, std::ranges::range_reference_t<R const>>, E>
		void build(std::size_t n, R const &arcs, Proj const &proj) {
			offset_.assign(n + 1, 0);
			for (auto const &a : arcs) {
				atl_assert(graph::source(a) < n && graph::target(a) < n);
				++offset_[graph::source(a) + 1];
				if constexpr (BothDirections) ++offset_[graph::target(a) + 1];
			}
			std::partial_sum(offset_.begin(), offset_.end(), offset_.begin());
			edges_.resize(offset_.back());
			std::vector<uint32> cursor(offset_.begin(), offset_.end() - 1);
			for (auto const &a : arcs) {
				edges_[cursor[graph::source(a)]++] = proj(a);
				if constexpr (BothDirections) edges_[cursor[graph::target(a)]++] = proj(reversed(a));
			}
		}

		template<typename A>
			requires requires(A a, vertex_id v) { a.from = v; a.to = v; }
		static constexpr A reversed(A a) {
			auto const s = graph::source(a), t = graph::target(a);
			a.from = t;
			a.to = s;
			return a;
		}

		csr() = default;

	public:
		template<edge_list R>
		static csr undirected(std::size_t n, R const &arcs) {
			csr g;
			g.build<true>(n, arcs, [](auto const &a) { return detail::adjacent_from<E>(a); });
			return g;
		}

		std::size_t vertex_count() const noexcept { return offset_.size() - 1; }

		std::span<E const> out_edges(vertex_id u) const {
			atl_assert(u + 1 < offset_.size());
			return {edges_.data() + offset_[u], edges_.data() + offset_[u + 1]};
		}
	};

	template<edge_list R>
	csr(std::size_t, R &&) -> csr<detail::adjacent_of_t<std::ranges::range_value_t<R>>>;

	template<edge_list R>
	auto undirected_csr(std::size_t n, R const &arcs) {
		return csr<detail::adjacent_of_t<std::ranges::range_value_t<R>>>::undirected(n, arcs);
	}
}

namespace atl::io {

	template<typename T>
	concept readable = atl::integral<T> || std::same_as<T, bool> || std::same_as<T, char>
					|| std::same_as<T, std::string>
#if defined(__cpp_lib_to_chars)
					|| std::floating_point<T>
#endif
		;

	class reader {
		static constexpr std::size_t cap = 1 << 18;
		std::FILE *src_;
		char *owned_ = nullptr;
		char const *cur_ = nullptr;
		char const *end_ = nullptr;
		bool mmapped_ = false;

		void refill() {
			if (mmapped_) return;
			std::size_t rem = std::size_t(end_ - cur_);
			if (rem && cur_ != owned_) std::memmove(owned_, cur_, rem);
			std::size_t got = std::fread(owned_ + rem, 1, cap - rem, src_);
			cur_ = owned_;
			end_ = owned_ + rem + got;
		}

		void ensure() {
			if (!mmapped_ && std::size_t(end_ - cur_) < 64) refill();
		}

		void skip_ws() {
			for (;;) {
				while (cur_ != end_ && static_cast<unsigned char>(*cur_) <= ' ') ++cur_;
				if (cur_ != end_ || mmapped_) return;
				refill();
				if (cur_ == end_) return;
			}
		}

		template<atl::unsigned_integral U>
		U parse_uint() {
			U x = 0;
			while (end_ - cur_ >= 8) {
				std::uint64_t v;
				std::memcpy(&v, cur_, 8);
				v ^= 0x3030303030303030ull;
				if (v & 0xf0f0f0f0f0f0f0f0ull) break;
				v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ffull;
				v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffffull;
				v = (v * 10000 + (v >> 32)) & 0x00000000ffffffffull;
				x = x * 100'000'000u + U(v);
				cur_ += 8;
			}
			if (end_ - cur_ >= 4) {
				std::uint32_t v;
				std::memcpy(&v, cur_, 4);
				v ^= 0x30303030u;
				if (!(v & 0xf0f0f0f0u)) {
					v = (v * 10 + (v >> 8)) & 0x00ff00ffu;
					v = (v * 100 + (v >> 16)) & 0x0000ffffu;
					x = x * 10000u + U(v);
					cur_ += 4;
				}
			}
			while (cur_ != end_) {
				unsigned c = static_cast<unsigned char>(*cur_);
				if (c - '0' > 9u) break;
				x = x * 10 + (c - '0');
				++cur_;
			}
			return x;
		}

	public:
		explicit reader(std::FILE *src = stdin) : src_(src) {
			owned_ = new char[cap];
			cur_ = end_ = owned_;
		}

		reader(reader const &) = delete;
		reader &operator=(reader const &) = delete;

		~reader() {
			delete[] owned_;
		}

		template<atl::unsigned_integral U>
		U read_uint() {
			skip_ws();
			ensure();
			return parse_uint<U>();
		}

		template<atl::signed_integral T>
		T read_int() {
			skip_ws();
			ensure();
			bool neg = (cur_ != end_ && *cur_ == '-');
			if (neg) ++cur_;
			using U = atl::traits::make_unsigned_t<T>;
			U v = parse_uint<U>();
			return neg ? T(U(0) - v) : T(v);
		}

		std::string read_token() {
			skip_ws();
			std::string s;
			for (;;) {
				char const *p = cur_;
				while (p != end_ && static_cast<unsigned char>(*p) > ' ') ++p;
				s.append(cur_, p);
				cur_ = p;
				if (p != end_ || mmapped_) break;
				refill();
				if (cur_ == end_) break;
			}
			return s;
		}

#if defined(__cpp_lib_to_chars)

		template<std::floating_point F>
		F read_float() {
			skip_ws();
			ensure();
			char const *p = cur_;
			while (p != end_ && static_cast<unsigned char>(*p) > ' ') ++p;
			F val = 0;
			if (p != end_ || mmapped_) {
				std::from_chars(cur_, p, val);
				cur_ = p;
			} else {
				std::string tok = read_token();
				std::from_chars(tok.data(), tok.data() + tok.size(), val);
			}
			return val;
		}
#endif

		template<readable T>
		T read_one() {
			if constexpr (atl::unsigned_integral<T>)
				return read_uint<T>();
			else if constexpr (atl::signed_integral<T>)
				return read_int<T>();
			else if constexpr (std::is_same_v<T, bool>)
				return read_uint<uint32>() != 0;
			else if constexpr (std::is_same_v<T, char>) {
				skip_ws();
				ensure();
				return cur_ != end_ ? *cur_++ : char(0);
			} else if constexpr (std::is_same_v<T, std::string>)
				return read_token();
#if defined(__cpp_lib_to_chars)
			else if constexpr (std::is_floating_point_v<T>)
				return read_float<T>();
#endif
			else
				;
		}
	};

	inline reader in;

	template<readable T>
	T read() {
		return in.read_one<T>();
	}

	template<readable A, readable B, readable... R>
	std::tuple<A, B, R...> read() {
		std::tuple<A, B, R...> t;
		std::apply([](auto &...es) { ((es = in.read_one<std::remove_cvref_t<decltype(es)>>()), ...); }, t);
		return t;
	}

}

namespace atl::io {
	namespace detail {

		inline constexpr std::array<char, 200> two_digits = [] {
			std::array<char, 200> t{};
			for (int32 i = 0; i < 100; ++i) {
				t[2 * i] = char('0' + i / 10);
				t[2 * i + 1] = char('0' + i % 10);
			}
			return t;
		}();

	}

	class writer {
		static constexpr std::size_t cap = 1 << 18;
		std::FILE *sink_;
		char *buf_;
		char *cur_;
		char *end_;

		template<atl::unsigned_integral U>
		void put_uint(U x) {
			char tmp[44];
			char *p = tmp + sizeof(tmp);

			while (x >= 100) {
				uint32 d = uint32(x % 100) * 2;
				x /= 100;
				*--p = detail::two_digits[d + 1];
				*--p = detail::two_digits[d];
			}
			if (x >= 10) {
				uint32 d = uint32(x) * 2;
				*--p = detail::two_digits[d + 1];
				*--p = detail::two_digits[d];
			} else {
				*--p = char('0' + uint32(x));
			}

			put(std::string_view(p, std::size_t(tmp + sizeof(tmp) - p)));
		}

	public:
		explicit writer(std::FILE *sink = stdout) : sink_(sink), buf_(new char[cap]), cur_(buf_), end_(buf_ + cap) {}

		writer(writer const &) = delete;
		writer &operator=(writer const &) = delete;

		~writer() {
			flush();
			delete[] buf_;
		}

		void flush() {
			if (cur_ != buf_) {
				std::fwrite(buf_, 1, std::size_t(cur_ - buf_), sink_);
				cur_ = buf_;
			}
		}

		void put(char c) {
			if (cur_ == end_) flush();
			*cur_++ = c;
		}

		void put(bool b) { put(char('0' + b)); }

		void put(std::string_view s) {
			if (s.size() >= cap) {
				flush();
				std::fwrite(s.data(), 1, s.size(), sink_);
				return;
			}
			if (std::size_t(end_ - cur_) < s.size()) flush();
			std::memcpy(cur_, s.data(), s.size());
			cur_ += s.size();
		}

		void put(char const *s) { put(std::string_view(s)); }

		void put(std::string const &s) { put(std::string_view(s)); }

		template<atl::unsigned_integral T>
		void put(T x) {
			put_uint(x);
		}

		template<atl::signed_integral T>
		void put(T x) {
			using U = atl::traits::make_unsigned_t<T>;
			if (x < 0) {
				put('-');
				put_uint(U(U(0) - U(x)));
			} else {
				put_uint(U(x));
			}
		}

#if defined(__cpp_lib_to_chars)

		template<std::floating_point F>
		void put(F x) {
			char tmp[32];
			auto res = std::to_chars(tmp, tmp + sizeof(tmp), x);
			put(std::string_view(tmp, std::size_t(res.ptr - tmp)));
		}

#endif
	};

	template<typename T>
	concept writable = requires(writer &w, T const &x) { w.put(x); };

	inline writer out;

	inline void write() {}

	template<writable T, writable... Ts>
	void write(T const &x, Ts const &...xs) {
		out.put(x);
		((out.put(' '), out.put(xs)), ...);
	}

	template<writable... Ts>
	void writeln(Ts const &...xs) {
		write(xs...);
		out.put('\n');
	}

#if defined(__cpp_lib_to_chars)

#endif

}

namespace atl::traits {

	template<typename T>
	struct widen_mul_type {
		using type = T;
	};

	template<typename T>
	using widen_mul_type_t = typename widen_mul_type<T>::type;

#if ATL_HAS_INT128

#endif

	template<>
	struct widen_mul_type<uint32> {
		using type = uint64;
	};

#if ATL_HAS_INT128
	template<>
	struct widen_mul_type<uint64> {
		using type = uint128;
	};
#endif
}

namespace atl {

	template<unsigned_integral T>
	constexpr inline T add_mod_unchecked(T a, T b, T m) {
		atl_assert(a < m);
		atl_assert(b < m);
		b = m - b;
		return (a >= b) ? a - b : (m - b) + a;
	}

	template<unsigned_integral T>
	constexpr inline T mul_mod_binary(T a, T b, T m) {
		atl_assert(a < m);
		atl_assert(b < m);
		T result = 0;
		while (b > 0) {
			if (b & 1) result = add_mod_unchecked(result, a, m);
			a = add_mod_unchecked(a, a, m);
			b >>= 1;
		}
		return result;
	}

#if ATL_HAS_INT128

#endif

	template<integral I, unsigned_integral T>
	constexpr inline T reduce_mod(I a, T m) {
		if (a >= 0) {
			return static_cast<traits::make_unsigned_t<I>>(a) < m ? a : a % m;
		} else {
			a++;
			a = -a;
			T r = a % m;
			r = m - r - 1;
			return r;
		}
	}

	template<auto Mod>
		requires unsigned_integral<decltype(Mod)>
	struct static_modulus {
		using value_type = decltype(Mod);

		static constexpr value_type mod = Mod;

		static constexpr bool mersenne = Mod > 1 && (value_type(Mod + 1) & Mod) == 0;

		static constexpr value_type mul(value_type a, value_type b) {
			using wide = traits::widen_mul_type_t<value_type>;
			if constexpr (sizeof(wide) > sizeof(value_type)) {
				return reduce(wide(a) * b);
			} else {
				return mul_mod_binary<value_type>(a, b, mod);
			}
		}

		static constexpr value_type to_form(value_type a) { return a; }

		static constexpr value_type from_form(value_type a) { return a; }

	private:
		static constexpr int mersenne_bits = mersenne ? std::bit_width(Mod) : 0;

		template<unsigned_integral Wide>
		static constexpr value_type reduce(Wide c) {
			if constexpr (mersenne) {
				value_type const lo = value_type(c) & mod, hi = value_type(c >> mersenne_bits);
				value_type const r = lo + hi;
				return r >= mod ? r - mod : r;
			} else {
				return value_type(c % mod);
			}
		}
	};

	template<auto Mod>
		requires unsigned_integral<decltype(Mod)>
	struct montgomery {
		using value_type = decltype(Mod);

		static constexpr value_type mod = Mod;

	private:
		using wide = traits::widen_mul_type_t<value_type>;

		static constexpr int bits = int(sizeof(value_type)) * 8;

		static constexpr value_type mod_inv = [] {
			value_type inv = 1;
			for (int i = 0; i < 6; ++i) inv *= value_type(2) - mod * inv;
			return inv;
		}();

		static constexpr value_type r_squared = [] {
			value_type const r = value_type((wide(1) << bits) % mod);
			return value_type((wide(r) * r) % mod);
		}();

		static constexpr value_type reduce(wide x) {
			value_type const q = value_type(x) * mod_inv;
			value_type const hi = value_type((wide(q) * mod) >> bits);
			value_type const xhi = value_type(x >> bits);
			return xhi >= hi ? value_type(xhi - hi) : value_type(xhi + mod - hi);
		}

	public:
		static constexpr value_type to_form(value_type a) { return mul(a, r_squared); }

		static constexpr value_type from_form(value_type a) { return reduce(wide(a)); }

		static constexpr value_type one() { return to_form(1); }

		static constexpr value_type mul(value_type a, value_type b) { return reduce(wide(a) * b); }

		static constexpr value_type reduce_lazy(wide x)
			requires(Mod <= value_type(-1) / 4)
		{
			value_type const q = value_type(x) * mod_inv;
			value_type const hi = value_type((wide(q) * mod) >> bits);
			return value_type(value_type(x >> bits) + mod - hi);
		}

		static constexpr value_type mul_lazy(value_type a, value_type b)
			requires(Mod <= value_type(-1) / 4)
		{
			return reduce_lazy(wide(a) * b);
		}

		static constexpr value_type companion(value_type w) { return w * mod_inv; }

		static constexpr value_type mul_lazy_fixed(value_type x, value_type w, value_type wq)
			requires(Mod <= value_type(-1) / 4)
		{
			value_type const q = x * wq;
			value_type const hi = value_type((wide(q) * mod) >> bits);
			value_type const xw = value_type((wide(x) * w) >> bits);
			return value_type(xw + mod - hi);
		}
	};

	namespace detail {

		template<auto Mod, bool UseMontgomery>
		struct modulus_engine {
			using type = static_modulus<Mod>;
		};

		template<auto Mod>
		struct modulus_engine<Mod, true> {
			using type = montgomery<Mod>;
		};
	}

	template<auto Mod>
	using modulus_engine =
		typename detail::modulus_engine<Mod, (sizeof(decltype(Mod)) == 8) && !static_modulus<Mod>::mersenne
												 && (Mod & 1)>::type;

	template<typename Base, unsigned_integral Exponent>
		requires requires(Base b) {
			{ b * b } -> std::convertible_to<Base>;
			Base(1);
		}
	constexpr Base power(Base base, Exponent exp) {
		auto res = Base(1);
		while (exp > 0) {
			if (exp & 1)
				res = res * base;
			base = base * base;
			exp >>= 1;
		}
		return res;
	}

	template<unsigned_integral T, T Mod>
	class mod_int {
		T value_;
		using mod_ops = modulus_engine<Mod>;

	public:
		static constexpr inline T mod() { return Mod; }

		constexpr mod_int() : value_(T(0)) {}

		template<integral I>
		constexpr mod_int(I v) {
			value_ = mod_ops::to_form(reduce_mod<I, T>(v, mod()));
		}

		constexpr mod_int &operator*=(mod_int const &rhs) {
			value_ = mod_ops::mul(value_, rhs.value_);
			return *this;
		}

		friend constexpr mod_int operator+(mod_int lhs, mod_int const &rhs) {
			return lhs += rhs;
		}

		friend constexpr mod_int operator-(mod_int lhs, mod_int const &rhs) {
			return lhs -= rhs;
		}

		friend constexpr mod_int operator*(mod_int lhs, mod_int const &rhs) {
			return lhs *= rhs;
		}

		constexpr bool operator==(mod_int const &rhs) const {
			return value_ == rhs.value_;
		}

		constexpr T value() const { return mod_ops::from_form(value_); }
	};

	template<typename Z>
	concept static_mod_int = requires(Z a, Z b) {
		{ Z::mod() } -> unsigned_integral;
		Z(1u);
		{ a * b } -> std::convertible_to<Z>;
		{ a + b } -> std::convertible_to<Z>;
		{ a - b } -> std::convertible_to<Z>;
		{ a == b } -> std::convertible_to<bool>;
		requires std::copyable<Z>;
	};

	namespace detail {

		template<static_mod_int Z>
		constexpr Z primitive_root() {
			using U = decltype(Z::mod());
			U const mod = Z::mod();
			U factors[32] = {};
			int count = 0;
			U m = mod - 1;
			for (U p = 2; p * p <= m; ++p)
				if (m % p == 0) {
					factors[count++] = p;
					while (m % p == 0) m /= p;
				}
			if (m > 1) factors[count++] = m;
			for (U g = 2;; ++g) {
				bool ok = true;
				for (int i = 0; i < count && ok; ++i)
					ok = power(Z(g), (mod - 1) / factors[i]) != Z(1);
				if (ok) return Z(g);
			}
		}
	}

	template<typename E>
	concept ntt_engine = requires(decltype(mod_int<uint32, 998244353>::mod()) *f, uint32 h) {
		{ E::template supports<mod_int<uint32, 998244353>>() } -> std::convertible_to<bool>;
		E::template forward<mod_int<uint32, 998244353>>(f, h);
		E::template inverse<mod_int<uint32, 998244353>>(f, h);
		E::template convolve<mod_int<uint32, 998244353>>(f, f, h);
	};

	template<typename Derived>
	struct ntt_engine_base {
	};

	struct scalar_ntt : ntt_engine_base<scalar_ntt> {
		template<static_mod_int Z>
		struct twiddles {
			using U = decltype(Z::mod());
			static constexpr U mod = Z::mod();

			U rate[30], irate[30];

			twiddles() {
				using mont = montgomery<mod>;
				uint32 const levels = uint32(std::countr_zero(mod - 1));
				Z root[30], iroot[30];
				root[levels] = power(detail::primitive_root<Z>(), (mod - 1) >> levels);
				iroot[levels] = power(root[levels], mod - 2);
				for (uint32 i = levels; i-- > 0;) {
					root[i] = root[i + 1] * root[i + 1];
					iroot[i] = iroot[i + 1] * iroot[i + 1];
				}
				Z prod(1), iprod(1);
				for (uint32 i = 0; i + 2 <= levels; ++i) {
					rate[i] = mont::to_form((root[i + 2] * prod).value());
					irate[i] = mont::to_form((iroot[i + 2] * iprod).value());
					prod *= iroot[i + 2];
					iprod *= root[i + 2];
				}
			}
		};

		template<static_mod_int Z>
		static twiddles<Z> const &tw() {
			static twiddles<Z> const t;
			return t;
		}
	};

#if defined(ATL_NTT_ENGINE)
	using default_ntt_engine = ATL_NTT_ENGINE;
#else
	using default_ntt_engine = scalar_ntt;
#endif

	template<typename R>
	concept integral_range = std::ranges::random_access_range<R> && std::ranges::sized_range<R>
						  && integral<std::ranges::range_value_t<R>>;

	namespace detail {

		template<ntt_engine Engine, static_mod_int Z, typename GetA, typename GetB>
		std::vector<decltype(Z::mod())> raw_convolution(std::size_t n, GetA const &at_a,
														std::size_t m, GetB const &at_b) {
			using U = decltype(Z::mod());
			constexpr U mod = Z::mod();
			using mont = montgomery<mod>;

			using Eng = std::conditional_t<Engine::template supports<Z>(), Engine, scalar_ntt>;
			std::size_t const need = n + m - 1;

			std::size_t const shortest = std::min(n, m);
			std::size_t const L = std::bit_ceil(2 * shortest);
			bool const lopsided = L * 4 <= std::bit_ceil(need);
			std::size_t const size = lopsided ? L : std::bit_ceil(need);
			atl_assert((mod - 1) % size == 0);
			uint32 const h = uint32(std::countr_zero(size));

			auto const load_short = [&](std::vector<U> &f) {
				for (std::size_t i = 0; i < shortest; ++i) f[i] = n <= m ? at_a(i) : at_b(i);
			};
			auto const at_long = [&](std::size_t i) { return n <= m ? at_b(i) : at_a(i); };
			std::size_t const longest = std::max(n, m);

			if (!lopsided) {
				std::vector<U> fa(size, 0), fb(size, 0);
				for (std::size_t i = 0; i < longest; ++i) fa[i] = at_long(i);
				load_short(fb);
				Eng::template convolve<Z>(fa.data(), fb.data(), h);
				fa.resize(need);
				return fa;
			}

			U const rr = mont::to_form(mont::to_form(U(1)));
			U const scale = mont::mul(mont::to_form(power(Z(U(size)), mod - 2).value()), rr);
			std::size_t const step = L - shortest + 1;
			std::vector<U> fs(L, 0), chunk(L), acc(need + L, 0);
			load_short(fs);
			Eng::template forward<Z>(fs.data(), h);
			for (std::size_t at = 0; at < longest; at += step) {
				std::size_t const len = std::min(step, longest - at);
				for (std::size_t i = 0; i < len; ++i) chunk[i] = at_long(at + i);
				std::fill(chunk.begin() + len, chunk.end(), U(0));
				Eng::template forward<Z>(chunk.data(), h);
				for (std::size_t i = 0; i < L; ++i) chunk[i] = mont::mul_lazy(chunk[i], fs[i]);
				Eng::template inverse<Z>(chunk.data(), h);
				for (std::size_t i = 0; i < L; ++i) {
					U x = mont::mul_lazy(chunk[i], scale);
					if (x >= mod) x -= mod;
					U const sum = acc[at + i] + x;
					acc[at + i] = sum >= mod ? sum - mod : sum;
				}
			}
			acc.resize(need);
			return acc;
		}

		inline constexpr uint32 crt_primes[3] = {167772161, 469762049, 998244353};

	}

	template<ntt_engine Engine = default_ntt_engine, integral_range A, integral_range B>
	std::vector<uint64> convolution_uint64(A const &a, B const &b) {
		std::size_t const n = std::ranges::size(a), m = std::ranges::size(b);
		if (n == 0 || m == 0) return {};
		std::size_t const need = n + m - 1;
		constexpr uint64 p1 = detail::crt_primes[1], p2 = detail::crt_primes[2];
		{
			uint64 top_a = 0, top_b = 0;
			for (std::size_t i = 0; i < n; ++i) top_a = std::max<uint64>(top_a, uint64(a[i]));
			for (std::size_t i = 0; i < m; ++i) top_b = std::max<uint64>(top_b, uint64(b[i]));
			atl_assert(uint128(top_a) * top_b * std::min(n, m) < uint128(p1) * p2);
		}
		if (std::min(n, m) <= 48) {
			std::vector<uint64> c(need, 0);
			for (std::size_t i = 0; i < n; ++i)
				for (std::size_t j = 0; j < m; ++j) c[i + j] += uint64(a[i]) * uint64(b[j]);
			return c;
		}
		auto const run = [&]<uint32 P>(std::integral_constant<uint32, P>) {
			return detail::raw_convolution<Engine, mod_int<uint32, P>>(
				n, [&](std::size_t i) { return uint32(uint64(a[i]) % P); }, m,
				[&](std::size_t i) { return uint32(uint64(b[i]) % P); });
		};
		auto const r1 = run(std::integral_constant<uint32, uint32(p1)>{});
		auto const r2 = run(std::integral_constant<uint32, uint32(p2)>{});
		constexpr uint64 inv_p1_p2 = power(mod_int<uint64, p2>(p1), p2 - 2).value();
		std::vector<uint64> c(need);
		for (std::size_t i = 0; i < need; ++i) {
			uint64 const v1 = r1[i];
			uint64 const v2 = (r2[i] + p2 - v1 % p2) * inv_p1_p2 % p2;
			c[i] = v1 + p1 * v2;
		}
		return c;
	}
}

#if defined(__GNUC__) && !defined(__clang__)
#if defined(__x86_64__) || defined(__i386__) || defined(__AVX2__)
#define ATL_SIMD_AVX2 1
#include <immintrin.h>
#elif defined(__aarch64__) || defined(__ARM_NEON)
#define ATL_SIMD_NEON 1
#include <arm_neon.h>
#endif
#endif

#if defined(ATL_SIMD_AVX2)
#pragma GCC push_options
#pragma GCC target("avx2")
#endif

namespace atl::detail {
	using u32x8 [[gnu::vector_size(32)]] = uint32;

	inline u32x8 load8(uint32 const *p) {
		u32x8 v;
		std::memcpy(&v, p, sizeof(v));
		return v;
	}

	inline void store8(uint32 *p, u32x8 v) { std::memcpy(p, &v, sizeof(v)); }

	inline u32x8 splat8(uint32 x) { return u32x8{} + x; }

	inline u32x8 min8(u32x8 a, u32x8 b) {
		u32x8 const m = (u32x8)(a < b);
		return (a & m) | (b & ~m);
	}

#if defined(ATL_SIMD_AVX2)
	inline u32x8 mulhi8(u32x8 a, u32x8 b) {
		__m256i const A = (__m256i)a, B = (__m256i)b;
		__m256i const lo = _mm256_srli_epi64(_mm256_mul_epu32(A, B), 32);
		__m256i const hi = _mm256_mul_epu32(_mm256_srli_epi64(A, 32), _mm256_srli_epi64(B, 32));

		return (u32x8)_mm256_blend_epi32(lo, hi, 0xAA);
	}
#elif defined(ATL_SIMD_NEON)
	inline u32x8 mulhi8(u32x8 av, u32x8 bv) {
		struct v2 {
			uint32x4_t x, y;
		};

		auto const a = std::bit_cast<v2>(av);
		auto const b = std::bit_cast<v2>(bv);
		uint32x4_t const h0 =
			vuzp2q_u32(vreinterpretq_u32_u64(vmull_u32(vget_low_u32(a.x), vget_low_u32(b.x))),
					   vreinterpretq_u32_u64(vmull_high_u32(a.x, b.x)));
		uint32x4_t const h1 =
			vuzp2q_u32(vreinterpretq_u32_u64(vmull_u32(vget_low_u32(a.y), vget_low_u32(b.y))),
					   vreinterpretq_u32_u64(vmull_high_u32(a.y, b.y)));
		return std::bit_cast<u32x8>(v2{h0, h1});
	}
#else
	inline u32x8 mulhi8(u32x8 a, u32x8 b) {
		u32x8 r;
		for (int i = 0; i < 8; ++i) r[i] = uint32((uint64(a[i]) * b[i]) >> 32);
		return r;
	}
#endif
}

namespace atl {

	struct simd_ntt : ntt_engine_base<simd_ntt> {
		template<static_mod_int Z>
		static constexpr bool supports() {
			using U = decltype(Z::mod());
			return sizeof(U) == 4 && Z::mod() % 2 == 1 && Z::mod() <= (U(1) << 30);
		}

		template<static_mod_int Z>
		static void forward(uint32 *f, uint32 h, uint32 stop = 0) {
			using mont = montgomery<Z::mod()>;
			constexpr uint32 mod = Z::mod();
			auto const &t = scalar_ntt::tw<Z>();
			detail::u32x8 const vmod = detail::splat8(mod), vmod2 = detail::splat8(2 * mod);
			for (uint32 len = 0; len + stop < h; ++len) {
				std::size_t const p = std::size_t(1) << (h - len - 1);
				uint32 rot = mont::one(), rotq = mont::companion(rot);
				for (std::size_t s = 0; s < (std::size_t(1) << len); ++s) {
					uint32 *lo = f + (s << (h - len));
					uint32 *hi = lo + p;
					if (p >= 8) {
						detail::u32x8 const vr = detail::splat8(rot), vrq = detail::splat8(rotq);
						for (std::size_t i = 0; i < p; i += 8) {
							detail::u32x8 const l = detail::load8(lo + i), x = detail::load8(hi + i);
							detail::u32x8 const r =
								detail::mulhi8(x, vr) + vmod - detail::mulhi8(x * vrq, vmod);
							detail::store8(lo + i, detail::min8(l + r, l + r - vmod2));
							detail::store8(hi + i, detail::min8(l - r, l + vmod2 - r));
						}
					} else {
						for (std::size_t i = 0; i < p; ++i) {
							uint32 const l = lo[i], r = mont::mul_lazy_fixed(hi[i], rot, rotq);
							lo[i] = std::min(l + r, l + r - 2 * mod);
							hi[i] = std::min(l - r, l + 2 * mod - r);
						}
					}
					if (s + 1 != (std::size_t(1) << len)) {
						rot = mont::mul(rot, t.rate[std::countr_zero(~s)]);
						rotq = mont::companion(rot);
					}
				}
			}
		}

		template<static_mod_int Z>
		static void inverse(uint32 *f, uint32 h, uint32 stop = 0) {
			using mont = montgomery<Z::mod()>;
			constexpr uint32 mod = Z::mod();
			auto const &t = scalar_ntt::tw<Z>();
			detail::u32x8 const vmod2 = detail::splat8(2 * mod);
			for (uint32 len = h - stop; len-- > 0;) {
				std::size_t const p = std::size_t(1) << (h - len - 1);
				uint32 rot = mont::one(), rotq = mont::companion(rot);
				for (std::size_t s = 0; s < (std::size_t(1) << len); ++s) {
					uint32 *lo = f + (s << (h - len));
					uint32 *hi = lo + p;
					if (p >= 8) {
						detail::u32x8 const vr = detail::splat8(rot), vrq = detail::splat8(rotq),
											vmod = detail::splat8(mod);
						for (std::size_t i = 0; i < p; i += 8) {
							detail::u32x8 const l = detail::load8(lo + i), r = detail::load8(hi + i);
							detail::store8(lo + i, detail::min8(l + r, l + r - vmod2));
							detail::u32x8 const d = l + vmod2 - r;
							detail::store8(hi + i,
										   detail::mulhi8(d, vr) + vmod - detail::mulhi8(d * vrq, vmod));
						}
					} else {
						for (std::size_t i = 0; i < p; ++i) {
							uint32 const l = lo[i], r = hi[i];
							lo[i] = std::min(l + r, l + r - 2 * mod);
							hi[i] = mont::mul_lazy_fixed(l + 2 * mod - r, rot, rotq);
						}
					}
					if (s + 1 != (std::size_t(1) << len)) {
						rot = mont::mul(rot, t.irate[std::countr_zero(~s)]);
						rotq = mont::companion(rot);
					}
				}
			}
		}
	};
}

#if defined(ATL_SIMD_AVX2)
#pragma GCC pop_options
#endif

#if defined(ATL_SIMD_AVX2)
#pragma GCC push_options
#pragma GCC target("avx2")
#endif

namespace atl::detail {

#if defined(ATL_SIMD_AVX2)
	inline u32x8 mule64(u32x8 a, u32x8 b) { return (u32x8)_mm256_mul_epu32((__m256i)a, (__m256i)b); }

	inline u32x8 shr64_32(u32x8 a) { return (u32x8)_mm256_srli_epi64((__m256i)a, 32); }

	inline u32x8 add64(u32x8 a, u32x8 b) { return (u32x8)_mm256_add_epi64((__m256i)a, (__m256i)b); }
#else
	inline u32x8 mule64(u32x8 a, u32x8 b) {
		uint32 A[8], B[8];
		std::memcpy(A, &a, 32);
		std::memcpy(B, &b, 32);
		uint64 R[4];
		for (int i = 0; i < 4; ++i) R[i] = uint64(A[2 * i]) * B[2 * i];
		u32x8 r;
		std::memcpy(&r, R, 32);
		return r;
	}

	inline u32x8 shr64_32(u32x8 a) {
		uint64 A[4];
		std::memcpy(A, &a, 32);
		for (int i = 0; i < 4; ++i) A[i] >>= 32;
		u32x8 r;
		std::memcpy(&r, A, 32);
		return r;
	}

	inline u32x8 add64(u32x8 a, u32x8 b) {
		uint64 A[4], B[4];
		std::memcpy(A, &a, 32);
		std::memcpy(B, &b, 32);
		for (int i = 0; i < 4; ++i) A[i] += B[i];
		u32x8 r;
		std::memcpy(&r, A, 32);
		return r;
	}
#endif
	inline u32x8 loadu8(uint32 const *p) {
		u32x8 v;
		std::memcpy(&v, p, sizeof(v));
		return v;
	}
}

namespace atl {
	struct conv8_ntt : ntt_engine_base<conv8_ntt> {
		template<static_mod_int Z>
		static constexpr bool supports() {
			return simd_ntt::supports<Z>();
		}

		template<static_mod_int Z>
		static void forward(uint32 *f, uint32 h, uint32 stop = 0) {
			simd_ntt::forward<Z>(f, h, stop);
		}

		template<static_mod_int Z>
		static void inverse(uint32 *f, uint32 h, uint32 stop = 0) {
			simd_ntt::inverse<Z>(f, h, stop);
		}

		template<static_mod_int Z>
		static void conv8_block8(uint32 *a, uint32 *b, uint32 w) {
			using mont = montgomery<Z::mod()>;
			constexpr uint32 mod = Z::mod();

			constexpr uint32 minv = [](uint32 m) {
				uint32 x = 1;
				for (int i = 0; i < 5; ++i) x *= 2 - m * x;
				return x;
			}(mod);
			auto const redc = [](uint64 x) -> uint32 {
				uint32 const q = uint32(x) * minv;
				uint32 const hi = uint32((uint64(q) * mod) >> 32);
				return uint32(x >> 32) + mod - hi;
			};
			uint32 B[8];
			alignas(32) uint32 awa[16];
			uint32 const wm = mont::to_form(w);
			for (int i = 0; i < 8; ++i) {
				uint32 x = a[i];
				x = x >= mod ? x - mod : x;
				uint32 y = b[i];
				B[i] = y >= mod ? y - mod : y;
				awa[8 + i] = x;
				uint32 aw = redc(uint64(x) * wm);
				awa[i] = aw >= mod ? aw - mod : aw;
			}
			detail::u32x8 re{}, ro{};
			for (int i = 0; i < 8; ++i) {
				detail::u32x8 const bi = detail::splat8(B[i]);
				detail::u32x8 const win = detail::loadu8(awa + 8 - i);
				re = detail::add64(re, detail::mule64(bi, win));
				ro = detail::add64(ro, detail::mule64(bi, detail::shr64_32(win)));
			}
			uint64 RE[4], RO[4];
			std::memcpy(RE, &re, 32);
			std::memcpy(RO, &ro, 32);
			for (int k = 0; k < 4; ++k) {
				uint32 e = redc(RE[k]), o = redc(RO[k]);
				if (e >= 2 * mod) e -= 2 * mod;
				if (o >= 2 * mod) o -= 2 * mod;
				a[2 * k] = e;
				a[2 * k + 1] = o;
			}
		}

		template<static_mod_int Z>
		static void convolve(uint32 *fa, uint32 *fb, uint32 h) {
			using mont = montgomery<Z::mod()>;
			constexpr uint32 mod = Z::mod();
			std::size_t const size = std::size_t(1) << h;
			simd_ntt::forward<Z>(fa, h, 3);
			simd_ntt::forward<Z>(fb, h, 3);

			std::size_t const nb = size >> 3;
			uint32 const kk = uint32(std::countr_zero(nb));
			uint32 const zn = power(detail::primitive_root<Z>(), (mod - 1) / uint32(nb)).value();
			uint32 brate[32];
			for (uint32 j = 0; j < kk; ++j)
				brate[j] = power(Z(zn), (uint32(1) << (kk - 1 - j)) + (uint32(1) << (kk - j))).value();
			uint32 w = 1;
			for (std::size_t b = 0; b < nb; ++b) {
				conv8_block8<Z>(fa + 8 * b, fb + 8 * b, w);
				if (b + 1 < nb) w = mont::from_form(mont::mul(mont::to_form(w), mont::to_form(brate[std::countr_zero(~b)])));
			}

			simd_ntt::inverse<Z>(fa, h, 3);

			uint32 const rr = mont::to_form(mont::to_form(uint32(1)));
			uint32 const scale =
				mont::mul(mont::to_form(power(Z(uint32(nb)), mod - 2).value()), rr);
			for (std::size_t i = 0; i < size; ++i) {
				uint32 const x = mont::mul_lazy(fa[i], scale);
				fa[i] = x >= mod ? x - mod : x;
			}
		}
	};
}

#if defined(ATL_SIMD_AVX2)
#pragma GCC pop_options
#endif

using namespace atl;
namespace g = atl::graph;
namespace io = atl::io;

int main() {
	auto const n = io::read<uint32>();
	std::vector<g::arc<>> edges;
	edges.reserve(n - 1);
	for (uint32 i = 1; i < n; ++i) {
		auto const [a, b] = io::read<uint32, uint32>();
		edges.push_back({a, b});
	}
	auto const tree = g::undirected_csr(n, edges);

	std::vector<uint64> answer(n, 0);
	if (n >= 2) answer[1] = n - 1;
	std::vector<uint32> ha, hb;
	g::experimental::centroid_bisect_split(tree, [&](g::vertex_id,
													 std::span<g::centroid_pair_vertex const> comp,
													 std::size_t n0) {
		auto const a = comp.subspan(1, n0);
		auto const b = comp.subspan(1 + n0);
		uint32 ma = 0, mb = 0;
		for (auto const &x : a) ma = std::max(ma, x.depth);
		for (auto const &x : b) mb = std::max(mb, x.depth);
		ha.assign(ma + 1, 0);
		hb.assign(mb + 1, 0);
		for (auto const &x : a) ++ha[x.depth];
		for (auto const &x : b) ++hb[x.depth];
		auto const cv = convolution_uint64<conv8_ntt>(ha, hb);
		for (std::size_t d = 0; d < cv.size() && d < n; ++d) answer[d] += cv[d];
	});

	bool first = true;
	for (uint32 d = 1; d < n; ++d) {
		if (!first) io::write(' ');
		io::write(answer[d]);
		first = false;
	}
	io::writeln();
	return 0;
}
