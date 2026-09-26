#include <array>
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

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

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

	inline constexpr uint32 none = uint32(-1);

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

		template<scannable_graph G>
		struct scan_position {
			struct by_iterator {
			};

			using type = std::conditional_t<edges_by_ref<G>, by_iterator, uint32>;
		};

		template<scannable_graph G>
		using scan_position_t = typename scan_position<G>::type;

		template<scannable_graph G>
		struct edge_scan {
			using position = scan_position_t<G>;

			[[no_unique_address]] std::conditional_t<edges_by_ref<G>, out_edges_t<G> const *, out_edges_t<G>>
				edges{};
			[[no_unique_address]] std::conditional_t<edges_by_ref<G>,
													 std::ranges::sentinel_t<out_edges_t<G> const>, uint32>
				last{};
			position at{};

			edge_scan() = default;

			edge_scan(G const &g, vertex_id u, position resume) : at(resume) {
				if constexpr (edges_by_ref<G>) {
					edges = &graph::out_edges(g, u);
					last = std::ranges::end(*edges);
				} else {
					edges = graph::out_edges(g, u);
					last = uint32(std::ranges::size(edges));
				}
			}

			static position initial(G const &g, vertex_id u) {
				if constexpr (edges_by_ref<G>)
					return {std::ranges::begin(graph::out_edges(g, u)), 0};
				else
					return {};
			}

			bool exhausted() const {
				if constexpr (edges_by_ref<G>)
					return at.it == last;
				else
					return at == last;
			}

			decltype(auto) current() const {
				if constexpr (edges_by_ref<G>)
					return *at.it;
				else
					return std::ranges::begin(edges)[at];
			}

			void advance() {
				if constexpr (edges_by_ref<G>) {
					++at.it;
					++at.k;
				} else {
					++at;
				}
			}

			position const &pos() const { return at; }
		};

	}

}

namespace atl::graph {

	struct block_decomposition {
		std::vector<uint32> block;

		std::vector<vertex_id> tops;

		std::size_t count() const { return tops.size(); }

		std::vector<std::vector<vertex_id>> groups() const {
			std::vector<std::vector<vertex_id>> out(tops.size());
			std::vector<uint32> sizes(tops.size(), 1);
			for (auto const b : block)
				if (b != none) ++sizes[b];
			for (std::size_t b = 0; b < tops.size(); ++b) {
				out[b].reserve(sizes[b]);
				out[b].push_back(tops[b]);
			}
			for (vertex_id v = 0; v < block.size(); ++v)
				if (block[v] != none) out[block[v]].push_back(v);
			return out;
		}
	};

	template<detail::scannable_graph G>
	block_decomposition biconnected_components(G const &g) {
		auto const n = uint32(graph::vertex_count(g));
		block_decomposition r{std::vector<uint32>(n, none), {}};
		std::vector<uint32> disc(n, none);
		std::vector<vertex_id> open;

		struct boundary {
			vertex_id v;
			uint32 pdisc;
		};

		std::vector<boundary> bounds;
		using scan = detail::edge_scan<G>;

		struct frame {
			vertex_id v, parent;
			[[no_unique_address]] typename scan::position at;
			uint8 skipped;
		};

		std::vector<frame> frames;
		uint32 timer = 0;
		for (vertex_id s = 0; s < n; ++s) {
			if (disc[s] != none) continue;
			disc[s] = timer++;
			frames.push_back({s, none, scan::initial(g, s), 0});
			while (!frames.empty()) {
				frame &f = frames.back();
				auto es = scan(g, f.v, f.at);
				bool descended = false;
				while (!es.exhausted()) {
					vertex_id const w = graph::target(es.current());
					es.advance();
					if (w == f.parent && !f.skipped) {
						f.skipped = 1;
						continue;
					}
					if (disc[w] == none) {
						f.at = es.pos();
						disc[w] = timer++;
						open.push_back(w);
						bounds.push_back({w, disc[f.v]});
						frames.push_back({w, f.v, scan::initial(g, w), 0});
						descended = true;
						break;
					}
					while (!bounds.empty() && bounds.back().pdisc > disc[w]) bounds.pop_back();
				}
				if (descended) continue;
				vertex_id const v = frames.back().v;
				frames.pop_back();
				if (frames.empty()) continue;
				if (!bounds.empty() && bounds.back().v == v) {
					bounds.pop_back();
					auto const id = uint32(r.tops.size());
					r.tops.push_back(frames.back().v);
					vertex_id w;
					do {
						w = open.back();
						open.pop_back();
						r.block[w] = id;
					} while (w != v);
				}
			}
		}
		return r;
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

		bool eof() {
			skip_ws();
			return cur_ == end_;
		}

		template<readable T>
		bool try_read(T &x) {
			if (eof()) return false;
			x = read_one<T>();
			return true;
		}
	};

	inline reader in;

	template<readable A, readable B, readable... R>
	std::tuple<A, B, R...> read() {
		std::tuple<A, B, R...> t;
		std::apply([](auto &...es) { ((es = in.read_one<std::remove_cvref_t<decltype(es)>>()), ...); }, t);
		return t;
	}

	template<readable... Ts>
	bool read(Ts &...xs) {
		return (in.try_read(xs) && ...);
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

namespace g = atl::graph;
namespace io = atl::io;

int main() {
	auto const [n, m] = io::read<std::size_t, std::size_t>();
	std::vector<g::arc<>> edges(m);
	for (auto &[a, b] : edges) io::read(a, b);

	auto const net = g::undirected_csr(n, edges);
	auto const r = g::biconnected_components(net);

	std::vector<atl::uint8> grouped(n, 0);
	for (g::vertex_id v = 0; v < n; ++v) grouped[v] = r.block[v] != g::none;
	for (auto const t : r.tops) grouped[t] = 1;
	std::size_t isolated = 0;
	for (auto const in : grouped) isolated += !in;

	io::writeln(r.count() + isolated);
	for (auto const &grp : r.groups()) {
		io::write(grp.size());
		for (auto const v : grp) io::write("", v);
		io::writeln();
	}
	for (g::vertex_id v = 0; v < n; ++v)
		if (!grouped[v]) io::writeln(1, v);
}
