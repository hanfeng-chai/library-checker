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
private:
  int n;
  std::vector<int>par;
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
public:
  explicit Tree(int n_):n(n_){
    edge.reserve(n-1);
  }
  Tree():n(0){}
  Tree(int n_,const std::vector<Edge<T>>&e,bool dir=false):n(n_),edge(e){
    if(!dir)build();
    else{
      par.resize(n,-1);
      ptr.resize(n+1);
      for(const Edge<T>&i:edge)ptr[i.from]++,par[i.to]=i.from;
      for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
      assert(ptr[n]==n-1);
      g.resize(n-1);
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
    par.resize(n);
    par[0]=-1;
    ptr.resize(n+1);
    for(int i=1;i<n;i++){
      int p;
      std::cin>>p;
      if constexpr(index)p--;
      edge.emplace_back(p,i,1,i-1);
      par[i]=p;
      ptr[p]++;
    }
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    g.resize(n-1);
    for(int i=0;i<n-1;i++){
      g[--ptr[edge[i].from]]=edge[i];
    }
  }
  void add_edge(int u,int v){edge.emplace_back(u,v,1,edge.size());}
  void add_edge(int u,int v,T w){edge.emplace_back(u,v,w,edge.size());}
  void add_edge(int u,int v,T w,int idx){edge.emplace_back(u,v,w,idx);}
  inline bool is_directed()const{return !par.empty();}
  void build(){
    ptr.resize(n+1,0);
    for(auto&&[u,v,w,i]:edge)ptr[u]++,ptr[v]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    assert(ptr[n]==n*2-2);
    g.resize(n*2-2);
    par.clear();
    for(auto&&[u,v,w,i]:edge){
      g[--ptr[u]]=Edge(u,v,w,i);
      g[--ptr[v]]=Edge(v,u,w,i);
    }
  }
  void remove_parent(int root=0){
    edge.resize(n-1);
    par.resize(n);
    par[root]=-1;
    std::queue<int>que;
    que.push(root);
    while(!que.empty()){
      int x=que.front();
      que.pop();
      for(const Edge<T>&e:(*this)[x])if(e.to!=par[x]){
        par[e.to]=x;
        edge[e.index]=e;
        que.push(e.to);
      }
    }
    ptr.resize(n+1);
    std::fill(ptr.begin(),ptr.end(),0);
    for(int i=0;i<n-1;i++)ptr[edge[i].from]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    g.resize(ptr[n]);
    for(const Edge<T>&e:edge)g[--ptr[e.from]]=e;
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
  tree_range operator[](int i){return tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]};}
  const_tree_range operator[](int i)const{return const_tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]};}
  const Edge<T>& get_edge(int i)const{return edge[i];}
  inline int parent(int i)const{return par[i];}
  inline int root()const{return std::find(par.begin(),par.end(),-1)-par.begin();}
  typename std::vector<Edge<T>>::iterator begin(){return edge.begin();}
  typename std::vector<Edge<T>>::iterator end(){return edge.end();}
  typename std::vector<Edge<T>>::const_iterator begin()const{return edge.begin();}
  typename std::vector<Edge<T>>::const_iterator end()const{return edge.end();}
};
struct StaticTopTree{
  std::vector<int>left,right,par,A,B;
  StaticTopTree(){}
  template<typename T>
  explicit StaticTopTree(Tree<T>t){
    assert(t.is_directed());
    t.hld();
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
};
template<typename DP>
struct DynamicTreeDP{
private:
  using S=typename DP::S;
  std::vector<S>data;
  StaticTopTree stt;
  inline void update(int x){
    int l=stt.left[x],r=stt.right[x];
    if(stt.A[l]==stt.A[r])data[x]=DP::rake(data[l],data[r]);
    else data[x]=DP::compress(data[l],data[r]);
  }
public:
  DynamicTreeDP(){}
  template<typename T>
  explicit DynamicTreeDP(Tree<T>t,std::vector<S>init){
    assert(t.size()==(int)init.size());
    int n=t.size();
    stt=StaticTopTree(std::move(t));
    data=std::move(init);
    data.resize(n*2-1);
    for(int i=n;i<n*2-1;i++)update(i);
  }
  void set(int x,S v){
    data[x]=std::move(v);
    for(x=stt.par[x];x!=-1;x=stt.par[x])update(x);
  }
  inline S get()const{return data.back();}
};
#include<optional>
#include<numeric>
constexpr int carmichael_constexpr(int n){
  if(n==998244353)return 998244352;
  if(n==1000000007)return 1000000006;
  if(n<=1)return n;
  int res=1;
  int t=0;
  while(n%2==0){
    n/=2;
    t++;
  }
  if(t==2)res=2;
  else if(t>=3)res=1<<(t-2);
  for(int i=3;i*i<=n;i++)if(n%i==0){
    int c=0;
    while(n%i==0){
      n/=i;
      c++;
    }
    int prod=i-1;
    for(int j=0;j<c-1;j++)prod*=i;
    res=std::lcm(res,prod);
  }
  if(n!=1)res=std::lcm(res,n-1);
  return res;
}
template<int m>
struct mod_int{
private:
  static constexpr unsigned int umod=static_cast<unsigned int>(m);
  static constexpr unsigned int car=carmichael_constexpr(m);
  using uint=unsigned int;
  using mint=mod_int;
  uint v;
  static_assert(m<uint(1)<<31);
  mint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if constexpr(m%8==1){
      mint b=2;
      while(b.pow((m-1)/2).val()==1)b++;
      int m2=m-1,e=0;
      while(m2%2==0)m2>>=1,e++;
      mint x=this->pow((m2-1)/2);
      mint y=(*this)*x*x;
      x*=*this;
      mint z=b.pow(m2);
      while(y.val()!=1){
        int j=0;
        mint t=y;
        while(t.val()!=1)t*=t,j++;
        z=z.pow(1<<(e-j-1));
        x*=z;
        z*=z;
        y*=z;e=j;
      }
      return x;
    }
    else if constexpr(m%8==5){
      mint ret=this->pow((m+3)/8);
      if((ret*ret).val()==this->val())return ret;
      else return ret*mint::raw(2).pow((m-1)/4);
    }
    else{
      return this->pow((m+1)/4);
    }
  }
public:
  using value_type=uint;
  mod_int():v(0){}
  template<typename T,std::enable_if_t<std::is_signed_v<T>,std::nullptr_t> =nullptr>
  mod_int(T a){
    a%=m;
    if(a<0)v=a+umod;
    else v=a;
  }
  template<typename T,std::enable_if_t<std::is_unsigned_v<T>,std::nullptr_t> =nullptr>
  mod_int(T a):v(a%umod){}
  static constexpr mint raw(int a){
    mint ret;
    ret.v=a;
    return ret;
  }
  inline uint val()const{return this->v;}
  static constexpr int mod(){return m;}
  inline mint &operator+=(const mint &b){
    this->v+=b.v;
    if(this->v>=umod)this->v-=umod;
    return *this;
  }
  inline mint &operator-=(const mint &b){
    this->v-=b.v;
    if(this->v>=umod)this->v+=umod;
    return *this;
  }
  inline mint &operator*=(const mint &b){
    this->v=((unsigned long long)this->v*b.v)%umod;
    return *this;
  }
  inline mint &operator/=(const mint &b){
    *this*=b.inv();
    return *this;
  }
  inline mint operator+()const{return *this;}
  inline mint operator-()const{return mint()-*this;}
  friend inline mint operator+(const mint &a,const mint &b){return mint(a)+=b;}
  friend inline mint operator-(const mint &a,const mint &b){return mint(a)-=b;}
  friend inline mint operator*(const mint &a,const mint &b){return mint(a)*=b;}
  friend inline mint operator/(const mint &a,const mint &b){return mint(a)/=b;}
  friend inline bool operator==(const mint &a,const mint &b){return a.val()==b.val();}
  friend inline bool operator!=(const mint &a,const mint &b){return !(a==b);}
  inline mint operator++(int){
    mint ret=*this;
    *this+=mint::raw(1);
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=mint::raw(1);
    return ret;
  }
  mint pow(long long n)const{
    mint ret=mint::raw(1),a(*this);
    while(n){
      if(n&1)ret*=a;
      a*=a;
      n>>=1;
    }
    return ret;
  }
  inline mint inv()const{
    assert(this->v!=0);
    return pow(car-1);
  }
  std::optional<mint>sqrt()const{
    if(this->val()<=1||this->pow((m-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  static constexpr unsigned int order(){return car;}
  friend std::istream &operator>>(std::istream &is,mint &b){
    long long a;
    is>>a;
    b=mint(a);
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const mint &b){
    os<<b.val();
    return os;
  }
};
template<int m>
struct std::hash<mod_int<m>>{
  std::size_t operator()(mod_int<m>x)const{
    return std::hash<unsigned int>()(x.val());
  }
};
using mint998=mod_int<998244353>;
using mint107=mod_int<1000000007>;
using mint=mint998;
struct DP{
  struct S{
    mint a,b;
    mint sum,sz;
  };
  static S rake(S a,S b){
    S res;
    res.a=a.a,res.b=a.b;
    res.sum=a.sum+b.sum;
    res.sz=a.sz+b.sz;
    return res;
  }
  static S compress(S a,S b){
    S res;
    res.a=a.a*b.a;
    res.b=a.b+b.b*a.a;
    res.sum=a.sum+b.sum*a.a+a.b*b.sz;
    res.sz=a.sz+b.sz;
    return res;
  }
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  for(int i=0;i<n;i++)rd(a[i]);
  Tree<std::pair<int,int>>t(n);
  for(int i=0;i<n-1;i++){
    int u,v,b,c;
    rd(u),rd(v),rd(b),rd(c);
    t.add_edge(u,v,{b,c});
  }
  t.build();
  t.remove_parent();
  std::vector<typename DP::S>init(n);
  std::vector<int>pos(n-1);
  init[0]={mint::raw(1),mint::raw(0),mint::raw(a[0]),mint::raw(1)};
  for(int i=0;i<n;i++){
    for(const auto&e:t[i]){
      pos[e.index]=e.to;
      init[e.to].a=mint::raw(e.weight.first);
      init[e.to].b=mint::raw(e.weight.second);
      init[e.to].sum=init[e.to].a*mint::raw(a[e.to])+init[e.to].b;
      init[e.to].sz=mint::raw(1);
    }
  }
  DynamicTreeDP<DP>dp(t,init);
  while(q--){
    int op;
    rd(op);
    if(op==0){
      int w,x;
      rd(w),rd(x);
      a[w]=x;
      init[w].sum=mint::raw(a[w])*init[w].a+init[w].b;
      dp.set(w,init[w]);
    }
    else{
      int e,y,z;
      rd(e),rd(y),rd(z);
      e=pos[e];
      init[e].a=mint::raw(y);
      init[e].b=mint::raw(z);
      init[e].sum=init[e].a*a[e]+init[e].b;
      dp.set(e,init[e]);
    }
    wt(dp.get().sum.val()),wt('\n');
  }
}
