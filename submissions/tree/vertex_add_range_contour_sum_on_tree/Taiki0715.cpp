#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdlib>
#include<type_traits>
namespace fastio{
static constexpr u_int32_t buf_size=1<<17;
char ibuf[buf_size];
char obuf[buf_size];
u_int32_t pil=0,pir=0,por=0;
struct Pre{
  char t[40000];
  constexpr Pre(){
    for(int i=0;i<10000;i++){
      int n=i;
      for(int j=3;j>=0;j--){
        t[i*4+j]='0'+n%10;
        n/=10;
      }
    }
  }
}constexpr pre;
inline void load(){
  if(pir-pil<=pil)memcpy(ibuf,ibuf+pil,pir-pil);
  else memmove(ibuf,ibuf+pil,pir-pil);
  pir=pir-pil+fread(ibuf+pir-pil,1,buf_size-pir+pil,stdin);
  pil=0;
}
inline void flush(){
  fwrite(obuf,1,por,stdout);
  por=0;
}
inline void rd(char&c){c=ibuf[pil++];}
inline void rd(std::string&s){
  char c;
  s.clear();
  if(pil==pir)load();
  rd(c);
  if(pil==pir)load();
  while(!std::isspace(c)){
    s+=c;
    rd(c);
    if(pil==pir)load();
  }
}
inline void rd(__int128_t&x){
  if(pil+50>pir)load();
  char c;
  do rd(c); while(c<'-');
  bool neg=false;
  if(c=='-'){
    neg=true;
    rd(c);
  }
  x=0;
  while(c>='0'){
    x=x*10+(c&15);
    rd(c);
  }
  if(neg)x=-x;
}
template<typename T>
inline void rd(T&x){
  if(pil+32>pir)load();
  char c;
  do rd(c); while(c<'-');
  bool neg=false;
  if constexpr(std::is_signed_v<T>){
    if(c=='-'){
      neg=true;
      rd(c);
    }
  }
  x=0;
  while(c>='0'){
    x=x*10+(c&15);
    rd(c);
  }
  if constexpr(std::is_signed_v<T>){
    if(neg)x=-x;
  }
}
inline void wt(char x){obuf[por++]=x;}
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr>
inline void wt(T x){
  if(por+32>buf_size)flush();
  if(x==0){
    wt('0');
    return;
  }
  if constexpr(std::is_signed_v<T>){
    if(x<0){
      wt('-');
      x=-x;
    }
  }
  if(x>=10000000000000000){
    u_int32_t r1=x%100000000;
    u_int64_t q1=x/100000000;
    u_int32_t r2=q1%100000000;
    u_int32_t q2=q1/100000000;
    u_int32_t n1=r1%10000,n2=r1/10000,n3=r2%10000,n4=r2/10000;
    if(x>=1000000000000000000){
      if constexpr(std::is_unsigned_v<T>){
        if(x>=10000000000000000000ull){
          obuf[por++]='1';
        }
      }
      memcpy(obuf+por,pre.t+(q2<<2)+1,3);
      memcpy(obuf+por+3,pre.t+(n4<<2),4);
      memcpy(obuf+por+7,pre.t+(n3<<2),4);
      memcpy(obuf+por+11,pre.t+(n2<<2),4);
      memcpy(obuf+por+15,pre.t+(n1<<2),4);
      por+=19;
    }
    else if(x>=100000000000000000){
      u_int32_t q3=(q2*205)>>11;
      u_int32_t r3=q2-q3*10;
      obuf[por]='0'+q3;
      obuf[por+1]='0'+r3;
      memcpy(obuf+por+2,pre.t+(n4<<2),4);
      memcpy(obuf+por+6,pre.t+(n3<<2),4);
      memcpy(obuf+por+10,pre.t+(n2<<2),4);
      memcpy(obuf+por+14,pre.t+(n1<<2),4);
      por+=18;
    }
    else{
      obuf[por]='0'+q2;
      memcpy(obuf+por+1,pre.t+(n4<<2),4);
      memcpy(obuf+por+5,pre.t+(n3<<2),4);
      memcpy(obuf+por+9,pre.t+(n2<<2),4);
      memcpy(obuf+por+13,pre.t+(n1<<2),4);
      por+=17;
    }
  }
  else{
    static char buf[12];
    int i=8;
    while(x>=10000){
      memcpy(buf+i,pre.t+((x%10000)<<2),4);
      x/=10000;
      i-=4;
    }
    if(x<100){
      if(x<10)obuf[por++]='0'+x;
      else{
        obuf[por]='0'+x/10;
        obuf[por+1]='0'+x%10;
        por+=2;
      }
    }
    else{
      if(x<1000){
        memcpy(obuf+por,pre.t+(x<<2)+1,3);
        por+=3;
      }
      else{
        memcpy(obuf+por,pre.t+(x<<2),4);
        por+=4;
      }
    }
    memcpy(obuf+por,buf+i+4,8-i);
    por+=8-i;
  }
}
inline void wt(const std::string&s){
  int sz=s.size();
  if(por+sz>buf_size)flush();
  memcpy(obuf+por,s.c_str(),sz);
  por+=sz;
}
inline void wt(__int128_t x){
  if(por+50>buf_size)flush();
  if(x==0){
    wt('0');
    return;
  }
  if(x<0){
    wt('-');
    x=-x;
  }
  static constexpr __int128_t b=10000000000000000000ull;
  if(x>=b){
    wt<unsigned long long>(x/b);
    unsigned long long y=x%b;
    memcpy(obuf+por,pre.t+((y/10000000000000000)<<2)+1,3);
    memcpy(obuf+por+3,pre.t+((y/1000000000000%10000)<<2),4);
    memcpy(obuf+por+7,pre.t+((y/100000000%10000)<<2),4);
    memcpy(obuf+por+11,pre.t+((y/10000%10000)<<2),4);
    memcpy(obuf+por+15,pre.t+((y%10000)<<2),4);
    por+=19;
  }
  else wt<unsigned long long>(x);
}
struct Dummy{
  Dummy(){std::atexit(flush);}
}dummy;
}
using fastio::rd;
using fastio::wt;
#include<queue>
#include<iostream>
#include<vector>
#include<cassert>
#include<algorithm>
#include<limits>
template<typename T=int>
struct Edge{
  int from,to;
  T weight;
  int index;
  Edge(int from_,int to_,T weight_=T(),int index_=-1):from(from_),to(to_),weight(weight_),index(index_){}
  Edge():from(-1),to(-1),weight(),index(-1){}
  friend std::ostream &operator<<(std::ostream &os,const Edge&e){
    os<<'[';
    os<<"from:"<<e.from;
    os<<"to:"<<e.to;
    os<<"weight:"<<e.weight;
    os<<"index:"<<e.index;
    os<<']';
    return os;
  }
};
template<typename T=int>
struct Tree{
  int n,r;
  std::vector<Edge<T>>edge;
  std::vector<Edge<T>>g;
  std::vector<int>ptr;
  struct tree_range{
    using iterator=typename std::vector<Edge<T>>::iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    bool empty()const{return !size();}
    Edge<T> &operator[](int i)const{return l[i];}
  };
  struct const_tree_range{
    using iterator=typename std::vector<Edge<T>>::const_iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    bool empty()const{return !size();}
    const Edge<T> &operator[](int i)const{return l[i];}
  };
  explicit Tree(int n_):n(n_){
    edge.reserve(n-1);
  }
  Tree():n(0){}
  Tree(int n_,const std::vector<Edge<T>>&e,bool dir=false):n(n_),r(-1),edge(e){
    if(!dir)build();
    else{
      std::vector<bool>seen(n,false);
      ptr.resize(n+1);
      for(const Edge<T>&i:edge)ptr[i.from]++,ptr[i.to]++,seen[e.to]=true;
      for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
      r=std::find(seen.begin(),seen.end(),false)-seen.begin();
      assert(ptr[n]==n*2-2);
      g.resize(ptr[n]);
      for(const Edge<T>&i:edge)g[--ptr[i.to]]=Edge<T>(i.to,i.from,i.weight,i.index);
      for(const Edge<T>&i:edge)g[--ptr[i.from]]=i;
    }
  }
  template<bool weighted=false,bool index=1>
  void read(){
    for(int i=0;i<n-1;i++){
      int u,v;
      T w=T();
      std::cin>>u>>v;
      if constexpr(index)u--,v--;
      if constexpr(weighted)std::cin>>w;
      else w=1;
      edge.emplace_back(u,v,w,i);
    }
    build();
  }
  template<bool index=1>
  void readp(){
    ptr.resize(n+1);
    for(int i=1;i<n;i++){
      int p;
      std::cin>>p;
      if constexpr(index)p--;
      edge.emplace_back(p,i,1,i-1);
      ptr[p]++;
      ptr[i]++;
    }
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    g.resize(n*2-2);
    for(auto&&[u,v,w,i]:edge)g[--ptr[v]]=Edge<T>(v,u,w,i);
    for(int i=0;i<n-1;i++){
      g[--ptr[edge[i].from]]=edge[i];
    }
    r=0;
  }
  void add_edge(int u,int v){edge.emplace_back(u,v,1,edge.size());}
  void add_edge(int u,int v,T w){edge.emplace_back(u,v,w,edge.size());}
  void add_edge(int u,int v,T w,int idx){edge.emplace_back(u,v,w,idx);}
  inline bool is_directed()const{return r!=-1;}
  void build(){
    r=-1;
    ptr.resize(n+1,0);
    for(auto&&[u,v,w,i]:edge)ptr[u]++,ptr[v]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    assert(ptr[n]==n*2-2);
    g.resize(n*2-2);
    for(auto&&[u,v,w,i]:edge){
      g[--ptr[u]]=Edge(u,v,w,i);
      g[--ptr[v]]=Edge(v,u,w,i);
    }
  }
  void remove_parent(int root=0){
    edge.resize(n-1);
    std::vector<int>par(n,-1);
    par[root]=-1;
    std::queue<int>que;
    que.push(root);
    while(!que.empty()){
      int x=que.front();
      que.pop();
      for(int i=ptr[x];i<ptr[x+1];){
        const Edge<T>&e=g[i];
        if(e.to!=par[x]){
          par[e.to]=x;
          assert(e.index<n-1);
          edge[e.index]=e;
          que.push(e.to);
          i++;
        }
        else{
          if(i+1==ptr[x+1])break;
          std::swap(g[i],g[ptr[x+1]-1]);
        }
      }
    }
    r=root;
  }
  std::vector<int>bfs_order()const{
    assert(is_directed());
    std::vector<int>bfs(n);
    int p=0,q=0;
    bfs[q++]=root();
    while(p<q){
      int x=bfs[p++];
      for(const Edge<T>&e:(*this)[x])bfs[q++]=e.to;
    }
    return bfs;
  }
  std::vector<int>dfs_order()const{
    assert(is_directed());
    std::vector<int>res;
    res.reserve(n);
    std::vector<int>st(n);
    int p=0;
    st[p++]=root();
    while(p){
      int x=st[--p];
      res.push_back(x);
      p+=(*this)[x].size();
      for(const Edge<T>&e:(*this)[x])st[--p]=e.to;
      p+=(*this)[x].size();
    }
    return res;
  }
  std::vector<int>rbfs_order()const{
    std::vector<int>bfs=bfs_order();
    std::reverse(bfs.begin(),bfs.end());
    return bfs;
  }
  void hld(){
    assert(is_directed());
    std::vector<int>sub(n);
    for(int x:rbfs_order()){
      sub[x]=1;
      int mx=-1;
      for(Edge<T>&e:(*this)[x]){
        sub[x]+=sub[e.to];
        if(mx<sub[e.to]){
          mx=sub[e.to];
          std::swap((*this)[x][0],e);
        }
      }
    }
  }
  std::pair<std::vector<int>,std::vector<int>>in_out_order(){
    assert(is_directed());
    std::vector<int>in(n),out(n);
    int p=0;
    auto dfs=[&](auto self,int x)->void {
      in[x]=p++;
      for(const Edge<T>&e:(*this)[x]){
        self(self,e.to);
      }
      out[x]=p;
    };
    dfs(dfs,root());
    return std::make_pair(in,out);
  }
  std::pair<T,std::vector<int>>diameter()const{
    assert(!is_directed());
    static constexpr T inf=std::numeric_limits<T>::max();
    std::vector<T>dst(n,inf);
    dst[0]=0;
    std::vector<int>que(n);
    int p=0,q=1;
    que[0]=0;
    while(p<q){
      int x=que[p++];
      for(const Edge<T>&e:(*this)[x])if(dst[e.to]==inf){
        dst[e.to]=dst[x]+e.weight;
        que[q++]=e.to;
      }
    }
    int u=std::max_element(dst.begin(),dst.end())-dst.begin();
    std::fill(dst.begin(),dst.end(),inf);
    dst[u]=0;
    p=0,q=1;
    que[0]=u;
    while(p<q){
      int x=que[p++];
      for(const Edge<T>&e:(*this)[x])if(dst[e.to]==inf){
        dst[e.to]=dst[x]+e.weight;
        que[q++]=e.to;
      }
    }
    int v=std::max_element(dst.begin(),dst.end())-dst.begin();
    T weight=dst[v];
    std::vector<int>res;
    while(u!=v){
      res.push_back(v);
      for(const Edge<T>&e:(*this)[v])if(dst[e.to]<dst[v]){
        v=e.to;
        break;
      }
    }
    res.push_back(u);
    return std::make_pair(weight,res);
  }
  int size()const{return n;}
  tree_range operator[](int i){return tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]-(r!=-1&&r!=i)};}
  const_tree_range operator[](int i)const{return const_tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]-(r!=-1&&r!=i)};}
  const Edge<T>& get_edge(int i)const{return edge[i];}
  inline int parent(int i)const{return i==r?-1:g[ptr[i+1]-1].to;}
  inline int root()const{return r;}
  typename std::vector<Edge<T>>::iterator begin(){return edge.begin();}
  typename std::vector<Edge<T>>::iterator end(){return edge.end();}
  typename std::vector<Edge<T>>::const_iterator begin()const{return edge.begin();}
  typename std::vector<Edge<T>>::const_iterator end()const{return edge.end();}
};
struct StaticTopTree{
private:
  template<typename T>
  void build(Tree<T>t){
    int n=t.size();
    left.reserve(n*2-1),right.reserve(n*2-1),par.reserve(n*2-1),A.reserve(n*2-1),B.reserve(n*2-1);
    left.resize(n,-1),right.resize(n,-1),par.resize(n,-1),A.resize(n),B.resize(n);
    for(int i=0;i<n;i++){
      A[i]=t.parent(i);
      B[i]=i;
    }
    auto dfs=[&](auto self,int x)->std::pair<int,int> {
      std::vector<std::pair<int,int>>vs{{0,x}};
      while(t[x].size()>=1){
        std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>>que;
        int heavy=t[x][0].to;
        que.emplace(0,heavy);
        for(int i=1;i<t[x].size();i++)que.push(self(self,t[x][i].to));
        while((int)que.size()>=2){
          auto [d1,v1]=que.top();que.pop();
          auto [d2,v2]=que.top();que.pop();
          if(B[v2]==heavy)std::swap(d1,d2),std::swap(v1,v2);
          int nv=left.size();
          left.push_back(v1),right.push_back(v2),par.push_back(-1),A.push_back(x),B.push_back(B[v1]);
          par[v1]=par[v2]=nv;
          que.emplace(std::max(d1,d2)+1,nv);
        }
        vs.push_back(que.top());
        while(true){
          int sz=vs.size();
          if(sz>=3&&(vs[sz-3].first==vs[sz-2].first||vs[sz-3].first<=vs[sz-1].first)){
            int nv=left.size();
            left.push_back(vs[sz-3].second),right.push_back(vs[sz-2].second),par.push_back(-1),A.push_back(A[vs[sz-3].second]),B.push_back(B[vs[sz-2].second]);
            par[vs[sz-3].second]=par[vs[sz-2].second]=nv;
            vs[sz-3].first=std::max(vs[sz-3].first,vs[sz-2].first)+1;
            vs[sz-3].second=nv;
            vs[sz-2]=vs[sz-1];
            vs.pop_back();
          }
          else if(sz>=2&&vs[sz-2].first<=vs[sz-1].first){
            int nv=left.size();
            left.push_back(vs[sz-2].second),right.push_back(vs[sz-1].second),par.push_back(-1),A.push_back(A[vs[sz-2].second]),B.push_back(B[vs[sz-1].second]);
            par[vs[sz-2].second]=par[vs[sz-1].second]=nv;
            vs[sz-2].first=std::max(vs[sz-2].first,vs[sz-1].first)+1;
            vs[sz-2].second=nv;
            vs.pop_back();
          }
          else break;
        }
        x=heavy;
      }
      while((int)vs.size()>=2){
        int sz=vs.size();
        int nv=left.size();
        left.push_back(vs[sz-2].second),right.push_back(vs[sz-1].second),par.push_back(-1),A.push_back(A[vs[sz-2].second]),B.push_back(B[vs[sz-1].second]);
        par[vs[sz-2].second]=par[vs[sz-1].second]=nv;
        vs[sz-2].first=std::max(vs[sz-2].first,vs[sz-1].first)+1;
        vs[sz-2].second=nv;
        vs.pop_back();
      }
      return vs[0];
    };
    dfs(dfs,t.root());
  }
public:
  std::vector<int>left,right,par,A,B;
  StaticTopTree(){}
  template<typename T>
  explicit StaticTopTree(Tree<T>t,int){
    build(std::move(t));
  }
  template<typename T>
  explicit StaticTopTree(Tree<T>t){
    assert(t.is_directed());
    t.hld();
    build(std::move(t));
  }
};
#include<functional>
struct ContourQuery{
private:
  std::vector<std::pair<int,int>>ptr;
  std::vector<std::vector<int>>dst;
  std::vector<int>stt_par,dep,lr;
  int vsc;
public:
  ContourQuery(){}
  template<typename T>
  ContourQuery(Tree<T>t){
    assert(!t.is_directed());
    int n=t.size();
    t.remove_parent();
    StaticTopTree stt(t);
    dep.resize(n*2-1);
    for(int i=n*2-1;i-->n;){
      dep[stt.left[i]]=dep[stt.right[i]]=dep[i]+1;
    }
    dst.resize(*std::max_element(dep.begin(),dep.begin()+n),std::vector<int>(n,-1));
    ptr.resize(n*2-1);
    std::vector<int>que(n+dst.size());
    int p=0,q=0;
    vsc=n;
    auto dfs=[&](auto self,int v)->std::vector<int> {
      if(v<n){
        return std::vector<int>{v};
      }
      const int d=dep[v];
      int lv=stt.left[v],rv=stt.right[v];
      std::vector<int>lch=self(self,lv);
      p=q=0;
      if(stt.A[lv]==stt.A[rv]){
        for(int x:lch){
          dst[d][x]=1;
          que[q++]=x;
        }
        while(p<q){
          int x=que[p++];
          if(x==stt.B[lv])continue;
          for(const Edge<T>&e:t[x]){
            dst[d][e.to]=dst[d][x]+1;
            que[q++]=e.to;
          }
        }
        int len=dst[d][que[q-1]]+1;
        ptr[lv]=std::make_pair(vsc,len);
        vsc+=len;
      }
      else{
        dst[d][stt.B[lv]]=0;
        bool boundaryA=false;
        if(t.parent(stt.B[lv])==stt.A[lv])que[q++]=~stt.B[lv],boundaryA=true;
        else{
          que[q++]=t.parent(stt.B[lv]);
          dst[d][stt.B[lv]]=0,dst[d][que[0]]=1;
        }
        while(p<q){
          int x=que[p++];
          if(x<0){
            x=~x;
            for(int y:lch)if(x!=y){
              dst[d][y]=dst[d][x]+2;
              que[q++]=y;
            }
          }
          else{
            for(const Edge<T>&e:t[x])if(dst[d][e.to]==-1){
              dst[d][e.to]=dst[d][x]+1;
              que[q++]=e.to;
            }
            int par=t.parent(x);
            if(par==stt.A[lv]){
              if(!boundaryA){
                que[q++]=~x;
                boundaryA=true;
              }
            }
            else if(dst[d][par]==-1){
              dst[d][par]=dst[d][x]+1;
              que[q++]=par;
            }
          }
        }
        int len;
        if(que[q-1]<0)len=q>=2?dst[d][que[q-2]]+1:1;
        else len=dst[d][que[q-1]]+1;
        ptr[lv]=std::make_pair(vsc,len);
        vsc+=len;
      }
      std::vector<int>rch=self(self,rv);
      p=q=0;
      for(int x:rch){
        dst[d][x]=1;
        que[q++]=x;
      }
      while(p<q){
        int x=que[p++];
        if(x==stt.B[rv])continue;
        for(const Edge<T>&e:t[x]){
          dst[d][e.to]=dst[d][x]+1;
          que[q++]=e.to;
        }
      }
      ptr[rv]=std::make_pair(vsc,dst[d][que[q-1]]+1);
      vsc+=dst[d][que[q-1]]+1;
      if(stt.A[lv]==stt.A[rv]){
        if(std::ssize(lch)<std::ssize(rch))std::swap(lch,rch);
        lch.insert(lch.end(),rch.begin(),rch.end());
      }
      return std::move(lch);
    };
    dfs(dfs,n*2-2);
    stt_par=std::move(stt.par);
    lr.resize(n-1);
    for(int i=0;i<n-1;i++)lr[i]=stt.left[i+n]^stt.right[i+n];
  }
  template<typename Func>
  void get_vs(int v,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int)>>);
    f(v);
    int d=dep[v]-1;
    int x=v;
    while(d>=0){
      f(ptr[v].first+dst[d][x]);
      v=stt_par[v];
      d--;
    }
  }
  template<typename Func>
  void get_range(int v,int l,int r,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int,int)>>);
    if(l>=r)return;
    if(l<=0&&1<=r)f(v,v+1);
    int x=v;
    while(true){
      int par=stt_par[v];
      if(par==-1)break;
      int another=lr[par-std::ssize(lr)-1]^v;
      int d=dep[par];
      int rtov=dst[d][x];
      int nl=std::clamp(l-rtov,0,ptr[another].second);
      int nr=std::clamp(r-rtov,0,ptr[another].second);
      if(nl!=nr)f(ptr[another].first+nl,ptr[another].first+nr);
      v=par;
    }
  }
  inline int size()const{return vsc;}
};
#include<concepts>
template<typename T>
constexpr std::enable_if_t<std::numeric_limits<T>::digits<=32,int>msb(T n){return n==0?-1:31-__builtin_clz(n);}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),int>msb(T n){return n==0?-1:63-__builtin_clzll(n);}

template<typename T>
constexpr std::enable_if_t<std::numeric_limits<T>::digits<=32,int>lsb(T n){return n==0?-1:__builtin_ctz(n);}
template<typename T>
constexpr std::enable_if_t<(std::numeric_limits<T>::digits>32),int>lsb(T n){return n==0?-1:__builtin_ctzll(n);}

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>,T>floor_pow2(T n){return n==0?0:T(1)<<msb(n);}

template<typename T>
constexpr std::enable_if_t<std::is_integral_v<T>,T>ceil_pow2(T n){return n<=1?1:T(1)<<(msb(n-1)+1);}

template<std::integral T>
constexpr T safe_div(T a,T b){return a/b-(a%b&&(a^b)<0);}
template<std::integral T>
constexpr T safe_ceil(T a,T b){return a/b+(a%b&&(a^b)>0);}
template<typename M>
struct BinaryIndexedTree{
private:
  using S=typename M::S;
  std::vector<S>dat;
  int z;
public:
  BinaryIndexedTree(){}
  explicit BinaryIndexedTree(int n):z(ceil_pow2(n)),dat(n,M::e()){}
  explicit BinaryIndexedTree(const std::vector<S>&init):z(ceil_pow2((int)init.size())),dat(init){
    for(int i=0;i<(int)dat.size();i++){
      int j=i+((i+1)&-(i+1));
      if(j<(int)dat.size())dat[j]=M::op(dat[i],dat[j]);
    }
  }
  inline void add(int i,S x){
    while(i<(int)dat.size()){
      dat[i]=M::op(dat[i],x);
      i+=(i+1)&-(i+1);
    }
  }
  inline S sum(int i)const{
    S res=M::e();
    while(i>0){
      res+=dat[i-1];
      i-=i&-i;
    }
    return res;
  }
  inline S sum(int l,int r)const{
    S lp=M::e(),rp=M::e();
    while(l<r){
      rp=M::op(dat[r-1],rp);
      r-=r&-r;
    }
    while(r<l){
      lp=M::op(dat[l-1],lp);
      l-=l&-l;
    }
    return M::op(M::inverse(lp),rp);
  }
  int lower_bound(S k)const{
    int res=0;
    for(int i=z;i>=1;i>>=1){
      if(res+i<=(int)dat.size()&&dat[res+i-1]<k){
        k-=dat[res+i-1];
        res+=i;
      }
    }
    return res;
  }
  friend std::ostream &operator<<(std::ostream &os,const BinaryIndexedTree&bit){
    os<<"{";
    for(int i=0;i<(int)bit.dat.size();i++)os<<bit.sum(i,i+1)<<",}"[i+1==(int)bit.dat.size()];
    if(bit.dat.empty())os<<"}";
    return os;
  }
};
template<typename T=int>
struct MonoidAdd{
  using S=T;
  using F=std::nullptr_t;
  static inline S op(S x,S y){return x+y;}
  static inline S e(){return T();}
  static inline S mapping(F,const S&x,long long){return x;}
  static inline F composition(F,F){return nullptr;}
  static inline F id(){return nullptr;}
  static inline S inverse(S x){return -x;}
  static inline void revS(S&x){}
  static inline S pow(S x,long long p){return x*p;}
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int&x:a)rd(x);
  Tree t(n);
  for(int i=0;i<n-1;i++){
    int u,v;
    rd(u),rd(v);
    t.add_edge(u,v);
  }
  t.build();
  ContourQuery cq(t);
  std::vector<long long>init(cq.size());
  for(int i=0;i<n;i++)cq.get_vs(i,[&](int j){init[j]+=a[i];});
  BinaryIndexedTree<MonoidAdd<long long>>BIT(init);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int p,x;
      rd(p),rd(x);
      cq.get_vs(p,[&](int j){BIT.add(j,x);});
    }
    else{
      int p,l,r;
      rd(p),rd(l),rd(r);
      long long ans=0;
      cq.get_range(p,l,r,[&](int l,int r){ans+=BIT.sum(l,r);});
      wt(ans),wt('\n');
    }
  }
}
