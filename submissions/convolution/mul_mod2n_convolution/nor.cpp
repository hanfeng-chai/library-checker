#ifndef LOCAL
    #pragma GCC optimize("O3,unroll-loops")
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")
#endif

#include "bits/stdc++.h"

#ifdef DEBUG
    #include "includes/debug/debug.hpp"
#else
    #define debug(...) 0
#endif

struct IOPre {
    static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN;
    std::array<char, 4 * SZ> num;
    constexpr IOPre() : num{} {
        for (int i = 0; i < SZ; i++) {
            int n = i;
            for (int j = 3; j >= 0; j--) {
                num[i * 4 + j] = static_cast<char>(n % TEN + '0');
                n /= TEN;
            }
        }
    }
};
struct IO {
#if !HAVE_DECL_FREAD_UNLOCKED
    #define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
    #define fwrite_unlocked fwrite
#endif
    static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN,
                         THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN,
                         MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15,
                         TWELVE = 12, SIXTEEN = 16;
    static constexpr IOPre io_pre = {};
    std::array<char, SZ> input_buffer, output_buffer;
    int input_ptr_left, input_ptr_right, output_ptr_right;

    IO()
        : input_buffer{},
          output_buffer{},
          input_ptr_left{},
          input_ptr_right{},
          output_ptr_right{} {}
    IO(const IO&) = delete;
    IO(IO&&) = delete;
    IO& operator=(const IO&) = delete;
    IO& operator=(IO&&) = delete;

    ~IO() { flush(); }

    template <class T>
    struct is_char {
        static constexpr bool value = std::is_same_v<T, char>;
    };

    template <class T>
    struct is_bool {
        static constexpr bool value = std::is_same_v<T, bool>;
    };

    template <class T>
    struct is_string {
        static constexpr bool value =
            std::is_same_v<T, std::string> || std::is_same_v<T, const char*> ||
            std::is_same_v<T, char*> || std::is_same_v<std::decay_t<T>, char*>;
        ;
    };

    template <class T, class D = void>
    struct is_custom {
        static constexpr bool value = false;
    };

    template <class T>
    struct is_custom<T, std::void_t<typename T::internal_value_type>> {
        static constexpr bool value = true;
    };

    template <class T>
    struct is_default {
        static constexpr bool value = is_char<T>::value || is_bool<T>::value ||
                                      is_string<T>::value ||
                                      std::is_integral_v<T>;
    };

    template <class T, class D = void>
    struct is_iterable {
        static constexpr bool value = false;
    };

    template <class T>
    struct is_iterable<
        T, typename std::void_t<decltype(std::begin(std::declval<T>()))>> {
        static constexpr bool value = true;
    };

    template <class T, class D = void, class E = void>
    struct is_applyable {
        static constexpr bool value = false;
    };

    template <class T>
    struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>,
                        std::void_t<decltype(std::get<0>(std::declval<T>()))>> {
        static constexpr bool value = true;
    };

    template <class T>
    static constexpr bool needs_newline = (is_iterable<T>::value ||
                                           is_applyable<T>::value) &&
                                          (!is_default<T>::value);

    template <typename T, typename U>
    struct any_needs_newline {
        static constexpr bool value = false;
    };
    template <typename T>
    struct any_needs_newline<T, std::index_sequence<>> {
        static constexpr bool value = false;
    };
    template <typename T, std::size_t I, std::size_t... Is>
    struct any_needs_newline<T, std::index_sequence<I, Is...>> {
        static constexpr bool value =
            needs_newline<decltype(std::get<I>(std::declval<T>()))> ||
            any_needs_newline<T, std::index_sequence<Is...>>::value;
    };

    inline void load() {
        memmove(std::begin(input_buffer),
                std::begin(input_buffer) + input_ptr_left,
                input_ptr_right - input_ptr_left);
        input_ptr_right =
            input_ptr_right - input_ptr_left +
            static_cast<int>(fread_unlocked(
                std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1,
                SZ - input_ptr_right + input_ptr_left, stdin));
        input_ptr_left = 0;
    }

    inline void read_char(char& c) {
        if (input_ptr_left + LEN > input_ptr_right) load();
        c = input_buffer[input_ptr_left++];
    }
    inline void read_string(std::string& x) {
        char c;
        while (read_char(c), c < '!') continue;
        x = c;
        while (read_char(c), c >= '!') x += c;
    }
    template <class T>
    inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T& x) {
        if (input_ptr_left + LEN > input_ptr_right) load();
        char c = 0;
        do c = input_buffer[input_ptr_left++];
        while (c < '-');
        [[maybe_unused]] bool minus = false;
        if constexpr (std::is_signed<T>::value == true)
            if (c == '-') minus = true, c = input_buffer[input_ptr_left++];
        x = 0;
        while (c >= '0')
            x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++];
        if constexpr (std::is_signed<T>::value == true)
            if (minus) x = -x;
    }

    inline void skip_space() {
        if (input_ptr_left + LEN > input_ptr_right) load();
        while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++;
    }

    inline void flush() {
        fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout);
        output_ptr_right = 0;
    }

    inline void write_char(char c) {
        if (output_ptr_right > SZ - LEN) flush();
        output_buffer[output_ptr_right++] = c;
    }

    inline void write_bool(bool b) {
        if (output_ptr_right > SZ - LEN) flush();
        output_buffer[output_ptr_right++] = b ? '1' : '0';
    }

    inline void write_string(const std::string& s) {
        for (auto x : s) write_char(x);
    }

    inline void write_string(const char* s) {
        while (*s) write_char(*s++);
    }

    inline void write_string(char* s) {
        while (*s) write_char(*s++);
    }

    template <typename T>
    inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) {
        if (output_ptr_right > SZ - LEN) flush();
        if (!x) {
            output_buffer[output_ptr_right++] = '0';
            return;
        }
        if constexpr (std::is_signed<T>::value == true)
            if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x;
        int i = TWELVE;
        std::array<char, SIXTEEN> buf{};
        while (x >= TENTHOUSAND) {
            memcpy(std::begin(buf) + i,
                   std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4);
            x /= TENTHOUSAND;
            i -= 4;
        }
        if (x < HUNDRED) {
            if (x < TEN) {
                output_buffer[output_ptr_right++] = static_cast<char>('0' + x);
            } else {
                std::uint32_t q =
                    (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >>
                    MAGIC_SHIFT;
                std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN;
                output_buffer[output_ptr_right] = static_cast<char>('0' + q);
                output_buffer[output_ptr_right + 1] =
                    static_cast<char>('0' + r);
                output_ptr_right += 2;
            }
        } else {
            if (x < THOUSAND) {
                memcpy(std::begin(output_buffer) + output_ptr_right,
                       std::begin(io_pre.num) + (x << 2) + 1, 3),
                    output_ptr_right += 3;
            } else {
                memcpy(std::begin(output_buffer) + output_ptr_right,
                       std::begin(io_pre.num) + (x << 2), 4),
                    output_ptr_right += 4;
            }
        }
        memcpy(std::begin(output_buffer) + output_ptr_right,
               std::begin(buf) + i + 4, TWELVE - i);
        output_ptr_right += TWELVE - i;
    }
    template <typename T_>
    IO& operator<<(T_&& x) {
        using T = typename std::remove_cv<
            typename std::remove_reference<T_>::type>::type;
        static_assert(is_custom<T>::value or is_default<T>::value or
                      is_iterable<T>::value or is_applyable<T>::value);
        if constexpr (is_custom<T>::value) {
            write_int(x.get());
        } else if constexpr (is_default<T>::value) {
            if constexpr (is_bool<T>::value) {
                write_bool(x);
            } else if constexpr (is_string<T>::value) {
                write_string(x);
            } else if constexpr (is_char<T>::value) {
                write_char(x);
            } else if constexpr (std::is_integral_v<T>) {
                write_int(x);
            }
        } else if constexpr (is_iterable<T>::value) {
            // strings are immune
            using E = decltype(*std::begin(x));
            constexpr char sep = needs_newline<E> ? '\n' : ' ';
            int i = 0;
            for (const auto& y : x) {
                if (i++) write_char(sep);
                operator<<(y);
            }
        } else if constexpr (is_applyable<T>::value) {
            // strings are immune
            constexpr char sep =
                (any_needs_newline<
                    T, std::make_index_sequence<std::tuple_size_v<T>>>::value)
                    ? '\n'
                    : ' ';
            int i = 0;
            std::apply(
                [this, &sep, &i](auto const&... y) {
                    (((i++ ? write_char(sep) : void()), this->operator<<(y)),
                     ...);
                },
                x);
        }
        return *this;
    }
    template <typename T>
    IO& operator>>(T& x) {
        static_assert(is_custom<T>::value or is_default<T>::value or
                      is_iterable<T>::value or is_applyable<T>::value);
        static_assert(!is_bool<T>::value);
        if constexpr (is_custom<T>::value) {
            typename T::internal_value_type y;
            read_int(y);
            x = y;
        } else if constexpr (is_default<T>::value) {
            if constexpr (is_string<T>::value) {
                read_string(x);
            } else if constexpr (is_char<T>::value) {
                read_char(x);
            } else if constexpr (std::is_integral_v<T>) {
                read_int(x);
            }
        } else if constexpr (is_iterable<T>::value) {
            for (auto& y : x) operator>>(y);
        } else if constexpr (is_applyable<T>::value) {
            std::apply([this](auto&... y) { ((this->operator>>(y)), ...); }, x);
        }
        return *this;
    }

    IO* tie(std::nullptr_t) { return this; }
    void sync_with_stdio(bool) {}
};
IO io;
#define cin io
#define cout io

namespace algebra {

    // source: modified from https://github.com/ei1333/library

    template <std::uint32_t P>
    struct ModInt32 {
       public:
        using i32 = std::int32_t;
        using u32 = std::uint32_t;
        using i64 = std::int64_t;
        using u64 = std::uint64_t;
        using m32 = ModInt32;
        using internal_value_type = u32;

       private:
        u32 v;
        static constexpr u32 get_r() {
            u32 iv = P;
            for (u32 i = 0; i != 4; ++i) iv *= 2U - P * iv;
            return -iv;
        }
        static constexpr u32 r = get_r(), r2 = -u64(P) % P;
        static_assert((P & 1) == 1);
        static_assert(-r * P == 1);
        static_assert(P < (1 << 30));
        static constexpr u32 pow_mod(u32 x, u64 y) {
            u32 res = 1;
            for (; y != 0; y >>= 1, x = u64(x) * x % P)
                if (y & 1) res = u64(res) * x % P;
            return res;
        }
        static constexpr u32 reduce(u64 x) {
            return (x + u64(u32(x) * r) * P) >> 32;
        }
        static constexpr u32 norm(u32 x) { return x - (P & -(x >= P)); }

       public:
        static constexpr u32 get_pr() {
            u32 tmp[32] = {}, cnt = 0;
            const u64 phi = P - 1;
            u64 m = phi;
            for (u64 i = 2; i * i <= m; ++i)
                if (m % i == 0) {
                    tmp[cnt++] = u32(i);
                    while (m % i == 0) m /= i;
                }
            if (m != 1) tmp[cnt++] = u32(m);
            for (u64 res = 2; res != P; ++res) {
                bool flag = true;
                for (u32 i = 0; i != cnt && flag; ++i)
                    flag &= pow_mod(res, phi / tmp[i]) != 1;
                if (flag) return u32(res);
            }
            return 0;
        }
        constexpr ModInt32() : v(0){};
        ~ModInt32() = default;
        constexpr ModInt32(u32 _v) : v(reduce(u64(_v) * r2)) {}
        constexpr ModInt32(i32 _v)
            : v(reduce(u64(_v % i64(P) + i64(P)) * r2)) {}
        constexpr ModInt32(u64 _v) : v(reduce((_v % P) * r2)) {}
        constexpr ModInt32(i64 _v)
            : v(reduce(u64(_v % i64(P) + i64(P)) * r2)) {}
        constexpr ModInt32(const m32& rhs) : v(rhs.v) {}
        constexpr u32 get() const { return norm(reduce(v)); }
        explicit constexpr operator u32() const { return get(); }
        explicit constexpr operator i32() const { return i32(get()); }
        constexpr m32& operator=(const m32& rhs) { return v = rhs.v, *this; }
        constexpr m32 operator-() const {
            m32 res;
            return res.v = (P << 1 & -(v != 0)) - v, res;
        }
        constexpr m32 inv() const { return pow(P - 2); }
        constexpr m32& operator+=(const m32& rhs) {
            return v += rhs.v - (P << 1), v += P << 1 & -(v >> 31), *this;
        }
        constexpr m32& operator-=(const m32& rhs) {
            return v -= rhs.v, v += P << 1 & -(v >> 31), *this;
        }
        constexpr m32& operator*=(const m32& rhs) {
            return v = reduce(u64(v) * rhs.v), *this;
        }
        constexpr m32& operator/=(const m32& rhs) {
            return this->operator*=(rhs.inv());
        }
        constexpr friend m32 operator+(const m32& lhs, const m32& rhs) {
            return m32(lhs) += rhs;
        }
        constexpr friend m32 operator-(const m32& lhs, const m32& rhs) {
            return m32(lhs) -= rhs;
        }
        constexpr friend m32 operator*(const m32& lhs, const m32& rhs) {
            return m32(lhs) *= rhs;
        }
        constexpr friend m32 operator/(const m32& lhs, const m32& rhs) {
            return m32(lhs) /= rhs;
        }
        constexpr friend bool operator==(const m32& lhs, const m32& rhs) {
            return norm(lhs.v) == norm(rhs.v);
        }
        constexpr friend bool operator!=(const m32& lhs, const m32& rhs) {
            return norm(lhs.v) != norm(rhs.v);
        }
        friend std::istream& operator>>(std::istream& is, m32& rhs) {
            return is >> rhs.v, rhs.v = reduce(u64(rhs.v) * r2), is;
        }
        friend std::ostream& operator<<(std::ostream& os, const m32& rhs) {
            return os << rhs.get();
        }
        constexpr m32 pow(i64 y) const {
            // assumes P is a prime
            i64 rem = y % (P - 1);
            if (y > 0 && rem == 0)
                y = P - 1;
            else
                y = rem;
            m32 res(1), x(*this);
            for (; y != 0; y >>= 1, x *= x)
                if (y & 1) res *= x;
            return res;
        }
    };

    template <std::uint32_t P>
    ModInt32<P> sqrt(const ModInt32<P>& x) {
        using value_type = ModInt32<P>;
        static constexpr value_type negative_one(P - 1), ZERO(0);
        if (x == ZERO || x.pow((P - 1) >> 1) == negative_one) return ZERO;
        if ((P & 3) == 3) return x.pow((P + 1) >> 2);
        static value_type w2, ax;
        ax = x;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<std::uint32_t> dis(1, P - 1);
        const value_type four(value_type(4) * x);
        static value_type t;
        do t = value_type(dis(gen)), w2 = t * t - four;
        while (w2.pow((P - 1) >> 1) != negative_one);

        struct Field_P2 {  // (A + Bx)(C+Dx)=(AC-BDa)+(AD+BC+BDt)x
           public:
            value_type a, b;
            Field_P2(const value_type& a, const value_type& b) : a(a), b(b) {}
            ~Field_P2() = default;
            Field_P2& operator*=(const Field_P2& rhs) {
                value_type tmp1(b * rhs.b), tmp2(a * rhs.a - tmp1 * ax),
                    tmp3(a * rhs.b + b * rhs.a + tmp1 * t);
                return a = tmp2, b = tmp3, *this;
            }
            Field_P2 pow(std::uint64_t y) const {
                Field_P2 res(value_type(1), ZERO), x(*this);
                for (; y != 0; y >>= 1, x *= x)
                    if (y & 1) res *= x;
                return res;
            }
        } res(ZERO, value_type(1));
        return res.pow((P + 1) >> 1).a;
    }
    std::uint64_t get_len(std::uint64_t n) {  // if n=0, boom
        return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8,
               n |= n >> 16, n |= n >> 32, ++n;
    }
    template <std::uint32_t P>
    struct NTT {
       public:
        using i32 = std::int32_t;
        using u32 = std::uint32_t;
        using i64 = std::int64_t;
        using u64 = std::uint64_t;
        using value_type = ModInt32<P>;

       private:
        static inline value_type ROOT[1 << 20], IROOT[1 << 20];

       public:
        NTT() = delete;
        static void idft(i32 n, value_type x[]) {
            for (i32 i = 2; i < n; i <<= 1) {
                for (i32 j = 0, l = i >> 1; j != l; ++j) {
                    value_type u = x[j], v = x[j + l];
                    x[j] = u + v, x[j + l] = u - v;
                }
                for (i32 j = i, l = i >> 1, m = 1; j != n; j += i, ++m) {
                    value_type root = IROOT[m];
                    for (i32 k = 0; k != l; ++k) {
                        value_type u = x[j + k], v = x[j + k + l];
                        x[j + k] = u + v, x[j + k + l] = (u - v) * root;
                    }
                }
            }
            value_type iv(P - (P - 1) / n);
            for (i32 j = 0, l = n >> 1; j != l; ++j) {
                value_type u = x[j] * iv, v = x[j + l] * iv;
                x[j] = u + v, x[j + l] = u - v;
            }
        }
        static void idft_without_division(i32 n, value_type x[]) {
            for (i32 i = 2; i < n; i <<= 1) {
                for (i32 j = 0, l = i >> 1; j != l; ++j) {
                    value_type u = x[j], v = x[j + l];
                    x[j] = u + v, x[j + l] = u - v;
                }
                for (i32 j = i, l = i >> 1, m = 1; j != n; j += i, ++m) {
                    value_type root = IROOT[m];
                    for (i32 k = 0; k != l; ++k) {
                        value_type u = x[j + k], v = x[j + k + l];
                        x[j + k] = u + v, x[j + k + l] = (u - v) * root;
                    }
                }
            }
            for (i32 j = 0, l = n >> 1; j != l; ++j) {
                value_type u = x[j], v = x[j + l];
                x[j] = u + v, x[j + l] = u - v;
            }
        }
        static void dft(i32 n, value_type x[]) {
            static i32 lim = 0;
            static constexpr u32 pr = value_type::get_pr();
            static_assert(pr != 0);
            static constexpr value_type G(pr);
            if (lim == 0) {
                ROOT[1 << 19] = G.pow((P - 1) >> 21),
                          IROOT[1 << 19] = G.pow(P - 1 - i32((P - 1) >> 21));
                for (i32 i = 18; i != -1; --i)
                    ROOT[1 << i] = ROOT[1 << (i + 1)] * ROOT[1 << (i + 1)],
                              IROOT[1 << i] =
                                  IROOT[1 << (i + 1)] * IROOT[1 << (i + 1)];
                lim = 1;
            }
            while ((lim << 1) < n) {
                for (i32 i = lim + 1, e = lim << 1; i < e; ++i)
                    ROOT[i] = ROOT[i - lim] * ROOT[lim],
                    IROOT[i] = IROOT[i - lim] * IROOT[lim];
                lim <<= 1;
            }
            for (i32 j = 0, l = n >> 1; j != l; ++j) {
                value_type u = x[j], v = x[j + l];
                x[j] = u + v, x[j + l] = u - v;
            }
            for (i32 i = n >> 1; i >= 2; i >>= 1) {
                for (i32 j = 0, l = i >> 1; j != l; ++j) {
                    value_type u = x[j], v = x[j + l];
                    x[j] = u + v, x[j + l] = u - v;
                }
                for (i32 j = i, l = i >> 1, m = 1; j != n; j += i, ++m) {
                    value_type root = ROOT[m];
                    for (i32 k = 0; k != l; ++k) {
                        value_type u = x[j + k], v = x[j + k + l] * root;
                        x[j + k] = u + v, x[j + k + l] = u - v;
                    }
                }
            }
        }
    };
    template <std::uint32_t P>
    void dft(std::uint32_t n, ModInt32<P> x[]) {
        NTT<P>::dft(n, x);
    }
    template <std::uint32_t P>
    void idft(std::uint32_t n, ModInt32<P> x[]) {
        NTT<P>::idft(n, x);
    }
    template <std::uint32_t P>
    void idft_without_division(std::uint32_t n, ModInt32<P> x[]) {
        NTT<P>::idft_without_division(n, x);
    }
}  // namespace algebra

constexpr int mod = 998'244'353;
using mint = algebra::ModInt32<mod>;

using ll = int64_t;
using ld = long double;

using namespace std;

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // cout << setprecision(20) << fixed;
    int _tests = 1;
    // cin >> _tests;
    for (int _test = 1; _test <= _tests; ++_test) {
        // cout << "Case #" << _test << ": ";
        int lg_n;
        cin >> lg_n;
        const int n = 1 << lg_n;
        const int all_mask = n - 1;
        vector<mint> a(n), b(n);
        for (auto& x : a) cin >> x;
        for (auto& x : b) cin >> x;
        vector<mint> ans(n);
        auto brute = [&] {
            vector<mint> ans(n);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    ans[(i * j) & all_mask] += a[i] * b[j];
            return ans;
        };
        debug(brute());
        if (lg_n <= 3) {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    ans[(i * j) & all_mask] += a[i] * b[j];
        } else {
            /*
            claim:  2, -1 and 5 generate the monoid (Z/2^nZ) for n >= 3
            proof:  for n = 3 it is trivial. wlog let n >= 4
                    using lifting the exponent lemma,
                    v2(5^(2^(n-3))-1) = v2(25^(2^(n-4))-1) = v2(24) + n-4 = n-1
                    so 5^(2^(n-3)) is not 1 mod 2^n.
                    also, again by LTE, 5^(2^(n-2)) is 1 mod 2^n
                    suppose there are some i, j such that 5^i is -5^j mod 2^n
                    then 5^(i-j) is -1 mod 2^n, so i and j are congruent mod
                    2^(n-2). so 5^(2^(n-2)) is -1 mod 2^n. but by LTE, this
                    means 1 = v2(-2) = v2(5^2^(n-2) - 1) = n, contradiction
                    so -1 and 5 generate the monoid
               */

            // lg_n >= 4 now
            // n >= 16
            vector p5(2, vector(n / 4, 0));
            // p5[i][j] = (-1)^i * 5^j mod 2^n
            p5[0][0] = 1;
            p5[1][0] = all_mask;
            for (int i = 1; i < n / 4; ++i) {
                p5[0][i] = (p5[0][i - 1] * 5) & all_mask;
                p5[1][i] = n - p5[0][i];
            }
            vector<vector<vector<mint>>> a_t(lg_n + 1), b_t(lg_n + 1),
                ans_t(lg_n + 1);
            // a_t[i][j][k] = a[2^i * (-1)^j * 5^k]
            // b_t[i][j][k] = b[2^i * (-1)^j * 5^k]
            for (int i = 0; i < lg_n - 1; ++i) {
                const int sz = n >> (i + 2);
                a_t[i] = vector(2, vector<mint>(sz));
                b_t[i] = vector(2, vector<mint>(sz));
                ans_t[i] = vector(2, vector<mint>(sz));
                for (auto j : {0, 1}) {
                    for (int k = 0; k < sz; k++) {
                        a_t[i][j][k] = a[(p5[j][k] << i) & all_mask];
                        b_t[i][j][k] = b[(p5[j][k] << i) & all_mask];
                    }
                }
            }

            // DFT on generators -1, 5 for (Z/2^(lg_n - i)Z)* for i < lg_n - 1
            for (int i = 0; i < lg_n - 1; ++i) {
                const int sz = n >> (i + 2);
                auto& a_t_i = a_t[i];
                auto& b_t_i = b_t[i];
                algebra::dft(sz, a_t_i[0].data());
                algebra::dft(sz, a_t_i[1].data());
                algebra::dft(sz, b_t_i[0].data());
                algebra::dft(sz, b_t_i[1].data());
                for (int k = 0; k < sz; ++k) {
                    const auto X = a_t_i[0][k];
                    const auto Y = a_t_i[1][k];
                    a_t_i[0][k] = X + Y;
                    a_t_i[1][k] = X - Y;
                }
                for (int k = 0; k < sz; ++k) {
                    const auto X = b_t_i[0][k];
                    const auto Y = b_t_i[1][k];
                    b_t_i[0][k] = X + Y;
                    b_t_i[1][k] = X - Y;
                }
            }

            // DFT on (Z/2^2Z)* and (Z/2^1Z)*
            a_t[lg_n - 1] = vector{vector{a[n / 2]}, vector{mint(0)}};
            b_t[lg_n - 1] = vector{vector{b[n / 2]}, vector{mint(0)}};
            a_t[lg_n] = vector{vector{a[0]}, vector{mint(0)}};
            b_t[lg_n] = vector{vector{b[0]}, vector{mint(0)}};

            ans_t[lg_n - 1] = vector{vector{mint(0)}, vector{mint(0)}};
            ans_t[lg_n] = vector{vector{mint(0)}, vector{mint(0)}};

            // naive point-wise accumulation for powers of 2
            for (int i = 0; i <= lg_n; ++i) {
                for (int j = 0; j <= lg_n; ++j) {
                    // 2^i, 2^j
                    // i + j > lg_n -> goes to n
                    const int k = min(lg_n, i + j);
                    const int sz = max(1, n >> (k + 2));
                    // max to catch everything divisible by 2^n
                    for (auto l : {0, 1})
                        for (int m = 0; m < sz; ++m)
                            ans_t[k][l][m] += a_t[i][l][m] * b_t[j][l][m];
                }
            }

            // inverse DFT on generators -1, 5 for (Z/2^(lg_n - i)Z)* for i <
            // lg_n - 1
            constexpr mint half = mint(2).inv();
            for (int i = 0; i < lg_n - 1; ++i) {
                auto& ans_t_i = ans_t[i];
                const int sz = n >> (i + 2);
                algebra::idft_without_division(sz, ans_t_i[0].data());
                algebra::idft_without_division(sz, ans_t_i[1].data());
                const mint factor = half.pow(lg_n - 1 - i);
                // const mint factor = 1;
                for (int k = 0; k < sz; ++k) {
                    const auto X = ans_t_i[0][k];
                    const auto Y = ans_t_i[1][k];
                    ans_t_i[0][k] = (X + Y) * factor;
                    ans_t_i[1][k] = (X - Y) * factor;
                }
            }

            // collecting ans
            ans[0] = ans_t[lg_n][0][0];
            ans[n / 2] = ans_t[lg_n - 1][0][0];
            for (int i = 0; i < lg_n - 1; ++i) {
                const int sz = n >> (i + 2);
                for (auto j : {0, 1})
                    for (int k = 0; k < sz; ++k)
                        ans[(p5[j][k] << i) & all_mask] = ans_t[i][j][k];
            }
        }
        for (auto x : ans) cout << x << ' ';
        cout << '\n';
    }
}
