#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <limits>
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
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

	using float64 = double;

}

namespace atl::traits {

	template<typename T>
	struct widen_mul_type {
		using type = T;
	};

	template<typename T>
	using widen_mul_type_t = typename widen_mul_type<T>::type;

	template<>
	struct widen_mul_type<int32> {
		using type = int64;
	};

#if ATL_HAS_INT128
	template<>
	struct widen_mul_type<int64> {
		using type = int128;
	};
#endif

#if ATL_HAS_INT128

#endif
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

namespace atl::geometry {

	template<typename T>
	concept coordinate =
		(std::signed_integral<T> && sizeof(T) <= 8) || std::same_as<T, float> || std::same_as<T, double>;

	namespace detail {

		template<std::signed_integral T>
		using normalized_int = std::conditional_t<
			sizeof(T) == 1, int8,
			std::conditional_t<sizeof(T) == 2, int16, std::conditional_t<sizeof(T) == 4, int32, int64>>>;

		template<coordinate T>
		struct product_type {
			using type = traits::widen_mul_type_t<normalized_int<T>>;
		};

	}

	template<coordinate T>
	using product_t = typename detail::product_type<T>::type;

	template<coordinate T>
	using product2_t =
		std::conditional_t<std::floating_point<T>, float64, traits::widen_mul_type_t<product_t<T>>>;

	template<coordinate T>
	struct point {
		using coordinate_type = T;

		T x{}, y{};

		friend constexpr auto operator<=>(point const &, point const &) = default;
	};

	namespace detail {

		template<typename P>
		struct is_point : std::false_type {};

		template<coordinate T>
		struct is_point<point<T>> : std::true_type {};
	}

	template<typename R>
	concept vertex_range =
		std::ranges::random_access_range<R> && std::ranges::sized_range<R>
		&& detail::is_point<std::remove_cvref_t<std::ranges::range_value_t<R>>>::value;

	using vertex_index = uint32;

	template<vertex_range R>
	using vertex_coordinate_t =
		typename std::remove_cvref_t<std::ranges::range_value_t<R>>::coordinate_type;

	namespace detail {

		template<typename V>
			requires std::constructible_from<V, int> && std::totally_ordered<V>
		constexpr int sgn(V const v) {
			return int(v > V(0)) - int(v < V(0));
		}

		template<coordinate T, std::same_as<point<T>>... Pts>
		constexpr void check_difference_bounds([[maybe_unused]] point<T> const p,
											   [[maybe_unused]] Pts const... pts) {
			if constexpr (std::signed_integral<T> && sizeof(T) == 8) {
				[[maybe_unused]] constexpr T bound = T(1) << 62;
				[[maybe_unused]] auto const ok = [&](point<T> const q) {
					return -bound < q.x && q.x < bound && -bound < q.y && q.y < bound;
				};
				atl_assert(ok(p) && (ok(pts) && ...));
			}
		}
	}

	template<coordinate T>
	constexpr product_t<T> cross(point<T> const a, point<T> const b) {
		using P = product_t<T>;
		if constexpr (std::signed_integral<T>) {
			if constexpr (sizeof(T) == 8) {
				[[maybe_unused]] constexpr T min = std::numeric_limits<T>::min();
				atl_assert(a.x != min || a.y != min || b.x != min || b.y != min);
			}
		}
		return P(a.x) * P(b.y) - P(a.y) * P(b.x);
	}

	template<coordinate T>
	constexpr product2_t<T> cross(point<T> const o, point<T> const a, point<T> const b) {
		if constexpr (std::floating_point<T>) {
			using F = float64;
			return (F(a.x) - F(o.x)) * (F(b.y) - F(o.y)) - (F(a.y) - F(o.y)) * (F(b.x) - F(o.x));
		} else {
			using D = product_t<T>;
			using P = product2_t<T>;

			detail::check_difference_bounds(o, a, b);
			D const acx = D(a.x) - D(o.x), acy = D(a.y) - D(o.y);
			D const bcx = D(b.x) - D(o.x), bcy = D(b.y) - D(o.y);
			return P(acx) * P(bcy) - P(acy) * P(bcx);
		}
	}

	template<coordinate T>
	constexpr product2_t<T> squared_distance(point<T> const a, point<T> const b) {
		if constexpr (std::floating_point<T>) {
			using F = float64;
			F const dx = F(a.x) - F(b.x), dy = F(a.y) - F(b.y);
			return dx * dx + dy * dy;
		} else {
			using D = product_t<T>;
			using P = product2_t<T>;

			detail::check_difference_bounds(a, b);
			D const dx = D(a.x) - D(b.x), dy = D(a.y) - D(b.y);
			return P(dx) * P(dx) + P(dy) * P(dy);
		}
	}

}

namespace atl::geometry {
	namespace detail {

		constexpr float64 two_sum(float64 const a, float64 const b, float64 &err) {
			float64 const s = a + b;
			float64 const bv = s - a;
			float64 const av = s - bv;
			err = (a - av) + (b - bv);
			return s;
		}

		constexpr float64 two_diff_tail(float64 const a, float64 const b, float64 const s) {
			float64 const bv = a - s;
			float64 const av = s + bv;
			return (a - av) + (bv - b);
		}

		constexpr float64 two_diff(float64 const a, float64 const b, float64 &err) {
			float64 const s = a - b;
			err = two_diff_tail(a, b, s);
			return s;
		}

		constexpr float64 two_product(float64 const a, float64 const b, float64 &err) {
			float64 const p = a * b;
			if consteval {
				constexpr float64 splitter = 0x1p27 + 1.0;
				auto const split = [](float64 const v, float64 &lo) {
					float64 const c = splitter * v;
					float64 const hi = c - (c - v);
					lo = v - hi;
					return hi;
				};
				float64 alo = 0, blo = 0;
				float64 const ahi = split(a, alo);
				float64 const bhi = split(b, blo);
				err = ((ahi * bhi - p) + ahi * blo + alo * bhi) + alo * blo;
			} else {
				err = std::fma(a, b, -p);
			}
			return p;
		}

		constexpr void two_one_diff(float64 const a1, float64 const a0, float64 const b, float64 &x2,
									float64 &x1, float64 &x0) {
			float64 mid = 0;
			mid = two_diff(a0, b, x0);
			x2 = two_sum(a1, mid, x1);
		}

		constexpr void two_two_diff(float64 const a1, float64 const a0, float64 const b1,
									float64 const b0, float64 &x3, float64 &x2, float64 &x1,
									float64 &x0) {
			float64 high = 0, mid = 0;
			two_one_diff(a1, a0, b0, high, mid, x0);
			two_one_diff(high, mid, b1, x3, x2, x1);
		}

		constexpr int expansion_sum(int const elen, float64 const *e, int const flen,
									float64 const *f, float64 *h) {
			int ei = 0, fi = 0, hn = 0;

			auto const take_e = [&] {
				return ei < elen && (fi >= flen || ((f[fi] > e[ei]) == (f[fi] > -e[ei])));
			};
			float64 q = take_e() ? e[ei++] : f[fi++];
			while (ei < elen || fi < flen) {
				float64 const next = take_e() ? e[ei++] : f[fi++];
				float64 err = 0;
				q = two_sum(q, next, err);
				if (err != 0) h[hn++] = err;
			}
			if (q != 0 || hn == 0) h[hn++] = q;
			return hn;
		}

		inline constexpr float64 epsilon = 0x1p-53;
		inline constexpr float64 orient_bound_a = (3.0 + 16.0 * epsilon) * epsilon;
		inline constexpr float64 orient_bound_b = (2.0 + 12.0 * epsilon) * epsilon;
		inline constexpr float64 orient_bound_c = (9.0 + 64.0 * epsilon) * epsilon * epsilon;
		inline constexpr float64 result_bound = (3.0 + 8.0 * epsilon) * epsilon;

		constexpr int cross2_adapt(point<float64> const a, point<float64> const b,
								   point<float64> const c, point<float64> const d,
								   float64 const detsum) {
			float64 const acx = b.x - a.x, bcx = d.x - c.x;
			float64 const acy = b.y - a.y, bcy = d.y - c.y;

			float64 left_err = 0, right_err = 0;
			float64 const left = two_product(acx, bcy, left_err);
			float64 const right = two_product(acy, bcx, right_err);
			float64 bexp[4] = {};
			two_two_diff(left, left_err, right, right_err, bexp[3], bexp[2], bexp[1], bexp[0]);
			float64 det = bexp[0] + bexp[1] + bexp[2] + bexp[3];
			float64 bound = orient_bound_b * detsum;
			if (det >= bound || -det >= bound) return sgn(det);

			float64 const acx_tail = two_diff_tail(b.x, a.x, acx);
			float64 const bcx_tail = two_diff_tail(d.x, c.x, bcx);
			float64 const acy_tail = two_diff_tail(b.y, a.y, acy);
			float64 const bcy_tail = two_diff_tail(d.y, c.y, bcy);
			if (acx_tail == 0 && acy_tail == 0 && bcx_tail == 0 && bcy_tail == 0)
				return sgn(det);

			bound = orient_bound_c * detsum + result_bound * (det >= 0 ? det : -det);
			det += (acx * bcy_tail + bcy * acx_tail) - (acy * bcx_tail + bcx * acy_tail);
			if (det >= bound || -det >= bound) return sgn(det);

			float64 u[4] = {}, c1[8] = {}, c2[12] = {}, dexp[16] = {};
			float64 s1 = 0, s0 = 0, t1 = 0, t0 = 0;

			s1 = two_product(acx_tail, bcy, s0);
			t1 = two_product(acy_tail, bcx, t0);
			two_two_diff(s1, s0, t1, t0, u[3], u[2], u[1], u[0]);
			int const c1n = expansion_sum(4, bexp, 4, u, c1);

			s1 = two_product(acx, bcy_tail, s0);
			t1 = two_product(acy, bcx_tail, t0);
			two_two_diff(s1, s0, t1, t0, u[3], u[2], u[1], u[0]);
			int const c2n = expansion_sum(c1n, c1, 4, u, c2);

			s1 = two_product(acx_tail, bcy_tail, s0);
			t1 = two_product(acy_tail, bcx_tail, t0);
			two_two_diff(s1, s0, t1, t0, u[3], u[2], u[1], u[0]);
			int const dn = expansion_sum(c2n, c2, 4, u, dexp);

			return sgn(dexp[dn - 1]);
		}

		constexpr int cross2_sign(point<float64> const a, point<float64> const b,
								  point<float64> const c, point<float64> const d) {
			float64 const left = (b.x - a.x) * (d.y - c.y);
			float64 const right = (b.y - a.y) * (d.x - c.x);
			float64 const det = left - right;
			float64 detsum = 0;
			if (left > 0) {
				if (right <= 0) return sgn(det);
				detsum = left + right;
			} else if (left < 0) {
				if (right >= 0) return sgn(det);
				detsum = -left - right;
			} else {
				return sgn(det);
			}
			float64 const bound = orient_bound_a * detsum;
			if (det >= bound || -det >= bound) return sgn(det);
			return cross2_adapt(a, b, c, d, detsum);
		}

		constexpr int orient2d(point<float64> const a, point<float64> const b,
							   point<float64> const c) {
			return cross2_sign(c, a, c, b);
		}

		template<std::floating_point T>
		constexpr point<float64> widen(point<T> const p) {
			return {float64(p.x), float64(p.y)};
		}

		template<coordinate T>
		constexpr int sweep_group(point<T> const from, point<T> const to) {
			if (to.y > from.y) return 1;
			if (to.y < from.y) return 3;
			return to.x > from.x ? 0 : 2;
		}

		template<coordinate T, typename P>
		constexpr bool sweep_before(point<T> const a, point<T> const b, point<T> const c,
									point<T> const d, P const p) {
			int const ga = sweep_group(a, b), gc = sweep_group(c, d);
			if (ga != gc) return ga < gc;
			return p.cross_sign(a, b, c, d) > 0;
		}
	}

	struct exact {
		template<coordinate T>
		constexpr int orientation(point<T> const o, point<T> const a, point<T> const b) const {
			if constexpr (std::floating_point<T>)
				return detail::orient2d(detail::widen(a), detail::widen(b), detail::widen(o));
			else
				return detail::sgn(cross(o, a, b));
		}

		template<coordinate T>
		constexpr int cross_sign(point<T> const a, point<T> const b, point<T> const c,
								 point<T> const d) const {
			if constexpr (std::floating_point<T>)
				return detail::cross2_sign(detail::widen(a), detail::widen(b), detail::widen(c),
										   detail::widen(d));
			else {
				using D = product_t<T>;
				using P2 = product2_t<T>;

				detail::check_difference_bounds(a, b, c, d);
				D const abx = D(b.x) - D(a.x), aby = D(b.y) - D(a.y);
				D const cdx = D(d.x) - D(c.x), cdy = D(d.y) - D(c.y);
				return detail::sgn(P2(abx) * P2(cdy) - P2(aby) * P2(cdx));
			}
		}

		template<typename V>
			requires std::constructible_from<V, int> && std::totally_ordered<V>
		constexpr int sign(V const v) const {
			return detail::sgn(v);
		}

		template<coordinate T>
		constexpr bool equal(point<T> const a, point<T> const b) const {
			return a == b;
		}
	};

	template<typename P, typename T>
	concept sign_policy = coordinate<T> && std::copyable<P>
					   && requires(P const p, point<T> const q, product2_t<T> const v) {
							  { p.orientation(q, q, q) } -> std::same_as<int>;
							  { p.cross_sign(q, q, q, q) } -> std::same_as<int>;
							  { p.sign(v) } -> std::same_as<int>;
							  { p.equal(q, q) } -> std::same_as<bool>;
						  };

}

namespace atl::geometry {

	struct keep_collinear_t {
		explicit keep_collinear_t() = default;
	};

	namespace detail {
		template<coordinate T>
		struct hull_entry {
			point<T> p;
			uint32 i;
		};

		template<bool Strict, coordinate T, sign_policy<T> P>
		constexpr std::vector<vertex_index> chain_hull(std::vector<hull_entry<T>> const &d, P const p) {
			uint32 const m = uint32(d.size());
			std::vector<vertex_index> hull;
			if (m <= 2) {
				for (auto const &e : d) hull.push_back(e.i);
				return hull;
			}
			bool collinear = true;
			for (uint32 i = 1; collinear && i + 1 < m; ++i)
				collinear = p.orientation(d[0].p, d[m - 1].p, d[i].p) == 0;
			if (collinear) {
				if constexpr (Strict) return {d[0].i, d[m - 1].i};
				for (auto const &e : d) hull.push_back(e.i);
				return hull;
			}

			auto const build = [&](auto &&order) {
				std::vector<uint32> chain;
				for (uint32 const at : order) {
					while (chain.size() >= 2) {
						int const o = p.orientation(d[chain[chain.size() - 2]].p,
													d[chain.back()].p, d[at].p);
						if (Strict ? o <= 0 : o < 0)
							chain.pop_back();
						else
							break;
					}
					chain.push_back(at);
				}
				chain.pop_back();
				return chain;
			};
			auto const lower = build(std::views::iota(uint32(0), m));
			auto const upper = build(std::views::iota(uint32(0), m) | std::views::reverse);
			hull.reserve(lower.size() + upper.size());
			for (uint32 const at : lower) hull.push_back(d[at].i);
			for (uint32 const at : upper) hull.push_back(d[at].i);
			return hull;
		}

		template<coordinate T, sign_policy<T> P>
		constexpr void prune_octagon(std::vector<hull_entry<T>> &entries, P const p) {
			using K = std::conditional_t<std::floating_point<T>, float64, int64>;
			auto const keys = [](point<T> const q) {
				return std::array<K, 4>{K(q.x), K(q.y), K(q.x) + K(q.y), K(q.x) - K(q.y)};
			};
			std::array<uint32, 8> extreme{};
			extreme.fill(0);
			for (uint32 e = 1; e < uint32(entries.size()); ++e) {
				auto const k = keys(entries[e].p);
				for (std::size_t axis = 0; axis < 4; ++axis) {
					if (k[axis] < keys(entries[extreme[2 * axis]].p)[axis]) extreme[2 * axis] = e;
					if (k[axis] > keys(entries[extreme[2 * axis + 1]].p)[axis])
						extreme[2 * axis + 1] = e;
				}
			}
			std::vector<hull_entry<T>> corners;
			for (uint32 const e : extreme) corners.push_back({entries[e].p, 0});
			std::ranges::sort(corners, {}, &hull_entry<T>::p);
			auto const [dead0, dead1] = std::ranges::unique(
				corners, [&](auto const &a, auto const &b) { return p.equal(a.p, b.p); });
			corners.erase(dead0, dead1);
			for (uint32 k = 0; k < uint32(corners.size()); ++k) corners[k].i = k;
			auto const octagon = chain_hull<true>(corners, p);
			if (octagon.size() < 3) return;
			auto const inside = [&](point<T> const q) {
				for (uint32 e = 0; e < uint32(octagon.size()); ++e) {
					auto const &a = corners[octagon[e]].p;
					auto const &b = corners[octagon[(e + 1) % octagon.size()]].p;
					if (p.orientation(a, b, q) <= 0) return false;
				}
				return true;
			};

			uint32 const stride = std::max<uint32>(1, uint32(entries.size() / 1024));
			uint32 sampled = 0, discarded = 0;
			for (uint32 e = 0; e < uint32(entries.size()); e += stride) {
				++sampled;
				discarded += inside(entries[e].p);
			}
			if (2 * discarded < sampled) return;
			std::erase_if(entries, [&](auto const &e) { return inside(e.p); });
		}

		template<bool Strict, vertex_range R, sign_policy<vertex_coordinate_t<R>> P>
		constexpr std::vector<vertex_index> hull_indices(R &&points, P const p, bool const prune) {
			using T = vertex_coordinate_t<R>;
			atl_assert(std::ranges::size(points) < (uint64(1) << 32));
			uint32 const n = uint32(std::ranges::size(points));
			auto const first = std::ranges::begin(points);
			std::vector<hull_entry<T>> entries;
			entries.reserve(n);
			for (uint32 i = 0; i < n; ++i) entries.push_back({first[i], i});
			if (prune && n >= 256) prune_octagon(entries, p);

			std::ranges::sort(entries, [](auto const &a, auto const &b) {
				return a.p != b.p ? a.p < b.p : a.i < b.i;
			});
			auto const [dead0, dead1] = std::ranges::unique(
				entries, [&](auto const &a, auto const &b) { return p.equal(a.p, b.p); });
			entries.erase(dead0, dead1);
			return chain_hull<Strict>(entries, p);
		}
	}

	template<vertex_range R, sign_policy<vertex_coordinate_t<R>> P = exact>
	constexpr std::vector<vertex_index> convex_hull_indices(R &&points, P const p = {}) {
		return detail::hull_indices<true>(points, p, true);
	}

	template<vertex_range R, sign_policy<vertex_coordinate_t<R>> P = exact>
	constexpr std::vector<vertex_index> convex_hull_indices(R &&points, keep_collinear_t,
															P const p = {}) {
		return detail::hull_indices<false>(points, p, true);
	}

}

namespace atl::geometry {

	enum class winding : int { clockwise = -1,
							   none = 0,
							   counterclockwise = 1 };

	struct convexity {
		bool convex;
		bool strict;
		winding direction;

		friend constexpr bool operator==(convexity const &, convexity const &) = default;
	};

	template<vertex_range R, sign_policy<vertex_coordinate_t<R>> P = exact>
	constexpr convexity convexity_of(R &&poly, P const p = {}) {
		using T = vertex_coordinate_t<R>;
		atl_assert(std::ranges::size(poly) < (uint64(1) << 32));
		uint32 const n = uint32(std::ranges::size(poly));
		constexpr convexity rejected{false, false, winding::none};
		if (n < 3) return rejected;
		auto const first = std::ranges::begin(poly);
		auto const at = [&](uint32 i) -> point<T> { return first[i >= n ? i - n : i]; };
		uint32 lefts = 0, rights = 0, straights = 0;
		for (uint32 i = 0; i < n; ++i) {
			point<T> const a = at(i), b = at(i + 1), c = at(i + 2);
			if (a == b) return rejected;
			int const turn = p.orientation(a, b, c);
			if (turn > 0)
				++lefts;
			else if (turn < 0)
				++rights;
			else {
				auto const sgn_of = [](auto const lo, auto const hi) {
					return int(hi > lo) - int(hi < lo);
				};
				if (sgn_of(a.x, b.x) != sgn_of(b.x, c.x) || sgn_of(a.y, b.y) != sgn_of(b.y, c.y))
					return rejected;
				++straights;
			}
		}
		if (lefts != 0 && rights != 0) return rejected;
		bool const ccw = lefts != 0;

		uint32 wraps = 0;
		for (uint32 i = 0; i < n; ++i) {
			point<T> const a = at(i), b = at(i + 1), c = at(i + 2);
			wraps += ccw ? detail::sweep_before(b, c, a, b, p) : detail::sweep_before(a, b, b, c, p);
		}
		if (wraps != 1) return rejected;
		return {true, straights == 0, ccw ? winding::counterclockwise : winding::clockwise};
	}

}

namespace atl::geometry {

	struct antipodal_event {
		vertex_index edge_start, edge_end;
		vertex_index opposite;

		friend constexpr bool operator==(antipodal_event const &, antipodal_event const &) = default;

		constexpr std::array<std::array<vertex_index, 2>, 2> antipodal_pairs() const {
			return {{{edge_start, opposite}, {edge_end, opposite}}};
		}
	};

	template<vertex_range R, sign_policy<vertex_coordinate_t<R>> P = exact>
	class antipodal_view {
		using T = vertex_coordinate_t<R>;

		R poly_;
		[[no_unique_address]] P policy_;
		uint32 vertex_count_ = 0;

		constexpr point<T> at(uint32 const i) const { return std::ranges::begin(poly_)[i]; }

		constexpr uint32 next(uint32 const i) const { return i + 1 == vertex_count_ ? 0 : i + 1; }

		constexpr int edge_cross_sign(uint32 const e, uint32 const j) const {
			return policy_.cross_sign(at(e), at(next(e)), at(j), at(next(j)));
		}

	public:
		template<typename Q>
			requires(!std::same_as<std::remove_cvref_t<Q>, antipodal_view>)
		constexpr explicit antipodal_view(Q &&poly, P const policy = {})
			: poly_(std::forward<Q>(poly)), policy_(policy) {
			atl_assert(std::ranges::size(poly_) < (uint64(1) << 32));
			vertex_count_ = uint32(std::ranges::size(poly_));
			atl_assert(vertex_count_ < 3 || [&] {
				auto const c = convexity_of(poly_, policy_);
				return c.convex && c.strict && c.direction == winding::counterclockwise;
			}());
		}

		class iterator {
			antipodal_view const *view_ = nullptr;
			uint32 edge_start_ = 0, opposite_ = 0;
			bool on_tie_event_ = false;

			constexpr void settle() {
				while (view_->edge_cross_sign(edge_start_, opposite_) > 0) opposite_ = view_->next(opposite_);
			}

			friend antipodal_view;

			constexpr iterator(antipodal_view const *view, uint32 const edge_start,
							   uint32 const opposite)
				: view_(view), edge_start_(edge_start), opposite_(opposite) {
				if (view_ != nullptr && edge_start_ < view_->vertex_count_) settle();
			}

		public:
			constexpr iterator() = default;

			constexpr antipodal_event operator*() const {
				return {edge_start_, view_->next(edge_start_), on_tie_event_ ? view_->next(opposite_) : opposite_};
			}

			constexpr iterator &operator++() {
				if (!on_tie_event_ && view_->edge_cross_sign(edge_start_, opposite_) == 0) {
					on_tie_event_ = true;
					return *this;
				}
				on_tie_event_ = false;
				++edge_start_;
				if (edge_start_ < view_->vertex_count_) settle();
				return *this;
			}

			friend constexpr bool operator==(iterator const &a, iterator const &b) {
				return a.edge_start_ == b.edge_start_ && a.on_tie_event_ == b.on_tie_event_;
			}
		};

		constexpr iterator begin() const { return vertex_count_ < 3 ? end() : iterator(this, 0, 1); }

		constexpr iterator end() const { return iterator(this, vertex_count_, 0); }
	};

	template<typename R, typename P = exact>
	antipodal_view(R &&, P = {}) -> antipodal_view<R, P>;

	template<vertex_range R, sign_policy<vertex_coordinate_t<R>> P = exact>
	constexpr std::array<vertex_index, 2> farthest_pair_indices(R &&points, P const p = {}) {
		using T = vertex_coordinate_t<R>;
		atl_assert(std::ranges::size(points) >= 1);
		auto const ids = convex_hull_indices(points, p);
		auto const first = std::ranges::begin(points);
		if (ids.size() == 1) return {ids[0], ids[0]};
		if (ids.size() == 2) return {ids[0], ids[1]};
		std::vector<point<T>> hull;
		hull.reserve(ids.size());
		for (vertex_index const i : ids) hull.push_back(first[i]);
		std::array<vertex_index, 2> best{ids[0], ids[0]};
		product2_t<T> best_distance{};
		for (auto const event : antipodal_view(hull, p))
			for (auto const [i, j] : event.antipodal_pairs()) {
				auto const d = squared_distance(hull[i], hull[j]);
				if (d > best_distance) {
					best_distance = d;
					best = {ids[i], ids[j]};
				}
			}
		return best;
	}

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
		T read() {
			return read_one<T>();
		}

		template<readable A, readable B, readable... R>
		std::tuple<A, B, R...> read() {
			std::tuple<A, B, R...> t;
			std::apply(
				[&](auto &...es) { ((es = read_one<std::remove_cvref_t<decltype(es)>>()), ...); },
				t);
			return t;
		}

		template<readable T>
		bool try_read(T &x) {
			if (eof()) return false;
			x = read_one<T>();
			return true;
		}

		template<readable... Ts>
		bool read(Ts &...xs) {
			return (try_read(xs) && ...);
		}
	};

	inline reader in;

	template<readable T>
	T read() {
		return in.read<T>();
	}

	template<readable A, readable B, readable... R>
	std::tuple<A, B, R...> read() {
		return in.read<A, B, R...>();
	}

}

namespace atl::io {
	namespace detail {

		inline constexpr std::array<char, 200> two_digits = [] {
			std::array<char, 200> t{};
			for (std::size_t i = 0; i < 100; ++i) {
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

		void write() {}

		template<typename T, typename... Ts, typename Self = writer>
			requires requires(Self &w, T const &t, Ts const &...ts) {
				w.put(t);
				(w.put(ts), ...);
			}
		void write(T const &x, Ts const &...xs) {
			put(x);
			((put(' '), put(xs)), ...);
		}

		template<typename... Ts, typename Self = writer>
			requires requires(Self &w, Ts const &...ts) { (w.put(ts), ...); }
		void writeln(Ts const &...xs) {
			write(xs...);
			put('\n');
		}
	};

	template<typename T>
	concept writable = requires(writer &w, T const &x) { w.put(x); };

	inline writer out;

	template<writable... Ts>
	void writeln(Ts const &...xs) {
		out.writeln(xs...);
	}

#if defined(__cpp_lib_to_chars)

#endif

}

using namespace atl;
namespace io = atl::io;
namespace rng = std::ranges;
namespace vws = std::views;
namespace geo = atl::geometry;

int main() {
	auto const t = io::read<int>();

	for (int tc = 0; tc < t; ++tc) {
		auto const n = io::read<int>();
		std::vector<geo::point<int>> points(n);
		for (auto &&[x, y] : points) {
			std::tie(x, y) = io::read<int, int>();
		}
		auto [i, j] = geo::farthest_pair_indices(points);
		if (i == j) ++j;
		io::writeln(i, j);
	}

	return 0;
}
