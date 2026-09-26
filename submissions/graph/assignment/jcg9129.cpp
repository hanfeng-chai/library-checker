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
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
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
	using int64 = std::int64_t;

	using uint8 = std::uint8_t;

	using uint32 = std::uint32_t;

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

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
	concept adjacency_edge = requires(E const &e) { graph::target(e); };

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

	template<typename Sum>
	struct assignment_result {
		Sum cost{};
		std::vector<uint32> row_to_col, col_to_row;
		std::vector<Sum> row_potential, col_potential;
	};

	namespace detail {

		template<typename Sum, typename C>
		assignment_result<Sum> dense_lap(std::size_t rows, std::size_t cols,
										 std::vector<C> const &cost, bool reduce) {
			atl_assert(rows <= cols && cost.size() == rows * cols);
			Sum constexpr far = std::numeric_limits<Sum>::max() / 4;
			assignment_result<Sum> r;
			r.row_to_col.assign(rows, none);
			r.col_to_row.assign(cols, none);
			std::vector<Sum> v(cols, 0);
			auto const at = [&](std::size_t i, std::size_t j) { return Sum(cost[i * cols + j]); };

			if (reduce && rows > 0) {
				std::vector<uint8> once(rows, 0);
				for (std::size_t j = cols; j-- > 0;) {
					std::size_t best = 0;
					for (std::size_t i = 1; i < rows; ++i)
						if (at(i, j) < at(best, j)) best = i;
					v[j] = at(best, j);
					if (r.row_to_col[best] == none) {
						r.row_to_col[best] = uint32(j);
						r.col_to_row[j] = uint32(best);
						once[best] = 1;
					} else {
						once[best] = 0;
					}
				}

				for (std::size_t i = 0; i < rows; ++i)
					if (once[i] && cols > 1) {
						uint32 const j1 = r.row_to_col[i];
						Sum slack = far;
						for (std::size_t j = 0; j < cols; ++j)
							if (j != j1) slack = std::min(slack, at(i, j) - v[j]);
						v[j1] = at(i, j1) - slack;
					}

				for (int pass = 0; pass < 2; ++pass)
					for (uint32 i = 0; i < rows; ++i) {
						if (r.row_to_col[i] != none) continue;
						std::size_t j1 = 0;
						Sum best = at(i, 0) - v[0], second = far;
						for (std::size_t j = 1; j < cols; ++j) {
							Sum const cur = at(i, j) - v[j];
							if (cur < best || (cur == best && r.col_to_row[j1] != none)) {
								second = best;
								best = cur;
								j1 = j;
							} else {
								second = std::min(second, cur);
							}
						}
						if (best < second) v[j1] -= second - best;
						uint32 const loser = r.col_to_row[j1];
						if (loser != none) r.row_to_col[loser] = none;
						r.row_to_col[i] = uint32(j1);
						r.col_to_row[j1] = i;
					}
			}

			std::vector<Sum> dist(cols);
			std::vector<uint32> from_row(cols), order(cols);
			std::ranges::iota(order, uint32(0));
			for (uint32 s = 0; s < uint32(rows); ++s) {
				if (r.row_to_col[s] != none) continue;
				for (std::size_t j = 0; j < cols; ++j) {
					dist[j] = at(s, j) - v[j];
					from_row[j] = s;
				}
				std::size_t scanned = 0, labeled = 0, settled = 0;
				uint32 open = none;
				while (open == none) {
					if (scanned == labeled) {
						settled = scanned;
						Sum best = dist[order[scanned]];
						for (std::size_t k = scanned; k < cols; ++k) {
							uint32 const j = order[k];
							if (dist[j] > best) continue;
							if (dist[j] < best) {
								best = dist[j];
								labeled = scanned;
							}
							std::swap(order[k], order[labeled]);
							++labeled;
						}
						for (std::size_t k = scanned; k < labeled; ++k)
							if (r.col_to_row[order[k]] == none) {
								open = order[k];
								break;
							}
						if (open != none) break;
					}
					uint32 const j1 = order[scanned++];
					uint32 const i2 = r.col_to_row[j1];
					Sum const tight = at(i2, j1) - v[j1];
					for (std::size_t k = labeled; k < cols; ++k) {
						uint32 const j2 = order[k];
						Sum const step = at(i2, j2) - v[j2] - tight;
						if (dist[j1] + step >= dist[j2]) continue;
						dist[j2] = dist[j1] + step;
						from_row[j2] = i2;
						if (step == 0) {
							if (r.col_to_row[j2] == none) {
								open = j2;
								break;
							}
							std::swap(order[k], order[labeled]);
							++labeled;
						}
					}
				}

				for (std::size_t k = 0; k < settled; ++k)
					v[order[k]] += dist[order[k]] - dist[open];
				for (uint32 j = open; j != none;) {
					uint32 const mover = from_row[j];
					r.col_to_row[j] = mover;
					std::swap(r.row_to_col[mover], j);
				}
			}

			r.row_potential.resize(rows);
			for (std::size_t i = 0; i < rows; ++i) {
				uint32 const j = r.row_to_col[i];
				r.row_potential[i] = at(i, j) - v[j];
				r.cost += at(i, j);
			}
			r.col_potential = std::move(v);
			return r;
		}

		template<typename Sum, typename C>
		int monge_kind(std::size_t n, std::vector<C> const &cost) {
			bool monge = true, anti = true;
			for (std::size_t i = 0; i + 1 < n && (monge || anti); ++i)
				for (std::size_t j = 0; j + 1 < n; ++j) {
					Sum const straight = Sum(cost[i * n + j]) + Sum(cost[(i + 1) * n + j + 1]);
					Sum const crossed = Sum(cost[i * n + j + 1]) + Sum(cost[(i + 1) * n + j]);
					monge = monge && straight <= crossed;
					anti = anti && straight >= crossed;
					if (!monge && !anti) return 0;
				}
			return monge ? 1 : anti ? -1
									: 0;
		}

		template<typename Sum, typename C>
		assignment_result<Sum> monge_diagonal(std::size_t n, std::vector<C> const &cost,
											  bool flip) {
			assignment_result<Sum> r;
			r.row_to_col.resize(n);
			r.col_to_row.resize(n);
			r.row_potential.resize(n);
			r.col_potential.resize(n);
			auto const at = [&](std::size_t i, std::size_t j) {
				return Sum(cost[i * n + (flip ? n - 1 - j : j)]);
			};
			Sum acc = 0;
			for (std::size_t i = 0; i < n; ++i) {
				if (i > 0) acc += at(i, i) - at(i, i - 1);
				std::size_t const j = flip ? n - 1 - i : i;
				r.row_to_col[i] = uint32(j);
				r.col_to_row[j] = uint32(i);
				r.col_potential[j] = acc;
				r.row_potential[i] = at(i, i) - acc;
				r.cost += at(i, i);
			}
			return r;
		}
	}

	template<typename M>
	concept cost_matrix = std::ranges::random_access_range<M> && std::ranges::sized_range<M>
					   && std::ranges::random_access_range<std::ranges::range_value_t<M>>;

	namespace detail {

		template<typename Sum, typename M>
		using sum_t = std::conditional_t<
			std::is_void_v<Sum>,
			std::ranges::range_value_t<std::ranges::range_value_t<M>>, Sum>;
	}

	template<typename Sum = void, cost_matrix M>
	auto assignment(M const &costs) {
		using C = std::ranges::range_value_t<std::ranges::range_value_t<M>>;
		using S = detail::sum_t<Sum, M>;
		std::size_t const rows = std::ranges::size(costs);
		std::size_t const cols = rows == 0 ? 0 : std::ranges::size(costs[0]);
		std::vector<C> flat;
		flat.reserve(rows * cols);
		bool const flip = rows > cols;
		if (!flip) {
			for (auto const &row : costs) {
				atl_assert(std::size_t(std::ranges::size(row)) == cols);
				for (auto const &x : row) flat.push_back(x);
			}
			if (rows == cols && rows > 0)
				if (int const kind = detail::monge_kind<S>(rows, flat); kind != 0)
					return detail::monge_diagonal<S>(rows, flat, kind < 0);
			return detail::dense_lap<S>(rows, cols, flat, rows == cols);
		}
		flat.resize(rows * cols);
		for (std::size_t i = 0; i < rows; ++i) {
			atl_assert(std::size_t(std::ranges::size(costs[i])) == cols);
			for (std::size_t j = 0; j < cols; ++j) flat[j * rows + i] = costs[i][j];
		}
		auto r = detail::dense_lap<S>(cols, rows, flat, false);
		std::swap(r.row_to_col, r.col_to_row);
		std::swap(r.row_potential, r.col_potential);
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

	template<readable T>
	T read() {
		return in.read_one<T>();
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
	auto const n = io::read<uint32>();
	std::vector<std::vector<int64>> cost(n, std::vector<int64>(n));
	for (auto &row : cost)
		for (auto &x : row) x = io::read<int64>();

	auto const r = g::assignment(cost);
	io::writeln(r.cost);
	bool first = true;
	for (uint32 i = 0; i < n; ++i) {
		if (!first) io::write(' ');
		io::write(r.row_to_col[i]);
		first = false;
	}
	io::writeln();
	return 0;
}
