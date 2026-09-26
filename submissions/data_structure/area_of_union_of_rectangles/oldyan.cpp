/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <vector>
#ifndef __OY_RECTUNION__
#define __OY_RECTUNION__
namespace OY {
    namespace RU {
        using size_type = uint32_t;
        template <typename Tp>
        class CoverTree {
        public:
            struct node {
                Tp m_sum, m_area;
                size_type m_tag;
                Tp get() const { return m_tag ? m_area : m_sum; }
            };
        private:
            size_type m_size, m_cap;
            std::vector<node> m_sub;
            static void _update(node *sub, size_type i) { sub[i].m_sum = sub[i * 2].get() + sub[i * 2 + 1].get(); }
            template <typename Callback>
            void _work(size_type left, size_type right, Callback &&call) {
                auto sub = m_sub.data();
                size_type cur = left;
                if (cur)
                    while (true) {
                        size_type j = std::countr_zero(cur), nxt = cur + (size_type(1) << j);
                        if (nxt > right) break;
                        call(sub + ((m_cap + cur) >> j));
                        for (size_type i = (m_cap + cur) >> j, end = nxt < right ? (m_cap + nxt) >> (std::countr_zero(nxt) + 1) : 0; (i >>= 1) != end;) _update(sub, i);
                        cur = nxt;
                    }
                while (cur < right) {
                    size_type j = std::bit_width(cur ^ right), nxt = cur + (size_type(1) << (j - 1));
                    call(sub + ((m_cap + cur) >> (j - 1)));
                    for (size_type i = (m_cap + cur) >> (j - 1), end = nxt < right ? (m_cap + cur) >> j : 0; (i >>= 1) != end;) _update(sub, i);
                    cur = nxt;
                }
            }
        public:
            CoverTree() = default;
            template <typename InitMapping>
            CoverTree(size_type length, InitMapping mapping) {
                m_cap = std::bit_ceil(m_size = length);
                m_sub.assign(m_cap * 2, {});
                for (size_type i = 0; i != m_size; i++) m_sub[m_cap + i].m_area = mapping(i);
                for (size_type len = m_cap / 2, cnt = (m_size + 1) / 2, w = 2; len; len >>= 1, cnt = (cnt + 1) / 2, w <<= 1)
                    for (size_type i = len; i != len + cnt; i++) m_sub[i].m_area = m_sub[i * 2].m_area + m_sub[i * 2 + 1].m_area;
            }
            void add(size_type left, size_type right) {
                _work(left, right, [](node *p) { ++p->m_tag; });
            }
            void remove(size_type left, size_type right) {
                _work(left, right, [](node *p) { --p->m_tag; });
            }
            Tp query_all() const { return m_sub[1].get(); }
        };
        template <typename SizeType>
        class Solver {
        public:
            struct event {
                SizeType m_time, m_low, m_high;
                bool operator<(const event &rhs) const { return m_time < rhs.m_time; }
            };
            struct pair {
                SizeType m_val;
                size_type m_index;
                bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
            };
        private:
            std::vector<event> m_es;
            std::vector<pair> m_ps;
        public:
            Solver(size_type rect_cnt = 0) { m_es.reserve(rect_cnt * 2), m_ps.reserve(rect_cnt * 2); }
            void add_rect(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) {
                m_es.push_back({x_min});
                m_es.push_back({x_max + 1});
                m_ps.push_back({y_min, (size_type)m_ps.size()});
                m_ps.push_back({y_max + 1, (size_type)m_ps.size()});
            }
            template <typename SumType = SizeType>
            SumType solve() {
                std::sort(m_ps.begin(), m_ps.end());
                std::vector<SizeType> ys;
                ys.reserve(m_ps.size());
                for (size_type i = 0; i != m_ps.size(); i++) {
                    if (!i || m_ps[i].m_val != m_ps[i - 1].m_val) ys.push_back(m_ps[i].m_val);
                    m_es[m_ps[i].m_index].m_low = m_es[m_ps[i].m_index ^ 1].m_high = ys.size() - 1;
                }
                std::sort(m_es.begin(), m_es.end());
                CoverTree<SizeType> S(ys.size() - 1, [&](size_type i) { return ys[i + 1] - ys[i]; });
                SumType h{}, last{}, ans{};
                for (auto &e : m_es) {
                    ans += h * (e.m_time - last);
                    last = e.m_time;
                    if (e.m_low < e.m_high)
                        S.add(e.m_low, e.m_high);
                    else
                        S.remove(e.m_high, e.m_low);
                    h = S.query_all();
                }
                return ans;
            }
        };
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
            char *m_p, *m_c, *m_end;
            InputHelper(FILE *file = stdin) {
#ifdef __unix__
                struct stat _stat;
                auto fd = fileno(file);
                fstat(fd, &_stat);
                m_c = m_p = (char *)mmap(nullptr, _stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
                m_end = m_p + _stat.st_size;
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
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t n;
    cin >> n;
    OY::RU::Solver<uint32_t> sol(n);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        sol.add_rect(l, r - 1, d, u - 1);
    }
    cout << sol.solve<uint64_t>() << endl;
}
