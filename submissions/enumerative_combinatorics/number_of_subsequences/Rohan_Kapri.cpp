
#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <limits>
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
#ifndef __OY_STATICMODINT32__
#define __OY_STATICMODINT32__
#if __cpp_constexpr >= 201304L
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif
namespace OY {
    template <uint32_t P, bool IsPrime, typename = typename std::enable_if<(P > 1 && P < uint32_t(1) << 31)>::type>
    struct StaticModInt32 {
        using mint = StaticModInt32<P, IsPrime>;
        using mod_type = uint32_t;
        mod_type m_val;
        static constexpr mod_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static constexpr mod_type _mul(mod_type a, mod_type b) { return uint64_t(a) * b % mod(); }
        constexpr StaticModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(_reduce_norm(val % int32_t(mod()))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(val % mod()) {}
        static CONSTEXPR14 mint raw(mod_type val) {
            mint res{};
            res.m_val = val;
            return res;
        }
        static constexpr mod_type mod() { return P; }
        constexpr mod_type val() const { return m_val; }
        CONSTEXPR14 mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return raw(res);
        }
        CONSTEXPR14 mint inv() const {
            if constexpr (IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        CONSTEXPR14 mint inv_exgcd() const {
            mod_type x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        CONSTEXPR14 mint &operator++() {
            if (++m_val == mod()) m_val = 0;
            return *this;
        }
        CONSTEXPR14 mint &operator--() {
            if (!m_val) m_val = mod();
            m_val--;
            return *this;
        }
        CONSTEXPR14 mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        CONSTEXPR14 mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        CONSTEXPR14 mint &operator+=(const mint &rhs) {
            m_val += rhs.m_val;
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        CONSTEXPR14 mint &operator-=(const mint &rhs) {
            m_val += mod() - rhs.m_val;
            if (m_val >= mod()) m_val -= mod();
            return *this;
        }
        CONSTEXPR14 mint &operator*=(const mint &rhs) {
            m_val = _mul(m_val, rhs.m_val);
            return *this;
        }
        CONSTEXPR14 mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        constexpr bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        constexpr bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        constexpr bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        constexpr bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        constexpr bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        constexpr bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
        template <typename Tp>
        constexpr explicit operator Tp() const { return Tp(m_val); }
        friend CONSTEXPR14 mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend CONSTEXPR14 mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend CONSTEXPR14 mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend CONSTEXPR14 mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    };
    template <typename Istream, uint32_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticModInt32<P, IsPrime> &x) { return is >> x.m_val; }
    template <typename Ostream, uint32_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticModInt32<P, IsPrime> &x) { return os << x.val(); }
    using mint998244353 = StaticModInt32<998244353, true>;
    using mint1000000007 = StaticModInt32<1000000007, true>;
}
#endif
#ifndef __OY_GLOBALHASHMAP__
#define __OY_GLOBALHASHMAP__
namespace OY {
    namespace GHASH {
        using size_type = uint32_t;
        template <size_type N>
        struct ModLevel : std::integral_constant<size_type, N % 2 == 0 && ~ModLevel<N / 2>::value ? ModLevel<N / 2>::value + 1 : -1> {};
        template <>
        struct ModLevel<1> : std::integral_constant<size_type, 0> {};
        template <size_type N, bool = ModLevel<N>::value != -1>
        struct Moder {
            static constexpr size_type L = 64 - ModLevel<N>::value;
            size_type operator()(uint64_t a) const { return a >> L; }
        };
        template <size_type N>
        struct Moder<N, false> {
            size_type operator()(uint64_t a) const { return a % N; }
        };
        struct HashCombine {
            uint64_t operator()(uint64_t a, uint64_t b) const { return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2)); }
            template <typename... Args>
            uint64_t operator()(uint64_t a, uint64_t b, Args... args) const { return operator()(operator()(a, b), args...); }
        };
        template <typename Tp>
        struct Hash {
            static uint64_t s_bias;
            uint64_t operator()(const Tp &x) const { return (x + s_bias) * 11995408973635179863ULL; }
        };
        template <typename Tp>
        uint64_t Hash<Tp>::s_bias = std::chrono::steady_clock::now().time_since_epoch().count();
        template <>
        struct Hash<std::string> {
            uint64_t operator()(const std::string &x) const { return std::hash<std::string>()(x); }
        };
        template <typename KeyType, typename MappedType>
        struct Node {
            KeyType m_key;
            MappedType m_mapped;
        };
        template <typename KeyType>
        struct Node<KeyType, void> {
            KeyType m_key;
        };
        template <bool Record>
        struct Recorder {};
        template <>
        struct Recorder<true> : std::vector<size_type> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type BUFFER>
        struct TableBase {
            using node = Node<KeyType, MappedType>;
            struct pair {
                node *m_ptr;
                bool m_flag;
            };
            node m_pool[BUFFER];
            std::bitset<BUFFER> m_occupied;
            Recorder<MakeRecord> m_recs;
            size_type m_size;
            void reserve(size_type count) {
                if constexpr (MakeRecord) m_recs.reserve(count);
            }
            size_type size() const { return m_size; }
            bool empty() const { return !size(); }
            void clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs) m_occupied[i] = false;
                m_recs.clear(), m_size = 0;
            }
            template <typename Callback>
            void do_for_each(Callback &&call) {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs)
                    if (m_occupied[i]) call(m_pool + i);
            }
            pair insert(const KeyType &key) {
                size_type ha = Moder<BUFFER>()(Hash<KeyType>()(key)), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return {m_pool + i, false};
                    i = i != BUFFER - 1 ? i + 1 : 0;
                }
                m_pool[i].m_key = key, m_occupied[i] = true, m_size++;
                if constexpr (MakeRecord) m_recs.push_back(i);
                return {m_pool + i, true};
            }
            bool erase(const KeyType &key) {
                size_type ha = Moder<BUFFER>()(Hash<KeyType>()(key)), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) {
                        m_occupied[i] = false, m_size--;
                        return true;
                    }
                    i = i != BUFFER - 1 ? i + 1 : 0;
                }
                return false;
            }
            node *find(const KeyType &key) const {
                size_type ha = Moder<BUFFER>()(Hash<KeyType>()(key)), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return (node *)(m_pool + i);
                    i = i != BUFFER - 1 ? i + 1 : 0;
                }
                return nullptr;
            }
        };
        template <typename KeyType, bool MakeRecord, size_type BUFFER>
        struct UnorderedSet : TableBase<KeyType, void, MakeRecord, BUFFER> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type BUFFER>
        struct UnorderedMap : TableBase<KeyType, MappedType, MakeRecord, BUFFER> {
            using typename TableBase<KeyType, MappedType, MakeRecord, BUFFER>::pair;
            pair insert_or_assign(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                res.m_ptr->m_mapped = mapped;
                return res;
            }
            pair insert_or_ignore(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                if (res.m_flag) res.m_ptr->m_mapped = mapped;
                return res;
            }
            MappedType &operator[](const KeyType &key) {
                pair res = this->insert(key);
                if (res.m_flag) res.m_ptr->m_mapped = MappedType{};
                return res.m_ptr->m_mapped;
            }
            MappedType get(const KeyType &key, const MappedType &_default) const {
                auto res = this->find(key);
                return res ? res->m_mapped : _default;
            }
            const MappedType &get(const KeyType &key) const { return this->find(key)->m_mapped; }
        };
    }
}
#endif
/*
lib code is above
temp code is below
*/
using mint = OY::mint998244353;
OY::GHASH::UnorderedMap<uint32_t, mint, false, 800003> GS;
int main() {
    uint32_t n;
    cin >> n;
    mint ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        auto [pre, flag] = GS.insert(x);
        mint cur = flag ? ans + 1 : ans - pre->m_mapped;
        pre->m_mapped = ans;
        ans += cur;
    }
    cout << ans;
}
