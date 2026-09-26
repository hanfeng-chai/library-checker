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
struct Dummy{
  Dummy(){std::atexit(flush);}
}dummy;
}
using fastio::rd;
using fastio::wt;
#include<vector>
template<typename T>
struct SubtreeSum{
private:
  struct node{
    node *left,*right,*par;
    T sum;
    bool rev;
    node():left(nullptr),right(nullptr),par(nullptr),sum(T()),rev(false){}
    node(node*nil):left(nil),right(nil),par(nil),sum(T()),rev(false){}
    inline void reverse(){
      std::swap(left,right);
      rev^=1;
    }
    inline void push(){
      if(rev){
        left->reverse();
        right->reverse();
        rev=false;
      }
    }
  };
  void splay(node *nd){
    while(nd->par!=&nil){
      node *p=nd->par;
      node *pp=p->par;
      if(pp!=&nil)pp->push();
      p->push();
      nd->push();
      if(p->left==nd){
        if(pp->left==p){
          pp->sum-=p->sum-p->right->sum;
          p->sum-=nd->sum-nd->right->sum+p->right->sum-pp->sum;
          nd->sum-=nd->right->sum-p->sum;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->left=p->right;
          pp->left->par=pp;
          pp->par=p;
          p->right=pp;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->right=p;
        }
        else if(pp->right==p){
          T sub=pp->sum;
          pp->sum-=p->sum-nd->left->sum;
          p->sum-=nd->sum-nd->right->sum;
          nd->sum=sub;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->right=nd->left;
          pp->right->par=pp;
          pp->par=nd;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->left=pp;
          nd->right=p;
        }
        else{
          p->sum-=nd->sum-nd->right->sum;
          nd->sum-=nd->right->sum-p->sum;
          nd->par=p->par;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->right=p;
        }
      }
      else if(p->right==nd){
        if(pp->left==p){
          T sub=pp->sum;
          pp->sum-=p->sum-nd->right->sum;
          p->sum-=nd->sum-nd->left->sum;
          nd->sum=sub;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->left=nd->right;
          pp->left->par=pp;
          pp->par=nd;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
          nd->right=pp;
        }
        else if(pp->right==p){
          pp->sum-=p->sum-p->left->sum;
          p->sum-=nd->sum-nd->left->sum+p->left->sum-pp->sum;
          nd->sum-=nd->left->sum-p->sum;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->right=p->left;
          pp->right->par=pp;
          pp->par=p;
          p->left=pp;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
        }
        else{
          p->sum-=nd->sum-nd->left->sum;
          nd->sum-=nd->left->sum-p->sum;
          nd->par=p->par;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
        }
      }
      else break;
    }
    nd->push();
  }
  void expose(node *nd){
    while(true){
      splay(nd);
      nd->right=&nil;
      if(nd->par==&nil)break;
      splay(nd->par);
      nd->par->right=nd;
    }
  }
  node nil;
  std::vector<node>nds;
public:
  SubtreeSum(){}
  explicit SubtreeSum(int n):nil(),nds(n,node(&nil)){}
  void link(int u,int v){
    expose(&nds[u]);
    expose(&nds[v]);
    nds[v].reverse();
    nds[v].par=&nds[u];
    nds[u].sum+=nds[v].sum;
  }
  void cut(int u,int v){
    expose(&nds[u]);
    nds[u].reverse();
    expose(&nds[v]);
    nds[v].sum-=nds[v].left->sum;
    nds[v].left->par=&nil;
    nds[v].left=&nil;
  }
  void add(int u,T x){
    expose(&nds[u]);
    nds[u].sum+=x;
  }
  T sum(int u){
    expose(&nds[u]);
    return nds[u].sum;
  }
};
int main(){
  int n,q;
  rd(n),rd(q);
  SubtreeSum<long long>lct(n);
  for(int i=0;i<n;i++){
    int a;
    rd(a);
    lct.add(i,a);
  }
  for(int i=0;i<n-1;i++){
    int u,v;
    rd(u),rd(v);
    lct.link(u,v);
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int u,v,w,x;
      rd(u),rd(v),rd(w),rd(x);
      lct.cut(u,v);
      lct.link(w,x);
    }
    else if(t==1){
      int p,x;
      rd(p),rd(x);
      lct.add(p,x);
    }
    else{
      int u,p;
      rd(u),rd(p);
      lct.cut(u,p);
      wt(lct.sum(u)),wt('\n');
      lct.link(u,p);
    }
  }
}
