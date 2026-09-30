#pragma once
#include <toy/buffer.h>
namespace toy {
struct SegmentBeats {
    static constexpr i64 infinity=1ll<<60;
    struct alignas(64) Node {
        i64 maximum=-infinity,second_maximum=-infinity,minimum=infinity,second_minimum=infinity,sum=0,add=0;
        u32 maximum_count=0,minimum_count=0,size=0;
    };
    Buffer<Node> tree;
    u32 capacity;
    void pull(u32 i){auto& x=tree[i];const auto& a=tree[2*i];const auto& b=tree[2*i+1];x.sum=a.sum+b.sum;
        if(a.maximum==b.maximum){x.maximum=a.maximum;x.maximum_count=a.maximum_count+b.maximum_count;x.second_maximum=max(a.second_maximum,b.second_maximum);}
        else if(a.maximum>b.maximum){x.maximum=a.maximum;x.maximum_count=a.maximum_count;x.second_maximum=max(a.second_maximum,b.maximum);}
        else{x.maximum=b.maximum;x.maximum_count=b.maximum_count;x.second_maximum=max(a.maximum,b.second_maximum);}
        if(a.minimum==b.minimum){x.minimum=a.minimum;x.minimum_count=a.minimum_count+b.minimum_count;x.second_minimum=min(a.second_minimum,b.second_minimum);}
        else if(a.minimum<b.minimum){x.minimum=a.minimum;x.minimum_count=a.minimum_count;x.second_minimum=min(a.second_minimum,b.minimum);}
        else{x.minimum=b.minimum;x.minimum_count=b.minimum_count;x.second_minimum=min(a.minimum,b.second_minimum);}
    }
    explicit SegmentBeats(span<const i64> values):capacity(bit_ceil(max<usize>(1,values.size()))){tree=Buffer<Node>(2*capacity);fill(tree.p,tree.p+tree.n,Node{});for(u32 i=0;i<values.size();++i){i64 x=values[i];tree[capacity+i]={x,-infinity,x,infinity,x,0,1,1,1};}for(u32 i=capacity;--i;){tree[i].size=tree[2*i].size+tree[2*i+1].size;pull(i);}}
    void assign(u32 i,i64 value){auto& x=tree[i];if(!x.size||(x.minimum==value&&x.maximum==value))return;x.maximum=x.minimum=value;x.second_maximum=-infinity;x.second_minimum=infinity;x.sum=value*x.size;x.add=0;x.maximum_count=x.minimum_count=x.size;}
    void lower(u32 i,i64 value){auto& x=tree[i];x.sum+=(value-x.maximum)*x.maximum_count;if(x.minimum==x.maximum){x.minimum=value;x.add=0;}else if(x.second_minimum==x.maximum)x.second_minimum=value;x.maximum=value;}
    void raise(u32 i,i64 value){auto& x=tree[i];x.sum+=(value-x.minimum)*x.minimum_count;if(x.minimum==x.maximum){x.maximum=value;x.add=0;}else if(x.second_maximum==x.minimum)x.second_maximum=value;x.minimum=value;}
    void shift(u32 i,i64 value){auto& x=tree[i];if(!x.size)return;x.sum+=value*x.size;x.maximum+=value;x.minimum+=value;if(x.second_maximum!=-infinity)x.second_maximum+=value;if(x.second_minimum!=infinity)x.second_minimum+=value;x.add+=value;}
    void push(u32 i){auto& x=tree[i];
        if(x.minimum==x.maximum){assign(2*i,x.minimum);assign(2*i+1,x.minimum);x.add=0;return;}
        if(x.add){shift(2*i,x.add);shift(2*i+1,x.add);x.add=0;}
        for(u32 child:{2*i,2*i+1}){if(tree[child].maximum>x.maximum)lower(child,x.maximum);if(tree[child].minimum<x.minimum)raise(child,x.minimum);}
    }
    template<int Type> void below(u32 i,i64 value){
        auto& x=tree[i];if constexpr(Type==2){shift(i,value);return;}
        else if constexpr(Type==0){if(x.maximum<=value)return;if(value>x.second_maximum){lower(i,value);return;}}
        else{if(x.minimum>=value)return;if(value<x.second_minimum){raise(i,value);return;}}
        push(i);below<Type>(2*i,value);below<Type>(2*i+1,value);pull(i);
    }
    template<int Type> void update_range(u32 l,u32 r,i64 value){
        l+=capacity;r+=capacity;u32 left=countr_zero(l),right=countr_zero(r),fork=bit_width(l^(r-1));
        // The bottom boundary nodes are cold; request them while visiting warm ancestors.
        for(u32 k=0;k<6;++k){u32 a=(l>>k)&-2u,b=((r-1)>>k)&-2u;
            __builtin_prefetch(tree.p+a,1,3);__builtin_prefetch(tree.p+a+1,1,3);
            __builtin_prefetch(tree.p+b,1,3);__builtin_prefetch(tree.p+b+1,1,3);}
        // Push shared ancestors once, then only the two unaligned boundary paths.
        u32 common=max(fork,min(left,right)+1);
        for(u32 k=countr_zero(capacity);k>=common;--k)push(l>>k);
        for(i32 k=i32(fork)-1;k>i32(left);--k)push(l>>k);
        for(i32 k=i32(fork)-1;k>i32(right);--k)push((r-1)>>k);
        for(u32 x=l,y=r;x<y;x>>=1,y>>=1){if(x&1)below<Type>(x++,value);if(y&1)below<Type>(--y,value);}
        for(u32 k=left+1;k<fork;++k)pull(l>>k);
        for(u32 k=right+1;k<fork;++k)pull((r-1)>>k);
        for(u32 k=common,depth=countr_zero(capacity);k<=depth;++k)pull(l>>k);
    }
    i64 query(u32 i,u32 l,u32 r,u32 first,u32 last,i64 add,i64 lower,i64 upper)const{
        const auto& x=tree[i];u32 count=min(r,last)-max(l,first);
        if(lower==upper)return lower*count;
        if(x.minimum==x.maximum)return clamp(x.minimum+add,lower,upper)*count;
        if(first<=l&&r<=last){i64 result=x.sum+add*x.size;
            if(x.maximum+add>upper)result+=(upper-x.maximum-add)*x.maximum_count;
            if(x.minimum+add<lower)result+=(lower-x.minimum-add)*x.minimum_count;return result;}
        i64 next_lower=clamp(x.minimum+add,lower,upper),next_upper=clamp(x.maximum+add,lower,upper);add+=x.add;u32 mid=(l+r)/2;
        if(last<=mid)return query(2*i,l,mid,first,last,add,next_lower,next_upper);
        if(first>=mid)return query(2*i+1,mid,r,first,last,add,next_lower,next_upper);
        return query(2*i,l,mid,first,last,add,next_lower,next_upper)+query(2*i+1,mid,r,first,last,add,next_lower,next_upper);
    }
    void chmin(u32 l,u32 r,i64 value){if(l<r&&tree[1].maximum>value)update_range<0>(l,r,value);}
    void chmax(u32 l,u32 r,i64 value){if(l<r&&tree[1].minimum<value)update_range<1>(l,r,value);}
    void add(u32 l,u32 r,i64 value){if(l<r&&value)update_range<2>(l,r,value);}
    i64 sum(u32 l,u32 r)const{return l==r?0:query(1,0,capacity,l,r,0,-infinity,infinity);}
};
}
