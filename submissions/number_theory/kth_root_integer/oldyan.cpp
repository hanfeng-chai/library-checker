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
#include <numeric>
#include <type_traits>
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
#ifndef __OY_KTHROOT__
#define __OY_KTHROOT__
namespace OY {
    namespace KthRoot {
        static constexpr double floor_einv[] = {0.33333333333333326, 0.24999999999999997, 0.19999999999999998, 0.16666666666666663, 0.14285714285714282, 0.12499999999999999, 0.11111111111111109, 0.099999999999999992, 0.090909090909090898, 0.083333333333333315, 0.076923076923076913, 0.071428571428571411, 0.066666666666666652, 0.062499999999999993, 0.058823529411764698, 0.055555555555555546, 0.052631578947368411, 0.049999999999999996, 0.047619047619047609, 0.045454545454545449, 0.043478260869565209, 0.041666666666666657, 0.039999999999999994, 0.038461538461538457, 0.037037037037037028, 0.035714285714285705, 0.034482758620689648, 0.033333333333333326, 0.032258064516129024};
        static constexpr double ceil_einv[] = {0.33333333333333337, 0.25000000000000006, 0.20000000000000004, 0.16666666666666669, 0.14285714285714288, 0.12500000000000003, 0.11111111111111112, 0.10000000000000002, 0.09090909090909093, 0.08333333333333334, 0.07692307692307694, 0.07142857142857144, 0.06666666666666668, 0.06250000000000001, 0.05882352941176471, 0.05555555555555556, 0.05263157894736843, 0.05000000000000001, 0.04761904761904762, 0.04545454545454546, 0.04347826086956522, 0.04166666666666667, 0.04000000000000001, 0.03846153846153847, 0.03703703703703704, 0.03571428571428572, 0.03448275862068966, 0.03333333333333334, 0.03225806451612904};
        static constexpr uint64_t pow3[] = {1853020188851841ull, 5559060566555523ull, 16677181699666569ull, 50031545098999707ull, 150094635296999121ull, 450283905890997363ull, 1350851717672992089ull, 4052555153018976267ull, 12157665459056928801ull};
        inline uint64_t fast_pow(uint64_t a, uint64_t k) {
            uint64_t res = 1;
            while (k) {
                if (k & 1) res *= a;
                k >>= 1, a *= a;
            }
            return res;
        }
        inline uint64_t floor_log(uint64_t base, uint64_t val) {
            if (base == 2) return std::bit_width(val) - 1;
            if (!(base & (base - 1))) return (std::bit_width(val) - 1) / (std::bit_width(base) - 1);
            if (base >> 32) return val >= base;
            if (base >> 22) return val < base ? 0 : 1 + (val >= base * base);
            auto v1 = std::log(val), v2 = std::log(base);
            uint64_t res = v1 / v2 + 1e-10;
            return res - (fast_pow(base, res) > val);
        }
        inline uint64_t ceil_log(uint64_t base, uint64_t val) {
            if (val == 1) return 0;
            if (val <= base) return 1;
            if (base == 2) return std::bit_width(val - 1);
            if (!(base & (base - 1))) return (std::bit_width(val - 1) - 1) / (std::bit_width(base) - 1) + 1;
            if (base >> 32) return 2;
            if (base >> 22) return 2 + (val > base * base);
            auto v1 = std::log(val), v2 = std::log(base);
            uint64_t res = v1 / v2 + 1e-10;
            return res + (fast_pow(base, res) < val);
        }
        inline uint64_t floor_iroot(uint64_t x, uint64_t k) {
            if (!x) return 0;
            if (k > 63 || !(x >> k)) return 1;
            if (k > 40) return 2;
            if (k >= 32) return 2 + (x >= pow3[k - 32]);
            if (k == 1) return x;
            if (k == 2) {
#ifdef _MSC_VER
                uint64_t res = std::sqrt(x);
                return res - (res * res > x);
#else
                return std::sqrt((long double)x);
#endif
            }
            double res = std::pow(x, floor_einv[k - 3]);
            if (fast_pow(res + 0.05, k) - 1 < x) res += 0.05;
            return res;
        }
        inline uint64_t ceil_iroot(uint64_t x, uint64_t k) {
            if (!x) return 0;
            if (x == 1) return 1;
            if (k > 63 || !((x - 1) >> k)) return 2;
            if (k > 40) return 3;
            if (k >= 32) return 3 + (x > pow3[k - 32]);
            if (k == 1) return x;
            if (k == 2) {
                uint64_t res = std::sqrt((long double)x);
                return res + (res * res < x);
            }
            uint64_t res = std::pow(x, ceil_einv[k - 3]);
            return res + (fast_pow(res, k) < x);
        }
    }
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
        uint64_t x;
        uint32_t k;
        cin >> x >> k;
        cout << OY::KthRoot::floor_iroot(x, k) << endl;
    }
}
