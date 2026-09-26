#include<bits/stdc++.h>
using namespace std;

#define endl '\n'
typedef long long LL;
typedef vector<int> VI;
typedef vector<LL> VL;
typedef pair<int, int> PII;
typedef pair<LL, LL> PLL;
#define rep(i, l, r) for(int i = l; i <= (r); i ++ )
#define per(i, r, l) for(int i = r; i >= (l); i -- )
#define reps(i, l, r, d) for(int i = l; i <= (r); i += d)
#define pers(i, r, l, d) for(int i = r; i >= (l); i -= d)

constexpr int N = 1e5 + 10;
constexpr int p = 1e9 + 7;

/**
 * @brief 素数检测
 */
namespace OY {
    bool is_prime32(uint32_t n) {
        if (n < 64) return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        struct Info {
            uint32_t m_mod, m_mod2, m_pinv, m_ninv;
            uint32_t m_one;
            uint64_t m_inv;
            void set_mod(uint32_t P) {
                m_mod = m_pinv = P, m_mod2 = P * 2, m_ninv = -uint64_t(P) % P, m_inv = uint64_t(-1) / P + 1;
                for (size_t i = 0; i != 4; i++) m_pinv *= uint32_t(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            uint32_t mod(uint64_t val) const {
                uint32_t res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
                if (res >= mod()) res += mod();
                return res;
            }
            uint32_t mod() const { return m_mod; }
            uint32_t reduce(uint64_t val) const { return (val + uint64_t(uint32_t(val) * m_pinv) * mod()) >> 32; }
            uint32_t strict_reduce(uint32_t val) const { return val >= mod() ? val - mod() : val; }
            uint32_t mul(uint32_t a, uint32_t b) const { return reduce(uint64_t(a) * b); }
            uint32_t pow(uint32_t a, uint32_t n) const {
                uint32_t res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            uint32_t one() const { return m_one; }
            uint32_t raw_init(uint32_t val) const { return mul(val, m_ninv); }
        };
        Info info;
        info.set_mod(n);
        uint32_t d = (n - 1) >> __builtin_ctz(n - 1), one = info.one(), minus_one = info.mod() - one;
        auto mr = [&](uint32_t a) {
            uint32_t s = d, y = info.pow(info.raw_init(a), s);
            while (s != n - 1 && info.strict_reduce(y) != one && info.strict_reduce(y) != minus_one) y = info.mul(y, y), s <<= 1;
            return info.strict_reduce(y) == minus_one || s % 2;
        };
        return mr(2) && mr(7) && mr(61);
    }
    bool is_prime64(uint64_t n) {
        if (n < 64) return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        struct Info {
            uint64_t m_mod, m_mod2, m_pinv, m_ninv, m_inv, m_one;
            void set_mod(uint64_t P) {
                m_ninv = -__uint128_t(P) % P;
                m_mod = m_pinv = P, m_mod2 = P * 2, m_inv = uint64_t(-1) / P + 1;
                for (size_t i = 0; i != 5; i++) m_pinv *= uint64_t(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            uint64_t mod(uint64_t val) const {
                uint64_t res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
                if (res >= mod()) res += mod();
                return res;
            }
            uint64_t mod() const { return m_mod; }
            uint64_t reduce(__uint128_t val) const {
                return (val + __uint128_t(uint64_t(val) * m_pinv) * mod()) >> 64;
            }
            uint64_t strict_reduce(uint64_t val) const { return val >= mod() ? val - mod() : val; }
            uint64_t mul(uint64_t a, uint64_t b) const {
                return reduce(__uint128_t(a) * b);
            }
            uint64_t pow(uint64_t a, uint64_t n) const {
                uint64_t res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            uint64_t one() const { return m_one; }
            uint64_t raw_init(uint64_t val) const { return mul(val, m_ninv); }
        };
        Info info;
        info.set_mod(n);
        uint64_t d = (n - 1) >> __builtin_ctzll(n - 1), one = info.one(), minus_one = info.mod() - one;
        auto mr = [d, info](uint32_t a) {
            if (a >= info.mod()) return true;
            uint64_t s = d, y = info.pow(info.raw_init(a), s);
            while (s != info.mod() - 1 && info.strict_reduce(y) != info.one() && info.strict_reduce(y) != info.mod() - info.one()) y = info.mul(y, y), s <<= 1;
            return info.strict_reduce(y) == info.mod() - info.one() || s % 2;
        };
        if (!(n >> 32)) return mr(2) && mr(7) && mr(61);
        return mr(2) && mr(325) && mr(9375) && mr(28178) && mr(450775) && mr(9780504) && mr(1795265022);
    }
    template <typename Tp>
    bool is_prime(Tp n) {
        if constexpr (std::is_same<Tp, uint32_t>::value)
            return is_prime32(n);
        else
            return is_prime64(n);
    }
}


/**
 * @brief Pollard Rho，因数分解
 */
namespace OY {
    class PollardRho {
        struct PollardRhoPair {
            uint64_t m_prime;
            uint32_t m_count;
            bool operator<(const PollardRhoPair &rhs) const { return m_prime < rhs.m_prime; }
        };
        static bool is_prime(uint64_t x) { return is_prime64(x); }
        template <typename Callback>
        static void _dfs(uint64_t cur, Callback &&call) {
            if (!is_prime(cur)) {
                uint64_t a = pick(cur);
                _dfs(a, call), _dfs(cur / a, call);
            } else
                call(cur);
        }
        template <typename Callback>
        static void _dfs(uint32_t index, uint64_t prod, const std::vector<PollardRhoPair> &pairs, Callback &&call) {
            if (index == pairs.size())
                call(prod);
            else {
                auto &&pair = pairs[index];
                uint64_t p = pair.m_prime, c = pair.m_count;
                _dfs(index + 1, prod, pairs, call);
                while (c--) _dfs(index + 1, prod *= p, pairs, call);
            }
        }
    public:
        /**
         * @brief 枚举 n 的所有质因子，重复的质因子会被枚举多次
         */
        template <typename Callback>
        static void enumerate_prime_factors(uint64_t n, Callback &&call) {
            if (n % 2 == 0) {
                uint32_t ctz = __builtin_ctzll(n);
                n >>= ctz;
                while (ctz--) call(uint64_t(2));
            }
            if (n > 1) _dfs(n, call);
        }
        /**
         * @brief 根据质因数分解的结果枚举所有因数
         */
        template <typename Callback>
        static void enumerate_factors(const std::vector<PollardRhoPair> &pairs, Callback &&call) { _dfs(0, 1, pairs, call); }
        /**
         * @brief 获得 n 的所有因数
         */
        template <typename Callback>
        static void enumerate_factors(uint64_t n, Callback &&call) { enumerate_factors(decomposite<false>(n), call); }
        /**
         * @brief 返回 n 的一个非平凡因数，需要保证 n 不是质数
         */
        static uint64_t pick(uint64_t n) {
            if (n % 2 == 0) { return 2; }
            struct Info {
                uint64_t m_mod, m_pinv;
                void set_mod(uint64_t n) {
                    m_mod = m_pinv = n;
                    for (size_t i = 0; i < 5; ++i) m_pinv *= 2 - m_mod * m_pinv;
                }
                uint64_t mul_add(uint64_t a, uint64_t b, uint64_t c) const {
                    __uint128_t d = __uint128_t(a) * b;
                    return c + m_mod + (d >> 64) - uint64_t((__uint128_t(uint64_t(d) * m_pinv) * m_mod) >> 64);
                }
                uint64_t mul(uint64_t a, uint64_t b) const { return mul_add(a, b, 0); }
            };
            Info info;
            info.set_mod(n);
            uint64_t C1 = 1, C2 = 2, M = 512, Z1 = 1, Z2 = 2, ans = 0;
            auto find = [&]() {
                uint64_t z1 = Z1, z2 = Z2;
                for (uint64_t k = M;; k *= 2) {
                    uint64_t x1 = z1 + n, x2 = z2 + n;
                    for (uint64_t j = 0; j < k; j += M) {
                        uint64_t y1 = z1, y2 = z2, q1 = 1, q2 = 2;
                        z1 = info.mul_add(z1, z1, C1), z2 = info.mul_add(z2, z2, C2);
                        for (uint64_t i = 0; i < M; ++i) {
                            uint64_t t1 = x1 - z1, t2 = x2 - z2;
                            z1 = info.mul_add(z1, z1, C1), z2 = info.mul_add(z2, z2, C2);
                            q1 = info.mul(q1, t1), q2 = info.mul(q2, t2);
                        }
                        q1 = info.mul(q1, x1 - z1), q2 = info.mul(q2, x2 - z2);
                        uint64_t q3 = info.mul(q1, q2), g3 = std::gcd(n, q3);
                        if (g3 == 1) continue;
                        if (g3 != n) return void(ans = g3);
                        uint64_t g1 = std::gcd(n, q1), g2 = std::gcd(n, q2), C = g1 != 1 ? C1 : C2, x = g1 != 1 ? x1 : x2, z = g1 != 1 ? y1 : y2, g = g1 != 1 ? g1 : g2;
                        if (g == n)
                            do z = info.mul_add(z, z, C), g = std::gcd(n, x - z);
                            while (g == 1);
                        if (g != n) return void(ans = g);
                        Z1 += 2, Z2 += 2;
                        return;
                    }
                }
            };
            do { find(); } while (!ans);
            return ans;
        }
        /**
         * @brief 分解质因数
         * @tparam Sorted 表示返回的质因数是否按照升序排列
         * @return `std::vector<PollardRhoPair>` ，其中 `PollardRhoPair` 包含 `m_prime` 和 `m_count` 两个属性，表示包含的质因子以及包含的数量。
         */
        template <bool Sorted = false>
        static std::vector<PollardRhoPair> decomposite(uint64_t n) {
            std::vector<PollardRhoPair> res;
            if (n % 2 == 0) {
                uint32_t ctz = __builtin_ctzll(n);
                res.push_back({uint64_t(2), ctz}), n >>= ctz;
            }
            auto call = [&](uint64_t x) {
                auto find = std::find_if(res.begin(), res.end(), [&](const PollardRhoPair &p) { return p.m_prime == x; });
                if (find == res.end())
                    res.push_back({x, 1});
                else
                    find->m_count++;
            };
            if (n > 1) _dfs(n, call);
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        /**
         * @brief 返回 n 的所有因数
         * @tparam Sorted 表示返回的因数是否按照升序排列
         */
        template <bool Sorted = false>
        static std::vector<uint64_t> get_factors(uint64_t n) {
            std::vector<uint64_t> res;
            uint32_t count = 1;
            auto pairs = decomposite<false>(n);
            for (auto &&pair : pairs) count *= pair.m_count + 1;
            res.reserve(count);
            enumerate_factors(pairs, [&](uint64_t f) { res.push_back(f); });
            if constexpr (Sorted) std::sort(res.begin(), res.end());
            return res;
        }
        /**
         * @brief 返回 n 的欧拉函数值
         */
        static uint64_t get_Euler_Phi(uint64_t n) {
            for (const auto &pair : decomposite(n)) n = n / pair.m_prime * (pair.m_prime - 1);
            return n;
        }
    };
}

void solve()
{
    LL n; cin >> n; cerr << n << endl;
    auto primes_pair = OY::PollardRho::decomposite<true>(n);
    int sum = 0;
    for(auto [prime, cnt] : primes_pair) {
        sum += cnt;
    }
    cout << sum << ' ';
    for(auto [prime, cnt] : primes_pair) {
        while(cnt -- ) {
            cout << prime << ' ';
        }
    }
    cout << endl;
}

int main()
{
    ios::sync_with_stdio(0); cin.tie(0);

    int T; cin >> T; cerr << T << endl;
    while(T -- )
        solve();


    return 0;
}