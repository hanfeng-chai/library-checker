
#include <bits/stdc++.h>

#pragma GCC target("avx2")
#include <immintrin.h>

namespace debug {

}  

#ifdef LOCAL
#define CHECK(expr) assert(expr)
#else
#define CHECK(expr) void(0)
#endif

#ifndef __SIZEOF_INT128__
#error "This library requires compiler support for __int128."
#endif

namespace internal {

template <typename T, typename U>
concept same_as = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template <typename T>
concept basic_signed_integral =
    std::signed_integral<std::remove_cvref_t<T>> && !same_as<T, char>;

template <typename T>
concept basic_unsigned_integral =
    std::unsigned_integral<std::remove_cvref_t<T>> && !same_as<T, bool>;

template <typename T>
concept basic_integral = basic_signed_integral<T> || basic_unsigned_integral<T>;

template <typename T>
concept signed_integral = basic_signed_integral<T> || same_as<T, __int128_t>;

template <typename T>
concept unsigned_integral =
    basic_unsigned_integral<T> || same_as<T, __uint128_t>;

template <typename T>
concept integral = signed_integral<T> || unsigned_integral<T>;

template <typename T>
struct make_unsigned : std::make_unsigned<std::remove_cvref_t<T>> {};

template <typename T>
  requires(same_as<T, __int128_t> || same_as<T, __uint128_t>)
struct make_unsigned<T> {
  using type = __uint128_t;
};

template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

}  // namespace internal

#ifdef __unix__
#ifndef DISABLE_MMAP
#define ENABLE_MMAP
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#endif

namespace fast_io {

template <int BufSize = 1 << 20>
struct FastInput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;
  size_t map_size;

  explicit FastInput(FILE* _file = stdin) : file(_file) {
#ifdef ENABLE_MMAP
    struct stat st;
    int fd = fileno(file);
    fstat(fd, &st);
    map_size = st.st_size;
    cur = buf = static_cast<char*>(
        mmap(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, 0));
    end = cur + map_size;
#else
    cur = buf = new char[BufSize + 64];
    end = buf + fread(buf, 1, BufSize, file);
    memset(const_cast<char*>(end), 0, 64);
#endif
  }

#ifdef ENABLE_MMAP
  ~FastInput() { munmap(buf, map_size); }
#else
  ~FastInput() { delete[] buf }
#endif

#ifndef ENABLE_MMAP
  void ensure() {
    int rem = end - cur;
    if (rem >= 40) [[likely]] return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, 64);
  }
#else
#define ensure() void(0)
#endif

  void skip_space() {
    ensure();
    while (*cur < 33) [[unlikely]] {
      ++cur;
      ensure();
    }
  }

  template <internal::unsigned_integral T>
  T read_small() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint32_t v;
    memcpy(&v, cur, 4);
    v ^= 0x30303030;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffffull;
      x = x * 10000 + v, cur += 4;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::signed_integral T>
  T read_small() {
    using U = internal::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read_small<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) < 8)
  T read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = *cur++ & 15;
    uint64_t v;
    memcpy(&v, cur, 8);
    v ^= 0x3030303030303030ull;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffffull;
      v = (v * 10000 + (v >> 32)) & 0xffffffffull;
      x = x * 100000000 + v, cur += 8;
    }

    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) == 8)
  uint64_t read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    __m128i raw = _mm_loadu_si128(reinterpret_cast<const __m128i*>(cur));
    __m128i diff = _mm_sub_epi8(raw, zero_128);
    uint32_t mask = _mm_movemask_epi8(diff);

    if (mask == 0) {
      uint64_t x = parse_w16(diff);
      cur += 16;
      for (; *cur >= 48; ++cur) {
        x = x * 10 + (*cur & 15);
      }
      ++cur;
      return x;
    } else {
      int len = __builtin_ctz(mask);
      cur += len + 1;
      diff = _mm_shuffle_epi8(
          diff, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&LUT[len])));
      return parse_w16(diff);
    }
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) > 8)
  __uint128_t read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    __m256i raw = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));
    __m256i diff = _mm256_sub_epi8(raw, zero_256);
    uint32_t mask = _mm256_movemask_epi8(diff);

    if (mask == 0) {
      __uint128_t x = parse_w32(diff);
      cur += 32;

      uint32_t v;
      memcpy(&v, cur, 4);
      v ^= 0x30303030;
      uint32_t val = 0, pow = 1;
      if (all_digits(v)) {
        v = (v * 10 + (v >> 8)) & 0xff00ff;
        v = (v * 100 + (v >> 16)) & 0xffff;
        val = v, pow = 10000, cur += 4;
      }

      for (; *cur >= 48; ++cur, pow *= 10) {
        val = val * 10 + (*cur & 15);
      }

      ++cur;
      return x * pow + val;
    } else {
      int len = __builtin_ctz(mask);
      cur += len + 1;
      if (len <= 16) {
        __m128i low = _mm256_castsi256_si128(diff);
        low = _mm_shuffle_epi8(
            low, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&LUT[len])));
        return parse_w16(low);
      } else {
        alignas(32) char aux[64]{};
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(aux + 32 - len), diff);
        diff = _mm256_load_si256(reinterpret_cast<const __m256i*>(aux));
        return parse_w32(diff);
      }
    }
  }

  template <internal::signed_integral T>
  T read() {
    using U = internal::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v = read<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <typename T>
    requires(internal::same_as<T, bool>)
  T read() {
    ensure();
    CHECK(*cur == '0' || *cur == '1');
    T x = *cur & 1;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(internal::same_as<T, char>)
  T read() {
    ensure();
    T x = *cur;
    cur += 2;
    return x;
  }

  template <typename T>
    requires(internal::same_as<T, std::string>)
  T read() {
    ensure();
    CHECK(*cur > 32);

#ifdef ENABLE_MMAP
    char* first = cur;
    while (*cur > 32) ++cur;
    std::string s(first, cur);
    ++cur;
    return s;
#else
    std::string s;
    while (true) {
      char* last = cur;
      while (last < end && *last > 32) ++last;
      if (last < end) {
        s.append(cur, last);
        cur = last + 1;
        return s;
      } else {
        s.append(cur, last);
        cur = end;
        ensure();
      }
    }
#endif
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    x = read<T>();
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) {
      *s++ = *cur++;
      ensure();
    }
    *s = 0, ++cur;
    return *this;
  }

 private:
  static constexpr auto E16 = 10'000'000'000'000'000ull;

  static inline const __m128i zero_128 = _mm_set1_epi8(0x30);
  static inline const __m256i zero_256 = _mm256_set1_epi8(0x30);
  static inline const __m128i w1_128 = _mm_set1_epi16(0x010a);
  static inline const __m256i w1_256 = _mm256_set1_epi16(0x010a);
  static inline const __m128i w2_128 = _mm_set1_epi32(0x00010064);
  static inline const __m256i w2_256 = _mm256_set1_epi32(0x00010064);
  static inline const __m128i w3_128 = _mm_set_epi32(1, 10000, 1, 10000);
  static inline const __m256i w3_256 =
      _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000);

  static constexpr auto LUT = [] {
    std::array<std::array<char, 16>, 17> res;
    for (int i = 0; i <= 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        res[i][j] = (i + j < 16) ? 0x80 : i + j - 16;
      }
    }
    return res;
  }();

  static inline uint64_t parse_w16(__m128i chunk) {
    __m128i t1 = _mm_maddubs_epi16(chunk, w1_128);
    __m128i t2 = _mm_madd_epi16(t1, w2_128);
    __m128i prod = _mm_mul_epu32(t2, w3_128);
    __m128i odd = _mm_srli_epi64(t2, 32);
    __m128i t3 = _mm_add_epi64(prod, odd);
    uint64_t r0 = _mm_cvtsi128_si64(t3);
    uint64_t r1 = _mm_extract_epi64(t3, 1);
    return r0 * 100000000 + r1;
  }

  static inline __uint128_t parse_w32(__m256i chunk) {
    __m256i t1 = _mm256_maddubs_epi16(chunk, w1_256);
    __m256i t2 = _mm256_madd_epi16(t1, w2_256);
    __m256i prod = _mm256_mul_epu32(t2, w3_256);
    __m256i odd = _mm256_srli_epi64(t2, 32);
    __m256i t3 = _mm256_add_epi64(prod, odd);
    __m128i r0 = _mm256_castsi256_si128(t3);
    __m128i r1 = _mm256_extracti128_si256(t3, 1);
    uint64_t s0 = _mm_cvtsi128_si64(r0) * 100000000 + _mm_extract_epi64(r0, 1);
    uint64_t s1 = _mm_cvtsi128_si64(r1) * 100000000 + _mm_extract_epi64(r1, 1);
    return static_cast<__uint128_t>(s0) * E16 + s1;
  }

  constexpr bool all_digits(uint32_t v) { return !(v & 0xf0f0f0f0); }
  constexpr bool all_digits(uint64_t v) { return !(v & 0xf0f0f0f0f0f0f0f0ull); }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 20>
struct FastOutput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;

  explicit FastOutput(FILE* _file = stdout) : file(_file) {
    cur = buf = new char[BufSize];
    end = buf + BufSize;
  }

  template <int N = BufSize>
  void flush() {
    if (end - cur < N) [[unlikely]] {
      fwrite(buf, 1, cur - buf, file);
      cur = buf;
    }
  }

  ~FastOutput() {
    flush();
    delete[] buf;
  }

  template <typename T>
    requires(sizeof(T) < 8)
  void write(T x) {
    if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(x);
    } else {
      print<0>(x);
    }
  }

  template <typename T>
    requires(sizeof(T) == 8)
  void write(T x) {
    if (x > 9'999'999'999'999'999ull) {
      print<4>(x);
    } else if (x > 999'999'999'999ull) {
      print<3>(x);
    } else if (x > 99'999'999) {
      print<2>(x);
    } else if (x > 9999) {
      print<1>(static_cast<uint32_t>(x));
    } else {
      print<0>(static_cast<uint32_t>(x));
    }
  }

  template <typename T>
    requires(sizeof(T) > 8)
  void write(T x) {
    if (x < E19) {
      write(static_cast<uint64_t>(x));
    } else if (x < E38) {
      auto high = x / E19;
      auto low = x - high * E19;
      write(static_cast<uint64_t>(high));
      print_w19(static_cast<uint64_t>(low));
    } else [[unlikely]] {
      auto high = x / E38;
      x -= high * E38;
      auto mid = x / E19;
      auto low = x - mid * E19;
      write(static_cast<uint32_t>(high));
      print_w19(static_cast<uint64_t>(mid));
      print_w19(static_cast<uint64_t>(low));
    }
  }

  template <internal::unsigned_integral T>
  FastOutput& operator<<(T x) {
    flush<std::numeric_limits<T>::digits10 + 1>();
    write(x);
    return *this;
  }

  template <internal::signed_integral T>
  FastOutput& operator<<(T x) {
    using U = internal::make_unsigned_t<T>;

    flush<std::numeric_limits<T>::digits10 + 2>();
    *cur = '-';
    cur += (x < 0);
    write(x < 0 ? -static_cast<U>(x) : static_cast<U>(x));
    return *this;
  }

  FastOutput& operator<<(bool x) {
    flush<1>();
    *cur++ = x + '0';
    return *this;
  }

  FastOutput& operator<<(char x) {
    flush<1>();
    *cur++ = x;
    return *this;
  }

  FastOutput& operator<<(const char* s) {
    uint32_t len = strlen(s);
    if (len > BufSize) [[unlikely]] {
      flush();
      do {
        fwrite(s, 1, BufSize, file);
        s += BufSize;
        len -= BufSize;
      } while (len > BufSize);
    }
    if (end - cur < len) [[unlikely]] flush();
    memcpy(cur, s, len);
    cur += len;
    return *this;
  }

  FastOutput& operator<<(char* s) {
    return *this << const_cast<const char*>(s);
  }

  FastOutput& operator<<(const std::string& s) {
    return *this << s.c_str();
  }

  FastOutput& operator<<(const EndLine& end_line) {
    flush<1>();
    *cur++ = '\n';
    flush();
    return *this;
  }

 private:
  static constexpr auto LUT = [] {
    std::array<std::array<char, 4>, 10000> a, b;

    for (int i = 0; i < 10000; ++i) {
      b[i][0] = '0' + i / 1000;
      b[i][1] = '0' + i / 100 % 10;
      b[i][2] = '0' + i / 10 % 10;
      b[i][3] = '0' + i % 10;

      int j = 0;
      if (i >= 1000) a[i][j++] = b[i][0];
      if (i >= 100) a[i][j++] = b[i][1];
      if (i >= 10) a[i][j++] = b[i][2];
      a[i][j] = b[i][3];
    }

    return std::make_pair(a, b);
  }();

  static constexpr auto E16 = 10'000'000'000'000'000ull;
  static constexpr auto E19 = E16 * 1000;
  static constexpr auto E38 = static_cast<__uint128_t>(E19) * E19;

  template <int N, typename T>
  void print(T x) {
    if constexpr (N == 0) {
      memcpy(cur, &LUT.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    } else {
      print<N - 1>(x / 10000);
      memcpy(cur, &LUT.second[x % 10000], 4);
      cur += 4;
    }
  }

  template <int N, typename T>
  void print_full(T x) {
    if constexpr (N > 0) print_full<N - 1>(x / 10000);
    memcpy(cur, &LUT.second[x % 10000], 4);
    cur += 4;
  }

  void print_w19(uint64_t x) {
    auto high = static_cast<uint32_t>(x / E16);
    auto low = x - high * E16;
    memcpy(cur, &LUT.second[high][1], 3);
    cur += 3;
    print_full<3>(low);
  }
};

template <uint32_t InputBufSize = 1 << 20, uint32_t OutputBufSize = 1 << 20>
struct FastIO {
  FastInput<InputBufSize>* in;
  FastOutput<OutputBufSize>* out;

  FastIO() : in(nullptr), out(nullptr) {}

  ~FastIO() {
    if (in != nullptr) delete in;
    if (out != nullptr) {
      out->flush();
      delete out;
    }
  }

  void init(FILE* input_file = stdin, FILE* output_file = stdout) {
    in = new FastInput<InputBufSize>(input_file);
    out = new FastOutput<OutputBufSize>(output_file);
  }

  void flush() { out->flush(); }

  template <typename T>
  FastIO& operator>>(T& x) {
    *in >> x;
    return *this;
  }

  template <typename T>
  FastIO& operator<<(const T& x) {
    *out << x;
    return *this;
  }

  FastIO& operator<<(const EndLine& x) {
    *out << x;
    return *this;
  }
};

}  // namespace fast_io

using fast_io::FastIO;

namespace internal {

template <integral T>
constexpr int __lg(T x) {
  if constexpr (sizeof(T) <= 4) {
    return 31 ^ __builtin_clz(static_cast<uint32_t>(x));
  } else {
    return 63 ^ __builtin_clzll(static_cast<uint64_t>(x));
  }
}

template <integral T>
constexpr int countl_zero(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_clz(static_cast<uint32_t>(x));
  } else {
    return __builtin_clzll(static_cast<uint64_t>(x));
  }
}

template <integral T>
constexpr int countr_zero(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_ctz(static_cast<uint32_t>(x));
  } else {
    return __builtin_ctzll(static_cast<uint64_t>(x));
  }
}

template <integral T>
constexpr int popcount(T x) {
  if constexpr (sizeof(T) <= 4) {
    return __builtin_popcount(static_cast<uint32_t>(x));
  } else {
    return __builtin_popcountll(static_cast<uint64_t>(x));
  }
}

template <uint32_t Max, uint32_t N = 0, typename F>
void bit_width_const(uint64_t n, F&& func) {
  if constexpr (N <= Max) {
    if (n <= 1ull << N) {
      func.template operator()<N>();
    } else {
      bit_width_const<Max, N + 1>(n, func);
    }
  }
}

}  // namespace internal

template <typename T>
struct LiChaoSegTreeOffline {
  using Line  = typename T::Line;
  using Value = typename T::Value;
  using Range = typename T::Range;

  int n = 0;
  int m = 0;
  int h = 0;
  std::vector<Line> t;
  std::vector<Range> idx;
  std::vector<Value> val;

  template <typename It>
  LiChaoSegTreeOffline(It first, It last) { build(first, last); }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    h = internal::__lg(n);
    t.assign(n << 1, T::id());
    idx.assign(n + 1, T::range_id());
    val.assign(n, T::evaluate(T::id(), T::range_id()));
    std::copy(first, last, idx.begin());
  }

  void add_seg(int l, int r, const Line& line) {
    CHECK(0 <= l && l <= r && r <= m);
    if (l == r) [[unlikely]] return;
    l += n - 1, r += n;
    int w = internal::__lg(l ^ r);

    int cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = internal::countr_zero(l);
      l &= l - 1;
      add(cl >> i ^ 1, i, line);
    }

    int cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = internal::countr_zero(r);
      r &= r - 1;
      add(cr >> i ^ 1, i, line);
    }
  }

  void add_line(const Line& line) {
    add(1, h, line);
  }

  Value get(int x) const {
    CHECK(0 <= x && x < m);
    Range v = idx[x];
    Value res = val[x];
    x += n;
    while (x >>= 1) res = T::op(res, T::evaluate(t[x], v));
    return res;
  }

 private:
  void add(int k, int i, Line x) {
    int l = (k << i) ^ n;
    int r = l + (1 << i);
    Value xl = T::evaluate(x, idx[l]), xr = T::evaluate(x, idx[r]);

    while (true) {
      if (l + 1 == r) {
        val[l] = T::op(xl, val[l]);
        return;
      }

      Line& y = t[k];
      Value yl = T::evaluate(y, idx[l]), yr = T::evaluate(y, idx[r]);
      int mid = (l + r) >> 1;
      if (T::compare(xl, yl)) {
        if (T::compare(xr, yr)) {
          std::swap(x, y);
          return;
        } else {
          Value xmid = T::evaluate(x, idx[mid]);
          Value ymid = T::evaluate(y, idx[mid]);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k = k << 1 | 1, xl = ymid, xr = yr, l = mid;
          } else {
            k <<= 1, xr = xmid, r = mid;
          }
        }
      } else {
        if (T::compare(xr, yr)) {
          Value xmid = T::evaluate(x, idx[mid]);
          Value ymid = T::evaluate(y, idx[mid]);
          if (T::compare(xmid, ymid)) {
            std::swap(x, y);
            k <<= 1, xl = yl, xr = ymid, r = mid;
          } else {
            k = k << 1 | 1, xl = xmid, l = mid;
          }
        } else {
          return;
        }
      }
    }
  }
};

template <typename T, uint32_t N = 16, typename It, typename F>
void radix_sort_u32(It f, int n, F&& func) {
  CHECK(n >= 0);

  static constexpr uint32_t Mask = (1u << N) - 1;

  if (static_cast<uint32_t>(n) < Mask / 8) {
    std::stable_sort(f, f + n, [&](const T& x, const T& y) {
      return func(x) < func(y);
    });
    return;
  }

  std::array<uint32_t, Mask + 1> cnt1{}, cnt2{};
  for (int i = 0; i < n; ++i) {
    uint32_t val = func(f[i]);
    ++cnt1[val & Mask];
    ++cnt2[val >> N];
  }

  for (uint32_t i = 0; i < Mask; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
  }

  std::vector<T> tmp(n);
  for (int i = n - 1; i >= 0; --i) {
    tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    f[--cnt2[func(tmp[i]) >> N]] = std::move(tmp[i]);
  }
}

// Unverified
template <typename T, uint32_t N = 16, typename It, typename F>
void radix_sort_u64(It f, int n, F&& func) {
  CHECK(n >= 0);

  static constexpr uint32_t Mask = (1u << N) - 1;
  static constexpr uint32_t N2 = N * 2;
  static constexpr uint32_t N3 = N * 3;

  if (static_cast<uint32_t>(n) < Mask / 8) {
    std::stable_sort(f, f + n, [&](const T& x, const T& y) {
      return func(x) < func(y);
    });
    return;
  }

  std::array<uint32_t, Mask + 1> cnt1{}, cnt2{}, cnt3{}, cnt4{};
  for (int i = 0; i < n; ++i) {
    uint64_t val = func(f[i]);
    ++cnt1[val & Mask];
    ++cnt2[val >> N & Mask];
    ++cnt3[val >> N2 & Mask];
    ++cnt4[val >> N3];
  }

  for (uint32_t i = 0; i < Mask; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
    cnt3[i + 1] += cnt3[i];
    cnt4[i + 1] += cnt4[i];
  }

  std::vector<T> tmp(n);
  for (int i = n - 1; i >= 0; --i) {
    tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    f[--cnt2[func(tmp[i]) >> N & Mask]] = std::move(tmp[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    tmp[--cnt1[func(f[i]) >> N2 & Mask]] = std::move(f[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    f[--cnt2[func(tmp[i]) >> N3]] = std::move(tmp[i]);
  }
}

using namespace std;

FastIO<1 << 20, 1 << 19> io;

constexpr int64_t inf = 3e18;

struct Node {
  using Line  = pair<int32_t, int64_t>;
  using Value = int64_t;
  using Range = int32_t;

  static constexpr Line id() { return make_pair(0, inf); }

  static constexpr Range range_id() { return 1e9 + 5; }

  static constexpr Value evaluate(const Line& line, Range x) {
    return static_cast<Value>(line.first) * x + line.second;
  }

  static constexpr bool compare(Value x, Value y) { return x < y; }

  static constexpr Value op(Value x, Value y) { return min(x, y); }
};

void solve_main() {
  int n, q;
  io >> n >> q;
  vector<tuple<int, int, int, int64_t>> seg;
  seg.resize(n + q);
  vector<int> points;
  vector<uint64_t> aux;
  points.reserve(q);
  aux.reserve((n + q) << 1);

  uint32_t offset = 1u << 31;

  auto compress = [&](int i, int32_t v) -> uint64_t {
    uint32_t tmp = static_cast<uint32_t>(v) + offset;
    return static_cast<uint64_t>(i) << 32 | tmp;
  };

  for (int i = 0; i < n; ++i) {
    int32_t l, r, a;
    int64_t b;
    io >> l >> r >> a >> b;
    seg[i] = make_tuple(l, r, a, b);
    aux.push_back(compress(i << 2 | 2, l));
    aux.push_back(compress(i << 2, r));
  }

  for (int i = n; i < n + q; ++i) {
    bool op;
    io >> op;
    if (op == 0) {
      int32_t l, r, a;
      int64_t b;
      io >> l >> r >> a >> b;
      seg[i] = make_tuple(l, r, a, b);
      aux.push_back(compress(i << 2 | 2, l));
      aux.push_back(compress(i << 2, r));
    } else {
      int32_t p;
      io >> p;
      seg[i] = make_tuple(p, 0, 0, inf + 1);
      aux.push_back(compress(i << 2 | 1, p));
    }
  }

  radix_sort_u32<uint64_t>(aux.data(), aux.size(), [&](uint64_t x) {
    return static_cast<uint32_t>(x);
  });

  for (uint32_t lst = 0, cnt = 0; uint64_t pos : aux) {
    uint32_t high = static_cast<uint32_t>(pos >> 32);
    uint32_t low = static_cast<uint32_t>(pos);
    auto& x = seg[high >> 2];
    if (high & 1) {
      if (low != lst) {
        ++cnt;
        points.push_back(static_cast<int32_t>(low - offset));
        lst = low;
      }
      get<0>(x) = cnt - 1;
    } else {
      (high & 2 ? get<0>(x) : get<1>(x)) = cnt - (low == lst);
    }
  }

  LiChaoSegTreeOffline<Node> t(points.begin(), points.end());

  for (int i = 0; i < n; ++i) {
    auto [l, r, a, b] = seg[i];
    t.add_seg(l, r, make_pair(a, b));
  }

  for (int i = n; i < n + q; ++i) {
    auto [l, r, a, b] = seg[i];
    if (b > inf) {
      int64_t v = t.get(l);
      if (v >= inf) {
        io << "INFINITY\n";
      } else {
        io << v << '\n';
      }
    } else {
      t.add_seg(l, r, make_pair(a, b));
    }
  }
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif
  // cin.tie(nullptr)->sync_with_stdio(false);
  io.init();

  int T;
  // cin >> T;
  // io >> T;
  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}
