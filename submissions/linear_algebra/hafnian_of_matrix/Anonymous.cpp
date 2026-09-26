#include<bits/stdc++.h>
#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
namespace Linear_Algebra{
	typedef vector<int> poly;
	typedef vector<int> Vec;
	typedef vector<Vec> Mat;
	const int Mod=998244353;
	inline int qpow(int x,int y,int z=1){
		for(;y;(y>>=1)&&(x=(LL)x*x%Mod))if(y&1)z=(LL)z*x%Mod;return z;
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
	inline int sub(int x,const int&y){
		return (x-=y)<0?x+Mod:x;
	}
	inline int neg(const int&x){
		return x?Mod-x:0;
	}
	inline int div2(const int&x){
		return x&1?(x+Mod)>>1:x>>1;
	}
	inline Mat operator+(Mat x,const Mat&y){
		if(x.empty())return Mat();
		const int n=x.size(),m=x[0].size();
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)x[i][j]=add(x[i][j],y[i][j]);
		return x;
	}
	inline Mat operator-(Mat x,const Mat&y){
		if(x.empty())return Mat();
		const int n=x.size(),m=x[0].size();
		for(int i=0;i<n;++i)for(int j=0;j<m;++j)x[i][j]=sub(x[i][j],y[i][j]);
		return x;
	}
	inline Mat operator-(Mat x){
		for(Vec&i:x)for(int&j:i)j=neg(j);
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
		for(int&i:x)i=mul(i,y);
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
	inline int Det(Mat A){
		const int n=A.size();int z=1;
		for(int i=0;i<n;++i)
			for(int j=i+1;j<n;++j){
				while(A[i][i]){
					const uLL v=trans(A[j][i]/A[i][i]);
					for(int k=i;k<n;++k)A[j][k]=sub(A[j][k],mul(A[i][k],v));
					swap(A[i],A[j]),z=-z;
				}
				swap(A[i],A[j]),z=-z;
			}
		for(int i=0;i<n;++i)z=(LL)z*A[i][i]%Mod;
		return z<0?z+Mod:z;
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
			const uLL r=trans(qpow(A[i][i],Mod-2));
			for(int j=0;j<n;++j)A[i][j]=mul(A[i][j],r),B[i][j]=mul(B[i][j],r);
			for(int j=0;j<n;++j)if(i!=j&&A[j][i]){
				const uLL v=trans(A[j][i]);
				for(int k=0;k<n;++k)A[j][k]=sub(A[j][k],mul(A[i][k],v)),B[j][k]=sub(B[j][k],mul(B[i][k],v));
			}
		}
		return B;
	}
	inline Vec Gauss(Mat A,Vec B){
		const int n=A.size();
		for(int i=0;i<n;++i)A[i].emplace_back(B[i]);
		for(int i=0;i<n;++i){
			for(int j=i;j<n;++j)if(A[j][i]){
				swap(A[i],A[j]);
				break;
			}
			if(!A[i][i])return Vec();
			const uLL r=trans(qpow(A[i][i],Mod-2));
			for(int j=0;j<=n;++j)A[i][j]=mul(A[i][j],r);
			for(int j=0;j<n;++j)if(i!=j&&A[j][i]){
				const uLL v=trans(A[j][i]);
				for(int k=0;k<=n;++k)A[j][k]=sub(A[j][k],mul(A[i][k],v));
			}
		}
		for(int i=0;i<n;++i)B[i]=A[i].back();
		return B;
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
				for(int k=i;k<n;++k)A[j][k]=sub(A[j][k],mul(A[i+1][k],w));
				for(int k=0;k<n;++k)A[k][i+1]=add(A[k][i+1],mul(A[k][j],w));
			}
		}
		B[0][0]=1;
		for(int i=0;i<n;++i){
			for(int j=0;j<=i;++j)B[i+1][j+1]=add(B[i+1][j+1],B[i][j]);
			for(int j=i,w=Mod-1;j<n;++j){
				const uLL r=trans((LL)A[i][j]*w%Mod);
				for(int k=0;k<=i;++k)B[j+1][k]=add(B[j+1][k],mul(B[i][k],r));
				if(j<n-1)w=(LL)w*A[j+1][j]%Mod;
			}
		}
		return B[n];
	}
	inline poly Det(Mat A,Mat B){
		int z=1,m=0;
		const int n=A.size();
		A=-A;
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
					const uLL r=trans(Mod-B[i][j]);
					B[i][j]=0;
					for(int k=0;k<n;++k)A[i][k]=add(A[i][k],mul(A[j][k],r));
					for(int k=i;k<n;++k)B[i][k]=add(B[i][k],mul(B[j][k],r));
				}
			}
			z=(LL)z*B[i][i]%Mod;
			const uLL r=trans(qpow(B[i][i],Mod-2));
			for(int j=0;j<n;++j)A[i][j]=mul(A[i][j],r);
			for(int j=i;j<n;++j)B[i][j]=mul(B[i][j],r);
			for(int j=0;j<n;++j)if(i!=j){
				const uLL r=trans(Mod-B[j][i]);B[j][i]=0;
				for(int k=0;k<n;++k)A[j][k]=add(A[j][k],mul(A[i][k],r));
				for(int k=i;k<n;++k)B[j][k]=add(B[j][k],mul(B[i][k],r));
			}
		}
		if(z<0)z+=Mod;
		poly ans=Character(A);
		ans.erase(ans.begin(),ans.begin()+m);
		for(int&i:ans)i=(LL)i*z%Mod;
		return ans.resize(n+1),ans;
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
		return z<0?z+Mod:z;
	}
	inline int Hafnian(Mat A){
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
					for(int l=i+1;l<n;++l)f[1<<l|j][l+l]=add(f[1<<l|j][l+l],r);
					if(j==(1<<n)-1)ans=add(ans,r);
				}
		return ans;
	}
}
using namespace Linear_Algebra;
signed main(){
	cin.tie(0)->sync_with_stdio(0);
	int n;cin>>n;
	Mat A(n,Vec(n));
	for(auto&i:A)for(auto&j:i)cin>>j;
	cout<<Hafnian(A)<<'\n';
	return 0;
}
/*
*/