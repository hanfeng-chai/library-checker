#pragma once
#include <toy/buffer.h>
namespace toy {
template<class T,class Operation> struct SegmentTree {
    u32 capacity;T identity;Operation operation;Buffer<T> tree;
    SegmentTree(span<const T> values,T unit,Operation op={}):capacity(bit_ceil(max<usize>(1,values.size()))),identity(unit),operation(op),tree(2*capacity){fill(tree.p,tree.p+tree.n,identity);if(!values.empty())memcpy(tree.p+capacity,values.data(),values.size_bytes());for(u32 i=capacity;--i;)tree[i]=operation(tree[2*i],tree[2*i+1]);}
    void set(u32 i,T value){tree[i+=capacity]=value;while(i>>=1)tree[i]=operation(tree[2*i],tree[2*i+1]);}
    T fold(u32 l,u32 r)const{T left=identity,right=identity;for(l+=capacity,r+=capacity;l<r;l>>=1,r>>=1){if(l&1)left=operation(left,tree[l++]);if(r&1)right=operation(tree[--r],right);}return operation(left,right);}
};
}
