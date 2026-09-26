#pragma GCC optimize("Ofast,unroll-loops")

#include <array>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <string>
#include <utility>

const uint32_t N = 500'000 + 1;
const uint32_t Q = 500'000 + 1;

/*nor's fastio: https://judge.yosupo.jp/submission/75233*/ /* clang-format off */struct IOPre { static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN; std::array<char, 4 * SZ> num; constexpr IOPre() : num{} { for (int i = 0; i < SZ; i++) { int n = i; for (int j = 3; j >= 0; j--) { num[i * 4 + j] = static_cast<char>(n % TEN + '0'); n /= TEN; } } } }; struct IO { static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN, MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16; static constexpr IOPre io_pre = {}; std::array<char, SZ> input_buffer, output_buffer; int input_ptr_left, input_ptr_right, output_ptr_right; IO() : input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {} IO(const IO &) = delete; IO(IO &&) = delete; IO &operator=(const IO &) = delete; IO &operator=(IO &&) = delete; ~IO() { flush(); } template <class T> struct is_char { static constexpr bool value = std::is_same_v<T, char>; }; template <class T> struct is_bool { static constexpr bool value = std::is_same_v<T, bool>; }; template <class T> struct is_string { static constexpr bool value = std::is_same_v<T, std::string> || std::is_same_v<T, const char *> || std::is_same_v<T, char *> || std::is_same_v<std::decay_t<T>, char *>; ; }; template <class T, class D = void> struct is_custom { static constexpr bool value = false; }; template <class T> struct is_custom<T, std::void_t<typename T::internal_value_type>> { static constexpr bool value = true; }; template <class T> struct is_default { static constexpr bool value = is_char<T>::value || is_bool<T>::value || is_string<T>::value || std::is_integral_v<T>; }; template <class T, class D = void> struct is_iterable { static constexpr bool value = false; }; template <class T> struct is_iterable< T, typename std::void_t<decltype(std::begin(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T, class D = void, class E = void> struct is_applyable { static constexpr bool value = false; }; template <class T> struct is_applyable<T, std::void_t<typename std::tuple_size<T>::type>, std::void_t<decltype(std::get<0>(std::declval<T>()))>> { static constexpr bool value = true; }; template <class T> static constexpr bool needs_newline = (is_iterable<T>::value || is_applyable<T>::value) && (!is_default<T>::value); template <typename T, typename U> struct any_needs_newline { static constexpr bool value = false; }; template <typename T> struct any_needs_newline<T, std::index_sequence<>> { static constexpr bool value = false; }; template <typename T, std::size_t I, std::size_t... Is> struct any_needs_newline<T, std::index_sequence<I, Is...>> { static constexpr bool value = needs_newline<decltype(std::get<I>(std::declval<T>()))> || any_needs_newline<T, std::index_sequence<Is...>>::value; }; inline void load() { memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left); input_ptr_right = input_ptr_right - input_ptr_left + static_cast<int>(fread_unlocked( std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1, SZ - input_ptr_right + input_ptr_left, stdin)); input_ptr_left = 0; } inline void read_char(char &c) { if (input_ptr_left + LEN > input_ptr_right) load(); c = input_buffer[input_ptr_left++]; } inline void read_string(std::string &x) { char c; while (read_char(c), c < '!') continue; x = c; while (read_char(c), c >= '!') x += c; } template <class T> inline std::enable_if_t<std::is_integral_v<T>, void> read_int(T &x) { if (input_ptr_left + LEN > input_ptr_right) load(); char c = 0; do c = input_buffer[input_ptr_left++]; while (c < '-'); [[maybe_unused]] bool minus = false; if constexpr (std::is_signed<T>::value == true) if (c == '-') minus = true, c = input_buffer[input_ptr_left++]; x = 0; while (c >= '0') x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++]; if constexpr (std::is_signed<T>::value == true) if (minus) x = -x; } inline void skip_space() { if (input_ptr_left + LEN > input_ptr_right) load(); while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++; } inline void flush() { fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout); output_ptr_right = 0; } inline void write_char(char c) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = c; } inline void write_bool(bool b) { if (output_ptr_right > SZ - LEN) flush(); output_buffer[output_ptr_right++] = b ? '1' : '0'; } inline void write_string(const std::string &s) { for (auto x : s) write_char(x); } inline void write_string(const char *s) { while (*s) write_char(*s++); } inline void write_string(char *s) { while (*s) write_char(*s++); } template <typename T> inline std::enable_if_t<std::is_integral_v<T>, void> write_int(T x) { if (output_ptr_right > SZ - LEN) flush(); if (!x) { output_buffer[output_ptr_right++] = '0'; return; } if constexpr (std::is_signed<T>::value == true) if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x; int i = TWELVE; std::array<char, SIXTEEN> buf{}; while (x >= TENTHOUSAND) { memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4); x /= TENTHOUSAND; i -= 4; } if (x < HUNDRED) { if (x < TEN) { output_buffer[output_ptr_right++] = static_cast<char>('0' + x); } else { std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT; std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN; output_buffer[output_ptr_right] = static_cast<char>('0' + q); output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r); output_ptr_right += 2; } } else { if (x < THOUSAND) { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3; } else { memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4; } } memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i); output_ptr_right += TWELVE - i; } template <typename T_> IO &operator<<(T_ &&x) { using T = typename std::remove_cv<typename std::remove_reference<T_>::type>::type; static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); if constexpr (is_custom<T>::value) { write_int(x.get()); } else if constexpr (is_default<T>::value) { if constexpr (is_bool<T>::value) { write_bool(x); } else if constexpr (is_string<T>::value) { write_string(x); } else if constexpr (is_char<T>::value) { write_char(x); } else if constexpr (std::is_integral_v<T>) { write_int(x); } } else if constexpr (is_iterable<T>::value) { using E = decltype(*std::begin(x)); constexpr char sep = needs_newline<E> ? '\n' : ' '; int i = 0; for (const auto &y : x) { if (i++) write_char(sep); operator<<(y); } } else if constexpr (is_applyable<T>::value) { constexpr char sep = (any_needs_newline< T, std::make_index_sequence<std::tuple_size_v<T>>>::value) ? '\n' : ' '; int i = 0; std::apply( [this, &sep, &i](auto const &...y) { (((i++ ? write_char(sep) : void()), this->operator<<(y)), ...); }, x); } return *this; } template <typename T> IO &operator>>(T &x) { static_assert(is_custom<T>::value or is_default<T>::value or is_iterable<T>::value or is_applyable<T>::value); static_assert(!is_bool<T>::value); if constexpr (is_custom<T>::value) { typename T::internal_value_type y; read_int(y); x = y; } else if constexpr (is_default<T>::value) { if constexpr (is_string<T>::value) { read_string(x); } else if constexpr (is_char<T>::value) { read_char(x); } else if constexpr (std::is_integral_v<T>) { read_int(x); } } else if constexpr (is_iterable<T>::value) { for (auto &y : x) operator>>(y); } else if constexpr (is_applyable<T>::value) { std::apply([this](auto &...y) { ((this->operator>>(y)), ...); }, x); } return *this; } IO *tie(std::nullptr_t) { return this; } void sync_with_stdio(bool) {} }; IO io;/* clang-format on */

#define cin io
#define cout io

template <size_t N> struct RMQ {
  constexpr static uint32_t M = (N >> 4) + 1;
  constexpr static uint32_t H = std::__lg(M) + 1;

  std::array<std::array<uint32_t, M>, H> table;
  std::array<uint32_t, N> data, pref, suff;

  void build(uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
      pref[i] = i & 15 ? std::min(pref[i - 1], data[i]) : data[i];
      if ((i & 15) == 15)
        table[0][i >> 4] = pref[i];
    }

    for (int32_t i = n - 1; i >= 0; i--)
      suff[i] = ~i & 15 ? std::min(suff[i + 1], data[i]) : data[i];

    for (uint32_t i = 1; i < H; i++)
      for (uint32_t j = 0, k = 1 << (i - 1); k < M; j++, k++)
        table[i][j] = std::min(table[i - 1][j], table[i - 1][k]);
  }

  uint32_t query(uint32_t l, uint32_t r) {
    uint32_t L = l >> 4, R = r >> 4;

    if (L == R) {
      uint32_t ret = data[l];
      for (uint32_t i = l; i <= r; i++)
        ret = std::min(ret, data[i]);
      return ret;
    } else if (L == R - 1) {
      return std::min(suff[l], pref[r]);
    }

    uint32_t ret = std::min(suff[l], pref[r]);
    uint32_t h = std::__lg(R - L - 1);
    ret = std::min(ret, table[h][L + 1]);
    ret = std::min(ret, table[h][R - (1 << h)]);
    return ret;
  }
};

std::array<uint32_t, N> deg, P, topo, t_in;
std::array<uint32_t, Q> qs, qi, cnt;

RMQ<N> rmq;

int main() {
  uint32_t n, q;
  cin >> n >> q;

  for (uint32_t _ = 1, ai, bi; _ < n; _++) {
    cin >> ai >> bi;
    deg[ai]++, deg[bi]++;
    P[ai] ^= bi, P[bi] ^= ai;
  }

  std::fill(t_in.begin(), t_in.begin() + n, 1);

  deg[0] = 0;
  for (uint32_t i = 0, topo_idx = n - 1; i < n; i++) {
    for (uint32_t u = i; deg[u] == 1; u = P[u]) {
      topo[topo_idx--] = u;
      t_in[P[u]] += t_in[u];
      deg[u]--, deg[P[u]]--;
      P[P[u]] ^= u;
    }
  }

  auto &depth = deg;
  depth[0] = 0;

  for (uint32_t i = 1; i < n; i++) {
    uint32_t u = topo[i], p = P[u];
    depth[u] = depth[p] + 1;
    t_in[u] = std::exchange(t_in[p], t_in[p] - t_in[u]);
  }

  auto &dfs = topo;
  for (uint32_t i = 0; i < n; i++) {
    dfs[--t_in[i]] = i;
    rmq.data[t_in[i]] = depth[i];
  }

  rmq.build(n);

  for (uint32_t j = 0, s, t, i; j < q; j++) {
    cin >> s >> t >> i;

    uint32_t ts = t_in[s], tt = t_in[t];

    uint32_t common = depth[s];
    if (ts < tt) {
      common = rmq.query(ts + 1, tt) - 1;
    } else if (tt < ts) {
      common = rmq.query(tt + 1, ts) - 1;
    }

    uint32_t dist = depth[s] + depth[t] - 2 * common + 1;

    if (i <= depth[s] - common) {
      cnt[ts]++;
      qs[j] = ts, qi[j] = i;
    } else if (i < dist) {
      cnt[tt]++;
      qs[j] = tt, qi[j] = dist - i - 1;
    } else {
      cnt[n]++;
      qs[j] = n;
    }
  }

  std::partial_sum(cnt.begin(), cnt.begin() + n + 1, cnt.begin());
  auto &order = rmq.data;
  for (uint32_t i = 0; i < q; i++)
    order[--cnt[qs[i]]] = i;

  auto &out = qs;
  auto &stack = depth;
  uint32_t stack_idx = 0;
  stack[stack_idx] = 0;

  for (uint32_t i = 0; i < n; i++) {
    uint32_t u = dfs[i], p = P[u];
    while (stack[stack_idx] != p)
      stack_idx--;
    stack[++stack_idx] = u;

    for (uint32_t j = cnt[i]; j < cnt[i + 1]; j++) {
      uint32_t k = order[j];
      out[k] = stack[stack_idx - qi[k]];
    }
  }

  for (uint32_t j = cnt[n]; j < q; j++)
    out[order[j]] = -1;

  for (uint32_t i = 0; i < q; i++)
    cout << int32_t(out[i]) << '\n';
}