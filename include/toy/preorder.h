#pragma once
#include <toy/buffer.h>
namespace toy {
// Subtree intervals when parent[0]=0 and parent[v]<v. Children are visited in reverse id order.
struct OrderedPreorder {
    Buffer<u32> position,size;
    explicit OrderedPreorder(std::span<const u32> parent):position(parent.size()),size(parent.size()){
        u32 n=parent.size();if(!n)return;std::fill(size.p,size.p+n,1u);for(u32 v=n;--v;)size[parent[v]]+=size[v];
        position[0]=n-1;for(u32 v=1;v<n;++v){position[v]=position[parent[v]];position[parent[v]]-=size[v];}
    }
};
}
