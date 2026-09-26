#include <iostream>
#include <vector>
#include <algorithm>
namespace __yzlf{
using std::cin;
using std::cout;
using i64=long long;
using u32=unsigned;
using u64=unsigned long long;
using idt=std::size_t;
constexpr u32 mod=998244353;
constexpr u32 mul(u32 x,u32 y){
    return u64(x)*y%mod;
}
constexpr u32 dilt(u32 x,u32 M=mod){
    return std::min(x,x+M);
}
constexpr u32 shrk(u32 x,u32 M=mod){
    return std::min(x,x-M);
}
constexpr void rfma(u32&a,u32 b,u32 c){
    a=(a+u64(b)*c)%mod;
}
constexpr void radd(u32&a,u32 b){
    a=shrk(a+b);
}
constexpr u32 qpw(u32 a,u32 b,u32 r=1){
    for(;b;b>>=1,a=mul(a,a)){
        if(b&1){
            r=mul(r,a);
        }
    }
    return r;
}
using ply=std::vector<u32>;
using mat=std::vector<ply>;
void work(){
    idt N;
    cin>>N;
    mat a(N,ply(N));
    for(auto&v:a){
        for(auto&x:v){
            cin>>x;
        }
    }
    idt N2=N/2;
    #define BS(x) (idt(1)<<(x))
    mat dp(BS(N2),ply(N));
    for(idt i=0;i<N2;++i){
        dp[BS(i)][2*i]=1;
    }
    u32 ans=0;
    for(idt w=0;w<N2;++w){
        for(idt i=BS(w);i<BS(w+1);++i){
            for(idt j=0;j<2*w+2;++j){
                if(!(i&BS(j/2))){
                    continue;
                }
                for(idt k=0;k<2*w;++k){
                    if(i&BS(k/2)){
                        continue;
                    }
                    rfma(dp[i|BS(k/2)][k^1],dp[i][j],a[j][k]);
                }
                u32 prd=mul(dp[i][j],a[j][2*w+1]);
                for(idt k=w+1;k<N2;++k){
                    radd(dp[i|BS(k)][2*k],prd);
                }
                if(i+1==BS(N2)){
                    radd(ans,prd);
                }
            }
        }
    }//why?
    // for(idt s=0;s<BS(N2);++s){
    //     cout<<"s:"<<s<<'\n';
    //     for(idt i=0;i<N;++i){
    //         cout<<dp[s][i]<<' ';
    //     }
    //     cout<<'\n';
    // }
    #undef BS
    cout<<ans;
}
}
/*
2
0 1
1 0

4
0 1 0 0
1 0 0 0
0 0 0 1
0 0 1 0
s:0
0 0 0 0
s:1
1 0 0 0
s:2
0 0 1 0
s:3
0 0 1 0

4
0 1 1 1
1 0 1 1
1 1 0 1
1 1 1 0
s:0
0 0 0 0
s:1
1 0 0 0
s:2
0 0 1 0
s:3
1 1 1 0
3
*/
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    __yzlf::work();
    return 0;
}