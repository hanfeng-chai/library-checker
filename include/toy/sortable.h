#pragma once
#include <toy/bitset.h>
#include <toy/segment.h>
namespace toy {
// Current keys are distinct u32; n<2^26. Blocks hold compressed binary tries.
template<class T,class Operation> struct SortableSequence {
    struct Node {T forward,backward;u32 key,packed,left,right;};
    Buffer<Node> pool;Buffer<u32> roots;Buffer<u8> reversed;Bitmap boundaries;
    SegmentTree<T,Operation> aggregate;T identity;Operation operation;u32 free=0;
    u32 count(u32 i)const{return pool[i].packed>>6;}
    int bit(u32 i)const{return int(pool[i].packed&63)-1;}
    u32 allocate(Node node){if(free){u32 i=free;free=pool[i].left;pool[i]=node;return i;}if(pool.n==pool.capacity)pool.reserve(std::max<usize>(4,2*pool.capacity));pool[pool.n]=node;return pool.n++;}
    void release(u32 i){pool[i].left=free;free=i;}
    u32 leaf(u32 key,T value){return allocate({value,value,key,1u<<6,0,0});}
    void pull(u32 i){auto& x=pool[i];auto a=pool[x.left],b=pool[x.right];x.key=a.key;x.packed=((count(x.left)+count(x.right))<<6)|(x.packed&63);x.forward=operation(a.forward,b.forward);x.backward=operation(b.backward,a.backward);}
    u32 branch(u32 k,u32 a,u32 b){if(pool[a].key>>k&1)std::swap(a,b);u32 i=allocate({identity,identity,pool[a].key,k+1,a,b});pull(i);return i;}
    u32 meld(u32 first,u32 second){
        if(!first||!second)return first|second;Node a=pool[first],b=pool[second];int difference=std::bit_width(a.key^b.key)-1,ka=bit(first),kb=bit(second);if(difference>std::max(ka,kb))return branch(difference,first,second);
        if(ka==kb){u32 l=meld(a.left,b.left),r=meld(a.right,b.right);pool[first].left=l;pool[first].right=r;release(second);}
        else if(ka>kb){if(b.key>>ka&1){u32 child=meld(a.right,second);pool[first].right=child;}else{u32 child=meld(a.left,second);pool[first].left=child;}}
        else{if(a.key>>kb&1){u32 child=meld(first,b.right);pool[second].right=child;}else{u32 child=meld(first,b.left);pool[second].left=child;}first=second;}
        pull(first);return first;
    }
    std::pair<u32,u32> split(u32 i,u32 prefix){
        if(!prefix)return {0,i};if(prefix==count(i))return {i,0};u32 l=pool[i].left,r=pool[i].right,n=count(l);
        if(prefix==n){release(i);return {l,r};}
        if(prefix<n){auto[a,b]=split(l,prefix);pool[i].left=b;pull(i);return {a,i};}
        auto[a,b]=split(r,prefix-n);pool[i].right=a;pull(i);return {i,b};
    }
    T fold_rank(u32 i,u32 l,u32 r,bool reverse)const{
        if(!l&&r==count(i))return reverse?pool[i].backward:pool[i].forward;
        u32 a=reverse?pool[i].right:pool[i].left,b=reverse?pool[i].left:pool[i].right,n=count(a);
        if(r<=n)return fold_rank(a,l,r,reverse);if(l>=n)return fold_rank(b,l-n,r-n,reverse);return operation(fold_rank(a,l,n,reverse),fold_rank(b,0,r-n,reverse));
    }
    T block(u32 i)const{return reversed[i]?pool[roots[i]].backward:pool[roots[i]].forward;}
    void split_at(u32 i){
        if(boundaries.contains(i))return;u32 begin=boundaries.previous(i),end=boundaries.next(i);boundaries.insert(i);
        auto[a,b]=split(roots[begin],reversed[begin]?end-i:i-begin);if(reversed[begin])std::swap(a,b);roots[begin]=a;roots[i]=b;reversed[i]=reversed[begin];aggregate.set(begin,block(begin));aggregate.set(i,block(i));
    }
    SortableSequence(std::span<const u32> keys,std::span<const T> values,T unit,Operation op={}):pool(1,2*keys.size()+65),roots(keys.size()),reversed(keys.size()),boundaries(keys.size()+1),aggregate(values,unit,op),identity(unit),operation(op){
        pool[0]={identity,identity,0,0,0,0};std::fill(reversed.p,reversed.p+reversed.n,u8(0));for(u32 i=0;i<keys.size();++i){roots[i]=leaf(keys[i],values[i]);boundaries.insert(i);}boundaries.insert(keys.size());
    }
    void set(u32 i,u32 key,T value){split_at(i);split_at(i+1);release(roots[i]);roots[i]=leaf(key,value);reversed[i]=0;aggregate.set(i,value);}
    void sort(u32 l,u32 r,bool reverse=false){
        if(r-l<=1)return;split_at(l);split_at(r);u32 next=boundaries.next(l+1);if(next==r&&bool(reversed[l])==reverse)return;
        while(next!=r){roots[l]=meld(roots[l],roots[next]);aggregate.set(next,identity);boundaries.erase(next);next=boundaries.next(next+1);}reversed[l]=reverse;aggregate.set(l,block(l));
    }
    T fold(u32 l,u32 r)const{
        if(l==r)return identity;u32 first=boundaries.previous(l),last=boundaries.previous(r-1);if(first==last)return fold_rank(roots[first],l-first,r-first,reversed[first]);
        T result=fold_rank(roots[first],l-first,count(roots[first]),reversed[first]);u32 middle=boundaries.next(first+1);if(middle<last)result=operation(result,aggregate.fold(middle,last));return operation(result,fold_rank(roots[last],0,r-last,reversed[last]));
    }
};
}
