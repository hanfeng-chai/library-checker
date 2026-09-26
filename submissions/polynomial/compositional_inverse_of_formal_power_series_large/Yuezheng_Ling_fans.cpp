#include <bits/stdc++.h>
using u32=unsigned;
using i64=long long;
using u64=unsigned long long;
using std::cin;
using std::cout;
constexpr u32 mod=998244353,_g=3;
constexpr auto mul(u32 x,u32 y)->u32{
    return u64(x)*y%mod;
}
constexpr auto dilt(u32 x,u32 M){
    return std::min(x,x+M);
}
constexpr auto shrk(u32 x,u32 M){
    return std::min(x,x-M);
}
constexpr auto qpw(u32 a,u32 b,u32 r=1){
    for(;b;b>>=1,a=mul(a,a)){
        if(b&1){
            r=mul(r,a);
        }
    }
    return r;
}
constexpr auto bcl(int x){
    return (x<2)?1:2<<std::__lg(x-1);
}
std::vector<u32> _w{1},iw{1},_w22{1};
auto init_w(int lm){
    _w.resize(lm),iw.resize(lm),_w22.resize(lm);
    for(auto i=1;i<lm;i<<=1){
        _w[i]=qpw(_g,((mod-1)>>2)/i);
        iw[i]=qpw(_g,mod-1-((mod-1)>>2)/i);
    }
    for(auto i=1;i<lm;++i){
        _w[i]=mul(_w[i&(i-1)],_w[i&-i]);
        iw[i]=mul(iw[i&(i-1)],iw[i&-i]);
    }
    for(auto i=1,i2=2;i<lm;i=i2,i2<<=1){
		auto _G=qpw(_g,(mod-1)/i2),r=mod-(mod-1)/i;
		for(auto j=i;j<i2;++j){
		    _w22[j]=r,r=mul(r,_G);
		}
	}
}
inline auto chk_w(int lm){
    if((lm>>=1)>int(_w.size())){
        init_w(lm);
    }
}
inline auto rot_R(u32*f,int L,u32 r){
    for(auto i=0;i<L;++i){
        auto x=f[i],y=mul(f[i+L],r);
        f[i]=shrk(x+y,mod),f[i+L]=dilt(x-y,mod);
    }
}
inline auto rot_L(u32*f,int L,u32 r){
    for(auto i=0;i<L;++i){
        auto x=f[i],y=f[i+L];
        f[i]=shrk(x+y,mod),f[i+L]=mul(x-y+mod,r);
    }
}
inline auto rrot_R(u32*f,int L,int lm){
    for(auto j=0,k=0;j<lm;j+=L*2,++k){
        rot_R(f+j,L,_w[k]);
    }
}
inline auto rrot_L(u32*f,int L,int lm){
    for(auto j=0,k=0;j<lm;j+=L*2,++k){
        rot_L(f+j,L,iw[k]);
    }
}
int fft_len=0;
auto dif(u32*f,int lm){
    fft_len+=lm;
    chk_w(lm);
    for(auto L=lm>>1;L;L>>=1){
        rrot_R(f,L,lm);
    }
}
auto fft_2D(u32*f,int n,int m){
    auto lm=n*m;
    fft_len+=lm;
    chk_w(lm);
    for(auto j=0;j<lm;j+=m){
        for(auto L=m>>1;L;L>>=1){
            rrot_R(f+j,L,m);
        }
    }
    for(auto L=lm>>1;L>=m;L>>=1){
        rrot_R(f,L,lm);
    }
}
auto print_2D(u32*f,int n,int m,int x=1){
    int fx=qpw(x,mod-2);
    cout<<"n:"<<n<<" m:"<<m<<"\n";
    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            cout<<mul(fx,f[i*m+j])<<" \n"[j+1==m];
        }
    }
}
auto dit(u32*f,int lm){
    fft_len+=lm;
    for(auto L=1;L<lm;L<<=1){
        rrot_L(f,L,lm);
    }
}
template<bool fx=true>auto ifft_2D(u32*f,int n,int m){
    auto lm=n*m;
    fft_len+=lm;
    for(auto j=0;j<lm;j+=m){
        for(auto L=1;L<m;L<<=1){
            rrot_L(f+j,L,m);
        }
    }
    for(auto L=m;L<lm;L<<=1){
        rrot_L(f,L,lm);
    }
    if constexpr(fx){
        const u32 iv=mod-(mod-1)/lm;
        for(auto i=0;i<lm;++i){
            f[i]=mul(f[i],iv);
        }
    }
}
inline auto dot(u32*f,const u32*g,int lm){
    for(auto i=0;i<lm;++i){
        f[i]=mul(f[i],g[i]);
    }
}
inline auto rdot(const u32*f,const u32*g,u32*h,int lm){
    for(auto i=0;i<lm;++i){
        h[i]=mul(f[i],g[i]);
    }
}
std::vector<u32> fac{1},ifac{1},iv{0};
auto init_fac(int n){
    fac.resize(n),ifac.resize(n),iv.resize(n);
	for(auto i=1;i<n;++i){fac[i]=mul(fac[i-1],i);}
	ifac[n-1]=qpw(fac[n-1],mod-2);
	for(auto i=n-1;i>0;--i){ifac[i-1]=mul(ifac[i],i),iv[i]=mul(ifac[i],fac[i-1]);}
}
inline auto chk_fac(int n){
    if(n>int(fac.size())){
        init_fac(std::max(n,int(fac.size())*2));
    }
}
std::vector<u32> Ax,Bx,Cx,Dx;
inline auto toBuf(std::vector<u32>&f,int lm){
    f.resize(lm);
    return f.data();
}
constexpr auto iv4=qpw(4,mod-2);
auto Inv(const u32*f,u32*g,int n){
	g[0]=qpw(f[0],mod-2);
	auto lm=bcl(n);
    auto ax=toBuf(Ax,lm),bx=toBuf(Bx,lm);
    auto fx=mod-iv4;
	for(auto t=2,m=1;t<=lm;m=t,t<<=1,fx=mul(fx,iv4)){
		auto xl=std::min(t,n);
		std::fill(std::copy_n(f,xl,ax),ax+t,0),std::fill(std::copy_n(g,m,bx),bx+t,0);
		dif(ax,t),dif(bx,t),dot(ax,bx,t),dit(ax,t),std::fill_n(ax,m,0),dif(ax,t),dot(ax,bx,t),dit(ax,t);
        for(auto i=m;i<xl;++i){g[i]=mul(ax[i],fx);}
	}
}
auto Quo(const u32*f,const u32*g,u32*h,int n){
	if(n==1){*h=qpw(*g,mod-2,*f);return;}
	auto lm=bcl(n),hl=lm>>1;
    const u32 iv=mod-(mod-1)/lm;
    auto ax=toBuf(Ax,lm),bx=toBuf(Bx,lm),cx=toBuf(Cx,lm);
	Inv(g,cx,hl),std::fill_n(cx+hl,hl,0),dif(cx,lm),std::fill(std::copy_n(f,hl,ax),ax+lm,0),dif(ax,lm),dot(ax,cx,lm),dit(ax,lm);
	for(auto i=0;i<hl;++i){h[i]=ax[i]=mul(ax[i],iv);}
	std::fill_n(ax+hl,hl,0),dif(ax,lm),std::fill(std::copy_n(g,n,bx),bx+lm,0),dif(bx,lm),dot(ax,bx,lm),dit(ax,lm),std::fill_n(ax,hl,0);
	for(auto i=hl;i<n;++i){ax[i]=(u64(ax[i])*iv+(mod-f[i]))%mod;}
	dif(ax,lm),dot(ax,cx,lm),dit(ax,lm);
    const u32 _iv=mod-iv;
	for(auto i=hl;i<n;++i){h[i]=mul(ax[i],_iv);}
}
auto Ln(const u32*f,u32*g,int n){
    auto dx=toBuf(Dx,n);
	for(auto i=0;i<n;++i){
        dx[i]=mul(f[i],i);
    }
	Quo(dx,f,g,n),dot(g,iv.data(),n);
}
auto Exp(const u32*f,u32*g,int n){
	auto lm=bcl(n);
    auto ax=toBuf(Ax,lm),bx=toBuf(Bx,lm),cx=toBuf(Cx,lm),dx=toBuf(Dx,lm);
    g[0]=dx[0]=ax[0]=ax[1]=1;
    auto fx=mod-iv4;
	for(auto t2=4,t=2,m=1;t<=lm;m=t,t=t2,t2<<=1,fx=mul(fx,iv4)){
		auto xl=std::min(t,n);
		for(auto i=0;i<m;++i){cx[i]=mul(f[i],i);}
		dif(cx,m),dot(cx,ax,m),dit(cx,m);
        const u32 IV=(mod-1)/m;
		for(auto i=0;i<m;++i){cx[m+i]=(u64(g[i])*i+u64(cx[i])*IV)%mod,cx[i]=0;}
		dif(cx,t),std::fill(std::copy(dx,dx+m,bx),bx+t,0),dif(bx,t),dot(cx,bx,t),dit(cx,t);
		const u32 Iv=(mod-1)/t;
        for(int i=m;i<t;++i){cx[i]=(mul(cx[i],Iv)*u64(iv[i])+(f[i]))%mod,cx[i-m]=0;}
		dif(cx,t),dot(cx,ax,t),dit(cx,t);
        const u32 iv=mod-Iv;
		for(int i=m;i<xl;++i){g[i]=mul(cx[i],iv);}
		if(t!=lm){
			std::fill(std::copy_n(g,t,ax),ax+t2,0),dif(ax,t2),rdot(ax,bx,cx,t),dit(cx,t);
			for(auto i=m;i<t;++i){cx[i]=mul(cx[i],fx),cx[i-m]=0;}
			dif(cx,t),dot(cx,bx,t),dit(cx,t),std::copy(cx+m,cx+t,dx+m);
		}
	}
}
auto __PowerXY(std::vector<u32> g,int n){
    std::vector<u32> dftP(4*n,1),dftQ(4*n);
    for(auto i=0;i<n;++i){dftQ[i]=dilt(-g[i],mod);}
    chk_w(n*2);
    for(auto L=n;L;L>>=1){rrot_R(dftQ.data(),L,n*2);}
    std::fill(dftQ.begin()+n*2,dftQ.end(),1);
    rot_R(dftQ.data(),n*2,1);
    auto k=1,t=n;
    u32 One=1;
    for(;;t>>=1,k<<=1){
        for(auto i=0;i<2*k;++i){
            for(auto j=0;j<2*t;j+=2){
                auto p=i*(2*t)+j,q=i*t+j/2;
                auto x=dftQ[p],y=dftQ[p+1];
                dftP[q]=mul(mul(dftP[p],y)-mul(dftP[p+1],x)+mod,iw[j/2]);
                dftQ[q]=mul(x,y);
            }
        }
        One=mul(One,One);
        if(t==2){
            ifft_2D<false>(dftP.data(),2*k,t);
            const u32 fx=qpw(mul(One,2*n),mod-2);
            for(auto i=0;i<n;++i){
                dftP[i]=mul(fx,dftP[i<<1]);
            }
            dftP.resize(n);
            break;
        }
        else{
            fft_len += 8*n;
            {
            for(auto i=0;i<2*k;++i){
                auto dft=dftP.data()+i*t;
                for(auto L=1;L<t;L<<=1){
                    rrot_L(dft,L,t);
                }
                std::fill(dft+t/2,dft+t,0);
                for(auto L=t>>1;L;L>>=1){
                    rrot_R(dft,L,t);
                }
            }
            auto g=dftP.data()+2*n;
            std::copy(dftP.cbegin(),dftP.cbegin()+2*n,g);
            for(auto L=t;L<n*2;L<<=1){
                rrot_L(g,L,n*2);
            }
            for(int j=0,k=0,diff=(n*2)/t;j<n*2;j+=t,++k){
                for(int i=0;i<t;++i){
                    g[j+i]=mul(g[j+i],_w22[diff+k]);
                }
            }
            for(auto L=n;L>=t;L>>=1){
                rrot_R(g,L,n*2);
            }

            }
            {
            for(auto i=0;i<2*k;++i){
                auto dft=dftQ.data()+i*t;
                for(auto L=1;L<t;L<<=1){
                    rrot_L(dft,L,t);
                }
                std::fill(dft+t/2,dft+t,0);
                for(auto L=t>>1;L;L>>=1){
                    rrot_R(dft,L,t);
                }
            }
            auto g=dftQ.data()+2*n;
            std::copy(dftQ.cbegin(),dftQ.cbegin()+2*n,g);
            
            for(auto L=t;L<n*2;L<<=1){
                rrot_L(g,L,n*2);
            }
            for(int j=0,k=0,diff=(n*2)/t;j<n*2;j+=t,++k){
                for(int i=0;i<t;++i){
                    g[j+i]=mul(g[j+i],_w22[diff+k]);
                }
            }
            One=mul(One,t);
            u32 Two=shrk(One+One,mod);
            for(int i=0;i<t;++i){
                g[i]=dilt(g[i]-Two,mod);
            }
            for(auto L=n;L>=t;L>>=1){
                rrot_R(g,L,n*2);
            }

            }
        }
    }
    return dftP;
}
auto CompInv(std::vector<u32> g,int n){
    int lm=bcl(n);

    g.resize(lm);
    int v=qpw(g[1],mod-2);
    for(auto&x:g){x=mul(x,v);}
    auto G=__PowerXY(g,lm);
    chk_fac(lm);
    for(int i=0,fx=1;i<lm;++i){
        g[i]=mul(mul(fx,iv[lm-i-1]),mul(G[i],lm-1));
        fx=mul(fx,v);
    }
    Ln(g.data(),G.data(),lm);
    for(auto&x:G){x=mul(x,mod-iv[lm-1]);}
    Exp(G.data(),g.data()+1,lm-1),g[0]=0;
    for(int i=1;i<lm;++i){g[i]=mul(g[i],v);}

    g.resize(n);
    return g;
}
auto solve(){
    int n;
    cin>>n;
    std::vector<u32> f(n);
    for(auto&x:f){
        cin>>x;
    }
    for(auto x:CompInv(f,n)){
        cout<<x<<" ";
    }
    std::cerr<<"\nfft_len:"<<fft_len<<"\n";
}
int main(){
    // freopen("test.in","r",stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}
/*
关注乐正绫谢谢.
懒得实现一个重型的二维fft了,就这样吧.
*/