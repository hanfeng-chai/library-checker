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
#include <limits>
#include <numeric>
#include <queue>
#include <vector>
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
#ifndef __OY_SIFTHEAP__
#define __OY_SIFTHEAP__
namespace OY {
    namespace Sift {
        using size_type = uint32_t;
        template <typename Sequence>
        struct Getter;
        template <typename Tp>
        struct Getter<std::vector<Tp>> {
            std::vector<Tp> &m_sequence;
            Getter(std::vector<Tp> &sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return m_sequence[index]; }
        };
        template <typename Tp>
        struct Getter<Tp *> {
            Tp *m_sequence;
            Getter(Tp *sequence) : m_sequence(sequence) {}
            const Tp &operator()(size_type index) const { return *(m_sequence + index); }
        };
        template <typename Mapping, typename Compare = std::less<void>>
        struct Heap {
            std::vector<size_type> m_heap, m_pos;
            size_type m_size;
            Mapping m_map;
            Compare m_comp;
            Heap(size_type length, Mapping map, Compare comp = Compare()) : m_map(map), m_comp(comp) { resize(length); }
            void resize(size_type length) {
                m_heap.resize(length), m_pos.assign(length, -1);
                m_size = 0;
            }
            void sift_up(size_type i) {
                size_type *pos = m_pos.data(), *heap = m_heap.data();
                size_type curpos = pos[i], p;
                auto &&curvalue = m_map(i);
                for (; curpos; curpos = (curpos - 1) >> 1) {
                    if (!m_comp(m_map(p = heap[(curpos - 1) >> 1]), curvalue)) break;
                    heap[pos[p] = curpos] = p;
                }
                heap[pos[i] = curpos] = i;
            }
            void sift_down(size_type i) {
                size_type *pos = m_pos.data(), *heap = m_heap.data();
                size_type curpos = pos[i], c;
                auto &&curvalue = m_map(i);
                for (; (c = curpos * 2 + 1) < m_size; curpos = c) {
                    if (c + 1 < m_size && m_comp(m_map(heap[c]), m_map(heap[c + 1]))) c++;
                    if (!m_comp(curvalue, m_map(heap[c]))) break;
                    pos[heap[curpos] = heap[c]] = curpos;
                }
                heap[pos[i] = curpos] = i;
            }
            void clear() {
                std::fill_n(m_pos.data(), m_pos.size(), -1);
                m_size = 0;
            }
            void push(size_type i) {
                if (!~m_pos[i]) {
                    m_pos[i] = m_size;
                    m_heap[m_size++] = i;
                }
                sift_up(i);
            }
            size_type top() const { return m_heap[0]; }
            void pop() {
                m_pos[m_heap[0]] = -1;
                if (--m_size) {
                    m_pos[m_heap[m_size]] = 0;
                    m_heap[0] = m_heap[m_size];
                    sift_down(m_heap[0]);
                }
            }
            bool empty() const { return !m_size; }
            size_type size() const { return m_size; }
        };
    }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = Sift::Heap<Sift::Getter<std::vector<Tp>>, Compare>>
    auto make_SiftHeap(Sift::size_type length, std::vector<Tp> &items, Compare comp = Compare()) -> HeapType { return HeapType(length, Sift::Getter<std::vector<Tp>>(items), comp); }
    template <typename Tp, typename Compare = std::less<Tp>, typename HeapType = Sift::Heap<Sift::Getter<Tp *>, Compare>>
    auto make_SiftHeap(Sift::size_type length, Tp *items, Compare comp = Compare()) -> HeapType { return HeapType(length, Sift::Getter<Tp *>(items), comp); }
    template <typename Sequence>
    using SiftGetter = Sift::Getter<Sequence>;
    template <typename Mapping, typename Compare = std::less<void>>
    using SiftHeap = Sift::Heap<Mapping, Compare>;
}
#endif
#ifndef __OY_KTHPATH__
#define __OY_KTHPATH__
namespace OY {
    namespace KPATH {
        using size_type = uint32_t;
        template <typename Tp, typename SumType, bool PassBy>
        struct Graph {
            struct edge {
                size_type m_from, m_to, m_next, m_reversed_next;
                Tp m_dis;
            };
            struct node {
                size_type m_id;
                SumType m_dis;
                bool operator<(const node &rhs) const { return m_dis < rhs.m_dis; }
            };
            struct leftist_node {
                size_type m_to, m_lchild{}, m_rchild{}, m_dist = 1;
                SumType m_dis;
                leftist_node(size_type to, const SumType &dis) : m_to(to), m_dis(dis) {}
            };
            struct cost_node {
                uint32_t m_root;
                SumType m_dis;
                bool operator<(const cost_node &_other) const { return m_dis > _other.m_dis; }
            };
            size_type m_vertex_cnt;
            std::vector<size_type> m_reversed, m_vertex, m_roots;
            std::vector<edge> m_edges;
            std::vector<node> m_nodes;
            std::vector<leftist_node> m_leftist;
            std::priority_queue<cost_node> m_queue;
            size_type _raw_merge(size_type a, size_type b) {
                if (!b) return a;
                if (m_leftist[a].m_dis > m_leftist[b].m_dis) std::swap(a, b);
                m_leftist[a].m_rchild = _raw_merge(b, m_leftist[a].m_rchild);
                if (m_leftist[m_leftist[a].m_rchild].m_dist > m_leftist[m_leftist[a].m_lchild].m_dist) std::swap(m_leftist[a].m_lchild, m_leftist[a].m_rchild);
                m_leftist[a].m_dist = m_leftist[m_leftist[a].m_rchild].m_dist + 1;
                return a;
            }
            size_type _safe_merge(size_type a, size_type b) {
                if (!b) return a;
                if (m_leftist[a].m_dis > m_leftist[b].m_dis) std::swap(a, b);
                size_type p = m_leftist.size();
                m_leftist.push_back(m_leftist[a]);
                m_leftist[p].m_rchild = _safe_merge(b, m_leftist[p].m_rchild);
                if (m_leftist[m_leftist[p].m_rchild].m_dist > m_leftist[m_leftist[p].m_lchild].m_dist) std::swap(m_leftist[p].m_lchild, m_leftist[p].m_rchild);
                m_leftist[p].m_dist = m_leftist[m_leftist[p].m_rchild].m_dist + 1;
                return p;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_reversed.assign(m_vertex_cnt, -1), m_vertex.assign(m_vertex_cnt, -1), m_roots.assign(m_vertex_cnt, {});
                m_edges.reserve(edge_cnt), m_nodes.resize(m_vertex_cnt);
            }
            void add_edge(size_type from, size_type to, Tp dis) {
                m_edges.push_back({from, to, m_vertex[from], m_reversed[to], dis});
                m_vertex[from] = m_reversed[to] = m_edges.size() - 1;
            }
            bool calc(size_type source, size_type target, const SumType &infinite = std::numeric_limits<SumType>::max() / 2) {
                struct distance {
                    SumType m_val;
                    size_type m_index;
                };
                std::vector<distance> dis(m_vertex_cnt, distance{infinite, size_type(-1)});
                auto mapping = [&](size_type i) { return dis[i].m_val; };
                Sift::Heap<decltype(mapping), std::greater<SumType>> heap(m_vertex_cnt, mapping, {});
                dis[target].m_val = {}, heap.push(target);
                while (!heap.empty()) {
                    size_type to = heap.top();
                    heap.pop();
                    for (size_type index = m_reversed[to]; ~index; index = m_edges[index].m_reversed_next) {
                        size_type from = m_edges[index].m_from;
                        SumType from_dis = dis[to].m_val + m_edges[index].m_dis;
                        if (dis[from].m_val > from_dis) dis[from].m_val = from_dis, dis[from].m_index = index, heap.push(from);
                    };
                }
                if (dis[source].m_val == infinite) return false;
                for (size_type i = 0; i != m_vertex_cnt; i++) m_nodes[i].m_id = i, m_nodes[i].m_dis = dis[i].m_val;
                std::swap(m_nodes[0], m_nodes[target]);
                std::sort(m_nodes.data() + 1, m_nodes.data() + m_vertex_cnt);
                m_leftist.emplace_back(-1, infinite), m_leftist[0].m_dist = 0;
                for (size_type i = !PassBy; i != m_vertex_cnt; i++) {
                    size_type from = m_nodes[i].m_id, cur_root = 0;
                    for (size_type index = m_vertex[from]; ~index; index = m_edges[index].m_next)
                        if (index != dis[from].m_index) {
                            size_type to = m_edges[index].m_to;
                            m_leftist.emplace_back(to, dis[to].m_val + m_edges[index].m_dis - dis[from].m_val);
                            cur_root = _raw_merge(m_leftist.size() - 1, cur_root);
                        }
                    if (~dis[from].m_index) {
                        size_type to = m_edges[dis[from].m_index].m_to;
                        if (m_roots[to]) cur_root = _safe_merge(m_roots[to], cur_root);
                    }
                    m_roots[from] = cur_root;
                }
                m_queue.push({0, dis[source].m_val});
                size_type root = m_roots[source];
                if (root) m_queue.push({root, dis[source].m_val + m_leftist[root].m_dis});
                return true;
            }
            SumType next(const SumType &infinite = std::numeric_limits<SumType>::max() / 2) {
                if (m_queue.empty()) return infinite;
                auto cost = m_queue.top();
                m_queue.pop();
                size_type root = cost.m_root;
                SumType dis = cost.m_dis;
                if (root) {
                    size_type lchild = m_leftist[root].m_lchild, rchild = m_leftist[root].m_rchild, nx = m_roots[m_leftist[root].m_to];
                    if (lchild) m_queue.push({lchild, dis - m_leftist[root].m_dis + m_leftist[lchild].m_dis});
                    if (rchild) m_queue.push({rchild, dis - m_leftist[root].m_dis + m_leftist[rchild].m_dis});
                    if (nx) m_queue.push({nx, dis + m_leftist[nx].m_dis});
                }
                return dis;
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
    uint32_t n, m, s, t, k;
    cin >> n >> m >> s >> t >> k;
    OY::KPATH::Graph<uint32_t, uint64_t, true> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    G.calc(s, t);
    uint32_t cur = 0;
    for (uint32_t i = 0; i != k; i++) {
        auto res = G.next();
        if (res < UINT64_MAX / 4)
            cout << res << endl;
        else {
            while (i != k) cout << "-1\n", i++;
            break;
        }
    }
}
