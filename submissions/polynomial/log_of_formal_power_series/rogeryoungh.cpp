// GENERATE DATE: 2023-06-11 22:10:16.296977
// magic!
#pragma GCC optimize("O3")
// GENERATE FROM: https://github.com/rogeryoungh/algorithm-cpp
#include <cstdint>
using i8 = signed char;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using i128 = __int128_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using u128 = __uint128_t;
#include <cstring>
#include <string>
#include <vector>
// ALGO_IO_NUMBER_ONLY 输入只有 0-9、-、空格、换行
namespace detail {
template <class Buf>
struct FastI : Buf {
  using Buf::pop;
  using Buf::top;
  FastI(FILE *f, u32 size = 1 << 18) : Buf(f, size) {}
  void skipSpace() {
    while (top() <= ' ')
      pop();
  }
  FastI &operator>>(char &x) {
    skipSpace();
    x = pop();
    return *this;
  }
  FastI &operator>>(std::string &x) {
    x.resize(0);
    skipSpace();
    while (std::isgraph(top()))
      x.push_back(pop());
    return *this;
  }
  template <std::unsigned_integral T>
  FastI &operator>>(T &x) {
    x = 0;
    skipSpace();
    while (top() >= '0')
      x = x * 10 + (pop() & 0xf);
    return *this;
  }
  template <std::signed_integral T>
  FastI &operator>>(T &x) {
    bool neg = false;
    x = 0;
    skipSpace();
    if (top() == '-')
      neg = true, pop();
    while (top() >= '0')
      x = x * 10 + (pop() & 0xf);
    x = neg ? -x : x;
    return *this;
  }
};
template <class Buf>
struct FastO : Buf {
  using Buf::push;
  using Buf::push_uncheck;
  using Buf::puts;
  std::vector<u32> pre;
  FastO(FILE *f, u32 size = 1 << 18) : Buf(f, size), pre(u64(1E4)) {
    for (i32 i = 0; i < i32(u64(1E4)); ++i) {
      i32 ti = i;
      for (i32 j = 0; j < 4; ++j) {
        pre[i] = pre[i] << 8 | ti % 10 | 0x30;
        ti /= 10;
      }
    }
  }
  ~FastO() {
    Buf::flush();
  }
  template <std::signed_integral T>
  FastO &operator<<(T x) {
    if (x < 0)
      push('-'), x = -x;
    return *this << std::make_unsigned<T>::type(x);
  }
  void output4(u32 t) {
    auto tp = (const char *)&pre[t];
    if (t >= u64(1E2)) {
      if (t >= u64(1E3))
        push_uncheck(tp, 4);
      else
        push_uncheck(tp + 1, 3);
    } else {
      if (t >= u64(1E1))
        push_uncheck(tp + 2, 2);
      else
        push_uncheck(t | 0x30);
    }
  };
  template <std::unsigned_integral T>
  FastO &operator<<(T x) {
    Buf::reserve(32);
    if (x >= u64(1E8)) {
      u64 q0 = x / u64(1E8), r0 = x % u64(1E8);
      if (x >= u64(1E16)) {
        u64 q1 = q0 / u64(1E8), r1 = q0 % u64(1E8);
        output4(q1);
        push_uncheck(&pre[r1 / u64(1E4)], 4);
        push_uncheck(&pre[r1 % u64(1E4)], 4);
      } else if (x >= u64(1E12)) {
        output4(q0 / u64(1E4));
        push_uncheck(&pre[q0 % u64(1E4)], 4);
      } else {
        output4(q0);
      }
      push_uncheck(&pre[r0 / u64(1E4)], 4);
      push_uncheck(&pre[r0 % u64(1E4)], 4);
    } else {
      if (x >= u64(1E4)) {
        output4(x / u64(1E4));
        push_uncheck(&pre[x % u64(1E4)], 4);
      } else {
        output4(x);
      }
    }
    return *this;
  }
  FastO &operator<<(char x) {
    return push(x), *this;
  }
  FastO &operator<<(const char *x) {
    return puts(x), *this;
  }
  FastO &operator<<(const std::string &x) {
    return push(x.c_str(), x.size()), *this;
  }
};
struct BufO {
  FILE *f;
  char *beg, *end, *p;
  BufO(FILE *f_, u32 sz) : f(f_), beg(new char[sz]), end(beg + sz - 1), p(beg) {}
  ~BufO() {
    delete[] beg;
  }
  void flush() {
    std::fwrite(beg, 1, p - beg, f);
    p = beg;
  }
  void reserve(u32 len) {
    if (end - p <= i32(len))
      flush();
  }
  void push(char s) {
    *p++ = s;
    reserve(0);
  }
  void push(const char *s, u32 len) {
    reserve(len);
    push_uncheck(s, len);
  }
  void push_uncheck(char s) {
    *p++ = s;
  }
  void push_uncheck(const void *s, u32 len) {
    std::memcpy(p, s, len);
    p += len;
  }
  void puts(const char *s) {
    while (*s != 0)
      push(*s++);
  }
};
} // namespace detail
#include <sys/mman.h>
#include <sys/stat.h>
namespace detail {
struct BufI {
  struct stat sb;
  char *p;
  BufI(FILE *f, u32) {
    i32 fd = fileno(f);
    fstat(fd, &sb);
    p = (char *)mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    madvise(p, sb.st_size, MADV_SEQUENTIAL);
  }
  ~BufI() {
    munmap(p, sb.st_size);
  }
  char pop() {
    return *p++;
  }
  char top() const {
    return *p;
  }
};
} // namespace detail
using FastI = detail::FastI<detail::BufI>;
using FastO = detail::FastO<detail::BufO>;
#include <cstdlib>
#include <vector>
template <class T, u32 align>
struct AlignedAllocator {
  using value_type = T;
  T *allocate(std::size_t n) {
    return new (std::align_val_t(align)) T[n];
  }
  template <class U>
  struct rebind {
    using other = AlignedAllocator<U, align>;
  };
  void deallocate(T *p, std::size_t) {
    delete[] p;
  }
};
template <class T, u32 align = 32>
using AVec = std::vector<T, AlignedAllocator<T, 32>>;
#include <type_traits>
template <class ModT>
concept static_modint_concept = ModT::isStatic::value;
template <class ModT>
concept raw32_modint_concept = ModT::rawU32::value;
template <class ModT>
concept static_raw32_modint_concept = static_modint_concept<ModT> && raw32_modint_concept<ModT>;
template <class ModT>
concept runtime_modint_concept = !
ModT::isStatic::value;
template <class ModT>
concept montgomery_modint_concept = ModT::isMontgomery::value;
template <class ModT>
concept static_basic_modint_concept = !
montgomery_modint_concept<ModT> &&static_modint_concept<ModT>;
#include <algorithm>
#include <bit>
#include <cassert>
#include <span>
#include <vector>
namespace detail {
u32 ntt_size = 0;
} // namespace detail
/////////////////////
// classical-radix-4
#include <algorithm>
#include <bit>
#include <cassert>
#include <span>
#include <vector>
namespace detail {
template <class ModT>
struct NttClassicalInfo4 {
  using ValueT = typename ModT::ValueT;
  std::array<ModT, 64> rt, irt, rate2, irate2, rate3, irate3;
  NttClassicalInfo4() {
    const ValueT P = ModT::mod();
    const ValueT g = 3;
    const i32 rank2 = std::countr_zero(P - 1);
    rt[rank2] = ModT(g).pow((P - 1) >> rank2);
    irt[rank2] = rt[rank2].inv();
    for (i32 i = rank2; i >= 1; --i) {
      rt[i - 1] = rt[i] * rt[i];
      irt[i - 1] = irt[i] * irt[i];
    }
    ModT prod = 1, iprod = 1;
    for (i32 i = 0; i < rank2 - 1; ++i) {
      rate2[i] = prod * rt[i + 2];
      irate2[i] = iprod * irt[i + 2];
      prod *= irt[i + 2];
      iprod *= rt[i + 2];
    }
    prod = 1, iprod = 1;
    for (i32 i = 0; i < rank2 - 2; ++i) {
      rate3[i] = prod * rt[i + 3];
      irate3[i] = iprod * irt[i + 3];
      prod *= irt[i + 3];
      iprod *= rt[i + 3];
    }
  }
};
template <class ModT>
static void ntt_classical_basic4(std::span<ModT> f) { // dif
  const NttClassicalInfo4<ModT> info;
  i32 n = f.size(), l = n / 2, n_4b = std::countr_zero<u32>(n) & 1;
  if (n_4b) {
    for (i32 j = 0; j < l; ++j) {
      ModT x = f[j], y = f[j + l];
      f[j] = x + y;
      f[j + l] = x - y;
    }
    l /= 2;
  }
  for (l /= 2; l >= 1; l /= 4) {
    ModT r = 1, img = info.rt[2];
    for (i32 i = 0, k = 0; i < n; i += l * 4, ++k) {
      ModT r2 = r * r, r3 = r2 * r;
      for (i32 j = 0; j < l; ++j) {
        ModT x0 = f[i + j + 0 * l];
        ModT x1 = f[i + j + 1 * l] * r;
        ModT x2 = f[i + j + 2 * l] * r2;
        ModT x3 = f[i + j + 3 * l] * r3;
        ModT x1x3 = ModT::submul(x1, x3, img);
        ModT x02 = x0 + x2, x0_2 = x0 - x2;
        ModT x13 = x1 + x3;
        f[i + j + 0 * l] = x02 + x13;
        f[i + j + 1 * l] = x02 - x13;
        f[i + j + 2 * l] = x0_2 + x1x3;
        f[i + j + 3 * l] = x0_2 - x1x3;
      }
      r *= info.rate3[std::countr_one<u32>(k)];
    }
  }
}
template <class ModT>
static void intt_classical_basic4(std::span<ModT> f) { // dit
  const NttClassicalInfo4<ModT> info;
  i32 n = f.size(), l = 1, n_4b = std::countr_zero<u32>(n) & 1;
  for (; l < (n_4b ? n / 2 : n); l *= 4) {
    ModT r = 1, img = info.irt[2];
    for (i32 i = 0, k = 0; i < n; i += l * 4, ++k) {
      ModT r2 = r * r, r3 = r2 * r;
      for (i32 j = 0; j < l; ++j) {
        ModT x0 = f[i + j + 0 * l];
        ModT x1 = f[i + j + 1 * l];
        ModT x2 = f[i + j + 2 * l];
        ModT x3 = f[i + j + 3 * l];
        ModT x2x3 = ModT::submul(x2, x3, img);
        ModT x01 = x0 + x1, x0_1 = x0 - x1;
        ModT x23 = x2 + x3;
        f[i + j + 0 * l] = x01 + x23;
        f[i + j + 1 * l] = ModT::addmul(x0_1, x2x3, r);
        f[i + j + 2 * l] = ModT::submul(x01, x23, r2);
        f[i + j + 3 * l] = ModT::submul(x0_1, x2x3, r3);
      }
      r *= info.irate3[std::countr_one<u32>(k)];
    }
  }
  if (n_4b) {
    for (i32 j = 0; j < l; ++j) {
      ModT x = f[j], y = f[j + l];
      f[j] = x + y;
      f[j + l] = x - y;
    }
  }
  const ModT ivn = ModT(n).inv();
  for (i32 i = 0; i < n; i++)
    f[i] *= ivn;
}
} // namespace detail
// https://judge.yosupo.jp/submission/92714
#pragma GCC target("avx2")
#include <immintrin.h>
#include <array>
namespace simd {
using I256 = __m256i;
using I128x2 = I256;
using I64x4 = I256;
using I32x8 = I256;
using U32x8 = I256;
using U64x4 = I256;
using U128x2 = I256;
namespace i256 {
inline I256 loadu(const I256 *p) {
  return _mm256_loadu_si256(p);
}
template <bool aligned = true>
inline I256 load(const I256 *p) {
  if constexpr (aligned)
    return _mm256_load_si256(p);
  else
    return _mm256_loadu_si256(p);
}
template <bool aligned = true>
inline void store(I256 *p, const I256 &v) {
  if constexpr (aligned)
    _mm256_store_si256(p, v);
  else
    _mm256_storeu_si256(p, v);
}
inline void storeu(I256 *p, const I256 &v) {
  _mm256_storeu_si256(p, v);
}
template <class T>
inline auto to_array(const I256 &v) {
  constexpr u32 sizeT = sizeof(T);
  static_assert(sizeof(I256) % sizeT == 0);
  alignas(32) std::array<T, sizeof(I256) / sizeT> arr;
  _mm256_storeu_si256((I256 *)arr.data(), v);
  return arr;
}
inline I256 bit_and(const I256 &a, const I256 &b) {
  return _mm256_and_si256(a, b);
}
} // namespace i256
namespace i128x2 {
template <i32 imm>
inline I128x2 permute(const I128x2 &a, const I128x2 &b) {
  return _mm256_permute2x128_si256(a, b, imm);
}
template <i32 imm>
inline I128x2 shuffle(const I128x2 &a) {
  return permute<imm>(a, a);
}
} // namespace i128x2
namespace i64x4 {
inline I64x4 add(const I64x4 &a, const I64x4 &b) {
  return _mm256_add_epi64(a, b);
}
} // namespace i64x4
namespace i32x8 {
inline I32x8 from(i32 v) {
  return _mm256_set1_epi32(v);
}
inline I32x8 add(const I32x8 &a, const I32x8 &b) {
  return _mm256_add_epi32(a, b);
}
inline I32x8 sub(const I32x8 &a, const I32x8 &b) {
  return _mm256_sub_epi32(a, b);
}
inline I64x4 mul(const I32x8 &a, const I32x8 &b) {
  return _mm256_mul_epi32(a, b);
}
template <i32 imm>
inline I32x8 shuffle(const I32x8 &a) {
  return _mm256_shuffle_epi32(a, imm);
}
template <i32 imm>
inline I32x8 blend(const I32x8 &a, const I32x8 &b) {
  return _mm256_blend_epi32(a, b, imm);
}
inline I32x8 zero() {
  return _mm256_setzero_si256();
}
inline I32x8 sign(const I32x8 &a) {
  return _mm256_cmpgt_epi32(zero(), a);
}
inline auto mul_0246_1357(const I32x8 &a, const I32x8 &b) {
  auto x0246 = mul(a, b);
  auto x1357 = mul(shuffle<0b11110101>(a), shuffle<0b11110101>(b));
  alignas(32) std::pair<I64x4, I64x4> p = {x0246, x1357};
  return p;
}
inline I32x8 abs(const I32x8 &a) {
  return _mm256_abs_epi32(a);
}
} // namespace i32x8
namespace u32x8 {
inline U64x4 mul(const U32x8 &a, const U32x8 &b) {
  return _mm256_mul_epu32(a, b);
}
inline auto mul_0246_1357(const U32x8 &a, const U32x8 &b) {
  auto x0246 = mul(a, b);
  auto x1357 = mul(i32x8::shuffle<0b11110101>(a), i32x8::shuffle<0b11110101>(b));
  alignas(32) std::pair<U64x4, U64x4> p = {x0246, x1357};
  return p;
}
} // namespace u32x8
} // namespace simd
#include <type_traits>
namespace simd {
// 仅在 Montgomery 空间里
template <montgomery_modint_concept ModT_>
struct M32x8 {
  using ModT = ModT_;
  I32x8 v;
  M32x8() = default;
  M32x8(const I32x8 &a) : v(a) {}
  template <class U32>
  M32x8(const std::array<U32, 8> &a) {
    static_assert(sizeof(U32) == 4);
    v = i256::load((const I256 *)a.data());
  }
  static M32x8 from(i32 v) {
    return i32x8::from(v);
  }
  static M32x8 from(ModT v) {
    return from(v.raw());
  }
  explicit operator I32x8() {
    return v;
  }
  static I32x8 get_irx8() {
    return i32x8::from(ModT::Space::ir());
  }
  static I32x8 get_mod2x8() {
    return i32x8::from(ModT::Space::mod2());
  }
  static I32x8 get_modx8() {
    return i32x8::from(ModT::Space::mod());
  }
  static I32x8 reduce_m(I32x8 v) {
    I32x8 sign = i32x8::sign(v);
    v = i32x8::add(v, i256::bit_and(sign, get_modx8()));
    return v;
  }
  static I32x8 reduce_2m(I32x8 v) {
    I32x8 sign = i32x8::sign(v);
    v = i32x8::add(v, i256::bit_and(sign, get_mod2x8()));
    return v;
  }
  M32x8 &operator+=(const M32x8 &rhs) {
    v = i32x8::add(v, rhs.v);
    v = i32x8::sub(v, get_mod2x8());
    v = reduce_2m(v);
    return *this;
  }
  M32x8 &operator-=(const M32x8 &rhs) {
    v = i32x8::sub(v, rhs.v);
    v = reduce_2m(v);
    return *this;
  }
  static I32x8 reduce(const U64x4 &x0246, const U64x4 &x1357) {
    auto km0246 = u32x8::mul(u32x8::mul(x0246, get_irx8()), get_modx8());
    auto km1357 = u32x8::mul(u32x8::mul(x1357, get_irx8()), get_modx8());
    auto z0246 = i64x4::add(x0246, km0246);
    z0246 = i32x8::shuffle<0b11110101>(z0246);
    auto z1357 = i64x4::add(x1357, km1357);
    // z1357 = i32x8::shuffle<0b11110101>(z1357);
    return i32x8::blend<0b10101010>(z0246, z1357);
  }
  M32x8 &operator*=(const M32x8 &rhs) {
    auto [x0246, x1357] = u32x8::mul_0246_1357(v, rhs.v);
    v = reduce(x0246, x1357);
    return *this;
  }
  friend M32x8 operator+(const M32x8 &lhs, const M32x8 &rhs) {
    return M32x8(lhs) += rhs;
  }
  friend M32x8 operator-(const M32x8 &lhs, const M32x8 &rhs) {
    return M32x8(lhs) -= rhs;
  }
  friend M32x8 operator*(const M32x8 &lhs, const M32x8 &rhs) {
    return M32x8(lhs) *= rhs;
  }
  constexpr static M32x8 addmul(const M32x8 &a, const M32x8 &b, const M32x8 &c) { // (a + b) * c
    auto v = i32x8::add(a.v, b.v);
    auto [x0246, x1357] = u32x8::mul_0246_1357(v, c.v);
    v = reduce(x0246, x1357);
    return v;
  }
  constexpr static M32x8 submul(const M32x8 &a, const M32x8 &b, const M32x8 &c) { // (a - b) * c
    auto v = i32x8::sub(a.v, b.v);
    v = i32x8::add(v, get_mod2x8());
    auto [x0246, x1357] = u32x8::mul_0246_1357(v, c.v);
    v = reduce(x0246, x1357);
    return v;
  }
  U32x8 raw() const {
    return v;
  }
  template <i32 imm>
  M32x8 neg() const {
    auto sub = i32x8::sub(get_mod2x8(), v);
    return i32x8::blend<imm>(v, sub);
  }
  auto to_array() const {
    return i256::to_array<u32>(v);
  }
  template <i32 imm>
  M32x8 shuffle() const {
    return i32x8::shuffle<imm>(v);
  }
  template <i32 imm>
  M32x8 shufflex4() const {
    return i128x2::shuffle<imm>(v);
  }
};
} // namespace simd
#include <algorithm>
#include <bit>
#include <cassert>
#include <span>
#include <vector>
namespace detail {
template <class X8>
struct NttClassicalInfoAvx4 {
  using ModT = typename X8::ModT;
  using ValueT = typename ModT::ValueT;
  std::array<ModT, 64> rt, irt, rate2, irate2, rate3, irate3, rate4, irate4;
  std::array<X8, 64> rate4ix8, irate4ix8;
  X8 rt2, irt2, rt4, irt4;
  NttClassicalInfoAvx4() {
    const ValueT P = ModT::mod();
    const ValueT g = 3;
    const i32 rank2 = std::countr_zero(P - 1);
    rt[rank2] = ModT(g).pow((P - 1) >> rank2);
    irt[rank2] = rt[rank2].inv();
    for (i32 i = rank2; i >= 1; --i) {
      rt[i - 1] = rt[i] * rt[i];
      irt[i - 1] = irt[i] * irt[i];
    }
    ModT prod = 1, iprod = 1;
    for (i32 i = 0; i < rank2 - 1; ++i) {
      rate2[i] = prod * rt[i + 2];
      irate2[i] = iprod * irt[i + 2];
      prod *= irt[i + 2];
      iprod *= rt[i + 2];
    }
    prod = 1, iprod = 1;
    for (i32 i = 0; i < rank2 - 2; ++i) {
      rate3[i] = prod * rt[i + 3];
      irate3[i] = iprod * irt[i + 3];
      prod *= irt[i + 3];
      iprod *= rt[i + 3];
    }
    prod = 1, iprod = 1;
    alignas(32) std::array<ModT, 8> r, ir;
    for (i32 i = 0; i < rank2 - 3; ++i) {
      rate4[i] = prod * rt[i + 4];
      irate4[i] = iprod * irt[i + 4];
      prod *= irt[i + 4];
      iprod *= rt[i + 4];
      for (i32 j = 0; j < 8; ++j) {
        r[j] = rate4[i].pow(j);
        ir[j] = irate4[i].pow(j);
      }
      rate4ix8[i] = r;
      irate4ix8[i] = ir;
      std::fill(r.begin(), r.end(), 1);
      std::fill(ir.begin(), ir.end(), 1);
      r[3] = r[7] = rt[2];
      ir[3] = ir[7] = irt[2];
      rt2 = r, irt2 = ir;
      std::fill(r.begin(), r.end(), 1);
      std::fill(ir.begin(), ir.end(), 1);
      for (i32 i = 5; i < 8; ++i) {
        r[i] = r[i - 1] * rt[3];
        ir[i] = ir[i - 1] * irt[3];
      }
      rt4 = r, irt4 = ir;
    }
  }
};
template <montgomery_modint_concept ModT, bool aligned>
static void ntt_classical_avx4(std::span<ModT> f0) { // dif
  using X8 = simd::M32x8<ModT>;
  static constexpr auto load = simd::i256::load<aligned>;
  static constexpr auto store = simd::i256::store<aligned>;
  static const NttClassicalInfoAvx4<X8> info;
  i32 n8 = f0.size(), n = n8 / 8, l = n / 2, n_4b = std::countr_zero<u32>(n) & 1;
  assert(n8 % 16 == 0);
  auto *f = reinterpret_cast<simd::I256 *>(f0.data());
  if (n_4b) {
    for (i32 j = 0; j < l; ++j) {
      X8 fx = load(&f[j]), fy = load(&f[j + l]);
      X8 rx = fx + fy;
      X8 ry = fx - fy;
      store(&f[j], rx.v);
      store(&f[j + l], ry.v);
    }
    l /= 2;
  }
  for (l /= 2; l >= 1; l /= 4) {
    ModT r = 1, r2 = 1, r3 = 1;
    for (i32 i = 0, k = 0; i < n; i += l * 4, ++k) {
      X8 rx8 = X8::from(r), r2x8 = X8::from(r2), r3x8 = X8::from(r3);
      for (i32 j = 0; j < l; ++j) {
        X8 x0 = X8(load(&f[i + j + 0 * l]));
        X8 x1 = X8(load(&f[i + j + 1 * l])) * rx8;
        X8 x2 = X8(load(&f[i + j + 2 * l])) * r2x8;
        X8 x3 = X8(load(&f[i + j + 3 * l])) * r3x8;
        X8 x1x3 = X8::submul(x1, x3, X8::from(info.rt[2]));
        X8 x02 = x0 + x2, x0_2 = x0 - x2;
        X8 x13 = x1 + x3;
        X8 y0 = x02 + x13;
        X8 y1 = x02 - x13;
        X8 y2 = x0_2 + x1x3;
        X8 y3 = x0_2 - x1x3;
        store(&f[i + j + 0 * l], y0.v);
        store(&f[i + j + 1 * l], y1.v);
        store(&f[i + j + 2 * l], y2.v);
        store(&f[i + j + 3 * l], y3.v);
      }
      r *= info.rate3[std::countr_one<u32>(k)];
      r2 = r * r, r3 = r2 * r;
    }
  }
  X8 rti = X8::from(ModT(1));
  for (i32 i = 0; i < n; ++i) {
    X8 fi = load(&f[i]);
    fi *= rti;
    fi = fi.template neg<0b11110000>() + fi.template shufflex4<0b01>();
    fi *= info.rt4;
    fi = fi.template neg<0b11001100>() + fi.template shuffle<0b01001110>();
    fi *= info.rt2;
    fi = fi.template neg<0b10101010>() + fi.template shuffle<0b10110001>();
    store(&f[i], fi.v);
    rti *= info.rate4ix8[std::countr_one<u32>(i)];
  }
}
template <montgomery_modint_concept ModT, bool aligned>
static void intt_classical_avx4(std::span<ModT> f0) { // dit
  using X8 = simd::M32x8<ModT>;
  static constexpr auto load = simd::i256::load<aligned>;
  static constexpr auto store = simd::i256::store<aligned>;
  static const NttClassicalInfoAvx4<X8> info;
  i32 n8 = f0.size(), n = n8 / 8, l = 1, n_4b = std::countr_zero<u32>(n) & 1;
  assert(n8 % 16 == 0);
  auto *f = reinterpret_cast<simd::I256 *>(f0.data());
  X8 rti = X8::from(ModT(1));
  for (i32 i = 0; i < n; ++i) {
    X8 fi = load(&f[i]);
    fi = fi.template neg<0b10101010>() + fi.template shuffle<0b10110001>();
    fi *= info.irt2;
    fi = fi.template neg<0b11001100>() + fi.template shuffle<0b01001110>();
    fi *= info.irt4;
    fi = fi.template neg<0b11110000>() + fi.template shufflex4<0b01>();
    fi *= rti;
    store(&f[i], fi.v);
    rti *= info.irate4ix8[std::countr_one<u32>(i)];
  }
  for (; l < (n_4b ? n / 2 : n); l *= 4) {
    ModT r = 1, r2 = 1, r3 = 1;
    for (i32 i = 0, k = 0; i < n; i += l * 4, ++k) {
      X8 rx8 = X8::from(r), r2x8 = X8::from(r2), r3x8 = X8::from(r3);
      for (i32 j = 0; j < l; ++j) {
        X8 x0 = load(&f[i + j + 0 * l]);
        X8 x1 = load(&f[i + j + 1 * l]);
        X8 x2 = load(&f[i + j + 2 * l]);
        X8 x3 = load(&f[i + j + 3 * l]);
        X8 x2x3 = X8::submul(x2, x3, X8::from(info.irt[2]));
        X8 x01 = x0 + x1, x0_1 = x0 - x1;
        X8 x23 = x2 + x3;
        X8 y0 = x01 + x23;
        X8 y1 = X8::addmul(x0_1, x2x3, rx8);
        X8 y2 = X8::submul(x01, x23, r2x8);
        X8 y3 = X8::submul(x0_1, x2x3, r3x8);
        store(&f[i + j + 0 * l], y0.v);
        store(&f[i + j + 1 * l], y1.v);
        store(&f[i + j + 2 * l], y2.v);
        store(&f[i + j + 3 * l], y3.v);
      }
      r *= info.irate3[std::countr_one<u32>(k)];
      r2 = r * r, r3 = r2 * r;
    }
  }
  if (n_4b) {
    for (i32 j = 0; j < l; ++j) {
      X8 fx = load(&f[j]), fy = load(&f[j + l]);
      X8 rx = fx + fy;
      X8 ry = fx - fy;
      store(&f[j], rx.v);
      store(&f[j + l], ry.v);
    }
  }
  X8 ivn8 = X8::from(ModT(n8).inv());
  for (i32 i = 0; i < n; ++i) {
    X8 fi = load(&f[i]);
    fi *= ivn8;
    store(&f[i], fi.v);
  }
}
} // namespace detail
template <class ModT>
void ntt(std::span<ModT> f) {
  assert(std::has_single_bit(f.size()));
  detail::ntt_size += f.size();
  if (montgomery_modint_concept<ModT> && f.size() > 16) {
    if (u64(f.data()) & 0x1f)
      detail::ntt_classical_avx4<ModT, false>(f);
    else
      detail::ntt_classical_avx4<ModT, true>(f);
  } else {
    detail::ntt_classical_basic4(f);
  }
}
template <class ModT>
void intt(std::span<ModT> f) {
  assert(std::has_single_bit(f.size()));
  detail::ntt_size += f.size();
  if (montgomery_modint_concept<ModT> && f.size() > 16) {
    if (u64(f.data()) & 0x1f)
      detail::intt_classical_avx4<ModT, false>(f);
    else
      detail::intt_classical_avx4<ModT, true>(f);
  } else {
    detail::intt_classical_basic4(f);
  }
}
#include <span>
template <class ModT>
static void dot_basic(std::span<ModT> f, std::span<const ModT> g, std::span<ModT> dst) {
  u32 n = dst.size();
  for (u32 i = 0; i < n; i++)
    dst[i] = f[i] * g[i];
}
template <class ModT>
static void dot_basic(std::span<ModT> f, std::span<const ModT> g) {
  u32 n = f.size();
  for (u32 i = 0; i < n; i++)
    f[i] *= g[i];
}
template <class ModT>
static void dot_avx(std::span<ModT> f, std::span<const ModT> g) {
  using X8 = simd::M32x8<ModT>;
  using simd::i256::load, simd::i256::store;
  u32 n = f.size(), lf = u64(&f[0]) & 0x1f;
  const auto loadg = lf == (u64(&g[0]) & 0x1f) ? load<true> : load<false>;
  if (n < 16) {
    dot_basic(f, g);
  } else {
    u32 i = 0;
    for (; u64(&f[i]) & 0x1f; ++i) {
      f[i] *= g[i];
    }
    for (; i + 7 < n; i += 8) {
      X8 fi = load((simd::I256 *)&f[i]);
      X8 gi = loadg((simd::I256 *)&g[i]);
      fi *= gi;
      store((simd::I256 *)&f[i], fi.v);
    }
    for (; i < n; ++i) {
      f[i] *= g[i];
    }
  }
}
template <class ModT>
static void dot_avx(std::span<ModT> f, std::span<const ModT> g, std::span<ModT> dst) {
  using X8 = simd::M32x8<ModT>;
  using simd::i256::load, simd::i256::store;
  u32 n = f.size(), lf = u64(&f[0]) & 0x1f;
  const auto loadg = lf == (u64(&g[0]) & 0x1f) ? load<true> : load<false>;
  const auto storeg = lf == (u64(&dst[0]) & 0x1f) ? store<true> : store<false>;
  if (n < 16) {
    dot_basic(f, g);
  } else {
    u32 i = 0;
    for (; u64(&f[i]) & 0x1f; ++i) {
      dst[i] = f[i] * g[i];
    }
    for (; i + 7 < n; i += 8) {
      X8 fi = load((simd::I256 *)&f[i]);
      X8 gi = loadg((simd::I256 *)&g[i]);
      fi *= gi;
      stored((simd::I256 *)&dst[i], fi.v);
    }
    for (; i < n; ++i) {
      dst[i] *= g[i];
    }
  }
}
template <class ModT>
static void dot(std::span<ModT> f, std::span<const ModT> g, std::span<ModT> dst) {
  if constexpr (montgomery_modint_concept<ModT>) {
    dot_avx(f, g, dst);
  } else {
    dot_basic(f, g, dst);
  }
}
template <class ModT>
static void dot(std::span<ModT> f, std::span<const ModT> g) {
  if constexpr (montgomery_modint_concept<ModT>) {
    dot_avx(f, g);
  } else {
    dot_basic(f, g);
  }
}
#include <span>
#include <optional>
#include <vector>
#include <bit>
enum { NT_BLOCK_B = 16 };
namespace detail {
std::pair<u32, u32> nt_block_len(u32 m) {
  u32 t = std::bit_ceil((m - 1) / NT_BLOCK_B + 1);
  u32 k = (m - 1) / t + 1;
  return {t, k};
}
template <class ModT>
auto nt_block_split(AVec<ModT> &v, u32 m) {
  u32 k = v.size() / m;
  AVec<std::span<ModT>> sv(k);
  for (u32 i = 0; i < k; ++i) {
    sv[i] = std::span<ModT>(v.begin() + i * m, m);
  }
  return sv;
}
} // namespace detail
template <class ModT>
AVec<ModT> poly_inv_10E_block(std::span<const ModT> self, u32 m) {
  if (m == 1)
    return {self[0].inv()};
  auto [n, u] = detail::nt_block_len(m);
  AVec<ModT> x = poly_inv_10E_block(self, n);
  x.resize(n * u);
  AVec<ModT> nf0(n * u * 2), ng0(n * u * 2);
  auto nf = detail::nt_block_split(nf0, n * 2);
  auto ng = detail::nt_block_split(ng0, n * 2);
  auto xk = detail::nt_block_split(x, n);
  for (u32 k = 0; k < u; ++k) {
    std::copy(self.begin() + k * n, std::min(self.begin() + (k + 1) * n, self.end()), nf[k].begin());
    ntt<ModT>(nf[k]);
    if (k == 0)
      continue;
    std::copy(xk[k - 1].begin(), xk[k - 1].end(), ng[k - 1].begin());
    ntt<ModT>(ng[k - 1]);
    AVec<ModT> psi(n * 2);
    if (montgomery_modint_concept<ModT> && n > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *psix8 = reinterpret_cast<X8 *>(psi.data());
      u32 nx8 = n / 8;
      for (u32 j = 0; j < k; ++j) {
        auto *nf1x8 = reinterpret_cast<X8 *>(nf[k - j].data());
        auto *nf2x8 = reinterpret_cast<X8 *>(nf[k - j - 1].data());
        auto *ngx8 = reinterpret_cast<X8 *>(ng[j].data());
        for (u32 i = 0; i < nx8; ++i)
          psix8[i] -= (nf1x8[i] + nf2x8[i]) * ngx8[i];
        for (u32 i = nx8; i < nx8 * 2; ++i)
          psix8[i] -= (nf1x8[i] - nf2x8[i]) * ngx8[i];
      }
    } else {
      for (u32 j = 0; j < k; ++j) {
        for (u32 i = 0; i < n; ++i)
          psi[i] -= (nf[k - j][i] + nf[k - 1 - j][i]) * ng[j][i];
        for (u32 i = n; i < n * 2; ++i)
          psi[i] -= (nf[k - j][i] - nf[k - 1 - j][i]) * ng[j][i];
      }
    }
    intt<ModT>(psi);
    std::fill_n(psi.begin() + n, n, 0);
    ntt<ModT>(psi);
    dot<ModT>(psi, ng[0]);
    intt<ModT>(psi);
    std::copy_n(psi.begin(), n, xk[k].begin());
  }
  return x.resize(m), x;
}
template <class ModT>
AVec<ModT> poly_div_10E_block(std::span<const ModT> lhs, std::span<const ModT> rhs, u32 m) {
  if (lhs.empty() || rhs.empty())
    return {};
  if (m == 1)
    return {lhs[0] / rhs[0]};
  auto [n, u] = detail::nt_block_len(m);
  AVec<ModT> x = poly_div_10E_block(lhs, rhs, n), h = poly_inv_10E_block(rhs, n);
  x.resize(n * u), h.resize(n * 2);
  AVec<ModT> nf0(n * u * 2), ng0(n * u * 2);
  auto nf = detail::nt_block_split(nf0, n * 2);
  auto ng = detail::nt_block_split(ng0, n * 2);
  auto xk = detail::nt_block_split(x, n);
  ntt<ModT>(h);
  for (u32 k = 0; k < u; ++k) {
    std::copy(rhs.begin() + k * n, std::min(rhs.begin() + (k + 1) * n, rhs.end()), nf[k].begin());
    ntt<ModT>(nf[k]);
    if (k == 0)
      continue;
    std::copy(xk[k - 1].begin(), xk[k - 1].end(), ng[k - 1].begin());
    ntt<ModT>(ng[k - 1]);
    AVec<ModT> psi(n * 2);
    if (montgomery_modint_concept<ModT> && n > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *psix8 = reinterpret_cast<X8 *>(psi.data());
      u32 nx8 = n / 8;
      for (u32 j = 0; j < k; ++j) {
        auto *nf1x8 = reinterpret_cast<X8 *>(nf[k - j].data());
        auto *nf2x8 = reinterpret_cast<X8 *>(nf[k - j - 1].data());
        auto *ngx8 = reinterpret_cast<X8 *>(ng[j].data());
        for (u32 i = 0; i < nx8; ++i)
          psix8[i] -= (nf1x8[i] + nf2x8[i]) * ngx8[i];
        for (u32 i = nx8; i < nx8 * 2; ++i)
          psix8[i] -= (nf1x8[i] - nf2x8[i]) * ngx8[i];
      }
    } else {
      for (u32 j = 0; j < k; ++j) {
        for (u32 i = 0; i < n; ++i)
          psi[i] -= (nf[k - j][i] + nf[k - 1 - j][i]) * ng[j][i];
        for (u32 i = n; i < n * 2; ++i)
          psi[i] -= (nf[k - j][i] - nf[k - 1 - j][i]) * ng[j][i];
      }
    }
    intt<ModT>(psi);
    std::fill_n(psi.begin() + n, n, 0);
    for (u32 j = 0; j < std::min<u32>(n, lhs.size() - n * k); ++j)
      psi[j] += lhs[n * k + j];
    ntt<ModT>(psi);
    dot<ModT>(psi, h);
    intt<ModT>(psi);
    std::copy_n(psi.begin(), n, xk[k].begin());
  }
  return x.resize(m), x;
}
template <class ModT>
auto &prepare_inc(u32 m) {
  static AVec<ModT> inc{0, 1};
  m = std::bit_ceil(m);
  if (inc.size() < m) {
    u32 n = inc.size();
    inc.resize(m);
    for (u32 i = n; i < m; ++i) {
      inc[i] = i;
    }
  }
  return inc;
}
template <class ModT>
auto &prepare_inv(u32 m) {
  static AVec<ModT> iv{1, 1};
  const auto P = ModT::mod();
  m = std::bit_ceil(m);
  if (iv.size() < m) {
    u32 n = iv.size();
    iv.resize(m);
    for (u32 i = n; i < m; ++i) {
      iv[i] = iv[P % i] * (P - P / i);
    }
  }
  return iv;
}
template <class ModT, auto poly_div>
auto poly_ln(std::span<const ModT> f, u32 m) {
  AVec<ModT> x(m);
  const auto &inc = prepare_inc<ModT>(m);
  const auto &iv = prepare_inv<ModT>(m);
  std::copy(f.begin(), std::min(f.begin() + m, f.end()), x.begin());
  dot<ModT>(x, inc);
  x = poly_div(x, f, m);
  dot<ModT>(x, iv);
  return x;
}
template <class ModT>
AVec<ModT> poly_exp_14E_block(std::span<const ModT> self, u32 m) {
  if (m == 1)
    return {1};
  auto [n, u] = detail::nt_block_len(m);
  AVec<ModT> x = poly_exp_14E_block(self, n), h = poly_inv_10E_block<ModT>(x, n);
  x.resize(n * u), h.resize(n * 2);
  AVec<ModT> nf0(n * u * 2), ng0(n * u * 2);
  auto &iv = prepare_inv<ModT>(n * u);
  auto &inc = prepare_inc<ModT>(n * u);
  auto nf = detail::nt_block_split(nf0, n * 2);
  auto ng = detail::nt_block_split(ng0, n * 2);
  auto xk = detail::nt_block_split(x, n);
  ntt<ModT>(h);
  for (u32 k = 0; k < u; ++k) {
    std::copy(self.begin() + k * n, std::min(self.begin() + (k + 1) * n, self.end()), nf[k].begin());
    dot<ModT>({nf[k].begin(), n}, {inc.begin() + k * n, n});
    ntt<ModT>(nf[k]);
    if (k == 0)
      continue;
    std::copy(xk[k - 1].begin(), xk[k - 1].end(), ng[k - 1].begin());
    ntt<ModT>(ng[k - 1]);
    AVec<ModT> psi(n * 2);
    if (montgomery_modint_concept<ModT> && n > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *psix8 = reinterpret_cast<X8 *>(psi.data());
      u32 nx8 = n / 8;
      for (u32 j = 0; j < k; ++j) {
        auto *nf1x8 = reinterpret_cast<X8 *>(nf[k - j].data());
        auto *nf2x8 = reinterpret_cast<X8 *>(nf[k - j - 1].data());
        auto *ngx8 = reinterpret_cast<X8 *>(ng[j].data());
        for (u32 i = 0; i < nx8; ++i)
          psix8[i] += (nf1x8[i] + nf2x8[i]) * ngx8[i];
        for (u32 i = nx8; i < nx8 * 2; ++i)
          psix8[i] += (nf1x8[i] - nf2x8[i]) * ngx8[i];
      }
    } else {
      for (u32 j = 0; j < k; ++j) {
        for (u32 i = 0; i < n; ++i)
          psi[i] += (nf[k - j][i] + nf[k - 1 - j][i]) * ng[j][i];
        for (u32 i = n; i < n * 2; ++i)
          psi[i] += (nf[k - j][i] - nf[k - 1 - j][i]) * ng[j][i];
      }
    }
    intt<ModT>(psi);
    std::fill_n(psi.begin() + n, n, 0);
    ntt<ModT>(psi);
    dot<ModT>(psi, h);
    intt<ModT>(psi);
    std::fill_n(psi.begin() + n, n, 0);
    dot<ModT>({psi.begin(), n}, {iv.begin() + n * k, n});
    ntt<ModT>(psi);
    dot<ModT>(psi, ng[0]);
    intt<ModT>(psi);
    std::copy_n(psi.begin(), n, xk[k].begin());
  }
  return x.resize(m), x;
}
template <class ModT>
auto poly_inv_10E(std::span<const ModT> f, u32 m) {
  u32 n = std::bit_ceil(m);
  AVec<ModT> x(n);
  x[0] = f[0].inv();
  for (u32 t = 1; t < n; t *= 2) {
    AVec<ModT> f2(t * 2), nx(t * 2);
    std::copy(f.begin(), std::min(f.begin() + t * 2, f.end()), f2.begin());
    std::copy_n(x.begin(), t * 2, nx.begin());
    ntt<ModT>(f2); // 2E
    ntt<ModT>(nx); // 2E
    dot<ModT>(f2, nx);
    intt<ModT>(f2); // 2E
    std::fill_n(f2.begin(), t, 0);
    ntt<ModT>(f2); // 2E
    dot<ModT>(f2, nx);
    intt<ModT>(f2); // 2E
    for (u32 i = t; i < t * 2; ++i) {
      x[i] = -f2[i];
    }
  }
  return x.resize(m), x;
}
template <class ModT>
AVec<ModT> poly_sqrt_8E_block(std::span<const ModT> self, u32 m, const ModT &x0) {
  if (m == 1)
    return {x0};
  auto [n, u] = detail::nt_block_len(m);
  AVec<ModT> x = poly_sqrt_8E_block(self, n, x0), h = poly_inv_10E<ModT>(x, n);
  x.resize(n * u), h.resize(n * 2);
  AVec<ModT> ng0(n * u * 2);
  auto ng = detail::nt_block_split(ng0, n * 2);
  auto xk = detail::nt_block_split(x, n);
  ntt<ModT>(h);
  for (u32 k = 1; k < u; ++k) {
    std::copy(xk[k - 1].begin(), xk[k - 1].end(), ng[k - 1].begin());
    ntt<ModT>(ng[k - 1]);
    AVec<ModT> psi(n * 2);
    if (montgomery_modint_concept<ModT> && n > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *psix8 = reinterpret_cast<X8 *>(psi.data());
      u32 nx8 = n / 8;
      for (u32 j = 0; j < k; ++j) {
        auto *ng1x8 = reinterpret_cast<X8 *>(ng[k - j].data());
        auto *ng2x8 = reinterpret_cast<X8 *>(ng[k - j - 1].data());
        auto *ngx8 = reinterpret_cast<X8 *>(ng[j].data());
        if (j == 0) {
          for (u32 i = 0; i < nx8; ++i)
            psix8[i] += ng2x8[i] * ngx8[i];
          for (u32 i = nx8; i < nx8 * 2; ++i)
            psix8[i] -= ng2x8[i] * ngx8[i];
        } else {
          for (u32 i = 0; i < nx8; ++i)
            psix8[i] += (ng1x8[i] + ng2x8[i]) * ngx8[i];
          for (u32 i = nx8; i < nx8 * 2; ++i)
            psix8[i] += (ng1x8[i] - ng2x8[i]) * ngx8[i];
        }
      }
    } else {
      for (u32 j = 0; j < k; ++j) {
        if (j == 0) {
          for (u32 i = 0; i < n; i++)
            psi[i] += ng[k - 1 - j][i] * ng[j][i];
          for (u32 i = n; i < n * 2; i++)
            psi[i] -= ng[k - 1 - j][i] * ng[j][i];
        } else {
          for (u32 i = 0; i < n; i++)
            psi[i] += (ng[k - j][i] + ng[k - 1 - j][i]) * ng[j][i];
          for (u32 i = n; i < n * 2; i++)
            psi[i] += (ng[k - j][i] - ng[k - 1 - j][i]) * ng[j][i];
        }
      }
    }
    intt<ModT>(psi);
    std::fill_n(psi.begin() + n, n, 0);
    for (u32 j = 0; j < std::min<u32>(n, self.size() - n * k); j++)
      psi[j] = (self[n * k + j] - psi[j]).shift2();
    ntt<ModT>(psi);
    dot<ModT>(psi, h);
    intt<ModT>(psi);
    std::copy_n(psi.begin(), n, xk[k].begin());
  }
  return x.resize(m), x;
}
template <class ModT>
auto poly_invsqrt_12E(std::span<const ModT> self, u32 m, const ModT &x0) {
  u32 n = std::bit_ceil(m);
  AVec<ModT> x(n * 2);
  x[0] = x0;
  ModT ivn2 = -ModT(2).inv();
  for (u32 t = 1; t < n; t *= 2) {
    AVec<ModT> u(t * 4), s(t * 4);
    std::copy(self.begin(), std::min(self.begin() + t * 2, self.end()), u.begin());
    std::copy(x.begin(), x.begin() + t, s.begin());
    ntt<ModT>(u); // 4E
    ntt<ModT>(s); // 4E
    if (montgomery_modint_concept<ModT> && t * 4 > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *ux8 = reinterpret_cast<X8 *>(u.data());
      auto *sx8 = reinterpret_cast<X8 *>(s.data());
      X8 ivn2x8 = X8::from(ivn2);
      u32 nx8 = t * 4 / 8;
      for (u32 i = 0; i < nx8; ++i) {
        ux8[i] *= sx8[i] * sx8[i] * sx8[i] * ivn2x8;
      }
    } else {
      for (u32 i = 0; i < t * 4; ++i) {
        u[i] *= s[i] * s[i] * s[i] * ivn2;
      }
    }
    intt<ModT>(u); // 4E
    std::copy_n(u.begin() + t, t, x.begin() + t);
  }
  return x.resize(m), x;
}
template <class ModT, auto poly_ln, auto poly_exp>
auto poly_pow(std::span<const ModT> f, u64 k, u32 m) {
  if (k == 0) {
    AVec<ModT> x(m);
    x[0] = 1;
    return x;
  } else {
    ModT mk = ModT::safe(k);
    auto x = poly_ln(f, m);
    u32 n = x.size(), i = 0;
    if (montgomery_modint_concept<ModT> && n > 16) {
      using X8 = simd::M32x8<ModT>;
      auto *x8 = reinterpret_cast<X8 *>(x.data());
      u32 nx8 = n / 8;
      X8 mkx8 = X8::from(mk);
      for (; i < nx8; ++i) {
        x8[i] *= mkx8;
      }
    }
    for (i *= 8; i < n; ++i)
      x[i] *= mk;
    return poly_exp(std::move(x), m);
  }
}
#include <optional>
template <class ModT, auto poly_sqrt>
std::optional<AVec<ModT>> poly_safe_sqrt(std::span<const ModT> f, u32 m) {
  auto it = f.begin();
  while (it != f.end() && *it == 0)
    ++it;
  if (it == f.end())
    return AVec<ModT>(m);
  auto sq = it->sqrt();
  u32 len = it - f.begin();
  if (len % 2 == 1 || !sq.has_value())
    return std::nullopt;
  auto x = poly_sqrt({it, f.end()}, m - len / 2, sq.value());
  x.insert(x.begin(), len / 2, 0);
  return x;
}
template <class ModT, auto poly_pow>
auto poly_safe_pow(std::span<const ModT> f, u64 k, u64 k2, u32 m) {
  auto it = f.begin();
  while (it != f.end() && *it == 0)
    ++it;
  u32 len = it - f.begin();
  if (it == f.end() || (k > 1E9 && len >= 1) || k * len >= m) {
    AVec<ModT> r(m);
    r[0] = k == 0;
    return r;
  }
  AVec<ModT> x(it, f.end());
  ModT f0 = x[0], f0iv = f0.inv(), f0k = f0.pow(k2);
  for (auto &i : x)
    i *= f0iv;
  x = poly_pow(std::move(x), k, m - k * len);
  for (auto &i : x)
    i *= f0k;
  x.insert(x.begin(), k * len, 0);
  return x;
}
template <class ModT>
auto poly_deriv(std::span<const ModT> f, u32 m) {
  const auto &inc = prepare_inc<ModT>(m);
  AVec<ModT> x(m);
  std::copy(f.begin() + 1, std::min(f.begin() + m + 1, f.end()), x.begin());
  dot<ModT>(x, {inc.begin() + 1, m});
  return x;
}
template <class ModT>
auto poly_integr(std::span<const ModT> f, u32 m, u32 C) {
  AVec<ModT> x(m);
  std::copy(f.begin(), std::min(f.begin() + m - 1, f.end()), x.begin() + 1);
  const auto &iv = prepare_inv<ModT>(m);
  dot<ModT>(x, iv);
  x[0] = C;
  return x;
}
template <class ModT>
class Poly : public AVec<ModT> {
  using Vec = AVec<ModT>;
public:
  using Vec::empty;
  using Vec::resize;
  using Vec::size;
  using Vec::operator[];
  using Vec::begin;
  using Vec::cbegin;
  using Vec::cend;
  using Vec::end;
  static constexpr auto m_inv = poly_inv_10E<ModT>;
  static constexpr auto m_invsqrt = poly_invsqrt_12E<ModT>;
  static constexpr auto m_deriv = poly_deriv<ModT>;
  static constexpr auto m_integr = poly_integr<ModT>;
  static constexpr auto m_div = poly_div_10E_block<ModT>;
  static constexpr auto m_ln = poly_ln<ModT, m_div>;
  static constexpr auto m_exp = poly_exp_14E_block<ModT>;
  static constexpr auto m_sqrt = poly_sqrt_8E_block<ModT>;
  static constexpr auto m_safe_sqrt = poly_safe_sqrt<ModT, m_sqrt>;
  static constexpr auto m_pow = poly_pow<ModT, m_ln, m_exp>;
  static constexpr auto m_safe_pow = poly_safe_pow<ModT, m_pow>;
  Poly() = default;
  Poly(u32 len) : Vec(len) {}
  Poly(const std::vector<u32> &v) : Vec(v.begin(), v.end()) {}
  Poly(const std::vector<ModT> &v) : Vec(v.begin(), v.end()) {}
  Poly(AVec<ModT> v) : Vec(std::move(v)) {}
  Poly(const ModT &v) : Vec({v}) {}
  Poly &operator+=(const Poly &rhs) {
    if (size() < rhs.size())
      resize(rhs.size());
    for (u32 i = 0; i < rhs.size(); ++i)
      (*this)[i] += rhs[i];
    return *this;
  }
  Poly &operator-=(const Poly &rhs) {
    if (size() < rhs.size())
      resize(rhs.size());
    for (u32 i = 0; i < rhs.size(); ++i)
      (*this)[i] -= rhs[i];
    return *this;
  }
  Poly &operator*=(const Poly &rhs) {
    if (empty() || rhs.empty()) {
      return resize(0), *this;
    } else {
      u32 m = size() + rhs.size() - 1;
      u32 n = std::bit_ceil(m);
      resize(n);
      ntt<ModT>(*this);
      if (this->data() == rhs.data()) {
        dot<ModT>(*this, *this);
      } else {
        Vec vr(n);
        std::copy(rhs.cbegin(), rhs.cend(), vr.begin());
        ntt<ModT>(vr);
        dot<ModT>(*this, vr);
      }
      intt<ModT>(*this);
      return resize(m), *this;
    }
  }
  friend Poly operator*(const Poly &lhs, const Poly &rhs) {
    return Poly(lhs) *= rhs;
  }
  friend Poly operator+(const Poly &lhs, const Poly &rhs) {
    return Poly(lhs) += rhs;
  }
  friend Poly operator-(const Poly &lhs, const Poly &rhs) {
    return Poly(lhs) -= rhs;
  }
  Poly inv(u32 m) const {
    return m_inv(*this, m);
  }
  Poly deriv(u32 m) const {
    return m_deriv(*this, m);
  }
  Poly integr(u32 m, u32 C = 0) const {
    return m_integr(*this, m, C);
  }
  Poly div(const Poly &rhs, u32 m) {
    return m_div(*this, rhs, m);
  }
  Poly ln(u32 m) const {
    return m_ln(*this, m);
  }
  Poly exp(u32 m) const {
    return m_exp(*this, m);
  }
  Poly invsqrt(u32 m) const {
    return m_invsqrt(*this, m, this->front().sqrt().value().inv());
  }
  Poly sqrt(u32 m) const {
    return m_sqrt(*this, m, this->front().sqrt().value());
  }
  std::optional<Poly> sqrt_safe(u32 m) const {
    return m_safe_sqrt(*this, m);
  }
  Poly pow(u64 k, u32 m) const {
    return m_pow(*this, k, m);
  }
  Poly safe_pow(u64 k, u64 k2, u32 m) const {
    return m_safe_pow(*this, k, k2, m);
  }
};
#include <type_traits>
template <class T, T MOD>
struct MontgomerySpace;
template <u32 MOD>
struct MontgomerySpace<u32, MOD> {
  static_assert(2 < MOD && MOD < u32(1) << 30, "mod must in [3, 2^30)");
  static_assert(MOD % 2 == 1, "mod must be odd");
  using ValueT = u32;
  using TransT = u32;
  using rawU32 = std::false_type;
  using isMontgomery = std::true_type;
  constexpr static u32 get_nr() {
    u32 x = 1;
    for (i32 i = 0; i < 5; ++i)
      x *= 2 - x * MOD;
    return x;
  }
  consteval static u32 mod() {
    return MOD;
  }
  consteval static u32 mod2() {
    return mod() * 2;
  }
  consteval static u32 r() {
    return R;
  }
  consteval static u32 ir() {
    return IR;
  }
  enum : u32 {
    R = u32(u64(1) << 32 % MOD),
    IR = u32(-get_nr()),
    MOD2 = MOD * 2,
  };
  constexpr static TransT trans(ValueT x) {
    return (u64(x) << 32) % MOD;
  }
  constexpr static u32 reduce(u64 x) {
    return (x + u64(u32(x) * IR) * MOD) >> 32;
  }
  constexpr static u32 reduce_m(u32 n) {
    return n >> 31 ? n + MOD : n;
  }
  constexpr static u32 reduce_2m(u32 n) {
    return n >> 31 ? n + MOD2 : n;
  }
  constexpr static u32 add(u32 a, u32 b) {
    return reduce_2m(a + b - MOD2);
  }
  constexpr static u32 sub(u32 a, u32 b) {
    return reduce_2m(a - b);
  }
  constexpr static u32 mul(u32 a, u32 b) {
    return reduce(u64(a) * b);
  }
  constexpr static u32 muladd(u32 a, u32 b, u32 c) { // a * b + c
    return reduce(u64(a) * b + c);
  }
  constexpr static u32 mulsub(u32 a, u32 b, u32 c) { // a * b - c
    return reduce(u64(a) * b + MOD2 - c);
  }
  constexpr static u32 addmul(u32 a, u32 b, u32 c) { // (a + b) * c
    return reduce(u64(a + b) * c);
  }
  constexpr static u32 submul(u32 a, u32 b, u32 c) { // (a - b) * c
    return reduce(u64(a + MOD2 - b) * c);
  }
  constexpr static u32 safe(i64 x) {
    return reduce_m(x % MOD);
  }
  constexpr static ValueT val(TransT x) {
    return reduce_m(reduce(x) - MOD);
  }
  constexpr static u32 shift2(u32 x) {
    x = reduce(x);
    return (x & 1 ? x + MOD : x) >> 1;
  }
};
constexpr u32 qpow(u32 a, u64 b, u32 m) {
  u32 r = 1;
  for (; b > 0; b /= 2) {
    if (b % 2 == 1)
      r = u64(a) * r % m;
    a = u64(a) * a % m;
  }
  return r;
}
#include <algorithm>
#include <cassert>
#include <optional>
u32 legendre(u32 a, u32 p) {
  return qpow(a, (p - 1) / 2, p);
}
template <class ModT>
ModT legendre(ModT a) {
  return a.pow((ModT::mod() - 1) / 2);
}
std::optional<u32> cipola(u32 n, u32 p) {
  if (n == 0)
    return 0;
  if (legendre(n, p) != 1)
    return std::nullopt;
  if (p == 2)
    return 1;
  for (u32 a = 0; a < p; a++) {
    u32 i = (a * a - n + p) % p;
    using FP2 = std::pair<u64, u64>;
    auto mul = [p, i](const FP2 &l, const FP2 &r) {
      auto [la, lb] = l;
      auto [ra, rb] = r;
      return FP2{(la * ra + lb * rb % p * i) % p, (lb * ra + la * rb) % p};
    };
    if (legendre(i, p) == p - 1) {
      FP2 x = {1, 1}, u = {a, 1};
      for (int b = (p + 1) / 2; b; b /= 2) {
        if (b % 2 == 1)
          x = mul(x, u);
        u = mul(u, u);
      }
      return std::min(x.first, p - x.first);
    }
  }
  return std::nullopt;
}
template <class ModT>
std::optional<u32> cipola(const ModT &n) {
  if (n == 0)
    return 0;
  const u32 P = ModT::mod();
  if (legendre(n) != 1)
    return std::nullopt;
  if (P == 2)
    return 1;
  for (u32 a = 0; a < P; a++) {
    ModT i = a;
    i = i * i - n;
    using FP2 = std::pair<ModT, ModT>;
    auto mul = [i](const FP2 &l, const FP2 &r) {
      auto [la, lb] = l;
      auto [ra, rb] = r;
      return FP2{la * ra + lb * rb * i, lb * ra + la * rb};
    };
    if (legendre(i) == ModT(P - 1)) {
      FP2 x = {1, 1}, u = {a, 1};
      for (i32 b = (P + 1) / 2; b; b /= 2) {
        if (b % 2 == 1)
          x = mul(x, u);
        u = mul(u, u);
      }
      return std::min(x.first.val(), P - x.first.val());
    }
  }
  return std::nullopt;
}
#include <iostream>
// 封装 Modint，功能由 Space 提供
template <class Space_>
struct StaticModint {
  using Space = Space_;
  using ValueT = typename Space::ValueT;
  using TransT = typename Space::TransT;
  using isStatic = std::true_type;
  using rawU32 = typename Space::rawU32;
  using isMontgomery = typename Space::isMontgomery;
  TransT v;
  constexpr StaticModint() = default;
  constexpr StaticModint(ValueT v_) : v(Space::trans(v_)) {}
  using Self = StaticModint;
  explicit operator ValueT() const {
    return val();
  }
  constexpr static Self safe(i64 v) {
    return Space::safe(v);
  }
  constexpr static Self raw(u32 v) {
    Self r;
    r.v = v;
    return r;
  }
  constexpr ValueT val() const {
    return Space::val(v);
  }
  constexpr TransT raw() const {
    return v;
  }
  constexpr static ValueT mod() {
    return Space::mod();
  }
  constexpr Self &operator+=(const Self &rhs) {
    v = Space::add(v, rhs.v);
    return *this;
  }
  constexpr Self &operator-=(const Self &rhs) {
    v = Space::sub(v, rhs.v);
    return *this;
  }
  constexpr Self &operator*=(const Self &rhs) {
    v = Space::mul(v, rhs.v);
    return *this;
  }
  friend constexpr inline Self operator+(const Self &lhs, const Self &rhs) {
    return Self(lhs) += rhs;
  }
  friend constexpr inline Self operator-(const Self &lhs, const Self &rhs) {
    return Self(lhs) -= rhs;
  }
  friend constexpr inline Self operator*(const Self &lhs, const Self &rhs) {
    return Self(lhs) *= rhs;
  }
  constexpr Self pow(u64 n) const {
    Self r(1), a(*this);
    for (; n > 0; n /= 2) {
      if (n % 2 == 1)
        r *= a;
      a *= a;
    }
    return r;
  }
  constexpr Self inv() const {
    return pow(Space::mod() - 2);
  }
  constexpr Self &operator/=(const Self &rhs) {
    return *this *= rhs.inv();
  }
  friend constexpr inline Self operator/(const Self &lhs, const Self &rhs) {
    return Self(lhs) /= rhs;
  }
  constexpr Self operator-() const {
    return Self() -= *this;
  }
  constexpr std::optional<Self> sqrt() const {
    return cipola(*this);
  }
  constexpr Self shift2() const {
    return Space::shift2(v);
  }
  constexpr static Self muladd(const Self &a, const Self &b, const Self &c) { // a * b + c
    return raw(Space::muladd(a.raw(), b.raw(), c.raw()));
  }
  constexpr static Self mulsub(const Self &a, const Self &b, const Self &c) { // a * b - c
    return raw(Space::mulsub(a.raw(), b.raw(), c.raw()));
  }
  constexpr static Self addmul(const Self &a, const Self &b, const Self &c) { // (a + b) * c
    return raw(Space::addmul(a.raw(), b.raw(), c.raw()));
  }
  constexpr static Self submul(const Self &a, const Self &b, const Self &c) { // (a - b) * c
    return raw(Space::submul(a.raw(), b.raw(), c.raw()));
  }
  friend inline std::istream &operator>>(std::istream &is, Self &m) {
    i64 x;
    is >> x;
    m = Self::safe(x);
    return is;
  }
  friend inline std::ostream &operator<<(std::ostream &os, const Self &m) {
    return os << m.val();
  }
  friend inline bool operator==(const Self &lhs, const Self &rhs) {
    return lhs.val() == rhs.val();
  }
  friend inline bool operator!=(const Self &lhs, const Self &rhs) {
    return !(lhs == rhs);
  }
};
template <class Space>
inline FastI &operator>>(FastI &is, StaticModint<Space> &m) {
  i64 x;
  is >> x;
  m = StaticModint<Space>(x);
  return is;
}
template <class Space>
inline FastO &operator<<(FastO &os, const StaticModint<Space> &m) {
  return os << m.val();
}
using Space = MontgomerySpace<u32, 998244353>;
using ModT = StaticModint<Space>;
using FPS = Poly<ModT>;
i32 main() {
  FastI fin(stdin);
  FastO fout(stdout);
  u32 n;
  fin >> n;
  FPS f(n);
  for (auto &i : f)
    fin >> i;
  for (auto i : f.ln(n))
    fout << i << ' ';
  std::cerr << std::endl << detail::ntt_size << std::endl;
  return 0;
}

