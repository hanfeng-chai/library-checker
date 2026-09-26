#pragma onece

// clang-format off

#include <immintrin.h>
#include <bits/stdc++.h>

using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using i128 = __int128_t;
using u128 = __uint128_t;

template<typename T> using vec = std::vector<T>;
template<typename T> using vvec = std::vector<std::vector<T>>;
template<typename T> using vvvec = std::vector<std::vector<std::vector<T>>>;

__attribute__((target("popcnt"))) inline int popcnt(const u32 &a) { return _mm_popcnt_u32(a); }
__attribute__((target("popcnt"))) inline int popcnt64(const u64 &a) { return _mm_popcnt_u64(a); }
inline int ctz32(const u32 &a) { return a ? __builtin_ctz(a) : 32; }
inline int ctz64(const u64 &a) { return a ? __builtin_ctzll(a) : 64; }
inline int msb32(const u32 &a) { return a ? 31 - __builtin_clz(a) : -1; }
inline int msb64(const u64 &a) { return a ? 63 - __builtin_clzll(a) : -1; }
template <typename T> inline int gbit(const T &a, int i) { return (a >> i) & 1; }
template <typename T> inline void sbit(T &a, int i, bool b) { if (gbit(a, i) != b) a ^= T(1) << i; }
constexpr u64 pow_bit(int n) { return 1ull << n; }
constexpr u64 mask_bit(int n) { return (1ull << n) - 1; }

namespace IO {

class Scanner {
  static constexpr int buf_size = (1 << 19);
  static constexpr int integer_size = 20;
  static constexpr int string_size = (int)5e5 + 5;  // default
  char buf[buf_size] = {};
  char *cur = buf, *ed = buf;

 public:
  Scanner() {}

  template <class T>
  inline Scanner& operator>>(T& val) {
    read(val);
    return *this;
  }

 private:
  inline void reload() {
    size_t len = ed - cur;
    memmove(buf, cur, len);
    char* tmp = buf + len;
    ed = tmp + fread(tmp, 1, buf_size - len, stdin);
    *ed = 0;
    cur = buf;
  }

  inline void skip_space() {
    while (true) {
      if (cur == ed) reload();
      while (*cur == ' ' || *cur == '\n') ++cur;
      if (__builtin_expect(cur != ed, 1)) return;
    }
  }

  template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
  inline void read(T& num) {
    skip_space();
    if (cur + integer_size >= ed) reload();
    bool neg = false;
    num = 0;
    if (*cur == '-') neg = true, ++cur;
    while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
    if (neg) num = -num;
  }

  template <class T, std::enable_if_t<std::is_same<T, i64>::value, int> = 0>
  inline void read(T& num) {
    skip_space();
    if (cur + integer_size >= ed) reload();
    bool neg = false;
    num = 0;
    if (*cur == '-') neg = true, ++cur;
    while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
    if (neg) num = -num;
  }

  template <class T,
            std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
  inline void read(T& str) {
    skip_space();
    if (cur + str.size() >= ed) reload();
    auto it = cur;
    while (!(*cur == ' ' || *cur == '\n')) ++cur;
    str = std::string(it, cur);
  }

  template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
  inline void read(T& c) {
    skip_space();
    if (cur + 1 >= ed) reload();
    c = *cur, ++cur;
  }

  template <class T, std::enable_if_t<std::is_same<T, double>::value, int> = 0>
  inline void read(T& num) {
    skip_space();
    if (cur + integer_size >= ed) reload();
    bool neg = false;
    num = 0;
    if (*cur == '-') neg = true, ++cur;
    while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
    if (*cur != '.') return;
    ++cur;
    T base = 0.1;
    while (*cur >= '0' && *cur <= '9') {
      num += base * (*cur ^ 48);
      ++cur;
      base *= 0.1;
    }
    if (neg) num = -num;
  }

  template <class T,
            std::enable_if_t<std::is_same<T, long double>::value, int> = 0>
  inline void read(T& num) {
    skip_space();
    if (cur + integer_size >= ed) reload();
    bool neg = false;
    num = 0;
    if (*cur == '-') neg = true, ++cur;
    while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
    if (*cur != '.') return;
    ++cur;
    T base = 0.1;
    while (*cur >= '0' && *cur <= '9') {
      num += base * (*cur ^ 48);
      ++cur;
      base *= 0.1;
    }
    if (neg) num = -num;
  }

  template <class T>
  inline void read(std::vector<T>& vec) {
    for (T& e : vec) read(e);
  }

  template <class T, class U>
  inline void read(std::pair<T, U>& p) {
    read(p.first, p.second);
  }

  template <class Tuple, std::size_t... Is>
  inline void tuple_scan(Tuple& tp, std::index_sequence<Is...>) {
    (read(std::get<Is>(tp)), ...);
  }

  template <class... Args>
  inline void read(std::tuple<Args...>& tp) {
    tuple_scan(tp, std::index_sequence_for<Args...>{});
  }

  inline void read() {}

  template <class Head, class... Tail>
  inline void read(Head&& head, Tail&&... tail) {
    read(head);
    read(std::forward<Tail>(tail)...);
  }
};

class Printer {
  static constexpr int buf_size = (1 << 18);
  static constexpr int integer_size = 20;
  static constexpr int string_size = (1 << 6);
  static constexpr int margin = 1;
  static constexpr int n = 10000;
  char buf[buf_size + margin] = {};
  char table[n * 4] = {};
  char* cur = buf;

 public:
  constexpr Printer() { build(); }

  ~Printer() { flush(); }

  template <class T>
  inline Printer& operator<<(T val) {
    write(val);
    return *this;
  }

  template <class T>
  inline void println(T val) {
    write(val);
    write('\n');
  }

 private:
  constexpr void build() {
    for (int i = 0; i < 10000; ++i) {
      int tmp = i;
      for (int j = 3; j >= 0; --j) {
        table[i * 4 + j] = tmp % 10 + '0';
        tmp /= 10;
      }
    }
  }

  inline void flush() {
    fwrite(buf, 1, cur - buf, stdout);
    cur = buf;
  }

  template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
  inline int get_digit(T n) {
    if (n >= (int)1e5) {
      if (n >= (int)1e8) return 9;
      if (n >= (int)1e7) return 8;
      if (n >= (int)1e6) return 7;
      return 6;
    } else {
      if (n >= (int)1e4) return 5;
      if (n >= (int)1e3) return 4;
      if (n >= (int)1e2) return 3;
      if (n >= (int)1e1) return 2;
      return 1;
    }
  }

  template <class T, std::enable_if_t<std::is_same<T, i64>::value, int> = 0>
  inline int get_digit(T n) {
    if (n >= (i64)1e10) {
      if (n >= (i64)1e14) {
        if (n >= (i64)1e18) return 19;
        if (n >= (i64)1e17) return 18;
        if (n >= (i64)1e16) return 17;
        if (n >= (i64)1e15) return 16;
        return 15;
      } else {
        if (n >= (i64)1e14) return 15;
        if (n >= (i64)1e13) return 14;
        if (n >= (i64)1e12) return 13;
        if (n >= (i64)1e11) return 12;
        return 11;
      }
    } else {
      if (n >= (i64)1e5) {
        if (n >= (i64)1e9) return 10;
        if (n >= (i64)1e8) return 9;
        if (n >= (i64)1e7) return 8;
        if (n >= (i64)1e6) return 7;
        return 6;
      } else {
        if (n >= (i64)1e4) return 5;
        if (n >= (i64)1e3) return 4;
        if (n >= (i64)1e2) return 3;
        if (n >= (i64)1e1) return 2;
        return 1;
      }
    }
  }

  template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
  inline void write(T num) {
    if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
    if (num == 0) {
      write('0');
      return;
    }
    if (num < 0) {
      write('-');
      num = -num;
    }
    int len = get_digit(num);
    int digits = len;
    while (num >= 10000) {
      memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
      num /= 10000;
      len -= 4;
    }
    memcpy(cur, table + num * 4 + (4 - len), len);
    cur += digits;
  }

  template <class T, std::enable_if_t<std::is_same<T, i64>::value, int> = 0>
  inline void write(T num) {
    if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
    if (num == 0) {
      write('0');
      return;
    }
    if (num < 0) {
      write('-');
      num = -num;
    }
    int len = get_digit(num);
    int digits = len;
    while (num >= 10000) {
      memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
      num /= 10000;
      len -= 4;
    }
    memcpy(cur, table + num * 4 + (4 - len), len);
    cur += digits;
  }

  template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
  inline void write(T c) {
    if (__builtin_expect(cur + 1 >= buf + buf_size, 0)) flush();
    *cur = c;
    ++cur;
  }

  template <class T,
            std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
  inline void write(T str) {
    if (__builtin_expect(cur + str.size() >= buf + buf_size, 0)) flush();
    for (char c : str) write(c);
  }

  template <class T,
            std::enable_if_t<std::is_same<T, const char*>::value, int> = 0>
  inline void write(T str) {
    if (__builtin_expect(cur + string_size >= buf + buf_size, 0)) flush();
    for (int i = 0; str[i]; ++i) write(str[i]);
  }
};

}  // namespace IO

IO::Scanner fin;
IO::Printer fout;

// clang-format on

template <class It, class ItEnd>
std::array<vec<int>, 2> manacher(It it, ItEnd it_end) {
  std::array p = {std::vector(int(it_end - it) + 1, 0),
                  std::vector(int(it_end - it), 0)};
  for (int z = 0; z < 2; ++z) {
    for (int i = 0, l = 0, r = 0, n = int(p[1].size()); i < n; ++i) {
      int t = r - i + !z;
      if (i < r) p[z][i] = std::min(t, p[z][l + t]);
      int l2 = i - p[z][i];
      int r2 = i + p[z][i] - !z;
      while (l2 && r2 + 1 < n && it[l2 - 1] == it[r2 + 1]) {
        ++p[z][i];
        --l2;
        ++r2;
      }
      if (r2 > r) {
        l = l2;
        r = r2;
      }
    }
  }
  return p;
}

int main() {
  std::string s;
  fin >> s;
  auto [even, odd] = manacher(begin(s), end(s));
  auto it_e = begin(even) + 1;
  auto it_o = begin(odd);
  for (int i = 0; i < 2 * int(s.size()) - 1; ++i)
    fout << (i % 2 ? 2 * *it_e++ : 2 * *it_o++ + 1) << " ";
  fout << "\n";
}