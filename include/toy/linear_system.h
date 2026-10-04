#pragma once
#include <toy/lazy_matrix.h>
namespace toy {
template <u32 P = 998244353>
struct FieldEchelon {
    using M = Mod<P>;
    u32 n, width, scale = 1;
    LazyMatrix<P> matrix;
    Buffer<u32> pivot;
    FieldEchelon(u32 n, u32 width, u32 columns, std::span<const u32> input, bool reduced = false)
        : n(n), width(width), matrix(n, width, input), pivot(0, std::min(n, columns)) {
        auto &row = matrix.row;
        Buffer<u32> factor(n);
        u32 rank = 0;
        for (u32 column = 0; column < columns && rank < n;) {
            u32 p = rank;
            while (p < n && !get(p, column)) ++p;
            if (p == n) {
                ++column;
                continue;
            }
            swap(p, rank);
            u64 *first = row[rank];
            u32 value = get(rank, column), inverse = M::pow(value, P - 2);
            scale = M::mul(scale, value);
            for (u32 j = column; j < width; ++j) first[j] = M::mul(first[j] % P, inverse);
            matrix.count[rank] = 0;
            pivot.p[pivot.n++] = column;
            if (reduced) {
                for (u32 i = 0; i < n; ++i)
                    if (i != rank) {
                        u32 value = get(i, column);
                        row[i][column] = 0;
                        if (value)
                            matrix.template add<false, false>(i, first, nullptr, P - value, 0, 0, 0,
                                                              column + 1);
                    }
                ++rank;
                ++column;
                continue;
            }
            for (u32 i = rank + 1; i < n; ++i) factor[i] = M::sub(0, get(i, column));
            u32 next = column + 1, second_value = 0;
            p = n;
            if (rank + 1 < n)
                for (; next < columns; ++next) {
                    for (p = rank + 1; p < n; ++p)
                        if ((second_value = (get(p, next) + u64(factor[p]) * first[next]) % P))
                            break;
                    if (p < n) break;
                }
            if (p == n) {
                for (u32 i = rank + 1; i < n; ++i) {
                    row[i][column] = 0;
                    if (factor[i])
                        matrix.template add<false, false>(i, first, nullptr, factor[i], 0, 0, 0,
                                                          column + 1);
                }
                break;
            }
            swap(p, rank + 1);
            std::swap(factor[p], factor[rank + 1]);
            u64 *second = row[rank + 1];
            u32 inverse2 = M::pow(second_value, P - 2), f = factor[rank + 1];
            scale = M::mul(scale, second_value);
            second[column] = 0;
            for (u32 j = column + 1; j < width; ++j)
                second[j] = M::mul((second[j] % P + u64(f) * first[j]) % P, inverse2);
            matrix.count[rank + 1] = 0;
            pivot.p[pivot.n++] = next;
            for (u32 i = rank + 2; i < n; i += 2) {
                u32 a = factor[i], b = M::sub(0, (get(i, next) + u64(a) * first[next]) % P);
                row[i][column] = 0;
                if (i + 1 < n) {
                    u32 c = factor[i + 1],
                        d = M::sub(0, (get(i + 1, next) + u64(c) * first[next]) % P);
                    row[i + 1][column] = 0;
                    if (a | b | c | d)
                        matrix.template add<true, true>(i, first, second, a, b, c, d, column + 1);
                    row[i + 1][next] = 0;
                } else if (a | b)
                    matrix.template add<false, true>(i, first, second, a, b, 0, 0, column + 1);
                row[i][next] = 0;
            }
            rank += 2;
            column = next + 1;
        }
    }
    void swap(u32 a, u32 b) {
        if (a != b) {
            matrix.swap_rows(a, b);
            scale = M::sub(0, scale);
        }
    }
    u32 get(u32 i, u32 j) const { return matrix.get(i, j); }
};
} // namespace toy
