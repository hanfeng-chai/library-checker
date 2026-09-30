#pragma once
#include <toy/array.h>
#include <toy/buckets.h>
#include <toy/heap.h>
#include <toy/prefix_tree.h>
namespace toy {
struct PermutationLisBumping {
    u32 n,block,blocks;
    Buffer<u32> positions;
    Array<BinaryHeap<u32>> maximum;
    Array<BinaryHeap<u32,true>> pending;
    explicit PermutationLisBumping(u32 n):n(n),block(max<u32>(1,sqrt(double(n)))),blocks((n+block-1)/block),positions(n),maximum(blocks),pending(blocks){
        fill(positions.p,positions.p+n,0u);for(u32 b=0;b<blocks;++b){maximum[b].values.reserve(block);pending[b].values.reserve(block);}}
    u32 end(u32 b)const{return min(n,(b+1)*block);}
    void materialize(u32 b){if(pending[b].empty())return;for(u32 i=b*block;i<end(b);++i)if(positions[i])positions[i]=pending[b].push_pop(positions[i]);pending[b].clear();}
    void rebuild(u32 b){auto& heap=maximum[b];heap.clear();for(u32 i=b*block;i<end(b);++i)if(positions[i])heap.values[heap.values.n++]=positions[i];heap.heapify();}
    u32 insert(u32 index,u32 value){
        u32 b=value/block;materialize(b);positions[value]=index+1;maximum[b].push(index+1);u32 carry=0,i=value+1,last=end(b);
        for(;i+8<=last;i+=8){auto x=_mm256_loadu_si256((const __m256i*)(positions.p+i));if(_mm256_testz_si256(x,x))continue;
            // Each slot keeps min(old, earlier maximum), forwarding the larger value.
            auto prefix=_mm256_max_epu32(x,_mm256_slli_si256(x,4));prefix=_mm256_max_epu32(prefix,_mm256_slli_si256(prefix,8));
            auto lower=_mm_shuffle_epi32(_mm256_castsi256_si128(prefix),0xff);prefix=_mm256_max_epu32(prefix,_mm256_inserti128_si256(_mm256_setzero_si256(),lower,1));prefix=_mm256_max_epu32(prefix,_mm256_set1_epi32(carry));
            auto before=_mm256_permutevar8x32_epi32(prefix,_mm256_setr_epi32(0,0,1,2,3,4,5,6));before=_mm256_blend_epi32(before,_mm256_set1_epi32(carry),1);
            _mm256_storeu_si256((__m256i*)(positions.p+i),_mm256_min_epu32(x,before));carry=_mm256_extract_epi32(prefix,7);
        }
        for(;i<last;++i)if(carry<positions[i])swap(carry,positions[i]);if(carry)rebuild(b);
        for(++b;b<blocks;++b){auto& heap=maximum[b];if(heap.empty()||heap.top()<=carry)continue;pending[b].push(carry);carry=carry?heap.replace_top(carry):heap.pop();}
        return carry;
    }
};
struct LisRange{u32 left,right;};
inline Buffer<u32> range_lis(span<const u32> permutation,span<const LisRange> ranges){
    struct Query{u32 left,right,index;};Buffer<Query> queries(ranges.size());for(u32 i=0;i<ranges.size();++i)queries[i]={ranges[i].left,ranges[i].right,i};Buckets grouped(permutation.size()+1,std::move(queries),[](Query x){return x.right;});
    PermutationLisBumping bump(permutation.size());PrefixTree32 active(permutation.size());Buffer<u32> answer(ranges.size());fill(answer.p,answer.p+answer.n,0u);
    for(u32 i=0;i<permutation.size();++i){active.add(i,1);u32 removed=bump.insert(i,permutation[i]);if(removed)active.add(removed-1,-1);for(auto q:grouped[i+1])answer[q.index]=active.total-active.prefix(q.left);}return answer;
}
}
