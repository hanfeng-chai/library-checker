#define NDEBUG

#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("avx2", "bmi", "bmi2", "popcnt", "lzcnt")

#include <array>
#include <cstdint>
#include <cstring>
#include <string>

#include <x86intrin.h>

const int MAX_N = 500'000;
const int MAX_Q = 500'000;

/*nor's fastio: https://judge.yosupo.jp/submission/75233*/ /* clang-format off */struct IOPre { static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN; std::array<char, 4 * SZ> num; constexpr IOPre() : num{} { for (int i = 0; i < SZ; i++) { int n = i; for (int j = 3; j >= 0; j--) { num[i * 4 + j] = static_cast<char>(n % TEN + '0'); n /= TEN; } } } }; struct IO { static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN, MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16; static constexpr IOPre io_pre = {}; std::array<char, SZ> input_buffer, output_buffer; int input_ptr_left, input_ptr_right, output_ptr_right; IO() : input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {} IO(const IO &) = delete; IO(IO &&) = delete; IO &operator=(const IO &) = delete; IO &operator=(IO &&) = delete; ~IO() { flush(); } template <class T> struct is_char { static constexpr bool value = std::is_same_v<T, char>; }; template <class T> struct is_bool { static constexpr bool value = std::is_same_v<T, bool>; }; template <class T> struct is_string { static constexpr bool value = std::is_same_v<T, std::string> || std::is_same_v<T, const char *> || std::is_same_v<T, char *> || std::is_same_v<std::decay_t<T>, char *>; ; }; template <class T, class D = void> struct is_custom { static constexpr bool value = false; }; template <class T> struct is_custom<T, std::void_t<typename T::internal_value_type>> { static constexpr bool value = true; }; template <class T> struct is_default { static constexpr bool value = is_char<T>::value || is_bool<T>::value || is_string<T>::value || std::is_integral_v<T>; }; template <class T, class D = void> struct is_iterable { static constexpr bool value = false; }; template <class T> struct is_iterable< T, typename std::void_t<decltype(std::begin(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T, class D = void, class E = void> struct is_applyable { static constexpr bool value = false; }; template <class T> struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>, std::void_t<decltype(std::get<0>(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T> static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value); template <typename T, typename U> struct any_needs_newline { static constexpr bool value = false; }; template <typename T> struct any_needs_newline<T, std::index_sequence<>> { static constexpr bool value = false; }; template <typename T, std::size_t I, std::size_t... Is> struct any_needs_newline<T, std::index_sequence<I, Is...>> { static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value; }; inline void load() { memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked( std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0; } inline void read_char(char &c) { if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++]; } inline void read_string(std::string &x) { char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c; } template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T &x) { if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true) if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0') x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true) if (minus) x = -x; } inline void skip_space() { if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++; } inline void flush() { fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0; } inline void write_char(char c) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c; } inline void write_bool(bool b) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0'; } inline void write_string(const std::string &s) { for (auto x : s) write_char(x); } inline void write_string(const char *s) { while (*s) write_char(*s++); } inline void write_string(char *s) { while (*s) write_char(*s++); } template <typename T> inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) { if (output_ptr_right > SZ - LEN) flush(); if (!x) { output_buffer[output_ptr_right++] = '0'; return; } if constexpr (std::is_signed<T>::value == true) if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) { memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4; } if (x < HUNDRED) { if (x < TEN) { output_buffer[output_ptr_right++] = static_cast<char>('0' + x); } else { std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2; } } else { if (x < THOUSAND) { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3; } else { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4; } } memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i; } template <typename T_> IO &operator<<(T_ &&x) { using T = typename std::remove_cv<typename std::remove_reference<T_>::type>::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) { write_int(x.get()); } else if constexpr (is_default<T>::value) { if constexpr (is_bool<T>::value) { write_bool(x); } else if constexpr (is_string<T>::value) { write_string(x); } else if constexpr (is_char<T>::value) { write_char(x); } else if constexpr (std::is_integral_v<T>) { write_int(x); } } else if constexpr (is_iterable<T>::value) { using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto &y : x) { if (i++) write_char(sep); operator<<(y); } } else if constexpr (is_applyable<T>::value) { constexpr char sep = (any_needs_newline< T, std::make_index_sequence<std::tuple_size_v<T>>>::value) ? '\n' : ' '; int i = 0; std::apply( [this, &sep, &i](auto const &...y) { (((i++ ? write_char(sep) : void()), this->operator<<(y)), ...); }, x); } return *this; } template <typename T> IO &operator>>(T &x) { static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) { typename T::internal_value_type y; read_int(y); x = y; } else if constexpr (is_default<T>::value) { if constexpr (is_string<T>::value) { read_string(x); } else if constexpr (is_char<T>::value) { read_char(x); } else if constexpr (std::is_integral_v<T>) { read_int(x); } } else if constexpr (is_iterable<T>::value) { for (auto &y : x) operator>>(y); } else if constexpr (is_applyable<T>::value) { std::apply([this](auto &...y) { ((this->operator>>(y)), ...); }, x); } return *this; } IO *tie(std::nullptr_t) { return this; } void sync_with_stdio(bool) {} }; IO io;/* clang-format on */

#define cin io
#define cout io

typedef int v8si __attribute__((vector_size(32)));

inline uint32_t pdep(uint32_t x, uint32_t y) { return _pdep_u32(x, y); }

inline uint64_t pdep(uint64_t x, uint64_t y) { return _pdep_u64(x, y); }

template <int> int rank(const int *, __m256i);

template <> inline int rank<8>(const int *data, __m256i x) {
  __m256i y = _mm256_load_si256((__m256i *)data);
  __m256i c = _mm256_cmpgt_epi32(x, y);
  return __builtin_popcount(_mm256_movemask_ps(_mm256_castsi256_ps(c)));
}

template <> inline int rank<16>(const int *data, __m256i x) {
  __m256i y1 = _mm256_load_si256((__m256i *)data);
  __m256i y2 = _mm256_load_si256((__m256i *)(data + 8));

  __m256i c1 = _mm256_cmpgt_epi32(x, y1);
  __m256i c2 = _mm256_cmpgt_epi32(x, y2);

  __m256i p = _mm256_blend_epi16(c1, c2, 0b01010101);

  return __builtin_popcount(_mm256_movemask_epi8(p)) >> 1;
}

template <> inline int rank<32>(const int *data, __m256i x) {
  __m256i y1 = _mm256_load_si256((__m256i *)data);
  __m256i y2 = _mm256_load_si256((__m256i *)(data + 8));
  __m256i y3 = _mm256_load_si256((__m256i *)(data + 16));
  __m256i y4 = _mm256_load_si256((__m256i *)(data + 24));

  __m256i c1 = _mm256_cmpgt_epi32(x, y1);
  __m256i c2 = _mm256_cmpgt_epi32(x, y2);
  __m256i c3 = _mm256_cmpgt_epi32(x, y3);
  __m256i c4 = _mm256_cmpgt_epi32(x, y4);

  __m256i p1 = _mm256_blend_epi16(c1, c2, 0b01010101);
  __m256i p2 = _mm256_blend_epi16(c3, c4, 0b01010101);

  __m256i p = _mm256_packs_epi16(p1, p2);

  return __builtin_popcount(_mm256_movemask_epi8(p));
}

template <> inline int rank<64>(const int *data, __m256i x) { return rank<32>(data, x) + rank<32>(data + 32, x); }

template <int B> inline int rank(const int *data, int x) { return rank<B>(data, _mm256_set1_epi32(x)); }

// Based on https://en.algorithmica.org/hpc/algorithms/prefix/
inline void prefix_x4(int *p) {
  __m256i x = _mm256_load_si256((__m256i *)p);
  x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
  x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
  _mm256_store_si256((__m256i *)p, x);
}

inline __m128i update(int *p, __m128i s) {
  __m128i d = (__m128i)_mm_broadcast_ss((float *)&p[3]);
  __m128i x = _mm_load_si128((__m128i *)p);
  x = _mm_add_epi32(s, x);
  _mm_store_si128((__m128i *)p, x);
  return _mm_add_epi32(s, d);
}

template <int N, int B = 1 << 6> inline void prefix(int *a) {
  __m128i s = _mm_setzero_si128();

  for (int i = 0; i < B; i += 8)
    prefix_x4(&a[i]);

  for (int i = B; i < N; i += 8) {
    prefix_x4(&a[i]);
    s = update(&a[i - B], s);
    s = update(&a[i - B + 4], s);
  }

  for (int i = N - B; i < N; i += 4)
    s = update(&a[i], s);
}

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/
template <int N, int b = 5> struct WideSegmentTree {
  constexpr static int B = 1 << b;

  struct Precalc {
    alignas(64) int mask[B][B];

    constexpr Precalc() : mask{} {
      for (int k = 0; k < B; k++)
        for (int i = 0; i < B; i++)
          mask[k][i] = (i > k ? -1 : 0);
    }
  };

  constexpr static Precalc T{};

  constexpr static int height(int n) { return (n <= B ? 1 : height(n / B) + 1); }

  constexpr static int offset(int h) {
    int s = 0, n = N;
    while (h--) {
      n = (n + B - 1) / B;
      s += n * B + B; // extra padding to make build easier to implement
    }
    return s;
  }

  constexpr static int H = height(N);

  alignas(64) int data[offset(H)];

  inline void build() {
    for (int h = 0; h < H - 1; h++) {
      const int s = offset(h + 1) - offset(h) - B; // outer loop isn't unrolled properly without this

      for (int i = 0; i < s; i += B) {
        data[offset(h + 1) + (i >> b) + 1] = data[offset(h) + i + B];
        data[offset(h) + i + B] = 0;
      }

      for (int i = 0; i < s; i += 8)
        prefix_x4(&data[offset(h) + i]);

      for (int i = 0; i < s; i += B) {
        __m128i acc = _mm_setzero_si128();
        for (int j = 0; j < B; j += 4)
          acc = update(&data[offset(h) + i + j], acc);
      }

      for (int i = 0; i < s; i += B)
        data[offset(h + 1) + (i >> b) + 1] += data[offset(h) + i + B - 1];
    }

    for (int i = 0; i < B; i += 8)
      prefix_x4(&data[offset(H - 1) + i]);

    __m128i acc = _mm_setzero_si128();
    for (int i = 0; i < B; i += 4)
      acc = update(&data[offset(H - 1) + i], acc);
  }

  inline int sum(int k) const {
    int res = 0;
    for (int h = H - 1; h >= 0; h--)
      res += data[offset(h) + (k >> (h * b))];
    return res;
  }

  inline void add(int k, int _x) {
    v8si x = _x + v8si{};
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v8si *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v8si *)T.mask[p & (B - 1)];
      for (int i = 0; i < B / 8; i++)
        l[i] += x & m[i];
    }
  }

  inline void add_1(int k) {
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v8si *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v8si *)T.mask[p & (B - 1)];
      for (int i = 0; i < B / 8; i++)
        l[i] -= m[i];
    }
  }

  inline void sub_1(int k) {
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v8si *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v8si *)T.mask[p & (B - 1)];
      for (int i = 0; i < B / 8; i++)
        l[i] += m[i];
    }
  }

  inline std::pair<int, int> kth(int k) const {
    int x = 0;
    for (int h = H - 1; h >= 0; --h) {
      x <<= b;
      x += rank<B>(&data[offset(h) + x], k + 1) - 1;
      k -= data[offset(h) + x];
    }
    return {x, k};
  }
};

template <int N> struct OrderedSet {
  using T = uint64_t;
  constexpr static int b = 6, B = 1 << b;
  // using T = uint32_t;
  // constexpr static int b = 5, B = 1 << b;

  T bitset[(N + B - 1) / B];
  WideSegmentTree<(N >> b)> tree;

  int size;

  OrderedSet() : size(0) {}

  inline void build(int n, const int *data) {
    size = n;
    for (int i = 0; i < size; i++)
      bitset[data[i] >> b] |= T(1) << (data[i] & (B - 1));

    for (int x = 0; x < N >> b; ++x)
      tree.data[tree.offset(0) + x + 1] = std::popcount(bitset[x]);
    tree.build();
  }

  inline bool contains(int x) const { return bitset[x >> b] & (T(1) << (x & (B - 1))); }

  inline void insert(int x) {
    if (contains(x))
      return;
    bitset[x >> b] |= T(1) << (x & (B - 1)), size++;
    tree.add_1(x >> b);
  }

  inline void erase(int x) {
    if (!contains(x))
      return;
    bitset[x >> b] ^= T(1) << (x & (B - 1)), size--;
    tree.sub_1(x >> b);
  }

  inline int rank(int x) const {
    return tree.sum(x >> b) + std::popcount(bitset[x >> b] & ((T(1) << (x & (B - 1))) - 1));
  }

  inline int kth(int k) const {
    auto [x, kk] = tree.kth(k);
    return (x << b) + std::countr_zero(pdep(T(1) << kk, bitset[x]));
  }
};

alignas(64) int cnt[1 << 15];

void radix_sort(int n, std::pair<int, int> *__restrict__ a, std::pair<int, int> *__restrict__ b) {
  for (int i = 0; i < 1 << 15; ++i)
    cnt[i] = 0;
  for (int i = 0; i < n; i++)
    cnt[a[i].first & ((1 << 15) - 1)]++;
  prefix<1 << 15>(cnt);
  for (int i = n - 1; i >= 0; i--)
    b[--cnt[a[i].first & ((1 << 15) - 1)]] = a[i];

  for (int i = 0; i < 1 << 15; ++i)
    cnt[i] = 0;
  for (int i = 0; i < n; i++)
    cnt[b[i].first >> 15]++;
  prefix<1 << 15>(cnt);
  for (int i = n - 1; i >= 0; i--)
    a[--cnt[b[i].first >> 15]] = b[i];
}

OrderedSet<MAX_N + MAX_Q> s;

std::array<char, MAX_Q> t;
std::array<int, MAX_Q> x;

std::array<int, MAX_N + MAX_Q> v;
std::array<std::pair<int, int>, MAX_N + MAX_Q> c, _c;

int main() {
  uint32_t n, q;
  cin >> n >> q;

  for (int i = 0; i < n; i++)
    cin >> c[i].first, c[i].second = i;
  if (n == 0)
    cin >> t[0];

  for (int i = 0; i < q; i++)
    cin >> t[i] >> x[i], c[i + n] = {x[i], i + n};

  radix_sort(n + q, c.data(), _c.data());
  for (int i = 0, prev = -1, prev_idx = -1; i < n + q; i++) {
    if (prev != c[i].first)
      prev = c[i].first, prev_idx = i;
    v[c[i].second] = prev_idx;
  }

  s.build(n, v.data());

  for (int i = 0; i < q; i++) {
    if (t[i] == '0') {
      s.insert(v[n + i]);
    } else if (t[i] == '1') {
      s.erase(v[n + i]);
    } else if (t[i] == '2') {
      cout << (x[i] - 1 < s.size ? c[s.kth(x[i] - 1)].first : -1) << '\n';
    } else if (t[i] == '3') {
      cout << (s.rank(v[n + i] + 1)) << '\n';
    } else if (t[i] == '4') {
      int rank = s.rank(v[n + i] + 1) - 1;
      cout << (rank == -1 ? -1 : c[s.kth(rank)].first) << '\n';
    } else if (t[i] == '5') {
      int rank = s.rank(v[n + i]);
      cout << (rank < s.size ? c[s.kth(rank)].first : -1) << '\n';
    }
  }
}