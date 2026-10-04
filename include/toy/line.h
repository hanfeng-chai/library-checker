#pragma once
#include <toy/common.h>
namespace toy {
struct Line {
    static constexpr i64 infinity = 3000000000000000000ll;
    i32 slope = 0;
    i64 intercept = infinity;
    i64 operator()(i32 x) const { return i64(slope) * x + intercept; }
};
} // namespace toy
