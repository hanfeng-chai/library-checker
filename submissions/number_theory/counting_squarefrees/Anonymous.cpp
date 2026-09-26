// time O(n^{2/5} \log^{-8/5} n)
// space O(n^{1/3})
// welcome to https://negiizhao.blog.uoj.ac/archive
// welcome to https://qoj.ac/problem/8327

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <functional>
#include <cstdint>
#include <vector>

typedef unsigned int uint;
typedef long long unsigned int uint64;

uint64 sq(uint64 x)
{
	return x * x;
}

uint64 sqrt(uint64 x)
{
	uint64 r = std::sqrt((long double)(x));
	while (sq(r) > x)
		--r;
	while (sq(r + 1) <= x)
		++r;
	return r;
}

uint64 cbrt(uint64 x)
{
	uint64 r = std::cbrt((long double)(x));
	while (r * r * r > x)
		--r;
	while ((r + 1) * (r + 1) * (r + 1) <= x)
		++r;
	return r;
}

using value_t = uint32_t;

struct DS
{
	static uint64 N;
	static int R3;
	static std::vector<int> sqrtNdiv;
	
	std::vector<value_t> sv;
	std::vector<value_t> lv;
	
	DS() : sv(1 + R3, 0), lv(1 + R3, 0) { }
	
	value_t &operator[](const uint64 x)
	{
		return x <= R3 ? sv[x] : lv[N / (x * x)];
	}
	
	const value_t &operator[](const uint64 x) const
	{
		return x <= R3 ? sv[x] : lv[N / (x * x)];
	}
	
	void swap(DS &b)
	{
		sv.swap(b.sv);
		lv.swap(b.lv);
	}
	
	DS &operator+=(const DS &b)
	{
		for (int i = 1; i <= R3; ++i)
			sv[i] += b.sv[i];
		for (int i = 1; i <= R3; ++i)
			lv[i] += b.lv[i];
		return *this;
	}
	
	DS &operator-=(const DS &b)
	{
		for (int i = 1; i <= R3; ++i)
			sv[i] -= b.sv[i];
		for (int i = 1; i <= R3; ++i)
			lv[i] -= b.lv[i];
		return *this;
	}
	
	DS &operator*=(const value_t &s)
	{
		for (int i = 1; i <= R3; ++i)
			sv[i] *= s;
		for (int i = 1; i <= R3; ++i)
			lv[i] *= s;
		return *this;
	}
	
	DS &operator/=(const value_t &s)
	{
		for (int i = 1; i <= R3; ++i)
			sv[i] /= s;
		for (int i = 1; i <= R3; ++i)
			lv[i] /= s;
		return *this;
	}
	
	void partial_sum()
	{
		for (int i = 2; i <= R3; ++i)
			sv[i] += sv[i - 1];
		lv[R3] += sv[R3];
		for (int i = R3 - 1; i >= 1; --i)
			lv[i] += lv[i + 1];
	}
	
	void adjacent_difference()
	{
		for (int i = 1; i != R3; ++i)
			lv[i] -= lv[i + 1];
		lv[R3] -= sv[R3];
		for (int i = R3; i >= 2; --i)
			sv[i] -= sv[i - 1];
	}
};
uint64 DS::N;
int DS::R3;
std::vector<int> DS::sqrtNdiv;

inline void add(value_t &x, const value_t a, const value_t b) { x += a * b; }
inline void sub(value_t &x, const value_t a, const value_t b) { x -= a * b; }

template<typename I>
struct subrange
{
	I i, s;
	
	subrange(I _i, I _s) : i(_i), s(_s) { }
	
	const I &begin() const { return i; }
	const I &end() const { return s; }
	bool empty() const { return i == s; }
	auto size() const { return s - i; }
};

DS operator+(const DS &a, const DS &b)
{
	DS res;
	for (int i = 1; i <= DS::R3; ++i)
		res.sv[i] = a.sv[i] + b.sv[i];
	for (int i = 1; i <= DS::R3; ++i)
		res.lv[i] = a.lv[i] + b.lv[i];
	return res;
}

DS operator-(const DS &a, const DS &b)
{
	DS res;
	for (int i = 1; i <= DS::R3; ++i)
		res.sv[i] = a.sv[i] - b.sv[i];
	for (int i = 1; i <= DS::R3; ++i)
		res.lv[i] = a.lv[i] - b.lv[i];
	return res;
}

DS operator*(const DS &a, const DS &b)
{
	const uint64 &N = DS::N;
	const int &R3 = DS::R3;
	const int R4 = sqrt(sqrt(N));
	
	DS res;
	
	auto s1 = [](const DS &ds) -> std::vector<std::pair<int, value_t>>
	{
		std::vector<std::pair<int, value_t>> vec;
		for (int i = 1; i <= R3; ++i)
			if (ds.sv[i] != ds.sv[i - 1])
				vec.emplace_back(i, ds.sv[i] - ds.sv[i - 1]);
		vec.emplace_back(R3 + 1, 0);
		return vec;
	};
	
	auto s2 = [&res](const int x, const value_t y, const std::vector<std::pair<int, value_t>> &p, const int l, const int r)
	{
		int i = l;
		for (const int X = R3 / x; i != r && p[i].first <= X; ++i)
			add(res.sv[x * p[i].first], y, p[i].second);
		for (const uint64 Nxx = N / sq(x); i != r; ++i)
			add(res.lv[Nxx / sq(p[i].first)], y, p[i].second);
	};
	
	auto s3 = [&res](const int x, const value_t y, int i, const DS &ds)
	{
		uint64 xx = sq(x);
		const uint64 Nxx = N / sq(x);
		for (int X = R3 / xx; i > X; --i)
			add(res.lv[i], y, ds.sv[DS::sqrtNdiv[i] / x]);
		for (; i >= 1; --i)
			add(res.lv[i], y, ds.lv[xx * i]);
	};
	
	if (&a == &b)
	{
		const auto pa = s1(a);
		// const auto va = std::ranges::subrange(pa.begin(), std::lower_bound(pa.begin(), pa.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		const auto va = subrange(pa.begin(), std::lower_bound(pa.begin(), pa.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		
		for (int l = 0, r = pa.size() - 1; const auto &[x, y] : va)
		{
			++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pa[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			add(res[1ULL * x * x], y, y);
			s2(x, y * 2, pa, l, r);
			if (r)
				sub(res[1ULL * x * pa[r].first], y, a.sv[pa[r - 1].first] * 2);
		}
		
		res.partial_sum();
		
		for (int l = 0, r = pa.size() - 1; const auto &[x, y] : va)
		{
			++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pa[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			s3(x, y * 2, Nxx / sq(pa[r].first), a);
		}
	}
	else
	{
		const auto pa = s1(a);
		// const auto va = std::ranges::subrange(pa.begin(), std::lower_bound(pa.begin(), pa.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		const auto va = subrange(pa.begin(), std::lower_bound(pa.begin(), pa.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		const auto pb = s1(b);
		// const auto vb = std::ranges::subrange(pb.begin(), std::lower_bound(pb.begin(), pb.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		const auto vb = subrange(pb.begin(), std::lower_bound(pb.begin(), pb.end(), std::pair<int, value_t>{R4 + 1, 0LL}));
		
		for (int i = 1; i <= R4; ++i)
			add(res[1ULL * i * i], a.sv[i] - a.sv[i - 1], b.sv[i] - b.sv[i - 1]);
		
		for (int l = 0, r = pb.size() - 1; const auto &[x, y] : va)
		{
			while (pb[l].first <= x)
				++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pb[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			s2(x, y, pb, l, r);
			if (r)
				sub(res[1ULL * x * pb[r].first], y, b.sv[pb[r - 1].first]);
		}
		
		for (int l = 0, r = pa.size() - 1; const auto &[x, y] : vb)
		{
			while (pa[l].first <= x)
				++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pa[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			s2(x, y, pa, l, r);
			if (r)
				sub(res[1ULL * x * pa[r].first], y, a.sv[pa[r - 1].first]);
		}
		
		res.partial_sum();
		
		for (int l = 0, r = pb.size() - 1; const auto &[x, y] : va)
		{
			while (pb[l].first <= x)
				++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pb[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			s3(x, y, Nxx / sq(pb[r].first), b);
		}
		
		for (int l = 0, r = pa.size() - 1; const auto &[x, y] : vb)
		{
			while (pa[l].first <= x)
				++l;
			const uint64 Nxx = N / sq(x);
			while (r > l && Nxx / sq(pa[r - 1].first) < r - 1)
				--r;
			if (r < l)
				r = l;
			s3(x, y, Nxx / sq(pa[r].first), a);
		}
	}
	
	return res;
}

DS calc(const uint64 N)
{
	DS::N = N;
	const int R3 = cbrt(N);
	const int R4 = sqrt(sqrt(N));
	DS::R3 = R3;
	if (N == 0)
		return DS();
	if (N == 1)
	{
		DS ds;
		ds.sv[1] = ds.lv[1] = 1;
		return ds;
	}
	
	DS::sqrtNdiv.resize(1 + R3);
	for (int i = 1; i <= R3; ++i)
		DS::sqrtNdiv[i] = sqrt(N / i);
	
	const int R6 = std::sqrt(R3 + 0.5);
	const int MaxSP = std::pow(R3 + 0.5, 1. / 5) * .5;
	
	std::vector<char> pf(1 + R3);
	std::vector<int> P;
	int L = -1;
	
	for (int p = 2; p <= R3; ++p)
		if (!pf[p])
		{
			if (L == -1 && p > MaxSP)
				L = P.size();
			if (p <= R6)
				for (int j = p * p; j <= R3; j += p)
					pf[j] = 1;
			P.push_back(p);
		}
	if (L == -1)
		L = P.size();
	
	auto attach_small = [&](DS &ds, const std::function<value_t (uint)> &f) -> DS & // no small p in `ds`!
	{
		ds.adjacent_difference();
		std::vector<int> pred(1 + R3 + 1);
		pred[1] = 0;
		for (int i = 2; i <= R3 + 1; ++i)
			pred[i] = ds.sv[i - 1] ? i - 1 : pred[i - 1];
		for (int pid = L - 1; pid >= 0; --pid)
		{
			const int p = P[pid];
			const int pp = p * p;
			value_t y = f(p);
			{
				int j = 1, k = pp;
				for (; (j + 1) * pp - 1 <= R3; ++j)
					for (; k != (j + 1) * pp; ++k)
						if (ds.lv[k])
							add(ds.lv[j], ds.lv[k], y);
				for (; k <= R3; ++k)
					if (ds.lv[k])
						add(ds.lv[j], ds.lv[k], y);
			}
			{
				int j = pred[R3 + 1];
				uint64 Npp = N / pp;
				for (int X = R3 / p; j > X; j = pred[j])
					add(ds.lv[Npp / sq(j)], ds.sv[j], y);
				int k = R3 + 1;
				for (; j; j = pred[j])
				{
					while (pred[k] > j * p)
						k = pred[k];
					// assert(k != j * p);
					pred[j * p] = pred[k], pred[k] = j * p;
					add(ds.sv[j * p], ds.sv[j], y);
				}
			}
		}
		ds.partial_sum();
		return ds;
	};
	
	auto eliminate_small_inv = [&](DS &ds, const std::function<value_t (uint)> &f) // no small p in `res / f`!
	{
		ds.adjacent_difference();
		std::vector<int> pred(1 + R3 + 1);
		pred[1] = 0;
		for (int i = 2; i <= R3 + 1; ++i)
			pred[i] = ds.sv[i - 1] ? i - 1 : pred[i - 1];
		for (int pid = 0; pid < L; ++pid)
		{
			const int p = P[pid];
			const int pp = p * p;
			value_t y = f(p);
			{
				int j = 1, k = pp;
				for (; (j + 1) * pp - 1 <= R3; ++j)
					for (; k != (j + 1) * pp; ++k)
						if (ds.lv[k])
							add(ds.lv[j], ds.lv[k], y);
				for (; k <= R3; ++k)
					if (ds.lv[k])
						add(ds.lv[j], ds.lv[k], y);
			}
			{
				int j = pred[R3 + 1];
				uint64 Npp = N / pp;
				for (int X = R3 / p; j > X; j = pred[j])
					add(ds.lv[Npp / sq(j)], ds.sv[j], y);
				int k = R3 + 1;
				for (; j; j = pred[j])
				{
					while (pred[k] > j * p)
						k = pred[k];
					// assert(pred[k] == j * p);
					pred[k] = pred[j * p];
					add(ds.sv[j * p], ds.sv[j], y);
				}
			}
		}
		ds.partial_sum();
		return ds;
	};
	
	DS zeta;
	for (int i = 1; i <= R3; ++i)
		zeta.sv[i] = i;
	for (int i = 1; i <= R3; ++i)
		zeta.lv[i] = DS::sqrtNdiv[i];
	eliminate_small_inv(zeta, [&](uint p) -> value_t { return -1; });
	
	DS mu0;
	mu0.sv[1] = 1;
	for (int i = 1; i <= R3; ++i)
		for (auto p : P)
		{
			if (i * p > R3)
				break;
			if (p <= MaxSP)
			{
				mu0.sv[i * p] = 0;
				if (i % p == 0)
					break;
			}
			else
			{
				if (i % p != 0)
				{
					mu0.sv[i * p] = -mu0.sv[i];
				}
				else
				{
					mu0.sv[i * p] = 0;
					break;
				}
			}
		}
	mu0.partial_sum();
	
	DS res = zeta * mu0;
	for (int i = 1; i <= R3; ++i)
		res.sv[i] -= 1;
	for (int i = 1; i <= R3; ++i)
		res.lv[i] -= 1;
	mu0 -= res * mu0;
	res.swap(mu0);
	
	auto fp = [&](uint p) -> value_t { return -1; };
	
	attach_small(res, fp);
	
	return res;
}
#include <ctime>
int main(int argc, char **argv)
{
	uint64 n;
	
	scanf("%llu", &n);
	
	DS ds = calc(n);
	
	ds.adjacent_difference();
	uint64 Ans = 0;
	for (int i = 1; i <= DS::R3; ++i)
		Ans += int(ds.sv[i]) * (n / sq(i));
	for (int i = 1; i <= DS::R3; ++i)
		Ans += int(ds.lv[i]) * i;
	
	printf("%llu\n", Ans);
	
	fprintf(stderr, "%d\n", (int)(clock()));
	
	return 0;
}
