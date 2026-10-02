#pragma once
#include <toy/mod.h>
#include <toy/page_buffer.h>
namespace toy {
struct MatrixEntry{u32 row,column,value;};
// Exact sparse Gaussian elimination. A minimum-degree row and then column
// reduce fill-in; row/column bitmaps enumerate only entries that can change.
template<u32 P=998244353>u32 sparse_determinant(u32 n,std::span<const MatrixEntry>entries){using M=Mod<P>;if(!n)return 1;Buffer<u32>rd(n),cd(n),mapping(n),weight(n);std::fill(rd.p,rd.p+n,0u);std::fill(cd.p,cd.p+n,0u);for(auto e:entries)if(e.value){++rd[e.row];++cd[e.column];mapping[e.row]=e.column;weight[e.row]=e.value;}for(u32 i=0;i<n;++i)if(!rd[i]||!cd[i])return 0;
    if(std::all_of(rd.p,rd.p+n,[](u32 d){return d==1;})){Buffer<u8>seen(n);std::fill(seen.p,seen.p+n,0);u32 result=1,cycles=0;for(u32 i=0;i<n;++i){result=M::mul(result,weight[i]);if(!seen[i]){++cycles;for(u32 j=i;!seen[j];j=mapping[j])seen[j]=1;}}return (n-cycles)%2?M::sub(0,result):result;}
    auto a=page_buffer<u32>(usize(n)*n);std::fill(a.p,a.p+a.n,0u);u32 words=(n+63)/64;Buffer<u64>rows(usize(n)*words),cols(usize(n)*words);std::fill(rows.p,rows.p+rows.n,0ull);std::fill(cols.p,cols.p+cols.n,0ull);for(auto e:entries)if(e.value){a[usize(e.row)*n+e.column]=e.value;rows[usize(e.row)*words+e.column/64]|=1ull<<(e.column%64);cols[usize(e.column)*words+e.row/64]|=1ull<<(e.row%64);}
    Buffer<u32>order(n),position(n),col_order(n),col_position(n),index(n),values(n),targets(n);std::iota(order.p,order.p+n,0u);std::iota(position.p,position.p+n,0u);std::iota(col_order.p,col_order.p+n,0u);std::iota(col_position.p,col_position.p+n,0u);u32 answer=1;
    for(u32 k=0;k<n;++k){u32 r=order[k];for(u32 i=k+1;rd[r]>1&&i<n;++i)if(rd[order[i]]<rd[r])r=order[i];if(!rd[r])return 0;u32 c=n,degree=~0u;for(u32 w=0;w<words&&degree>1;++w)for(u64 bits=rows[usize(r)*words+w];bits;bits&=bits-1){u32 j=64*w+std::countr_zero(bits);if(cd[j]<degree){degree=cd[j];c=j;}}
        auto swap=[&](Buffer<u32>&order,Buffer<u32>&position,u32 i){u32 p=position[i],old=order[k];if(p!=k){std::swap(order[p],order[k]);position[old]=p;position[i]=k;answer=M::sub(0,answer);}};swap(order,position,r);swap(col_order,col_position,c);
        u32 pivot=a[usize(r)*n+c],inverse=M::pow(pivot,P-2),count=0,affected=0;answer=M::mul(answer,pivot);
        for(u32 w=0;w<words;++w)for(u64 bits=rows[usize(r)*words+w];bits;bits&=bits-1){u32 j=64*w+std::countr_zero(bits);cols[usize(j)*words+r/64]&=~(1ull<<(r%64));--cd[j];if(j!=c){index[count]=j;values[count++]=M::mul(a[usize(r)*n+j],inverse);}}
        for(u32 w=0;w<words;++w)for(u64 bits=cols[usize(c)*words+w];bits;bits&=bits-1)targets[affected++]=64*w+std::countr_zero(bits);
        for(u32 t=0;t<affected;++t){u32 i=targets[t];u32*dst=a.p+usize(i)*n;u32 factor=dst[c],ratio=(u64(factor)<<32)/P;dst[c]=0;rows[usize(i)*words+c/64]&=~(1ull<<(c%64));--rd[i];
            for(u32 j=0;j<count;++j){u32 column=index[j],value=values[j],old=dst[column],product=u64(value)*factor-(u64(value)*ratio>>32)*P;product=std::min(product,product-P);u32 result=M::sub(old,product);dst[column]=result;
                if(bool(old)!=bool(result)){u64 bit=1ull<<(column%64),bit_row=1ull<<(i%64);if(result){rows[usize(i)*words+column/64]|=bit;cols[usize(column)*words+i/64]|=bit_row;++rd[i];++cd[column];}else{rows[usize(i)*words+column/64]&=~bit;cols[usize(column)*words+i/64]&=~bit_row;--rd[i];--cd[column];}}
            }if(!rd[i])return 0;
        }
    }return answer;
}
}
