#include<bits/stdc++.h>
using namespace std;
using ll=long long;
using lll=unsigned __int128_t;

constexpr int mod=998244353;
int x[105];

lll g[105];
ll ans=0;

void dfs(int v, lll bs, ll prod){
        ans+=prod;
        if(ans>=mod)ans-=mod;
        lll nxt=bs&g[v];
        int i=0;
        while(nxt){
                i=(ll)nxt?__builtin_ctzll((ll)nxt):64+__builtin_ctzll((ll)(nxt>>64));
                dfs(i,nxt,prod*x[i]%mod);
                nxt^=((lll)1<<i);
        }
}

int main(){
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
        short n,m;
        cin>>n>>m;
        for(short i=0;i<n;++i)cin>>x[i];
        while(m--){
                short a,b;
                cin>>a>>b;
                g[a]|=((lll)1<<b);
                g[b]|=((lll)1<<a);
        }
        for(short i=0;i<n;++i){
                lll msk=~((lll)0);
                msk<<=(i+1);
                lll bs=g[i]&msk;
                dfs(i,bs,x[i]);
        }
        cout<<ans<<'\n';
}