#include <bitset>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
using u32=unsigned;
using i64=long long;
using u64=unsigned long long;
namespace __yzlf{
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
using __yzlf::qin;
using __yzlf::qout;
constexpr u32 mod=998244353u;
std::vector<u32> sieve_prime(u32 N){
    std::bitset<10000001> ntp={};
    std::vector<u32> r;
    r.reserve(100000);
    u32 i=2;
    for(;i*i<=N;++i){
        if(!ntp[i]){
            r.emplace_back(i);
            for(u32 j=i*i;j<=N;j+=i){ntp[j]=true;}
        }
    }
    for(;i<=N;++i){
        if(!ntp[i]){r.emplace_back(i);}
    }
    return r;
}
constexpr u32&add(u32&x,u32 y){return x+=y,x=std::min(x,x-mod);}
constexpr u32&sub(u32&x,u32 y){return x-=y,x=std::min(x,x+mod);}
constexpr u32 mul(u32 x,u32 y){return u64(x)*y%mod;}
void solve(){
    u32 N;
    qin>>N;
    std::vector<u32> A(N+1),B(N+1),pr=sieve_prime(N);
    for(u32 i=1;i<=N;++i){qin>>A[i];}
    for(u32 i=1;i<=N;++i){qin>>B[i];}
    for(auto p:pr){
        for(u32 i=N/p,j=i*p;i>0;--i,j-=p){add(A[i],A[j]),add(B[i],B[j]);}
    }
    for(u32 i=1;i<=N;++i){A[i]=mul(A[i],B[i]);}
    for(auto p:pr){
        for(u32 i=1,j=p;j<=N;++i,j+=p){sub(A[i],A[j]);}
    }
    for(u32 i=1;i<=N;++i){qout<<A[i]<<' ';}
}
int main(){
    solve();
    return 0;
}