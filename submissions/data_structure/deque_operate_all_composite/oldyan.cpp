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
#ifndef __OY_INFODEQUE__
#define __OY_INFODEQUE__
namespace OY {
    namespace INFODEQUE {
        using size_type = uint32_t;
        template <typename Tp>
        struct InfoPair {
            Tp m_val, m_sum;
        };
        template <typename Tp>
        struct VectorAdapter : std::vector<Tp> {
            static constexpr bool is_special = false;
            using Base = std::vector<Tp>;
            const Tp &top() const { return Base::back(); }
            template <typename Callback1, typename Callback2>
            void drop_head(Callback1 &&call1, Callback2 &&call2) {
                size_type sz = Base::size();
                call1((*this)[(sz - 1) / 2].m_val);
                for (size_type i = (sz - 1) / 2 - 1; ~i; i--) call2((*this)[i].m_val);
                Base::erase(Base::begin(), Base::begin() + (sz + 1) / 2);
            }
        };
        template <typename Tp, size_t N, size_t ID>
        struct StaticAdapter {
            static constexpr bool is_special = false;
            static Tp s_buf[N];
            Tp *m_l = s_buf, *m_r = s_buf;
            bool empty() const { return m_l == m_r; }
            size_type size() const { return m_r - m_l; }
            void push_back(const Tp &x) { *m_r++ = x; }
            void pop_back() { m_r--; }
            const Tp &top() const { return *(m_r - 1); }
            template <typename Callback1, typename Callback2>
            void drop_head(Callback1 &&call1, Callback2 &&call2) {
                size_type sz = size();
                call1(m_l[(sz - 1) / 2].m_val);
                for (size_type i = (sz - 1) / 2 - 1; ~i; i--) call2(m_l[i].m_val);
                m_l += (sz + 1) / 2;
            }
            Tp &operator[](size_type i) { return m_l[i]; }
        };
        template <typename Tp, size_t N, size_t ID>
        Tp StaticAdapter<Tp, N, ID>::s_buf[N];
        template <typename Tp, size_t N>
        struct StaticContinuousAdapter {
            static Tp s_buf[N];
            struct type1 {
                static constexpr bool is_special = true;
                Tp *m_l = s_buf + N / 2, *m_r = s_buf + N / 2;
                bool empty() const { return m_l == m_r; }
                size_type size() const { return m_r - m_l; }
                void push_back(const Tp &x) { *--m_l = x; }
                void pop_back() { m_l++; }
                const Tp &top() const { return *m_l; }
                template <typename Callback1, typename Callback2>
                void drop_head(Callback1 &&call1, Callback2 &&call2) {
                    size_type sz = size();
                    call1(m_l[(sz - 1) / 2].m_val);
                    for (size_type i = (sz - 1) / 2 - 1; ~i; i--) call2(m_l[i].m_val);
                    m_l += (sz + 1) / 2;
                }
                Tp &operator[](size_type i) { return *(m_r - i - 1); }
            };
            struct type2 {
                static constexpr bool is_special = true;
                Tp *m_l = s_buf + N / 2, *m_r = s_buf + N / 2;
                bool empty() const { return m_l == m_r; }
                size_type size() const { return m_r - m_l; }
                void push_back(const Tp &x) { *m_r++ = x; }
                void pop_back() { m_r--; }
                const Tp &top() const { return *(m_r - 1); }
                template <typename Callback1, typename Callback2>
                void drop_head(Callback1 &&call1, Callback2 &&call2) {
                    size_type sz = size();
                    call1(m_l[(sz - 1) / 2].m_val);
                    for (size_type i = (sz - 1) / 2 - 1; ~i; i--) call2(m_l[i].m_val);
                    m_l += (sz + 1) / 2;
                }
                Tp &operator[](size_type i) { return m_l[i]; }
            };
        };
        template <typename Tp, size_t N>
        Tp StaticContinuousAdapter<Tp, N>::s_buf[N];
        template <typename Tp, typename Operation, typename Adapter1 = VectorAdapter<InfoPair<Tp>>, typename Adapter2 = VectorAdapter<InfoPair<Tp>>>
        struct Deque {
            Adapter1 m_left;
            Adapter2 m_right;
            Operation m_op;
            void _trans_left() {
                if constexpr (Adapter1::is_special && Adapter2::is_special) {
                    size_type sz = m_right.size();
                    m_left.m_l = m_right.m_l, m_left.m_r = m_right.m_l = m_right.m_l + (sz + 1) / 2;
                    m_left[0].m_sum = m_left[0].m_val;
                    for (size_type i = 1, sz = m_left.size(); i != sz; i++) m_left[i].m_sum = m_op(m_left[i].m_val, m_left[i - 1].m_sum);
                } else {
                    auto call1 = [&](const Tp &x) { m_left.push_back({x, x}); };
                    auto call2 = [&](const Tp &x) { m_left.push_back({x, m_op(x, m_left.top().m_sum)}); };
                    m_right.drop_head(call1, call2);
                }
                if (!m_right.empty()) {
                    m_right[0].m_sum = m_right[0].m_val;
                    for (size_type i = 1, sz = m_right.size(); i != sz; i++) m_right[i].m_sum = m_op(m_right[i - 1].m_sum, m_right[i].m_val);
                }
            }
            void _trans_right() {
                if constexpr (Adapter1::is_special && Adapter2::is_special) {
                    size_type sz = m_left.size();
                    m_right.m_r = m_left.m_r, m_right.m_l = m_left.m_r = m_left.m_r - (sz + 1) / 2;
                    m_right[0].m_sum = m_right[0].m_val;
                    for (size_type i = 1, sz = m_right.size(); i != sz; i++) m_right[i].m_sum = m_op(m_right[i - 1].m_sum, m_right[i].m_val);
                } else {
                    auto call1 = [&](const Tp &x) { m_right.push_back({x, x}); };
                    auto call2 = [&](const Tp &x) { m_right.push_back({x, m_op(m_right.top().m_sum, x)}); };
                    m_left.drop_head(call1, call2);
                }
                if (!m_left.empty()) {
                    m_left[0].m_sum = m_left[0].m_val;
                    for (size_type i = 1, sz = m_left.size(); i != sz; i++) m_left[i].m_sum = m_op(m_left[i].m_val, m_left[i - 1].m_sum);
                }
            }
            Deque(Operation op = Operation()) : m_op(op) {}
            void push_back(const Tp &x) {
                if (m_right.empty())
                    m_right.push_back({x, x});
                else
                    m_right.push_back({x, m_op(m_right.top().m_sum, x)});
            }
            void pop_back() {
                if (m_right.empty()) _trans_right();
                m_right.pop_back();
            }
            void push_front(const Tp &x) {
                if (m_left.empty())
                    m_left.push_back({x, x});
                else
                    m_left.push_back({x, m_op(x, m_left.top().m_sum)});
            }
            void pop_front() {
                if (m_left.empty()) _trans_left();
                m_left.pop_back();
            }
            const Tp &front() const {
                if (m_left.empty()) _trans_left();
                return m_left.top().m_val;
            }
            const Tp &back() const {
                if (m_right.empty()) _trans_right();
                return m_right.top().m_val;
            }
            bool empty() const { return m_left.empty() && m_right.empty(); }
            size_type size() const { return m_left.size() + m_right.size(); }
            Tp query_all() const {
                if (m_left.empty())
                    return m_right.top().m_sum;
                else if (m_right.empty())
                    return m_left.top().m_sum;
                else
                    return m_op(m_left.top().m_sum, m_right.top().m_sum);
            }
        };
    }
    template <typename Tp, typename Operation, size_t N, size_t ID1 = 0, size_t ID2 = 1>
    using GlobalInfoDeque = INFODEQUE::Deque<Tp, Operation, INFODEQUE::StaticAdapter<INFODEQUE::InfoPair<Tp>, N, ID1>, INFODEQUE::StaticAdapter<INFODEQUE::InfoPair<Tp>, N, ID2>>;
    template <typename Tp, typename Operation, size_t N>
    using GlobalContinuousInfoDeque = INFODEQUE::Deque<Tp, Operation, typename INFODEQUE::StaticContinuousAdapter<INFODEQUE::InfoPair<Tp>, N>::type1, typename INFODEQUE::StaticContinuousAdapter<INFODEQUE::InfoPair<Tp>, N>::type2>;
    template <typename Tp, typename Operation>
    using VectorInfoDeque = INFODEQUE::Deque<Tp, Operation, INFODEQUE::VectorAdapter<INFODEQUE::InfoPair<Tp>>, INFODEQUE::VectorAdapter<INFODEQUE::InfoPair<Tp>>>;
}
#endif
/*
lib code is above
temp code is below
*/
static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};
int main() {
    uint32_t q;
    cin >> q;
    OY::GlobalContinuousInfoDeque<node, std::plus<node>, 1000000> Q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b;
            cin >> a >> b;
            Q.push_front({a, b});
        } else if (op == '1') {
            uint32_t a, b;
            cin >> a >> b;
            Q.push_back({a, b});
        } else if (op == '2')
            Q.pop_front();
        else if (op == '3')
            Q.pop_back();
        else {
            uint32_t x;
            cin >> x;
            cout << (Q.empty() ? x : Q.query_all().calc(x)) << endl;
        }
    }
}
