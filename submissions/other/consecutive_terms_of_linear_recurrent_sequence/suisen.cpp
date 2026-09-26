#include <cstdio>
#include <type_traits>
#include <utility>
#include <string>

namespace io {
    namespace internal {
        void read(char &c) {
            do c = getchar_unlocked(); while (not isprint(c));
        }
        template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
        void read(T& x) {
            char c;
            do c = getchar_unlocked(); while (not isprint(c));
            if (c == '-') {
                read<T>(x), x = -x;
                return;
            }
            if (not ('0' <= c and c <= '9')) throw - 1;
            x = 0;
            do x = x * 10 + (std::exchange(c, getchar_unlocked()) - '0'); while ('0' <= c and c <= '9');
        }
        void read(std::string& x) {
            x.clear();
            char c;
            do c = getchar_unlocked(); while (not isprint(c));
            do x += std::exchange(c, getchar_unlocked()); while (isprint(c));
        }
        
        void write(char c) { putchar_unlocked(c); }
        template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
        void write(T x) {
            static char buf[50];
            if constexpr (std::is_signed_v<T>) if (x < 0) putchar_unlocked('-'), x = -x;
            int i = 0;
            do buf[i++] = '0' + (x % 10), x /= 10; while (x);
            while (i--) putchar_unlocked(buf[i]);
        }
    }
    template <typename ...Args>
    void read(Args &...args) { (internal::read(args), ...); }
    template <typename Head, typename ...Tails>
    void print(Head&& head, Tails &&...tails) { internal::write(head), ((internal::write(' '), internal::write(tails)), ...); }
    template <typename ...Args>
    void println(Args &&...args) { print(std::forward<Args>(args)...), internal::write('\n'); }
}

#include <tuple>

#include <cassert>
#include <cstdint>
#include <limits>
#include <optional>
#include <iostream>

namespace suisen {
    namespace internal::modint {
        constexpr long long safe_mod(long long x, long long m) { return (x %= m) < 0 ? x + m : x; }
        constexpr long long pow_mod(long long x, long long n, int m) {
            if (m == 1) return 0;
            unsigned int um = m;
            unsigned long long r = 1, y = safe_mod(x, m);
            for (; n; n >>= 1) {
                if (n & 1) r = (r * y) % um;
                y = (y * y) % um;
            }
            return r;
        }
        constexpr bool is_prime(int n) {
            if (n <= 1) return false;
            if (n == 2 or n == 7 or n == 61) return true;
            if (n % 2 == 0) return false;
            long long d = n - 1;
            while (d % 2 == 0) d /= 2;
            constexpr long long bases[3] = { 2, 7, 61 };
            for (long long a : bases) {
                long long t = d, y = pow_mod(a, t, n);
                for (; t != n - 1 and y != 1 and y != n - 1; t <<= 1) y = y * y % n;
                if (y != n - 1 and t % 2 == 0) return false;
            }
            return true;
        }
        constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
            a = safe_mod(a, b);
            if (a == 0) return { b, 0 };
            long long s = b, t = a, m0 = 0, m1 = 1, tmp = 0;
            while (t) {
                long long u = s / t;
                s -= t * u, m0 -= m1 * u;
                tmp = s, s = t, t = tmp;
                tmp = m0, m0 = m1, m1 = tmp;
            }
            if (m0 < 0) m0 += b / s;
            return { s, m0 };
        }
        
        struct barrett_K128 {
            uint32_t M;      // mod
            __uint128_t L;   // ceil(2^K / M), where K = 128
            uint64_t dL, uL; // dL | (uL << 64) = L
            constexpr barrett_K128(uint32_t M) : M(M), L(~__uint128_t(0) / M + 1), dL(L), uL(L >> 64) {}
            constexpr uint32_t umod() const { return M; }
            // c mod M (correctly works for all 0<=c<2^64)
            template <bool care_M1 = true>
            constexpr uint32_t rem(uint64_t c) const {
                if constexpr (care_M1) if (M == 1) return 0;
                // uint32_t q = (c * L) >> 128;
                __uint128_t cu = __uint128_t(c) * uL;
                uint64_t cd = (__uint128_t(c) * dL) >> 64;
                uint32_t r = c - uint64_t(cu >> 64) * M;
                return uint64_t(cu) > ~cd ? r - M : r;
            }
            // a*b mod M
            constexpr uint32_t mul(uint32_t a, uint32_t b) const { return rem<false>(uint64_t(a) * b); }
        };
    }

    template <int m, std::enable_if_t<(1 <= m), std::nullptr_t> = nullptr>
    class static_modint {
        using mint = static_modint;

        struct raw_construct {};
        constexpr static_modint(int v, raw_construct) : _v(v) {}
    public:
        static constexpr int mod() { return m; }
        static constexpr unsigned int umod() { return m; }

        static constexpr mint raw(int v) { return mint(v, raw_construct{}); }

        constexpr static_modint() : _v(0) {}
        template <class T, std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_signed<T>>, std::nullptr_t> = nullptr>
        constexpr static_modint(T v) : _v{} {
            int x = v % mod();
            if (x < 0) x += mod();
            _v = x;
        }
        template <class T, std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>, std::nullptr_t> = nullptr>
        constexpr static_modint(T v) : _v(v % umod()) {}

        constexpr unsigned int val() const { return _v; }

        constexpr mint& operator++() {
            ++_v;
            if (_v == umod()) _v = 0;
            return *this;
        }
        constexpr mint& operator--() {
            if (_v == 0) _v = umod();
            --_v;
            return *this;
        }
        constexpr mint operator++(int) { mint x = *this; ++*this; return x; }
        constexpr mint operator--(int) { mint x = *this; --*this; return x; }

        constexpr mint& operator+=(const mint& rhs) {
            _v += rhs._v;
            if (_v >= umod()) _v -= umod();
            return *this;
        }
        constexpr mint& operator-=(const mint& rhs) {
            _v -= rhs._v;
            if (_v >= umod()) _v += umod();
            return *this;
        }
        constexpr mint& operator*=(const mint& rhs) {
            _v = (unsigned long long) _v * rhs._v % umod();
            return *this;
        }
        constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }

        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return _v == 0 ? *this : raw(umod() - _v); }

        constexpr mint pow(long long n) const {
            assert(0 <= n);
            mint x = *this, r = 1;
            for (; n; n >>= 1) {
                if (n & 1) r *= x;
                x *= x;
            }
            return r;
        }
        constexpr mint xpow(long long n) const { return n < 0 ? inv().pow(-n) : pow(n); }
        constexpr mint inv() const {
            if constexpr (is_prime_mod) {
                assert(_v);
                return pow(umod() - 2);
            } else {
                const auto [g, res] = internal::modint::inv_gcd(_v, mod());
                assert(g == 1);
                return res;
            }
        }
        friend constexpr mint operator+(const mint& lhs, const mint& rhs) { mint res = lhs; res += rhs; return res; }
        friend constexpr mint operator-(const mint& lhs, const mint& rhs) { mint res = lhs; res -= rhs; return res; }
        friend constexpr mint operator*(const mint& lhs, const mint& rhs) { mint res = lhs; res *= rhs; return res; }
        friend constexpr mint operator/(const mint& lhs, const mint& rhs) { mint res = lhs; res /= rhs; return res; }
        friend constexpr bool operator==(const mint& lhs, const mint& rhs) { return lhs._v == rhs._v; }
        friend constexpr bool operator!=(const mint& lhs, const mint& rhs) { return lhs._v != rhs._v; }
    private:
        unsigned int _v;
        static constexpr bool is_prime_mod = internal::modint::is_prime(mod());
    };

    template <int id>
    class dynamic_modint {
        using mint = dynamic_modint;
        using barrett = internal::modint::barrett_K128;

        struct raw_construct {};
        constexpr dynamic_modint(int v, raw_construct) : _v(v) {}
    public:
        static int mod() { return bt.umod(); }
        static unsigned int umod() { return bt.umod(); }

        static void set_mod(int m) {
            assert(1 <= m);
            bt = barrett(m);
        }
        static mint raw(int v) { return dynamic_modint(v, raw_construct{}); }

        dynamic_modint() : _v(0) {}
        template <class T, std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_signed<T>>, std::nullptr_t> = nullptr>
        dynamic_modint(T v) {
            if (v < 0) {
                int x = v % mod();
                if (x < 0) x += mod();
                _v = x;
            } else _v = bt.rem(v);
        }
        template <class T, std::enable_if_t<std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>>, std::nullptr_t> = nullptr>
        dynamic_modint(T v) : _v(bt.rem(v)) {}

        dynamic_modint(__uint128_t v) : _v(v % umod()) {}
        dynamic_modint(__int128_t v) {
            int x = v % mod();
            if (x < 0) x += mod();
            _v = x;
        }

        unsigned int val() const { return _v; }

        mint& operator++() {
            ++_v;
            if (_v == umod()) _v = 0;
            return *this;
        }
        mint& operator--() {
            if (_v == 0) _v = umod();
            --_v;
            return *this;
        }
        mint operator++(int) { mint x = *this; ++*this; return x; }
        mint operator--(int) { mint x = *this; --*this; return x; }

        mint& operator+=(const mint& rhs) {
            _v += rhs._v;
            if (_v >= umod()) _v -= umod();
            return *this;
        }
        mint& operator-=(const mint& rhs) {
            _v -= rhs._v;
            if (_v >= umod()) _v += umod();
            return *this;
        }
        mint& operator*=(const mint& rhs) {
            _v = bt.mul(_v, rhs._v);
            return *this;
        }
        mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }

        mint pow(long long n) const {
            assert(0 <= n);
            mint x = *this, r = 1;
            for (; n; n >>= 1) {
                if (n & 1) r *= x;
                x *= x;
            }
            return r;
        }
        mint xpow(long long n) const { return n < 0 ? inv().pow(-n) : pow(n); }
        mint inv() const {
            const auto [g, res] = internal::modint::inv_gcd(_v, mod());
            assert(g == 1);
            return res;
        }

        friend mint operator+(const mint& lhs, const mint& rhs) { mint res = lhs; res += rhs; return res; }
        friend mint operator-(const mint& lhs, const mint& rhs) { mint res = lhs; res -= rhs; return res; }
        friend mint operator*(const mint& lhs, const mint& rhs) { mint res = lhs; res *= rhs; return res; }
        friend mint operator/(const mint& lhs, const mint& rhs) { mint res = lhs; res /= rhs; return res; }
        friend bool operator==(const mint& lhs, const mint& rhs) { return lhs._v == rhs._v; }
        friend bool operator!=(const mint& lhs, const mint& rhs) { return lhs._v != rhs._v; }
    private:
        unsigned int _v;
        static inline barrett bt{ 998244353 };
    };

    using modint998244353 = static_modint<998244353>;
    using modint1000000007 = static_modint<1000000007>;
    using modint = dynamic_modint<-1>;

    template <typename T> struct is_modint : std::false_type {};
    template <int m> struct is_modint<static_modint<m>> : std::true_type {};
    template <int id> struct is_modint<dynamic_modint<id>> : std::true_type {};
    template <typename T> constexpr bool is_modint_v = is_modint<T>::value;

    template <typename T> struct is_static_modint : std::false_type {};
    template <int m> struct is_static_modint<static_modint<m>> : std::true_type {};
    template <typename T> constexpr bool is_static_modint_v = is_static_modint<T>::value;

    template <typename T> struct is_dynamic_modint : std::false_type {};
    template <int id> struct is_dynamic_modint<dynamic_modint<id>> : std::true_type {};
    template <typename T> constexpr bool is_dynamic_modint_v = is_dynamic_modint<T>::value;

    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    std::optional<mint> mod_sqrt(mint a) {
        const int p = mint::mod();
        if (a == 0) return mint(0);
        if (p == 2) return a;
        if (a.pow((p - 1) / 2) != 1) return std::nullopt;
        mint b = 1;
        while (b.pow((p - 1) / 2) == 1) ++b;
        const int tlz = __builtin_ctz(p - 1), q = (p - 1) >> tlz;
        mint x = a.pow((q + 1) / 2);
        b = b.pow(q);
        for (int shift = 2; x * x != a; ++shift) {
            mint e = a.inv() * x * x;
            if (e.pow(1 << (tlz - shift)) != 1) x *= b;
            b *= b;
        }
        return x;
    }

    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    mint sqrt(mint a) { return *mod_sqrt(a); }
    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    mint log(mint a) { assert(a == 1); return 0; }
    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    mint exp(mint a) { assert(a == 0); return 1; }
    template <typename mint, typename T, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    mint pow(mint a, T b) { return a.xpow(b); }
    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    mint inv(mint a) { return a.inv(); }

    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    std::istream& operator>>(std::istream& is, mint& v) { int val; is >> val, v = val; return is; }
    template <typename mint, std::enable_if_t<is_modint_v<mint>, std::nullptr_t> = nullptr>
    std::ostream& operator<<(std::ostream& os, const mint& v) { return os << v.val(); }
} // namespace suisen

#include <array>
#include <vector>

namespace suisen {
    namespace internal::dft {
        template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
        constexpr int primitive_root() {
            constexpr int m = mint::mod();
            if constexpr (m == 2) return 1;
            if constexpr (m == 167772161) return 3;
            if constexpr (m == 469762049) return 3;
            if constexpr (m == 754974721) return 11;
            if constexpr (m == 998244353) return 3;
            int divs[20] = {};
            divs[0] = 2;
            int cnt = 1;
            int x = (m - 1) / 2;
            while (x % 2 == 0) x /= 2;
            for (int i = 3; (long long) i * i <= x; i += 2) if (x % i == 0) {
                divs[cnt++] = i;
                while (x % i == 0) x /= i;
            }
            if (x > 1) divs[cnt++] = x;
            for (mint g = 2;; ++g) {
                bool ok = true;
                for (int i = 0; ok and i < cnt; i++) ok &= g.pow((m - 1) / divs[i]) != 1;
                if (ok) return g.val();
            }
        }
        constexpr int bsf(int n) { return __builtin_ctz(n); }

        constexpr bool is_pow_of_2(int n) { return n > 0 and n == (-n & n); }

        template <class mint,
            std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
        struct fft_info {
            static constexpr int g = primitive_root<mint>();
            static constexpr int rank2 = bsf(mint::mod() - 1);

            std::array<mint, rank2 + 1> root;   // root[i]^(2^i) == 1
            std::array<mint, rank2 + 1> iroot;  // root[i] * iroot[i] == 1

            std::array<mint, std::max(0, rank2 - 2 + 1)> rate2;
            std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;

            std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;
            std::array<mint, std::max(0, rank2 - 3 + 1)> irate3;

            std::array<mint, rank2 + 1> idft_coef; // 2^-i

            constexpr fft_info() : root{}, iroot{}, rate2{}, irate2{}, rate3{}, irate3{}, idft_coef{} {
                root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
                iroot[rank2] = root[rank2].inv();
                for (int i = rank2 - 1; i >= 0; i--) {
                    root[i] = root[i + 1] * root[i + 1];
                    iroot[i] = iroot[i + 1] * iroot[i + 1];
                }

                {
                    mint prod = 1, iprod = 1;
                    for (int i = 0; i <= rank2 - 2; i++) {
                        rate2[i] = root[i + 2] * prod;
                        irate2[i] = iroot[i + 2] * iprod;
                        prod *= iroot[i + 2];
                        iprod *= root[i + 2];
                    }
                }
                {
                    mint prod = 1, iprod = 1;
                    for (int i = 0; i <= rank2 - 3; i++) {
                        rate3[i] = root[i + 3] * prod;
                        irate3[i] = iroot[i + 3] * iprod;
                        prod *= iroot[i + 3];
                        iprod *= root[i + 3];
                    }
                }
                const mint inv_2 = mint(2).inv();
                idft_coef[0] = 1;
                for (int i = 1; i <= rank2; ++i) {
                    idft_coef[i] = idft_coef[i - 1] * inv_2;
                }
            }
        };
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    struct DFT {
    private:
        static constexpr internal::dft::fft_info<mint> fft_info{};
    public:
        static constexpr int max_size() { return 1 << fft_info.rank2; }
        static constexpr int get_root(int k) { return fft_info.root[k]; }
        static constexpr int get_root_inv(int k) { return fft_info.iroot[k]; }

        static constexpr int ceil_log2(int n) {
            return n == 1 ? 0 : std::numeric_limits<std::make_unsigned_t<int>>::digits - __builtin_clz(n - 1);
        }

        // F_i = Sum[j=0,2^n-1] f_j w_n^{ij}
        static void dft(std::vector<mint>& a) {
            using namespace internal::dft;
            const int n = a.size(), h = bsf(n);
            assert(n == 1 << h);

            for (int len = 0; len < h;) {
                // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
                if (h - len == 1) {
                    int p = 1 << (h - len - 1);
                    mint rot = 1;
                    for (int s = 0; s < (1 << len); s++) {
                        int offset = s << (h - len);
                        for (int i = 0; i < p; i++) {
                            mint l = a[i + offset];
                            mint r = a[i + offset + p] * rot;
                            a[i + offset] = l + r;
                            a[i + offset + p] = l - r;
                        }
                        if (s + 1 != (1 << len)) rot *= fft_info.rate2[bsf(~s)];
                    }
                    len++;
                } else {
                    // 4-base
                    int p = 1 << (h - len - 2);
                    mint rot = 1, imag = fft_info.root[2];
                    for (int s = 0; s < (1 << len); s++) {
                        mint rot2 = rot * rot;
                        mint rot3 = rot2 * rot;
                        int offset = s << (h - len);
                        for (int i = 0; i < p; i++) {
                            auto mod2 = 1ULL * mint::mod() * mint::mod();
                            auto a0 = 1ULL * a[i + offset].val();
                            auto a1 = 1ULL * a[i + offset + p].val() * rot.val();
                            auto a2 = 1ULL * a[i + offset + 2 * p].val() * rot2.val();
                            auto a3 = 1ULL * a[i + offset + 3 * p].val() * rot3.val();
                            auto a1na3imag = 1ULL * mint(a1 + mod2 - a3).val() * imag.val();
                            auto na2 = mod2 - a2;
                            a[i + offset] = a0 + a2 + a1 + a3;
                            a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                            a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                            a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                        }
                        if (s + 1 != (1 << len)) rot *= fft_info.rate3[bsf(~s)];
                    }
                    len += 2;
                }
            }
        }

        // f_i = Sum[j=0,2^n-1] F_j w_n^{-ij}
        static void idft(std::vector<mint>& a) {
            using namespace internal::dft;

            const int n = a.size(), h = bsf(n);
            assert(n == 1 << h);

            const mint coef = fft_info.idft_coef[h];
            for (auto &e : a) e *= coef;

            for (int len = h; len;) {
                // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
                if (len == 1) {
                    int p = 1 << (h - len);
                    mint irot = 1;
                    for (int s = 0; s < (1 << (len - 1)); s++) {
                        int offset = s << (h - len + 1);
                        for (int i = 0; i < p; i++) {
                            mint l = a[i + offset];
                            mint r = a[i + offset + p];
                            a[i + offset] = l + r;
                            a[i + offset + p] = (unsigned long long) (mint::mod() + l.val() - r.val()) * irot.val();
                        }
                        if (s + 1 != (1 << (len - 1))) irot *= fft_info.irate2[bsf(~s)];
                    }
                    len--;
                } else {
                    // 4-base
                    int p = 1 << (h - len);
                    mint irot = 1, iimag = fft_info.iroot[2];
                    for (int s = 0; s < (1 << (len - 2)); s++) {
                        mint irot2 = irot * irot;
                        mint irot3 = irot2 * irot;
                        int offset = s << (h - len + 2);
                        for (int i = 0; i < p; i++) {
                            auto a0 = 1ULL * a[i + offset + 0 * p].val();
                            auto a1 = 1ULL * a[i + offset + 1 * p].val();
                            auto a2 = 1ULL * a[i + offset + 2 * p].val();
                            auto a3 = 1ULL * a[i + offset + 3 * p].val();

                            auto a2na3iimag = 1ULL * mint((mint::mod() + a2 - a3) * iimag.val()).val();

                            a[i + offset] = a0 + a1 + a2 + a3;
                            a[i + offset + 1 * p] = (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();
                            a[i + offset + 2 * p] = (a0 + a1 + (mint::mod() - a2) + (mint::mod() - a3)) * irot2.val();
                            a[i + offset + 3 * p] = (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag)) * irot3.val();
                        }
                        if (s + 1 != (1 << (len - 2))) irot *= fft_info.irate3[bsf(~s)];
                    }
                    len -= 2;
                }
            }
        }

        // DFT((f_i)_{i=0}^{2^n-1}) --> DFT((f_i)_{i=0}^{2^n-1} ++ (0)_{i=0}^{2^n-1})  (size: n --> 2n)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_doubling(std::vector<mint>& dft_f, const std::optional<std::vector<mint>>& f = std::nullopt) {
            using namespace internal::dft;

            const size_t n = dft_f.size();
            const size_t k = bsf(n);
            if (f) {
                assert(n >= f->size());
            }
            assert(is_pow_of_2(n));

            std::vector<mint> odd;
            if (f) {
                odd = *f, odd.resize(n);
            } else {
                odd = dft_f, idft(odd);
            }
            const mint w = fft_info.root[k + 1];
            mint pow_w = 1;
            for (size_t i = 0; i < n; ++i) {
                odd[i] *= pow_w;
                pow_w *= w;
            }
            dft(odd);
            dft_f.resize(2 * n);
            std::move(odd.begin(), odd.end(), dft_f.begin() + n);
        }

        // DFT(f(-x))  (size: n --> n)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_neg_x(std::vector<mint>& dft_f) {
            using namespace internal::dft;

            const size_t n = dft_f.size();
            assert(is_pow_of_2(n));
            for (size_t i = 0; i < n; i += 2) {
                std::swap(dft_f[i], dft_f[i + 1]);
            }
        }

        // DFT(f(x)) --> DFT(f(x^2))  (size: n --> 2n)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_square_x(std::vector<mint>& dft_f) {
            using namespace internal::dft;

            const size_t n = dft_f.size();
            assert(is_pow_of_2(n));
            dft_f.resize(2 * n);
            for (size_t i = n; i--;) {
                dft_f[2 * i + 0] = dft_f[2 * i + 1] = dft_f[i];
            }
        }

        // DFT(f(x)) --> DFT(x^a f(x))
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_cyclic_shift(std::vector<mint>& dft_f, const int a) {
            if (a == 0) return;
            using namespace internal::dft;

            const size_t n = dft_f.size();
            const size_t k = bsf(n);
            assert(is_pow_of_2(n));
            const mint w = fft_info.root[k].xpow(a);
            // pow_w[i] = w^{bit_rev(i)}
            std::vector<mint> pow_w = bit_reversed_powers(w, n);
            for (size_t i = 0; i < n; ++i) {
                dft_f[i] *= pow_w[i];
            }
        }

        // f(x)=E(x^2)+xO(x^2), DFT(f(x)) --> DFT(E(x))  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_even(std::vector<mint>& dft_f) {
            using namespace internal::dft;

            static const mint inv_2 = fft_info.idft_coef[1];

            const size_t n = dft_f.size();
            assert(is_pow_of_2(n) and n >= 2);
            for (size_t i = 0; 2 * i < n; ++i) {
                dft_f[i] = (dft_f[2 * i] + dft_f[2 * i + 1]) * inv_2;
            }
            dft_f.resize(n / 2);
        }

        // f(x)=E(x^2)+xO(x^2), DFT(f(x)) --> DFT(O(x))  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_odd(std::vector<mint>& dft_f) {
            using namespace internal::dft;

            static const mint inv_2 = fft_info.idft_coef[1];

            const size_t n = dft_f.size();
            const size_t k = bsf(n);
            assert(is_pow_of_2(n) and n >= 2);
            std::vector<mint> pow_iw = bit_reversed_powers(fft_info.iroot[k], n / 2);
            for (size_t i = 0; 2 * i < n; ++i) {
                dft_f[i] = (dft_f[2 * i] - dft_f[2 * i + 1]) * inv_2 * pow_iw[i];
            }
            dft_f.resize(n / 2);
        }

        // f(x)=E(x^2)+xO(x^2), DFT(f(x)) --> { DFT(E(x)), DFT(O(x)) }  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static std::array<std::vector<mint>, 2> dft_split_parity(const std::vector<mint>& dft_f) {
            std::array<std::vector<mint>, 2> res{ dft_f, dft_f };
            dft_even(res[0]), dft_odd(res[1]);
            return res;
        }

        // f(x)=lo(x)+x^{n/2} hi(x), DFT(f(x)) --> DFT(lo(x))  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_lower(std::vector<mint>& dft_f) {
            dft_f = dft_split(dft_f)[0];
        }

        // f(x)=lo(x)+x^{n/2} hi(x), DFT(f(x)) --> DFT(hi(x))  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static void dft_higher(std::vector<mint>& dft_f) {
            dft_f = dft_split(dft_f)[1];
        }

        // f(x)=lo(x)+x^{n/2} hi(x), DFT(f(x)) --> { DFT(lo(x)), DFT(hi(x)) }  (size: n --> n/2)
        // @sa https://noshi91.hatenablog.com/entry/2023/12/10/163348
        static std::array<std::vector<mint>, 2> dft_split(const std::vector<mint>& dft_f) {
            using namespace internal::dft;

            static const mint inv_2 = fft_info.idft_coef[1];

            const size_t n = dft_f.size();
            const size_t k = bsf(n);
            assert(is_pow_of_2(n) and n >= 2);
            const size_t h = n / 2;

            std::vector<mint> odd(h);
            for (size_t i = 0; i < h; ++i) {
                odd[i] = dft_f[h + i];
            }
            idft(odd);
            const mint iw = fft_info.iroot[k];
            mint pow_iw = 1;
            for (size_t i = 0; i < h; ++i) {
                odd[i] *= pow_iw;
                pow_iw *= iw;
            }
            // DFT(lo - hi)
            dft(odd);
            std::vector<mint> dft_lo(h), dft_hi(h);
            for (size_t i = 0; i < h; ++i) {
                dft_lo[i] = (dft_f[i] + odd[i]) * inv_2;
                dft_hi[i] = (dft_f[i] - odd[i]) * inv_2;
            }
            return { std::move(dft_lo), std::move(dft_hi) };
        }

    private:
        // a[i] = x^bit_rev(i) for i=0,1,...,n
        static std::vector<mint> bit_reversed_powers(mint x, size_t n) {
            std::vector<mint> pow_x(n);
            pow_x[0] = 1;
            for (size_t i = 1; i < n; i <<= 1) {
                for (size_t j = 0; j < i; ++j) {
                    pow_x[j] *= pow_x[j];
                    pow_x[j + i] = x * pow_x[j];
                }
            }
            return pow_x;
        }
    };

    template <typename T>
    void pointwise_product_inplace(std::vector<T> &f, const std::vector<T> &g) {
        assert(f.size() == g.size());
        for (size_t i = 0; i < g.size(); ++i) f[i] *= g[i];
    }
    
    template <typename T>
    [[nodiscard]] std::vector<T> pointwise_product(const std::vector<T>& f, const std::vector<T>& g) {
        std::vector<T> h = f;
        pointwise_product_inplace(h, g);
        return h;
    }

    template <typename T>
    std::vector<T> convolution_cyclic_naive(const std::vector<T> &f, const std::vector<T> &g) {
        const size_t z = f.size();
        assert(g.size() == z);
        assert(internal::dft::is_pow_of_2(z));
        std::vector<T> h(z);
        // O(z^2)
        for (size_t i = 0; i < z; ++i) for (size_t j = 0; j < z; ++j) h[(i + j) & (z - 1)] += f[i] * g[j];
        return h;
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_cyclic(std::vector<mint> &&f, std::vector<mint> &&g) {
        using DFT = DFT<mint>;

        const size_t z = f.size();
        if (z <= 64) {
            return convolution_cyclic_naive(f, g);
        }
        assert(g.size() == z);
        assert(internal::dft::is_pow_of_2(z));
        const bool is_square = (f == g);
        DFT::dft(f);
        if (is_square) {
            g = f;
        } else {
            DFT::dft(g);
        }
        pointwise_product_inplace(f, g);
        DFT::idft(f);
        return std::move(f);
    }
    
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_cyclic(const std::vector<mint> &f, const std::vector<mint> &g) {
        auto a = f, b = g;
        return convolution_cyclic(std::move(a), std::move(b));
    }

    // z: power of 2
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_cyclic(std::vector<mint> &&f, std::vector<mint> &&g, const size_t z) {
        assert(internal::dft::is_pow_of_2(z));
        if (f.size() > z) for (size_t i = z; i < f.size(); ++i) f[i & (z - 1)] += f[i];
        if (g.size() > z) for (size_t i = z; i < g.size(); ++i) g[i & (z - 1)] += g[i];
        f.resize(z);
        g.resize(z);
        return convolution_cyclic(std::move(f), std::move(g));
    }

    // z: power of 2
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_cyclic(const std::vector<mint> &f, const std::vector<mint> &g, const size_t z) {
        auto a = f, b = g;
        return convolution_cyclic(std::move(a), std::move(b), z);
    }

    template <typename T>
    std::vector<T> convolution_naive(const std::vector<T> &f, const std::vector<T> &g) {
        const size_t n = f.size(), m = g.size();
        if (n == 0 or m == 0) {
            return {};
        }
        // naive: O(nm)
        std::vector<T> h(n + m - 1);
        if (n > m) {
            for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) h[i + j] += f[i] * g[j];
        } else {
            for (size_t j = 0; j < m; ++j) for (size_t i = 0; i < n; ++i) h[i + j] += f[i] * g[j];
        }
        return h;
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution(std::vector<mint> &&f, std::vector<mint> &&g) {
        using DFT = DFT<mint>;

        const size_t n = f.size(), m = g.size();
        if (n == 0 or m == 0) {
            return {};
        }
        if (std::min(n, m) <= 60) {
            return convolution_naive(f, g);
        }

        const size_t z = 1 << DFT::ceil_log2(n + m - 1);

        if (n + m - 3 <= z / 2) {
            const mint back_f = f.back();
            const mint back_g = g.back();
            std::vector<mint> res = convolution(std::vector<mint>(f.begin(), f.end() - 1), std::vector<mint>(g.begin(), g.end() - 1));
            res.push_back(0);
            res.push_back(back_f * back_g);
            for (size_t i = 0; i < n - 1; ++i) res[m - 1 + i] += f[i] * back_g;
            for (size_t j = 0; j < m - 1; ++j) res[n - 1 + j] += g[j] * back_f;
            return res;
        }
        f = convolution_cyclic(std::move(f), std::move(g), z);
        f.resize(n + m - 1);
        return f;
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution(const std::vector<mint> &f, const std::vector<mint> &g) {
        auto a = f, b = g;
        return convolution(std::move(a), std::move(b));
    }

    template <typename T>
    std::vector<T> convolution_trunc_naive(const std::vector<T> &f, const std::vector<T> &g, size_t k) {
        const size_t n = f.size(), m = g.size();
        if (n == 0 or m == 0) {
            return std::vector<T>(k);
        }
        // naive: O(nm)
        std::vector<T> h(k);
        if (n > m) {
            for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m and i + j < k; ++j) h[i + j] += f[i] * g[j];
        } else {
            for (size_t j = 0; j < m; ++j) for (size_t i = 0; i < n and i + j < k; ++i) h[i + j] += f[i] * g[j];
        }
        return h;
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_trunc(std::vector<mint> &&f, std::vector<mint> &&g, size_t k) {
        if (f.size() > k) f.resize(k);
        if (g.size() > k) g.resize(k);
        if (std::min(f.size(), g.size()) <= 60) {
            return convolution_trunc_naive(f, g, k);
        }
        std::vector<mint> h = convolution(std::move(f), std::move(g));
        h.resize(k);
        return h;
    }

    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> convolution_trunc(const std::vector<mint> &f, const std::vector<mint> &g, size_t k) {
        auto a = f, b = g;
        return convolution_trunc(std::move(a), std::move(b), k);
    }
} // namespace suisen

namespace suisen {
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> fps_inv(const std::vector<mint>& f, int n) {
        assert(n >= 0);

        using DFT = DFT<mint>;
        assert(f.size() and f[0] != 0);
        std::vector<mint> g{ f[0].inv() }, a, b;
        g.reserve(2 * n), a.reserve(2 * n), b.reserve(2 * n);
        for (int k = 1; k < n; k *= 2) {
            g.resize(2 * k), a.assign(2 * k, 0), b.resize(2 * k);
            const int len_f = std::min<int>(2 * k, f.size());
            std::copy(f.begin(), f.begin() + len_f, a.begin());
            std::copy(g.begin(), g.begin() + k, b.begin());
            DFT::dft(a);
            DFT::dft(b);
            pointwise_product_inplace(a, b);
            DFT::idft(a);
            for (int i = 0; i < k; ++i) a[i] = 0;
            DFT::dft(a);
            pointwise_product_inplace(a, b);
            DFT::idft(a);
            for (int i = k; i < 2 * k; ++i) g[i] = -a[i];
        }
        g.resize(n);
        return g;
    }
} // namespace suisen

#include <algorithm>

namespace suisen {
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> poly_div(std::vector<mint> f, std::vector<mint> g) {
        while (f.size() and f.back() == 0) f.pop_back();
        while (g.size() and g.back() == 0) g.pop_back();
        assert(g.size());
        if (f.empty()) return {};
        const size_t deg_f = f.size() - 1, deg_g = g.size() - 1;
        if (deg_f < deg_g) return {};
        const size_t deg_q = deg_f - deg_g;
        std::reverse(f.begin(), f.end());
        std::reverse(g.begin(), g.end());
        f.resize(deg_q + 1);
        std::vector<mint> q = convolution(f, fps_inv(g, deg_q + 1));
        q.resize(deg_q + 1);
        std::reverse(q.begin(), q.end());
        return q;
    }
    
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::array<std::vector<mint>, 2> poly_divmod(const std::vector<mint> &f, const std::vector<mint> &g) {
        const std::vector<mint> q = poly_div(f, g);
        const size_t deg_g = g.rend() - std::find_if(g.rbegin(), g.rend(), [](mint x) { return x != 0; }) - 1;
        std::vector<mint> r;
        if (q.empty()) {
            r = f;
        } else if (deg_g) {
            const size_t z = 1 << DFT<mint>::ceil_log2(deg_g);
            r = convolution_cyclic(q, g, z);
            for (size_t i = 0; i < f.size(); ++i) r[i & (z - 1)] -= f[i];
            r.resize(deg_g);
            for (size_t i = 0; i < deg_g; ++i) r[i] = -r[i];
        } else {
            r = {};
        }
        while (r.size() and r.back() == 0) {
            r.pop_back();
        }
        return { std::move(q), std::move(r) };
    }
    
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> poly_mod(const std::vector<mint> &f, const std::vector<mint> &g) {
        return poly_divmod(f, g)[1];
    }
} // namespace suisen

namespace suisen {
    namespace internal::linear_recurrence {
        // [ x^(k-2^d, k] ] 1/Q(x)  (deg Q < 2^d)
        template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
        std::pair<std::vector<mint>, bool> msb_first_bostan_mori(std::vector<mint> &&dft_q, const unsigned long long k) {
            using DFT = DFT<mint>;
            const size_t z = dft_q.size();
            if (k <= 2 * z) {
                assert(k + 1 >= z);
                DFT::idft(dft_q);
                std::vector<mint> res = fps_inv(dft_q, k + 1);
                res.erase(res.begin(), res.begin() + (k + 1 - z));
                return { res, false };
            }
            // Q(-x) 1/V(x^2), where V(x^2)=Q(-x)Q(x)

            // DFT(Q(-x))  (z)
            DFT::dft_neg_x(dft_q);
            // DFT(Q(-x))  (2z)
            DFT::dft_doubling(dft_q);
            // DFT(V(x))   (z)
            std::vector<mint> dft_v(z);
            // DFT(V(x^2)) (2z)  -->  DFT(V(x)) (z)
            for (size_t i = 0; i < z; ++i) dft_v[i] = dft_q[2 * i] * dft_q[2 * i + 1];
            auto [dft_a, is_dft] = msb_first_bostan_mori(std::move(dft_v), k / 2);
            if (not is_dft) DFT::dft(dft_a);
            // DFT(A(x^2))                  (2z): Range = [k-t-2z+2,k-t+2)   (t: k mod 2)
            DFT::dft_square_x(dft_a);
            // DFT(Q(-x) * A(x^2))          (2z): Range = [k-t-2z+2,k-t+z+1)
            pointwise_product_inplace(dft_a, dft_q);
            // DFT(Q(-x) * x^(1-t) A(x^2))  (2z): Range = [k-2z+1,k+z)
            // NOTE: this CYCLIC shift is safe since [x^(k-t+1)] A(x^2) = 0 (k-t+1 is odd).
            DFT::dft_cyclic_shift(dft_a, not (k & 1));
            // (k-z,k] (middle product)
            DFT::dft_higher(dft_a);
            return { dft_a, true };
        }
    }

    // Represent a linear recurrence as P(x)/Q(x) + R(x) (deg P < deg Q)
    // a_0,a_1,...: first terms, c: a_i=Sum[j=1,|c|] c_{j+1} a_{i-j} for i>=|c|
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::array<std::vector<mint>, 3> rational_form_of_linear_recurrence(const std::vector<mint> &a, const std::vector<mint> &c) {
        assert(a.size() >= c.size());
        // Q(x): denominator
        std::vector<mint> q(c.size() + 1);
        q[0] = 1;
        for (size_t i = 0; i < c.size(); ++i) q[i + 1] = -c[i];
        while (q.back() == 0) q.pop_back();
        // P(x): numerator
        auto [r, p] = poly_divmod(convolution_trunc(a, q, a.size()), q);
        return { std::move(p), std::move(q), std::move(r) };
    }

    // [x^{k+i}] 1/Q(x) for i=0,1,...,m-1
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> consecutive_terms_of_inv(std::vector<mint> q, const unsigned long long k, const size_t m) {
        using DFT = DFT<mint>;

        assert(q.front() != 0);
        if (m == 0) return {};

        while (q.back() == 0) {
            q.pop_back();
        }
        const size_t d = q.size() - 1;
        if (d == 0) {
            std::vector<mint> res(m);
            res[0] = k == 0 ? q[0].inv() : 0;
            return res;
        }
        const size_t z = 1 << DFT::ceil_log2(d + 1);
        std::vector<mint> dft_q(z);
        std::copy(q.begin(), q.end(), dft_q.begin());
        DFT::dft(dft_q);
        auto [res, is_dft] = internal::linear_recurrence::msb_first_bostan_mori(std::move(dft_q), k + z - 1);
        if (is_dft) DFT::idft(res);
        if (z >= m) {
            res.resize(m);
            return res;
        }
        return convolution_trunc(convolution_trunc(res, q, d), fps_inv(q, m), m);
    }

    // a_0,a_1,...: first terms, c: a_i=Sum[j=1,|c|] c_{j+1} a_{i-j} for i>=|c|
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    mint kth_term_of_linear_recurrent_sequence(const std::vector<mint> &a, const std::vector<mint> &c, unsigned long long k) {
        assert(a.size() >= c.size());
        if (k < a.size()) return a[k];

        // P(x)/Q(x) + R(x)
        auto [p, q, r] = rational_form_of_linear_recurrence(a, c);
        const size_t d = q.size() - 1;

        // 1/Q(x) (k-d,...,k]
        std::vector<mint> t = consecutive_terms_of_inv(q, k - d + 1, d);
        // [x^k] P(x)/Q(x)
        mint ans = 0;
        for (size_t i = 0; i < p.size(); ++i) ans += p[i] * t[d - 1 - i];
        return ans;
    }

    // a_0,a_1,...: first terms, c: a_i=Sum[j=1,|c|] c_{j+1} a_{i-j} for i>=|c|
    template <typename mint, std::enable_if_t<is_static_modint_v<mint>, std::nullptr_t> = nullptr>
    std::vector<mint> consecutive_terms_of_linear_recurrent_sequence(const std::vector<mint> &a, const std::vector<mint> &c, unsigned long long k, const size_t m) {
        assert(a.size() >= c.size());

        // P(x)/Q(x) + R(x)
        auto [p, q, r] = rational_form_of_linear_recurrence(a, c);
        const size_t d = q.size() - 1;

        // [x^{k+i}] R(x) for i=0,1,...,m-1
        r.erase(r.begin(), r.begin() + std::min<unsigned long long>(k, r.size()));
        r.resize(m);

        // x^{-k} mod Q(x)
        std::vector<mint> ix_k = convolution_trunc(consecutive_terms_of_inv(q, k, d), q, d);
        std::vector<mint> s = poly_mod(convolution(p, ix_k), q);
        // [x^{k+i}] P(x)/Q(x) for i=0,1,...,m-1
        std::vector<mint> pq = convolution_trunc(s, fps_inv(q, m), m);
        for (size_t i = 0; i < m; ++i) pq[i] += r[i];
        return pq;
    }

    // s_0,s_1,...: first terms
    template <typename F>
    std::vector<F> find_linear_recurrence(const std::vector<F>& s) {
        std::vector<F> B{ 1 }, C{ 1 };
        B.reserve(s.size()), C.reserve(s.size());
        F b = 1;
        size_t L = 0;
        for (size_t N = 0, x = 1; N < s.size(); ++N) {
            F d = s[N];
            for (size_t i = 1; i <= L; ++i) d += C[i] * s[N - i];
            if (d == 0) {
                ++x;
            } else {
                F c = d / b;
                if (C.size() < B.size() + x) C.resize(B.size() + x);
                if (2 * L > N) {
                    for (size_t i = 0; i < B.size(); ++i) C[x + i] -= c * B[i];
                    ++x;
                } else {
                    std::vector<F> T = C;
                    for (size_t i = 0; i < B.size(); ++i) C[x + i] -= c * B[i];
                    L = N + 1 - L, B = std::move(T), b = d, x = 1;
                }
            }
        }
        C.resize(L + 1);
        for (size_t N = 1; N <= L; ++N) C[N] = -C[N];
        return C;
    }
} // namespace suisen

int main() {
    using mint = suisen::modint998244353;

    int d;
    long long k;
    int m;
    io::read(d, k, m);

    std::vector<mint> a(d), c(d);
    for (int i = 0, v; i < d; ++i) std::cin >> v, a[i] = v;
    for (int i = 0, v; i < d; ++i) std::cin >> v, c[i] = v;

    std::vector<mint> ans = suisen::consecutive_terms_of_linear_recurrent_sequence(a, c, k, m);
    for (int i = 0; i < m; ++i) {
        if (i) io::print(' ');
        io::print(ans[i].val());
    }
    io::print('\n');
}

