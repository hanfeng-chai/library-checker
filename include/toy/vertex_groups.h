#pragma once
#include <toy/buffer.h>
namespace toy {
struct VertexGroups {
    Buffer<u32> offset, vertex;
    VertexGroups(u32 vertices, u32 groups) : offset(1, groups + 1), vertex(0, vertices) {
        offset[0] = 0;
    }
    u32 size() const { return offset.n - 1; }
    void add(u32 v) { vertex.p[vertex.n++] = v; }
    void finish() { offset.p[offset.n++] = vertex.n; }
    std::span<const u32> operator[](u32 i) const {
        return {vertex.p + offset[i], offset[i + 1] - offset[i]};
    }
};
} // namespace toy
