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
struct has_middle_impl{
  template<typename T>
  static auto check(T&&x)->decltype(x.middle,std::true_type{});
  template<typename T>
  static auto check(...)->std::false_type;
};
template<typename T>
struct has_middle:public decltype(has_middle_impl::check<T>(std::declval<T>())){};
template<typename T>
inline constexpr bool has_middle_v=has_middle<T>::value;

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
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
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
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
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
        if constexpr(has_middle_v<T>){
          if(pp->middle==p)pp->middle=nd;
          else if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
        else{
          if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
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
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
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
            if constexpr(has_middle_v<T>){
              if(pp->par->middle==pp)nd->par->middle=nd;
              else if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
            else{
              if(pp->par->left==pp)nd->par->left=nd;
              else if(pp->par->right==pp)nd->par->right=nd;
            }
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
        if constexpr(has_middle_v<T>){
          if(pp->middle==p)pp->middle=nd;
          else if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
        else{
          if(pp->left==p)pp->left=nd;
          else if(pp->right==p)pp->right=nd;
        }
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
  if constexpr(has_push_v<T>)r->push();
  splay<T,true>(r);
  r->left=l;
  l->par=r;
  if constexpr(has_update_v<T>)r->update();
  return r;
}
template<typename T,bool correct_parent=true>
[[nodiscard]]T* left_most(T*nd){
  T nil;
  T *rnd=&nil;
  while(nd->left){
    if constexpr(has_push_v<T>)nd->push();
    T *c=nd->left;
    if(!c->left){
      rnd->left=nd;
      if constexpr(has_update_v<T>)nd->par=rnd;
      rnd=rnd->left;
      nd=c;
    }
    else{
      if constexpr(has_push_v<T>)c->push();
      nd->left=c->right;
      c->right=nd;
      if constexpr(has_update_v<T>&&correct_parent){
        if(nd->left)nd->left->par=nd;
        nd->par=c;
      }
      if constexpr(has_update_v<T>){
        nd->update();
        c->par=rnd;
      }
      rnd->left=c;
      nd=c->left;
      rnd=rnd->left;
    }
  }
  if constexpr(has_push_v<T>)nd->push();
  rnd->left=nd->right;
  nd->right=nil.left;
  if constexpr(has_update_v<T>&&correct_parent){
    if(rnd->left)rnd->left->par=rnd;
    if(nd->right)nd->right->par=nd;
    nd->par=nullptr;
  }
  if constexpr(has_update_v<T>){
    while(rnd){
      rnd->update();
      rnd=rnd->par;
    }
  }
  return nd;
}
template<typename T,bool correct_parent=true>
[[nodiscard]]T* right_most(T*nd){
  T nil;
  T *lnd=&nil;
  while(nd->right){
    if constexpr(has_push_v<T>)nd->push();
    T *c=nd->right;
    if(!c->right){
      lnd->right=nd;
      if constexpr(has_update_v<T>)nd->par=lnd;
      lnd=lnd->right;
      nd=c;
    }
    else{
      if constexpr(has_push_v<T>)c->push();
      nd->right=c->left;
      c->left=nd;
      if constexpr(has_update_v<T>&&correct_parent){
        if(nd->right)nd->right->par=nd;
        nd->par=c;
      }
      if constexpr(has_update_v<T>){
        nd->update();
        c->par=lnd;
      }
      lnd->right=c;
      nd=c->right;
      lnd=lnd->right;
    }
  }
  if constexpr(has_push_v<T>)nd->push();
  lnd->right=nd->left;
  nd->left=nil.right;
  if constexpr(has_update_v<T>&&correct_parent){
    if(lnd->right)lnd->right->par=lnd;
    if(nd->left)nd->left->par=nd;
    nd->par=nullptr;
  }
  if constexpr(has_update_v<T>){
    while(lnd){
      lnd->update();
      lnd=lnd->par;
    }
  }
  return nd;
}
template<typename T>
void lct_expose(T*nd){
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
void lct_link(T*u,T*v){
  lct_expose(v);
  v->reverse();
  v->par=u;
}
template<typename T>
void lct_cut(T*u,T*v){
  lct_expose(u);
  u->reverse();
  lct_expose(v);
  v->left->par=nullptr;
  v->left=nullptr;
  v->update();
}
using ll=long long;
struct node{
  node *left=nullptr,*right=nullptr,*par=nullptr;
  ll v=0,sum=0;
  bool rev=false;
  void update(){
    sum=v;
    if(left)sum+=left->sum;
    if(right)sum+=right->sum;
  }
  inline void reverse(){
    rev^=1;
    std::swap(left,right);
  }
  inline void push(){
    if(rev){
      if(left)left->reverse();
      if(right)right->reverse();
      rev=false;
    }
  }
};
node nd[200000];
int main(){
  int n,q;
  rd(n),rd(q);
  for(int i=0;i<n;i++){
    int a;
    rd(a);
    nd[i].v=nd[i].sum=a;
  }
  for(int i=0;i<n-1;i++){
    int u,v;
    rd(u),rd(v);
    lct_link(&nd[u],&nd[v]);
  }
  while(q--){
    int t;
    rd(t);
    if(t==0){
      int u,v,w,x;
      rd(u),rd(v),rd(w),rd(x);
      lct_cut(&nd[u],&nd[v]);
      lct_link(&nd[w],&nd[x]);
    }
    else if(t==1){
      int p,x;
      rd(p),rd(x);
      lct_expose(&nd[p]);
      nd[p].v+=x;
      nd[p].sum+=x;
    }
    else{
      int u,v;
      rd(u),rd(v);
      lct_expose(&nd[u]);
      nd[u].reverse();
      lct_expose(&nd[v]);
      wt(nd[v].sum),wt('\n');
    }
  }
}
