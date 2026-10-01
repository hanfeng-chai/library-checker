#pragma once
#include <toy/rmq.h>
namespace toy {
struct IntervalDecomposition {
    struct Node{u32 left=0,right=0,low=0,high=0,first=0,last=0,next=0;bool linear=true;};
    Buffer<Node> nodes;u32 root=0;
    u32 create(Node node){u32 id=nodes.n++;nodes[id]=node;return id;}
    void append(u32 parent,u32 child){auto& p=nodes[parent];if(p.last)nodes[p.last].next=child;else p.first=child;p.last=child;}
    explicit IntervalDecomposition(std::span<const u32> values):nodes(1,2*values.size()+1){
        u32 n=values.size();nodes[0]={};if(!n)return;Buffer<u32>inverse(n),end(n),id(n);
        for(u32 i=0;i<n;++i){inverse[values[i]]=i;end[i]=i+1;id[i]=create({i,i+1,values[i],values[i]+1});}
        RMQ position(std::move(inverse));
        auto merge=[&](u32 l,u32 r,u32 low){u32 m=end[l],a=id[l];
            if(end[m]==r){u32 b=id[m];bool increasing=values[l]<values[r-1];
                if(nodes[a].first&&nodes[a].linear&&(values[l]<values[m-1])==increasing){append(a,b);nodes[a].right=r;nodes[a].low=low;nodes[a].high=low+r-l;}
                else{u32 x=create({l,r,low,low+r-l});append(x,a);append(x,b);id[l]=x;}
            }else{u32 x=create({l,r,low,low+r-l,0,0,0,false});for(u32 p=l;p<r;p=end[p])append(x,id[p]);id[l]=x;}
            end[l]=r;
        };
        struct Base{u32 left,low,high;};Buffer<Base>stack(n);u32 count=0;
        for(u32 r=1;r<=n;++r){Base current{r-1,values[r-1],values[r-1]+1};
            while(count){auto& top=stack[count-1];top.low=std::min(top.low,current.low);top.high=std::max(top.high,current.high);Base next=top;
                // A smaller inverse position invalidates this left boundary forever.
                if(position.min(next.low,next.high)<next.left){--count;auto& previous=stack[count-1];previous.low=std::min(previous.low,next.low);previous.high=std::max(previous.high,next.high);}
                else if(next.high-next.low==r-next.left){current=next;--count;merge(next.left,r,next.low);}
                else break;
            }
            stack[count++]=current;
        }
        while(count>1){auto next=stack[--count];auto& top=stack[count-1];top.low=std::min(top.low,next.low);top.high=std::max(top.high,next.high);if(top.high-top.low==n-top.left)merge(top.left,n,top.low);}
        root=id[0];
    }
};
}
