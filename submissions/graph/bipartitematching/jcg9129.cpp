#include <algorithm>
#include <array>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <ranges>
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

	using uint32 = std::uint32_t;

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

}

namespace atl::graph {

	using vertex_id = uint32;

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
	concept adjacency_edge = requires(E const &e) { graph::target(e); };

	template<typename E>
	concept list_edge = requires(E const &e) {
		graph::source(e);
		graph::target(e);
	};

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

	}

}

namespace atl::graph {

	struct bipartite_matching_result {
		std::size_t size = 0;
		std::vector<uint32> left_pair;
		std::vector<uint32> right_pair;
	};

	template<edge_list E>
	bipartite_matching_result bipartite_matching(std::size_t left, std::size_t right,
												 E const &edges) {
		auto const m = std::size_t(std::ranges::distance(edges));
		std::vector<uint32> loff(left + 1, 0), lto(m), roff(right + 1, 0), rto(m);
		for (auto const &e : edges) {
			atl_assert(graph::source(e) < left && graph::target(e) < right);
			++loff[graph::source(e) + 1];
			++roff[graph::target(e) + 1];
		}
		for (std::size_t v = 0; v < left; ++v) loff[v + 1] += loff[v];
		for (std::size_t v = 0; v < right; ++v) roff[v + 1] += roff[v];
		{
			std::vector<uint32> cl(loff.begin(), loff.end() - 1), cr(roff.begin(), roff.end() - 1);
			for (auto const &e : edges) {
				lto[cl[graph::source(e)]++] = graph::target(e);
				rto[cr[graph::target(e)]++] = graph::source(e);
			}
		}

		bipartite_matching_result r{0, std::vector<uint32>(left, none), std::vector<uint32>(right, none)};
		auto &ml = r.left_pair;
		auto &mr = r.right_pair;
		std::size_t const lim = std::min(left, right);
		std::size_t size = 0;

		std::vector<uint32> lv(left), queue;
		queue.reserve(left);
		std::vector<uint32> ring(right + 1);
		std::size_t head = 0, tail = 0, live = 0;
		auto const push = [&](uint32 w) {
			ring[tail] = w;
			if (++tail == ring.size()) tail = 0;
			++live;
		};
		auto const pop = [&] {
			uint32 const w = ring[head];
			if (++head == ring.size()) head = 0;
			--live;
			return w;
		};
		for (uint32 w = 0; w < right; ++w) push(w);

		auto const global_relabel = [&] {
			std::ranges::fill(lv, none);
			queue.clear();
			for (uint32 l = 0; l < left; ++l)
				if (ml[l] == none) {
					lv[l] = 0;
					queue.push_back(l);
				}
			for (std::size_t at = 0; at < queue.size(); ++at) {
				uint32 const l = queue[at];
				for (uint32 k = loff[l]; k < loff[l + 1]; ++k) {
					uint32 const u = mr[lto[k]];
					if (u != none && lv[u] > lv[l] + 2) {
						lv[u] = lv[l] + 2;
						queue.push_back(u);
					}
				}
			}
		};

		std::size_t const period = left + right;
		std::size_t ticks = 0;
		while (live > 0 && size < lim) {
			if (ticks == 0) global_relabel();
			if (++ticks == period) ticks = 0;
			uint32 const w = pop();
			uint32 best = none, best_lv = none;
			for (uint32 k = roff[w]; k < roff[w + 1]; ++k)
				if (lv[rto[k]] < best_lv) {
					best_lv = lv[rto[k]];
					best = rto[k];
				}
			if (best_lv == none) continue;
			uint32 const old = ml[best];
			if (old != none) {
				mr[old] = none;
				push(old);
			} else {
				++size;
			}
			ml[best] = w;
			mr[w] = best;
			lv[best] = best_lv + 2;
		}

		std::vector<uint32> dist(left), it(left), stack, chosen;
		for (;;) {
			std::ranges::fill(dist, none);
			queue.clear();
			for (uint32 l = 0; l < left; ++l)
				if (ml[l] == none) {
					dist[l] = 0;
					queue.push_back(l);
				}
			bool free_right_reachable = false;
			for (std::size_t at = 0; at < queue.size(); ++at) {
				uint32 const l = queue[at];
				for (uint32 k = loff[l]; k < loff[l + 1]; ++k) {
					uint32 const l2 = mr[lto[k]];
					if (l2 == none)
						free_right_reachable = true;
					else if (dist[l2] == none) {
						dist[l2] = dist[l] + 1;
						queue.push_back(l2);
					}
				}
			}
			if (!free_right_reachable) break;
			std::copy(loff.begin(), loff.end() - 1, it.begin());
			for (uint32 root = 0; root < left; ++root) {
				if (ml[root] != none) continue;
				stack.assign(1, root);
				chosen.clear();
				while (!stack.empty()) {
					uint32 const l = stack.back();
					bool advanced = false;
					while (it[l] < loff[l + 1]) {
						uint32 const w = lto[it[l]++];
						uint32 const l2 = mr[w];
						if (l2 == none) {
							chosen.push_back(w);
							for (std::size_t i = stack.size(); i-- > 0;) {
								ml[stack[i]] = chosen[i];
								mr[chosen[i]] = stack[i];
							}
							stack.clear();
							chosen.clear();
							advanced = true;
							break;
						}
						if (dist[l2] == dist[l] + 1) {
							chosen.push_back(w);
							stack.push_back(l2);
							advanced = true;
							break;
						}
					}
					if (!advanced) {
						dist[l] = none;
						stack.pop_back();
						if (!chosen.empty()) chosen.pop_back();
					}
				}
			}
		}
		for (uint32 l = 0; l < left; ++l) r.size += ml[l] != none;
		return r;
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

using namespace atl;
namespace g = atl::graph;
namespace io = atl::io;

int main() {
	auto const [l, r, m] = io::read<uint32, uint32, uint32>();
	std::vector<g::arc<>> edges(m);
	for (auto &[a, b] : edges) {
		auto const [x, y] = io::read<uint32, uint32>();
		a = x;
		b = y;
	}

	auto const match = g::bipartite_matching(l, r, edges);
	io::writeln(match.size);
	for (uint32 v = 0; v < l; ++v)
		if (match.left_pair[v] != g::none) io::writeln(v, match.left_pair[v]);
	return 0;
}
