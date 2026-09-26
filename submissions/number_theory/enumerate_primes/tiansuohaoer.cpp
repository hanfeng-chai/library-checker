#include<bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("no-stack-protector,unroll-loops,fast-math,inline")
#pragma GCC target("avx,avx2,abm,mmx,fma")
#pragma GCC target("sse,sse2,sse3,sse4,sse4.1,sse4.2,ssse3")

typedef long long ll;
typedef unsigned long long ull;
using uint=unsigned int;


const uint blk=30030*64,bn=blk/128;

uint i,j,k,n,m,p[160005],stk[160005],it,lst,x,y;
uint mx,ans[1000500];
ull b[40500],tmp[50500];

inline uint max(uint x,uint y){return (x>y)?x:y;}
inline uint min(uint x,uint y){return (x<y)?x:y;}

#define get(b,x) (b[x>>6]&(1ll<<(x&63)))
#define add(b,x) (b[x>>6]|=(1ll<<(x&63)))
#define Add(x) add(b,x>>1),add(b,(x+w30)>>1),x+=(w30*2)

void init(){
	uint i,j,k,st1,st7,st11,st13,st17,st19,st23,st29,mn;
	uint v[]={3,5,7,11,13};
	p[++m]=2;
	for(uint I=0;I<5;I++){
		i=v[I]; p[++m]=i;
		for(j=i;j<=blk;j+=i*2){
			add(tmp,(j-1)>>1); add(b,(j-1)>>1);
		}
	}
	for(i=3;i<=blk;i+=2)if(!get(b,(i-1)>>1)){
		p[++m]=i;
		for(j=i;j<=blk;j+=i*2)add(b,(j-1)>>1);
	}
	for(i=1;i<=m;i++)stk[i]=i;
	
	if(n<=blk){
		for(i=1;i<=m;i++)if(p[i]<=n)lst++;
		while(p[it*x+y]<=n){
			ans[it+1]=p[it*x+y]; it++;
		}
		return;
	}
	while(it*x+y<=m){
		ans[it+1]=p[it*x+y]; it++;
	}
	for(i=1;;i++){
		const uint l=i*blk+1,r=l+blk-1;
		memcpy(b,tmp,sizeof(b));
		for(j=7;p[j]*p[j]<=r;j++){
			if(1ll*p[j]*p[j]*p[j]>=r){
				for(;p[j]*p[stk[j]]<=r;stk[j]++){
				    if(p[j]*p[stk[j]]<l)continue;
					add(b,(p[j]*p[stk[j]]-l)>>1);
				}
				continue;
			}
			const uint w=p[j],w30=w*30;
			const uint st=max(w,(l+w-1)/w); 
			st1=((st-1+29)/30)*30+1; 
			st7=((st-7+29)/30)*30+7; 
			st11=((st-11+29)/30)*30+11; 
			st13=((st-13+29)/30)*30+13; 
			st17=((st-17+29)/30)*30+17; 
			st19=((st-19+29)/30)*30+19; 
			st23=((st-23+29)/30)*30+23; 
			st29=((st-29+29)/30)*30+29; 
			st1*=w; st1-=l;
			st7*=w; st7-=l;
			st11*=w; st11-=l;
			st13*=w; st13-=l;
			st17*=w; st17-=l;
			st19*=w; st19-=l;
			st23*=w; st23-=l;
			st29*=w; st29-=l;
			mn=min(min(min(st1,st7),min(st11,st13)),min(min(st17,st19),min(st23,st29)));
			while(mn<blk){
				Add(st1);
				Add(st7);
				Add(st11);
				Add(st13);
				Add(st17);
				Add(st19);
				Add(st23);
				Add(st29);
				mn+=w30*2;
			}
		}
		for(j=0;j<bn;j++){
			unsigned long long t1=~b[j];
			lst=m; m+=__builtin_popcountll(t1);
			if( (m>=it*x+y) || (((j+1)<<7)+l>=n) ){
				while(t1){
					k=((__builtin_ctzll(t1)<<1)|(j<<7))+l;
					if(k>n)return;
					lst++;
					if(it*x+y==lst){
						ans[it+1]=k; it++;
					}
					t1-=(-t1&t1);
				}
				if((((j+1)<<7)+l>=n))return;
			}
		}
	}
}

namespace IO{
    #ifdef LOCAL
    FILE*Fin(fopen("test.in","r")),*Fout(fopen("test.out","w"));
    #else
    FILE*Fin(stdin),*Fout(stdout);
    #endif
    class qistream{static const size_t SIZE=1<<20,BLOCK=32;FILE*fp;char buf[SIZE];int p;public:qistream(FILE*_fp=stdin):fp(_fp),p(0){fread(buf+p,1,SIZE-p,fp);}void flush(){memmove(buf,buf+p,SIZE-p),fread(buf+SIZE-p,1,p,fp),p=0;}qistream&operator>>(char&str){str=getch();while(isspace(str))str=getch();return*this;}template<class T>qistream&operator>>(T&x){x=0;p+BLOCK>=SIZE?flush():void();bool flag=false;for(;!isdigit(buf[p]);++p)flag=buf[p]=='-';for(;isdigit(buf[p]);++p)x=x*10+buf[p]-'0';x=flag?-x:x;return*this;}char getch(){return buf[p++];}qistream&operator>>(char*str){char ch=getch();while(ch<=' ')ch=getch();for(int i=0;ch>' ';++i,ch=getch())str[i]=ch;return*this;}}qcin(Fin);
    class qostream{static const size_t SIZE=1<<20,BLOCK=32;FILE*fp;char buf[SIZE];int p;public:qostream(FILE*_fp=stdout):fp(_fp),p(0){}~qostream(){fwrite(buf,1,p,fp);}void flush(){fwrite(buf,1,p,fp),p=0;}template<class T>qostream&operator<<(T x){int len=0;p+BLOCK>=SIZE?flush():void();x<0?(x=-x,buf[p++]='-'):0;do buf[p+len]=x%10+'0',x/=10,++len;while(x);for(int i=0,j=len-1;i<j;++i,--j)swap(buf[p+i],buf[p+j]);p+=len;return*this;}qostream&operator<<(char x){putch(x);return*this;}void putch(char ch){p+BLOCK>=SIZE?flush():void();buf[p++]=ch;}qostream&operator<<(const char*str){for(int i=0;str[i];++i)putch(str[i]);return*this;}}qcout(Fout);
}using namespace IO;

int main(){
	qcin>>n>>x>>y; y++;
	init();
	qcout<<lst<<' '<<it<<'\n';
	
	for(i=1;i<=it;i++)qcout<<ans[i]<<'\n';
}