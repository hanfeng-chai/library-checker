#pragma once
#include <toy/buffer.h>
namespace toy {
// Sparse alphabet rows: a bit mask gives the sorted child's rank. Rows grow
// geometrically in one arena; node IDs stay fixed and no per-node allocation.
struct AlphabetTrie {
    struct Row{u32 mask=0,offset=0;};Buffer<Row>row;Buffer<u32>child;
    explicit AlphabetTrie(u32 capacity):row(0,capacity),child(0,2*capacity){}
    u32 node(){u32 id=row.n++;row[id]={};return id;}
    u32 clone(u32 from){u32 id=node();auto old=row[from];u32 size=std::popcount(old.mask);if(size){u32 capacity=std::bit_ceil(size);if(child.n+capacity>child.capacity)child.reserve(std::max<usize>(child.n+capacity,2*child.capacity));u32 offset=child.n;child.n+=capacity;memcpy(child.p+offset,child.p+old.offset,size*4);row[id]={old.mask,offset};}return id;}
    u32 get(u32 v,u32 c)const{auto r=row[v];u32 bit=1u<<c;return r.mask&bit?child[r.offset+std::popcount(r.mask&(bit-1))]:0;}
    void set(u32 v,u32 c,u32 target){auto&r=row[v];u32 bit=1u<<c,at=std::popcount(r.mask&(bit-1));if(r.mask&bit){child[r.offset+at]=target;return;}u32 size=std::popcount(r.mask);
        if(!size||std::has_single_bit(size)){u32 capacity=std::max(1u,2*size);if(child.n+capacity>child.capacity)child.reserve(std::max<usize>(child.n+capacity,2*child.capacity));u32 next=child.n;child.n+=capacity;if(size)memcpy(child.p+next,child.p+r.offset,size*4);r.offset=next;}
        memmove(child.p+r.offset+at+1,child.p+r.offset+at,(size-at)*4);child[r.offset+at]=target;r.mask|=bit;
    }
};
}
