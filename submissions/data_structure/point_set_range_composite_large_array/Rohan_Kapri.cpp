

#pragma GCC optimize(3, "unroll-loops")
#include <bits/stdc++.h>

namespace debug {

} 

#ifdef LOCAL
#define CHECK(expr) assert(expr)
#else
#define CHECK(expr) void(0)
#endif

namespace internal {

template <typename T>
struct make_unsigned : std::make_unsigned<std::remove_cvref_t<T>> {};

#ifdef __SIZEOF_INT128__

template <typename T>
  requires(std::same_as<std::remove_cvref_t<T>, __int128_t> ||
           std::same_as<std::remove_cvref_t<T>, __uint128_t>)
struct make_unsigned<T> {
  using type = __uint128_t;
};

template <typename T>
concept signed_integral = std::signed_integral<std::remove_cvref_t<T>> ||
                          std::same_as<std::remove_cvref_t<T>, __int128_t>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<std::remove_cvref_t<T>> ||
                            std::same_as<std::remove_cvref_t<T>, __uint128_t>;

template <typename T>
concept integral = signed_integral<T> || unsigned_integral<T>;

#else

template <typename T>
concept signed_integral = std::signed_integral<std::remove_cvref_t<T>>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<std::remove_cvref_t<T>>;

template <typename T>
concept integral = std::integral<std::remove_cvref_t<T>>;

#endif

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
  static constexpr size_t Offset = 64;

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
    cur = buf = new char[BufSize + Offset];
    end = buf + fread(buf, 1, BufSize, file);
    memset(const_cast<char*>(end), 0, Offset);
#endif
  }

#ifdef ENABLE_MMAP
  ~FastInput() { munmap(buf, map_size); }
#else
  ~FastInput() { delete[] buf; }
#endif

#ifdef ENABLE_MMAP
  template <int N>
  void ensure() {}
#else
  template <int N>
  void ensure() {
    int rem = end - cur;
    if (rem >= N) [[likely]]
      return;
    if (rem > 0 && cur != buf) memmove(buf, cur, rem);
    cur = buf;
    end = buf + rem + fread(buf + rem, 1, BufSize - rem, file);
    memset(const_cast<char*>(end), 0, Offset);
  }
#endif

  void skip_space() {
    ensure<1>();
    while (*cur < 33) [[unlikely]] {
      ++cur;
      ensure<1>();
      CHECK(cur < end);
    }
  }

  void read(bool& x) {
    CHECK(*cur == '0' || *cur == '1');
    x = (*cur++ == '1');
    ensure<1>();
    CHECK(*cur <= 32);
    ++cur;
  }

  template <internal::unsigned_integral T,
            int N = std::numeric_limits<T>::digits10 / 8>
  void read(T& x) {
    ensure<40>();
    CHECK(*cur >= '0' && *cur <= '9');
    x = *cur++ & 15;

    for (int i = 0; i < N; ++i) {
      uint64_t v;
      memcpy(&v, cur, 8);
      if ((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) break;
      v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
      v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
      v = (v * 10000 + (v >> 32)) & 0xffffffff;
      x = x * 100000000 + v;
      cur += 8;
    }

    for (; *cur >= '0'; ++cur) {
      CHECK(*cur <= '9');
      x = x * 10 + (*cur & 15);
    }

    CHECK(*cur <= 32);
    ++cur;
  }

  template <internal::signed_integral T,
            int N = std::numeric_limits<T>::digits10 / 8>
  void read(T& x) {
    using U = internal::make_unsigned_t<T>;

    bool neg = (*cur == '-');
    cur += neg;

    U v;
    read<U, N>(v);
    x = static_cast<T>(neg ? -v : v);
  }

  void read(char& c) {
    c = *cur++;
    ensure<1>();
    CHECK(*cur <= 32);
    ++cur;
  }

  void read(std::string& s) {
    CHECK(*cur > 32);
#ifdef ENABLE_MMAP
    char* first = cur;
    while (*cur > 32) ++cur;
    s.assign(first, cur);
    ++cur;
#else
    s.clear();
    while (true) {
      char* last = cur;
      while (last < end && *last > 32) ++last;
      if (last < end) {
        s.append(cur, last);
        cur = last + 1;
        return;
      } else {
        s.append(cur, last);
        cur = end;
        ensure<1>();
      }
    }
#endif
  }

  template <typename T>
  FastInput& operator>>(T& x) {
    skip_space();
    read(x);
    return *this;
  }

  FastInput& operator>>(char* s) {
    skip_space();
    while (*cur > 32) {
      *s++ = *cur++;
      ensure<1>();
    }
    *s = 0;
    ++cur;
    return *this;
  }
};

struct EndLine {
} endl;

template <uint32_t BufSize = 1 << 20>
struct FastOutput {
  FILE* file;
  char* buf;
  char* cur;
  char* end;

  static constexpr auto table = [] {
    std::array<std::array<char, 4>, 10000> res1;
    std::array<std::array<char, 4>, 10000> res2;

    for (int i = 0; i < 10000; ++i) {
      res2[i][0] = '0' + i / 1000;
      res2[i][1] = '0' + i / 100 % 10;
      res2[i][2] = '0' + i / 10 % 10;
      res2[i][3] = '0' + i % 10;

      int j = 0;
      if (i >= 1000) res1[i][j++] = res2[i][0];
      if (i >= 100) res1[i][j++] = res2[i][1];
      if (i >= 10) res1[i][j++] = res2[i][2];
      res1[i][j] = res2[i][3];
    }

    return std::make_pair(res1, res2);
  }();

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

  template <bool head = false>
  void print_unit(uint32_t x) {
    if constexpr (head) {
      memcpy(cur, &table.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    } else {
      memcpy(cur, &table.second[x], 4);
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
    static constexpr uint64_t kE16 = 1'0000'0000'0000'0000ull;
    static constexpr __uint128_t kE32 =
        static_cast<__uint128_t>(kE16) * static_cast<__uint128_t>(kE16);

    if (x < kE16) {
      write(static_cast<uint64_t>(x));
    } else if (x < kE32) {
      write(static_cast<uint64_t>(x / kE16));
      print<3, false>(static_cast<uint64_t>(x % kE16));
    } else {
      write(static_cast<uint32_t>(x / kE32));
      x %= kE32;
      print<3, false>(static_cast<uint64_t>(x / kE16));
      print<3, false>(static_cast<uint64_t>(x % kE16));
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

  FastOutput& operator<<(char c) {
    flush<1>();
    *cur++ = c;
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
    } else if (end - cur < len) [[unlikely]] {
      flush();
    }
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
struct SegTree {
  using Info = typename T::Info;

  int n = 0;
  int m = 0;
  std::vector<Info> t;

  SegTree() = default;
  explicit SegTree(int _m) { build(_m); }

  template <typename F>
  SegTree(int _m, F&& func) { build(_m, func); }

  template <typename It>
  SegTree(It first, It last) { build(first, last); }

  void build(int _m) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
  }

  template <typename F>
  void build(int _m, F&& func) {
    m = _m;
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    for (int i = 0; i < m; ++i) t[n + i] = func(i);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  template <typename It>
  void build(It first, It last) {
    m = std::distance(first, last);
    CHECK(m >= 0);
    n = 1;
    while (n < m) n <<= 1;
    t.assign(n << 1, T::id());
    std::copy(first, last, t.begin() + n);
    for (int i = n - 1; i > 0; --i) push_up(i);
  }

  void reset() {
    std::fill(t.begin(), t.end(), T::id());
  }

  void set(int x, const Info& v) {
    CHECK(0 <= x && x < m);
    x += n;
    t[x] = v;
    while (x >>= 1) push_up(x);
  }

  Info get(int x) const {
    CHECK(0 <= x && x < m);
    return t[n + x];
  }

  Info prod(int l, int r) const {
    CHECK(0 <= l && l <= r && r <= m);
    return evaluate(l, r, T::id(), [&](const Info& x, const Info& y) {
      return T::op(x, y);
    });
  }

  Info all_prod() const { return t[1]; }

  template <typename Value, typename F>
  Value evaluate(int l, int r, Value val, F&& func) const {
    CHECK(0 <= l && l <= r && r <= m);
    l += n - 1, r += n;
    int w = internal::__lg(l ^ r);

    int cl = l;
    l = ~l & ((1 << w) - 1);
    while (l > 0) {
      int i = internal::countr_zero(l);
      l ^= 1 << i;
      val = func(val, t[cl >> i ^ 1]);
    }

    int cr = r;
    r &= (1 << w) - 1;
    while (r > 0) {
      int i = internal::__lg(r);
      r ^= 1 << i;
      val = func(val, t[cr >> i ^ 1]);
    }

    return val;
  }

 private:
  void push_up(int x) {
    t[x] = T::op(t[x << 1], t[x << 1 | 1]);
  }
};

template <typename T, uint32_t N = 16, typename It, typename F>
void radix_sort_u32(It f, int n, F&& func) {
  CHECK(n >= 0);

  static constexpr uint32_t Mask = (1u << N) - 1;

  if (Mask / n > 8) {
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

using namespace std;

using ll  = long long;
using ull = unsigned long long;

FastIO<1 << 20, 1 << 20> io;

constexpr int P = 998244353;

struct PointSetRangeComposite {
  using Info = pair<uint32_t, uint32_t>;

  static constexpr Info id() noexcept { return {1, 0}; };
  static constexpr Info op(const Info& lhs, const Info& rhs) {
    return {static_cast<uint64_t>(lhs.first) * rhs.first % P,
            (static_cast<uint64_t>(lhs.second) * rhs.first + rhs.second) % P};
  }
};

void solve_main() {
  int n, q;
  io >> n >> q;

  vector<tuple<bool, uint32_t, uint32_t, uint32_t>> qry(q);
  vector<uint64_t> aux;
  aux.reserve(q << 1);

  for (int i = 0; i < q; ++i) {
    auto& [op, a, b, c] = qry[i];
    io >> op >> a >> b >> c;
    if (!op) {
      aux.push_back(static_cast<uint64_t>(i << 2) << 32 | a);
    } else {
      aux.push_back(static_cast<uint64_t>(i << 2 | 1) << 32 | a);
      aux.push_back(static_cast<uint64_t>(i << 2 | 2) << 32 | b);
    }
  }

  radix_sort_u32<uint64_t, 15>(aux.data(), aux.size(), [&](auto x) {
    return static_cast<uint32_t>(x);
  });

  uint32_t lst = -1u;
  int m = 0;
  for (auto val : aux) {
    uint32_t x = static_cast<uint32_t>(val);
    uint32_t y = val >> 32;
    auto& t = qry[y >> 2];
    if (y & 3) {
      (y & 1 ? get<1>(t) : get<2>(t)) = m - (x == lst);
    } else {
      m += (x != lst);
      lst = x;
      get<1>(t) = m - 1;
    }
  }

  SegTree<PointSetRangeComposite> t(m);

  auto eval = [&](uint32_t x, const auto& info) -> uint32_t {
    return (static_cast<uint64_t>(x) * info.first + info.second) % P;
  };

  for (const auto& [op, a, b, c] : qry) {
    if (op == 0) {
      t.set(a, {b, c});
    } else {
      io << t.evaluate(a, b, c, eval) << '\n';
    }
  }
}

int main() {
#ifdef LOCAL
  assert(freopen("test.in", "r", stdin));
  assert(freopen("test.out", "w", stdout));
#endif

  io.init();

  int T;

  T = 1;

  while (T--) {
    solve_main();
  }

  return 0;
}