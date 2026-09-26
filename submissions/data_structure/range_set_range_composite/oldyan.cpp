/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
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
#ifndef __OY_ASZKWTREE__
#define __OY_ASZKWTREE__
namespace OY {
    namespace ASZKW {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename ValueType>
        struct BaseNode {
            using value_type = ValueType;
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
        template <typename ValueType>
        struct FastSquareNode {
            using value_type = ValueType;
            static value_type square(const value_type &x, size_type n) { return x * n; }
            value_type m_val;
            const value_type &get() const { return m_val; }
            void set(const value_type &val) { m_val = val; }
        };
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
        template <typename Tp, typename ValueType, typename SizeType, typename = void>
        struct Has_Square : std::false_type {};
        template <typename Tp, typename ValueType, typename SizeType>
        struct Has_Square<Tp, ValueType, SizeType, void_t<decltype(Tp::square(std::declval<ValueType>(), std::declval<SizeType>()))>> : std::true_type {};
        template <typename Tp, typename ValueType>
        struct Has_Square<Tp, ValueType, void, void_t<decltype(Tp::square(std::declval<ValueType>()))>> : std::true_type {};
        template <typename Node, bool Info, bool FastCalc>
        struct AssignNode : Node {
            using typename Node::value_type;
            value_type m_lazy;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const value_type &get_lazy() const { return m_lazy; }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Node>
        struct AssignNode<Node, true, false> : Node {
            using typename Node::value_type;
            const value_type *m_lazy;
            bool has_lazy() const { return m_lazy; }
            const value_type *get_lazy() const { return m_lazy; }
            void clear_lazy() { m_lazy = nullptr; }
        };
        template <typename Node>
        struct AssignNode<Node, false, false> : Node {
            using typename Node::value_type;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const value_type &get_lazy() const { return Node::get(); }
            void clear_lazy() { m_cover = false; }
        };
        template <size_type BATCH = 1 << 17>
        struct VectorBufferWrap {
            template <typename Tp>
            struct type {
                static std::vector<std::vector<Tp>> s_bufs;
                static Tp *s_cur;
                static size_type s_rem;
                static void malloc(Tp *&ptr, size_type len) {
                    if (len > s_rem) s_bufs.emplace_back(BATCH), s_cur = s_bufs.back().data(), s_rem = BATCH;
                    ptr = s_cur, s_cur += len, s_rem -= len;
                }
            };
        };
        template <size_type BATCH>
        template <typename Tp>
        std::vector<std::vector<Tp>> VectorBufferWrap<BATCH>::template type<Tp>::s_bufs;
        template <size_type BATCH>
        template <typename Tp>
        Tp *VectorBufferWrap<BATCH>::template type<Tp>::s_cur;
        template <size_type BATCH>
        template <typename Tp>
        size_type VectorBufferWrap<BATCH>::template type<Tp>::s_rem;
        template <typename Node, bool Info, template <typename> typename BufferType = VectorBufferWrap<>::type>
        struct Tree {
            using value_type = typename Node::value_type;
            static constexpr bool has_fast_square = Has_Square<Node, value_type, size_type>::value;
            using node = AssignNode<Node, Info, has_fast_square>;
            static constexpr bool need_buffer = Info && !has_fast_square;
            using buffer_type = typename std::conditional<need_buffer, BufferType<value_type>, Ignore>::type;
            struct DefaultGetter {
                using value_type = typename node::value_type;
                value_type operator()(const node *p) const { return p->get(); }
                void operator()(value_type &x, const node *p) const { x = x + p->get(); }
                void operator()(const node *p, value_type &x) const { x = p->get() + x; }
                void operator()(value_type &x, const value_type &y) const { x = x + y; }
            };
            struct iterator {
                size_type m_index;
                node *m_ptr;
            };
            mutable std::vector<node> m_sub;
            size_type m_size, m_capacity, m_depth;
            static void _apply(node *p, const value_type *lazy, size_type level) {
                if constexpr (Info && !has_fast_square) p->set(lazy[level]), p->m_lazy = lazy;
            }
            static void _apply(node *p, const value_type &lazy, size_type level) {
                if constexpr (!Info)
                    p->set(lazy);
                else if constexpr (has_fast_square)
                    p->set(node::square(lazy, 1 << level));
                p->m_cover = true;
            }
            static void _pushdown(node *sub, size_type i, size_type level) {
                if (!sub[i].has_lazy()) return;
                _apply(sub + i * 2, sub[i].get_lazy(), level - 1);
                _apply(sub + (i * 2 + 1), sub[i].get_lazy(), level - 1);
                sub[i].clear_lazy();
            }
            static void _pushup(node *sub, size_type i, size_type len) { sub[i].set(sub[i * 2].get() + sub[i * 2 + 1].get()); }
            static void _fetch(node *sub, size_type l, size_type r, size_type level) {
                if (l == 1) return;
                _fetch(sub, l >> 1, r >> 1, level + 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(sub, i, level);
            }
            template <typename InitMapping = Ignore>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping = Ignore>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::max<size_type>(1, std::bit_width(m_size - 1)), m_capacity = 1 << m_depth;
                m_sub.assign(m_capacity * 2, {});
                node *sub = m_sub.data();
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_size; i++) sub[m_capacity + i].set(mapping(i));
                    for (size_type len = m_capacity / 2, cnt = (m_size + 1) / 2, k = 2; len; len >>= 1, cnt = (cnt + 1) / 2, k <<= 1)
                        for (size_type i = len; i != len + cnt; i++) _pushup(sub, i, k);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, const value_type &val) {
                do_for_node<false>(i, [&](node *p) { p->set(val); });
            }
            void modify(size_type left, size_type right, const value_type &val) {
                if (left == right) return modify(left, val);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_depth; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                sub[left].set(val), sub[right].set(val);
                size_type level = 0;
                if (j)
                    if constexpr (!Info || has_fast_square)
                        while (left >> 1 < right >> 1) {
                            if (!(left & 1)) _apply(sub + (left + 1), val, level);
                            _pushup(sub, left >>= 1, 1 << (level + 1));
                            if (right & 1) _apply(sub + (right - 1), val, level);
                            _pushup(sub, right >>= 1, 1 << (++level));
                        }
                    else {
                        value_type *lazy;
                        buffer_type::malloc(lazy, j);
                        lazy[0] = val;
                        for (size_type i = 1; i != j; i++)
                            if constexpr (Has_Square<node, value_type, void>::value)
                                lazy[i] = node::square(lazy[i - 1]);
                            else
                                lazy[i] = lazy[i - 1] + lazy[i - 1];
                        while (left >> 1 < right >> 1) {
                            if (!(left & 1)) _apply(sub + (left + 1), lazy, level);
                            _pushup(sub, left >>= 1, 1 << (level + 1));
                            if (right & 1) _apply(sub + (right - 1), lazy, level);
                            _pushup(sub, right >>= 1, 1 << (++level));
                        }
                    }
                while (left >>= 1) _pushup(sub, left, 1 << ++level);
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type i) const {
                return ((Tree *)(this))->do_for_node<true>(i, [&](const node *p) { return Getter()(p); });
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type left, size_type right) const {
                if (left == right) return query<Getter>(left);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_depth; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                typename Getter::value_type resl = Getter()(sub + left), resr = Getter()(sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) Getter()(resl, sub + (left ^ 1));
                    if (right & 1) Getter()(sub + (right ^ 1), resr);
                }
                Getter()(resl, resr);
                return resl;
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query_all() const { return Getter()(m_sub.data() + 1); }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                left += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, left >> d, d);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + left);
                if (!judge(val)) return left - m_capacity - 1;
                left++;
                for (size_type level = 0; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    get_type a(val);
                    Getter()(a, sub + (left >>= ctz));
                    level += ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_capacity; level--) {
                            _pushdown(sub, left, level);
                            get_type a(val);
                            Getter()(a, sub + (left <<= 1));
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_capacity, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Getter = DefaultGetter, typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                right += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, right >> d, d);
                using get_type = typename Getter::value_type;
                get_type val = Getter()(sub + right);
                if (!judge(val)) return right - m_capacity + 1;
                for (size_type level = 0; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_capacity);
                    get_type a(val);
                    Getter()(sub + ((right >>= ctz) - 1), a);
                    level -= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_capacity; level--) {
                            _pushdown(sub, right - 1, level);
                            get_type a(val);
                            Getter()(sub + ((right <<= 1) - 1), a);
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_capacity;
                    }
                }
                return 0;
            }
            template <typename Getter = DefaultGetter>
            iterator kth(typename Getter::value_type k) const {
                node *sub = m_sub.data();
                size_type i = 1, level = m_depth;
                for (; i < 1 << m_depth; level--) {
                    _pushdown(sub, i, level);
                    if (Getter()(sub + (i <<= 1)) <= k) k -= Getter()(sub + (i++));
                }
                return {i - (1 << m_depth), sub + i};
            }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(size_type i, Callback &&call) const -> decltype(call(m_sub.data())) {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth; d; d--) _pushdown(sub, i >> d, d);
                if constexpr (ReadOnly)
                    return call(sub + i);
                else {
                    call(sub + i);
                    for (size_type len = 1; len <<= 1, i >>= 1;) _pushup(sub, i, len);
                }
            }
            template <typename Callback>
            void do_for_each(Callback &&call) const {
                node *sub = m_sub.data();
                _fetch(sub, m_capacity, m_capacity + m_size - 1, 1);
                for (size_type i = m_capacity, j = 0; j != m_size; i++, j++) call(sub + i);
            }
        };
        template <typename Ostream, typename Node, bool Info, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Node, Info, BufferType> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename InitMapping = ASZKW::Ignore, typename TreeType = ASZKW::Tree<ASZKW::BaseNode<Tp>, false, BufferType>>
    auto make_AssignZkwTree(ASZKW::size_type length, InitMapping mapping = InitMapping()) -> TreeType { return TreeType(length, mapping); }
    template <template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename Iterator, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ASZKW::Tree<ASZKW::BaseNode<Tp>, false, BufferType>>
    auto make_AssignZkwTree(Iterator first, Iterator last) -> TreeType { return TreeType(first, last); }
    template <typename Tp, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename InitMapping, typename TreeType = ASZKW::Tree<ASZKW::BaseNode<Tp>, true, BufferType>>
    auto make_lazy_AssignZkwTree(ASZKW::size_type length, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename InitMapping, typename TreeType = ASZKW::Tree<ASZKW::FastSquareNode<Tp>, true, BufferType>>
    auto make_fast_square_AssignZkwTree(ASZKW::size_type length, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
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
    uint32_t n, q;
    cin >> n >> q;
    auto S = OY::make_lazy_AssignZkwTree<node>(n, [&](auto...) {
        uint32_t mul, add;
        cin >> mul >> add;
        return node{mul, add};
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r, mul, add;
            cin >> l >> r >> mul >> add;
            S.modify(l, r - 1, {mul, add});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}
