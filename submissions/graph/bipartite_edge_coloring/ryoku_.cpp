#define CP_BUNDLED_SOURCE
#ifndef CP_BUNDLE_HEADER_3840A09F237A779E
#define CP_BUNDLE_HEADER_3840A09F237A779E
#ifdef TEMPLATE
#else
#define TEMPLATE
# pragma GCC optimize("O3")
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <cassert>
#include <functional>
#include <iterator>
#include <utility>
#include <complex>
#include <bitset>
#include <chrono>
#include <random>
#include <limits>
#include <optional>
#include <variant>
#include <any>

#include <array>
#include <bit>
#include <compare>
#include <concepts>
#include <numbers>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <version>
using namespace std;
using uint=unsigned;
using ll=long long;
using ull=unsigned long long;
using ld=long double;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using i128=__int128;
using u128=unsigned __int128;
template<class T>using vc=vector<T>;
template<class T>using vvc=vc<vc<T>>;
template<class T>using vvvc=vvc<vc<T>>;
template<class T>using smpq=priority_queue<T,vector<T>,greater<T>>;
template<class T>using bipq=priority_queue<T>;
#define rep(i,n) for(ll i=0;i<(ll)(n);i++)
#define REP(i,j,n) for(ll i=(j);i<(ll)(n);i++)
#define DREP(i,n,m) for(ll i=(n);i>=(m);i--)
#define drep(i,n) for(ll i=((n)-1);i>=0;i--)
#define rall(x) x.rbegin(),x.rend()
#define mp(...) make_pair(__VA_ARGS__)
#define pb push_back
#define fi first
#define se second
#define is insert
#define bg begin()
#define ed end()
#define all(x) x.begin(),x.end()
void scan(int&a) { cin >> a; }
void scan(ll&a) { cin >> a; }
void scan(string&a) { cin >> a; }
void scan(char&a) { cin >> a; }
void scan(uint&a) { cin >> a; }
void scan(ull&a) { cin >> a; }
void scan(bool&a) { cin >> a; }
void scan(ld&a){ cin>> a;}
template<class T> void scan(vector<T>&a) { for(auto&x:a) scan(x); }
void read() {}
template<class Head, class... Tail> void read(Head&head, Tail&... tail) { scan(head); read(tail...); }
#define INT(...) int __VA_ARGS__; read(__VA_ARGS__);
#define LL(...) ll __VA_ARGS__; read(__VA_ARGS__);
#define ULL(...) ull __VA_ARGS__; read(__VA_ARGS__);
#define STR(...) string __VA_ARGS__; read(__VA_ARGS__);
#define VC(type, name, ...) vector<type> name(__VA_ARGS__); read(name);
#define VVC(type, name, size, ...) vector<vector<type>> name(size, vector<type>(__VA_ARGS__)); read(name);
template<class T>void print(T a) { cout << a; }
template<class T> void print(vector<T>a) { for(int i=0;i<(int)a.size();i++){if(i)cout<<" ";print(a[i]);}cout<<endl;}
void PRT() { cout <<endl; return ; }
template<class T> void PRT(T a) { print(a); cout <<endl; return; }
template<class Head, class... Tail> void PRT(Head head, Tail ... tail) { print(head); cout << " "; PRT(tail...); return; }
template<class T,class F>
bool chmin(T &x, F y){
    if(x>y){
        x=y;
        return true;
    }
    return false;
}
template<class T, class F>
bool chmax(T &x, F y){
    if(x<y){
        x=y;
        return true;
    }
    return false;
}
template <typename T>
T floor(T a, T b) {
  return a / b - (a % b && (a ^ b) < 0);
}
template <typename T>
T ceil(T x, T y) {
  return floor(x + y - 1, y);
}
template <typename T>
T bmod(T x, T y) {
  return x - y * floor(x, y);
}
template <typename T>
pair<T, T> divmod(T x, T y) {
  T q = floor(x, y);
  return {q, x - q * y};
}
void YesNo(bool b){
    cout<<(b?"Yes":"No")<<endl;
}
void YESNO(bool b){
    cout<<(b?"YES":"NO")<<endl;
}
void AliceBob(bool b){
    cout<<(b?"Alice":"Bob")<<endl;
}
void TakahashiAoki(bool b){
    cout<<(b?"Takahashi":"Aoki")<<endl;
}
void Yes(){
    cout<<"Yes"<<endl;
}
void No(){
    cout<<"No"<<endl;
}
vc<int>stovi(const string&s,const string&S){
    vc<int>v(s.size());
    rep(i,s.size()){
        auto t=S.find(s[i]);
        assert(t!=string::npos);
        v[i]=t;
    }
    return v;
}
template<class T=ll>
T isqrt(T x){
    T F=sqrtl(x);
    while((F+1)*(F+1)<=x)F++;
    while(F*F>x)F--;
    return F;
}
template<class T>
vvc<T>trans(const vvc<T>&a){
    assert(a.size()&&a[0].size());
    vvc<T>b(a[0].size(),vc<T>(a.size()));
    rep(i,a.size())rep(j,a[0].size()){
        b[j][i]=a[i][j];
    }
    return b;
}
template<class T>
vc<string>trans(const vc<string>&a){
    assert(a.size()&&a[0].size());
    vc<string>b(a[0].size(),string(a.size(),0));
    rep(i,a.size())rep(j,a[0].size()){
        b[j][i]=a[i][j];
    }
    return b;
}
template<class T>
int popcount(T n){
    return __builtin_popcountll(n);
}
template<class T,class L=ll>
L sum(vc<T>&a){
    return accumulate(all(a),L(0));
}
template<class T>
vc<T>subset(T S){
    vc<T>ans;
    for(T x=S;x>0;x=(x-1)&S)ans.pb(x);
    ans.pb(0);
    return ans;
}
template<class T>
T max(vc<T>&a){
    return *max_element(all(a));
}
template<class T>
T min(vc<T>&a){
    return *min_element(all(a));
}
template<class T,class L=ll>
vc<L> presum(vc<T> &a){
    vc<L> ret(a.size()+1);
    rep(i,a.size())ret[i+1]=ret[i]+a[i];
    return ret;
}
template<class T, class F>
vc<T> &operator+=(vc<T> &a,F b){
    for (auto&v:a)v += b;
    return a;
}
template<class T, class F>
vc<T> &operator-=(vc<T>&a,F b){
    for (auto&v:a)v-=b;
    return a;
}
template<class T, class F>
vc<T> &operator*=(vc<T>&a,F b){
    for (auto&v:a)v*=b;
    return a;
}
template<class T=ll>
constexpr T pow(T a,T b){
    T res=1;
    while(b){
        if(b&1)res*=a;
        a*=a;
        b/=2;
    }
    return res;
}
constexpr ll ten(ll a){
    return pow<ll>(10,a);
}
template<typename T>constexpr T inf=numeric_limits<T>::max()/2-1;
template<class T>
int tbit(T x){
    using U=make_unsigned_t<T>;
    U y=(U)x;
    return y?(int)bit_width(y)-1:-1;
}
template<class T>
int lbit(T x){
    using U=make_unsigned_t<T>;
    U y=(U)x;
    return y?(int)countr_zero(y):-1;
}
template<class T>
int tbit(T x,int p){
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    U y=(U)x;
    if(p<0)return -1;
    if(p>=W-1)return tbit(y);
    return tbit(y&((U(1)<<(p+1))-1));
}
template<class T>
int lbit(T x,int p){
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    U y=(U)x;
    if(p<0)return lbit(y);
    if(p>=W)return -1;
    return lbit(y&(~U(0)<<p));
}
istream& operator>>(istream&is,i128&x){
    string s;is>>s;
    x=0;
    int i=0,neg=0;
    if(s[0]=='-')neg=1,i=1;
    for(;i<(int)s.size();i++)x=x*10+s[i]-'0';
    if(neg)x=-x;
    return is;
}
ostream& operator<<(ostream&os,i128 x){
    if(x==0)return os<<0;
    if(x<0)os<<"-";
    u128 y=x<0?-(u128)x:(u128)x;
    string s;
    while(y)s.pb('0'+y%10),y/=10;
    reverse(all(s));
    return os<<s;
}
#ifdef LOCAL
#include "debug/debug.hpp"
#else
#define dbg(...) ((void)0)
#endif

struct template_setup{
    template_setup(){
        #ifdef LOCAL
        freopen("input.txt","r",stdin);
        freopen("output.txt","w",stdout);
        
        #endif
        cin.tie(0)->sync_with_stdio(0);
        #ifdef LOCAL
        cout<<fixed<<setprecision(6);
        dbg("==============="s);
        #else
        cout<<fixed<<setprecision(20);
        #endif
    }
};
inline template_setup template_setup;

#if defined(LOCAL)&&!defined(CP_NO_AUTO_ALL)&&!defined(CP_BUNDLED_SOURCE)
#include <all.hpp>
#endif

#endif
#endif
#ifndef CP_BUNDLE_HEADER_DFBD140B71193A16
#define CP_BUNDLE_HEADER_DFBD140B71193A16
#ifndef CP_BUNDLE_HEADER_A415336C78CC3AB3
#define CP_BUNDLE_HEADER_A415336C78CC3AB3
struct unweighted{
    unweighted()=default;
    unweighted(int){}
    operator int()const{return 1;}
};
template<class T=unweighted>
struct edge{
    int from,to,id;
    [[no_unique_address]]T cost;
#ifdef LOCAL
    friend ostream&operator<<(ostream&os,const edge&e){
        return os<<"{from:"<<e.from<<",to:"<<e.to<<",id:"<<e.id<<",cost:"<<e.cost<<"}";
    }
#endif
};
template<bool is_directed,class T=unweighted>
struct static_graph{
    constexpr static bool directed(){return is_directed;}
    using edge=::edge<T>;
    using cost_t=T;
private:
    int n;
    mutable bool built=false,inv_built=false;
    vc<edge>edges;
    mutable vc<int>start,inv_start;
    mutable vc<edge>csr,inv_csr;
public:
    static_graph(int n):n(n),start(n+1),inv_start(n+1){assert(n>=0);}
    static_graph(int n,int m):static_graph(n){assert(m>=0);edges.reserve(m);}
    void resize(int size){
        assert(n<=size&&!built);
        assert(!inv_built);
        n=size;
        start.resize(n+1);
        inv_start.resize(n+1);
    }
    void add_edge(const edge&e){
        assert(!built&&!inv_built);
        assert(0<=e.from&&e.from<n&&0<=e.to&&e.to<n);
        edges.pb(e);
    }
    void add_edge(int a,int b,cost_t cost=1,int id=-1){
        assert(!built&&!inv_built);
        assert(0<=a&&a<n&&0<=b&&b<n);
        if(id==-1)id=edges.size();
        edges.pb({a,b,id,cost});
    }
    template<int substract=0>
    void input(int m){
        assert(m>=0);
        rep(i,m){
            INT(a,b);
            a-=substract;b-=substract;
            add_edge(a,b);
        }
        build();
    }
    void build()const{
        if(built)return;
        built=true;
        start.assign(n+1,0);
        for(auto&e:edges){
            ++start[e.from];
            if constexpr(!is_directed)++start[e.to];
        }
        rep(i,n)start[i+1]+=start[i];
        csr.resize(start[n]);
        for(auto it=edges.rbegin();it!=edges.rend();++it){
            auto&e=*it;
            csr[--start[e.from]]=e;
            if constexpr(!is_directed)csr[--start[e.to]]={e.to,e.from,e.id,e.cost};
        }
    }
    void buildinv()const{
        if(inv_built)return;
        inv_start.assign(n+1,0);
        for(auto&e:edges){
            ++inv_start[e.to];
            if constexpr(!is_directed)++inv_start[e.from];
        }
        rep(i,n)inv_start[i+1]+=inv_start[i];
        inv_csr.resize(inv_start[n]);
        for(auto it=edges.rbegin();it!=edges.rend();++it){
            auto&e=*it;
            inv_csr[--inv_start[e.to]]={e.to,e.from,e.id,e.cost};
            if constexpr(!is_directed)inv_csr[--inv_start[e.from]]={e.from,e.to,e.id,e.cost};
        }
        inv_built=true;
    }
    auto operator[](int u){
        build();
        assert(0<=u&&u<n);
        return span<edge>(csr.data()+start[u],start[u+1]-start[u]);
    }

    auto operator[](int u)const{
        build();
        assert(0<=u&&u<n);
        return span<const edge>(csr.data()+start[u],start[u+1]-start[u]);
    }

    auto inv(int u){
        buildinv();
        assert(0<=u&&u<n);
        return span<edge>(inv_csr.data()+inv_start[u],inv_start[u+1]-inv_start[u]);
    }

    auto inv(int u)const{
        buildinv();
        assert(0<=u&&u<n);
        return span<const edge>(inv_csr.data()+inv_start[u],inv_start[u+1]-inv_start[u]);
    }

    const vc<edge>&all_edges()const{return edges;}
    int edge_size()const{return edges.size();}

    edge get_edge(int id)const{
        assert(0<=id&&id<edge_size());
        return edges[id];
    }
    int out_deg(int u)const{
        build();
        assert(0<=u&&u<n);
        return start[u+1]-start[u];
    }
    int in_deg(int u)const{
        buildinv();
        assert(0<=u&&u<n);
        return inv_start[u+1]-inv_start[u];
    }
    int deg(int u)const{return out_deg(u);}
    int size()const{return n;}
    template<class F>
    vvc<F>adj()const{
        vvc<F>res(n,vc<F>(n));
        for(auto&e:edges){
            res[e.from][e.to]=e.cost;
            if constexpr(!is_directed)res[e.to][e.from]=e.cost;
        }
        return res;
    }
    void clear(){
        built=false;
        inv_built=false;
        edges.clear();
        csr.clear();
        inv_csr.clear();
        start.assign(n+1,0);
        inv_start.assign(n+1,0);
    }
    template<class F>
    void sort(int i,F f){
        build();
        assert(0<=i&&i<n);
        std::sort(csr.begin()+start[i],csr.begin()+start[i+1],f);
    }
};
#endif
struct bipartite_matching{
    using graph=static_graph<0>;
    int l,r;
    vc<int>pm,qm;
    vvc<int>g;
    vc<graph::edge>es;
    vc<int>pe;
    vc<int>dist;
    bipartite_matching(int l,int r):l(l),r(r){
        assert(l>=0&&r>=0);
        pm.assign(l,-1);
        qm.assign(r,-1);
        g.resize(l);
        pe.assign(l,-1);
        dist.resize(l);
    }
    void add_edge(int a,int b,int id=-1){
        assert(0<=a&&a<l);
        assert(0<=b&&b<r);
        if(id==-1)id=es.size();
        es.pb({a,b,id});
        g[a].push_back(es.size()-1);
    }
    bool bfs(){
        queue<int>que;
        bool find=0;
        fill(all(dist),-1);
        rep(i,l){
            if(pm[i]==-1){
                dist[i]=0;
                que.push(i);
            }
        }
        while(que.size()){
            auto u=que.front();que.pop();
            for(auto&x:g[u]){
                int nu=qm[es[x].to];
                if(nu==-1){
                    find=1;
                }else if(dist[nu]==-1){
                    dist[nu]=dist[u]+1;
                    que.push(nu);
                }
            }
        }
        return find;
    }
    bool dfs(int u){
        assert(0<=u&&u<l);
        for(auto&x:g[u]){
            int nu=qm[es[x].to];
            if(nu==-1||(dist[nu]==dist[u]+1&&dfs(nu))){
                qm[es[x].to]=u;
                pm[u]=es[x].to;
                pe[u]=x;
                return 1;
            }
        }
        dist[u]=-1;
        return 0;
    }
    vc<graph::edge> work(){
        while(bfs()){
            rep(i,l)if(pm[i]==-1)dfs(i);
        }
        vc<graph::edge>ans;
        rep(i,l)if(pm[i]!=-1)ans.pb(es[pe[i]]);
        return ans;
    }
};
#endif
#ifndef CP_BUNDLE_HEADER_9DAAB5F384944713
#define CP_BUNDLE_HEADER_9DAAB5F384944713
template<class T,auto op,int extra>
struct base_disjoint_set_union{
        vector<int>par;
        vector<T>data;
        base_disjoint_set_union(int n){
            static_assert(!extra,"e is needed");
            assert(n>=0);
            par.assign(n,-1);
        }
        base_disjoint_set_union(int n,T e){
            assert(n>=0);
            par.assign(n,-1);
            data.assign(n,e);
        }
        T& operator[](int i){
            static_assert(extra,"no data");
            return data[leader(i)];
        }
        int root(int x){
            assert(0<=x&&x<par.size());
            if(par[x]<0)return x;
            else return par[x]=root(par[x]);
        }
        bool same(int x,int y){
            assert(0<=x&&x<par.size()&&0<=y&&y<par.size());
            return root(x)==root(y);
        }
        bool merge(int x,int y){
            assert(0<=x&&x<par.size()&&0<=y&&y<par.size());
            x=root(x);
            y=root(y);
            if(x==y)return false;
            if(par[x]>par[y])swap(x,y);
            par[x]+=par[y];
            par[y]=x;
            if constexpr(extra){
                data[x]=op(data[y],data[x]);
            }
            return true;
        }
        int size(int x){
            assert(0<=x&&x<par.size());
            return -par[root(x)];
        }
        int leader(int x){
            assert(0<=x&&x<par.size());
            return root(x);
        }

};
int tf323(int a,int b){return a;}
using disjoint_set_union=base_disjoint_set_union<int,tf323,0>;
template<class T,auto op>
using extra_disjoint_set_union=base_disjoint_set_union<T,op,1>;
#endif
using Graph=static_graph<0>;
vc<int>bipatite_edge_coloring(vc<pii>edge,int L,int R){
    //D regular graph part
    vc<int>d1(L),d2(R);
    for(auto&[x,y]:edge)d1[x]++,d2[y]++;
    int D=max(max(d1),max(d2));
    if(D==0)return{};
    auto build=[&](int N,vc<int>d1){
        disjoint_set_union dsu(N);
        smpq<pii>que;rep(i,N)que.push({d1[i],i});
        while(que.size()>1){
            auto p=que.top();que.pop();
            auto q=que.top();que.pop();
            if(p.fi+q.fi>D)break;
            que.push({p.fi+q.fi,p.se});
            dsu.merge(p.se,q.se);
        }
        return dsu;
    };
    auto dsuL=build(L,d1);
    auto dsuR=build(R,d2);
    int ML=0,MR=0;rep(i,L)ML+=dsuL.root(i)==i;rep(i,R)MR+=dsuR.root(i)==i;
    int N=max(ML,MR);
    d1.assign(N,0),d2.assign(N,0);
    static_graph<0>g(2*N);
    vc<int>vaal(L,-1),vaar(R,-1);
    int iil=0,iir=0;
    for(auto&[x,y]:edge){
        x=dsuL.root(x),y=dsuR.root(y);
        if(vaal[x]==-1)vaal[x]=iil++;
        if(vaar[y]==-1)vaar[y]=iir++;
        x=vaal[x],y=vaar[y];
        g.add_edge(x,y+N);
        d1[x]++,d2[y]++;
    }
    queue<pii>qL,qR;
    rep(i,N)if(d1[i]<D)qL.push({d1[i],i});
    rep(i,N)if(d2[i]<D)qR.push({d2[i],i});
    while(qL.size()&&qR.size()){
        auto li=qL.front();qL.pop();
        auto ri=qR.front();qR.pop();
        g.add_edge(li.se,ri.se+N);
        li.fi=++d1[li.se];
        ri.fi=++d2[ri.se];
        if(li.fi<D)qL.push(li);
        if(ri.fi<D)qR.push(ri);
    }
    //---------------------------
    int M=D*N;
    vc<int>ord(M);iota(all(ord),0);
    vc<int>waier(N*2,-1);
    vc<array<int,2>>twin(M);
    vc<int>side(M,-1);
    vc<int>first_match(M);
    auto divide_segment=[&](int L,int R){
        rep(z,2)REP(i,L,R){
            int eey=ord[i];
            int lvc=g.get_edge(eey).from;if(z)lvc=g.get_edge(eey).to;
            if(waier[lvc]==-1){
                waier[lvc]=eey;
            }else{
                twin[eey][z]=waier[lvc];
                twin[waier[lvc]][z]=eey;
                waier[lvc]=-1;
            }
        }
        REP(i,L,R){
            int eey=ord[i];if(side[eey]!=-1)continue;
            int z=0;
            side[eey]=z;
            while(1){
                eey=twin[eey][z];z^=1;
                if(eey==ord[i])break;
                side[eey]=z;
            }
        }
        partition(ord.begin()+L,ord.begin()+R,[&](int eid){
            return side[eid]==0;
        });
        REP(i,L,R)side[ord[i]]=-1;
    };
    auto dfs=[&](auto&dfs,int L,int R,int D)->void{
        if(D==1)return;
        if(D%2==0){ 
            divide_segment(L,R);
            dfs(dfs,L,L+R>>1,D/2);
            dfs(dfs,L+R>>1,R,D/2);
        }else{
            using EdgeSeg=pair<int,int>;
            vc<EdgeSeg>abb;
            bipartite_matching bm(N,N);
            REP(i,L,R)bm.add_edge(g.get_edge(ord[i]).from,g.get_edge(ord[i]).to-N,g.get_edge(ord[i]).id);
            auto res=bm.work();
            for(auto&e:res)first_match[e.id]=1;
            partition(ord.begin()+L,ord.begin()+R,[&](int eid){
                return first_match[eid]==1;
            });
            divide_segment(L+N,R);
            int LRM=(L+N+R)>>1;
            abb={{L,L+N},{L+N,LRM},{LRM,R}};
            vc<EdgeSeg>T;
            while(1){   
                auto W=[&](pii x){return x.se-x.fi==N;};
                if(W(abb[0])&&W(abb[1])&&W(abb[2]))break;
                 if((abb[1].se-abb[1].fi)/N%2==0){
                    T.pb(abb[2]);
                    int M=abb[1].se+abb[1].fi>>1;
                    divide_segment(abb[1].fi,abb[1].se);
                    abb[2]={M,abb[1].se};
                    abb[1]={abb[1].fi,M};
                }else{
                    divide_segment(abb[0].fi,abb[1].se);
                    int W=(abb[1].se-abb[0].fi)>>1;
                    int W2=(abb[2].se-abb[2].fi);
                    rotate(ord.begin()+abb[0].fi,ord.begin()+abb[0].fi+W*2,ord.begin()+abb[2].se);
                    abb[0]={L,L+W2};
                    abb[1]={L+W2,L+W2+W};
                    abb[2]={L+W2+W,L+W2+W*2};
                }
            }
            while(T.size()){
                auto TLR=T.back();T.pop_back();
                int next=1<<tbit((TLR.se-TLR.fi)/N);
                if(next<(TLR.se-TLR.fi)/N)next*=2;
                int make=(next-(TLR.se-TLR.fi)/N)*N;
                int L=TLR.fi-make;
                int R=TLR.se;
                dfs(dfs,L,R,(R-L)/N);
            }
        }
    };
    dfs(dfs,0,D*N,D);
    vc<int>ans(edge.size());
    rep(i,M)if(ord[i]<edge.size())ans[ord[i]]=i/N;
    return ans;
}
void solve(){
    INT(l,r,m);
    vc<pii>es;
    rep(i,m){
        INT(a,b);es.pb({a,b});
    }
    auto ans=bipatite_edge_coloring(es,l,r);
    cout<<max(ans)+1<<"\n";
    rep(i,ans.size())cout<<ans[i]<<"\n";
}
signed main(){
    int t=1;
    // cin >> t;
    while(t--)solve();
}