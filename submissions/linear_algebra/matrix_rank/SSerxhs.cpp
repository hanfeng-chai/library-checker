//这回只花了114514min就打完了。
//真好。记得多手造几组。ACM拍什么拍。 
#include "bits/stdc++.h"
using namespace std;
template<typename T1,typename T2> istream &operator>>(istream &cin,pair<T1,T2> &a) { return cin>>a.first>>a.second; }
template<typename T1> istream &operator>>(istream &cin,vector<T1> &a) { for (auto &x:a) cin>>x; return cin; }
template<typename T1> istream &operator>>(istream &cin,valarray<T1> &a) { for (auto &x:a) cin>>x; return cin; }
template<typename T1,typename T2> ostream &operator<<(ostream &cout,const pair<T1,T2> &a) { return cout<<a.first<<' '<<a.second; }
template<typename T1,typename T2> ostream &operator<<(ostream &cout,const vector<pair<T1,T2>> &a) { for (auto &x:a) cout<<x<<'\n'; return cout; }
template<typename T1> ostream &operator<<(ostream &cout,const vector<T1> &a) { int n=a.size(); if (!n) return cout; cout<<a[0]; for (int i=1; i<n; i++) cout<<' '<<a[i]; return cout; }
template<typename T1,typename T2> bool cmin(T1 &x,const T2 &y) { if (y<x) { x=y; return 1; } return 0; }
template<typename T1,typename T2> bool cmax(T1 &x,const T2 &y) { if (x<y) { x=y; return 1; } return 0; }
template<typename T1> vector<T1> range(T1 l,T1 r,T1 step=1) { assert(step>0); int n=(r-l+step-1)/step,i; vector<T1> res(n); for (i=0; i<n; i++) res[i]=l+step*i; return res; }
template<typename T1> basic_string<T1> operator*(const basic_string<T1> &s,int m) { auto r=s; m*=s.size(); r.resize(m); for (int i=s.size(); i<m; i++) r[i]=r[i-s.size()]; return r; }
#if !defined(ONLINE_JUDGE)&&defined(LOCAL)
#include "my_header\debug.h"
#else
#define dbg(...) ;
#define dbgn(...) ;
#endif
typedef unsigned int ui;
typedef double db;
#define all(x) (x).begin(),(x).end()
// template<typename T1,typename T2> void inc(T1 &x,const T2 &y) { if ((x+=y)>=p) x-=p; }
// template<typename T1,typename T2> void dec(T1 &x,const T2 &y) { if ((x+=p-y)>=p) x-=p; }
const int N=1e6+5;
typedef unsigned long long ll;
const ll p=998244353;
ll ksm(ll x,ll y)
{
	ll r=1;
	while (y)
	{
		if (y&1) r=r*x%p;
		x=x*x%p; y>>=1;
	}
	return r;
}
struct matrix:vector<vector<ll>>
{
	explicit matrix(int n=0,int m=0):vector(n,vector<ll>(m)) { }
	pair<int,int> sz() const { if (size()) return {size(),back().size()}; return {0,0}; }
	int rank() const
	{
		vector<vector<ll>> a=*this;
		auto [n,m]=sz();
		int i,j,k,l,r=0;
		for (i=0,j=0; i<n&&j<m; j++)
		{
			for (k=i; k<n; k++) if (a[k][j]) break;
			if (k==n) continue;
			::swap(a[i],a[k]);
			ll iv=ksm(a[i][j],p-2);
			for (k=j; k<m; k++) a[i][k]=a[i][k]*iv%p;
			for (k=i+1; k<n; k++) for (l=j+1; l<m; l++) a[k][l]=(a[k][l]+(p-a[k][j])*a[i][l])%p;
			++i; ++r;
		}
		return r;
	}
	vector<ll> poly()
	{
		auto [n,m]=sz();
		vector<vector<ll>> a=*this;
		assert(n==m);
		int i,j,k;
		for (i=1; i<n; i++)
		{
			for (j=i; j<n&&!a[j][i-1]; j++);
			if (j==n) continue;
			if (j>i)
			{
				::swap(a[i],a[j]);
				for (k=0; k<n; k++) ::swap(a[k][j],a[k][i]);
			}
			ll r=a[i][i-1];
			for (j=0; j<n; j++) a[j][i]=a[j][i]*r%p;
			r=ksm(r,p-2);
			for (j=i-1; j<n; j++) a[i][j]=a[i][j]*r%p;
			for (j=i+1; j<n; j++)
			{
				r=a[j][i-1];
				for (k=0; k<n; k++) a[k][i]=(a[k][i]+a[k][j]*r)%p;
				r=p-r;
				for (k=i-1; k<n; k++) a[j][k]=(a[j][k]+a[i][k]*r)%p;
			}
		}
		vector g(n+1,vector<ll>(n+1));
		g[0][0]=1;
		for (i=0; i<n; i++)
		{
			ll r=p-1,rr;
			for (j=i; j>=0; j--)//第 j 行选第 n 列
			{
				rr=r*a[j][i]%p;
				for (k=0; k<=j; k++) g[i+1][k]=(g[i+1][k]+rr*g[j][k])%p;
				if (j) r=r*a[j][j-1]%p;
			}
			for (k=1; k<=i+1; k++) (g[i+1][k]+=g[i][k-1])%=p;
		}
		auto f=g[n];
		//if (n&1) for (i=0;i<=n;i++) if (f[i]) f[i]=p-f[i];//若注释掉则为 |kE-A|
		return f;
	}
};
istream &operator>>(istream &cin,matrix &r) { for (auto &v:r) for (ll &x:v) cin>>x; return cin; }
ostream &operator<<(ostream &cout,const matrix &r) { auto [n,m]=r.sz(); for (int i=0; i<n; i++) for (int j=0; j<m; j++) cout<<r[i][j]<<" \n"[j+1==m]; return cout; }
matrix &operator+=(matrix &a,const matrix &b)
{
	assert(a.size()==b.size());
	auto [n,m]=a.sz();
	for (int i=0; i<n; i++) for (int j=0; j<m; j++) (a[i][j]+=b[i][j])%=p;
	return a;
}
matrix &operator-=(matrix &a,const matrix &b)
{
	assert(a.size()==b.size());
	auto [n,m]=a.sz();
	for (int i=0; i<n; i++) for (int j=0; j<m; j++) (a[i][j]+=p-b[i][j])%=p;
	return a;
}
matrix operator*(const matrix &a,const matrix &b)
{
	auto [n,m]=a.sz();
	auto [_,q]=b.sz();
	assert(m==_);
	int i,j,k;
	matrix c(n,q);
	for (k=0; k<m; k++)
	{
		for (i=0; i<n; i++) for (j=0; j<q; j++) c[i][j]+=a[i][k]*b[k][j];
		if (!((k^q-1)&15)) for (auto &v:c) for (ll &x:v) x%=p;
	}
	return c;
}
matrix operator+(matrix a,const matrix &b) { return a+=b; }
matrix operator-(matrix a,const matrix &b) { return a-=b; }
matrix &operator*=(matrix &a,const matrix &b) { return a=a*b; }
matrix &operator*=(matrix &a,ll k) { for (auto &v:a) for (ll &x:v) x=x*k%p; return a; }
matrix operator*(matrix a,ll k) { return a*=k; }
matrix E(int n) { matrix r(n,n); for (int i=0; i<n; i++) r[i][i]=1; return r; }
matrix pow(matrix a,long long k)
{
	assert(k>=0);
	auto [n,m]=a.sz();
	assert(n==m);
	matrix r=k&1?a:E(n);
	k>>=1;
	while (k)
	{
		a*=a;
		if (k&1) r*=a;
		k>>=1;
	}
	return r;
}
matrix pow2(matrix a,long long k)
{
	vector<ll> f=a.poly();
	int n=f.size()-1,i,j;
	if (!n) return matrix();
	if (n==1) return E(1)*ksm(a[0][0],k);
	assert(f[n]==1);
	vector<ll> r(n),x(n),t(n*2);
	r[0]=x[1]=1;
	for (ll &x:f) x=(p-x)%p;
	reverse(all(f));
	fill(all(t),0);
	if (k&1)
	{
		for (i=0; i<n; i++) for (j=0; j<n; j++) t[i+j]=(t[i+j]+r[i]*x[j])%p;
		for (i=n*2-2; i>=n; i--) for (j=1; j<=n; j++) t[i-j]=(t[i-j]+f[j]*t[i])%p;
		for (i=0; i<n; i++) r[i]=t[i];
	}
	k>>=1;
	while (k)
	{
		fill(all(t),0);
		for (i=0; i<n; i++) for (j=0; j<n; j++) t[i+j]=(t[i+j]+x[i]*x[j])%p;
		for (i=n*2-2; i>=n; i--) for (j=1; j<=n; j++) t[i-j]=(t[i-j]+f[j]*t[i])%p;
		for (i=0; i<n; i++) x[i]=t[i];
		if (k&1)
		{
			fill(all(t),0);
			for (i=0; i<n; i++) for (j=0; j<n; j++) t[i+j]=(t[i+j]+r[i]*x[j])%p;
			for (i=n*2-2; i>=n; i--) for (j=1; j<=n; j++) t[i-j]=(t[i-j]+f[j]*t[i])%p;
			for (i=0; i<n; i++) r[i]=t[i];
		}
		k>>=1;
	}
	matrix res(n,n);
	int b=ceil(sqrt(n));
	vector<matrix> s(b+1);
	s[0]=E(n); s[1]=a;
	for (i=2; i<=b; i++) s[i]=s[i-1]*a;
	for (i=b-1; i>=0; i--)
	{
		res*=s[b];
		for (j=min(n,(i+1)*b)-1; j>=i*b; j--) res+=s[j-i*b]*r[j];
	}
	return res;
}
int main()
{
	ios::sync_with_stdio(0); cin.tie(0);
	cout<<fixed<<setprecision(15);
	int n,m,i,j;
	cin>>n>>m;
	matrix a(n,m);
	cin>>a;
	cout<<a.rank()<<endl;
}
