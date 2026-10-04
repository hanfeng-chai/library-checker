#pragma once
#include <toy/lazy_matrix.h>
namespace toy {
// In-place Gauss-Jordan: eliminated columns store the inverse, so no identity
// augmentation is needed. Complete pivoting also exposes rank n-1 adjugates.
template <u32 P = 998244353>
struct SquareReduction {
    using M = Mod<P>;
    u32 n, rank = 0, scale = 1;
    LazyMatrix<P> matrix;
    Buffer<u32> row_swap, column_swap, factor;
    SquareReduction(u32 n, std::span<const u32> input)
        : n(n), matrix(n, n, input), row_swap(n), column_swap(n), factor(n) {
        std::iota(row_swap.p, row_swap.p + n, 0u);
        std::iota(column_swap.p, column_swap.p + n, 0u);
        auto &row = matrix.row;
        for (u32 k = 0; k < n; k += 2) {
            u32 r = n, c = k;
            for (; c < n; ++c) {
                for (r = k; r < n && !matrix.get(r, c); ++r);
                if (r < n) break;
            }
            if (r == n) break;
            swap(k, r, c);
            u64 *first = row[k];
            u32 value = matrix.get(k, k), inv = M::pow(value, P - 2);
            scale = M::mul(scale, value);
            for (u32 j = 0; j < n; ++j) first[j] = j == k ? inv : M::mul(first[j] % P, inv);
            matrix.count[k] = 0;
            for (u32 i = 0; i < n; ++i)
                if (i != k) factor[i] = M::sub(0, matrix.get(i, k));
            r = n;
            c = k + 1;
            u32 second_value = 0;
            if (k + 1 < n)
                for (; c < n; ++c) {
                    for (r = k + 1; r < n; ++r)
                        if ((second_value = (matrix.get(r, c) + u64(factor[r]) * first[c]) % P))
                            break;
                    if (r < n) break;
                }
            if (r == n) {
                for (u32 i = 0; i < n; ++i)
                    if (i != k) {
                        row[i][k] = 0;
                        if (factor[i])
                            matrix.template add<false, false>(i, first, nullptr, factor[i], 0, 0, 0,
                                                              0);
                    }
                rank = k + 1;
                break;
            }
            swap(k + 1, r, c);
            std::swap(factor[k + 1], factor[r]);
            u64 *second = row[k + 1];
            u32 inv2 = M::pow(second_value, P - 2), f = factor[k + 1];
            scale = M::mul(scale, second_value);
            second[k] = 0;
            for (u32 j = 0; j < n; ++j)
                second[j] =
                    j == k + 1 ? inv2 : M::mul((second[j] % P + u64(f) * first[j]) % P, inv2);
            matrix.count[k + 1] = 0;
            // Overwriting the second eliminated column discards its first
            // update; restore that inverse column explicitly after the batch.
            for (u32 i = 0; i < n;) {
                if (i == k || i == k + 1) {
                    ++i;
                    continue;
                }
                u32 a = factor[i],
                    b = M::sub(0, (matrix.get(i, k + 1) + u64(a) * first[k + 1]) % P);
                row[i][k] = 0;
                if (i + 1 < n && i + 1 != k && i + 1 != k + 1) {
                    u32 c = factor[i + 1],
                        d = M::sub(0, (matrix.get(i + 1, k + 1) + u64(c) * first[k + 1]) % P);
                    row[i + 1][k] = 0;
                    if (a | b | c | d)
                        matrix.template add<true, true>(i, first, second, a, b, c, d, 0);
                    row[i][k + 1] = M::mul(b, inv2);
                    row[i + 1][k + 1] = M::mul(d, inv2);
                    i += 2;
                } else {
                    if (a | b) matrix.template add<false, true>(i, first, second, a, b, 0, 0, 0);
                    row[i][k + 1] = M::mul(b, inv2);
                    ++i;
                }
            }
            u32 coefficient = M::sub(0, first[k + 1]);
            first[k + 1] = 0;
            if (coefficient)
                for (u32 j = 0; j < n; ++j)
                    first[j] = (first[j] + u64(coefficient) * second[j]) % P;
            rank = k + 2;
        }
    }
    void swap(u32 k, u32 r, u32 c) {
        row_swap[k] = r;
        column_swap[k] = c;
        if (r != k) {
            matrix.swap_rows(r, k);
            scale = M::sub(0, scale);
        }
        if (c != k) {
            for (u32 i = 0; i < n; ++i) std::swap(matrix.row[i][c], matrix.row[i][k]);
            scale = M::sub(0, scale);
        }
    }
    Buffer<u32> result(bool adjugate = false) const {
        if (!adjugate && rank < n) return {};
        Buffer<u32> answer(usize(n) * n);
        if (rank == n) {
            for (u32 i = 0; i < n; ++i)
                for (u32 j = 0; j < n; ++j) {
                    u32 value = matrix.get(i, j);
                    answer[usize(i) * n + j] = adjugate ? M::mul(scale, value) : value;
                }
        } else {
            std::fill(answer.p, answer.p + answer.n, 0u);
            if (rank + 1 < n) return answer;
            for (u32 i = 0; i < n; ++i) {
                u32 v = i + 1 == n ? 1 : M::sub(0, matrix.get(i, n - 1));
                v = M::mul(v, scale);
                for (u32 j = 0; j < n; ++j)
                    answer[usize(i) * n + j] = M::mul(v, j + 1 == n ? 1 : matrix.get(n - 1, j));
            }
        }
        for (u32 k = rank; k--;) {
            u32 r = column_swap[k], c = row_swap[k];
            if (r != k)
                for (u32 j = 0; j < n; ++j)
                    std::swap(answer[usize(k) * n + j], answer[usize(r) * n + j]);
            if (c != k)
                for (u32 i = 0; i < n; ++i)
                    std::swap(answer[usize(i) * n + k], answer[usize(i) * n + c]);
        }
        return answer;
    }
};
} // namespace toy
