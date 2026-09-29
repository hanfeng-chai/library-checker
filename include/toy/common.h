#pragma once

// x86-64 / Linux / GCC or Clang / C++23. Headers do not imply runtime support:
// -nostdlib++ rejects operations that need the C++ library at link time.
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <charconv>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <compare>
#include <complex>
#include <concepts>
#include <coroutine>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#include <expected>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <numbers>
#include <numeric>
#include <optional>
#include <random>
#include <ranges>
#include <ratio>
#include <scoped_allocator>
#include <source_location>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <version>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace toy {
using namespace std;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
using isize = intptr_t;
using usize = uintptr_t;
using f32 = float;
using f64 = double;
using f80 = long double;
}
