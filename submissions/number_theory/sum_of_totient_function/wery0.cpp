#include <bits/stdc++.h>
using namespace std;
__int128_t abs(__int128_t x) {return x < 0 ? -x : x;}
__int128_t stoi128(string s) {int sign = s[0] == '-' ? -1 : 1; __int128_t x = 0; for (size_t i = sign == -1; i < s.size(); ++i) x = x * 10 + s[i] - '0'; return x * sign;}
string to_string(__int128_t x) {int sign = x < 0 ? -1 : 1; x *= sign; if (x == 0) return "0"; string s; while (x) s += '0' + x % 10, x /= 10; if (sign == -1) s += '-'; reverse(s.begin(), s.end()); return s;}
string to_string(__uint128_t x) {if (x == 0) return "0"; string s; while (x) s += '0' + x % 10, x /= 10; reverse(s.begin(), s.end()); return s;}
istream& operator>>(istream& in, __int128_t& x) {string s; in >> s; x = stoi128(s); return in;}
ostream& operator<<(ostream& out, __int128_t x) {return out << to_string(x);}
istream& operator>>(istream& in, __uint128_t& x) {string s; in >> s; x = 0; for (char c : s) x = x * 10 + c - '0'; return in;}
ostream& operator<<(ostream& out, __uint128_t x) {return out << to_string(x);}


inline __int128 totient_sum(const int64_t N) {
    int32_t v = int32_t(std::sqrt(N));
    while (int64_t(v) * v < N) ++v;
    while (int64_t(v) * v > N) --v;

    std::vector<int32_t> primes;
    std::vector<int64_t> s0(v + 1), s1(v + 1), l0(v + 1);
    std::vector<__int128> l1(v + 1);

    const auto f = [&](int32_t p, int32_t e) -> int64_t {
        int64_t ret = p - 1;
        while (e > 1) --e, ret *= p;
        return ret;
    };

    const auto divide = [](int64_t n, int64_t d) -> int64_t { return int64_t(double(n) / d); };

    for (int32_t i = 1; i <= v; ++i) s0[i] = i - 1, s1[i] = int64_t(i) * (i + 1) / 2 - 1;
    for (int32_t i = 1; i <= v; ++i) l0[i] = N / i - 1, l1[i] = __int128(N / i) * (N / i + 1) / 2 - 1;
    for (int32_t p = 2; p <= v; ++p) {
        if (s0[p] > s0[p - 1]) {
            primes.push_back(p);
            int64_t q = int64_t(p) * p, M = N / p, t0 = s0[p - 1], t1 = s1[p - 1];
            int64_t t = v / p, u = std::min<int64_t>(v, N / q);
            for (int64_t i = 1; i <= t; ++i) l0[i] -= (l0[i * p] - t0), l1[i] -= (l1[i * p] - t1) * p;
            for (int64_t i = t + 1; i <= u; ++i) l0[i] -= (s0[divide(M, i)] - t0), l1[i] -= (s1[divide(M, i)] - t1) * p;
            for (int32_t i = v; i >= q; --i) s0[i] -= (s0[divide(i, p)] - t0), s1[i] -= (s1[divide(i, p)] - t1) * p;
        }
    }
    for (int32_t i = 1; i <= v; ++i) s1[i] -= s0[i];
    for (int32_t i = 1; i <= v; ++i) l1[i] -= l0[i];

    for (auto it = primes.rbegin(); it != primes.rend(); ++it) {
        int32_t p = *it;
        int64_t q = int64_t(p) * p, M = N / p, s = s1[p - 1];
        int64_t t = v / p, u = std::min<int64_t>(v, N / q);
        for (int64_t i = q; i <= v; ++i) s1[i] += (s1[divide(i, p)] - s) * f(p, 1);
        for (int64_t i = u; i > t; --i) l1[i] += (s1[divide(M, i)] - s) * f(p, 1);
        for (int64_t i = t; i >= 1; --i) l1[i] += (l1[i * p] - s) * f(p, 1);
    }

    for (int32_t i = 1; i <= v; ++i) s1[i] += 1;
    for (int32_t i = 1; i <= v; ++i) l1[i] += 1;

    const auto dfs = [&](auto &&self, int64_t n, size_t beg, int64_t coeff) -> __int128 {
        if (!coeff) return 0;

        __int128 ret = __int128(coeff) * (n > v ? l1[divide(N, n)] : s1[n]);
        for (size_t i = beg; i < primes.size(); ++i) {
            int32_t p = primes[i];
            int64_t q = int64_t(p) * p;
            if (q > n) break;

            int64_t nn = divide(n, q);
            for (int32_t e = 2; nn > 0; nn = divide(nn, p), ++e)
                ret += self(self, nn, i + 1, coeff * (f(p, e) - f(p, 1) * f(p, e - 1)));
        }

        return ret;
    };

    return dfs(dfs, N, 0, 1);
}

int32_t main() {
    int64_t n;
    std::cin >> n;

    std::cout << totient_sum(n) % 998'244'353 << '\n';
}
