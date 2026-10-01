#pragma once
#include <toy/centroid.h>
#include <toy/fft_integer.h>
#include <toy/radix_sort.h>
namespace toy {
inline Buffer<u64> tree_distance_histogram(const Adjacency& graph){
    u32 n=graph.size();Buffer<u64>answer(n);std::fill(answer.p,answer.p+n,0ull);Buffer<u32>all(n),part(n);Buffer<std::array<u32,2>>order(n);IntegerFFT fft;
    auto add_product=[&](std::span<const u32> a,std::span<const u32> b){
        if(std::min(a.size(),b.size())<=32){if(a.size()>b.size())std::swap(a,b);for(u32 i=0;i<a.size();++i)if(a[i])for(u32 j=0;j<b.size()&&i+j<n;++j)answer[i+j]+=u64(a[i])*b[j];return;}
        auto values=fft(a,b);for(u32 i=1;i<std::min<usize>(n,values.n);++i)answer[i]+=values[i];
    };
    centroid_decompose(graph,[&](u32,std::span<const CentroidPoint> points,std::span<const u32> starts){
        if(points.size()==1)return;u32 branches=starts.size()-1;for(u32 b=0;b<branches;++b)order[b]={starts[b],starts[b+1]};
        // Small branches first: an accumulated long histogram never multiplies many tiny branches.
        radix_sort(std::span(order.p,branches),[](auto x){return x[1]-x[0];});u32 length=1;all[0]=1;
        for(auto[first,last]:std::span(order.p,branches)){u32 size=1;for(u32 i=first;i<last;++i)size=std::max(size,points[i].distance+1);std::fill(part.p,part.p+size,0u);for(u32 i=first;i<last;++i)++part[points[i].distance];
            add_product(std::span<const u32>(all.p,length),std::span<const u32>(part.p,size));if(size>length)std::fill(all.p+length,all.p+size,0u);for(u32 i=0;i<size;++i)all[i]+=part[i];length=std::max(length,size);}
    });
    return answer;
}
}
