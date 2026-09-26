#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <vector>
#include <immintrin.h>
#include <array>
#include <cstring>
#include <cassert>
#include <ctime>
#include <ccomplex>
#include <chrono>
#include <random>
#include <cstddef>
#include <sys/mman.h>
#include <sys/stat.h>
#pragma GCC target("avx2","fma")
namespace __yzlf{
namespace _yio{
using i64=long long;
using u8=unsigned char;
using u16=unsigned short;
using u32=unsigned;
using u64=unsigned long long;
constexpr std::size_t buf_def_size=262144;
constexpr std::size_t buf_flush_threshold=32;
constexpr std::size_t string_copy_threshold=512;
constexpr u64 E16=1e16,E12=1e12,E8=1e8,E4=1e4;
struct _io_t{
    u8 t_i[1<<15];
    int t_o[10000];
    constexpr _io_t(){
        std::fill(t_i,t_i+(1<<15),u8(-1));
        for(int i=0;i<10;++i){
            for(int j=0;j<10;++j){
                t_i[0x3030+256*j+i]=j+10*i;
            }
        }
        for(int e0=(48<<0),j=0;e0<(58<<0);e0+=(1<<0)){
			for(int e1=(48<<8);e1<(58<<8);e1+=(1<<8)){
				for(int e2=(48<<16);e2<(58<<16);e2+=(1<<16)){
					for(int e3=(48<<24);e3<(58<<24);e3+=(1<<24)){
						t_o[j++]=e0^e1^e2^e3;
					}
				}
			}
		}
    }
    void get(char*s,u32 p)const{
        *((int*)s)=t_o[p];
    }
};
constexpr _io_t _iot={};
struct Qinf{
    explicit Qinf(FILE*fi):f(fi){
		auto fd=fileno(f);
		fstat(fd,&Fl);
		bg=(char*)mmap(0,Fl.st_size+4,PROT_READ,MAP_PRIVATE,fd,0);
		p=bg,ed=bg+Fl.st_size;
        madvise(bg,Fl.st_size+4,MADV_SEQUENTIAL);
	}
	~Qinf(){
		munmap(bg,Fl.st_size+4);
	}
	template<std::unsigned_integral T>Qinf&operator>>(T&x){
		skip_space();
        x=*p++-'0';
        for(;;){
            T y=_iot.t_i[*reinterpret_cast<u16*>(p)];
            if(y>99){break;}
            x=x*100+y,p+=2;
        }
        if(*p>' '){
            x=x*10+(*p++&15);
        }
        return *this;
    }
    template<std::signed_integral T>Qinf&operator>>(T&x){
		skip_space();
        int sign;
        p+=(sign=(*p=='-'));
        x=*p++-'0';
        for(;;){
            u32 y=_iot.t_i[*reinterpret_cast<u16*>(p)];
            if(y>99){break;}
            x=x*100+y,p+=2;
        }
        if(*p>' '){
            x=x*10+(*p++&15);
        }
        x=(sign?-x:x);
        return *this;
    }
    std::string_view read_token(){
        skip_space();
        auto bg=p;
        while(*p>' '){++p;}
        return {bg,p};
    }
	private:
	void skip_space(){
		while(*p<=' '){
			++p;
		}	
	}
	FILE*f;
	char*bg,*ed,*p;
	struct stat Fl;
}qin(stdin);
struct Qoutf{
    explicit Qoutf(FILE*fi,std::size_t sz=buf_def_size):f(fi),bg(new char[sz]),ed(bg+sz-buf_flush_threshold),p(bg){}
    ~Qoutf(){
		flush();
		delete[] bg;
	}
	void flush(){
		fwrite_unlocked(bg,1,p-bg,f),p=bg;
	}
	Qoutf&operator<<(u32 x){
		wt_u32(x);
		return *this;
	}
	Qoutf&operator<<(u64 x){
		wt_u64(x);
		return *this;
	}
    Qoutf&operator<<(int x){
        x<0?(*p++='-',wt_u32(-x)):wt_u32(x);
        return *this;
    }
    Qoutf&operator<<(i64 x){
        x<0?(*p++='-',wt_u64(-x)):wt_u64(x);
        return *this;
    }
	Qoutf&operator<<(char ch){
		*p++=ch;
		return *this;
	}
    Qoutf&operator<<(std::string_view s){
        if(s.size()>=string_copy_threshold){
            flush(),fwrite_unlocked(s.data(),1,s.size(),f);
        }
        else{
            if((p+s.size())>ed)[[unlikely]]{
                flush();
            }
            memcpy(p,s.data(),s.size()),p+=s.size();
        }
        return*this;
    }
    private:
    void wt_u32(u32 x){
        if(x>=E8){
			put2(x/E8),x%=E8,putb(x/E4),putb(x%E4);
		}
		else if(x>=E4) {
			put4(x/E4),putb(x%E4);
		}
		else{
			put4(x);
		}
		chk();
    }
    void wt_u64(u64 x){
        if(x>=E8){
			u64 q0=x/E8,r0=x%E8;
			if(x>=E16){
				u64 q1=q0/E8,r1=q0%E8;
				put4(q1),putb(r1/E4),putb(r1%E4);
			}
			else if(x>=E12){
				put4(q0/E4),putb(q0%E4);
			}
			else{
				put4(q0);
			}
			putb(r0/E4),putb(r0%E4);
		}
		else{
			if(x>=E4){
				put4(x/E4),putb(x%E4);
			}
			else{
				put4(x);
			}
		}
		chk();
    }
	void putb(u32 x){
		_iot.get(p,x),p+=4;
	}
	void put4(u32 x){
		if(x>99){
			if(x>999){
				putb(x);
			}
			else{
				_iot.get(p,x*10),p+=3;
			}	
		}
		else{
			put2(x);
		}
	}
	void put2(u32 x){
		if(x>9){
			_iot.get(p,x*100),p+=2;
		}
		else{
			*p++=x+'0';
		}
	}
	void chk(){
		if(p>ed)[[unlikely]]{
			flush();
		}
	}
	FILE *f;
	char *bg,*ed,*p;
}qout(stdout);
}
using _yio::qin;
using _yio::qout;
std::mt19937_64 rng(0xee0000+std::chrono::system_clock().now().time_since_epoch().count());
using u32=uint32_t;
using i64=int64_t;
using u64=uint64_t;
using f64=double;
using ldb=long double;
using u128=__uint128_t;
using idt=std::size_t; 
using std::cin;
using std::cout;
template<class T>inline T*cpy(T*f,const T*g,idt n){return (T*)memcpy(f,g,n*sizeof(T));}
template<class T>inline T*clr(T*f,idt n){return (T*)memset(f,0,n*sizeof(T));}
template<class T,idt aln=32>inline T*alc(idt n){return new(std::align_val_t(aln))T[n];}
template<class T,idt aln=32>inline void fre(T*p){::operator delete[](p,std::align_val_t(aln));}
constexpr idt bcl(idt x){return x<2?1:idt(2)<<std::__lg(x-1);}
//I do know what I am doing.
struct cpx{
	__m128d v;
	cpx()=default;
	cpx(__m128d vv):v(vv){}
	explicit cpx(f64 x,f64 y=0.0):v(_mm_set_pd(y,x)){}
	template<class U>cpx(std::complex<U> c):v(_mm_set_pd(c.imag(),c.real())){}
	cpx operator+(cpx y)const{return v+y.v;}
	cpx operator-(cpx y)const{return v-y.v;}
	cpx operator*(cpx y)const{return _mm_fmaddsub_pd(_mm_unpacklo_pd(v,v),y.v,_mm_unpackhi_pd(v,v)*_mm_permute_pd(y.v,1));}
	cpx operator*(f64 x)const{return v*_mm_set1_pd(x);}
    cpx operator-()const{return -v;}
    f64 real(){return v[0];}
    f64 imag(){return v[1];}
};
inline cpx mulT(cpx x,cpx y){
	return _mm_fmsubadd_pd(_mm_unpacklo_pd(y.v,y.v),x.v,_mm_unpackhi_pd(y.v,y.v)*_mm_permute_pd(x.v,1));
}
inline std::ostream&operator <<(std::ostream&os,cpx x){
    return os<<'('<<x.real()<<','<<x.imag()<<')';
}
inline cpx get_wn(auto a){
    return cpx(f64(std::cos(a)),f64(std::sin(a)));
}
inline cpx conj(cpx x){
    return cpx(x.real(),-x.imag());
}
using f64x4=__m256d;
using I256=__m256i;
struct vcpx{
    f64x4 x,y;
    vcpx()=default;
	explicit vcpx(f64x4 xx,f64x4 yy):x(xx),y(yy){}
    vcpx operator+(vcpx b)const{return vcpx(x+b.x,y+b.y);}
	vcpx operator-(vcpx b)const{return vcpx(x-b.x,y-b.y);}
	vcpx operator*(vcpx b)const{return vcpx(_mm256_fmsub_pd(x,b.x,y*b.y),_mm256_fmadd_pd(y,b.x,x*b.y));}
};
inline vcpx mulT(vcpx a,vcpx b){
    return vcpx(_mm256_fmadd_pd(a.x,b.x,a.y*b.y),_mm256_fmsub_pd(a.y,b.x,a.x*b.y));
}
inline vcpx mulR(vcpx a,f64x4 b){
    return vcpx(a.x*b,a.y*b);
}
inline vcpx subI(vcpx a,vcpx b){
    return vcpx(b.y-a.y,a.x-b.x);
}
inline std::ostream&operator <<(std::ostream&os,vcpx x){
    cout<<"4x ";
    for(int i=0;i<4;++i){
        cout<<'('<<x.x[i]<<','<<x.y[i]<<") ";
    }
    cout<<'\n';
    return os;
}
//return a*b+c
inline vcpx fma(vcpx a,vcpx b,vcpx c){
    return vcpx(_mm256_fmsub_pd(a.x,b.x,_mm256_fmsub_pd(a.y,b.y,c.x)),_mm256_fmadd_pd(a.y,b.x,_mm256_fmadd_pd(a.x,b.y,c.y)));
}
inline vcpx vcpx_set(cpx a,cpx b,cpx c,cpx d){
    return vcpx{_mm256_set_pd(d.real(),c.real(),b.real(),a.real()),_mm256_set_pd(d.imag(),c.imag(),b.imag(),a.imag())};
}
template<int z>inline cpx select(vcpx x){
    return cpx(x.x[z],x.y[z]);
}
inline vcpx expand(cpx x){
    auto v=_mm256_castpd128_pd256(x.v);
    return vcpx(_mm256_permute4x64_pd(v,0),_mm256_permute4x64_pd(v,0x55));
}
template<int z>inline vcpx select_and_expand(vcpx x){
    static_assert(0<=z&&z<4);
    return vcpx(_mm256_permute4x64_pd(x.x,0x55*z),_mm256_permute4x64_pd(x.y,0x55*z));
}
inline vcpx expand_odd(vcpx x){
    return vcpx{_mm256_movedup_pd(x.x),_mm256_movedup_pd(x.y)};
}
//1,-1,i,-i
//x,-x,-y,y
//y,-y,x,-x
inline vcpx rotate_expand_odd(vcpx x){
    auto real=_mm256_permute_pd(x.x,0),imag=_mm256_permute_pd(x.y,0);
    static constexpr i64 f64_neg_mask=i64(-1)<<63;
    static const f64x4 neg_mask0=_mm256_castsi256_pd(_mm256_set_epi64x(f64_neg_mask,0,f64_neg_mask,0));
    static const f64x4 neg_mask1=_mm256_castsi256_pd(_mm256_set_epi64x(0,f64_neg_mask,f64_neg_mask,0));
    real=_mm256_xor_pd(real,neg_mask0),imag=_mm256_xor_pd(imag,neg_mask1);
    return vcpx{_mm256_blend_pd(real,imag,0xc),_mm256_blend_pd(real,imag,0x3)};
}
struct imag_fft_omega_table{
    idt n,kl;
    vcpx*vf0,*vf1;
    imag_fft_omega_table():n(256),kl(3),vf0(alc<vcpx>(64)),vf1(alc<vcpx>(kl)){
        const auto p1=std::numbers::pi_v<ldb>/(2*n),p2=p1+p1,p3=p2+p1;
        auto nn=n;
        for(idt i=0,k=(nn*3)>>1,p=0;i<64;++i,p-=nn-(k>>__builtin_ctzll(i))){
            auto r1=get_wn(p*p1),r2=get_wn(p*p2),r3=get_wn(p*p3);
            vf0[i]=vcpx_set(r1,r2,r1,r3);
        }
        nn=n/64;
        for(idt i=0,k=(nn*3)>>1,p=0;i<kl;++i,p-=nn-(k>>__builtin_ctzll(i))){
            auto r1=get_wn(p*p1),r2=get_wn(p*p2),r3=get_wn(p*p3);
            vf1[i]=vcpx_set(r1,r2,r1,r3);
        }
    }
    ~imag_fft_omega_table(){
        fre(vf0),fre(vf1);
    }
    void reserve(idt _n){
        assert(std::has_single_bit(_n));
        if(_n<=n){
            return;
        }
        n=_n,kl=(_n*3)>>8,fre(vf1),vf1=alc<vcpx>(kl);
        const auto p1=std::numbers::pi_v<ldb>/(2*n),p2=p1+p1,p3=p2+p1;
        auto nn=n/64;
        for(idt i=0,k=(nn*3)>>1,p=0;i<kl;++i,p-=nn-(k>>__builtin_ctzll(i))){
            auto r1=get_wn(p*p1),r2=get_wn(p*p2),r3=get_wn(p*p3);
            vf1[i]=vcpx_set(r1,r2,r1,r3);
        }
    }
    vcpx operator [](idt p){
        return vf0[p&63]*vf1[p>>6];
    }
}roselia;
void _vfft(vcpx*f,idt n){
    if(__builtin_ctzll(n)&1){
        idt l=n/2;
        const auto omega=expand_odd(roselia[1]);
        // cout<<omega<<'\n';
        for(idt j=0;j<l;++j){
            auto x=f[j],y=f[l+j]*omega;
            f[j]=x+y,f[l+j]=x-y;
        }
    }
    for(idt i=0;i<n;i+=4){
        int t=__builtin_ctzll(n+i)&-2;
        for(;t>1;t-=2){
            idt l=idt(1)<<t,L=l>>2;
            auto rr=roselia[(i+n*2)>>t];
            auto r1=expand_odd(rr),r2=select_and_expand<1>(rr),r3=select_and_expand<3>(rr);
            // cout<<l<<" "<<i<<" "<<L<<"\n";
            // cout<<r1<<'\n'<<r2<<'\n'<<roselia[(n*2)>>t]<<'\n';
            for(idt j=0;j<L;++j){
                auto f0=f[i+j],f1=f[i+j+L]*r1;
                auto f2=f[i+j+L*2]*r2,f3=f[i+j+L*3]*r3;
                auto g0=f0+f2,g1=f1+f3;
                auto g2=f0-f2,g3=subI(f1,f3);
                f[i+j+L*0]=g0+g1,f[i+j+L*1]=g0-g1;
                f[i+j+L*2]=g2+g3,f[i+j+L*3]=g2-g3;
            }
        }
    }
}
void _vifft(vcpx*f,idt n){
    for(idt i=0;i+3<n;i+=4){
        int tl=__builtin_ctzll(i+4);
        //std::cout<<i<<" "<<tl<<std::endl;
        for(int t=2;t<=tl;t+=2){
            idt l=idt(1)<<t,L=l>>2,b=(i-l+4);
            auto rr=roselia[(i+n*2)>>t];
            auto r1=expand_odd(rr),r2=select_and_expand<1>(rr),r3=select_and_expand<3>(rr);
            for(idt j=0;j<L;++j){
                auto f0=f[b+j],f1=f[b+j+L];
                auto f2=f[b+j+L*2],f3=f[b+j+L*3];    
                auto g0=f0+f1,g1=f0-f1;
                auto g2=f2+f3,g3=subI(f3,f2);
                f[b+j]=g0+g2,f[b+j+L]=mulT(g1+g3,r1);
                f[b+j+L*2]=mulT(g0-g2,r2),f[b+j+L*3]=mulT(g1-g3,r3);
            }
        }
    }
    if(__builtin_ctzll(n)&1){
        idt l=n/2;
        const auto omega=expand_odd(roselia[1]);
        for(idt j=0;j<l;++j){
            auto x=f[j],y=f[l+j];
            f[j]=x+y,f[l+j]=mulT(x-y,omega);
        }
    }
}
[[gnu::always_inline]] inline void conv_mod_x4_w_4(vcpx*a,const vcpx*b,vcpx w){
    alignas(64) f64 awa[4][16];
#define KIL(t) {\
    auto aw=a[t]*select_and_expand<t>(w);\
    _mm256_store_pd(awa[t],aw.x),_mm256_store_pd(awa[t]+4,a[t].x);\
    _mm256_store_pd(awa[t]+8,aw.y),_mm256_store_pd(awa[t]+12,a[t].y);\
}
    KIL(0)KIL(1)KIL(2)KIL(3)
#undef KIL
    for(int j=0;j<4;++j){
        auto bi=select_and_expand<0>(b[j]);
        a[j]=bi*a[j];
    }
#define JOK(t) {\
    for(int j=0;j<4;++j){\
        auto bi=select_and_expand<t>(b[j]);\
        auto aj=vcpx(_mm256_loadu_pd(awa[j]+4-t),_mm256_loadu_pd(awa[j]+12-t));\
        a[j]=fma(bi,aj,a[j]);\
    }\
}
    JOK(1)JOK(2)JOK(3)
#undef JOK
}
inline void _vdot(vcpx*f,const vcpx*g,idt n){
    for(idt i=0;i<n;i+=4){
        conv_mod_x4_w_4(f+i,g+i,rotate_expand_odd(roselia[(i+n*2)>>2]));
    }
}
void test1(){
    roselia.reserve(1<<17);
    auto lm=idt(1)<<6;
    auto f=alc<f64>(lm),g=alc<f64>(lm);
    clr(f,lm),clr(g,lm);
    f[0]=f[1]=f[2]=f[3]=1;
    g[0]=1,g[1]=1,g[2]=1,g[3]=1,g[4]=1;
    _vfft((vcpx*)f,lm>>3);
    _vfft((vcpx*)g,lm>>3);
    _vdot((vcpx*)f,(vcpx*)g,lm>>3);
    _vifft((vcpx*)f,lm>>3);
    for(int i=0;i<4;++i){
        cout<<f[i]<<' '<<f[i+4]<<'\n';
    }
    for(int i=0;i<4;++i){
        cout<<f[i+8]<<' '<<f[i+12]<<'\n';
    }
}
inline void to_poi(const u32*a,idt n,vcpx*f0,vcpx*f1,idt lm){
    auto vextract=[&](const u32*a,idt p){
        static const __m128i mask=_mm_set_epi32(32767,32767,32767,32767);
        auto aa=_mm_load_si128((const __m128i*)a);
        _mm256_store_pd(((f64*)f0)+p,_mm256_cvtepi32_pd(_mm_and_si128(aa,mask)));
        _mm256_store_pd(((f64*)f1)+p,_mm256_cvtepi32_pd(_mm_srli_epi32(aa,15)));
    };
    auto extract=[&](const u32*a,idt p){
        *(((f64*)f0)+p)=int(*a)&32767;
        *(((f64*)f1)+p)=int(*a)>>15;
    };
    //and _mm256_cvtepu32_epi64?
    idt i=0,p=0;
    for(;i+3<std::min(n,lm);i+=4,p+=8){
        vextract(a+i,p);
    }
    if(n>lm){
        for(p=4;i+3<n;i+=4,p+=8){
            vextract(a+i,p);
        }
    }
    for(;i<n;++i,++p){
        extract(a+i,p);
    }
}
constexpr u32 mod=1e9+7;
/*
floor(x/y)
floor(x*im / B^n)

*/
struct barrett{
    u32 mod;
    int k;
    u32 im;
    //mod > 1
    constexpr barrett(u32 _mod):mod(_mod),k(std::__lg(mod-1)+32),im((u64(1)<<k)/mod){}
    constexpr u32 operator()(u64 x)const{
        u32 p=(__uint128_t(x)*im)>>k;
        u32 r=x-p*mod;
        return std::min(r-mod,r);
    }
};//It's bad, I know.
struct vbarrett{
    I256 mod,im;
    int k;
    vbarrett(barrett bt):mod(_mm256_set1_epi64x(bt.mod)),im(_mm256_set1_epi64x(bt.im)),k(bt.k-32){}
    I256 operator()(I256 x)const{
        auto hi=_mm256_mul_epu32(_mm256_srli_epi64(x,32),im);
        auto lo=_mm256_srli_epi64(_mm256_mul_epu32(x,im),32);
        auto r=_mm256_sub_epi64(x,_mm256_mul_epu32(_mm256_srli_epi64(_mm256_add_epi64(lo,hi),k),mod));
        return _mm256_min_epu32(r,_mm256_sub_epi32(r,mod));
    }
};//but this is good.
//https://judge.yosupo.jp/submission/285631
//from https://stackoverflow.com/a/77376595
inline I256 _round(f64x4 x){
    static const f64x4 magic=_mm256_set1_pd(3ll<<51);
    return _mm256_castpd_si256(x+magic)-_mm256_castpd_si256(magic);
}
inline void un_poi(u32*a,idt n,vcpx*f0,vcpx*f1,vcpx*f2,idt lm){
    static const auto ad=(1u<<30)%mod;
    static const auto vad=_mm256_set1_epi64x(ad);
    static const auto idx=_mm256_setr_epi32(0,2,4,6,0,0,0,0);
    static const auto vbt=vbarrett(mod);
    auto vextract=[&](u32*a,idt p){
        auto x0=_round(_mm256_load_pd(((f64*)f0)+p));
        auto x1=_round(_mm256_load_pd(((f64*)f1)+p));
        auto x2=_round(_mm256_load_pd(((f64*)f2)+p));
        auto res=vbt(_mm256_add_epi64(_mm256_mul_epu32(vbt(x2),vad),_mm256_add_epi64(x0,_mm256_slli_epi64(vbt(x1),15))));
        _mm_store_si128((__m128i*)a,_mm256_castsi256_si128(_mm256_permutevar8x32_epi32(res,idx)));
    };
    auto extract=[&](u32*a,idt p){
        *a=(u64(i64(*(((f64*)f0)+p)+0.5))+((u64(i64(*(((f64*)f1)+p)+0.5))%mod)<<15)+(u64(i64(*(((f64*)f2)+p)+0.5))%mod)*ad)%mod;
    };
    idt i=0,p=0;
    for(;i<lm;i+=4,p+=8){
        vextract(a+i,p);
    }
    for(p=4;i+3<n;i+=4,p+=8){
        vextract(a+i,p);
    }
    for(;i<n;++i,++p){
        extract(a+i,p);
    }
}
struct timer{
    std::string nam;
	std::chrono::system_clock::time_point lst;
	timer(std::string name):nam(name),lst(std::chrono::system_clock::now()){
		
	}
	void report(){
		std::chrono::duration<long double,std::milli> tott=std::chrono::system_clock::now()-lst;
		char bbuf[24];
		snprintf(bbuf,24,"%.6Lf",tott.count());
		std::clog<<nam<<":"<<bbuf<<"ms"<<std::endl;
	}
};
//https://codeforces.com/blog/entry/14286
template <typename T>
class big_alloc: public std::allocator<T> {
    public:
    using value_type = T;
    using base = std::allocator<T>;

    big_alloc() noexcept = default;

    template <typename U>
    big_alloc(const big_alloc<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if(n * sizeof(T) < 1024 * 1024) {
            return base::allocate(n);
        }
        n *= sizeof(T);
        void* raw = mmap(nullptr, n,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);
        madvise(raw, n, MADV_HUGEPAGE);
        madvise(raw, n, MADV_POPULATE_WRITE);
        return static_cast<T*>(raw);
    }

    void deallocate(T* p, std::size_t n) noexcept {
        if(n * sizeof(T) < 1024 * 1024) {
            return base::deallocate(p, n);
        }
        if(p) {
            munmap(p, n * sizeof(T));
        }
    }
};
using cvec=std::vector<vcpx,big_alloc<vcpx> >;
void conv(const u32*a,idt n,const u32*b,idt m,u32*c){
    clr(c,n+m-1);
    if(std::min(n,m)<=16){
        for(idt i=0;i<n;++i){
            for(idt j=0;j<m;++j){
                c[i+j]=(c[i+j]+a[i]*u64(b[j]))%mod;
            }
        }
        return;
    }
    idt lm=bcl(n+m-1)/2,vlm=lm/4;
    roselia.reserve(vlm);
    timer ot("alc & clr");
    cvec _a0(vlm),_a1(vlm),_b0(vlm),_b1(vlm); // test, alloc have some problems.
    vcpx*a0=_a0.data(),*a1=_a1.data(),*b0=_b0.data(),*b1=_b1.data();
    ot.report();
    timer ot2("to_poi & fft");
    to_poi(a,n,a0,a1,lm);
    _vfft(a0,vlm),_vfft(a1,vlm);
    to_poi(b,m,b0,b1,lm);
    _vfft(b0,vlm),_vfft(b1,vlm);
    ot2.report();
    const f64x4 fx=_mm256_set1_pd(1./vlm);
#define KIL(t) {\
    auto B0t=select_and_expand<t>(B0),B1t=select_and_expand<t>(B1);\
    auto A0t=vcpx(_mm256_loadu_pd(bufr0+4-t),_mm256_loadu_pd(bufi0+4-t));\
    auto A1t=vcpx(_mm256_loadu_pd(bufr1+4-t),_mm256_loadu_pd(bufi1+4-t));\
    res00=fma(A0t,B0t,res00);\
    res10=fma(A1t,B0t,res10);\
    res01=fma(A0t,B1t,res01);\
    res11=fma(A1t,B1t,res11);\
}
#define calc(i,w) {\
    alignas(64) f64 bufr0[8],bufi0[8],bufr1[8],bufi1[8];\
    auto A0=a0[i],A1=a1[i],B0=mulR(b0[i],fx),B1=mulR(b1[i],fx); \
    auto A0w=A0*w,A1w=A1*w;\
    _mm256_store_pd(bufr0,A0w.x),_mm256_store_pd(bufr0+4,A0.x);\
    _mm256_store_pd(bufi0,A0w.y),_mm256_store_pd(bufi0+4,A0.y);\
    _mm256_store_pd(bufr1,A1w.x),_mm256_store_pd(bufr1+4,A1.x);\
    _mm256_store_pd(bufi1,A1w.y),_mm256_store_pd(bufi1+4,A1.y);\
    auto B00=select_and_expand<0>(B0),B10=select_and_expand<0>(B1);\
    auto res00=A0*B00;\
    auto res10=A1*B00;\
    auto res01=A0*B10;\
    auto res11=A1*B10;\
    KIL(1)KIL(2)KIL(3)\
    a0[i]=res00;\
    a1[i]=res01+res10;\
    b0[i]=res11;\
    }
    for(idt i=0;i<vlm;i+=4){
        auto omega=rotate_expand_odd(roselia[(i+vlm*2)>>2]);
        calc(i+0,select_and_expand<0>(omega));
        calc(i+1,select_and_expand<1>(omega));
        calc(i+2,select_and_expand<2>(omega));
        calc(i+3,select_and_expand<3>(omega));
        //cout<<omega<<'\n';
    }
#undef calc
 #undef KIL  
    // cout<<"a0:\n";
    // for(idt i=0;i<vlm;++i){
    //     cout<<a0[i]<<'\n';
    // }
    timer ot3("ifft & un_poi");
    _vifft(a0,vlm),_vifft(a1,vlm),_vifft(b0,vlm);
    un_poi(c,n+m-1,a0,a1,b0,lm);
    ot3.report();
}
void work(){
    idt n,m;
    qin>>n>>m;
    //default alloc aligned with 16, so it's ok.
    std::vector<u32> a(n),b(m),c(n+m-1);
    u32 k=1+rng()%(mod-1);
    u32 kn=k;
    for(auto&x:a){qin>>x,x=u64(x)*kn%mod,kn=u64(k)*kn%mod;}
    kn=k;
    for(auto&x:b){qin>>x,x=u64(x)*kn%mod,kn=u64(k)*kn%mod;}
    timer ot("total");
    conv(a.data(),n,b.data(),m,c.data());
    ot.report();
    u32 ik=[](u32 a,u32 b=mod-2,u32 r=1){
        for(;b;b/=2,a=(u64(a)*a)%mod){b&1?r=(u64(r)*a)%mod:r;}return r;
    }(k),ikn=u64(ik)*ik%mod;
    // cout<<k<<" "<<ik<<" "<<(k*u64(ik)%mod)<<"\n";
    for(auto x:c){qout<<u32((u64(x)*ikn)%mod)<<' ',ikn=u64(ikn)*ik%mod;}
}
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    //freopen("test.in","r",stdin);
    __yzlf::work();
    return 0;
}

/*
20 20
10000000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
10000000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
*/