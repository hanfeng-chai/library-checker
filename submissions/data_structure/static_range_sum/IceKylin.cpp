// https://judge.yosupo.jp/problem/static_range_sum

#pragma GCC optimize(3)
#include <bits/stdc++.h>

namespace debug {

}  // namespace debug

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
    buf = cur = static_cast<char*>(
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
  ~FastInput() { delete[] buf; }
#endif

  void ensure() {
#ifndef ENABLE_MMAP
    int rem = end - cur;
    if (rem >= 40) [[likely]] return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, Offset);
#endif
  }

  void skip_space() {
    ensure();
    while (*cur < 33) [[unlikely]] {
      ++cur;
      ensure();
    }
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

    for (; *cur >= '0'; ++cur) {
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

    for (; *cur >= '0'; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) == 8)
  T read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    union {
      char ch[16];
      uint64_t v[2];
    };
    memcpy(ch, cur, 16);
    uint64_t a = v[0] ^ 0x3030303030303030ull;
    uint64_t b = v[1] ^ 0x3030303030303030ull;

    T x = 0;
    if (all_digits(a)) {
      a = (a * 10 + (a >> 8)) & 0xff00ff00ff00ffull;
      a = (a * 100 + (a >> 16)) & 0xffff0000ffffull;
      a = (a * 10000 + (a >> 32)) & 0xffffffffull;
      x = a, cur += 8;
      if (all_digits(b)) {
        b = (b * 10 + (b >> 8)) & 0xff00ff00ff00ffull;
        b = (b * 100 + (b >> 16)) & 0xffff0000ffffull;
        b = (b * 10000 + (b >> 32)) & 0xffffffffull;
        x = a * 100000000 + b, cur += 8;
      }
    }

    for (; *cur >= '0'; ++cur) {
      x = x * 10 + (*cur & 15);
    }

    ++cur;
    return x;
  }

  template <internal::unsigned_integral T>
    requires(sizeof(T) > 8)
  T read() {
    ensure();
    CHECK(*cur >= '0' && *cur <= '9');

    T x = 0;
    for (int i = 0; i < 4; ++i) {
      uint64_t v;
      memcpy(&v, cur, 8);
      v ^= 0x3030303030303030ull;
      if (!all_digits(v)) break;
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ffull;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffffull;
      v = (v * 10000 + (v >> 32)) & 0xffffffffull;
      if (i != 0) x *= 100000000;
      x += v, cur += 8;
    }

    uint32_t v;
    memcpy(&v, cur, 4);
    v ^= 0x30303030;
    uint32_t val = 0, pow = 1;
    if (all_digits(v)) {
      v = (v * 10 + (v >> 8)) & 0xff00ff;
      v = (v * 100 + (v >> 16)) & 0xffff;
      val = v, pow = 10000, cur += 4;
    }

    for (; *cur >= '0'; ++cur) {
      val = val * 10 + (*cur & 15), pow *= 10;
    }

    x = x * pow + val;

    ++cur;
    return x;
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
  constexpr bool all_digits(uint32_t v) {
    return !(v & 0xf0f0f0f0);
  }

  constexpr bool all_digits(uint64_t v) {
    return !(v & 0xf0f0f0f0f0f0f0f0ull);
  }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 19>
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
    if (x > 9999'9999) {
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
    if (x > 9999'9999'9999'9999ull) {
      print<4>(x);
    } else if (x > 9999'9999'9999ull) {
      print<3>(x);
    } else if (x > 9999'9999) {
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
      print_E19(static_cast<uint64_t>(low));
    } else [[unlikely]] {
      auto high = x / E38;
      x -= high * E38;
      auto mid = x / E19;
      auto low = x - mid * E19;
      write(static_cast<uint32_t>(high));
      print_E19(static_cast<uint64_t>(mid));
      print_E19(static_cast<uint64_t>(low));
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

  template <bool head>
  void print_unit(uint32_t x) {
    if constexpr (head) {
      memcpy(cur, &LUT.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    } else {
      memcpy(cur, &LUT.second[x], 4);
      cur += 4;
    }
  }

  template <int N, bool head = true, typename T>
  void print(T x) {
    if constexpr (N == 0) {
      print_unit<head>(x);
    } else {
      print<N - 1, head>(x / 10000);
      print_unit<false>(x % 10000);
    }
  }

  void print_E19(uint64_t x) {
    auto high = static_cast<uint32_t>(x / E16);
    auto low = x - high * E16;
    memcpy(cur, &LUT.second[high][1], 3);
    cur += 3;
    print<3, false>(low);
  }
};

template <uint32_t InputBufSize = 1 << 20, uint32_t OutputBufSize = 1 << 19>
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

using namespace std;

using ll  = long long;
using ull = unsigned long long;

constexpr int N = 5e5 + 5;

FastIO<1 << 20, 1 << 19> io;

uint64_t sum[N];

void solve_main() {
  int n, q;
  io >> n >> q;

  for (int i = 1; i <= n; ++i) {
    sum[i] = io.in->read<uint32_t>();
  }

  partial_sum(sum + 1, sum + n + 1, sum + 1);

  while (q--) {
    uint32_t l = io.in->read_small<uint32_t>();
    uint32_t r = io.in->read_small<uint32_t>();
    io << sum[r] - sum[l] << '\n';
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