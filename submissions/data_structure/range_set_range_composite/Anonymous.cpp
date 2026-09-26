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
        template <typename Tp>
        struct BaseMonoid {
            using value_type = Tp;
        };
        template <typename Tp>
        struct SumMonoid {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp square(const Tp &x, size_type n) { return x * n; }
        };
        template <typename Tp, Tp Identity, typename Operation, typename Square>
        struct FastSquareMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
            static Tp square(const Tp &x, size_type n) { return Square()(x, n); }
        };
        template <typename Tp, Tp Identity, typename Operation>
        struct LazyMonoid {
            using value_type = Tp;
            static Tp identity() { return Identity; }
            static Tp op(const Tp &x, const Tp &y) { return Operation()(x, y); }
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
        template <typename Tp, typename ValueType, typename = void>
        struct Has_Op : std::false_type {};
        template <typename Tp, typename ValueType>
        struct Has_Op<Tp, ValueType, void_t<decltype(Tp::op(std::declval<ValueType>(), std::declval<ValueType>()))>> : std::true_type {};
        template <typename Tp, bool Info, bool FastCalc>
        struct AssignNode {
            Tp m_val;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const Tp &get_lazy() const { return m_val; }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Tp>
        struct AssignNode<Tp, true, true> {
            Tp m_val, m_lazy;
            bool m_cover;
            bool has_lazy() const { return m_cover; }
            const Tp &get_lazy() const { return m_lazy; }
            void clear_lazy() { m_cover = false; }
        };
        template <typename Tp>
        struct AssignNode<Tp, true, false> {
            Tp m_val;
            const Tp *m_lazy;
            bool has_lazy() const { return m_lazy; }
            const Tp *get_lazy() const { return m_lazy; }
            void clear_lazy() { m_lazy = nullptr; }
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
        template <typename Monoid, template <typename> typename BufferType = VectorBufferWrap<>::type>
        class Tree {
        public:
            using value_type = typename Monoid::value_type;
            static constexpr bool has_fast_square = Has_Square<Monoid, value_type, size_type>::value;
            static constexpr bool has_info = Has_Op<Monoid, value_type>::value;
            using node = AssignNode<value_type, has_info, has_fast_square>;
            static constexpr bool need_buffer = has_info && !has_fast_square;
            using buffer_type = typename std::conditional<need_buffer, BufferType<value_type>, Ignore>::type;
            struct iterator {
                size_type m_index;
                node *m_ptr;
            };
        private:
            mutable std::vector<node> m_sub;
            size_type m_size, m_cap, m_dep;
            static void _apply(node *p, const value_type *lazy, size_type level) { p->m_val = lazy[level], p->m_lazy = lazy; }
            static void _apply(node *p, const value_type &lazy, size_type level) {
                if constexpr (!has_info)
                    p->m_val = lazy;
                else if constexpr (has_fast_square)
                    p->m_val = Monoid::square(lazy, 1 << level);
                p->m_cover = true;
            }
            static void _pushdown(node *sub, size_type i, size_type level) {
                if (!sub[i].has_lazy()) return;
                _apply(sub + i * 2, sub[i].get_lazy(), level - 1);
                _apply(sub + (i * 2 + 1), sub[i].get_lazy(), level - 1);
                sub[i].clear_lazy();
            }
            static void _pushup(node *sub, size_type i, size_type len) {
                if constexpr (has_info) sub[i].m_val = Monoid::op(sub[i * 2].m_val, sub[i * 2 + 1].m_val);
            }
            static void _fetch(node *sub, size_type l, size_type r, size_type level) {
                if (l == 1) return;
                _fetch(sub, l >> 1, r >> 1, level + 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(sub, i, level);
            }
        public:
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            size_type size() const { return m_size; }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_dep = std::max<size_type>(1, std::bit_width(m_size - 1)), m_cap = 1 << m_dep;
                m_sub.assign(m_cap * 2, Monoid::identity());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_dep = std::max<size_type>(1, std::bit_width(m_size - 1)), m_cap = 1 << m_dep;
                m_sub.assign(m_cap * 2, {});
                node *sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[m_cap + i].m_val = mapping(i);
                if constexpr (has_info)
                    for (size_type i = m_size; i != m_cap; i++) sub[m_cap + i].m_val = Monoid::identity();
                for (size_type len = m_cap / 2, k = 2; len; len >>= 1, k <<= 1)
                    for (size_type i = len; i != len << 1; i++) _pushup(sub, i, k);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void modify(size_type i, value_type val) {
                do_for_node<false>(i, [&val](node *p) { p->m_val = val; });
            }
            void modify(size_type left, size_type right, value_type val) {
                if (left == right) return modify(left, val);
                left += m_cap, right += m_cap;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_dep; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                sub[left].m_val = sub[right].m_val = val;
                size_type level = 0;
                if (j)
                    if constexpr (!has_info || has_fast_square)
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
                            if constexpr (Has_Square<Monoid, value_type, void>::value)
                                lazy[i] = Monoid::square(lazy[i - 1]);
                            else
                                lazy[i] = Monoid::op(lazy[i - 1], lazy[i - 1]);
                        while (left >> 1 < right >> 1) {
                            if (!(left & 1)) _apply(sub + (left + 1), lazy, level);
                            _pushup(sub, left >>= 1, 1 << (level + 1));
                            if (right & 1) _apply(sub + (right - 1), lazy, level);
                            _pushup(sub, right >>= 1, 1 << (++level));
                        }
                    }
                while (left >>= 1) _pushup(sub, left, 1 << ++level);
            }
            value_type query(size_type i) const {
                return ((Tree *)(this))->do_for_node<true>(i, [&](const node *p) { return p->m_val; });
            }
            value_type query(size_type left, size_type right) const {
                if (left == right) return query(left);
                left += m_cap, right += m_cap;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1;
                for (size_type d = m_dep; d > j; d--) _pushdown(sub, left >> d, d);
                for (size_type d = j; d; d--) _pushdown(sub, left >> d, d), _pushdown(sub, right >> d, d);
                value_type resl = sub[left].m_val, resr = sub[right].m_val;
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) resl = Monoid::op(resl, sub[left ^ 1].m_val);
                    if (right & 1) resr = Monoid::op(sub[right ^ 1].m_val, resr);
                }
                return Monoid::op(resl, resr);
            }
            value_type query_all() const { return m_sub[1].m_val; }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                left += m_cap;
                node *sub = m_sub.data();
                for (size_type d = m_dep; d; d--) _pushdown(sub, left >> d, d);
                value_type val = sub[left].m_val;
                if (!judge(val)) return left - m_cap - 1;
                left++;
                for (size_type level = 0; std::popcount(left) > 1;) {
                    size_type ctz = std::countr_zero(left);
                    value_type a = Monoid::op(val, sub[left >>= ctz].m_val);
                    level += ctz;
                    if (judge(a))
                        val = a, left++;
                    else {
                        for (; left < m_cap; level--) {
                            _pushdown(sub, left, level);
                            value_type a = Monoid::op(val, sub[left <<= 1].m_val);
                            if (judge(a)) val = a, left++;
                        }
                        return std::min(left - m_cap, m_size) - 1;
                    }
                }
                return m_size - 1;
            }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                right += m_cap;
                node *sub = m_sub.data();
                for (size_type d = m_dep; d; d--) _pushdown(sub, right >> d, d);
                value_type val = sub[right].m_val;
                if (!judge(val)) return right - m_cap + 1;
                for (size_type level = 0; std::popcount(right) > 1;) {
                    size_type ctz = std::countr_zero(right - m_cap);
                    value_type a = Monoid::op(sub[(right >>= ctz) - 1].m_val, val);
                    level -= ctz;
                    if (judge(a))
                        val = a, right--;
                    else {
                        for (; right <= m_cap; level--) {
                            _pushdown(sub, right - 1, level);
                            value_type a = Monoid::op(sub[(right <<= 1) - 1].m_val, val);
                            if (judge(a)) val = a, right--;
                        }
                        return right - m_cap;
                    }
                }
                return 0;
            }
            iterator kth(value_type k) const {
                static_assert(has_info, "Kth Operation Cannot Be Done Without Range Sum");
                node *sub = m_sub.data();
                size_type i = 1, level = m_dep;
                for (; i < 1 << m_dep; level--) {
                    _pushdown(sub, i, level);
                    if (sub[i <<= 1].m_val <= k) k -= sub[i++].m_val;
                }
                return {i - (1 << m_dep), sub + i};
            }
            template <bool ReadOnly, typename Callback>
            auto do_for_node(size_type i, Callback &&call) const -> decltype(call(m_sub.data())) {
                i += m_cap;
                node *sub = m_sub.data();
                for (size_type d = m_dep; d; d--) _pushdown(sub, i >> d, d);
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
                _fetch(sub, m_cap, m_cap + m_size - 1, 1);
                for (size_type i = m_cap, j = 0; j != m_size; i++, j++) call(sub + i);
            }
        };
        template <typename Ostream, typename Monoid, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Monoid, BufferType> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, Tp Identity, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename Operation, typename Square, typename InitMapping, typename TreeType = ASZKW::Tree<ASZKW::FastSquareMonoid<Tp, Identity, Operation, Square>, BufferType>>
    auto make_fast_square_AssignZkwTree(ASZKW::size_type length, Operation op, Square square, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Tp Identity, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type, typename Operation, typename InitMapping, typename TreeType = ASZKW::Tree<ASZKW::LazyMonoid<Tp, Identity, Operation>, BufferType>>
    auto make_lazy_AssignZkwTree(ASZKW::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type>
    using AssignZkw = ASZKW::Tree<ASZKW::BaseMonoid<Tp>, BufferType>;
    template <typename Tp, template <typename> typename BufferType = ASZKW::VectorBufferWrap<>::type>
    using AssignSumZkw = ASZKW::Tree<ASZKW::SumMonoid<Tp>, BufferType>;
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
constexpr node identity{1, 0};
int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        uint32_t mul, add;
        cin >> mul >> add;
        return node{mul, add};
    };
    auto S = OY::make_lazy_AssignZkwTree<node, identity>(n, std::plus<>(), read);
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
