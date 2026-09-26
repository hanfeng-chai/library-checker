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
#ifndef __OY_MONOQUEUE__
#define __OY_MONOQUEUE__
namespace OY {
    namespace QUE {
        using size_type = uint32_t;
        template <typename Tp>
        struct NoOpMonoid {
            using value_type = Tp;
        };
        template <typename ValueType, typename SumType, ValueType Identity, typename Operation>
        struct BaseMonoid {
            using value_type = ValueType;
            using sum_type = SumType;
            static constexpr value_type identity() { return Identity; }
            static sum_type op(const sum_type &x, const sum_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename ValueType, typename SumType>
        struct InfoPair {
            ValueType m_val;
            SumType m_sum;
        };
        template <typename ValueType>
        struct InfoPair<ValueType, void> {
            ValueType m_val;
        };
        template <typename Tp>
        struct VectorContainer {
            static constexpr bool is_global = false;
            struct type : std::vector<Tp> {
                static constexpr bool is_special = false;
                const Tp &top() const { return std::vector<Tp>::back(); }
            };
            using type1 = type;
            using type2 = type;
        };
        template <size_t N>
        struct GlobalContainer {
            template <typename Tp>
            struct type {
                static constexpr bool is_global = true;
                static Tp s_buf[N];
                struct type1 {
                    static constexpr bool is_special = true;
                    Tp *m_l = s_buf, *m_r = s_buf;
                    bool empty() const { return m_l == m_r; }
                    size_type size() const { return m_r - m_l; }
                    void pop_back() { m_l++; }
                    const Tp &top() const { return *m_l; }
                    Tp &operator[](size_type i) const { return *(m_r - i - 1); }
                };
                struct type2 {
                    static constexpr bool is_special = true;
                    Tp *m_l = s_buf, *m_r = s_buf;
                    bool empty() const { return m_l == m_r; }
                    size_type size() const { return m_r - m_l; }
                    void push_back(const Tp &x) { *m_r++ = x; }
                    const Tp &top() const { return *(m_r - 1); }
                    const Tp &operator[](size_type i) const { return m_l[i]; }
                };
            };
        };
        template <size_t N>
        template <typename Tp>
        Tp GlobalContainer<N>::type<Tp>::s_buf[N];
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
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Sum_Type : std::false_type {
            using type = ValueType;
        };
        template <typename Tp, typename ValueType>
        struct Has_Sum_Type<Tp, ValueType, void_t<typename Tp::sum_type>> : std::true_type {
            using type = typename Tp::sum_type;
        };
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename Monoid, template <typename> typename Container = VectorContainer>
        class Queue {
        public:
            using monoid = Monoid;
            using value_type = typename Monoid::value_type;
            using sum_type = typename Has_Sum_Type<Monoid, value_type>::type;
            static constexpr bool has_op = Has_Op<Monoid, sum_type>::value;
            using info_type = InfoPair<value_type, typename std::conditional<has_op, sum_type, void>::type>;
            using container_type = Container<info_type>;
            mutable container_type::type1 m_left;
            mutable container_type::type2 m_right;
            void _trans() const {
                if constexpr (container_type::is_global) {
                    m_left.m_l = m_right.m_l, m_left.m_r = m_right.m_l = m_right.m_r;
                    if constexpr (has_op) {
                        m_left[0].m_sum = m_left[0].m_val;
                        for (size_type i = 1, sz = m_left.size(); i != sz; i++) m_left[i].m_sum = Monoid::op(m_left[i].m_val, m_left[i - 1].m_sum);
                    }
                } else {
                    if constexpr (has_op)
                        m_left.push_back({m_right.top().m_val, sum_type(m_right.top().m_val)});
                    else
                        m_left.push_back({m_right.top().m_val});
                    m_right.pop_back();
                    size_type sz = m_right.size();
                    while (sz--) {
                        if constexpr (has_op)
                            m_left.push_back({m_right.top().m_val, Monoid::op(m_right.top().m_val, m_left.top().m_sum)});
                        else
                            m_left.push_back({m_right.top().m_val});
                        m_right.pop_back();
                    }
                }
            }
        public:
            void push(value_type x) {
                if constexpr (!has_op)
                    m_right.push_back({x});
                else if (m_right.empty())
                    m_right.push_back({x, sum_type(x)});
                else
                    m_right.push_back({x, Monoid::op(m_right.top().m_sum, x)});
            }
            void pop() {
                if (m_left.empty()) _trans();
                m_left.pop_back();
            }
            const value_type &front() const {
                if (m_left.empty()) _trans();
                return m_left.top().m_val;
            }
            const value_type &back() const { return m_right.empty() ? m_right.top().m_val : m_left[0].m_val; }
            bool empty() const { return m_left.empty() && m_right.empty(); }
            size_type size() const { return m_left.size() + m_right.size(); }
            sum_type query_all() const {
                if (m_left.empty())
                    return m_right.top().m_sum;
                else if (m_right.empty())
                    return m_left.top().m_sum;
                else
                    return Monoid::op(m_left.top().m_sum, m_right.top().m_sum);
            }
            const value_type &operator[](size_type i) const { return i < m_left.size() ? m_left[m_left.size() - 1 - i].m_val : m_right[i - m_left.size()].m_val; }
        };
    }
    template <typename Monoid, size_t N>
    using GlobalQueue = QUE::Queue<Monoid, QUE::GlobalContainer<N>::template type>;
    template <typename Monoid>
    using VectorQueue = QUE::Queue<Monoid, QUE::VectorContainer>;
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MaxQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, Minimum, QUE::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max(), bool MaintainReverse = true, QUE::size_type MAX_NODE = 1 << 20>
    using MinQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, Maximum, QUE::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using GcdQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, 0, QUE::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using LcmQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, 1, QUE::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using BitAndQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using BitOrQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using BitXorQueue = QUE::Queue<QUE::BaseMonoid<Tp, Tp, ZeroMask, std::bit_xor<Tp>>>;
    template <typename ValueType, typename SumType, ValueType Zero = ValueType()>
    using SumQueue = QUE::Queue<QUE::BaseMonoid<ValueType, SumType, Zero, std::plus<SumType>>>;
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
constexpr node id{1, 0};
int main() {
    uint32_t q;
    cin >> q;
    using monoid = OY::SumQueue<node, node, id>::monoid;
    OY::GlobalQueue<monoid, 500000> Q;
    while (q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, b;
            cin >> a >> b;
            Q.push({a, b});
        } else if (op == '1')
            Q.pop();
        else {
            uint32_t x;
            cin >> x;
            cout << (Q.empty() ? x : Q.query_all().calc(x)) << endl;
        }
    }
}
