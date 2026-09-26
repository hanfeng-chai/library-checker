/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
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
#ifndef __OY_SQRTDECOMPOSITION__
#define __OY_SQRTDECOMPOSITION__
namespace OY {
    struct SqrtDecomposition {
        struct iterator {
            struct _range {
                uint64_t m_quot, m_left, m_right;
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const _range &x) { return out << '(' << x.m_quot << ": " << x.m_left << '~' << x.m_right << ')'; }
            } m_range;
            uint64_t m_val;
            iterator &operator--() {
                if (m_range.m_quot == 1)
                    --m_range.m_quot;
                else {
                    m_range.m_left = m_range.m_right + 1;
                    m_range.m_quot = m_val / m_range.m_left;
                    if (m_range.m_quot >= m_range.m_left)
                        m_range.m_right = m_range.m_left;
                    else
                        m_range.m_right = m_val / m_range.m_quot;
                }
                return *this;
            }
            iterator &operator++() {
                if (m_range.m_quot + 1 < m_range.m_left) {
                    m_range.m_right = m_range.m_left - 1;
                    m_range.m_left = m_val / (m_range.m_quot + 2) + 1;
                    m_range.m_quot++;
                } else if (--m_range.m_left)
                    m_range.m_quot = m_val / (m_range.m_right = m_range.m_left);
                else
                    m_range.m_quot = -1;
                return *this;
            }
            const auto &operator*() const { return m_range; }
            const _range *operator->() const { return &m_range; }
            bool operator!=(const iterator &rhs) const { return m_range.m_quot != rhs.m_range.m_quot; }
            bool operator==(const iterator &rhs) const { return m_range.m_quot == rhs.m_range.m_quot; }
            static iterator null(uint64_t val) { return iterator{{uint64_t(-1)}, val}; }
        };
        struct reverse_iterator : iterator {
            iterator base() const { return *this; }
            reverse_iterator &operator--() {
                iterator::operator++();
                return *this;
            }
            reverse_iterator &operator++() {
                iterator::operator--();
                return *this;
            }
            static reverse_iterator null(uint64_t val) { return reverse_iterator{{{0, val + 1, uint64_t(-1)}, val}}; }
        };
        uint64_t m_val;
        SqrtDecomposition(uint64_t val) : m_val(val) {}
        uint64_t size() const {
            uint64_t r = std::sqrt((long double)m_val);
            return r * 2 - (m_val * 2 + 1 < (r + 1) * r * 2);
        }
        iterator lower_bound(uint64_t quot) const {
            if (!quot) return rend().base();
            if (quot > m_val) return end();
            uint64_t right = m_val / quot;
            if (quot < right) {
                uint64_t left = m_val / (quot + 1) + 1;
                return iterator{{quot, left, right}, m_val};
            } else
                return iterator{{m_val / right, right, right}, m_val};
        }
        iterator upper_bound(uint64_t quot) const { return lower_bound(quot + 1); }
        iterator find_by_divisor(uint64_t divisor) const {
            if (!divisor) return end();
            if (divisor > m_val) return rend().base();
            uint64_t quot = m_val / divisor;
            if (divisor <= quot) return iterator{{quot, divisor, divisor}, m_val};
            uint64_t right = m_val / quot;
            uint64_t left = m_val / (quot + 1) + 1;
            return iterator{{quot, left, right}, m_val};
        }
        iterator begin() const { return iterator{{1, m_val / 2 + 1, m_val}, m_val}; }
        iterator end() const { return iterator::null(m_val); }
        reverse_iterator rbegin() const { return reverse_iterator{{{m_val, 1, 1}, m_val}}; }
        reverse_iterator rend() const { return reverse_iterator::null(m_val); }
    };
    template <typename Ostream>
    Ostream &operator<<(Ostream &out, const SqrtDecomposition &x) {
        out << '{';
        for (auto i : x) {
            if (i.m_quot != 1) out << ", ";
            out << i;
        }
        return out << '}';
    }
}
#endif
/*
lib code is above
temp code is below
*/
int main() {
    uint64_t n, cnt{};
    cin >> n;
    OY::SqrtDecomposition sd(n);
    cout << sd.size() << endl;
    for (auto x : sd) cout << x.m_quot << ' ';
}
