#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_set>
using std::array, std::bitset, std::cin, std::copy, std::cout, std::deque, std::endl, std::fill, std::greater, std::less, std::lower_bound, std::map, std::multiset, std::pair, std::partial_sum, std::priority_queue, std::queue, std::reverse, std::set, std::sort, std::stack, std::string, std::swap, std::tuple, std::upper_bound, std::vector;
template <typename Tp, typename Fp, typename Compare = std::less<void>>
bool chmax(Tp &a, const Fp &b, Compare comp = Compare()) { return comp(a, b) ? a = b, true : false; }
template <typename Tp, typename Fp, typename Compare = std::less<void>>
bool chmin(Tp &a, const Fp &b, Compare comp = Compare()) { return comp(b, a) ? a = b, true : false; }
#define cin OY::IO::InputHelper::getInstance()
#define cout OY::IO::OutputHelper::getInstance()
#define endl '\n'
namespace OY {
    namespace IO {
        using size_type = size_t;
        static constexpr size_type INPUT_BUFFER_SIZE = 1 << 16, OUTPUT_BUFFER_SIZE = 1 << 16, MAX_INTEGER_SIZE = 20, MAX_FLOAT_SIZE = 50;
        struct InputHelper {
            FILE *m_filePtr;
            char m_buf[INPUT_BUFFER_SIZE], *m_end, *m_cursor;
            bool m_ok;
            InputHelper &setBad() {
                m_ok = false;
                return *this;
            }
            template <size_type BlockSize>
            void _reserve() {
                size_type a = m_end - m_cursor;
                if (a >= BlockSize) return;
                memmove(m_buf, m_cursor, a);
                m_cursor = m_buf;
                size_type b = fread(m_buf + a, 1, INPUT_BUFFER_SIZE - a, m_filePtr);
                if (a + b < INPUT_BUFFER_SIZE) m_end = m_buf + a + b, *m_end = EOF;
            }
            template <typename Tp, typename BinaryOperation>
            InputHelper &fillInteger(Tp &ret, BinaryOperation op) {
                if (!isdigit(*m_cursor)) return setBad();
                ret = op(Tp(0), *m_cursor - '0');
                size_type len = 1;
                while (isdigit(*(m_cursor + len))) ret = op(ret * 10, *(m_cursor + len++) - '0');
                m_cursor += len;
                return *this;
            }
            explicit InputHelper(const char *inputFileName) : m_ok(true), m_cursor(m_buf + INPUT_BUFFER_SIZE), m_end(m_buf + INPUT_BUFFER_SIZE) { m_filePtr = *inputFileName ? fopen(inputFileName, "rt") : stdin; }
            ~InputHelper() { fclose(m_filePtr); }
            static InputHelper &getInstance() {
#ifdef OY_LOCAL
                static InputHelper s_obj("in.txt");
#else
                static InputHelper s_obj("");
#endif
                return s_obj;
            }
            static bool isBlank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
            static bool isEndline(char c) { return c == '\n' || c == EOF; }
            const char &getChar_Checked() {
                _reserve<1>();
                return *m_cursor;
            }
            const char &getChar_Unchecked() const { return *m_cursor; }
            void next() { ++m_cursor; }
            template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            InputHelper &operator>>(Tp &num) {
                while (isBlank(getChar_Checked())) next();
                _reserve<MAX_INTEGER_SIZE>();
                if (getChar_Unchecked() != '-') return fillInteger(num, std::plus<Tp>());
                next();
                return fillInteger(num, std::minus<Tp>());
            }
            template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            InputHelper &operator>>(Tp &num) {
                while (isBlank(getChar_Checked())) next();
                _reserve<MAX_INTEGER_SIZE>();
                return fillInteger(num, std::plus<Tp>());
            }
            InputHelper &operator>>(char &c) {
                while (isBlank(getChar_Checked())) next();
                if (getChar_Checked() == EOF) return setBad();
                c = getChar_Checked();
                next();
                return *this;
            }
        };
        struct OutputHelper {
            FILE *m_filePtr = nullptr;
            char m_buf[OUTPUT_BUFFER_SIZE], *m_end, *m_cursor;
            char m_tempBuf[MAX_FLOAT_SIZE], *m_tempBufCursor, *m_tempBufDot;
            uint64_t m_floatReserve, m_floatRatio;
            void _write() {
                fwrite(m_buf, 1, m_cursor - m_buf, m_filePtr);
                m_cursor = m_buf;
            }
            template <size_type BlockSize>
            void _reserve() {
                size_type a = m_end - m_cursor;
                if (a >= BlockSize) return;
                _write();
            }
            OutputHelper(const char *outputFileName, size_type prec = 6) {
                if (!*outputFileName)
                    m_filePtr = stdout;
                else
                    m_filePtr = fopen(outputFileName, "wt");
                m_cursor = m_buf;
                m_end = m_buf + OUTPUT_BUFFER_SIZE;
                m_tempBufCursor = m_tempBuf;
            }
            static OutputHelper &getInstance() {
#ifdef OY_LOCAL
                static OutputHelper s_obj("out.txt");
#else
                static OutputHelper s_obj("");
#endif
                return s_obj;
            }
            ~OutputHelper() {
                flush();
                fclose(m_filePtr);
            }
            OutputHelper &flush() {
                _write(), fflush(m_filePtr);
                return *this;
            }
            void putChar(const char &c) {
                if (m_cursor == m_end) _write();
                *m_cursor++ = c;
            }
            void putS(const char *c) {
                while (*c) putChar(*c++);
            }
            template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            OutputHelper &operator<<(Tp ret) {
                _reserve<MAX_INTEGER_SIZE>();
                size_type len = 0;
                if (ret >= 0) {
                    do *(m_cursor + len++) = '0' + ret % 10, ret /= 10;
                    while (ret);

                } else {
                    putChar('-');
                    do *(m_cursor + len++) = '0' - ret % 10, ret /= 10;
                    while (ret);
                }
                for (size_type i = 0, j = len - 1; i < j;) std::swap(*(m_cursor + i++), *(m_cursor + j--));
                m_cursor += len;
                return *this;
            }
            template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
            OutputHelper &operator<<(Tp ret) {
                _reserve<MAX_INTEGER_SIZE>();
                size_type len = 0;
                do *(m_cursor + len++) = '0' + ret % 10, ret /= 10;
                while (ret);
                for (size_type i = 0, j = len - 1; i < j;) std::swap(*(m_cursor + i++), *(m_cursor + j--));
                m_cursor += len;
                return *this;
            }
            OutputHelper &operator<<(const char &ret) {
                putChar(ret);
                return *this;
            }
            OutputHelper &operator<<(const char *ret) {
                putS(ret);
                return *this;
            }
        };
    }
}

namespace OY {
    namespace SegBeat {
        using size_type = uint32_t;
        struct NoInit {};
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
        template <typename Tp, typename NodePtr, typename SizeType, typename = void>
        struct Has_pushdown : std::false_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, size_type, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>(), std::declval<size_type>()))>> : std::true_type {};
        template <typename Tp, typename NodePtr>
        struct Has_pushdown<Tp, NodePtr, void, void_t<decltype(std::declval<Tp>().pushdown(std::declval<NodePtr>(), std::declval<NodePtr>()))>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_unswapable : std::false_type {};
        template <typename Tp>
        struct Has_unswapable<Tp, void_t<decltype(Tp::unswapable)>> : std::true_type {};
        template <typename Tp, typename = void>
        struct Has_init_clear_lazy : std::false_type {};
        template <typename Tp>
        struct Has_init_clear_lazy<Tp, void_t<decltype(Tp::init_clear_lazy)>> : std::true_type {};
        template <typename Node, size_type MAX_NODE>
        struct Tree {
            using node = Node;
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            node *m_sub;
            size_type m_capacity, m_depth, m_size;
            template <typename Modify>
            void _apply(size_type i, Modify &&modify, size_type len) {
                if (!node::map(modify, m_sub + i, len)) {
                    _pushdown(i, len);
                    _apply(i * 2, modify, len >> 1), _apply(i * 2 + 1, modify, len >> 1);
                    _pushup(i);
                }
            }
            void _pushdown(size_type i, size_type len) const {
                if constexpr (Has_pushdown<node, node *, size_type>::value)
                    m_sub[i].pushdown(m_sub + i * 2, m_sub + (i * 2 + 1), len);
                else
                    m_sub[i].pushdown(m_sub + i * 2, m_sub + (i * 2 + 1));
            }
            void _pushup(size_type i) const { m_sub[i].pushup(m_sub + (i * 2), m_sub + (i * 2 + 1)); }
            template <typename InitMapping = NoInit>
            Tree(size_type length = 0, InitMapping mapping = InitMapping()) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping = InitMapping()) {
                if (m_size = length) {
                    m_depth = std::bit_width(m_size - 1), m_capacity = 1 << m_depth;
                    m_sub = s_buffer + s_use_count, s_use_count += m_capacity << 1;
                    if constexpr (!std::is_same<InitMapping, NoInit>::value) {
                        for (size_type i = 0; i < m_size; i++) m_sub[m_capacity + i].set(mapping(i));
                        for (size_type i = m_capacity; --i;) _pushup(i);
                    }
                    if constexpr (Has_init_clear_lazy<node>::value)
                        if constexpr (node::init_clear_lazy)
                            for (size_type i = 1; i < m_capacity; i++) m_sub[i].clear_lazy();
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            template <typename Modify>
            void add(size_type i, Modify &&modify) {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                _apply(i, modify, 1);
                while (i >>= 1) _pushup(i);
            }
            template <typename Modify>
            void add(size_type left, size_type right, Modify &&modify) {
                if (left == right) return add(left, modify);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                _apply(left, modify, 1), _apply(right, modify, 1), len = 1;
                while (left >> 1 < right >> 1) {
                    if (!(left & 1)) _apply(left + 1, modify, len);
                    _pushup(left >>= 1);
                    if (right & 1) _apply(right - 1, modify, len);
                    _pushup(right >>= 1);
                    len <<= 1;
                }
                while (left >>= 1) _pushup(left);
            }
            template <typename Getter>
            typename Getter::value_type query(size_type i) const {
                i += m_capacity;
                for (size_type d = m_depth, len = m_capacity; d; d--, len >>= 1) _pushdown(i >> d, len);
                return Getter()(m_sub + i);
            }
            template <typename Getter>
            typename Getter::value_type query(size_type left, size_type right) const {
                if (left == right) return query<Getter>(left);
                left += m_capacity, right += m_capacity;
                size_type j = std::bit_width(left ^ right) - 1, len = m_capacity;
                for (size_type d = m_depth; d > j; d--, len >>= 1) _pushdown(left >> d, len);
                for (size_type d = j; d; d--, len >>= 1) _pushdown(left >> d, len), _pushdown(right >> d, len);
                if constexpr (Has_unswapable<node>::value)
                    if constexpr (node::unswapable) {
                        typename Getter::value_type res = Getter()(m_sub + left);
                        for (size_type i = 0; i < j; i++)
                            if (!(left >> i & 1)) res = Getter()(res, Getter()(m_sub + (left >> i ^ 1)));
                        for (size_type i = j - 1; ~i; i--)
                            if (right >> i & 1) res = Getter()(res, Getter()(m_sub + (right >> i ^ 1)));
                        return Getter()(res, Getter()(m_sub + right));
                    }
                typename Getter::value_type res = Getter()(Getter()(m_sub + left), Getter()(m_sub + right));
                for (; left >> 1 != right >> 1; left >>= 1, right >>= 1) {
                    if (!(left & 1)) res = Getter()(res, Getter()(m_sub + (left ^ 1)));
                    if (right & 1) res = Getter()(res, Getter()(m_sub + (right ^ 1)));
                }
                return res;
            }
        };
        template <typename Node, size_type MAX_NODE>
        typename Tree<Node, MAX_NODE>::node Tree<Node, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Node, size_type MAX_NODE>
        size_type Tree<Node, MAX_NODE>::s_use_count;
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNodeBase {
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, true> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_min1, m_min2, m_inc;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, true, false> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_min1, m_min2;
            CountType m_max_cnt, m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, true> {
            SumType m_sum;
            ValueType m_max1, m_max2, m_inc;
            CountType m_max_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, true, false, false> {
            SumType m_sum;
            ValueType m_max1, m_max2;
            CountType m_max_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, true> {
            SumType m_sum;
            ValueType m_min1, m_min2, m_inc;
            CountType m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType>
        struct ChminChmaxNodeBase<ValueType, CountType, SumType, false, true, false> {
            SumType m_sum;
            ValueType m_min1, m_min2;
            CountType m_min_cnt;
        };
        template <typename ValueType, typename CountType, typename SumType, bool ChangeMin, bool ChangeMax, bool ChangeAdd>
        struct ChminChmaxNode : ChminChmaxNodeBase<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd> {
            static constexpr ValueType min = std::numeric_limits<ValueType>::min() / 2;
            static constexpr ValueType max = std::numeric_limits<ValueType>::max() / 2;
            using node_type = ChminChmaxNode<ValueType, CountType, SumType, ChangeMin, ChangeMax, ChangeAdd>;
            struct Chmin {
                ValueType m_chmin_by;
            };
            struct Chmax {
                ValueType m_chmax_by;
            };
            struct Add {
                ValueType m_add_by;
            };
            struct MinGetter {
                using value_type = ValueType;
                ValueType operator()(node_type *x) const { return x->m_min1; }
                ValueType operator()(ValueType x, ValueType y) const { return std::min(x, y); }
            };
            struct MaxGetter {
                using value_type = ValueType;
                ValueType operator()(node_type *x) const { return x->m_max1; }
                ValueType operator()(ValueType x, ValueType y) const { return std::max(x, y); }
            };
            struct SumGetter {
                using value_type = SumType;
                SumType operator()(node_type *x) const { return x->m_sum; }
                SumType operator()(SumType x, SumType y) const { return x + y; }
            };
            static bool map(const Chmin &chmin, node_type *x, uint32_t len) {
                if (x->m_max1 <= chmin.m_chmin_by) return true;
                if (x->m_max2 < chmin.m_chmin_by) return x->chmin_by(chmin.m_chmin_by), true;
                return false;
            }
            static bool map(const Chmax &chmax, node_type *x, uint32_t len) {
                if (x->m_min1 >= chmax.m_chmax_by) return true;
                if (x->m_min2 > chmax.m_chmax_by) return x->chmax_by(chmax.m_chmax_by), true;
                return false;
            }
            static bool map(const Add &add, node_type *x, uint32_t len) { return x->add_by(add.m_add_by, len), true; }
            void set(ValueType val) {
                this->m_sum = val;
                if constexpr (ChangeMin) this->m_max1 = val, this->m_max2 = min, this->m_max_cnt = 1;
                if constexpr (ChangeMax) this->m_min1 = val, this->m_min2 = max, this->m_min_cnt = 1;
            }
            void add_by(ValueType inc, uint32_t len) {
                this->m_sum += SumType(inc) * len;
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
                this->m_sum += SumType(val - this->m_max1) * this->m_max_cnt;
                if constexpr (ChangeMax) {
                    if (this->m_min1 == this->m_max1) this->m_min1 = val;
                    if (this->m_min2 == this->m_max1) this->m_min2 = val;
                }
                this->m_max1 = val;
            }
            void chmax_by(ValueType val) {
                this->m_sum += SumType(val - this->m_min1) * this->m_min_cnt;
                if constexpr (ChangeMin) {
                    if (this->m_max1 == this->m_min1) this->m_max1 = val;
                    if (this->m_max2 == this->m_min1) this->m_max2 = val;
                }
                this->m_min1 = val;
            }
            void pushup(node_type *lchild, node_type *rchild) {
                this->m_sum = lchild->m_sum + rchild->m_sum;
                if constexpr (ChangeMin)
                    if (lchild->m_max1 == rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max2);
                        this->m_max_cnt = lchild->m_max_cnt + rchild->m_max_cnt;
                    } else if (lchild->m_max1 > rchild->m_max1) {
                        this->m_max1 = lchild->m_max1;
                        this->m_max_cnt = lchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max2, rchild->m_max1);
                    } else {
                        this->m_max1 = rchild->m_max1;
                        this->m_max_cnt = rchild->m_max_cnt;
                        this->m_max2 = std::max(lchild->m_max1, rchild->m_max2);
                    }
                if constexpr (ChangeMax)
                    if (lchild->m_min1 == rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min2);
                        this->m_min_cnt = lchild->m_min_cnt + rchild->m_min_cnt;
                    } else if (lchild->m_min1 < rchild->m_min1) {
                        this->m_min1 = lchild->m_min1;
                        this->m_min_cnt = lchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min2, rchild->m_min1);
                    } else {
                        this->m_min1 = rchild->m_min1;
                        this->m_min_cnt = rchild->m_min_cnt;
                        this->m_min2 = std::min(lchild->m_min1, rchild->m_min2);
                    }
            }
            void pushdown(node_type *lchild, node_type *rchild, uint32_t len) {
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
}

int main() {
    uint32_t N, Q;
    cin >> N >> Q;
    using Tree = OY::SegBeat::Tree<OY::SegBeat::ChminChmaxNode<int64_t, int32_t, int64_t, 1, 1, 1>, 1 << 22>;
    Tree seg(N, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    using node_type = decltype(seg)::node;
    while (Q--) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            seg.add(l, r - 1, node_type::Chmin{b});
        } else if (op == '1') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            seg.add(l, r - 1, node_type::Chmax{b});
        } else if (op == '2') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            seg.add(l, r - 1, node_type::Add{b});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << seg.query<node_type::SumGetter>(l, r - 1) << '\n';
        }
    }
}