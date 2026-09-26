#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

// clang-format off
#if defined(_MSC_VER) || (defined(__clang__) && defined(_WIN32))

#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else

#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
#ifndef ONLINE_JUDGE

#define LOCAL
#endif
#include <bits/stdc++.h>
using namespace std;
struct FastIO;
namespace FastIODetail {
[[gnu::always_inline]] inline size_t fread_fast(void *ptr, size_t size, size_t count, FILE *stream) {
#if defined(_WIN32) || defined(_WIN64)
return fread(ptr, size, count, stream);
#else
return fread_unlocked(ptr, size, count, stream);
#endif
}
[[gnu::always_inline]] inline size_t fwrite_fast(const void *ptr, size_t size, size_t count, FILE *stream) {
#if defined(_WIN32) || defined(_WIN64)
return fwrite(ptr, size, count, stream);
#else
return fwrite_unlocked(ptr, size, count, stream);
#endif
}
template<class T>
concept ADLReadable= requires(FastIO &io, T &x) {
{ fastio_read(io, x) } -> convertible_to<bool>;
};
template<class T>
concept ADLWritable= requires(FastIO &io, const T &x) { fastio_write(io, x); };
}
/**
 * @brief 高速入出力
 * @note `read` は空白区切り、`print` は末尾改行つき
 */
struct FastIO {
static constexpr size_t INPUT_BUFFER_SIZE= 1 << 22;
static constexpr size_t OUTPUT_BUFFER_SIZE= 1 << 22;
char input_buffer[INPUT_BUFFER_SIZE + 1];
char output_buffer[OUTPUT_BUFFER_SIZE];
char *input_ptr= input_buffer;
char *input_end= input_buffer;
char *output_ptr= output_buffer;

FastIO()= default;

~FastIO() { flush(); }

[[gnu::always_inline]] inline void load() {
const size_t len= FastIODetail::fread_fast(input_buffer, 1, INPUT_BUFFER_SIZE, stdin);
input_ptr= input_buffer;
input_end= input_buffer + len;
*input_end= '\0';
}

[[gnu::always_inline]] inline char get_char() {
if(__builtin_expect(input_ptr == input_end, 0)) { load(); }
return *input_ptr++;
}

[[gnu::always_inline]] inline char skip_blanks() {
char c= get_char();
while(c <= ' ') {
if(c == '\0') { return '\0'; }
c= get_char();
}
return c;
}

[[gnu::always_inline]] inline void flush() {
const size_t len= static_cast<size_t>(output_ptr - output_buffer);
if(len) {
FastIODetail::fwrite_fast(output_buffer, 1, len, stdout);
output_ptr= output_buffer;
}
}

[[gnu::always_inline]] inline void put_char(char c) {
if(__builtin_expect(output_ptr == output_buffer + OUTPUT_BUFFER_SIZE, 0)) { flush(); }
*output_ptr++= c;
}
/**
 * @brief 生バイト列を出力
 * @param s 先頭ポインタ
 * @param n バイト数
 */
[[gnu::always_inline]] inline void write_bytes(const char *s, size_t n) {
const size_t rem= static_cast<size_t>(output_buffer + OUTPUT_BUFFER_SIZE - output_ptr);
if(n <= rem) {
memcpy(output_ptr, s, n);
output_ptr+= n;
return;
}
flush();
if(n >= OUTPUT_BUFFER_SIZE) {
FastIODetail::fwrite_fast(s, 1, n, stdout);
return;
}
memcpy(output_ptr, s, n);
output_ptr+= n;
}
template<class T>
requires(std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)
/**
 * @brief 整数を読み込む
 * @param x 読み込み先
 * @return 読み込み成功なら `true`
 * @complexity 入力長に比例
 */
[[gnu::always_inline]] bool read(T &x) {
using U= std::make_unsigned_t<T>;
char c= skip_blanks();
if(c == '\0') return false;
bool neg= false;
if constexpr(std::signed_integral<T>) {
if(c == '-') {
neg= true;
c= get_char();
}
}
U val= 0;
while(static_cast<unsigned>(c - '0') < 10) {
val= val * 10 + static_cast<U>(c - '0');
c= get_char();
}
if constexpr(std::signed_integral<T>) {
x= neg ? static_cast<T>(U(0) - val) : static_cast<T>(val);
} else {
x= static_cast<T>(val);
}
return true;
}
/**
 * @brief `bool` を読み込む
 * @param x 読み込み先
 * @return 読み込み成功なら `true`
 */
[[gnu::always_inline]] bool read(bool &x) {
string s;
if(!read(s)) return false;
if(s == "true" || s == "True" || s == "TRUE") {
x= true;
return true;
}
if(s == "false" || s == "False" || s == "FALSE") {
x= false;
return true;
}
char *end= nullptr;
long long v= strtoll(s.c_str(), &end, 10);
if(end == s.c_str() || *end != '\0') return false;
x= static_cast<bool>(v);
return true;
}
/**
 * @brief 1 文字を読み込む
 * @param c 読み込み先
 * @return 読み込み成功なら `true`
 */
[[gnu::always_inline]] bool read(char &c) {
c= skip_blanks();
return c != '\0';
}
/**
 * @brief 文字列を読み込む
 * @param s 読み込み先
 * @return 読み込み成功なら `true`
 */
bool read(string &s) {
char c= skip_blanks();
if(c == '\0') return false;
s.clear();
while(c > ' ') {
s.push_back(c);
c= get_char();
}
return true;
}

bool read(float &x) {
string s;
if(!read(s)) return false;
x= strtof(s.c_str(), nullptr);
return true;
}

bool read(double &x) {
string s;
if(!read(s)) return false;
x= strtod(s.c_str(), nullptr);
return true;
}

bool read(long double &x) {
string s;
if(!read(s)) return false;
x= strtold(s.c_str(), nullptr);
return true;
}
/**
 * @brief ADL `fastio_read` 経由で読み込む
 * @tparam T 対応型
 * @param x 読み込み先
 * @return 読み込み成功なら `true`
 */
template<FastIODetail::ADLReadable T> [[gnu::always_inline]] bool read(T &x) { return static_cast<bool>(fastio_read(*this, x)); }
static constexpr char digit_pairs[201]= "00010203040506070809"
                                        "10111213141516171819"
                                        "20212223242526272829"
                                        "30313233343536373839"
                                        "40414243444546474849"
                                        "50515253545556575859"
                                        "60616263646566676869"
                                        "70717273747576777879"
                                        "80818283848586878889"
                                        "90919293949596979899";
template<class T>
requires(std::unsigned_integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)
/**
 * @brief 符号なし整数を出力
 * @param x 出力値
 * @complexity 桁数に比例
 */
[[gnu::always_inline]] void write(T x) {
char s[std::numeric_limits<T>::digits10 + 3];
char *p= s + sizeof(s);
while(x >= 100) {
auto q= x / 100;
auto r= static_cast<unsigned>((x - q * 100) * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
x= q;
}
if(x < 10) {
*--p= static_cast<char>('0' + x);
} else {
auto r= static_cast<unsigned>(x * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
}
write_bytes(p, static_cast<size_t>(s + sizeof(s) - p));
}
template<class T>
requires(std::signed_integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)
/**
 * @brief 符号付き整数を出力
 * @param x 出力値
 * @complexity 桁数に比例
 */
[[gnu::always_inline]] void write(T x) {
using U= std::make_unsigned_t<T>;
U v= static_cast<U>(x);
if(x < 0) {
put_char('-');
v= U(0) - v;
}
write(v);
}
template<class T>
requires(std::unsigned_integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)

[[gnu::always_inline]] void writeln_int(T x) {
char s[std::numeric_limits<T>::digits10 + 4];
char *p= s + sizeof(s);
*--p= '\n';
while(x >= 100) {
auto q= x / 100;
auto r= static_cast<unsigned>((x - q * 100) * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
x= q;
}
if(x < 10) {
*--p= static_cast<char>('0' + x);
} else {
auto r= static_cast<unsigned>(x * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
}
write_bytes(p, static_cast<size_t>(s + sizeof(s) - p));
}
template<class T>
requires(std::signed_integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)

[[gnu::always_inline]] void writeln_int(T x) {
using U= std::make_unsigned_t<T>;
char s[std::numeric_limits<U>::digits10 + 5];
char *p= s + sizeof(s);
*--p= '\n';
U v= static_cast<U>(x);
if(x < 0) { v= U(0) - v; }
while(v >= 100) {
auto q= v / 100;
auto r= static_cast<unsigned>((v - q * 100) * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
v= q;
}
if(v < 10) {
*--p= static_cast<char>('0' + v);
} else {
auto r= static_cast<unsigned>(v * 2);
p-= 2;
p[0]= digit_pairs[r];
p[1]= digit_pairs[r + 1];
}
if(x < 0) { *--p= '-'; }
write_bytes(p, static_cast<size_t>(s + sizeof(s) - p));
}

[[gnu::always_inline]] void write(char c) { put_char(c); }

[[gnu::always_inline]] void write(const char *s) { write_bytes(s, strlen(s)); }

[[gnu::always_inline]] void write(string_view s) { write_bytes(s.data(), s.size()); }

[[gnu::always_inline]] void write(const string &s) { write_bytes(s.data(), s.size()); }

[[gnu::always_inline]] void write(bool x) { write(static_cast<int>(x)); }

void write(float x) {
char s[64];
const int n= snprintf(s, sizeof(s), "%.15f", static_cast<double>(x));
write_bytes(s, static_cast<size_t>(n));
}

void write(double x) {
char s[64];
const int n= snprintf(s, sizeof(s), "%.15f", x);
write_bytes(s, static_cast<size_t>(n));
}

void write(long double x) {
char s[80];
const int n= snprintf(s, sizeof(s), "%.15Lf", x);
write_bytes(s, static_cast<size_t>(n));
}
/**
 * @brief ADL `fastio_write` 経由で出力
 * @tparam T 対応型
 * @param x 出力値
 */
template<FastIODetail::ADLWritable T> [[gnu::always_inline]] void write(const T &x) { fastio_write(*this, x); }
/**
 * @brief 値を出力して改行
 * @tparam T 出力型
 * @param x 出力値
 */
template<class T> [[gnu::always_inline]] void writeln(const T &x) {
write(x);
put_char('\n');
}
};
inline FastIO io;
namespace FastIOImpl {
template<class T>
concept WritableRange= ranges::range<T> && !same_as<decay_t<T>, string> && !same_as<decay_t<T>, string_view>;
template<class T> [[gnu::always_inline]] void write_one(const T &x);
template<class T, class U> [[gnu::always_inline]] void write_one(const pair<T, U> &p) {
write_one(p.first);
io.write(' ');
write_one(p.second);
}
template<WritableRange R> void write_one(const R &r) {
bool first= true;
for(const auto &x: r) {
if(!first) io.write(' ');
first= false;
write_one(x);
}
}
template<class T> [[gnu::always_inline]] inline void write_one(const T &x) { io.write(x); }
}


inline void print() { io.write('\n'); }
template<class T>
requires(std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>)
/**
 * @brief 整数を 1 つ出力して改行
 * @param x 出力値
 */
inline void print(const T &x) {
io.writeln_int(x);
}

inline void print(char x) {
io.write(x);
io.write('\n');
}

inline void print(bool x) { io.writeln_int(static_cast<int>(x)); }

inline void print(const char *s) {
io.write(s);
io.write('\n');
}

inline void print(string_view s) {
io.write(s);
io.write('\n');
}

inline void print(const string &s) {
io.write(s);
io.write('\n');
}

inline void print(float x) { io.writeln(x); }

inline void print(double x) { io.writeln(x); }

inline void print(long double x) { io.writeln(x); }
/**
 * @brief 複数値を空白区切りで出力して改行
 * @param head 先頭要素
 * @param tail 残り要素
 */
template<class Head, class... Tail> inline void print(const Head &head, const Tail &...tail) {
FastIOImpl::write_one(head);
((io.write(' '), FastIOImpl::write_one(tail)), ...);
io.write('\n');
}
/**
 * @brief `Yes` / `No` を出力
 * @param cond 条件
 * @return `cond`
 */
inline bool Yes(bool cond= true) {
print(cond ? "Yes" : "No");
return cond;
}
/**
 * @brief `YES` / `NO` を出力
 * @param cond 条件
 * @return `cond`
 */
inline bool YES(bool cond= true) {
print(cond ? "YES" : "NO");
return cond;
}

namespace SimpleI {
template<class T>
concept DirectlyReadable= requires(T &t) {
{ io.read(t) } -> convertible_to<bool>;
} && (!ranges::range<T> || is_same_v<T, string>);
template<class T>
concept ReadableRange= ranges::range<T> && !is_same_v<T, string>;
template<class T>
concept TupleLike= requires { typename tuple_size<T>::type; } && !ranges::range<T>;
template<class T> void read(T &x);
/**
 * @brief 直接読み込める型を読む
 * @tparam T 入力型
 * @param x 読み込み先
 */
template<DirectlyReadable T> void read(T &x) { (void)static_cast<bool>(io.read(x)); }
/**
 * @brief `pair` を読む
 * @param p 読み込み先
 * @complexity 各要素の読込に依存
 */
template<class T, class U> void read(pair<T, U> &p) {
read(p.first);
read(p.second);
}

template<TupleLike T, size_t... Is> void read_tuple_impl(T &t, index_sequence<Is...>) { (read(get<Is>(t)), ...); }
/**
 * @brief tuple-like を読む
 * @param t 読み込み先
 */
template<TupleLike T> void read(T &t) { read_tuple_impl(t, make_index_sequence<tuple_size_v<T>>{}); }
/**
 * @brief range 全体を読む
 * @param r 読み込み先
 */
template<ReadableRange R> void read(R &r) {
for(auto &x: r) read(x);
}
template<class T, class... Ts>
requires(sizeof...(Ts) > 0)
/**
 * @brief 複数変数を順に読む
 * @param first 先頭
 * @param rest 残り
 */
void read(T &first, Ts &...rest) {
read(first);
read(rest...);
}
/**
 * @brief 型 `T` を 1 つ読んで返す
 * @tparam T 読み込む型
 * @return 入力値
 */
template<class T> [[nodiscard]] T in() {
T x;
read(x);
return x;
}
/**
 * @brief 入力済み多次元 vector を構築
 * @tparam T 要素型
 * @param first 第 1 次元サイズ
 * @param rest 残り次元サイズ
 * @return 入力済み vector
 */
template<class T, class... Sizes> auto make_vec(size_t first, Sizes... rest) {
if constexpr(sizeof...(rest) == 0) {
vector<T> v(first);
read(v);
return v;
} else {
vector<decltype(make_vec<T>(rest...))> v;
v.reserve(first);
for(size_t i= 0; i < first; i++) v.push_back(make_vec<T>(rest...));
return v;
}
}
/**
 * @brief 列ごとの vector へ `n` 行入力する
 * @param n 行数
 * @param vecs 列ベクトル群
 * @complexity 入力サイズに比例
 */
template<class... Ts> void read_cols(size_t n, vector<Ts> &...vecs) {
(vecs.resize(n), ...);
for(size_t i= 0; i < n; i++) read(vecs[i]...);
}
/**
 * @brief 1 次元 vector を読みつつ各要素へ処理をかける
 * @tparam T 要素型
 * @tparam F コールバック型
 * @param n 要素数
 * @param func 各要素読込後に呼ばれる関数
 * @return 入力済み vector
 */
template<class T, class F> auto make_vec_proc(size_t n, F func) {
vector<T> v(n);
for(size_t i= 0; i < n; i++) {
read(v[i]);
func(v[i], i);
}
return v;
}

template<class T> auto make_vec_nd(size_t n) { return vector<T>(n); }

template<class T, class... Args> auto make_vec_nd(size_t n, Args... args) { return vector<decltype(make_vec_nd<T>(args...))>(n, make_vec_nd<T>(args...)); }
/**
 * @brief 多次元 vector 入力後に各末端要素へ処理をかける補助構造
 * @tparam V 保持する vector 型
 * @tparam BaseType 末端要素型
 */
template<class V, class BaseType> struct VecRunner {
V v;
/**
 * @brief 各末端要素を読み込みつつ関数を適用
 * @param f `(要素, 座標配列)` を受け取る関数
 * @return 入力済み vector
 */
template<class F> V operator|(F &&f) {
auto dfs= [&](auto &&self, auto &vec, auto... coords) -> void {
if constexpr(is_same_v<decay_t<decltype(vec)>, decay_t<BaseType>>) {
read(vec);
std::array<size_t, sizeof...(coords)> p{static_cast<size_t>(coords)...};
f(vec, p);
} else {
for(size_t i= 0; i < vec.size(); ++i) { self(vec[i], coords..., i); }
}
};
for(size_t i= 0; i < v.size(); ++i) { dfs(dfs, v[i], i); }
return v;
}
};
/**
 * @brief `VecRunner` を生成
 * @param v 対象 vector
 * @return `VecRunner`
 */
template<class BaseType, class V> auto make_runner(V &&v) { return VecRunner<decay_t<V>, decay_t<BaseType>>{std::forward<V>(v)}; }
}


#define def(type, ...) type __VA_ARGS__; SimpleI::read(__VA_ARGS__)

#define defv(type, name, ...) auto name = SimpleI::make_vec<type>(__VA_ARGS__)

#define defvp(type, name, ...) auto name = SimpleI::make_runner<type>(SimpleI::make_vec_nd<type>(__VA_ARGS__)) | [&](type &e, auto p)

#define defc(type, n, ...) vector<type> __VA_ARGS__; SimpleI::read_cols(n, __VA_ARGS__)

#define defmv(type, n, ...) vector<type> __VA_ARGS__; SimpleI::read_cols(n, __VA_ARGS__)

#define defmvp(type, n, ...) vector<type> __VA_ARGS__; SimpleI::read_cols(n, __VA_ARGS__); for (long long i = 0; i < (long long)(n); ++i)

#define input(...) SimpleI::read(__VA_ARGS__)

#include <bits/stdc++.h>
using namespace std;
using ui= unsigned int;
using ul= unsigned long;
using ll= long long;
using ull= unsigned long long;
using ld= long double;
using i128= __int128_t;
using u128= __uint128_t;
using u8= uint8_t;
using pii= pair<int, int>;
using pll= pair<ll, ll>;

template<class T> using pqg= priority_queue<T, vector<T>, greater<T>>;
/**
 * @brief `__int128_t` 入力
 * @param is 入力 stream
 * @param v 読み込み先
 * @return `is`
 */
inline istream &operator>>(istream &is, i128 &v) {
string s;
is >> s;
v= 0;
bool neg= false;
for(char c: s) {
if(c == '-') neg= true;
else
v= v * 10 + (c - '0');
}
if(neg) v= -v;
return is;
}
/**
 * @brief `__int128_t` 出力
 * @param os 出力 stream
 * @param v 出力値
 * @return `os`
 */
inline ostream &operator<<(ostream &os, i128 v) {
if(v == 0) return os << "0";
if(v < 0) {
os << '-';
v= -v;
}
string s;
while(v) {
s+= (char)(v % 10 + '0');
v/= 10;
}
ranges::reverse(s);
return os << s;
}
/**
 * @brief `__uint128_t` 入力
 * @param is 入力 stream
 * @param v 読み込み先
 * @return `is`
 */
inline istream &operator>>(istream &is, u128 &v) {
string s;
is >> s;
v= 0;
for(char c: s) v= v * 10 + (c - '0');
return is;
}
/**
 * @brief `__uint128_t` 出力
 * @param os 出力 stream
 * @param v 出力値
 * @return `os`
 */
inline ostream &operator<<(ostream &os, u128 v) {
if(v == 0) return os << "0";
string s;
while(v) {
s+= (char)(v % 10 + '0');
v/= 10;
}
ranges::reverse(s);
return os << s;
}

#include <bits/stdc++.h>
using namespace std;
namespace UtilInternal {
template<typename... Ts> using last_type_t= std::tuple_element_t<sizeof...(Ts) - 1, std::tuple<Ts...>>;
template<typename T, typename U> vector<T> make_v_impl(size_t a, U b) { return vector<T>(a, static_cast<T>(b)); }
template<typename T, typename... Ts> auto make_v_impl(size_t a, Ts... ts) { return vector<decltype(make_v_impl<T>(ts...))>(a, make_v_impl<T>(ts...)); }
}

/**
 * @brief 多次元 vector を構築
 * @tparam T 要素型
 * @param a 第 1 次元サイズ
 * @param ts 残り次元サイズまたは初期値
 * @return 構築した vector
 */
template<typename T= void, typename... Ts> auto make_v(size_t a, Ts... ts) {
if constexpr(std::is_void_v<T>) {
return UtilInternal::make_v_impl<UtilInternal::last_type_t<Ts...>>(a, ts...);
} else {
return UtilInternal::make_v_impl<T>(a, ts...);
}
}
/**
 * @brief vector を空白区切りで出力
 * @param os 出力 stream
 * @param v 出力対象
 * @return `os`
 */
template<typename T> ostream &operator<<(ostream &os, const vector<T> &v) {
for(size_t i= 0; i < v.size(); ++i) { os << v[i] << (i + 1 == v.size() ? "" : " "); }
return os;
}
/**
 * @brief pair を空白区切りで出力
 * @param os 出力 stream
 * @param p 出力対象
 * @return `os`
 */
template<typename T, typename U> ostream &operator<<(ostream &os, const pair<T, U> &p) { return os << p.first << " " << p.second; }

#define reps(i, a, n) for (int i = (a); i < (int)(n); i++)

#define rep(i, n) reps(i, 0, n)

#define rrep(i, n) reps(i, 1, (n) + 1)

#define repds(i, a, n) for (int i = (a) - 1; i >= (int)(n); i--)

#define repd(i, n) repds(i, n, 0)

#define rrepd(i, n) repds(i, (n) + 1, 1)

#define repe(i, a) for (auto &&i : a)

#define loop(n) for (auto _ : views::iota(0, (n)))

#define iloop while (1)

#define elif else if

#define all(x) (x).begin(), (x).end()

#define rall(x) (x).rbegin(), (x).rend()

#define SORT(x) ranges::sort(x)

#define RSORT(x) ranges::sort(x, greater{})

#define MIN(x) *ranges::min_element(x)

#define MAX(x) *ranges::max_element(x)

#define SUM(x) accumulate(all(x), 0LL)
/**
 * @brief 疑似無限大を表す型
 * @tparam IsPositive `true` なら正、`false` なら負
 */
template<bool IsPositive> struct InfBase {
static constexpr int sign= IsPositive ? 1 : -1;
template<class T>
requires is_arithmetic_v<T>
/**
 * @brief 算術型へ変換
 * @tparam T 変換先型
 * @return 型に応じた十分大きい値
 */
constexpr operator T() const {
if constexpr(is_same_v<T, int>) {
return sign * 1 << 30;
} else if constexpr(is_same_v<T, ll>) {
return sign * 1LL << 62;
} else if constexpr(is_same_v<T, double> || is_same_v<T, ld>) {
return sign * numeric_limits<T>::infinity();
} else if constexpr(is_signed_v<T>) {
return sign * (numeric_limits<T>::max() / 2);
} else {
static_assert(IsPositive, "Negative inf for unsigned type");
return numeric_limits<T>::max() / 2;
}
}

template<class T> friend constexpr bool operator<(InfBase lhs, const T &) { return !IsPositive; }
template<class T> friend constexpr bool operator>(InfBase lhs, const T &) { return IsPositive; }
template<class T> friend constexpr bool operator<(const T &, InfBase rhs) { return IsPositive; }
template<class T> friend constexpr bool operator>(const T &, InfBase rhs) { return !IsPositive; }
template<class T> friend constexpr bool operator<=(InfBase lhs, const T &rhs) { return !IsPositive; }
template<class T> friend constexpr bool operator>=(InfBase lhs, const T &rhs) { return IsPositive; }
template<class T> friend constexpr bool operator<=(const T &lhs, InfBase rhs) { return IsPositive; }
template<class T> friend constexpr bool operator>=(const T &lhs, InfBase rhs) { return !IsPositive; }
template<class T> friend constexpr bool operator==(InfBase lhs, const T &rhs) { return static_cast<T>(lhs) == rhs; }
template<class T> friend constexpr bool operator!=(InfBase lhs, const T &rhs) { return static_cast<T>(lhs) != rhs; }
template<class T> friend constexpr bool operator==(const T &lhs, InfBase rhs) { return lhs == static_cast<T>(rhs); }
template<class T> friend constexpr bool operator!=(const T &lhs, InfBase rhs) { return lhs != static_cast<T>(rhs); }

constexpr InfBase<!IsPositive> operator-() const { return {}; }
};

template<bool P> constexpr bool operator<(InfBase<P>, InfBase<P>) { return false; }
template<bool P> constexpr bool operator>(InfBase<P>, InfBase<P>) { return false; }
template<bool P> constexpr bool operator==(InfBase<P>, InfBase<P>) { return true; }
constexpr bool operator<(InfBase<false>, InfBase<true>) { return true; }
constexpr bool operator>(InfBase<false>, InfBase<true>) { return false; }
constexpr bool operator<(InfBase<true>, InfBase<false>) { return false; }
constexpr bool operator>(InfBase<true>, InfBase<false>) { return true; }
using PosInf= InfBase<true>;
using NegInf= InfBase<false>;
constexpr PosInf inf;
constexpr NegInf minf;
/**
 * @brief `a = min(a, b)` を行う
 * @param a 更新先
 * @param b 候補値
 * @return 更新されたら `true`
 * @complexity \(O(1)\)
 */
template<class T, class U> bool chmin(T &a, const U &b) {
if(b < a) {
a= b;
return 1;
} else
return 0;
}
/**
 * @brief `a = max(a, b)` を行う
 * @param a 更新先
 * @param b 候補値
 * @return 更新されたら `true`
 * @complexity \(O(1)\)
 */
template<class T, class U> bool chmax(T &a, const U &b) {
if(b > a) {
a= b;
return 1;
} else
return 0;
}

#ifdef LOCAL

#define debug(x) cerr << #x << " = " << (x) << '\n'
#else

#define debug(x)
#endif

#include <bits/stdc++.h>
using namespace std;
/**
 * @brief 二部グラフの最大マッチング
 * @note 左側頂点は `0` 以上 `L-1` 以下、右側頂点は `0` 以上 `R-1` 以下
 * @note Global relabel matching + Hopcroft-Karp
 */
struct BipartiteMatching {
int L, R, matching_size, target_dist;
bool built;
vector<pair<int, int>> es;
vector<int> ls, rs, ladj, radj;
vector<int> match_l, match_r;
vector<int> dist, iter, que, used, free_l;
vector<int> stk, pre;
vector<int> lv_l, lv_r, act;

BipartiteMatching(): L(0), R(0), matching_size(0), target_dist(0), built(false) {}
/**
 * @brief `L` 個の左頂点、`R` 個の右頂点からなる空の二部グラフを構築
 * @param L_ 左側頂点数
 * @param R_ 右側頂点数
 * @complexity \(O(L + R)\)
 */
BipartiteMatching(int L_, int R_): L(L_), R(R_), matching_size(0), target_dist(0), built(false), ls(L_ + 1), rs(R_ + 1), match_l(L_, -1), match_r(R_, -1), dist(L_, -1), iter(L_), que(L_), stk(L_), pre(L_), lv_l(L_), lv_r(R_), act(max(2, R_ + 1)) {
used.reserve(L);
free_l.reserve(L);
}
/**
 * @brief 空の二部グラフに初期化
 * @param L_ 左側頂点数
 * @param R_ 右側頂点数
 * @complexity \(O(L + R)\)
 */
void init(int L_, int R_) {
L= L_, R= R_;
matching_size= target_dist= 0;
built= false;
es.clear();
ls.assign(L + 1, 0);
rs.assign(R + 1, 0);
ladj.clear();
radj.clear();
match_l.assign(L, -1);
match_r.assign(R, -1);
dist.assign(L, -1);
iter.assign(L, 0);
que.assign(L, 0);
stk.assign(L, 0);
pre.assign(L, 0);
lv_l.assign(L, 0);
lv_r.assign(R, 0);
act.assign(max(2, R + 1), 0);
used.clear();
free_l.clear();
used.reserve(L);
free_l.reserve(L);
}
/**
 * @brief 辺メモリを予約
 * @param m 追加予定の辺数
 * @complexity \(O(m)\)
 */
void reserve_edges(int m) {
es.reserve(m);
ladj.reserve(m);
radj.reserve(m);
}
/**
 * @brief 左頂点 `l` と右頂点 `r` を結ぶ辺を追加
 * @param l 左側頂点
 * @param r 右側頂点
 * @return 追加された辺番号
 * @complexity 償却 \(O(1)\)
 */
int add_edge(int l, int r) {
assert(0 <= l && l < L);
assert(0 <= r && r < R);
built= false;
es.emplace_back(l, r);
return (int)es.size() - 1;
}
/**
 * @brief 辺数を返す
 * @return 辺数
 * @complexity \(O(1)\)
 */
int edge_count() const { return (int)es.size(); }
/**
 * @brief 現在のマッチングを消去
 * @complexity \(O(L + R)\)
 */
void clear_matching() {
fill(match_l.begin(), match_l.end(), -1);
fill(match_r.begin(), match_r.end(), -1);
matching_size= 0;
free_l.clear();
}
/**
 * @brief 最大マッチングを求める
 * @return 最大マッチングの本数
 * @note global relabel 型で高速に初期マッチングを作り、最後に Hopcroft-Karp で補完する
 * @complexity \(O(M\sqrt{L+R})\)
 */
int max_matching() {
build();
matching_size= count_matching();
int lim= min(L, R);
if(matching_size == lim) return matching_size;
relabel_matching_core();
if(matching_size == lim) return matching_size;
return hopcroft_karp_core();
}
/**
 * @brief Hopcroft-Karp のみで最大マッチングを求める
 * @return 最大マッチングの本数
 * @complexity \(O(M\sqrt{L+R})\)
 */
int max_matching_hopcroft_karp() {
build();
matching_size= count_matching();
if(matching_size == min(L, R)) return matching_size;
return hopcroft_karp_core();
}
/**
 * @brief 現在のマッチングの辺集合を返す
 * @return `(左側頂点, 右側頂点)` の列
 * @complexity \(O(L)\)
 */
vector<pair<int, int>> matching() const {
vector<pair<int, int>> res;
res.reserve(matching_size);
for(int i= 0; i < L; i++) {
if(match_l[i] != -1) res.emplace_back(i, match_l[i]);
}
return res;
}
/**
 * @brief 最小点被覆を返す
 * @return `(左側で選ぶ頂点, 右側で選ぶ頂点)`
 * @note 事前に `max_matching()` を呼ぶこと
 * @complexity \(O(L + R + M)\)
 */
pair<vector<int>, vector<int>> min_vertex_cover() {
build();
vector<char> vl(L, 0), vr(R, 0);
vector<int> q(L);
int ql= 0, qr= 0;
for(int i= 0; i < L; i++) {
if(match_l[i] == -1) {
vl[i]= 1;
q[qr++]= i;
}
}
while(ql < qr) {
int v= q[ql++];
for(int id= ls[v]; id < ls[v + 1]; id++) {
int r= ladj[id];
if(match_l[v] == r || vr[r]) continue;
vr[r]= 1;
int u= match_r[r];
if(u != -1 && !vl[u]) {
vl[u]= 1;
q[qr++]= u;
}
}
}
vector<int> left, right;
for(int i= 0; i < L; i++) {
if(!vl[i]) left.emplace_back(i);
}
for(int i= 0; i < R; i++) {
if(vr[i]) right.emplace_back(i);
}
return {left, right};
}
private:
void build() {
if(built) return;
int m= (int)es.size();
ls.assign(L + 1, 0);
rs.assign(R + 1, 0);
for(auto [l, r]: es) {
++ls[l];
++rs[r];
}
for(int i= 1; i < L; i++) ls[i]+= ls[i - 1];
for(int i= 1; i < R; i++) rs[i]+= rs[i - 1];
ls[L]= m;
rs[R]= m;
ladj.assign(m, 0);
radj.assign(m, 0);
for(auto [l, r]: es) {
ladj[--ls[l]]= r;
radj[--rs[r]]= l;
}
built= true;
}
int count_matching() const {
int res= 0;
for(int i= 0; i < L; i++) res+= match_l[i] != -1;
return res;
}
void global_relabel() {
fill(lv_l.begin(), lv_l.end(), inf);
fill(lv_r.begin(), lv_r.end(), inf);
int ql= 0, qr= 0;
for(int i= 0; i < L; i++) {
if(match_l[i] == -1) {
lv_l[i]= 0;
que[qr++]= i;
}
}
while(ql < qr) {
int v= que[ql++];
int nd= lv_l[v] + 1;
for(int id= ls[v]; id < ls[v + 1]; id++) {
int r= ladj[id];
if(lv_r[r] <= nd) continue;
lv_r[r]= nd;
int u= match_r[r];
if(u != -1 && lv_l[u] > nd + 1) {
lv_l[u]= nd + 1;
que[qr++]= u;
}
}
}
}
void relabel_matching_core() {
int lim= min(L, R);
if(matching_size == lim) return;
int cap= max(2, R + 1);
if((int)act.size() < cap) act.assign(cap, 0);
int ql= 0, qr= 0, qs= 0;
auto push= [&](int x) {
act[qr]= x;
if(++qr == cap) qr= 0;
++qs;
};
auto pop= [&]() {
int x= act[ql];
if(++ql == cap) ql= 0;
--qs;
return x;
};
for(int r= 0; r < R; r++) {
if(match_r[r] == -1) push(r);
}
int period= max(1, L + R);
int cnt= 0;
while(qs && matching_size < lim) {
if(cnt == 0) global_relabel();
int r= pop();
int best= -1;
int best_lv= inf;
for(int id= rs[r]; id < rs[r + 1]; id++) {
int l= radj[id];
if(lv_l[l] < best_lv) {
best_lv= lv_l[l];
best= l;
}
}
if(best != -1 && best_lv != inf) {
int old= match_l[best];
if(old != -1) {
match_r[old]= -1;
push(old);
} else {
++matching_size;
}
match_l[best]= r;
match_r[r]= best;
lv_l[best]= best_lv + 2;
}
if(++cnt == period) cnt= 0;
}
}
int hopcroft_karp_core() {
int lim= min(L, R);
free_l.clear();
for(int i= 0; i < L; i++) {
if(match_l[i] == -1) free_l.emplace_back(i);
}
while(matching_size < lim && bfs()) {
int add= 0;
for(int s: free_l) {
if(match_l[s] == -1 && dist[s] == 0 && dfs(s)) ++add;
}
if(add == 0) break;
matching_size+= add;
int k= 0;
for(int s: free_l) {
if(match_l[s] == -1) free_l[k++]= s;
}
free_l.resize(k);
}
return matching_size;
}
bool bfs() {
for(int v: used) dist[v]= -1;
used.clear();
target_dist= inf;
int ql= 0, qr= 0;
for(int s: free_l) {
if(match_l[s] != -1) continue;
dist[s]= 0;
iter[s]= ls[s];
used.emplace_back(s);
que[qr++]= s;
}
while(ql < qr) {
int v= que[ql++];
int nd= dist[v] + 1;
if(nd > target_dist) continue;
for(int id= ls[v]; id < ls[v + 1]; id++) {
int r= ladj[id];
int u= match_r[r];
if(u == -1) {
target_dist= nd;
} else if(nd < target_dist && dist[u] == -1) {
dist[u]= nd;
iter[u]= ls[u];
used.emplace_back(u);
que[qr++]= u;
}
}
}
return target_dist != inf;
}
bool dfs(int s) {
int top= 0;
stk[top++]= s;
while(top) {
int v= stk[top - 1];
int nd= dist[v] + 1;
int &id= iter[v];
while(id < ls[v + 1]) {
int r= ladj[id];
int u= match_r[r];
if(u == -1) {
if(nd != target_dist) {
++id;
continue;
}
pre[top - 1]= id++;
for(int i= top - 1; i >= 0; i--) {
int a= stk[i];
int b= ladj[pre[i]];
match_l[a]= b;
match_r[b]= a;
}
return true;
}
if(nd < target_dist && dist[u] == nd) {
pre[top - 1]= id++;
stk[top++]= u;
goto next_vertex;
}
++id;
}
dist[v]= -1;
--top;
next_vertex:;
}
return false;
}
};
// clang-format on

int main()
{
    def(int, L, R, M);

    BipartiteMatching bm(L, R);
    bm.reserve_edges(M);

    rep (i, M)
    {
        def(int, a, b);
        bm.add_edge(a, b);
    }

    print(bm.max_matching());

    rep (i, L)
    {
        if (bm.match_l[i] != -1)
        {
            print(i, bm.match_l[i]);
        }
    }
}