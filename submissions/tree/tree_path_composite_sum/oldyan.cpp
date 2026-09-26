/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <numeric>
#include <vector>
#ifndef __OY_TREETRANSFER__
#define __OY_TREETRANSFER__
namespace OY {
    namespace TreeTransfer {
        using size_type = uint32_t;
        template <typename Tp, typename InitMapping>
        struct PreWork {
            std::vector<Tp> &m_dp;
            InitMapping &m_mapping;
            template <typename Fp>
            void operator()(size_type a, size_type p, Fp e) const { m_dp[a] = m_mapping(a); }
        };
        template <typename Tp, typename Merge>
        struct Report {
            std::vector<Tp> &m_dp;
            Merge &m_merge;
            template <typename Fp>
            void operator()(size_type a, size_type to, Fp e) const { m_merge(m_dp[a], m_dp[to], a, to, e); }
        };
        template <typename Tp, typename Exclude>
        struct ExcludeWork {
            std::vector<Tp> &m_dp;
            Exclude &m_exclude;
            template <typename Fp>
            void operator()(size_type a, size_type p, Fp e) const {
                if (~p)
                    m_exclude(m_dp[a], m_dp[p], a, p, e);
                else {
                    auto dp_null = Tp();
                    m_exclude(m_dp[a], dp_null, a, -1, e);
                }
            }
        };
        template <typename Tp, typename Tree, typename InitMapping, typename Merge, typename Exclude>
        inline void solve(const Tree &rooted_tree, InitMapping mapping, Merge &&merge, Exclude &&exclude) {
            size_type n = rooted_tree.vertex_cnt();
            std::vector<Tp> dp(n);
            rooted_tree.tree_dp_edge(rooted_tree.m_root, PreWork<Tp, InitMapping>{dp, mapping}, Report<Tp, Merge>{dp, merge}, {});
            rooted_tree.tree_dp_edge(rooted_tree.m_root, ExcludeWork<Tp, Exclude>{dp, exclude}, {}, {});
        }
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
#ifndef __OY_FLATTREE__
#define __OY_FLATTREE__
namespace OY {
    namespace FlatTree {
        using size_type = uint32_t;
        struct Ignore {
            template <typename... Args>
            void operator()(Args... args) const {}
        };
        template <typename Tp>
        struct Edge {
            size_type m_from, m_to;
            Tp m_dis;
        };
        template <>
        struct Edge<bool> {
            size_type m_from, m_to;
        };
        template <typename Tp>
        struct Adj {
            size_type m_to;
            Tp m_dis;
        };
        template <>
        struct Adj<bool> {
            size_type m_to;
        };
        template <typename Tp, size_type MAX_VERTEX>
        struct Tree {
            static Edge<Tp> s_edge_buffer[MAX_VERTEX << 1];
            static Adj<Tp> s_buffer[MAX_VERTEX << 1];
            static size_type s_start_buffer[MAX_VERTEX << 1], s_use_count;
            Edge<Tp> *m_edges;
            Adj<Tp> *m_adj;
            size_type *m_starts, m_root = -1, m_vertex_cnt, m_edge_cnt;
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0)), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_vertex(size_type a, size_type p, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p);
                else if (!pre_work(a, p))
                    return false;
                do_for_each_adj_vertex(a, [&](size_type to) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_vertex(to, a, pre_work, report, after_work)) report(a, to);
                    } else if (to != p)
                        _tree_dp_vertex(to, a, pre_work, report, after_work), report(a, to);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0, 0)), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_edge(size_type a, size_type p, Tp up_dis, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p, up_dis);
                else if (!pre_work(a, p, up_dis))
                    return false;
                do_for_each_adj_edge(a, [&](size_type to, Tp dis) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_edge(to, a, dis, pre_work, report, after_work)) report(a, to, dis);
                    } else if (to != p)
                        _tree_dp_edge(to, a, dis, pre_work, report, after_work), report(a, to, dis);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            Tree(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                m_root = -1;
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_edges = s_edge_buffer + s_use_count, m_adj = s_buffer + s_use_count, m_starts = s_start_buffer + s_use_count, m_edge_cnt = 0, s_use_count += m_vertex_cnt << 1;
            }
            void add_edge(size_type a, size_type b, Tp dis = Tp()) {
                if constexpr (std::is_same<Tp, bool>::value)
                    m_edges[m_edge_cnt++] = {a, b};
                else
                    m_edges[m_edge_cnt++] = {a, b, dis};
            }
            void prepare() {
                std::fill_n(m_starts, m_vertex_cnt + 1, 0);
                for (size_type i = 0; i < m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    m_starts[from + 1]++, m_starts[to + 1]++;
                }
                std::partial_sum(m_starts, m_starts + m_vertex_cnt + 1, m_starts);
                std::vector<size_type> cursor(m_starts, m_starts + m_vertex_cnt);
                for (size_type i = 0; i < m_edge_cnt; i++) {
                    size_type from = m_edges[i].m_from, to = m_edges[i].m_to;
                    if constexpr (std::is_same<Tp, bool>::value)
                        m_adj[cursor[from]++] = {to}, m_adj[cursor[to]++] = {from};
                    else {
                        Tp dis = m_edges[i].m_dis;
                        m_adj[cursor[from]++] = {to, dis}, m_adj[cursor[to]++] = {from, dis};
                    }
                }
            }
            void set_root(size_type root) { m_root = root; }
            size_type vertex_cnt() const { return m_vertex_cnt; }
            template <typename Callback>
            void do_for_each_adj_vertex(size_type a, Callback &&call) const {
                for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to);
            }
            template <typename Callback>
            void do_for_each_adj_edge(size_type a, Callback &&call) const {
                if constexpr (std::is_same<Tp, bool>::value)
                    for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to, 1);
                else
                    for (size_type cur = m_starts[a], end = m_starts[a + 1]; cur != end; cur++) call(m_adj[cur].m_to, m_adj[cur].m_dis);
            }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_vertex(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_vertex(a, -1, pre_work, report, after_work); }
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_edge(size_type a, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { _tree_dp_edge(a, -1, {}, pre_work, report, after_work); }
        };
        template <typename Tp, size_type MAX_VERTEX>
        Edge<Tp> Tree<Tp, MAX_VERTEX>::s_edge_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        Adj<Tp> Tree<Tp, MAX_VERTEX>::s_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        size_type Tree<Tp, MAX_VERTEX>::s_start_buffer[MAX_VERTEX << 1];
        template <typename Tp, size_type MAX_VERTEX>
        size_type Tree<Tp, MAX_VERTEX>::s_use_count;
        template <typename Ostream, typename Tp, size_type MAX_VERTEX>
        Ostream &operator<<(Ostream &out, const Tree<Tp, MAX_VERTEX> &tree) { 
            tree.tree_dp_vertex(
                ~tree.m_root ? tree.m_root : 0, [&](size_type a, size_type) { out << '[' << a; }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
}
#endif
/*
lib code is above
temp code is below
*/
static constexpr uint32_t P = 998244353, N = 200000;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    uint32_t calc(uint64_t i, uint64_t size) const {
        return (i * mul + add * size) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
uint32_t a[N];
int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> a[i];
    OY::FlatTree::Tree<node, N> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t u, v, b, c;
        cin >> u >> v >> b >> c;
        S.add_edge(u, v, {b, c});
    }
    S.prepare(), S.set_root(0);
    struct Tp {
        uint32_t m_val, m_cnt;
    };
    auto mapping = [](uint32_t i) -> Tp { return {a[i], 1}; };
    auto merge = [](Tp &dp_a, Tp dp_to, uint32_t a, uint32_t to, const node &e) {
        dp_a.m_val += e.calc(dp_to.m_val, dp_to.m_cnt);
        if (dp_a.m_val >= P) dp_a.m_val -= P;
        dp_a.m_cnt += dp_to.m_cnt;
    };
    auto exclude = [](Tp &dp_a, Tp dp_p, uint32_t a, uint32_t p, const node &e) {
        if (~p) {
            auto up_val = dp_p.m_val + P - e.calc(dp_a.m_val, dp_a.m_cnt);
            if (up_val >= P) up_val -= P;
            auto up_cnt = dp_p.m_cnt - dp_a.m_cnt;
            dp_a.m_val += e.calc(up_val, up_cnt);
            if (dp_a.m_val >= P) dp_a.m_val -= P;
            dp_a.m_cnt += up_cnt;
        }
        ::a[a] = dp_a.m_val;
    };
    OY::TreeTransfer::solve<Tp>(S, mapping, merge, exclude);
    for (uint32_t i = 0; i != n; i++) cout << a[i] << ' ';
}
