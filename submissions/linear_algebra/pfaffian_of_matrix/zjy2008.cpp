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
namespace LinearAlgebra{
	inline Mat operator+(Mat x,const Mat&y){
		if(x.empty())return Mat();
		const int n=x.size(),m=x[0].size();
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)addeq(x[i][j],y[i][j]);
		return x;
	}
	inline Mat operator-(Mat x,const Mat&y){
		if(x.empty())return Mat();
		const int n=x.size(),m=x[0].size();
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)subeq(x[i][j],y[i][j]);
		return x;
	}
	inline Mat operator-(Mat x){
		for(Vec&i:x)for(int&j:i)j=neg(j);
		return x;
	}
	inline Vec operator+(Vec x,const Vec&y){
		for(int i=x.size();i--;)addeq(x[i],y[i]);
		return x;
	}
	inline Vec operator-(Vec x,const Vec&y){
		for(int i=x.size();i--;)subeq(x[i],y[i]);
		return x;
	}
	inline Vec operator-(Vec x){
		for(int&i:x)i=neg(i);
		return x;
	}
	inline Mat operator*(const Mat&x,const Mat&y){
		if(y.empty())return x;
		const int n=x.size(),m=y.size(),q=y[0].size();
		Mat z(n,Vec(q));
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)if(x[i][j])
			for(int k=0;k<q;++k)z[i][k]=(z[i][k]+(LL)x[i][j]*y[j][k])%Mod;
		return z;
	}
	inline Mat operator*(Mat x,const int&y){
		const uLL r=trans(y);
		for(Vec&i:x)for(int&j:i)j=mul(j,r);
		return x;
	}
	inline Vec operator*(Vec x,const int&y){
		const uLL r=trans(y);
		for(int&i:x)i=mul(i,r);
		return x;
	}
	inline Mat operator*(const int&x,const Mat&y){
		return y*x;
	}
	inline Vec operator*(const int&x,const Vec&y){
		return y*x;
	}
	inline Vec operator*(const Vec&x,const Mat&y){
		if(x.empty())return Vec();
		const int n=x.size(),m=y[0].size();
		Vec z(m);
		for(int i=0;i<n;++i)if(x[i])for(int j=0;j<m;++j)z[j]=(z[j]+(LL)x[i]*y[i][j])%Mod;
		return z;
	}
	inline Vec operator*(const Mat&x,const Vec&y){
		if(y.empty())return Vec();
		const int n=x.size(),m=y.size();
		Vec z(n);
		for(int j=0;j<m;++j)if(y[j])for(int i=0;i<n;++i)z[i]=(z[i]+(LL)x[i][j]*y[j])%Mod;
		return z;
	}
	inline int operator*(const Vec&x,const Vec&y){
		int z=0;
		for(int i=x.size();i--;)z=(z+(LL)x[0]*y[0])%Mod;
		return z;
	}
	template<class T>inline Mat& operator+=(Mat&x,const T&y){
		return x=x*y;
	}
	template<class T>inline Mat& operator-=(Mat&x,const T&y){
		return x=x-y;
	}
	template<class T>inline Mat& operator*=(Mat&x,const T&y){
		return x=x*y;
	}
	template<class T>inline Vec& operator+=(Vec&x,const T&y){
		return x=x+y;
	}
	template<class T>inline Vec& operator-=(Vec&x,const T&y){
		return x=x-y;
	}
	template<class T>inline Vec& operator*=(Vec&x,const T&y){
		return x=x*y;
	}
	inline Mat MatT(const Mat&x){
		if(x.empty())return x;
		const int n=x.size(),m=x[0].size();
		Mat y(m,Vec(n));
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)y[j][i]=x[i][j];
		return y;
	}
	inline int Det(Mat A){
		const int n=A.size();int z=1;
		for(int i=0;i<n;++i)
			for(int j=i+1;j<n;++j){
				while(A[i][i])A[j]-=A[i]*(A[j][i]/A[i][i]),swap(A[i],A[j]),z=-z;
				swap(A[i],A[j]),z=-z;
			}
		for(int i=0;i<n;++i)z=(LL)z*A[i][i]%Mod;
		return sub(z,0);
	}
	inline int Rank(Mat A){
		if(A.empty()||A[0].empty())return 0;
		if(A.size()>A[0].size())A=MatT(A);
		const int n=A.size(),m=A[0].size();
		int j=0;
		for(int i=0;i<m&&j<n;++i){
			for(int k=j;k<n;++k)if(A[k][i]){
				swap(A[j],A[k]);break;
			}
			if(A[j][i]){
				const int r=qpow(A[j][i],Mod-2);
				A[j]*=r;
				for(int k=0;k<n;++k)if(j!=k&&A[k][i])A[k]-=A[j]*A[k][i];
				++j;
			}
		}
		return j;
	}
	inline Mat Inv(Mat A){
		const int n=A.size();
		Mat B(n,Vec(n));
		for(int i=0;i<n;++i)B[i][i]=1;
		for(int i=0;i<n;++i){
			for(int j=i;j<n;++j)if(A[j][i]){
				swap(A[i],A[j]),swap(B[i],B[j]);
				break;
			}
			if(!A[i][i])return Mat({Vec({-1})});
			const int r=qpow(A[i][i],Mod-2);
			A[i]*=r,B[i]*=r;
			for(int j=0;j<n;++j)if(i!=j&&A[j][i])
				B[j]-=B[i]*A[j][i],A[j]-=A[i]*A[j][i];
		}
		return B;
	}
	inline pair<Mat,Vec>Equation(Mat A,Vec B){
		const int n=A.size(),m=A[0].size();
		for(int i=0;i<n;++i)A[i].emplace_back(B[i]);
		vector<int>D;vector<pair<int,int>>C;
		int k=0;
		for(int i=0;i<m;++i){
			for(int j=k;j<n;++j)if(A[j][i]){
				swap(A[k],A[j]);break;
			}
			if(k<n&&A[k][i]){
				A[k]*=qpow(A[k][i],Mod-2);
				for(int j=0;j<n;++j)if(k!=j&&A[j][i]){
					const uLL r=trans(A[j][i]);
					for(int t=i;t<=m;++t)subeq(A[j][t],mul(A[k][t],r));
				}
				C.emplace_back(i,k++);
			}
			else D.emplace_back(i);
		}
		for(int i=k;i<n;++i)if(A[i][m])return make_pair(Mat(),Vec({-1}));
		vector<int>P(m);
		for(auto [x,y]:C)P[x]=A[y][m];
		Mat Q(D.size(),Vec(m));
		for(int i=0;i<D.size();++i){
			Q[i][D[i]]=neg(1);
			for(auto [x,y]:C)Q[i][x]=A[y][D[i]];
		}
		return make_pair(Q,P);
	}
	inline poly Character(Mat A){
		const int n=A.size();
		Mat B(n+1,Vec(n+1));
		for(int i=0;i<n-1;++i){
			if(!A[i+1][i])for(int j=i+2;j<n;++j)if(A[j][i]){
				swap(A[i+1],A[j]);
				for(int k=0;k<n;++k)swap(A[k][i+1],A[k][j]);
				break;
			}
			if(!A[i+1][i])continue;
			const uLL r=trans(qpow(A[i+1][i],Mod-2));
			for(int j=i+2;j<n;++j){
				const uLL w=trans(mul(r,A[j][i]));
				for(int k=i;k<n;++k)subeq(A[j][k],mul(A[i+1][k],w));
				for(int k=0;k<n;++k)addeq(A[k][i+1],mul(A[k][j],w));
			}
		}
		B[0][0]=1;
		for(int i=0;i<n;++i){
			for(int j=0;j<=i;++j)addeq(B[i+1][j+1],B[i][j]);
			for(int j=i,w=Mod-1;j<n;++j){
				const uLL r=trans((LL)A[i][j]*w%Mod);
				for(int k=0;k<=i;++k)addeq(B[j+1][k],mul(B[i][k],r));
				if(j<n-1)w=(LL)w*A[j+1][j]%Mod;
			}
		}
		return B[n];
	}
	inline poly Det(Mat A,Mat B){
		int z=1,m=0;
		const int n=A.size();
		for(int i=0;i<n;++i){
			while(!B[i][i]){
				for(int j=i+1;j<n;++j)if(B[i][j]){
					z=-z;
					for(int k=0;k<n;++k)swap(A[k][i],A[k][j]),swap(B[k][i],B[k][j]);
					break;
				}
				if(B[i][i])break;
				B[i]=A[i],A[i].assign(n,0);
				if((++m)>n)return poly(n+1,0);
				for(int j=0;j<i;++j){
					const uLL r=trans(B[i][j]);
					B[i][j]=0;
					for(int k=0;k<n;++k)subeq(A[i][k],mul(A[j][k],r));
					for(int k=i;k<n;++k)subeq(B[i][k],mul(B[j][k],r));
				}
			}
			z=(LL)z*B[i][i]%Mod;
			const uLL r=trans(qpow(B[i][i],Mod-2));
			for(int j=0;j<n;++j)A[i][j]=mul(A[i][j],r);
			for(int j=i;j<n;++j)B[i][j]=mul(B[i][j],r);
			for(int j=0;j<n;++j)if(i!=j){
				const uLL r=trans(B[j][i]);B[j][i]=0;
				for(int k=0;k<n;++k)subeq(A[j][k],mul(A[i][k],r));
				for(int k=i;k<n;++k)subeq(B[j][k],mul(B[i][k],r));
			}
		}
		z=sub(z,0);
		poly ans=Character(-A);
		ans.erase(ans.begin(),ans.begin()+m);
		for(int&i:ans)i=(LL)i*z%Mod;
		return ans.resize(n+1),ans;
	}
	inline Mat Adj(Mat A){
		const int n=A.size();
		Mat B(n,Vec(n));
		for(int i=0;i<n;++i)B[i][i]=1;
		int q=-1,z=1;
		for(int i=0;i<n;++i){
			int t=q;
			for(int j=i;j<n;++j)if(A[j][i]){t=j;break;}
			if(t<0){q=i;continue;}
			if(!A[t][i])return Mat(n,Vec(n));
			if(t!=i)A[i]+=A[t],B[i]+=B[t];
			z=(LL)z*A[i][i]%Mod;
			const int r=qpow(A[i][i],Mod-2);
			A[i]*=r,B[i]*=r;
			for(int j=0;j<n;++j)if(i!=j&&A[j][i])
				B[j]-=B[i]*A[j][i],A[j]-=A[i]*A[j][i];
		}
		B*=z;
		if(~q)for(int i=0;i<n;++i)if(i!=q)B[i]=B[q]*neg(A[i][q]);
		return B;
	}
	inline int Pfaffian(Mat A){
		const int n=A.size();
		int z=1;
		vector<uLL>B(n);
		for(int i=0;i<n;i+=2){
			if(!A[i+1][i])for(int j=i+2;j<n;++j)
				if(A[j][i]){
					swap(A[i+1],A[j]);
					for(int t=i;t<n;++t)swap(A[t][j],A[t][i+1]);
					z=-z;break;
				}
			if(A[i+1][i]){
				z=(LL)z*A[i][i+1]%Mod;
				const uLL t=trans(qpow(A[i][i+1],Mod-2));
				for(int j=i+2;j<n;++j)B[j]=trans(mul(t,A[i][j]));
				for(int j=i+2;j<n;++j)for(int k=i+2;k<n;++k)
					A[j][k]=sub(add(A[j][k],mul(B[j],A[k][i+1])),mul(A[j][i+1],B[k]));
			}
			else return 0;
		}
		return sub(z,0);
	}
	inline int Hafnian(const Mat&A){
		const int n=A.size()/2;
		Mat f(1<<n,Vec(n*2));
		for(int i=0;i<n;++i)f[1<<i][i+i]=1;
		int ans=0;
		for(int i=0;i<n;++i)
			for(int j=1<<i;j<(1<<(i+1));++j)
				for(int k=0;k<i+i+2;++k)if(j>>(k>>1)&1){
					for(int l=0;l<i+i;++l)if(!(j>>(l>>1)&1))
						f[1<<(l>>1)|j][l^1]=(f[1<<(l>>1)|j][l^1]+(LL)f[j][k]*A[k][l])%Mod;
					const int r=(LL)f[j][k]*A[k][i+i+1]%Mod;
					for(int l=i+1;l<n;++l)addeq(f[1<<l|j][l+l],r);
					if(j==(1<<n)-1)addeq(ans,r);
				}
		return ans;
	}
	inline tuple<Mat,Mat,Mat>Forbenius(const Mat&A){
		const int n=A.size();
		const auto ins=[&](Mat&S,Mat&P,Mat&Q,Vec A){
			int x=S.size();
			Vec B(n);
			S.emplace_back(A);
			for(int i=0;i<n;++i)if(A[i])
				if(!P[i].empty()){
					const int r=A[i];
					A-=P[i]*r,B-=Q[i]*r;
				}
				else{
					const int r=qpow(A[i],Mod-2);
					B[x]=1,P[i]=A*r,Q[i]=B*r;
					return poly();
				}
			return B.resize(x+1),B[x]=1,B;
		};
		Mat W,S,P(n),Q(n);
		uniform_int_distribution<int>rnd(0,Mod-1);
		while(S.size()<n){
			Vec E(n);
			for(int i=0;i<n;++i)E[i]=rnd(rng);
			Vec D=E;poly C;Mat T=S,U=P,V=Q;
			for(;;D=A*D)if(!(C=ins(T,U,V,D)).empty())break;
			poly F(C.begin()+S.size(),C.end());
			const auto div=[&](poly x){
				poly y(x.size()-F.size()+1);
				for(int i=x.size()-1;i>=F.size()-1;--i){
					y[i-F.size()+1]=x[i];
					for(int j=0;j<F.size();++j)
						x[i-F.size()+1+j]=(x[i-F.size()+1+j]+(LL)(Mod-x[i])*F[j])%Mod;
				}
				return y;
			};
			int pre=0;
			for(poly&i:W){
				if(i.size()>F.size()){
					poly cur(div(poly(C.begin()+pre,C.begin()+pre+i.size()-1)));
					for(int j=0;j<cur.size();++j)E+=S[pre+j]*cur[j];
				}
				pre+=i.size()-1;
			}
			W.emplace_back(F);
			for(int i=0;i<F.size()-1;++i)ins(S,P,Q,E),E=A*E;
		}
		for(int i=n;i--;)
			for(int j=0;j<i;++j){
				const int r=P[j][i];
				P[j]-=P[i]*r,Q[j]-=Q[i]*r;
			}
		return make_tuple(MatT(S),MatT(Q),W);
	}
	inline Mat Qpowi(Mat A,vector<bool>k){
		const int n=A.size();
		Mat B(n,Vec(n));
		auto [U,V,W]=Forbenius(A);
		int pre=0;
		for(poly&i:W){
			const auto mod=[&](poly x){
				if(x.size()<i.size()-1)x.resize(i.size()-1);
				for(int j=x.size()-1;j>=i.size()-1;--j)
					for(int k=1;k<i.size();++k)
						x[j-k]=(x[j-k]+(LL)(Mod-x[j])*i.end()[-1-k])%Mod;
				return x.resize(i.size()-1),x;
			};
			const auto mul=[&](poly x,poly y){
				poly z(x.size()+y.size()-1);
				for(int i=0;i<x.size();++i)for(int j=0;j<y.size();++j)z[i+j]=(z[i+j]+(LL)x[i]*y[j])%Mod;
				return mod(z);
			};
			poly cur({1}),a({0,1});
			for(int i=0;i<k.size();++i){
				if(k[i])cur=mul(cur,a);
				if(i+1<k.size())a=mul(a,a);
			}
			cur.resize(i.size()-1);
			for(int j=0;j<i.size()-1;++j){
				for(int k=0;k<i.size()-1;++k)
					B[pre+k][pre+j]=cur[k];
				cur.insert(cur.begin(),0),cur=mod(cur);
			}
			pre+=i.size()-1;
		}
		return U*B*V;
	}
	template<class T>inline Mat Qpow(Mat A,T k){
		vector<bool>v;
		while(k)v.emplace_back(k&1),k>>=1;
		return Qpowi(A,v);
	}
	template<>inline Mat Qpow(Mat A,string k){
		vector<bool>v;
		for(char i:k)v.emplace_back(i&1);
		reverse(v.begin(),v.end());
		return Qpowi(A,v);
	}
}
using namespace LinearAlgebra;
signed main(){
	cin.tie(0)->sync_with_stdio(0);
	int n;cin>>n,n+=n;
	Mat A(n,Vec(n));
	for(auto&i:A)for(auto&j:i)cin>>j;
	cout<<Pfaffian(A)<<'\n';
	return 0;
}