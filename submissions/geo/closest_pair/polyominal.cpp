#line 1 "oj-test/closest-pair.test.cpp"
// verification-helper: PROBLEM https://judge.yosupo.jp/problem/closest_pair

#line 2 "src/geometry/closest-pair.hpp"

/**
 * Description: Given a set of points, returns an arbitrary closest pair of points.
 * Source: https://judge.yosupo.jp/submission/214022
 * Status: Tested with https://judge.yosupo.jp/problem/closest_pair
 */

#line 2 "src/contest/base.hpp"

#include <bits/stdc++.h>

using std::vector, std::array, std::string;
using std::set, std::map, std::multiset;
using std::min, std::max, std::swap;
using std::pair, std::tuple;
using std::tie;
using std::abs, std::sin, std::cos, std::tan, std::asin, std::acos, std::atan2;

template <class T> using Vec = vector<T>;
template <class T> using Opt = std::optional<T>;

using i32 = int32_t;
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

template <class F> struct yc_result { /// start-hash
	F f;
	template <class T> explicit yc_result(T&& f_) : f(std::forward<T>(f_)) {}
	template <class... A> decltype(auto) operator()(A&&... as) {
		return f(std::ref(*this), std::forward<A>(as)...);
	}
};
template <class F> decltype(auto) yc(F&& f) {
	return yc_result<std::decay_t<F>>(std::forward<F>(f));
} /// end-hash

inline std::mt19937_64 mt(std::chrono::steady_clock::now().time_since_epoch().count());
#line 10 "src/geometry/closest-pair.hpp"

namespace closest_pair_impl {

template <class T> using P = pair<T, T>;

// PRECONDITION: There are at least 2 points
template <class T> inline tuple<T, P<T>, P<T>> closest_pair(Vec<P<T>> pts) {
	int n = int(size(pts));
	using PT = P<T>;
	std::ranges::sort(pts,
					  [](PT a, PT b) -> bool { return a.first < b.first; });
	auto sq = [&](T a) -> T { return a * a; };
	auto dist2 = [&](PT a, PT b) -> T {
		return sq(a.first - b.first) + sq(a.second - b.second);
	};

	T d = std::numeric_limits<T>::max();
	PT pa, pb;
	auto update = [&](PT a, PT b) {
		auto nd = dist2(a, b);
		if (nd < d) {
			d = nd, pa = a, pb = b;
		}
	};

	auto st =
		multiset<PT,
				 decltype([](PT a, PT b) { return a.second < b.second; })>();
	auto its = Vec<typename decltype(st)::const_iterator>(size(pts));

	for (int i = 0, f = 0; i < n; i++) {
		PT p = pts[i];
		while (f < i && sq(p.first - pts[f].first) >= d) {
			st.erase(its[f++]);
		}
		auto u = st.upper_bound(p);
		{
			auto t = u;
			while (true) {
				if (t == begin(st)) break;
				t = prev(t);
				update(*t, p);
				if (sq(p.second - t->second) >= d) break;
			}
		}
		{
			auto t = u;
			while (true) {
				if (t == end(st)) break;
				if (sq(p.second - t->second) >= d) break;
				update(*t, p);
				t = next(t);
			}
		}
		its[i] = st.emplace_hint(u, p);
	}

	return {d, pa, pb};
}

}  // namespace closest_pair_impl
#line 2 "src/contest/fast-input.hpp"

/**
 * Author: Hanfei Chen
 * Description: Fast scanner implementation based on \texttt{fread}
 * Status: Tested with
 * - https://judge.yosupo.jp/problem/associative_array (uint64_t)
 */

#line 11 "src/contest/fast-input.hpp"

namespace fast_input {

struct Scanner {
	FILE* f;
	Scanner(FILE* f_) : f(f_) {}

	void read() {} /// start-hash
	template <class H, class... T> void read(H& h, T&... t) {
		read_single(h);
		read(t...);
	} /// end-hash

	char buf[1 << 16]; /// start-hash
	size_t s = 0, e = 0;
	char get() {
		if (s >= e) {
			buf[0] = 0;
			s = 0;
			e = fread(buf, 1, sizeof(buf), f);
		}
		return buf[s++];
	} /// end-hash

	template <class T> void read_single(T& r) { /// start-hash
		char c;
		while ((c = get()) <= ' ') {}
		bool neg = false;
		if (c == '-') {
			neg = true;
			c = get();
		}
		r = 0;
		do {
			r = 10 * r + (c & 15);
		} while ((c = get()) >= '0');
		if (neg) r = -r;
	} /// end-hash

	void read_single(string& r) { /// start-hash
		char c;
		while ((c = get()) <= ' ') {}
		r = "";
		do {
			r += c;
		} while ((c = get()) > ' ');
	} /// end-hash

	void read_single(double& r) { /// start-hash
		string z;
		read_single(z);
		r = stod(z);
	} /// end-hash
};

} // namespace fast_input
#line 6 "oj-test/closest-pair.test.cpp"

using P = pair<i64, i64>;

pair<int, int> solve(const Vec<P>& pts) {
	auto best = closest_pair_impl::closest_pair(pts);
	const auto p0 = get<1>(best);
	const auto p1 = get<2>(best);
	if (p0 != p1) {
		auto get_idx = [&](const P& p) -> int {
			return int(std::find(begin(pts), end(pts), p) - begin(pts));
		};
		return {get_idx(get<1>(best)), get_idx(get<2>(best))};
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
	sc.read(T);
	while (T--) {
		int N;
		sc.read(N);
		auto pts = Vec<P>(N);
		for (auto& [x, y] : pts) {
			sc.read(x, y);
		}

		auto res = solve(pts);
		std::cout << res.first << ' ' << res.second << '\n';
	}

	return 0;
}