#pragma once
#include <toy/buffer.h>
namespace toy {
struct Assignment {
    i64 cost = 0;
    Buffer<u32> column;
};
// Dense shortest augmenting paths. Column prices are updated once per path;
// dist stores absolute labels, avoiding a full slack subtraction at every step.
template <bool Refine = true>
Assignment assignment(u32 n, std::span<const i32> cost) {
    Assignment result{0, Buffer<u32>(n)};
    bool monge = true, anti = true;
    for (u32 i = 1; i < n && (monge || anti); ++i)
        for (u32 j = 1; j < n; ++j) {
            i64 a = i64(cost[(i - 1) * n + j - 1]) + cost[i * n + j],
                b = i64(cost[(i - 1) * n + j]) + cost[i * n + j - 1];
            monge &= a <= b;
            anti &= a >= b;
            if (!monge && !anti) break;
        }
    // Adjacent Monge inequalities imply that uncrossing never increases cost.
    if (monge || anti) {
        for (u32 i = 0; i < n; ++i) {
            u32 j = monge ? i : n - 1 - i;
            result.column[i] = j;
            result.cost += cost[i * n + j];
        }
        return result;
    }
    Buffer<i64> price(n), distance(n);
    Buffer<u32> match(n), previous(n), visited(n);
    Buffer<u8> used(n), unique(n);
    std::fill(unique.p, unique.p + n, 0);
    std::fill(price.p, price.p + n, INT64_MAX);
    std::fill(match.p, match.p + n, ~0u);
    std::fill(result.column.p, result.column.p + n, ~0u);
    // Column reduction supplies a feasible dual and an initial partial matching.
    for (u32 i = 0; i < n; ++i)
        for (u32 j = 0; j < n; ++j)
            if (cost[i * n + j] < price[j]) {
                price[j] = cost[i * n + j];
                previous[j] = i;
            }
    for (u32 j = 0; j < n; ++j) {
        u32 i = previous[j];
        if (result.column[i] == ~0u) {
            result.column[i] = j;
            match[j] = i;
            unique[i] = 1;
        } else
            unique[i] = 0;
    }
    for (u32 i = 0; i < n; ++i)
        if (unique[i]) {
            u32 c = result.column[i];
            i64 slack = INT64_MAX;
            for (u32 j = 0; j < n; ++j)
                if (j != c) slack = std::min(slack, i64(cost[i * n + j]) - price[j]);
            price[c] -= slack;
        }
    if constexpr (Refine) {
        // Two augmenting row-reduction passes usually leave few free rows.
        for (int pass = 0; pass < 2; ++pass)
            for (u32 i = 0; i < n; ++i)
                if (result.column[i] == ~0u) {
                    i64 first = INT64_MAX, second = INT64_MAX;
                    u32 column = 0;
                    for (u32 j = 0; j < n; ++j) {
                        i64 value = i64(cost[i * n + j]) - price[j];
                        if (value < first || (value == first && match[column] != ~0u)) {
                            second = first;
                            first = value;
                            column = j;
                        } else
                            second = std::min(second, value);
                    }
                    if (first < second) price[column] -= second - first;
                    u32 old = match[column];
                    if (old != ~0u) result.column[old] = ~0u;
                    match[column] = i;
                    result.column[i] = column;
                }
        std::iota(visited.p, visited.p + n, 0u);
        for (u32 start = 0; start < n; ++start)
            if (result.column[start] == ~0u) {
                for (u32 j = 0; j < n; ++j) {
                    distance[j] = i64(cost[start * n + j]) - price[j];
                    previous[j] = start;
                }
                u32 scanned = 0, labeled = 0, last = 0, free = ~0u;
                while (free == ~0u) {
                    if (scanned == labeled) {
                        last = scanned;
                        i64 best = distance[visited[scanned]];
                        for (u32 j = scanned; j < n; ++j) {
                            u32 c = visited[j];
                            if (distance[c] <= best) {
                                if (distance[c] < best) {
                                    best = distance[c];
                                    labeled = scanned;
                                }
                                std::swap(visited[j], visited[labeled++]);
                            }
                        }
                        for (u32 j = scanned; j < labeled; ++j)
                            if (match[visited[j]] == ~0u) {
                                free = visited[j];
                                break;
                            }
                        if (free != ~0u) break;
                    }
                    u32 c = visited[scanned++], row = match[c];
                    i64 shift = distance[c] - cost[row * n + c] + price[c];
                    for (u32 j = labeled; j < n; ++j) {
                        u32 v = visited[j];
                        i64 next = shift + cost[row * n + v] - price[v];
                        if (next < distance[v]) {
                            distance[v] = next;
                            previous[v] = row;
                            if (next == distance[c]) {
                                if (match[v] == ~0u) {
                                    free = v;
                                    break;
                                }
                                std::swap(visited[j], visited[labeled++]);
                            }
                        }
                    }
                }
                for (u32 j = 0; j < last; ++j) {
                    u32 c = visited[j];
                    price[c] += distance[c] - distance[free];
                }
                while (free != ~0u) {
                    u32 row = previous[free];
                    match[free] = row;
                    free = std::exchange(result.column[row], free);
                }
            }
    } else
        for (u32 row = 0; row < n; ++row)
            if (result.column[row] == ~0u) {
                for (u32 j = 0; j < n; ++j) {
                    distance[j] = i64(cost[row * n + j]) - price[j];
                    previous[j] = ~0u;
                }
                memset(used.p, 0, n);
                u32 count = 0, column;
                for (;;) {
                    i64 best = INT64_MAX;
                    column = 0;
                    for (u32 j = 0; j < n; ++j)
                        if (!used[j] &&
                            (distance[j] < best || (distance[j] == best && match[j] == ~0u))) {
                            best = distance[j];
                            column = j;
                        }
                    if (match[column] == ~0u) break;
                    used[column] = 1;
                    visited[count++] = column;
                    u32 i = match[column];
                    i64 shift = best - cost[i * n + column] + price[column];
                    for (u32 j = 0; j < n; ++j)
                        if (!used[j]) {
                            i64 next = shift + cost[i * n + j] - price[j];
                            if (next < distance[j]) {
                                distance[j] = next;
                                previous[j] = column;
                            }
                        }
                }
                for (u32 k = 0; k < count; ++k) {
                    u32 j = visited[k];
                    price[j] += distance[j] - distance[column];
                }
                while (previous[column] != ~0u) {
                    u32 p = previous[column];
                    match[column] = match[p];
                    column = p;
                }
                match[column] = row;
                result.column[row] = column;
            }
    for (u32 j = 0; j < n; ++j) {
        result.column[match[j]] = j;
        result.cost += cost[match[j] * n + j];
    }
    return result;
}
} // namespace toy
