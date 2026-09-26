#include<bits/stdc++.h>
#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
namespace BasicMath{
	typedef vector<int> poly;
	typedef vector<int> Vec;
	typedef vector<Vec> Mat;
	typedef tuple<poly,poly,poly,poly> Mat2;
	mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
	const int Mod=998244353,Mod_G=3;
	const LL Mod2=(LL)Mod*Mod;
	poly frc({1,1}),inv({0,1}),ivf({1,1});
	inline int qpow(int x,int y,int z=1){
		for(;y;(y>>=1)&&(x=(LL)x*x%Mod))if(y&1)z=(LL)z*x%Mod;return z;
	}
	inline void Init(const int&n){
		for(int i=frc.size();i<=n;++i)
			frc.emplace_back((LL)frc.back()*i%Mod),
			inv.emplace_back(Mod-Mod/i*(LL)inv[Mod%i]%Mod),
			ivf.emplace_back((LL)ivf.back()*inv.back()%Mod);
	}
	inline int Binom(const int&n,const int&m){
		if(n<m||m<0)return 0;
		return Init(n),(LL)frc[n]*ivf[m]%Mod*ivf[n-m]%Mod;
	}
	inline poly invLinear(poly P){
		const int n=P.size();
		poly Q(n+1,1);
		for(int i=0;i<n;++i)Q[i+1]=(LL)Q[i]*P[i]%Mod;
		int t=qpow(Q[n],Mod-2);Q.pop_back();
		for(int i=n;i--;)Q[i]=(LL)Q[i]*t%Mod,t=(LL)t*P[i]%Mod;
		return Q;
	}
	inline uLL trans(const uLL&x){
		constexpr uLL A=-(uLL)Mod/Mod+1;
		constexpr uLL Q=(((__uint128_t)(-(uLL)Mod%Mod)<<64)+Mod-1)/Mod;
		return x*A+(uLL)((__uint128_t)x*Q>>64)+1;
	}
	inline uLL mul(const uLL&x,const uLL&y){
		return x*y*(__uint128_t)Mod>>64;
	}
	inline int add(int x,const int&y){
		return ((x+=y)-Mod)>=0?x-Mod:x;
	}
	inline void addeq(int&x,const int&y){
		((x+=y)-Mod)>=0?x-=Mod:0;
	}
	inline int sub(int x,const int&y){
		return (x-=y)<0?x+Mod:x;
	}
	inline void subeq(int&x,const int&y){
		(x-=y)<0?x+=Mod:0;
	}
	inline int neg(const int&x){
		return x?Mod-x:0;
	}
	inline void negeq(int&x){
		x?x=Mod-x:0;
	}
	inline int div2(const int&x){
		return x&1?(x+Mod)>>1:x>>1;
	}
}
using namespace BasicMath;
namespace LagInter{
	inline int Lagrange(const poly&P,int x){
		const int n=P.size();
		if(x<n)return P[x];
		Init(n);
		poly pre(n+1,1),suf(n+1,1);
		for(int i=0;i<n;++i)pre[i+1]=(LL)pre[i]*(x-i)%Mod;
		for(int i=n;i--;)suf[i]=(LL)suf[i+1]*(x-i)%Mod;
		int z=0;
		for(int i=0;i<n;++i)
			z=(z+(n-1-i&1?-1ll:1ll)*P[i]*pre[i]%Mod*suf[i+1]%Mod*ivf[i]%Mod*ivf[n-1-i])%Mod;
		return sub(z,0);
	}
	inline int Lagrange(const poly&P,const poly&Q,int x){
		const int n=P.size();
		for(int i=0;i<n;++i)if(P[i]==x)return Q[i];
		int z=0;
		for(int i=0;i<n;++i){
			int u=Q[i],v=1;
			for(int j=0;j<n;++j)if(i!=j)
				u=(LL)u*(x-P[j])%Mod,v=(LL)v*(P[i]-P[j])%Mod;
			z=add(z,qpow(sub(v,0),Mod-2,sub(u,0)));
		}
		return z;
	}
	inline poly Lagrange(const poly&P,const poly&Q){
		const int n=P.size();
		poly A({1}),B(n);
		for(int u:P){
			A.emplace_back(0);
			const uLL r=trans(Mod-u);
			for(int i=A.size();i--;)A[i]=((i?A[i-1]:0)+mul(A[i],r))%Mod;
		}
		for(int i=0;i<n;++i){
			poly C=A;
			if(P[i]){
				const uLL r=trans(qpow(Mod-P[i],Mod-2));
				for(int i=0;i<C.size()-1;++i)C[i]=mul(C[i],r),subeq(C[i+1],C[i]);
				C.pop_back();
			}
			else C.erase(C.begin());
			int v=1;
			for(int j=0;j<n;++j)if(i!=j)v=(LL)v*(P[i]-P[j]+Mod)%Mod;
			const uLL r=trans(qpow(v,Mod-2,Q[i]));
			for(int i=0;i<n;++i)addeq(B[i],mul(C[i],r));
		}
		return B;
	}
	inline poly Lagrange(const poly&P){
		poly Q(P.size());
		iota(Q.begin(),Q.end(),0);
		return Lagrange(Q,P);
	}
}
template<int N>struct SetPoly:public poly{public:
	SetPoly():poly(){}
	template<class...T>SetPoly(T...x):poly(x...){}
	static inline void FMT(const int&n,const int*A,int (*F)[N+1]){
		for(int S=0;S<(1<<n);++S)fill(F[S],F[S]+n+1,0),F[S][__builtin_popcount(S)]=A[S];
		for(int i=1;i<(1<<n);i*=2)for(int j=0;j<(1<<n);j+=i*2)for(int k=j;k<j+i;++k)for(int l=0;l<=n;++l)addeq(F[k|i][l],F[k][l]);
	}
	static inline void IFMT(const int&n,int (*F)[N+1],int*A){
		for(int i=1;i<(1<<n);i*=2)for(int j=0;j<(1<<n);j+=i*2)for(int k=j;k<j+i;++k)for(int l=0;l<=n;++l)subeq(F[k|i][l],F[k][l]);
		for(int S=0;S<(1<<n);++S)A[S]=F[S][__builtin_popcount(S)];
	}
	static inline void Mul(const int&n,const int*A,const int*B,int*C){
		if(n<=10){
			for(int S=0,T;S<(1<<n);++S)for(C[T=S]=(LL)A[S]*B[0]%Mod;T;(--T)&=S)C[S]=(C[S]+(LL)A[S^T]*B[T])%Mod;
		}
		else{
			static int a[1<<N][N+1],b[1<<N][N+1];
			FMT(n,A,a),FMT(n,B,b);
			for(int S=0;S<(1<<n);++S){
				const int u=__builtin_popcount(S);
				for(int i=min(u+u,n);i>=u;--i){
					__uint128_t t=0;
					for(int j=i-u;j<=u;++j)t+=(uLL)a[S][j]*b[S][i-j];
					a[S][i]=t%Mod;
				}
			}
			IFMT(n,a,C);
		}
	}
	static inline void MulT(const int&n,int*A,const int*B,int*C){
		reverse(A,A+(1<<n)),Mul(n,A,B,C),reverse(A,A+(1<<n)),reverse(C,C+(1<<n));
	}
	static inline void Quo(const int&n,int*A,const int*B){
		if(n<=10){
			for(int S=1;S<(1<<n);++S)for(int T=S;T;(--T)&=S)A[S]=(A[S]+(LL)(Mod-A[S^T])*B[T])%Mod;
		}
		else{
			static int a[1<<N][N+1],b[1<<N][N+1];
			FMT(n,A,a),FMT(n,B,b);
			for(int S=0;S<(1<<n);++S){
				const int u=__builtin_popcount(S);
				for(int i=0;i<=n;++i){
					__uint128_t t=a[S][i];
					for(int j=max(0,i-u);j<i;++j)t+=(LL)(Mod-a[S][j])*b[S][i-j];
					a[S][i]=t%Mod;
				}
			}
			IFMT(n,a,A);
		}
	}
	static inline void CompEGF(const int&n,const int*A,const int*B,int*C){
		for(int i=n;~i;C[0]=A[i--])for(int j=n-i;j--;)Mul(j,C,B+(1<<j),C+(1<<j));
	}
	static inline void CompTEGF(const int&n,const int*A,const int*B,int*C){
		static int D[1<<N],E[1<<N];
		copy(A,A+(1<<n),E);
		for(int i=0;i<=n;++i){
			C[i]=E[0],E[0]=0;
			for(int j=0;j<n-i;++j){
				MulT(j,E+(1<<j),B+(1<<j),D),fill(E+(1<<j),E+(2<<j),0);
				for(int k=0;k<(1<<j);++k)addeq(E[k],D[k]);
			}
		}
	}
	inline friend SetPoly&operator+=(SetPoly&x,const SetPoly&y){
		for(int i=x.size();i--;)addeq(x[i],y[i]);
		return x;
	}
	inline friend SetPoly&operator-=(SetPoly&x,const SetPoly&y){
		for(int i=x.size();i--;)subeq(x[i],y[i]);
		return x;
	}
	inline friend SetPoly operator+(SetPoly x,const SetPoly&y){
		return x+=y;
	}
	inline friend SetPoly operator-(SetPoly x,const SetPoly&y){
		return x-=y;
	}
	inline friend SetPoly operator-(SetPoly x){
		for(int&i:x)negeq(i);
		return x;
	}
	inline friend SetPoly operator*(const SetPoly&x,const SetPoly&y){
		const int n=__lg(x.size());
		SetPoly z(1<<n);
		return Mul(n,x.data(),y.data(),z.data()),z;
	}
	inline friend SetPoly operator/(SetPoly x,const SetPoly&y){
		const int n=__lg(x.size());
		SetPoly z(1<<n);
		return Quo(n,x.data(),y.data()),z;
	}
	inline friend SetPoly&operator*=(SetPoly&x,const SetPoly&y){
		return x=x*y;
	}
	inline friend SetPoly&operator/=(SetPoly&x,const SetPoly&y){
		return x=x/y;
	}
	inline friend SetPoly Inv(const SetPoly&x){
		const int n=__lg(x.size());
		SetPoly z(1<<n);
		return z[0]=1,Quo(n,z.data(),x.data()),z;
	}
	inline friend SetPoly Exp(SetPoly x){
		const int n=__lg(x.size());
		SetPoly z(1<<n);z[0]=1;
		for(int i=0;i<n;++i)Mul(i,z.data(),x.data()+(1<<i),z.data()+(1<<i));
		return z;
	}
	inline friend SetPoly Ln(SetPoly x){
		const int n=__lg(x.size());
		for(int i=n;i--;)Quo(i,x.data()+(1<<i),x.data());
		return x[0]=0,x;
	}
	inline friend SetPoly CompEGF(const poly&a,const SetPoly&x){
		const int n=__lg(x.size());
		SetPoly y(1<<n);
		return CompEGF(n,a.data(),x.data(),y.data()),y;
	}
	inline friend poly CompTEGF(const SetPoly&f,const SetPoly&x){
		const int n=__lg(x.size());
		poly y(n+1);
		return CompTEGF(n,f.data(),x.data(),y.data()),y;
	}
	inline friend SetPoly Comp(poly a,SetPoly x){
		const int n=__lg(x.size()),m=a.size();
		if(!m)return SetPoly(1<<n);
		poly b(n+1);
		for(int i=0;i<=n;++i){
			for(int j=m;j--;)b[i]=((LL)b[i]*x[0]+a[j])%Mod;
			for(int j=1;j<m;++j)a[j-1]=(LL)j*a[j]%Mod;
			a[m-1]=0;
		}
		SetPoly y(1<<n);
		return x[0]=0,CompEGF(n,b.data(),x.data(),y.data()),y;
	}
	inline friend poly CompT(SetPoly f,SetPoly a,int m){
		const int n=__lg(a.size()),c=a[0];
		poly y(n+1),x(m),z(n+1);
		a[0]=0,CompTEGF(n,f.data(),a.data(),y.data()),z[0]=1;
		for(int i=0;i<m;++i){
			for(int j=0;j<=n;++j)x[i]=(x[i]+(LL)y[j]*z[j])%Mod;
			for(int j=n;j;--j)z[j]=(LL)(i+1)*z[j-1]%Mod;
			z[0]=(LL)z[0]*c%Mod;
		}
		return x;
	}
};
const int N=20;
signed main(){
	cin.tie(0)->sync_with_stdio(0);
	int n,m;cin>>n>>m;
	vector<vector<int>>G(n);
	for(int i=0;i<m;++i){
		int u,v;cin>>u>>v;
		G[u].emplace_back(v);
		G[v].emplace_back(u);
	}
	int ans=n+1;
	vector<int>col;
	const auto dfs=[&](auto&dfs,int c,int cnt)->void
	{
		static vector<int>cur(n);
		if(c>=ans)return;
		if(!cnt)return ans=c,col=cur,void();
		int u=-1,d=-1;LL q=0;
		for(int i=0;i<n;++i)if(!cur[i]){
			LL s=0;
			for(int j:G[i])if(cur[j])s|=1ll<<cur[j];
			int t=__builtin_popcountll(s);
			if(t>d)d=t,u=i,q=s;
		}
		for(int i=1;i<=c+1;++i)if(!(q>>i&1))
			cur[u]=i,dfs(dfs,max(c,i),cnt-1);
		cur[u]=0;
	};
	dfs(dfs,0,n);
	cout<<ans;
	return 0;
}
/*
*/