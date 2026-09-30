#pragma once
#include <toy/radix_sort.h>
#include <toy/hash.h>
namespace toy {
struct StaticRangeMode {
    struct Mode{u32 value=0,count=0;};
    Buffer<u32> values,ids,positions,start,rank;
    Buffer<Mode> modes;
    Buffer<u32> prefix,layers;
    u32 block,blocks,shift=0,stride=0,maximum=0,strategy=0;
    explicit StaticRangeMode(span<const u32> input,u32 queries=0):values(input.size()),ids(input.size()),positions(input.size()),rank(input.size()){
        bool sorted_needed=false;values.n=0;
        {HashMap<u64,u32> dictionary(1024);
            for(u32 i=0;i<input.size();++i){u32& entry=dictionary[u64(input[i])];if(!entry){values[values.n++]=input[i];entry=values.n;}ids[i]=entry-1;if(values.n>1024){sorted_needed=true;break;}}
        }
        if(sorted_needed){
            struct Item{u32 value,index;};Buffer<Item> sorted(input.size());for(u32 i=0;i<input.size();++i)sorted[i]={input[i],i};radix_sort(span(sorted.p,sorted.n),[](Item x){return x.value;});
            values.n=0;for(u32 i=0;i<sorted.n;++i){auto x=sorted[i];if(!i||x.value!=sorted[i-1].value)values[values.n++]=x.value;ids[x.index]=values.n-1;}
        }
        u32 root=max<u32>(1,input.size()/sqrt(double(max<usize>(1,queries?queries:input.size()))));block=values.n<=64?bit_ceil(root):bit_floor(root);blocks=(input.size()+block-1)/block;shift=countr_zero(block);
        start=Buffer<u32>(values.n+1);fill(start.p,start.p+start.n,0u);for(u32 id:span(ids.p,ids.n))++start[id+1];for(u32 i=1;i<start.n;++i)start[i]+=start[i-1];Buffer<u32> next(values.n);if(values.n)memcpy(next.p,start.p,4*values.n);
        for(u32 i=0;i<input.size();++i){rank[i]=next[ids[i]]++;positions[rank[i]]=i;}
        for(u32 id=0;id<values.n;++id)maximum=max(maximum,start[id+1]-start[id]);
        if(values.n<=1||maximum<=1)return;
        if(values.n==2){strategy=3;prefix=Buffer<u32>(input.size()+1);prefix[0]=0;for(u32 i=0;i<input.size();++i)prefix[i+1]=prefix[i]+(ids[i]==0);return;}
        if(values.n<=1024){
            strategy=1;block=values.n<=8?1:32;shift=countr_zero(block);stride=(values.n+7)&-usize(8);prefix=Buffer<u32>(((input.size()>>shift)+1)*stride);fill(prefix.p,prefix.p+stride,0u);
            Buffer<u32> count(stride);fill(count.p,count.p+stride,0u);
            for(u32 i=0;i<input.size();++i){++count[ids[i]];if(((i+1)&(block-1))==0)memcpy(prefix.p+usize((i+1)>>shift)*stride,count.p,4*stride);}return;
        }
        if(maximum<=128){
            strategy=2;stride=maximum-1;layers=Buffer<u32>(input.size()*stride);Buffer<u32> best(stride);fill(best.p,best.p+stride,u32(input.size()));
            for(u32 i=input.size();i--;){u32 at=rank[i],available=start[ids[i]+1]-at-1;for(u32 j=0;j<available;++j)best[j]=min(best[j],positions[at+j+1]);memcpy(layers.p+usize(i)*stride,best.p,4*stride);}return;
        }
        modes=Buffer<Mode>(usize(blocks)*blocks);
        Buffer<u32> frequency(values.n);
        for(u32 l=0;l<blocks;++l){fill(frequency.p,frequency.p+frequency.n,0u);Mode best;
            for(u32 r=l;r<blocks;++r){for(u32 i=r*block,end=min<usize>((r+1)*block,input.size());i<end;++i){u32 id=ids[i],count=++frequency[id];if(count>best.count)best={id,count};}modes[l*blocks+r]=best;}}
    }
    Mode dense(u32 l,u32 r)const{
        alignas(32) u32 counts[1024];Mode best;u32 a=(l+block-1)>>shift,b=r>>shift;
        if(a<b){
            auto maximum=_mm256_setzero_si256(),winner=maximum;auto index=_mm256_setr_epi32(0,1,2,3,4,5,6,7);
            for(u32 j=0;j<stride;j+=8){auto x=_mm256_sub_epi32(_mm256_load_si256((const __m256i*)(prefix.p+usize(b)*stride+j)),_mm256_load_si256((const __m256i*)(prefix.p+usize(a)*stride+j)));_mm256_store_si256((__m256i*)(counts+j),x);auto mask=_mm256_cmpgt_epi32(x,maximum);maximum=_mm256_max_epi32(maximum,x);winner=_mm256_blendv_epi8(winner,index,mask);index=_mm256_add_epi32(index,_mm256_set1_epi32(8));}
            auto reduced=_mm_max_epi32(_mm256_castsi256_si128(maximum),_mm256_extracti128_si256(maximum,1));reduced=_mm_max_epi32(reduced,_mm_shuffle_epi32(reduced,0x4e));reduced=_mm_max_epi32(reduced,_mm_shuffle_epi32(reduced,0xb1));best.count=_mm_cvtsi128_si32(reduced);
            u32 mask=_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpeq_epi32(maximum,_mm256_set1_epi32(best.count))));alignas(32) u32 winners[8];_mm256_store_si256((__m256i*)winners,winner);best.value=winners[countr_zero(mask)];
        }else fill(counts,counts+stride,0u);
        auto add=[&](u32 i){u32 id=ids[i],count=++counts[id];if(count>best.count)best={id,count};};
        u32 end=min(a*block,r);for(u32 i=l;i<end;++i)add(i);for(u32 i=max(b*block,end);i<r;++i)add(i);return best;
    }
    pair<u32,u32> query(u32 l,u32 r)const{
        if(values.n==1)return {values[0],r-l};if(maximum==1)return {values[ids[l]],1};
        if(strategy==3){u32 first=prefix[r]-prefix[l],second=r-l-first;return first>=second?pair(values[0],first):pair(values[1],second);}
        if(strategy==1){auto best=dense(l,r);return {values[best.value],best.count};}
        if(strategy==2){Mode best{ids[l],1};u32 lo=1,hi=min(maximum,r-l)+1;const u32* row=layers.p+usize(l)*stride;
            while(lo+1<hi){u32 mid=(lo+hi)/2,end=row[mid-2];if(end<r){lo=mid;best={ids[end],mid};}else hi=mid;}return {values[best.value],best.count};}

        u32 a=(l+block-1)>>shift,b=r>>shift;Mode best;if(a<b)best=modes[a*blocks+b-1];u32 end=min(a*block,r);
        for(u32 i=l;i<end;++i){u32 id=ids[i],at=rank[i],limit=start[id+1];if(at+best.count<limit&&positions[at+best.count]<r){u32 count=best.count;
            do{++count;}while(at+count<limit&&positions[at+count]<r);best={id,count};if(2*count>=r-l)return {values[id],count};}}
        for(u32 i=r,begin=max(b*block,end);i-->begin;){u32 id=ids[i],at=rank[i],limit=at-start[id];if(best.count<=limit&&positions[at-best.count]>=l){u32 count=best.count;
            do{++count;}while(count<=limit&&positions[at-count]>=l);best={id,count};if(2*count>=r-l)return {values[id],count};}}
        return {values[best.value],best.count};
    }
};
}
