#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <tuple>
#include <cassert>
#pragma GCC target("popcnt")
namespace yzlf{
using u32=unsigned int;
using i64=long long;
using u64=unsigned long long;
using u128=__uint128_t;
using idt=std::size_t;
std::mt19937_64 rng(0xee0000+std::chrono::system_clock().now().time_since_epoch().count()+std::random_device{}());
template<class T>inline T*cpy(T*f,const T*g,idt n){return (T*)memcpy(f,g,n*sizeof(T));}
template<class T>inline T*clr(T*f,idt n){return (T*)memset(f,0,n*sizeof(T));}
using f64=double;
using cpx=std::complex<f64>;
constexpr idt bcl(idt x){
    return x<2?1:idt(2)<<std::__lg(x-1);
}
namespace __fft{
using f64=double;
using ldb=long double;
struct cpx{
    f64 x,y;
    cpx()=default;
    cpx(f64 xx,f64 yy=0.):x(xx),y(yy){}
    cpx operator+(cpx b)const{return {x+b.x,y+b.y};}
    cpx operator-(cpx b)const{return {x-b.x,y-b.y};}
    cpx operator*(cpx b)const{return {x*b.x-y*b.y,x*b.y+y*b.x};}
    //a*conj(b)
    friend cpx mulT(cpx a,cpx b){return {a.x*b.x+a.y*b.y,a.y*b.x-a.x*b.y};}
    //(a-b)*i
    friend cpx subI(cpx a,cpx b){return {b.y-a.y,a.x-b.x};}
    //mod (x^2-1)
    friend cpx mulY(cpx a,cpx b){return {a.x*b.x+a.y*b.y,a.x*b.y+a.y*b.x};}
    cpx operator-(){return {-x,-y};}
};
inline cpx Wn(ldb a){return {f64(std::cos(a)),f64(std::sin(a))};}
inline cpx conj(cpx x){return {x.x,-x.y};}
struct ffter{
    std::vector<cpx> wn;
    ffter():wn{1.}{}
    void reserve(idt l){
        idt sz=wn.size();
        if(l>sz*2){
            int t=std::__lg(l),t2=t>>1;
            idt l2=idt(1)<<t2;
            std::vector<cpx> bas(l2<<1);
            const auto p0=std::acos(-1.l)/l2,p1=p0/l2;
            for(idt i=0,j=(l2*3)>>1,p=0;i<l2;p-=l2-(j>>__builtin_ctzll(++i))){
                bas[i]=Wn(i64(p)*p0),bas[i|l2]=Wn(i64(p)*p1);
            }
            wn.resize(l>>1);
            for(idt i=sz;i<(l>>1);++i){
                wn[i]=bas[i&(l2-1)]*bas[l2|(i>>t2)];
            }
        }
    }
    void dif(cpx*f,idt n){
        idt L=n>>1;
		if(__builtin_ctzll(n)&1){
			for(idt j=0;j<L;++j){
				cpx x=f[j],y=f[j+L];
				f[j]=x+y,f[j+L]=x-y;
			}
			L>>=1;
		}
		L>>=1;
        for(idt l=L<<2;L;l=L,L>>=2){
            for(idt j=0;j<L;++j){
                cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                cpx g0=f0+f2,g1=f1+f3,g2=f0-f2,g3=subI(f1,f3);
                f[j]=g0+g1,f[j+L]=g0-g1,f[j+L*2]=g2+g3,f[j+L*3]=g2-g3;
            }
            for(idt i=l,k=1;i<n;i+=l,++k){
                auto r1=wn[k*2],r2=wn[k],r3=r1*r2;
                for(idt j=i;j<i+L;++j){
                    cpx f0=f[j],f1=f[j+L]*r1,f2=f[j+L*2]*r2,f3=f[j+L*3]*r3;
                    cpx g0=f0+f2,g1=f1+f3,g2=f0-f2,g3=subI(f1,f3);
					f[j]=g0+g1,f[j+L]=g0-g1,f[j+L*2]=g2+g3,f[j+L*3]=g2-g3;
                }
            }
        }
    }
    void dit(cpx*f,idt n){
        idt L=1;
		for(idt l=L<<2;L<(n>>1);L=l,l<<=2){
            for(idt j=0;j<L;++j){
                cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                cpx g0=f0+f1,g1=f0-f1,g2=f2+f3,g3=subI(f3,f2);
                f[j]=g0+g2,f[j+L]=g1+g3,f[j+L*2]=g0-g2,f[j+L*3]=g1-g3;
            }
            for(idt i=l,k=1;i<n;i+=l,++k){
                auto r1=wn[k*2],r2=wn[k],r3=r1*r2;
                for(idt j=i;j<i+L;++j){
                    cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                    cpx g0=f0+f1,g1=f0-f1,g2=f2+f3,g3=subI(f3,f2);
					f[j]=g0+g2,f[j+L]=mulT(g1+g3,r1),f[j+L*2]=mulT(g0-g2,r2),f[j+L*3]=mulT(g1-g3,r3);
                }
            }
        }
        if(L!=n){
			for(idt j=0;j<L;++j){
				cpx x=f[j],y=f[j+L];
				f[j]=x+y,f[j+L]=x-y;
			}
		}
    }
    void __fconv(cpx*F,cpx*G,idt lm){
        reserve(lm),dif(F,lm),dif(G,lm);
        f64 fx=1./lm,fx2=0.25*fx;
        F[0]=mulY(F[0],G[0])*fx,F[1]=F[1]*G[1]*fx;
        for(idt k=2,m=3;k<lm;k<<=1,m<<=1){
			for(idt i=k,j=i+k-1;i<m;++i,--j){
				cpx oi=F[i]+conj(F[j]),hi=F[i]-conj(F[j]);
                cpx Oi=G[i]+conj(G[j]),Hi=G[i]-conj(G[j]);
				cpx r0=oi*Oi-hi*Hi*((i&1)?-wn[i>>1]:wn[i>>1]),r1=Oi*hi+oi*Hi;
                F[i]=(r0+r1)*fx2,F[j]=conj(r0-r1)*fx2;
			}
		}
        dit(F,lm);
    }
    void fconv(f64*f,f64*g,idt lm){
        __fconv((cpx*)f,(cpx*)g,lm/2);
    }
}fft;
}//__fft
struct WildcardMatcher{
	f64 num[128],inum[128];
	std::vector<f64> a,b;
	WildcardMatcher(){
		std::uniform_real_distribution urd(0.5,2.0);
		for(int i=0;i<128;++i){
			num[i]=urd(rng);
			inum[i]=1./num[i];
		}
	}
	void get(std::string_view S,std::string_view T,auto&&op,char w='?'){
		idt n=S.size(),m=T.size();
		idt lm=bcl(n);
		a.resize(lm),b.resize(lm);
		auto get=[&](auto&&f,char c){
			if(c==w){
				return 0.;
			}
			return f[int(c)];
		};
		for(idt i=0;i<n;++i){
			a[i]=get(num,S[i]);
		}
		b[0]=get(inum,T[0]);
		for(idt i=1;i<m;++i){
			b[lm-i]=get(inum,T[i]);
		}
		__fft::fft.fconv(a.data(),b.data(),lm);
		auto ok=[&](f64 x){
			return std::abs(x-i64(x+0.5))<1e-8;
		};
		for(idt i=0;i<=n-m;++i){
			if(ok(a[i])){
				op(i);
			}
		}
	}
}wm;
void work(){
	std::string S,T,res;
	std::cin>>S>>T;
	res.assign(S.size()-T.size()+1,'0');
	wm.get(S,T,[&](idt k){
		res[k]='1';
	},'*');
	std::cout<<res;
}
}
int main(){
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	yzlf::work();
	return 0;
}