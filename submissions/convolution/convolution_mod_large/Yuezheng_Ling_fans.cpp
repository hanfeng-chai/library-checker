#pragma GCC target("avx2")
#include <immintrin.h>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
/*
我想要太多太多灿如朝日,光芒透入方寸鱼池,
Author:Killer_joke
*/
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
using i32=int32_t;
using i64=int64_t;
using u32=uint32_t;
using u64=uint64_t;
using idt=std::size_t;
using I128=__m128i;
using I256=__m256i;
using std::cin;
using std::cout;
inline void store256(void*p,I256 x){
    _mm256_store_si256((I256*)p,x);
}
inline I256 load256(const void*p){
    return _mm256_load_si256((const I256*)p);
}
inline I256 loadu256(const void*p){
    return _mm256_loadu_si256((const __m256i_u*)p);
}
inline void print_as_u32x8(I256 x){
    alignas(32) std::array<u32,8> ar;
    store256(&ar,x);
    for(int i=0;i<8;++i){
        std::cout<<ar[i]<<" \n"[i+1==8];
    }
}
inline void print_as_u64x4(I256 x){
    alignas(32) std::array<u64,4> ar;
    store256(&ar,x);
    for(int i=0;i<4;++i){
        std::cout<<ar[i]<<" \n"[i+1==4];
    }
}
//constexpr inline = constexpr.
constexpr u32 shrk(u32 x,u32 M){
    return std::min(x,x-M);
}
constexpr u32 dilt(u32 x,u32 M){
    return std::min(x,x+M);
}
constexpr u32 reduce(u64 x,u32 niv,u32 M){
    return (x+u64(u32(x)*niv)*M)>>32;
}
constexpr u32 mul(u32 x,u32 y,u32 niv,u32 M){
    return reduce(u64(x)*y,niv,M);
}
constexpr u32 mul_s(u32 x,u32 y,u32 niv,u32 M){
    return shrk(reduce(u64(x)*y,niv,M),M);
}
constexpr u32 qpw(u32 a,u32 b,u32 niv,u32 M,u32 r){
    for(;b;b>>=1,a=mul(a,a,niv,M)){
        if(b&1){
            r=mul(r,a,niv,M);
        }
    }
    return r;
}
constexpr u32 qpw_s(u32 a,u32 b,u32 niv,u32 M,u32 r){
    return shrk(qpw(a,b,niv,M,r),M);
}
inline I256 shrk32(I256 x,I256 M){
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,M));
}
inline I256 dilt32(I256 x,I256 M){
    return _mm256_min_epu32(x,_mm256_add_epi32(x,M));
}
inline I256 Ladd32(I256 x,I256 y,I256 M){
    return _mm256_add_epi32(x,y);
}
inline I256 Lsub32(I256 x,I256 y,I256 M){
    return _mm256_add_epi32(x,_mm256_sub_epi32(M,y));
}
inline I256 add32(I256 x,I256 y,I256 M){
    return shrk32(_mm256_add_epi32(x,y),M);
}
inline I256 sub32(I256 x,I256 y,I256 M){
    return dilt32(_mm256_sub_epi32(x,y),M);
}
template<int msk>inline I256 neg32_m(I256 x,I256 M){
    return _mm256_blend_epi32(x,_mm256_sub_epi32(M,x),msk);
}
inline I256 reduce(I256 a,I256 b,I256 niv,I256 M){
    I256 c=_mm256_mul_epu32(a,niv),d=_mm256_mul_epu32(b,niv);
    c=_mm256_mul_epu32(c,M),d=_mm256_mul_epu32(d,M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a,c),32),_mm256_add_epi64(b,d),0xaa);
}
inline I256 mul(I256 a,I256 b,I256 niv,I256 M){
    return reduce(_mm256_mul_epu32(a,b),_mm256_mul_epu32(_mm256_srli_epi64(a,32),_mm256_srli_epi64(b,32)),niv,M);
}
inline I256 mul_s(I256 a,I256 b,I256 niv,I256 M){
    return shrk32(mul(a,b,niv,M),M);
}
inline I256 mul_bsm(I256 a,I256 b,I256 niv,I256 M){
    return reduce(_mm256_mul_epu32(a,b),_mm256_mul_epu32(_mm256_srli_epi64(a,32),b),niv,M);
}
//reduce dependency.
inline I256 mul_bsmfxd(I256 a,I256 b,I256 bniv,I256 M){
    I256 cc=_mm256_mul_epu32(a,bniv),dd=_mm256_mul_epu32(_mm256_srli_epi64(a,32),bniv);
    I256 c=_mm256_mul_epu32(a,b),d=_mm256_mul_epu32(_mm256_srli_epi64(a,32),b);
    cc=_mm256_mul_epu32(cc,M),dd=_mm256_mul_epu32(dd,M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(c,cc),32),_mm256_add_epi64(d,dd),0xaa);
}
inline I256 mul_bfxd(I256 a,I256 b,I256 bniv,I256 M){
    I256 cc=_mm256_mul_epu32(a,bniv),dd=_mm256_mul_epu32(_mm256_srli_epi64(a,32),_mm256_srli_epi64(bniv,32));
    I256 c=_mm256_mul_epu32(a,b),d=_mm256_mul_epu32(_mm256_srli_epi64(a,32),_mm256_srli_epi64(b,32));
    cc=_mm256_mul_epu32(cc,M),dd=_mm256_mul_epu32(dd,M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(c,cc),32),_mm256_add_epi64(d,dd),0xaa);
}
inline I256 mul_bsmfxd_cross(I256 a,I256 b,I256 bniv,I256 M){
    I256 cc=_mm256_mul_epu32(_mm256_srli_epi64(a,32),bniv),dd=_mm256_mul_epu32(a,bniv);
    I256 c=_mm256_mul_epu32(_mm256_srli_epi64(a,32),b),d=_mm256_mul_epu32(a,b);
    cc=_mm256_mul_epu32(cc,M),dd=_mm256_mul_epu32(dd,M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(c,cc),32),_mm256_add_epi64(d,dd),0xaa);
}
inline I256 mul_upd_rt(I256 a,I256 bu,I256 M){
    I256 cc=_mm256_mul_epu32(a,bu),c=_mm256_mul_epu32(a,_mm256_srli_epi64(bu,32));
    cc=_mm256_mul_epu32(cc,M);
    return shrk32(_mm256_srli_epi64(_mm256_add_epi64(c,cc),32),M);
}
inline I256 mul_upd_rr(I256 a,I256 bu,I256 M){
    return mul_bsmfxd(a,bu,_mm256_srli_epi64(bu,32),M);
}
template<class T,idt al=32>inline T*alc(idt n){
    return new(std::align_val_t(al))T[n];
}
template<class T,idt al=32>inline void fre(T*p){
    ::operator delete[](p,std::align_val_t(al));
}
template<class T>inline T*cpy(T*f,const T*g,idt n){
    return (T*)memcpy(f,g,n*sizeof(T));
}
template<class T>inline T*clr(T*f,idt n){
    return (T*)memset(f,0,n*sizeof(T));
}
constexpr auto _mxlg=26,_lg_itth=6;
constexpr auto _itth=idt(1)<<_lg_itth;
static_assert(_lg_itth%2==0);
//m < 2**30.
struct FNTT32_info{
    u32 mod,mod2,niv,one,r2,r3,img,imgniv,RT1[_mxlg],RT3[_mxlg];
    alignas(32) std::array<u32,8> rt3[_mxlg-2],rt3i[_mxlg-2],bwb,bwbi;
    u64 rt4n[_mxlg-3],rt4n2[_mxlg-3],rt4ni[_mxlg-3],rt4n2i[_mxlg-3];
    alignas(32) std::array<u32,8> rt4nr,rt4nr2,rt4nri,rt4nr2i;
    constexpr FNTT32_info(const u32 m):
    mod(m),mod2(m*2),niv([&]{u32 n=2+m;for(int i=0;i<4;++i){n*=2+m*n;}return n;}()),
    one((-m)%m),r2((-u64(m))%m),r3(mul_s(r2,r2,niv,m)),
    img{},imgniv{},RT1{},RT3{},rt3{},rt3i{},bwb{},bwbi{},rt4n{},rt4n2{},rt4ni{},rt4n2i{},rt4nr{},rt4nr2{},rt4nri{},rt4nr2i{}{
        const int k=__builtin_ctz(m-1);
		u32 _g=mul(3,r2,niv,mod);
        for(;;++_g){
            if(qpw_s(_g,mod>>1,niv,mod,one)!=one){
                break;
            }
        }
		_g=qpw(_g,mod>>k,niv,mod,one);
        u32 rt1[_mxlg-1]={},rt1i[_mxlg-1]={};
        rt1[k-2]=_g,rt1i[k-2]=qpw(_g,mod-2,niv,mod,one);
        for(int i=k-2;i>0;--i){
            rt1[i-1]=mul(rt1[i],rt1[i],niv,mod);
            rt1i[i-1]=mul(rt1i[i],rt1i[i],niv,mod);
        }
        RT1[k-1]=qpw_s(_g,3,niv,mod,one);
        for(int i=k-1;i>0;--i){
			RT1[i-1]=mul_s(RT1[i],RT1[i],niv,mod);
        }
        img=rt1[0],imgniv=img*niv;
        bwb={rt1[1],0,rt1[0],0,mod-mul_s(rt1[0],rt1[1],niv,mod)};
        bwbi={rt1i[1],0,rt1i[0],0,mul_s(rt1i[0],rt1i[1],niv,mod)};
        u32 pr=one,pri=one;
        for(int i=0;i<k-2;++i){
            const u32 r=mul_s(pr,rt1[i+1],niv,mod),ri=mul_s(pri,rt1i[i+1],niv,mod);
            const u32 r2=mul_s(r,r,niv,mod),r2i=mul_s(ri,ri,niv,mod);
            const u32 r3=mul_s(r,r2,niv,mod),r3i=mul_s(ri,r2i,niv,mod);
            rt3[i]={r*niv,r,r2*niv,r2,r3*niv,r3};
            RT3[i+2]=rt3[i][1];
            rt3i[i]={ri*niv,ri,r2i*niv,r2i,r3i*niv,r3i};
            pr=mul(pr,rt1i[i+1],niv,mod),pri=mul(pri,rt1[i+1],niv,mod);
        }
        u32 w[8]={},wi[8]={};
        w[0]=one,wi[0]=one;
        for(int i=0;i<3;++i){
            pr=rt1[i],pri=rt1i[i];
            for(int j=1<<i,k=0;k<j;++k){
                w[j+k]=mul_s(w[k],pr,niv,mod);
                wi[j+k]=mul_s(wi[k],pri,niv,mod);
            }
        }
        rt4nr={w[7],w[0],w[6],w[1],w[5],w[2],w[4],w[3]};
        rt4nr2={w[1],w[3],w[1],w[0],w[0],w[2],w[0],w[1]};
        rt4nri={wi[7],wi[0],wi[6],wi[1],wi[5],wi[2],wi[4],wi[3]};
        rt4nr2i={wi[1],wi[3],wi[1],wi[0],wi[0],wi[2],wi[0],wi[1]};
        pr=one,pri=one;
        for(int i=1;i<k-3;++i){
            const u32 r=mul_s(pr,rt1[i+2],niv,mod),ri=mul_s(pri,rt1i[i+2],niv,mod);
            const u32 r2=mul_s(r,r,niv,mod),r2i=mul_s(ri,ri,niv,mod);
            const u32 r4=mul_s(r2,r2,niv,mod),r4i=mul_s(r2i,r2i,niv,mod);
            rt4n[i]=u64(r*niv)<<32|r,rt4ni[i]=u64(ri*niv)<<32|ri;
            rt4n2[i]=u64(r2)<<32|r4,rt4n2i[i]=u64(r2i)<<32|r4i;
            pr=mul(pr,rt1i[i+2],niv,mod),pri=mul(pri,rt1[i+2],niv,mod);
        }
    }
};
inline void __vec_dif(I256*const f,const idt n,const FNTT32_info*const info){
    alignas(32) std::array<u32,8> st_1[_mxlg>>1];
    const I256 Mod=_mm256_set1_epi32(info->mod),Mod2=_mm256_set1_epi32(info->mod2),Niv=_mm256_set1_epi32(info->niv);
    const I256 Img=_mm256_set1_epi32(info->img),ImgNiv=_mm256_set1_epi32(info->imgniv);
    const I256 id24=_mm256_setr_epi32(0,2,0,4,0,2,0,4),idr=_mm256_set_epi32(0,1,2,3,4,5,6,7);
    const int lgn=__builtin_ctzll(n);
    std::fill(st_1,st_1+(lgn>>1),info->bwb);
    const idt nn=n>>(lgn&1),m=std::min(n,_itth);
    I256 rr0=load256(&info->rt4nr),rr1=load256(&info->rt4nr2);
    if(nn!=n){
        for(idt i=0;i<nn;++i){
            auto const p0=f+i,p1=f+nn+i;
            const auto f0=load256(p0),f1=load256(p1);
            const auto g0=add32(f0,f1,Mod2),g1=Lsub32(f0,f1,Mod2);
            store256(p0,g0),store256(p1,g1);
        }
    }
    for(idt L=nn>>2;L>0;L>>=2){
        for(idt i=0;i<L;++i){
            auto const p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
            const auto f1=load256(p1),f3=load256(p3),f2=load256(p2),f0=load256(p0);
            const auto g3=mul_bsmfxd(Lsub32(f1,f3,Mod2),Img,ImgNiv,Mod),g1=add32(f1,f3,Mod2);
            const auto g0=add32(f0,f2,Mod2),g2=sub32(f0,f2,Mod2);
            const auto h0=add32(g0,g1,Mod2),h1=Lsub32(g0,g1,Mod2);
            const auto h2=Ladd32(g2,g3,Mod2),h3=Lsub32(g2,g3,Mod2);
            store256(p0,h0),store256(p1,h1),store256(p2,h2),store256(p3,h3);
        }
    }
    int t=std::min(_lg_itth,lgn)&-2,p=(t-2)>>1;
    for(idt j=0;j<n;j+=m,t=__builtin_ctzll(j)&-2,p=(t-2)>>1){
        I256*const g=f+j;
        for(idt l=(idt(1)<<t),L=l>>2;L;l=L,L>>=2,t-=2,--p){
            auto rt=load256(st_1+p);
            for(idt i=(j==0?l:0),k=(j+i)>>t;i<m;i+=l,++k){
                const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                const auto r1Niv=_mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt,Niv),id24);
                rt=mul_upd_rt(rt,load256(info->rt3+__builtin_ctzll(~k)),Mod);
                const auto r2=_mm256_shuffle_epi32(r1,_MM_PERM_BBBB),nr3=_mm256_shuffle_epi32(r1,_MM_PERM_DDDD);
                const auto r2Niv=_mm256_shuffle_epi32(r1Niv,_MM_PERM_BBBB),nr3Niv=_mm256_shuffle_epi32(r1Niv,_MM_PERM_DDDD);
                for(idt j=0;j<L;++j){
                    auto const p0=g+i+j,p1=p0+L,p2=p1+L,p3=p2+L;
                    const auto f1=load256(p1),f3=load256(p3),f2=load256(p2),f0=load256(p0);
                    const auto g1=mul_bsmfxd(f1,r1,r1Niv,Mod),ng3=mul_bsmfxd(f3,nr3,nr3Niv,Mod);
                    const auto g2=mul_bsmfxd(f2,r2,r2Niv,Mod),g0=shrk32(f0,Mod2);
                    const auto h3=mul_bsmfxd(Ladd32(g1,ng3,Mod2),Img,ImgNiv,Mod),h1=sub32(g1,ng3,Mod2);
                    const auto h0=add32(g0,g2,Mod2),h2=sub32(g0,g2,Mod2);
                    const auto o0=Ladd32(h0,h1,Mod2),o1=Lsub32(h0,h1,Mod2);
                    const auto o2=Ladd32(h2,h3,Mod2),o3=Lsub32(h2,h3,Mod2);
                    store256(p0,o0),store256(p1,o1),store256(p2,o2),store256(p3,o3);
                }
            }
            store256(st_1+p,rt);
        }
    }
}
inline void __vec_dit(I256*const f,idt n,const FNTT32_info*const info){
    alignas(32) std::array<u32,8> st_1[_mxlg>>1];
    const I256 Mod=_mm256_set1_epi32(info->mod),Mod2=_mm256_set1_epi32(info->mod2),Niv=_mm256_set1_epi32(info->niv);
    const I256 Img=_mm256_set1_epi32(info->img),ImgNiv=_mm256_set1_epi32(info->imgniv);
    const I256 id24=_mm256_setr_epi32(0,2,0,4,0,2,0,4),idr=_mm256_set_epi32(0,1,2,3,4,5,6,7);
    const int lgn=__builtin_ctzll(n);
    std::fill(st_1+1,st_1+(lgn>>1),info->bwbi);
    const idt nn=n>>(lgn&1),m=std::min(n,_itth);
    const auto fx=mul_s(info->mod-((info->mod-1)>>__builtin_ctzll(n)),info->r3,info->niv,info->mod);
    const auto Fx=_mm256_set1_epi32(fx),FxNiv=_mm256_set1_epi32(fx*info->niv);
    store256(st_1,Fx);
    for(idt j=0;j<n;j+=m){
        int tt=__builtin_ctzll(j+m),t=4,p=1;
        {//L == 1,append coefficient
            I256 rt=load256(st_1);
            for(idt i=j;i<j+m;i+=4){
                const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                rt=mul_upd_rt(rt,load256(info->rt3i+__builtin_ctzll(~i>>2)),Mod);
                const auto r2=_mm256_shuffle_epi32(r1,_MM_PERM_BBBB),r3=_mm256_shuffle_epi32(r1,_MM_PERM_DDDD);
                auto const p0=f+i,p1=p0+1,p2=p1+1,p3=p2+1;
                const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                const auto g3=mul_bsmfxd(Lsub32(f3,f2,Mod2),Img,ImgNiv,Mod),g2=add32(f2,f3,Mod2);
                const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                const auto h2=Lsub32(g0,g2,Mod2),h3=Lsub32(g1,g3,Mod2);
                const auto h0=Ladd32(g0,g2,Mod2),h1=Ladd32(g1,g3,Mod2);
                const auto o2=mul_bsm(h2,r2,Niv,Mod),o0=mul_bsmfxd(h0,Fx,FxNiv,Mod);
                const auto o1=mul_bsm(h1,r1,Niv,Mod),o3=mul_bsm(h3,r3,Niv,Mod);
                store256(p0,o0),store256(p1,o1),store256(p2,o2),store256(p3,o3);
            }
            store256(st_1,rt);
        }
        for(idt l=16,L=4;t<=tt;L=l,l<<=2,t+=2,++p){
            idt diff=j+m-std::max(l,m),i=0;
            I256 rt=load256(st_1+p),*const g=f+diff;
            if(diff==0){
                if(l==n){
                    for(idt i=0;i<L;++i){
                        auto const p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
                        const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                        const auto g3=mul_bsmfxd(Lsub32(f3,f2,Mod2),Img,ImgNiv,Mod),g2=add32(f2,f3,Mod2);
                        const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                        const auto h0=add32(g0,g2,Mod2),h1=add32(g1,g3,Mod2);
                        const auto h2=sub32(g0,g2,Mod2),h3=sub32(g1,g3,Mod2);
                        const auto o0=shrk32(h0,Mod),o1=shrk32(h1,Mod);
                        const auto o2=shrk32(h2,Mod),o3=shrk32(h3,Mod);
                        store256(p0,o0),store256(p1,o1),store256(p2,o2),store256(p3,o3);
                    }
                }
                else{
                    for(idt i=0;i<L;++i){
                        auto const p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
                        const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                        const auto g3=mul_bsmfxd(Lsub32(f3,f2,Mod2),Img,ImgNiv,Mod),g2=add32(f2,f3,Mod2);
                        const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                        const auto h0=add32(g0,g2,Mod2),h1=add32(g1,g3,Mod2);
                        const auto h2=sub32(g0,g2,Mod2),h3=sub32(g1,g3,Mod2);
                        store256(p0,h0),store256(p1,h1),store256(p2,h2),store256(p3,h3);
                    }
                }i=l;
            }
            for(idt k=(j+i)>>t;i<m;i+=l,++k){
                const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                const auto r1Niv=_mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt,Niv),id24);
                rt=mul_upd_rt(rt,load256(info->rt3i+__builtin_ctzll(~k)),Mod);
                const auto r2=_mm256_shuffle_epi32(r1,_MM_PERM_BBBB),r3=_mm256_shuffle_epi32(r1,_MM_PERM_DDDD);
                const auto r2Niv=_mm256_shuffle_epi32(r1Niv,_MM_PERM_BBBB),r3Niv=_mm256_shuffle_epi32(r1Niv,_MM_PERM_DDDD);
                for(idt j=0;j<L;++j){
                    auto const p0=g+i+j,p1=p0+L,p2=p1+L,p3=p2+L;
                    const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                    const auto g3=mul_bsmfxd(Lsub32(f3,f2,Mod2),Img,ImgNiv,Mod),g2=add32(f2,f3,Mod2);
                    const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                    const auto h2=Lsub32(g0,g2,Mod2),h3=Lsub32(g1,g3,Mod2);
                    const auto h0=Ladd32(g0,g2,Mod2),h1=Ladd32(g1,g3,Mod2);
                    const auto o2=mul_bsmfxd(h2,r2,r2Niv,Mod),o0=shrk32(h0,Mod2);
                    const auto o1=mul_bsmfxd(h1,r1,r1Niv,Mod),o3=mul_bsmfxd(h3,r3,r3Niv,Mod);
                    store256(p0,o0),store256(p1,o1),store256(p2,o2),store256(p3,o3);
                }
            }
            store256(st_1+p,rt);
        }
    }
    if(nn!=n){
        for(idt i=0;i<nn;++i){
            auto const p0=f+i,p1=f+nn+i;
            const auto f0=load256(p0),f1=load256(p1);
            const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
            const auto h0=shrk32(g0,Mod),h1=shrk32(g1,Mod);
            store256(p0,h0),store256(p1,h1);
        }
    }
}
//f[0,8) = fx * f[0,8) * g[0,8) (mod x^8 - ww)
[[gnu::always_inline]] inline void __conv8(I256*f,const I256*g,I256 ww,I256 fx,I256 Niv,I256 Mod,I256 Mod2){
    const auto raa=load256(f),rbb=load256(g);
    const auto taa=shrk32(raa,Mod2),bb=shrk32(mul_bsm(rbb,fx,Niv,Mod),Mod);
    const auto aw=shrk32(mul_bsm(taa,ww,Niv,Mod),Mod);
    const auto aa=shrk32(taa,Mod);
    const auto awa=_mm256_permute2x128_si256(aa,aw,3);
    
    const auto b0=_mm256_permute4x64_epi64(bb,0x00),b1=_mm256_shuffle_epi32(b0,_MM_PERM_CDAB);
    const auto a0=aa,a1=_mm256_srli_epi64(a0,32);
    const auto aw7=_mm256_alignr_epi8(aa,awa,12);
    auto res00=_mm256_mul_epu32(a0,b0);
    auto res01=_mm256_mul_epu32(a1,b0);
    auto res10=_mm256_mul_epu32(aw7,b1);
    auto res11=_mm256_mul_epu32(a0,b1);

    const auto b2=_mm256_permute4x64_epi64(bb,0x55),b3=_mm256_shuffle_epi32(b2,_MM_PERM_CDAB);
    const auto aw6=_mm256_alignr_epi8(aa,awa,8);
    const auto aw5=_mm256_alignr_epi8(aa,awa,4);
    res00=_mm256_add_epi64(res00,_mm256_mul_epu32(aw6,b2));
    res01=_mm256_add_epi64(res01,_mm256_mul_epu32(aw7,b2));
    res10=_mm256_add_epi64(res10,_mm256_mul_epu32(aw5,b3));
    res11=_mm256_add_epi64(res11,_mm256_mul_epu32(aw6,b3));

    const auto b4=_mm256_permute4x64_epi64(bb,0xaa),b5=_mm256_shuffle_epi32(b4,_MM_PERM_CDAB);
    const auto aw3=_mm256_alignr_epi8(awa,aw,12);
    res00=_mm256_add_epi64(res00,_mm256_mul_epu32(awa,b4));
    res01=_mm256_add_epi64(res01,_mm256_mul_epu32(aw5,b4));
    res10=_mm256_add_epi64(res10,_mm256_mul_epu32(aw3,b5));
    res11=_mm256_add_epi64(res11,_mm256_mul_epu32(awa,b5));

    const auto b6=_mm256_permute4x64_epi64(bb,0xff),b7=_mm256_shuffle_epi32(b6,_MM_PERM_CDAB);
    const auto aw2=_mm256_alignr_epi8(awa,aw,8);
    const auto aw1=_mm256_alignr_epi8(awa,aw,4);
    res00=_mm256_add_epi64(res00,_mm256_mul_epu32(aw2,b6));
    res01=_mm256_add_epi64(res01,_mm256_mul_epu32(aw3,b6));
    res10=_mm256_add_epi64(res10,_mm256_mul_epu32(aw1,b7));
    res11=_mm256_add_epi64(res11,_mm256_mul_epu32(aw2,b7));

    res00=_mm256_add_epi64(res00,res10);
    res01=_mm256_add_epi64(res01,res11);

    store256(f,shrk32(reduce(res00,res01,Niv,Mod),Mod2));
}
inline void __vec_dot(I256*f,const I256*g,idt lm,const FNTT32_info*const info){
    const I256 Mod=_mm256_set1_epi32(info->mod),Niv=_mm256_set1_epi32(info->niv);
    for(idt i=0;i<lm;++i){
        store256(f+i,mul(load256(f+i),load256(g+i),Niv,Mod));
    }
}
inline void __vec_cvdt(I256*f,const I256*g,idt lm,const FNTT32_info*const info){
    u32 RR=info->one;
    const auto mod=info->mod,niv=info->niv;
    const auto Fx=_mm256_set1_epi32(mul_s((mod-((mod-1)>>(__builtin_ctzll(lm)))),info->r3,niv,mod));
    const auto Niv=_mm256_set1_epi32(niv),Mod=_mm256_set1_epi32(mod),Mod2=_mm256_set1_epi32(info->mod2);
    for(idt i=0;i<lm;++i){
        __conv8(f+i,g+i,_mm256_set1_epi32(RR),Fx,Niv,Mod,Mod2);
        RR=mul(RR,info->RT1[__builtin_ctzll(~i)],niv,mod);
    }
}
//f[0,8) = fx * f[0,8) * g[0,8) (mod x^8 - ww)
[[gnu::always_inline]] inline void __conv8_4(u32*__restrict__ f,u32*__restrict__ g,std::array<u32,4> ww,I256 Niv,I256 Mod,I256 Mod2){
    alignas(64) u32 awa[4][16];
    alignas(64) I256 res0[4]={},res1[4]={};
    #pragma GCC unroll(2)
    for(int i=0;i<4;++i){
        store256(g+i*8,shrk32(shrk32(load256(g+i*8),Mod2),Mod));
        auto ff=load256(f+i*8);
        ff=shrk32(ff,Mod2);
        auto ffw=shrk32(mul_bsm(ff,_mm256_set1_epi32(ww[i]),Niv,Mod),Mod);
        ff=shrk32(ff,Mod);
        store256(awa[i],ffw),store256(awa[i]+8,ff);
    }
    for(int i=0;i<8;++i){
        #pragma GCC unroll(4)
        for(int j=0;j<4;++j){
            auto bi=_mm256_set1_epi32(g[j*8+i]);
            auto aj=loadu256(awa[j]+8-i);
            auto aj2=_mm256_srli_epi64(aj,32);
            res0[j]+=_mm256_mul_epu32(bi,aj);
            res1[j]+=_mm256_mul_epu32(bi,aj2);
        }
    }
    #pragma GCC unroll(4)
    for(int i=0;i<4;++i){
        store256(f+i*8,shrk32(reduce(res0[i],res1[i],Niv,Mod),Mod2));
    }
}
void __vec_cvdt8(I256*f,I256*g,idt lm,const FNTT32_info*const info){
    u32 RR=info->one;
    const auto mod=info->mod,niv=info->niv,img=info->img,mod2=info->mod2;
    const auto Niv=_mm256_set1_epi32(niv),Mod=_mm256_set1_epi32(mod),Mod2=_mm256_set1_epi32(info->mod2);
    for(idt i=0;i<lm;i+=4){
        const u32 RRi=mul(RR,img,niv,mod);
        __conv8_4((u32*)(f+i),(u32*)(g+i),{RR,mod2-RR,RRi,mod2-RRi},Niv,Mod,Mod2);
        RR=mul(RR,info->RT3[__builtin_ctzll(i+4)],niv,mod);
    }
}
struct auto_timer{
	std::chrono::system_clock::time_point lst;
	auto_timer():lst(std::chrono::system_clock::now()){}
	~auto_timer(){
		std::chrono::duration<long double,std::milli> tott=std::chrono::system_clock::now()-lst;
		std::clog<<tott.count()<<"ms"<<std::endl;
	}
};
constexpr idt bcl(idt x){
    return x<2?1:idt(2)<<std::__lg(x-1);
}
inline void work(){
    constexpr u32 mod=998244353;
    constexpr FNTT32_info fnt(mod);
    idt n,m,lm;
    qin>>n>>m;
    lm=bcl(std::max<idt>(64,n+m-1));
    auto f=alc<u32>(lm),g=alc<u32>(lm);
    for(idt i=0;i<n;++i){
        qin>>f[i];
    }
    clr(f+n,lm-n);
    for(idt i=0;i<m;++i){
        qin>>g[i];
    }
    clr(g+m,lm-m);
    {
        auto_timer ot;
        __vec_dif((I256*)f,lm>>3,&fnt);
        __vec_dif((I256*)g,lm>>3,&fnt);
        __vec_cvdt8((I256*)f,(I256*)g,lm>>3,&fnt);
        __vec_dit((I256*)f,lm>>3,&fnt);
    }
    for(idt i=0;i<n+m-1;++i){
        qout<<f[i]<<' ';
    }
    fre(f),fre(g);
}
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    __yzlf::work();
    return 0;
}
//resubmit but with mmap.