#include <cstdint>
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>
#include <cassert>
#include <cstring>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#pragma GCC target("avx2")
//乐正司百曲,绫动万年红.
//sto EI orz
namespace __yzlf{
using i64=int64_t;
using u8=uint8_t;
using u16=uint16_t;
using u32=uint32_t;
using u64=uint64_t;
using I256=__m256i;
using idt=std::size_t;
constexpr u32 mod=998244353;
constexpr u32 niv=[]{u32 n=2+mod;for(int i=0;i<4;++i){n*=2+mod*n;}return n;}();
constexpr u32 R2=-u64(mod)%mod;
constexpr i64 ngm=i64(1)<<63;
constexpr u32 reduce(u64 x,u32 niv,u32 M){
    return (x+u64(u32(x)*niv)*M)>>32;
}
constexpr u32 shrk32(u32 x,u32 M){
    return std::min(x,x-M);
}
inline I256 reduce(I256 a,I256 b,I256 niv,I256 M){
    I256 kil=_mm256_mul_epu32(a,niv),jok=_mm256_mul_epu32(b,niv);
    kil=_mm256_mul_epu32(kil,M),jok=_mm256_mul_epu32(jok,M);
    return _mm256_blend_epi32(_mm256_srli_epi64(_mm256_add_epi64(a,kil),32),_mm256_add_epi64(b,jok),0xaa);
}
inline I256 mul_sm(I256 a,I256 b,I256 niv,I256 M){
    return reduce(_mm256_mul_epu32(a,b),_mm256_mul_epu32(_mm256_srli_epi64(a,32),b),niv,M);
}
inline I256 shrk32(I256 x,I256 M){
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,M));
}
inline I256 dilt32(I256 x,I256 M){
    return _mm256_min_epu32(x,_mm256_add_epi32(x,M));
}
inline I256 shrk64(I256 x,I256 M){
    //return _mm256_min_epu64(x,_mm256_sub_epi64(x,M)); if avx512
    return _mm256_sub_epi64(x,_mm256_andnot_si256(_mm256_cmpgt_epi64(M,x),M));//M > x ? 0 : M
}

//a:[BLK x BLK] b:[BLK x BLK] c:[BLK x BLK] 
//BLK must be multiple of 8.
//a * b -> c
//trans : none
//montgo : -1
constexpr idt BLK=64;
#define UNR4(d) d(0)d(1)d(2)d(3)
#define UNR8(d) d(0)d(1)d(2)d(3)d(4)d(5)d(6)d(7)
#define INIT_R(p) I256 r##p=_mm256_setzero_si256(),R##p=_mm256_setzero_si256();
#define YZLF_WORK(p) r##p=_mm256_add_epi64(r##p,_mm256_mul_epu32(_mm256_set1_epi32(a[(i+p)*BLK+w]),z0)),R##p=_mm256_add_epi64(R##p,_mm256_mul_epu32(_mm256_set1_epi32(a[(i+p)*BLK+w]),Z0));
#define R_SHRK(p) r##p=_mm256_sub_epi64(r##p,_mm256_min_epu32(M2,_mm256_and_si256(Ngm,r##p))),R##p=_mm256_sub_epi64(R##p,_mm256_min_epu32(M2,_mm256_and_si256(Ngm,R##p)));
#define REDUCE(p) _mm256_store_si256((I256*)(c+(i+p)*BLK+j),shrk32(shrk32(reduce(r##p,R##p,Niv,M),M2),M));
[[gnu::noinline]] void __kernel_1(const u32*__restrict__ a,const u32*__restrict__ b,u32*__restrict__ c){
    const I256 M=_mm256_set1_epi32(mod),M2=_mm256_set1_epi32(mod*2);
    const I256 Niv=_mm256_set1_epi32(niv),Ngm=_mm256_set1_epi64x(ngm);
    //You do know nothing nothing about the real powerrrr!!!
    for(idt i=0;i<BLK;i+=8){
        for(idt j=0;j<BLK;j+=8){
            UNR8(INIT_R)
            for(idt k=0;k<BLK;k+=8){
                for(idt w=k;w<k+8;++w){
                    const I256 z0=_mm256_load_si256((I256*)(b+w*BLK+j)),Z0=_mm256_srli_epi64(z0,32);
                    UNR8(YZLF_WORK)
                }
                UNR8(R_SHRK)
            }
            UNR8(REDUCE)
        }
    }
}
#undef INIT_R
#undef YZLF_WORK
#undef R_SHRK
#undef REDUCE
#undef UNR4
#undef UNR8
alignas(32) unsigned int A[(1<<22)/3],B[(1<<22)/3],C[(1<<22)/3];
/*
0,1  4,5
2,3  6,7

8,9  c,d
a,b  e,f
*/
constexpr idt bcl(idt x){
    return x<2?1:idt(2)<<std::__lg(x-1);
}
void __place_mat(idt x,idt y,idt n,idt N,const u32*__restrict__ a,u32*__restrict__ A){
    if(n==BLK){ 
        for(idt i=0;i<BLK;++i){
            memcpy(A+i*BLK,a+(y+i)*N+x,BLK*sizeof(u32));
        }
        return;
    }
    idt nn=n/2,D=nn*nn;
    __place_mat(x,y,nn,N,a,A);
    __place_mat(x+nn,y,nn,N,a,A+D);
    __place_mat(x,y+nn,nn,N,a,A+D*2);
    __place_mat(x+nn,y+nn,nn,N,a,A+D*3);
}
//100~ ms
void __naive_matmul_rec(const u32*__restrict__ a,const u32*__restrict__ b,u32*__restrict__ c,idt n){
    if(n==BLK){
        __kernel_1(a,b,c);
        return;
    }
    idt nn=n/2,D=nn*nn;
    auto add=[&](u32*a,const u32*b){
        const I256 M=_mm256_set1_epi32(mod);
        for(idt i=0;i<D;i+=8){
            _mm256_store_si256((I256*)(a+i),shrk32(_mm256_add_epi32(_mm256_load_si256((I256*)(a+i)),_mm256_load_si256((I256*)(b+i))),M));
        }
    };
    __naive_matmul_rec(a,b,c,nn);//a*e
    __naive_matmul_rec(a+D,b+D*2,c+D,nn);//b*g
    add(c,c+D);
    __naive_matmul_rec(a,b+D,c+D,nn);//a*f
    __naive_matmul_rec(a+D,b+D*3,c+D*2,nn);//b*h
    add(c+D,c+D*2);
    __naive_matmul_rec(a+D*2,b,c+D*2,nn);//c*e
    __naive_matmul_rec(a+D*3,b+D*2,c+D*3,nn);//d*g
    add(c+D*2,c+D*3);
    __naive_matmul_rec(a+D*2,b+D,c+D*3,nn);//c*f
    __naive_matmul_rec(a+D*3,b+D*3,c+D*4,nn);//d*h
    add(c+D*3,c+D*4);
}
void __place_mat_anti_fx(idt x,idt y,idt n,idt N,u32*__restrict__ a,const u32*__restrict__ A){
    if(n==BLK){ 
        const I256 R2x8=_mm256_set1_epi32(R2),Niv=_mm256_set1_epi32(niv),M=_mm256_set1_epi32(mod);
        for(idt i=0;i<BLK;++i){
            for(idt j=0;j<BLK;j+=8){
                _mm256_store_si256((I256*)(a+(y+i)*N+x+j),shrk32(mul_sm(_mm256_load_si256((I256*)(A+i*BLK+j)),R2x8,Niv,M),M));
            }
        }
        return;
    }
    idt nn=n/2,D=nn*nn;
    __place_mat_anti_fx(x,y,nn,N,a,A);
    __place_mat_anti_fx(x+nn,y,nn,N,a,A+D);
    __place_mat_anti_fx(x,y+nn,nn,N,a,A+D*2);
    __place_mat_anti_fx(x+nn,y+nn,nn,N,a,A+D*3);
}
//75~ ms
void __Strassen_matmul(u32*__restrict__ a,u32*__restrict__ b,u32*__restrict__ c,idt n,u32*__restrict__ aa,u32*__restrict__ bb){
    if(n==BLK){
        __kernel_1(a,b,c);
        return;
    }
    idt nn=n/2,D=nn*nn;
    u32*cc=c+n*n;
    idt i00=0,i01=D,i10=D*2,i11=D*3;
    auto add=[&](u32*a,const u32*b,const u32*c){
        const I256 M=_mm256_set1_epi32(mod);
        for(idt i=0;i<D;i+=8){
            _mm256_store_si256((I256*)(a+i),shrk32(_mm256_add_epi32(_mm256_load_si256((I256*)(b+i)),_mm256_load_si256((I256*)(c+i))),M));
        }
    };
    auto sub=[&](u32*a,const u32*b,const u32*c){
        const I256 M=_mm256_set1_epi32(mod);
        for(idt i=0;i<D;i+=8){
            _mm256_store_si256((I256*)(a+i),dilt32(_mm256_sub_epi32(_mm256_load_si256((I256*)(b+i)),_mm256_load_si256((I256*)(c+i))),M));
        }
    };
    sub(aa,a+i01,a+i11);
    add(bb,b+i10,b+i11);
    __Strassen_matmul(aa,bb,c+i00,nn,aa+D,bb+D);

    add(aa,a+i00,a+i01);
    __Strassen_matmul(aa,b+i11,c+i01,nn,aa+D,bb+D);
    sub(c+i00,c+i00,c+i01);

    sub(bb,b+i10,b+i00);
    __Strassen_matmul(a+i11,bb,c+i10,nn,aa+D,bb+D);
    add(c+i00,c+i00,c+i10);

    sub(aa,a+i10,a+i00);
    add(bb,b+i00,b+i01);
    __Strassen_matmul(aa,bb,c+i11,nn,aa+D,bb+D);

    add(aa,a+i10,a+i11);
    __Strassen_matmul(aa,b+i00,cc,nn,aa+D,bb+D);
    add(c+i10,c+i10,cc);
    sub(c+i11,c+i11,cc);

    add(aa,a+i00,a+i11);
    add(bb,b+i00,b+i11);
    __Strassen_matmul(aa,bb,cc,nn,aa+D,bb+D);
    add(c+i00,c+i00,cc);
    add(c+i11,c+i11,cc);

    sub(bb,b+i01,b+i11);
    __Strassen_matmul(a+i00,bb,cc,nn,aa+D,bb+D);
    add(c+i01,c+i01,cc);
    add(c+i11,c+i11,cc);
}
void mul_Strassen(const u32*__restrict__ a,const u32*__restrict__ b,u32*__restrict__ c,idt N){
    __place_mat(0,0,N,N,a,A);
    __place_mat(0,0,N,N,b,B);
    auto bg=clock();
    __Strassen_matmul(A,B,C,N,A+N*N,B+N*N);
    auto ed=clock();
    std::clog<<"Strassen:"<<(ed-bg)<<'\n';
    __place_mat_anti_fx(0,0,N,N,c,C);
}
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
		bg=(char*)mmap(0,Fl.st_size+1,PROT_READ,MAP_PRIVATE,fd,0);
		p=bg,ed=bg+Fl.st_size;
	}
	~Qinf(){
		munmap(bg,Fl.st_size+1);
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
		return *this;
	}
	Qoutf&operator<<(u64 x){
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
		return *this;
	}
	Qoutf&operator<<(char ch){
		*p++=ch;
		return *this;
	}
    private:
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
using __yzlf::idt;
using __yzlf::qin;
using __yzlf::qout;
std::mt19937_64 jok(0x66ccff);
inline int rnd_range(int l,int r){
    std::uniform_int_distribution<int> kil(l,r);
    return kil(jok);
}
alignas(32) unsigned int a[1<<20],b[1<<20],c[1<<20];
void solve(){
    idt n,m,p;
    qin>>n>>m>>p;
    auto N=std::max(__yzlf::BLK,__yzlf::bcl(std::max({n,m,p})));
    for(int i=0;i<n;++i){
        for(int j=0,x;j<m;++j){
            qin>>a[i*N+j];
        }
    }
    for(int i=0;i<m;++i){
        for(int j=0,x;j<p;++j){
            qin>>b[i*N+j];
        }
    }
    auto bg=clock();
    __yzlf::mul_Strassen(a,b,c,N);
    auto ed=clock();
    std::clog<<"work:"<<(ed-bg)<<'\n';
    for(int i=0;i<n;++i){
        for(int j=0;j<p;++j){
            qout<<c[i*N+j]<<" \n"[j+1==p];
        }
    }
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}