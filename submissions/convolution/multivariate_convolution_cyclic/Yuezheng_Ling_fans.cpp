#include <bits/stdc++.h>
namespace __yzlf{
using std::cin;
using std::cout;
using u32=unsigned;
using i64=long long;
using u64=unsigned long long;
using f64=double;
using idt=std::size_t;
namespace __fft{
struct cpx{
    f64 x,y;
    cpx()=default;
    cpx(f64 xx,f64 yy):x(xx),y(yy){}
    cpx operator+(cpx b)const{return {x+b.x,y+b.y};}
    cpx operator-(cpx b)const{return {x-b.x,y-b.y};}
    //mod (x^2+1)
    cpx operator*(cpx b)const{return {x*b.x-y*b.y,x*b.y+y*b.x};}
    cpx operator*(f64 b)const{return {x*b,y*b};}
    //a*conj(b)
    friend cpx mulT(cpx a,cpx b){return {a.x*b.x+a.y*b.y,a.y*b.x-a.x*b.y};}
    template<bool t=false>friend cpx mul(cpx a,cpx b){
        if constexpr(t){return mulT(a,b);}
        return a*b;
    }
    //(a-b)*i
    friend cpx subI(cpx a,cpx b){return {b.y-a.y,a.x-b.x};}
    template<bool t=false>friend cpx sub_rot90(cpx a,cpx b){
        if constexpr(t){return subI(b,a);}
        return subI(a,b);
    }
    //mod (x^2-1)
    friend cpx mulY(cpx a,cpx b){return {a.x*b.x+a.y*b.y,a.x*b.y+a.y*b.x};}
    cpx operator-(){return {-x,-y};}
};
inline cpx Wn(auto a){return cpx(std::cos(a),std::sin(a));}
inline cpx conj(cpx x){return {x.x,-x.y};}
struct ffter{
    std::vector<std::array<cpx,3> > wn;
    ffter():wn{}{reserve(2);}
    //reserve(n) for dif(f,4n).
    void reserve(idt n){
        idt sz=wn.size();
        if(n<=sz){return;}
        int t=std::__lg(n),t2=(t+1)>>1;
        auto z=idt(1)<<t2;
        std::vector<std::array<cpx,3> > b(z*2);
        const auto r=0.5*acosl(-1)/z,q=r/z;
        for(idt i=0,j=(z*3)>>1,p=0;i<z;p-=z-(j>>__builtin_ctzll(++i))){
            b[i]={Wn(p*r),Wn(p*r*2),Wn(p*r*3)};
            b[i|z]={Wn(p*q),Wn(p*q*2),Wn(p*q*3)};
        }
        wn.resize(n);
        for(idt i=sz;i<n;++i){
            const auto x=b[i&(z-1)],y=b[z|(i>>t2)];
            wn[i]={x[0]*y[0],x[1]*y[1],x[2]*y[2]};
        }
    }
    template<bool t>void dif(cpx*f,idt n){
        if((n>>2)>wn.size()){reserve(n>>2);}
        idt L=n>>1;
		if(__builtin_ctzll(n)&1){
			for(idt j=0;j<L;++j){
				const cpx x=f[j],y=f[j+L];
				f[j]=x+y,f[j+L]=x-y;
			}
			L>>=1;
		}
		L>>=1;
        for(idt l=L<<2;L;l=L,L>>=2){
            for(idt j=0;j<L;++j){
                const cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                const cpx g0=f0+f2,g1=f1+f3,g2=f0-f2,g3=sub_rot90<t>(f1,f3);
                f[j]=g0+g1,f[j+L]=g0-g1,f[j+L*2]=g2+g3,f[j+L*3]=g2-g3;
            }
            for(idt i=l,k=1;i<n;i+=l,++k){
                const auto [r1,r2,r3]=wn[k];
                for(idt j=i;j<i+L;++j){
                    const cpx f0=f[j],f1=mul<t>(f[j+L],r1),f2=mul<t>(f[j+L*2],r2),f3=mul<t>(f[j+L*3],r3);
                    const cpx g0=f0+f2,g1=f1+f3,g2=f0-f2,g3=sub_rot90<t>(f1,f3);
					f[j]=g0+g1,f[j+L]=g0-g1,f[j+L*2]=g2+g3,f[j+L*3]=g2-g3;
                }
            }
        }
    }
    template<bool t>void dit(cpx*f,idt n){
        if((n>>2)>wn.size()){reserve(n>>2);}
        idt L=1;
		for(idt l=L<<2;L<(n>>1);L=l,l<<=2){
            for(idt j=0;j<L;++j){
                const cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                const cpx g0=f0+f1,g1=f0-f1,g2=f2+f3,g3=sub_rot90<t>(f2,f3);
                f[j]=g0+g2,f[j+L]=g1+g3,f[j+L*2]=g0-g2,f[j+L*3]=g1-g3;
            }
            for(idt i=l,k=1;i<n;i+=l,++k){
                const auto [r1,r2,r3]=wn[k];
                for(idt j=i;j<i+L;++j){
                    const cpx f0=f[j],f1=f[j+L],f2=f[j+L*2],f3=f[j+L*3];
                    const cpx g0=f0+f1,g1=f0-f1,g2=f2+f3,g3=sub_rot90<t>(f2,f3);
					f[j]=g0+g2,f[j+L]=mul<t>(g1+g3,r1),f[j+L*2]=mul<t>(g0-g2,r2),f[j+L*3]=mul<t>(g1-g3,r3);
                }
            }
        }
        if(L!=n){
			for(idt j=0;j<L;++j){
				const cpx x=f[j],y=f[j+L];
				f[j]=x+y,f[j+L]=x-y;
			}
		}
    }
}fft;
template<bool t=false>inline void dif(std::vector<cpx>&f){fft.dif<t>(f.data(),f.size());}
template<bool t=true>inline void dit(std::vector<cpx>&f){fft.dit<t>(f.data(),f.size());}
}//namespace __fft
using __fft::cpx;
using __fft::dif;
using __fft::dit;
struct Barrett{
    Barrett()=default;
    Barrett(u64 P):M(P),im(u64(-1)/M+1){}
    u64 operator()(u64 x)const{
        u64 y=x-u64((__uint128_t(x)*im)>>64)*M;
        return std::min(y,y+M);
    }
    u64 mod()const{return M;}
    u32 mul_(u32 x,u32 y)const{
        return operator()(u64(x)*y);
    }
    u32 qpw_(u32 a,u32 b,u32 r=1)const{
        for(;b;b>>=1,a=mul_(a,a)){
            if(b&1){
                r=mul_(r,a);
            }
        }
        return r;
    }
    private:
    u64 M,im;
};
constexpr idt bcl(idt x){
    return x<2?1:2<<std::__lg(x-1);
}
namespace MTT{
template<bool t=false>void to_poi(const u32*a,idt n,std::vector<cpx>&b,idt lm){
    b.assign(lm,{});
    for(idt i=0;i<n;++i){b[i]=cpx(a[i]>>15,a[i]&32767);}
    dif<t>(b);
}
void poi_dot(std::vector<cpx>&a,std::vector<cpx>&b){
    idt lm=a.size();
    f64 fx=0.5/lm;
    for(idt i=0;i<std::min<idt>(2,lm);++i){
        cpx p=a[i],r=b[i]*fx;
        a[i]=r*(2*p.x),b[i]=cpx{r.y,-r.x}*(2*p.y);
    }
    for(idt k=2,m=3;k<lm;k<<=1,m<<=1){
        for(idt i=k,j=k<<1;--j,i<m;++i){
            cpx p=a[i],q=a[j],r=b[i]*fx,s=b[j]*fx;
            a[i]=(p+conj(q))*r,b[i]=(conj(q)-p)*r;
            a[j]=(q+conj(p))*s,b[j]=(conj(p)-q)*s;
        }
    }
}
template<bool t=true>void un_poi(u32*a,std::vector<cpx>&b,std::vector<cpx>&c,idt l,idt r,Barrett md){
    dit<t>(b),dit<t>(c);
    for(idt i=l;i<r;++i){
        a[i]=md((md(i64(b[i].x+0.5))<<30)+(md(i64(b[i].y+0.5)+i64(0.5-c[i].y))<<15)+md(i64(c[i].x+0.5)));
    }
}
struct CacheChirpZ{
    idt n,u,lm;
    Barrett md;
    std::vector<cpx> A,B,C;
    std::vector<u32> pir;
    CacheChirpZ(idt _n,u32 r,Barrett _m):n(_n),u(n+n-1),lm(bcl(u)),md(_m),A{},B{},C{},pir(u){
        u32 pr=1,fx=1,ir=md.qpw_(r,md.mod()-2);
        for(idt i=0;i<u;++i){
            pir[i]=pr,pr=md.mul_(pr,fx),fx=md.mul_(fx,r);
        }
        to_poi(pir.data(),u,B,lm),pir.resize(n),pr=1,fx=1;
        for(idt i=0;i<n;++i){
            pir[i]=pr,pr=md.mul_(pr,fx),fx=md.mul_(fx,ir);
        }
    }
    void operator()(u32*g,const u32*f){
        for(idt i=0;i<n;++i){g[i]=md.mul_(f[i],pir[i]);}
        to_poi<true>(g,n,A,lm),C=B;
        //real diff conv.
        poi_dot(A,C),un_poi(g,A,C,0,n,md);
        for(idt i=0;i<n;++i){g[i]=md.mul_(g[i],pir[i]);}
    }
};
}//namespace MTT
constexpr u32 get_pri_rt(u32 M){
    u32 R=0,n=M-1,d[11]={};
    for(u32 i=2;i*i<=n;++i){
        if(n%i==0){d[R++]=i;do{n/=i;}while(n%i==0);}
    }
    if(n>1){d[R++]=n;}
    for(u32 g=2,r=0;;++g){
        for(u32 i=0;i<R;++i){
            u32 b=(M-1)/d[i],a=g;
            for(r=1;b;b>>=1,a=u64(a)*a%M){b&1?r=u64(r)*a%M:r;}
            if(r==1){break;}
        }
        if(r!=1){return g;}
    }
}
//sz*ni but faster than sz*log ni when ni is tiny.
inline void _brute_rot_k(u32*f,idt sz,idt step,idt ni,Barrett md,u32 rr){
    //assert(ni<=16);
    u32 tmp[16],ini[16][16];
    {
    u32 r=1;
    for(idt k=0;k<ni;++k){
        u32 rrr=1;
        for(idt w=0;w<ni;++w){
            ini[k][w]=rrr;
            rrr=md.mul_(rrr,r);
        }
        r=md.mul_(r,rr);
    }
    }
    for(idt i=0;i<sz;i+=step*ni){
        for(idt j=0;j<step;++j){
            for(idt k=0;k<ni;++k){
                tmp[k]=f[i+k*step+j];
            }
            for(idt k=0;k<ni;++k){
                u64 sum=0;
                for(idt w=0;w<ni;++w){
                    sum+=ini[k][w]*u64(tmp[w]);
                }
                f[i+k*step+j]=md(sum);
            }
        }
    }
}
void work(){
    u32 P,_g;
    int K;
    cin>>P>>K;
    Barrett md(P);
    std::vector<idt> n(K),prn(K);
    _g=get_pri_rt(P);
    for(auto&x:n){cin>>x;}
    idt sz=1;
    for(int i=0;i<K;++i){
        sz*=n[i];
    }
    std::vector<u32> f(sz),g(sz);
    std::vector<MTT::CacheChirpZ> vccz;
    vccz.reserve(K);
    for(auto&x:f){cin>>x;}
    for(auto&x:g){cin>>x;}

    idt step=1;
    for(int i=0;i<K;step*=n[i++]){
        u32 wn=md.qpw_(_g,(P-1)/n[i]);
        vccz.emplace_back(n[i],wn,md);
        if(n[i]<10){
            _brute_rot_k(f.data(),sz,step,n[i],md,wn);
            _brute_rot_k(g.data(),sz,step,n[i],md,wn);
        }
        else{
            std::vector<u32> bufa(n[i]),bufb(n[i]);
            idt rsz=sz/n[i],rrsz=rsz/step,n_step=n[i]*step;
            for(idt j=0;j<rrsz;++j){
                for(idt k=0;k<step;++k){
                    for(idt w=0;w<n[i];++w){
                        bufa[w]=f[j*n_step+w*step+k];
                        bufb[w]=g[j*n_step+w*step+k];
                    }
                    vccz[i](bufa.data(),bufa.data());
                    vccz[i](bufb.data(),bufb.data());
                    for(idt w=0;w<n[i];++w){
                        f[j*n_step+w*step+k]=bufa[w];
                        g[j*n_step+w*step+k]=bufb[w];
                    }
                }
            }   
        }
    }
    
    u32 iv=md.qpw_(sz,P-2);
    for(idt i=0;i<sz;++i){
        f[i]=md.mul_(md.mul_(f[i],g[i]),iv);
    }

    step=1;
    for(int i=0;i<K;step*=n[i++]){
        if(n[i]<10){
            _brute_rot_k(f.data(),sz,step,n[i],md,md.qpw_(_g,(P-1)-((P-1)/n[i])));
        }
        else{
            std::vector<u32> bufa(n[i]);
            idt rsz=sz/n[i],rrsz=rsz/step,n_step=n[i]*step;
            /*
            note:
            dft(dft(f,n),n) = n (f[0] + rev(f+1,f+n))
            */
            for(idt j=0;j<rrsz;++j){
                for(idt k=0;k<step;++k){
                    for(idt w=0;w<n[i];++w){
                        bufa[w]=f[j*n_step+w*step+k];
                    }
                    vccz[i](bufa.data(),bufa.data());
                    f[j*n_step+k]=bufa[0];
                    for(idt w=1;w<n[i];++w){
                        f[j*n_step+w*step+k]=bufa[n[i]-w];
                    }
                }
            }
        }
    }

    for(idt i=0;i<sz;++i){
        cout<<f[i]<<' ';
    }
}//"我忘记了所有悲剧,所见皆是奇迹."
}//namespace __yzlf
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    __yzlf::work();
    return 0;
}