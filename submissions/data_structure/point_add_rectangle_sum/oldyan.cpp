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
#ifndef __OY_WAVELET__
#define __OY_WAVELET__
namespace OY {
    namespace WaveLet {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        struct Ignore {};
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        struct BitRank {
            std::vector<mask_type> m_bits;
            std::vector<size_type> m_sum;
            void resize(size_type length) { m_bits.assign((length >> MASK_WIDTH) + 1, 0), m_sum.resize(m_bits.size()); }
            void set(size_type i, mask_type val) { m_bits[i >> MASK_WIDTH] |= val << (i & (MASK_SIZE - 1)); }
            void prepare() {
                for (size_type i = 1; i != m_bits.size(); i++) m_sum[i] = m_sum[i - 1] + std::popcount(m_bits[i - 1]);
            }
            size_type rank1(size_type i) const { return m_sum[i >> MASK_WIDTH] + std::popcount(m_bits[i >> MASK_WIDTH] & ((mask_type(1) << (i & (MASK_SIZE - 1))) - 1)); }
            size_type rank1(size_type l, size_type r) const { return rank1(r) - rank1(l); }
            size_type rank0(size_type i) const { return i - rank1(i); }
            size_type rank0(size_type l, size_type r) const { return rank0(r) - rank0(l); }
        };
        struct VoidTable {
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {}
            template <typename InitMapping>
            void resize(size_type length, InitMapping) {}
            size_type query(size_type left, size_type right) const { return right - left + 1; }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Table {
            size_type m_size, m_alpha;
            std::vector<BitRank> m_ranks;
            std::vector<size_type> m_pos;
            std::vector<SumTable> m_sumer;
            Table() = default;
            template <typename InitMapping, typename TableMapping = Ignore>
            Table(size_type length, InitMapping mapping, size_type alpha = 0, TableMapping table_mapping = TableMapping()) { resize(length, mapping, alpha, table_mapping); }
            template <typename Iterator, typename TableMapping = Ignore>
            Table(Iterator first, Iterator last, size_type alpha = 0, TableMapping table_mapping = TableMapping()) { reset(first, last, alpha, table_mapping); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping mapping, size_type alpha = 0, TableMapping table_mapping = TableMapping()) {
                static_assert(std::is_unsigned<Tp>::value, "Tp Must Be Unsigned Type");
                if (!(m_size = length)) return;
                std::vector<Tp> numbers(m_size);
                for (size_type i = 0; i != m_size; i++) numbers[i] = mapping(i);
                m_alpha = alpha ? alpha : std::max<uint32_t>(1, std::bit_width(*std::max_element(numbers.begin(), numbers.end())));
                m_ranks.resize(m_alpha), m_pos.resize(m_alpha);
                m_sumer.resize(m_alpha);
                for (size_type d = m_alpha - 1; ~d; d--) {
                    m_ranks[d].resize(m_size);
                    for (size_type i = 0; i != m_size; i++) m_ranks[d].set(i, numbers[i] >> d & 1);
                    m_ranks[d].prepare();
                    m_pos[d] = std::stable_partition(numbers.begin(), numbers.end(), [&](size_type val) { return !(val >> d & 1); }) - numbers.begin();
                    if constexpr (std::is_same<TableMapping, Ignore>::value)
                        m_sumer[d].reset(numbers.begin(), numbers.end());
                    else
                        m_sumer[d].resize(m_size, [&](size_type i) { return table_mapping(numbers[i]); });
                }
            }
            template <typename Iterator, typename TableMapping = Ignore>
            void reset(Iterator first, Iterator last, size_type alpha = 0, TableMapping table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, alpha, table_mapping);
            }
            size_type count(size_type left, size_type right, Tp val) const {
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (!(val >> d & 1))
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr;
                }
                return right - left;
            }
            size_type count(size_type left, size_type right, Tp minimum, Tp maximum) const {
                size_type l1 = left, r1 = right + 1, l2 = left, r2 = right + 1, res = 0;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl1 = m_ranks[d].rank0(l1), zr1 = m_ranks[d].rank0(r1), zl2 = m_ranks[d].rank0(l2), zr2 = m_ranks[d].rank0(r2);
                    if (!(minimum >> d & 1))
                        l1 = zl1, r1 = zr1;
                    else
                        res -= zr1 - zl1, l1 += m_pos[d] - zl1, r1 += m_pos[d] - zr1;
                    if (!(maximum >> d & 1))
                        l2 = zl2, r2 = zr2;
                    else
                        res += zr2 - zl2, l2 += m_pos[d] - zl2, r2 += m_pos[d] - zr2;
                }
                return r2 - l2 + res;
            }
            size_type rank(size_type left, size_type right, Tp val) const {
                size_type ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (!(val >> d & 1))
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans += zr - zl;
                }
                return ans;
            }
            Tp minimum(size_type left, size_type right) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (zl != zr)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp maximum(size_type left, size_type right) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right);
                    if (zr - zl == right - left)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp quantile(size_type left, size_type right, size_type k) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (k < z)
                        left = zl, right = zr;
                    else
                        left += m_pos[d] - zl, right += m_pos[d] - zr, k -= z, ans |= Tp(1) << d;
                }
                return ans;
            }
            Tp max_bitxor(size_type left, size_type right, Tp val) const {
                Tp ans = 0;
                right++;
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (val >> d & 1)
                        if (z)
                            left = zl, right = zr, ans |= Tp(1) << d;
                        else
                            left += m_pos[d] - zl, right += m_pos[d] - zr;
                    else if (right - left - z)
                        left += m_pos[d] - zl, right += m_pos[d] - zr, ans |= Tp(1) << d;
                    else
                        left = zl, right = zr;
                }
                return ans;
            }
            template <typename Callback>
            void do_for_rank_range(size_type left, size_type right, size_type rk1, size_type rk2, Callback &&call) const {
                right++;
                auto handle = [&](size_type l1, size_type r1, size_type l2, size_type r2, size_type k1, size_type k2, size_type d) {
                    for (; ~d; d--) {
                        size_type one_l = m_ranks[d].rank1(l1), one_r = m_ranks[d].rank1(r1), one = one_r - one_l, zl = m_ranks[d].rank0(l2), zr = m_ranks[d].rank0(r2), z = zr - zl;
                        if (k1 < one)
                            l1 = m_pos[d] + one_l, r1 = m_pos[d] + one_r;
                        else {
                            l1 -= one_l, r1 -= one_r, k1 -= one;
                            if (one) call(m_sumer[d].query(m_pos[d] + one_l, m_pos[d] + one_r - 1));
                        }
                        if (k2 < z)
                            l2 = zl, r2 = zr;
                        else {
                            l2 += m_pos[d] - zl, r2 += m_pos[d] - zr, k2 -= z;
                            if (z) call(m_sumer[d].query(zl, zr - 1));
                        }
                    }
                    if (k1) call(m_sumer[0].query(l1, l1 + k1 - 1));
                    if (k2) call(m_sumer[0].query(r2 - k2, r2 - 1));
                };
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if (rk2 < z)
                        left = zl, right = zr;
                    else if (rk1 >= z)
                        left += m_pos[d] - zl, right += m_pos[d] - zr, rk1 -= z, rk2 -= z;
                    else
                        return handle(zl, zr, left + m_pos[d] - zl, right + m_pos[d] - zr, z - rk1, rk2 - z + 1, d - 1);
                }
                if (left != right) call(m_sumer[0].query(left + rk1, left + rk2));
            }
            template <typename Callback>
            void do_for_value_range(size_type left, size_type right, Tp floor, Tp ceil, Callback &&call) const {
                right++;
                auto handle = [&](size_type l1, size_type r1, size_type l2, size_type r2, size_type d) {
                    for (; ~d; d--) {
                        size_type one_l = m_ranks[d].rank1(l1), one_r = m_ranks[d].rank1(r1), one = one_r - one_l, zl = m_ranks[d].rank0(l2), zr = m_ranks[d].rank0(r2), z = zr - zl;
                        if (floor >> d & 1)
                            l1 = m_pos[d] + one_l, r1 = m_pos[d] + one_r;
                        else {
                            l1 -= one_l, r1 -= one_r;
                            if (one) call(m_sumer[d].query(m_pos[d] + one_l, m_pos[d] + one_r - 1));
                        }
                        if (!(ceil >> d & 1))
                            l2 = zl, r2 = zr;
                        else {
                            l2 += m_pos[d] - zl, r2 += m_pos[d] - zr;
                            if (z) call(m_sumer[d].query(zl, zr - 1));
                        }
                    }
                    if (l1 != r1) call(m_sumer[0].query(l1, r1 - 1));
                    if (l2 != r2) call(m_sumer[0].query(l2, r2 - 1));
                };
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(left), zr = m_ranks[d].rank0(right), z = zr - zl;
                    if ((floor >> d & 1) == (ceil >> d & 1))
                        if (!(floor >> d & 1))
                            left = zl, right = zr;
                        else
                            left += m_pos[d] - zl, right += m_pos[d] - zr;
                    else
                        return handle(zl, zr, left + m_pos[d] - zl, right + m_pos[d] - zr, d - 1);
                }
                if (left != right) call(m_sumer[0].query(left, right - 1));
            }
            template <typename Callback>
            void do_in_table(size_type pos, Callback &&call) {
                for (size_type d = m_alpha - 1; ~d; d--) {
                    size_type zl = m_ranks[d].rank0(pos), zr = m_ranks[d].rank0(pos + 1);
                    call(m_sumer[d], pos = zl == zr ? pos + m_pos[d] - zl : zl);
                }
            }
        };
        template <typename Tp, typename SumTable = VoidTable>
        struct Tree {
            Table<size_type, SumTable> m_table;
            std::vector<Tp> m_discretizer;
            size_type m_size;
            size_type _find(const Tp &val) const { return std::lower_bound(m_discretizer.begin(), m_discretizer.end(), val) - m_discretizer.begin(); }
            size_type _upper_find(const Tp &val) const { return std::upper_bound(m_discretizer.begin(), m_discretizer.end(), val) - m_discretizer.begin(); }
            Tree() = default;
            template <typename InitMapping, typename TableMapping = Ignore>
            Tree(size_type length, InitMapping mapping, TableMapping table_mapping = TableMapping()) { resize(length, mapping, table_mapping); }
            template <typename Iterator, typename TableMapping = Ignore>
            Tree(Iterator first, Iterator last, TableMapping table_mapping = TableMapping()) { reset(first, last, table_mapping); }
            template <typename InitMapping, typename TableMapping = Ignore>
            void resize(size_type length, InitMapping mapping, TableMapping table_mapping = TableMapping()) {
                if (!(m_size = length)) return;
                std::vector<Tp> items(m_size);
                for (size_type i = 0; i != m_size; i++) items[i] = mapping(i);
                m_discretizer = items;
                std::sort(m_discretizer.begin(), m_discretizer.end());
                m_discretizer.erase(std::unique(m_discretizer.begin(), m_discretizer.end()), m_discretizer.end());
                if constexpr (std::is_same<TableMapping, Ignore>::value)
                    m_table.resize(
                        m_size, [&](size_type i) { return _find(items[i]); }, std::bit_width(m_discretizer.size()), [&](size_type val) { return m_discretizer[val]; });
                else
                    m_table.resize(
                        m_size, [&](size_type i) { return _find(items[i]); }, std::bit_width(m_discretizer.size()), [&](size_type val) { return table_mapping(m_discretizer[val]); });
            }
            template <typename Iterator, typename TableMapping>
            void reset(Iterator first, Iterator last, TableMapping table_mapping = TableMapping()) {
                resize(
                    last - first, [&](size_type i) { return *(first + i); }, table_mapping);
            }
            size_type count(size_type left, size_type right, const Tp &val) const {
                size_type find = _find(val);
                return find < m_discretizer.size() && m_discretizer[find] == val ? m_table.count(left, right, find) : 0;
            }
            size_type count(size_type left, size_type right, const Tp &minimum, const Tp &maximum) const {
                size_type find1 = _find(minimum);
                if (find1 == m_discretizer.size()) return 0;
                size_type find2 = _find(maximum);
                return find2 < m_discretizer.size() && m_discretizer[find2] == maximum ? m_table.count(left, right, find1, find2) : m_table.count(left, right, find1, find2 - 1);
            }
            size_type rank(size_type left, size_type right, const Tp &val) const { return m_table.rank(left, right, _find(val)); }
            Tp minimum(size_type left, size_type right) const { return m_discretizer[m_table.minimum(left, right)]; }
            Tp maximum(size_type left, size_type right) const { return m_discretizer[m_table.maximum(left, right)]; }
            Tp quantile(size_type left, size_type right, size_type k) const { return m_discretizer[m_table.quantile(left, right, k)]; }
            template <typename Callback>
            void do_for_rank_range(size_type left, size_type right, size_type rk1, size_type rk2, Callback &&call) const { m_table.do_for_rank_range(left, right, rk1, rk2, call); }
            template <typename Callback>
            void do_for_value_range(size_type left, size_type right, const Tp &floor, const Tp &ceil, Callback &&call) const { m_table.do_for_value_range(left, right, _find(floor), _upper_find(ceil) - 1, call); }
        };
    }
}
#endif
#ifndef __OY_POINTADDRECTSUMCOUNTER2D__
#define __OY_POINTADDRECTSUMCOUNTER2D__
namespace OY {
    namespace PARSC2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType, bool HasModify>
        struct Point {
            SizeType m_x, m_y;
            WeightType m_w;
        };
        template <typename SizeType, typename WeightType>
        struct Point<SizeType, WeightType, true> {
            SizeType m_x, m_y;
            size_type m_index;
            WeightType m_w;
        };
        template <typename SizeType>
        struct Point<SizeType, bool, false> {
            SizeType m_x, m_y;
        };
        template <typename SizeType>
        struct Point<SizeType, bool, true> {
            SizeType m_x, m_y;
            size_type m_index;
        };
        template <typename Tp>
        struct AdjTable {
            std::vector<Tp> m_sum;
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length + 1);
                for (size_type i = 0; i != length; i++) m_sum[i + 1] = m_sum[i] + mapping(i);
            }
            Tp query(size_type left, size_type right) const { return m_sum[right + 1] - m_sum[left]; }
        };
        template <typename Tp>
        struct SimpleBIT {
            std::vector<Tp> m_sum;
            static size_type _lowbit(size_type x) { return x & -x; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_sum.resize(length);
                for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                for (size_type i = 0; i != length; i++) {
                    size_type j = i + _lowbit(i + 1);
                    if (j < length) m_sum[j] += m_sum[i];
                }
            }
            void add(size_type i, size_type inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            Tp presum(size_type i) const {
                Tp res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left - 1); }
        };
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type, bool HasModify = false>
        struct Table {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using point = Point<SizeType, WeightType, HasModify>;
            using inner_table = typename std::conditional<HasModify, SimpleBIT<SumType>, AdjTable<SumType>>::type;
            using wavelet = WaveLet::Table<size_type, inner_table>;
            std::vector<point> m_points;
            std::vector<SizeType> m_sorted_xs, m_ys;
            std::vector<size_type> m_point_pos;
            wavelet m_table;
            Table() = default;
            Table(size_type point_cnt) { m_points.reserve(point_cnt); }
            void add_point(SizeType x, SizeType y, WeightType w = 1) {
                if constexpr (is_bool)
                    if constexpr (HasModify)
                        m_points.push_back({x, y, size_type(m_points.size())});
                    else
                        m_points.push_back({x, y});
                else if constexpr (HasModify)
                    m_points.push_back({x, y, size_type(m_points.size()), w});
                else
                    m_points.push_back({x, y, w});
            }
            void prepare() {
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_y < y.m_y; });
                m_ys.reserve(m_points.size());
                std::vector<WeightType> wtable;
                if constexpr (!is_bool) wtable.reserve(m_points.size());
                for (auto &p : m_points) {
                    m_ys.push_back(p.m_y), p.m_y = m_ys.size() - 1;
                    if constexpr (!is_bool) wtable.push_back(p.m_w);
                }
                std::sort(m_points.begin(), m_points.end(), [](const point &x, const point &y) { return x.m_x < y.m_x; });
                auto mapping = [&](size_type i) { return m_points[i].m_y; };
                auto table_mapping = [&](size_type y) -> SumType {
                    if constexpr (is_bool)
                        return 1;
                    else
                        return wtable[y];
                };
                m_table.resize(m_points.size(), mapping, std::bit_width(m_points.size()), table_mapping);
                m_sorted_xs.reserve(m_points.size());
                for (auto &p : m_points) m_sorted_xs.push_back(p.m_x);
                if constexpr (HasModify) {
                    m_point_pos.resize(m_points.size());
                    for (size_type i = 0; i != m_points.size(); i++) m_point_pos[m_points[i].m_index] = i;
                }
            }
            SumType query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) const {
                auto y1 = std::lower_bound(m_ys.begin(), m_ys.end(), y_min) - m_ys.begin(), y2 = std::upper_bound(m_ys.begin(), m_ys.end(), y_max) - m_ys.begin() - 1;
                SumType res{};
                if (y1 != y2 + 1) m_table.do_for_value_range(std::lower_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_min) - m_sorted_xs.begin(), std::upper_bound(m_sorted_xs.begin(), m_sorted_xs.end(), x_max) - m_sorted_xs.begin() - 1, y1, y2, [&](SumType val) { res += val; });
                return res;
            }
            void add_point_value(size_type point_id, WeightType w) {
                static_assert(HasModify, "HasModify Must Be True");
                m_table.do_in_table(m_point_pos[point_id], [w](inner_table &tr, size_type pos) { tr.add(pos, w); });
            }
        };
    };
}
#endif
/*
lib code is above
temp code is below
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::PARSC2D::Table<uint32_t, uint32_t, uint64_t, true> sol(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x, y, w;
        cin >> x >> y >> w;
        sol.add_point(x, y, w);
    }
    std::vector<std::tuple<char, uint32_t, uint32_t, uint32_t, uint32_t>> events;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t x, y, w;
            cin >> x >> y >> w;
            sol.add_point(x, y, 0);
            events.emplace_back(op, x, y, w, 0);
        } else {
            uint32_t l, d, r, u;
            cin >> l >> d >> r >> u;
            events.emplace_back(op, l, d, r, u);
        }
    }
    sol.prepare();
    uint32_t id = n;
    for (auto &[op, l, d, r, u] : events) {
        if (op == '0')
            sol.add_point_value(id++, r);
        else
            cout << sol.query(l, r - 1, d, u - 1) << endl;
    }
}
