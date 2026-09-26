/*
Author : QedDust413 & Killer_joke
need C++20.
poly tem IV.
*/
#include <bits/stdc++.h>
#pragma GCC target("avx2")
#include <immintrin.h>
struct auto_timer{
	std::chrono::system_clock::time_point lst;
	auto_timer() : lst(std::chrono::system_clock::now()){
		
	}
	~auto_timer(){
		std::chrono::duration<long double,std::milli> tott=std::chrono::system_clock::now()-lst;
		std::clog<<tott.count()<<"ms"<<std::endl;
	}
};
#ifdef NDEBUG
#define TEST(p) 
#else
#define TEST(p) p;
#endif
namespace No_Poly{
using i64=int64_t;
using u32=uint32_t;
using u64=uint64_t;
using idt=std::size_t;
constexpr u32 M=998244353;
using u32x8=__v8su;
using u64x4=__v4du;
using I256=__m256i;
inline u64x4 fus_mul(u32x8 x,u32x8 y){return (u64x4)_mm256_mul_epu32((I256)x,(I256)y);}
inline u32x8 swaplohi128(u32x8 x){return (u32x8)_mm256_permute2x128_si256((I256)x,(I256)x,1);}
template<int typ>inline u32x8 shuffle(u32x8 x){return (u32x8)_mm256_shuffle_epi32((I256)x,typ);}
template<int typ>inline u32x8 blend(u32x8 x,u32x8 y){return (u32x8)_mm256_blend_epi32((I256)x,(I256)y,typ);}
inline u32x8&x8(u32*p){return*((u32x8*)p);}
inline const u32x8&x8(const u32*p){return*((u32x8*)p);}
inline u32x8 min_u32(u32x8 x,u32x8 y){return (u32x8)_mm256_min_epu32((I256)x,(I256)y);}
constexpr u32x8 padd(u32 x){return (u32x8){x,x,x,x,x,x,x,x};}
inline u32x8 loadu_u32x8(const u32*f){return (u32x8)_mm256_loadu_si256((const __m256i_u*)f);}
inline void storeu(void*f,u32x8 x){_mm256_storeu_si256((__m256i_u*)f,(I256)x);}

constexpr u32 get_nr(u32 M){u32 Iv=1;for(int i=0;i<5;++i){Iv*=2-M*Iv;}return Iv;}
template<u32 M,std::signed_integral T>constexpr u32 sf_md(T x){
    u32 y=x%M;
    return std::min(y,y+M);
}
template<u32 M,std::unsigned_integral T>constexpr u32 sf_md(T x){return x%M;}
constexpr idt bcl(idt x){return ((x<2)?1:idt(2)<<std::__lg(x-1));}

constexpr u32 R=(-M)%M,E={},nR=M-R,M2=M*2,niv=-get_nr(M),R2=(-u64(M))%M;
constexpr u32 shrk(u32 x){return std::min(x,x-M);}
constexpr u32 shrk2(u32 x){return std::min(x,x-M2);}
constexpr u32 dil2(u32 x){return std::min(x,x+M2);}
constexpr u32 reduce(u64 x){return (x+u64(u32(x)*niv)*M)>>32;}
constexpr u32 reduce_s(u64 x){return shrk(reduce(x));}

constexpr u32 add(u32 x,u32 y){return shrk2(x+y);}
constexpr u32 sub(u32 x,u32 y){return dil2(x-y);}
constexpr u32 mul(u32 x,u32 y){return reduce(u64(x)*y);}
constexpr u32 mul_s(u32 x,u32 y){return reduce_s(u64(x)*y);}
constexpr u32 qpw(u32 a,u32 b,u32 r=R){for(;b;b/=2,a=mul(a,a)){b&1?r=mul(r,a):r;}return r;}
template<std::integral T>constexpr u32 sf_qpw(u32 a,T b,u32 r=R){return shrk(qpw(a,sf_md<(M-1)>(b),r));}
constexpr u32 inv(u32 x){return qpw(x,M-2);}
constexpr u32 dvs(u32 x,u32 y){return qpw(y,M-2,x);}
constexpr u32 neg(u32 x){return M2-x;}
constexpr u32 in(u32 x){return mul(x,R2);}
template<std::integral T>constexpr u32 sf_in(T x){return in(sf_md<M>(x));}
constexpr u32 in_s(u32 x){return mul_s(x,R2);}
constexpr u32 out(u32 x){return reduce_s(x);}
constexpr bool equals(u32 x,u32 y){return out(x)==out(y);}
constexpr void clr(u32&x){x=E;}

constexpr auto Rx8=padd(R),Ex8=padd(E),Mx8=padd(M),M2x8=padd(M2),nivx8=padd(niv);
inline u32x8 shrk(u32x8 x){return min_u32(x,x-Mx8);}
inline u32x8 dil2(u32x8 x){return min_u32(x,x+M2x8);}
inline u32x8 shrk2(u32x8 x){return min_u32(x,x-M2x8);}
inline u32x8 add(u32x8 x,u32x8 y){return shrk2(x+y);}
inline u32x8 sub(u32x8 x,u32x8 y){return dil2(x-y);}
inline u32x8 reduce(u64x4 a,u64x4 b){
    auto c=fus_mul(u32x8(a),nivx8),d=fus_mul(u32x8(b),nivx8);
    c=fus_mul(u32x8(c),Mx8),d=fus_mul(u32x8(d),Mx8);
    return blend<0xaa>(u32x8((a+c)>>32),u32x8(b+d));
}
inline u32x8 mul(u32x8 x,u32x8 y){
    return reduce(fus_mul(x,y),fus_mul(u32x8(u64x4(x)>>32),u32x8(u64x4(y)>>32)));
}
inline u32x8 qpw(u32x8 a,u32 b,u32x8 r=Rx8){for(;b;a=mul(a,a),b/=2){if(b&1){r=mul(r,a);}}return r;}
inline u32x8 inv(u32x8 x){return qpw(x,M-2);}
inline u32x8 dvs(u32x8 x,u32x8 y){return qpw(y,M-2,x);}
inline u32x8 mul_s(u32x8 x,u32x8 y){return shrk(mul(x,y));}
inline u32x8 neg(u32x8 x){return M2x8-x;}
inline void clr(u32x8&x){x=Ex8;}

constexpr u32 _Amul(u32 a,u32 b,u32 c){return mul(a+b,c);}
constexpr u32 _Smul(u32 a,u32 b,u32 c){return mul(a-b+M2,c);}
inline u32x8 _Amul(u32x8 a,u32x8 b,u32x8 c){return mul(a+b,c);}
inline u32x8 _Smul(u32x8 a,u32x8 b,u32x8 c){return mul(a-b+M2x8,c);}
template<int typ>inline u32x8 Neg(u32x8 x){return blend<typ>(x,M2x8-x);}
constexpr u32x8 powXx8(u32 X){
	auto X2=mul_s(X,X),X3=mul_s(X2,X),X4=mul_s(X2,X2),X5=mul_s(X4,X),X6=mul_s(X4,X2),X7=mul_s(X4,X3);
	return (u32x8){R,X,X2,X3,X4,X5,X6,X7};
}
constexpr u32 _ADmul(u32 a,u32 b,u32 c,u32 d){return reduce_s(u64(a)*b+u64(c)*d);}
inline u32x8 _ADmul(u32x8 a,u32x8 b,u32x8 c,u32x8 d){
    return shrk(reduce(fus_mul(a,b)+fus_mul(c,d),fus_mul(u32x8(u64x4(a)>>32),u32x8(u64x4(b)>>32))+fus_mul(u32x8(u64x4(c)>>32),u32x8(u64x4(d)>>32))));
}

constexpr u32 sqt(u32 x){
	auto y=R,Im=E;
	while(sf_qpw(Im=sub(mul(y,y),x),M/2)==R){++y;}
    auto a0=y,a1=R,r0=R,r1=E;
	for(auto b=(M+1)/2;b;b/=2){
        if(b&1){
            std::tie(r0,r1)=std::tuple{_ADmul(r0,a0,mul(r1,a1),Im),_ADmul(r0,a1,r1,a0)};
        }
        std::tie(a0,a1)=std::tuple{_ADmul(a0,a0,mul(a1,a1),Im),_ADmul(a0,a1,a1,a0)};
    }
    auto z=out(r0);
	return in_s(std::min(z,M-z));
}
constexpr u32 sf_sqt(u32 x){
    if(out(x)==0){return 0;}
    if(sf_qpw(x,M/2)!=nR){return sqt(x);}
    return -1;
}
constexpr u32 pr2_rt(u32 M,u32 _g=2){
    for(;sf_qpw(_g,M/2)==R;++_g);
    return _g;
}
constexpr auto Half=shrk(inv(in(2))),nHalf=M-Half;
struct vec_v{
    u32x8 v;
    constexpr vec_v(u32 x):v{padd(x)}{}
    constexpr operator u32()const{return v[0];}
    constexpr operator u32x8()const{return v;}
};
inline void vec_op(auto f,idt n,auto&&op){idt i=0;for(;i+7<n;i+=8){op(x8(f+i));}for(;i<n;++i){op(f[i]);}}
inline void vec_op(auto f,auto g,idt n,auto&&op){idt i=0;for(;i+7<n;i+=8){op(x8(f+i),x8(g+i));}for(;i<n;++i){op(f[i],g[i]);}}
inline void vec_op(auto f,auto g,auto h,idt n,auto&&op){idt i=0;for(;i+7<n;i+=8){op(x8(f+i),x8(g+i),x8(h+i));}for(;i<n;++i){op(f[i],g[i],h[i]);}}
inline void vec_op(auto f,auto g,auto h,auto o,idt n,auto&&op){idt i=0;for(;i+7<n;i+=8){op(x8(f+i),x8(g+i),x8(h+i),x8(o+i));}for(;i<n;++i){op(f[i],g[i],h[i],o[i]);}}
namespace raw_ntt{
constexpr auto _g=pr2_rt(M);
constexpr auto lml=__builtin_ctz(M-1);
struct P_R_Tab{
	u32 t[lml+1];
	constexpr P_R_Tab(u32 G):t{}{
        t[lml]=sf_qpw(G,M>>lml);
        for(int i=lml;i>0;--i){t[i-1]=mul_s(t[i],t[i]);}
    }
	constexpr u32 operator[](int i)const{return t[i];}
};
struct ntt_info_base4x8{
	u32 rt3[lml-2],rt3_I[lml-2];
	u32x8 rt4ix8[lml-3],rt4ix8_I[lml-3];
	constexpr ntt_info_base4x8(const P_R_Tab&w,const P_R_Tab&wI):rt3{},rt3_I{},rt4ix8{},rt4ix8_I{}{   
		auto pr=R,pr_I=R;
		for(int i=0;i<lml-2;pr=mul(pr,wI[i+3]),pr_I=mul(pr_I,w[i+3]),++i){
			rt3[i]=mul_s(pr,w[i+3]),rt3_I[i]=mul_s(pr_I,wI[i+3]);
		}
		pr=R,pr_I=R;
		for(int i=0;i<lml-3;pr=mul(pr,wI[i+4]),pr_I=mul(pr_I,w[i+4]),++i){
			rt4ix8[i]=powXx8(mul_s(pr,w[i+4])),rt4ix8_I[i]=powXx8(mul_s(pr_I,wI[i+4]));
		}
	}
};
constexpr P_R_Tab rt1={_g},rt1_I={inv(_g)};
constexpr ntt_info_base4x8 iab4={rt1,rt1_I};
constexpr auto Img=rt1[2];
constexpr auto Imgx8=padd(Img);
template<bool strict=false>inline void dif_2(u32&x,u32&y){
	auto sum=add(x,y),diff=sub(x,y);x=sum,y=diff;
	if constexpr(strict){x=shrk(x),y=shrk(y);}
}
template<bool strict=false>inline void dif_4(u32&x,u32&y,u32&z,u32&w){
	auto a=sub(x,z),b=_Smul(y,w,Img);
	x=add(x,z),y=add(y,w),z=add(a,b),w=sub(a,b),a=add(x,y),b=sub(x,y),x=a,y=b;
	if constexpr(strict){x=shrk(x),y=shrk(y),z=shrk(z),w=shrk(w);}
}
template<bool strict=false>inline void vec_dif_base4(u32x8*f,idt n){
	auto L=n/2;
	if(__builtin_ctzll(n)&1){
		for(idt j=0;j<L;++j){
			auto x=f[j],y=f[j+L];
			f[j]=x+y,f[j+L]=x-y+M2x8;
		}L/=2;
	}L/=2;
	for(idt l=L*4,k;L;l=L,L/=4){
		auto r=R,r2=R,r3=nR;k=1;
		for(auto i=f;i!=(f+n);r=mul_s(r,iab4.rt3[__builtin_ctzll(k++)]),r2=mul_s(r,r),r3=mul_s(r2,neg(r)),i+=l){
			auto rx8=padd(r),r2x8=padd(r2),r3x8=padd(r3);
			for(auto F0=i,F1=F0+L,F2=F1+L,F3=F2+L;F3!=i+l;++F0,++F1,++F2,++F3){
				auto f0=shrk2(*F0),f1=mul(*F1,rx8),f2=mul(*F2,r2x8),f3=mul(*F3,r3x8);
				auto f1f3=_Amul(f1,f3,Imgx8),f02=add(f0,f2),f13=sub(f1,f3),f_02=sub(f0,f2);
				*F0=f02+f13,*F1=f02-f13+M2x8,*F2=f_02+f1f3,*F3=f_02-f1f3+M2x8;
			}
		}
	}
	constexpr u32x8 pr2={R,R,R,Img,R,R,R,Img},pr4={R,R,R,R,R,rt1[3],Img,mul_s(Img,rt1[3])};
	auto rx8=Rx8;
	for(idt i=0;i<n;++i){
		auto&fi=f[i];
        fi=mul(fi,rx8),rx8=mul_s(rx8,iab4.rt4ix8[__builtin_ctzll(~i)]);
		fi=_Amul(Neg<0xf0>(fi),swaplohi128(fi),pr4);
		fi=_Amul(Neg<0xcc>(fi),shuffle<0x4e>(fi),pr2);
		fi=sub(shuffle<0xb1>(fi),Neg<0x55>(fi));
		if constexpr(strict){fi=shrk(fi);}
	}
}
template<u32 fx>inline void dit_2(u32&x,u32&y){
	constexpr auto iv2=mul_s(inv(in(2)),fx);
	auto a=_Amul(x,y,iv2),b=_Smul(x,y,iv2);x=a,y=b;
}
template<u32 fx>inline void dit_4(u32&x,u32&y,u32&z,u32&w){
	constexpr auto iv4=mul_s(inv(in(4)),fx),Imgi4=mul_s(iv4,Img);
	auto a=_Amul(x,y,iv4),b=_Smul(x,y,iv4);
	x=a,y=b,a=_Amul(z,w,iv4),b=_Smul(w,z,Imgi4),z=sub(x,a),w=sub(y,b),x=add(x,a),y=add(y,b);
}
template<u32 fx>inline void vec_dit_base4(u32x8*f,idt n){
	constexpr auto nR2=in_s(nR),M8=(M-1)/8;
	constexpr u32x8 pr2={nR2,nR2,nR2,in(Img),nR2,nR2,nR2,in(Img)},pr4={fx,fx,fx,fx,fx,mul_s(fx,rt1_I[3]),mul_s(fx,rt1_I[2]),mul_s(fx,mul_s(rt1_I[2],rt1_I[3]))};
	auto rx8=padd(M8>>__builtin_ctzll(n));
    idt L=1;
	for(idt i=0;i<n;++i){
		auto&fi=f[i];
		fi=_Amul(Neg<0xaa>(fi),shuffle<0xb1>(fi),pr2);
		fi=_Amul(Neg<0xcc>(fi),shuffle<0x4e>(fi),pr4);
		fi=_Amul(Neg<0xf0>(fi),swaplohi128(fi),rx8);
        rx8=mul_s(rx8,iab4.rt4ix8_I[__builtin_ctzll(~i)]);
	}
	for(idt l=L*4,k;L<(n/2);L=l,l*=4){
		auto r=R,r2=R,r3=R;k=1;
		for(auto i=f;i!=(f+n);r=mul_s(r,iab4.rt3_I[__builtin_ctzll(k++)]),r2=mul_s(r,r),r3=mul_s(r2,r),i+=l){
			auto rx8=padd(r),r2x8=padd(r2),r3x8=padd(r3);
			for(auto F0=i,F1=F0+L,F2=F1+L,F3=F2+L;F3!=i+l;++F0,++F1,++F2,++F3){
				auto f0=*F0,f1=*F1,f2=neg(*F2),f3=*F3;
				auto f2f3=_Amul(f3,f2,Imgx8),f01=add(f0,f1),f23=sub(f2,f3),f_01=sub(f0,f1);
				*F0=sub(f01,f23),*F1=_Amul(f_01,f2f3,rx8),*F2=_Amul(f01,f23,r2x8),*F3=_Smul(f_01,f2f3,r3x8);
			}
		}
	}
	if(__builtin_ctzll(n)&1){
		for(idt j=0;j<L;++j){
			auto x=f[j],y=f[j+L];
			f[j]=add(x,y),f[j+L]=sub(x,y);
		}
	}
}
TEST(idt ntt_len)
template<bool strict=false>inline void dif(u32*A,idt lm){
    TEST(ntt_len+=lm)
	switch(lm){
		case 1:if constexpr(strict){A[0]=shrk(A[0]);}break;
		case 2:dif_2<strict>(A[0],A[1]);break;
		case 4:dif_4<strict>(A[0],A[1],A[2],A[3]);break;
		default:vec_dif_base4<strict>((u32x8*)A,lm/8);
	}
}
template<u32 fx=R>inline void dit(u32*A,idt lm){
    TEST(ntt_len+=lm)
	switch(lm){
		case 1:if constexpr(!equals(fx,R)){A[0]=mul(A[0],fx);}break;
		case 2:dit_2<fx>(A[0],A[1]);break;
		case 4:dit_4<fx>(A[0],A[1],A[2],A[3]);break;
		default:vec_dit_base4<fx>((u32x8*)A,lm/8);
	}
}
}//namespace raw_ntt
using raw_ntt::dif;
using raw_ntt::dit;
//u32* u32 const u32* idt oth
TEST(std::set<u32*> __p_st)
inline u32*alc(idt n){
    auto p=new(std::align_val_t(32))u32[n];
    TEST(__p_st.emplace(p))
    return p;
}
inline void fre(u32*p){
#ifndef NDEBUG
    auto z=__p_st.erase(p);
    if(z==0){std::cerr<<"fre ptr which never alc\n",exit(0);}
#endif
    ::operator delete[](p,std::align_val_t(32));
}
template<class T,idt al>struct aligned_alcor{
	typedef T value_type;
	T*allocate(idt n){return new(std::align_val_t(al))T[n];}
	template<class Jok>struct rebind{using other=aligned_alcor<Jok,al>;};
	void deallocate(T*p,idt){::operator delete[](p,std::align_val_t(al));}
};
using vec=std::vector<u32,aligned_alcor<u32,32> >;
template<class T>inline T*cpy(T*f,const T*g,idt n){return (T*)memcpy(f,g,n*sizeof(T));}
template<class T>inline T*mmv(T*f,const T*g,idt n){return (T*)memmove(f,g,n*sizeof(T));}
template<class T>inline T*clr(T*f,idt n){return (T*)memset(f,0,n*sizeof(T));}
template<class T>inline T*rcpy(T*f,const T*g,idt n){return std::reverse_copy(g,g+n,f),f;}
template<class T>inline void rev(T*f,idt n){std::reverse(f,f+n);}
void dot(u32*f,const u32*g,idt n){vec_op(f,g,n,[](auto&fi,auto&gi){fi=mul(fi,gi);});}
void dot(u32*f,const u32*g,const u32*h,idt n){vec_op(f,g,h,n,[](auto&fi,auto&gi,auto&hi){return fi=mul(gi,hi);});}
void add(u32*f,const u32*g,idt n){vec_op(f,g,n,[](auto&fi,auto&gi){fi=add(fi,gi);});}
void add(u32*f,const u32*g,const u32*h,idt n){vec_op(f,g,h,n,[](auto&fi,auto&gi,auto&hi){return fi=add(gi,hi);});}
void sub(u32*f,const u32*g,idt n){vec_op(f,g,n,[](auto&fi,auto&gi){fi=sub(fi,gi);});}
void sub(u32*f,const u32*g,const u32*h,idt n){vec_op(f,g,h,n,[](auto&fi,auto&gi,auto&hi){return fi=sub(gi,hi);});}
void adddot(u32*a,const u32*b,const u32*c,const u32*d,idt n){vec_op(a,b,c,d,n,[](auto&A,auto&B,auto&C,auto&D){A=_ADmul(A,B,C,D);});}
void vec_multi_iv(u32x8*f,const u32x8*g,idt n){
	if(n==0){return;}
	f[0]=g[0];for(idt i=1;i<n;++i){f[i]=mul(f[i-1],g[i]);}
	f[n-1]=inv(f[n-1]);
	for(auto i=n-1;i;--i){
		auto ivi=f[i];
		f[i]=mul(ivi,f[i-1]),f[i-1]=mul(ivi,g[i]);
	}
}
void multi_iv(u32*f,const u32*g,idt n){
	vec_multi_iv((u32x8*)f,(u32x8*)g,n/8);
	for(auto i=(n/8)*8;i<n;++i){f[i]=inv(g[i]);}
}
template<u32 fx=R>inline void conv(u32*f,u32*g,idt lm){dif(f,lm),dif(g,lm),dot(f,g,lm),dit<fx>(f,lm);}

template<class V,idt alz=16>struct sim_inf_seq{
    using T=typename V::value_type;
	std::function<void(T*,idt,idt)> f;
	mutable V v;
	sim_inf_seq(auto&&F):f{F}{}
    const T*raw()const{return v.data();}
	const T*rsv(idt l)const{
		auto ol=v.size();
		if(l>ol)[[unlikely]]{l=std::max((l+alz-1)&-alz,ol*2),v.resize(l),f(v.data(),ol,l);}
		return raw();
	}
	const T&operator[](idt pos)const{return rsv(pos+1)[pos];}
};
sim_inf_seq<vec> 
Id=[](u32*f,idt l,idt r){
	for(;l<8;++l){f[l]=in(l);}
	for(auto i=l;i<r;i+=8){x8(f+i)=add(x8(f+i-8),padd(in(8)));}
},
Iv=[](u32*f,idt l,idt r){
	auto id=Id.rsv(r);
	if(l<8){x8(f)=inv(x8(id)),l=8;}
	vec_multi_iv((u32x8*)(f+l),(const u32x8*)(id+l),(r-l)/8);
},
Fac=[](u32*f,idt l,idt r){
	auto id=Id.rsv(r);
	if(l==0){f[0]=R,l=1;}
	for(auto i=l;i<r;++i){f[i]=mul(f[i-1],id[i]);}
},
IFac=[](u32*f,idt l,idt r){
	auto iv=Iv.rsv(r);
	if(l==0){f[0]=R,l=1;}
	for(auto i=l;i<r;++i){f[i]=mul(f[i-1],iv[i]);}
};

void deriv(u32*f,const u32*g,idt n){
	idt i=1;
	auto id=Id.rsv(n);
	if(n>16){
		for(;i<8;++i){f[i-1]=mul(g[i],id[i]);}
		for(;i+7<n;i+=8){
            storeu(f+i-1,mul(x8(g+i),x8(id+i)));
        }
	}
	for(;i<n;++i){f[i-1]=mul(g[i],id[i]);}
}
void integ(u32*f,u32 C,const u32*g,idt n){
	auto i=n;
	auto iv=Iv.rsv(n);
	if(n>16){
		for(;(i&7)!=7;--i){f[i]=mul(g[i-1],iv[i]);}
		for(;i>7;i-=8){x8(f+i-7)=mul(loadu_u32x8(g+i-8),x8(iv+i-7));};
	}
	for(;i>0;--i){f[i]=mul(g[i-1],iv[i]);}f[0]=C;
}
inline void integ(u32*f,const u32*g,idt n){integ(f,E,g,n);}

inline void scanp(u32*f,idt n=1){
	for(idt i=0;i<n;++i){
		std::cin>>f[i],f[i]=in(f[i]);
	}
}
inline void printp(const u32*f,idt n=1,const char*fmt=" \n"){
	for(idt i=0;i<n;++i){
		std::cout<<out(f[i])<<fmt[i+1==n];
	}
}
inline void printp(u32 x){
    std::cout<<out(x);
}
inline void rndp(u32*f,idt n,std::mt19937_64&rng){
    for(idt i=0;i<n;++i){
        f[i]=rng()%M;
    }
}
inline void __print_difp(const u32*f,idt n){
    std::cout<<"dif:",printp(f,n);
    auto g=alc(n);
    cpy(g,f,n),dit(g,n);
    std::cout<<"real:",printp(g,n);
    fre(g);
}
inline idt equalp(const u32*f,const u32*g,idt n){
    for(idt i=0;i<n;++i){
        if(!equals(f[i],g[i])){return i;}
    }
    return -1;
}
inline void cdif(u32*f,const u32*g,idt n,idt lm){clr(cpy(f,g,n)+n,lm-n),dif(f,lm);}
void inv(u32*f,const u32*g,idt n){
	auto lm=bcl(n);
	auto o=alc(lm*2),h=o+lm;
	f[0]=inv(g[0]);
	for(idt t=2,m=1,xl;t<=lm;m=t,t*=2){
		xl=std::min(n,t),cdif(o,g,xl,t),cdif(h,f,m,t),dot(o,h,t),dit(o,t);
		clr(o,m),dif(o,t),dot(o,h,t),dit<nR>(o,t),cpy(f+m,o+m,xl-m);
	}fre(o);
}
void quo(u32*f,const u32*g,const u32*h,idt n){
    if(n<=64){
		auto lm=bcl(n*2);
        auto o=alc(lm*2),s=o+lm;
		inv(o,h,n),clr(o+n,lm-n),cpy(s,g,n),clr(s+n,lm-n),conv(o,s,lm),cpy(f,o,n),fre(o);
        return;
	}
    auto bn=bcl(n)/16,bt=(n+bn-1)/bn,bn2=bn*2;
    auto o=alc(bn2),qed=alc(bn2);
    inv(o,h,bn),clr(o+bn,bn),clr(cpy(qed,g,bn)+bn,bn),conv(qed,o,bn2),cpy(f,qed,bn);
	auto Nh=alc(bn2*(bt-1)),Nf=alc(bn2*(bt-1));
    for(idt ds=bn,ds2=bn2;ds<n;ds+=bn,ds2+=bn2){
        idt xl=std::min(bn,n-ds),xl2=std::min(bn2,n+bn-ds);
        cdif(Nh+ds2-bn2,h+ds-bn,xl2,bn2),cdif(Nf+ds2-bn2,f+ds-bn,bn,bn2),clr(qed,bn2);
        for(idt dj=0;dj<ds2;dj+=bn2){
            for(idt i=0;i<bn2;i+=8){x8(qed+i)=add(x8(qed+i),mul(x8(Nf+ds2-bn2-dj+i),x8(Nh+dj+i)));}
		}
        dit<nR>(qed,bn2),clr(qed,bn),add(qed+bn,g+ds,xl),dif(qed,bn2),dot(qed,o,bn2),dit(qed,bn2),cpy(f+ds,qed+bn,xl);
	}
    fre(o),fre(qed),fre(Nh),fre(Nf);
}
void dvs(u32*q,const u32*f,const u32*g,idt n,idt m){
    assert(n>=m);
	auto lm=n-m+1,R=std::min(m,lm);
	auto o=alc(lm);
	clr(rcpy(o,g+m-R,R)+R,lm-R),quo(q,rcpy(q,f+m-1,lm),o,lm),rev(q,lm),fre(o);
}
void dvs(u32*q,u32*r,const u32*f,const u32*g,idt n,idt m){
	dvs(q,f,g,n,m);
	auto lm=bcl(std::min(n,m+m-3)),u=m-1,v=std::min(u,n-u);
	if(v<=16){
		for(idt i=0,k;i<u;++i){
			for(k=0,r[i]=f[i];k<std::min(v,i+1);++k){r[i]=sub(r[i],mul(q[k],g[i-k]));}
		}
	}
	else{
		auto o=alc(lm*2),h=o+lm;
		cdif(o,g,u,lm),cdif(h,q,v,lm),dot(o,h,lm),dit(o,lm),sub(r,f,o,u),fre(o);
	}
}
void ln(u32*f,const u32*g,idt n){dot(f,Id.rsv(n),g,n),quo(f,f,g,n),dot(f,Iv.rsv(n),n);}
template<bool c_inv>void __expi(u32*f,u32*h,const u32*g,idt n){
	f[0]=h[0]=R;if(n==1){return;}
	auto lm=bcl(n);
	auto id=Id.rsv(lm),iv=Iv.rsv(lm);
	auto o=alc(lm*3),A=o+lm,B=A+lm;
	clr(A,lm),A[0]=A[1]=R;
	for(idt t=2,m=1,xl;t<=lm;m=t,t*=2){
		xl=std::min(n,t),dot(o,id,g,m),dif(o,m),dot(o,A,m),dit(o,m);dot(o+m,f,id,m);
		vec_op(o+m,o,m,[](auto&fi,auto&gi){fi=sub(fi,gi),clr(gi);}),dif(o,t);
        cdif(B,h,m,t),dot(o,B,t),dit(o,t),dot(clr(o,m)+m,iv+m,m);
		sub(o+m,g+m,xl-m),dif(o,t),dot(A,o,t),dit<nR>(A,t),cpy(f+m,A+m,xl-m);
		if(c_inv||(t!=lm)){
			cpy(A,f,m),dif(A,std::min(t*2,lm)),dot(o,A,B,t),dit(o,t),clr(o,m);
			dif(o,t),dot(o,B,t),dit<nR>(o,t),cpy(h+m,o+m,xl-m);
		}
	}fre(o);
}
void exp(u32*f,const u32*g,idt n){
    if(n<=64){
        auto p=alc(n);
        return __expi<0>(f,p,g,n),fre(p);
    }
    auto bn=bcl(n)/16,bt=(n+bn-1)/bn,bn2=bn*2;
	auto o=alc(bn2),h=alc(bn2);
	auto id=Id.rsv(n),iv=Iv.rsv(n);
    __expi<1>(f,h,g,bn),clr(h+bn,bn),dif(h,bn2);
	auto Ng=alc(bn2*(bt-1)),Nf=alc(bn2*(bt-1));
	dot(Ng,g,id,bn);
    for(idt ds=bn,ds2=bn2;ds<n;ds+=bn,ds2+=bn2){
        idt xl=std::min(bn,n-ds);
        dot(Ng+ds2-bn,g+ds,id+ds,xl),clr(Ng+ds2-bn+xl,bn-xl);
        if(ds+bn<n){cpy(Ng+ds2,Ng+ds2-bn,bn);}
        dif(Ng+ds2-bn2,bn2),cdif(Nf+ds2-bn2,f+ds-bn,bn,bn2),clr(o,bn2);
        for(idt dj=0;dj<ds2;dj+=bn2){
            for(idt i=0;i<bn2;i+=8){x8(o+i)=add(x8(o+i),mul(x8(Nf+ds2-bn2-dj+i),x8(Ng+dj+i)));}
		}
        dit(o,bn2),clr(o,bn),dif(o,bn2),dot(o,h,bn2),dit(o,bn2);
        dot(o+bn,iv+ds,xl),clr(o,bn),dif(o,bn2),dot(o,Nf,bn2),dit(o,bn2),cpy(f+ds,o+bn,xl); 
    }fre(o),fre(h),fre(Ng),fre(Nf);
}
void sqrtinv(u32*f,const u32*g,idt n){
	auto lm=bcl(n);
	auto o=alc(lm*4),h=o+lm*2;
	f[0]=inv(sqt(g[0]));
	for(idt r=4,t=2,m=1,xl;t<=lm;m=t,t=r,r*=2){
		xl=std::min(n,t),cdif(o,f,m,r),cdif(h,g,xl,r);
		vec_op(h,o,r,[](auto&fi,auto&gi){fi=mul(mul(fi,gi),mul(gi,gi));}),dit<nHalf>(h,r),cpy(f+m,h+m,xl-m);
	}fre(o);
}
template<bool c_inv>void __sqrti(u32*f,u32*h,const u32*g,idt n){
	auto lm=bcl(n);
    auto o=alc(lm*3),H=o+lm,F=H+lm;
	f[0]=sqt(g[0]),h[0]=inv(f[0]),F[0]=f[0];
	for(idt t=2,m=1,xl;t<=lm;m=t,t*=2){
		xl=std::min(t,n),dot(F,F,m),dit(F,m);
		vec_op(F,F+m,g,g+m,m,[](auto&a0,auto&a1,auto&b0,auto&b1){a1=sub(sub(a0,b0),b1),clr(a0);});
		clr(cpy(H,h,m)+m,m),conv<nHalf>(F,H,t),cpy(f+m,F+m,xl-m);
		if(c_inv||(t!=lm)){
			dif(cpy(o,f,t),t),cpy(F,o,t),dot(o,H,t),dit(o,t),dif(clr(o,m),t),dot(o,H,t),dit<nR>(o,t),cpy(h+m,o+m,xl-m);
		}
	}fre(o);
}
void sqrt(u32*f,const u32*g,idt n){
	if(n<=64){auto p=alc(n);return __sqrti<false>(f,p,g,n),fre(p);}
	auto bn=bcl(n)/16,bt=(n+bn-1)/bn,bn2=bn*2;
	auto o=alc(bn2),jok=alc(bn2);
	__sqrti<true>(f,o,g,bn),clr(o+bn,bn),dif(o,bn2);
	auto Nf=alc(bn2*(bt-1)),nf=Nf;
	for(idt ds=bn,xl;ds<n;ds+=bn){
		xl=std::min(bn,n-ds),clr(cpy(nf,f+ds-bn,bn)+bn,bn),dif<1>(nf,bn2),nf+=bn2;
		auto nF=nf,nF1=nf-bn2,NF=Nf;
		for(idt i=0;i<bn;i+=8){x8(jok+i)=neg(mul(x8(nF1+i),x8(NF+i)));}
		for(idt i=bn;i<bn2;i+=8){x8(jok+i)=mul(x8(nF1+i),x8(NF+i));}
		for(idt dj=bn;nF-=bn2,nF1-=bn2,NF+=bn2,dj<ds;dj+=bn){
			for(idt i=0;i<bn;i+=8){x8(jok+i)=sub(x8(jok+i),_Amul(x8(nF1+i),x8(nF+i),x8(NF+i)));}
			for(idt i=bn;i<bn2;i+=8){x8(jok+i)=sub(x8(jok+i),_Smul(x8(nF+i),x8(nF1+i),x8(NF+i)));}
		}
		dit<nR>(jok,bn2),clr(jok+bn,bn),sub(jok,g+ds,xl),dif(jok,bn2),dot(jok,o,bn2),dit<nHalf>(jok,bn2),cpy(f+ds,jok,xl);
	}fre(o),fre(jok),fre(Nf);
}

void Ci(u32*f,u32 z,idt n){
	auto x=R;
    idt i=0;
	for(;i<std::min<idt>(n,8);++i,x=mul(x,z)){f[i]=x;}
	if(n>16){
		auto xx8=padd(x);
		for(;i+7<n;i+=8){x8(f+i)=mul(x8(f+i-8),xx8);}
	}
	for(;i<n;++i){f[i]=mul(f[i-1],z);}
}
void mulk(u32*f,u32 k,const u32*g,idt n){
	auto k_v=vec_v{k};
	vec_op(f,g,n,[k_v](auto&fi,auto&gi){fi=mul(gi,k_v);});
}
template<std::integral T>void pow_c1(u32*f,const u32*g,idt n,T k){
    ln(f,g,n);
    auto h=alc(n);
    mulk(h,in(sf_md<M>(k)),f,n),exp(f,h,n),fre(h);
}

namespace ntt_op{
using namespace raw_ntt;
sim_inf_seq<vec> 
sim_wn=[](u32*f,idt l,idt r){
	for(idt i=std::max<idt>(1,l);i<r;i*=2){
		Ci(f+i,rt1[std::__lg(i)+1],i);
	}
},
bv_wn=[](u32*f,idt l,idt r){
	if(l==0){f[0]=R,l=1;}
	for(auto i=l;i<r;i*=2){mulk(f+i,rt1[std::__lg(i)+1],f,i);}
},
bv_wi2=[](u32*f,idt l,idt r){
    if(l==0){f[0]=R,l=1;}
	for(auto i=l;i<r;i*=2){mulk(f+i,rt1_I[std::__lg(i)+2],f,i);}
};
template<bool strict=false>inline void dif2(u32*f,idt l){
	cpy(f+l,f,l),dit(f+l,l),dot(f+l,sim_wn.raw()+l,l),dif<strict>(f+l,l);
}
constexpr u32 Two=in(2);
template<bool strict=false>inline void dif2_c1(u32*f,idt l){
	cpy(f+l,f,l),dit(f+l,l),dot(f+l,sim_wn.raw()+l,l),f[l]=sub(Two,f[l]),dif<strict>(f+l,l);
}
template<bool strict=false>inline void dif2_xn(u32*f,idt l){
	cpy(f+l,f,l),dit(f+l,l),dot(f+l,sim_wn.raw()+l,l),f[l]=sub(f[l],Two),dif<strict>(f+l,l);
}
void rev_dif(u32*f,const u32*g,idt l){
	dot(f,bv_wn.raw(),g,l);
	for(idt i=2;i<l;i*=2){rev(f+i,i);}
}
idt locate_wn(u32 w){
	if(shrk(w)==R){return 0;}
	auto res=locate_wn(mul(w,w))*2;
	return res+(shrk(w)!=shrk(bv_wn.v[res]));
}
void dot_neg(u32*f,const u32*g,idt lm){
    idt i=0;
    for(;i+7<lm;i+=8){x8(f+i)=mul(x8(f+i),shuffle<0xb1>(x8(g+i)));}
    for(u32 x={},y={};i+1<lm;i+=2){x=g[i],y=g[i+1],f[i]=mul(f[i],y),f[i+1]=mul(f[i+1],x);}
    if(i!=lm){f[i]=mul(f[i],g[i]);}
}
void dift(u32*f,idt l,idt t){
    if(t>l){
        cpy(f+l,f,l),dit(f+l,l);
        for(idt tt=t/2;tt>=l;tt/=2){dot(f+tt,f+l,sim_wn.raw()+tt,l),clr(f+tt+l,tt-l),dif(f+tt,tt);}
    }
}
template<std::integral T>void dif_xk(u32*f,u32 c,idt l,T k){
    P_R_Tab W(sf_qpw(_g,k));
    f[0]=c;
    for(idt i=1;i<l;i*=2){mulk(f+i,W[std::__lg(i)+1],f,i);}
}
}//namespace ntt_op
inline u32 eval(u32 x,const u32*f,idt n){
	auto xn=R,res=E;
	for(idt i=0;i<n;++i){res=add(res,mul(xn,f[i])),xn=mul(xn,x);}
	return res;
}
void eval(u32*res,const u32*f,const u32*o,idt n,idt m){
	if(std::min(n,m)<=16){
		for(idt i=0;i<m;++i){res[i]=eval(o[i],f,n);}
		return;
	}
	using namespace ntt_op;
	u32*GG[lml];
	idt lm=bcl(std::max(n,m)),lm2=lm*2,m2=0;
	int lgn=std::__lg(lm);
	auto buf=alc(lm*3),pwh=alc(m),nw=GG[0]=alc(m*2),lt=nw;
	cdif(buf,f,n,lm),bv_wn.rsv(lm);
	vec_op(pwh,o,m,[lgn](auto&fi,auto&gi){
		fi=gi;
		for(int i=0;i<lgn;++i){fi=mul(fi,fi);}
		fi=shrk(sub(vec_v{R},fi));
	});
	for(idt i=0;i<m;++i){
		if(pwh[i]){nw[m2]=sub(R,o[i]),nw[m2|1]=sub(o[i],nR),m2+=2;}
        else{res[i]=buf[locate_wn(o[i])];}
	}
	if(m2>32){
		rev_dif(buf+lm,buf,lm),sim_wn.rsv(lm);
		for(int dep=1;dep<lgn;++dep,lt=nw){
			idt t=idt(1)<<dep,t2=t*2,l=0,r=t;
			nw=GG[dep]=alc((m2+t2-1)&-t2);
			for(;r<m2;l+=t2,r+=t2){dot(nw+l,lt+l,lt+r,t),dif2_c1(nw+l,t);}
			if(l<m2){cpy(nw+l,lt+l,t),dif2(nw+l,t);}
		}
		if(m2<=lm){cpy(buf,lt,lm);}else{dot(buf,lt,lt+lm,lm);}
		multi_iv(buf+lm2,buf,lm),dot(buf+lm,buf+lm2,lm);
        if(m2>lm){dot(buf,buf+lm,lt+lm,lm),dot(buf+lm,lt,lm),dit(buf,lm),dit(buf+lm,lm);}
        else{cpy(buf,buf+lm,lm),dit(buf,lm);}
		for(int dep=lgn-1;lt=GG[dep-1],dep>0;--dep){
			idt t=idt(1)<<dep,t2=t*2,l=0,r=t,mid=t/2;
			for(;r<m2;l+=t2,r+=t2){
                dif(buf+r,t),dot(buf+l,buf+r,lt+r,t),dot(buf+r,lt+l,t),dit(buf+l,t),dit(buf+r,t);
            }
			if(l<m2){
                cpy(buf+l+mid,buf+r+mid,mid);
            }
		}
		for(idt i=0,j=1;i<m;++i){if(pwh[i]){res[i]=mul(pwh[i],buf[j]),j+=2;}}
        for(int dep=1;dep<lgn;++dep){fre(GG[dep]);}
	}
	else{
		for(idt i=0;i<m;++i){if(pwh[i]){res[i]=eval(o[i],f,n);}}
	}fre(buf),fre(pwh),fre(GG[0]);
}
void intpol(u32*f,const u32*x,const u32*y,idt n){
	if(n==1){
		*f=*y;
		return;
	}
	using namespace ntt_op;
	u32*GG[lml];
	idt lm=bcl(n),lm2=lm*2,n2=n*2;
	int lgn=std::__lg(lm);
	sim_wn.rsv(lm);
	auto nw=GG[0]=alc(n2),lt=nw,buf=alc(lm*3);
	for(idt i=0;i<n;++i){nw[i*2]=sub(R,x[i]),nw[i*2+1]=sub(nR,x[i]);}
	for(int dep=1;dep<lgn;++dep,lt=nw){
		idt t=idt(1)<<dep,t2=t*2,ed=n2&-t2,i=0;
		nw=GG[dep]=alc((n2+t2-1)&-t2);
		for(;i<ed;i+=t2){dot(nw+i,lt+i,lt+i+t,t),dif2_xn(nw+i,t);}
		if(i<n2){((n2-i)>t)?dot(nw+i,lt+i,lt+i+t,t):((void)cpy(nw+i,lt+i,t));dif2(nw+i,t);}
	}
	dot(buf,lt,lt+lm,lm),dit(buf,lm);
	if(n==lm){buf[lm]=R,buf[0]=sub(buf[0],R);}
	deriv(buf+lm2,buf,n+1),rev(buf,n+1),rev(buf+lm2,n);
	quo(buf+lm2,buf+lm2,buf,n),clr(buf+lm,lm-n),rcpy(buf+lm2-n,buf+lm2,n);
	for(int dep=lgn;dep>0;--dep){
		lt=GG[dep-1];
		idt t=idt(1)<<dep,t2=t*2,l=0,r=t,mid=t/2;
		for(;r<n2;l+=t2,r+=t2){dif(buf+r,t),dot(buf+l,buf+r,lt+r,t),dit(buf+l,t),dot(buf+r,lt+l,t),dit(buf+r,t);}
		if(l<n2){cpy(buf+l+mid,buf+r+mid,mid);}
	}
	for(idt i=0;i<n;++i){buf[i]=buf[i*2+1];}
	multi_iv(buf+lm2,buf,n);
	for(idt i=0;i<n;++i){buf[i*2]=buf[i*2+1]=mul_s(buf[i|lm2],y[i]);}
	for(int dep=1;lt=GG[dep-1],dep<lgn;++dep){
		idt t=idt(1)<<dep,t2=t*2,l=0,r=t;
		for(;r<n2;l+=t2,r+=t2){adddot(buf+l,lt+r,buf+r,lt+l,t),dif2<true>(buf+l,t);}
		if(l<n2){dif2<true>(buf+l,t);}
	}
	adddot(buf,lt+lm,buf+lm,lt,lm),dit(buf,lm),cpy(f,buf,n),fre(buf);
    for(int dep=0;dep<lgn;++dep){fre(GG[dep]);}
}
void taylor(u32*f,u32 c,const u32*g,idt n){
    auto lm=bcl(n*2);
    auto o=alc(lm*2),h=o+lm;
    dot(o,g,Fac.rsv(n),n),rev(o,n),clr(o+n,lm-n);
    Ci(h,c,n),dot(h,IFac.rsv(n),n),clr(h+n,lm-n);
    conv(o,h,lm),rcpy(f,o,n),dot(f,IFac.rsv(n),n),fre(o);
}
void chirpZ(u32*f,u32 a,u32 r,const u32*g,idt m,idt n){
    if(out(r)==0){
        f[0]=eval(a,g,n),std::fill(f+1,f+m,g[0]);
        return;
    }
    idt u=n+m-1,lm=bcl(u);
    u32 ir=inv(r),pr=a,fx=mul(a,ir);
    auto A=alc(lm),B=alc(lm);
    for(idt i=1;i<n;++i){A[lm-i]=mul(g[i],pr),pr=mul(pr,fx),fx=mul(fx,ir);}
    A[0]=g[0],clr(A+1,lm-n),pr=R,fx=R;
	for(idt i=0;i<u;++i){B[i]=pr,pr=mul(pr,fx),fx=mul(fx,r);}
    clr(B+u,lm-u),conv(A,B,lm),pr=R,fx=R;
    for(idt i=0;i<m;++i){f[i]=mul(A[i],pr),pr=mul(pr,fx),fx=mul(fx,ir);}
    fre(A),fre(B);
}
void ichirpZ(u32*f,u32 a,u32 r,const u32*g,idt n){
    if(n==1){
        *f=*g;
        return;
    }
    idt u=n+n-1,lm=bcl(u);
    auto z=alc(lm),B=alc(lm);
    u32 ir=inv(r),pr={},fx=r,pws={};
    z[0]=R;
    for(idt i=1;i<n;++i){z[i]=_Amul(fx,nR,z[i-1]),fx=mul(fx,r);}
    pws=_Amul(fx,nR,z[n-1]),fx=qpw(ir,n-2,nR),pr=(n&1)?R:nR,multi_iv(f,z,n);
    z[0]=mul(mul(g[0],pr),f[n-1]),pr=mul(pr,fx);
    for(idt i=1;i<n;++i){
        z[lm-i]=mul(mul(g[i],pr),mul(f[n-i-1],f[i])),pr=mul(pr,fx);
    }
    clr(z+1,lm-n),pr=R,fx=R;
    for(idt i=0;i<u;++i){
        B[i]=pr,pr=mul(pr,fx),fx=mul(fx,r);
    }
    clr(B+u,lm-u),conv(z,B,lm),pr=R,fx=R;//inv(a)
    for(idt i=0;i<n;++i){
        z[i]=mul(z[i],pr),pr=mul(pr,fx),fx=mul(fx,ir);
    }
    clr(z+n,lm-n);
    pr=neg(pws),fx=neg(r),B[0]=R;
    for(idt i=1;i<n;++i){
        B[i]=mul(pr,mul(f[i],f[n-i]));
        pr=mul(pr,fx),fx=mul(fx,r);
    }
    clr(B+n,lm-n),conv(z,B,lm),pr=R,fx=inv(a);
    for(idt i=0;i<n;++i){f[i]=mul(z[n-i-1],pr),pr=mul(pr,fx);}
    fre(z),fre(B);
}
u32 __quo_at(u32*p,u32*q,idt lm,idt k){
    using namespace ntt_op;
    auto iv=R;
    auto hm=lm/2;
    auto wi=bv_wi2.rsv(hm);
    sim_wn.rsv(lm);
    for(;k;k/=2,iv=mul(iv,Half)){
        dot_neg(p,q,lm),dot_neg(q,q,lm);
        if(k&1){for(idt i=0;i<hm;++i){p[i]=_Smul(p[i*2],p[i*2+1],wi[i]),q[i]=q[i*2];}}
        else{for(idt i=0;i<hm;++i){p[i]=add(p[i*2],p[i*2+1]),q[i]=q[i*2];}}
        if(k<hm){lm=hm,hm/=2,dit(p,lm),dit(q,lm),clr(p+hm,hm),clr(q+hm,hm),dif(p,lm),dif(q,lm);}
        else{dif2(p,hm),dif2(q,hm);}
    }
    return mul(dvs(p[0],q[0]),iv);
}
void __inv_range(u32*f,const u32*g,idt n,idt l,idt r){
    using namespace ntt_op;
    auto lm=bcl(n),lmrl=bcl(r-l),llm=std::max(lm,lmrl),llm2=llm*2;
    if(l<n || r<=n*2){
        auto gg=alc(std::max(r,lm));
        auto t=std::min(n,r);
        cpy(gg,g,lm),dit(gg,lm),clr(gg+t,r-t),inv(f,gg,r);
        mmv(f,f+l,r-l),clr(f+r-l,llm-(r-l)),dif(f,llm),fre(gg);
        return;
    }
    auto p=alc(llm2);
    for(idt i=0;i<lm;++i){p[i]=mul(g[i*2],g[i*2+1]);}
    auto ll=(l-n+2)/2,rr=(r+1)/2,nn=rr-ll,lmrrll=std::max(bcl(nn),lm);
    dif2(p,lm),__inv_range(f,p,n,ll,rr);
    for(auto i=lmrrll-1;~i;--i){f[i*2]=f[i*2+1]=f[i];}
    dift(f,lmrrll*2,llm2),cpy(p,g,lm*2),dift(p,lm*2,llm2),dot_neg(f,p,llm2);
    dif_xk(p,Half,llm2,llm+ll*2-l),dot(f,p,llm2),dit(f+llm,llm),Ci(p,rt1_I[std::__lg(llm2)],llm);
    dot(f+llm,p,llm),dif(f+llm,llm),sub(f,f,f+llm,llm),fre(p);
}
u32 quo_at(const u32*f,const u32*g,idt n,idt m,idt k){
    assert(n<m);
    auto lm=bcl(m),lm2=lm*2;
    auto df=alc(lm2),dg=alc(lm2);
    cpy(df,f,n),clr(df+n,lm2-n),cpy(dg,g,m),clr(dg+m,lm2-m);
    u32 res={};
    if(k<lm){quo(df,df,dg,k+1),res=df[k];}
    else{dif(df,lm2),dif(dg,lm2),res=__quo_at(df,dg,lm2,k);}
    return fre(df),fre(dg),res;
}
void quo_range(u32*f,const u32*g,const u32*h,idt n,idt m,idt l,idt r){
    assert(n<m);
    auto ll=l-m+1,rr=r,u=rr-ll,llm=bcl(u),lm=bcl(m);
    if(l<m||r<llm){
        auto p=alc(r),q=alc(r);
        idt s=std::min(n,r),t=std::min(m,r);
        cpy(p,g,s),clr(p+s,r-s),cpy(q,h,t),clr(q+t,r-t),quo(p,p,q,r),cpy(f,p+l,r-l);
        fre(p),fre(q);
        return;
    }
    auto p=alc(llm*2),q=alc(std::max(lm*2,llm));
    cdif(q,h,m,lm*2),ntt_op::sim_wn.rsv(llm*2);
    __inv_range(p,q,m,ll,rr),cdif(q,g,n,llm);
    dot(q,p,llm),dit(q,llm),cpy(f,q+m-1,r-l),fre(p),fre(q);
}
#ifndef NDEBUG
struct reporter{
    ~reporter(){
        if(!__p_st.empty()){std::cerr<<"memory leak!\n";}
        std::clog<<"ntt_len : "<<raw_ntt::ntt_len<<"\n";
    }   
}__rp;
#endif
void cut_string(){
    _Exit(0);
}
}
using std::cin;
using std::cout;
void solve(){
	using namespace No_Poly;
    idt N;
    cin>>N;
    u32 a,r;
    scanp(&a),scanp(&r);
    if(N==0){return;}
    auto y=alc(N),f=alc(N);
    scanp(y,N),ichirpZ(f,a,r,y,N),printp(f,N);
    fre(y),fre(f);
}
int main(){
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
    solve();
    return 0;
}