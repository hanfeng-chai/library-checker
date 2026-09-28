#include <toy/io.hpp>
#include <toy/parallel_dsu.hpp>
int main(){
    constexpr toy::u32 mod=998244353;toy::Reader in(toy::direct_mapping);toy::Writer out;int n=in.read_uniform<6,int>(),q=in.read_uniform<6,int>();
    std::vector<toy::u32>x(n);for(auto&v:x)v=in.read_uniform<9,toy::u32>();toy::ParallelUnionFind<mod>dsu(x);
    while(q--){int k=in.read_uniform<6,int>(),a=in.read_uniform<6,int>(),b=in.read_uniform<6,int>();
        if(k){int level=std::bit_width((unsigned)k)-1,length=1<<level;dsu.unite(level,a,b);dsu.unite(level,a+k-length,b+k-length);}
        out.write_padded_u32(dsu.value());}
}
