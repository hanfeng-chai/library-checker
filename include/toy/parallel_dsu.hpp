#pragma once
#include <bits/extc++.h>
namespace toy {
template<uint32_t Mod>
class ParallelUnionFind {
    int levels;
    std::vector<std::size_t> offsets;
    std::vector<int> parents;
    std::vector<uint32_t> component_sum;
    uint32_t answer=0;
    static int leader(int* parent,int x){
        return parent[x]<0?x:parent[x]=leader(parent,parent[x]);
    }
public:
    explicit ParallelUnionFind(const std::vector<uint32_t>& values)
        :levels(std::bit_width(values.size())),offsets(levels+1),component_sum(values){
        for(int level=0;level<levels;++level)
            offsets[level+1]=offsets[level]+values.size()-(1u<<level)+1;
        parents.assign(offsets.back(),-1);
    }
    bool unite(int level,int a,int b){
        int* parent=parents.data()+offsets[level];
        int first=a,second=b;
        a=leader(parent,a);b=leader(parent,b);if(a==b)return false;
        if(parent[a]<parent[b]){
            parent[a]+=parent[b];parent[b]=a;
        }else{
            parent[b]+=parent[a];parent[a]=b;std::swap(a,b);
        }
        if(level==0){
            answer=(answer+(uint64_t)component_sum[a]*component_sum[b])%Mod;
            component_sum[a]+=component_sum[b];if(component_sum[a]>=Mod)component_sum[a]-=Mod;
        }else{
            int half=1<<(level-1);unite(level-1,first,second);unite(level-1,first+half,second+half);
        }
        return true;
    }
    uint32_t value()const{return answer;}
};
}
