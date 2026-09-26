#include <iostream>
#include <cstdint>
#include <cmath>
#include <array>
#include <cstring>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
//ban avx512f :( 
//https://judge.yosupo.jp/submission/189811
namespace __yzlf{
namespace __io{
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
            if((p+s.size())>ed)[[unlikely]]{flush();}
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
using __io::qin;
using __io::qout;
using i64=long long;
using u32=unsigned;
using u64=unsigned long long;
using idt=std::size_t;
using std::cin;
using std::cout;
//if overflow, ret 0
constexpr u64 sf_qpw(u64 a,int b,u64 r=1){
    for(;b;){
        if(b&1){if(__builtin_umulll_overflow(a,r,&r)){return 0;}}
        if(b>>=1){if(__builtin_umulll_overflow(a,a,&a)){return 0;}}
    }
    return r;
}
template<idt _N>constexpr auto make_ary(auto&&op){
    std::array<decltype(op(0)),_N> res;
    for(idt i=0;i<_N;++i){res[i]=op(i);}
    return res;
}
inline u64 k_rt(u64 A,int k){
    static const auto iv=make_ary<29>([&](int i){return std::nextafter(1./(i+3),0.);});
    static const auto pw3=make_ary<32>([&](int i){return sf_qpw(3,i+32)-1;});
    if(A==0||k==1){return A;}
    if(k<32){
        if(k==2){return sqrtl(A);}
        u64 r=i64(std::pow(A,iv[k-3]));
        return r+((sf_qpw(r+1,k)-1)<A);
    }
    if(k>63){return 1;}
    return 1+(A>=(u64(1)<<k))+(A>pw3[k-32]);//ans < 4
}
void work(){
    u32 T;
    qin>>T;
    while(T--){
        u64 A;
        u32 k;
        qin>>A>>k;
        qout<<k_rt(A,k)<<'\n';
    }
}
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    __yzlf::work();
    return 0;
}
//fallout