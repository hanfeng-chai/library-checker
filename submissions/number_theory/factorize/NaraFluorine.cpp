#include<bits/stdc++.h>
using namespace std;
#define enter fout<<"\n";
#define space fout<<" ";
#define dot fout<<",";
#define oui fout<<"Yes\n";
#define non fout<<"No\n";
#define si fout<<"?";
#define i32 int
#define u32 unsigned int
#define i64 long long
#define u64 unsigned long long
#define i128 __int128
#define u128 unsigned __int128
#define debug(x) fout<<#x<<"="<<x<<"\n";
#define vdebug(a) fout<<#a<<"=";for(autox:a)fout<<x<<" ";fout<<"\n";
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
namespace fastio{
	const int bufl=1<<20;
	const double base1[16]={1,1e-1,1e-2,1e-3,1e-4,1e-5,1e-6,1e-7,1e-8,1e-9,1e-10,1e-11,1e-12,1e-13,1e-14,1e-15};
	const double base2[16]={1,1e1,1e2,1e3,1e4,1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12,1e13,1e14,1e15};
	struct IN{
		FILE *IT;char ibuf[bufl],*is=ibuf,*it=ibuf;
		IN(){IT=stdin;}IN(char *a){IT=fopen(a,"r");}
		inline char getChar(){if(is==it){it=(is=ibuf)+fread(ibuf,1,bufl,IT);if(is==it)return EOF;}return *is++;}
		template<typename Temp>inline void getInt(Temp &a){a=0;int b=0,c=getChar();while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)a=(a<<1)+(a<<3)+c-48,c=getChar();if(b)a=-a;}
		template<typename Temp>inline void getDouble(Temp &a){a=0;int b=0,c=getChar(),d=0;__int128 e=0,f=0;while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)e=(e<<1)+(e<<3)+c-48,c=getChar();if(c==46){c=getChar();while(c>=48&&c<=57)d++,f=(f<<1)+(f<<3)+c-48,c=getChar();}a=e+base1[d]*f;if(b)a=-a;}
		IN& operator>>(char &a){a=getChar();while(a<=32)a=getChar();return *this;}
		IN& operator>>(char *a){do{*a=getChar();}while(*a<=32);while(*a>32)*++a=getChar();*a=0;return *this;}
		IN& operator>>(string &a){char b=getChar();while(b<=32)b=getChar();while(b>32)a+=b,b=getChar();return *this;}
		IN& operator>>(int &a){getInt(a);return *this;}
		IN& operator>>(long long &a){getInt(a);return *this;}
		IN& operator>>(__int128 &a){getInt(a);return *this;}
		IN& operator>>(float &a){getDouble(a);return *this;}
		IN& operator>>(double &a){getDouble(a);return *this;}
		IN& operator>>(long double &a){getDouble(a);return *this;}
	};
	struct OUT{
		FILE *IT;char obuf[bufl],*os=obuf,*ot=obuf+bufl;int Eps;long double Acc;
		OUT(){IT=stdout,Eps=6,Acc=0.5;}OUT(char *a){IT=fopen(a,"w"),Eps=6,Acc=0.5;}
		inline void ChangEps(int x=6){Eps=x;}
		inline void flush(){fwrite(obuf,1,os-obuf,IT);os=obuf;}
		inline void putChar(int a){*os++=a;if(os==ot)flush();}
		template<typename Temp>inline void putInt(Temp a){if(a<0){putChar(45);a=-a;}if(a<10){putChar(a+48);return;}putInt(a/10);putChar(a%10+48);}
		template<typename Temp>inline void putLeading(Temp a,int b){if(!b)return;putLeading(a/10,b-1);putChar(a%10+48);}
		template<typename Temp>inline void putDouble(Temp a){if(a<0){putChar(45);a=-a;}__int128 ff=(a-(__int128)a)*base2[Eps+2],gg=0;ff+=50;while(ff>0){ff/=10;gg++;}__int128 b=a;if(gg==Eps+3){putInt(b+1);}else{putInt(b);}a-=b;a*=base2[Eps];b=a+Acc;putChar(46);putLeading(b,Eps);}
		OUT& operator<<(char a){putChar(a);return *this;}
		OUT& operator<<(const char *a){while(*a)putChar(*a++);return *this;}
		OUT& operator<<(string a){for(auto c:a)putChar(c);return *this;}
		OUT& operator<<(int a){putInt(a);return *this;}
		OUT& operator<<(long long a){putInt(a);return *this;}
		OUT& operator<<(__int128 a){putInt(a);return *this;}
		OUT& operator<<(unsigned int a){putInt(a);return *this;}
		OUT& operator<<(unsigned long long a){putInt(a);return *this;}
		OUT& operator<<(unsigned __int128 a){putInt(a);return *this;}
		OUT& operator<<(float a){putDouble(a);return *this;}
		OUT& operator<<(double a){putDouble(a);return *this;}
		OUT& operator<<(long double a){putDouble(a);return *this;}
		~OUT(){flush();}
	};
}
fastio::IN fin;
fastio::OUT fout;


namespace decompos{
	unsigned long long Number(unsigned long long a,unsigned long long b){
		mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
		return (a+rng()%(b-a+1));
	}
	unsigned long long stein(unsigned long long x,unsigned long long y){
		if(x==y)return x;
		unsigned long long a=y-x;
		unsigned long long b=x-y;
		int n=__builtin_ctzll(b);
		unsigned long long s=(x<y?a:b);
		unsigned long long t=(x<y?x:y);
		return stein(s>>n,t);
	}
	unsigned long long gcd(unsigned long long x,unsigned long long y){
		if(x==0)return y;
		if(y==0)return x;
		int n=__builtin_ctzll(x);
		int m=__builtin_ctzll(y);
		return (stein(x>>n,y>>m)<<(n<m?n:m));
	}
	unsigned long long limit=2147483648;
	vector<int>small={2,7,61};
	vector<int>large={2,325,9375,28178,450775,9780504,1795265022};
	unsigned long long qp(unsigned long long x,unsigned long long y,unsigned long long m){
		unsigned long long res=1;
		while(y){
			if(y&1)res=((unsigned __int128)res*x)%m;
			x=((unsigned __int128)x*x)%m;
			y>>=1;
		}
		return res;
	}
	bool isComposite(unsigned long long a,unsigned long long d,unsigned long long n,unsigned long long s){
		unsigned long long x=qp(a,d,n);
		if(x==1)return false;
		while(s--){
			if(x==n-1)return false;
			x=((unsigned __int128)x*x)%n;
		}
		return true;
	}
	bool isPrime(unsigned long long n){
		if(n==1)
			return false;
		vector<int>bases=(n<limit?small:large);
		if(find(bases.begin(),bases.end(),n)!=bases.end())
			return true;
		for(int p:bases)
			if(n%p==0)
				return false;
		unsigned long long s=__builtin_ctzll(n-1);
		unsigned long long d=(n-1)>>s;
		for(int base:bases){
			if(isComposite(base,d,n,s))
				return false;
		}
		return true;
	}
	class Montgomery{
		unsigned long long m;
		unsigned long long r;
		public:
		Montgomery(unsigned long long n):m(n),r(n){
			for(int i=0;i<5;i++){
				r*=2-m*r;
			}
		}
		unsigned long long fma(unsigned long long a,unsigned long long b,unsigned long long c)const{
			unsigned __int128 d=(unsigned __int128)(a)*b;
			unsigned long long e=c+m+(d>>64);
			unsigned long long f=(unsigned long long)(d)*r;
			unsigned long long g=((unsigned __int128)(f)*m)>>64;
			return (e-g);
		}
		unsigned long long mul(unsigned long long a,unsigned long long b)const{return fma(a,b,0);}
	};
	unsigned long long PollardRho(unsigned long long n){
		if(n%2==0)return 2;
		Montgomery m(n);
		unsigned long long c1=1;
		unsigned long long c2=2;
		unsigned long long M=512;
		unsigned long long w1=1;
		unsigned long long w2=2;
	retry:
		unsigned long long z1=w1;
		unsigned long long z2=w2;
		for(unsigned long long k=M;;k <<= 1){
			unsigned long long x1=z1+n;
			unsigned long long x2=z2+n;
			for(unsigned long long j=0;j<k;j+=M){
				unsigned long long y1=z1;
				unsigned long long y2=z2;
				unsigned long long q1=1;
				unsigned long long q2=2;
				z1=m.fma(z1,z1,c1);
				z2=m.fma(z2,z2,c2);
				for(unsigned long long i=0;i<M;i++){
					unsigned long long t1=x1-z1;
					unsigned long long t2=x2-z2;
					z1=m.fma(z1,z1,c1);
					z2=m.fma(z2,z2,c2);
					q1=m.mul(q1,t1);
					q2=m.mul(q2,t2);
				}
				q1=m.mul(q1,x1-z1);
				q2=m.mul(q2,x2-z2);
				unsigned long long q3=m.mul(q1,q2);
				unsigned long long g3=gcd(n,q3);
				if(g3==1)continue;
				if(g3!=n)return g3;
				unsigned long long g1=gcd(n,q1);
				unsigned long long g2=gcd(n,q2);
				unsigned long long c=(g1!=1?c1:c2);
				unsigned long long x=(g1!=1?x1:x2);
				unsigned long long z=(g1!=1?y1:y2);
				unsigned long long g=(g1!=1?g1:g2);
				if(g==n){
					do{
						z=m.fma(z,z,c);
						g=gcd(n,x-z);
					}while(g==1);
				}
				if(g!=n)return g;
				w1+=2;
				w2+=2;
				goto retry;
			}
		}
	}
	void factorize(unsigned long long n,vector<unsigned long long>& res){
		if(n<=1)return;
		if(isPrime(n)){
			res.push_back(n);
			return;
		}
		unsigned long long p=PollardRho(n);
		factorize(p,res);
		factorize(n/p,res);
	}
}








long long n,m,res;
//#define NaraFluorine
int main(){
	#ifdef NaraFluorine
	freopen("P_.in","r",stdin);
	freopen("std.out","w",stdout);
	#endif
	int t;
	fin>>t;
	while(t--){
		fin>>n;
		vector<u64>tmp;
		decompos::factorize(n,tmp);
		sort(tmp.begin(),tmp.end());
		fout<<(int)tmp.size()<<" ";
		for(auto i:tmp){
			fout<<i<<" ";
		}
		enter;
	}
	return 0;
}