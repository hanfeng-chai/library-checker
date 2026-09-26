#pragma GCC optimize("Ofast")
#pragma GCC target("sse3","sse2","sse")
#pragma GCC target("avx","sse4","sse4.1","sse4.2","ssse3")
#pragma GCC target("f16c")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fstrict-aliasing")

#define USEAVX
// #define USEAVX512
// #define USEOMP
#if defined(USEAVX512) and not defined(USEAVX)
#define USEAVX
#endif
#ifdef USEAVX
#pragma GCC target("avx","avx2","fma")
#include<immintrin.h>
using m256d=__m256d;
#endif
#ifdef USEAVX512
#pragma GCC target("avx512f")
using m512d=__m512d;
#endif
#include<bits/stdc++.h>
int OMP_THREADS=1;
#ifdef USEOMP
#include<omp.h>
bool init_omp=[](){
	omp_set_nested(1);
	OMP_THREADS=omp_get_max_threads();
	return true;
}();
#endif

using namespace std;
using ui=unsigned;
using ll=long long;
using ull=unsigned long long;

// #define timer(func,args...) do{auto st=clock();func(args);std::clog<<#func<<" : "<<double(clock()-st)/CLOCKS_PER_SEC*1000<<"ms\n";}while(0)
#define timer(func,args...) do{func(args);}while(0)

// #ifdef USEAVX
// inline m256d make22cp(double x,double y){return _mm256_set_pd(y,x,y,x);}
// inline m256d make2(const std::complex<double> &x){return make22cp(x.real(),x.imag());}
// inline m256d make2(const std::complex<double> &x,const std::complex<double> &y){return _mm256_set_pd(y.imag(),y.real(),x.imag(),x.real());}
// #endif
// #ifdef USEAVX512
// #endif

namespace FFT{
	const double pi=acos(-1);
	std::vector<std::vector<std::complex<double>>> rt,rt3;
	std::vector<std::complex<double>> rtrev={{1,0},{0,1}};
	void init(int n){
		int nrt=rtrev.size();
		if (nrt>=(n>>1)) return;
		rtrev.resize(n>>1);
		for (;nrt<(n>>1);nrt<<=1){
			auto w=std::polar(1.,pi/(nrt<<1));
			for (int i=0;i<nrt;i++) rtrev[i+nrt]=rtrev[i]*w;
		}
		n=std::__lg(n);
		if (int(rt.size())<3){
			rt.resize(3),rt3.resize(3);
			rt[0]=rt[1]=rt[2]=rt3[0]=rt3[1]=rt3[2]=std::vector<std::complex<double>>(1,1);
		}
		nrt=rt.size();
		if (nrt>=n+1) return;
		rt.resize(n+1),rt3.resize(n+1);
		for (;nrt<=n;nrt++){
			rt[nrt].resize(1<<(nrt-2)),rt3[nrt].resize(1<<(nrt-2));
			const std::complex<double> w=std::polar(1.,pi/(1<<(nrt-1))),w3=std::polar(1.,3*pi/(1<<(nrt-1)));
			for (int j=0;j<(1<<(nrt-2));j+=2){
				rt[nrt][j]=rt[nrt-1][j>>1],rt[nrt][j+1]=rt[nrt-1][j>>1]*w;
				rt3[nrt][j]=rt3[nrt-1][j>>1],rt3[nrt][j+1]=rt3[nrt-1][j>>1]*w3;
			}
		}
	}
}

#define PARALLEL_RECURSIVE_CALLS(f)\
if (n<=64) tds=1;\
if (tds<=1){\
	f(a,h),f(a+h,q),f(a+h+q,q);\
}else if (tds==2){\
	_Pragma("omp parallel num_threads(2)")\
	{\
		int tid=omp_get_thread_num();\
		if (tid==0) f(a,h);\
		if (tid!=0 or omp_get_num_threads()<2) f(a+h,q),f(a+h+q,q);\
	}\
}else{\
	const int tds0=tds>>1,tds1=(tds-tds0)>>1,tds2=tds-tds0-tds1;\
	_Pragma("omp parallel num_threads(3)")\
	{\
		int tid=omp_get_thread_num();\
		if (tid==0) f(a,h,tds0);\
		if (tid==1 or omp_get_num_threads()<2) f(a+h,q,tds1);\
		if (tid==2 or omp_get_num_threads()<3) f(a+h+q,q,tds2);\
	}\
}

namespace FFT_split_radix{
	inline void dif_btf(const std::complex<double> o,const std::complex<double> o3,std::complex<double> *a,int n){
		std::complex<double> t0=*a,t1=a[n],t2=a[n+n],t3=a[n*3],u=t0-t2,v=t1-t3;
		v=std::complex<double>(v.imag(),-v.real());
		*a=t0+t2,a[n]=t1+t3,a[n+n]=(u-v)*o,a[n*3]=(u+v)*o3;
	}
	void dif(std::complex<double> *a,int n,int tds=1){
		if (n<=1) return;
		if (n==2){
			std::complex<double> u=*a,v=a[1];
			*a=u+v,a[1]=u-v;
			return;
		}
		if (n==4){
			std::complex<double> t0=*a,t1=a[1],t2=a[2],t3=a[3],u=t0-t2,v=t1-t3,x=t0+t2,y=t1+t3;
			v=std::complex<double>(v.imag(),-v.real());
			*a=x+y,a[1]=x-y,a[2]=u-v,a[3]=u+v;
			return;
		}
		const int h=n>>1,q=h>>1;
		const auto p=FFT::rt[__lg(n)].data(),p3=FFT::rt3[__lg(n)].data();
		for (int i=0;i<q;i++) dif_btf(p[i],p3[i],a+i,q);
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dif)
#else
		dif(a,h),dif(a+h,q),dif(a+h+q,q);
#endif
	}
	inline void dit_btf(const std::complex<double> o,const std::complex<double> o3,std::complex<double> *a,int n){
		std::complex<double> t0=*a,t1=a[n],t2=a[n+n]*o,t3=a[n*3]*o3,u=t2+t3,v=t2-t3;
		v=std::complex<double>(-v.imag(),v.real());
		*a=t0+u,a[n]=t1+v,a[n+n]=t0-u,a[n*3]=t1-v;
	}
	void dit(std::complex<double> *a,int n,int tds=1){
		if (n<=1) return;
		if (n==2){
			std::complex<double> u=*a,v=a[1];
			*a=u+v,a[1]=u-v;
			return;
		}
		if (n==4){
			std::complex<double> x=*a,y=a[1],t0=x+y,t1=x-y,t2=a[2],t3=a[3],u=t2+t3,v=t2-t3;
			v=std::complex<double>(-v.imag(),v.real());
			*a=t0+u,a[1]=t1+v,a[2]=t0-u,a[3]=t1-v;
			return;
		}
		const int h=n>>1,q=h>>1;
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dit)
#else
		dit(a,h),dit(a+h,q),dit(a+h+q,q);
#endif
		const auto p=FFT::rt[std::__lg(n)].data(),p3=FFT::rt3[std::__lg(n)].data();
		for (int i=0;i<q;i++) dit_btf(p[i],p3[i],a+i,q);
	}
}
#ifdef USEAVX
#define load(x) _mm256_loadu_pd((double*)(x))
#define store(x,y) _mm256_storeu_pd((double*)(x),(y))
inline m256d mul2cp(const m256d &x,const m256d &y){return _mm256_fmaddsub_pd(_mm256_movedup_pd(x),y,_mm256_permute_pd(x,15)*_mm256_permute_pd(y,5));}

namespace FFT_split_radix_avx{
	const m256d conjmask=_mm256_castsi256_pd(_mm256_set_epi64x(ll(1ull<<63),0,ll(1ull<<63),0));
	inline void dif_btf(const m256d &o,const m256d &o3,std::complex<double> *a,int n){
		m256d t0=load(a),t1=load(a+n),t2=load(a+n+n),t3=load(a+n*3),u=t0-t2,v=t1-t3;
		v=_mm256_xor_pd(_mm256_permute_pd(v,5),conjmask);
		store(a,t0+t2),store(a+n,t1+t3),store(a+n+n,mul2cp(u-v,o)),store(a+n*3,mul2cp(u+v,o3));
	}
	void dif(std::complex<double> *a,int n,int tds=1){
		if (n<=4){
			FFT_split_radix::dif(a,n);
			return;
		}
		const int h=n>>1,q=h>>1;
		const auto p=FFT::rt[std::__lg(n)].data(),p3=FFT::rt3[std::__lg(n)].data();
		for (int i=0;i<q;i+=2) dif_btf(load(p+i),load(p3+i),a+i,q);
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dif)
#else
		dif(a,h),dif(a+h,q),dif(a+h+q,q);
#endif
	}
	inline void dit_btf(const m256d &o,const m256d &o3,std::complex<double> *a,int n){
		m256d t0=load(a),t1=load(a+n),t2=mul2cp(load(a+n+n),o),t3=mul2cp(load(a+n*3),o3),u=t2+t3,v=t2-t3;
		v=_mm256_permute_pd(_mm256_xor_pd(v,conjmask),5);
		store(a,t0+u),store(a+n,t1+v),store(a+n+n,t0-u),store(a+n*3,t1-v);
	}
	void dit(std::complex<double> *a,int n,int tds=1){
		if (n<=4){
			FFT_split_radix::dit(a,n);
			return;
		}
		const int h=n>>1,q=h>>1;
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dit)
#else
		dit(a,h),dit(a+h,q),dit(a+h+q,q);
#endif
		const auto p=FFT::rt[std::__lg(n)].data(),p3=FFT::rt3[std::__lg(n)].data();
		for (int i=0;i<q;i+=2) dit_btf(load(p+i),load(p3+i),a+i,q);
	}
}
#undef load
#undef store
#endif

#ifdef USEAVX512
#define load(x) _mm512_loadu_pd((double*)(x))
#define store(x,y) _mm512_storeu_pd((double*)(x),(y))
inline m512d mul4cp(const m512d &x,const m512d &y){return _mm512_fmaddsub_pd(_mm512_movedup_pd(x),y,_mm512_permute_pd(x,255)*_mm512_permute_pd(y,85));}

namespace FFT_split_radix_avx512{
	const m512d conjmask=_mm512_castsi512_pd(_mm512_set_epi64(ll(1ull<<63),0,ll(1ull<<63),0,ll(1ull<<63),0,ll(1ull<<63),0));
	inline void dif_btf(const m512d &o,const m512d &o3,std::complex<double> *a,int n){
		m512d t0=load(a),t1=load(a+n),t2=load(a+n+n),t3=load(a+n*3),u=t0-t2,v=t1-t3;
		v=_mm512_xor_pd(_mm512_permute_pd(v,85),conjmask);
		store(a,t0+t2),store(a+n,t1+t3),store(a+n+n,mul4cp(u-v,o)),store(a+n*3,mul4cp(u+v,o3));
	}
	void dif(std::complex<double> *a,int n,int tds=1){
		if (n<=8){
			FFT_split_radix::dif(a,n);
			return;
		}
		const int h=n>>1,q=h>>1;
		const auto p=FFT::rt[std::__lg(n)].data(),p3=FFT::rt3[std::__lg(n)].data();
		for (int i=0;i<q;i+=4) dif_btf(load(p+i),load(p3+i),a+i,q);
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dif)
#else
		dif(a,h),dif(a+h,q),dif(a+h+q,q);
#endif
	}
	inline void dit_btf(const m512d &o,const m512d &o3,std::complex<double> *a,int n){
		m512d t0=load(a),t1=load(a+n),t2=mul4cp(load(a+n+n),o),t3=mul4cp(load(a+n*3),o3),u=t2+t3,v=t2-t3;
		v=_mm512_permute_pd(_mm512_xor_pd(v,conjmask),85);
		store(a,t0+u),store(a+n,t1+v),store(a+n+n,t0-u),store(a+n*3,t1-v);
	}
	void dit(std::complex<double> *a,int n,int tds=1){
		if (n<=8){
			FFT_split_radix::dit(a,n);
			return;
		}
		const int h=n>>1,q=h>>1;
#ifdef USEOMP
		PARALLEL_RECURSIVE_CALLS(dit)
#else
		dit(a,h),dit(a+h,q),dit(a+h+q,q);
#endif
		const auto p=FFT::rt[std::__lg(n)].data(),p3=FFT::rt3[std::__lg(n)].data();
		for (int i=0;i<q;i+=4) dit_btf(load(p+i),load(p3+i),a+i,q);
	}
}
#undef load
#undef store
#endif
#undef PARALLEL_RECURSIVE_CALLS

namespace FFT{
	inline void conv(std::complex<double> *a,std::complex<double> *b,int n,int tds=1){
		FFT::init(n);
#if defined(USEAVX512)
		FFT_split_radix_avx512::dif(a,n,tds),FFT_split_radix_avx512::dif(b,n,tds);
#elif defined(USEAVX)
		FFT_split_radix_avx::dif(a,n,tds),FFT_split_radix_avx::dif(b,n,tds);
#else
		FFT_split_radix::dif(a,n,tds),FFT_split_radix::dif(b,n,tds);
#endif
		const double r=1./n,qr=0.25*r;
		a[0]={a[0].real()*b[0].real()+a[0].imag()*b[0].imag(),a[0].real()*b[0].imag()+a[0].imag()*b[0].real()};
		a[0]*=r,a[1]*=b[1]*r;
		for (int k=2,m=3;k<n;k+=k,m+=m){
			for (int i=k,j=i+k-1;i<m;i++,j--){
				auto a1=a[i]+std::conj(a[j]),a2=a[i]-std::conj(a[j]);
				auto b1=b[i]+std::conj(b[j]),b2=b[i]-std::conj(b[j]);
				auto u=a1*b1+a2*b2*((i&1)?FFT::rtrev[i>>1]:-FFT::rtrev[i>>1]),v=a1*b2+a2*b1;
				a[i]=(u+v)*qr,a[j]=std::conj(u-v)*qr;
			}
		}
#if defined(USEAVX512)
		FFT_split_radix_avx512::dit(a,n,tds);
#elif defined(USEAVX)
		FFT_split_radix_avx::dit(a,n,tds);
#else
		FFT_split_radix::dit(a,n,tds);
#endif
		std::reverse(a+1,a+n);
	}
	inline void conv_sqr(std::complex<double> *a,int n,int tds=1){
		FFT::init(n);
#if defined(USEAVX512)
		FFT_split_radix_avx512::dif(a,n,tds);
#elif defined(USEAVX)
		FFT_split_radix_avx::dif(a,n,tds);
#else
		FFT_split_radix::dif(a,n,tds);
#endif
		const double r=1./n,qr=0.25*r;
		a[0]={a[0].real()*a[0].real()+a[0].imag()*a[0].imag(),a[0].real()*a[0].imag()*2};
		a[0]*=r,a[1]*=a[1]*r;
		for (int k=2,m=3;k<n;k+=k,m+=m){
			for (int i=k,j=i+k-1;i<m;i++,j--){
				auto a1=a[i]+std::conj(a[j]),a2=a[i]-std::conj(a[j]);
				auto u=a1*a1+a2*a2*((i&1)?FFT::rtrev[i>>1]:-FFT::rtrev[i>>1]),v=a1*a2*2.;
				a[i]=(u+v)*qr,a[j]=std::conj(u-v)*qr;
			}
		}
#if defined(USEAVX512)
		FFT_split_radix_avx512::dit(a,n,tds);
#elif defined(USEAVX)
		FFT_split_radix_avx::dit(a,n,tds);
#else
		FFT_split_radix::dit(a,n,tds);
#endif
		std::reverse(a+1,a+n);
	}
}

constexpr int base=100000000,digit=8;
constexpr int FFTbase=10000;

struct BigInt{
	bool sign;
	vector<int> a;
	BigInt(){sign=false,a.resize(1);}
	BigInt(ll x){
		a.clear();
		if(x<0) sign=true,x=-x;
		else sign=false;
		do{
			a.push_back(x%base);
			x/=base;
		}while(x);
	}

	inline void fix(){
		while (not a.empty() and a.back()==0) a.pop_back();
		if (a.empty()) a.push_back(0),sign=false;
		// assert(find_if(a.begin(),a.end(),[](int x){return x>=base or x<0;})==a.end());
	}

	BigInt(const string &buf){
		assert(count(buf.begin(),buf.end(),'-')<=1);
		sign=false;
		a.clear();
		for (int i=buf.length()-1,j=base;i>=0;i--){
			if (buf[i]=='-') sign=true;
			else if (isdigit(buf[i])){
				if (j==base) a.push_back(0),j=1;
				a.back()+=j*(buf[i]^48);
				j*=10;
			}
		}
		fix();
	}

	operator string() const{
		stringstream buf;
		if (sign) buf<<"-";
		buf<<a.back();
		for (int i=a.size()-2;i>=0;i--) buf<<setw(digit)<<setfill('0')<<a[i];
		return buf.str();
	}
	ull to_int() const{
		ull ans=0;
		for (int i=a.size()-1;i>=0;i--) ans*=base,ans+=a[i];
		return ans;
	}

	friend istream& operator >> (istream &in,BigInt &a){
		string buf;
		in>>buf;
		a=BigInt(buf);
		return in;
	}
	friend ostream& operator << (ostream &out,const BigInt &a){
		if (a.sign) out<<"-";
		out<<a.a.back();
		for (int i=a.a.size()-2;i>=0;i--) out<<setw(digit)<<setfill('0')<<a.a[i];
		return out;
	}

	BigInt& operator *= (const BigInt &b){
		if (a.size()<=16 or b.a.size()<=16){
			vector<int> tmp(a.size()+b.a.size());
			for (ui i=0;i<a.size();i++){
				for (ui j=0;j<b.a.size();j++){
					ll u=ll(a[i])*b.a[j]+tmp[i+j];
					tmp[i+j]=u%base;
					tmp[i+j+1]+=u/base;
				}
			}
			a=tmp;
			sign^=b.sign;
			fix();
			return *this;
		}else{
			int len=1<<(__lg(a.size()+b.a.size()-1)+1);
			vector<complex<double>> f(len),g(len);
			for (ui i=0;i<a.size();i++) f[i]={double(a[i]%FFTbase),double(a[i]/FFTbase)};
			for (ui i=0;i<b.a.size();i++) g[i]={double(b.a[i]%FFTbase),double(b.a[i]/FFTbase)};
			FFT::conv(f.data(),g.data(),len,OMP_THREADS);
			ll tmp=0;
			a.resize(a.size()+b.a.size());
			for (int i=0;i<int(a.size());i++){
				tmp+=(ll(f[i].real()+0.5)+ll(f[i].imag()+0.5)*FFTbase);
				a[i]=tmp%base;
				tmp/=base;
			}
			sign^=b.sign;
			fix();
			return *this;
		}
	}
};

using namespace std;

unsigned itc[10000];
inline void init_output_table(){
	int cnt=0;
	for (int i=0;i<10;i++){
		for (int j=0;j<10;j++){
			for (int k=0;k<10;k++){
				for (int l=0;l<10;l++){
					itc[cnt++]=(i|(j<<8)|(k<<16)|(l<<24))+808464432;
				}
			}
		}
	}
}

BigInt a,b;
char buf[5000100];

#include<sys/stat.h>
#include<sys/mman.h>

// inline void input(){
// 	// mt19937 rnd(random_device{}());
// 	// BigInt t=pow10(10000000);
// 	// a=t.randless(rnd),b=t.randless(rnd);
// 	// cin>>a>>b;
// 	a.a.clear();
// 	a.a.reserve(130000);
// 	b.a.clear();
// 	b.a.reserve(130000);
// 	struct stat s;
// 	fstat(0,&s);
// 	char *c=(char*)mmap(nullptr,s.st_size,1,2,0,0),*x=c;
// 	c+=s.st_size;
// 	// cout<<s.st_size;
// 	// exit(0);
// 	// int n=fread(buf,1,2000100,stdin),i=n-1;
// 	while (not isdigit(*c)) c--;
// 	for (int j=base;isdigit(*c);c--){
// 		if (j==base) a.a.push_back(0),j=1;
// 		a.a.back()+=j*(*c^48);
// 		j*=10;
// 	}
// 	while (not isdigit(*c)) c--;
// 	for (int j=base;c>=x;c--){
// 		if (j==base) a.a.push_back(0),j=1;
// 		a.a.back()+=j*(*c^48);
// 		j*=10;
// 	}
// 	// cout<<a<<" "<<b<<"*"<<endl;
// }
// inline void output(const BigInt &x){
// 	// cout<<x<<"*"<<endl;
// 	char *p=buf;
// 	for (int i=a.a.size()-1;i>=0;i--,p+=8){
// 		memcpy(p,&itc[x.a[i]/10000],sizeof(unsigned));
// 		memcpy(p+4,&itc[x.a[i]%10000],sizeof(unsigned));
// 	}
// 	char *q=buf;
// 	while (*q=='0') q++;
// 	fwrite(q,1,p-q,stdout);
// }

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	init_output_table();
	// /*
	struct stat s;
	fstat(0,&s);
	char *c=(char*)mmap(nullptr,s.st_size,1,2,0,0),*x=c;
	c+=s.st_size;
	a.a.reserve(510000);
	b.a.reserve(260000);
	char *p=buf+5000000;
	while (true){
		a.a.clear();
		b.a.clear();
		a.sign=b.sign=false;
		while (not isdigit(*c)) c--;
		for (int j=base;c>=x and isdigit(*c);c--){
			if (j==base) b.a.push_back(0),j=1;
			b.a.back()+=j*(*c^48);
			j*=10;
		}
		if (c<x) break;
		if (*c=='-') b.sign=true;
		while (not isdigit(*c)) c--;
		for (int j=base;isdigit(*c);c--){
			if (j==base) a.a.push_back(0),j=1;
			a.a.back()+=j*(*c^48);
			j*=10;
		}
		if (*c=='-') a.sign=true;
		a*=b;
		for (int i=0;i<int(a.a.size()-1);i++){
			memcpy(p-3,&itc[a.a[i]%10000],sizeof(unsigned));
			memcpy(p-7,&itc[a.a[i]/10000],sizeof(unsigned));
			p-=8;
		}
		int x=a.a.back();
		do{
			*(p--)=(x%10)^48;
			x/=10;
		}while (x);
		if (a.sign) *(p--)='-';
		*(p--)='\n';
	}
	fwrite(p+2,1,buf+5000000-p-1,stdout);
	// input();
	// output(a);
	// */
	return 0;
}