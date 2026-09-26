#line 1 "oj-test/geometry/closest_pair.test.cpp"
// verification-helper: PROBLEM https://judge.yosupo.jp/problem/closest_pair

#line 2 "src/contest/base.hpp"

#include <bits/stdc++.h>

using std::abs, std::sin, std::cos, std::tan, std::asin, std::acos, std::atan2;
using std::min, std::max, std::swap;
using std::pair, std::tuple;
using std::set, std::map, std::multiset;
using std::tie;
using std::vector, std::array, std::string;

template <class T> using Vec = vector<T>;
template <class T> using Opt = std::optional<T>;

using i8 = int8_t;
using u8 = uint8_t;
using i32 = int32_t;
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i128 = __int128_t;
using u128 = __uint128_t;

inline std::mt19937_64 mt(
	std::chrono::steady_clock::now().time_since_epoch().count());

template <class T> T rand_int(T l, T r) {
	return std::uniform_int_distribution<T>(l, r)(mt);
}
#line 2 "src/contest/fast-input.hpp"

/**
 * Description: Fast scanner implementation based on \texttt{fread}
 * Status: Tested with
 * - https://judge.yosupo.jp/problem/associative_array (uint64_t)
 */

#line 11 "src/contest/fast-input.hpp"

namespace fast_input {

struct Scanner {
	FILE* f;
	Scanner(FILE* f_ = stdin) : f(f_) {}

	char get() {  /// start-hash
		static array<char, 1 << 16> buf;
		static size_t s = 0, e = 0;
		if (s >= e) {
			buf[0] = 0;
			s = 0;
			e = fread(data(buf), 1, sizeof(buf), f);
		}
		return buf[s++];
	}  /// end-hash

	using Self = Scanner;

	char skip_whitespaces() {
		char c;
		while ((c = get()) <= ' ') {
		}
		return c;
	}

	template <class T> Self& operator>>(T& x) {
		char c = skip_whitespaces();
		bool neg = false;
		if (c == '-') {
			neg = true;
			c = get();
		}
		x = 0;
		do {
			x = 10 * x + (c & 15);
		} while ((c = get()) >= '0');
		if (neg) x = -x;
		return *this;
	}

	Self& operator>>(string& x) {
		char c = skip_whitespaces();
		x = {};
		do {
			x += c;
		} while ((c = get()) > ' ');
		return *this;
	}

	Self& operator>>(double& x) {
		string z;
		*this >> z;
		x = stod(z);
		return *this;
	}
};

}  // namespace fast_input
#line 2 "src/geometry/base.hpp"

/**
 * Description: Primitive operations
 * Source: Gifted Infants library
 * Status: Good luck
 */

#line 10 "src/geometry/base.hpp"

namespace geometry {

using std::fmod;

const double EPS = 1e-9;
template <class T> inline int sgn(T a) { return (a > EPS) - (a < -EPS); }
template <class T> inline int sgn(T a, T b) { return sgn(a - b); }

const double PI = acos(-1.);

template <class T> struct Point {
	using P = Point;  /// start-hash
	T x, y;
	Point(T x_ = T(), T y_ = T()) : x(x_), y(y_) {}	 /// end-hash

	P& operator+=(const P& p) {
		x += p.x, y += p.y;
		return *this;
	}  /// start-hash
	P& operator-=(const P& p) {
		x -= p.x, y -= p.y;
		return *this;
	}
	friend P operator+(const P& a, const P& b) { return P(a) += b; }
	friend P operator-(const P& a, const P& b) {
		return P(a) -= b;
	}  /// end-hash

	P& operator*=(const T& t) {
		x *= t, y *= t;
		return *this;
	}  /// start-hash
	P& operator/=(const T& t) {
		x /= t, y /= t;
		return *this;
	}
	friend P operator*(const P& a, const T& t) { return P(a) *= t; }
	friend P operator/(const P& a, const T& t) {
		return P(a) /= t;
	}  /// end-hash

	friend T dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y; }
	friend T crs(const P& a, const P& b) { return a.x * b.y - a.y * b.x; }

	P operator-() const { return P(-x, -y); }

	friend int cmp(const P& a, const P& b) {  /// start-hash
		int z = sgn(a.x, b.x);
		return z ? z : sgn(a.y, b.y);
	}  /// end-hash

	friend bool operator<(const P& a, const P& b) { return cmp(a, b) < 0; }
	friend bool operator<=(const P& a, const P& b) { return cmp(a, b) <= 0; }

	friend T dist2(const P& p) { return p.x * p.x + p.y * p.y; }
	friend auto dist(const P& p) { return sqrt(D(dist2(p))); }

	friend P unit(const P& p) { return p / p.dist(); }

	friend double arg(const P& p) { return atan2(p.y, p.x); }

	friend T rabs(const P& p) { return max(abs(p.x), abs(p.y)); }

	friend bool operator==(const P& a, const P& b) {
		return sgn(rabs(a - b)) == 0;
	}
	friend bool operator!=(const P& a, const P& b) { return !(a == b); }

	explicit operator pair<T, T>() const { return pair<T, T>(x, y); }

	static P polar(double m, double a) { return P(m * cos(a), m * sin(a)); }
};
template <class T>
int sgncrs(const Point<T>& a, const Point<T>& b) {	/// start-hash
	T cr = crs(a, b);
	if (abs(cr) <= (rabs(a) + rabs(b)) * EPS) return 0;
	return (cr < 0 ? -1 : 1);
}  /// end-hash

// not tested
template <class D> D norm_angle(D a) {	/// start-hash
	D res = fmod(a + PI, 2 * PI);
	if (res < 0) {
		res += PI;
	} else {
		res -= PI;
	}
	return res;
}  /// end-hash

// not tested
template <class D> D norm_nonnegative(D a) {  /// start-hash
	D res = fmod(a, 2 * PI);
	if (res < 0) res += 2 * PI;
	return res;
}  /// end-hash

// arg given lengths a, b, c,
// assumming a, b, c are valid
template <class D> D arg(D a, D b, D c) {  /// start-hash
	return acos(std::clamp<D>((a * a + b * b - c * c) / (2 * a * b), -1, 1));
}  /// end-hash

}  // namespace geometry
#line 2 "src/geometry/closest-pair.hpp"

/**
 * Description: Given a set of points, returns an arbitrary closest pair of points.
 * Source: https://judge.yosupo.jp/submission/214022
 * Status: Tested with https://judge.yosupo.jp/problem/closest_pair
 */

#line 11 "src/geometry/closest-pair.hpp"

namespace geometry {

template <class T> using P = Point<T>;

// PRECONDITION: There are at least 2 points
template <class T, class F> inline void closest_pair(Vec<P<T>> pts, F f) {
	int n = int(size(pts));
	using PT = P<T>;
	std::ranges::sort(pts, [](PT a, PT b) -> bool { return a.x < b.x; });
	T d = std::numeric_limits<T>::max();

	auto st = multiset<PT, decltype([](PT a, PT b) { return a.y < b.y; })>();
	auto its = Vec<typename decltype(st)::const_iterator>(size(pts));

	auto update = [&](PT a, PT b) {
		T d2 = dist2(a - b);
		if (d2 < d) {
			d = d2;
			f(a, b);
		}
	};

	for (int i = 0, j = 0; i < n; i++) {
		PT p = pts[i];

		auto sq = [](T x) { return x * x; };
		while (j < i && sq(p.x - pts[j].x) >= d) {
			st.erase(its[j++]);
		}
		auto u = st.upper_bound(p);
		{
			auto t = u;
			while (true) {
				if (t == begin(st)) break;
				t = prev(t);
				update(*t, p);
				if (sq(p.y - t->y) >= d) break;
			}
		}
		{
			auto t = u;
			while (true) {
				if (t == end(st)) break;
				if (sq(p.y - t->y) >= d) break;
				update(*t, p);
				t = next(t);
			}
		}
		its[i] = st.emplace_hint(u, p);
	}
}

}  // namespace geometry
#line 7 "oj-test/geometry/closest_pair.test.cpp"

using geometry::closest_pair;
using geometry::Point;

using P = Point<i64>;

pair<int, int> solve(const Vec<P>& pts) {
	auto p0 = pts[0];
	auto p1 = pts[1];
	closest_pair(pts, [&](P a, P b) { p0 = a, p1 = b; });
	if (p0 != p1) {
		auto get_idx = [&](const P& p) -> int {
			return int(std::find(begin(pts), end(pts), p) - begin(pts));
		};
		return {get_idx(p0), get_idx(p1)};
	} else {
		// must find two different indices
		auto it0 = std::find(begin(pts), end(pts), p0);
		auto it1 = std::find(next(it0), end(pts), p0);
		return {int(it0 - begin(pts)), int(it1 - begin(pts))};
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);

	auto sc = fast_input::Scanner(stdin);

	int T;
	sc >> T;
	while (T--) {
		int N;
		sc >> N;
		auto pts = Vec<P>(N);
		for (auto& [x, y] : pts) {
			sc >> x >> y;
		}

		auto res = solve(pts);
		std::cout << res.first << ' ' << res.second << '\n';
	}

	return 0;
}