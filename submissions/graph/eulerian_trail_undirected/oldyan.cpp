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
#ifndef __OY_EULERPATH_UDG__
#define __OY_EULERPATH_UDG__
namespace OY {
    namespace EulerPathUDG {
        using size_type = uint32_t;
        struct Solver {
            struct node {
                size_type m_cur, m_end, m_deg;
            };
            struct edge {
                size_type m_index, m_from, m_to;
            };
            size_type m_vertex_cnt, m_edge_id, m_even_cnt, m_source;
            std::vector<node> m_iter;
            std::vector<edge> m_edges;
            std::vector<bool> m_visit;
            template <typename FindNext, typename Callback>
            void _dfs(size_type cur, FindNext &&find_next, Callback &&call) {
                while (m_iter[cur].m_cur != m_iter[cur].m_end)
                    call(m_iter[cur].m_cur, [&](size_type index, size_type to) {
                        size_type i = m_iter[cur].m_cur;
                        m_iter[cur].m_cur = find_next(i);
                        if (!m_visit[index]) {
                            m_visit[index] = true;
                            _dfs(to, find_next, call);
                            m_edges[--m_edge_id] = {index, cur, to};
                        }
                    });
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) : m_vertex_cnt(vertex_cnt), m_edge_id(edge_cnt), m_even_cnt(0), m_source(-1), m_iter(vertex_cnt), m_edges(edge_cnt), m_visit(edge_cnt) {}
            template <typename Traverser, typename FindBegin, typename FindEnd, typename FindNext, typename Callback>
            void run(size_type prefer_source, Traverser &&traverser, FindBegin &&find_begin, FindEnd &&find_end, FindNext &&find_next, Callback &&call) {
                if (!m_edge_id) return void(m_source = prefer_source);
                for (size_type from = 0; from != m_vertex_cnt; from++) m_iter[from].m_cur = find_begin(from), m_iter[from].m_end = find_end(from);
                traverser([&](size_type from, size_type to) { m_iter[from].m_deg++, m_iter[to].m_deg++; });
                for (size_type i = 0; i != m_vertex_cnt; i++) m_even_cnt += m_iter[i].m_deg % 2 == 0;
                if (m_even_cnt == m_vertex_cnt) {
                    size_type source = m_iter[prefer_source].m_deg ? prefer_source : 0;
                    while (!m_iter[source].m_deg) source++;
                    _dfs(source, find_next, call);
                    if (!m_edge_id) m_source = source;
                } else if (m_even_cnt + 2 == m_vertex_cnt) {
                    size_type source = m_iter[prefer_source].m_deg % 2 ? prefer_source : 0;
                    while (m_iter[source].m_deg % 2 == 0) source++;
                    _dfs(source, find_next, call);
                    if (!m_edge_id) m_source = source;
                }
            }
            bool is_Euler_graph() const { return ~m_source && m_even_cnt == m_vertex_cnt; }
            bool is_half_Euler_graph() const { return ~m_source && m_even_cnt != m_vertex_cnt; }
            size_type get_source() const { return m_source; }
            template <typename Callback>
            void trace(Callback &&call) {
                for (size_type i = 0; i != m_edges.size(); i++) call(m_edges[i].m_index, m_edges[i].m_from, m_edges[i].m_to);
            }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            struct edge {
                size_type m_index, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type i, Callback &&call) const { call(m_edges[i].m_index, m_edges[i].m_to); }
            template <typename Callback>
            void operator()(Callback &&call) const {
                for (auto &e : m_raw_edges) call(e.m_from, e.m_to);
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) {
                    m_starts[e.m_from + 1]++;
                    if (e.m_from != e.m_to) m_starts[e.m_to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    size_type from = m_raw_edges[i].m_from, to = m_raw_edges[i].m_to;
                    m_edges[cursor[from]++] = {i, to};
                    if (from != to) m_edges[cursor[to]++] = {i, from};
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            void add_edge(size_type a, size_type b) { m_raw_edges.push_back({a, b}); }
            Solver calc(size_type prefer_source = 0) const {
                _prepare();
                Solver sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                return sol;
            }
            template <typename Callback>
            std::vector<size_type> get_path(size_type prefer_source = 0) const {
                _prepare();
                std::vector<size_type> res;
                Solver sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(
                    prefer_source,
                    *this, [&](size_type from) { return m_starts[from]; }, [&](size_type from) { return m_starts[from + 1]; }, [&](size_type i) { return i + 1; }, *this);
                if (!~sol.get_source()) return res;
                res.reserve(m_raw_edges.size() + 1);
                res.push_back(sol.m_source);
                sol.trace([&](size_type index, size_type from, size_type to) { res.push_back(to); });
                return res;
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
    uint32_t t;
    cin >> t;
    OY::EulerPathUDG::Graph G;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        G.resize(n, m);
        for (uint32_t i = 0; i != m; i++) {
            uint32_t a, b;
            cin >> a >> b;
            G.add_edge(a, b);
        }
        auto sol = G.calc();
        auto source = sol.get_source();
        if (!~source)
            cout << "No\n";
        else {
            cout << "Yes\n";
            bool started = false;
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                if (!started) started = true, cout << from;
                cout << ' ' << to;
            });
            if (!started) cout << 0;
            cout << endl;
            sol.trace([&](uint32_t index, uint32_t from, uint32_t to) {
                cout << index << ' ';
            });
            cout << endl;
        }
    }
}
