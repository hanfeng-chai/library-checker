#pragma once
#include <toy/buckets.h>
#include <toy/radix_sort.h>
namespace toy {
struct PrefixPointCounter {
    Buffer<u32> direct,lazy;
    explicit PrefixPointCounter(u32 n):direct(n),lazy((n+255)/256){std::fill(direct.p,direct.p+direct.n,0u);std::fill(lazy.p,lazy.p+lazy.n,0u);}
    u32 get(u32 i)const{return direct[i]+lazy[i>>8];}
    void increment(u32 end){u32 block=end>>8;for(u32 i=0;i<block;++i)++lazy[i];for(u32 i=block*256;i<=end;++i)++direct[i];}
    u64 sum(std::span<const u32> ranks)const{
        auto a=_mm256_setzero_si256(),b=a,z=a;usize i=0;
        for(;i+8<=ranks.size();i+=8){auto index=_mm256_loadu_si256((const __m256i*)(ranks.data()+i));auto values=_mm256_add_epi32(_mm256_i32gather_epi32((const int*)direct.p,index,4),_mm256_i32gather_epi32((const int*)lazy.p,_mm256_srli_epi32(index,8),4));a=_mm256_add_epi64(a,_mm256_unpacklo_epi32(values,z));b=_mm256_add_epi64(b,_mm256_unpackhi_epi32(values,z));}
        a=_mm256_add_epi64(a,b);auto x=_mm_add_epi64(_mm256_castsi256_si128(a),_mm256_extracti128_si256(a,1));u64 total=_mm_cvtsi128_si64(_mm_add_epi64(x,_mm_srli_si128(x,8)));for(;i<ranks.size();++i)total+=get(ranks[i]);return total;
    }
};
struct InversionRange{u32 left,right;};
inline Buffer<u64> range_inversions(std::span<const u32> input,std::span<const InversionRange> ranges){
    u32 n=input.size();struct Item{u32 value,index;};Buffer<Item> ordered(n);for(u32 i=0;i<n;++i)ordered[i]={input[i],i};radix_sort(std::span(ordered.p,ordered.n),[](Item x){return x.value;});Buffer<u32> rank(n);for(u32 i=0;i<n;++i)rank[ordered[i].index]=i;
    struct Query{u32 left,right,id;};Buffer<Query> queries(0,ranges.size());Buffer<u64> answers(ranges.size());std::fill(answers.p,answers.p+answers.n,u64(0));
    for(u32 i=0;i<ranges.size();++i)if(ranges[i].left<ranges[i].right)queries[queries.n++]={ranges[i].left,ranges[i].right-1,i};
    radix_sort<64>(std::span(queries.p,queries.n),[](Query x){u32 block=x.left>>9;return (u64(block)<<32)|(block&1?~x.right:x.right);});
    struct Event{u32 sweep,left,right,id; i32 sign;u32 length_term;};Buffer<Event> events(0,4*queries.n);i32 left=0,right=0;
    auto event=[&](i32 sweep,u32 l,u32 r,i32 sign,u32 length,u32 id){if(sweep>=0)events[events.n++]={u32(sweep),l,r,id,sign,length};};
    for(auto q:std::span(queries.p,queries.n)){
        if(right<i32(q.right)){event(left-1,right+1,q.right,1,0,q.id);right=q.right;}
        if(i32(q.left)<left){event(right,q.left,left-1,1,1,q.id);left=q.left;}
        if(i32(q.right)<right){event(left-1,q.right+1,right,-1,0,q.id);right=q.right;}
        if(left<i32(q.left)){event(right,left,q.left-1,-1,1,q.id);left=q.left;}
    }
    Buckets grouped(n,std::move(events),[](Event e){return e.sweep;});PrefixPointCounter counter(n);Buffer<i64> prefix(n),delta(ranges.size());std::fill(delta.p,delta.p+delta.n,i64(0));
    for(u32 sweep=0;sweep<n;++sweep){prefix[sweep]=counter.get(rank[sweep]);counter.increment(rank[sweep]);
        for(auto e:grouped[sweep]){i64 length=e.right-e.left+1; i64 contribution=e.length_term*((i64(sweep)+1)*length-i64(e.left+e.right)*length/2)-i64(counter.sum({rank.p+e.left,usize(length)}));delta[e.id]+=e.sign*contribution;}}
    for(u32 i=1;i<n;++i)prefix[i]+=prefix[i-1];i64 correction=0;
    for(auto q:std::span(queries.p,queries.n)){correction+=delta[q.id];answers[q.id]=correction+prefix[q.right]-(q.left?prefix[q.left-1]:0);}return answers;
}
}
