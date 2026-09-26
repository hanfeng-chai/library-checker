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
#include <vector>
#ifndef __OY_RANGEMODE__
#define __OY_RANGEMODE__
namespace OY {
    template <typename Tp>
    struct RangeMode {
        using size_type = uint32_t;
        template <typename Fp>
        struct Pair {
            size_type m_val, m_cnt;
            bool operator<(const Pair<Fp> &rhs) const { return m_cnt < rhs.m_cnt; }
        };
        size_type m_size, m_block_size, m_block_cnt;
        std::vector<Tp> m_sorted;
        std::vector<size_type> m_arr, m_pos, m_start, m_pos2;
        std::vector<Pair<size_type>> m_ps;
        template <typename InitMapping>
        RangeMode(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            if (!(m_size = length)) return;
            std::vector<Pair<Tp>> ps(m_size);
            for (size_type i = 0; i != m_size; i++) ps[i] = {Tp(mapping(i)), i};
            std::sort(ps.begin(), ps.end(), [](const Pair<Tp> &x, const Pair<Tp> &y) { return x.m_val < y.m_val; });
            m_sorted.reserve(m_size);
            m_arr.resize(m_size);
            for (size_type i = 0; i != m_size; i++) {
                if (!i || ps[i].m_val != ps[i - 1].m_val) m_sorted.push_back(ps[i].m_val);
                m_arr[ps[i].m_cnt] = m_sorted.size() - 1;
            }
            m_pos.resize(m_size);
            std::vector<size_type> cnt(m_size + 1);
            for (size_type i = 0; i != m_size; i++) cnt[m_arr[i] + 1]++;
            for (size_type i = 0; i != m_sorted.size(); i++) cnt[i + 1] += cnt[i];
            m_start = cnt;
            m_pos2.resize(m_size);
            for (size_type i = 0; i != m_size; i++) {
                size_type pos = cnt[m_arr[i]]++;
                m_pos[pos] = i, m_pos2[i] = pos;
            }
            std::fill_n(cnt.data(), m_sorted.size(), 0);
            m_block_size = m_sorted.size() <= 64 ? std::bit_ceil<size_type>(sqrt(m_size)) : std::bit_floor<size_type>(sqrt(m_size)), m_block_cnt = (m_size + m_block_size - 1) / m_block_size;
            m_ps.resize(m_block_cnt * m_block_cnt);
            for (size_type l = 0; l != m_block_cnt; l++) {
                Pair<size_type> p{};
                std::fill_n(cnt.data(), m_sorted.size(), 0);
                for (size_type r = l; r != m_block_cnt; r++) {
                    size_type p1 = r * m_block_size, p2 = std::min((r + 1) * m_block_size, m_size);
                    for (size_type i = p1; i != p2; i++)
                        if (++cnt[m_arr[i]] > p.m_cnt) p = {m_arr[i], cnt[m_arr[i]]};
                    m_ps[l * m_block_cnt + r] = p;
                }
            }
        }
        Pair<Tp> query(size_type left, size_type right) const {
            size_type l = (left + m_block_size - 1) / m_block_size, r = (right + 1) / m_block_size - 1, elem{}, cnt{};
            if (l < r + 1) elem = m_ps[l * m_block_cnt + r].m_val, cnt = m_ps[l * m_block_cnt + r].m_cnt;
            for (size_type i = left, j = std::min(l * m_block_size, right + 1); i != j; i++) {
                size_type c = m_arr[i], pos = m_pos2[i];
                if (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right) {
                    do cnt++;
                    while (pos + cnt < m_start[c + 1] && m_pos[pos + cnt] <= right);
                    elem = c;
                    if (cnt * 2 >= right - left + 1) return {m_sorted[elem], cnt};
                }
            }
            if (right >= l * m_block_size)
                for (size_type i = right, j = std::max((r + 1) * m_block_size - 1, left - 1); i != j; i--) {
                    size_type c = m_arr[i], pos = m_pos2[i];
                    if (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left) {
                        do cnt++;
                        while (cnt <= pos - m_start[c] && m_pos[pos - cnt] >= left);
                        elem = c;
                        if (cnt * 2 >= right - left + 1) return {m_sorted[elem], cnt};
                    }
                }
            return {m_sorted[elem], cnt};
        }
    };
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
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::RangeMode<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        auto res = S.query(l, r - 1);
        cout << res.m_val << ' ' << res.m_cnt << endl;
    }
}
