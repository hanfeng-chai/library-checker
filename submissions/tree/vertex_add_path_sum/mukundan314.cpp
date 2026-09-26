// nor's submission (https://judge.yosupo.jp/submission/243452) with linear build on segment tree

#pragma GCC optimize("Ofast,unroll-loops")

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

#include <x86intrin.h>

const int N = 500'000 + 10;

/*nor's fastio: https://judge.yosupo.jp/submission/75233*/ /* clang-format off */struct IOPre { static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN; std::array<char, 4 * SZ> num; constexpr IOPre() : num{} { for (int i = 0; i < SZ; i++) { int n = i; for (int j = 3; j >= 0; j--) { num[i * 4 + j] = static_cast<char>(n % TEN + '0'); n /= TEN; } } } }; struct IO { static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN, MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16; static constexpr IOPre io_pre = {}; std::array<char, SZ> input_buffer, output_buffer; int input_ptr_left, input_ptr_right, output_ptr_right; IO() : input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {} IO(const IO &) = delete; IO(IO &&) = delete; IO &operator=(const IO &) = delete; IO &operator=(IO &&) = delete; ~IO() { flush(); } template <class T> struct is_char { static constexpr bool value = std::is_same_v<T, char>; }; template <class T> struct is_bool { static constexpr bool value = std::is_same_v<T, bool>; }; template <class T> struct is_string { static constexpr bool value = std::is_same_v<T, std::string> || std::is_same_v<T, const char *> || std::is_same_v<T, char *> || std::is_same_v<std::decay_t<T>, char *>; ; }; template <class T, class D = void> struct is_custom { static constexpr bool value = false; }; template <class T> struct is_custom<T, std::void_t<typename T::internal_value_type>> { static constexpr bool value = true; }; template <class T> struct is_default { static constexpr bool value = is_char<T>::value || is_bool<T>::value || is_string<T>::value || std::is_integral_v<T>; }; template <class T, class D = void> struct is_iterable { static constexpr bool value = false; }; template <class T> struct is_iterable< T, typename std::void_t<decltype(std::begin(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T, class D = void, class E = void> struct is_applyable { static constexpr bool value = false; }; template <class T> struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>, std::void_t<decltype(std::get<0>(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T> static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value); template <typename T, typename U> struct any_needs_newline { static constexpr bool value = false; }; template <typename T> struct any_needs_newline<T, std::index_sequence<>> { static constexpr bool value = false; }; template <typename T, std::size_t I, std::size_t... Is> struct any_needs_newline<T, std::index_sequence<I, Is...>> { static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value; }; inline void load() { memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked( std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0; } inline void read_char(char &c) { if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++]; } inline void read_string(std::string &x) { char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c; } template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T &x) { if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true) if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0') x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true) if (minus) x = -x; } inline void skip_space() { if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++; } inline void flush() { fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0; } inline void write_char(char c) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c; } inline void write_bool(bool b) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0'; } inline void write_string(const std::string &s) { for (auto x : s) write_char(x); } inline void write_string(const char *s) { while (*s) write_char(*s++); } inline void write_string(char *s) { while (*s) write_char(*s++); } template <typename T> inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) { if (output_ptr_right > SZ - LEN) flush(); if (!x) { output_buffer[output_ptr_right++] = '0'; return; } if constexpr (std::is_signed<T>::value == true) if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) { memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4; } if (x < HUNDRED) { if (x < TEN) { output_buffer[output_ptr_right++] = static_cast<char>('0' + x); } else { std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2; } } else { if (x < THOUSAND) { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3; } else { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4; } } memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i; } template <typename T_> IO &operator<<(T_ &&x) { using T = typename std::remove_cv<typename std::remove_reference<T_>::type>::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) { write_int(x.get()); } else if constexpr (is_default<T>::value) { if constexpr (is_bool<T>::value) { write_bool(x); } else if constexpr (is_string<T>::value) { write_string(x); } else if constexpr (is_char<T>::value) { write_char(x); } else if constexpr (std::is_integral_v<T>) { write_int(x); } } else if constexpr (is_iterable<T>::value) { using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto &y : x) { if (i++) write_char(sep); operator<<(y); } } else if constexpr (is_applyable<T>::value) { constexpr char sep = (any_needs_newline< T, std::make_index_sequence<std::tuple_size_v<T>>>::value) ? '\n' : ' '; int i = 0; std::apply( [this, &sep, &i](auto const &...y) { (((i++ ? write_char(sep) : void()), this->operator<<(y)), ...); }, x); } return *this; } template <typename T> IO &operator>>(T &x) { static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) { typename T::internal_value_type y; read_int(y); x = y; } else if constexpr (is_default<T>::value) { if constexpr (is_string<T>::value) { read_string(x); } else if constexpr (is_char<T>::value) { read_char(x); } else if constexpr (std::is_integral_v<T>) { read_int(x); } } else if constexpr (is_iterable<T>::value) { for (auto &y : x) operator>>(y); } else if constexpr (is_applyable<T>::value) { std::apply([this](auto &...y) { ((this->operator>>(y)), ...); }, x); } return *this; } IO *tie(std::nullptr_t) { return this; } void sync_with_stdio(bool) {} }; IO io;/* clang-format on */

#define cin io
#define cout io

typedef int64_t v4ll __attribute__((vector_size(32)));

// Based on https://en.algorithmica.org/hpc/algorithms/prefix/
inline void prefix_x2(int64_t *p) {
  __m256i x = _mm256_load_si256((__m256i *)p);
  x = _mm256_add_epi64(x, _mm256_slli_si256(x, 8));
  _mm256_store_si256((__m256i *)p, x);
}

inline __m128i update(int64_t *p, __m128i s) {
  __m128i d = _mm_set1_epi64x(p[1]);
  __m128i x = _mm_load_si128((__m128i *)p);
  x = _mm_add_epi64(s, x);
  _mm_store_si128((__m128i *)p, x);
  return _mm_add_epi64(s, d);
}

template <int N> inline void prefix(int64_t *a) {
  for (int i = 0; i < N; i += 4)
    prefix_x2(&a[i]);

  __m128i s = _mm_setzero_si128();
  for (int i = 0; i < N; i += 2)
    s = update(&a[i], s);
}

// Based on https://en.algorithmica.org/hpc/data-structures/segment-trees/
template <int N, int b = 4> struct WideSegmentTree {
  constexpr static int B = 1 << b;

  struct Precalc {
    alignas(64) int64_t mask[B][B];

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

  alignas(64) int64_t data[offset(H)];

  inline void build() {
    for (int h = 0; h < H - 1; h++) {
      const int s = offset(h + 1) - offset(h) - B; // outer loop isn't unrolled properly without this
      for (int i = 0; i < s; i += B) {
        prefix<B>(&data[offset(h) + i]);
        data[offset(h + 1) + (i >> b) + 1] = data[offset(h) + i + B - 1] + std::exchange(data[offset(h) + i + B], 0);
      }
    }

    prefix<B>(&data[offset(H - 1)]);
  }

  inline int64_t sum(int k) const {
    int64_t res = 0;
    for (int h = H - 1; h >= 0; h--)
      res += data[offset(h) + (k >> (h * b))];
    return res;
  }

  inline void add(int k, int _x) {
    v4ll x = _x + v4ll{};
    for (int h = H - 1; h >= 0; h--) {
      int p = k >> (h * b);
      auto l = (v4ll *)&data[offset(h) + (p & ~(B - 1))];
      auto m = (v4ll *)T.mask[p & (B - 1)];
      for (int i = 0; i < B / 4; i++)
        l[i] += x & m[i];
    }
  }
};

WideSegmentTree<N> s;

void update(int k, int x) { s.add(k, x); }

void lazy_update(int k, int x) { s.data[s.offset(0) + k + 1] = x; }

int64_t query(int l, int r) { return s.sum(r + 1) - s.sum(l); }

using Node = int64_t;
using Update = int;

unsigned arr[N];
int neighbour_xor[N], degree[N];
int xor_order[N], subtree_size[N], heavy_child[N];

signed main() {
  cin.tie(0)->sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  for (int i = 0; i < n; ++i)
    cin >> arr[i];

  constexpr int root = 0;

  for (int i = 0; i < n - 1; i++) {
    unsigned u, v;
    cin >> u >> v;
    degree[u]++, neighbour_xor[u] ^= v;
    degree[v]++, neighbour_xor[v] ^= u;
  }
  degree[root] = 0;

  int xor_order_index = n - 1;
  std::fill(subtree_size, subtree_size + n, 1);
  std::memset(heavy_child, -1, n * sizeof(heavy_child[0]));

  for (int i = 0; i < n; i++) {
    int u = i;
    while (degree[u] == 1) {
      xor_order[xor_order_index--] = u;
      int p = neighbour_xor[u]; // parent
      if (heavy_child[p] == -1 || subtree_size[u] > subtree_size[heavy_child[p]]) {
        heavy_child[p] = u;
      }
      subtree_size[p] += subtree_size[u];
      neighbour_xor[p] ^= u;
      degree[u]--;
      degree[p]--;
      u = p;
    }
  }
  xor_order[0] = root;

  auto &id = subtree_size;
  std::memset(id, -1, n * sizeof(heavy_child[0]));
  auto &heavy_chain_root = degree;

  int timer = 0;
  for (int i : xor_order) {
    if (id[i] != -1)
      continue;
    heavy_chain_root[i] = i;
    while (true) {
      id[i] = timer++;
      lazy_update(id[i], arr[i]);
      if (heavy_child[i] == -1)
        break;
      heavy_chain_root[heavy_child[i]] = heavy_chain_root[i];
      i = heavy_child[i];
    }
  }

  s.build();

  while (q--) {
    int t, a, b;
    cin >> t >> a >> b;
    if (t == 0) {
      update(id[a], b);
    } else {
      Node ans = 0;
      for (; heavy_chain_root[a] != heavy_chain_root[b]; b = neighbour_xor[heavy_chain_root[b]]) {
        if (id[heavy_chain_root[a]] > id[heavy_chain_root[b]])
          std::swap(a, b);
        ans += query(id[heavy_chain_root[b]], id[b]);
      }
      if (id[a] > id[b])
        std::swap(a, b);
      ans += query(id[a], id[b]);
      cout << ans << '\n';
    }
  }

  return 0;
}