/*
lib:        https://github.com/old-yan/CP-template
author:     oldyan
*/
#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
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
#ifndef __OY_SEGMENTBEAT__
#define __OY_SEGMENTBEAT__
namespace OY {
    namespace SegBeat {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Node>
        struct Tree {
            using node = Node;
            struct DefaultGetter {
                using value_type = decltype(std::declval<node>().get());
                const value_type &operator()(node *x) const { return x->get(); }
            };
            mutable std::vector<node> m_sub;
            size_type m_capacity, m_depth, m_size;
            template <typename Modify>
            static void _apply(node *sub, size_type i, Modify &&modify, size_type len) {
                if (!node::map(modify, sub + i, len)) {
                    _pushdown(sub, i, len);
                    _apply(sub, i * 2, modify, len >> 1), _apply(sub, i * 2 + 1, modify, len >> 1);
                    _pushup(sub, i);
                }
            }
            static void _pushdown(node *sub, size_type i, size_type len) { sub[i].pushdown(sub + i * 2, sub + (i * 2 + 1), len); }
            static void _pushup(node *sub, size_type i) { sub[i].pushup(sub + (i * 2), sub + (i * 2 + 1)); }
            static void _fetch(node *sub, size_type l, size_type r, size_type len) {
                if (l == 1) return;
                _fetch(sub, l >> 1, r >> 1, len << 1);
                for (size_type i = l >> 1; i <= r >> 1; i++) _pushdown(sub, i, len);
            }
            Tree() = default;
            template <typename InitMapping = Ignore>
            Tree(size_type length, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (!(m_size = length)) return;
                m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth;
                m_sub.resize(m_capacity * 2);
                node *sub = m_sub.data();
                if constexpr (!std::is_same<InitMapping, Ignore>::value) {
                    for (size_type i = 0; i < m_size; i++) sub[m_capacity + i].set(mapping(i));
                    for (size_type len = m_capacity / 2, cnt = (m_size + 1) / 2, k = 2; len; len >>= 1, cnt = (cnt + 1) / 2, k <<= 1)
                        for (size_type i = len; i != len + cnt; i++) _pushup(sub, i);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            template <typename Modify>
            void modify(size_type i, Modify &&modify) {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, i >> d, len);
                modify(sub + i);
                while (i >>= 1) _pushup(sub, i);
            }
            template <typename Modify>
            void add(size_type i, Modify &&modify) {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, i >> d, len);
                _apply(sub, i, modify, 1);
                while (i >>= 1) _pushup(sub, i);
            }
            template <typename Modify>
            void add(size_type left, size_type right, Modify &&modify) {
                if (left == right) return add(left, modify);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                _apply(sub, left, modify, 1), _apply(sub, right, modify, 1), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) _apply(sub, left + 1, modify, len);
                    _pushup(sub, left >>= 1);
                    if (right & 1) _apply(sub, right - 1, modify, len);
                    _pushup(sub, right >>= 1);
                    len <<= 1;
                }
                while (left >>= 1) _pushup(sub, left);
            }
            template <typename Getter = DefaultGetter>
            typename Getter::value_type query(size_type i) const {
                i += m_capacity;
                node *sub = m_sub.data();
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(sub, i >> d, len);
                return Getter()(sub + i);
            }
            template <typename Getter>
            typename Getter::value_type query(size_type left, size_type right) const {
                if (left == right) return query<Getter>(left);
                left += m_capacity, right += m_capacity;
                node *sub = m_sub.data();
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(sub, left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(sub, left >> d, len), _pushdown(sub, right >> d, len);
                typename Getter::value_type resl = Getter()(sub + left), resr = Getter()(sub + right);
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) resl = Getter()(resl, Getter()(sub + (left ^ 1)));
                    if (right & 1) resr = Getter()(Getter()(sub + (right ^ 1)), resr);
                }
                return Getter()(resl, resr);
            }
            template <typename Callback>
            void do_for_each(Callback &&call) {
                node *sub = m_sub.data();
                _fetch(sub, m_capacity, m_capacity + m_size - 1, 2);
                for (size_type i = m_capacity, j = 0; j != m_size; i++, j++) call(sub + i);
            }
        };
        template <typename Ostream, typename Node>
        Ostream &operator<<(Ostream &out, const Tree<Node> &x) {
            out << "[";
            for (size_type i = 0; i < x.m_size; i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
        template <typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool IsVoid>
        struct ChminChmaxNodeBaseBase {
            SumType m_sum;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, true, false, false> {
            SumType m_sum;
            CountType m_max_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, false, true, false> {
            SumType m_sum;
            CountType m_min_cnt;
        };
        template <typename CountType, typename SumType>
        struct ChminChmaxNodeBaseBase<CountType, SumType, false, false, false> {
            SumType m_sum;
        };
        template <typename CountType, bool ChangeMin, bool ChangeMax>
        struct ChminChmaxNodeBaseBase<CountType, void, ChangeMin, ChangeMax, true> {
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNodeBase {
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, true> : ChminChmaxNodeBaseBase<CountType, SumType, true, true, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_min1, m_min2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, false> : ChminChmaxNodeBaseBase<CountType, SumType, true, true, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_min1, m_min2;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, true> : ChminChmaxNodeBaseBase<CountType, SumType, true, false, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, false> : ChminChmaxNodeBaseBase<CountType, SumType, true, false, std::is_void<SumType>::value> {
            ValueType m_max1, m_max2;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, true> : ChminChmaxNodeBaseBase<CountType, SumType, false, true, std::is_void<SumType>::value> {
            ValueType m_min1, m_min2, m_inc;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, false> : ChminChmaxNodeBaseBase<CountType, SumType, false, true, std::is_void<SumType>::value> {
            ValueType m_min1, m_min2;
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd, ValueType Min = std::numeric_limits<ValueType>::min() / 2, ValueType Max = std::numeric_limits<ValueType>::max() / 2>
        struct ChminChmaxNode : ChminChmaxNodeBase<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd> {
            static constexpr ValueType min = Min, max = Max;
            using node_type = ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd, Min, Max>;
            struct Chmin {
                ValueType m_chmin_by;
            };
            struct Chmax {
                ValueType m_chmax_by;
            };
            struct Assign {
                ValueType m_val;
            };
            struct Add {
                ValueType m_add_by;
            };
            struct MinGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_min1; }
                value_type operator()(value_type x, value_type y) const { return std::min(x, y); }
            };
            struct MaxGetter {
                using value_type = ValueType;
                value_type operator()(node_type *x) const { return x->m_max1; }
                value_type operator()(value_type x, value_type y) const { return std::max(x, y); }
            };
            struct SumGetter {
                using value_type = SumType;
                value_type operator()(node_type *x) const { return x->m_sum; }
                value_type operator()(value_type x, value_type y) const { return x + y; }
            };
            static bool map(const Chmin &chmin, node_type *x, CountType len) {
                if (x->m_max1 <= chmin.m_chmin_by) return true;
                if (x->m_max2 < chmin.m_chmin_by) return x->chmin_by(chmin.m_chmin_by), true;
                return false;
            }
            static bool map(const Chmax &chmax, node_type *x, CountType len) {
                if (x->m_min1 >= chmax.m_chmax_by) return true;
                if (x->m_min2 > chmax.m_chmax_by) return x->chmax_by(chmax.m_chmax_by), true;
                return false;
            }
            static bool map(const Assign &assign, node_type *x, CountType len) { return x->m_min1 == x->m_max1 ? x->add_by(assign.m_val - x->m_max1, len), true : false; }
            static bool map(const Add &inc, node_type *x, CountType len) { return x->add_by(inc.m_add_by, len), true; }
            void set(ValueType val) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum = val;
                if constexpr (ChangeMin) {
                    this->m_max1 = val, this->m_max2 = min;
                    if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = 1;
                }
                if constexpr (ChangeMax) {
                    this->m_min1 = val, this->m_min2 = max;
                    if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = 1;
                }
            }
            const ValueType &get() const { return this->m_max1; }
            void add_by(ValueType inc, CountType len) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(inc) * len;
                if constexpr (ChangeMin) {
                    this->m_max1 += inc;
                    if (this->m_max2 != min) this->m_max2 += inc;
                }
                if constexpr (ChangeMax) {
                    this->m_min1 += inc;
                    if (this->m_min2 != max) this->m_min2 += inc;
                }
                this->m_inc += inc;
            }
            void chmin_by(ValueType val) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(val - this->m_max1) * this->m_max_cnt;
                if constexpr (ChangeMax) {
                    if (this->m_min1 == this->m_max1) this->m_min1 = val;
                    if (this->m_min2 == this->m_max1) this->m_min2 = val;
                }
                this->m_max1 = val;
            }
            void chmax_by(ValueType val) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum += SumType(val - this->m_min1) * this->m_min_cnt;
                if constexpr (ChangeMin) {
                    if (this->m_max1 == this->m_min1) this->m_max1 = val;
                    if (this->m_max2 == this->m_min1) this->m_max2 = val;
                }
                this->m_min1 = val;
            }
            void pushup(node_type *lchild, node_type *rchild) {
                if constexpr (!std::is_void<SumType>::value) this->m_sum = lchild->m_sum + rchild->m_sum;
                if constexpr (ChangeMin)
                    if (lchild->m_max1 == rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max2);
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = lchild->m_max_cnt + rchild->m_max_cnt;
                    } else if (lchild->m_max1 > rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = lchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max1);
                    } else {
                        this->m_max1 = rchild->m_max1;
                        if constexpr (!std::is_void<SumType>::value) this->m_max_cnt = rchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max1, rchild->m_max2);
                    }
                if constexpr (ChangeMax)
                    if (lchild->m_min1 == rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min2);
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = lchild->m_min_cnt + rchild->m_min_cnt;
                    } else if (lchild->m_min1 < rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = lchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min1);
                    } else {
                        this->m_min1 = rchild->m_min1;
                        if constexpr (!std::is_void<SumType>::value) this->m_min_cnt = rchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min1, rchild->m_min2);
                    }
            }
            void pushdown(node_type *lchild, node_type *rchild, CountType len) {
                if constexpr (ChangeAdd)
                    if (this->m_inc) {
                        lchild->add_by(this->m_inc, len >> 1);
                        rchild->add_by(this->m_inc, len >> 1);
                        this->m_inc = 0;
                    }
                if constexpr (ChangeMin)
                    if (this->m_max1 < lchild->m_max1) lchild->chmin_by(this->m_max1);
                if constexpr (ChangeMax)
                    if (this->m_min1 > lchild->m_min1) lchild->chmax_by(this->m_min1);
                if constexpr (ChangeMin)
                    if (this->m_max1 < rchild->m_max1) rchild->chmin_by(this->m_max1);
                if constexpr (ChangeMax)
                    if (this->m_min1 > rchild->m_min1) rchild->chmax_by(this->m_min1);
            }
        };
    }
    template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd, ValueType Min = std::numeric_limits<ValueType>::min() / 2, ValueType Max = std::numeric_limits<ValueType>::max() / 2>
    using ChminChmaxAddTree = SegBeat::Tree<SegBeat::ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd, Min, Max>>;
}
#endif
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::ChminChmaxAddTree<int64_t, int32_t, int64_t, true, true, true> S(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    using node = decltype(S)::node;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Chmin{b});
        } else if (op == '1') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Chmax{b});
        } else if (op == '2') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Add{b});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<node::SumGetter>(l, r - 1) << '\n';
        }
    }
}
