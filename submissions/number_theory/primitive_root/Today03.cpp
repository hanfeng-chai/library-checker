#ifdef TODAY_KYOPRO

void run() {
    II(q);
    rep(q) {
        II(p);
        say(PrimitiveRoot(p));
    }
}

#else

// https://judge.yosupo.jp/problem/primitive_root
//------>8-------- begin kyopro_library/template.hpp --------->8------
//------>8------ begin kyopro_library/base/include.hpp ------->8------

#include <iostream>
#include <algorithm>
#include <type_traits>
#include <vector>
#include <array>
#include <bitset>
#include <cmath>
#include <complex>
#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include <utility>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cassert>
#include <unistd.h>

using namespace std;
//------>8------- end kyopro_library/base/include.hpp -------->8------

//------>8-------- begin kyopro_library/base/type.hpp -------->8------

using i8 = int8_t;
using i16 = short;
using i32 = int;
using i64 = long long;
using i128 = __int128_t;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = __uint128_t;
using f64 = double;
using f80 = long double;

using ii = i64;
using ll = i64;
using ull = u64;
using vi = vector<ii>;
using vvi = vector<vector<ii>>;
using vvvi = vector<vector<vector<ii>>>;
using vl = vector<ll>;
using vvl = vector<vector<ll>>;
using vvvl = vector<vector<vector<ll>>>;
using ii3 = array<ii, 3>;
using ii4 = array<ii, 4>;
using ii5 = array<ii, 5>;
using lll = i128;
using vlll = vector<lll>;
using vvlll = vector<vector<lll>>;
using ulll = u128;
using ld = f80;
using str = string;
using vstr = vector<str>;
constexpr ii operator""_ii(ull x) { return static_cast<ii>(x); }
constexpr lll operator""_lll(ull x) { return static_cast<lll>(x); }

template <typename T>
using V = vector<T>;
template <typename T>
using VV = vector<vector<T>>;
template <typename T>
using VVV = vector<vector<vector<T>>>;
template <typename T>
using VVVV = vector<vector<vector<vector<T>>>>;
template <typename T>
using VVVVV = vector<vector<vector<vector<vector<T>>>>>;
template <typename T>
using VVVVVV = vector<vector<vector<vector<vector<vector<T>>>>>>;

template <typename T>
using max_pq = priority_queue<T>;
template <typename T>
using min_pq = priority_queue<T, vector<T>, greater<T>>;
template <typename T>
using unset = unordered_set<T>;
template <typename T, typename T2>
using unmap = unordered_map<T, T2>;

template <typename T, typename U>
struct PR : pair<T, U> {
    template <typename... Args>
    PR(Args... args) : pair<T, U>(args...) {}
    PR() = default;
    using pair<T, U>::first;
    using pair<T, U>::second;
    PR& operator+=(const PR& r) {
        first += r.first;
        second += r.second;
        return *this;
    }
    PR& operator-=(const PR& r) {
        first -= r.first;
        second -= r.second;
        return *this;
    }
    PR& operator*=(const PR& r) {
        first *= r.first;
        second *= r.second;
        return *this;
    }
    template <typename S>
    PR& operator+=(const S& r) {
        first += r;
        second += r;
        return *this;
    }
    template <typename S>
    PR& operator-=(const S& r) {
        first -= r;
        second -= r;
        return *this;
    }
    template <typename S>
    PR& operator*=(const S& r) {
        first *= r;
        second *= r;
        return *this;
    }
    PR operator+(const PR& r) const { return PR(*this) += r; }
    PR operator-(const PR& r) const { return PR(*this) -= r; }
    PR operator*(const PR& r) const { return PR(*this) *= r; }
    template <typename S>
    PR operator+(const S& r) const { return PR(*this) += r; }
    template <typename S>
    PR operator-(const S& r) const { return PR(*this) -= r; }
    template <typename S>
    PR operator*(const S& r) const { return PR(*this) *= r; }
    PR operator-() const { return PR{-first, -second}; }
};
using pi = PR<ii, ii>;
using vpi = vector<pi>;
using vvpi = vector<vector<pi>>;
using pl = PR<ll, ll>;
using vpl = vector<pl>;
using vvpl = vector<vector<pl>>;

template <typename T, typename U, typename V>
struct TR : tuple<T, U, V> {
    using tuple<T, U, V>::tuple;
    T& x = get<0>(*this);
    U& y = get<1>(*this);
    V& z = get<2>(*this);
    TR() : tuple<T, U, V>() {}
    TR(const T& a, const U& b, const V& c) : tuple<T, U, V>(a, b, c) {}
    TR(const TR& other) : tuple<T, U, V>(other), x(get<0>(*this)), y(get<1>(*this)), z(get<2>(*this)) {}
    TR(TR&& other) noexcept : tuple<T, U, V>(move(other)), x(get<0>(*this)), y(get<1>(*this)), z(get<2>(*this)) {}
    TR& operator=(const TR& other) {
        tuple<T, U, V>::operator=(other);
        return *this;
    }
    TR& operator=(TR&& other) noexcept {
        tuple<T, U, V>::operator=(move(other));
        return *this;
    }
};
using ti = TR<ii, ii, ii>;
using vti = vector<ti>;
using vvti = vector<vector<ti>>;
using tl = TR<ll, ll, ll>;
using vtl = vector<ti>;
using vvtl = vector<vector<tl>>;

const i32 INFI = 1e9 + 10;
const i64 INFL = 4e18;
const i128 INFLL = 1_lll << 120;

template <typename T>
constexpr T inf = 0;
template <>
constexpr i32 inf<i32> = INFI;
template <>
constexpr i64 inf<i64> = INFL;
template <>
constexpr i128 inf<i128> = INFLL;
template <>
constexpr u32 inf<u32> = INFI;
template <>
constexpr u64 inf<u64> = INFL;
template <>
constexpr u128 inf<u128> = INFLL;
template <>
constexpr f64 inf<f64> = numeric_limits<f64>::infinity();
template <>
constexpr f80 inf<f80> = numeric_limits<f80>::infinity();

istream& operator>>(istream& is, lll& x) {
    int c = is.peek();
    while(c == ' ' || c == '\n') is.get(), c = is.peek();
    bool neg = false;
    if(c == '-') neg = true, is.get();
    x = 0;
    while(isdigit(is.peek())) x = x * 10 + is.get() - '0';
    if(neg) x = -x;
    return is;
}

ostream& operator<<(ostream& os, lll x) {
    if(x < 0) os << '-', x = -x;
    if(x == 0) return os << '0';
    string s;
    while(x > 0) s += char('0' + x % 10), x /= 10;
    reverse(s.begin(), s.end());
    return os << s;
}

#ifdef TDY
lll abs(lll x) {
    if(x < 0) return -x;
    return x;
}
lll gcd(lll a, lll b) {
    while(b) a %= b, swap(a, b);
    return a;
}
#endif
//------>8--------- end kyopro_library/base/type.hpp --------->8------

//------>8------- begin kyopro_library/base/fastio.hpp ------->8------

// AI作FastIO
// インタラクティブも対応！

/// @brief 高速入出力 (fread/fwrite ベース)。cin/cout を透過的に置換する
/// @note 既存の input()/say/line/put/operator<< やマクロ・生 cin/cout がそのまま高速化される。
/// @note 未対応の型 (modint/fraction/set/deque など) は同一バッファを共有する
///       std ストリームへフォールバックするため、書式・順序は従来と一致する。
/// @note operator>>/<< の本体は fastio_impl.hpp で定義する。フォールバック時に io.hpp の
///       グローバル operator<< (tuple/set/deque/array など) を通常の名前検索で見つけるため、
///       template.hpp が io.hpp を include した後に fastio_impl.hpp を include する。
/// @attention using namespace std 前提で裸の cin/cout を使うこと。std::cin / std::cout と
///            完全修飾で書くと #define により壊れる。
namespace FastIO {

    /// @brief pair (および PR など pair 派生型) を判定するコンセプト
    template <typename A, typename B>
    void pair_probe(const pair<A, B>&);
    template <typename T>
    concept PairLike = requires(const T& t) { pair_probe(t); };

    template <typename>
    struct is_vec : false_type {};
    template <typename T>
    struct is_vec<vector<T>> : true_type {};
    template <typename>
    struct is_vec2 : false_type {};
    template <typename T>
    struct is_vec2<vector<vector<T>>> : true_type {};
    template <typename>
    struct is_vec3 : false_type {};
    template <typename T>
    struct is_vec3<vector<vector<vector<T>>>> : true_type {};

    /// @brief fread で stdin をチャンク読みする streambuf
    struct Reader : streambuf {
        static constexpr int SZ = 1 << 18;
        char buf[SZ];
        Reader() { setg(buf, buf, buf); }
        /// @note fread ではなく read(2) を使う。fread はバッファが満杯 (or EOF) になるまで
        ///       block するため、応答が1行ずつ来るインタラクティブ問題でデッドロックする。
        int_type underflow() override {
            ssize_t n;
            do n = ::read(0, buf, SZ);
            while(n < 0 && errno == EINTR);
            if(n <= 0) {
                setg(buf, buf, buf);
                return traits_type::eof();
            }
            setg(buf, buf, buf + n);
            return traits_type::to_int_type(buf[0]);
        }
        /// @brief 1 文字取得して進める (EOF は -1)
        int gc() { return this->sbumpc(); }
        /// @brief 空白を読み飛ばし、最初の非空白文字を返す (消費済み)
        int skip_ws() {
            int c = gc();
            while(c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
            return c;
        }
        template <typename T>
        void read_int(T& x) {
            int c = skip_ws();
            bool neg = false;
            if constexpr(is_signed_v<T>) {
                if(c == '-') neg = true, c = gc();
            }
            T v = 0;
            while(c >= '0' && c <= '9') v = v * 10 + (c - '0'), c = gc();
            if constexpr(is_signed_v<T>) {
                if(neg) v = -v;
            }
            x = v;
        }
        void read_i128(lll& x) {
            int c = skip_ws();
            bool neg = false;
            if(c == '-') neg = true, c = gc();
            ulll v = 0;
            while(c >= '0' && c <= '9') v = v * 10 + (ulll)(c - '0'), c = gc();
            x = neg ? -(lll)v : (lll)v;
        }
        void read_u128(ulll& x) {
            int c = skip_ws();
            ulll v = 0;
            while(c >= '0' && c <= '9') v = v * 10 + (ulll)(c - '0'), c = gc();
            x = v;
        }
        void read_char(char& ch) { ch = (char)skip_ws(); }
        void read_str(string& s) {
            s.clear();
            int c = skip_ws();
            while(c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') s += (char)c, c = gc();
        }
        template <typename F>
        void read_float(F& x) {
            static string t;
            read_str(t);
            x = (F)strtold(t.c_str(), nullptr);
        }
    };

    /// @brief fwrite で stdout へ書き出す streambuf
    struct Writer : streambuf {
        static constexpr int SZ = 1 << 18;
        char buf[SZ];
        Writer() { setp(buf, buf + SZ); }
        ~Writer() { flush(); }
        /// @note write(2) で fd 1 へ直接書く。libc バッファを挟まないので flush() が即座に
        ///       ジャッジへ届き、インタラクティブ問題でそのまま使える。
        void flush() {
            char* p = pbase();
            size_t len = pptr() - p;
            while(len > 0) {
                ssize_t w = ::write(1, p, len);
                if(w < 0) {
                    if(errno == EINTR) continue;
                    break;
                }
                p += w, len -= w;
            }
            setp(buf, buf + SZ);
        }
        int_type overflow(int_type c) override {
            flush();
            if(c != traits_type::eof()) *pptr() = (char)c, pbump(1);
            return c;
        }
        int sync() override {
            flush();
            return 0;
        }
        void pc(char c) { this->sputc(c); }
        void ps(const char* s, int n) { this->sputn(s, n); }
        template <typename T>
        void write_int(T x) {
            using U = make_unsigned_t<T>;
            U u;
            bool neg = false;
            if constexpr(is_signed_v<T>) {
                if(x < 0) neg = true, u = U(0) - (U)x;
                else u = (U)x;
            } else u = (U)x;
            char t[24];
            int n = 0;
            do t[n++] = (char)('0' + int(u % 10)), u /= 10;
            while(u);
            if(neg) pc('-');
            while(n) pc(t[--n]);
        }
        void write_i128(lll x) {
            bool neg = x < 0;
            ulll u = neg ? (ulll)0 - (ulll)x : (ulll)x;
            char t[40];
            int n = 0;
            do t[n++] = (char)('0' + int(u % 10)), u /= 10;
            while(u);
            if(neg) pc('-');
            while(n) pc(t[--n]);
        }
        void write_u128(ulll u) {
            char t[40];
            int n = 0;
            do t[n++] = (char)('0' + int(u % 10)), u /= 10;
            while(u);
            while(n) pc(t[--n]);
        }
        void write_float(long double x) {
            char t[64];
            int n = snprintf(t, sizeof(t), "%.15Lf", x);
            ps(t, n);
        }
    };

    /// @brief 高速入力ラッパ (cin を置換)。未対応型は fb (std::istream) にフォールバック
    struct FastIn {
        Reader rd;
        istream fb{&rd};
        template <typename T>
        FastIn& operator>>(T& x);  // 本体は fastio_impl.hpp
        /// @brief std::ws などのマニピュレータ (テンプレート関数のため専用オーバーロードが必要)
        FastIn& operator>>(istream& (*f)(istream&)) {
            f(fb);
            return *this;
        }
        template <typename T>
        void tie(T) {}
    };

    /// @brief 高速出力ラッパ (cout を置換)。未対応型は fb (std::ostream) にフォールバック
    struct FastOut {
        Writer wt;
        ostream fb{&wt};
        FastOut() { fb << fixed << setprecision(15); }
        void flush() { wt.flush(); }
        template <typename T>
        void write_vec1(const vector<T>& a) {
            int n = a.size();
            for(int i = 0; i < n; i++) {
                *this << a[i];
                if(i != n - 1) wt.pc(' ');
            }
        }
        template <typename T>
        void write_vec2(const vector<vector<T>>& a) {
            int I = a.size();
            for(int i = 0; i < I; i++) {
                int J = a[i].size();
                for(int j = 0; j < J; j++) {
                    *this << a[i][j];
                    if(j != J - 1) wt.pc(' ');
                }
                if(i != I - 1) wt.pc('\n');
            }
        }
        template <typename T>
        void write_vec3(const vector<vector<vector<T>>>& a) {
            int I = a.size();
            for(int i = 0; i < I; i++) {
                int J = a[i].size();
                for(int j = 0; j < J; j++) {
                    int K = a[i][j].size();
                    for(int k = 0; k < K; k++) {
                        *this << a[i][j][k];
                        if(k != K - 1) wt.pc(' ');
                    }
                    wt.pc('\n');
                }
                if(i != I - 1) wt.pc('\n');
            }
        }
        template <typename T>
        FastOut& operator<<(const T& x);  // 本体は fastio_impl.hpp
        /// @brief std::endl / std::flush / std::ends などのマニピュレータ
        FastOut& operator<<(ostream& (*f)(ostream&)) {
            f(fb);
            return *this;
        }
    };

    inline FastIn in;
    inline FastOut out;

}  // namespace FastIO

#define cin FastIO::in
#define cout FastIO::out
//------>8-------- end kyopro_library/base/fastio.hpp -------->8------

//------>8---- begin kyopro_library/base/fastio_impl.hpp ----->8------

/// @brief FastIn/FastOut の operator 本体。io.hpp より後に include し、フォールバック時に
///        io.hpp のグローバル operator>>/<< (tuple/set/deque/array など) を名前検索で解決する。
/// @note modint/fraction/geo などの friend 演算子は ADL で解決されるため include 順に依らない。
namespace FastIO {

    template <typename T>
    FastIn& FastIn::operator>>(T& x) {
        if constexpr(is_same_v<T, char>) rd.read_char(x);
        else if constexpr(is_same_v<T, bool>) fb >> x;
        else if constexpr(is_same_v<T, lll>) rd.read_i128(x);
        else if constexpr(is_same_v<T, ulll>) rd.read_u128(x);
        else if constexpr(is_integral_v<T>) rd.read_int(x);
        else if constexpr(is_floating_point_v<T>) rd.read_float(x);
        else if constexpr(is_same_v<T, string>) rd.read_str(x);
        else if constexpr(is_vec<T>::value)
            for(auto& e : x) *this >> e;
        else if constexpr(PairLike<T>) *this >> x.first, *this >> x.second;
        else fb >> x;
        return *this;
    }

    template <typename T>
    FastOut& FastOut::operator<<(const T& x) {
        using D = decay_t<T>;
        if constexpr(is_same_v<D, char>) wt.pc(x);
        else if constexpr(is_same_v<D, bool>) wt.pc(x ? '1' : '0');
        else if constexpr(is_same_v<D, lll>) wt.write_i128(x);
        else if constexpr(is_same_v<D, ulll>) wt.write_u128(x);
        else if constexpr(is_integral_v<D>) wt.write_int(x);
        else if constexpr(is_floating_point_v<D>) wt.write_float((long double)x);
        else if constexpr(is_same_v<D, string>) wt.ps(x.data(), (int)x.size());
        else if constexpr(is_same_v<D, char*> || is_same_v<D, const char*>) wt.ps(x, (int)strlen(x));
        else if constexpr(is_vec3<D>::value) write_vec3(x);
        else if constexpr(is_vec2<D>::value) write_vec2(x);
        else if constexpr(is_vec<D>::value) write_vec1(x);
        else if constexpr(PairLike<D>) {
            *this << x.first;
            wt.pc(' ');
            *this << x.second;
        } else fb << x;
        return *this;
    }

}  // namespace FastIO
//------>8----- end kyopro_library/base/fastio_impl.hpp ------>8------

//------>8------- begin kyopro_library/base/macro.hpp -------->8------

#define rep1(n) for(ii i = 0; i < (n); i++)
#define rep2(i, n) for(ii i = 0; i < (n); i++)
#define rep3(i, a, b) for(ii i = (a); i < (b); i++)
#define rep4(i, a, b, c) for(ii i = (a); i < (b); i += (c))
#define rep_overload(a, b, c, d, e, ...) e
#define rep(...) rep_overload(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)

#define per1(n) for(ii i = (n) - 1; i >= 0; i--)
#define per2(i, n) for(ii i = (n) - 1; i >= 0; i--)
#define per3(i, a, b) for(ii i = (b) - 1; i >= (a); i--)
#define per4(i, a, b, c) for(ii i = (b) - 1; i >= (a); i -= (c))
#define per_overload(a, b, c, d, e, ...) e
#define per(...) per_overload(__VA_ARGS__, per4, per3, per2, per1)(__VA_ARGS__)

#define fore(x, a) for(auto &x : a)

#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()

#define QYN(q, a, b) ((q) ? (a) : (b))

#define pb push_back
#define eb emplace_back
#define mkp make_pair
#define mkt make_tuple
#define fi first
#define se second

#define dov(v, f)         \
    [&]() {               \
        auto &&_v = (v);  \
        for(auto &x : _v) \
            f(x);         \
    }()
#define mkv(v, f)                                            \
    [&]() {                                                  \
        auto &&_v = (v);                                     \
        using Type = std::decay_t<decltype(f(*_v.begin()))>; \
        std::vector<Type> ret;                               \
        ret.reserve(_v.size());                              \
        for(const auto &x : _v)                              \
            ret.push_back(f(x));                             \
        return ret;                                          \
    }()

#define II(...)     \
    ii __VA_ARGS__; \
    input(__VA_ARGS__)
#define LL(...)     \
    ll __VA_ARGS__; \
    input(__VA_ARGS__)
#define LLL(...)     \
    lll __VA_ARGS__; \
    input(__VA_ARGS__)
#define IDX(...)        \
    ii __VA_ARGS__;     \
    input(__VA_ARGS__); \
    input_index(__VA_ARGS__)
#define STR(...)        \
    string __VA_ARGS__; \
    input(__VA_ARGS__);
#define CHR(...)      \
    char __VA_ARGS__; \
    input(__VA_ARGS__);
#define LD(...)     \
    ld __VA_ARGS__; \
    input(__VA_ARGS__);
#define VI(A, N)     \
    vector<ii> A(N); \
    input(A);
#define VVI(A, N, M)                        \
    vector<vector<ii>> A(N, vector<ii>(M)); \
    input(A);
#define VL(A, N)     \
    vector<ll> A(N); \
    input(A);
#define VVL(A, N, M)                        \
    vector<vector<ll>> A(N, vector<ll>(M)); \
    input(A);
#define VPI(A, N) \
    vpi A(N);     \
    input(A);
#define VTI(A, N) \
    vti A(N);     \
    input(A);
#define VI2(A, B, N)       \
    vector<ii> A(N), B(N); \
    rep(i, N) cin >> A[i] >> B[i];
#define VL2(A, B, N)       \
    vector<ll> A(N), B(N); \
    rep(i, N) cin >> A[i] >> B[i];
#define VI3(A, B, C, N)          \
    vector<ii> A(N), B(N), C(N); \
    rep(i, N) cin >> A[i] >> B[i] >> C[i];
#define VL3(A, B, C, N)          \
    vector<ll> A(N), B(N), C(N); \
    rep(i, N) cin >> A[i] >> B[i] >> C[i];
#define VST(A, N)        \
    vector<string> A(N); \
    input(A);
#define IN2(A, B) rep(i, siz(A)) cin >> A[i] >> B[i];
#define IN3(A, B, C) rep(i, siz(A)) cin >> A[i] >> B[i] >> C[i];
#define IN4(A, B, C, D) rep(i, siz(A)) cin >> A[i] >> B[i] >> C[i] >> D[i];
#define IN5(A, B, C, D, E) \
    rep(i, siz(A)) cin >> A[i] >> B[i] >> C[i] >> D[i] >> E[i];
//------>8-------- end kyopro_library/base/macro.hpp --------->8------

//------>8--------- begin kyopro_library/base/io.hpp --------->8------


const char NL = '\n';
void flush() { cout.flush(); }

const string Yes = "Yes";
const string No = "No";
const string YES = "YES";
const string NO = "NO";
const string ALICE = "Alice";
const string BOB = "Bob";
const string FIRST = "First";
const string SECOND = "Second";
inline string YesNo(bool f) { return f ? Yes : No; }
inline string YESNO(bool f) { return f ? YES : NO; }
inline string AliBo(bool f) { return f ? ALICE : BOB; }
inline string FiSe(bool f) { return f ? FIRST : SECOND; }

template <typename T>
istream& operator>>(istream& is, vector<vector<T>>& v) {
    for(auto& x : v)
        for(auto& y : x) is >> y;
    return is;
}
template <typename T>
istream& operator>>(istream& is, vector<T>& v) {
    for(auto& x : v) is >> x;
    return is;
}
template <typename T1, typename T2>
istream& operator>>(istream& is, pair<T1, T2>& p) {
    is >> p.first >> p.second;
    return is;
}

template <class... T>
void input(T&... a) { (cin >> ... >> a); }

template <class T>
void input_index(T& a) { a--; }
template <class T, class... Ts>
void input_index(T& a, Ts&... b) {
    a--;
    input_index(b...);
}

template <typename T1, typename T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
    os << p.fi << ' ' << p.se;
    return os;
}
template <typename T1, typename T2>
ostream& operator<<(ostream& os, const PR<T1, T2>& p) {
    os << p.fi << ' ' << p.se;
    return os;
}
template <typename T1, typename T2, typename T3>
ostream& operator<<(ostream& os, const tuple<T1, T2, T3>& t) {
    os << ' ' << get<0>(t) << ' ' << get<1>(t) << ' ' << get<2>(t);
    return os;
}
template <typename T1, typename T2, typename T3>
ostream& operator<<(ostream& os, const TR<T1, T2, T3>& t) {
    os << ' ' << get<0>(t) << ' ' << get<1>(t) << ' ' << get<2>(t);
    return os;
}
template <typename T1, typename T2, typename T3, typename T4>
ostream& operator<<(ostream& os, const tuple<T1, T2, T3, T4>& t) {
    os << ' ' << get<0>(t) << ' ' << get<1>(t) << ' ' << get<2>(t) << ' ' << get<3>(t);
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, const vector<vector<vector<T>>>& a) {
    int I = a.size();
    for(int i = 0; i < I; i++) {
        int J = a[i].size();
        for(int j = 0; j < J; j++) {
            int K = a[i][j].size();
            for(int k = 0; k < K; k++) {
                os << a[i][j][k];
                if(k != K - 1) os << ' ';
            }
            os << NL;
        }
        if(i != I - 1) os << NL;
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, const vector<vector<T>>& a) {
    int I = a.size();
    for(int i = 0; i < I; i++) {
        int J = a[i].size();
        for(int j = 0; j < J; j++) {
            os << a[i][j];
            if(j != J - 1) os << ' ';
        }
        if(i != I - 1) cout << NL;
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& a) {
    int n = a.size();
    for(int i = 0; i < n; i++) {
        os << a[i];
        if(i != n - 1) os << ' ';
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const set<T>& a) {
    for(auto itr = a.begin(); itr != a.end(); itr++) {
        os << *itr;
        if(next(itr) != a.end()) os << ' ';
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, const multiset<T>& a) {
    for(auto itr = a.begin(); itr != a.end(); itr++) {
        os << *itr;
        if(next(itr) != a.end()) os << ' ';
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, const deque<T>& a) {
    for(auto itr = a.begin(); itr != a.end(); itr++) {
        os << *itr;
        if(next(itr) != a.end()) os << ' ';
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, queue<T> a) {
    while(!a.empty()) {
        os << a.front();
        a.pop();
        if(a.size()) os << ' ';
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, priority_queue<T> a) {
    while(!a.empty()) {
        os << a.top();
        a.pop();
        if(a.size()) os << ' ';
    }
    return os;
}
template <typename T>
ostream& operator<<(ostream& os, priority_queue<T, vector<T>, greater<T>> a) {
    while(!a.empty()) {
        os << a.top();
        a.pop();
        if(a.size()) os << ' ';
    }
    return os;
}
template <typename T, auto N>
ostream& operator<<(ostream& os, array<T, N> a) {
    for(int i = 0; i < N; i++) {
        os << a[i];
        if(i != N - 1) os << ' ';
    }
    return os;
}

template <class T, class... Ts>
void put(const T& a, const Ts&... b) {
    cout << a;
    (void)(cout << ... << b);
}

template <class T, class... Ts>
void line(const T& a, const Ts&... b) {
    cout << a;
    (void)(cout << ... << (cout << ' ', b));
    cout << ' ';
}

void say() { cout << '\n'; }
template <class T, class... Ts>
void say(const T& a, const Ts&... b) {
    cout << a;
    (void)(cout << ... << (cout << ' ', b));
    cout << NL;
}

void esay() {
#ifdef TDY
    flush();
    cerr << NL;
#endif
}
template <class T, class... Ts>
void esay(const T& a, const Ts&... b) {
#ifdef TDY
    flush();
    cerr << a;
    (void)(cerr << ... << (cerr << ' ', b));
    cerr << NL;
#endif
}

#define O(...)            \
    {                     \
        say(__VA_ARGS__); \
        return;           \
    }
//------>8---------- end kyopro_library/base/io.hpp ---------->8------

//------>8-------- begin kyopro_library/base/util.hpp -------->8------

// 便利関数たち

// https://trap.jp/post/1224/
template <class... T>
constexpr auto min(T... a) {
    return min(initializer_list<common_type_t<T...>>{a...});
}

template <class... T>
constexpr auto max(T... a) {
    return max(initializer_list<common_type_t<T...>>{a...});
}

template <typename A, typename B>
bool chmin(A& a, B b) {
    if(a > b) {
        a = b;
        return true;
    }
    return false;
}

template <typename A, typename B>
bool chmax(A& a, B b) {
    if(a < b) {
        a = b;
        return true;
    }
    return false;
}

// A/B以下の最大の整数
template <typename A, typename B>
common_type_t<A, B> myfloor(A a, B b) {
    assert(b != 0);
    if(b < 0) a = -a, b = -b;
    return a / b - (a % b < 0);
}

// A/B以上の最小の整数
template <typename A, typename B>
common_type_t<A, B> myceil(A a, B b) {
    assert(b != 0);
    if(b < 0) a = -a, b = -b;
    return a / b + (a % b > 0);
}

// A%B(in [0,B))
template <typename A, typename B>
common_type_t<A, B> mymod(A a, B b) {
    assert(b != 0);
    if(b < 0) b = -b;
    if(a > 0) return a % b;
    return (a % b + b) % b;
}

template <typename A, typename B>
common_type_t<A, B> mylcm(A a, B b) {
    using T = common_type_t<A, B>;
    T g = gcd<T>(a, b);
    return T(a) / g * b;
}

template <typename T>
inline ii siz(const T& v) { return v.size(); }

template <typename T>
T minv(const V<T>& v) {
    if(v.empty()) return inf<T>;
    return *min_element(all(v));
}
template <typename T>
T maxv(const V<T>& v) {
    if(v.empty()) return -inf<T>;
    return *max_element(all(v));
}
template <typename T>
ii minidx(const V<T>& v) { return min_element(all(v)) - v.begin(); }
template <typename T>
ii maxidx(const V<T>& v) { return max_element(all(v)) - v.begin(); }
template <typename T>
T sumv(const V<T>& v) { return reduce(v.begin(), v.end()); }
template <typename T>
ii lob(const V<T>& v, const T& x) { return lower_bound(all(v), x) - v.begin(); }
template <typename T>
ii upb(const V<T>& v, const T& x) { return upper_bound(all(v), x) - v.begin(); }
template <typename T>
ii findv(const V<T>& v, const T& x) {
    for(ii i = 0; i < siz(v); i++)
        if(v[i] == x) return i;
    return siz(v);
}
template <typename T>
ii find_lastv(const V<T>& v, const T& x) {
    for(ii i = siz(v) - 1; i >= 0; i--)
        if(v[i] == x) return i;
    return -1;
}

// do~~ 破壊的に変える
// mk~~ 新しく作る

template <typename T>
void doso(T& v) {
    sort(v.begin(), v.end());
}

template <typename T>
T mkso(const T& v) {
    auto w = v;
    doso(w);
    return w;
}

template <typename T>
void dosor(T& v) {
    sort(v.rbegin(), v.rend());
}

template <typename T>
T mksor(const T& v) {
    auto w = v;
    dorsort(w);
    return w;
}

template <typename T>
void douniq(V<T>& v) {
    sort(all(v));
    v.erase(unique(v.begin(), v.end()), v.end());
}

template <typename T>
V<T> mkuniq(const V<T>& v) {
    auto w = v;
    douniq(w);
    return w;
}

template <typename T>
V<T> mkcomp(V<T> v) {
    auto w = v;
    douniq(w);
    for(T& x : v) x = lob(w, x);
    return v;
}

ii countv(const auto& a, auto v) {
    return count(a.begin(), a.end(), v);
}

void doinsert(auto& a, ii idx, auto v) {
    assert(idx <= siz(a));
    a.insert(a.begin() + idx, v);
}

auto mkinsert(const auto& a, ii idx, auto v) {
    auto b = a;
    doinsert(b, idx, v);
    return b;
}

void doerase(auto& a, ii idx) {
    assert(idx < siz(a));
    a.erase(a.begin() + idx);
}

auto mkerase(const auto& a, ii idx) {
    auto b = a;
    doerase(b, idx);
    return b;
}

// 先頭をoffset個分後ろに
void dorotateback(auto& a, ii offset) {
    offset %= siz(a);
    rotate(a.begin(), a.end() - offset, a.end());
}

// 末尾をoffset個分前の方に
void dorotatefront(auto& a, ii offset) {
    offset %= siz(a);
    rotate(a.begin(), a.begin() + offset, a.end());
}

auto mkpush(const auto& a, auto v) {
    auto b = a;
    b.push_back(v);
    return b;
}

template <typename T>
V<T> mkslice(const V<T>& a, ii l, ii r) {
    assert(l <= r && l >= 0 && r <= siz(a));
    V<T> b(a.begin() + l, a.begin() + r);
    return b;
}

template <typename T>
V<T> mkconcat(const V<T>& a, const V<T>& b) {
    auto ret = a;
    ret.reserve(siz(a) + siz(b));
    for(auto x : b) ret.push_back(x);
    return ret;
}

// aの後ろにbをくっつける
// a <<= bと同じ
template <typename T>
void doconcat(V<T>& a, const V<T>& b) {
    for(auto x : b) a.push_back(x);
}

template <typename A, typename B>
vector<PR<A, B>> zip(const vector<A>& a, const vector<B>& b) {
    ii n = siz(a);
    vector<PR<A, B>> ret(n);
    for(ii i = 0; i < n; i++) ret[i] = {a[i], b[i]};
    return ret;
}

template <typename A, typename B, typename C>
vector<tuple<A, B, C>> zip(const vector<A>& a, const vector<B>& b, const vector<C>& c) {
    ii n = siz(a);
    vector<tuple<A, B, C>> ret(n);
    for(ii i = 0; i < n; i++) ret[i] = {a[i], b[i], c[i]};
    return ret;
}

template <typename A, typename B>
PR<vector<A>, vector<B>> unzip(const vector<PR<A, B>>& p) {
    ii n = siz(p);
    vector<A> reta(n);
    vector<B> retb(n);
    for(ii i = 0; i < n; i++) {
        reta[i] = p[i].first;
        retb[i] = p[i].second;
    }
    return mkp(reta, retb);
}

template <typename A, typename B, typename C>
TR<vector<A>, vector<B>, vector<C>> unzip(const vector<TR<A, B, C>>& p) {
    ii n = siz(p);
    vector<A> reta(n);
    vector<B> retb(n);
    vector<C> retc(n);
    for(ii i = 0; i < n; i++) {
        auto [a, b, c] = p[i];
        reta[i] = a;
        retb[i] = b;
        retc[i] = c;
    }
    return mkt(reta, retb, retc);
}

template <typename T>
T pick(max_pq<T>& v) {
    T ret = v.top();
    v.pop();
    return ret;
}
template <typename T>
T pick(min_pq<T>& v) {
    T ret = v.top();
    v.pop();
    return ret;
}
template <typename T>
T pick(queue<T>& v) {
    T ret = v.front();
    v.pop();
    return ret;
}
template <typename T>
T pick(vector<T>& v) {
    T ret = v.back();
    v.pop_back();
    return ret;
}
template <typename T>
T pickf(deque<T>& v) {
    T ret = v.front();
    v.pop_front();
    return ret;
}
template <typename T>
T pickb(deque<T>& v) {
    T ret = v.back();
    v.pop_back();
    return ret;
}

template <typename T>
bool nxperm(T& v) { return next_permutation(v.begin(), v.end()); }

// 前置インクリメント (++v)
template <typename T>
V<T>& operator++(V<T>& a) {
    for(auto& x : a) ++x;
    return a;
}
// 後置インクリメント (v++)
template <typename T>
V<T> operator++(V<T>& a, int) {
    V<T> res = a;
    ++a;
    return res;
}
// 前置デクリメント (--v)
template <typename T>
V<T>& operator--(V<T>& a) {
    for(auto& x : a) --x;
    return a;
}
// 後置デクリメント (v--)
template <typename T>
V<T> operator--(V<T>& a, int) {
    V<T> res = a;
    --a;
    return res;
}

template <typename T, typename U>
V<T> operator>>=(const U& b, V<T>& a) {
    a.insert(a.begin(), b);
    return a;
}
template <typename T, typename U>
V<T> operator>>(const U& b, V<T> a) {
    b >>= a;
    return a;
}

template <typename T, typename U>
V<T> operator<<=(V<T>& a, const U& b) {
    a.push_back(b);
    return a;
}
template <typename T, typename U>
void operator<<=(V<T>& a, const V<U>& b) {
    V<T> ret = a;
    for(const auto& x : b) ret.push_back(x);
    a = ret;
}
template <typename T, typename U>
V<T> operator<<(V<T> a, const U& b) {
    a <<= b;
    return a;
}
template <typename T, typename U>
V<T> operator<<(V<T> a, const V<U>& b) {
    a <<= b;
    return a;
}

// 演算子オーバーロードを一括定義するマクロ
#define DEFINE_VECTOR_OP(OP)                               \
    /* V<T> OP= スカラー */                            \
    template <typename T, typename U>                      \
    V<T>& operator OP##=(V<T>& a, const U & b) {           \
        for(auto& x : a) x OP## = b;                       \
        return a;                                          \
    }                                                      \
    /* V<T> OP= V<U> (ベクトル同士) */               \
    template <typename T, typename U>                      \
    V<T>& operator OP##=(V<T>& a, const V<U>& b) {         \
        assert(a.size() == b.size());                      \
        for(ii i = 0; i < a.size(); i++) a[i] OP## = b[i]; \
        return a;                                          \
    }                                                      \
    /* V<T> OP スカラー */                             \
    template <typename T, typename U>                      \
    V<T> operator OP(V<T> a, const U & b) {                \
        a OP## = b;                                        \
        return a;                                          \
    }                                                      \
    /* スカラー OP V<T> */                             \
    template <typename T, typename U>                      \
    V<T> operator OP(const U & a, V<T> b) {                \
        for(auto& x : b) x = a OP x;                       \
        return b;                                          \
    }                                                      \
    /* V<T> OP V<U> (ベクトル同士) */                \
    template <typename T, typename U>                      \
    V<T> operator OP(V<T> a, const V<U>& b) {              \
        a OP## = b;                                        \
        return a;                                          \
    }

DEFINE_VECTOR_OP(+)
DEFINE_VECTOR_OP(-)
DEFINE_VECTOR_OP(*)
DEFINE_VECTOR_OP(/)
DEFINE_VECTOR_OP(%)
DEFINE_VECTOR_OP(&)
DEFINE_VECTOR_OP(|)
DEFINE_VECTOR_OP(^)

#undef DEFINE_VECTOR_OP

// 多次元のvectorを作る
// initで型が推論されるので注意
template <typename T>
V<T> mkvec(ii n, T init) {
    return V<T>(n, init);
}
template <typename... Ts>
auto mkvec(ii n, Ts... ts) {
    return V<decltype(mkvec(ts...))>(n, mkvec(ts...));
}

// 累積和
template <typename T = i64, typename U>
V<T> mksum(const V<U>& v) {
    ii n = siz(v);
    V<T> ret(n + 1);
    for(ii i = 0; i < n; i++) ret[i + 1] = ret[i] + v[i];
    return ret;
}

// 累積max
template <typename T>
V<T> mkprefmax(const V<T>& v) {
    ii n = siz(v);
    V<T> ret(n + 1, -inf<T>);
    for(ii i = 0; i < n; i++) ret[i + 1] = max(ret[i], v[i]);
    return ret;
}

// 累積min
template <typename T>
V<T> mkprefmin(const V<T>& v) {
    ii n = siz(v);
    V<T> ret(n + 1, inf<T>);
    for(ii i = 0; i < n; i++) ret[i + 1] = min(ret[i], v[i]);
    return ret;
}

template <typename T>
V<T> mksuffmax(const V<T>& v) {
    ii n = siz(v);
    V<T> ret(n + 1, -inf<T>);
    for(ii i = n - 1; i >= 0; i--) ret[i] = max(ret[i + 1], v[i]);
    return ret;
}

template <typename T>
V<T> mksuffmin(const V<T>& v) {
    ii n = siz(v);
    V<T> ret(n + 1, inf<T>);
    for(ii i = n - 1; i >= 0; i--) ret[i] = min(ret[i + 1], v[i]);
    return ret;
}

vi mkiota(ii n) {
    vi ret(n);
    iota(ret.begin(), ret.end(), 0);
    return ret;
}

template <typename T>
T mkrev(T A) {
    reverse(A.begin(), A.end());
    return A;
}

template <typename T>
void dorev(T& A) {
    reverse(A.begin(), A.end());
}

vi mkinv(const vi& A, ii mx = -1) {
    ii n = siz(A);
    if(mx == -1) mx = maxv(A) + 1;
    vi ret(mx);
    for(ii i = 0; i < n; i++) ret[A[i]] = i;
    return ret;
}

template <typename T>
vvi mkinvvec(const vi& A, ii mx = -1) {
    ii n = siz(A);
    if(mx == -1) mx = maxv(A) + 1;
    vvi ret(mx);
    for(ii i = 0; i < n; i++) ret[A[i]].push_back(i);
    return ret;
}

vi mkfreq(const vi& A, ii mx = -1) {
    ii n = siz(A);
    if(mx == -1) mx = maxv(A) + 1;
    vi ret(mx);
    for(ii i = 0; i < n; i++) ret[A[i]]++;
    return ret;
}

template <typename T>
vi mkarg(const V<T>& A) {
    vi ret = mkiota(siz(A));
    sort(ret.begin(), ret.end(), [&](ii i, ii j) {
        return (A[i] == A[j] ? i < j : A[i] < A[j]);
    });
    return ret;
}

template <typename T>
ii digit_siz(T n) {
    ii ret = 0;
    while(n) {
        ret++;
        n /= 10;
    }
    return ret;
}

template <typename T>
vi digits(T n) {
    vi ret;
    while(n) {
        ret.push_back(n % 10);
        n /= 10;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

template <typename T = i64>
T intpow(T x, T r) {
    T ret = 1;
    while(r) {
        if(r & 1) ret *= x;
        x *= x;
        r >>= 1;
    }
    return ret;
}

i64 tenpow(ii r) {
    i64 ret = 1;
    while(r--) ret *= 10;
    return ret;
}

template <typename T>
T intsqrt(T x) {
    i64 sq = (T)sqrtl(ld(x));
    while(sq * sq > x) sq--;
    while((sq + 1) * (sq + 1) <= x) sq++;
    return sq;
}

template <typename T = i128>
T euc_dist(auto ax, auto ay, auto bx, auto by) {
    return T(ax - bx) * (ax - bx) + T(ay - by) * (ay - by);
}

template <typename T = i64>
T man_dist(auto ax, auto ay, auto bx, auto by) {
    return abs(T(ax) - bx) + abs(T(ay) - by);
}

ii lotoi(char c) {
    assert('a' <= c && c <= 'z');
    return c - 'a';
}

char itolo(ii i) {
    assert(0 <= i && i <= 25);
    return char('a' + i);
}

ii hitoi(char c) {
    assert('A' <= c && c <= 'Z');
    return c - 'A';
}

char itohi(ii i) {
    assert(0 <= i && i <= 25);
    return char('A' + i);
}

ii ctoi(char c) {
    assert('0' <= c && c <= '9');
    return c - '0';
}

char itoc(ii i) {
    assert(0 <= i && i <= 9);
    return char('0' + i);
}

vi stov(const str& s, char base = 'a') {
    vi ret(s.size());
    rep(i, s.size()) ret[i] = s[i] - base;
    return ret;
}

str vtos(const vi& a, char base = 'a') {
    str ret;
    rep(i, a.size()) ret.push_back(char(base + a[i]));
    return ret;
}

ii popcount(i32 n) { return __builtin_popcount(n); }
ii popcount(i64 n) { return __builtin_popcountll(n); }

ii parity(i32 n) { return __builtin_parity(n); }
ii parity(i64 n) { return __builtin_parityll(n); }

// 最上位ビットの位置
ii topbit(i32 n) { return n ? 31 - __builtin_clz(n) : -1; }
ii topbit(i64 n) { return n ? 63 - __builtin_clzll(n) : -1; }

// 2進表現の長さ
ii bitsiz(i32 n) { return n ? 32 - __builtin_clz(n) : 1; }
ii bitsiz(i64 n) { return n ? 64 - __builtin_clzll(n) : 1; }

// 最下位ビットの位置
ii bottombit(i32 n) { return n ? __builtin_ctz(n) : -1; }
ii bottombit(i64 n) { return n ? __builtin_ctzll(n) : -1; }

bool ispower2(i32 n) { return n && (n & -n) == n; }

ll mkmask(ii n) { return (1LL << n) - 1; }

bool hasbit(i64 n, ii i) { return (n >> i & 1); }

vi mksubset(ii s) {
    vi ret;
    ii t = s;
    do {
        ret.push_back(t);
        --t &= s;
    } while(t != s);
    return ret;
}

// 2進数表記を返す
string tobinary(i64 n, ii len = 32, bool rev = false) {
    string ret;
    for(ii i = 0; i < len; i++) ret += (hasbit(n, rev ? len - 1 - i : i) ? '1' : '0');
    return ret;
}

// Mint有理数の復元用
template <ii iter = 1000, typename Mint>
str tofraction(const Mint& a) {
    for(ii deno = 1; deno <= iter; deno++) {
        Mint inv = ((Mint)deno).inv();
        for(ii nume = 0; nume <= iter; nume++) {
            Mint val = inv * nume;
            if(val == a) {
                if(deno == 1) return to_string(nume);
                return to_string(nume) + "/" + to_string(deno);
            } else if(-val == a) {
                if(deno == 1) return to_string(-nume);
                return to_string(-nume) + "/" + to_string(deno);
            }
        }
    }
    return "NF";
}
//------>8--------- end kyopro_library/base/util.hpp --------->8------


void run();
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(7);
#ifdef MULTI
    II(T);
    rep(t, T) {
#ifdef TDY
        // esay("============ Case: #", t + 1, " ============");
#endif
        run();
    }
#else
    run();
#endif
}

#ifdef DEBUG
#include "./debug.hpp"
#else
#define debug(...)
#define print_line
#endif
//------>8--------- end kyopro_library/template.hpp ---------->8------

//------>8--- begin kyopro_library/math/primitive_root.hpp --->8------
//------>8------ begin kyopro_library/others/modcal.hpp ------>8------

/// @brief x^n (mod m) を返す
template <typename T = ll>
T ModPow(T x, T n, T mod) {
    T ret = 1;
    if(typeid(T) == typeid(i64) && mod > INFI * 2) return ModPow<i128>(x, n, mod);
    while(n > 0) {
        if(n & 1) (ret *= x) %= mod;
        (x *= x) %= mod;
        n >>= 1;
    }
    return ret;
}

/// @brief x^(-1) (mod m) を返す
ll ModInv(ll a, ll m) {
    ll b = m, u = 1, v = 0;
    while(b) {
        ll t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    return (u + m) % m;
}
//------>8------- end kyopro_library/others/modcal.hpp ------->8------

//------>8-- begin kyopro_library/math/prime_factorize.hpp --->8------
//------>8--- begin kyopro_library/math/primality_test.hpp --->8------

/// @brief ミラーラビン素数判定法によりNが素数であるかを判定する
/// @note O(k log^3 N)
/// @ref https://drken1215.hatenablog.com/entry/2023/05/23/233000
/// @ref verify: https://judge.yosupo.jp/problem/primality_test
bool PrimalityTest(ll N) {
    using lll = __int128_t;
    if(N == 2) return true;
    if(N <= 1 || N % 2 == 0) return false;

    vector<ll> test;
    if(N < 4759123141ll) test = {2, 7, 61};
    else test = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    ll s = 0, d = N - 1;
    while(d % 2 == 0) d >>= 1, s++;

    for(ll a : test) {
        if(a >= N) break;
        lll x = ModPow<lll>(a, d, N);

        if(x == 1 || x == N - 1) continue;
        else {
            for(ll r = 1; r < s; r++) {
                x = x * x % N;
                if(x == 1) return false;
                else if(x == N - 1) break;
            }
        }
        if(x != N - 1) return false;
    }

    return true;
}
//------>8---- end kyopro_library/math/primality_test.hpp ---->8------


/// @brief ポラードのロー法で N を素因数分解する
/// @note O(N^(1/4))
/// @ref https://qiita.com/t_fuki/items/7cd50de54d3c5d063b4a
/// @ref verify: https://algo-method.com/tasks/553
/// @ref verify: https://judge.yosupo.jp/problem/factorize
vector<pair<ll, ll>> PrimeFactorize(ll N) {
    using lll = __int128_t;
    if(PrimalityTest(N)) return {{N, 1}};
    auto find_factor = [](auto&& find_factor, ll N) -> ll {
        lll m = (ll)pow(N, 0.125) + 1;
        auto _gcd = [](lll a, lll b) {
            while(a) b %= a, swap(a, b);
            return b;
        };
        auto _abs = [](lll x) { return x < 0 ? -x : x; };
        for(ll c = 1; c <= N; c++) {
            auto f = [&](lll x) { return ((x % N) * (x % N) + c) % N; };
            lll y = 0, r = 1, q = 1, g = 1, k = 0, x = 0, ys = 0;
            while(g == 1) {
                x = y;
                while(k < r * 3 / 4) y = f(y), k++;
                while(k < r && g == 1) {
                    ys = y;
                    rep(i, min(m, r - k)) y = f(y), q = q * _abs(x - y) % N;
                    g = _gcd(q, N);
                    k += m;
                }
                k = r;
                r *= 2;
            }
            if(g == N) {
                g = 1, y = ys;
                while(g == 1) y = f(y), g = _gcd(_abs(x - y), N);
            }
            if(g < N) {
                if(PrimalityTest(g)) return g;
                if(PrimalityTest(N / g)) return N / g;
                return find_factor(find_factor, g);
            }
        }
        return 0;
    };
    map<ll, ll> mp;
    ll i = 2;
    while(i * i <= N) {
        ll k = 0;
        while(N % i == 0) N /= i, k++;
        if(k) mp[i] = k;
        i += i % 2 + 1;
        if(i == 101 && N >= (1ll << 20)) {
            while(N > 1) {
                if(PrimalityTest(N)) mp[N] = 1, N = 1;
                else {
                    ll j = find_factor(find_factor, N);
                    k = 0;
                    while(N % j == 0) N /= j, k++;
                    mp[j] = k;
                }
            }
        }
    }
    if(N > 1) mp[N] = 1;
    vector<pair<ll, ll>> ret;
    for(auto p : mp) ret.push_back(p);
    sort(all(ret));
    return ret;
}
//------>8--- end kyopro_library/math/prime_factorize.hpp ---->8------

//------>8------ begin kyopro_library/others/xor128.hpp ------>8------

/// @brief 疑似乱数生成

u64 Xor128() {
    static bool flag = false;
    static u64 x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    if(!flag) {
        random_device seedgen;
        w = seedgen();
        flag = true;
    }
    u64 t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

i64 Xor128(i64 n) { return Xor128() % n; }

i64 Xor128(i64 l, i64 r) { return Xor128(r - l) + l; }  //[l,r)

f80 Xor128Prob() { return (f80)Xor128() / (ULLONG_MAX); }
//------>8------- end kyopro_library/others/xor128.hpp ------->8------


/// @brief n の原始根を求める
/// @ref https://37zigen.com/primitive-root/
/// @ref verify: https://judge.yosupo.jp/problem/primitive_root
ll PrimitiveRoot(ll n) {
    if(!PrimalityTest(n)) return -1;
    if(n == 2) return 1;

    auto pf = PrimeFactorize(n - 1);
    while(true) {
        ll i = Xor128(2, n);
        bool ok = true;
        for(auto [p, _] : pf) {
            if(ModPow(i, (n - 1) / p, n) == 1) {
                ok = false;
                break;
            }
        }
        if(ok) return i;
    }
    return -1;
}
//------>8---- end kyopro_library/math/primitive_root.hpp ---->8------

#define TODAY_KYOPRO
#include __FILE__

#endif

