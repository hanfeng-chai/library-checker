#pragma once
#include <toy/buffer.h>

namespace toy {

// Finite values; additions must fit T. Convex means nondecreasing differences.
template <class T>
Buffer<T> min_plus_convex_convex(std::span<const T> a, std::span<const T> b) {
    if (a.empty() || b.empty()) return {};
    Buffer<T> c(a.size() + b.size() - 1);
    usize i = 0, j = 0;
    c[0] = a[0] + b[0];
    while (i + 1 < a.size() && j + 1 < b.size()) {
        T x = a[i + 1] + b[j], y = a[i] + b[j + 1];
        bool take = x < y;
        i += take;
        j += !take;
        c[i + j] = std::min(x, y);
    }
    while (i + 1 < a.size()) ++i, c[i + j] = a[i] + b[j];
    while (j + 1 < b.size()) ++j, c[i + j] = a[i] + b[j];
    return c;
}

template <class T>
Buffer<T> min_plus_convex_arbitrary(std::span<const T> a, std::span<const T> b) {
    if (a.empty() || b.empty()) return {};
    int n = a.size(), m = b.size();
    Buffer<T> c(n + m - 1);
    // Convexity makes the leftmost minimizing index in B nondecreasing.
    auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {
        if (l > r) return;
        int mid = (l + r) / 2, first = std::max(lo, mid - n + 1), last = std::min(hi, mid);
        int best = first;
        T value = a[mid - first] + b[first];
        for (int j = first + 1; j <= last; ++j) {
            T next = a[mid - j] + b[j];
            if (next < value) value = next, best = j;
        }
        c[mid] = value;
        self(self, l, mid - 1, lo, best);
        self(self, mid + 1, r, best, hi);
    };
    solve(solve, 0, n + m - 2, 0, m - 1);
    return c;
}

template <class T>
Buffer<T> min_plus_concave_arbitrary(std::span<const T> a, std::span<const T> b) {
    if (a.empty() || b.empty()) return {};
    int n = a.size(), m = b.size();
    Buffer<T> c(n + m - 1);
    std::fill(c.p, c.p + c.n, std::numeric_limits<T>::max());
    struct Segment {
        int index, end;
    };
    Buffer<Segment> stack(n);
    // On a triangular block, a newly inserted translate beats an older one
    // on a prefix. Keep the lower envelope as a stack of expiration indices.
    auto prefix = [&](int rows, int columns, auto value, auto store) {
        int top = 0;
        for (int t = 0; t < rows; ++t) {
            // The difference only increases: losing at insertion means losing
            // at every later row too, so most candidates need no binary search.
            if (t < columns && (!top || value(t, t) < value(stack[top - 1].index, t))) {
                while (top && value(t, stack[top - 1].end) <=
                                  value(stack[top - 1].index, stack[top - 1].end))
                    --top;
                int end = rows - 1;
                if (top) {
                    int l = t - 1, r = stack[top - 1].end;
                    while (l + 1 < r) {
                        int mid = (l + r) / 2;
                        if (value(t, mid) <= value(stack[top - 1].index, mid))
                            l = mid;
                        else
                            r = mid;
                    }
                    end = l;
                }
                if (end >= t) stack[top++] = {t, end};
            }
            store(t, value(stack[top - 1].index, t));
            if (stack[top - 1].end == t) --top;
        }
    };
    for (int s = 0; s < m; s += n) {
        int k = std::min(n, m - s);
        prefix(
            n, k, [&](int j, int t) { return a[t - j] + b[s + j]; },
            [&](int t, T v) { c[s + t] = std::min(c[s + t], v); });
        // Reverse both inputs for the right triangle, without copying them.
        prefix(
            k - 1, k, [&](int j, int t) { return a[n - 1 - t + j] + b[s + k - 1 - j]; },
            [&](int t, T v) {
                int i = s + n + k - 2 - t;
                c[i] = std::min(c[i], v);
            });
    }
    return c;
}
} // namespace toy
