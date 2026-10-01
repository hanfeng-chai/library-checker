#pragma once
#include <toy/line.h>
#include <toy/radix_sort.h>
namespace toy {
// Lower envelope in decreasing slope order; breakpoints increase with x.
struct LineHull {
    Buffer<Line> lines;
    explicit LineHull(Buffer<Line> input) : lines(std::move(input)) {
        radix_sort(std::span(lines.p,lines.n),[](Line f){return ~(u32(f.slope)^0x80000000u);});usize used=0;
        for(usize i=0;i<lines.n;++i){Line f=lines[i];
            if(used&&lines[used-1].slope==f.slope){if(lines[used-1].intercept<=f.intercept)continue;--used;}
            while(used>1){auto a=lines[used-2],b=lines[used-1];
                if((i128(b.intercept)-a.intercept)*(i64(b.slope)-f.slope)<(i128(f.intercept)-b.intercept)*(i64(a.slope)-b.slope))break;--used;}
            lines[used++]=f;
        }
        lines.n=used;
    }
    i64 minimum(i32 x)const{
        if(!lines.n)return Line::infinity;usize l=0,r=lines.n-1;
        while(l<r){usize m=(l+r)/2;if(lines[m](x)<lines[m+1](x))r=m;else l=m+1;}return lines[l](x);
    }
    void evaluate(std::span<const i32> sorted_x,std::span<i64> output)const{
        if(!lines.n){std::fill(output.begin(),output.end(),Line::infinity);return;}usize j=0;
        for(usize i=0;i<sorted_x.size();++i){i32 x=sorted_x[i];while(j+1<lines.n&&lines[j+1](x)<=lines[j](x))++j;output[i]=lines[j](x);}
    }
};
}
