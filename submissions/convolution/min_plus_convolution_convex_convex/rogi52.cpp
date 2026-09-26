#line 1 "a.cpp"

#line 2 "/Users/korogi/Desktop/cp-cpp/template.hpp"
#include <iostream>
#include <queue>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <random>
#include <bit>
#include <array>
#include <cctype>
#include <set>
#include <map>
#include <bitset>
#include <stack>
using namespace std;

using i16 = int16_t;
using u16 = uint16_t;
using i32 = int;
using i64 = long long;
using i128 = __int128;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = unsigned __int128;
using f32 = double;
using f64 = long double;

#define DMP(x) cout << "[" << __LINE__ << "]" << " " << #x << ":" << " " << x << endl;

#define FOR1(n)          for(int _ =  0 , n_ = (n); _ < n_; _++)
#define FOR2(i, n)       for(int i =  0 , n_ = (n); i < n_; i++)
#define FOR3(i, s, t)    for(int i = (s), t_ = (t); i < t_; i++)
#define FOR4(i, s, t, d) for(int i = (s), t_ = (t), d_ = (d); i < t_; i += d_)
#define OVERLOAD4(a, b, c, d, e, ...) e
#define FOR(...) OVERLOAD4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)

#define REV1(n)          for(int _ = (n) - 1; _ >=  0 ; _--)
#define REV2(i, n)       for(int i = (n) - 1; i >=  0 ; i--)
#define REV3(i, s, t)    for(int i = (t) - 1, s_ = (s); i >= s_; i--)
#define REV4(i, s, t, d) for(int i = (t) - 1, s_ = (s), d_ = (d); i >= s_; i -= d_)
#define OVERLOAD3(a, b, c, d, ...) d
#define REV(...) OVERLOAD4(__VA_ARGS__, REV4, REV3, REV2, REV1)(__VA_ARGS__)

#define FOR_SUBSET(T, S) for(int S_ = (S), T = S_; T >= 0; T = (T == 0 ? -1 : (T - 1) & S_))

#define MULTI for(int testcase_ = in(), testcase = 0; testcase < testcase_; testcase++) [&]

template < class T > using heap_max = priority_queue< T, vector< T >, less< T > >;
template < class T > using heap_min = priority_queue< T, vector< T >, greater< T >>;

template < class T, class U > bool chmin(T& a, const U& b) { return a > b ? a = b, 1 : 0; }
template < class T, class U > bool chmax(T& a, const U& b) { return a < b ? a = b, 1 : 0; }

i64 floor_div(const i64 n, const i64 d) { assert(d != 0); return n / d - ((n ^ d) <  0 && n % d != 0); }
i64  ceil_div(const i64 n, const i64 d) { assert(d != 0); return n / d + ((n ^ d) >= 0 && n % d != 0); }

template < class T, class F > T bin_search(T ok, T ng, const F& check) { while((ok > ng ? ok - ng : ng - ok) > 1) { T mid = midpoint(ok, ng); (check(mid) ? ok : ng) = mid; } return ok; }
template < class T, class F > T bin_search_real(T ok, T ng, const F& check, int step = 100) { FOR(step) { T mid = (ok + ng) / 2; (check(mid) ? ok : ng) = mid; } return ok; }

template < class T, class U > T accum(const vector< U >& a) { return accumulate(a.begin(), a.end(), T(0)); }
template < class T > void sort(vector< T >& a) { sort(a.begin(), a.end()); }
template < class T > void rsort(vector< T >& a) { sort(a.rbegin(), a.rend()); }
template < class T > void reverse(vector< T >& a) { reverse(a.begin(), a.end()); }
void sort(string& s) { sort(s.begin(), s.end()); }
void rsort(string& s) { sort(s.rbegin(), s.rend()); }
void reverse(string& s) { reverse(s.begin(), s.end()); }
template < class T, class Cmp > void sort(vector< T >& a, Cmp cmp) { sort(a.begin(), a.end(), cmp); }
template < class T > int LB(const vector< T >& a, T x) { return distance(a.begin(), lower_bound(a.begin(), a.end(), x)); }
template < class T > int UB(const vector< T >& a, T x) { return distance(a.begin(), upper_bound(a.begin(), a.end(), x)); }
template < class T > void unique(vector< T >& a) { sort(a.begin(), a.end()); a.erase(unique(a.begin(), a.end()), a.end()); }
vector<int> iota(int n) { vector<int> a(n); iota(a.begin(), a.end(), 0); return a; }

istream& operator >> (istream& is, i128& x) {
    string s; is >> s;
    int m = (s[0] == '-');
    x = 0;
    FOR(i, m, ssize(s)) x = x * 10 + (s[i] - '0');
    if(m) x *= -1;
    return is;
}
ostream& operator << (ostream& os, const i128& x) {
    if(x == 0) return os << '0';
    i128 y = x; if(y < 0) { os << '-'; y *= -1; }
    vector<int> ny;
    while(y) ny.push_back(y % 10), y /= 10;
    REV(i, ssize(ny)) os << ny[i];
    return os;
}
namespace scan {
struct x0 { template < class T > operator T() { T x; cin >> x; return x; } };
struct x1 { int n; x1(int n) : n(n) {} template < class T > operator vector< T >() { vector< T > a(n); for(T& x : a) cin >> x; return a; } };
struct x2 { int h, w; x2(int h, int w) : h(h), w(w) {} template < class T > operator vector< vector< T > >() { vector m(h, vector< T >(w)); for(vector< T >& a : m) for(T& x : a) cin >> x; return m; } };
struct cppio { cppio() { cin.tie(0); ios::sync_with_stdio(0); } } cppio_instance;
}
scan::x0 in() { return scan::x0(); }
scan::x1 in(int n) { return scan::x1(n); }
scan::x2 in(int h, int w) { return scan::x2(h, w); }

template < class T > ostream& operator << (ostream& os, const vector< T >& a) {
    const int n = a.size();
    FOR(i, n) { os << a[i]; if(i + 1 != n) os << ' '; }
    return os;
}
template < class T > int print_n(const vector< T >& a) { for(const T& x : a) cout << x << '\n'; return 0; }
int print() { cout << '\n'; return 0; }
template < class Head, class... Tail > int print(Head&& h, Tail&&... t) { cout << h; if(sizeof...(Tail)) cout << ' '; return print(forward<Tail>(t)...); }
namespace printer {
    void prec(int n) { cout << fixed << setprecision(n); }
    void flush() { cout.flush(); }
}

vector<int>& operator ++ (vector<int>& a) { for(auto& e : a) e++; return a; }
vector<int>& operator -- (vector<int>& a) { for(auto& e : a) e--; return a; }
vector<int>  operator ++ (vector<int>& a, int) { vector<int> b = a; ++a; return b; }
vector<int>  operator -- (vector<int>& a, int) { vector<int> b = a; --a; return b; }

template < class T > vector<pair< T, int>> RLE(const vector< T >& a) { vector<pair< T, int>> v; for(const T& x : a) { if(not v.empty() and v.back().first == x) v.back().second++; else v.emplace_back(x, 1); } return v; }
vector<pair<char, int>> RLE(const string& s) { vector<pair<char, int>> v; for(const char& c : s) { if(not v.empty() and v.back().first == c) v.back().second++; else v.emplace_back(c, 1); } return v; }
template < class String, class Same > vector<String> RLE(const String& a, const Same same) { vector<String> v; for(const auto& x : a) { if(not v.empty() and same(v.back().back(), x)) v.back().push_back(x); else v.push_back({x}); } return v; }

int YESNO(bool yes) { return print(yes ? "YES" : "NO"); }
int YesNo(bool yes) { return print(yes ? "Yes" : "No"); }
int Yes() { return print("Yes"); }
int No() { return print("No"); }

constexpr i32 INF32 = 1e9;
constexpr i64 INF64 = 1e18;
template < class T > constexpr T infty = 0;
template <> constexpr int infty<int> = 1e9;
template <> constexpr int infty<u32> = 1e9;
template <> constexpr i64 infty<i64> = 1e18;
template <> constexpr u64 infty<u64> = 1e18;

namespace bit {
int pop(int x) { return popcount<u32>(x); }
int pop(u32 x) { return popcount<u32>(x); }
int pop(i64 x) { return popcount<u64>(x); }
int pop(u64 x) { return popcount<u64>(x); }
int parity(int x) { return __builtin_parity(x); }
int parity(u32 x) { return __builtin_parity(x); }
int parity(i64 x) { return __builtin_parityll(x); }
int parity(u64 x) { return __builtin_parityll(x); }
int sgn(int x) { return parity(x) ? -1 : +1; }
int sgn(u32 x) { return parity(x) ? -1 : +1; }
int sgn(i64 x) { return parity(x) ? -1 : +1; }
int sgn(u64 x) { return parity(x) ? -1 : +1; }
int top(int x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
int top(u32 x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
int top(i64 x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int top(u64 x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int low(int x) { return x == 0 ? -1 : __builtin_ctz(x); }
int low(u32 x) { return x == 0 ? -1 : __builtin_ctz(x); }
int low(i64 x) { return x == 0 ? -1 : __builtin_ctzll(x); }
int low(u64 x) { return x == 0 ? -1 : __builtin_ctzll(x); }
int ceil(int x) { return bit_ceil<u32>(x); }
i64 ceil(i64 x) { return bit_ceil<u64>(x); }
int floor(int x) { return bit_floor<u32>(x); }
i64 floor(i64 x) { return bit_floor<u64>(x); }
}

// (-1)^n
int parity_sign(int n) { return n % 2 == 0 ? +1 : -1; }

template < class Key, class Value >
struct key_value {
    Key key;
    Value value;
};
template < class Value > key_value<int, Value> min(const vector<Value>& a) {
    assert(1 <= ssize(a));
    auto itr = min_element(a.begin(), a.end());
    return {static_cast<int>(distance(a.begin(), itr)), *itr};
}
template < class Value > key_value<int, Value> max(const vector<Value>& a) {
    assert(1 <= ssize(a));
    auto itr = max_element(a.begin(), a.end());
    return {static_cast<int>(distance(a.begin(), itr)), *itr};
}

#line 1 "/Users/korogi/Desktop/cp-cpp/util/grid.hpp"
struct grid {
    int H, W;
    grid(int H, int W) : H(H), W(W) {}
    static constexpr pair<int, int> dir4[] = {
                  {-1,  0}, 
        { 0, -1},           { 0, +1}, 
                  {+1,  0}
    };
    static constexpr pair<int, int> dir8[] = {
        {-1, -1}, {-1,  0}, {-1, +1},
        { 0, -1},           { 0, +1},
        {+1, -1}, {+1,  0}, {+1, +1}
    };
    bool contains(int i, int j) const {
        return 0 <= i and i < H and 0 <= j and j < W;
    }
    template < class F > 
    void for_each_dir4(int i, int j, const F& f) const {
        for(const auto [di, dj] : dir4) {
            const int ni = i + di, nj = j + dj;
            if(contains(ni, nj)) f(ni, nj);
        }
    }
    template < class F >
    void for_each_dir8(int i, int j, const F& f) const {
        for(const auto [di, dj] : dir8) {
            const int ni = i + di, nj = j + dj;
            if(contains(ni, nj)) f(ni, nj);
        }
    }
};
#line 1 "/Users/korogi/Desktop/cp-cpp/util/imos.hpp"
template < class Sum > struct psum1D {
    int n;
    vector<Sum> s;
    psum1D() : n(0), s(1, Sum()) {}
    template < class Value >
    psum1D(const vector<Value>& a) : n(ssize(a)), s(n + 1, Sum()) {
        FOR(i, n) s[i + 1] = s[i] + static_cast<Sum>(a[i]);
    }
    // [l, r)
    Sum v(int l, int r) const {
        assert(0 <= l and l <= r and r <= n);
        return s[r] - s[l];
    }
    void push_back(const Sum& x) {
        s.push_back(s.back() + x);
        n += 1;
    }
};
template < class Value > struct psum2D {
    int H, W;
    vector<vector<Value>> A;
    bool built;
    psum2D(int H, int W) : H(H), W(W), A(H + 1, vector(W + 1, Value(0))), built(false) {}
    // A[x][y] += v
    void add(int x, int y, Value v) {
        assert(not built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        A[x + 1][y + 1] += v;
    }
    void build() {
        FOR(x, H) FOR(y, W + 1) A[x + 1][y] += A[x][y];
        FOR(x, H + 1) FOR(y, W) A[x][y + 1] += A[x][y];
        built = true;
    }
    // [xL, xR) * [yL, yR)
    Value sum(int xL, int xR, int yL, int yR) {
        assert(built);
        assert(0 <= xL and xL <= xR and xR <= H);
        assert(0 <= yL and yL <= yR and yR <= W);
        return A[xR][yR] - A[xR][yL] - A[xL][yR] + A[xL][yL];
    }
    Value get(int x, int y) {
        assert(built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        return sum(x, x + 1, y, y + 1);
    }
};
template < class Value > struct imos2D {
    int H, W;
    vector<vector<Value>> A;
    bool built;
    imos2D(int H, int W) : H(H), W(W), A(H + 1, vector(W + 1, Value(0))), built(false) {}
    void add(int xL, int xR, int yL, int yR, Value v) {
        assert(not built);
        assert(0 <= xL and xL <= xR and xR <= H);
        assert(0 <= yL and yL <= yR and yR <= W);
        A[xL][yL] += v;
        A[xR][yL] -= v;
        A[xL][yR] -= v;
        A[xR][yR] += v;
    }
    void build() {
        assert(not built);
        FOR(i, H + 1) FOR(j, W) A[i][j + 1] += A[i][j];
        FOR(i, H) FOR(j, W + 1) A[i + 1][j] += A[i][j];
        built = true;
    }
    Value get(int x, int y) {
        assert(built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        return A[x][y];
    }
};
#line 3 "/Users/korogi/Desktop/cp-cpp/util/rnd.hpp"

namespace rnd {
    u32 seed; mt19937 mt;
    struct gen_seed { gen_seed() { seed = random_device()(); mt = mt19937(seed); } } gen_seed_instance;
    // [L, R)
    template < class Int > Int i(Int L, Int R) { assert(L < R); return uniform_int_distribution<Int>(L, R - 1)(mt); }
    template < class Real > Real r(Real L, Real R) { assert(L <= R); return uniform_real_distribution<Real>(L, R)(mt); }
}

template < int n, array<u32, n> mod > struct hash_vector {
    array<u32, n> a;
    using hvec = hash_vector;
    hvec& s(array<u32, n> a) { FOR(i, n) this->a[i] = a[i] < mod[i] ? a[i] : a[i] - mod[i]; return *this; }
    hash_vector(u32 v = 0) { FOR(i, n) a[i] = v % mod[i] + mod[i]; s(a); }
    hvec operator - () const { return hvec() - *this; }
    hvec& operator += (const hvec& r) { FOR(i, n) a[i] += r.a[i]; return s(a); }
    hvec& operator -= (const hvec& r) { FOR(i, n) a[i] += mod[i] - r.a[i]; return s(a); }
    hvec& operator *= (const hvec& r) { FOR(i, n) a[i] = u64(a[i]) * r.a[i] % mod[i]; return *this; }
    hvec& operator /= (const hvec& r) { return *this *= inv(r); }
    hvec operator + (const hvec& r) const { return hvec(*this) += r; }
    hvec operator - (const hvec& r) const { return hvec(*this) -= r; }
    hvec operator * (const hvec& r) const { return hvec(*this) *= r; }
    hvec operator / (const hvec& r) const { return hvec(*this) /= r; }
    bool operator == (const hvec& r) const { return a == r.a; }
    bool operator != (const hvec& r) const { return a != r.a; }
    bool operator < (const hvec& r) const { return a < r.a; }
};
template < int n, array<u32, n> mod > hash_vector<n, mod> pow(hash_vector<n, mod> x, u64 m) {
    hash_vector<n, mod> p(1);
    for(; m; m >>= 1) { if(m & 1) p *= x; x *= x; }
    return p;
}
template < int n, array<u32, n> mod > hash_vector<n, mod> inv(hash_vector<n, mod> x) {
    hash_vector<n, mod> res;
    FOR(i, n) {
        u32 a = x.a[i], b = mod[i], u = 1, v = 0;
        while(b) { u32 t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
        res[i] = u;
    }
    return res;
}
template < int n, array<u32, n> mod > ostream& operator << (ostream& os, const hash_vector< n, mod >& x) { FOR(i, n) { if(i) os << ' '; os << x.a[i]; } return os; }
using hvec1 = hash_vector< 1, array<u32, 1>{999999937} >;
using hvec2 = hash_vector< 2, array<u32, 2>{999999937, 1000000007} >;
using hvec3 = hash_vector< 3, array<u32, 3>{999999937, 1000000007, 1000000009} >;
using hvec4 = hash_vector< 4, array<u32, 4>{999999937, 1000000007, 1000000009, 1000000021} >;
#line 186 "/Users/korogi/Desktop/cp-cpp/template.hpp"

namespace r52 {
int abs(int x) { return x >= 0 ? x : -x; }
i64 abs(i64 x) { return x >= 0 ? x : -x; }
i128 abs(i128 x) { return x >= 0 ? x : -x; }
}
#line 1 "/Users/korogi/Desktop/cp-cpp/fastio.hpp"
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#line 6 "/Users/korogi/Desktop/cp-cpp/fastio.hpp"

// [LC: Many A+B]
// https://judge.yosupo.jp/problem/many_aplusb
// https://judge.yosupo.jp/submission/364363

// [LC: Many A+B (128bit)]
// https://judge.yosupo.jp/problem/many_aplusb_128bit
// https://judge.yosupo.jp/submission/364365

namespace fastio {
    template<typename T> struct is_vector : std::false_type {};
    template<typename T, typename Alloc> struct is_vector<std::vector<T, Alloc>> : std::true_type {};

    struct Reader {
        char *p, *l, *r;
        bool mmap_success;
        Reader() : p(nullptr), l(nullptr), r(nullptr), mmap_success(false) {
            struct stat st;
            if (fstat(STDIN_FILENO, &st) == 0 && st.st_size > 0) {
                l = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
                if (l != MAP_FAILED) {
                    p = l; r = l + st.st_size;
                    mmap_success = true;
                } else { l = nullptr; }
            }
        }
        ~Reader() { if (l) munmap(l, r - l); }

        inline void skip_space() { while (p < r && *p <= ' ') p++; }
        
        static constexpr bool all_digit(u64 x) {
            return !((x ^ 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0);
        }

        static constexpr u64 parse_8_digits(u64 val) {
            val ^= 0x3030303030303030;
            val = (val * 10 + (val >> 8)) & 0x00ff00ff00ff00ff;
            val = (val * 100 + (val >> 16)) & 0x0000ffff0000ffff;
            return (val * 10000 + (val >> 32)) & 0x00000000ffffffff;
        }
    } reader_internal;

    struct Table {
        uint32_t data[10000];
        constexpr Table() : data{} {
            for(int i = 0; i < 10000; ++i) {
                uint32_t d0 = (i / 1000) + '0';
                uint32_t d1 = (i / 100 % 10) + '0';
                uint32_t d2 = (i / 10 % 10) + '0';
                uint32_t d3 = (i % 10) + '0';
                data[i] = d0 | (d1 << 8) | (d2 << 16) | (d3 << 24);
            }
        }
    };
    inline constexpr Table table{};

    struct Writer {
        static constexpr int BUF_SIZE = 1 << 20;
        char buf[BUF_SIZE];
        char *p = buf;

        ~Writer() { flush(); }
        inline void flush() {
            if (p > buf) {
                [[maybe_unused]] auto res = write(STDOUT_FILENO, buf, p - buf);
                p = buf;
            }
        }
        inline void write_char(char c) {
            if (p > buf + BUF_SIZE - 1) flush();
            *p++ = c;
        }
        inline void write_str(const char* s, size_t len) {
            if (p + len > buf + BUF_SIZE) flush();
            if (len >= BUF_SIZE) { [[maybe_unused]] auto res = write(STDOUT_FILENO, s, len); } 
            else { std::memcpy(p, s, len); p += len; }
        }
        
        template <typename U>
        inline void write_unsigned(U x) {
            if (p > buf + BUF_SIZE - 40) flush();
            if (x == 0) { *p++ = '0'; return; }
            
            char temp[40];
            char* q = temp + 40;
            
            if constexpr (std::is_same_v<U, u128>) {
                while (x > static_cast<u128>(UINT64_MAX)) {
                    q -= 4;
                    u64 rem = static_cast<u64>(x % 10000);
                    std::memcpy(q, &table.data[rem], 4);
                    x /= 10000;
                }
            }
            
            u64 y = static_cast<u64>(x);
            if (y >= 10000000000000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            } else if (y >= 1000000000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            } else if (y >= 100000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            }
            while (y >= 10000) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            }
            
            if (y >= 1000) {
                q -= 4; std::memcpy(q, &table.data[y], 4);
            } else if (y >= 100) {
                *--q = (y % 10) + '0'; y /= 10;
                *--q = (y % 10) + '0'; y /= 10;
                *--q = y + '0';
            } else if (y >= 10) {
                *--q = (y % 10) + '0';
                *--q = (y / 10) + '0';
            } else {
                *--q = y + '0';
            }
            
            int len = (temp + 40) - q;
            std::memcpy(p, q, len);
            p += len;
        }
    } writer_internal;

    template <typename T>
    inline void scan_single(T& x) {
        if (!reader_internal.mmap_success) {
            std::cin >> x;
            return;
        }
        if constexpr (std::is_same_v<T, char>) {
            reader_internal.skip_space();
            if (reader_internal.p < reader_internal.r) x = *reader_internal.p++;
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            x = 0; reader_internal.skip_space();
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    x = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    x = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    x = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { x = x * 10 + (*reader_internal.p++ - '0'); }
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            x = 0; reader_internal.skip_space();
            bool neg = false; if (*reader_internal.p == '-') { neg = true; reader_internal.p++; }
            using U = std::conditional_t<std::is_same_v<T, i128>, u128, std::make_unsigned_t<T>>;
            U ux = 0;
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    ux = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { ux = ux * 10 + (*reader_internal.p++ - '0'); }
            x = neg ? -static_cast<T>(ux) : static_cast<T>(ux);
        } else if constexpr (std::is_same_v<T, u128> || std::is_same_v<T, i128>) {
            x = 0; reader_internal.skip_space();
            bool neg = false; 
            if constexpr (std::is_same_v<T, i128>) {
                if (*reader_internal.p == '-') { neg = true; reader_internal.p++; }
            }
            u128 ux = 0;
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    ux = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { ux = ux * 10 + (*reader_internal.p++ - '0'); }
            x = neg ? -static_cast<T>(ux) : static_cast<T>(ux);
        } else if constexpr (std::is_same_v<T, std::string>) {
            x.clear(); reader_internal.skip_space();
            while (*reader_internal.p > ' ') { x += *reader_internal.p++; }
        }
    }

    template <typename... Args> inline void scan(Args&... args) { (scan_single(args), ...); }

    template <typename T>
    inline void print_single(const T& x) {
        if constexpr (std::is_same_v<T, char>) {
            writer_internal.write_char(x);
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            writer_internal.write_unsigned(x);
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            using U = std::make_unsigned_t<T>;
            if (x < 0) { writer_internal.write_char('-'); writer_internal.write_unsigned(static_cast<U>(~static_cast<U>(x) + 1)); } 
            else { writer_internal.write_unsigned(static_cast<U>(x)); }
        } else if constexpr (std::is_same_v<T, u128>) {
            writer_internal.write_unsigned(x);
        } else if constexpr (std::is_same_v<T, i128>) {
            if (x < 0) { writer_internal.write_char('-'); writer_internal.write_unsigned(static_cast<u128>(~static_cast<u128>(x) + 1)); } 
            else { writer_internal.write_unsigned(static_cast<u128>(x)); }
        } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            writer_internal.write_str(x, std::strlen(x));
        } else if constexpr (std::is_same_v<T, std::string>) {
            writer_internal.write_str(x.c_str(), x.size());
        } else if constexpr (is_vector<T>::value) {
            for (size_t i = 0; i < x.size(); ++i) {
                if (i > 0) writer_internal.write_char(' ');
                print_single(x[i]);
            }
        }
    }

    inline void print() {}
    template <typename Head, typename... Tail>
    inline void print(const Head& h, const Tail&... t) {
        print_single(h);
        if constexpr (sizeof...(t) > 0) {
            writer_internal.write_char(' ');
            print(t...);
        }
    }
    template <typename... Args>
    inline void println(const Args&... args) {
        print(args...);
        writer_internal.write_char('\n');
    }
    
    inline void flush() { writer_internal.flush(); }
} // namespace fastio
#line 2 "/Users/korogi/Desktop/cp-cpp/dp/min-plus_convolution_convex_convex.hpp"

template < class T > 
vector< T > min_plus_convolution_convex_convex(const vector< T >& A, const vector< T >& B) {
    const int N = ssize(A);
    const int M = ssize(B);
    assert(1 <= N);
    assert(1 <= M);
    vector< T > C(N + M - 1);
    int i = 0, j = 0;
    FOR(k, N + M - 1) {
        C[k] = A[i] + B[j];
        if(j == M - 1 or (i + 1 < N and A[i + 1] + B[j] <= A[i] + B[j + 1])) {
            i += 1;
        } else {
            j += 1;
        }
    }
    return C;
}
#line 5 "a.cpp"

int main() {
    u32 N, M;
    fastio::scan(N, M);
    vector<u32> A(N), B(M);
    FOR(i, N) fastio::scan(A[i]);
    FOR(j, M) fastio::scan(B[j]);
    fastio::println(min_plus_convolution_convex_convex(A, B));
}
