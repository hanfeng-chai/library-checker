// 2 Sat
#include <cstdio>
#include <iostream>
#include <vector>
#define ll long long
#define rep(i, s, t) for(int i=s; i<=t; ++i)
#define debug(x) cerr<<#x<<":"<<x<<endl;
const int N=1000010;
using namespace std;
char buf[1<<21], *p1=buf, *p2=buf;
#define gc() (p1==p2 && (p2=(p1=buf)+fread(buf,1,1<<21,stdin),p1==p2)?EOF:*p1++)
inline int read()
{
    int x=0, f=1; char c=gc();
    while(c<'0' || c>'9') c=='-' && (f=-1), c=gc();
    while('0'<=c && c<='9') x=(x<<3)+(x<<1)+c-'0', c=gc();
    return x*f;
}

int n, m, scc, c[N];
struct edge {int v, n;} e[N]; int tot, hd[N];
inline void add(int u, int v) {e[++tot]={v, hd[u]}, hd[u]=tot;}
int dfn[N], low[N], tim, stk[N], tp; bool in[N];
void tarjan(int u)
{
    dfn[u]=low[u]=++tim, stk[++tp]=u, in[u]=1;
    for(int i=hd[u]; i; i=e[i].n)
    {
        int v=e[i].v;
        if(!dfn[v]) tarjan(v), low[u]=min(low[u], low[v]);
        else if(in[v]) low[u]=min(low[u], dfn[v]);
    }
    if(dfn[u]==low[u])
    {
        int v=0; scc++;
        while(u!=v) v=stk[tp--], in[v]=0, c[v]=scc;
    }
}
inline int rev(int u) {return u>n?u-n:u+n;}

int main()
{
#ifdef Jerrywang
    freopen("in.txt", "r", stdin);
#endif
    n=read(), m=read();
    rep(i, 1, m)
    {
        int u=read(), v=read(), rubbish=read();
        if(u<0) u=n-u; if(v<0) v=n-v;
        add(u, rev(v)), add(v, rev(u));
    }
    rep(i, 1, n+n) if(!dfn[i]) tarjan(i);
    rep(i, 1, n) if(c[i]==c[n+i]) return puts("s UNSATISFIABLE"), 0;
    puts("s SATISFIABLE"); putchar('v');
    rep(i, 1, n) if(c[i]>c[n+i]) printf(" %d", i); else printf(" %d", -i);
    printf(" 0");

    return 0;
}