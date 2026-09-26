// Time complexity: O(n^{2/5}/log n)
// Space complexity: O(n^{1/3})

#include<bits/stdc++.h>
#define rep(i,l,r) for (int i=l; i<=r; ++i)
#define rrep(i,r,l) for (int i=r; i>=l; --i)
using namespace std;
typedef long long i64;

const int N=1e6+16;
const long double I=1+1e-19L;
i64 n; int n3,cnt,k,m,pri[80000];
bitset<N/2>v;
long double sq,inv[32000],inv2[N];
int s0[N],S0[N],sf[N],Sf[N];
struct Node { int x,f; }rough[N];

void init(int m) {
    int sq=sqrtl(m),m2=m/2;
    for (int i=3; i<=sq; i+=2) if (!v[i/2])
        for (int j=i*i/2; j<=m2; j+=i) v[j]=1;
    pri[cnt=1]=2;
    rep(i,1,m2) if (!v[i]) pri[++cnt]=i*2+1;
    pri[cnt+1]=1e8;
    int n4=powl(n,0.25);
    rep(i,1,n4) inv[i]=I/i;
    rep(i,1,m) inv2[i]=I/sqrtl(i);
}
int dv(int x,int y) { return x*inv[y]; }

void dfs(int i,int x,int mu) {
    (x>n3?Sf[n/((i64)x*x)]:sf[x])+=mu+1;
    int mx=m/x,p;
    while ((p=pri[++i])<=mx) {
        dfs(i,x*p,-mu);
        for (i64 x2=(i64)x*p*p; x2<=m; x2*=p) dfs(i,x2,0);
    }
}

void solve() {
    k=upper_bound(pri+1,pri+cnt+1,powl(n3,0.2))-pri;
    rep(i,1,n3) s0[i]=i;
    rep(i,1,n3) S0[i]=sqrtl(n/i);
    rep(j,1,k) {
        int p=pri[j],p2=p*p,t=dv(dv(n3,p),p);
        long double tn=sq*inv[p];
        rep(i,1,t) S0[i]-=S0[i*p2];
        rep(i,t+1,n3) S0[i]-=s0[int(tn*inv2[i])];
        for (int i2=dv(n3,p),i=n3; i2; --i2)
            for (int lim=i2*p; i>=lim; --i) s0[i]-=s0[i2];
    }
    m=pri[k]*n3; dfs(k,1,1);
    rep(i,2,n3) sf[i]+=sf[i-1];
    Sf[n3]+=sf[n3];
    rrep(i,n3,2) Sf[i-1]+=Sf[i];
    rep(i,1,n3) sf[i]-=s0[i];
    rep(i,1,n3) Sf[i]-=S0[i];
    int num=0;
    rep(i,pri[k+1],n3) if (s0[i]!=s0[i-1])
        rough[++num]={i,sf[i]-sf[i-1]};
    rough[num+1]={n3+1,0};
    rrep(i,n/((i64)m*m),1) {
        int n2=sqrtl(n/i),B=sqrtl(n2);
        int s=1-S0[i]+s0[B]*sf[B],t,j=1;
        for ( ; (t=i*rough[j].x*rough[j].x)<=n3; ++j)
            s-=rough[j].f*S0[t]+Sf[t];
        for ( ; rough[j].x<=B; ++j)
            t=dv(n2,rough[j].x),
            s-=rough[j].f*s0[t]+sf[t];
        Sf[i]=s;
    }
    rep(j,1,k) {
        int p=pri[j],p2=p*p,t=dv(dv(n3,p),p);
        long double tn=sq*inv[p];
        rep(i,1,t) Sf[i]-=Sf[i*p2];
        rep(i,t+1,n3) Sf[i]-=sf[int(tn*inv2[i])];
        for (int i2=dv(n3,p),i=n3; i2; --i2)
            for (int lim=i2*p; i>=lim; --i) sf[i]-=sf[i2];
    }
    i64 ans=-(i64)sf[n3]*n3;
    rep(i,1,n3) ans+=n/((i64)i*i)*(sf[i]-sf[i-1]);
    rep(i,1,n3) ans+=Sf[i];
    printf("%lld\n",ans);
}

int main() {
    scanf("%lld",&n);
    sq=sqrtl(n);
    init(n3=cbrtl(n));
    solve();
    return 0;
}