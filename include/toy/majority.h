#pragma once
#include <toy/buffer.h>
namespace toy {
// Pair cancellation preserves every strict majority; the candidate needs counting.
struct MajorityTree {
    struct Vote{u32 value=0,count=0;};
    usize n;
    Buffer<Vote> tree;
    static Vote merge(Vote a,Vote b){if(a.value==b.value)return {a.value,a.count+b.count};return a.count>b.count?Vote{a.value,a.count-b.count}:Vote{b.value,b.count-a.count};}
    explicit MajorityTree(span<const u32> values):n(values.size()),tree(2*max<usize>(1,n)){
        fill(tree.p,tree.p+tree.n,Vote{});for(usize i=0;i<n;++i)tree[n+i]={values[i],1};for(usize i=n;i-->1;)tree[i]=merge(tree[2*i],tree[2*i+1]);
    }
    void set(u32 i,u32 value){tree[i+=n]={value,1};while(i>>=1)tree[i]=merge(tree[2*i],tree[2*i+1]);}
    Vote candidate(u32 l,u32 r)const{Vote result;for(l+=n,r+=n;l<r;l>>=1,r>>=1){if(l&1)result=merge(result,tree[l++]);if(r&1)result=merge(result,tree[--r]);}return result;}
};
}
