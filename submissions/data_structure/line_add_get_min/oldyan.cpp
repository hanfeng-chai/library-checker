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
#ifndef __OY_VECTORBUFFERWITHOUTCOLLECT__
#define __OY_VECTORBUFFERWITHOUTCOLLECT__
namespace OY {
    namespace VectorBufferWithoutCollect_imp {
        using size_type = uint32_t;
        template <typename Node>
        struct VectorBufferWithoutCollect {
            static constexpr bool is_vector_buffer = true;
            static std::vector<Node> s_buf;
            static Node *data() { return s_buf.data(); }
            static size_type newnode() {
                s_buf.push_back({});
                return s_buf.size() - 1;
            }
            static size_type newnode(size_type length) {
                s_buf.resize(s_buf.size() + length);
                return s_buf.size() - length;
            }
            static void collect(size_type) {}
        };
        template <typename Node>
        std::vector<Node> VectorBufferWithoutCollect<Node>::s_buf{Node{}};
    }
    using VectorBufferWithoutCollect_imp::VectorBufferWithoutCollect;
}
#endif
#ifndef __OY_LICHAOSEGTREE__
#define __OY_LICHAOSEGTREE__
namespace OY {
    namespace LCSEG {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseLine {
            Tp m_k, m_b;
            BaseLine() = default;
            BaseLine(Tp k, Tp b) : m_k(k), m_b(b) {}
            Tp calc(Tp i) const { return m_k * i + m_b; }
        };
        struct BaseLess {
            template <typename Tp>
            bool operator()(const Tp &x, const Tp &y) const { return x < y; }
        };
        template <typename Line = BaseLine<double>, typename Compare = BaseLess, typename SizeType = uint64_t, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using value_type = decltype(std::declval<Line>().calc(0));
            struct node {
                Line m_line;
                size_type m_lc, m_rc;
                bool is_null() const { return this == _ptr(0); }
                node *lchild() const { return _ptr(m_lc); }
                node *rchild() const { return _ptr(m_rc); }
            };
            using buffer_type = BufferType<node>;
            static void _reserve(size_type capacity) {
                static_assert(buffer_type::is_vector_buffer, "Only In Vector Mode");
                buffer_type::s_buf.reserve(capacity);
            }
        private:
            size_type m_root;
            SizeType m_size;
            Line m_default_line;
            static node *_ptr(size_type cur) { return buffer_type::data() + cur; }
            size_type _newnode() {
                size_type c = buffer_type::newnode();
                _ptr(c)->m_line = m_default_line;
                return c;
            }
            size_type _lchild(size_type cur) {
                if (!_ptr(cur)->m_lc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_lc = c;
                }
                return _ptr(cur)->m_lc;
            }
            size_type _rchild(size_type cur) {
                if (!_ptr(cur)->m_rc) {
                    size_type c = _newnode();
                    _ptr(cur)->m_rc = c;
                }
                return _ptr(cur)->m_rc;
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, Line line) {
                SizeType mid = (floor + ceil) >> 1;
                node *p = _ptr(cur);
                if (Compare()(p->m_line.calc(mid), line.calc(mid))) std::swap(p->m_line, line);
                if (floor < ceil)
                    if (Compare()(p->m_line.calc(floor), line.calc(floor))) {
                        _add(_lchild(cur), floor, mid, line);
                    } else if (Compare()(p->m_line.calc(ceil), line.calc(ceil)))
                        _add(_rchild(cur), mid + 1, ceil, line);
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType left, SizeType right, Line line) {
                if (left <= floor && right >= ceil)
                    _add(cur, floor, ceil, line);
                else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (left <= mid) _add(_lchild(cur), floor, mid, left, right, line);
                    if (right > mid) _add(_rchild(cur), mid + 1, ceil, left, right, line);
                }
            }
            void _add(size_type cur, SizeType floor, SizeType ceil, SizeType i, Line line) {
                if (floor == ceil) {
                    if (Compare()(_ptr(cur)->m_line.calc(i), line.calc(i))) _ptr(cur)->m_line = line;
                } else {
                    SizeType mid = (floor + ceil) >> 1;
                    if (i <= mid)
                        _add(_lchild(cur), floor, mid, i, line);
                    else
                        _add(_rchild(cur), mid + 1, ceil, i, line);
                }
            }
            Line _query(size_type cur, SizeType floor, SizeType ceil, SizeType i) const {
                node *p = _ptr(cur);
                if (floor == ceil) return p->m_line;
                SizeType mid = (floor + ceil) >> 1;
                if (i <= mid) {
                    if (!p->m_lc) return p->m_line;
                    Line line = _query(p->m_lc, floor, mid, i);
                    return Compare()(p->m_line.calc(i), line.calc(i)) ? line : p->m_line;
                } else {
                    if (!p->m_rc) return p->m_line;
                    Line line = _query(p->m_rc, mid + 1, ceil, i);
                    return Compare()(p->m_line.calc(i), line.calc(i)) ? line : p->m_line;
                }
            }
        public:
            Tree(SizeType length = 0, Line default_line = Line()) : m_default_line(default_line) { resize(length); }
            void resize(SizeType length) {
                if (m_size = length) m_root = _newnode();
            }
            void add(SizeType i, const Line &line) { _add(m_root, 0, m_size - 1, i, line); }
            void add(SizeType left, SizeType right, const Line &line) { _add(m_root, 0, m_size - 1, left, right, line); }
            Line query(SizeType i) const { return _query(m_root, 0, m_size - 1, i); }
        };
    }
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChmaxSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::less<Tp>, SizeType, VectorBufferWithoutCollect>;
    template <typename Tp, typename SizeType = uint64_t>
    using VectorLichaoSlopeChminSegTree = LCSEG::Tree<LCSEG::BaseLine<Tp>, std::greater<Tp>, SizeType, VectorBufferWithoutCollect>;
}
#endif
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    static constexpr uint32_t M = 1000000000;
    static constexpr int64_t inf = 4e18;
    OY::VectorLichaoSlopeChminSegTree<int64_t, uint32_t>::_reserve(250000);
    OY::VectorLichaoSlopeChminSegTree<int64_t, uint32_t> S(M * 2 + 1, {0, inf});
    for (uint32_t i = 0; i != n; i++) {
        int64_t a, b;
        cin >> a >> b;
        S.add(0, M * 2, {a, b - a * M});
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            int64_t a, b;
            cin >> a >> b;
            S.add(0, M * 2, {a, b - a * M});
        } else {
            int p;
            cin >> p;
            cout << S.query(p + M).calc(p + M) << endl;
        }
    }
}
