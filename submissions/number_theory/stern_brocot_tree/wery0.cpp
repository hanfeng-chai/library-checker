#pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")

#include "bits/stdc++.h"

//#define NDEBUG
#define F first
#define S second
#define vec vector
#define pb push_back
#define pll pair<ll, ll>
#define pdd pair<ld, ld>
#define pii pair<int, int>
#define all(m) m.begin(), m.end()
#define rall(m) m.rbegin(), m.rend()
#define uid uniform_int_distribution
#define timeStamp() std::chrono::steady_clock::now()
#define unify(m) sort(all(m)), m.erase(unique(all(m)), m.end());
#define duration_micro(a) chrono::duration_cast<chrono::microseconds>(a).count()
#define duration_milli(a) chrono::duration_cast<chrono::milliseconds>(a).count()
#define fast cin.tie(0), cout.tie(0), cin.sync_with_stdio(0), cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
using uint = unsigned int;
using ull = unsigned long long;
mt19937 rnd(timeStamp().time_since_epoch().count());
mt19937_64 rndll(timeStamp().time_since_epoch().count());
template<typename T, typename U> bool chmin(T& a, const U& b) {return (T)b < a ? a = b, 1 : 0;}
template<typename T, typename U> bool chmax(T& a, const U& b) {return (T)b > a ? a = b, 1 : 0;}
struct custom_hash {static uint64_t xs(uint64_t x) {x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);} template<typename T> size_t operator()(T x) const {static const uint64_t C = timeStamp().time_since_epoch().count(); return xs(hash<T> {}(x) + C);}};
template<typename K> using uset = unordered_set<K, custom_hash>;
template<typename K, typename V> using umap = unordered_map<K, V, custom_hash>;
template<typename T1, typename T2> ostream& operator<<(ostream& out, const pair<T1, T2>& x) {return out << x.F << ' ' << x.S;}
template<typename T1, typename T2> istream& operator>>(istream& in, pair<T1, T2>& x) {return in >> x.F >> x.S;}
template<typename T, size_t N> istream& operator>>(istream& in, array<T, N>& a) {for (auto& x : a) in >> x; return in;}
template<typename T, size_t N> ostream& operator<<(ostream& out, const array<T, N>& a) {for (size_t i = 0; i < a.size(); ++i) {out << a[i]; if (i + 1 < a.size()) out << ' ';} return out;}
template<typename T> istream& operator>>(istream& in, vector<T>& a) {for (auto& x : a) in >> x; return in;}
template<typename T> ostream& operator<<(ostream& out, const vector<T>& a) {for (size_t i = 0; i < a.size(); ++i) {out << a[i]; if (i + 1 < a.size()) out << ' ';} return out;}

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
    template <typename T_>IO& operator<<(T_&& x) {using T = typename std::remove_cv < typename std::remove_reference<T_>::type >::type; if constexpr (is_custom<T>::value) {write_int(x.get());} else if constexpr (is_default<T>::value) {if constexpr (is_bool<T>::value) {write_bool(x);} else if constexpr (is_string<T>::value) {write_string(x);} else if constexpr (is_char<T>::value) {write_char(x);} else if constexpr (std::is_integral_v<T>) {write_int(x);}} else if constexpr (is_iterable<T>::value) {using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto& y : x) {if (i++) write_char(sep); operator<<(y);}} else if constexpr (is_applyable<T>::value) {constexpr char sep = (any_needs_newline < T, std::make_index_sequence<std::tuple_size_v<T> >>::value) ? '\n' : ' '; int i = 0; std::apply([this, &sep, &i](auto const & ... y) {(((i++ ? write_char(sep) : void()), this->operator<<(y)), ...);}, x);} return *this;}
    template <typename T>IO& operator>>(T& x) {if constexpr (is_custom<T>::value) {typename T::internal_value_type y; read_int(y); x = y;} else if constexpr (is_default<T>::value) {if constexpr (is_string<T>::value) {read_string(x);} else if constexpr (is_char<T>::value) {read_char(x);} else if constexpr (std::is_integral_v<T>) {read_int(x);}} else if constexpr (is_iterable<T>::value) {for (auto& y : x) operator>>(y);} else if constexpr (is_applyable<T>::value) {std::apply([this](auto & ... y) { ((this->operator>>(y)), ...); }, x);} return *this;}
    IO* tie(std::nullptr_t) { return this; }
    void sync_with_stdio(bool) {}
} io;
#define cin io
#define cout io

template<typename T = int64_t>
class Sfraction {
    static_assert(is_integral_v<T>&& is_signed_v<T>, "Numeric type must be signed and integral");

    T numerator_ = 0;
    T denominator_ = 1;

    static T binpow(T val, T pow) {
        T res = 1;
        for (T x = val; pow; pow >>= 1) {
            if (pow & 1) res *= x;
            x *= x;
        }
        return res;
    }

    void normalize() {
        assert((denominator_ || numerator_) && "0 / 0 is not allowed!");
        if (denominator_ < 0) {
            denominator_ *= -1;
            numerator_ *= -1;
        }
        // T gcd = __gcd(abs(numerator_), denominator_);
        // numerator_ /= gcd;
        // denominator_ /= gcd;
    }

public:
    Sfraction() = default;

    template<typename U>
    Sfraction(U value) : numerator_(value) {
        static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
    }

    template <typename U>
    Sfraction(U numerator, U denominator) : numerator_(numerator), denominator_(denominator) {
        static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
        assert((denominator_ || numerator_) && "0 / 0 is not allowed");
        normalize();
    }

    Sfraction& operator+=(const Sfraction& rhs) {
        numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Sfraction operator+(const Sfraction& lhs, const Sfraction& rhs) {
        Sfraction res = lhs;
        res += rhs;
        return res;
    }

    Sfraction& operator-=(const Sfraction& rhs) {
        numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Sfraction operator-(const Sfraction& lhs, const Sfraction& rhs) {
        Sfraction res = lhs;
        res -= rhs;
        return res;
    }

    Sfraction& operator*=(const Sfraction& rhs) {
        numerator_ *= rhs.numerator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Sfraction operator*(const Sfraction& lhs, const Sfraction& rhs) {
        Sfraction res = lhs;
        res *= rhs;
        return res;
    }

    Sfraction& operator/=(const Sfraction& rhs) {
        assert((!is_zero() || !rhs.is_zero()) && "0 / 0 is not allowed!");
        numerator_ *= rhs.denominator_;
        denominator_ *= rhs.numerator_;
        normalize();
        return *this;
    }
    friend Sfraction operator/(const Sfraction& lhs, const Sfraction& rhs) {
        Sfraction res = lhs;
        res /= rhs;
        return res;
    }

    Sfraction operator-() const { return { -numerator_, denominator_}; }

    bool operator==(const Sfraction& rhs) const { return numerator_ == rhs.numerator_ && denominator_ == rhs.denominator_; }
    friend bool operator!=(const Sfraction& lhs, const Sfraction& rhs) { return !(lhs == rhs); }
    bool operator<(const Sfraction& rhs) const { return numerator_ * rhs.denominator_ < rhs.numerator_ * denominator_; }
    friend bool operator>(const Sfraction& lhs, const Sfraction& rhs) { return rhs < lhs; }
    friend bool operator<=(const Sfraction& lhs, const Sfraction& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Sfraction& lhs, const Sfraction& rhs) { return !(lhs < rhs); }

    void pow(T power) {
        if (numerator_ == 0) return;
        T sign = numerator_ > 0 ? 1 : -1;
        numerator_ *= sign;
        if (power < 0) {
            swap(numerator_, denominator_);
            power *= -1;
        }
        numerator_ = binpow(numerator_, power);
        denominator_ = binpow(denominator_, power);
        if ((power & 1) && sign == -1) numerator_ *= -1;
    }
    friend Sfraction pow(const Sfraction& rhs, T power) {
        Sfraction res = rhs;
        res.pow(power);
        return res;
    }

    void invert() {
        swap(numerator_, denominator_);
    }
    friend Sfraction invert(const Sfraction& rhs) {
        Sfraction res = rhs;
        res.invert();
        return res;
    }

    friend Sfraction mediant(const Sfraction& lhs, const Sfraction& rhs) {
        return {lhs.numerator_ + rhs.numerator_, lhs.denominator_ + rhs.denominator_};
    }

    bool is_zero() const { return numerator_ == 0; }
    bool is_infinity() const { return denominator_ == 0; }
    bool is_integer() const { return denominator_ == 1; }
    int get_sign() const { return numerator_ < 0 ? -1 : numerator_ == 0 ? 0 : 1; }
    T get_numerator() const { return numerator_; }
    T get_denominator() const { return denominator_; }
    friend Sfraction abs(const Sfraction& rhs) { return rhs.numerator_ > 0 ? rhs : -rhs; }

    friend ostream& operator<<(ostream& os, const Sfraction& rhs) {
        // if (rhs.is_infinity()) return os << (rhs.numerator_ < 0 ? "-" : "") << "inf";
        // if (rhs.is_integer()) return os << rhs.numerator_;
        // return os << string("\\frac{") << rhs.numerator_ << "}{" << rhs.denominator_ << "}";

        if (rhs.is_infinity()) return os << "1 0";
        if (rhs == -1) return os << "-1";
        return os <<  rhs.numerator_ << " " << rhs.denominator_;
    }
};

template<typename T>
void print(const Sfraction<T>& rhs) {
    if (rhs.is_infinity()) cout << "1 0";
    else if (rhs == -1) cout << "-1";
    else cout << rhs.get_numerator() << " " << rhs.get_denominator();
}

template<typename T>
Sfraction<T> decode_path(vector<T> steps, bool is_left_first) {
    Sfraction<T> l = {0, 1}, r = {1, 0};
    if (steps.empty()) return mediant(l, r);
    auto go_left = [&](T step) {
        r = {l.get_numerator()* step + r.get_numerator(), l.get_denominator()* step + r.get_denominator()};
    };
    auto go_right = [&](T step) {
        l = {l.get_numerator() + r.get_numerator()* step, l.get_denominator() + r.get_denominator()* step};
    };
    if (!is_left_first) go_right(steps[0]);
    if (!is_left_first ^ (steps.size() & 1)) steps.push_back(0);
    for (size_t i = !is_left_first; i < steps.size(); i += 2) {
        go_left(steps[i]);
        go_right(steps[i + 1]);
    }
    return mediant(l, r);
}

template<typename T>
pair<vector<T>, array<Sfraction<T>, 2>> encode_path(Sfraction<T> f) {
    assert(f >= 0 && "No path!");
    assert(f > 0 && !f.is_infinity() && "Infinite path!");
    bool was_inverted = false;
    if (f > 1) was_inverted = true, f.invert();
    vector<T> ans;
    Sfraction<T> l = {0, 1}, r = {1, 0};
    auto go_left = [&](T step) {
        r = {l.get_numerator()* step + r.get_numerator(), l.get_denominator()* step + r.get_denominator()};
    };
    auto go_right = [&](T step) {
        l = {l.get_numerator() + r.get_numerator()* step, l.get_denominator() + r.get_denominator()* step};
    };
    // cout << "NEED: " << f << endl;
    while (true) {
        if (mediant(l, r) == f) {
            return {ans, {l, r}};
        }
        // (l.n* x + r.n) / (l.d* x + r.d) < f.n / f.d
        // f.d(l.n * x + r.n) < f.n(l.d * x + r.d)
        // f.d* l.n* x + f.d* r.n < f.n* l.d* x + f.n* r.d
        // (f.d * l.n - f.n * l.d) * x < f.n* r.d - f.d* r.n
        // x < (f.n * r.d - f.d * r.n) / (f.d * l.n - f.n * l.d);
        // cout << "asldfa" << endl;
        T ch = f.get_numerator() * r.get_denominator() - f.get_denominator() * r.get_numerator();
        T zn = f.get_denominator() * l.get_numerator() - f.get_numerator() * l.get_denominator();
        assert(zn != 0);
        T x = zn < 0 ? (-ch - 1) / -zn : (ch - 1) / zn;
        assert(x > 0);
        ans.push_back(x);
        go_left(x);
        if (mediant(l, r) == f) {
            return {ans, {l, r}};
        }

        ch = f.get_numerator() * l.get_denominator() - f.get_denominator() * l.get_numerator();
        zn = f.get_denominator() * r.get_numerator() - f.get_numerator() * r.get_denominator();
        assert(zn != 0);
        x = zn < 0 ? (-ch - 1) / -zn : (ch - 1) / zn;
        assert(x > 0);
        ans.push_back(x);
        go_right(x);
    }
    assert(0);
    return {ans, {l, r}};
}

template<typename T>
Sfraction<T> lca(Sfraction<T> x, Sfraction<T> y) {
    if (x > y) swap(x, y);
    if (x <= 1 && y >= 1) return 1;
    auto px = encode_path(x).first;
    auto py = encode_path(y).first;
    if (px > py) swap(px, py);
    auto [ix, iy] = mismatch(px.begin(), px.end(), py.begin(), py.end());
    if (ix != px.end()) px.resize(ix - px.begin() + 1);
    return decode_path(px, x < 1);
}

template<typename T>
Sfraction<T> ancestor(Sfraction<T> f, int64_t k) {
    if (k == 0) return 1;
    auto p = encode_path(f).first;
    for (size_t i = 0; i < p.size(); ++i) {
        if (k <= p[i]) {
            p[i] = k;
            p.resize(i + 1);
            return decode_path(p, f < 1);
        }
        k -= p[i];
    }
    return -1;
}

template<typename T>
array<Sfraction<T>, 2> range(Sfraction<T> f) {
    auto [p, gr] = encode_path(f);
    auto [l, r] = gr;
    if (f > 1) {
        l.invert();
        r.invert();
        swap(l, r);
    }
    return {l, r};
}

int main() {
    fast;
    int z; cin >> z;
    for (; z--;) {
        str s; cin >> s;
        if (s == "DECODE_PATH") {
            int a; cin >> a;
            vector<int64_t> m(a);
            bool is_left_first;
            if (a) {
                char c;
                cin >> c >> m[0];
                is_left_first = c == 'L';
                for (int q = 1; q < a; ++q) {
                    cin >> c >> m[q];
                }
            }
            print(decode_path<int64_t>(m, is_left_first)); cout << '\n';
        } else if (s == "ENCODE_PATH") {
            int x, y; cin >> x >> y;
            Sfraction<int64_t> f(x, y);
            auto ans = encode_path<int64_t>(f).first;
            char cur = f < 1 ? 'L' : 'R';
            cout << ans.size() << ' ';
            for (auto step : ans) {
                cout << cur << ' ' << step << ' ';
                cur = cur == 'L' ? 'R' : 'L';
            }
            cout << '\n';
        } else if (s == "LCA") {
            int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
            print(lca<ll>({x1, y1}, {x2, y2})); cout <<'\n';
        } else if (s == "ANCESTOR") {
            ll k, x, y; cin >> k >> x >> y;
            print(ancestor<ll>({x, y}, k)); cout << '\n';
        } else if (s == "RANGE") {
            ll x, y; cin>>x>>y;
            auto [L, R] = range<ll>({x, y});
            print(L); cout << ' ';print(R); cout << '\n';
        } else {
            assert(0);
        }
    }
}
