#pragma once
#include <toy/adjacency.h>
#include <toy/vertex_groups.h>
namespace toy {
struct ComplementComponents:VertexGroups {
    explicit ComplementComponents(const Adjacency& g):VertexGroups(g.size(),g.size()){
        u32 n=g.size();Buffer<u32>remaining(n),mark(n);std::iota(remaining.p,remaining.p+n,0u);std::fill(mark.p,mark.p+n,0u);
        while(remaining.n){u32 begin=vertex.n;add(remaining[--remaining.n]);
            // Removed vertices are charged once; retained vertices correspond
            // to actual edges, giving O(n+m) total scans of the remaining list.
            for(u32 head=begin;head<vertex.n&&remaining.n;++head){u32 v=vertex[head],stamp=v+1;
                for(u32 u:g[v])mark[u]=stamp;
                u32 kept=0;for(u32 u:std::span(remaining.p,remaining.n)){if(mark[u]==stamp)remaining[kept++]=u;else add(u);}remaining.n=kept;
            }
            finish();
        }
    }
};
}
