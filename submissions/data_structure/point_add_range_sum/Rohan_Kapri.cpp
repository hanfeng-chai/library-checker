

#define NDEBUG

#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("avx2", "bmi", "bmi2", "popcnt", "lzcnt")

#include <utility>
#include <cstdint>
#include <cstring>
#include <string>

#include <x86intrin.h>

const int MAX_N = 500000;

#ifndef __OY_LINUXIO__
#define __OY_LINUXIO__
#include <sys/mman.h>
#include <sys/stat.h>

#define cin OY::LinuxIO::InputHelper<>::get_instance()
#define cout OY::LinuxIO::OutputHelper::get_instance()
#define endl '\n'
#ifndef INPUT_FILE
#define INPUT_FILE "in.txt"
#endif
#ifndef OUTPUT_FILE
#define OUTPUT_FILE "out.txt"
#endif
namespace OY {
namespace LinuxIO {
static constexpr size_t INPUT_BUFFER_SIZE = 1 << 26, OUTPUT_BUFFER_SIZE = 1 << 20;
#ifdef OY_LOCAL
static constexpr char input_file[] = INPUT_FILE, output_file[] = OUTPUT_FILE;
#else
static constexpr char input_file[] = "", output_file[] = "";
#endif
template <typename U, size_t E> struct TenPow {
  static constexpr U value = TenPow<U, E - 1>::value * 10;
};
template <typename U> struct TenPow<U, 0> {
  static constexpr U value = 1;
};
struct InputPre {
  uint32_t m_data[0x10000];
  constexpr InputPre() : m_data{} {
    std::fill(m_data, m_data + 0x10000, -1);
    for (size_t i = 0, val = 0; i != 10; i++)
      for (size_t j = 0; j != 10; j++)
        m_data[0x3030 + i + (j << 8)] = val++;
  }
};
struct OutputPre {
  uint32_t m_data[10000];
  constexpr OutputPre() : m_data{} {
    uint32_t *c = m_data;
    for (size_t i = 0; i != 10; i++)
      for (size_t j = 0; j != 10; j++)
        for (size_t k = 0; k != 10; k++)
          for (size_t l = 0; l != 10; l++)
            *c++ = i + (j << 8) + (k << 16) + (l << 24) + 0x30303030;
  }
};
template <size_t MMAP_SIZE = 1 << 30> struct InputHelper {
  static constexpr InputPre pre{};
  struct stat m_stat;
  char *m_p, *m_c;
  InputHelper(FILE *file = stdin) {
#ifdef __unix__
    auto fd = fileno(file);
    fstat(fd, &m_stat);
    m_c = m_p = (char *)mmap(nullptr, m_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
#else
    fread(m_c = m_p = new char[INPUT_BUFFER_SIZE], 1, INPUT_BUFFER_SIZE, file);
#endif
  }
  static InputHelper<MMAP_SIZE> &get_instance() {
    static InputHelper<MMAP_SIZE> s_obj(*input_file ? fopen(input_file, "rt") : stdin);
    return s_obj;
  }
  template <typename Tp,
            typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
  InputHelper &operator>>(Tp &x) {
    x = 0;
    while (!isdigit(*m_c))
      m_c++;
    x = *m_c++ ^ '0';
    while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)])
      x = x * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
    if (isdigit(*m_c))
      x = x * 10 + (*m_c++ ^ '0');
    return *this;
  }
  template <typename Tp,
            typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
  InputHelper &operator>>(Tp &x) {
    typename std::make_unsigned<Tp>::type t{};
    bool sign{};
    while (!isdigit(*m_c))
      sign = (*m_c++ == '-');
    t = *m_c++ ^ '0';
    while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)])
      t = t * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
    if (isdigit(*m_c))
      t = t * 10 + (*m_c++ ^ '0');
    x = sign ? -t : t;
    return *this;
  }
  InputHelper &operator>>(char &x) {
    while (*m_c <= ' ')
      m_c++;
    x = *m_c++;
    return *this;
  }
  InputHelper &operator>>(std::string &x) {
    while (*m_c <= ' ')
      m_c++;
    char *c = m_c;
    while (*c > ' ')
      c++;
    x.assign(m_c, c - m_c), m_c = c;
    return *this;
  }
  InputHelper &operator>>(std::string_view &x) {
    while (*m_c <= ' ')
      m_c++;
    char *c = m_c;
    while (*c > ' ')
      c++;
    x = std::string_view(m_c, c - m_c), m_c = c;
    return *this;
  }
};
struct OutputHelper {
  static constexpr OutputPre pre{};
  FILE *m_file;
  char m_p[OUTPUT_BUFFER_SIZE], *m_c, *m_end;
  OutputHelper(FILE *file = stdout) {
    m_file = file;
    m_c = m_p, m_end = m_p + OUTPUT_BUFFER_SIZE;
  }
  ~OutputHelper() { flush(); }
  static OutputHelper &get_instance() {
    static OutputHelper s_obj(*output_file ? fopen(output_file, "wt") : stdout);
    return s_obj;
  }
  void flush() { fwrite(m_p, 1, m_c - m_p, m_file), m_c = m_p; }
  OutputHelper &operator<<(char x) {
    if (m_end - m_c < 20)
      flush();
    *m_c++ = x;
    return *this;
  }
  OutputHelper &operator<<(const std::string &s) {
    if (m_end - m_c < s.size())
      flush();
    memcpy(m_c, s.data(), s.size()), m_c += s.size();
    return *this;
  }
  OutputHelper &operator<<(uint64_t x) {
    if (m_end - m_c < 20)
      flush();
#define CASEW(w)                                                                                                       \
  case TenPow<uint64_t, w - 1>::value... TenPow<uint64_t, w>::value - 1:                                               \
    *(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, w - 4>::value];                                                 \
    m_c += 4, x %= TenPow<uint64_t, w - 4>::value;
    switch (x) {
      CASEW(19);
      CASEW(15);
      CASEW(11);
      CASEW(7);
    case 100 ... 999:
      *(uint32_t *)m_c = pre.m_data[x * 10];
      m_c += 3;
      break;
      CASEW(18);
      CASEW(14);
      CASEW(10);
      CASEW(6);
    case 10 ... 99:
      *(uint32_t *)m_c = pre.m_data[x * 100];
      m_c += 2;
      break;
      CASEW(17);
      CASEW(13);
      CASEW(9);
      CASEW(5);
    case 0 ... 9:
      *m_c++ = '0' + x;
      break;
    default:
      *(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, 16>::value];
      m_c += 4;
      x %= TenPow<uint64_t, 16>::value;
      CASEW(16);
      CASEW(12);
      CASEW(8);
    case 1000 ... 9999:
      *(uint32_t *)m_c = pre.m_data[x];
      m_c += 4;
      break;
    }
#undef CASEW
    return *this;
  }
  OutputHelper &operator<<(uint32_t x) {
    if (m_end - m_c < 20)
      flush();
#define CASEW(w)                                                                                                       \
  case TenPow<uint32_t, w - 1>::value... TenPow<uint32_t, w>::value - 1:                                               \
    *(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, w - 4>::value];                                                 \
    m_c += 4, x %= TenPow<uint32_t, w - 4>::value;
    switch (x) {
    default:
      *(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, 6>::value];
      m_c += 4;
      x %= TenPow<uint32_t, 6>::value;
      CASEW(6);
    case 10 ... 99:
      *(uint32_t *)m_c = pre.m_data[x * 100];
      m_c += 2;
      break;
      CASEW(9);
      CASEW(5);
    case 0 ... 9:
      *m_c++ = '0' + x;
      break;
      CASEW(8);
    case 1000 ... 9999:
      *(uint32_t *)m_c = pre.m_data[x];
      m_c += 4;
      break;
      CASEW(7);
    case 100 ... 999:
      *(uint32_t *)m_c = pre.m_data[x * 10];
      m_c += 3;
      break;
    }
#undef CASEW
    return *this;
  }
  OutputHelper &operator<<(int64_t x) {
    if (x >= 0)
      return (*this) << uint64_t(x);
    else
      return (*this) << '-' << uint64_t(-x);
  }
  OutputHelper &operator<<(int32_t x) {
    if (x >= 0)
      return (*this) << uint32_t(x);
    else
      return (*this) << '-' << uint32_t(-x);
  }
};
} // namespace LinuxIO
} // namespace OY
#endif

typedef uint64_t v4ull __attribute__((vector_size(32)));

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/
template <int N, int b = 4> struct WideSegmentTree {
  constexpr static int B = 1 << b;

  struct Precalc {
    alignas(64) uint64_t mask[B][B];

    constexpr Precalc() : mask{} {
      for (int k = 0; k < B; k++)
        for (int i = 0; i < B; i++)
          mask[k][i] = (i > k ? -1 : 0);
    }
  };

  constexpr static Precalc T{};

  constexpr static int height(int n) { return (n < B ? 1 : height(n / B) + 1); }

  constexpr static int offset(int h) {
    int s = 0, n = N;
    while (h--) {
      n = (n + B - 1) / B;
      s += n * B + B;
    }
    return s;
  }

  constexpr static int H = height(N);

  alignas(64) uint64_t data[offset(H)];

  void build() {
#pragma unroll(1024)
    for (int h = 0; h < H - 1; h++) {
      for (int i = 0; i < offset(h + 1) - offset(h) - B; i += B) {
        for (int j = 0; j < B; j++)
          data[offset(h) + i + j + 1] += data[offset(h) + i + j];
        data[offset(h + 1) + (i >> b) + 1] = std::exchange(data[offset(h) + i + B], 0);
      }
    }

    for (int i = 0; i < B - 1; i++)
      data[offset(H - 1) + i + 1] += data[offset(H - 1) + i];
  }

  uint64_t sum(int k) const {
    uint64_t res = 0;
    for (int h = H - 1; h >= 0; h--)
      res += data[offset(h) + (k >> (h * b))];
    return res;
  }

  void add(int k, int _x) {
    v4ull x = _x + v4ull{};
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v4ull *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v4ull *)T.mask[p & (B - 1)];
      for (int i = 0; i < B / 4; i++)
        l[i] += x & m[i];
    }
  }
};

WideSegmentTree<MAX_N> s;

int main() {
  uint32_t n, q;
  cin >> n >> q;

  for (int i = 0; i < n; i++)
    cin >> s.data[s.offset(0) + i + 1];
  s.build();

  for (int i = 0; i < q; i++) {
    char t;
    cin >> t;

    if (t == '0') {
      uint32_t p, x;
      cin >> p >> x;
      s.add(p, x);
    } else {
      uint32_t l, r;
      cin >> l >> r;
      cout << s.sum(r) - s.sum(l) << '\n';
    }
  }
}
