#include<bits/stdc++.h>
struct multi_timer{
	std::chrono::system_clock::time_point lst;
    std::chrono::system_clock::duration tim;
    std::string name;
	multi_timer(std::string nam):lst{},tim{},name{nam}{}
    void start(){lst=std::chrono::system_clock::now();}
    void stop(){tim+=std::chrono::system_clock::now()-lst;}
	~multi_timer(){
		char bbuf[24];
		snprintf(bbuf,24,"%.6Lfms",std::chrono::duration<long double,std::milli>{tim}.count());
		std::clog<<name<<":"<<bbuf<<std::endl;
	}
};
struct auto_timer{
	std::chrono::system_clock::time_point lst;
    std::string name;
	auto_timer(std::string nam):lst{std::chrono::system_clock::now()},name{nam}{}
	~auto_timer(){
		char bbuf[24];
		snprintf(bbuf,24,"%.6Lfms",std::chrono::duration<long double,std::milli>{std::chrono::system_clock::now()-lst}.count());
		std::clog<<name<<":"<<bbuf<<std::endl;
	}
};
using u32=unsigned;
using i64=long long;
using u64=unsigned long long;
using f64=double;
using cpx=std::complex<f64>;
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
        Qinf& operator >> (u64 &x){
			skip_space(),x=0;
			for(; *p > ' '; ++p){
				x = x * 10 + (*p & 0xf);
			}
			return *this;
		}
		Qinf& operator >> (int &x){
			skip_space();
			if(*p == '-'){
                for(++p, x = 48 - *p++;*p > ' ';++p){
					x = x * 10 - (*p ^ 48);
				}	
			}
			else{
				for(x = *p++ ^ 48;*p > ' ';++p){
					x = x * 10 + (*p ^ 48);
				}
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
		int fp;
		u64 _fpi;
		Qoutf(FILE *fo,size_t sz = O_buffer_default_size):
			f(fo),
			bg(new char[sz]),ed(bg+sz),p(bg),
			ed_thre(ed - O_buffer_default_flush_threshold),
			fp(6),
			_fpi(1000000ull){
		}
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
		Qoutf &write(const char *s, size_t count) {
			if (count > 1024 || p + count > ed_thre)
				flush(), fwrite_unlocked(s, 1, count, f);
			else
				memcpy(p, s, count), p += count, chk();

			return *this;
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
		Qoutf& operator << (int x){
			if(x < 0){
				*p++ = '-', x = -x;
			}
			return *this << static_cast<u32>(x);
		}
		Qoutf& operator << (u64 x){
			if(x >= E8){
				u64 q0 = x / E8, r0 = x % E8;
				if (x >= E16) {
					u64 q1 = q0 / E8, r1 = q0 % E8;
					put4(q1);
					memcpy(p, ot.num + r1 / E4, 4), p += 4;
					memcpy(p, ot.num + r1 % E4, 4), p += 4;
				} 
				else if (x >= E12) {
					put4(q0 / E4);
					memcpy(p, ot.num + q0 % E4, 4), p += 4;
				} 
				else {
					put4(q0);
				}
				memcpy(p, ot.num + r0 / E4, 4), p += 4;
				memcpy(p, ot.num + r0 % E4, 4), p += 4;
			}
			else {
				if (x >= E4) {
					put4(x / E4);
					memcpy(p, ot.num + x % E4, 4), p += 4;
				} else {
					put4(x);
				}
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
struct ffter{
    std::vector<cpx> w{1.0};
    void init(int l){
        int old=w.size();
        if(l<=(old<<1)){return;}
		int t=std::__lg(l-1);
		l=1<<t,w.resize(l);
		for(int i=old;i<l;i<<=1){w[i]=std::polar(1.0,acos(-1.0)/(i<<1));}
		for(int i=old;i<l;++i){w[i]=w[i&(i-1)]*w[i&-i];}
    }
    void dif(cpx*f,int L){
		for(int l=L>>1,r=L;l;l>>=1,r>>=1){
            for(cpx*k=f;k!=f+l;++k){
                cpx x=*k,y=k[l];
				*k=x+y,k[l]=x-y;
            }
			for(cpx*j=f+r,*o=w.data()+1;j!=f+L;j+=r,++o){
				for(cpx*k=j;k!=j+l;++k){
					cpx x=*k,y=k[l]**o;
					*k=x+y,k[l]=x-y;
				}
			}
		}
	}
    void dit(cpx*f,int L){
		for(int l=1,r=2;l<L;l<<=1,r<<=1){
            for(cpx*k=f;k!=f+l;++k){
                cpx x=*k,y=k[l];
				*k=x+y,k[l]=x-y;
            }
			for(cpx*j=f+r,*o=w.data()+1;j!=f+L;j+=r,++o){
				for(cpx*k=j;k!=j+l;++k){
					cpx x=*k,y=k[l];
					*k=x+y,k[l]=(x-y)*std::conj(*o);
				}
			}
		}
	}
    void fdot(cpx*res,const cpx*F,const cpx*G,int lm){
        f64 fx=1.0/lm,fx2=0.25*fx;
        res[0]=cpx{F[0].real()*G[0].real()+F[0].imag()*G[0].imag(),F[0].real()*G[0].imag()+F[0].imag()*G[0].real()}*fx;
        res[1]=F[1]*G[1]*fx;
        for(int k=2,m=3;k<lm;k<<=1,m<<=1){
			for(int i=k,j=i+k-1;i<m;++i,--j){
				cpx oi=F[i]+std::conj(F[j]),hi=F[i]-std::conj(F[j]);
                cpx Oi=G[i]+std::conj(G[j]),Hi=G[i]-std::conj(G[j]);
				cpx r0=oi*Oi-hi*Hi*((i&1)?-w[i>>1]:w[i>>1]),r1=Oi*hi+oi*Hi;
                res[i]=(r0+r1)*fx2,res[j]=std::conj(r0-r1)*fx2;
			}
		}
    }
    void fadot(cpx*res,const cpx*F,const cpx*G,int lm){
        f64 fx=1.0/lm,fx2=0.25*fx;
        res[0]+=cpx{F[0].real()*G[0].real()+F[0].imag()*G[0].imag(),F[0].real()*G[0].imag()+F[0].imag()*G[0].real()}*fx;
        res[1]+=F[1]*G[1]*fx;
        for(int k=2,m=3;k<lm;k<<=1,m<<=1){
			for(int i=k,j=i+k-1;i<m;++i,--j){
				cpx oi=F[i]+std::conj(F[j]),hi=F[i]-std::conj(F[j]);
                cpx Oi=G[i]+std::conj(G[j]),Hi=G[i]-std::conj(G[j]);
				cpx r0=oi*Oi-hi*Hi*((i&1)?-w[i>>1]:w[i>>1]),r1=Oi*hi+oi*Hi;
                res[i]+=(r0+r1)*fx2,res[j]+=std::conj(r0-r1)*fx2;
			}
		}
    }
}ft;
using std::cin;
using std::cout;
void solve(){
    constexpr int mask=(1<<13)-1;
    int n,m;
    qin>>n>>m;
    std::vector<u64> a(n),b(m),c(n+m-1);
    {
        auto_timer ot{"input"};
        for(int i=0;i<n;++i){qin>>a[i];}
        for(int i=0;i<m;++i){qin>>b[i];}
    }
    int lim=1<<std::__lg(std::max(n+m-1,15)),lm=lim<<1;
    using Vf=std::vector<f64>;
    auto fdot=[&](Vf&a,const Vf&f,const Vf&g){
        ft.fdot((cpx*)a.data(),(cpx*)f.data(),(cpx*)g.data(),lim);
    };
    auto fadot=[&](Vf&a,const Vf&f,const Vf&g){
        ft.fadot((cpx*)a.data(),(cpx*)f.data(),(cpx*)g.data(),lim);
    };
    auto ext=[&](Vf&a,const std::vector<u64>&b,int id){
        a.assign(lm,0.0);
        for(int n=b.size(),j=id*13,i=0;i<n;++i){a[i]=(b[i]>>j)&mask;}
        ft.dif((cpx*)a.data(),lim);
    };
    auto con=[&](const Vf&a,int id){
        ft.dit((cpx*)a.data(),lim);
        for(int i=0,j=id*13;i<n+m-1;++i){
            c[i]+=i64(a[i]+0.5)<<j;
        }
    };
    ft.init(lim);
    Vf A(lm),B(lm),C(lm),D(lm),E(lm),F(lm),G(lm);
    ext(A,a,0),ext(B,b,0),fdot(C,A,B),con(C,0);
    //A->a0,B->b0 h0=a0*b0->con
    ext(C,a,1),ext(D,b,1),fdot(E,A,D),fadot(E,B,C),con(E,1);
    //C->a1,D->b1 h1=a1*b0+a0*b1->con
    ext(E,a,4),fdot(E,E,B),ext(F,b,4),fadot(E,F,A);
    //E->h4:a0*b4+a4*b0
    ext(F,a,3),ext(G,b,3),fadot(E,F,D),fadot(E,G,C);
    //F->a3,G->b3 E(h4) now a0*b4+a4*b0+a1*b3+a3*b1
    fdot(F,F,B),fadot(F,G,A),ext(G,a,2),fdot(B,B,G);
    //F->h3:b0*a3+a0*b3 G->a2 B->h2:b0*a2
    fadot(B,C,D);
    //B(h2) now b0*a2+a1*b1 
    fadot(F,D,G);
    //F(h3) now b0*a3+a0*b3+a2*b1
    ext(D,b,2),fadot(B,D,A),con(B,2);
    //D->b2, B(h2) now b0*a2+a1*b1+a0*b2->con
    fadot(F,C,D),con(F,3);
    //F(h3) now b0*a3+a0*b3+a2*b1+a1*b2->con 
    fadot(E,D,G),con(E,4);
    //E(h4) now a0*b4+a4*b0+a1*b3+a3*b1+a2*b2->con
    {
        auto_timer ot{"output"};
        for(int i=0;i<n+m-1;++i){qout<<c[i]<<' ';}
    }
}
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t=1;
    while(t--){
        solve();
    }
    return 0;
} 