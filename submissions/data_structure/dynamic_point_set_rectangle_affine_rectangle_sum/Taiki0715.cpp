#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum"
#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdlib>
#include<cstdint>
#include<type_traits>
namespace fastio{
static constexpr uint32_t buf_size=1<<17;
char ibuf[buf_size];
char obuf[buf_size];
uint32_t pil=0,pir=0,por=0;
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
    uint32_t r1=x%100000000;
    uint64_t q1=x/100000000;
    uint32_t r2=q1%100000000;
    uint32_t q2=q1/100000000;
    uint32_t n1=r1%10000,n2=r1/10000,n3=r2%10000,n4=r2/10000;
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
      uint32_t q3=(q2*205)>>11;
      uint32_t r3=q2-q3*10;
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
#include<vector>
#include<array>
#include<limits>
#include<tuple>
#include<cassert>
#include<algorithm>
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
template<typename I,typename M>
struct kdTree{
private:
  using S=typename M::M1::S;
  using F=typename M::M2::S;
  struct Rectangle{
    I lx,rx,ly,ry;
    Rectangle(){}
    Rectangle(I x,I y):lx(x),rx(x+1),ly(y),ry(y+1){}
    Rectangle(I lx,I rx,I ly,I ry):lx(lx),rx(rx),ly(ly),ry(ry){}
    template<int mask>
    constexpr bool contains(const Rectangle&r)const{
      if constexpr(mask==0)return r.lx<=lx&&rx<=r.rx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==1)return rx<=r.rx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==2)return r.lx<=lx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==3)return r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==4)return r.lx<=lx&&rx<=r.rx&&ry<=r.ry;
      else if constexpr(mask==5)return rx<=r.rx&&ry<=r.ry;
      else if constexpr(mask==6)return r.lx<=lx&&ry<=r.ry;
      else if constexpr(mask==7)return ry<=r.ry;
      else if constexpr(mask==8)return r.lx<=lx&&rx<=r.rx&&r.ly<=ly;
      else if constexpr(mask==9)return rx<=r.rx&&r.ly<=ly;
      else if constexpr(mask==10)return r.lx<=lx&&r.ly<=ly;
      else if constexpr(mask==11)return r.ly<=ly;
      else if constexpr(mask==12)return r.lx<=lx&&rx<=r.rx;
      else if constexpr(mask==13)return rx<=r.rx;
      else if constexpr(mask==14)return r.lx<=lx;
    }
    template<int mask>
    constexpr bool disjoint(const Rectangle&r)const{;
      if constexpr(mask==0)return rx<=r.lx||r.rx<=lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==1)return r.rx<=lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==2)return rx<=r.lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==3)return ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==4)return rx<=r.lx||r.rx<=lx||r.ry<=ly;
      else if constexpr(mask==5)return r.rx<=lx||r.ry<=ly;
      else if constexpr(mask==6)return rx<=r.lx||r.ry<=ly;
      else if constexpr(mask==7)return r.ry<=ly;
      else if constexpr(mask==8)return rx<=r.lx||r.rx<=lx||ry<=r.ly;
      else if constexpr(mask==9)return r.rx<=lx||ry<=r.ly;
      else if constexpr(mask==10)return rx<=r.lx||ry<=r.ly;
      else if constexpr(mask==11)return ry<=r.ly;
      else if constexpr(mask==12)return rx<=r.lx||r.rx<=lx;
      else if constexpr(mask==13)return r.rx<=lx;
      else if constexpr(mask==14)return rx<=r.lx;
    }
    template<int mask>
    constexpr bool contains(const std::pair<I,I>&xy)const{
      if constexpr(mask==0)return lx<=xy.first&&xy.first<rx&&ly<=xy.second&&xy.second<ry;
      else if constexpr(mask==1)return xy.first<rx&&ly<=xy.second&&xy.second<ry;
      else if constexpr(mask==2)return lx<=xy.first&&ly<=xy.second&&xy.second<ry;
      else if constexpr(mask==3)return ly<=xy.second&&xy.second<ry;
      else if constexpr(mask==4)return lx<=xy.first&&xy.first<rx&&xy.second<ry;
      else if constexpr(mask==5)return xy.first<rx&&xy.second<ry;
      else if constexpr(mask==6)return lx<=xy.first&&xy.second<ry;
      else if constexpr(mask==7)return xy.second<ry;
      else if constexpr(mask==8)return lx<=xy.first&&xy.first<rx&&ly<=xy.second;
      else if constexpr(mask==9)return xy.first<rx&&ly<=xy.second;
      else if constexpr(mask==10)return lx<=xy.first&&ly<=xy.second;
      else if constexpr(mask==11)return ly<=xy.second;
      else if constexpr(mask==12)return lx<=xy.first&&xy.first<rx;
      else if constexpr(mask==13)return xy.first<rx;
      else if constexpr(mask==14)return lx<=xy.first;
    }
  };
  struct node{
    Rectangle rect;
    I split;
    S val;
    F lazy;
    bool flag;
    node():rect(),split(),val(M::M1::e()),lazy(M::M2::e()),flag(false){}
  };
  struct Block{
    std::array<std::pair<I,I>,8>xy;
    std::array<S,8>dat;
    inline void propagate(const F&f){
      for(int i=0;i<8;i++)dat[i]=M::act(dat[i],f);
    }
    template<int mask>
    inline void apply(const Rectangle&r,const F&f){
      for(int i=0;i<8;i++)if(r.template contains<mask>(xy[i]))dat[i]=M::act(dat[i],f);
    }
    template<int mask>
    inline S prod(const Rectangle&r){
      S res=M::M1::e();
      for(int i=0;i<8;i++)if(r.template contains<mask>(xy[i]))res=M::M1::op(res,dat[i]);
      return res;
    }
    Block():xy{},dat{M::M1::e()}{}
  };
  int n,z,log2n;
  std::vector<std::pair<I,int>>xz,yz;
  std::vector<node>dat;
  std::vector<std::pair<int,int>>pos;
  std::vector<Block>block;
  inline void propagate(int i,F f){
    dat[i].val=M::act(dat[i].val,f);
    dat[i].lazy=M::M2::op(dat[i].lazy,f);
    dat[i].flag=true;
  }
  inline void push(int i){
    if(dat[i].flag){
      propagate(i*2,dat[i].lazy);
      propagate(i*2+1,dat[i].lazy);
      dat[i].lazy=M::M2::e();
      dat[i].flag=false;
    }
  }
  inline void update(int i){dat[i].val=M::M1::op(dat[i*2].val,dat[i*2+1].val);}
  template<bool splitx,int mask>
  S prod_rec(int id,const Rectangle&r){
    if constexpr(mask==15)return dat[id].val;
    else{
      if(dat[id].rect.template contains<mask>(r))return dat[id].val;
      if(dat[id].rect.template disjoint<mask>(r))return M::M1::e();
      if(id>=z){
        S res=block[id-z].template prod<mask>(r);
        if(dat[id].flag)res=M::act(res,dat[id].lazy);
        return res;
      }
      else{
        push(id);
        if constexpr(splitx){
          if(r.rx<=dat[id].split)return prod_rec<0,mask>(id*2,r);
          else if(dat[id].split<=r.lx)return prod_rec<0,mask>(id*2+1,r);
          else return M::M1::op(prod_rec<0,mask|2>(id*2,r),prod_rec<0,mask|1>(id*2+1,r));
        }
        else{
          if(r.ry<=dat[id].split)return prod_rec<1,mask>(id*2,r);
          else if(dat[id].split<=r.ly)return prod_rec<1,mask>(id*2+1,r);
          else return M::M1::op(prod_rec<1,mask|8>(id*2,r),prod_rec<1,mask|4>(id*2+1,r));
        }
      }
    }
  }
  template<bool splitx,int mask>
  void apply_rec(int id,const Rectangle&r,F f){
    if constexpr(mask==15)return propagate(id,f);
    else{
      if(dat[id].rect.template contains<mask>(r))return propagate(id,f);
      if(dat[id].rect.template disjoint<mask>(r))return;
      if(id>=z){
        if(dat[id].flag){
          block[id-z].propagate(dat[id].lazy);
          dat[id].lazy=M::M2::e();
          dat[id].flag=false;
        }
        block[id-z].template apply<mask>(r,f);
        dat[id].val=M::M1::e();
        for(int i=0;i<8;i++)dat[id].val=M::M1::op(dat[id].val,block[id-z].dat[i]);
      }
      else{
        push(id);
        if constexpr(splitx){
          if(r.rx<=dat[id].split)apply_rec<0,mask>(id*2,r,f);
          else if(dat[id].split<=r.lx)apply_rec<0,mask>(id*2+1,r,f);
          else apply_rec<0,mask|2>(id*2,r,f),apply_rec<0,mask|1>(id*2+1,r,f);
        }
        else{
          if(r.ry<=dat[id].split)apply_rec<1,mask>(id*2,r,f);
          else if(dat[id].split<=r.ly)apply_rec<1,mask>(id*2+1,r,f);
          else apply_rec<1,mask|8>(id*2,r,f),apply_rec<1,mask|4>(id*2+1,r,f);
        }
        update(id);
      }
    }
  }
  inline int getx(I x)const{return std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,0))-xz.begin();}
  inline int gety(I y)const{return std::lower_bound(yz.begin(),yz.end(),std::make_pair(y,0))-yz.begin();}
public:
  kdTree(){}
  kdTree(std::vector<std::tuple<I,I,S>>init):n(init.size()),z(ceil_pow2((n+7)/8)),log2n(msb(z)),xz(n),yz(n),dat(z*2),pos(n),block(z*8){
    for(int i=0;i<n;i++){
      const auto&[x,y,v]=init[i];
      xz[i]=std::make_pair(x,i);
      yz[i]=std::make_pair(y,i);
    }
    std::sort(xz.begin(),xz.end()),std::sort(yz.begin(),yz.end());
    std::vector<std::tuple<int,int,int>>idx(n);
    for(int i=0;i<n;i++){
      auto&[x,y,v]=init[i];
      x=std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,i))-xz.begin();
      y=std::lower_bound(yz.begin(),yz.end(),std::make_pair(y,i))-yz.begin();
      idx[i]=std::make_tuple(i,x,y);
    }
    std::vector<std::pair<int,int>>lr(z*2);
    lr[1]=std::make_pair(0,n);
    for(int i=1;i<z;i++){
      auto [l,r]=lr[i];
      if(msb(i)%2==0){
        int m=(l+r)/2;
        std::nth_element(idx.begin()+l,idx.begin()+m,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<1>(lhs)<std::get<1>(rhs);});
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
        dat[i].split=std::get<1>(idx[m]);
      }
      else{
        int m=(l+r)/2;
        std::nth_element(idx.begin()+l,idx.begin()+m,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<2>(lhs)<std::get<2>(rhs);});
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
        dat[i].split=std::get<2>(idx[m]);
      }
    }
    for(int i=z;i<z*2;i++){
      if(lr[i].first!=lr[i].second){
        int len=lr[i].second-lr[i].first;
        auto&[lx,rx,ly,ry]=dat[i].rect;
        lx=ly=std::numeric_limits<I>::max();
        rx=ry=std::numeric_limits<I>::min();
        for(int j=0;j<len;j++){
          auto [id,x,y]=idx[lr[i].first+j];
          block[i-z].xy[j]=std::make_pair(x,y);
          block[i-z].dat[j]=std::get<2>(init[id]);
          dat[i].val=M::M1::op(dat[i].val,std::get<2>(init[id]));
          pos[id]=std::make_pair(i-z,j);
          if(lx>x)lx=x;
          if(rx<x)rx=x;
          if(ly>y)ly=y;
          if(ry<y)ry=y;
        }
        rx++,ry++;
      }
    }
    for(int i=z;--i;){
      update(i);
      if(lr[i].first+1==lr[i].second){
        dat[i].rect=dat[i*2+1].rect;
        continue;
      }
      const auto&[lx1,rx1,ly1,ry1]=dat[i*2].rect;
      const auto&[lx2,rx2,ly2,ry2]=dat[i*2+1].rect;
      dat[i].rect=Rectangle(std::min(lx1,lx2),std::max(rx1,rx2),std::min(ly1,ly2),std::max(ry1,ry2));
    }
  }
  void set(int i,S v){
    auto [p,j]=pos[i];
    for(int j=log2n;j>=1;j--)push((p+z)>>j);
    block[p].propagate(dat[p+z].lazy);
    dat[p+z].lazy=M::M2::e();
    dat[p+z].flag=false;
    block[p].dat[j]=v;
    dat[p+z].val=M::M1::e();
    for(int i=0;i<8;i++)dat[p+z].val=M::M1::op(dat[p+z].val,block[p].dat[i]);
    p+=z;
    while(p>>=1)update(p);
  }
  S prod(I lx,I rx,I ly,I ry){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx==rx||ly==ry)return M::M1::e();
    return prod_rec<1,0>(1,Rectangle(lx,rx,ly,ry));
  }
  void apply(I lx,I rx,I ly,I ry,F f){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx<rx&&ly<ry)apply_rec<1,0>(1,Rectangle(lx,rx,ly,ry),f);
  }
  S all_prod()const{return dat[1];}
};
struct Query{
  int t;
  int lx,rx,ly,ry;
  int a,b;
  Query(){}
};
constexpr long long mod=998244353;
struct Monoid{
  struct M1{
    using S=std::pair<int,int>;
    static S op(S x,S y){
      int s=x.first+y.first;
      if(s>=mod)s-=mod;
      return {s,x.second+y.second};
    }
    static S e(){return {0,0};}
  };
  struct M2{
    using S=std::pair<int,int>;
    static S op(S x,S y){return {(long long)x.first*y.first%mod,((long long)x.second*y.first+y.second)%mod};}
    static S e(){return {1,0};}
  };
  static M1::S act(M1::S x,M2::S f){return {((long long)x.first*f.first+(long long)x.second*f.second)%mod,x.second};}
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<std::tuple<int,int,std::pair<int,int>>>init(n);
  std::vector<Query>query(q);
  for(int i=0;i<n;i++){
    int x,y,z;
    rd(x),rd(y),rd(z);
    init[i]={x,y,std::make_pair(z,1)};
  }
  for(int i=0;i<q;i++){
    int t;
    rd(t);
    if(t==0){
      int x,y,w;
      rd(x),rd(y),rd(w);
      init.emplace_back(x,y,std::make_pair(0,0));
      query[i].t=0;
      query[i].a=w;
    }
    else if(t==1){
      int x,w;
      rd(x),rd(w);
      query[i].t=1;
      query[i].lx=x;
      query[i].a=w;
    }
    else if(t==2){
      int lx,rx,ly,ry;
      rd(lx),rd(ly),rd(rx),rd(ry);
      query[i].t=2;
      query[i].lx=lx,query[i].rx=rx,query[i].ly=ly,query[i].ry=ry;
    }
    else{
      int lx,rx,ly,ry,a,b;
      rd(lx),rd(ly),rd(rx),rd(ry),rd(a),rd(b);
      query[i].t=3;
      query[i].lx=lx,query[i].rx=rx,query[i].ly=ly,query[i].ry=ry;
      query[i].a=a,query[i].b=b;
    }
  }
  kdTree<int,Monoid>seg(init);
  int p=n;
  for(const Query&a:query){
    if(a.t==0){
      seg.set(p++,std::make_pair(a.a,1));
    }
    else if(a.t==1){
      seg.set(a.lx,std::make_pair(a.a,1));
    }
    else if(a.t==2){
      wt(seg.prod(a.lx,a.rx,a.ly,a.ry).first);
      wt('\n');
    }
    else{
      seg.apply(a.lx,a.rx,a.ly,a.ry,std::make_pair(a.a,a.b));
    }
  }
}
