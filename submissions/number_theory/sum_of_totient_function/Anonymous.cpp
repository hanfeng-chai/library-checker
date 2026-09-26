// Time complexity: O(n^{2/3}/log n)
// Space complexity: O(n^{1/2})

#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
#define rep(i,l,r) for (int i=l; i<=r; ++i)
#define rrep(i,r,l) for (int i=r; i>=l; --i)
using namespace std;
typedef long long i64;

const int N=1e5+10,P=998244353;
const double I=1+1e-15;
i64 n; int cnt,k,sq,m,pri[10000];
bitset<N/2>v; double inv[N];
int sI[N],SI[N],smu[N],Smu[N];
struct Node { int x,mu; }roughs[N];

void init(int n) {
    int n2=n/2,sq=sqrt(n);
    for (int i=3; i<=sq; i+=2) if (!v[i/2])
        for (int j=i*i/2; j<=n2; j+=i) v[j]=1;
    pri[cnt=1]=2;
    rep(i,1,n2) if (!v[i]) pri[++cnt]=i*2+1;
    rep(i,1,n) inv[i]=I/i;
}

i64 dv(i64 x,int y) { return x*inv[y]; }
i64 S(i64 x) { return x%=P,x*(x+1ll)/2%P; }

void solve() {
    rep(i,1,sq) sI[i]=i;
    rep(i,1,sq) SI[i]=dv(n,i);
    rep(j,1,k) {
        int p=pri[j],t=dv(sq,p); i64 tn=dv(n,p);
        rep(i,1,t) SI[i]-=SI[i*p];
        rep(i,t+1,sq) SI[i]-=sI[dv(tn,i)];
        for (int i2=t,i=sq; i2; --i2) {
            int mn=i2*p,tmp=sI[i2];
            for ( ; i>=mn; --i) sI[i]-=tmp;
        }
    }
    smu[1]=2;
    rep(j,k+1,cnt) {
        int p=pri[j],p2=p*p;
        if (p2>m) break;
        int t=dv(sq,p),tn=dv(n,p),tm=dv(m,p),j2,q;
        for (j2=j+1; (q=pri[j2])<=t; ++j2) smu[p*q]+=2;
        for ( ; (q=pri[j2])<=tm; ++j2) Smu[dv(tn,q)]+=2;
        t=dv(t,p),tn=dv(tn,p),tm=dv(tm,p);
        p2<=sq?++smu[p2]:++Smu[tn];
        for (j2=k+1; (q=pri[j2])<=t; ++j2) ++smu[p2*q];
        for ( ; (q=pri[j2])<=tm; ++j2) ++Smu[dv(tn,q)];
    }
    rep(i,1,sq) smu[i]+=smu[i-1];
    Smu[sq]+=smu[sq];
    rrep(i,sq,1) Smu[i-1]+=Smu[i];
    rep(i,1,sq) smu[i]-=sI[i];
    rep(i,1,sq) Smu[i]-=SI[i];
    int num=0;
    rep(i,2,sq) if (sI[i]!=sI[i-1])
        roughs[++num]={i,smu[i]-smu[i-1]};
    roughs[num+1]={sq+1,0};
    rrep(i,n/m,1) {
        i64 n2=dv(n,i); int B=sqrt(n2),j,t;
        int s=1-SI[i]+sI[B]*smu[B];
        for (j=1; (t=i*roughs[j].x)<=sq; ++j)
            s-=Smu[t]+SI[t]*roughs[j].mu;
        for ( ; roughs[j].x<=B; ++j) {
            t=dv(n2,roughs[j].x);
            s-=smu[t]+sI[t]*roughs[j].mu;
        }
        Smu[i]=s;
    }
    rrep(j,k,1) {
        int p=pri[j],t=dv(sq,p); i64 tn=dv(n,p);
        rep(i,1,t) Smu[i]-=Smu[i*p];
        rep(i,t+1,sq) Smu[i]-=smu[dv(tn,i)];
        for (int i2=t,i=sq; i2; --i2) {
            int mn=i2*p,tmp=smu[i2];
            for ( ; i>=mn; --i) smu[i]-=tmp;
        }
    }
    i64 Sphi=0;
    rep(i,1,sq) Sphi+=i*Smu[i];
    rep(i,1,sq) Sphi+=(smu[i]-smu[i-1])*S(dv(n,i));
    Sphi=(Sphi-S(sq)*smu[sq])%P;
    printf("%lld\n",Sphi<0?Sphi+P:Sphi);
}

int main() {
    scanf("%lld",&n);
    init(sq=sqrt(n));
    k=upper_bound(pri+1,pri+cnt+1,cbrt(sq))-pri;
    m=pri[k]*sq;
    solve();
    return 0;
}