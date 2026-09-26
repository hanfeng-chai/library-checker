#include<bits/stdc++.h>
using u32=unsigned;
using i64=long long;
using u64=unsigned long long;
#include <sys/mman.h>
#include <sys/stat.h>
namespace QIO_base{
    constexpr int O_buffer_default_size = 1 << 18;
	constexpr int O_buffer_default_flush_threshold = 40;
	constexpr u64 E16 = 1e16, E12 = 1e12;
	constexpr u32 E8 = 1e8, E4 = 1e4;
	struct ict{
		int num[10000];
		constexpr ict(){
			int j = 0;
			for(int e0 = (48 << 0); e0 < (58 << 0); e0 += (1 << 0)){
				for(int e1 = (48 << 8); e1 < (58 << 8); e1 += (1 << 8)){
					for(int e2 = (48 << 16); e2 < (58 << 16); e2 += (1 << 16)){
						for(int e3 = (48 << 24); e3 < (58 << 24); e3 += (1 << 24)){
							num[j] = e0 ^ e1 ^ e2 ^ e3, ++j;
						}
					}
				}
			}
		}
	}constexpr ot;
}
namespace QIO_I {
	using namespace QIO_base;
	struct Qinf{
		FILE* f;
		char *bg,*ed,*p;
		struct stat Fl;
		Qinf(FILE *fi) : f(fi){
			int fd = fileno(f);
			fstat(fd, &Fl);
			bg = (char*)mmap(0, Fl.st_size + 1, PROT_READ,MAP_PRIVATE, fd, 0);
			p = bg, ed = bg + Fl.st_size;
		}
		~Qinf(){
			munmap(bg,Fl.st_size + 1);
		}
		void skip_space(){
			while(*p <= ' '){
				++p;
			}
		}
		char get(){
			return *p++;
		}
		char seek()const{
			return *p;
		}
		Qinf& read(char* s, size_t count){
			return memcpy(s, p, count), p += count, *this;
		}
		Qinf& operator >> (u32 &x){
			skip_space(),x=0;
			for(; *p > ' '; ++p){
				x = x * 10 + (*p & 0xf);
			}
			return *this;
		}
	}qin(stdin);
}
namespace QIO_O{
	using namespace QIO_base;
	struct Qoutf{
		FILE *f;
		char *bg,*ed,*p;
		char *ed_thre;
		Qoutf(FILE *fo,size_t sz = O_buffer_default_size):
			f(fo),
			bg(new char[sz]),ed(bg+sz),p(bg),
			ed_thre(ed - O_buffer_default_flush_threshold){}
		void flush(){
			fwrite_unlocked(bg,1,p - bg,f),p=bg;
		}
		void chk(){
			if(__builtin_expect(p > ed_thre, 0)){
				flush();
			}
		}
		~Qoutf(){
			flush();
			delete[] bg;
		}
		void put4(u32 x) {
			auto C = (const char*)(ot.num + x);
			if (x > 99u) {
				if (x > 999u){
					memcpy(p, C, 4), p += 4;
				}
				else{
					memcpy(p, C + 1, 3), p += 3;
				}	
			} 
			else {
				if (x > 9u){
					memcpy(p, C + 2, 2), p += 2;
				}
				else{
					*p++ = x ^ 48;
				}
			}
		}
		void put2(u32 x) {
			if (x > 9u){	
				memcpy(p, (const char*)(ot.num + x) + 2, 2), p += 2;
			}
			else{
				*p++ = x ^ 48;
			}
		}
		Qoutf &operator << (char ch) {
			return *p++ = ch, *this;
		}
		Qoutf &operator << (u32 x) {
			if (x >= E8) {
				put2(x / E8), x %= E8;
				memcpy(p, ot.num + x / E4, 4), p += 4;
				memcpy(p, ot.num + x % E4, 4), p += 4;
			} else if (x >= E4) {
				put4(x / E4);
				memcpy(p, ot.num + x % E4, 4), p += 4;
			} else{
				put4(x);
			}
			return chk(), *this;
		}
	}qout(stdout);
}
namespace QIO{
	using QIO_I::Qinf;
	using QIO_I::qin;
	using QIO_O::Qoutf;
	using QIO_O::qout;
}
using namespace QIO;
constexpr u32 mod=998244353u;
template<u32 N>std::vector<u32> sieve_prime(){
    std::bitset<N+1> ntp={};
    std::vector<u32> r;
    r.reserve(N/10);
    u32 i=2;
    for(;i*i<=N;++i){
        if(!ntp[i]){
            r.emplace_back(i);
            for(u32 j=i+i;j<=N;j+=i){ntp[j]=1;}
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
    auto pr=sieve_prime<1000000>();
    u32 N;
    qin>>N;
    std::vector<u32> A(N+1),B(N+1);
    for(u32 i=1;i<=N;++i){qin>>A[i];}
    for(u32 i=1;i<=N;++i){qin>>B[i];}
    for(auto p:pr){
        if(p>N){break;}
         for(u32 i=1,j=p;j<=N;++i,j+=p){add(A[j],A[i]),add(B[j],B[i]);}
    }
    for(u32 i=1;i<=N;++i){A[i]=mul(A[i],B[i]);}
    for(auto p:pr){
        if(p>N){break;}
        for(u32 i=N/p,j=i*p;i>0;--i,j-=p){sub(A[j],A[i]);}
    }
    for(u32 i=1;i<=N;++i){qout<<A[i]<<' ';}
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}