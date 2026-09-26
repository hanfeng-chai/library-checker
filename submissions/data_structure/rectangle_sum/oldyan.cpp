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
#ifndef __OY_OFFLINEPOINTADDRECTSUMCOUNTER2D__
#define __OY_OFFLINEPOINTADDRECTSUMCOUNTER2D__
namespace OY {
    namespace OFFLINEPARSC2D {
        using size_type = uint32_t;
        template <typename Tp>
        struct SimpleBIT {
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            SimpleBIT(size_type length) : m_sum(length) {}
            void add(size_type i, Tp inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
        };
        template <typename SizeType, typename WeightType>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
        };
        template <typename SizeType>
        struct Point<SizeType, bool> {
            SizeType m_x, m_y;
        };
        template <typename SizeType, typename WeightType = bool>
        struct Solver {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using point = Point<SizeType, WeightType>;
            struct Query {
                SizeType m_x_min, m_x_max, m_y_min, m_y_max;
            };
            std::vector<point> m_points;
            std::vector<Query> m_queries;
            Solver() = default;
            Solver(size_type point_cnt, size_type query_cnt) { m_points.reserve(point_cnt), m_queries.reserve(query_cnt); }
            void add_point(SizeType x, SizeType y, WeightType w = 1) {
                if constexpr (is_bool)
                    m_points.push_back({x, y});
                else
                    m_points.push_back({x, y, w});
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) { m_queries.push_back({x_min, x_max, y_min, y_max}); }
            template <typename SumType = typename std::conditional<is_bool, size_type, WeightType>::type, typename CountTree = SimpleBIT<SumType>>
            std::vector<SumType> solve() {
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                std::vector<SizeType> ys;
                ys.reserve(m_points.size());
                for (auto &p : m_points) {
                    if (ys.empty() || ys.back() != p.m_y) ys.push_back(p.m_y);
                    p.m_y = ys.size() - 1;
                }
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                auto get_y = [&](SizeType y) { return std::lower_bound(ys.begin(), ys.end(), y) - ys.begin(); };
                struct query {
                    SizeType m_x;
                    size_type m_id, m_y_min, m_y_max;
                    bool m_isleft;
                };
                std::vector<query> qs;
                qs.reserve(m_queries.size() * 2);
                for (size_type i = 0; i != m_queries.size(); i++) {
                    auto &q = m_queries[i];
                    size_type y_min = std::lower_bound(ys.begin(), ys.end(), q.m_y_min) - ys.begin();
                    size_type y_max = std::upper_bound(ys.begin(), ys.end(), q.m_y_max) - ys.begin();
                    if (y_min == y_max) continue;
                    qs.push_back({q.m_x_min, i, y_min, y_max - 1, true});
                    qs.push_back({q.m_x_max + 1, i, y_min, y_max - 1, false});
                }
                std::sort(qs.begin(), qs.end(), [](const query &x, const query &y) { return x.m_x < y.m_x; });
                std::vector<SumType> res(m_queries.size());
                CountTree cnt(ys.size());
                auto query = [&](size_type l, size_type r) { return l ? cnt.presum(r) - cnt.presum(l - 1) : cnt.presum(r); };
                size_type cur = 0, n = m_points.size();
                for (auto &q : qs) {
                    while (cur != n && m_points[cur].m_x < q.m_x) {
                        auto &p = m_points[cur++];
                        if constexpr (is_bool)
                            cnt.add(p.m_y, 1);
                        else
                            cnt.add(p.m_y, p.m_w);
                    }
                    res[q.m_id] += q.m_isleft ? -query(q.m_y_min, q.m_y_max) : query(q.m_y_min, q.m_y_max);
                }
                return res;
            }
        };
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
#ifndef __OY_WTREE__
#define __OY_WTREE__
namespace OY {
    namespace WTree {
        using size_type = size_t;
        struct Ignore {};
        struct Plus {
            template <typename Tp1, typename Tp2>
            void operator()(Tp1 &a, const Tp2 &b) const { a += b; }
        };
        struct BitXor {
            template <typename Tp1, typename Tp2>
            void operator()(Tp1 &a, const Tp2 &b) const { a ^= b; }
        };
        template <typename Tp, typename Operation = Plus>
        struct Tree {
            static constexpr size_type Z = 64, W = Z / sizeof(Tp), b = __builtin_ctz(W);
            typedef Tp vec_type __attribute((vector_size(Z / 2)));
            static constexpr size_type _calc_height(size_type n) { return n <= W ? 1 : _calc_height((n + W - 1) / W) + 1; }
            static constexpr size_type _calc_offset(size_type h, size_type len) {
                size_type s = 0, n = len + 1;
                while (h--) s += (n + W - 1) & -W, n = (n + W - 1) >> b;
                return s;
            }
            struct Pre {
                Tp m_mask[W][W];
                constexpr Pre() : m_mask{} {
                    for (size_type i = 0; i != W; i++)
                        for (size_type j = i + 1; j != W; j++) m_mask[i][j] = -1;
                }
            };
            static constexpr Pre pre{};
            Tp *m_data;
            size_type m_size, m_height, m_offset[10];
            template <typename InitMapping>
            Tp _init(size_type h, size_type cur, size_type &index, InitMapping &&mapping) {
                Tp sum{};
                if (!h)
                    for (size_type i = 0; i != W; i++, index++) {
                        m_data[cur + i] = sum;
                        if (index < m_size) Operation()(sum, mapping(index));
                    }
                else {
                    size_type nxt = (cur - m_offset[h]) * W + m_offset[h - 1];
                    for (size_type i = 0; i != W && index <= m_size; i++) m_data[cur + i] = sum, Operation()(sum, _init(h - 1, nxt, index, mapping)), nxt += W;
                }
                return sum;
            }
            Tree() : m_data{} {}
            template <typename InitMapping = Ignore>
            Tree(size_type length, InitMapping mapping = InitMapping()) : m_data{} { resize(length, mapping); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                clear();
                m_size = length;
                m_height = _calc_height(m_size + 1);
                for (size_type i = 0; i != m_height; i++) m_offset[i] = _calc_offset(i, m_size);
                size_type buf_len = _calc_offset(m_height, m_size);
                m_data = new (std::align_val_t(sizeof(Tp) * W)) Tp[buf_len]{};
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    size_type index = 0;
                    _init(m_height - 1, m_offset[m_height - 1], index, mapping);
                }
            }
            ~Tree() { clear(); }
            void clear() {
                if (m_data) ::operator delete[](m_data, std::align_val_t(sizeof(Tp) * W));
            }
            void regard_as(size_type length) {
                m_size = length;
                m_height = _calc_height(m_size + 1);
                for (size_type i = 0; i != m_height; i++) m_offset[i] = _calc_offset(i, m_size);
            }
            Tp presum(size_type i) const {
                Tp res{};
#pragma GCC unroll 64
                for (size_type h = 0; h != m_height; h++) Operation()(res, m_data[m_offset[h] + (i + 1 >> (h * b))]);
                return res;
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
            Tp query_all() const { return presum(m_size - 1); }
            void add(size_type i, const Tp &inc) {
                vec_type v{};
                v += inc;
#pragma GCC unroll 64
                for (size_type h = 0; h != m_height; h++) {
                    auto t = (vec_type *)&m_data[m_offset[h] + (i >> (h * b) & -W)];
                    auto m = (vec_type *)pre.m_mask[i >> (h * b) & (W - 1)];
                    Operation()(t[0], v & m[0]), Operation()(t[1], v & m[1]);
                }
            }
        };
        template <typename Ostream, typename Tp, typename Operation>
        Ostream &operator<<(Ostream &out, const Tree<Tp, Operation> &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.presum(i);
            }
            return out << "]";
        }
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
    OY::OFFLINEPARSC2D::Solver<uint32_t, uint32_t> sol(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x, y, w;
        cin >> x >> y >> w;
        sol.add_point(x, y, w);
    }
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, d, r, u;
        cin >> l >> d >> r >> u;
        sol.add_query(l, r - 1, d, u - 1);
    }
    for (auto a : sol.solve<uint64_t, OY::WTree::Tree<uint64_t>>()) cout << a << endl;
}
