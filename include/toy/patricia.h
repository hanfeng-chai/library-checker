#pragma once
#include <toy/buffer.h>

namespace toy {
// Compressed binary trie of unique u32 keys. Deleted leaves/branches are reused.
struct PatriciaSet {
    struct Node { u32 value; int child[2]; int bit; };
    Buffer<Node> nodes;
    int root = 0, free = 0;
    u32 count = 0;
    explicit PatriciaSet(usize capacity = 0) : nodes(1, 2 * capacity + 1) { nodes[0] = {0, {0,0}, -1}; }
    int allocate(Node value) {
        if (free) { int i = free; free = nodes[i].child[0]; nodes[i] = value; return i; }
        if (nodes.n == nodes.capacity) nodes.reserve(std::max<usize>(4, 2 * nodes.capacity));
        nodes[nodes.n] = value; return nodes.n++;
    }
    void recycle(int i) { nodes[i].child[0] = free; free = i; }
    u32 size() const { return count; }
    int leaf(u32 value) const {
        int node = root; while (nodes[node].bit >= 0) node = nodes[node].child[value >> nodes[node].bit & 1]; return node;
    }
    bool contains(u32 value) const { return root && nodes[leaf(value)].value == value; }
    bool insert(u32 value) {
        if (!root) { root = allocate({value,{0,0},-1}); ++count; return true; }
        int found = leaf(value); u32 difference = value ^ nodes[found].value; if (!difference) return false;
        int bit = std::bit_width(difference) - 1, parent = 0, direction = 0, node = root;
        while (nodes[node].bit > bit) { parent = node; direction = value >> nodes[node].bit & 1; node = nodes[node].child[direction]; }
        int added = allocate({value,{0,0},-1}); u32 side = value >> bit & 1;
        Node branch{0,{node,node},bit}; branch.child[side] = added; int joined = allocate(branch);
        if (parent) nodes[parent].child[direction] = joined; else root = joined;
        ++count; return true;
    }
    bool erase(u32 value) {
        if (!root) return false;
        int node = root, parent = 0, grandparent = 0, side = 0, previous_side = 0;
        while (nodes[node].bit >= 0) {
            grandparent = parent; previous_side = side; parent = node;
            side = value >> nodes[node].bit & 1; node = nodes[node].child[side];
        }
        if (nodes[node].value != value) return false;
        if (!parent) root = 0;
        else {
            int sibling = nodes[parent].child[side ^ 1];
            if (grandparent) nodes[grandparent].child[previous_side] = sibling; else root = sibling;
            recycle(parent);
        }
        recycle(node); --count; return true;
    }
    u32 min_xor(u32 value) const { return nodes[leaf(value)].value ^ value; }
};
}
