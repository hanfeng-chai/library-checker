#include <bits/stdc++.h>
using namespace std;

static const auto IOSetup = [] {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    // std::cout << std::setprecision(6) << std::fixed;
    return nullptr;}();
struct IOPre {static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN;std::array<char, 4 * SZ> num;constexpr IOPre() : num{} {for (int i = 0; i < SZ; i++) {int n = i;for (int j = 3; j >= 0; j--) {num[i * 4 + j] = static_cast<char>(n % TEN + '0');n /= TEN;}}}};
struct IO {
#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
#define fwrite_unlocked fwrite
#endif
    static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN,THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN,MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15,TWELVE = 12, SIXTEEN = 16;static constexpr IOPre io_pre = {};std::array<char, SZ> input_buffer, output_buffer;int input_ptr_left, input_ptr_right, output_ptr_right;
    IO(): input_buffer{},output_buffer{},input_ptr_left{},input_ptr_right{},output_ptr_right{} {}
    IO(const IO&) = delete;IO(IO&&) = delete;IO& operator=(const IO&) = delete;IO& operator=(IO&&) = delete;
    ~IO() { flush(); }
    template <class T>struct is_char {static constexpr bool value = std::is_same_v<T, char>;};
    template <class T>struct is_bool {static constexpr bool value = std::is_same_v<T, bool>;};
    template <class T>struct is_string {static constexpr bool value =std::is_same_v<T, std::string> || std::is_same_v<T, const char*> ||std::is_same_v<T, char*> || std::is_same_v<std::decay_t<T>, char*>;;};
    template <class T, class D = void>struct is_custom {static constexpr bool value = false;};
    template <class T>struct is_custom<T, std::void_t<typename T::internal_value_type>> {static constexpr bool value = true;};
    template <class T>struct is_default {static constexpr bool value = is_char<T>::value || is_bool<T>::value ||is_string<T>::value ||std::is_integral_v<T>;};
    template <class T, class D = void>struct is_iterable {static constexpr bool value = false;};
    template <class T>struct is_iterable <T, typename std::void_t<decltype(std::begin(std::declval<T>())) >> {static constexpr bool value = true;};
    template <class T, class D = void, class E = void>struct is_applyable {static constexpr bool value = false;};
    template <class T>struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>,std::void_t<decltype(std::get<0>(std::declval<T>()))>> {static constexpr bool value = true;};
    template <class T>static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value);
    template <typename T, typename U> struct any_needs_newline {static constexpr bool value = false;}; template <typename T>
    struct any_needs_newline<T, std::index_sequence<>> {static constexpr bool value = false;};
    template <typename T, std::size_t I, std::size_t... Is>struct any_needs_newline<T, std::index_sequence<I, Is...>> {static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value;};
    inline void load() {memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked(std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0;}
    inline void read_char(char& c) {if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++];}
    inline void read_string(std::string& x) {char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c;}
    template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T& x) {if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true)if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0')x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true)if (minus) x = -x;}
    inline void skip_space() {if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++;}
    inline void flush() {fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0;}
    inline void write_char(char c) {if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c;}
    inline void write_bool(bool b) {if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0';}
    inline void write_string(const std::string& s) {for (auto x : s) write_char(x);}
    inline void write_string(const char* s) {while (*s) write_char(*s++);}
    inline void write_string(char* s) {while (*s) write_char(*s++);}
    template <typename T>inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) {if (output_ptr_right > SZ - LEN) flush(); if (!x) {output_buffer[output_ptr_right++] = '0'; return;} if constexpr (std::is_signed<T>::value == true)if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) {memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4;} if (x < HUNDRED) {if (x < TEN) {output_buffer[output_ptr_right++] = static_cast<char>('0' + x);} else {std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2;}} else {if (x < THOUSAND) {memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3;} else {memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4;}} memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i;}
    template <typename T_>IO& operator<<(T_&& x) {using T = typename std::remove_cv < typename std::remove_reference<T_>::type >::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) {write_int(x.get());} else if constexpr (is_default<T>::value) {if constexpr (is_bool<T>::value) {write_bool(x);} else if constexpr (is_string<T>::value) {write_string(x);} else if constexpr (is_char<T>::value) {write_char(x);} else if constexpr (std::is_integral_v<T>) {write_int(x);}} else if constexpr (is_iterable<T>::value) {using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto& y : x) {if (i++) write_char(sep); operator<<(y);}} else if constexpr (is_applyable<T>::value) {constexpr char sep = (any_needs_newline < T, std::make_index_sequence<std::tuple_size_v<T> >>::value) ? '\n' : ' '; int i = 0; std::apply([this, &sep, &i](auto const & ... y) {(((i++ ? write_char(sep) : void()), this->operator<<(y)), ...);}, x);} return *this;}
    template <typename T>IO& operator>>(T& x) {static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) {typename T::internal_value_type y; read_int(y); x = y;} else if constexpr (is_default<T>::value) {if constexpr (is_string<T>::value) {read_string(x);} else if constexpr (is_char<T>::value) {read_char(x);} else if constexpr (std::is_integral_v<T>) {read_int(x);}} else if constexpr (is_iterable<T>::value) {for (auto& y : x) operator>>(y);} else if constexpr (is_applyable<T>::value) {std::apply([this](auto & ... y) { ((this->operator>>(y)), ...); }, x);} return *this;}
    IO* tie(std::nullptr_t) { return this; }
    void sync_with_stdio(bool) {}
} io;
#define cin io
#define cout io

//This polynomial hasher efficiently uses modulo 2 ^ N. Fails on Thue-Morse strings, be careful.
//Uncomment commented lines to return real hash, rather then multiplied by P ^ n. Requires P to be odd.
template<size_t N, const uint64_t P = 2807516534892679321>
struct hasher_pow2 {
    static_assert(3 <= N && N <= 128);
    using T = conditional_t < N <= 8, uint8_t,
          conditional_t < N <= 16, uint16_t,
          conditional_t<N <= 32, uint32_t,
          conditional_t<N <= 64, uint64_t,
          __uint128_t
          >>>>;
    static constexpr size_t uN = N & (N - 1) ? 2 << __lg(N) : N;
    static constexpr T MASK = ((T) - 1) >> (uN - N);

    size_t n;
    vector<T> pref_hash;
    vector<T> pows;
    // vector<T> ipows;

    template<typename T>
    auto hash_elem(T x) {return hash<T> {}(x);}

    static T binpow(T x, T k) {
        T o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o *= x;
            x *= x;
        }
        return o;
    }

public:
    hasher_pow2() = default;

    template<typename Iterator>
    hasher_pow2(Iterator first, Iterator last): n(last - first), pref_hash(n), pows(n + 1) {
        pows[0] = 1;
        if (!n) return;
        pref_hash[0] = hash_elem(*first); ++first;
        for (size_t i = 1; i < n; ++i, ++first) {
            pows[i] = pows[i - 1] * P;
            pref_hash[i] = pref_hash[i - 1] + hash_elem(*first) * pows[i];
        }
        pows[n] = pows[n - 1] * P;
        // ipows.resize(n + 1);
        // ipows[n] = binpow(pows[n], MASK / 4);
        // for (ssize_t i = n - 1; i >= 0; --i) {
        //     ipows[i] = ipows[i + 1] * P;
        // }
    }

    //Returns hash of string l + r, where hash(l) = hl, hash(r) = hr, len(l) = len_l
    //O(1) if len_l <= n, O(log(len_l)) otherwise
    T merge_hashes(T len_l, T hl, T hr) const {
        T pw = len_l < pows.size() ? pows[len_l] : binpow(P, len_l);
        T hlr = hl + hr * pw;
        return hlr & MASK;
    }

    //O(1)
    T seg_hash(size_t l, size_t r) const {
        if (l > r) return 0;
        assert(r < n);
        T o = pref_hash[r] - (l ? pref_hash[l - 1] : 0);
        return o * pows[n - l] & MASK;
        // return o * ipows[l] & MASK;
    }

    //Returns the hash of string s[l, r] * k = s[l, r] + ... + s[l, r] (k - 1 concatenations)
    //O(log(k))
    T seg_hash_repeated(size_t l, size_t r, T k) const {
        T ans = 0, anspw = 1;
        T hs = seg_hash(l, r), hspw = pows[r - l + 1];
        for (; k; k >>= 1) {
            if (k & 1) {
                ans += hs * anspw;
                anspw *= hspw;
            }
            hs += hs * hspw;
            hspw *= hspw;
        }
        return ans & MASK;
    }

    //O(|borders|)
    T calc_hash_of_substrings_concatenation(vector<pair<int, int>> borders) const {
        T res = 0;
        for (T cpw = 1; auto [l, r] : borders) {
            if (l > r) continue;
            res += seg_hash(l, r) * cpw;
            cpw *= pows[r - l + 1];
        }
        return res & MASK;
    }

    //Returns length of longest common prefix of suffixes s[i, n - 1] and s[j, n - 1]
    //O(log(n))
    size_t lcp(size_t i, size_t j, size_t mx) const {
        if (i > j) swap(i, j);
        if (j >= n) return 0;
        assert(j < n);
        size_t l = 0, r = min(mx + 1, n - j + 1);
        if (seg_hash(i, i + r - 2) == seg_hash(j, j + r - 2)) return r - 1;
        while (l + 1 < r) {
            size_t m = l + (r - l) / 2;
            (seg_hash(i, i + m - 1) == seg_hash(j, j + m - 1) ? l : r) = m;
        }
        return l;
    }

    //Returns length of longest common suffix of prefixes s[0, i] and s[0, j]
    //O(log(n))
    size_t lcs(size_t i, size_t j, size_t mx) const {
        if (i > j) swap(i, j);
        if (j >= n) return 0;
        assert(j < n);
        size_t l = 0, r = min(mx + 1, i + 2);
        if (seg_hash(i - r + 2, i) == seg_hash(j - r + 2, j)) return r - 1;
        while (l + 1 < r) {
            size_t m = l + (r - l) / 2;
            (seg_hash(i + 1 - m, i) == seg_hash(j + 1 - m, j) ? l : r) = m;
        }
        return l;
    }
};

//Returns all runs of s as vector of {p, l, r}, which means that period of s[l, r] is p
//O(n) for main cycle + O(sorting(runs))
vector<array<int, 3>> solve(const string& s) {
    const size_t n = s.size();
    vector<int> st(n + 1);
    vector<array<int, 3>> runs;
    hasher_pow2<64> kek(s.begin(), s.end());
    for (int inv = 0; inv < 2; ++inv) {
        st[0] = n + 1;
        for (int i = n, top = 0, lt = 0; i; --i) {
            while (top) {
                int x = min(st[top] - i, st[top - 1] - st[top]);
                lt = s[i - 1] == s[st[top] - 1] ? kek.lcp(i - 1, st[top] - 1, x) : 0;
                if ((lt == x && st[top] - i < st[top - 1] - st[top]) ||
                        (lt < x && ((s[i + lt - 1] < s[st[top] + lt - 1]) ^ inv))) {
                    --top, lt = 0;
                } else {
                    break;
                }
            }
            int j = st[top], x = s[i-2] == s[j-2] ? kek.lcs(i - 2, j - 2, j - i) : 0;
            st[++top] = i;
            if (x < j - i) {
                int y = s[i + lt - 1] == s[j + lt - 1] ? lt + kek.lcp(i + lt - 1, j + lt - 1, n) : lt;
                if (x + y >= j - i) {
                    runs.push_back({j - i, i - x - 1, j + y - 2});
                }
            }
        }
    }
    sort(runs.begin(), runs.end());
    runs.erase(unique(runs.begin(), runs.end()), runs.end());
    return runs;
}

int main() {
    string s; cin >> s;
    auto res = solve(s);
    cout << res.size() << '\n';
    for (auto [p, l, r] : res) {
        cout << p << ' ' << l << ' ' << r + 1 << '\n';
    }
}
