#include <bits/allocator.h>
#pragma clang attribute push (__attribute__((target("avx2"))), apply_to = function)
#pragma GCC target "avx2"
#pragma GCC optimize "tree-vectorize,unroll-loops"
#include <bits/stdc++.h>
#include <immintrin.h>

#include <cstdio>
struct SaferInput {
  static constexpr int bufsz = 1 << 15;
 
  SaferInput() {
    // read();
  }
 
  void read() {
    auto e = std::copy(bufptr, std::end(buf) - 1, buf);
    bufptr = buf;
    int tok = fread(e, 1, std::end(buf) - 1 - e, stdin);
    e[tok] = 0;
  }

  void operator()(char* b, int n) { rd(b, n); }
 
  template <typename T>
  void operator()(T* b, int n) { for (int i = 0; i < n; ++i) rd(b[i]); }
 
  template <typename... Ts, std::enable_if_t<(!std::is_pointer_v<std::decay_t<Ts>> && ...)>* = nullptr>
  auto operator()(Ts&... xs) { (rd(xs), ...); }

  template <char start = '!', int prefetch = 32>
  void skiplt() {
    if (std::end(buf) - 1 - bufptr < prefetch) read();
    while (*bufptr < start) ++bufptr;
  }
 
  char buf[bufsz], *bufptr = std::end(buf) - 1;
 
  template <char start = '-', typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
  void rd(T& x) {
    if constexpr (std::is_unsigned_v<T> && start == '-') return rd<'0'>(x);
    skiplt<start>();
    bool sign = start == '-' && *bufptr == '-'? 1: 0;
    bufptr += start < '0' && *bufptr < '0';
    T res{};
    while (*bufptr >= '0') res = res * 10 + (*bufptr++ - '0');
    x = sign? -res: res;
  }
 
  template <char start = '!'>
  void rd(char& x) {
    skiplt<start>();
    x = *bufptr++;
  }
 
  void rd(char* s, int n) {
    char* e = s + n;
    do s = std::copy_n(bufptr, std::min(e - s, std::end(buf) - 1 - buf), s);
    while (s < e && (read(), 1));
  }
 
  template <char start = '!'>
  void rd(std::string& s) {
    skiplt<start>();
    do {
      auto l = bufptr;
      while (*bufptr >= start) ++bufptr;
      s.insert(s.end(), l, bufptr);
    } while (bufptr == std::end(buf) - 1 && (read(), 1));
  }
} reader;

#include <cstdio>
#include <cstring>
struct UnsafeOutput {
  static constexpr int bufsz = 1 << 15;
 
  UnsafeOutput() {
    for (int x = 0; x < 1000; ++x) {
      rep[3 * x] = x / 100 + '0';
      rep[3 * x + 1] = x / 10 % 10 + '0';
      rep[3 * x + 2] = x % 10 + '0';
    }
  }
 
  void flush() {
    fwrite(buf, 1, bufptr - buf, stdout);
    bufptr = buf;
  }
 
  void operator<<(uint32_t x) {
    if (std::end(buf) - bufptr < 10) flush();
    auto st = bufptr, ptr = bufptr += 10;
    ptr[-1] = '\n';
    #pragma GCC unroll 99
    for (int i = 0; i < 3; ++i, ------ptr) {
      ptr[-4] = rep[x % 1000 * 3 + 0];
      ptr[-3] = rep[x % 1000 * 3 + 1];
      ptr[-2] = rep[x % 1000 * 3 + 2];
      if (x < 1000) {
        ptr += (x < 10) + (x < 100);
        break;
      }
      x /= 1000;
    }
    std::fill(st, ptr - 4, ' ');
  }
 
  ~UnsafeOutput() { flush(); }
 
  char rep[3000];
  char buf[bufsz], *bufptr = buf;
} writer;

using namespace std;

struct { using X = int; template <typename T = X> T operator()() const { T t; reader(t); return t; } operator X() const { return operator()(); } template <typename T> operator T() const { return operator()<T>(); } template <auto=0> string operator~() const { return *this; } char operator!() const { return *this; } } $;
void print(const auto&... ts) { string sep = ""; ((cout << sep << ts, sep = " "), ...); cout << '\n'; }
void prints(const auto& c) { cout << distance(c.begin(), c.end()) << ' ' << c << '\n'; }
string operator*(int n, string s) { string t; while (n-- > 0) t += s; return t; }
namespace std {
auto operator>>(istream& in, auto&& c) -> enable_if_t<!is_same_v<decay_t<decltype(c.begin(), c)>, string>, decltype(in)> { for (auto& i: c) in >> i; return in; }
auto operator<<(ostream& out, const auto& c) -> enable_if_t<!is_same_v<decay_t<decltype(c.begin(), c)>, string>, decltype(out)> { string sep = ""; for (auto i: c) out << sep << i, sep = " "; return out; }
auto operator>>(istream& in, auto&& p) -> decltype(p.first, p.second, in) { return in >> p.first >> p.second; }
auto operator<<(ostream& out, const auto& p) -> decltype(p.first, p.second, out) { return out << p.first << ' ' << p.second; }
}
template <typename It> struct range { It first, last; constexpr range() {} constexpr range(It first, It last) : first{first}, last{last} {} constexpr range(It first, auto n) : range{first, first + n} {} constexpr range(auto&& c) : range{c.begin(), c.end()} {} constexpr It begin() const { return first; } constexpr It end() const { return last; } constexpr int size() const { return last - first; } constexpr const auto& operator[](int i) const { return first[i]; } constexpr auto& operator[](int i) { return first[i]; } auto operator~() const { return range<decltype(reverse_iterator(last))>{reverse_iterator(last), reverse_iterator(first)}; } }; range(auto&& c) -> range<decltype(c.begin())>;
template <int from, int which> auto getfield(const auto& a) -> const auto& { static_assert(1 <= which && which <= from && from <= 6); auto fix = [](auto& x) -> auto& { return x; }; if constexpr (from == 1) { if constexpr (is_scalar_v<decay_t<decltype(a)>>) { if constexpr (which == 1) return fix(a); } else { auto& [a1] = a; if constexpr (which == 1) return fix(a1); } } else if constexpr (from == 2) { auto& [a1, a2] = a; if constexpr (which == 1) return fix(a1); if constexpr (which == 2) return fix(a2); } else if constexpr (from == 3) { auto& [a1, a2, a3] = a; if constexpr (which == 1) return fix(a1); if constexpr (which == 2) return fix(a2); if constexpr (which == 3) return fix(a3); } else if constexpr (from == 4) { auto& [a1, a2, a3, a4] = a; if constexpr (which == 1) return fix(a1); if constexpr (which == 2) return fix(a2); if constexpr (which == 3) return fix(a3); if constexpr (which == 4) return fix(a4); } else if constexpr (from == 5) { auto& [a1, a2, a3, a4, a5] = a; if constexpr (which == 1) return fix(a1); if constexpr (which == 2) return fix(a2); if constexpr (which == 3) return fix(a3); if constexpr (which == 4) return fix(a4); if constexpr (which == 5) return fix(a5); } else if constexpr (from == 6) { auto& [a1, a2, a3, a4, a5, a6] = a; if constexpr (which == 1) return fix(a1); if constexpr (which == 2) return fix(a2); if constexpr (which == 3) return fix(a3); if constexpr (which == 4) return fix(a4); if constexpr (which == 5) return fix(a5); if constexpr (which == 6) return fix(a6); } }
template <int from, int which> struct GetField { decltype(auto) operator()(const auto& a) { return getfield<from, which>(a); } };
template <int from, int which, typename Cmp = equal_to<>> struct CompareField { bool operator()(const auto& a, const auto& b) const { return Cmp{}(getfield<from, which>(a), getfield<from, which>(b)); } };
template <int from, int which, int... whichs> struct Ordering { bool operator()(const auto& a, const auto& b) const { auto& aa = getfield<from, labs(which)>(a), & bb = getfield<from, labs(which)>(b); if (aa < bb) { return which > 0; } else if (bb < aa) { return which < 0; } else if constexpr (sizeof...(whichs)) { return Ordering<from, whichs...>{}(a, b); } return 0; } };
bool minb(auto& a, const auto& b) { return b < a? a = b, 1: 0; } auto& mini(auto& a, const auto& b) { return a = a < b? a: b; } auto mind(auto& a, const auto& b) { auto t = a; return t - mini(a, b); }
bool maxb(auto& a, const auto& b) { return a < b? a = b, 1: 0; } auto& maxi(auto& a, const auto& b) { return a = a < b? b: a; } auto maxd(auto& a, const auto& b) { auto t = a; return maxi(a, b) - t; }
auto unz(auto a) { return max(a, {}); } int sgn(auto a) { return (0 < a) - (a < 0); } auto sqr(auto a) { return a * a; } auto change(auto&& a, const auto& b) { auto t = +a; return (a = b) - t; }
void ilset(auto& first, auto&& second, auto&&... args) { first = move(second); if constexpr (sizeof...(args)) ilset(forward<decltype(args)>(args)...); }
auto lshift(auto&& first, auto&&... args) { auto t = move(first); if constexpr (sizeof...(args)) { static_assert(is_lvalue_reference_v<decltype(first)>); first = lshift(forward<decltype(args)>(args)...); } return t; } auto rshift(auto&& first, auto&... args) { auto tup = tie(first, args...); constexpr int k = sizeof...(args); auto t = move(get<k>(tup)); [&]<size_t... i>(index_sequence<i...>) { ((get<k - i>(tup) = move(get<k - i - 1>(tup))), ...); }(make_index_sequence<k>{}); return t; }
void lrotate(auto& arg, auto&... args) { get<sizeof...(args)>(tie(arg, args...)) = lshift(arg, args...); } void rrotate(auto& arg, auto&... args) { arg = rshift(arg, args...); }
template <auto x> integral_constant<decltype(x), x> CC;
template <int X, int mn = 0> auto dispatch(int x, auto f) { if (X == x) return f(CC<X>); else if constexpr (X == mn) __builtin_unreachable(); else return dispatch<X - 1, mn>(x, f); }
template <typename T, typename Cmp = greater<>> using PQ = priority_queue<T, vector<T>, Cmp>;
auto pop(auto& c) -> decay_t<decltype(c.top())> { auto t = move((decay_t<decltype(c.top())>&)c.top()); c.pop(); return t; } auto pop(auto& c) -> decay_t<decltype(c.back())> { auto t = move(c.back()); c.pop_back(); return t; } auto pop(auto&& c) -> decay_t<decltype(c.front())> { auto t = move(c.front()); c.pop_front(); return t; } auto popfront(auto& c) -> decay_t<decltype(c.front())> { auto t = move(c.front()); c.pop_front(); return t; }

constexpr int mod = 998244353;

auto& mul(auto&& a, auto... b) { return ((a = a * (uint64_t)b % mod), ...); }
auto& add(auto&& a, auto... b) { return ((a -= (a += b) >= mod? mod: 0), ...); }
auto& sub(auto&& a, auto... b) { return ((a += (a -= b) < 0? mod: 0), ...); }
auto& addp(auto&& a, auto b, auto c) { return a = (a + b * (uint64_t)c) % mod; }
int fpow(int a, int b) { int r = 1; for (; b; b /= 2, mul(a, a)) if (b % 2) mul(r, a); return r; }

#define begend(a) (a).begin(), (a).end()
#define begendbeg(a) (a).begin(), (a).end(), (a).begin()
#define begendend(a, ...) (a).begin(), (a).end() __VA_OPT__(,) __VA_ARGS__), (a).end(



int cnt[1 << 9]; template <bool rev = 0, uint32_t mx = 1 << 20, uint32_t presorted = 1, uint32_t radix, uint32_t... radixes> void radix_sort_lsb(auto src, auto dst, int sz, auto key) {
  constexpr bool last = presorted > mx / radix || presorted == mx / radix && mx % radix == 0;
  auto extract = [last, key](auto x) {
    auto k = key(x) / presorted;
    if (!last) k %= radix;
    return rev? radix - 1 - k: k;
  };
  fill_n(cnt, radix, 0);
  for (int i = 0; i < sz; ++i) ++cnt[extract(src[i])];
  for (int i = 0, p = 0; i < radix; ++i) p += exchange(cnt[i], p);
  for (int i = 0; i < sz; ++i) dst[cnt[extract(src[i])]++] = src[i];
  if constexpr (!last) return radix_sort_lsb<rev, mx, presorted * radix, radixes...>(dst, src, sz, key);
}

using V __attribute((vector_size(32))) = uint32_t;
using VV __attribute((vector_size(32))) = uint64_t;

struct PleaseReuse {
  V vmod, vmod2, vinvn;
  VV vvmod, vvmod2, vvinvn;
  PleaseReuse();
} reuse;

struct Montgomery_v8u32 {
  using T = uint32_t;
  using TT = uint64_t;
  constexpr static int bits = sizeof(T) * 8;
  constexpr static int lg = __builtin_ctz(bits);
  using V __attribute((vector_size(32))) = T;
  using VV __attribute((vector_size(32))) = TT;

  T mod, mod2, invn, p1, m1, r2;
  V vmod, vmod2, vinvn, vp1, vm1, vr2;
  VV vvmod, vvmod2, vvinvn, vvp1, vvm1, vvr2;

  constexpr Montgomery_v8u32(T n): mod(n), mod2(n + n), invn(1), p1(-n % n), m1(n - p1), r2(p1),
                                   vmod{}, vmod2{}, vinvn{}, vp1{}, vm1{}, vr2{},
                                   vvmod{}, vvmod2{}, vvinvn{}, vvp1{}, vvm1{}, vvr2{} {
    for (int i = 0; i < lg; ++i) {
      invn *= 2 + n * invn;
    }
    for (int i = 0; i < 4; ++i) {
      r2 *= 2;
      if (r2 >= mod) {
        r2 -= mod;
      }
    }
    for (int i = 0; i < lg - 2; ++i) {
      r2 = prod(r2, r2);
    }
    vmod = V{} + mod;
    vmod2 = V{} + mod2;
    vinvn = V{} + invn;
    vp1 = V{} + p1;
    vm1 = V{} + m1;
    vr2 = V{} + r2;
    vvmod = VV{} + (TT)mod;
    vvmod2 = VV{} + (TT)mod2;
    vvinvn = VV{} + (TT)invn;
    vvp1 = VV{} + (TT)p1;
    vvm1 = VV{} + (TT)m1;
    vvr2 = VV{} + (TT)r2;
  }

  constexpr T in(T x) const {
    return prod(x, r2);
  }

  constexpr T out(T x) const {
    return redc(x);
  }

  constexpr T redc(TT x) const {
    T lo = x, hi = x >> bits;
    T q = lo * invn;
    T a = x + q * TT{mod} >> bits;
    return a;
  }

  constexpr T prod(T a, T b) const {
    return redc(a * TT{b});
  }

  constexpr T sum(T a, T b) const {
    return a += b, a = a > ~T{} / 2? a - mod2: a;
  }

  constexpr T dif(T a, T b) const {
    return a -= b, a = a > ~T{} / 2? a + mod2: a;
  }

  constexpr T fix(T a) const {
    return a >= mod? a - mod: a;
  }

  V in(V x) const {
    return prod(x, vr2);
  }

  V out(V x) const {
    return fix(prod(x, V{} + 1));
  }

  __m256i redcu(VV x) const {
    return (__m256i)(x + (VV)_mm256_mul_epu32(_mm256_mul_epu32((__m256i)x, (__m256i)reuse.vinvn), (__m256i)reuse.vmod));
  }

  VV redc0(VV x) const {
    return (VV)_mm256_bsrli_epi128(redcu(x), 4);
  }

  VV prod0(VV a, VV b) const {
    return redc0((VV)_mm256_mul_epu32((__m256i)a, (__m256i)b));
  }

  V prod(V a, V b) const {
    auto even = _mm256_bsrli_epi128(redcu((VV)_mm256_mul_epu32((__m256i)a, (__m256i)b)), 4);
    auto odd = redcu((VV)_mm256_mul_epu32(_mm256_bsrli_epi128((__m256i)a, 4), _mm256_bsrli_epi128((__m256i)b, 4)));
    return (V)even + (V)odd;
  }

  T hprod0(VV a) const {
    a = redc0((VV)_mm256_mul_epu32((__m256i)a, _mm256_permute4x64_epi64((__m256i)a, 0x4e)));
    a = redc0((VV)_mm256_mul_epu32((__m256i)a, _mm256_shuffle_epi32((__m256i)a, 0x4e)));
    return ((V)a)[0];
  }

  T hprod(V a) const {
    return hprod0(redc0((VV)_mm256_mul_epu32((__m256i)a, _mm256_bsrli_epi128((__m256i)a, 4))));
  }

  V sum(V a, V b) const {
    return a += b, a = a > a - reuse.vmod2? a - reuse.vmod2: a;
  }

  V dif(V a, V b) const {
    return a -= b, a = a > a + reuse.vmod2? a + reuse.vmod2: a;
  }

  V fix(V a) const {
    return a >= reuse.vmod? a - reuse.vmod: a;
  }
};

constexpr Montgomery_v8u32 vmod{998244353};

PleaseReuse::PleaseReuse() : vmod{::vmod.vmod}, vmod2{::vmod.vmod2}, vinvn{::vmod.vinvn}, vvmod{::vmod.vvmod}, vvmod2{::vmod.vvmod2}, vvinvn{::vmod.vvinvn} { }

constexpr int N = 1e5;

struct LargeDiffTable64 {
  constexpr static int A = 128, B = 4, S = A * B, G = mod / 2 / 4, F = 31 / B * B;
  constexpr static double ccost = 1 / 2.3e6 * G / S * 4 * A * B / F / 4;
  constexpr static double cost = 1 / 2.3e6 * G / S * (9 + A) * B / 4;
  constexpr static double limcost = 1 / 2.3e6 * G / S / 1024 * (9 + A * 1024) * B / 4;
  constexpr static double totcost = ccost + cost;
  constexpr static double costgap = cost - limcost;
  VV dif[A + 1], prods[B];
  uint64_t difir[B + 1], difr, prodr;

  __always_inline LargeDiffTable64(int ofs, int& ovfl) {
    for (int j = 0; j <= A; ++j) {
      VV m;
      #pragma GCC unroll 9
      for (int k = 0; k < 4; ++k) m[k] = k * G;
      m += uint32_t(ofs + A * j);
      dif[j] = VV{} + 1;
      for (int k = 0; k < A; ++k) {
        dif[j] = vmod.prod0(dif[j], m);
        m += 1;
      }
    }
    for (int i = 1; i <= A; ++i)
    for (int j = A; j >= i; --j) dif[j] = (VV)vmod.dif((V)dif[j], (V)dif[j - 1]);
    difr = fpow(vmod.p1, A + 2);
    // prodr * difr^B = p1^A
    prodr = fpow(vmod.p1, mod - 1 - (A + 2) * B);
    difir[0] = 1;
    difir[1] = fpow(vmod.p1, A + 2);
    for (int i = 2; i <= B; ++i) difir[i] = mul(+difir[i / 2], difir[i - i / 2]);
    difr = mul(+difr, vmod.p1);
    ovfl = 1;
  }

  __always_inline void step(VV& prod, int& ovfl) {
    auto f = [&](auto covfl) {
      asm("" : : : "memory");
      asm("" : : "v"(reuse.vinvn), "v"(reuse.vmod));
      constexpr int nr = B + 1;
      VV reg[nr];
      reg[B] = prod;
      if (covfl) {
        difir[1] = difr;
        #pragma GCC unroll 99
        for (int j = 2; j <= B; ++j) difir[j] = mul(+difir[j - 1], difr);
        difr = mul(+difr, vmod.p1);
      }
      #pragma GCC unroll 999
      for (int i = -B; i <= A; ++i) {
        if (i + B <= A) {
          asm("" : "+m"(dif[i + B]) : "m"(dif[i % nr]));
          if (!covfl) {
            reg[(i + B) % nr] = dif[i + B];
            if ((i + B) % 2 == 0) asm("" : "+v"(reg[(i + B) % nr]));
          } else {
            asm("" : : "v"(reuse.vinvn), "v"(reuse.vmod));
            reg[(i + B) % nr] = vmod.redc0(dif[i + B]);
            asm("" : "+v"(reg[(i + B) % nr]));
          }
        }
        if (!covfl)
        if (i > -B && i + B <= A + 1 && (i + B) % 2 == 0) {
          asm("" : "+m"(dif[i + B - 1]));
          reg[(i + B - 1) % nr] = dif[i + B - 1];
        }
        #pragma GCC unroll 99
        for (int j = B - 1; ~j; --j) {
          if (i + j == -1) prods[i + B] = reg[B], reg[B] = vmod.prod0(reg[B], vmod.redc0(reg[0]));
          else if (i + j >= 0 && i + j < A) reg[(i + j) % nr] += reg[(i + j + 1) % nr];
        }
        if (i == -1) {
          prod = reg[B];
        } else if (i >= 0) {
          dif[i] = reg[i % nr];
          asm("" : "+v"(reg[(i + 1) % nr]) : "m"(dif[i]));
          // asm("" : "+x"(dif[i]));
        }
      }
    };
    prodr = mul(+prodr, difir[B]);
    if (__builtin_expect(!--ovfl, 0)) ovfl = F / B, f(CC<1>);
    else f(CC<0>);
  }
};

struct SmallDiffTable32 {
  constexpr static int A = 4, S = A;
  constexpr static double ccost = 1 / 2.3e6 * N / 8 * (14 * 3 + (A - 1) * 23 + (A - 2) * (A - 1) / 2 * 3) / 4;
  constexpr static double cost = 1 / 2.3e6 * N * LargeDiffTable64::A / 4 / 8 / S * (11 + A * 3) / 4;
  constexpr static double limcost = 1 / 2.3e6 * N * LargeDiffTable64::A / 4 / 8 / S / 1024 * (11 + A * 3 * 1024) / 4;
  constexpr static double costgap = cost - limcost;
  constexpr static double totcost = ccost + cost + LargeDiffTable64::totcost;

  void work(V& vi, V& prod, int len) {
    for (int i = 0; i < len % A; ++i) {
      prod = vmod.prod(prod, vi);
      vi = vmod.sum(vi, vmod.vp1);
    }
    V dif[A + 1];
    static_assert(A == 4);
    {
      V dif2[3];
      dif2[0] = vmod.prod(vi, vmod.sum(vi, vmod.vp1));
      dif2[1] = vmod.sum(V{} + mul(+vmod.p1, 6), vmod.prod(vi, V{} + mul(+vmod.p1, 4)));
      dif2[2] = V{} + mul(+vmod.p1, 8);
      for (int j = 0; j < A - 1; ++j) {
        dif[j] = dif2[0];
        dif2[0] = vmod.sum(dif2[0], dif2[1]);
        dif2[1] = vmod.sum(dif2[1], dif2[2]);
        dif[j] = vmod.prod(dif[j], dif2[0]);
        dif2[0] = vmod.sum(dif2[0], dif2[1]);
        dif2[1] = vmod.sum(dif2[1], dif2[2]);
      }
      for (int i = 1; i < A - 1; ++i)
      for (int j = A - 2; j >= i; --j) dif[j] = vmod.dif(dif[j], dif[j - 1]);
      dif[3] = vmod.sum(V{} + mul(+vmod.p1, 11520), vmod.prod(vi, V{} + mul(+vmod.p1, 1536)));
      dif[4] = V{} + mul(+vmod.p1, 6144);
    }
    for (int i = len / A; i--; ) {
      prod = vmod.prod(prod, dif[0]);
      #pragma GCC unroll 99
      for (int i = 0; i < A; ++i) {
        dif[i] = vmod.sum(dif[i], dif[i + 1]);
      }
    }
  }
};

constexpr int bits0 = 32 - __builtin_clz(LargeDiffTable64::S - 1), bits1 = 32 - __builtin_clz((LargeDiffTable64::G - 1) / LargeDiffTable64::S);
constexpr int batchcnt = LargeDiffTable64::A * 2, batchcap = 120;

struct Inverser32 {
  constexpr static double cost = 1 / 2.3e6 * (1. * N / 8 * (11 * 3) + (1. * N / batchcap + batchcnt) * (56 * 5)) / 4;
  constexpr static double limcost = 1 / 2.3e6 * N / 8 * (2 * 13) / 4;
  constexpr static double costgap = cost - limcost;
  constexpr static double totcost = cost + SmallDiffTable32::totcost;

  void work(V* src, V* tmp, int len) {
    V prod = vmod.vp1;
    for (int i = 0; i < len; ++i) {
      tmp[i] = prod;
      prod = vmod.prod(prod, src[i]);
    }
    VV hprod = vmod.prod0((VV)prod, (VV)prod >> 32);
    VV ihprod = vmod.vvp1, t = hprod;
    #pragma GCC unroll 99
    for (int b = mod - 2; b; b /= 2) {
      if (b & 1) ihprod = vmod.prod0(ihprod, t);
      t = vmod.prod0(t, t);
    }
    V iprod = (V)(vmod.prod0(ihprod, (VV)prod) << 32 | vmod.prod0(ihprod, (VV)prod >> 32));
    for (int i = len; i--; ) {
      auto t = src[i];
      src[i] = vmod.prod(iprod, tmp[i]);
      iprod = vmod.prod(iprod, t);
    }
  }
};

uint64_t qs[N + 1];
alignas(8) uint32_t lprod[N * 2];
int ans[N], ofs;
VV flprod;
int smallf[2][LargeDiffTable64::S * 2];

void proc_large(int ofs) {
  int ovfl;
  LargeDiffTable64 dt(ofs, ovfl);
  VV prod{}; prod += 1;
  int cq{-1}, ct, cp;
  auto ldct = [&] { ++cq, ct = qs[cq] >> 32, cp = qs[cq] >> 28 & 6; };
  ldct();
  for (int i = -LargeDiffTable64::S; i += LargeDiffTable64::S, 1; ) {
    for (; __builtin_expect(i > ct, 0); ldct()) {
      if (__builtin_expect(i > LargeDiffTable64::G, 0)) { flprod = vmod.prod0(dt.prods[0], VV{} + dt.prodr); return; }
      int q = (ct + LargeDiffTable64::S - i) / LargeDiffTable64::A;
      int x = ((V)dt.prods[q])[cp];
      mul(x, dt.prodr, dt.difir[q]);
      lprod[cq] = x;
    }
    dt.step(prod, ovfl);
  }
}

alignas(32) uint32_t batchi[batchcnt][batchcap], batchq[batchcnt][batchcap], batchp[batchcnt][batchcap], bsz[batchcnt];

void postproc(int b) {
  int sz = bsz[b];
  bool more = b & LargeDiffTable64::A / 2;
  int len = b & LargeDiffTable64::A / 2 - 1;
  if (!more) len = LargeDiffTable64::A / 2 - len;
  alignas(32) uint32_t res[batchcap];
  fill(batchi[b] + sz, batchi[b] + (sz + 7) / 8 * 8, 123456);
  fill(batchp[b] + sz, batchp[b] + (sz + 7) / 8 * 8, 123456);
  for (int i = 0; i < sz; i += 8) {
    V vi = (V&)batchi[b][i];
    V vp{}; vp += vmod.p1;
    vi -= uint32_t(more? len: 0);
    vi = vmod.in(vi);
    SmallDiffTable32 dt;
    dt.work(vi, vp, len);
    (V&)res[i] = vp;
  }
  dispatch<7>(b & 1 & b / LargeDiffTable64::A | b / (LargeDiffTable64::A / 2) * 2, [&](auto tp) {
    int vsz = (sz + 7) / 8;
    alignas(32) V p[batchcap / 8], q[batchcap / 8];
    for (int i = 0; i < vsz; ++i) {
      V hi = (V&)batchp[b][i * 8], lo = (V&)res[i * 8];
      lo = tp & 1? vmod.vmod2 - lo: lo;
      if (tp == 2) p[i] = vmod.prod(hi, lo);
      if (tp == 0) p[i] = hi, q[i] = lo;
      if ((tp & 6) == 6) q[i] = vmod.prod(hi, lo);
      if ((tp & 6) == 4) q[i] = hi, p[i] = lo;
    }
    if (tp != 2) Inverser32{}.work(q, (V*)res, vsz);
    for (int i = 0; i < vsz; ++i) {
      if (tp == 2) (V&)res[i * 8] = vmod.fix(vmod.out(p[i]));
      if (tp == 0) (V&)res[i * 8] = vmod.fix(vmod.out(vmod.prod(p[i], q[i])));
      if ((tp & 6) == 6) (V&)res[i * 8] = vmod.fix(vmod.out(q[i]));
      if ((tp & 6) == 4) (V&)res[i * 8] = vmod.fix(vmod.out(vmod.prod(p[i], q[i])));
    }
  });
  for (int i = 0; i < sz; ++i) ans[batchq[b][i] & (1 << 29) - 1] = res[i];
  bsz[b] = 0;
}

int main() {
  int n = $, m{};
  for (int i = 0, f = 1; i < size(smallf[0]); ++i) smallf[0][i] = f, mul(f, i + 1);
  for (int i = size(smallf[0]), f = fpow(smallf[1][-1], mod - 2); i--; ) smallf[1][i] = f, mul(f, mod - i);
  ofs = LargeDiffTable64::A;
  for (int i = 0; i < n; ++i) {
    uint64_t n = (uint32_t)$;
    uint64_t f = n > vmod.mod - 1 - n; n = f? vmod.mod - 1 - n: n;
    if (n < ofs + LargeDiffTable64::S) {
      ans[i] = smallf[f][n];
      continue;
    }
    ++n;
    n -= ofs;
    n += LargeDiffTable64::A / 2;
    uint64_t w = n / LargeDiffTable64::G; n -= w * LargeDiffTable64::G;
    uint64_t h = n / LargeDiffTable64::S; n -= h * LargeDiffTable64::S;
    assert(w < 4);
    qs[m++] = h << 32 + bits0 | n << 32 | f + 0ull << 31 | w + 0ull << 29 | i;
  } 
  qs[m] = LargeDiffTable64::G + 0ull << 32;
  radix_sort_lsb<0, 1 << bits1, 1, 1 << bits1 / 2, 1 << bits1 - bits1 / 2>(qs, (uint64_t*)(char*)lprod, m, [](auto x) { return x >> 32 + bits0; });
  proc_large(ofs);
  int eprod[4]{vmod.in(smallf[0][ofs - 1])};
  for (int i = 1; i < 4; ++i) eprod[i] = mul(+eprod[i - 1], flprod[i - 1], vmod.p1);
  for (int i = 0; i < m; ++i) {
    uint64_t n0 = qs[i] >> 32 & (1 << bits0) - 1;
    uint64_t n1 = qs[i] >> 32 + bits0;
    uint64_t w = qs[i] >> 29 & 3;
    uint64_t b = n0 % LargeDiffTable64::A + LargeDiffTable64::A * (qs[i] >> 31 & 1);
    batchi[b][bsz[b]] = w * LargeDiffTable64::G + n1 * LargeDiffTable64::S + n0 + ofs - LargeDiffTable64::A / 2;
    batchp[b][bsz[b]] = mul(+lprod[i], eprod[w]);
    batchq[b][bsz[b]] = qs[i];
    if (++bsz[b] == batchcap) postproc(b);
  }
  for (int i = 0; i < batchcnt; ++i) if (bsz[i]) postproc(i);
  for (int i = 0; i < n; ++i) writer << ans[i];
}
#pragma clang attribute pop