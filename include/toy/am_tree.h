#pragma once
#include <toy/buffer.h>
namespace toy {
// AM-tree: label-preserving shortcuts and rotations maintain path maxima in a
// transformed forest. Parent links need not be original graph edges; labels do.
// Unique positive u32 weights. add returns the discarded edge ID, or -1.
struct AMForest {
    struct Node{u64 key=~0ull;u32 parent=0,size=1;};
    Buffer<Node>node;u32 components,refresh=0,maximum=~0u;
    explicit AMForest(u32 n):node(n+1),components(n){std::fill(node.p,node.p+node.n,Node{});node[0].key=node[0].size=0;}
    void promote(u32 x){auto&a=node[x];u32 y=a.parent;auto&b=node[y];b.size-=a.size;a.parent=b.parent;if(a.key<b.key){a.size+=b.size;b.parent=x;std::swap(a.key,b.key);}}
    void calibrate(u32 x){while(u32 p=node[x].parent){if(u64(node[x].size)*3<=u64(node[p].size)*2)x=p;else promote(x);}}
    void perch(u32 x,u32 stop){while(node[x].parent&&node[x].parent!=stop)promote(x);}
    u32 path_max(u32 x,u32 y){u32 best=0;while(x!=y){if(node[x].size>node[y].size)std::swap(x,y);if(!node[x].parent)return 0;if(node[x].key>node[best].key)best=x;x=node[x].parent;}return best;}
    struct Probe{u32 maximum;bool complete;};
    Probe probe(u32 x,u32 y,u64 incoming){u32 best=0,left=16;
        while(x!=y){if(!left--)return {0,false};if(node[x].size>node[y].size)std::swap(x,y);u32 p=node[x].parent;if(!p){x=y;best=0;break;}if(node[x].key>node[best].key)best=x;x=p;}
        if(best&&incoming>=node[best].key)return {best,true};while(node[x].parent){if(!left--)return {0,false};x=node[x].parent;}return {best,true};
    }
    i32 perch_link(u32 u,u32 v,u64 key,u32 id){perch(v,0);perch(u,v);
        if(node[u].parent==v){if(key>=node[u].key)return id;i32 removed=u32(node[u].key)-1;node[u].key=key;return removed;}
        if(node[u].size>node[v].size)std::swap(u,v);node[u].parent=v;node[u].key=key;node[v].size+=node[u].size;--components;return -1;
    }
    void adjust(u32 v,i64 delta){node[v].size=i64(node[v].size)+delta;}
    i32 add(u32 u,u32 v,u32 weight,u32 id){
        if(u==v)return id;if(components==1){if(!refresh){maximum=0;for(u32 x=1;x<node.n;++x)if(node[x].parent)maximum=std::max(maximum,u32(node[x].key>>32));refresh=node.n-1;}--refresh;if(weight>=maximum)return id;}
        ++u;++v;u64 key=(u64(weight)<<32)|(id+1);auto found=probe(u,v,key);
        if(!found.complete){calibrate(u);calibrate(v);if(components==1)return perch_link(u,v,key,id);found.maximum=path_max(u,v);}
        i32 removed=-1;
        if(u32 x=found.maximum){if(key>=node[x].key)return id;removed=u32(node[x].key)-1;u32 size=node[x].size;for(u32 p=node[x].parent;p;p=node[p].parent)node[p].size-=size;node[x].parent=0;node[x].key=~0ull;}
        else --components;
        // Stitch the two transformed paths, propagating subtree-size deltas.
        // The bounded probe certifies short paths; calibration handles others.
        i64 du=0,dv=0;
        for(;;){while(key>=node[u].key){u=node[u].parent;adjust(u,du);}while(key>=node[v].key){v=node[v].parent;adjust(v,dv);}
            if(node[u].size>node[v].size){std::swap(u,v);std::swap(du,dv);}u32 size=node[u].size;du-=size;dv+=size;node[v].size+=size;u32 old=node[u].parent;u64 previous=node[u].key;node[u].parent=v;node[u].key=key;if(!old)break;key=previous;u=old;adjust(u,du);
        }
        for(u32 p=node[v].parent;p;p=node[p].parent)adjust(p,dv);return removed;
    }
};
}
