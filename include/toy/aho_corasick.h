#pragma once
#include <toy/alphabet_trie.h>
namespace toy {
struct AhoCorasick {
    AlphabetTrie trie;
    Buffer<u32> parent, suffix;
    explicit AhoCorasick(u32 capacity) : trie(capacity), parent(capacity), suffix(capacity) {
        trie.node();
        parent[0] = suffix[0] = 0;
    }
    u32 add(std::string_view s) {
        u32 v = 0;
        for (char x : s) {
            u32 c = x - 'a', next = trie.get(v, c);
            if (!next) {
                next = trie.node();
                parent[next] = v;
                trie.set(v, c, next);
            }
            v = next;
        }
        return v;
    }
    u32 step(u32 v, u32 c) const {
        u32 next;
        while (v && !(next = trie.get(v, c))) v = suffix[v];
        return trie.get(v, c);
    }
    void build() {
        Buffer<u32> queue(trie.row.n);
        u32 head = 0, tail = 1;
        queue[0] = 0;
        while (head < tail) {
            u32 v = queue[head++];
            auto r = trie.row[v];
            u32 offset = 0;
            for (u32 bits = r.mask; bits; bits &= bits - 1) {
                u32 c = std::countr_zero(bits), u = trie.child[r.offset + offset++];
                suffix[u] = v ? step(suffix[v], c) : 0;
                queue[tail++] = u;
            }
        }
    }
    u32 size() const { return trie.row.n; }
};
} // namespace toy
