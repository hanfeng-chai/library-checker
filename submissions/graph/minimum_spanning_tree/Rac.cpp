
#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#endif
#if defined(EVAL) && !defined(ONLINE_JUDGE)
#define ONLINE_JUDGE
#endif
#define NDEBUG


#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
namespace gsh {
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = long long;
using u64 = unsigned long long;
static_assert(sizeof(i64) == 8);
static_assert(sizeof(u64) == 8);
class InvalidFloat16Tag;
class InvalidBfloat16Tag;
class InvalidFloat128Tag;
#ifdef __STDCPP_FLOAT16_T__
using f16 = std::float16_t;
#else
using f16 = InvalidFloat16Tag;
#endif
#ifdef __STDCPP_FLOAT32_T__
using f32 = std::float32_t;
#else
static_assert(std::numeric_limits<float>::is_iec559, "There are no types compliant with IEC 559 binary32.");
using f32 = float;
#endif
#ifdef __STDCPP_FLOAT64_T__
using f64 = std::float64_t;
#else
static_assert(std::numeric_limits<double>::is_iec559, "There are no types compliant with IEC 559 binary64.");
using f64 = double;
#endif
#ifdef __STDCPP_FLOAT128_T__
using f128 = std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, __float128>;
#else
using f128 = std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, InvalidFloat128Tag>;
#endif
#ifdef __STDCPP_BFLOAT16_T__
using bf16 = std::bfloat16_t;
#else
using bf16 = InvalidBfloat16Tag;
#endif
using c8 = char;
using wc = wchar_t;
using utf8 = char8_t;
using utf16 = char16_t;
using utf32 = char32_t;
} // namespace gsh

namespace gsh {
class Exception {
  char str[512];
  char* cur = str;
  void write(const char* x) {
    for(int i = 0; i != 512; ++i, ++cur) {
      if(x[i] == '\0') break;
      *cur = x[i];
    }
  }
  void write(long long x) {
    if(x == 0) *(cur++) = '0';
    else {
      if(x < 0) {
        *(cur++) = '-';
        x = -x;
      }
      char buf[20];
      int i = 0;
      while(x != 0) buf[i++] = x % 10 + '0', x /= 10;
      while(i--) *(cur++) = buf[i];
    }
  }
  template<class T, class... Args> void generate_message(T x, Args... args) {
    write(x);
    if constexpr(sizeof...(Args) > 0) generate_message(args...);
  }
public:
  Exception() noexcept { *cur = '\0'; }
  Exception(const Exception& x) noexcept {
    for(int i = 0; i != 512; ++i) str[i] = x.str[i];
    cur = x.cur;
  }
  explicit Exception(const char* what_arg) noexcept {
    for(int i = 0; i != 512; ++i, ++cur) {
      *cur = what_arg[i];
      if(what_arg[i] == '\0') break;
    }
  }
  template<class... Args> explicit Exception(Args... args) noexcept {
    generate_message(args...);
    *cur = '\0';
  }
  Exception& operator=(const Exception& x) noexcept {
    for(int i = 0; i != 512; ++i) str[i] = x.str[i];
    cur = x.cur;
    return *this;
  }
  const char* what() const noexcept { return str; }
};
} // namespace gsh

#include <utility>
namespace gsh { namespace internal {
template<class D> class ArithmeticInterface {
  constexpr D& derived() { return *static_cast<D*>(this); }
  constexpr const D& derived() const { return *static_cast<const D*>(this); }
public:
  constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
    D copy = derived();
    ++derived();
    return copy;
  }
  constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
    D copy = derived();
    --derived();
    return copy;
  }
  constexpr D operator+() const noexcept(std::is_nothrow_copy_constructible_v<D>) requires requires(D x) { -x; } { return derived(); }
  constexpr bool operator!() const noexcept(noexcept(static_cast<bool>(derived()))) { return !static_cast<bool>(derived()); }
  friend constexpr auto operator+(const D& t1, const D& t2) noexcept(noexcept(D(t1) += t2)) { return D(t1) += t2; }
  friend constexpr auto operator-(const D& t1, const D& t2) noexcept(noexcept(D(t1) -= t2)) { return D(t1) -= t2; }
  friend constexpr auto operator*(const D& t1, const D& t2) noexcept(noexcept(D(t1) *= t2)) { return D(t1) *= t2; }
  friend constexpr auto operator/(const D& t1, const D& t2) noexcept(noexcept(D(t1) /= t2)) { return D(t1) /= t2; }
  friend constexpr auto operator%(const D& t1, const D& t2) noexcept(noexcept(D(t1) %= t2)) { return D(t1) %= t2; }
  friend constexpr auto operator&(const D& t1, const D& t2) noexcept(noexcept(D(t1) &= t2)) { return D(t1) &= t2; }
  friend constexpr auto operator|(const D& t1, const D& t2) noexcept(noexcept(D(t1) |= t2)) { return D(t1) |= t2; }
  friend constexpr auto operator^(const D& t1, const D& t2) noexcept(noexcept(D(t1) ^= t2)) { return D(t1) ^= t2; }
  template<class T> friend constexpr auto operator<<(const D& t1, const T& t2) noexcept(noexcept(D(t1) <<= t2)) { return D(t1) <<= t2; }
  template<class T> friend constexpr auto operator>>(const D& t1, const T& t2) noexcept(noexcept(D(t1) >>= t2)) { return D(t1) >>= t2; }
};
template<class D> class IteratorInterface {
  constexpr D& derived() { return *static_cast<D*>(this); }
  constexpr const D& derived() const { return *static_cast<const D*>(this); }
public:
  using size_type = u32;
  using difference_type = i32;
  constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
    D copy = derived();
    ++derived();
    return copy;
  }
  constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
    D copy = derived();
    --derived();
    return copy;
  }
  constexpr auto operator->() noexcept(noexcept(&*derived())) { return &*derived(); }
  constexpr auto operator->() const noexcept(noexcept(&*derived())) { return &*derived(); }
  template<class T> friend constexpr D operator+(const D& a, T&& n) noexcept(noexcept(D(a) += std::forward<T>(n))) { return D(a) += std::forward<T>(n); }
  template<class T> friend constexpr D operator-(const D& a, T&& n) noexcept(noexcept(D(a) -= std::forward<T>(n))) { return D(a) -= std::forward<T>(n); }
};
} // namespace internal
} // namespace gsh

#define GSH_INTERNAL_SELECT1(a, ...) a
#define GSH_INTERNAL_SELECT2(a, b, ...) b
#define GSH_INTERNAL_SELECT3(a, b, c, ...) c
#define GSH_INTERNAL_SELECT4(a, b, c, d, ...) d
#define GSH_INTERNAL_SELECT5(a, b, c, d, e, ...) e
#define GSH_INTERNAL_SELECT6(a, b, c, d, e, f, ...) f
#define GSH_INTERNAL_SELECT7(a, b, c, d, e, f, g, ...) g
#define GSH_INTERNAL_SELECT8(a, b, c, d, e, f, g, h, ...) h
#define GSH_INTERNAL_SELECT9(a, b, c, d, e, f, g, h, i, ...) i
#define GSH_INTERNAL_STR(s) #s
#define GSH_INTERNAL_CONCAT(a, b) a##b
#define GSH_INTERNAL_VA_SIZE(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#if defined(__clang__) || defined(__ICC)
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(unroll n))
#elif defined __GNUC__
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(GCC unroll n))
#else
#define GSH_INTERNAL_UNROLL(n)
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_INLINE __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE __attribute__((noinline))
#define GSH_INTERNAL_INLINE_LAMBDA __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE_LAMBDA __attribute__((noinline))
#elif defined _MSC_VER
#define GSH_INTERNAL_INLINE [[msvc::forceinline]]
#define GSH_INTERNAL_NOINLINE [[msvc::noinline]]
#define GSH_INTERNAL_INLINE_LAMBDA
#define GSH_INTERNAL_NOINLINE_LAMBDA
#else
#define GSH_INTERNAL_INLINE
#define GSH_INTERNAL_NOINLINE
#define GSH_INTERNAL_INLINE_LAMBDA
#define GSH_INTERNAL_NOINLINE_LAMBDA
#endif
#if defined(__GNUC__) || defined(__ICC)
#define GSH_INTERNAL_RESTRICT __restrict__
#elif defined _MSC_VER
#define GSH_INTERNAL_RESTRICT __restrict
#else
#define GSH_INTERNAL_RESTRICT
#endif
#ifdef __clang__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma(GSH_INTERNAL_STR(clang attribute push(__attribute__((__VA_ARGS__)), apply_to = apply)))
#define GSH_INTERNAL_POP_ATTRIBUTE _Pragma("clang attribute pop")
#elif defined __GNUC__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma("GCC push_options") _Pragma(GSH_INTERNAL_STR(GCC __VA_ARGS__))
#define GSH_INTERNAL_POP_ATTRIBUTE _Pragma("GCC pop_options")
#else
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...)
#define GSH_INTERNAL_POP_ATTRIBUTE
#endif

#include <bit>
#include <cstring>

namespace gsh {
[[noreturn]] inline void Unreachable() {
#if defined __GNUC__ || defined __clang__
  __builtin_unreachable();
#elif _MSC_VER
  __assume(false);
#else
  [[maybe_unused]] u32 n = 1 / 0;
#endif
};
GSH_INTERNAL_INLINE constexpr void Assume(const bool f) {
  if(std::is_constant_evaluated()) return;
#if defined __clang__
  __builtin_assume(f);
#elif defined __GNUC__
  if(!f) __builtin_unreachable();
#elif _MSC_VER
  __assume(f);
#else
  if(!f) Unreachable();
#endif
}
template<bool Likely = true> GSH_INTERNAL_INLINE constexpr bool Expect(const bool f) {
  if(std::is_constant_evaluated()) return f;
#if defined __GNUC__ || defined __clang__
  return __builtin_expect(f, Likely);
#else
  if constexpr(Likely) {
    if(f) [[likely]]
      return true;
    else return false;
  } else {
    if(f) [[unlikely]]
      return false;
    else return true;
  }
#endif
}
GSH_INTERNAL_INLINE constexpr bool Unpredictable(const bool f) {
  if(std::is_constant_evaluated()) return f;
#if defined __clang__
  return __builtin_unpredictable(f);
#elif defined __GNUC__
  return __builtin_expect_with_probability(f, 1, 0.5);
#else
  return f;
#endif
}
GSH_INTERNAL_INLINE inline void PreventConstexpr() noexcept {
  [[maybe_unused]] thread_local u8 dummy = 0;
  ++dummy;
}
class InPlaceTag {};
[[maybe_unused]] constexpr InPlaceTag InPlace;
template<class T> requires std::is_trivial_v<T> GSH_INTERNAL_INLINE constexpr void MemorySet(T* p, c8 byte, u32 len) {
  if(std::is_constant_evaluated()) {
    struct mem {
      c8 buf[sizeof(T)] = {};
    };
    mem init;
    for(u32 i = 0; i != sizeof(T); ++i) init.buf[i] = byte;
    for(u32 i = 0; i != len / sizeof(T); ++i) p[i] = std::bit_cast<T>(init);
    if(len % sizeof(T) != 0) {
      auto& ref = p[len / sizeof(T)];
      mem tmp = std::bit_cast<mem>(ref);
      for(u32 i = 0; i != len % sizeof(T); ++i) tmp.buf[i] = byte;
      ref = std::bit_cast<T>(tmp);
    }
  } else std::memset(p, byte, len);
}
template<class T> requires std::is_trivial_v<T> GSH_INTERNAL_INLINE constexpr u32 MemoryChar(T* p, c8 byte, u32 len) {
  if(std::is_constant_evaluated()) {
    struct mem {
      c8 buf[sizeof(T)] = {};
    };
    for(u32 i = 0; i != len / sizeof(T); ++i) {
      mem tmp = std::bit_cast<mem>(p[i]);
      for(u32 j = 0; j != sizeof(T); ++j) {
        if(tmp.buf[j] == byte) return i * sizeof(T) + j;
      }
    }
    if(len % sizeof(T) != 0) {
      mem tmp = std::bit_cast<mem>(p[len / sizeof(T)]);
      for(u32 i = 0; i != len % sizeof(T); ++i) {
        if(tmp.buf[i] == byte) return len / sizeof(T) * sizeof(T) + i;
      }
    }
    return 0xffffffff;
  } else {
    const void* tmp = std::memchr(p, byte, len);
    return (tmp == nullptr ? 0xffffffff : static_cast<const c8*>(tmp) - reinterpret_cast<const c8*>(p));
  }
}
template<class T, class U> requires std::is_trivial_v<T> GSH_INTERNAL_INLINE constexpr void MemoryCopy(T* GSH_INTERNAL_RESTRICT dst, U* GSH_INTERNAL_RESTRICT src, u32 len) {
  if(std::is_constant_evaluated()) {
    struct mem1 {
      c8 buf[sizeof(T)] = {};
    };
    struct mem2 {
      c8 buf[sizeof(U)] = {};
    };
    mem1 tmp1;
    mem2 tmp2;
    for(u32 i = 0; i != len; ++i) {
      if(i % sizeof(U) == 0) tmp2 = std::bit_cast<mem2>(src[i / sizeof(U)]);
      tmp1.buf[i % sizeof(T)] = tmp2.buf[i % sizeof(U)];
      if((i + 1) % sizeof(T) == 0) {
        dst[i / sizeof(T)] = std::bit_cast<T>(tmp1);
        tmp1 = mem1{};
      }
    }
    if(len % sizeof(T) != 0) {
      mem1 tmp3 = std::bit_cast<mem1>(dst[len / sizeof(T)]);
      for(u32 i = 0; i != len % sizeof(T); ++i) tmp3.buf[i] = tmp1.buf[i];
      dst[len / sizeof(T)] = std::bit_cast<T>(tmp3);
    }
  } else std::memcpy(dst, src, len);
}
/*
template<class T, class U>
    requires std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>
GSH_INTERNAL_INLINE constexpr void MemoryMove(T* dst, U* src, u32 len) {
    if (std::is_constant_evaluated()) {
    } else std::memmove(dst, src, len);
}
*/
GSH_INTERNAL_INLINE constexpr u32 StrLen(const c8* p) {
  if(std::is_constant_evaluated()) {
    auto q = p;
    while(*q != '\0') ++q;
    return q - p;
  } else return std::strlen(p);
}
namespace internal {
template<u32 N, class First, class... Tail> class TypeAtImpl : public TypeAtImpl<N - 1, Tail...> {};
template<class T, class... Types> class TypeAtImpl<0, T, Types...> {
public:
  using type = T;
};
} // namespace internal
template<u32 N, class... Types> using TypeAt = typename internal::TypeAtImpl<N, Types...>::type;
template<class... Types> class TypeArr {
public:
  constexpr static u32 size() noexcept { return sizeof...(Types); }
  template<u32 N> using type = std::conditional_t<(N < sizeof...(Types)), TypeAt<N, Types...>, void>;
};
template<> class TypeArr<> {
public:
  constexpr static u32 size() noexcept { return 0; }
  template<u32 N> using type = void;
};
} // namespace gsh

#include <compare>
namespace gsh {
namespace internal {
GSH_INTERNAL_INLINE constexpr std::pair<u64, u64> Divu128(u64 high, u64 low, u64 div) noexcept {
  if constexpr(sizeof(void*) == 8) {
    if(!std::is_constant_evaluated()) {
      u64 res, rem;
      __asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(div), "a"(low), "d"(high));
      return {res, rem};
    }
  }
  __uint128_t n = (static_cast<__uint128_t>(high) << 64 | low);
  __uint128_t res = n / div;
  return {res, n - res * div};
}
}
using i128 = __int128_t;
using u128 = __uint128_t;
} // namespace gsh

#include <array>

#include <concepts>

#include <functional>
#include <iterator>

#include <typeindex>

#include <variant>
namespace gsh {
namespace internal {
template<typename T, typename U> concept LessPtrCmp = requires(T&& t, U&& u) {
  { t < u } -> std::same_as<bool>;
} && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
} // namespace internal
class Less {
public:
  template<class T, class U> requires std::totally_ordered_with<T, U> GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
    if constexpr(internal::LessPtrCmp<T, U>) {
      if(std::is_constant_evaluated()) return t < u;
      auto x = reinterpret_cast<u64>(static_cast<const volatile void*>(std::forward<T>(t)));
      auto y = reinterpret_cast<u64>(static_cast<const volatile void*>(std::forward<U>(u)));
      return x < y;
    } else return std::forward<T>(t) < std::forward<U>(u);
  }
  using is_transparent = void;
};
class Greater {
public:
  template<class T, class U> requires std::totally_ordered_with<T, U> GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<U>() < std::declval<T>())) {
    if constexpr(internal::LessPtrCmp<U, T>) {
      if(std::is_constant_evaluated()) return u < t;
      auto x = reinterpret_cast<u64>(static_cast<const volatile void*>(std::forward<T>(t)));
      auto y = reinterpret_cast<u64>(static_cast<const volatile void*>(std::forward<U>(u)));
      return y < x;
    } else return std::forward<U>(u) < std::forward<T>(t);
  }
  using is_transparent = void;
};
class EqualTo {
public:
  template<class T, class U> requires std::equality_comparable_with<T, U> GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) { return std::forward<T>(t) == std::forward<U>(u); }
  using is_transparent = void;
};
class Identity {
public:
  template<class T> [[nodiscard]] GSH_INTERNAL_INLINE constexpr T&& operator()(T&& t) const noexcept { return std::forward<T>(t); }
  using is_transparent = void;
};
template<class F> class SwapArgs : public F {
public:
  constexpr SwapArgs() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
  constexpr SwapArgs(const F& f) noexcept(std::is_nothrow_copy_constructible_v<F>) : F(f) {}
  constexpr SwapArgs(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>) : F(std::move(f)) {}
  constexpr SwapArgs& operator=(const F& f) noexcept(std::is_nothrow_copy_assignable_v<F>) {
    F::operator=(f);
    return *this;
  }
  constexpr SwapArgs& operator=(F&& f) noexcept(std::is_nothrow_move_assignable_v<F>) {
    F::operator=(std::move(f));
    return *this;
  }
  constexpr SwapArgs& operator=(const SwapArgs&) noexcept(std::is_nothrow_copy_assignable_v<F>) = default;
  constexpr SwapArgs& operator=(SwapArgs&&) noexcept(std::is_nothrow_move_assignable_v<F>) = default;
  template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
  template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) const noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
};
template<class F, class... G> class BindFront {
  [[no_unique_address]] F func;
  [[no_unique_address]] BindFront<G...> bind;
public:
  constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F> && noexcept(BindFront<G...>())) : func(), bind() {}
  template<class Arg, class... Args> requires (sizeof...(Args) == sizeof...(G)) constexpr BindFront(Arg&& arg, Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Arg> && noexcept(BindFront<G...>(std::forward<Args>(args)...))) : func(std::forward<Arg>(arg)), bind(std::forward<Args>(args)...) {}
  template<class... Args> constexpr decltype(auto) operator()(Args&&... args) & noexcept(std::is_nothrow_invocable_v<F, Args...>) { return std::invoke(bind, std::invoke(func, std::forward<Args>(args)...)); }
  template<class... Args> constexpr decltype(auto) operator()(Args&&... args) && noexcept(std::is_nothrow_invocable_v<F, Args...>) { return std::invoke(std::move(bind), std::invoke(std::move(func), std::forward<Args>(args)...)); }
  template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return std::invoke(bind, std::invoke(func, std::forward<Args>(args)...)); }
  template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const&& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return std::invoke(std::move(bind), std::invoke(std::move(func), std::forward<Args>(args)...)); }
};
template<class F> class BindFront<F> : public F {
public:
  constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F>) : F() {}
  template<class... Args> constexpr BindFront(Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Args...>) : F(std::forward<Args>(args)...) {}
};
template<class T> class CustomizedHash;
namespace internal {
GSH_INTERNAL_INLINE constexpr u64 HashMix(u64 x) {
  constexpr u64 m = 0xe9846af9b1a615d;
  x ^= x >> 32;
  x *= m;
  x ^= x >> 32;
  x *= m;
  x ^= x >> 28;
  return x;
}
GSH_INTERNAL_INLINE constexpr u64 Mulx(u64 x, u64 y) {
  u128 r = static_cast<u128>(x) * y;
  return static_cast<u64>(r) ^ static_cast<u64>(r >> 64);
}
template<class T> concept IsCharType = std::same_as<T, char> || std::same_as<T, signed char> || std::same_as<T, unsigned char> || std::same_as<T, char8_t> || std::same_as<T, std::byte>;
template<std::random_access_iterator It> requires IsCharType<typename std::iterator_traits<It>::value_type> GSH_INTERNAL_INLINE constexpr u64 HashRangeBytes(It first, It last) {
  const auto* p = std::to_address(first);
  const u64 n = last - first;
  constexpr u64 q = 0x9e3779b97f4a7c15;
  constexpr u64 k = 0xdf442d22ce4859b9;
  u64 w = Mulx(0ull + q, k);
  u64 h = w ^ n;
  u64 remaining = n;
  while(remaining >= 8) {
    u64 v1;
    if(std::is_constant_evaluated()) {
      v1 = 0;
      for(int i = 0; i < 8; ++i) v1 |= static_cast<u64>(static_cast<unsigned char>(p[i])) << (i * 8);
    } else {
      std::memcpy(&v1, p, sizeof(v1));
    }
    w += q;
    h ^= Mulx(v1 + w, k);
    p += 8;
    remaining -= 8;
  }
  u64 v1 = 0;
  if(remaining > 0) {
    if(std::is_constant_evaluated()) {
      for(u64 i = 0; i < remaining; ++i) v1 |= static_cast<u64>(static_cast<unsigned char>(p[i])) << (i * 8);
    } else {
      std::memcpy(&v1, p, remaining);
    }
  }
  w += q;
  h ^= Mulx(v1 + w, k);
  return Mulx(h + w, k);
}
template<class T> concept CustomizedHashCallable = requires(T x) {
  { CustomizedHash<T>{}(x) } -> std::integral;
};
} // namespace internal
// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
class Hash {
public:
  template<class T> GSH_INTERNAL_INLINE constexpr u64 operator()(const T& v) const {
    using Type = std::remove_cvref_t<T>;
    constexpr bool tuple_like = requires {
      std::get<0>(v);
      std::tuple_size_v<Type>;
    };
    constexpr bool optional_like = requires {
      v.has_value();
      *v;
    };
    constexpr bool variant_like = requires {
      v.index();
      std::visit([](auto&&) {}, v);
    };
    if constexpr(internal::CustomizedHashCallable<Type>) {
      return static_cast<u64>(CustomizedHash<Type>{}(v));
    } else if constexpr(std::is_same_v<Type, std::nullptr_t>) {
      return (*this)(static_cast<void*>(nullptr));
    } else if constexpr(std::is_pointer_v<Type>) {
      auto x = reinterpret_cast<std::uintptr_t>(v);
      return (*this)(x + (x >> 3));
    } else if constexpr(std::is_enum_v<Type>) {
      return (*this)(static_cast<std::underlying_type_t<Type>>(v));
    } else if constexpr(std::integral<Type>) {
      if constexpr(sizeof(Type) <= sizeof(u64)) {
        return internal::HashMix(static_cast<u64>(v));
      } else {
        u128 val = static_cast<u128>(v);
        u64 seed = (*this)(static_cast<u64>(val >> 64));
        seed ^= (*this)(static_cast<u64>(val)) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
        return seed;
      }
    } else if constexpr(std::floating_point<Type>) {
      if constexpr(sizeof(Type) == 4) return (*this)(std::bit_cast<u32>(v));
      else if constexpr(sizeof(Type) == 8) return (*this)(std::bit_cast<u64>(v));
      else {
        u128 bits{};
        if(std::is_constant_evaluated()) {
          auto bytes = std::bit_cast<std::array<c8, sizeof(Type)>>(v);
          for(size_t i = 0; i < sizeof(Type); ++i) { reinterpret_cast<c8*>(&bits)[i] = bytes[i]; }
        } else {
          std::memcpy(&bits, &v, sizeof(v));
        }
        return (*this)(bits);
      }
    } else if constexpr(std::ranges::range<T>) {
      constexpr bool has_ptr = requires {
        v.data();
        v.size();
      };
      if constexpr(has_ptr && internal::IsCharType<typename std::iterator_traits<decltype(v.begin())>::value_type>) {
        return internal::HashRangeBytes(v.data(), v.data() + v.size());
      } else {
        u64 seed = 0;
        for(const auto& elem : v) { seed ^= (*this)(elem) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2); }
        return seed;
      }
    } else if constexpr(tuple_like) {
      u64 seed = 0;
      [&]<std::size_t... I>(std::index_sequence<I...>) { ((seed ^= (*this)(std::get<I>(v)) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2)), ...); }(std::make_index_sequence<std::tuple_size_v<Type>>{});
      return seed;
    } else if constexpr(optional_like) {
      if(v.has_value()) { return (*this)(*v); }
      return 0;
    } else if constexpr(variant_like) {
      u64 seed = (*this)(v.index());
      std::visit([&](auto&& val) { seed ^= (*this)(val) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2); }, v);
      return seed;
    } else if constexpr(std::is_standard_layout_v<Type> && std::is_trivial_v<Type>) {
      return internal::HashRangeBytes(reinterpret_cast<const c8*>(&v), reinterpret_cast<const c8*>(&v) + sizeof(Type));
    } else {
      static_assert(sizeof(T), "gsh::Hash cannot hash this type. Please provide a gsh::CustomizedHash specialization.");
      return 0;
    }
  }
  using is_transparent = void;
  using is_avalanching = void;
};
class Plus {
public:
  template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u))) { return std::forward<T>(t) + std::forward<U>(u); }
  using is_transparent = void;
};
class Minus {
public:
  template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) - std::forward<U>(u))) { return std::forward<T>(t) - std::forward<U>(u); }
  using is_transparent = void;
};
class Multiplies {
public:
  template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) * std::forward<U>(u))) { return std::forward<T>(t) * std::forward<U>(u); }
  using is_transparent = void;
};
class Divides {
public:
  template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) / std::forward<U>(u))) { return std::forward<T>(t) / std::forward<U>(u); }
  using is_transparent = void;
};
class Negate {
public:
  template<class T> constexpr decltype(auto) operator()(T&& t) const noexcept(noexcept(-std::forward<T>(t))) { return -std::forward<T>(t); }
  using is_transparent = void;
};
class True {
public:
  template<class... Args> constexpr bool operator()(Args&&...) noexcept { return true; }
  using is_transparent = void;
};
class False {
public:
  template<class... Args> constexpr bool operator()(Args&&...) noexcept { return false; }
  using is_transparent = void;
};
} // namespace gsh

#include <memory>
namespace gsh {
template<class T, class Alloc> requires std::is_same_v<T, typename std::allocator_traits<Alloc>::value_type> && (!std::is_const_v<T>)class NoInitArr;
template<class T, class Alloc> requires std::is_same_v<T, typename std::allocator_traits<Alloc>::value_type> && (!std::is_const_v<T>)class Vec;
} // namespace gsh

#include <algorithm>
#include <cctype>
#include <ranges>
#include <set>
namespace gsh {
enum class RangeKind { Sized, Unsized };
namespace internal {
template<class F, class T, class I, class U> concept IndirectlyBinaryLeftFoldableImpl = std::movable<T> && std::movable<U> && std::convertible_to<T, U> && std::invocable<F&, U, std::iter_reference_t<I>> && std::assignable_from<U&, std::invoke_result_t<F&, U, std::iter_reference_t<I>>>;
template<class F, class T, class I> concept IndirectlyBinaryLeftFoldable = std::copy_constructible<F> && std::indirectly_readable<I> && std::invocable<F&, T, std::iter_reference_t<I>> && std::convertible_to<std::invoke_result_t<F&, T, std::iter_reference_t<I>>, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>> && IndirectlyBinaryLeftFoldableImpl<F, T, I, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>>;
// These functions are defined in gsh/Algorithm.hpp
template<class R, class Comp, class Proj> constexpr auto MinImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto MaxImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class T, class F> constexpr auto FoldImpl(R&& r, T init, F&& f);
template<class R, class F> constexpr auto SumImpl(R&& r, F&& f);
template<class R, class F> constexpr void AdjacentDifferenceImpl(R&& r, F&& f);
template<class R> constexpr void ReverseImpl(R&& r);
template<class R, class Comp, class Proj> constexpr void SortImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto SortIndexImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto OrderImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto IsSortedImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto IsSortedUntilImpl(R&& r, Comp&& comp, Proj&& proj);
template<class Iter, class Sent, class T, class Proj, class Comp> constexpr auto LowerBoundImpl(Iter first, Sent last, const T& value, Comp&& comp, Proj&& proj);
template<class Iter, class Sent, class T, class Proj, class Comp> constexpr auto UpperBoundImpl(Iter first, Sent last, const T& value, Comp&& comp, Proj&& proj);
template<class R, class Equal> constexpr auto IsPalindromeImpl(R&& r, Equal&& equal);
// These functions are defined in gsh/Numeric.hpp
template<class R, class Proj> constexpr auto GCDImpl(R&& r, Proj&& proj);
template<class R, class Proj> constexpr auto LCMImpl(R&& r, Proj&& proj);
} // namespace internal
template<std::input_or_output_iterator I, std::sentinel_for<I> S, RangeKind K> requires (K == RangeKind::Sized || !std::sized_sentinel_for<S, I>) class Subrange;
template<class D, class V> requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>> class ViewInterface {
  constexpr D& derived() noexcept { return *static_cast<D*>(this); }
  constexpr const D& derived() const noexcept { return *static_cast<const D*>(this); }
  constexpr u32 get_size() const noexcept { return std::ranges::size(derived()); }
  GSH_INTERNAL_INLINE constexpr void check_index(u32 n) const {
    u32 sz = get_size();
    if(n >= sz) [[unlikely]]
      throw Exception("gsh::ViewInterface::check_index / The index is out of range. ( n=", n, ", size=", sz, " )");
  }
  GSH_INTERNAL_INLINE constexpr void check_index_on_debug(u32 n) const {
#ifndef NDEBUG
    check_index(n);
#else
    Assume(n < get_size());
#endif
  }
  using derived_type = D;
  using noinitarr_type = NoInitArr<V, std::allocator<V>>;
  using vec_type = Vec<V, std::allocator<V>>;
public:
  using value_type = V;
  using reference = V&;
  using const_reference = const V&;
  using pointer = V*;
  using const_pointer = const V*;
  using size_type = u32;
  using difference_type = i32;
  constexpr auto begin() { return std::ranges::begin(derived()); }
  constexpr auto begin() const { return std::ranges::begin(derived()); }
  constexpr auto end() { return std::ranges::end(derived()); }
  constexpr auto end() const { return std::ranges::end(derived()); }
  constexpr auto rbegin() { return std::reverse_iterator(end()); }
  constexpr auto rbegin() const { return std::reverse_iterator(cend()); }
  constexpr auto rend() { return std::reverse_iterator(begin()); }
  constexpr auto rend() const { return std::reverse_iterator(cbegin()); }
  constexpr auto cbegin() const { return std::ranges::cbegin(derived()); }
  constexpr auto cend() const { return std::ranges::cend(derived()); }
  constexpr auto crbegin() const { return std::reverse_iterator(cend()); }
  constexpr auto crend() const { return std::reverse_iterator(cbegin()); }
  constexpr reference front() { return *begin(); }
  constexpr const_reference front() const { return *cbegin(); }
  constexpr reference back() { return *rbegin(); }
  constexpr const_reference back() const { return *rbegin(); }
  constexpr reference operator[](u32 n) {
    check_index_on_debug(n);
    return *std::ranges::next(begin(), n);
  }
  constexpr const_reference operator[](u32 n) const {
    check_index_on_debug(n);
    return *std::ranges::next(begin(), n);
  }
  constexpr reference at(u32 n) {
    check_index(n);
    return *std::ranges::next(begin(), n);
  }
  constexpr const_reference at(u32 n) const {
    check_index(n);
    return *std::ranges::next(begin(), n);
  }
  template<class Iter, std::sentinel_for<Iter> Sent> constexpr void assign(Iter first, Sent last) { derived() = derived_type(std::move(first), std::move(last)); }
  template<std::ranges::input_range R, class... Args> static constexpr derived_type from_range(R&& r, Args&&... args) { return derived_type(std::ranges::begin(r), std::ranges::end(r), std::forward<Args>(args)...); }
  template<std::ranges::input_range R> constexpr void assign_range(R&& r) { derived().assign(std::ranges::begin(r), std::ranges::end(r)); }
  constexpr void assign_range(std::initializer_list<value_type> init) { derived().assign(init); }
  constexpr derived_type copy() const& { return derived(); }
  constexpr derived_type copy() & { return derived(); }
  constexpr derived_type copy() && { return std::move(derived()); }
  constexpr vec_type as_vec() const { return vec_type::from_range(derived()); };
  constexpr vec_type as_vec() { return vec_type::from_range(derived()); };
  constexpr auto as_set() const { return std::set(begin(), end()); }
  constexpr auto as_set() { return std::set(begin(), end()); }
  constexpr auto as_multiset() const { return std::multiset(begin(), end()); }
  constexpr auto as_multiset() { return std::multiset(begin(), end()); }
  constexpr auto slice() { return Subrange(begin(), end()); }
  constexpr auto slice(u32 start) { return Subrange(std::ranges::next(begin(), start), end()); }
  constexpr auto slice(u32 start, u32 end) { return Subrange(std::ranges::next(begin(), start), std::ranges::next(begin(), end)); }
  constexpr auto slice() const { return Subrange(begin(), end()); }
  constexpr auto slice(u32 start) const { return Subrange(std::ranges::next(begin(), start), end()); }
  constexpr auto slice(u32 start, u32 end) const { return Subrange(std::ranges::next(begin(), start), std::ranges::next(begin(), end)); }
  constexpr auto drop(u32 n) { return slice(n); }
  constexpr auto drop(u32 n) const { return slice(n); }
  constexpr auto take(u32 n) { return Subrange(begin(), std::ranges::next(begin(), n)); }
  constexpr auto take(u32 n) const { return Subrange(begin(), std::ranges::next(begin(), n)); }
  template<class T> static constexpr derived_type iota(T&& end) { return from_range(std::views::iota(std::decay_t<T>(), end)); }
  template<class T, class U> static constexpr derived_type iota(T&& start, U&& end) {
    using ct = std::common_type_t<T, U>;
    return from_range(std::views::iota(static_cast<ct>(start), static_cast<ct>(end)));
  }
  template<class Proj> requires std::ranges::input_range<derived_type> constexpr void iterate(Proj&& proj = {}) {
    auto itr = begin();
    auto sent = end();
    for(u32 idx = 0; itr != sent; ++itr, ++idx) {
      if constexpr(requires { std::invoke(proj, *itr, idx, *this); }) std::invoke(proj, *itr, idx, *this);
      else if constexpr(requires { std::invoke(proj, *itr, idx); }) std::invoke(proj, *itr, idx);
      else if constexpr(requires { std::invoke(proj, *itr); }) std::invoke(proj, *itr);
      else proj();
    }
  }
  template<class Proj> requires std::ranges::input_range<derived_type> constexpr void iterate(Proj&& proj = {}) const {
    auto itr = begin();
    auto sent = end();
    for(u32 idx = 0; itr != sent; ++itr, ++idx) {
      if constexpr(requires { std::invoke(proj, *itr, idx, *this); }) std::invoke(proj, *itr, idx, *this);
      else if constexpr(requires { std::invoke(proj, *itr, idx); }) std::invoke(proj, *itr, idx);
      else if constexpr(requires { std::invoke(proj, *itr); }) std::invoke(proj, *itr);
      else std::invoke(proj);
    }
  }
  template<std::indirect_unary_predicate<std::ranges::iterator_t<derived_type>> Pred> requires std::ranges::input_range<derived_type> [[nodiscard]] constexpr auto filter_view(Pred&& pred = {}) const { return derived() | std::views::filter(std::forward<Pred>(pred)); }
  template<std::invocable<value_type> Proj> requires std::ranges::input_range<derived_type> [[nodiscard]] constexpr auto transform_view(Proj&& proj = {}) const { return derived() | std::views::transform(std::forward<Proj>(proj)); }
  template<std::indirect_unary_predicate<std::ranges::iterator_t<derived_type>> Pred> requires std::ranges::input_range<derived_type> [[nodiscard]] constexpr auto erase_if(Pred&& pred = {}) {
    auto view = derived().filter([&](auto&& x) { return !std::invoke(pred, x); });
    derived() = derived_type(std::ranges::begin(view), std::ranges::end(view));
  }
  template<std::invocable<value_type> Proj> requires std::ranges::input_range<derived_type> && std::ranges::output_range<derived_type, std::invoke_result_t<Proj, value_type&&>> constexpr void apply(Proj&& proj = {}) {
    for(auto&& x : derived()) x = std::invoke(proj, std::move(x));
  }
  template<class T> requires std::ranges::output_range<derived_type, const T&> constexpr void fill(const T& x) {
    for(auto& el : derived()) el = x;
  }
  /*
  template<class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::forward_range<derived_type> && std::permutable<std::ranges::iterator_t<derived_type>> constexpr void unique(Comp&& comp = {}, Proj&& proj = {}) {
    const auto sr = std::ranges::unique(begin(), end(), std::forward<Comp>(comp), std::forward<Proj>(proj));
    if constexpr(requires { derived().erase(sr.begin(), sr.end()); }) derived().erase(sr.begin(), sr.end());
    else derived() = derived_type(begin(), sr.begin());
  }
  */
  template<std::predicate<value_type> Pred> requires std::ranges::input_range<derived_type> constexpr bool all_of(Pred&& f = {}) const {
    for(auto&& el : derived())
      if(!std::invoke(f, std::forward<decltype(el)>(el))) return false;
    return true;
  }
  template<std::predicate<value_type> Pred> requires std::ranges::input_range<derived_type> constexpr bool any_of(Pred&& f = {}) const {
    for(auto&& el : derived())
      if(std::invoke(f, std::forward<decltype(el)>(el))) return true;
    return false;
  }
  template<std::predicate<value_type> Pred> requires std::ranges::input_range<derived_type> constexpr bool none_of(Pred f) const {
    for(auto&& el : derived())
      if(std::invoke(f, std::forward<decltype(el)>(el))) return false;
    return true;
  }
  template<class T, class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::input_range<derived_type> constexpr bool contains(const T& x, Comp&& comp = {}, Proj&& proj = {}) const {
    for(auto&& el : derived())
      if(std::invoke(comp, x, std::invoke(proj, std::forward<decltype(el)>(el)))) return true;
    return false;
  }
  template<class T, class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::input_range<derived_type> constexpr auto find(const T& x, Comp&& comp = {}, Proj&& proj = {}) const {
    const auto sent = end();
    auto itr = begin();
    for(; itr != sent; ++itr)
      if(std::invoke(comp, x, std::invoke(proj, *itr))) return itr;
    return itr;
  }
  template<class T, class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::input_range<derived_type> constexpr auto rfind(const T& x, Comp&& comp = {}, Proj&& proj = {}) const {
    if constexpr(std::ranges::bidirectional_range<derived_type>) {
      const auto sent = rend();
      auto itr = rbegin();
      for(; itr != sent; ++itr)
        if(std::invoke(comp, x, std::invoke(proj, *itr))) return --itr.base();
      return end();
    } else {
      const auto sent = end();
      auto itr = begin();
      decltype(itr) result = itr;
      bool found = false;
      for(; itr != sent; ++itr) {
        if(std::invoke(comp, x, std::invoke(proj, *itr))) {
          result = itr;
          found = true;
        }
      }
      if(!found) result = itr;
      return result;
    }
  }
  template<class T, class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::input_range<derived_type> constexpr u32 index_of(const T& x, Comp&& comp = {}, Proj&& proj = {}) const {
    const auto sent = end();
    u32 cnt = 0;
    for(auto itr = begin(); itr != sent; ++itr) {
      if(std::invoke(comp, x, std::invoke(proj, *itr))) return cnt;
      ++cnt;
    }
    return cnt;
  }
  template<class T, class Proj = Identity, std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = EqualTo> requires std::ranges::input_range<derived_type> constexpr u32 count(const T& x, Comp&& comp = {}, Proj&& proj = {}) const {
    u32 res = 0;
    for(auto&& el : derived()) res += static_cast<bool>(std::invoke(comp, x, std::invoke(proj, std::forward<decltype(el)>(el))));
    return res;
  }
  template<class Proj = Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> && std::indirectly_copyable_storable<std::ranges::iterator_t<derived_type>, value_type*> constexpr auto min(Comp&& comp = {}, Proj&& proj = {}) const { return internal::MinImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class Proj = Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> && std::indirectly_copyable_storable<std::ranges::iterator_t<derived_type>, value_type*> constexpr auto max(Comp&& comp = {}, Proj&& proj = {}) const { return internal::MaxImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class T = value_type, internal::IndirectlyBinaryLeftFoldable<T, std::ranges::iterator_t<derived_type>> F = Plus> requires std::ranges::forward_range<derived_type> constexpr auto fold(T&& init = {}, F&& f = {}) const { return internal::FoldImpl(derived(), std::forward<T>(init), std::forward<F>(f)); }
  template<internal::IndirectlyBinaryLeftFoldable<value_type, std::ranges::iterator_t<derived_type>> F = Plus> requires std::ranges::forward_range<derived_type> constexpr auto sum(F&& f = {}) const { return internal::SumImpl(derived(), std::forward<F>(f)); }
  template<class F = Minus> requires std::ranges::forward_range<derived_type> && std::indirectly_writable<std::ranges::iterator_t<derived_type>, std::invoke_result_t<F, value_type, value_type>> constexpr void adjacent_difference(F&& f = {}) { internal::AdjacentDifferenceImpl(derived(), std::forward<F>(f)); }
  template<class F = Minus> constexpr auto adjacent_differenced(F&& f = {}) {
    auto res = derived();
    internal::AdjacentDifferenceImpl(res, std::forward<F>(f));
    return res;
  }
  void reverse() requires std::ranges::bidirectional_range<derived_type> && std::permutable<std::ranges::iterator_t<derived_type>> { internal::ReverseImpl(derived()); }
  [[nodiscard]] auto reversed() const requires std::permutable<std::ranges::iterator_t<vec_type>> {
    auto res = as_vec();
    internal::ReverseImpl(res);
    return res;
  }
  [[nodiscard]] auto reversed_view() const requires std::ranges::view<derived_type> && std::ranges::bidirectional_range<derived_type> { return derived() | std::views::reverse; }
  template<class Proj = Identity, class Comp = Less> requires std::ranges::forward_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj> constexpr void sort(Comp&& comp = {}, Proj&& proj = {}) { internal::SortImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class Proj = Identity, class Comp = Less> requires std::sortable<std::ranges::iterator_t<vec_type>, Comp, Proj> [[nodiscard]] constexpr auto sorted(Comp&& comp = {}, Proj&& proj = {}) const {
    auto res = as_vec();
    internal::SortImpl(res, std::forward<Comp>(comp), std::forward<Proj>(proj));
    return res;
  }
  template<class Proj = Identity, class Comp = Less> requires std::ranges::random_access_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj> constexpr auto sort_index(Comp&& comp = {}, Proj&& proj = {}) const { return internal::SortIndexImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class Proj = Identity, class Comp = Less> requires std::ranges::random_access_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj> constexpr auto order(Comp&& comp = {}, Proj&& proj = {}) const { return internal::OrderImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class Proj = Identity, class Comp = Less, class R> requires std::ranges::random_access_range<derived_type> && std::ranges::forward_range<R> constexpr void sort_with(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
#ifndef NDEBUG
    if(std::ranges::size(r) != std::ranges::size(derived())) { throw Exception("gsh::ViewInterface::sort_with / The size of the range passed as an argument differs from the size of the key."); }
#endif
    auto ord = derived().sort_index(std::forward<Comp>(comp), std::forward<Proj>(proj)).inverse();
    derived().permute(ord);
    Subrange{std::ranges::begin(r), std::ranges::end(r)}.permute(ord);
  };
  template<class Proj = Identity, class Comp = Less, class... Rs> requires std::ranges::random_access_range<derived_type> && (std::ranges::forward_range<Rs> && ...) constexpr void sort_with(std::tuple<Rs...> rs, Comp&& comp = {}, Proj&& proj = {}) {
#ifndef NDEBUG
    u32 sz = std::ranges::size(derived());
    auto check = [&](auto&&... r) {
      if((... || (std::ranges::size(r) != sz))) { throw Exception("gsh::ViewInterface::sort_with / The size of the range passed as an argument differs from the size of the key."); }
    };
    std::apply(check, rs);
#endif
    auto ord = derived().sort_index(std::forward<Comp>(comp), std::forward<Proj>(proj)).inverse();
    derived().permute(ord);
    std::apply([&](auto&&... r) { (Subrange{std::ranges::begin(r), std::ranges::end(r)}.permute(ord), ...); }, rs);
  };
  template<class Proj = Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto is_sorted(Comp&& comp = {}, Proj&& proj = {}) const { return internal::IsSortedImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class Proj = Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto is_sorted_until(Comp&& comp = {}, Proj&& proj = {}) const { return internal::IsSortedUntilImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto lower_bound(const T& value, Comp&& comp = {}, Proj&& proj = {}) const { return internal::LowerBoundImpl(begin(), end(), value, std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto lower_bound_index(const T& value, Comp&& comp = {}, Proj&& proj = {}) const { return std::ranges::distance(begin(), lower_bound(value, std::forward<Comp>(comp), std::forward<Proj>(proj))); }
  template<class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto upper_bound(const T& value, Comp&& comp = {}, Proj&& proj = {}) const { return internal::UpperBoundImpl(begin(), end(), value, std::forward<Comp>(comp), std::forward<Proj>(proj)); }
  template<class T, class Proj = Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp = Less> requires std::ranges::forward_range<derived_type> constexpr auto upper_bound_index(const T& value, Comp&& comp = {}, Proj&& proj = {}) const { return std::ranges::distance(begin(), upper_bound(value, std::forward<Comp>(comp), std::forward<Proj>(proj))); }
  template<std::indirect_binary_predicate<std::ranges::iterator_t<derived_type>, std::ranges::iterator_t<derived_type>> Equal = EqualTo> requires std::ranges::bidirectional_range<derived_type> constexpr auto is_palindrome(Equal&& equal = {}) const { return internal::IsPalindromeImpl(derived(), std::forward<Equal>(equal)); }
  template<class Proj = Identity> requires std::ranges::input_range<derived_type> && std::invocable<Proj, value_type> constexpr auto gcd(Proj&& proj = {}) const { return internal::GCDImpl(derived(), std::forward<Proj>(proj)); }
  template<class Proj = Identity> requires std::ranges::input_range<derived_type> && std::invocable<Proj, value_type> constexpr auto lcm(Proj&& proj = {}) const { return internal::LCMImpl(derived(), std::forward<Proj>(proj)); }
  constexpr bool is_capitalized() const requires std::ranges::input_range<derived_type> && std::same_as<value_type, c8> {
    auto itr = begin();
    auto sent = end();
    if(!(itr != sent) || !std::isupper(*itr)) return false;
    ++itr;
    while(itr != sent) {
      if(!std::islower(*itr)) return false;
      ++itr;
    }
    return true;
  }
  constexpr void lower() requires std::ranges::input_range<derived_type> && std::same_as<value_type, c8> {
    auto itr = begin();
    auto sent = end();
    while(itr != sent) {
      *itr = std::tolower(*itr);
      ++itr;
    }
  }
  constexpr void upper() requires std::ranges::input_range<derived_type> && std::same_as<value_type, c8> {
    auto itr = begin();
    auto sent = end();
    while(itr != sent) {
      *itr = std::toupper(*itr);
      ++itr;
    }
  }
  template<class R> requires std::ranges::forward_range<derived_type> && std::ranges::input_range<R> constexpr void permute(R&& location) {
    auto itr = begin();
    auto sent = end();
    auto loc_begin = std::ranges::begin(location);
    u32 size = std::ranges::distance(itr, sent);
    noinitarr_type tmp(size);
    for(u32 i = 0; i != size; ++i) std::construct_at(&tmp[*(loc_begin++)], std::move(*(itr++)));
    itr = begin();
    for(u32 i = 0; i != size; ++i) *(itr++) = std::move(tmp[i]);
  }
  constexpr auto inverse() requires std::ranges::random_access_range<derived_type> {
    auto itr = begin();
    auto sent = end();
    u32 size = std::ranges::distance(itr, sent);
    vec_type tmp(size);
    for(auto i = static_cast<value_type>(0); i != size; ++i) std::construct_at(&tmp[*(itr++)], i);
    return tmp;
  }
};
template<std::ranges::forward_range T, std::ranges::forward_range U> requires (std::derived_from<T, ViewInterface<T, typename T::value_type>> || std::derived_from<U, ViewInterface<U, typename U::value_type>>) auto operator<=>(const T& a, const U& b) { return std::lexicographical_compare_three_way(std::ranges::begin(a), std::ranges::end(a), std::ranges::begin(b), std::ranges::end(b)); }
template<std::ranges::forward_range T, std::ranges::forward_range U> requires (std::derived_from<T, ViewInterface<T, typename T::value_type>> || std::derived_from<U, ViewInterface<U, typename U::value_type>>) bool operator==(const T& a, const U& b) { return std::ranges::equal(std::ranges::begin(a), std::ranges::end(a), std::ranges::begin(b), std::ranges::end(b)); }
namespace internal {
template<class T, class U> concept difference_from = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
template<class From, class To> concept convertible_to_non_slicing = std::convertible_to<From, To> && !(std::is_pointer_v<std::decay_t<From>> && std::is_pointer_v<std::decay_t<To>> && !std::convertible_to<std::remove_pointer_t<std::decay_t<From>> (*)[], std::remove_pointer_t<std::decay_t<To>> (*)[]>);
template<class T> concept pair_like = /* tuple-like<T> && */ std::tuple_size_v<std::remove_cvref_t<T>> == 2;
template<class T, class U, class V> concept pair_like_convertible_from = !std::ranges::range<T> && !std::is_reference_v<T> && pair_like<T> && std::constructible_from<T, U, V> && convertible_to_non_slicing<U, std::tuple_element_t<0, T>> && std::convertible_to<V, std::tuple_element_t<1, T>>;
} // namespace internal
template<std::input_or_output_iterator I, std::sentinel_for<I> S = I, RangeKind K = std::sized_sentinel_for<S, I> ? RangeKind::Sized : RangeKind::Unsized> requires (K == RangeKind::Sized || !std::sized_sentinel_for<S, I>) class Subrange : public ViewInterface<Subrange<I, S, K>, std::iter_value_t<I>> {
  I itr;
  S sent;
  static constexpr bool StoreSize = (K == RangeKind::Sized && !std::sized_sentinel_for<S, I>);
  struct empty_sz {};
  [[no_unique_address]] std::conditional_t<StoreSize, std::make_unsigned_t<std::iter_difference_t<I>>, empty_sz> sz;
public:
  constexpr Subrange() = default;
  constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s) requires (!StoreSize) : itr(i), sent(s) {}
  constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s, std::make_unsigned_t<std::iter_difference_t<I>> n) requires (K == RangeKind::Sized) : itr(i), sent(s) {
    if constexpr(StoreSize) sz = n;
  }
  template<internal::difference_from<Subrange> R> requires std::ranges::borrowed_range<R> && internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I> && std::convertible_to<std::ranges::sentinel_t<R>, S> constexpr Subrange(R&& r) requires (!StoreSize || std::ranges::sized_range<R>) : itr(std::ranges::begin(r)), sent(std::ranges::end(r)) {}
  template<std::ranges::borrowed_range R> requires internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I> && std::convertible_to<std::ranges::sentinel_t<R>, S> constexpr Subrange(R&& r, std::make_unsigned_t<std::iter_difference_t<I>> n) requires (K == RangeKind::Sized) : Subrange{std::ranges::begin(r), std::ranges::end(r), n} {}
  template<internal::difference_from<Subrange> PairLike> requires internal::pair_like_convertible_from<PairLike, const I&, const S&> constexpr operator PairLike() const { return PairLike(itr, sent); }
  constexpr I begin() const requires std::copyable<I> { return itr; }
  [[nodiscard]] constexpr I begin() requires (!std::copyable<I>) { return std::move(itr); }
  constexpr S end() const { return sent; }
  constexpr bool empty() const { return itr == sent; }
  constexpr u32 size() const requires (K == RangeKind::Sized) { return static_cast<u32>(std::ranges::distance(itr, sent)); }
  constexpr I data() const requires (std::is_pointer_v<I> && std::copyable<I>) { return itr; }
  constexpr I data() const requires (std::is_pointer_v<I> && !std::copyable<I>) { return std::move(itr); }
  constexpr decltype(auto) operator[](const std::iter_difference_t<I>& n) { return *std::ranges::next(begin(), n); }
  constexpr decltype(auto) operator[](const std::iter_difference_t<I>& n) const { return *std::ranges::next(begin(), n); }
  [[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n = 1) const& requires std::forward_iterator<I> {
    auto tmp = *this;
    tmp.advance(n);
    return tmp;
  }
  [[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n = 1) && {
    advance(n);
    return std::move(*this);
  }
  [[nodiscard]] constexpr Subrange prev(std::iter_difference_t<I> n = 1) const requires std::bidirectional_iterator<I> {
    auto tmp = *this;
    tmp.advance(-n);
    return tmp;
  }
  constexpr Subrange& advance(std::iter_difference_t<I> n) {
    if constexpr(StoreSize) {
      auto d = n - std::ranges::advance(itr, n, sent);
      if(d >= 0) sz -= static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
      else sz += static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
      return *this;
    } else {
      std::ranges::advance(itr, n, sent);
      return *this;
    }
  }
};
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S) -> Subrange<I, S>;
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S, std::make_unsigned_t<std::iter_difference_t<I>>) -> Subrange<I, S, RangeKind::Sized>;
template<std::ranges::borrowed_range R> Subrange(R&&) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, (std::ranges::sized_range<R> || std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::iterator_t<R>>) ? RangeKind::Sized : RangeKind::Unsized>;
template<std::ranges::borrowed_range R> Subrange(R&&, std::make_unsigned_t<std::ranges::range_difference_t<R>>) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, RangeKind::Sized>;
} // namespace gsh
namespace std::ranges { template<class I, class S, gsh::RangeKind K> constexpr bool enable_borrowed_range<gsh::Subrange<I, S, K>> = true; }

namespace gsh {
template<class T, class Alloc = std::allocator<T>> requires std::is_same_v<T, typename std::allocator_traits<Alloc>::value_type> && (!std::is_const_v<T>)class NoInitArr : public ViewInterface<NoInitArr<T, Alloc>, T> {
public:
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using allocator_type = Alloc;
private:
  using traits = std::allocator_traits<Alloc>;
  [[no_unique_address]] Alloc alloc;
  T* p;
  u32 s;
public:
  constexpr NoInitArr(u32 n, const Alloc& a = Alloc()) : alloc(a), s(n) { p = traits::allocate(alloc, n); }
  constexpr NoInitArr(const NoInitArr& x) = delete;
  constexpr NoInitArr(const NoInitArr&& x) noexcept : alloc(std::move(x.alloc)), p(x.p), s(x.s) {}
  constexpr ~NoInitArr() {
    if(!p) {
      if constexpr(!std::is_trivially_destructible_v<T>) {
        for(u32 i = 0; i != s; ++i) traits::destroy(alloc, p + i);
      }
      traits::deallocate(alloc, p, s);
    }
  }
  template<class... Args> constexpr void construct(T* ptr, Args&&... args) { traits::construct(alloc, ptr, std::forward<Args>(args)...); }
  constexpr auto release() {
    auto res = p;
    p = nullptr;
    s = 0;
    return res;
  }
  constexpr bool released() { return p == nullptr; }
  constexpr auto begin() noexcept { return p; }
  constexpr auto begin() const noexcept { return p; }
  constexpr auto end() noexcept { return p + s; }
  constexpr auto end() const noexcept { return p + s; }
  constexpr auto data() noexcept { return p; }
  constexpr auto data() const noexcept { return p; }
  constexpr allocator_type get_allocator() const noexcept { return alloc; }
  constexpr u32 size() const noexcept { return s; }
  constexpr bool empty() const noexcept { return s == 0; }
  constexpr auto operator=(const NoInitArr&) = delete;
  constexpr auto operator=(NoInitArr&& x) requires (traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
    if constexpr(traits::propagate_on_container_move_assignment::value) alloc = std::move(x.alloc);
    p = x.p, s = x.s;
    return *this;
  }
};
template<class T, u32 N> class StaticArr : public ViewInterface<StaticArr<T, N>, T> {
  union {
    T elems[(N == 0 ? 1 : N)];
  };
public:
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  constexpr StaticArr() noexcept(noexcept(T{})) : elems{} {}
  constexpr explicit StaticArr(const T& value) {
    for(u32 i = 0; i != N; ++i) std::construct_at(elems + i, value);
  }
  template<std::input_iterator InputIter> constexpr explicit StaticArr(InputIter first) {
    for(u32 i = 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
  }
  template<std::input_iterator InputIter> constexpr StaticArr(InputIter first, InputIter last) {
    const u32 n = std::distance(first, last);
    if(n != N) throw gsh::Exception("gsh::StaticArr::StaticArr / The size of the given " "range differs from the size of the array.");
    for(u32 i = 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
  }
  constexpr StaticArr(const T (&a)[N]) {
    for(u32 i = 0; i != N; ++i) std::construct_at(elems + i, a[i]);
  }
  constexpr StaticArr(T (&&a)[N]) {
    for(u32 i = 0; i != N; ++i) std::construct_at(elems + i, std::move(a[i]));
  }
  constexpr StaticArr(const StaticArr& x) {
    for(u32 i = 0; i != N; ++i) std::construct_at(elems + i, x.elems[i]);
  }
  constexpr StaticArr(StaticArr&& y) {
    for(u32 i = 0; i != N; ++i) std::construct_at(elems + i, std::move(y.elems[i]));
  }
  constexpr StaticArr(std::initializer_list<T> il) : StaticArr(il.begin(), il.end()) {}
  constexpr ~StaticArr() noexcept {
    if constexpr(!std::is_trivially_destructible_v<T>)
      for(u32 i = 0; i != N; ++i) std::destroy_at(elems + i);
  }
  constexpr StaticArr& operator=(const StaticArr& x) {
    for(u32 i = 0; i != N; ++i) elems[i] = x.elems[i];
    return *this;
  }
  constexpr StaticArr& operator=(StaticArr&& x) noexcept {
    for(u32 i = 0; i != N; ++i) elems[i] = std::move(x.elems[i]);
    return *this;
  }
  constexpr StaticArr& operator=(std::initializer_list<T> init) {
    assign(init.begin(), init.end());
    return *this;
  }
  constexpr iterator begin() noexcept { return elems; }
  constexpr const_iterator begin() const noexcept { return elems; }
  constexpr iterator end() noexcept { return elems + N; }
  constexpr const_iterator end() const noexcept { return elems + N; }
  constexpr static u32 size() noexcept { return N; }
  constexpr static u32 max_size() noexcept { return N; }
  [[nodiscard]] constexpr static bool empty() noexcept { return N != 0; }
  constexpr T* data() noexcept { return elems; }
  constexpr const T* data() const noexcept { return elems; }
  template<std::input_iterator InputIter> constexpr void assign(InputIter first) {
    for(u32 i = 0; i != N; ++first, ++i) elems[i] = *first;
  }
  template<std::input_iterator InputIter> constexpr void assign(InputIter first, const InputIter last) {
    const u32 n = std::distance(first, last);
    if(n != N) throw gsh::Exception("gsh::StaticArr::assign / The size of the given " "range differs from the size of the array.");
    for(u32 i = 0; i != N; ++first, ++i) elems[i] = *first;
  }
  constexpr void assign(const T& value) {
    for(u32 i = 0; i != N; ++i) elems[i] = value;
  }
  constexpr void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
  constexpr void swap(StaticArr& x) {
    using std::swap;
    for(u32 i = 0; i != N; ++i) swap(elems[i], x.elems[i]);
  }
  friend constexpr void swap(StaticArr& x, StaticArr& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
} // namespace gsh
namespace std {
template<class T, gsh::u32 N> struct tuple_size<gsh::StaticArr<T, N>> : integral_constant<size_t, N> {};
template<std::size_t M, class T, gsh::u32 N> struct tuple_element<M, gsh::StaticArr<T, N>> {
  static_assert(M < N, "std::tuple_element<gsh::StaticArr<T, N>> / The index is out of range.");
  using type = T;
};
} // namespace std
namespace gsh {
template<std::size_t M, class T, u32 N> const T& get(const StaticArr<T, N>& a) {
  static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
  return a[M];
}
template<std::size_t M, class T, u32 N> T& get(StaticArr<T, N>& a) {
  static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
  return a[M];
}
template<std::size_t M, class T, u32 N> T&& get(StaticArr<T, N>&& a) {
  static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
  return std::move(a[M]);
}
} // namespace gsh

#include <charconv>
#include <string>
#include <string_view>
namespace gsh {
using Str = std::basic_string<c8>;
using Str8 = std::basic_string<utf8>;
using Str16 = std::basic_string<utf16>;
using Str32 = std::basic_string<utf32>;
using Strw = std::basic_string<wc>;
using StrView = std::basic_string_view<c8>;
using StrView8 = std::basic_string_view<utf8>;
using StrView16 = std::basic_string_view<utf16>;
using StrView32 = std::basic_string_view<utf32>;
using StrVieww = std::basic_string_view<wc>;
template<class T> class Parser;
template<> class Parser<Str> {
public:
  template<class Stream> constexpr Str operator()(Stream&& stream) const {
    stream.reload(16);
    Str res;
    while(true) {
      const c8* e = stream.current();
      while(*e >= '!') ++e;
      const u32 len = e - stream.current();
      const u32 curlen = res.size();
      res.resize(curlen + len);
      MemoryCopy(res.data() + curlen, stream.current(), len);
      stream.skip(len);
      if(stream.avail() == 0) stream.reload();
      else break;
    }
    stream.skip(1);
    return res;
  }
  template<class Stream> constexpr Str operator()(Stream&& stream, u32 n) const {
    u32 rem = n;
    Str res;
    u32 avail = stream.avail();
    while(avail <= rem) {
      const u32 curlen = res.size();
      res.resize(curlen + avail);
      MemoryCopy(res.data() + curlen, stream.current(), avail);
      rem -= avail;
      stream.skip(avail);
      if(rem == 0) return res;
      stream.reload();
      avail = stream.avail();
    }
    const u32 curlen = res.size();
    res.resize(curlen + rem);
    MemoryCopy(res.data() + curlen, stream.current(), rem);
    stream.skip(rem + 1);
    return res;
  }
};
template<class T> class Formatter;
template<> class Formatter<StrView> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, const StrView& str) const {
    const c8* s = str.data();
    u32 len = str.size();
    u32 avail = stream.avail();
    if(avail >= len) [[likely]] {
      MemoryCopy(stream.current(), s, len);
      stream.skip(len);
    } else {
      MemoryCopy(stream.current(), s, avail);
      len -= avail;
      s += avail;
      stream.skip(avail);
      while(len != 0) {
        stream.reload();
        avail = stream.avail();
        const u32 tmp = len < avail ? len : avail;
        MemoryCopy(stream.current(), s, tmp);
        len -= tmp;
        s += tmp;
        stream.skip(tmp);
      }
    }
  }
};
template<> class Formatter<Str> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, const Str& str) const { Formatter<StrView>{}(stream, StrView(str.data(), str.size())); }
};
template<class... Args> requires ((std::same_as<Args, c8> && ...) && sizeof...(Args) >= 1) constexpr Str UniteChars(Args... c) { return Str{c...}; }
template<std::integral T> constexpr Str NumToStr(const T& val, const i32 base = 10) {
  char buf[sizeof(T) * 8];
  char* last = std::to_chars(buf, buf + sizeof(T) * 8, val, base).ptr;
  return Str(buf, last);
}
template<std::floating_point T> constexpr Str NumToStr(const T& val, const i32 precision = 10) {
  char buf[64];
  auto result = std::to_chars(buf, buf + 64, val, std::chars_format::fixed, precision);
  if(result.ec != std::errc()) { throw Exception("gsh::ToStr / Conversion error in floating point to string."); }
  return Str(buf, result.ptr);
}
template<class T> constexpr T StrToNum(const StrView& str, const i32 base = 10) {
  T res{};
  auto result = std::from_chars(str.data(), str.data() + str.size(), res, base);
  if(result.ec != std::errc() || result.ptr != str.data() + str.size()) { throw Exception("gsh::FromStr / Conversion error in string to number."); }
  return res;
}
template<class T> constexpr T StrToNum(const Str& str, const i32 base = 10) { return StrToNum<T>(StrView(str), base); }
} // namespace gsh

namespace gsh {
struct NoInit_ {};
constexpr NoInit_ NoInit;
template<class T, class Alloc = std::allocator<T>> requires std::is_same_v<T, typename std::allocator_traits<Alloc>::value_type> && (!std::is_const_v<T>)class Vec : public ViewInterface<Vec<T, Alloc>, T> {
  using traits = std::allocator_traits<Alloc>;
public:
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using allocator_type = Alloc;
private:
  [[no_unique_address]] allocator_type alloc;
  T* ptr = nullptr;
  u32 len = 0, cap = 0;
public:
  constexpr Vec() noexcept(noexcept(Alloc())) {}
  constexpr explicit Vec(const allocator_type& a) noexcept : alloc(a) {}
  constexpr explicit Vec(u32 n, const Alloc& a = Alloc()) : alloc(a) {
    if(n == 0) [[unlikely]]
      return;
    ptr = traits::allocate(alloc, n);
    len = n, cap = n;
    for(u32 i = 0; i != n; ++i) traits::construct(alloc, ptr + i);
  }
  constexpr explicit Vec(NoInit_, u32 n, const Alloc& a = Alloc()) : alloc(a) {
    if(n == 0) [[unlikely]]
      return;
    ptr = traits::allocate(alloc, n);
    len = n, cap = n;
  }
  constexpr explicit Vec(const u32 n, const T& value, const allocator_type& a = Alloc()) : alloc(a) {
    if(n == 0) [[unlikely]]
      return;
    ptr = traits::allocate(alloc, n);
    len = n, cap = n;
    for(u32 i = 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
  }
  template<std::forward_iterator Iter, std::sentinel_for<Iter> Sent> constexpr Vec(Iter first, Sent last, const allocator_type& a = Alloc()) : alloc(a) {
    const u32 n = std::ranges::distance(first, last);
    if(n == 0) [[unlikely]]
      return;
    ptr = traits::allocate(alloc, n);
    len = n, cap = n;
    u32 i = 0;
    for(; i != n; ++first, ++i) traits::construct(alloc, ptr + i, *first);
  }
  constexpr Vec(const Vec& x) : Vec(x, traits::select_on_container_copy_construction(x.alloc)) {}
  constexpr Vec(Vec&& x) noexcept : alloc(std::move(x.alloc)), ptr(x.ptr), len(x.len), cap(x.cap) { x.ptr = nullptr, x.len = 0, x.cap = 0; }
  constexpr Vec(const Vec& x, const allocator_type& a) : alloc(a), len(x.len), cap(x.len) {
    if(len == 0) [[unlikely]]
      return;
    ptr = traits::allocate(alloc, cap);
    for(u32 i = 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
  }
  constexpr Vec(Vec&& x, const allocator_type& a) : alloc(a) {
    if(traits::is_always_equal || x.get_allocator() == a) {
      ptr = x.ptr, len = x.len, cap = x.cap;
      x.ptr = nullptr, x.len = 0, x.cap = 0;
    } else {
      if(x.len == 0) [[unlikely]]
        return;
      len = x.len, cap = x.cap;
      ptr = traits::allocate(alloc, len);
      for(u32 i = 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
      traits::deallocate(x.alloc, x.ptr, x.cap);
      x.ptr = nullptr, x.len = 0, x.cap = 0;
    }
  }
  constexpr Vec(std::initializer_list<T> il, const allocator_type& a = Alloc()) : Vec(il.begin(), il.end(), a) {}
  constexpr ~Vec() {
    if(cap != 0) {
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      traits::deallocate(alloc, ptr, cap);
    }
  }
  constexpr Vec& operator=(const Vec& x) {
    if(&x == this) return *this;
    for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
    if(traits::propagate_on_container_copy_assignment::value || cap < x.len) {
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      if constexpr(traits::propagate_on_container_copy_assignment::value) alloc = x.alloc;
      cap = x.len;
      ptr = traits::allocate(alloc, cap);
    }
    len = x.len;
    for(u32 i = 0; i != len; ++i) *(ptr + i) = *(x.ptr + i);
    return *this;
  }
  constexpr Vec& operator=(Vec&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
    if(&x == this) return *this;
    if(cap != 0) {
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      traits::deallocate(alloc, ptr, cap);
    }
    if constexpr(traits::propagate_on_container_move_assignment::value) alloc = std::move(x.alloc);
    ptr = x.ptr, len = x.len, cap = x.cap;
    x.ptr = nullptr, x.len = 0, x.cap = 0;
    return *this;
  }
  constexpr Vec& operator=(std::initializer_list<T> init) {
    assign(init.begin(), init.end());
    return *this;
  }
  constexpr iterator begin() noexcept { return ptr; }
  constexpr const_iterator begin() const noexcept { return ptr; }
  constexpr iterator end() noexcept { return ptr + len; }
  constexpr const_iterator end() const noexcept { return ptr + len; }
  constexpr u32 size() const noexcept { return len; }
  constexpr u32 max_size() const noexcept {
    const auto tmp = traits::max_size(alloc);
    return tmp < 2147483647 ? tmp : 2147483647;
  }
  constexpr void resize(const u32 sz) {
    if(cap < sz) {
      T* new_ptr = traits::allocate(alloc, sz);
      if(cap != 0) {
        for(u32 i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
        for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        traits::deallocate(alloc, ptr, cap);
      }
      ptr = new_ptr;
      for(u32 i = len; i != sz; ++i) traits::construct(alloc, ptr + i);
      len = sz, cap = sz;
    } else if(len < sz) {
      for(u32 i = len; i != sz; ++i) traits::construct(alloc, ptr + i);
      len = sz;
    } else {
      for(u32 i = sz; i != len; ++i) traits::destroy(alloc, ptr + i);
      len = sz;
    }
  }
  constexpr void resize(const u32 sz, const T& c) {
    if(cap < sz) {
      T* new_ptr = traits::allocate(alloc, sz);
      if(cap != 0) {
        for(u32 i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
        for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        traits::deallocate(alloc, ptr, cap);
      }
      ptr = new_ptr;
      for(u32 i = len; i != sz; ++i) traits::construct(alloc, ptr + i, c);
      len = sz, cap = sz;
    } else if(len < sz) {
      for(u32 i = len; i != sz; ++i) traits::construct(alloc, ptr + i, c);
      len = sz;
    } else {
      for(u32 i = sz; i != len; ++i) traits::destroy(alloc, ptr + i);
      len = sz;
    }
  }
  constexpr u32 capacity() const noexcept { return cap; }
  [[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
  constexpr void reserve(const u32 n) {
    if(n > cap) {
      T* new_ptr = traits::allocate(alloc, n);
      if(cap != 0) {
        for(u32 i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
        for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        traits::deallocate(alloc, ptr, cap);
      }
      ptr = new_ptr, cap = n;
    }
  }
  constexpr void shrink_to_fit() {
    if(len == 0) {
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = nullptr, cap = 0;
      return;
    }
    if(len != cap) {
      T* new_ptr = traits::allocate(alloc, len);
      for(u32 i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = len;
    }
  }
  constexpr T* data() noexcept { return ptr; }
  constexpr const T* data() const noexcept { return ptr; }
  template<std::forward_iterator Iter, std::sentinel_for<Iter> Sent> constexpr void assign(Iter first, Sent last) {
    const u32 n = std::ranges::distance(first, last);
    if(n > cap) {
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      traits::deallocate(alloc, ptr, cap);
      ptr = traits::allocate(alloc, n);
      cap = n;
      for(u32 i = 0; i != n; ++first, ++i) traits::construct(alloc, ptr + i, *first);
    } else if(n > len) {
      u32 i = 0;
      for(; i != len; ++first, ++i) *(ptr + i) = *first;
      for(; i != n; ++first, ++i) traits::construct(alloc, ptr + i, *first);
    } else {
      for(u32 i = n; i != len; ++i) traits::destroy(alloc, ptr + i);
      for(u32 i = 0; i != n; ++first, ++i) *(ptr + i) = *first;
    }
    len = n;
  }
  constexpr void assign(const u32 n, const T& t) {
    if(n > cap) {
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      traits::deallocate(alloc, ptr, cap);
      ptr = traits::allocate(alloc, n);
      cap = n;
      for(u32 i = 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
    } else if(n > len) {
      u32 i = 0;
      for(; i != len; ++i) *(ptr + i) = t;
      for(; i != n; ++i) traits::construct(alloc, ptr + i, t);
    } else {
      for(u32 i = n; i != len; ++i) traits::destroy(alloc, ptr + i);
      for(u32 i = 0; i != n; ++i) *(ptr + i) = t;
    }
    len = n;
  }
  constexpr void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
#ifdef NDEBUG
  GSH_INTERNAL_INLINE
#endif
  constexpr T& operator[](u32 n) {
#ifndef NDEBUG
    if(n >= len) [[unlikely]]
      throw Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
    return ptr[n];
  };
#ifdef NDEBUG
  GSH_INTERNAL_INLINE
#endif
  constexpr const T& operator[](u32 n) const {
#ifndef NDEBUG
    if(n >= len) [[unlikely]]
      throw Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
    return ptr[n];
  }
private:
  constexpr void extend_one() {
    if(len == cap) {
      T* new_ptr = traits::allocate(alloc, cap * 2 + 8);
      if(cap != 0) {
        for(u32 i = 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
        for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
        traits::deallocate(alloc, ptr, cap);
      }
      ptr = new_ptr, cap = cap * 2 + 8;
    }
  }
public:
  constexpr void push_back(const T& x) {
    extend_one();
    traits::construct(alloc, ptr + (len++), x);
  }
  constexpr void push_back(T&& x) {
    extend_one();
    traits::construct(alloc, ptr + (len++), std::move(x));
  }
  template<class... Args> constexpr T& emplace_back(Args&&... args) {
    extend_one();
    traits::construct(alloc, ptr + len, std::forward<Args>(args)...);
    return *(ptr + (len++));
  }
  constexpr void pop_back() {
#ifndef NDEBUG
    if(len == 0) [[unlikely]]
      throw gsh::Exception("gsh::Vec::pop_back / The container is empty.");
#endif
    traits::destroy(alloc, ptr + (--len));
  }
  constexpr iterator insert(const const_iterator position, const T& x) {
#ifndef NDEBUG
    if(len == 0) {
      if(position != ptr) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    } else {
      if(position < ptr || position > ptr + len) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    }
#endif
    const u32 idx = (len == 0 ? 0u : static_cast<u32>(position - ptr));
    if(len + 1 > cap) {
      u32 new_cap = cap;
      while(new_cap < len + 1) new_cap = new_cap * 2 + 8;
      T* new_ptr = traits::allocate(alloc, new_cap);
      for(u32 i = 0; i != idx; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
      traits::construct(alloc, new_ptr + idx, x);
      for(u32 i = idx; i != len; ++i) traits::construct(alloc, new_ptr + (i + 1), std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = new_cap;
      ++len;
      return ptr + idx;
    }
    if(idx == len) {
      traits::construct(alloc, ptr + len, x);
      ++len;
      return ptr + idx;
    }
    for(u32 i = len; i > idx; --i) {
      const u32 src = i - 1;
      const u32 dst = src + 1;
      if(dst >= len) {
        traits::construct(alloc, ptr + dst, std::move_if_noexcept(*(ptr + src)));
      } else {
        *(ptr + dst) = std::move_if_noexcept(*(ptr + src));
      }
    }
    traits::destroy(alloc, ptr + idx);
    traits::construct(alloc, ptr + idx, x);
    ++len;
    return ptr + idx;
  }
  constexpr iterator insert(const const_iterator position, T&& x) {
#ifndef NDEBUG
    if(len == 0) {
      if(position != ptr) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    } else {
      if(position < ptr || position > ptr + len) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    }
#endif
    const u32 idx = (len == 0 ? 0u : static_cast<u32>(position - ptr));
    if(len + 1 > cap) {
      u32 new_cap = cap;
      while(new_cap < len + 1) new_cap = new_cap * 2 + 8;
      T* new_ptr = traits::allocate(alloc, new_cap);
      for(u32 i = 0; i != idx; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
      traits::construct(alloc, new_ptr + idx, std::move(x));
      for(u32 i = idx; i != len; ++i) traits::construct(alloc, new_ptr + (i + 1), std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = new_cap;
      ++len;
      return ptr + idx;
    }
    if(idx == len) {
      traits::construct(alloc, ptr + len, std::move(x));
      ++len;
      return ptr + idx;
    }
    for(u32 i = len; i > idx; --i) {
      const u32 src = i - 1;
      const u32 dst = src + 1;
      if(dst >= len) {
        traits::construct(alloc, ptr + dst, std::move_if_noexcept(*(ptr + src)));
      } else {
        *(ptr + dst) = std::move_if_noexcept(*(ptr + src));
      }
    }
    traits::destroy(alloc, ptr + idx);
    traits::construct(alloc, ptr + idx, std::move(x));
    ++len;
    return ptr + idx;
  }
  constexpr iterator insert(const const_iterator position, const u32 n, const T& x) {
#ifndef NDEBUG
    if(len == 0) {
      if(position != ptr) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    } else {
      if(position < ptr || position > ptr + len) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    }
#endif
    if(n == 0) return (len == 0 ? ptr : const_cast<iterator>(position));
    const u32 idx = (len == 0 ? 0u : static_cast<u32>(position - ptr));
    if(len + n > cap) {
      u32 new_cap = cap;
      while(new_cap < len + n) new_cap = new_cap * 2 + 8;
      T* new_ptr = traits::allocate(alloc, new_cap);
      for(u32 i = 0; i != idx; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != n; ++i) traits::construct(alloc, new_ptr + (idx + i), x);
      for(u32 i = idx; i != len; ++i) traits::construct(alloc, new_ptr + (i + n), std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = new_cap;
      len += n;
      return ptr + idx;
    }
    if(idx == len) {
      for(u32 i = 0; i != n; ++i) traits::construct(alloc, ptr + (len + i), x);
      len += n;
      return ptr + idx;
    }
    for(u32 i = len; i > idx; --i) {
      const u32 src = i - 1;
      const u32 dst = src + n;
      if(dst >= len) {
        traits::construct(alloc, ptr + dst, std::move_if_noexcept(*(ptr + src)));
      } else {
        *(ptr + dst) = std::move_if_noexcept(*(ptr + src));
      }
    }
    for(u32 i = 0; i != n; ++i) {
      traits::destroy(alloc, ptr + (idx + i));
      traits::construct(alloc, ptr + (idx + i), x);
    }
    len += n;
    return ptr + idx;
  }
  template<class InputIter> constexpr iterator insert(const const_iterator position, const InputIter first, const InputIter last) {
#ifndef NDEBUG
    if(len == 0) {
      if(position != ptr) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    } else {
      if(position < ptr || position > ptr + len) [[unlikely]]
        throw Exception("gsh::Vec::insert / The iterator is out of range.");
    }
#endif
    Vec tmp(alloc);
    for(auto it = first; it != last; ++it) tmp.push_back(*it);
    const u32 n = tmp.size();
    if(n == 0) return (len == 0 ? ptr : const_cast<iterator>(position));
    const u32 idx = (len == 0 ? 0u : static_cast<u32>(position - ptr));
    if(len + n > cap) {
      u32 new_cap = cap;
      while(new_cap < len + n) new_cap = new_cap * 2 + 8;
      T* new_ptr = traits::allocate(alloc, new_cap);
      for(u32 i = 0; i != idx; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != n; ++i) traits::construct(alloc, new_ptr + (idx + i), tmp[i]);
      for(u32 i = idx; i != len; ++i) traits::construct(alloc, new_ptr + (i + n), std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = new_cap;
      len += n;
      return ptr + idx;
    }
    if(idx == len) {
      for(u32 i = 0; i != n; ++i) traits::construct(alloc, ptr + (len + i), tmp[i]);
      len += n;
      return ptr + idx;
    }
    for(u32 i = len; i > idx; --i) {
      const u32 src = i - 1;
      const u32 dst = src + n;
      if(dst >= len) {
        traits::construct(alloc, ptr + dst, std::move_if_noexcept(*(ptr + src)));
      } else {
        *(ptr + dst) = std::move_if_noexcept(*(ptr + src));
      }
    }
    for(u32 i = 0; i != n; ++i) {
      traits::destroy(alloc, ptr + (idx + i));
      traits::construct(alloc, ptr + (idx + i), tmp[i]);
    }
    len += n;
    return ptr + idx;
  }
  constexpr iterator insert(const const_iterator position, const std::initializer_list<T> il) { return insert(position, il.begin(), il.end()); }
  template<class... Args> constexpr iterator emplace(const_iterator position, Args&&... args) {
#ifndef NDEBUG
    if(len == 0) {
      if(position != ptr) [[unlikely]]
        throw Exception("gsh::Vec::emplace / The iterator is out of range.");
    } else {
      if(position < ptr || position > ptr + len) [[unlikely]]
        throw Exception("gsh::Vec::emplace / The iterator is out of range.");
    }
#endif
    const u32 idx = (len == 0 ? 0u : static_cast<u32>(position - ptr));
    if(len + 1 > cap) {
      u32 new_cap = cap;
      while(new_cap < len + 1) new_cap = new_cap * 2 + 8;
      T* new_ptr = traits::allocate(alloc, new_cap);
      for(u32 i = 0; i != idx; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
      traits::construct(alloc, new_ptr + idx, std::forward<Args>(args)...);
      for(u32 i = idx; i != len; ++i) traits::construct(alloc, new_ptr + (i + 1), std::move_if_noexcept(*(ptr + i)));
      for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
      if(cap != 0) traits::deallocate(alloc, ptr, cap);
      ptr = new_ptr, cap = new_cap;
      ++len;
      return ptr + idx;
    }
    if(idx == len) {
      traits::construct(alloc, ptr + len, std::forward<Args>(args)...);
      ++len;
      return ptr + idx;
    }
    for(u32 i = len; i > idx; --i) {
      const u32 src = i - 1;
      const u32 dst = src + 1;
      if(dst >= len) {
        traits::construct(alloc, ptr + dst, std::move_if_noexcept(*(ptr + src)));
      } else {
        *(ptr + dst) = std::move_if_noexcept(*(ptr + src));
      }
    }
    traits::destroy(alloc, ptr + idx);
    traits::construct(alloc, ptr + idx, std::forward<Args>(args)...);
    ++len;
    return ptr + idx;
  }
  constexpr iterator erase(const_iterator position) {
#ifndef NDEBUG
    if(len == 0) [[unlikely]]
      throw Exception("gsh::Vec::erase / The container is empty.");
    if(position < ptr || position >= ptr + len) [[unlikely]]
      throw Exception("gsh::Vec::erase / The iterator is out of range.");
#endif
    const u32 idx = static_cast<u32>(position - ptr);
    for(u32 i = idx + 1; i != len; ++i) *(ptr + (i - 1)) = std::move_if_noexcept(*(ptr + i));
    traits::destroy(alloc, ptr + (len - 1));
    --len;
    return ptr + idx;
  }
  constexpr iterator erase(const_iterator first, const_iterator last) {
#ifndef NDEBUG
    if(first == last) return const_cast<iterator>(first);
    if(len == 0) [[unlikely]]
      throw Exception("gsh::Vec::erase / The container is empty.");
    if(first < ptr || first > ptr + len || last < ptr || last > ptr + len || last < first) [[unlikely]]
      throw Exception("gsh::Vec::erase / The iterator range is out of range.");
#endif
    if(first == last) return const_cast<iterator>(first);
    const u32 idx_f = static_cast<u32>(first - ptr);
    const u32 idx_l = static_cast<u32>(last - ptr);
    const u32 n = idx_l - idx_f;
    for(u32 i = idx_l; i != len; ++i) *(ptr + (i - n)) = std::move_if_noexcept(*(ptr + i));
    for(u32 i = len - n; i != len; ++i) traits::destroy(alloc, ptr + i);
    len -= n;
    return ptr + idx_f;
  }
  constexpr void swap(Vec& x) noexcept(traits::propagate_on_container_swap::value || traits::is_always_equal::value) {
    using std::swap;
    swap(ptr, x.ptr);
    swap(len, x.len);
    swap(cap, x.cap);
    if constexpr(traits::propagate_on_container_swap::value) swap(alloc, x.alloc);
  }
  constexpr void clear() {
    for(u32 i = 0; i != len; ++i) traits::destroy(alloc, ptr + i);
    len = 0;
  }
  constexpr void reset() {
    if(cap != 0) {
      traits::deallocate(alloc, ptr, cap);
      ptr = nullptr, len = 0, cap = 0;
    }
  }
  constexpr void abandon() noexcept { ptr = nullptr, len = 0, cap = 0; }
  constexpr allocator_type get_allocator() const noexcept { return alloc; }
  friend constexpr void swap(Vec& x, Vec& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<std::input_iterator InputIter, class Alloc = std::allocator<typename std::iterator_traits<InputIter>::value_type>> Vec(InputIter, InputIter, Alloc = Alloc()) -> Vec<typename std::iterator_traits<InputIter>::value_type, Alloc>;
template<class T, class Alloc = std::allocator<T>> using Vec2 = Vec<Vec<T, Alloc>, typename std::allocator_traits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>;
template<class T, class Alloc = std::allocator<T>> using Vec3 = Vec<Vec<Vec<T, Alloc>, typename std::allocator_traits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>, typename std::allocator_traits<Alloc>::template rebind_alloc<Vec<Vec<T, Alloc>, typename std::allocator_traits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>>>;
} // namespace gsh

#include <cmath>
namespace gsh {
template<class T, class U> constexpr std::common_type_t<T, U> Min(const T& a, const U& b) { return a < b ? a : b; }
template<class T, class... Args> requires (sizeof...(Args) >= 2) constexpr auto Min(const T& x, const Args&... args) { return Min(x, Min(args...)); }
template<class T, class U> constexpr std::common_type_t<T, U> Max(const T& a, const U& b) { return a < b ? b : a; }
template<class T, class... Args> requires (sizeof...(Args) >= 2) constexpr auto Max(const T& x, const Args&... args) { return Max(x, Max(args...)); }
template<class T, class U> constexpr bool Chmin(T& a, const U& b) {
  const bool f = b < a;
  a = f ? b : a;
  return f;
}
template<class T, class... Args> requires (sizeof...(Args) >= 2) constexpr bool Chmin(T& a, const Args&... b) { return Chmin(a, Min(b...)); }
template<class T, class U> constexpr bool Chmax(T& a, const U& b) {
  const bool f = a < b;
  a = f ? b : a;
  return f;
}
template<class T, class... Args> requires (sizeof...(Args) >= 2) constexpr bool Chmax(T& a, const Args&... b) { return Chmax(a, Max(b...)); }
namespace internal {
template<class R, class Comp, class Proj> constexpr auto MinImpl(R&& r, Comp&& comp, Proj&& proj) {
  auto first = std::ranges::begin(r);
  auto last = std::ranges::end(r);
  if(first == last) {
    if constexpr(std::same_as<Proj, Identity>) {
      if constexpr(std::same_as<Comp, Less> && requires {
                     { std::numeric_limits<std::ranges::range_value_t<R>>::max() } -> std::same_as<std::ranges::template range_value_t<R>>;
                   }) {
        return std::numeric_limits<std::ranges::range_value_t<R>>::max();
      }
      if constexpr(std::same_as<Comp, Greater> && requires {
                     { std::numeric_limits<std::ranges::range_value_t<R>>::lowest() } -> std::same_as<std::ranges::template range_value_t<R>>;
                   }) {
        return std::numeric_limits<std::ranges::range_value_t<R>>::lowest();
      }
    }
    throw Exception("gsh::internal::MinImpl / The input is empty.");
  }
  auto res = *(first++);
  for(; first != last; ++first) {
    const bool f = std::invoke(comp, std::invoke(proj, static_cast<const decltype(res)&>(*first)), std::invoke(proj, res));
    if constexpr(std::is_trivial_v<decltype(res)>) res = f ? *first : res;
    else if(f) res = *first;
  }
  return res;
}
template<class R, class Comp, class Proj> constexpr auto MaxImpl(R&& r, Comp&& comp, Proj&& proj) {
  if constexpr(std::same_as<std::remove_cvref_t<Comp>, Less>) return MinImpl(std::forward<R>(r), Greater(), proj);
  else if constexpr(std::same_as<std::remove_cvref_t<Comp>, Greater>) return MinImpl(std::forward<R>(r), Less(), proj);
  else return MinImpl(std::forward<R>(r), SwapArgs(std::forward<Comp>(comp)), proj);
}
template<class R, class T, class F> constexpr auto FoldImpl(R&& r, T init, F&& f) {
  for(auto&& x : std::forward<R>(r)) init = std::invoke(f, std::move(init), std::forward<decltype(x)>(x));
  return init;
}
template<class R, class F> constexpr auto SumImpl(R&& r, F&& f) {
  auto itr = std::ranges::begin(r);
  auto sent = std::ranges::end(r);
  if(!(itr != sent)) {
    if constexpr(std::is_arithmetic_v<std::ranges::range_value_t<R>> && std::is_constructible_v<std::ranges::range_value_t<R>, u32>) {
      return std::ranges::range_value_t<R>(static_cast<u32>(0));
    } else {
      throw Exception("gsh::internal::SumImpl / The input is empty.");
    }
  }
  auto res = *itr;
  for(++itr; itr != sent; ++itr) res = std::invoke(f, std::move(res), *itr);
  return res;
}
template<class R, class F> constexpr void AdjacentDifferenceImpl(R&& r, F&& f) {
  auto itr = std::ranges::begin(r);
  auto sent = std::ranges::end(r);
  if(!(itr != sent)) return;
  using value_type = std::ranges::range_value_t<R>;
  auto prev = std::move(*itr);
  while(true) {
    auto nx = std::ranges::next(itr);
    if(!(nx != sent)) break;
    value_type tmp = std::invoke(f, *nx, std::move(*itr));
    *itr = std::move(prev);
    prev = std::move(tmp);
    itr = nx;
  }
  *itr = prev;
}
template<class R> constexpr void ReverseImpl(R&& r) { std::ranges::reverse(std::forward<R>(r)); }
template<class T, class Proj = Identity> void SortUnsigned8(T* const p, const u32 n, Proj&& proj = {}) {
  std::unique_ptr<u32[]> cnt(new u32[1 << 8]{});
  for(u32 i = 0; i != n; ++i) ++cnt[std::invoke(proj, p[i]) & 0xff];
  for(u32 i = 0; i != (1 << 8) - 1; ++i) cnt[i + 1] += cnt[i];
  NoInitArr<T> tmp(n);
  for(u32 i = n; i--;) tmp.construct(&tmp[--cnt[std::invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
  for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
}
template<class T, class Proj = Identity> void SortUnsigned16(T* const p, const u32 n, Proj&& proj = {}) {
  std::unique_ptr<u32[]> cnt(new u32[1 << 16]{});
  for(u32 i = 0; i != n; ++i) ++cnt[std::invoke(proj, p[i]) & 0xffff];
  for(u32 i = 0; i != (1 << 16) - 1; ++i) cnt[i + 1] += cnt[i];
  NoInitArr<T> tmp(n);
  for(u32 i = n; i--;) tmp.construct(&tmp[--cnt[std::invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
  for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
}
template<class T, class Proj = Identity> void SortUnsigned32(T* const p, const u32 n, Proj&& proj = {}) {
  std::unique_ptr<u32[]> cnt(new u32[2 * (1 << 16)]{});
  u32 *const cnt1 = cnt.get(), *const cnt2 = cnt.get() + (1 << 16);
  for(u32 i = 0; i != n; ++i) {
    auto tmp = std::invoke(proj, p[i]);
    const u16 a = tmp & 0xffff;
    const u16 b = tmp >> 16 & 0xffff;
    ++cnt1[a];
    ++cnt2[b];
  }
  for(u32 i = 0; i != (1 << 16) - 1; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
  }
  NoInitArr<T> tmp(n);
  for(u32 i = n; i--;) tmp.construct(&tmp[--cnt1[std::invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
  if(cnt2[0] == n) {
    for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    return;
  }
  for(u32 i = n; i--;) p[--cnt2[std::invoke(proj, tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
}
template<class T, class Proj = Identity> void SortUnsigned64(T* const p, const u32 n, Proj&& proj = {}) {
  std::unique_ptr<u32[]> cnt(new u32[4 * (1 << 16)]{});
  u32 *const cnt1 = cnt.get(), *const cnt2 = cnt.get() + (1 << 16), *const cnt3 = cnt.get() + 2 * (1 << 16), *const cnt4 = cnt.get() + 3 * (1 << 16);
  for(u32 i = 0; i != n; ++i) {
    auto tmp = std::invoke(proj, p[i]);
    const u16 a = tmp & 0xffff;
    const u16 b = tmp >> 16 & 0xffff;
    const u16 c = tmp >> 32 & 0xffff;
    const u16 d = tmp >> 48 & 0xffff;
    ++cnt1[a];
    ++cnt2[b];
    ++cnt3[c];
    ++cnt4[d];
  }
  for(u32 i = 0; i != (1 << 16) - 1; ++i) {
    cnt1[i + 1] += cnt1[i];
    cnt2[i + 1] += cnt2[i];
    cnt3[i + 1] += cnt3[i];
    cnt4[i + 1] += cnt4[i];
  }
  NoInitArr<T> tmp(n);
  for(u32 i = n; i--;) tmp.construct(&tmp[--cnt1[std::invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
  if(cnt2[0] == n) {
    for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    return;
  }
  for(u32 i = n; i--;) p[--cnt2[std::invoke(proj, tmp[i]) >> 16 & 0xffff]] = std::move(tmp[i]);
  if(cnt3[0] == n) return;
  for(u32 i = n; i--;) tmp[--cnt3[std::invoke(proj, p[i]) >> 32 & 0xffff]] = std::move(p[i]);
  if(cnt4[0] == n) {
    for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    return;
  }
  for(u32 i = n; i--;) p[--cnt4[std::invoke(proj, tmp[i]) >> 48 & 0xffff]] = std::move(tmp[i]);
}
struct Revmsb {
  template<class T> constexpr auto operator()(T x) const {
    using result_type = std::make_unsigned_t<T>;
    return std::bit_cast<result_type>(x) ^ (result_type(1) << (sizeof(T) * 8 - 1));
  }
};
struct ToUnsigned {
  template<class T> constexpr auto operator()(T x) const {
    if constexpr(std::same_as<T, f16>) {
      u16 y = std::bit_cast<u16>(x);
      return u16(y ^ ((y >> 15) != 0 ? ~u16(0) : (u16(1) << 15)));
    } else if constexpr(std::same_as<T, f32>) {
      u32 y = std::bit_cast<u32>(x);
      return u32(y ^ ((y >> 31) != 0 ? ~u32(0) : (u32(1) << 31)));
    } else if constexpr(std::same_as<T, f64>) {
      u64 y = std::bit_cast<u64>(x);
      return u64(y ^ ((y >> 63) != 0 ? ~u64(0) : (u64(1) << 63)));
    } else if constexpr(std::same_as<T, f128>) {
      u128 y = std::bit_cast<u128>(x);
      return u128(y ^ ((y >> 127) != 0 ? ~u128(0) : (u128(1) << 127)));
    }
  }
};
constexpr u32 SortBlockIndex[60][2] = {
// clang-format off
    {0,13},{1,12},{2,15},{3,14},{4,8},{5,6},{7,11},{9,10},
    {0,5},{1,7},{2,9},{3,4},{6,13},{8,14},{10,15},{11,12},
    {0,1},{2,3},{4,5},{6,8},{7,9},{10,11},{12,13},{14,15},
    {0,2},{1,3},{4,10},{5,11},{6,7},{8,9},{12,14},{13,15},
    {1,2},{3,12},{4,6},{5,7},{8,10},{9,11},{13,14},
    {1,4},{2,6},{5,8},{7,10},{9,13},{11,14},
    {2,4},{3,6},{9,12},{11,13},
    {3,5},{6,8},{7,9},{10,12},
    {3,4},{5,6},{7,8},{9,10},{11,12},
    {6,7},{8,9}
// clang-format on
};
template<class T, class Comp = Less, class Proj = Identity> constexpr void SortBlock(T* const p, Comp&& comp = {}, Proj&& proj = {}) {
  if constexpr(std::is_scalar_v<T>) {
    GSH_INTERNAL_UNROLL(60)
    for(u32 i = 0; i != 60; ++i) {
      u32 a = SortBlockIndex[i][0], b = SortBlockIndex[i][1];
      bool f = std::invoke(comp, std::invoke(proj, p[a]), std::invoke(proj, p[b]));
      const auto tmp_a = p[a], tmp_b = p[b];
      p[a] = f ? tmp_a : tmp_b;
      p[b] = f ? tmp_b : tmp_a;
    }
  } else {
    for(u32 i = 0; i != 60; ++i) {
      u32 a = SortBlockIndex[i][0], b = SortBlockIndex[i][1];
      bool f = std::invoke(comp, std::invoke(proj, p[a]), std::invoke(proj, p[b]));
      const auto tmp_a = std::move(p[a]), tmp_b = std::move(p[b]);
      p[a] = f ? tmp_a : tmp_b;
      p[b] = f ? tmp_b : tmp_a;
    }
  }
}
template<class R, class Comp, class Proj> constexpr void SortImpl(R&& r, Comp&& comp, Proj&& proj) {
  if constexpr(!requires { std::ranges::data(r); }) {
    Vec tmp(std::move_iterator(std::ranges::begin(r)), std::move_sentinel(std::ranges::end(r)));
    SortImpl(tmp, std::forward<Comp>(comp), std::forward<Proj>(proj));
    for(u32 i = 0; auto&& el : r) el = std::move(tmp[i++]);
    return;
  } else {
    const u32 n = std::ranges::size(r);
    auto* const p = std::ranges::data(r);
    using value_type = std::remove_cvref_t<decltype(*p)>;
    if(!std::is_constant_evaluated()) {
      constexpr bool is_less = std::same_as<std::remove_cvref_t<Comp>, Less>;
      constexpr bool is_greater = std::same_as<std::remove_cvref_t<Comp>, Greater>;
      if constexpr((is_less || is_greater) && std::is_nothrow_move_constructible_v<value_type>) {
        using inv_result = std::remove_cvref_t<std::invoke_result_t<Proj, std::ranges::range_value_t<R>>>;
        if constexpr(std::same_as<inv_result, u8> || std::same_as<inv_result, i8> || std::same_as<inv_result, c8>) {
          if(n >= 33) {
            if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned8(p, n, std::forward<Proj>(proj));
            else internal::SortUnsigned8(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, u16> || std::same_as<inv_result, i16>) {
          if(n >= 1200) {
            if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned16(p, n, std::forward<Proj>(proj));
            else internal::SortUnsigned16(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, u32> || std::same_as<inv_result, i32>) {
          if(n >= 3200) {
            if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned32(p, n, std::forward<Proj>(proj));
            else internal::SortUnsigned32(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, u64> || std::same_as<inv_result, i64>) {
          if(n >= 25000) {
            if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned64(p, n, std::forward<Proj>(proj));
            else internal::SortUnsigned64(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, f16>) {
          if(n >= 1500) {
            internal::SortUnsigned16(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, f32>) {
          if(n >= 4000) {
            internal::SortUnsigned32(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
        if constexpr(std::same_as<inv_result, f64>) {
          if(n >= 30000) {
            internal::SortUnsigned64(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
            if constexpr(is_greater) ReverseImpl(std::forward<R>(r));
            return;
          }
        }
      }
    }
    constexpr u32 minrun = 16;
    u32 rem = n % minrun;
    for(u32 i = 1; i < rem; ++i) {
      auto tmp = std::move(p[i]);
      u32 j = i;
      for(; j != 0 && std::invoke(comp, std::invoke(proj, tmp), std::invoke(proj, p[j - 1])); --j) p[j] = std::move(p[j - 1]);
      p[j] = std::move(tmp);
    }
    for(u32 i = 0, blocks = n / minrun; i != blocks; ++i) { SortBlock(p + rem + i * minrun, comp, proj); }
    if(n <= 16) return;
    rem = rem == 0 ? 16 : rem;
    NoInitArr<value_type> tmp(n);
    auto merge_seq = [&](value_type* a, u32 a_size, value_type* b, u32 b_size, value_type* dst, auto construct) {
      constexpr bool construct_v = std::same_as<decltype(construct), std::true_type>;
      u32 i = 0, j = 0, k = 0;
      while(i != a_size && j != b_size) {
        bool f = std::invoke(comp, std::invoke(proj, a[i]), std::invoke(proj, b[j]));
        if constexpr(construct_v) tmp.construct(&dst[k], std::move(f ? a[i] : b[j]));
        else dst[k] = std::move(f ? a[i] : b[j]);
        i += f;
        j += !f;
        k += 1;
      }
      for(; i != a_size; ++i, ++k) {
        if constexpr(construct_v) tmp.construct(&dst[k], std::move(a[i]));
        else dst[k] = std::move(a[i]);
      }
      for(; j != b_size; ++j, ++k) {
        if constexpr(construct_v) tmp.construct(&dst[k], std::move(b[j]));
        else dst[k] = std::move(b[j]);
      }
    };
    merge_seq(p, rem, p + rem, minrun, tmp.data(), std::true_type());
    u32 i = rem + minrun;
    for(; i + 2 * minrun <= n; i += 2 * minrun) { merge_seq(p + i, minrun, p + i + minrun, minrun, tmp.data() + i, std::true_type()); }
    for(; i < n; ++i) tmp.construct(tmp.data() + i, std::move(p[i]));
    value_type *from = tmp.data(), *to = p;
    u32 run = 2 * minrun;
    while(true) {
      u32 nrem = run - minrun + rem;
      if(nrem + run >= n) {
        merge_seq(from, nrem, from + nrem, n - nrem, to, std::false_type());
        break;
      }
      merge_seq(from, nrem, from + nrem, run, to, std::false_type());
      u32 i = nrem + run;
      for(; i + 2 * run <= n; i += 2 * run) { merge_seq(from + i, run, from + i + run, run, to + i, std::false_type()); }
      if(i + run < n) merge_seq(from + i, run, from + i + run, n - (i + run), to + i, std::false_type());
      else {
        for(; i != n; ++i) to[i] = std::move(from[i]);
      }
      run *= 2;
      std::swap(from, to);
    }
    if(to != p) {
      for(u32 i = 0; i != n; ++i) p[i] = std::move(tmp[i]);
    }
  }
}
template<class R, class Comp, class Proj> constexpr auto SortIndexImpl(R&& r, Comp&& comp, Proj&& proj) {
  u32 n = std::ranges::size(r);
  Vec<u32> res(n);
  for(u32 i = 0; i != n; ++i) res[i] = i;
  SortImpl(res, std::forward<Comp>(comp), [start = std::ranges::begin(r), pj = std::forward<Proj>(proj)](u32 n) { return std::invoke(pj, *std::ranges::next(start, n)); });
  return res;
}
template<class R, class Comp, class Proj> constexpr auto OrderImpl(R&& r, Comp&& comp, Proj&& proj) {
  u32 n = std::ranges::size(r);
  if(n == 0) return Vec<u32>();
  auto idx = SortIndexImpl(r, comp, proj);
  Vec<u32> res(n);
  u32 cnt = 0;
  auto new_proj = [start = std::ranges::begin(r), pj = std::forward<Proj>(proj)](u32 n) { return std::invoke(pj, *std::ranges::next(start, n)); };
  for(u32 i = 1; i != n; ++i) {
    cnt += std::invoke(comp, new_proj(idx[i - 1]), new_proj(idx[i]));
    res[idx[i]] = cnt;
  }
  return res;
}
template<class R, class Comp, class Proj> constexpr auto IsSortedImpl(R&& r, Comp&& comp, Proj&& proj) { return std::ranges::is_sorted(std::forward<R>(r), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
template<class R, class Comp, class Proj> constexpr auto IsSortedUntilImpl(R&& r, Comp&& comp, Proj&& proj) { return std::ranges::is_sorted_until(std::forward<R>(r), std::forward<Comp>(comp), std::forward<Proj>(proj)); }
template<class R, class Equal> constexpr auto IsPalindromeImpl(R&& r, Equal&& equal) {
  u32 n = std::ranges::size(r);
  if(n == 0) return true;
  auto first = std::ranges::begin(r);
  auto last = std::ranges::next(first, n - 1);
  for(u32 i = 0; i != n / 2; ++i) {
    if(!std::invoke(equal, *first, *last)) return false;
    ++first;
    --last;
  }
  return true;
}
template<class Iter, class Sent, class T, class Proj, class Comp> constexpr auto LowerBoundImpl(Iter first, Sent last, const T& value, Comp&& comp, Proj&& proj) {
  u32 len = std::ranges::distance(first, last);
  if(len == 0) [[unlikely]]
    return first;
  auto back = std::ranges::next(first, len - 1);
  if(std::invoke(comp, std::invoke(proj, *back), value)) [[unlikely]]
    return std::ranges::next(back);
  while(len > 1) {
    u32 half = len / 2;
    std::ranges::advance(first, static_cast<bool>(std::invoke(comp, std::invoke(proj, *std::ranges::next(first, half - 1)), value)) * half);
    len -= half;
  }
  return first;
}
template<class Iter, class Sent, class T, class Proj, class Comp> constexpr auto UpperBoundImpl(Iter first, Sent last, const T& value, Comp&& comp, Proj&& proj) {
  u32 len = std::ranges::distance(first, last);
  if(len == 0) [[unlikely]]
    return first;
  auto back = std::ranges::next(first, len - 1);
  if(!std::invoke(comp, value, std::invoke(proj, *back))) [[unlikely]]
    return std::ranges::next(back);
  while(len > 1) {
    u32 half = len / 2;
    std::ranges::advance(first, static_cast<bool>(!std::invoke(comp, value, std::invoke(proj, *std::ranges::next(first, half - 1)))) * half);
    len -= half;
  }
  return first;
}
} // namespace internal
template<std::ranges::input_range R1, std::ranges::input_range R2, class Proj = Identity, class Comp = EqualTo> constexpr u32 HammingDistance(R1&& r1, R2&& r2, Comp&& comp = {}, Proj&& proj = {}) {
  auto itr1 = std::ranges::begin(r1);
  auto itr2 = std::ranges::begin(r2);
  auto sent1 = std::ranges::end(r1);
  auto sent2 = std::ranges::end(r2);
  u32 result = 0;
  while(itr1 != sent1 && itr2 != sent2) {
    result += !static_cast<bool>(std::invoke(comp, std::invoke(proj, *itr1), std::invoke(proj, *itr2)));
    ++itr1;
    ++itr2;
  }
  if(itr1 != sent1 || itr2 != sent2) { throw Exception("gsh::HammingDistance / The sizes of the two ranges are different."); }
  return result;
}
template<std::ranges::forward_range R, class Proj = Identity, class Comp = Less> constexpr Vec<u32> LongestIncreasingSubsequence(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
  using T = std::ranges::range_value_t<R>;
  Vec<u32> idx(std::ranges::size(r));
  u32 len = 0;
  {
    Vec<T> dp(idx.size());
    u32 i = 0;
    T *begin = dp.data(), *last = dp.data();
    for(auto&& x : r) {
      if(begin == last || std::invoke(comp, *(last - 1), std::invoke(proj, x))) [[unlikely]] {
        idx[i++] = last - begin;
        *last = x;
        ++last;
      } else {
        T* loc = Subrange{begin, last - 1}.lower_bound(x, comp, proj);
        idx[i++] = loc - begin;
        *loc = x;
      }
    }
    len = last - begin;
  }
  u32 cnt = len - 1;
  Vec<u32> res(len);
  for(u32 i = idx.size(); i--;) {
    if(idx[i] == cnt) res[cnt--] = i;
  }
  return res;
}
template<std::ranges::forward_range R, class Proj = Identity, class Comp = Less> constexpr u32 LongestIncreasingSubsequenceLength(R&& r, Comp&& comp = {}, Proj&& proj = {}) {
  using T = std::ranges::range_value_t<R>;
  Vec<T> dp(std::ranges::size(r));
  T *begin = dp.data(), *last = dp.data();
  for(auto&& x : r) {
    if(begin == last || std::invoke(comp, *(last - 1), std::invoke(proj, x))) [[unlikely]] {
      *last = x;
      ++last;
    } else {
      T* loc = Subrange{begin, last - 1}.lower_bound(x, comp, proj);
      *loc = x;
    }
  }
  return last - begin;
}
template<std::ranges::random_access_range R> constexpr Vec<u32> LongestCommonPrefixArray(R&& r) {
  const u32 n = std::ranges::size(r);
  Vec<u32> res(n);
  if(n == 0) return res;
  res[0] = n;
  const auto itr = std::ranges::begin(r);
  u32 i = 1, j = 0;
  while(i != n) {
    while(i + j < n && *std::ranges::next(itr, j) == *std::ranges::next(itr, i + j)) ++j;
    res[i] = j;
    if(j == 0) {
      ++i;
      continue;
    }
    u32 k = 1;
    while(k < j && k + res[k] < j) ++k;
    const auto a = res.data() + i + 1;
    const auto b = res.data() + 1;
    for(u32 l = 0; l != k - 1; ++l) a[l] = b[l];
    i += k;
    j -= k;
  }
  return res;
}
template<class T = u64, std::ranges::range R> requires std::same_as<std::ranges::range_value_t<R>, u32> constexpr T CountDistinctSubsequences(R&& r) {
  const u32 n = std::ranges::size(r);
  if(n == 0) return 0;
  Vec<u64> s(n);
  for(u32 i = 0; u32 x : r) {
    s[i] = static_cast<u64>(i) << 32 | x;
    ++i;
  }
  s.sort({}, [](u64 x) { return static_cast<u32>(x); });
  Vec<u32> rank(n);
  rank[s[0] >> 32] = 0;
  u32 cnt = 0, end = s[0];
  for(u32 i = 1; i != n; ++i) {
    cnt += end != static_cast<u32>(s[i]);
    rank[s[i] >> 32] = cnt;
    end = s[i];
  }
  Vec<u32> last(cnt + 1, n);
  Vec<T> dp(n + 1);
  dp[0] = 1;
  const T m = 2;
  for(u32 i = 0; i != n; ++i) {
    dp[i + 1] = m * dp[i] - dp[last[rank[i]]];
    last[rank[i]] = i;
  }
  return dp[n] - dp[0];
}
template<std::ranges::forward_range R> constexpr auto Majority(R&& r) {
  u32 c = 0;
  u32 len = 0;
  auto i = std::ranges::begin(r);
  auto j = std::ranges::end(r), k = j;
  for(; i != j; ++i) {
    ++len;
    if(c == 0) k = i, c = 1;
    else c += static_cast<u32>(static_cast<bool>(*i == *k)) * 2 - 1;
  }
  c = 0;
  for(i = std::ranges::begin(r); i != j; ++i) c += static_cast<bool>(*i == *k);
  return (2 * c >= len ? k : j);
}
class Mo {
  struct rg {
    u32 l, r;
    constexpr rg(u32 a, u32 b) : l(a), r(b) {}
  };
  Vec<rg> qu;
  double coef = 1.05;
public:
  constexpr Mo() {}
  constexpr void reserve(u32 q) { qu.reserve(q); }
  constexpr void query(u32 l, u32 r) { qu.emplace_back(l, r); }
  constexpr void set_coef(double c) { coef = c; }
  template<class F1, class F2, class F3> void run(F1&& add, F2&& del, F3&& slv) const { run(add, add, del, del, slv); }
  template<class F1, class F2, class F3, class F4, class F5> void run(F1&& addl, F2&& addr, F3&& dell, F4&& delr, F5&& slv) const {
    const u32 Q = qu.size();
    u32 N = 0;
    for(u32 i = 0; i != Q; ++i) N = N < qu[i].r ? qu[i].r : N;
    u32 width = coef * std::sqrt(static_cast<f64>(3ull * N * N) / (2 * Q));
    width += width == 0;
    Vec<u32> cnt(N + 1), buf(Q), block(Q), idx(Q);
    for(u32 i = 0; i != Q; ++i) ++cnt[qu[i].r];
    for(u32 i = 0; i != N; ++i) cnt[i + 1] += cnt[i];
    for(u32 i = 0; i != Q; ++i) buf[--cnt[qu[i].r]] = i;
    cnt.assign(N / width + 2, 0);
    for(u32 i = 0; i != Q; ++i) block[i] = qu[i].l / width;
    for(u32 i = 0; i != Q; ++i) ++cnt[block[i]];
    for(u32 i = 0; i != cnt.size() - 1; ++i) cnt[i + 1] += cnt[i];
    for(u32 i = 0; i != Q; ++i) idx[--cnt[block[buf[i]]]] = buf[i];
    for(u32 i = 0; i < cnt.size() - 1; i += 2) {
      const u32 l = cnt[i], r = cnt[i + 1];
      for(u32 j = 0; j != (r - l) / 2; ++j) {
        const u32 t = idx[l + j];
        idx[l + j] = idx[r - j - 1], idx[r - j - 1] = t;
      }
    }
    u32 nl = 0, nr = 0;
    for(u32 i : idx) {
      while(nl > qu[i].l) std::invoke(addl, --nl);
      while(nr < qu[i].r) std::invoke(addr, nr++);
      while(nl < qu[i].l) std::invoke(dell, nl++);
      while(nr > qu[i].r) std::invoke(delr, --nr);
      std::invoke(slv, i);
    }
  }
};
} // namespace gsh

#include <tuple>

namespace gsh {
namespace io {
struct i4dig;
struct u4dig;
struct i8dig;
struct u8dig;
struct i16dig;
struct u16dig;
struct i8_pad;
struct u8_pad;
struct u16_pad;
struct u32_pad;
struct u64_pad;
struct u128_pad;
struct u4dig_pad;
struct u8dig_pad;
struct u16dig_pad;
} // namespace io
template<class T> class Formatter;
namespace internal {
template<bool Flag> constexpr auto InttoStr = [] {
  struct {
    c8 table[40004] = {};
  } res;
  if constexpr(Flag) {
    for(u32 i = 0; i != 10000; ++i) {
      res.table[4 * i + 0] = i < 1000 ? ' ' : (i / 1000 + '0');
      res.table[4 * i + 1] = i < 100 ? ' ' : (i / 100 % 10 + '0');
      res.table[4 * i + 2] = i < 10 ? ' ' : (i / 10 % 10 + '0');
      res.table[4 * i + 3] = (i % 10 + '0');
    }
  } else {
    for(u32 i = 0; i != 10000; ++i) {
      res.table[4 * i + 0] = (i / 1000 + '0');
      res.table[4 * i + 1] = (i / 100 % 10 + '0');
      res.table[4 * i + 2] = (i / 10 % 10 + '0');
      res.table[4 * i + 3] = (i % 10 + '0');
    }
  }
  return res;
}();
template<bool Flag = false, class Stream> constexpr void Formatu16(Stream&& stream, u16 n) {
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u16 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u16 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  if(n < 10000) copy1(n);
  else {
    copy1(n / 10000);
    copy2(n % 10000);
  }
  stream.skip(p - cur);
}
template<bool Flag = false, class Stream> constexpr void Formatu32(Stream&& stream, u32 n) {
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u32 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u32 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  if(n < 100000000) {
    if(n < 10000) copy1(n);
    else {
      copy1(n / 10000);
      copy2(n % 10000);
    }
  } else {
    copy1(n / 100000000);
    copy2(n / 10000 % 10000);
    copy2(n % 10000);
  }
  stream.skip(p - cur);
}
template<bool Flag = false, class Stream> constexpr void Formatu64(Stream&& stream, u64 n) {
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u64 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u64 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  if(n >= 10000000000000000) {
    u64 a = n / 100000000, b = n % 100000000;
    u64 c = a / 10000, d = a % 10000, e = b / 10000, f = b % 10000;
    u64 g = c / 10000, h = c % 10000;
    copy1(g), copy2(h), copy2(d), copy2(e), copy2(f);
  } else if(n >= 1000000000000) {
    u64 a = n / 100000000, b = n % 100000000;
    u64 c = a / 10000, d = a % 10000, e = b / 10000, f = b % 10000;
    copy1(c), copy2(d), copy2(e), copy2(f);
  } else if(n >= 100000000) {
    u64 a = n / 100000000, b = n % 100000000;
    u64 c = b / 10000, d = b % 10000;
    copy1(a), copy2(c), copy2(d);
  } else if(n >= 10000) {
    u64 a = n / 10000, b = n % 10000;
    copy1(a), copy2(b);
  } else {
    copy1(n);
  }
  stream.skip(p - cur);
}
template<bool Flag = false, class Stream> constexpr void Formatu128(Stream&& stream, u128 n) {
  if(u64(n >> 64) == 0) {
    Formatu64<Flag>(std::forward<Stream>(stream), n);
    return;
  }
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u32 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u32 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  constexpr u128 t = static_cast<u128>(10000000000000000) * 10000000000000000;
  if(n >= t) {
    const u32 dv = n / t;
    n -= dv * t;
    if(dv >= 10000) {
      copy1(dv / 10000);
      copy2(dv % 10000);
    } else copy1(dv);
    auto [a, b] = Divu128(n >> 64, n, 10000000000000000);
    const u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
    copy2(c / 10000), copy2(c % 10000);
    copy2(d / 10000), copy2(d % 10000);
    copy2(e / 10000), copy2(e % 10000);
    copy2(f / 10000), copy2(f % 10000);
  } else {
    auto [a, b] = Divu128(n >> 64, n, 10000000000000000);
    const u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
    const u32 g = c / 10000, h = c % 10000, i = d / 10000, j = d % 10000, k = e / 10000, l = e % 10000, m = f / 10000, n = f % 10000;
    if(c == 0) {
      if(i == 0) copy1(j);
      else copy1(i), copy2(j);
    } else {
      if(g == 0) copy1(h), copy2(i), copy2(j);
      else copy1(g), copy2(h), copy2(i), copy2(j);
    }
    copy2(k), copy2(l), copy2(m), copy2(n);
  }
  stream.skip(p - cur);
}
template<bool Flag = false, class Stream> constexpr void Formatu4dig(Stream&& stream, u16 x) {
  if constexpr(Flag) {
    MemoryCopy(stream.current(), InttoStr<Flag>.table + 4 * x, 4);
    stream.skip(4);
  } else {
    u32 off = (x < 10) + (x < 100) + (x < 1000);
    MemoryCopy(stream.current(), InttoStr<false>.table + (4 * x + off), 4);
    stream.skip(4 - off);
  }
}
template<bool Flag = false, class Stream> constexpr void Formatu8dig(Stream&& stream, u32 n) {
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u32 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u32 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  if(n < 10000) copy1(n);
  else {
    copy1(n / 10000);
    copy2(n % 10000);
  }
  stream.skip(p - cur);
}
template<bool Flag = false, class Stream> constexpr void Formatu16dig(Stream&& stream, u64 n) {
  auto *cur = stream.current(), *p = cur;
  auto copy1 = [&](u64 x) {
    if constexpr(Flag) {
      MemoryCopy(p, InttoStr<Flag>.table + 4 * x, 4);
      p += 4;
    } else {
      u32 off = (x < 10) + (x < 100) + (x < 1000);
      MemoryCopy(p, InttoStr<false>.table + (4 * x + off), 4);
      p += 4 - off;
    }
  };
  auto copy2 = [&](u64 x) {
    MemoryCopy(p, InttoStr<false>.table + 4 * x, 4);
    p += 4;
  };
  if(n < 1000000000000) {
    if(n < 100000000) {
      if(n < 10000) copy1(n);
      else {
        copy1(n / 10000);
        copy2(n % 10000);
      }
    } else {
      copy1(n / 100000000);
      copy2(n / 10000 % 10000);
      copy2(n % 10000);
    }
  } else {
    copy1(n / 1000000000000);
    copy2(n / 100000000 % 10000);
    copy2(n / 10000 % 10000);
    copy2(n % 10000);
  }
  stream.skip(p - cur);
}
} // namespace internal
template<> class Formatter<u16> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u16 n) const {
    stream.reload(8);
    internal::Formatu16(stream, n);
  }
};
template<> class Formatter<i16> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i16 n) const {
    stream.reload(8);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu16(stream, n < 0 ? -n : n);
  }
};
template<> class Formatter<u32> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u32 n) const {
    stream.reload(16);
    internal::Formatu32(stream, n);
  }
};
template<> class Formatter<i32> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i32 n) const {
    stream.reload(16);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu32(stream, n < 0 ? -n : n);
  }
};
template<> class Formatter<u64> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u64 n) const {
    stream.reload(32);
    internal::Formatu64(stream, n);
  }
};
template<> class Formatter<i64> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i64 n) const {
    stream.reload(32);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu64(stream, n < 0 ? -n : n);
  }
};
template<> class Formatter<u128> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u128 n) const {
    stream.reload(64);
    internal::Formatu128(stream, n);
  }
};
template<> class Formatter<i128> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i128 n) const {
    stream.reload(64);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu128(stream, n < 0 ? -n : n);
  }
};
template<> class Formatter<io::u4dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u16 n) const {
    stream.reload(4);
    internal::Formatu4dig(stream, n);
  }
};
template<> class Formatter<io::i4dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i16 n) const {
    stream.reload(5);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu4dig(stream, static_cast<u16>(n < 0 ? -n : n));
  }
};
template<> class Formatter<io::u8dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u32 n) const {
    stream.reload(8);
    internal::Formatu8dig(stream, n);
  }
};
template<> class Formatter<io::i8dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i32 n) const {
    stream.reload(9);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu8dig(stream, static_cast<u32>(n < 0 ? -n : n));
  }
};
template<> class Formatter<io::u16dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u64 n) const {
    stream.reload(16);
    internal::Formatu16dig(stream, n);
  }
};
template<> class Formatter<io::i16dig> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, i64 n) const {
    stream.reload(17);
    *stream.current() = '-';
    stream.skip(n < 0);
    internal::Formatu16dig(stream, static_cast<u64>(n < 0 ? -n : n));
  }
};
template<> class Formatter<io::u16_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u16 n) const {
    stream.reload(8);
    internal::Formatu16<true>(stream, n);
  }
};
template<> class Formatter<io::u32_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u32 n) const {
    stream.reload(16);
    internal::Formatu32<true>(stream, n);
  }
};
template<> class Formatter<io::u64_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u64 n) const {
    stream.reload(32);
    internal::Formatu64<true>(stream, n);
  }
};
template<> class Formatter<io::u128_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u128 n) const {
    stream.reload(64);
    internal::Formatu128<true>(stream, n);
  }
};
template<> class Formatter<io::u4dig_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u16 n) const {
    stream.reload(4);
    internal::Formatu4dig<true>(stream, n);
  }
};
template<> class Formatter<io::u8dig_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u32 n) const {
    stream.reload(8);
    internal::Formatu8dig<true>(stream, n);
  }
};
template<> class Formatter<io::u16dig_pad> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, u64 n) const {
    stream.reload(16);
    internal::Formatu16dig<true>(stream, n);
  }
};
template<> class Formatter<c8> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, c8 c) const {
    stream.reload(1);
    *stream.current() = c;
    stream.skip(1);
  }
};
namespace io {
enum class FormatterOption : std::underlying_type_t<std::chars_format> {};
constexpr FormatterOption operator|(FormatterOption a, FormatterOption b) noexcept { return static_cast<FormatterOption>(static_cast<std::underlying_type_t<FormatterOption>>(a) | static_cast<std::underlying_type_t<FormatterOption>>(b)); }
[[maybe_unused]] constexpr auto Fixed = static_cast<FormatterOption>(std::chars_format::fixed);
[[maybe_unused]] constexpr auto General = static_cast<FormatterOption>(std::chars_format::general);
[[maybe_unused]] constexpr auto Hex = static_cast<FormatterOption>(std::chars_format::hex);
[[maybe_unused]] constexpr auto Scientific = static_cast<FormatterOption>(std::chars_format::scientific);
} // namespace io
namespace internal {
template<class T> class FloatFormatter {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, T f, io::FormatterOption fmt = io::Fixed, i32 precision = 12) {
    stream.reload(32);
    auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, static_cast<std::chars_format>(fmt), precision);
    if(err != std::errc{}) [[unlikely]] {
      stream.reload();
      auto [ptr, err] = std::to_chars(stream.current(), stream.current() + stream.avail(), f, static_cast<std::chars_format>(fmt), precision);
      if(err != std::errc{}) throw Exception("gsh::internal::FloatFormatter::operator() / The value is too large.");
      stream.skip(ptr - stream.current());
    } else {
      stream.skip(ptr - stream.current());
    }
  }
};
} // namespace internal
template<> class Formatter<float> : public internal::FloatFormatter<float> {};
template<> class Formatter<double> : public internal::FloatFormatter<double> {};
template<> class Formatter<long double> : public internal::FloatFormatter<long double> {};
#ifdef __STDCPP_FLOAT16_T__
template<> class Formatter<std::float16_t> : public internal::FloatFormatter<std::float16_t> {};
#endif
#ifdef __STDCPP_FLOAT32_T__
template<> class Formatter<std::float32_t> : public internal::FloatFormatter<std::float32_t> {};
#endif
#ifdef __STDCPP_FLOAT64_T__
template<> class Formatter<std::float64_t> : public internal::FloatFormatter<std::float64_t> {};
#endif
#ifdef __STDCPP_FLOAT128_T__
template<> class Formatter<std::float128_t> : public internal::FloatFormatter<std::float128_t> {};
#endif
#ifdef __STDCPP_BFLOAT16_T__
template<> class Formatter<std::bfloat16_t> : public internal::FloatFormatter<std::bfloat16_t> {};
#endif
#ifdef __SIZEOF_FLOAT128__
template<> class Formatter<__float128> : public internal::FloatFormatter<__float128> {};
#endif
template<> class Formatter<InvalidFloat16Tag> {};
template<> class Formatter<InvalidFloat128Tag> {};
template<> class Formatter<InvalidBfloat16Tag> {};
template<> class Formatter<bool> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, bool b) const {
    stream.reload(1);
    *stream.current() = '0' + b;
    stream.skip(1);
  }
};
template<> class Formatter<const c8*> {
public:
  template<class Stream> constexpr void operator()(Stream&& stream, const c8* s) const { operator()(stream, s, StrLen(s)); }
  template<class Stream> constexpr void operator()(Stream&& stream, const c8* s, u32 len) const {
    u32 avail = stream.avail();
    if(avail >= len) [[likely]] {
      MemoryCopy(stream.current(), s, len);
      stream.skip(len);
    } else {
      MemoryCopy(stream.current(), s, avail);
      len -= avail;
      s += avail;
      stream.skip(avail);
      while(len != 0) {
        stream.reload();
        avail = stream.avail();
        const u32 tmp = len < avail ? len : avail;
        MemoryCopy(stream.current(), s, tmp);
        len -= tmp;
        s += tmp;
        stream.skip(tmp);
      }
    }
  }
};
template<> class Formatter<c8*> : public Formatter<const c8*> {};
class NoOutTag {};
constexpr NoOutTag NoOut;
template<> class Formatter<NoOutTag> {
public:
  template<class Stream> constexpr void operator()(Stream&&, NoOutTag) const {}
};
template<class R> concept FormatableRange = std::ranges::forward_range<R> && requires { sizeof(Formatter<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<FormatableRange R> class Formatter<R> {
  template<class Stream, class T, class U> constexpr void print(Stream&& stream, T&& r, U&& sep) const {
    auto first = std::ranges::begin(r);
    auto last = std::ranges::end(r);
    if(!(first != last)) return;
    Formatter<std::decay_t<std::ranges::range_value_t<R>>> formatter;
    while(true) {
      formatter(stream, *first);
      ++first;
      if(first != last) {
        Formatter<std::decay_t<U>>()(stream, sep);
      } else break;
    }
  }
public:
  template<class Stream, class T> requires std::same_as<std::decay_t<T>, R> constexpr void operator()(Stream&& stream, T&& r) const { print(std::forward<Stream>(stream), std::forward<T>(r), ' '); }
  template<class Stream, class T, class U> requires std::same_as<std::decay_t<T>, R> constexpr void operator()(Stream&& stream, T&& r, U&& sep) const { print(std::forward<Stream>(stream), std::forward<T>(r), std::forward<U>(sep)); }
};
namespace internal {
template<class T, class U> constexpr bool FormatableTupleImpl = false;
template<class T, std::size_t... I> constexpr bool FormatableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Formatter<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
} // namespace internal
template<class T> concept FormatableTuple = requires { std::tuple_size<T>::value; } && internal::FormatableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<FormatableTuple T> requires (!FormatableRange<T>) class Formatter<T> {
  template<u32 I, class Stream, class U, class Sep> constexpr void print_element(Stream&& stream, U&& x, Sep&& sep) const {
    using std::get;
    using element_type = std::decay_t<std::tuple_element_t<I, T>>;
    if constexpr(requires { x.template get<I>(); }) Formatter<element_type>()(stream, x.template get<I>());
    else Formatter<element_type>()(stream, get<I>(x));
    if constexpr(I < std::tuple_size_v<T> - 1) Formatter<std::decay_t<Sep>>()(stream, sep);
  }
  template<class Stream, class U, class Sep> constexpr void print(Stream&& stream, U&& x, Sep&& sep) const {
    [&]<u32... I>(std::integer_sequence<u32, I...>) { (..., print_element<I>(stream, x, sep)); }(std::make_integer_sequence<u32, std::tuple_size_v<T>>());
  }
public:
  template<class Stream, class U> constexpr void operator()(Stream&& stream, U&& x) const { print(std::forward<Stream>(stream), std::forward<U>(x), ' '); }
  template<class Stream, class U, class Sep> constexpr void operator()(Stream&& stream, U&& x, Sep&& sep) const { print(std::forward<Stream>(stream), std::forward<U>(x), std::forward<Sep>(sep)); }
};
} // namespace gsh

#include <cerrno>
#include <cstdlib>

namespace gsh {
namespace io {
struct i4dig;
struct u4dig;
struct i8dig;
struct u8dig;
struct i16dig;
struct u16dig;
} // namespace io
template<class T> class Parser;
namespace internal {
template<class Stream> constexpr u16 Parseu4dig(Stream&& stream) {
  u32 v;
  MemoryCopy(&v, stream.current(), 4);
  v ^= 0x30303030;
  i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
  v <<= (32 - (tmp << 3));
  v = (v * 10 + (v >> 8)) & 0x00ff00ff;
  v = (v * 100 + (v >> 16)) & 0x0000ffff;
  stream.skip(tmp + 1);
  return v;
}
template<class Stream> constexpr u32 Parseu8dig(Stream&& stream) {
  u64 v;
  MemoryCopy(&v, stream.current(), 8);
  v ^= 0x3030303030303030;
  i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
  v <<= (64 - (tmp << 3));
  v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
  v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
  v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
  stream.skip(tmp + 1);
  return v;
}
template<class Stream> constexpr u8 Parseu8(Stream&& stream) { return Parseu4dig(stream); }
template<class Stream> constexpr u16 Parseu16(Stream&& stream) { return Parseu8dig(stream); }
template<class Stream> constexpr u32 Parseu32(Stream&& stream) {
  u32 res = 0;
  u64 buf[2];
  MemoryCopy(buf, stream.current(), 16);
  u64 rem;
  {
    buf[0] ^= 0x3030303030303030, buf[1] ^= 0x3030303030303030;
    u64 v = buf[0];
    rem = v;
    if(!(v & 0xf0f0f0f0f0f0f0f0)) [[likely]] {
      stream.skip(8);
      rem = buf[1];
      v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
      v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
      v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
      res = v;
    }
  }
  {
    u32 v = rem;
    if(!(v & 0xf0f0f0f0)) {
      rem >>= 32;
      v = (v * 10 + (v >> 8)) & 0x00ff00ff;
      v = (v * 100 + (v >> 16)) & 0x0000ffff;
      res = 10000 * res + v;
      stream.skip(4);
    }
  }
  {
    u32 v = rem & 0xffff;
    if(!(v & 0xf0f0)) {
      rem >>= 16;
      v = (v * 10 + (v >> 8)) & 0x00ff;
      res = 100 * res + v;
      stream.skip(2);
    }
  }
  {
    const bool f = !(rem & 0xf0);
    res = f ? 10 * res + (rem & 0xff) : res;
    stream.skip(f + 1);
  }
  return res;
};
template<class Stream> constexpr u64 Parseu64(Stream&& stream) {
  c8 const* cur = stream.current();
  u64 v;
  MemoryCopy(&v, cur, 8);
  if(!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
    u64 u;
    MemoryCopy(&u, cur + 8, 8);
    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
    if(!((u ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
      u = (u * 10 + (u >> 8)) & 0x00ff00ff00ff00ff;
      u = (u * 100 + (u >> 16)) & 0x0000ffff0000ffff;
      u = (u * 10000 + (u >> 32)) & 0x00000000ffffffff;
      v = v * 100000000 + u;
      u32 rem;
      MemoryCopy(&rem, cur + 16, 4);
      rem ^= 0x30303030;
      if((rem & 0xf0f0f0f0) == 0) [[unlikely]] {
        rem = (rem * 10 + (rem >> 8)) & 0x00ff00ff;
        rem = (rem * 100 + (rem >> 16)) & 0x0000ffff;
        v = v * 10000 + rem;
        stream.skip(21);
      } else if((rem & 0xf0f0f0) == 0) {
        v = v * 1000 + ((rem & 0xff) * 100) + (((rem * 2561) & 0xff0000) >> 16);
        stream.skip(20);
      } else if((rem & 0xf0f0) == 0) {
        v = v * 100 + (((rem >> 8) + (rem * 10)) & 0xff);
        stream.skip(19);
      } else if((rem & 0xf0) == 0) {
        v = v * 10 + (rem & 0x0000000f);
        stream.skip(18);
      } else {
        stream.skip(17);
      }
      return v;
    } else {
      u32 c = 9;
      while(!(u & 0xf0)) {
        v = v * 10 + (u & 0xff);
        u >>= 8;
        ++c;
      }
      stream.skip(c);
      return v;
    }
  } else {
    i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
    v <<= (64 - (tmp << 3));
    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
    stream.skip(tmp + 1);
    return v;
  }
}
template<class Stream> constexpr u128 Parseu128(Stream&& stream) {
  u128 res = 0;
  for(u32 i = 0; i != 4; ++i) {
    u64 v;
    MemoryCopy(&v, stream.current(), 8);
    if(((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) != 0) break;
    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
    if(i == 0) res = v;
    else res = res * 100000000 + v;
    stream.skip(8);
  }
  u64 buf;
  MemoryCopy(&buf, stream.current(), 8);
  buf ^= 0x3030303030303030;
  u64 res2 = 0, pw = 1;
  {
    u32 v = buf;
    if(!(v & 0xf0f0f0f0)) {
      buf >>= 32;
      v = (v * 10 + (v >> 8)) & 0x00ff00ff;
      v = (v * 100 + (v >> 16)) & 0x0000ffff;
      res2 = v;
      pw = 10000;
      stream.skip(4);
    }
  }
  {
    u32 v = buf & 0xffff;
    if(!(v & 0xf0f0)) {
      buf >>= 16;
      v = (v * 10 + (v >> 8)) & 0x00ff;
      res2 = res2 * 100 + v;
      pw *= 100;
      stream.skip(2);
    }
  }
  {
    const c8 v = buf;
    const bool f = (v & 0xf0) == 0;
    const volatile auto tmp1 = pw * 10, tmp2 = res2 * 10 + v;
    const auto tmp3 = tmp1, tmp4 = tmp2;
    pw = f ? tmp3 : pw;
    res2 = f ? tmp4 : res2;
    stream.skip(f + 1);
  }
  return res * pw + res2;
}
} // namespace internal
template<> class Parser<u8> {
public:
  template<class Stream> constexpr u8 operator()(Stream&& stream) const {
    stream.reload(8);
    return internal::Parseu8(stream);
  }
};
template<> class Parser<i8> {
public:
  template<class Stream> constexpr i8 operator()(Stream&& stream) const {
    stream.reload(8);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i8 tmp = internal::Parseu8(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<u16> {
public:
  template<class Stream> constexpr u16 operator()(Stream&& stream) const {
    stream.reload(8);
    return internal::Parseu16(stream);
  }
};
template<> class Parser<i16> {
public:
  template<class Stream> constexpr i16 operator()(Stream&& stream) const {
    stream.reload(8);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i16 tmp = internal::Parseu16(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<u32> {
public:
  template<class Stream> constexpr u32 operator()(Stream&& stream) const {
    stream.reload(16);
    return internal::Parseu32(stream);
  }
};
template<> class Parser<i32> {
public:
  template<class Stream> constexpr i32 operator()(Stream&& stream) const {
    stream.reload(16);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i32 tmp = internal::Parseu32(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<u64> {
public:
  template<class Stream> constexpr u64 operator()(Stream&& stream) const {
    stream.reload(32);
    return internal::Parseu64(stream);
  }
};
template<> class Parser<i64> {
public:
  template<class Stream> constexpr i64 operator()(Stream&& stream) const {
    stream.reload(32);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i64 tmp = internal::Parseu64(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<u128> {
public:
  template<class Stream> constexpr u128 operator()(Stream&& stream) const {
    stream.reload(64);
    return internal::Parseu128(stream);
  }
};
template<> class Parser<i128> {
public:
  template<class Stream> constexpr i128 operator()(Stream&& stream) const {
    stream.reload(64);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i128 tmp = internal::Parseu128(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<io::u4dig> {
public:
  using value_type = u16;
  template<class Stream> constexpr u16 operator()(Stream&& stream) const {
    stream.reload(8);
    return internal::Parseu4dig(stream);
  }
};
template<> class Parser<io::i4dig> {
public:
  using value_type = i16;
  template<class Stream> constexpr i16 operator()(Stream&& stream) const {
    stream.reload(8);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i16 tmp = internal::Parseu4dig(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<io::u8dig> {
public:
  using value_type = u32;
  template<class Stream> constexpr u32 operator()(Stream&& stream) const {
    stream.reload(16);
    return internal::Parseu8dig(stream);
  }
};
template<> class Parser<io::i8dig> {
public:
  using value_type = i32;
  template<class Stream> constexpr i32 operator()(Stream&& stream) const {
    stream.reload(16);
    bool neg = *stream.current() == '-';
    stream.skip(neg);
    i32 tmp = internal::Parseu8dig(stream);
    if(neg) tmp = -tmp;
    return tmp;
  }
};
template<> class Parser<c8> {
public:
  template<class Stream> constexpr c8 operator()(Stream&& stream) const {
    stream.reload(2);
    c8 tmp = *stream.current();
    stream.skip(2);
    return tmp;
  }
};
template<> class Parser<c8*> {
public:
  template<class Stream> constexpr c8* operator()(Stream&& stream, c8* s) const {
    stream.reload(16);
    c8* c = s;
    while(true) {
      const c8* e = stream.current();
      while(*e >= '!') ++e;
      const u32 len = e - stream.current();
      MemoryCopy(c, stream.current(), len);
      stream.skip(len);
      c += len;
      if(stream.avail() == 0) stream.reload();
      else break;
    }
    stream.skip(1);
    *c = '\0';
    return s;
  }
  template<class Stream> constexpr c8* operator()(Stream&& stream, c8* s, u32 n) const {
    u32 rem = n;
    c8* c = s;
    u32 avail = stream.avail();
    while(avail <= rem) {
      MemoryCopy(c, stream.current(), avail);
      c += avail;
      rem -= avail;
      stream.skip(avail);
      if(rem == 0) {
        *c = '\0';
        return s;
      }
      stream.reload();
      avail = stream.avail();
    }
    MemoryCopy(c, stream.current(), rem);
    c += rem;
    stream.skip(rem + 1);
    *c = '\0';
    return s;
  }
};
template<> class Parser<float> {
public:
  template<class Stream> constexpr float operator()(Stream&& stream) const {
    stream.reload(128);
    const c8* cur = stream.current();
    c8* end = nullptr;
    float res = std::strtof(cur, &end);
    if(errno) { throw Exception("gsh::Parser<float>::operator() / Failed to parse."); }
    stream.skip(end - cur + 1);
    return res;
  }
};
template<> class Parser<double> {
public:
  template<class Stream> constexpr double operator()(Stream&& stream) const {
    stream.reload(128);
    const c8* cur = stream.current();
    c8* end = nullptr;
    double res = std::strtod(cur, &end);
    if(errno) { throw Exception("gsh::Parser<double>::operator() / Failed to parse."); }
    stream.skip(end - cur + 1);
    return res;
  }
};
template<> class Parser<long double> {
public:
  template<class Stream> constexpr long double operator()(Stream&& stream) const {
    stream.reload(128);
    const c8* cur = stream.current();
    c8* end = nullptr;
    long double res = std::strtold(cur, &end);
    if(errno) { throw Exception("gsh::Parser<long double>::operator() / Failed to parse."); }
    stream.skip(end - cur + 1);
    return res;
  }
};
namespace internal {
template<class T, class P, class Stream, class... Args> struct ParsingIterator {
  using value_type = T;
  using difference_type = i32;
  using pointer = T*;
  using reference = T&;
  u32 n;
  P* ref;
  Stream* stream;
  std::tuple<Args...>* args;
  constexpr ParsingIterator() noexcept : ParsingIterator(0, nullptr, nullptr, nullptr) {}
  constexpr ParsingIterator(u32 m, P* r, Stream* s, std::tuple<Args...>* a) noexcept : n(m), ref(r), stream(s), args(a) {}
  GSH_INTERNAL_INLINE friend constexpr u32 operator-(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n - b.n; }
  GSH_INTERNAL_INLINE friend constexpr bool operator==(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n == b.n; }
  GSH_INTERNAL_INLINE constexpr ParsingIterator& operator++() noexcept { return ++n, *this; }
  GSH_INTERNAL_INLINE constexpr ParsingIterator operator++(int) noexcept { return {n++, *ref, *stream, *args}; }
  GSH_INTERNAL_INLINE constexpr T operator*() const {
    return [this]<u32... I>(std::integer_sequence<u32, I...>) -> T { return (*ref)(*stream, std::get<I>(*args)...); }(std::make_integer_sequence<u32, sizeof...(Args)>());
  }
};
} // namespace internal
template<class R> concept ParsableRange = std::ranges::forward_range<R> && requires { sizeof(Parser<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<ParsableRange R> class Parser<R> {
public:
  template<class Stream, class... Args> constexpr R operator()(Stream&& stream, u32 len, Args&&... args) const {
    Parser<std::ranges::range_value_t<R>> p;
    std::tuple<Args...> a(std::forward<Args>(args)...);
    using iter = internal::ParsingIterator<std::ranges::range_value_t<R>, decltype(p), std::remove_cvref_t<Stream>, Args...>;
    return R(iter(0, &p, &stream, &a), iter(len, nullptr, nullptr, nullptr));
  }
};
/*
namespace internal {
    template<class T, class U> constexpr bool ParsableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool ParsableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Parser<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept ParsableTuple = requires { std::tuple_size<T>::value; } && internal::ParsableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<ParsableTuple T>
    requires(!ParsableRange<T>)
class Parser<T> {
public:
    template<class Stream> constexpr T operator()(Stream&&& stream) const {}
};
*/
} // namespace gsh

#include <unistd.h>

#if defined(__linux__)
#include <sys/mman.h> // mmap
#include <sys/stat.h> // stat, fstat
#endif
namespace gsh {
namespace internal { template<class D> class IstreamInterface; } // namespace internal
template<class D, class Types, class... Args> class ParsingChain;
class NoParsingResult {
  template<class D, class Types, class... Args> friend class ParsingChain;
  constexpr NoParsingResult() noexcept {}
  NoParsingResult(const NoParsingResult&) = delete;
  NoParsingResult(NoParsingResult&&) = delete;
};
class CustomParser {
  ~CustomParser() = delete;
};
template<class D, class... Types, class... Args> class ParsingChain<D, TypeArr<Types...>, Args...> {
  friend class internal::IstreamInterface<D>;
  template<class D2, class Types2, class... Args2> friend class ParsingChain;
  D& ref;
  [[no_unique_address]] std::tuple<Args...> args;
  GSH_INTERNAL_INLINE constexpr ParsingChain(D& r, std::tuple<Args...>&& a) : ref(r), args(std::move(a)) {}
  template<class... Options> requires (sizeof...(Args) < sizeof...(Types)) GSH_INTERNAL_INLINE constexpr auto next_chain(Options&&... options) const { return ParsingChain<D, TypeArr<Types...>, Args..., std::tuple<Options...>>(ref, std::tuple_cat(args, std::make_tuple(std::forward_as_tuple(std::forward<Options>(options)...)))); };
public:
  ParsingChain() = delete;
  ParsingChain(const ParsingChain&) = delete;
  ParsingChain(ParsingChain&&) = delete;
  ParsingChain& operator=(const ParsingChain&) = delete;
  ParsingChain& operator=(ParsingChain&&) = delete;
  template<class... Options> requires (sizeof...(Args) == 0) [[nodiscard]] constexpr auto option(Options&&... options) const { return next_chain(std::forward<Options>(options)...); }
  template<class... Options> requires (sizeof...(Args) != 0) [[nodiscard]] constexpr auto operator()(Options&&... options) const { return next_chain(std::forward<Options>(options)...); }
  template<std::size_t N> friend constexpr decltype(auto) get(const ParsingChain& chain) {
    auto get_result = [](auto&& parser, auto&&... args) GSH_INTERNAL_INLINE -> decltype(auto) {
      if constexpr(std::is_void_v<std::invoke_result_t<decltype(parser), decltype(args)...>>) {
        std::invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
        return NoParsingResult{};
      } else {
        return std::invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
      }
    };
    using value_type = typename TypeArr<Types...>::template type<N>;
    if constexpr(N < sizeof...(Args)) {
      if constexpr(std::same_as<CustomParser, value_type>) {
        return std::apply([get_result, &chain](auto&& parser, auto&&... args) -> decltype(auto) { return get_result(std::forward<decltype(parser)>(parser), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
      } else {
        return std::apply([get_result, &chain](auto&&... args) -> decltype(auto) { return get_result(Parser<value_type>(), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
      }
    } else {
      return get_result(Parser<value_type>(), chain.ref);
    }
  }
  constexpr void ignore() const {
    [this]<u32... I>(std::integer_sequence<u32, I...>) { (..., get<I>(*this)); }(std::make_integer_sequence<u32, sizeof...(Types)>());
  }
  template<class T> constexpr operator T() const {
    static_assert(sizeof...(Types) == 1);
    return static_cast<T>(get<0>(*this));
  }
  constexpr decltype(auto) val() const {
    static_assert(sizeof...(Types) == 1);
    return get<0>(*this);
  }
  template<class... To> requires (sizeof...(To) == 0 || sizeof...(To) == sizeof...(Types)) constexpr auto bind() const {
    if constexpr(sizeof...(To) == 0) {
      return [this]<u32... I>(std::integer_sequence<u32, I...>) { return std::tuple{get<I>(*this)...}; }(std::make_integer_sequence<u32, sizeof...(Types)>());
    } else {
      return [this]<u32... I>(std::integer_sequence<u32, I...>) { return std::tuple<To...>{static_cast<To>(get<I>(*this))...}; }(std::make_integer_sequence<u32, sizeof...(Types)>());
    }
  }
};
namespace internal {
template<class D> class IstreamInterface {
  constexpr D& derived() { return *static_cast<D*>(this); }
public:
  template<class T, class... Types> [[nodiscard]] constexpr auto read() { return ParsingChain<D, TypeArr<T, Types...>>(derived(), std::tuple<>()); }
};
} // namespace internal
} // namespace gsh
namespace std {
template<class D, class... Types, class... Args> class tuple_size<gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> : public integral_constant<size_t, sizeof...(Types)> {};
template<size_t N, class D, class... Types, class... Args> class tuple_element<N, gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> {
public:
  using type = decltype(get<N>(std::declval<const gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>&>()));
};
} // namespace std
namespace gsh {
namespace internal {
template<class D> class OstreamInterface {
  constexpr D& derived() { return *static_cast<D*>(this); }
public:
  template<class Sep, class... Args> constexpr void write_sep(Sep&& sep, Args&&... args) {
    [&]<u32... I>(std::integer_sequence<u32, I...>) {
      auto print_value = [&]<u32 Idx>(std::integral_constant<u32, Idx>, auto&& val) {
        Formatter<std::decay_t<decltype(val)>>()(derived(), val);
        if constexpr(Idx != sizeof...(Args) - 1) Formatter<std::decay_t<Sep>>()(derived(), std::forward<Sep>(sep));
      };
      (..., print_value(std::integral_constant<u32, I>(), std::forward<Args>(args)));
    }(std::make_integer_sequence<u32, sizeof...(Args)>());
  }
  template<class Sep, class... Args> constexpr void writeln_sep(Sep&& sep, Args&&... args) {
    write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
    Formatter<c8>()(derived(), '\n');
  }
  template<class... Args> constexpr void write(Args&&... args) { write_sep(' ', std::forward<Args>(args)...); }
  template<class... Args> constexpr void writeln(Args&&... args) {
    write_sep(' ', std::forward<Args>(args)...);
    Formatter<c8>()(derived(), '\n');
  }
};
} // namespace internal
template<u32 Bufsize = (1 << 18)> class BasicReader : public internal::IstreamInterface<BasicReader<Bufsize>> {
  i32 fd = 0;
  c8 buf[Bufsize + 1] = {};
  c8 *cur = buf, *eof = buf;
public:
  BasicReader() {}
  BasicReader(i32 filehandle) : fd(filehandle) {}
  BasicReader(const BasicReader& rhs) {
    fd = rhs.fd;
    std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
    cur = buf + (rhs.cur - rhs.buf);
    eof = buf + (rhs.cur - rhs.eof);
  }
  BasicReader& operator=(const BasicReader& rhs) {
    fd = rhs.fd;
    std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
    cur = buf + (rhs.cur - rhs.buf);
    eof = buf + (rhs.cur - rhs.eof);
    return *this;
  }
  void reload() {
    if(eof == buf + Bufsize || eof == cur ||
    [&] {
      auto p = cur;
      while(*p >= '!') ++p;
      return p;
    }()
    == eof) [[likely]] {
      u32 rem = eof - cur;
      std::memmove(buf, cur, rem);
      *(eof = buf + rem + read(fd, buf + rem, Bufsize - rem)) = '\0';
      cur = buf;
    }
  }
  void reload(u32 len) {
    if(avail() < len) [[unlikely]]
      reload();
  }
  u32 avail() const { return eof - cur; }
  const c8* current() const { return cur; }
  void skip(u32 n) { cur += n; }
};
class StaticStrReader : public internal::IstreamInterface<StaticStrReader> {
  const c8* cur;
public:
  constexpr StaticStrReader() {}
  constexpr StaticStrReader(const c8* c) : cur(c) {}
  constexpr void reload() const {}
  constexpr void reload(u32) const {}
  constexpr u32 avail() const { return static_cast<u32>(-1); }
  constexpr const c8* current() { return cur; }
  constexpr void skip(u32 n) { cur += n; }
};
template<u32 Bufsize = (1 << 18)> class BasicWriter : public internal::OstreamInterface<BasicWriter<Bufsize>> {
  i32 fd = 1;
  c8 buf[Bufsize + 1] = {};
  c8 *cur = buf, *eof = buf + Bufsize;
public:
  BasicWriter() {}
  BasicWriter(i32 filehandle) : fd(filehandle) {}
  BasicWriter(const BasicWriter& rhs) {
    fd = rhs.fd;
    std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
    cur = buf + (rhs.cur - rhs.buf);
  }
  BasicWriter& operator=(const BasicWriter& rhs) {
    fd = rhs.fd;
    std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
    cur = buf + (rhs.cur - rhs.buf);
    return *this;
  }
  void reload() {
    [[maybe_unused]] i32 tmp = write(fd, buf, cur - buf);
    cur = buf;
  }
  void reload(u32 len) {
    if(eof - cur < len) [[unlikely]]
      reload();
  }
  u32 avail() const { return eof - cur; }
  c8* current() { return cur; }
  void skip(u32 n) { cur += n; }
};
class StaticStrWriter : public internal::OstreamInterface<StaticStrWriter> {
  c8* cur;
public:
  constexpr StaticStrWriter() {}
  constexpr StaticStrWriter(c8* c) : cur(c) {}
  constexpr void reload() const {}
  constexpr void reload(u32) const {}
  constexpr u32 avail() const { return static_cast<u32>(-1); }
  constexpr c8* current() { return cur; }
  constexpr void skip(u32 n) { cur += n; }
};
class MmapReader : public internal::IstreamInterface<MmapReader> {
  [[maybe_unused]] const i32 fh;
  c8 *buf, *cur, *eof;
public:
  MmapReader() : fh(0) {
#if !defined(__linux__)
    buf = nullptr;
    BasicWriter<128> wt(2);
    wt.write("gsh::MmapReader / gsh::MmapReader is not available for Windows.\n");
    wt.reload();
    std::exit(1);
#else
    struct stat st;
    fstat(0, &st);
    buf = reinterpret_cast<c8*>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0));
    cur = buf;
    eof = buf + st.st_size;
#endif
  }
  void reload() const {}
  void reload(u32) const {}
  u32 avail() const { return eof - cur; }
  const c8* current() const { return cur; }
  void skip(u32 n) { cur += n; }
};
} // namespace gsh

namespace gsh {
namespace internal {
template<class D> class UnionFindImpl {
  D& derived() noexcept { return static_cast<D&>(*this); }
  const D& derived() const noexcept { return static_cast<const D&>(*this); }
public:
  constexpr u32 size() const noexcept { return derived().parent.size(); }
  [[nodiscard]] constexpr bool empty() const noexcept { return derived().parent.empty(); }
  constexpr u32 leader(u32 n) {
#ifndef NDEBUG
    if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::leader / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
    return derived().root(n);
  }
  constexpr bool is_leader(u32 n) const {
#ifndef NDEBUG
    if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::is_leader / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
    return derived().parent[n] < 0;
  }
  constexpr bool same(u32 a, u32 b) {
#ifndef NDEBUG
    if(a >= derived().parent.size() || b >= derived().parent.size()) throw gsh::Exception("gsh::UnionFind::same / The index is out of range. ( a=", a, ", b=", b, ", size=", derived().parent.size(), " )");
#endif
    return derived().root(a) == derived().root(b);
  }
protected:
  GSH_INTERNAL_INLINE constexpr u32 merge_impl(u32 ar, u32 br) noexcept {
    const i32 sa = derived().parent[ar], sb = derived().parent[br];
    const i32 ss = sa + sb;
    const i32 tmp1 = sa < sb ? ar : br;
    const i32 tmp2 = sa < sb ? br : ar;
    --derived().cnt;
    derived().parent[tmp1] = ss;
    derived().parent[tmp2] = tmp1;
    return tmp1;
  }
public:
  constexpr u32 group_size(u32 n) {
#ifndef NDEBUG
    if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::group_size(u32) / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
    return -derived().parent[derived().root(n)];
  }
  constexpr u32 max_group_size() const noexcept {
    i32 res = 1;
    for(u32 i = 0; i != size(); ++i) { res = -derived().parent[i] < res ? res : -derived().parent[i]; }
    return res;
  }
  constexpr u32 count_groups() const noexcept { return derived().cnt; }
  constexpr Vec<u32> extract(u32 n) {
#ifndef NDEBUG
    if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::extract / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
    const i32 nr = derived().root(n);
    u32 ccnt = 0;
    for(u32 i = 0; i != derived().parent.size(); ++i) ccnt += derived().root(i) == nr;
    Vec<u32> res(ccnt);
    for(u32 i = 0, j = 0; i != derived().parent.size(); ++i)
      if(i == static_cast<u32>(nr) || derived().parent[i] == nr) res[j++] = i;
    return res;
  }
  constexpr Vec<Vec<u32>> groups() {
    Vec<u32> key(derived().parent.size());
    u32 cnt = 0;
    for(u32 i = 0; i != derived().parent.size(); ++i) {
      if(derived().parent[i] < 0) key[i] = cnt++;
    }
    Vec<u32> cnt2(cnt);
    for(u32 i = 0; i != derived().parent.size(); ++i) ++cnt2[key[derived().root(i)]];
    Vec<Vec<u32>> res(cnt);
    for(u32 i = 0; i != cnt; ++i) {
      res[i].resize(cnt2[i]);
      cnt2[i] = 0;
    }
    for(u32 i = 0; i != derived().parent.size(); ++i) {
      const u32 idx = key[derived().parent[i] < 0 ? i : derived().parent[i]];
      res[idx][cnt2[idx]++] = i;
    }
    return res;
  }
};
} // namespace internal
class UnionFind : public internal::UnionFindImpl<UnionFind> {
  friend class internal::UnionFindImpl<UnionFind>;
  Vec<i32> parent;
  u32 cnt = 0;
  constexpr i32 rootimpl(i32 n) noexcept {
    if(parent[n] < 0) return n;
    i32 r = rootimpl(parent[n]);
    parent[n] = r;
    return r;
  }
  GSH_INTERNAL_INLINE constexpr i32 root(i32 n) noexcept {
    if(parent[n] < 0) return n;
    i32 m = parent[n];
    if(parent[m] < 0) return parent[n] = m, m;
    i32 r = rootimpl(parent[m]);
    return parent[n] = r, parent[m] = r, r;
  }
public:
  using size_type = u32;
  constexpr UnionFind() noexcept {}
  constexpr explicit UnionFind(u32 n) : parent(n, -1), cnt(n) {}
  constexpr void reset() noexcept {
    cnt = parent.size();
    for(u32 i = 0; i != cnt; ++i) parent[i] = -1;
  }
  constexpr void resize(u32 n) {
    if(n < parent.size()) throw gsh::Exception("gsh::UnionFind::resize / It cannot be smaller than it is now.");
    cnt += n - parent.size();
    parent.resize(n, -1);
  }
  constexpr u32 merge(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::UnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return ar;
    return this->merge_impl(ar, br);
  }
  constexpr bool merge_same(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::UnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return true;
    this->merge_impl(ar, br);
    return false;
  }
};
class RollbackUnionFind : public internal::UnionFindImpl<RollbackUnionFind> {
  friend class internal::UnionFindImpl<RollbackUnionFind>;
  Vec<i32> parent;
  u32 cnt = 0;
  struct change {
    u32 a, b;
    i32 c, d;
    bool merged;
    constexpr change(u32 A, u32 B, i32 C, i32 D, bool M) : a(A), b(B), c(C), d(D), merged(M) {}
  };
  Vec<change> history;
  constexpr i32 root(i32 n) const noexcept {
    while(parent[n] >= 0) n = parent[n];
    return n;
  }
public:
  using size_type = u32;
  constexpr RollbackUnionFind() noexcept {}
  constexpr explicit RollbackUnionFind(u32 n) : parent(n, -1), cnt(n) {}
  constexpr void reset() noexcept {
    cnt = parent.size();
    history.clear();
    for(u32 i = 0; i != parent.size(); ++i) parent[i] = -1;
  }
  constexpr void reserve(u32 q) { history.reserve(history.size() + q); }
  constexpr void resize(u32 n) {
    if(n < parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::resize / It cannot be smaller than it is now.");
    cnt += n - parent.size();
    parent.resize(n, -1);
  }
  constexpr u32 merge(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    history.emplace_back(ar, br, parent[ar], parent[br], ar != br);
    if(ar == br) return ar;
    return merge_impl(ar, br);
  }
  constexpr bool merge_same(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    history.emplace_back(ar, br, parent[ar], parent[br], ar != br);
    if(ar == br) return true;
    merge_impl(ar, br);
    return false;
  }
  constexpr void undo() {
#ifndef NDEBUG
    if(history.empty()) throw Exception("gsh::RollbackUnionFind::undo / The history is empty.");
#endif
    auto [a, b, c, d, e] = history.back();
    history.pop_back();
    parent[a] = c, parent[b] = d;
    cnt += e;
  }
  class state {
    friend class RollbackUnionFind;
    u32 s;
    state() = delete;
    constexpr state(u32 n) noexcept : s(n) {}
  public:
    constexpr state(const state&) noexcept = default;
    constexpr state& operator=(const state&) noexcept = default;
    constexpr state next(i32 n) const noexcept { return state{s + n}; }
    constexpr state prev(i32 n) const noexcept { return state{s - n}; }
    constexpr void advance(i32 n) noexcept { s += n; }
  };
  constexpr state current() const noexcept { return state{history.size()}; }
  constexpr void rollback(state st) {
    while(st.s < history.size()) {
      auto [a, b, c, d, e] = history.back();
      history.pop_back();
      parent[a] = c, parent[b] = d;
      cnt += e;
    }
  }
};
template<class T = i64, class F = Plus, class I = Negate> class PotentializedUnionFind : public internal::UnionFindImpl<PotentializedUnionFind<T, F, I>> {
  friend class internal::UnionFindImpl<PotentializedUnionFind<T, F, I>>;
  Vec<i32> parent;
  Vec<T> diff;
  u32 cnt = 0;
  [[no_unique_address]] F func{};
  [[no_unique_address]] I inv{};
  T el{};
  constexpr i32 root(i32 n) noexcept {
    if(parent[n] < 0) return n;
    const i32 r = root(parent[n]);
    diff[n] = std::invoke(func, diff[parent[n]], diff[n]);
    return parent[n] = r;
  }
public:
  using value_type = T;
  using size_type = u32;
  constexpr PotentializedUnionFind() noexcept(std::is_nothrow_default_constructible_v<F> && std::is_nothrow_default_constructible_v<I> && std::is_nothrow_default_constructible_v<T>) {}
  constexpr explicit PotentializedUnionFind(F f, I i = I(), const T& e = T()) : func(f), inv(i), el(e) {}
  constexpr explicit PotentializedUnionFind(u32 n, F f = F(), I i = I(), const T& e = T()) : parent(n, -1), diff(n, e), cnt(n), func(f), inv(i), el(e) {}
  constexpr void reset() {
    cnt = parent.size();
    for(u32 i = 0; i != parent.size(); ++i) parent[i] = -1;
    diff.assign(parent.size(), el);
  }
  constexpr void resize(u32 n) {
    if(n < parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::resize / It cannot be smaller than it is now.");
    cnt += n - parent.size();
    parent.resize(n, -1);
    diff.resize(n, el);
  }
  constexpr const T& operator[](u32 n) { return potential(n); }
  constexpr const T& potential(u32 n) {
#ifndef NDEBUG
    if(n >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::potential / The index is out of range. ( n=", n, ", size=", parent.size(), " )");
#endif
    root(n);
    return diff[n];
  }
  // A[a] = func(A[b], result)
  constexpr T potential(u32 a, u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::potential / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    root(a);
    root(b);
    return std::invoke(func, std::invoke(inv, diff[b]), diff[a]);
  }
  // A[a] = func(A[b], w) return leader(a)
  constexpr u32 merge(const u32 a, const u32 b, const T& w) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return ar;
    const i32 sa = parent[ar], sb = parent[br];
    const bool f = sa < sb;
    const i32 tmp1 = f ? ar : br, tmp2 = f ? br : ar;
    parent[tmp1] += parent[tmp2];
    parent[tmp2] = tmp1;
    diff[tmp2] = std::invoke(func, diff[f ? a : b], std::invoke(inv, std::invoke(func, diff[f ? b : a], f ? w : std::invoke(inv, w))));
    --cnt;
    return tmp1;
  }
  // A[a] = func(A[b], w) return same(a, b)
  constexpr bool merge_same(const u32 a, const u32 b, const T& w) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return true;
    const i32 sa = parent[ar], sb = parent[br];
    const bool f = sa < sb;
    const i32 tmp1 = f ? ar : br, tmp2 = f ? br : ar;
    parent[tmp1] += parent[tmp2];
    parent[tmp2] = tmp1;
    diff[tmp2] = std::invoke(func, diff[f ? a : b], std::invoke(inv, std::invoke(func, diff[f ? b : a], f ? w : std::invoke(inv, w))));
    --cnt;
    return false;
  }
  // A[a] = func(A[b], w)
  constexpr bool merge_valid(const u32 a, const u32 b, const T& w) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge_valid / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return diff[a] == std::invoke(func, diff[b], w);
    const i32 sa = parent[ar], sb = parent[br];
    const bool f = sa < sb;
    const i32 tmp1 = f ? ar : br, tmp2 = f ? br : ar;
    parent[tmp1] += parent[tmp2];
    parent[tmp2] = tmp1;
    diff[tmp2] = std::invoke(func, diff[f ? a : b], std::invoke(inv, std::invoke(func, diff[f ? b : a], f ? w : std::invoke(inv, w))));
    --cnt;
    return true;
  }
};
template<class T, class F> class MonoidalUnionFind : public internal::UnionFindImpl<MonoidalUnionFind<T, F>> {
  friend class internal::UnionFindImpl<MonoidalUnionFind<T, F>>;
  Vec<i32> parent;
  Vec<T> monoid;
  [[no_unique_address]] F func;
  u32 cnt = 0;
  constexpr i32 root(i32 n) noexcept {
    if(parent[n] < 0) return n;
    return parent[n] = root(parent[n]);
  }
public:
  using value_type = T;
  using size_type = u32;
  constexpr MonoidalUnionFind() noexcept {}
  constexpr explicit MonoidalUnionFind(F f) : func(std::move(f)) {}
  constexpr explicit MonoidalUnionFind(u32 n, F f = F()) : parent(n, -1), monoid(n), func(std::move(f)), cnt(n) {}
  constexpr MonoidalUnionFind(u32 n, F f, const T& init) : parent(n, -1), monoid(n, init), func(std::move(f)), cnt(n) {}
  constexpr ~MonoidalUnionFind() {
    for(u32 i = 0; i != parent.size(); ++i) {
      if(parent[i] < 0) std::destroy_at(&monoid[i]);
    }
    monoid.reset();
  }
  constexpr void reset() {
    cnt = parent.size();
    for(u32 i = 0; i != parent.size(); ++i) {
      if(parent[i] < 0) std::destroy_at(&monoid[i]);
      parent[i] = -1;
      std::construct_at(&monoid[i]);
    }
  }
  constexpr void reset(const T& init) {
    cnt = parent.size();
    for(u32 i = 0; i != parent.size(); ++i) {
      if(parent[i] < 0) std::destroy_at(&monoid[i]);
      parent[i] = -1;
      std::construct_at(&monoid[i], init);
    }
  }
  constexpr void resize(u32 n) {
    if(n < parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::resize / It cannot be smaller than it is now.");
    cnt += n - parent.size();
    parent.resize(n, -1);
    monoid.resize(n);
  }
  constexpr void resize(u32 n, const T& value) {
    if(n < parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::resize / It cannot be smaller than it is now.");
    cnt += n - parent.size();
    parent.resize(n, -1);
    monoid.resize(n, value);
  }
  constexpr T& operator[](u32 n) {
#ifndef NDEBUG
    if(n >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::operator[] / The index is out of range. ( n=", n, ", size=", parent.size(), " )");
#endif
    return monoid[root(n)];
  }
  constexpr u32 merge(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return ar;
    const i32 sa = parent[ar], sb = parent[br];
    const bool f = sa < sb;
    const i32 tmp1 = f ? ar : br, tmp2 = f ? br : ar;
    parent[tmp1] += parent[tmp2];
    parent[tmp2] = tmp1;
    monoid[tmp1] = std::invoke(func, std::move(monoid[tmp1]), std::move(monoid[tmp2]));
    std::destroy_at(&monoid[tmp2]);
    --cnt;
    return tmp1;
  }
  constexpr bool merge_same(const u32 a, const u32 b) {
#ifndef NDEBUG
    if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
    const i32 ar = root(a), br = root(b);
    if(ar == br) return true;
    const i32 sa = parent[ar], sb = parent[br];
    const bool f = sa < sb;
    const i32 tmp1 = f ? ar : br, tmp2 = f ? br : ar;
    parent[tmp1] += parent[tmp2];
    parent[tmp2] = tmp1;
    monoid[tmp1] = std::invoke(func, std::move(monoid[tmp1]), std::move(monoid[tmp2]));
    std::destroy_at(&monoid[tmp2]);
    --cnt;
    return false;
  }
};
} // namespace gsh



using namespace std;
using namespace gsh;
using namespace gsh::io;

int main() {
#if defined(ONLINE_JUDGE)
    gsh::MmapReader rd;
#else
    static gsh::BasicReader rd;
#endif
    static gsh::BasicWriter<1 << 19> wt;
    /*========================================*/
    auto [N, M] = rd.read<u32, u32>();
    UnionFind uf(N);
    static u64 p[500000];
    static u64 e[500000];
    for (u32 i = 0; i != M; ++i) {
        auto [a, b, c] = rd.read<u32, u32, u32>();
        e[i] = u64(a) << 32 | b;
        p[i] = u64(i) << 32 | c;
    }
    internal::SortUnsigned32(p, M);
    static u32 res[500000];
    u32 cnt = 0;
    u64 sum = 0;
    for (u32 i = 0; i != M; ++i) {
        u32 idx = p[i] >> 32;
        u64 v = e[idx];
        if (!uf.merge_same(u32(v >> 32), u32(v))) {
            res[cnt++] = idx;
            sum += p[i] & 0xffffffff;
            if (uf.count_groups() == 1) break;
        }
    }
    wt.writeln(sum);
    wt.writeln(Subrange(res, res + cnt));
    /*========================================*/
    wt.reload();
    return 0;
}
