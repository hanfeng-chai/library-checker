#pragma once
#include <toy/page_buffer.h>
#include <toy/string_basic.h>
#include <toy/suffix_order.h>
namespace toy {
// SA-IS induced sorting, following the CC0 ac-library implementation.
template <class T, bool Fast = false>
Buffer<u32> suffix_array(std::span<const T> s, u32 upper) {
    u32 n = s.size();
    auto sa = page_buffer<u32>(n);
    if (n < 16) {
        std::iota(sa.p, sa.p + n, 0u);
        std::sort(sa.p, sa.p + n, [&](u32 a, u32 b) {
            while (a < n && b < n && s[a] == s[b]) {
                ++a;
                ++b;
            }
            return b < n && (a == n || s[a] < s[b]);
        });
        return sa;
    }
    if constexpr (Fast && sizeof(T) == 1)
        if (suffix_detail::periodic_order({(const char *)s.data(), n}, sa)) return sa;
    // A verified alternating strict minimum is a removable separator.
    if constexpr (Fast)
        if (n >= 256 && s[0] != s[1]) {
            u32 parity = s[0] < s[1] ? 0 : 1, offset = 1 - parity;
            T separator = s[parity];
            bool valid = true;
            for (u32 i = 0; i < n; ++i)
                if ((i % 2 == parity) ? s[i] != separator : s[i] <= separator) {
                    valid = false;
                    break;
                }
            if (valid) {
                u32 m = (n + 1 - offset) / 2;
                Buffer<T> text(m);
                for (u32 i = 0; i < m; ++i) text[i] = s[2 * i + offset];
                auto order = suffix_array<T, true>(text, upper);
                u32 at = 0;
                if ((n - 1) % 2 == parity) sa[at++] = n - 1;
                for (u32 i : std::span(order.p, order.n)) {
                    u32 p = 2 * i + offset;
                    if (p) sa[at++] = p - 1;
                }
                for (u32 i : std::span(order.p, order.n)) sa[at++] = 2 * i + offset;
                return sa;
            }
        }
    Buffer<u32> left(upper + 2), right(upper + 2), cursor(upper + 2), lms(0, n / 2);
    std::fill(right.p, right.p + right.n, 0u);
    u32 *end = lms.p + lms.capacity, *begin = end;
    bool type = false;
    ++right[u32(s[n - 1])];
    for (u32 i = n - 1; i--;) {
        bool previous = s[i] == s[i + 1] ? type : s[i] < s[i + 1];
        if (type && !previous) *--begin = i + 1;
        type = previous;
        ++right[u32(s[i])];
    }
    lms.n = end - begin;
    memmove(lms.p, begin, lms.n * 4);
    u32 total = 0, alphabet_size = 0, small = 0, large = 0;
    for (u32 c = 0; c <= upper; ++c) {
        if (right[c]) {
            if (!alphabet_size) small = c;
            large = c;
            ++alphabet_size;
        }
        left[c] = total;
        total += right[c];
        right[c] = total;
    }
    // Sign encodes the predecessor's type: positive induces L, negative S.
    // This removes random accesses to the type array in both induction scans.
    auto induce = [&](std::span<const u32> order) {
        // With two symbols, the two buckets are queues: induce large-symbol
        // predecessors forwards, then small-symbol predecessors backwards.
        if (alphabet_size == 2) {
            u32 tail = n, head = 0;
            while (tail && s[tail - 1] == T(small)) sa[head++] = --tail;
            u32 count = right[small], write = count;
            if (tail) sa[write++] = tail - 1;
            for (u32 p : order) sa[write++] = p - 1;
            for (u32 read = count; read < write; ++read) {
                u32 p = sa[read];
                if (p && s[p - 1] == T(large)) sa[write++] = p - 1;
            }
            write = count;
            for (u32 read = n; read > count;) {
                u32 p = sa[--read];
                if (p && s[p - 1] == T(small)) sa[--write] = p - 1;
            }
            for (u32 read = count; read > head;) {
                u32 p = sa[--read];
                if (p && s[p - 1] == T(small)) sa[--write] = p - 1;
            }
            return;
        }
        auto a = (i32 *)sa.p;
        std::fill(a, a + n, 0);
        memcpy(cursor.p, right.p, right.n * 4);
        for (u32 i = order.size(); i--;) {
            u32 v = order[i];
            a[--cursor[u32(s[v])]] = v + 1;
        }
        memcpy(cursor.p, left.p, left.n * 4);
        a[cursor[u32(s[n - 1])]++] = s[n - 2] < s[n - 1] ? -i32(n) : i32(n);
        for (u32 i = 0; i < n; ++i)
            if (i32 p = a[i]; p > 1) {
                u32 j = p - 2, c = s[j];
                i32 v = j + 1;
                if (!j || s[j - 1] < s[j]) v = -v;
                a[cursor[c]++] = v;
            }
        memcpy(cursor.p, right.p, right.n * 4);
        for (u32 i = n; i--;) {
            i32 p = a[i];
            a[i] = std::abs(p) - 1;
            if (p < -1) {
                u32 j = -p - 2, c = s[j];
                i32 v = j + 1;
                if (j && s[j - 1] <= s[j]) v = -v;
                a[--cursor[c]] = v;
            }
        }
    };
    if constexpr (Fast && sizeof(T) == 1)
        if (suffix_detail::prefix_order({(const char *)s.data(), n}, lms)) {
            induce(lms);
            return sa;
        }
    if constexpr (Fast) {
        Buffer<u32> names(lms.n);
        if (u32 alphabet = suffix_detail::factor_names<T>(s, lms, upper, names)) {
            auto reduced = suffix_array<u32, true>(names, alphabet - 1);
            Buffer<u32> order(lms.n);
            for (u32 i = 0; i < lms.n; ++i) order[i] = lms[reduced[i]];
            induce(order);
            return sa;
        }
    }
    induce(lms);
    u32 m = lms.n;
    if (m) {
        Buffer<u32> index((n + 1) / 2), order(0, m), names(m);
        Buffer<u64> bitmap((n + 63) / 64);
        std::fill(bitmap.p, bitmap.p + bitmap.n, 0ull);
        for (u32 i = 0; i < m; ++i) {
            index[lms[i] / 2] = (i + 1 < m ? lms[i + 1] : n) - lms[i] + 1;
            bitmap[lms[i] / 64] |= 1ull << (lms[i] % 64);
        }
        for (u32 v : std::span(sa.p, sa.n))
            if ((bitmap[v / 64] >> (v % 64)) & 1) order.p[order.n++] = v;
        u32 alphabet = 0, previous = 0, previous_length = 0;
        for (u32 i = 0; i < m; ++i) {
            u32 p = order[i], length = index[p / 2];
            bool same = i && length == previous_length && length <= n - std::max(p, previous) &&
                        !memcmp(s.data() + p, s.data() + previous, length * sizeof(T));
            if (i && !same) ++alphabet;
            index[p / 2] = alphabet;
            previous = p;
            previous_length = length;
        }
        for (u32 i = 0; i < m; ++i) names[i] = index[lms[i] / 2];
        if (alphabet + 1 == m) {
            for (u32 i = 0; i < m; ++i) order[names[i]] = lms[i];
        } else {
            auto reduced = suffix_array<u32, Fast>(names, alphabet);
            for (u32 i = 0; i < m; ++i) order[i] = lms[reduced[i]];
        }
        induce(order);
    }
    return sa;
}
inline Buffer<u32> suffix_array(std::string_view s) {
    return suffix_array<char, true>({s.data(), s.size()}, 127);
}
// Kasai usually extends by zero or one byte after carrying the previous LCP.
[[gnu::always_inline]] inline u32 residual_lcp(const char *a, const char *b, u32 n) {
    u32 i = 0;
    for (; i < std::min(n, 4u); ++i)
        if (a[i] != b[i]) return i;
    return i + common_prefix(a + i, b + i, n - i);
}
inline Buffer<u32> lcp_array(std::string_view s, std::span<const u32> sa) {
    u32 n = s.size();
    Buffer<u32> rank(n), lcp(n ? n - 1 : 0);
    for (u32 i = 0; i < n; ++i) rank[sa[i]] = i;
    for (u32 i = 0, h = 0; i < n; ++i) {
        if (h) --h;
        if (!rank[i]) continue;
        u32 j = sa[rank[i] - 1];
        h += residual_lcp(s.data() + i + h, s.data() + j + h, n - std::max(i, j) - h);
        lcp[rank[i] - 1] = h;
    }
    return lcp;
}
inline u64 distinct_substrings(std::string_view s) {
    u32 n = s.size();
    if (n < 2 || common_prefix(s.data(), s.data() + 1, n - 1) == n - 1) return n;
    auto sa = suffix_array(s);
    Buffer<u32> next(n);
    for (u32 i = 1; i < n; ++i) next[sa[i - 1]] = sa[i];
    next[sa[n - 1]] = n;
    u64 answer = u64(n) * (n + 1) / 2;
    for (u32 i = 0, h = 0; i < n; ++i) {
        u32 j = next[i];
        if (j == n) {
            h = 0;
            continue;
        }
        h += residual_lcp(s.data() + i + h, s.data() + j + h, n - std::max(i, j) - h);
        answer -= h;
        if (h) --h;
    }
    return answer;
}

} // namespace toy
