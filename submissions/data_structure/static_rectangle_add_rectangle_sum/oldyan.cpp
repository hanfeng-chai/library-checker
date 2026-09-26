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
        constexpr bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        constexpr bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        constexpr bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        constexpr bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
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
/*
lib code is above
temp code is below
*/
namespace OY {
    namespace OFFLINERARSC2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType>
        struct Rect {
            SizeType m_x[2], m_y[2];
            WeightType m_w;
            WeightType weight() const { return m_w; }
        };
        template <typename SizeType>
        struct Rect<SizeType, bool> {
            SizeType m_x[2], m_y[2];
            static constexpr bool weight() { return true; }
        };
        template <typename SizeType>
        struct Query {
            SizeType m_x[2], m_y[2], m_y2[2];
        };
        template <typename SizeType, typename WeightType, typename SumType = WeightType>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using rect = Rect<SizeType, WeightType>;
            using query = Query<SizeType>;
            std::vector<rect> m_rects;
            std::vector<query> m_queries;
            std::vector<SizeType> m_sorted_ys;
            Table(size_type rect_cnt = 0, size_type query_cnt = 0) { m_rects.reserve(rect_cnt), m_queries.reserve(query_cnt); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max, WeightType w = 1) {
                if constexpr (is_bool)
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max});
                else
                    m_rects.push_back({x_min, x_max + 1, y_min, y_max, w});
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) { m_queries.push_back({x_min, x_max + 1, y_min, y_max + 1}); }
            std::vector<SumType> solve() {
                struct pair {
                    SizeType m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                struct node {
                    SumType m_val[4];
                    node &operator+=(const node &rhs) {
                        m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1], m_val[2] += rhs.m_val[2], m_val[3] += rhs.m_val[3];
                        return *this;
                    }
                };
                std::vector<pair> ps(m_rects.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_y[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_y[1] + 1, i * 2 + 1};
                std::sort(ps.begin(), ps.end());
                m_sorted_ys.reserve(ps.size());
                for (size_type i = 0; i != ps.size(); i++) {
                    if (!i || ps[i].m_val != ps[i - 1].m_val) m_sorted_ys.push_back(ps[i].m_val);
                    m_rects[ps[i].m_index >> 1].m_y[ps[i].m_index & 1] = m_sorted_ys.size() - 1;
                }
                std::vector<pair> qs(m_queries.size() * 2);
                for (size_type i = 0; i != m_rects.size(); i++) ps[i * 2] = {m_rects[i].m_x[0], i * 2}, ps[i * 2 + 1] = {m_rects[i].m_x[1], i * 2 + 1};
                for (size_type i = 0; i != m_queries.size(); i++) {
                    m_queries[i].m_y2[0] = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), m_queries[i].m_y[0]) - m_sorted_ys.begin();
                    m_queries[i].m_y2[1] = std::lower_bound(m_sorted_ys.begin(), m_sorted_ys.end(), m_queries[i].m_y[1]) - m_sorted_ys.begin();
                    qs[i * 2] = {m_queries[i].m_x[0], i * 2}, qs[i * 2 + 1] = {m_queries[i].m_x[1], i * 2 + 1};
                }
                std::sort(ps.begin(), ps.end());
                std::sort(qs.begin(), qs.end());
                pair *p = ps.data(), *pend = ps.data() + ps.size();
                std::vector<node> sum(m_sorted_ys.size() + 1);
                std::vector<SumType> res(m_queries.size());
                auto add = [&](size_type i, const node &inc) {
                    for (; i < sum.size(); i += (i + 1) & (-i - 1)) sum[i] += inc;
                };
                auto presum = [&](size_type i) {
                    node res{};
                    for (; ~i; i -= (i + 1) & (-i - 1)) res += sum[i];
                    return res;
                };
                for (auto &q : qs) {
                    for (; p != pend && p->m_val < q.m_val; p++) {
                        auto &rect = m_rects[p->m_index >> 1];
                        size_type d = p->m_index & 1;
                        SumType l0 = m_sorted_ys[rect.m_y[0]], r0 = m_sorted_ys[rect.m_y[1]];
                        if (d) {
                            SumType w = -(SumType)rect.weight(), w2 = -w * rect.m_x[1];
                            add(rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                            add(rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                        } else {
                            SumType w = rect.weight(), w2 = -w * rect.m_x[0];
                            add(rect.m_y[0], {-l0 * w2, w2, -l0 * w, w});
                            add(rect.m_y[1], {r0 * w2, -w2, r0 * w, -w});
                        }
                    }
                    auto &qr = m_queries[q.m_index >> 1];
                    auto s1 = presum(qr.m_y2[0] - 1), s2 = presum(qr.m_y2[1] - 1);
                    SumType a = s2.m_val[0] + (SumType)s2.m_val[1] * qr.m_y[1] - (s1.m_val[0] + (SumType)s1.m_val[1] * qr.m_y[0]);
                    SumType b = s2.m_val[2] + (SumType)s2.m_val[3] * qr.m_y[1] - (s1.m_val[2] + (SumType)s1.m_val[3] * qr.m_y[0]);
                    if (q.m_index & 1)
                        res[q.m_index >> 1] += a + m_queries[q.m_index >> 1].m_x[1] * b;
                    else
                        res[q.m_index >> 1] -= a + m_queries[q.m_index >> 1].m_x[0] * b;
                }
                return res;
            }
        };
    }
}
int main() {
    using mint = OY::mgint998244353;
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINERARSC2D::Table<uint32_t, mint, mint> S(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t l, d, r, u;
        mint w;
        cin >> l >> d >> r >> u >> w;
        S.add_rect(l, r - 1, d, u - 1, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        S.add_query(l, r - 1, d, u - 1);
    }
    auto res = S.solve();
    for (auto a : res) cout << a << endl;
}
