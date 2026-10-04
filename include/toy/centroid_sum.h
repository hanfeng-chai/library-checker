#pragma once
#include <toy/centroid_clusters.h>
#include <toy/fenwick.h>
namespace toy {
template <class T = i64, bool Dual = false>
struct CentroidCore {
    static constexpr u32 Block = 32, Small = Dual ? 32 : 96;
    struct Leaf {
        u32 first, length, distance;
    };
    struct Step {
        u32 group, branch, distance;
    };
    u32 n, diameter;
    Buffer<u32> offset, begin, length, owner;
    Buffer<u8> degree;
    Buffer<Step> path;
    Buffer<T> data, base;
    Buffer<u32> which, index;
    Buffer<T> leaf_values;
    Buffer<u8> distances;
    Buffer<Leaf> leaves;
    CentroidCore(const Adjacency &graph, std::span<const T> initial = {})
        : n(graph.size()), diameter(n ? n - 1 : 0), offset(n + 1), length(2 * n), owner(2 * n),
          degree(n), which(n), index(n), leaf_values(0, n), distances(0, usize(n) * Block),
          leaves(0, n) {
        u32 groups = n;
        std::fill(which.p, which.p + n, ~0u);
        std::fill(length.p, length.p + length.n, 0u);
        std::fill(offset.p, offset.p + offset.n, 0u);
        struct Link {
            Step step;
            u32 vertex;
        };
        Buffer<Link> links(0, usize(n) * std::bit_width(std::max(1u, n)));
        auto append = [&](CentroidPoint p, u32 group, u32 branch) {
            u32 id = links.n++;
            links[id] = {{group, branch, p.distance}, p.vertex};
            ++offset[p.vertex + 1];
        };
        centroid_clusters(
            graph,
            [&](u32 c, std::span<const CentroidPoint> points, std::span<const u32> starts) {
                if (points.size() == n) {
                    u32 d = 0;
                    for (auto p : points) d = std::max(d, p.distance);
                    diameter = std::min(diameter, 2 * d);
                }
                u32 branches = starts.size() - 1, first = groups;
                degree[c] = std::min(branches, 3u);
                length[c] = 0;
                if (branches > 2) {
                    for (auto p : points) length[c] = std::max(length[c], p.distance + 1);
                }
                append(points[0], c, branches <= 2 ? first : ~0u);
                for (u32 b = 0; b < branches; ++b) {
                    u32 branch = groups++, largest = 0;
                    owner[branch] = c;
                    for (u32 i = starts[b]; i < starts[b + 1]; ++i) {
                        append(points[i], branches <= 2 ? branch : c,
                               branches <= 2 ? (branches == 2 ? first + (b ^ 1) : ~0u) : branch);
                        largest = std::max(largest, points[i].distance);
                    }
                    length[branch] = largest + 1;
                }
            },
            [&](std::span<const u32> vertices) {
                u32 id = leaves.n++, first = leaf_values.n, k = vertices.size(),
                    start = distances.n;
                leaves[id] = {first, k, start};
                distances.n += k * k;
                for (u32 v : vertices) {
                    which[v] = id;
                    index[v] = leaf_values.n;
                    leaf_values[leaf_values.n++] = initial.empty() ? T{} : initial[v];
                }
                std::array<u32, Block> queue;
                for (u32 i = 0; i < k; ++i) {
                    u8 *row = distances.p + start + i * k;
                    std::fill(row, row + k, u8(255));
                    row[i] = 0;
                    queue[0] = vertices[i];
                    for (u32 at = 0, count = 1; at < count; ++at) {
                        u32 v = queue[at];
                        for (u32 w : graph[v])
                            if (which[w] == id) {
                                u32 j = index[w] - first;
                                if (row[j] == 255) {
                                    row[j] = row[index[v] - first] + 1;
                                    queue[count++] = w;
                                }
                            }
                    }
                }
            },
            Block);
        for (u32 v = 1; v <= n; ++v) offset[v] += offset[v - 1];
        path = Buffer<Step>(links.n);
        Buffer<u32> cursor(n);
        if (n) memcpy(cursor.p, offset.p, 4 * n);
        for (auto e : std::span(links.p, links.n)) path[cursor[e.vertex]++] = e.step;
        length.n = groups;
        begin = Buffer<u32>(groups + 1);
        begin[0] = 0;
        for (u32 g = 0; g < groups; ++g) begin[g + 1] = begin[g] + length[g] + 1;
        data = Buffer<T>(begin[groups]);
        std::fill(data.p, data.p + data.n, T{});
        base = Buffer<T>(n);
        if (initial.empty())
            std::fill(base.p, base.p + n, T{});
        else
            memcpy(base.p, initial.data(), initial.size_bytes());
        if constexpr (!Dual) {
            for (u32 v = 0; v < n; ++v)
                for (auto e : steps(v)) {
                    if (e.group >= n)
                        data[begin[e.group] + e.distance + 1] += base[v];
                    else if (degree[e.group] > 2) {
                        data[begin[e.group] + e.distance + 1] += base[v];
                        if (e.branch != ~0u) data[begin[e.branch] + e.distance + 1] += base[v];
                    }
                }
            for (u32 g = 0; g < groups; ++g) {
                if (length[g] <= Small) continue;
                T *p = data.p + begin[g];
                for (u32 i = 1; i <= length[g]; ++i) {
                    u32 next = i + (i & -i);
                    if (next <= length[g]) p[next] += p[i];
                }
            }
        }
    }
    T leaf_sum(u32 vertex, u32 l, u32 r) const {
        auto b = leaves[which[vertex]];
        const u8 *d = distances.p + b.distance + (index[vertex] - b.first) * b.length;
        const T *p = leaf_values.p + b.first;
        u32 i = 0;
        T result{};
        if constexpr (std::is_same_v<T, i64> || std::is_same_v<T, u64>) {
            auto total = _mm256_setzero_si256(), low = _mm256_set1_epi64x(i64(l) - 1),
                 high = _mm256_set1_epi64x(r);
            for (; i + 4 <= b.length; i += 4) {
                u32 bits;
                memcpy(&bits, d + i, 4);
                auto distance = _mm256_cvtepu8_epi64(_mm_cvtsi32_si128(bits));
                auto mask = _mm256_and_si256(_mm256_cmpgt_epi64(distance, low),
                                             _mm256_cmpgt_epi64(high, distance));
                total = _mm256_add_epi64(
                    total, _mm256_and_si256(mask, _mm256_loadu_si256((const __m256i *)(p + i))));
            }
            auto v =
                _mm_add_epi64(_mm256_castsi256_si128(total), _mm256_extracti128_si256(total, 1));
            result =
                std::bit_cast<T>(u64(_mm_cvtsi128_si64(_mm_add_epi64(v, _mm_srli_si128(v, 8)))));
        }
        for (; i < b.length; ++i)
            if (l <= d[i] && d[i] < r) result += p[i];
        return result;
    }
    void leaf_apply(u32 vertex, u32 l, u32 r, T value) {
        auto b = leaves[which[vertex]];
        const u8 *d = distances.p + b.distance + (index[vertex] - b.first) * b.length;
        T *p = leaf_values.p + b.first;
        u32 i = 0;
        if constexpr (std::is_same_v<T, i64> || std::is_same_v<T, u64>) {
            auto low = _mm256_set1_epi64x(i64(l) - 1), high = _mm256_set1_epi64x(r),
                 delta = _mm256_set1_epi64x(value);
            for (; i + 4 <= b.length; i += 4) {
                u32 bits;
                memcpy(&bits, d + i, 4);
                auto distance = _mm256_cvtepu8_epi64(_mm_cvtsi32_si128(bits));
                auto mask = _mm256_and_si256(_mm256_cmpgt_epi64(distance, low),
                                             _mm256_cmpgt_epi64(high, distance));
                auto *q = (__m256i *)(p + i);
                _mm256_storeu_si256(
                    q, _mm256_add_epi64(_mm256_loadu_si256(q), _mm256_and_si256(mask, delta)));
            }
        }
        for (; i < b.length; ++i)
            if (l <= d[i] && d[i] < r) p[i] += value;
    }
    std::span<const Step> steps(u32 v) const {
        return {path.p + offset[v], offset[v + 1] - offset[v]};
    }
    void add_row(u32 group, u32 at, T value) {
        T *p = data.p + begin[group];
        if (length[group] <= Small) {
            p[at + 1] += value;
            return;
        }
        for (++at; at <= length[group]; at += at & -at) p[at] += value;
    }
    T range(u32 group, u32 l, u32 r) const {
        l = std::min(l, length[group]);
        r = std::min(r, length[group]);
        if (l == r) return {};
        const T *p = data.p + begin[group];
        if (length[group] <= Small) {
            p += l + 1;
            u32 count = r - l, i = 0;
            T answer{};
            if constexpr (sizeof(T) == 8) {
                auto total = _mm256_setzero_si256();
                for (; i + 4 <= count; i += 4)
                    total = _mm256_add_epi64(total, _mm256_loadu_si256((const __m256i *)(p + i)));
                auto x = _mm_add_epi64(_mm256_castsi256_si128(total),
                                       _mm256_extracti128_si256(total, 1));
                answer = T(_mm_cvtsi128_si64(_mm_add_epi64(x, _mm_srli_si128(x, 8))));
            }
            for (; i < count; ++i) answer += p[i];
            return answer;
        }
        u32 common = l & (~0u << std::bit_width(l ^ r));
        T a{}, b{};
        for (; l != common; l &= l - 1) a += p[l];
        for (; r != common; r &= r - 1) b += p[r];
        return b - a;
    }
    void difference(u32 group, u32 l, u32 r, T value) {
        l = std::min(l, length[group]);
        r = std::min(r, length[group]);
        if (l == r) return;
        T *p = data.p + begin[group];
        if (length[group] <= Small) {
            p += l + 1;
            u32 count = r - l, i = 0;
            if constexpr (sizeof(T) == 8) {
                auto delta = _mm256_set1_epi64x(value);
                for (; i + 4 <= count; i += 4) {
                    auto *x = (__m256i *)(p + i);
                    _mm256_storeu_si256(x, _mm256_add_epi64(_mm256_loadu_si256(x), delta));
                }
            }
            for (; i < count; ++i) p[i] += value;
            return;
        }
        u32 stop = std::min(length[group] + 1, (l | ((1u << std::bit_width(l ^ r)) - 1)) + 1);
        for (++l; l < stop; l += l & -l) p[l] += value;
        for (++r; r < stop; r += r & -r) p[r] -= value;
    }
    T point_row(u32 group, u32 at) const {
        return length[group] <= Small ? data[begin[group] + at + 1] : range(group, 0, at + 1);
    }
    void add(u32 vertex, T value)
        requires(!Dual)
    {
        if (which[vertex] == ~0u)
            base[vertex] += value;
        else
            leaf_values[index[vertex]] += value;
        for (auto e : steps(vertex)) {
            if (e.group >= n)
                add_row(e.group, e.distance, value);
            else if (degree[e.group] > 2) {
                add_row(e.group, e.distance, value);
                if (e.branch != ~0u) add_row(e.branch, e.distance, value);
            }
        }
    }
    T sum(u32 vertex, u32 l, u32 r) const
        requires(!Dual)
    {
        if (l >= r || l > diameter) return {};
        r = std::min(r, diameter + 1);
        T result = which[vertex] == ~0u ? T{} : leaf_sum(vertex, l, r);
        for (auto e : steps(vertex)) {
            if (e.group < n && degree[e.group] < 3) {
                if (!l) result += base[e.group];
                for (u32 b = 0; b < degree[e.group]; ++b) result += range(e.branch + b, l, r);
            } else if (e.group >= n) {
                if (l <= e.distance && e.distance < r) result += base[owner[e.group]];
                if (e.branch != ~0u && r > e.distance)
                    result += range(e.branch, l > e.distance ? l - e.distance : 0, r - e.distance);
            } else if (r > e.distance) {
                u32 lo = l > e.distance ? l - e.distance : 0, hi = r - e.distance;
                result += range(e.group, lo, hi);
                if (e.branch != ~0u) result -= range(e.branch, lo, hi);
            }
        }
        return result;
    }
    void add_range(u32 vertex, u32 l, u32 r, T value)
        requires(Dual)
    {
        if (l >= r || l > diameter) return;
        r = std::min(r, diameter + 1);
        if (which[vertex] != ~0u) leaf_apply(vertex, l, r, value);
        for (auto e : steps(vertex)) {
            if (e.group < n && degree[e.group] < 3) {
                if (!l) base[e.group] += value;
                for (u32 b = 0; b < degree[e.group]; ++b) difference(e.branch + b, l, r, value);
            } else if (e.group >= n) {
                if (l <= e.distance && e.distance < r) base[owner[e.group]] += value;
                if (e.branch != ~0u && r > e.distance)
                    difference(e.branch, l > e.distance ? l - e.distance : 0, r - e.distance,
                               value);
            } else if (r > e.distance) {
                u32 lo = l > e.distance ? l - e.distance : 0, hi = r - e.distance;
                difference(e.group, lo, hi, value);
                if (e.branch != ~0u) difference(e.branch, lo, hi, -value);
            }
        }
    }
    T get(u32 vertex) const
        requires(Dual)
    {
        T result = which[vertex] == ~0u ? base[vertex] : leaf_values[index[vertex]];
        for (auto e : steps(vertex)) {
            if (e.group >= n)
                result += point_row(e.group, e.distance);
            else if (degree[e.group] > 2) {
                result += point_row(e.group, e.distance);
                if (e.branch != ~0u) result += point_row(e.branch, e.distance);
            }
        }
        return result;
    }
};

template <class T = i64, bool Dual = false>
struct CentroidSum {
    u32 n, shape = 0, center = 0;
    T total{}, leaf_tag{};
    Buffer<T> base;
    Buffer<u32> position;
    std::optional<CentroidCore<T, Dual>> core;
    std::optional<WideFenwick> line;
    CentroidSum(const Adjacency &graph, std::span<const T> initial = {}) : n(graph.size()) {
        u32 degree = 0, leaf = 0;
        for (u32 v = 0; v < n; ++v) {
            u32 d = graph[v].size();
            if (d > degree) degree = d, center = v;
            if (d == 1) leaf = v;
        }
        if constexpr (sizeof(T) == 8) {
            if (n && degree <= 2) {
                shape = 1;
                position = Buffer<u32>(n);
                Buffer<u64> a(n);
                u32 previous = ~0u, v = leaf;
                for (u32 i = 0; i < n; ++i) {
                    position[v] = i;
                    a[i] = initial.empty() ? 0 : u64(initial[v]);
                    u32 next = ~0u;
                    for (u32 w : graph[v])
                        if (w != previous) next = w;
                    previous = v;
                    v = next;
                }
                if constexpr (Dual)
                    for (u32 i = n; i-- > 1;) a[i] -= a[i - 1];
                line.emplace(std::move(a));
                return;
            }
            if (n > 2 && degree == n - 1) {
                shape = 2;
                base = Buffer<T>(n);
                for (u32 v = 0; v < n; ++v) total += base[v] = initial.empty() ? T{} : initial[v];
                return;
            }
        }
        core.emplace(graph, initial);
    }
    template <class F>
    void intervals(u32 v, u32 l, u32 r, F visit) const {
        if (l >= r || l >= n) return;
        r = std::min(r, n);
        u32 p = position[v], a = p + 1 > r ? p + 1 - r : 0, b = p + 1 > l ? p + 1 - l : 0,
            c = std::min<usize>(n, usize(p) + l), d = std::min<usize>(n, usize(p) + r);
        if (!l)
            visit(a, d);
        else {
            if (a < b) visit(a, b);
            if (c < d) visit(c, d);
        }
    }
    void add(u32 v, T x)
        requires(!Dual)
    {
        if (shape == 1)
            line->add(position[v], u64(x));
        else if (shape == 2)
            base[v] += x, total += x;
        else
            core->add(v, x);
    }
    T sum(u32 v, u32 l, u32 r) const
        requires(!Dual)
    {
        if (l >= r) return {};
        if (shape == 1) {
            u64 answer = 0;
            intervals(v, l, r, [&](u32 a, u32 b) { answer += line->sum(a, b); });
            return T(answer);
        }
        if (shape == 2) {
            T answer = l == 0 ? base[v] : T{};
            if (l <= 1 && 1 < r) answer += v == center ? total - base[v] : base[center];
            if (v != center && l <= 2 && 2 < r) answer += total - base[v] - base[center];
            return answer;
        }
        return core->sum(v, l, r);
    }
    void add_range(u32 v, u32 l, u32 r, T x)
        requires(Dual)
    {
        if (l >= r) return;
        if (shape == 1)
            intervals(v, l, r, [&](u32 a, u32 b) { line->add_difference(a, b, u64(x)); });
        else if (shape == 2) {
            if (!l) base[v] += x;
            if (l <= 1 && 1 < r) {
                if (v == center)
                    leaf_tag += x;
                else
                    base[center] += x;
            }
            if (v != center && l <= 2 && 2 < r) leaf_tag += x, base[v] -= x;
        } else
            core->add_range(v, l, r, x);
    }
    T get(u32 v) const
        requires(Dual)
    {
        if (shape == 1) return T(line->prefix(position[v] + 1));
        if (shape == 2) return base[v] + (v == center ? T{} : leaf_tag);
        return core->get(v);
    }
};
} // namespace toy
