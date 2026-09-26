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
#ifndef __OY_MONOZKWTREE__
#define __OY_MONOZKWTREE__
namespace OY {
    namespace MONOZKW {
        using size_type = uint32_t;
        template <typename Tp, Tp Val>
        struct ConstexprIdentity {
            template <typename... Args>
            Tp operator()(Args...) const { return Val; }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename Tp, typename IdentityMapping, typename Operation>
        struct Tree {
            size_type m_size, m_depth, m_capacity;
            std::vector<Tp> m_sub;
            static bool _check_pushup(Tp &old, Tp val) { return old != val ? old = val, true : false; }
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.assign(m_capacity * 2, IdentityMapping()());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.resize(m_capacity * 2);
                Tp *sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[m_capacity + i] = mapping(i);
                std::fill_n(sub + m_capacity + m_size, m_capacity - m_size, IdentityMapping()());
                for (size_type len = m_capacity / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) sub[i] = Operation()(sub[i * 2], sub[i * 2 + 1]);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, Tp val) {
                Tp *sub = m_sub.data();
                sub[i += m_capacity] = val;
                while ((i >>= 1) && _check_pushup(sub[i], Operation()(sub[i * 2], sub[i * 2 + 1]))) {}
            }
            Tp query(size_type i) const { return m_sub[m_capacity + i]; }
            Tp query(size_type left, size_type right) const {
                if (left == right) return query(left);
                const Tp *sub = m_sub.data();
                Tp res = IdentityMapping()();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res = Operation()(res, sub[(m_capacity + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res = Operation()(res, sub[(m_capacity + left) >> (j - 1)]);
                    left = left2;
                }
                return res;
            }
            Tp query_all() const { return m_sub[1]; }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                const Tp *sub = m_sub.data();
                Tp val = sub[left += m_capacity];
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type len = 1; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    Tp a(Operation()(val, sub[left >>= ctz]));
                    len <<= ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; len >>= 1) {
                            Tp a(Operation()(val, sub[left <<= 1]));
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                const Tp *sub = m_sub.data();
                Tp val = sub[right += m_capacity];
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type len = 1; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    Tp a(Operation()(sub[(right >>= ctz) - 1], val));
                    len >>= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; len >>= 1) {
                            Tp a(Operation()(sub[(right <<= 1) - 1], val));
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
        };
        template <typename Ostream, typename Tp, typename IdentityMapping, typename Operation>
        Ostream &operator<<(Ostream &out, const Tree<Tp, IdentityMapping, Operation> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Minimum>, MONOZKW::ChoiceByCompare<Tp, std::less<Tp>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Maximum>, MONOZKW::ChoiceByCompare<Tp, std::greater<Tp>>>;
    template <typename Tp>
    using MonoGcdTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, 0>, MONOZKW::FpTransfer<Tp, std::gcd<Tp>>>;
    template <typename Tp>
    using MonoLcmTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, 1>, MONOZKW::FpTransfer<Tp, std::lcm<Tp>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, OneMask>, std::bit_and<Tp>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, ZeroMask>, std::bit_or<Tp>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumTree = MONOZKW::Tree<Tp, MONOZKW::ConstexprIdentity<Tp, Zero>, std::plus<Tp>>;
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
    bool operator!=(const node &rhs) const { return mul != rhs.mul || add != rhs.add; }
};
constexpr node identity{1, 0};
int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto S = OY::MonoSumTree<node, identity>(n, [](auto...) {
        uint32_t a, b;
        cin >> a >> b;
        return node{a, b};
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t p, c, d;
            cin >> p >> c >> d;
            S.modify(p, {c, d});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}
