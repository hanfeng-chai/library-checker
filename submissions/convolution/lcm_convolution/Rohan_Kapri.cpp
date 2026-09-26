#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <numeric>
#include <type_traits>
#include <vector>
#ifndef __OY_STATICMONTGOMERYMODINT32__
#define __OY_STATICMONTGOMERYMODINT32__
#if __cpp_constexpr >= 201304L
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif
namespace OY {
    template <uint32_t P, bool IsPrime, typename = typename std::enable_if<(P % 2 && P > 1 && P < uint32_t(1) << 30)>::type>
    struct StaticMontgomeryModInt32 {
        using mint = StaticMontgomeryModInt32<P, IsPrime>;
        using mod_type = uint32_t;
        using fast_type = mod_type;
        using long_type = uint64_t;
        static constexpr mod_type _conv(mod_type x) { return x * (mod_type(2) - P * x); }
        static constexpr mod_type pinv = -_conv(_conv(_conv(_conv(P))));
        static constexpr mod_type ninv = -long_type(P) % P;
        fast_type m_val;
        static constexpr mod_type _mod(uint64_t val) { return val % mod(); }
        static constexpr fast_type _init(uint64_t val) { return _raw_init(_mod(val)); }
        static constexpr fast_type _raw_init(mod_type val) { return _mul(val, ninv); }
        static constexpr fast_type _reduce(long_type val) { return (val + long_type(mod_type(val) * pinv) * mod()) >> 32; }
        static constexpr fast_type _reduce_zero(mod_type x) { return x == mod() ? 0 : x; }
        static constexpr fast_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static constexpr fast_type _strict_reduce(fast_type val) { return val >= mod() ? val - mod() : val; }
        static constexpr fast_type _mul(fast_type a, fast_type b) { return _reduce(long_type(a) * b); }
        constexpr StaticMontgomeryModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        constexpr StaticMontgomeryModInt32(Tp val) : m_val(_raw_init(_reduce_norm(val % int32_t(mod())))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        constexpr StaticMontgomeryModInt32(Tp val) : m_val{_init(val)} {}
        static CONSTEXPR14 mint _raw(fast_type val) {
            mint res{};
            res.m_val = val;
            return res;
        }
        static constexpr mint raw(mod_type val) { return _raw(_raw_init(val)); }
        static constexpr mod_type mod() { return P; }
        constexpr mod_type val() const { return _reduce_zero(_reduce(m_val)); }
        CONSTEXPR14 mint pow(uint64_t n) const {
            fast_type res = _raw_init(1), b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return _raw(res);
        }
        CONSTEXPR14 mint inv() const {
            if constexpr (IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        CONSTEXPR14 mint inv_exgcd() const {
            mod_type x = mod(), y = val(), m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return m0;
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        CONSTEXPR14 mint &operator++() {
            (*this) += raw(1);
            return *this;
        }
        CONSTEXPR14 mint &operator--() {
            (*this) += raw(mod() - 1);
            return *this;
        }
        CONSTEXPR14 mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        CONSTEXPR14 mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        CONSTEXPR14 mint &operator+=(const mint &rhs) {
            fast_type val1 = m_val + rhs.m_val, val2 = val1 - mod() * 2;
            m_val = int32_t(val2) < 0 ? val1 : val2;
            return *this;
        }
        CONSTEXPR14 mint &operator-=(const mint &rhs) {
            fast_type val1 = m_val - rhs.m_val, val2 = val1 + mod() * 2;
            m_val = int32_t(val1) < 0 ? val2 : val1;
            return *this;
        }
        CONSTEXPR14 mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        CONSTEXPR14 mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return _raw(m_val ? mod() * 2 - m_val : 0); }
        constexpr bool operator==(const mint &rhs) const { return _strict_reduce(m_val) == _strict_reduce(rhs.m_val); }
        constexpr bool operator!=(const mint &rhs) const { return _strict_reduce(m_val) != _strict_reduce(rhs.m_val); }
        constexpr bool operator<(const mint &rhs) const { return _strict_reduce(m_val) < _strict_reduce(rhs.m_val); }
        constexpr bool operator>(const mint &rhs) const { return _strict_reduce(m_val) > _strict_reduce(rhs.m_val); }
        constexpr bool operator<=(const mint &rhs) const { return _strict_reduce(m_val) <= _strict_reduce(rhs.m_val); }
        constexpr bool operator>=(const mint &rhs) const { return _strict_reduce(m_val) >= _strict_reduce(rhs.m_val); }
        template <typename Tp>
        constexpr explicit operator Tp() const { return Tp(val()); }
        friend CONSTEXPR14 mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend CONSTEXPR14 mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend CONSTEXPR14 mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend CONSTEXPR14 mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <typename Istream, uint32_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticMontgomeryModInt32<P, IsPrime> &x) {
        uint32_t val;
        is >> val;
        x.m_val = StaticMontgomeryModInt32<P, IsPrime>::_raw_init(val);
        return is;
    }
    template <typename Ostream, uint32_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticMontgomeryModInt32<P, IsPrime> &x) { return os << x.val(); }
    using mgint998244353 = StaticMontgomeryModInt32<998244353, true>;
    using mgint1000000007 = StaticMontgomeryModInt32<1000000007, true>;
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
#ifndef __OY_FASTTRANSFORM__
#define __OY_FASTTRANSFORM__
namespace OY {
    namespace FASTTRANS {
        using size_type = uint32_t;
        struct DivideBy2_naive {
            template <typename Tp>
            Tp operator()(Tp x) const { return x / 2; }
        };
        struct FastTranformer {
            size_type m_length;
            std::vector<std::pair<size_type, size_type>> m_transfers;
            FastTranformer(size_type length, size_type transfer_cnt) : m_length(length) { m_transfers.reserve(transfer_cnt); }
            void add_transfer(size_type prev, size_type next) { m_transfers.emplace_back(prev, next); }
            template <bool Forward, typename Sequence>
            void transform(Sequence &sequence) {
                if constexpr (Forward)
                    for (auto &[prev, next] : m_transfers) {
                        sequence[next] += sequence[prev];
                    }
                else
                    for (auto it = m_transfers.rbegin(); it != m_transfers.rend(); ++it) {
                        auto &[prev, next] = *it;
                        sequence[next] -= sequence[prev];
                    }
            }
        };
        template <bool Forward, typename Iterator, typename DivideBy2 = DivideBy2_naive>
        void fast_bitxor_transform(Iterator first, Iterator last, DivideBy2 div_by2 = DivideBy2()) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y, *it2 = x - y;
                        else
                            *it = div_by2(x + y), *it2 = div_by2(x - y);
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_bitor_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it2 = x + y;
                        else
                            *it2 = y - x;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_bitand_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y;
                        else
                            *it = x - y;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_max_transform(Iterator first, Iterator last) {
            if constexpr (Forward)
                std::partial_sum(first, last, first);
            else
                std::adjacent_difference(first, last, first);
        }
        template <bool Forward, typename Iterator>
        void fast_min_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            if constexpr (Forward)
                for (size_type i = length - 2; ~i; i--) *(first + i) += *(first + i + 1);
            else
                for (size_type i = 0; i + 1 < length; i++) *(first + i) -= *(first + i + 1);
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_gcd_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = (length - 1) / p; j; j--) *(first + j) += *(first + j * p);
                else
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j) -= *(first + j * p);
            }
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_lcm_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j * p) += *(first + j);
                else
                    for (size_type j = (length - 1) / p; j; j--) *(first + j * p) -= *(first + j);
            }
        }
    }
}
#endif
#ifndef __OY_FASTSIEVE__
#define __OY_FASTSIEVE__
namespace OY {
    namespace FASTSIEVE {
        using size_type = uint32_t;
        using mask_type = unsigned char;
        struct SieveNode {
            size_type m_val, m_pos[8];
        };
        static constexpr size_type remainder_30[8] = {1, 7, 11, 13, 17, 19, 23, 29};
        static constexpr size_type block_size = 7 * 11 * 13 * 17;
        constexpr size_type get_estimated_ln(size_type x) {
            return x <= 7            ? 1
                   : x <= 32         ? 2
                   : x <= 119        ? 3
                   : x <= 359        ? 4
                   : x <= 1133       ? 5
                   : x <= 3093       ? 6
                   : x <= 8471       ? 7
                   : x <= 24299      ? 8
                   : x <= 64719      ? 9
                   : x <= 175196     ? 10
                   : x <= 481451     ? 11
                   : x <= 1304718    ? 12
                   : x <= 3524653    ? 13
                   : x <= 9560099    ? 14
                   : x <= 25874783   ? 15
                   : x <= 70119984   ? 16
                   : x <= 189969353  ? 17
                   : x <= 514278262  ? 18
                   : x <= 1394199299 ? 19
                                     : 20;
        }
        constexpr size_type get_estimated_Pi(size_type x) { return x / get_estimated_ln(x); }
        constexpr size_type get_estimated_sqrt_Pi(size_type x) { return 5 << size_type(get_estimated_ln(x) * 0.55); }
        template <size_type MAX_RANGE>
        struct Sieve {
            static constexpr size_type max_r = (get_estimated_sqrt_Pi(MAX_RANGE) + block_size * 2 - 1) / (block_size * 2) * block_size * 2, max_pi = get_estimated_Pi(MAX_RANGE);
            size_type m_primes[max_pi], m_prime_cnt;
            mask_type m_masks[block_size], m_buffer[block_size];
            SieveNode m_nodes[max_r];
            template <typename Callback>
            void _init_sieve(size_type range, Callback &&call) const {
                if (range < 19) return;
                std::vector<bool> vis(range + 1);
                for (size_type i = 3; i * i <= range; i += 2)
                    if (!vis[i])
                        for (size_type j = i * i; j <= range; j += i << 1) vis[j] = true;
                for (size_type i = 19; i <= range; i += 2)
                    if (!vis[i]) call(i);
            }
            void _add_prime(size_type p) { m_primes[m_prime_cnt++] = p; }
            Sieve(size_type range = MAX_RANGE) {
                for (size_type p : {2, 3, 5, 7, 11, 13, 17})
                    if (p <= range) _add_prime(p);
                if (range <= block_size) {
                    _init_sieve(range, [&](size_type i) { _add_prime(i); });
                    return;
                }
                std::fill_n(m_masks, block_size, -1);
                for (size_type p : {7, 11, 13, 17})
                    for (size_type i = 0; i != 8; i++) {
                        size_type j = p;
                        while (j % 30 != remainder_30[i]) j += p << 1;
                        for (j /= 30; j < block_size; j += p) m_masks[j] &= ~(1 << i);
                    }
                SieveNode *end = m_nodes;
                auto add_node = [&](size_type p) {
                    for (size_type i = 0; i != 8; i++) {
                        size_type j = p * p;
                        while (j % 30 != remainder_30[i]) j += p << 1;
                        end->m_pos[i] = j / 30;
                    }
                    end++->m_val = p;
                };
                _init_sieve(sqrt(range), add_node);
                for (size_type first = 0, tot = (range - 1) / 30 + 1; first < tot; first += block_size) {
                    size_type last = std::min(tot, first + block_size);
                    std::copy_n(m_masks, block_size, m_buffer);
                    if (!first) m_buffer[0] &= 0xfe;
                    for (auto it = m_nodes; it != end; ++it)
                        for (size_type p = it->m_val, i = 0; i != 8; i++) {
                            size_type j = it->m_pos[i];
                            for (; j < last; j += p) m_buffer[j - first] &= ~(1 << i);
                            it->m_pos[i] = j;
                        }
                    for (size_type i = first; i != last; i++)
                        for (mask_type mask = m_buffer[i - first]; mask;) {
                            size_type x = std::countr_zero(mask);
                            _add_prime(i * 30 + remainder_30[x]), mask -= size_type(1) << x;
                        }
                }
                while (m_primes[m_prime_cnt - 1] > range) m_prime_cnt--;
            }
            std::bitset<MAX_RANGE + 1> to_bitset() const {
                std::bitset<MAX_RANGE + 1> res;
                for (size_type i = 0; i != m_prime_cnt; i++) res.set(m_primes[i]);
                return res;
            }
            size_type query_kth_prime(size_type k) const { return m_primes[k]; }
            size_type count() const { return m_prime_cnt; }
        };
    }
}
#endif
/*
lib code is above
temp code is below
*/
OY::FASTSIEVE::Sieve<1000000> ps;
int main() {
    using mint = OY::mgint998244353;
    uint32_t n;
    cin >> n;
    std::vector<mint> a(n + 1), b(n + 1);
    for (uint32_t i = 1; i <= n; i++) cin >> a[i];
    for (uint32_t i = 1; i <= n; i++) cin >> b[i];
    auto find = [](uint32_t k) { return ps.query_kth_prime(k); };
    OY::FASTTRANS::fast_lcm_transform<true>(a.begin(), a.end(), find);
    OY::FASTTRANS::fast_lcm_transform<true>(b.begin(), b.end(), find);
    for (uint32_t i = 0; i <= n; i++) a[i] *= b[i];
    OY::FASTTRANS::fast_lcm_transform<false>(a.begin(), a.end(), find);
    for (uint32_t i = 1; i <= n; i++) cout << a[i] << ' ';
}
