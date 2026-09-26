/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <type_traits>
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
#ifndef __OY_POTENTIALIZEDDSU__
#define __OY_POTENTIALIZEDDSU__
namespace OY {
    namespace PDSU {
        using size_type = uint32_t;
        template <typename ValueType>
        struct AddGroup {
            using value_type = ValueType;
            static value_type identity() { return value_type{}; }
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type inverse(const value_type &x) { return -x; }
        };
        template <typename ValueType, ValueType Mod>
        struct ModAddGroup {
            using value_type = ValueType;
            static value_type identity() { return value_type{}; }
            static value_type op(const value_type &x, const value_type &y) { return x + y - (x + y >= Mod ? Mod : 0); }
            static value_type inverse(const value_type &x) { return x ? Mod - x : 0; }
        };
        template <typename Group, bool MaintainGroupSize>
        class Table {
        public:
            using group = Group;
            using value_type = typename group::value_type;
            struct info {
                size_type m_head;
                value_type m_val;
            };
        private:
            mutable std::vector<size_type> m_parent, m_group_size;
            mutable std::vector<value_type> m_dis;
            size_type m_size, m_group_cnt;
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size);
                if constexpr (MaintainGroupSize) m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
                m_dis.assign(m_size, group::identity());
            }
            info find(size_type i) const {
                if (m_parent[i] == i) return {i, m_dis[i]};
                auto res = find(m_parent[i]);
                m_parent[i] = res.m_head;
                m_dis[i] = group::op(m_dis[i], res.m_val);
                return {m_parent[i], m_dis[i]};
            }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            void unite_to(size_type head_a, size_type head_b, value_type dis) {
                m_parent[head_a] = head_b;
                if constexpr (MaintainGroupSize) m_group_size[head_b] += m_group_size[head_a];
                m_dis[head_a] = dis;
                m_group_cnt--;
            }
            bool unite_by_size(size_type a, size_type b, value_type dis) {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                info ia = find(a), ib = find(b);
                if (ia.m_head == ib.m_head) return false;
                dis = group::op(group::op(group::inverse(ia.m_val), dis), ib.m_val);
                if (m_group_size[ia.m_head] > m_group_size[ib.m_head]) std::swap(ia, ib), dis = group::inverse(dis);
                unite_to(ia.m_head, ib.m_head, dis);
                return true;
            }
            bool unite_by_ID(size_type a, size_type b, value_type dis) {
                info ia = find(a), ib = find(b);
                if (ia.m_head == ib.m_head) return false;
                dis = group::op(group::op(group::inverse(ia.m_val), dis), ib.m_val);
                if (a < b) std::swap(a, b), dis = group::inverse(dis);
                unite_to(a, b, dis);
                return true;
            }
            value_type calc_dis(size_type a, size_type b) {
            }
            std::pair<bool, value_type> calc(size_type a, size_type b) const {
                info ia = find(a), ib = find(b);
                if (ia.m_head != ib.m_head) return std::make_pair(false, group::identity());
                return std::make_pair(true, group::op(ia.m_val, group::inverse(ib.m_val)));
            }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i != m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() const {
                if constexpr (MaintainGroupSize) {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i).m_head]].push_back(i);
                    return ret;
                } else {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size), cnt(m_group_cnt);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) cnt[index[find(i).m_head]]++;
                    for (size_type i = 0; i != m_group_cnt; i++) ret[i].reserve(cnt[i]);
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i).m_head]].push_back(i);
                    return ret;
                }
            }
        };
    }
    template <typename Tp, bool MaintainGroupSize>
    using AddPDSUTable = PDSU::Table<PDSU::AddGroup<Tp>, MaintainGroupSize>;
    template <typename Tp, Tp Mod, bool MaintainGroupSize>
    using ModAddPDSUTable = PDSU::Table<PDSU::ModAddGroup<Tp, Mod>, MaintainGroupSize>;
}
#endif
/*
lib code is above
temp code is below
*/
int main_commutative() {
    uint32_t n, q;
    cin >> n >> q;
    static constexpr uint32_t Mod = 998244353;
    auto reduce = [](uint32_t x) { return x >= Mod ? x - Mod : x; };
    OY::ModAddPDSUTable<uint32_t, Mod, true> U(n);
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b, dis;
            cin >> a >> b >> dis;
            if (U.unite_by_size(b, a, dis) || U.calc(b, a).second == dis)
                cout << "1\n";
            else
                cout << "0\n";
        } else {
            uint32_t a, b;
            cin >> a >> b;
            auto res = U.calc(b, a);
            if (res.first)
                cout << res.second << endl;
            else
                cout << "-1\n";
        }
    }
    return 0;
}
int main_non_commutative() {
    uint32_t n, q;
    cin >> n >> q;
    static constexpr uint32_t Mod = 998244353;
    auto reduce = [](uint32_t x) { return x >= Mod ? x - Mod : x; };
    struct Group {
        using value_type = std::array<std::array<uint32_t, 2>, 2>;
        static value_type identity() { return {1, 0, 0, 1}; }
        static value_type op(value_type x, value_type y) { return {(1ull * x[0][0] * y[0][0] + 1ull * x[0][1] * y[1][0]) % Mod, (1ull * x[0][0] * y[0][1] + 1ull * x[0][1] * y[1][1]) % Mod, (1ull * x[1][0] * y[0][0] + 1ull * x[1][1] * y[1][0]) % Mod, (1ull * x[1][0] * y[0][1] + 1ull * x[1][1] * y[1][1]) % Mod}; }
        static value_type inverse(value_type x) { return {x[1][1], x[0][1] ? Mod - x[0][1] : 0, x[1][0] ? Mod - x[1][0] : 0, x[0][0]}; }
    };
    using value_type = Group::value_type;
    OY::PDSU::Table<Group, true> U(n);
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b;
            value_type dis;
            cin >> a >> b >> dis[0][0] >> dis[0][1] >> dis[1][0] >> dis[1][1];
            if (U.unite_by_size(b, a, dis) || U.calc(b, a).second == dis)
                cout << "1\n";
            else
                cout << "0\n";
        } else {
            uint32_t a, b;
            cin >> a >> b;
            auto res = U.calc(b, a);
            if (res.first)
                cout << res.second[0][0] << ' ' << res.second[0][1] << ' ' << res.second[1][0] << ' ' << res.second[1][1] << endl;
            else
                cout << "-1\n";
        }
    }
    return 0;
}
int main() {
    main_non_commutative();
}
