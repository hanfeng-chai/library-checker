#pragma GCC optimize("Ofast,unroll-loops")

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

#include <immintrin.h>

const uint32_t N = 500'000;
const uint32_t Q = 500'000;
const uint32_t MOD = 998244353;

/*nor's fastio: https://judge.yosupo.jp/submission/75233*/ /* clang-format off */struct IOPre { static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN; std::array<char, 4 * SZ> num; constexpr IOPre() : num{} { for (int i = 0; i < SZ; i++) { int n = i; for (int j = 3; j >= 0; j--) { num[i * 4 + j] = static_cast<char>(n % TEN + '0'); n /= TEN; } } } }; struct IO { static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN, MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16; static constexpr IOPre io_pre = {}; std::array<char, SZ> input_buffer, output_buffer; int input_ptr_left, input_ptr_right, output_ptr_right; IO() : input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {} IO(const IO &) = delete; IO(IO &&) = delete; IO &operator=(const IO &) = delete; IO &operator=(IO &&) = delete; ~IO() { flush(); } template <class T> struct is_char { static constexpr bool value = std::is_same_v<T, char>; }; template <class T> struct is_bool { static constexpr bool value = std::is_same_v<T, bool>; }; template <class T> struct is_string { static constexpr bool value = std::is_same_v<T, std::string> || std::is_same_v<T, const char *> || std::is_same_v<T, char *> || std::is_same_v<std::decay_t<T>, char *>; ; }; template <class T, class D = void> struct is_custom { static constexpr bool value = false; }; template <class T> struct is_custom<T, std::void_t<typename T::internal_value_type>> { static constexpr bool value = true; }; template <class T> struct is_default { static constexpr bool value = is_char<T>::value || is_bool<T>::value || is_string<T>::value || std::is_integral_v<T>; }; template <class T, class D = void> struct is_iterable { static constexpr bool value = false; }; template <class T> struct is_iterable< T, typename std::void_t<decltype(std::begin(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T, class D = void, class E = void> struct is_applyable { static constexpr bool value = false; }; template <class T> struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>, std::void_t<decltype(std::get<0>(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T> static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value); template <typename T, typename U> struct any_needs_newline { static constexpr bool value = false; }; template <typename T> struct any_needs_newline<T, std::index_sequence<>> { static constexpr bool value = false; }; template <typename T, std::size_t I, std::size_t... Is> struct any_needs_newline<T, std::index_sequence<I, Is...>> { static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value; }; inline void load() { memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked( std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0; } inline void read_char(char &c) { if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++]; } inline void read_string(std::string &x) { char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c; } template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T &x) { if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true) if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0') x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true) if (minus) x = -x; } inline void skip_space() { if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++; } inline void flush() { fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0; } inline void write_char(char c) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c; } inline void write_bool(bool b) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0'; } inline void write_string(const std::string &s) { for (auto x : s) write_char(x); } inline void write_string(const char *s) { while (*s) write_char(*s++); } inline void write_string(char *s) { while (*s) write_char(*s++); } template <typename T> inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) { if (output_ptr_right > SZ - LEN) flush(); if (!x) { output_buffer[output_ptr_right++] = '0'; return; } if constexpr (std::is_signed<T>::value == true) if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) { memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4; } if (x < HUNDRED) { if (x < TEN) { output_buffer[output_ptr_right++] = static_cast<char>('0' + x); } else { std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2; } } else { if (x < THOUSAND) { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3; } else { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4; } } memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i; } template <typename T_> IO &operator<<(T_ &&x) { using T = typename std::remove_cv<typename std::remove_reference<T_>::type>::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) { write_int(x.get()); } else if constexpr (is_default<T>::value) { if constexpr (is_bool<T>::value) { write_bool(x); } else if constexpr (is_string<T>::value) { write_string(x); } else if constexpr (is_char<T>::value) { write_char(x); } else if constexpr (std::is_integral_v<T>) { write_int(x); } } else if constexpr (is_iterable<T>::value) { using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto &y : x) { if (i++) write_char(sep); operator<<(y); } } else if constexpr (is_applyable<T>::value) { constexpr char sep = (any_needs_newline< T, std::make_index_sequence<std::tuple_size_v<T>>>::value) ? '\n' : ' '; int i = 0; std::apply( [this, &sep, &i](auto const &...y) { (((i++ ? write_char(sep) : void()), this->operator<<(y)), ...); }, x); } return *this; } template <typename T> IO &operator>>(T &x) { static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) { typename T::internal_value_type y; read_int(y); x = y; } else if constexpr (is_default<T>::value) { if constexpr (is_string<T>::value) { read_string(x); } else if constexpr (is_char<T>::value) { read_char(x); } else if constexpr (std::is_integral_v<T>) { read_int(x); } } else if constexpr (is_iterable<T>::value) { for (auto &y : x) operator>>(y); } else if constexpr (is_applyable<T>::value) { std::apply([this](auto &...y) { ((this->operator>>(y)), ...); }, x); } return *this; } IO *tie(std::nullptr_t) { return this; } void sync_with_stdio(bool) {} }; IO io;/* clang-format on */

#define cin io
#define cout io

template <uint32_t mod> struct Montgomery {
  constexpr static uint32_t _n_inv() {
    uint32_t ret = 1;
    for (int i = 0; i < 5; i++)
      ret *= 2u + ret * mod;
    return ret;
  }

  constexpr static uint32_t mod2 = mod * 2;               // 2 * mod
  constexpr static uint32_t n_inv = _n_inv();             // n_inv * mod == -1 (mod 2^32)
  constexpr static uint32_t r = (1ULL << 32) % mod;       // 2^32 % mod
  constexpr static uint32_t r2 = ((uint64_t)r * r) % mod; // 2^64 % mod

  static_assert(mod % 2 != 0);
  static_assert(mod < 1 << 30);
  static_assert(n_inv * mod == -1u);

  constexpr static uint32_t add(uint32_t x, uint32_t y) { return std::min(x + y, x + y - mod2); }
  constexpr static uint32_t mul(uint32_t x, uint32_t y) { return reduce((uint64_t)x * y); }
  constexpr static uint32_t reduce(uint64_t x) { return (x + uint32_t(x) * n_inv * uint64_t(mod)) >> 32; }
  constexpr static uint32_t transform(uint32_t x) { return mul(x, r2); }
};

template <uint32_t mod_> struct MontgomerySIMD {
  using mt = Montgomery<mod_>;

  static __m256i mod() { return _mm256_set1_epi32(mod_); }
  static __m256i mod2() { return _mm256_set1_epi32(mt::mod2); }
  static __m256i n_inv() { return _mm256_set1_epi32(mt::n_inv); }
  static __m256i r() { return _mm256_set1_epi32(mt::r); }
  static __m256i r2() { return _mm256_set1_epi32(mt::r2); }

  static __m256i add(__m256i x, __m256i y, __m256i mod2) {
    __m256i z = _mm256_add_epi32(x, y);
    return _mm256_min_epu32(z, _mm256_sub_epi32(z, mod2));
  }

  template <bool scalar = false> static __m256i mul(__m256i x, __m256i y, __m256i n_inv, __m256i mod) {
    __m256i z0246 = _mm256_mul_epu32(x, y);
    __m256i z1357 = _mm256_mul_epu32(_mm256_bsrli_epi128(x, 4), scalar ? y : _mm256_bsrli_epi128(y, 4));
    return reduce(z0246, z1357, n_inv, mod);
  }

  static __m256i reduce(__m256i x0246, __m256i x1357, __m256i n_inv, __m256i mod) {
    __m256i m0246 = _mm256_mul_epu32(x0246, n_inv);
    __m256i m1357 = _mm256_mul_epu32(x1357, n_inv);
    __m256i y0246 = _mm256_add_epi64(x0246, _mm256_mul_epu32(m0246, mod));
    __m256i y1357 = _mm256_add_epi64(x1357, _mm256_mul_epu32(m1357, mod));
    return _mm256_blend_epi32(_mm256_bsrli_epi128(y0246, 4), y1357, 0b10101010);
  }

  // not sure if this is accurate returns reduce - 1
  static __m256i reduce(__m256i x, __m256i n_inv, __m256i mod) {
    __m256i m0246 = _mm256_mul_epu32(_mm256_bsrli_epi128(x, 4), n_inv);
    __m256i m1357 = _mm256_mul_epu32(x, n_inv);
    __m256i y0246 = _mm256_mul_epu32(m0246, mod);
    __m256i y1357 = _mm256_mul_epu32(m1357, mod);
    return _mm256_blend_epi32(_mm256_bsrli_epi128(y1357, 4), y0246, 0b10101010);
  }

  static __m256i transform(__m256i x, __m256i r2, __m256i n_inv, __m256i mod) { return mul<true>(x, r2, n_inv, mod); }

  static __m256i add(__m256i x, __m256i y) { return add(x, y, mod2()); }
  template <bool scalar = false> static __m256i mul(__m256i x, __m256i y) { return mul<scalar>(x, y, n_inv(), mod()); }
  static __m256i reduce(__m256i x) { return reduce(x, n_inv(), mod()); }
  static __m256i transform(__m256i x) { return transform(x, r2(), n_inv(), mod()); }
};

using mt = Montgomery<MOD>;
using mt_simd = MontgomerySIMD<MOD>;

template <int B> void affine(uint32_t *mul_val, uint32_t *add_val, __m256i mul, __m256i add) {
  for (int i = 0; i < B; i += 8) {
    __m256i initial_mul = _mm256_load_si256((__m256i *)&mul_val[i]);
    __m256i initial_add = _mm256_load_si256((__m256i *)&add_val[i]);
    __m256i updated_mul = mt_simd::mul<true>(initial_mul, mul);
    __m256i updated_add = mt_simd::add(mt_simd::mul<true>(initial_add, mul), add);
    _mm256_store_si256((__m256i *)&mul_val[i], updated_mul);
    _mm256_store_si256((__m256i *)&add_val[i], updated_add);
  }
}

template <int B>
void masked_affine(uint32_t *mul_val, uint32_t *add_val, __m256i mul, __m256i add, const uint32_t mask[B]) {
  for (int i = 0; i < B; i += 8) {
    __m256i mask_vec = _mm256_load_si256((__m256i *)&mask[i]);
    __m256i initial_mul = _mm256_load_si256((__m256i *)&mul_val[i]);
    __m256i initial_add = _mm256_load_si256((__m256i *)&add_val[i]);
    __m256i updated_mul = mt_simd::mul<true>(initial_mul, mul);
    __m256i updated_add = mt_simd::add(mt_simd::mul<true>(initial_add, mul), add);
    _mm256_store_si256((__m256i *)&mul_val[i], _mm256_blendv_epi8(initial_mul, updated_mul, mask_vec));
    _mm256_store_si256((__m256i *)&add_val[i], _mm256_blendv_epi8(initial_add, updated_add, mask_vec));
  }
}

template <uint32_t N, uint32_t b = 3> struct WideLazySegmentTree {
  constexpr static uint32_t B = 1 << b;

  constexpr static uint32_t height(uint32_t n) { return (n <= B ? 1 : height(n / B) + 1); }
  constexpr static uint32_t H = height(N);

  constexpr static int offset(int h) {
    int s = 0, n = N;
    while (h--) {
      n = (n + B - 1) / B;
      s += n * B;
    }
    return s;
  }

  struct Precalc {
    alignas(64) uint32_t l_mask[B][B];
    alignas(64) uint32_t r_mask[B][B];
    alignas(64) uint32_t lr_mask[B][B][B];

    constexpr Precalc() : l_mask{}, r_mask{}, lr_mask{} {
      // TODO: try generating only an iota and generating the rest dynamically with cmpgt

      for (int i = 1; i < B; i++) // NOTE: 0 is a special case
        for (int j = 0; j < B; j++)
          l_mask[i][j] = i <= j ? -1 : 0;

      for (int i = 0; i < B; i++)
        for (int j = 0; j < B; j++)
          r_mask[i][j] = j < i ? -1 : 0;

      for (int i = 0; i < B; i++)
        for (int j = 0; j < B; j++)
          for (int k = 0; k < B; k++)
            lr_mask[i][j][k] = i <= k && k < j ? -1 : 0;
    };
  };

  constexpr static Precalc T{};

  alignas(64) std::array<uint32_t, offset(H)> tree_mul; // NOTE: [0, offset(1)) is unused
  alignas(64) std::array<uint32_t, offset(H)> tree_add;

  uint32_t ret;
  alignas(64) uint32_t ret_mul[H][Q];
  alignas(64) uint32_t ret_add[H][Q];

  constexpr WideLazySegmentTree() { std::fill(tree_mul.begin(), tree_mul.end(), mt::transform(1)); }

  void build() {
    for (int i = 0; i < N; i += 8) {
      auto ptr = (__m256i *)&tree_add[i];
      _mm256_store_si256(ptr, mt_simd::transform(_mm256_load_si256(ptr)));
    }
  }

  [[gnu::always_inline]] inline void push(uint32_t h, uint32_t i) {
    __m256i mul = _mm256_set1_epi32(std::exchange(tree_mul[offset(h) + i], mt::transform(1)));
    __m256i add = _mm256_set1_epi32(std::exchange(tree_add[offset(h) + i], 0));
    affine<B>(&tree_mul[offset(h - 1) + (i << b)], &tree_add[offset(h - 1) + (i << b)], mul, add);
  }

  void apply(uint32_t l, uint32_t r, uint32_t _mul, uint32_t _add) {
    _mul = mt::transform(_mul), _add = mt::transform(_add);
    __m256i mul = _mm256_set1_epi32(_mul), add = _mm256_set1_epi32(_add);

#pragma GCC unroll(64)
    for (int h = H - 1; h > 0; h--) {
      push(h, l >> (b * h));
      push(h, r >> (b * h));
    }

#pragma GCC unroll(64)
    for (int h = 0; h < H; h++) {
      // clang-format off

      if (l >> b >= r >> b) {
        return masked_affine<B>(
          &tree_mul[offset(h) + (l & ~(B - 1))],
          &tree_add[offset(h) + (l & ~(B - 1))],
          mul,
          add,
          T.lr_mask[l & (B - 1)][r & (B - 1)]
        );
      }

      masked_affine<B>(
        &tree_mul[offset(h) + (l & ~(B - 1))],
        &tree_add[offset(h) + (l & ~(B - 1))],
        mul,
        add,
        T.l_mask[l & (B - 1)]
      );

      masked_affine<B>(
        &tree_mul[offset(h) + (r & ~(B - 1))],
        &tree_add[offset(h) + (r & ~(B - 1))],
        mul,
        add,
        T.r_mask[r & (B - 1)]
      );

      // clang-format on

      l = (l + B - 1) >> b, r >>= b;
    }
  }

  void get(uint32_t i) {
    ret_add[0][ret] = tree_add[offset(0) + i];

    for (int h = 1; h < H; h++) {
      ret_mul[h][ret] = tree_mul[offset(h) + (i >> (h * b))];
      ret_add[h][ret] = tree_add[offset(h) + (i >> (h * b))];
    }

    ret++;
  }
};

WideLazySegmentTree<N> s;

int main() {
  uint32_t n, q;
  cin >> n >> q;

  for (int i = 0; i < n; i++)
    cin >> s.tree_add[s.offset(0) + i];
  s.build();


  for (int _ = 0; _ < q; _++) {
    char t;
    cin >> t;

    if (t == '0') {
      uint32_t l, r, b, c;
      cin >> l >> r >> b >> c;
      // TODO: try collecting and vectorizing the transform
      s.apply(l, r, b, c);
    } else {
      uint32_t i;
      cin >> i;
      s.get(i);
    }
  }

  for (int i = 0; i < s.ret; i += 8) {
    __m256i vec = _mm256_load_si256((__m256i *)&s.ret_add[0][i]);
    for (int h = 1; h < s.H; h++) {
      __m256i mul = _mm256_load_si256((__m256i *)&s.ret_mul[h][i]);
      __m256i add = _mm256_load_si256((__m256i *)&s.ret_add[h][i]);
      vec = mt_simd::add(mt_simd::mul(vec, mul), add);
    }

    vec = mt_simd::reduce(vec);
    _mm256_store_si256((__m256i *)&s.ret_add[0][i], vec);
  }

  for (int i = 0; i < s.ret; i++)
    cout << (s.ret_add[0][i] + 1) << '\n';
}