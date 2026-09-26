#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
namespace __yzlf{
using std::cin;
using std::cout;
using i64=long long;
using u32=unsigned;
using u64=unsigned long long;
using idt=std::size_t;
constexpr u32 mod=998244353;
constexpr u32 dilt(u32 x,u32 M=mod){
    return std::min(x,x+M);
}
constexpr u32 shrk(u32 x,u32 M=mod){
    return std::min(x,x-M);
}
struct Brt{
    Brt()=default;
    Brt(u64 m){setmod(m);}
    u64 operator()(u64 x)const{
        u64 y=x-(u64)((__uint128_t(x)*im)>>64)*M;
        return std::min(y,y-M);
    }
    u64 mod()const{return M;}
    void setmod(u64 P){M=P,im=u64(-1)/M;}
    private:
    u64 M,im;
};
using ply=std::vector<u32>;
u32 det(std::vector<ply> A,Brt md){
    idt n=A.size();
    u32 res=1,P=md.mod();
    for(idt i=0;i<n;++i){
        for(idt j=i+1;j<n;std::swap(A[i],A[j]),res=P-res,++j){
            for(;A[i][i];std::swap(A[i],A[j]),res=P-res){
				u64 fx=P-(A[j][i]/A[i][i]);
				for(idt k=i;k<n;++k){
					A[j][k]=md(A[j][k]+fx*A[i][k]);
				}
            }
        }
		res=md(u64(res)*A[i][i]);
		if(!res){break;}
    }
    return res;
}
void work(){
    idt n;
    u32 m;
    cin>>n>>m;
    std::vector A(n,ply(n));
    for(auto&v:A){
        for(auto&x:v){
            cin>>x;
        }
    }
    cout<<det(A,m);
}
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    __yzlf::work();
    return 0;
}