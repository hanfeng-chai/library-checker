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
struct has_update_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.update(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_update:public decltype(has_update_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_update_v=has_update<T>::value;
struct has_push_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.push(),std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_push:public decltype(has_push_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_push_v=has_push<T>::value;

template<typename T,bool no_push=false>
void splay(T*nd){
  if constexpr(has_push_v<T>&&!no_push)nd->push();
  while(nd->par){
    T *p=nd->par;
    T *pp=p->par;
    if constexpr(has_push_v<T>&&!no_push){
      if(pp)pp->push();
      p->push();
      nd->push();
    }
    if(p->left==nd){
      if(pp){
        if(pp->left==p){
          nd->par=pp->par;
          if(pp->par){
            if(pp->par->left==pp)nd->par->left=nd;
            else if(pp->par->right==pp)nd->par->right=nd;
          }
          pp->left=p->right;
          if(pp->left)pp->left->par=pp;
          p->left=nd->right;
          if(p->left)p->left->par=p;
          nd->right=p;
          p->par=nd;
          p->right=pp;
          pp->par=p;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
        else if(pp->right==p){
          nd->par=pp->par;
          if(pp->par){
            if(pp->par->left==pp)nd->par->left=nd;
            else if(pp->par->right==pp)nd->par->right=nd;
          }
          p->left=nd->right;
          if(p->left)p->left->par=p;
          pp->right=nd->left;
          if(pp->right)pp->right->par=pp;
          nd->left=pp;
          pp->par=nd;
          nd->right=p;
          p->par=nd;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
      }
      nd->par=pp;
      if(pp){
        if(pp->left==p)pp->left=nd;
        else if(pp->right==p)pp->right=nd;
      }
      p->left=nd->right;
      if(p->left)p->left->par=p;
      nd->right=p;
      p->par=nd;
      if constexpr(has_update_v<T>)p->update(),nd->update();
      break;
    }
    else if(p->right==nd){
      if(pp){
        if(pp->left==p){
          nd->par=pp->par;
          if(pp->par){
            if(pp->par->left==pp)nd->par->left=nd;
            else if(pp->par->right==pp)nd->par->right=nd;
          }
          p->right=nd->left;
          if(p->right)p->right->par=p;
          pp->left=nd->right;
          if(pp->left)pp->left->par=pp;
          nd->left=p;
          p->par=nd;
          nd->right=pp;
          pp->par=nd;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
        else if(pp->right==p){
          nd->par=pp->par;
          if(pp->par){
            if(pp->par->left==pp)nd->par->left=nd;
            else if(pp->par->right==pp)nd->par->right=nd;
          }
          pp->right=p->left;
          if(pp->right)pp->right->par=pp;
          p->right=nd->left;
          if(p->right)p->right->par=p;
          nd->left=p;
          p->par=nd;
          p->left=pp;
          pp->par=p;
          if constexpr(has_update_v<T>)pp->update(),p->update(),nd->update();
          continue;
        }
      }
      nd->par=pp;
      if(pp){
        if(pp->left==p)pp->left=nd;
        else if(pp->right==p)pp->right=nd;
      }
      p->right=nd->left;
      if(p->right)p->right->par=p;
      nd->left=p;
      p->par=nd;
      if constexpr(has_update_v<T>)p->update(),nd->update();
      break;
    }
    else break;
  }
}
template<typename T>
[[nodiscard]]T* near(T *nd,decltype(T::key)k){
  while(true){
    if(k<nd->key){
      if constexpr(has_push_v<T>)nd->push();
      if(nd->left)nd=nd->left;
      else{
        splay<T,true>(nd);
        return nd;
      }
    }
    else if(nd->key<k){
      if constexpr(has_push_v<T>)nd->push();
      if(nd->right)nd=nd->right;
      else{
        splay<T,true>(nd);
        return nd;
      }
    }
    else{
      splay<T,true>(nd);
      return nd;
    }
  }
  return nullptr;
}
template<typename T>
[[nodiscard]]T* get_k(T *nd,decltype(T::sz)k){
  while(true){
    if constexpr(has_push_v<T>)nd->push();
    decltype(T::sz) lsz=nd->left?nd->left->sz:0;
    if(lsz==k)break;
    else if(k<lsz)nd=nd->left;
    else{
      nd=nd->right;
      k-=1+lsz;
    }
  }
  splay<T,true>(nd);
  return nd;
}
template<typename T>
[[nodiscard]]T* merge(T* l,T *r){
  if(!l)return r;
  if(!r)return l;
  while(r->left){
    if constexpr(has_push_v<T>)r->push();
    r=r->left;
  }
  splay<T,true>(r);
  r->left=l;
  l->par=r;
  if constexpr(has_update_v<T>)r->update();
  return r;
}
template<typename T>
void expose(T*nd){
  while(true){
    splay(nd);
    nd->right=nullptr;
    if constexpr(has_update_v<T>)nd->update();
    if(!nd->par)break;
    splay(nd->par);
    nd->par->right=nd;
    if constexpr(has_update_v<T>)nd->par->update();
  }
}
template<typename T>
void link(T*u,T*v){
  expose(v);
  v->reverse();
  v->par=u;
}
template<typename T>
void cut(T*u,T*v){
  expose(u);
  u->reverse();
  expose(v);
  v->left->par=nullptr;
  v->left=nullptr;
  v->update();
}
#include<vector>
using ll=long long;
constexpr ll mod=998244353;
struct node{
  node *left=nullptr,*right=nullptr,*par=nullptr;
  ll va,vb;
  ll pa,pb,pc;
  bool rev=false;
  void update(){
    pa=va,pb=pc=vb;
    if(left){
      pb=(pa*left->pb+pb)%mod;
      pc=(pc*left->pa+left->pc)%mod;
      pa=(pa*left->pa)%mod;
    }
    if(right){
      pb=(pb*right->pa+right->pb)%mod;
      pc=(pa*right->pc+pc)%mod;
      pa=(pa*right->pa)%mod;
    }
  }
  void reverse(){
    std::swap(pb,pc);
    std::swap(left,right);
    rev^=1;
  }
  void push(){
    if(rev){
      if(left)left->reverse();
      if(right)right->reverse();
      rev=false;
    }
  }
};
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<node>dat(n);
  for(int i=0;i<n;i++){
    int a,b;
    rd(a),rd(b);
    dat[i].va=dat[i].pa=a;
    dat[i].vb=dat[i].pb=dat[i].pc=b;
  }
  for(int i=0;i<n-1;i++){
    int u,v;
    rd(u),rd(v);
    link(&dat[u],&dat[v]);
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int u,v,w,x;
      rd(u),rd(v),rd(w),rd(x);
      cut(&dat[u],&dat[v]);
      link(&dat[w],&dat[x]);
    }
    else if(t==1){
      int p,c,d;
      rd(p),rd(c),rd(d);
      expose(&dat[p]);
      dat[p].va=c,dat[p].vb=d;
      dat[p].update();
    }
    else{
      int u,v,x;
      rd(u),rd(v),rd(x);
      expose(&dat[u]);
      dat[u].reverse();
      expose(&dat[v]);
      wt((dat[v].pa*x+dat[v].pb)%mod);
      wt('\n');
    }
  }
}
