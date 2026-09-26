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
#ifndef __OY_LINKBUCKET__
#define __OY_LINKBUCKET__
namespace OY {
    namespace LBC {
        using size_type = uint32_t;
        struct Ignore {};
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Func, typename Para, typename = void>
        struct Can_call : std::false_type {};
        template <typename Func, typename Para>
        struct Can_call<Func, Para, void_t<decltype(std::declval<Func>()(std::declval<Para>()))>> : std::true_type {};
        template <typename Tp>
        struct LinkBucket {
            struct node {
                Tp m_value;
                size_type m_next;
            };
            struct iterator {
                node *m_item;
                size_type m_id;
                iterator(node *item, size_type id) : m_item(item), m_id(id) {}
                iterator &operator++() {
                    m_id = m_item[m_id].m_next;
                    return *this;
                }
                iterator operator++(int) {
                    iterator old(*this);
                    m_id = m_item[m_id].m_next;
                    return old;
                }
                Tp &operator*() const { return m_item[m_id].m_value; }
                bool operator==(const iterator &rhs) const { return m_id == rhs.m_id; }
                bool operator!=(const iterator &rhs) const { return m_id != rhs.m_id; }
            };
            struct Bucket {
                LinkBucket *m_lbc;
                size_type m_buc_id;
                Bucket(LinkBucket *lbc, size_type buc_id) : m_lbc(lbc), m_buc_id(buc_id) {}
                bool empty() const { return !~m_lbc->m_bucket[m_buc_id]; }
                Tp &front() { return m_lbc->m_item[m_lbc->m_bucket[m_buc_id]].m_value; }
                template <typename Modify = Tp>
                void push_front(Modify &&modify) {
                    if constexpr (Can_call<Modify, node *>::value)
                        modify(m_lbc->m_item.data() + m_lbc->m_cursor);
                    else
                        m_lbc->m_item[m_lbc->m_cursor].m_value = modify;
                    m_lbc->m_item[m_lbc->m_cursor].m_next = m_lbc->m_bucket[m_buc_id];
                    m_lbc->m_bucket[m_buc_id] = m_lbc->m_cursor++;
                }
                void pop_front() { m_lbc->m_bucket[m_buc_id] = m_lbc->m_item[m_lbc->m_bucket[m_buc_id]].m_next; }
                iterator begin() const { return iterator(m_lbc->m_item.data(), m_lbc->m_bucket[m_buc_id]); }
                iterator end() const { return iterator(m_lbc->m_item.data(), -1); }
            };
            std::vector<size_type> m_bucket;
            std::vector<node> m_item;
            size_type m_bucket_cnt, m_cursor;
            LinkBucket(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                m_bucket.assign(m_bucket_cnt, -1), m_item.resize(item_cnt), m_cursor = 0;
            }
            Bucket operator[](size_type buc_id) { return Bucket(this, buc_id); }
            iterator bucket_begin(size_type buc_id) { return iterator(m_item, m_bucket[buc_id]); }
            iterator bucket_end(size_type = 0) { return iterator(m_item, -1); }
        };
    }
};
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
#ifndef __OY_ROLLBACKDISJOINTUNION__
#define __OY_ROLLBACKDISJOINTUNION__
namespace OY {
    namespace RollbackDSU {
        using size_type = uint32_t;
        struct Table {
            struct Record {
                size_type m_head_a, m_head_b;
            };
            std::vector<size_type> m_parent, m_group_size;
            std::vector<Record> m_records;
            size_type m_size, m_group_cnt;
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size), m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
            }
            size_type find(size_type i) const { return m_parent[i] == i ? i : find(m_parent[i]); }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            template <bool MakeRecord = true>
            void unite_to(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_b;
                m_group_size[head_b] += m_group_size[head_a];
                m_group_cnt--;
                if constexpr (MakeRecord) m_records.push_back({head_a, head_b});
            }
            template <bool MakeRecord = true>
            bool unite_by_size(size_type a, size_type b) {
                a = find(a), b = find(b);
                if (a == b) return false;
                if (m_group_size[a] > m_group_size[b]) std::swap(a, b);
                unite_to<MakeRecord>(a, b);
                return true;
            }
            void cancle(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            void cancle() {
                size_type head_a = m_records.back().m_head_a, head_b = m_records.back().m_head_b;
                m_records.pop_back();
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            bool in_same_group(size_type a, size_type b) const { return find(a) == find(b); }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i < m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() const {
                std::vector<std::vector<size_type>> ret(m_group_cnt);
                std::vector<size_type> index(m_size);
                for (size_type i = 0, j = 0; i != m_size; i++)
                    if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream>
        Ostream &operator<<(Ostream &out, const Table &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.m_parent[i];
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
static constexpr uint32_t N = 200000, M = 200000;
struct Node {
    uint32_t ver, is_query, a, b;
};
OY::LBC::LinkBucket<Node> buckets;
uint32_t id[M + 1];
void solve_rollbackdsu() {
    uint32_t n, m;
    cin >> n >> m;
    buckets.resize(m + 1, m);
    uint32_t cur = 0;
    for (uint32_t i = 1; i <= m; i++) {
        char op;
        int k;
        uint32_t u, v;
        cin >> op >> k >> u >> v;
        k++;
        if (op == '0')
            buckets[id[k]].push_front(Node{id[i] = ++cur, false, u, v});
        else
            buckets[id[k]].push_front(Node{i - cur - 1, true, u, v});
    }
    OY::RollbackDSU::Table U(n);
    std::string res(m - cur, ' ');
    auto dfs = [&](auto self, uint32_t cur, uint32_t a, uint32_t b) -> void {
        uint32_t head_a = U.find(a), head_b = U.find(b);
        if (head_a != head_b) {
            if (U.size<true>(head_a) > U.size<true>(head_b)) std::swap(head_a, head_b);
            U.unite_to<false>(head_a, head_b);
        }
        for (auto &&[ver, is_q, a, b] : buckets[cur])
            if (is_q)
                res[ver] = '0' + U.in_same_group(a, b);
            else
                self(self, ver, a, b);
        if (head_a != head_b) U.cancle(head_a, head_b);
    };
    dfs(dfs, 0, 0, 0);
    for (char c : res) cout << c << endl;
}
int main() {
    solve_rollbackdsu();
}
