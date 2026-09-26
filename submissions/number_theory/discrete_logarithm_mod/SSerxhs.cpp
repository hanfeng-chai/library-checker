#include <bits/stdc++.h>
using namespace std;
#if !defined(ONLINE_JUDGE)&&defined(LOCAL)
#include "my_header\debug.h"
#else
#define dbg(...) ;
#define dbgn(...) ;
#endif
namespace BSGS
{
	typedef unsigned int ui;
	typedef unsigned long long ll;
	template<int N,typename T,typename TT> struct ht//个数，定义域，值域
	{
		const static int p=1e6+7,M=p+2;
		TT a[N];
		T v[N];
		int fir[p+2],nxt[N],st[p+2];//和模数相适应
		int tp,ds;//自定义模数
		ht(){memset(fir,0,sizeof fir);tp=ds=0;}
		void mdf(T x,TT z)//位置，值
		{
			ui y=x%p;
			for (int i=fir[y];i;i=nxt[i]) if (v[i]==x) return a[i]=z,void();//若不可能重复不需要 for
			v[++ds]=x;a[ds]=z;
			if (!fir[y]) st[++tp]=y;
			nxt[ds]=fir[y];fir[y]=ds;
		}
		TT find(T x)
		{
			ui y=x%p;
			int i;
			for (i=fir[y];i;i=nxt[i]) if (v[i]==x) return a[i];
			return 0;//返回值和是否判断依据要求决定
		}
		void clear()
		{
			++tp;
			while (--tp) fir[st[tp]]=0;
			ds=0;
		}
	};
	const int N=4e4;
	ht<N,ui,ui> s;
	int exgcd(int a,int b)
	{
		if (a==1) return 1;
		return (1-(long long)b*exgcd(b%a,a))/a;//not ll
	}
	int bsgs(ui a,ui b,ui p)
	{
		ui i,j,k,x,y;
		x=ceil(sqrt(p));
		for (i=0,j=1;i<x;i++,j=(ll)j*a%p)
		{
			if (j==b) return i;
			s.mdf((ll)j*b%p,i+1);
		}
		k=j;
		for (i=1;i<=x;i++,j=(ll)j*k%p) if (y=s.find(j)) return (ll)i*x-y+1;
		return -1;
	}
	int exbsgs(ui a,ui b,ui p)//a^x=b(mod p)
	{
		s.clear();
		a%=p;b%=p;
		ui i,j,k,x,y=__lg(p),cnt=0;
		for (i=0,j=1%p;i<=y;i++,j=(ll)j*a%p) if (j==b) return i;
		y=1;
		while (1)
		{
			if ((x=gcd(a,p))==1) break;
			if (b%x) return -1;//no sol
			++cnt;
			p/=x;b/=x;
			y=(ll)y*(a/x)%p;
		}
		a%=p;
		b=(ll)b*(p+exgcd(y,p))%p;
		int r=bsgs(a,b,p);
		return r==-1?-1:r+cnt;
	}
}
using BSGS::exbsgs;
int main()
{
	int T;
	cin>>T;
	while (T--)
	{
		int a,b,m;
		cin>>a>>b>>m;
		cout<<exbsgs(a,b,m)<<'\n';
	}
}