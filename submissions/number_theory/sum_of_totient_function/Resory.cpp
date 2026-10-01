// Single N<=1e10: cached quotient counts and a contiguous rough-denominator list.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;
constexpr u32 MOD = 998244353;
constexpr int W = 2310; // Physical tables stay small.
#ifndef EXTRA_13
#define EXTRA_13 1
#endif
constexpr double DENSITY = EXTRA_13 ? 5760.0 / 30030 : 480.0 / W;
constexpr double SPLIT = 1.5;
u64 N;
int L, cnt[W + 1];
vector<u32> sp, memo, small_phi, count_small, count_large;
int root;
vector<int> gap;
vector<u32> denominators;
u32 A[W + 1], B[W + 1];
vector<bool> seen;
vector<double> reciprocal;
// IEEE binary64: the positive bias exceeds rounding error; for x<=1e10,
// x times the relative error is < 2e-5, so no next-integer boundary is crossed.
u64 divide(u64 x, u32 d) { return u64(double(x) * reciprocal[d]); }
u32 point(int i) { return sp[i] >= sp[i-1] ? sp[i]-sp[i-1] : sp[i]+MOD-sp[i-1]; }
u32 base_count(u64 x) { return x / W * 480 + cnt[x % W]; }
u32 coprime_count(u64 x) { return base_count(x) - (EXTRA_13 ? base_count(x / 13) : 0); }
// For W=2310: A[W]=1659120, B[W]=3317760. Reduce q*q before multiplying.
u32 base_rhs(u64 x) {
    u64 q = x / W; int r = x % W;
    return ((q * q % MOD) * 1658880 + q * (B[r] + 240) + A[r]) % MOD;
}

u32 rhs(u64 x) {
    u32 a = base_rhs(x), b = EXTRA_13 ? base_rhs(x / 13) : 0;
    return a >= b ? a - b : a + MOD - b;
}

// Only mark odd numbers. Any prime divisor suffices for prefix recovery.
void sieve(int n) {
    sp.resize(n + 1);
    int root = sqrt(n);
    vector<int> primes;
    vector<bool> composite(root + 1);
    vector<u32> inv(root + 1), bound(root + 1);
    for (int p = 2; p <= root; ++p) if (!composite[p]) {
        primes.push_back(p);
        for (int j = p * p; j <= root; j += p) composite[j] = true;
        if (p & 1) {
            u32 x = p;
            for (int j = 0; j < 5; ++j) x *= 2 - p * x;
            inv[p] = x; bound[p] = UINT32_MAX / p;
        }
    }
    const int B = 32760; // A multiple of 3*5*7; B odd positions per block.
    vector<uint16_t> pattern(B), factor(B);
    for (int p : {7, 5, 3})
        for (int j = (p - 1) / 2; j < B; j += p) pattern[j] = p;
    for (int lo = 0; lo <= n; lo += 2 * B) {
        int hi = min(n, lo + 2 * B - 1), len = (hi - lo + 1) / 2;
        copy_n(pattern.begin(), len, factor.begin());
        for (int p : primes) if (p >= 11 && p * p <= hi) {
            int m = max(p, (lo + p - 1) / p);
            if (!(m & 1)) ++m;
            for (int j = (m * p - lo) / 2; j < len; j += p) factor[j] = p;
        }
        for (int v = max(1, lo); v <= hi; ++v) {
            u32 ph;
            if (v == 1) ph = 1;
            else if (!(v & 1)) {
                int m = v / 2;
                ph = point(m) * ((m & 1) ? 1 : 2);
            } else {
                int p = factor[(v - lo) / 2];
                if (!p || p == v) ph = v - 1;
                else {
                    u32 m = u32(v) * inv[p];
                    bool divides = u32(m * inv[p]) <= bound[p];
                    ph = point(m) * (p - !divides);
                }
            }
            sp[v] = sp[v - 1] + ph;
            if (sp[v] >= MOD) sp[v] -= MOD;
        }
    }
}

// Large states only: x=floor(N/id)>L. Cached children need no division.
u32 solve(u32 id) {
    if (seen[id]) return memo[id];
    u64 x = divide(N, id);
    i64 a = rhs(x);
    int s = max(1, int(SPLIT * sqrt(x * DENSITY)));
    u32 t = divide(x, s + 1);
    a += i64(coprime_count(t)) * sp[s];
    // point(m) is exact phi(m), since L<MOD; the whole accumulator fits int64.
    int cut = min(s, root / int(id));
    for (int m = 1; m <= cut; ++m)
        a -= i64(small_phi[m]) * count_large[id * m];
    for (int m = cut + 1; m <= s; ++m)
        a -= i64(small_phi[m]) * count_small[divide(x, m)];
    u32 last = min(u64(t), x / (L + 1));
    int j = 0;
    for (; denominators[j] <= last; ++j) a -= solve(id * denominators[j]);
    for (; denominators[j] <= t; ++j) a -= sp[divide(x, denominators[j])];
    a %= MOD;
    if (a < 0) a += MOD;
    seen[id] = true;
    return memo[id] = a;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if (!(cin >> N)) return 0;
    if (!N) { cout << 0 << '\n'; return 0; }
    L = min(N, u64(max(int(sqrt(N)), int(500000 * pow(N / 1e10, 2.0 / 3)))));
    sieve(L);
    int bound = int(sqrt(N / DENSITY) / SPLIT) + 3;
    reciprocal.resize(bound + 1);
    for (int d = 1; d <= bound; ++d) reciprocal[d] = (1 + 1e-15) / d;
    int weight[W + 1];
    fill(weight, weight + W + 1, W);
    for (int p : {2, 3, 5, 7, 11})
        for (int i = p; i <= W; i += p) weight[i] = weight[i] / p * (p - 1);
    vector<int> residues;
    for (int i = 1; i <= W; ++i) {
        A[i] = A[i-1] + i * weight[i] / W;
        B[i] = B[i-1] + weight[i];
        cnt[i] = cnt[i-1] + (gcd(i, W) == 1);
        if (cnt[i] != cnt[i-1]) residues.push_back(i);
    }
    residues.push_back(W + 1);
    for (int i = 0; i + 1 < int(residues.size()); ++i) gap.push_back(residues[i+1]-residues[i]);
    for (int d = 1 + gap[0], j = 1; d <= bound;) {
        if (!EXTRA_13 || d % 13) denominators.push_back(d);
        d += gap[j]; if (++j == int(gap.size())) j = 0;
    }
    denominators.push_back(bound + 1);
    // Cache C(floor(N/i)) and C(i); all recursive queries lie on this grid.
    root = sqrt(N);
    small_phi.resize(root + 1); count_small.resize(root + 1); count_large.resize(root + 1);
    for (int i = 1; i <= root; ++i) {
        small_phi[i] = point(i);
        count_small[i] = coprime_count(i);
        count_large[i] = coprime_count(divide(N, i));
    }
    memo.resize(N / L + 2); seen.resize(memo.size());
    cout << (N <= u32(L) ? sp[N] : solve(1)) << '\n';
    return 0;
}
