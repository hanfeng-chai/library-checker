#include <bits/stdc++.h>
#include <x86intrin.h>
#pragma GCC target("avx,avx2,sse")
using namespace std;
using ll = unsigned long long;

#if __OPTIMIZE__
void dbg(const char *v, const auto &...x) { (void) v, ((void) x, ...); }
#else
void dbg(const char *v, const auto &...x) { cerr << "[" << v << "]", ((cerr << " " << x), ...) << "\n"; }
#endif
#define dbg(...) dbg(#__VA_ARGS__, __VA_ARGS__)

constexpr ll mod = 998'244'353;
ll multInv(ll x, ll m) { // x^{-1} mod m
	return 1 < (x %= m) ? m - multInv(m, x) * m / x : 1;
}

int main() {
	constexpr ll R = 1ll << 32;
	constexpr ll R2 = (R % mod) * R % mod;
	constexpr ll R3 = R2 * R % mod;
	auto divR = [](ll x) -> ll { return x >> 32; };
	auto modR = [](ll x) -> ll { return x & (R - 1); };
	ll inv = multInv(R - mod, R);
	auto redc = [&](ll v) -> ll {
		ll x = modR(modR(v) * inv);
		v = divR(v + x*mod);
		if (v >= mod) v -= mod;
		return v;
	};
	auto mul = [&](ll a, ll b) -> ll { return redc(a * b); };
	auto add __attribute__((unused)) = [&](ll a, ll b) -> ll {
		ll r = a + b;
		if (r >= mod) r -= mod;
		return r;
	};

	int n;
	cin >> n;

	char *buf = new char[n*n*11];
	cin.read(buf, n*n*11);
	char *p = buf + 1;

	int stride = (n + 3) & ~3;
	ll *flat = (ll *) aligned_alloc(32, n * stride * 8);
	ll *out = (ll *) aligned_alloc(32, n * stride * 8);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ll x = 0;
			while ('0' <= *p && *p <= '9') x = x * 10 + (*p++ - '0');
			p++;
			flat[i*stride+j] = mul(x, R2);
			out[i*stride+j] = i == j ? R % mod : 0;
		}
	}
	auto loop = [&](ll *src, ll *dst, int start, int end, ll f) -> void {
		__m256i invs = _mm256_set1_epi64x(inv);
		__m256i mods = _mm256_set1_epi64x(mod);
		__m256i fs = _mm256_set1_epi64x(f == 0 ? 0 : mod - f);
		int j;
		for (j = start & ~3; j < end; j += 4) {
			__m256i xr = _mm256_load_si256((__m256i *) (src + j));
			__m256i prod = _mm256_mul_epu32(fs, xr);
			__m256i x = _mm256_mul_epu32(prod, invs);
			x = _mm256_mul_epu32(x, mods);
			prod = _mm256_add_epi64(prod, x);
			prod = _mm256_srli_epi64(prod, 32);
			__m256i mask = _mm256_cmpgt_epi64(mods, prod);
			mask = _mm256_andnot_si256(mask, mods);
			prod = _mm256_sub_epi64(prod, mask);
			__m256i xi = _mm256_load_si256((__m256i *) (dst + j));
			prod = _mm256_add_epi64(prod, xi);
			mask = _mm256_cmpgt_epi64(mods, prod);
			mask = _mm256_andnot_si256(mask, mods);
			prod = _mm256_sub_epi64(prod, mask);
			_mm256_store_si256((__m256i *) (dst + j), prod);
		}
	};
	for (int r = 0; r < n; r++) {
		for (int i = r; i < n; i++) {
			if (flat[i*stride+r] != 0){
				ranges::swap_ranges(flat + r*stride, flat + r*stride+stride, flat + i*stride, flat + i*stride + stride);
				ranges::swap_ranges(out + r*stride, out + r*stride+stride, out + i*stride, out + i*stride + stride);
				break;
		}}
		if (flat[r*stride+r] == 0) {
			cout << "-1\n";
			return 0;
		}
		ll f = mul(multInv(flat[r*stride+r], mod), R3);
		for (int j = 0; j < stride; j++) flat[r*stride+j] = mul(flat[r*stride+j], f);
		for (int j = 0; j < stride; j++) out[r*stride+j] = mul(out[r*stride+j], f);
		for (int i = r+1; i < n; i++) {
			f = flat[i*stride+r];
			loop(flat + r*stride, flat + i*stride, r, stride, f);
			loop(out + r*stride, out + i*stride, 0, r+1, f);
		}
	}
	for (int r = n-1; r >= 0; r--) {
		for (int i = 0; i < r; i++) {
			ll f = flat[i*stride+r];
			flat[i*stride+r] = 0;
			loop(out + r*stride, out + i*stride, 0, stride, f);
		}
	}
	p = buf;
	memset(buf, ' ', n*n*10);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ll x = redc(out[i*stride+j]);
			char *q = p + 9;
			do *--q = '0'+x%10, x /= 10; while (x);
			p += 10;
		}
		p[-1] = '\n';
	}
	cout.write(buf, p - buf);
}

