/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#ifndef __OY_CIPOLLA__
#define __OY_CIPOLLA__
namespace OY {
    template <typename Tp, typename ModType = typename Tp::mod_type>
    ModType Cipolla(Tp a) {
        const ModType P = Tp::mod();
        auto one = Tp::raw(1);
        if (a.pow((P - 1) / 2) != one) return 0;
        auto b = one;
        while (true) {
            Tp c = b * b - a;
            if (c.pow((P - 1) / 2) != one) break;
            b += one;
        }
        Tp neg = b * b - a;
        struct node {
            Tp a, b;
        };
        auto mul = [&](const node &x, const node &y) {
            return node{x.a * y.a + x.b * y.b * neg, x.b * y.a + x.a * y.b};
        };
        auto pow = [&](node x, ModType n) {
            node res{one, Tp{}};
            while (n) {
                if (n & 1) res = mul(res, x);
                x = mul(x, x), n >>= 1;
            }
            return res;
        };
        auto ans = pow({b, one}, (P + 1) / 2).a.val();
        return ans * 2 > P ? P - ans : ans;
    }
}
#endif
#ifndef __OY_LINUXIO__
#define __OY_LINUXIO__
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#ifdef __unix__
#include <sys/mman.h>
#include <sys/stat.h>
#endif
#if __cpp_constexpr >= 201907L
#define CONSTEXPR20 constexpr
#define INLINE20 constexpr
#else
#define CONSTEXPR20
#define INLINE20 inline
#endif
#define cin OY::LinuxIO::InputHelper<>::get_instance()
#define cout OY::LinuxIO::OutputHelper::get_instance()
#define endl '\n'
#ifndef INPUT_FILE
#define INPUT_FILE "in.txt"
#endif
#ifndef OUTPUT_FILE
#define OUTPUT_FILE "out.txt"
#endif
namespace OY {
    namespace LinuxIO {
        static constexpr size_t INPUT_BUFFER_SIZE = 1 << 26, OUTPUT_BUFFER_SIZE = 1 << 20;
#ifdef OY_LOCAL
        static constexpr char input_file[] = INPUT_FILE, output_file[] = OUTPUT_FILE;
#else
        static constexpr char input_file[] = "", output_file[] = "";
#endif
        template <typename U, size_t E>
        struct TenPow {
            static constexpr U value = TenPow<U, E - 1>::value * 10;
        };
        template <typename U>
        struct TenPow<U, 0> {
            static constexpr U value = 1;
        };
        struct InputPre {
            uint32_t m_data[0x10000];
            CONSTEXPR20 InputPre() {
                std::fill(m_data, m_data + 0x10000, -1);
                for (size_t i = 0, val = 0; i != 10; i++)
                    for (size_t j = 0; j != 10; j++) m_data[0x3030 + i + (j << 8)] = val++;
            }
        };
        struct OutputPre {
            uint32_t m_data[10000];
            CONSTEXPR20 OutputPre() {
                uint32_t *c = m_data;
                for (size_t i = 0; i != 10; i++)
                    for (size_t j = 0; j != 10; j++)
                        for (size_t k = 0; k != 10; k++)
                            for (size_t l = 0; l != 10; l++) *c++ = i + (j << 8) + (k << 16) + (l << 24) + 0x30303030;
            }
        };
        template <size_t MMAP_SIZE = 1 << 30>
        struct InputHelper {
            static INLINE20 InputPre pre{};
            struct stat m_stat;
            char *m_p, *m_c, *m_end;
            InputHelper(FILE *file = stdin) {
#ifdef __unix__
                auto fd = fileno(file);
                fstat(fd, &m_stat);
                m_c = m_p = (char *)mmap(nullptr, m_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
                m_end = m_p + m_stat.st_size;
#else
                uint32_t size = fread(m_c = m_p = new char[INPUT_BUFFER_SIZE], 1, INPUT_BUFFER_SIZE, file);
                m_end = m_p + size;
#endif
            }
            static InputHelper<MMAP_SIZE> &get_instance() {
                static InputHelper<MMAP_SIZE> s_obj(*input_file ? fopen(input_file, "rt") : stdin);
                return s_obj;
            }
            template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            InputHelper &operator>>(Tp &x) {
                x = 0;
                while (!isdigit(*m_c)) m_c++;
                x = *m_c++ ^ '0';
                while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)]) x = x * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
                if (isdigit(*m_c)) x = x * 10 + (*m_c++ ^ '0');
                return *this;
            }
            template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            InputHelper &operator>>(Tp &x) {
                typename std::make_unsigned<Tp>::type t{};
                bool sign{};
                while (!isdigit(*m_c)) sign = (*m_c++ == '-');
                t = *m_c++ ^ '0';
                while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)]) t = t * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
                if (isdigit(*m_c)) t = t * 10 + (*m_c++ ^ '0');
                x = sign ? -t : t;
                return *this;
            }
            InputHelper &operator>>(char &x) {
                while (*m_c <= ' ') m_c++;
                x = *m_c++;
                return *this;
            }
            InputHelper &operator>>(std::string &x) {
                while (*m_c <= ' ') m_c++;
                char *c = m_c;
                while (*c > ' ') c++;
                x.assign(m_c, c - m_c), m_c = c;
                return *this;
            }
            InputHelper &operator>>(std::string_view &x) {
                while (*m_c <= ' ') m_c++;
                char *c = m_c;
                while (*c > ' ') c++;
                x = std::string_view(m_c, c - m_c), m_c = c;
                return *this;
            }
        };
        struct OutputHelper {
            static INLINE20 OutputPre pre{};
            FILE *m_file;
            char m_p[OUTPUT_BUFFER_SIZE], *m_c, *m_end;
            OutputHelper(FILE *file = stdout) {
                m_file = file;
                m_c = m_p, m_end = m_p + OUTPUT_BUFFER_SIZE;
            }
            ~OutputHelper() { flush(); }
            static OutputHelper &get_instance() {
                static OutputHelper s_obj(*output_file ? fopen(output_file, "wt") : stdout);
                return s_obj;
            }
            void flush() { fwrite(m_p, 1, m_c - m_p, m_file), m_c = m_p; }
            OutputHelper &operator<<(char x) {
                if (m_end - m_c < 20) flush();
                *m_c++ = x;
                return *this;
            }
            OutputHelper &operator<<(std::string_view s) {
                if (m_end - m_c < s.size()) flush();
                memcpy(m_c, s.data(), s.size()), m_c += s.size();
                return *this;
            }
            OutputHelper &operator<<(uint64_t x) {
                if (m_end - m_c < 20) flush();
#define CASEW(w)                                                           \
    case TenPow<uint64_t, w - 1>::value... TenPow<uint64_t, w>::value - 1: \
        *(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, w - 4>::value]; \
        m_c += 4, x %= TenPow<uint64_t, w - 4>::value;
                switch (x) {
                    CASEW(19);
                    CASEW(15);
                    CASEW(11);
                    CASEW(7);
                    case 100 ... 999:
                        *(uint32_t *)m_c = pre.m_data[x * 10];
                        m_c += 3;
                        break;
                        CASEW(18);
                        CASEW(14);
                        CASEW(10);
                        CASEW(6);
                    case 10 ... 99:
                        *(uint32_t *)m_c = pre.m_data[x * 100];
                        m_c += 2;
                        break;
                        CASEW(17);
                        CASEW(13);
                        CASEW(9);
                        CASEW(5);
                    case 0 ... 9:
                        *m_c++ = '0' + x;
                        break;
                    default:
                        *(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, 16>::value];
                        m_c += 4;
                        x %= TenPow<uint64_t, 16>::value;
                        CASEW(16);
                        CASEW(12);
                        CASEW(8);
                    case 1000 ... 9999:
                        *(uint32_t *)m_c = pre.m_data[x];
                        m_c += 4;
                        break;
                }
#undef CASEW
                return *this;
            }
            OutputHelper &operator<<(uint32_t x) {
                if (m_end - m_c < 20) flush();
#define CASEW(w)                                                           \
    case TenPow<uint32_t, w - 1>::value... TenPow<uint32_t, w>::value - 1: \
        *(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, w - 4>::value]; \
        m_c += 4, x %= TenPow<uint32_t, w - 4>::value;
                switch (x) {
                    default:
                        *(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, 6>::value];
                        m_c += 4;
                        x %= TenPow<uint32_t, 6>::value;
                        CASEW(6);
                    case 10 ... 99:
                        *(uint32_t *)m_c = pre.m_data[x * 100];
                        m_c += 2;
                        break;
                        CASEW(9);
                        CASEW(5);
                    case 0 ... 9:
                        *m_c++ = '0' + x;
                        break;
                        CASEW(8);
                    case 1000 ... 9999:
                        *(uint32_t *)m_c = pre.m_data[x];
                        m_c += 4;
                        break;
                        CASEW(7);
                    case 100 ... 999:
                        *(uint32_t *)m_c = pre.m_data[x * 10];
                        m_c += 3;
                        break;
                }
#undef CASEW
                return *this;
            }
            OutputHelper &operator<<(int64_t x) {
                if (x >= 0)
                    return (*this) << uint64_t(x);
                else
                    return (*this) << '-' << uint64_t(-x);
            }
            OutputHelper &operator<<(int32_t x) {
                if (x >= 0)
                    return (*this) << uint32_t(x);
                else
                    return (*this) << '-' << uint32_t(-x);
            }
        };
    }
}
#endif
#ifndef __OY_DYNAMICMONTGOMERYMODINT32__
#define __OY_DYNAMICMONTGOMERYMODINT32__
namespace OY {
    template <size_t Id>
    struct DynamicMontgomeryModInt32 {
        using mint = DynamicMontgomeryModInt32<Id>;
        using mod_type = uint32_t;
        using fast_type = mod_type;
        using long_type = uint64_t;
        struct Info {
            mod_type m_mod, m_mod2, m_pinv, m_ninv;
            fast_type m_one;
            uint64_t m_inv;
            bool m_is_prime;
            void set_mod(mod_type P, bool is_prime = false) {
                assert(P % 2 && P > 1 && P < mod_type(1) << 30);
                m_mod = m_pinv = P, m_mod2 = P * 2, m_ninv = -long_type(P) % P, m_inv = uint64_t(-1) / P + 1, m_is_prime = is_prime;
                for (size_t i = 0; i != 4; i++) m_pinv *= mod_type(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            mod_type mod(uint64_t val) const {
#ifdef _MSC_VER
                uint64_t x;
                _umul128(val, m_inv, &x);
                mod_type res = val - x * mod();
#else
                mod_type res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
#endif
                if (res >= mod()) res += mod();
                return res;
            }
            mod_type mod() const { return m_mod; }
            fast_type reduce(long_type val) const { return (val + long_type(mod_type(val) * m_pinv) * mod()) >> 32; }
            fast_type reduce_zero(mod_type x) const { return x == mod() ? 0 : x; }
            fast_type strict_reduce(fast_type val) const { return val >= mod() ? val - mod() : val; }
            fast_type add(fast_type a, fast_type b) const {
                fast_type val1 = a + b, val2 = val1 - m_mod2;
                return int32_t(val2) < 0 ? val1 : val2;
            }
            fast_type sub(fast_type a, fast_type b) const {
                fast_type val1 = a + m_mod2 - b, val2 = val1 - m_mod2;
                return int32_t(val2) < 0 ? val1 : val2;
            }
            fast_type neg(fast_type val) const { return val ? m_mod2 - val : 0; }
            fast_type mul(fast_type a, fast_type b) const { return reduce(long_type(a) * b); }
            fast_type pow(fast_type a, uint64_t n) const {
                fast_type res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            fast_type one() const { return m_one; }
            fast_type raw_init(mod_type val) const { return mul(val, m_ninv); }
            mod_type val(fast_type _val) const { return reduce_zero(reduce(_val)); }
            bool is_prime() const { return m_is_prime; }
        };
        static Info s_info;
        fast_type m_val;
        static mod_type _mod(uint64_t val) { return s_info.mod(val); }
        static fast_type _init(uint64_t val) { return _raw_init(_mod(val)); }
        static fast_type _raw_init(mod_type val) { return s_info.raw_init(val); }
        static fast_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static fast_type _mul(fast_type a, fast_type b) { return s_info.mul(a, b); }
        DynamicMontgomeryModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt32(Tp val) : m_val(_raw_init(_reduce_norm(val % int32_t(mod())))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        DynamicMontgomeryModInt32(Tp val) : m_val{val < mod() ? _raw_init(val) : _init(val)} {}
        static mint _raw(fast_type val) {
            mint res;
            res.m_val = val;
            return res;
        }
        static mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static void set_mod(mod_type P, bool is_prime = false) { s_info.set_mod(P, is_prime); }
        static mod_type mod() { return s_info.mod(); }
        mod_type val() const { return s_info.val(m_val); }
        mint pow(uint64_t n) const { return _raw(s_info.pow(m_val, n)); }
        mint inv() const { return s_info.is_prime() ? inv_Fermat() : inv_exgcd(); }
        mint inv_exgcd() const {
            mod_type x = mod(), y = val(), m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return m0;
        }
        mint inv_Fermat() const { return pow(mod() - 2); }
        mint &operator++() {
            (*this) += _raw(s_info.one());
            return *this;
        }
        mint &operator--() {
            (*this) -= _raw(s_info.one());
            return *this;
        }
        mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        mint &operator+=(const mint &rhs) {
            m_val = s_info.add(m_val, rhs.m_val);
            return *this;
        }
        mint &operator-=(const mint &rhs) {
            m_val = s_info.sub(m_val, rhs.m_val);
            return *this;
        }
        mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return _raw(s_info.neg(m_val)); }
        bool operator==(const mint &rhs) const { return s_info.strict_reduce(m_val) == s_info.strict_reduce(rhs.m_val); }
        bool operator!=(const mint &rhs) const { return s_info.strict_reduce(m_val) != s_info.strict_reduce(rhs.m_val); }
        bool operator<(const mint &rhs) const { return s_info.strict_reduce(m_val) < s_info.strict_reduce(rhs.m_val); }
        bool operator>(const mint &rhs) const { return s_info.strict_reduce(m_val) > s_info.strict_reduce(rhs.m_val); }
        bool operator<=(const mint &rhs) const { return s_info.strict_reduce(m_val) <= s_info.strict_reduce(rhs.m_val); }
        bool operator>=(const mint &rhs) const { return s_info.strict_reduce(m_val) >= s_info.strict_reduce(rhs.m_val); }
        template <typename Tp>
        explicit operator Tp() const { return Tp(val()); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <size_t Id>
    typename DynamicMontgomeryModInt32<Id>::Info DynamicMontgomeryModInt32<Id>::s_info;
    template <typename Istream, size_t Id>
    Istream &operator>>(Istream &is, DynamicMontgomeryModInt32<Id> &x) {
        uint32_t val;
        is >> val;
        x.m_val = DynamicMontgomeryModInt32<Id>::_raw_init(val);
        return is;
    }
    template <typename Ostream, size_t Id>
    Ostream &operator<<(Ostream &os, const DynamicMontgomeryModInt32<Id> &x) { return os << x.val(); }
}
#endif
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t y, p;
        cin >> y >> p;
        if (y < 2)
            cout << y << endl;
        else {
            using mint = OY::DynamicMontgomeryModInt32<0>;
            mint::set_mod(p, true);
            auto res = OY::Cipolla(mint(y));
            if (res)
                cout << res << endl;
            else
                cout << "-1\n";
        }
    }
}
