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
#ifndef __OY_OFFLINEMODE__
#define __OY_OFFLINEMODE__
namespace OY {
    namespace OFFLINEMODE {
        using size_type = uint32_t;
        enum ResultTag {
            FREQUENCY = 0,
            ELEM_FREQUENCY = 1,
            MINELEM_FREQUENCY = 2
        };
        template <typename Tp, ResultTag T>
        struct Result {
            size_type m_freq;
        };
        template <typename Tp>
        struct Result<Tp, ELEM_FREQUENCY> {
            Tp m_elem;
            size_type m_freq;
        };
        template <typename Tp>
        struct Result<Tp, MINELEM_FREQUENCY> {
            Tp m_min_elem;
            size_type m_freq;
        };
        template <typename Tp>
        struct Solver {
            struct Query {
                size_type m_id, m_left, m_right;
            };
            std::vector<Tp> m_sorted;
            std::vector<size_type> m_arr;
            mutable std::vector<Query> m_queries;
            Solver() = default;
            template <typename InitMapping>
            Solver(size_type length, InitMapping mapping, size_type query_cnt = 0) { resize(length, mapping, query_cnt); }
            template <typename Iterator>
            Solver(Iterator first, Iterator last, size_type query_cnt = 0) { reset(first, last, query_cnt); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping, size_type query_cnt = 0) {
                struct pair {
                    Tp m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                std::vector<pair> ps(length);
                for (size_type i = 0; i != length; i++) ps[i] = {Tp(mapping(i)), i};
                std::sort(ps.begin(), ps.end());
                m_sorted.reserve(ps.size());
                m_arr.resize(length);
                for (size_type i = 0; i != length; i++) {
                    if (!i || ps[i - 1].m_val < ps[i].m_val) m_sorted.push_back(ps[i].m_val);
                    m_arr[ps[i].m_index] = m_sorted.size() - 1;
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, size_type query_cnt = 0) {
                resize(last - first, [&](size_type i) { return *(first + i); }, query_cnt);
            }
            void add_query(size_type left, size_type right) { m_queries.push_back({(size_type)m_queries.size(), left, right + 1}); }
            std::vector<size_type> solve1() const {
                const size_type block_size = sqrt(m_arr.size() * 3);
                std::sort(m_queries.begin(), m_queries.end(), [block_size](const Query &x, const Query &y) {
                    size_type l = x.m_left / block_size, r = y.m_left / block_size;
                    if (l != r)
                        return l < r;
                    else if (l & 1)
                        return x.m_right > y.m_right;
                    else
                        return x.m_right < y.m_right;
                });
                std::vector<size_type> cnt_buf(m_sorted.size() + m_arr.size() + 1);
                const size_type *arr = m_arr.data();
                size_type *cnt1 = cnt_buf.data(), *cnt2 = cnt_buf.data() + m_sorted.size();
                size_type l = 0, r = 0, mx = 0, c;
                auto add = [&](size_type i) {
                    size_type &cnt = cnt1[arr[i]];
                    --cnt2[cnt];
                    ++cnt2[++cnt] > mx;
                    if (cnt > mx) mx = cnt, c = arr[i];
                };
                auto remove = [&](size_type i) {
                    size_type &cnt = cnt1[arr[i]];
                    if (cnt == mx && cnt2[cnt] == 1) --mx;
                    --cnt2[cnt];
                    ++cnt2[--cnt];
                };
                std::vector<size_type> ans(m_queries.size());
                for (auto &q : m_queries) {
                    size_type id = q.m_id, left = q.m_left, right = q.m_right;
                    while (l > left) add(--l);
                    while (r < right) add(r++);
                    while (r > right) remove(--r);
                    while (l < left) remove(l++);
                    ans[id] = mx;
                }
                return ans;
            }
            std::vector<Result<Tp, ELEM_FREQUENCY>> solve2() const {
                const size_type block_size = std::max<size_type>(1, m_arr.size() / sqrt(m_queries.size()));
                std::sort(m_queries.begin(), m_queries.end(), [block_size](const Query &x, const Query &y) {
                    size_type l = x.m_left / block_size, r = y.m_left / block_size;
                    if (l != r)
                        return l < r;
                    else
                        return x.m_right < y.m_right;
                });
                std::vector<size_type> _cnt(m_sorted.size());
                const size_type *arr = m_arr.data();
                size_type *cnt = _cnt.data();
                size_type l = block_size - 1, r = block_size - 1, mx, c, base_c = 0, base_mx = 0;
                auto add = [&](size_type i) {
                    auto &_cnt = ++cnt[arr[i]];
                    if (_cnt > mx) mx = _cnt, c = arr[i];
                };
                auto remove = [&](size_type i) {
                    --cnt[arr[i]];
                };
                std::vector<Result<Tp, ELEM_FREQUENCY>> ans(m_queries.size());
                for (size_type i = 0, j; i != m_queries.size();) {
                    size_type left = m_queries[i].m_left, start = left / block_size * block_size;
                    for (j = i + 1; j != m_queries.size() && m_queries[j].m_left < start + block_size; j++) {}
                    l = r = start + block_size - 1, base_c = base_mx = 0;
                    for (; i != j && m_queries[i].m_right <= start + block_size; i++) {
                        size_type id = m_queries[i].m_id, left = m_queries[i].m_left, right = m_queries[i].m_right;
                        c = mx = 0;
                        for (size_type i = left; i != right; i++) add(i);
                        ans[id] = {m_sorted[c], mx};
                        for (size_type i = left; i != right; i++) remove(i);
                    }
                    for (; i != j; i++) {
                        size_type id = m_queries[i].m_id, left = m_queries[i].m_left, right = m_queries[i].m_right;
                        while (l != start + block_size - 1) remove(l++);
                        c = base_c, mx = base_mx;
                        while (r != right) add(r++);
                        base_c = c, base_mx = mx;
                        while (l != left) add(--l);
                        ans[id] = {m_sorted[c], mx};
                    }
                    std::fill_n(cnt, m_sorted.size(), 0);
                }
                return ans;
            };
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
    OY::OFFLINEMODE::Solver<uint32_t> sol(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x; }, q);
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        sol.add_query(l, r - 1);
    }
    for (auto a : sol.solve2())
        cout << a.m_elem << ' ' << a.m_freq << endl;
}
