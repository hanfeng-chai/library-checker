#include<iostream>
#include<algorithm>
#include<functional>

using i32 = int;
using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;

#define IL __inline__ __attribute__((always_inline))
#define RC(T, x) reinterpret_cast<T>(x)

/*
一个高性能的多项式实现
作者为 QedDust413 & killer_joke
*/

namespace Setting{
	constexpr u32 mod = 998244353;
	constexpr int sta_l_MB = 64;
	constexpr int Detail = 1;
}

namespace __qed{
	constexpr u32 primitive_root(u32 M)
	{
		u32 ed = 0, n = M - 1, d[11] = {};
		for (int i = 2; i * i <= n; i++){
			if (n % i == 0){
				d[ed++] = i;
				do{n /= i;}while (n % i == 0);
			}
		}
		if (n > 1){d[ed++] = n;}
		for (u32 g = 2, r = 0; ; ++g){
			for (u32 i = 0; i < ed; ++i){
				u32 b = (M - 1) / d[i], a = g;
				for(r = 1; b; b >>= 1, a = u64(a) * a % M){
					if(b & 1){r = u64(r) * a % M;}
				}
				if (r == 1){break;}
			}
			if(r != 1){return g;}
		}	
	}
	constexpr int bceil(int x){return 1 << (32 - __builtin_clz(x - 1));}
	constexpr int bit_up(int x){return 1 << (32 - __builtin_clz(x));}
	constexpr int cro_32(int x){return __builtin_ctz(~x);}
	constexpr bool ispow2(u64 x){return (x & (x - 1)) == 0;}
}

namespace Stat_Info{
	using Setting::Detail;
	i64 ntt_size;
	i64 fill0_size, copy_size, rev_size;
	size_t max_cost_sta_l;
	constexpr const char* Author = "QedDust413 & killer_joke";
	constexpr const char* Thanks = "negiizhao, chaihf, rogeryoungh, Qdedawsd2233, bh1234666, yosupo, Pulsating_Dust, KKKKa, qdc, and more.";
	template<typename Tf = std::ostream>void report(Tf &outf = std::clog){
		outf << '\n';
		if constexpr(Detail <= 0){outf << "Statistics are turned off.\n";}
		if constexpr(Detail > 0){outf << "ntt_size:" << ntt_size << "\n";}
		if constexpr(Detail > 1){outf << "fill0_size:" << fill0_size << " copy_size:" << copy_size << " rev_size:" << rev_size << "\nmax_cost_sta:" << (double(max_cost_sta_l) / double(1 << 20)) << "\n";}
		outf.flush();
	}
}

namespace mem_helper{
	using namespace __qed;
	template<size_t align>void *to_align(void *mem){
		static_assert(ispow2(align) && (align != 0));
		return RC(void*, (RC(u64, mem) + (align - 1)) & (-align));
	}
	template<size_t align>bool is_align(const void* mem){
		static_assert(ispow2(align) && (align != 0));
		return (RC(u64, mem) & (align - 1)) == 0;
	}
	using Setting::sta_l_MB;
	char _mem[sta_l_MB << 20];
	void *now = _mem;
	struct pre_aloc{
		void *t;
		pre_aloc(){t = now;}
		~pre_aloc(){now = t;}
	};
	template<typename T, size_t align>T *AlocS(size_t n){
		T *r = RC(T*, to_align<align>(now));
		now = r + n;
        if constexpr(Stat_Info::Detail > 1){
            Stat_Info::max_cost_sta_l = std::max<size_t>(RC(char*, now) - _mem, Stat_Info::max_cost_sta_l);
        }
        return r;
	}
	template<size_t align>void *_alocH(size_t n){
		constexpr size_t ptr_size = sizeof(void*), Extra = (align + ptr_size);
		void *base = std::malloc(n + Extra), *p = to_align<align>(RC(char*, base) + ptr_size);
		return RC(void**, p)[-1] = base, p;
	}
	template<typename T, size_t align>T *AlocH(size_t n){
		return RC(T*, _alocH<align>(n * sizeof(T)));
	}
	void Freed(void* p){
		std::free(RC(void**, p)[-1]);
	}
	template<typename T, size_t align>struct Trivial_Aligned_Allocator{
		static_assert(std::is_trivial<T>::value);
      	typedef T value_type;
		T *allocate(std::size_t n) {return AlocH<T, align>(n);}
    	template <class T1>struct rebind {using other = Trivial_Aligned_Allocator<T1, align>;};
    	void deallocate(T *p, std::size_t n) {Freed(p);}	
	};
}

namespace Montgo{
	//Author : QedDust413
	struct Mont32{
		u32 Mod, Mod2, Inv, NInv, R2;
		constexpr Mont32(u32 n):Mod(n), Mod2(n << 1), Inv(1), NInv(), R2((-u64(n)) % n){
			for (int i = 0; i < 5; ++i){Inv *= 2 - n * Inv;}NInv = -Inv;
		}
		constexpr IL u32 reduce (u64 x)const{return (x + u64(u32(x) * NInv) * Mod) >> 32;}
		constexpr IL u32 reduce_s (u64 x)const{
			u32 r = (x >> 32) - ((u64(u32(x) * Inv) * Mod) >> 32);
			return r >> 31 ? r + Mod : r;
		}
		constexpr IL u32 mul(u32 x, u32 y)const{return reduce(u64(x) * y);}
		constexpr IL u32 mul_s(u32 x, u32 y)const{return reduce_s(u64(x) * y);}
		constexpr IL u32 In(u32 x)const{return mul(x, R2);}
		constexpr IL u32 In_s(u32 x)const{return mul_s(x, R2);}
		constexpr IL u32 Out(u32 x)const{
			u32 r = (x + (u64(x * NInv) * Mod)) >> 32;
			return __builtin_expect(r < Mod, 1) ? r : r - Mod;
		}
	};
}

namespace field_Z{
	//Author : QedDust413
	using Setting::mod;
	constexpr Montgo::Mont32 Space(mod);
	constexpr u32 mod2 = Space.Mod2;
	constexpr IL u32 shrink(u32 x){return x >= mod ? x - mod : x;}
	constexpr IL u32 dilate2(u32 x){return x >> 31 ? x + mod2 : x;}
	using Z = u32;
	constexpr bool isgood(Z x){return x < mod2;}
	constexpr IL Z InZ(u32 x){return Space.In(x);}
	constexpr IL Z InZs(u32 x){return Space.In_s(x);}
	constexpr Z zero_Z(0), one_Z(InZs(1)), not_exist_Z(-1);
	constexpr IL u32 OutZ(Z x){return Space.Out(x);}
	constexpr bool equalZ(Z x, Z y){return shrink(x) == shrink(y);}
	namespace calc{
		constexpr IL Z addZ(Z x, Z y){return dilate2(x + y - mod2);}
		constexpr IL Z subZ(Z x, Z y){return dilate2(x - y);}
		constexpr IL Z mulZ(Z x, Z y){return Space.mul(x, y);}
		constexpr Z powZ(Z a, u32 b, Z r = one_Z){
			for(; b; a = mulZ(a, a), b >>= 1){if(b & 1){r = mulZ(r, a);}}
			return r;
		}
		constexpr IL Z invZ(Z x){return powZ(x, mod - 2);}
		constexpr IL Z divZ(Z x, Z y){return powZ(y, mod - 2, x);}
		template<bool strict = true>constexpr IL Z mulZs(Z x, Z y){
			if constexpr(strict){return Space.mul_s(x, y);}
			return mulZ(x, y);
		}
		constexpr Z negZ(Z x){return (!x - 1) & (mod2 - x);}
		namespace extend_{
			constexpr Z absZ(Z x){
				u32 r = OutZ(x);
				return InZs(std::min(r, mod - r));
			}
			constexpr Z legendre(Z x){return powZ(x, (mod - 1) >> 1);}
			constexpr bool isQR(Z x){return equalZ(legendre(x), one_Z);}
			constexpr Z sqrtZ(Z x){
				if(!isQR(x)){return not_exist_Z;}
				Z a(1), I_mul(0);
				while(isQR(I_mul = subZ(mulZ(a, a), x))){++a;}
				struct dZ{
					Z r, i;
					constexpr void Mul(dZ d, Z I_){*this = {addZ(mulZ(r, d.r), mulZ(mulZ(I_, i), d.i)), addZ(mulZ(r, d.i), mulZ(i, d.r))};}
				};
				dZ s = {a, one_Z}, r = {one_Z, zero_Z};
				for(u32 b = (mod + 1) >> 1; b; b >>= 1, s.Mul(s, I_mul)){if(b & 1){r.Mul(s, I_mul);}}
				return absZ(r.r);
			}
		}
	}
	template<int fixes, bool strict = true>constexpr Z trans(Z x){
		constexpr Z o = fixes > 0 ? calc::powZ(Space.R2, fixes) : calc::powZ(1, -fixes);
		return calc::mulZs<strict>(x, o);
	}
	template<bool strict = true>constexpr Z trans(Z x, int fixes){
		return calc::mulZs<strict>(x, fixes > 0 ? calc::powZ(Space.R2, fixes) : calc::powZ(1, -fixes));
	}
	namespace Const{
		constexpr Z _half = InZs((mod + 1) >> 1), _neghalf = InZs((mod - 1) >> 1), neg_one = InZs(mod - 1);
		constexpr Z img = calc::extend_::sqrtZ(neg_one), imgI = mod - img, _imghalf = calc::mulZs(_half, img);
	}
}
#pragma GCC target("avx2")
#include <immintrin.h>
#define VEC(sz, T) __attribute((vector_size(sz))) T
namespace SIMD{
	using i32x8 = VEC(32, i32);
	using u32x8 = VEC(32, u32);
	using i64x4 = VEC(32, i64);
	using u64x4 = VEC(32, u64);
	using I256 = __m256i;
	constexpr IL u32x8 setu32x8(u32 x){return (u32x8){x, x, x, x, x, x, x, x};}
	template<int typ>IL u32x8 shuffle(const u32x8 &x){return RC(u32x8, _mm256_shuffle_epi32(RC(I256, x), typ));}
	template<int typ>IL u32x8 blend(const u32x8 &x, const u32x8 &y){return RC(u32x8, _mm256_blend_epi32(RC(I256, x), RC(I256, y), typ));}
	IL I256 swaplohi128(const I256 &x){return _mm256_permute2x128_si256(x, x, 1);}	
	IL u32x8& x8(u32 *data){return *RC(u32x8* ,data);}
	IL const u32x8& x8(const u32 *data){return *RC(const u32x8*, data);}
	IL I256 loadu(const void* data){return _mm256_loadu_si256(RC(const __m256i_u*, data));}
	IL void storeu(const I256 &x, void* data){return _mm256_storeu_si256(RC(__m256i_u*, data), x);}
	IL u64x4 fus_mul(const u32x8 &x, const u32x8 &y){return RC(u64x4, _mm256_mul_epu32(RC(I256, x), RC(I256, y)));}
}
namespace field_Z{
	using SIMD::x8;
	using SIMD::u32x8;
	using SIMD::setu32x8;
	using Zx8 = u32x8;
	constexpr u32x8 modx8 = setu32x8(mod), mod2x8 = setu32x8(mod2), NInvx8 = setu32x8(Space.NInv);
	constexpr Zx8 one_Zx8 = setu32x8(one_Z), zerox8 = setu32x8(0u);
	IL Zx8 dilate2x8(const Zx8 &x){return x + (RC(Zx8, RC(SIMD::i32x8, x) < RC(SIMD::i32x8, zerox8)) & mod2x8);}
	IL Zx8 shrinkx8(const Zx8 &x){return x - ((x >= modx8) & modx8);}
	namespace calc{
		//Author : killer_joke
		using namespace SIMD;
		IL Zx8 addZx8(const Zx8 &x, const Zx8 &y){return dilate2x8(x + y - mod2x8);}
		IL Zx8 subZx8(const Zx8 &x, const Zx8 &y){return dilate2x8(x - y);}
		IL Zx8 mulZx8(const Zx8 &x, const Zx8 &y){
			u32x8 z = (NInvx8 * x * y);
			return blend<0xaa>(RC(u32x8, (fus_mul(x, y) + fus_mul(z, modx8)) >> 32), RC(u32x8, (fus_mul(u32x8(u64x4(x) >> 32), u32x8(u64x4(y) >> 32)) + fus_mul(shuffle<0xf5>(z), modx8))));
		}
		IL Zx8 powZx8(const Zx8 &y, u32 b, const Zx8 &_r = one_Zx8){
			Zx8 x = y, r = _r;
			for(; b; x = mulZx8(x, x), b >>= 1){if(b & 1){r = mulZx8(r, x);}}
			return r;
		}
		IL Zx8 invZx8(const Zx8 &x){return powZx8(x, mod - 2);}
		IL Zx8 divZx8(const Zx8 &x, const Zx8 &y){return powZx8(y, mod - 2, x);}
		template<bool strict = true>IL Zx8 mulZsx8(const Zx8 &x, const Zx8 &y){
			if constexpr (strict){
				u32x8 z = (NInvx8 * x * y);
				z = blend<0xaa>(RC(u32x8, (fus_mul(x, y) + fus_mul(z, modx8)) >> 32), RC(u32x8, (fus_mul(u32x8(u64x4(x) >> 32), u32x8(u64x4(y) >> 32)) + fus_mul(shuffle<0xf5>(z), modx8)))) - modx8;
				return z + (RC(Zx8, RC(i32x8, z) < RC(i32x8, zerox8)) & modx8);
			}
			return mulZx8(x, y);
		}
		IL Zx8 negZx8(const Zx8 &x){return (x != zerox8) & (mod2x8 - x);}
		IL Zx8 _AmulZx8(const Zx8 &a, const Zx8 &b, const Zx8 &c){return mulZx8(a + b, c);}
		IL Zx8 _SmulZx8(const Zx8 &a, const Zx8 &b, const Zx8 &c){return mulZx8(a - b + mod2x8, c);}
	}
}
#undef VEC

#define STATI(ifo, dt, num) if constexpr (Stat_Info::Detail > dt){Stat_Info::ifo += (num);}
namespace poly_base{
	using namespace field_Z;
	using __qed::bit_up;
	void fl0(Z *f, int l){STATI(fill0_size, 1, l)std::fill_n(f, l, zero_Z);}
	void fl0(Z *bg, Z *ed){STATI(fill0_size, 1, ed - bg)std::fill(bg, ed, zero_Z);}
	void Cpy(const Z *f, int l, Z *g){STATI(copy_size, 1, l)std::copy_n(f, l, g);}
	void Cpy(const Z *bg, const Z *ed, Z *g){STATI(copy_size, 1, ed - bg)std::copy(bg, ed, g);}
	void rev(Z *bg, Z *ed){STATI(rev_size, 1, ed - bg)std::reverse(bg, ed);}
	void rev(Z *f, int l){STATI(rev_size, 1, l)std::reverse(f, f + l);}
	void Cpy_fl0(const Z *f, int n, Z *g, int lim){Cpy(f, n, g), fl0(g + n, g + lim);}
	void Cpy_rev(const Z *f, int l, Z *g){STATI(rev_size, 1, l)STATI(copy_size, 1, l)std::reverse_copy(f, f + l, g);}
	#define flx(nam, opt) void nam(const Z *A, int l, Z *B, Z t){int i=0;if(l>16){Zx8 tx8=setu32x8(t);for(;i+7<l;i+=8){x8(B+i)=calc::opt##Zx8(x8(A+i),tx8);}}for(;i<l;++i){B[i]=calc::opt##Z(A[i],t);}}
	flx(mul_t_s, mul)
	flx(add_t_s, add)
	flx(sub_t_s, sub)
	#undef flx
	#define flx(nam, opt) void nam(Z *A,int l,const Z *B){int i=0;for(;i+7<l;i+=8){x8(A + i)=calc::opt##Zx8(x8(A+i),x8(B+i));}for(;i<l;++i){A[i]=calc::opt##Z(A[i],B[i]);}}  void nam(const Z *A,const Z *B,int l,Z *C){int i=0;for(;i+7<l;i+=8){x8(C+i)=calc::opt##Zx8(x8(A+i),x8(B+i));}for(;i<l;++i){C[i]=calc::opt##Z(A[i],B[i]);}}
	flx(dot, mul)
	flx(addP, add)
	flx(subP, sub)
	#undef flx
	void NegP(const Z *A, int l, int r, Z *B){
		int i = l;
		if(r - l >= 16){
			for(; i & 7; ++i){B[i] = calc::negZ(A[i]);}
			for(; i + 7 < r; i += 8){x8(B + i) = calc::negZx8(x8(A + i));}	
		}
		for(; i < r; ++i){B[i] = calc::negZ(A[i]);}
	}
}

namespace poly_base{
	using mem_helper::pre_aloc;
	using mem_helper::Freed;
	const std::function<Z*(size_t)> alocS = mem_helper::AlocS<Z, 32>;
	const std::function<Z*(size_t)> alocH = mem_helper::AlocH<Z, 32>;
	namespace cal_helper{
		using namespace calc;
		constexpr Zx8 eight_Zx8 = setu32x8(InZs(8));
		Z *_iv = nullptr, *_i = nullptr;
		int sz_iv = 0;
		void precal(int n){
			if(n > sz_iv){
				n = ((n + 7) >> 3) << 3;
				{
					Z *nxtiv = alocH(n), *nxti = alocH(n);
					Cpy(_iv, sz_iv, nxtiv), Cpy(_i, sz_iv, nxti);
					if(_iv != nullptr){Freed(_iv), Freed(_i);}
					_iv = nxtiv, _i = nxti;
				}
				Zx8 *_ix8 = RC(Zx8*, _i), *_ivx8 = RC(Zx8*, _iv);
				if(sz_iv == 0){
					for(int i = 0; i < 8; ++i){_i[i] = InZs(i);}
					*_ivx8 = invZx8(*_ix8), sz_iv = 8;
					if(n == 8){return ;}
				}
				int L = sz_iv >> 3, R = n >> 3;
				for(int i = L; i < R; ++i){_ix8[i] = addZx8(_ix8[i - 1], eight_Zx8);}
				_ivx8[L] = _ix8[L];
				for(int i = L + 1; i < R; ++i){_ivx8[i] = mulZx8(_ivx8[i - 1], _ix8[i]);}
				_ivx8[R - 1] = invZx8(_ivx8[R - 1]);
				for(int i = R - 1; i > L; --i){
					Zx8 ivix8 = _ivx8[i];
					_ivx8[i] = mulZx8(_ivx8[i], _ivx8[i - 1]), _ivx8[i - 1] = mulZx8(ivix8, _ix8[i]);
				}
				sz_iv = n;
			}
		}
		void deriv(const Z *g, int n, Z *f){
			precal(n + 1);
			int i = 0;
			if(n > 7){
				for(; i < 7; ++i){f[i] = mulZ(g[i + 1], _i[i + 1]);}
				for(; i + 7 < n; i += 8){storeu(RC(I256, mulZx8(x8(g + i + 1), x8(_i + i + 1))), f + i);}
			}
			for(; i < n; ++i){f[i] = mulZ(g[i + 1], _i[i + 1]);}
			f[n] = zero_Z;
		}
		void integ(const Z *g, int n, Z *f){
			precal(n + 1);
			int i = n - 1;
			if(i > 7){
				for(; (i & 7) != 6; --i){f[i + 1] = mulZ(g[i], _iv[i + 1]);}
				for(i -= 7; ~i; i -= 8){x8(f + i + 1) = mulZx8(RC(Zx8, loadu(g + i)), x8(_iv + i + 1));}
				i += 7;
			}
			for(; ~i; --i){f[i + 1] = mulZ(g[i], _iv[i + 1]);}
			f[0] = zero_Z;
		}
	}
	using cal_helper::precal;
	using cal_helper::deriv;
	using cal_helper::integ;
}

namespace poly_base{
	namespace Tools{
		int compP(const Z *f, int l, const Z *g){
			for(int i = 0; i < l; ++i){if(!equalZ(f[i], g[i])){return i;}}
			return -1;
		}
		template<int fixes = 1, typename Tf = std::istream>void scanP(Z *f, int l, Tf &inf = std::cin){
			for(int i = 0; i < l; ++i){inf >> f[i], f[i] = trans<fixes, false>(f[i]);}
		}
		template<int fixes = -1, typename Tf = std::ostream>void printP(const Z *f, int l, Tf &outf = std::cout){
			if(l){
				outf << trans<fixes>(f[0]);
				for(int i = 1; i < l; ++i){outf << ' ' << trans<fixes>(f[i]);}
				outf << '\n';
			}
		}
		template<typename T = int, typename lT = long long>T stoi_m(const std::string &s, const T m){
			T r(0);
			for(auto ch:s){r = ((lT)r * 10 + (ch - '0')) % m;}
			return r;
		}
		int clzP(const Z *f, int n){
			int i = 0;
			while(i < n && equalZ(f[i], zero_Z)){++i;}return i;
		}
		int crzP(const Z *f, int n){
			int i = n;
			while(i && equalZ(f[i - 1], zero_Z)){--i;}return n - i;
		}
	}
}

namespace poly_base{
	//Author : QedDust413
	namespace f_n_t_t_base{
		using namespace calc;
		using __qed::cro_32;
		constexpr int base4thre = 16;
		template<bool strict = false>void mul_t(Z *A, int l, Z t){
			for(int j = 0; j < l; ++j){A[j] = mulZs<strict>(A[j], t);}
		}
		constexpr int mp2 = __builtin_ctz(mod - 1);
		constexpr Z _g(InZ(__qed::primitive_root(mod)));
		struct P_R_Tab{
			Z t[mp2 + 1];
			constexpr P_R_Tab(Z G) : t(){
				t[mp2] = shrink(powZ(G, (mod - 1) >> mp2));
				for(int i = mp2 - 1; ~i; --i){t[i] = mulZs(t[i + 1], t[i + 1]);}
			}
			constexpr Z operator [] (int i) const {
				return t[i];
			}
		};
		constexpr P_R_Tab rt1(_g), rt1_I(invZ(_g));
		template<int lim>struct omega_info_base2{
			Z o[lim >> 1];
			constexpr omega_info_base2(const P_R_Tab &Tb) : o(){
				o[0] = one_Z;
				for(int i = 0; (1 << i) < (lim >> 1); ++i){o[1 << i] = Tb[i + 2];}
				for(int i = 1, l = lim >> 1; i < l; ++i){o[i] = mulZ(o[i & (i - 1)], o[i & -i]);}
			}
			constexpr Z operator [] (int i) const {
				return o[i];
			}
		};
		const omega_info_base2<base4thre> w(rt1), wI(rt1_I);
		constexpr Zx8 powXx8(Z X){
			Z X2 = mulZs(X, X), X3 = mulZs(X2, X), X4 = mulZs(X3, X), X5 = mulZs(X4, X), X6 = mulZs(X5, X), X7 = mulZs(X6, X);
			return (Zx8){one_Z, X, X2, X3, X4, X5, X6, X7};
		}
		struct ntt_info_base4x8{
			Z rt3[mp2 - 2], rt3_I[mp2 - 2];
			Zx8 rt4ix8[mp2 - 3], rt4ix8_I[mp2 - 3];
			constexpr ntt_info_base4x8() : rt3(), rt3_I(), rt4ix8(), rt4ix8_I()
			{   
				Z pr = one_Z, pr_I = one_Z;
				for(int i = 0; i < mp2 - 2; ++i){
					rt3[i] = mulZs(pr, rt1[i + 3]), rt3_I[i] = mulZs(pr_I, rt1_I[i + 3]);
					pr = mulZs(pr, rt1_I[i + 3]), pr_I = mulZs(pr_I, rt1[i + 3]);
				}
				pr = one_Z, pr_I = one_Z;
				for(int i = 0; i < mp2 - 3; ++i){
					rt4ix8[i] = powXx8(mulZs(pr, rt1[i + 4])), rt4ix8_I[i] = powXx8(mulZs(pr_I, rt1_I[i + 4]));
					pr = mulZs(pr, rt1_I[i + 4]), pr_I = mulZs(pr_I, rt1[i + 4]);
				}
			}
		};
		template<int typ>IL u32x8 Neg(const u32x8 &x){return blend<typ>(x, mod2x8 - x);}
	}
	//fast_number_theoretic_transform 
	//Author : QedDust413
    namespace f_n_t_t{
		using namespace f_n_t_t_base;
		template<bool strict = false, int fixes = 0>void dif_base2(Z *A, int lim){
			STATI(ntt_size, 0, lim);
			for(int L = lim >> 1, R = lim; L; L >>= 1, R >>= 1){
				for(int i = 0, k = 0; i < lim; i += R, ++k){
					for(int j = 0; j < L; ++j){
						Z x = dilate2(A[i + j] - mod2) , y = mulZ(w[k], A[i + j + L]);
						A[i + j] = x + y, A[i + j + L] = x - y + mod2;
					}
				}
			}
			if constexpr(fixes){mul_t<strict>(A, lim, trans<fixes>(one_Z));}
			else{
				for(int j = 0; j < lim; ++j){
					A[j] = dilate2(A[j] - mod2);
					if constexpr(strict){A[j] = shrink(A[j]);}
				}
			}
		}
		template<bool strict = false, int fixes = 0>void dit_base2(Z *A, int lim){
			STATI(ntt_size, 0, lim);
			for(int L = 1, R = 2; L < lim; L <<= 1, R <<= 1){
				for(int i = 0, k = 0; i < lim; i += R, ++k){
					for(int j = 0; j < L; ++j){
						Z x = A[i + j], y = A[i + j + L];
						A[i + j] = addZ(x, y), A[i + j + L] = mulZ(x - y + mod2, wI[k]);
					}
				}
			}
			mul_t<strict>(A, lim, trans<fixes + 1>(mod - ((mod - 1) / lim)));
		}
		constexpr Zx8 imagx8 = setu32x8(rt1[2]), imag_Ix8 = setu32x8(rt1_I[2]);
		constexpr Z w38 = mulZs(rt1[2], rt1[3]), w38I = mulZs(rt1_I[2], rt1_I[3]);
		constexpr ntt_info_base4x8 iab4;
        template<bool strict = false, int fixes = 0>void dif_base4x8(Z *A, int lim){
			STATI(ntt_size, 0, lim);
			int n = lim >> 3, L = n >> 1;
			Zx8 *f = RC(Zx8*, A);
			if(__builtin_ctz(n) & 1){
				for(int j = 0; j < L; ++j){
					Zx8 x = f[j], y = f[j + L];
					f[j] = x + y, f[j + L] = x - y + mod2x8;
				}
				L >>= 1;
			}
			L >>= 1;
			for(int R = L << 2; L; L >>= 2, R >>= 2){
				Z _r = one_Z, _r2 = one_Z, _r3 = one_Z; 
				for(int i = 0, k = 0; i < n; i += R, ++k){
					Zx8 r = setu32x8(_r), r2 = setu32x8(_r2), r3 = setu32x8(_r3);
					for(int j = 0; j < L; ++j){
						#define F(c) f[i + j + c * L]
						Zx8 f0 = dilate2x8(F(0) - mod2x8), f1 = mulZx8(F(1), r), f2 = mulZx8(F(2), r2), f3 = mulZx8(F(3), r3);
						Zx8 f1f3 = _SmulZx8(f1, f3, imagx8), f02 = addZx8(f0, f2), f13 = addZx8(f1, f3), f_02 = subZx8(f0, f2);
						F(0) = f02 + f13, F(1) = f02 - f13 + mod2x8, F(2) = f_02 + f1f3, F(3) = f_02 - f1f3 + mod2x8;
						#undef F
					}
					_r = mulZs(_r, iab4.rt3[cro_32(k)]), _r2 = mulZs(_r, _r), _r3 = mulZs(_r2, _r);
				}
			}
			{
				constexpr Zx8 _r = setu32x8(trans<fixes>(one_Z)), pr2 = {one_Z, one_Z, one_Z, rt1[2], one_Z, one_Z, one_Z, rt1[2]}, pr4 = {one_Z, one_Z, one_Z, one_Z, one_Z, rt1[3], rt1[2], w38};
				Zx8 r = _r;
				for(int i = 0; i < n; ++i){
					Zx8& fi = f[i];
					fi = mulZx8(fi, r);
					fi = _AmulZx8(Neg<0xf0>(fi), RC(Zx8, swaplohi128(RC(I256, fi))), pr4);
					fi = _AmulZx8(Neg<0xcc>(fi), shuffle<0x4e>(fi), pr2);
					fi = addZx8(Neg<0xaa>(fi), shuffle<0xb1>(fi));
					if constexpr(strict){fi = shrinkx8(fi);}
					r = mulZsx8(r, iab4.rt4ix8[cro_32(i)]);
				}
			}
        }
		template<bool strict = false, int fixes = 0>void dit_base4x8(Z *A, int lim){
			STATI(ntt_size, 0, lim);
			int n = lim >> 3, L = 1;
			Zx8 *f = RC(Zx8*, A);
			{
				constexpr Zx8 pr2 = {one_Z, one_Z, one_Z, rt1_I[2], one_Z, one_Z, one_Z, rt1_I[2]}, pr4 = {one_Z, one_Z, one_Z, one_Z, one_Z, rt1_I[3], rt1_I[2], w38I}; 
				Zx8 r = setu32x8(trans<fixes + 1>(mod - ((mod - 1) / lim)));
				for(int i = 0; i < n; ++i){
					Zx8& fi = f[i];
					fi = _AmulZx8(Neg<0xaa>(fi), shuffle<0xb1>(fi), pr2);
					fi = _AmulZx8(Neg<0xcc>(fi), shuffle<0x4e>(fi), pr4);
					fi = _AmulZx8(Neg<0xf0>(fi), RC(Zx8, swaplohi128(RC(I256, fi))), r);
					r = mulZsx8(r, iab4.rt4ix8_I[cro_32(i)]);
				}
			}
			for (int R = L << 2; L < (n >> 1) ; L <<= 2, R <<= 2){
				Z _r = one_Z, _r2 = one_Z, _r3 = one_Z;
				for(int i = 0, k = 0; i < n; i += R, ++k){
					Zx8 r = setu32x8(_r), r2 = setu32x8(_r2), r3 = setu32x8(_r3);
					for(int j = 0; j < L; ++j){
						#define F(c) f[i + j + c * L]
						Zx8 f0 = F(0), f1 = F(1), f2 = F(2), f3 = F(3);
						Zx8 f2f3 = _SmulZx8(f2, f3, imag_Ix8), f01 = addZx8(f0, f1), f23 = addZx8(f2, f3), f_01 = subZx8(f0, f1);
						F(0) = addZx8(f01, f23), F(1) = _AmulZx8(f_01, f2f3 ,r), F(2) = _SmulZx8(f01, f23, r2), F(3) = _SmulZx8(f_01, f2f3, r3);
						#undef F
					}
					_r = mulZs(_r, iab4.rt3_I[cro_32(k)]), _r2 = mulZs(_r, _r), _r3 = mulZs(_r2, _r);
				}
			}
			if(__builtin_ctz(n) & 1){
				for(int j = 0; j < L; ++j){
					Zx8 x = f[j], y = f[j + L];
					f[j] = addZx8(x, y), f[j + L] = subZx8(x, y);
				}
			}
			if constexpr (strict){for(int i = 0; i < n; ++i){f[i] = shrinkx8(f[i]);}}
		}
		template<bool strict = false, int fixes = 0>void dif(Z *A, int lim){
			lim >= base4thre ? dif_base4x8<strict, fixes>(A, lim) : dif_base2<strict, fixes>(A, lim);
		}
		template<bool strict = false, int fixes = 0>void dit(Z *A,int lim){
			lim >= base4thre ? dit_base4x8<strict, fixes>(A, lim) : dit_base2<strict, fixes>(A, lim);
		}
    }
	using f_n_t_t::dif;
	using f_n_t_t::dit;
	void Conv(Z *f, int lim, Z *g){
		dif(f, lim), dif(g, lim), dot(f, lim, g), dit(f, lim);
	}
}

namespace poly{
	using namespace poly_base;
}

#undef STATI
namespace poly{
	//Author : QedDust413
	void Mul(const Z *f, int n, const Z *g, int m, Z *h){
		int lim = bit_up(n + m);
		pre_aloc mem;
		Z *F = alocS(lim), *G = alocS(lim);
		Cpy_fl0(f, n + 1, F, lim), Cpy_fl0(g, m + 1, G, lim), Conv(F, lim, G), Cpy(F, n + m + 1, h);
	}

	void Inv(const Z *g, int n, Z *f){
		int lim = bit_up(n);
		pre_aloc mem;
		Z *o = alocS(lim), *h = alocS(lim);
		f[0] = calc::invZ(g[0]);
		for(int t = 2, mid = 1; t <= lim; t <<= 1, mid <<= 1){
			int xl = std::min<int>(t, n + 1);
			Cpy_fl0(g, xl, o, t), Cpy_fl0(f, mid, h, t), Conv(o, t, h), fl0(o, mid), dif(o, t), dot(o, t, h), dit(o, t), NegP(o, mid, xl, f);
		}
	}

	void _Div(const Z *g, int n, const Z *f, Z *q){
		if(n == 0){
			return q[0] = calc::divZ(g[0], f[0]), void();
		}
		int lim = bit_up(n) << 1;
		pre_aloc mem;
		Z *o = alocS(lim), *h = alocS(lim);
		Inv(f, n, o), fl0(o + n + 1, o + lim), Cpy_fl0(g, n + 1, h, lim), Conv(h, lim, o), Cpy(h, n + 1, q);
	}

	void Div(const Z *g, int n, const Z *f, Z *q){
		using namespace calc;
		if(n <= 64){return _Div(g, n, f, q);}
		int bn = bit_up(n >> 4), bt = (n / bn) + 1, bn2 = bn << 1;
		pre_aloc mem;
		Z *o = alocS(bn2), *jok = alocS(bn2);
		Inv(f, bn - 1, o), fl0(o + bn, bn), Cpy_fl0(g, bn, jok, bn2), Conv(jok, bn2, o);
		Z *nf = alocS(bn2 * bt), *ng = alocS(bn2 * (bt - 1));
		Cpy(jok, bn, q), Cpy_fl0(f, bn, nf, bn2), dif(nf, bn2);
		for(int bi = 1; bi < bt; ++bi){
			int xl = std::min<int>(bn, n + 1 - bn * bi);
			Cpy_fl0(f + bi * bn, xl, nf + bi * bn2, bn2), dif(nf + bi * bn2, bn2);
			Cpy_fl0(q + (bi - 1) * bn, bn, ng + (bi - 1) * bn2, bn2), dif<true>(ng + (bi - 1) * bn2, bn2), fl0(jok, bn2);
			for(int j = 0; j < bi; ++j){
				Z *nF = nf + (bi - j) * bn2, *nF1 = nF - bn2, *ngj = ng + j * bn2;
				for(int i = 0; i < bn; i += 8){
					x8(jok + i) = subZx8(x8(jok + i), _AmulZx8(x8(nF + i), x8(nF1 + i), x8(ngj + i)));
				}
				for(int i = bn; i < bn2; i += 8){
					x8(jok + i) = subZx8(x8(jok + i), _SmulZx8(x8(nF + i), x8(nF1 + i), x8(ngj + i)));
				}
			}
			dit(jok, bn2), fl0(jok + bn, bn), addP(jok, xl, g + bn * bi), dif(jok, bn2), dot(jok, bn2, o), dit(jok, bn2), Cpy(jok, xl, q + bn * bi);
		}
	}
	
	void Ln(const Z *g, int n, Z *f){
		precal(n + 1), dot(g, cal_helper::_i, n + 1, f), Div(f, n, g, f), dot(f, n + 1, cal_helper::_iv);
	}

	template<bool calc_inv = true>void Expi(const Z *g, int n, Z *f, Z *h){
		f[0] = h[0] = one_Z;
		if(n < 1){return ;}
        int lim = bit_up(n);
		precal(lim);
		pre_aloc mem;
		Z *o = alocS(lim), *A = alocS(lim), *B = alocS(lim);
		A[0] = A[1] = one_Z;
        for(int t = 2, mid = 1; t <= lim; t <<= 1, mid <<= 1){
            int xl = std::min(n + 1, t), dl = std::min(t << 1, lim);
            deriv(g, mid - 1, o), dif(o, mid), dot(o, mid, A), dit(o, mid), deriv(f, mid - 1, o + mid);
			subP(o + mid, mid - 1, o), o[t - 1] = 0, fl0(o, mid - 1), o[mid - 1] = calc::negZ(o[mid - 1]);
            Cpy_fl0(h, mid, B, t), dif(B, t), dif(o, t), dot(o, t, B), dit(o, t);
            integ(o, t - 1, o), fl0(o, mid), subP(o + mid, xl - mid, g + mid);
			dif(o, t), dot(o, t, A), dit(o, t), NegP(o, mid, xl, f);
			if(t != lim || calc_inv){
				Cpy_fl0(f, xl, A, dl), dif(A, dl), dot(A, B, t, o), dit(o, t), fl0(o, mid);
				dif(o, t), dot(o, t, B), dit(o, t), NegP(o, mid, xl, h);
			}
        }
    }

	void Exp(const Z *g, int n, Z *f){
		using namespace calc;
		pre_aloc mem;
		if(n <= 64){
			Z *h = alocS(n + 1);
			return Expi<false>(g, n, f, h);
		}
		int bn = bit_up(n >> 4), bt = (n / bn) + 1, bn2 = bn << 1;
		precal(n + 1);
		Z *h = alocS(bn2);
		Expi(g, bn - 1, f, h);
		fl0(h + bn, bn), dif(h, bn2);
		Z *jok = alocS(bn2), *nf = alocS(bn2 * bt), *ng = alocS(bn2 * (bt - 1));
		dot(g, cal_helper::_i, bn, nf), fl0(nf + bn, nf + bn2), dif(nf, bn2);
		for(int bi = 1; bi < bt; ++bi){
			int xl = std::min<int>(bn, n + 1 - bn * bi);
			dot(g + bi * bn, cal_helper::_i + bi * bn, xl, nf + bi * bn2), fl0(nf + bi * bn2 + xl, bn2 - xl), dif(nf + bi * bn2, bn2);
			Cpy_fl0(f + (bi - 1) * bn, bn, ng + (bi - 1) * bn2, bn2), dif<true>(ng + (bi - 1) * bn2, bn2), fl0(jok, bn2);
			for(int j = 0; j < bi; ++j){
				Z *nF = nf + (bi - j) * bn2, *nF1 = nF - bn2, *ngj = ng + j * bn2;
				for(int i = 0; i < bn; i += 8){
					x8(jok + i) = addZx8(x8(jok + i), _AmulZx8(x8(nF + i), x8(nF1 + i), x8(ngj + i)));
				}
				for(int i = bn; i < bn2; i += 8){
					x8(jok + i) = addZx8(x8(jok + i), _SmulZx8(x8(nF + i), x8(nF1 + i), x8(ngj + i)));
				}
			}
			dit(jok, bn2), fl0(jok + bn, bn), dif(jok, bn2), dot(jok, bn2, h), dit(jok, bn2), fl0(jok + bn, bn);
            dot(jok, xl, cal_helper::_iv + bn * bi), dif(jok, bn2), dot(jok, bn2, ng), dit(jok, bn2), Cpy(jok, xl, f + bn * bi);
		}
	}

	void Pow(const Z *g, int n, int k, Z *f){
		k = ((k % mod) + mod) % mod;
		if(k == 0){f[0] = one_Z, fl0(f + 1, n);}
		else if(k == 1){Cpy(g, n + 1, f);}
		else{
			pre_aloc mem;
			Z *h = alocS(n + 1);
			Ln(g, n, f), mul_t_s(f, n + 1, h, InZ(k)), Exp(h, n, f);
		}
	}

	void Pow(const Z *g, int n, int k, int k2, Z *f){
		if(g[0] != one_Z){
			pre_aloc mem;
			Z *h = alocS(n + 1);
			mul_t_s(g, n + 1, f, calc::invZ(g[0])), Pow(f, n, k, h), mul_t_s(h, n + 1, f, calc::powZ(g[0], ((k2 % (mod - 1)) + (mod - 1)) % (mod - 1)));
		}
		else{Pow(g, n, k, f);}
	}
}

namespace poly{
	constexpr int good_poly = 1, empty_poly = 0, not_exist_poly = -1;
	int Safe_Pow(const Z *g, int n, std::string s, Z *f){
        using namespace Tools;
		int shift = clzP(g, n + 1);
		if(shift){
			if((s.size() > size_t(8)) || ((stoll(s) * shift) > i64(n))){return empty_poly;}
			int rf = (stoi(s) * shift), l = n + 1 - rf;
			pre_aloc mem;
			Z *h = alocS(l);
			Cpy(g + shift, l, f), Pow(f, l - 1, stoi_m(s, mod), stoi_m(s, mod - 1), h), Cpy(h, l, f + rf), fl0(f, rf);
		}
		else{
			Pow(g, n, stoi_m(s, mod), stoi_m(s, mod - 1), f);
		}
		return good_poly;
	}
}

namespace Command{
	void cut_string(){
		_Exit(0);
	}
}

using poly_base::alocS;
using poly_base::pre_aloc;
using namespace poly_base::Tools;
using namespace field_Z;

#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
namespace QIO_base{
	/*
	verison 0.4.1
	Author : QedDust413
	Date : 2023 / 6 / 22
	*/
	constexpr int O_buffer_default_size = 1 << 18;
	constexpr int O_buffer_default_flush_threshold = 40;
	struct _int_to_char_tab{
		char tab[40000];
		constexpr _int_to_char_tab():tab(){
			for(int i=0;i!=10000;++i){
				for(int j=3,n=i;~j;--j){
					tab[i*4+j]=n%10+48,n/=10;
				}
			}
		}
	}constexpr _otab;
}
namespace QIO_I {
	using namespace QIO_base;
	struct Qinf{
		FILE* f;
		char *bg,*ed,*p;
		struct stat Fl;
		Qinf(FILE *fi):f(fi)
		{
			int fd = fileno(f);
			fstat(fd, &Fl);
			bg = (char*)mmap(0, Fl.st_size + 1, PROT_READ,MAP_PRIVATE, fd, 0);
			p = bg, ed = bg + Fl.st_size;
			madvise(p,Fl.st_size + 1, MADV_SEQUENTIAL);
		}
		~Qinf(){
			munmap(bg,Fl.st_size + 1);
		}
		void skip_space(){
			while(*p <= ' '){
				++p;
			}
		}
		char get(){
			return *p++;
		}
		char seek(){
			return *p;
		}
		bool eof(){
			return p == ed;
		}
		Qinf& read(char* s, size_t count){
			return memcpy(s, p, count), p += count, *this;
		}
		Qinf& operator >> (u32 &x){
			skip_space(),x=0;
			for(; *p > ' '; ++p){
				x = x * 10 + (*p & 0xf);
			}
			return *this;
		}
		Qinf& operator >> (int &x){
			skip_space();
			if(*p == '-'){
                for(++p, x = 48 - *p++;*p > ' ';++p){
					x = x * 10 - (*p ^ 48);
				}	
			}
			else{
				for(x = *p++ ^ 48;*p > ' ';++p){
					x = x * 10 + (*p ^ 48);
				}
			}
			return *this;
		}
		Qinf& operator >> (std::string &s){
			skip_space();
			for(; *p > ' '; ++p){
				s += *p;
			}
			return *this;
		}
	}qin(stdin);
}
namespace QIO_O{
	using namespace QIO_base;
	struct Qoutf{
		FILE *f;
		char *bg,*ed,*p;
		char *ed_thre;
		int fp;
		u64 _fpi;
		Qoutf(FILE *fo,size_t sz = O_buffer_default_size):
			f(fo),
			bg(new char[sz]),ed(bg+sz),p(bg),
			ed_thre(ed - O_buffer_default_flush_threshold),
			fp(6),
			_fpi(1000000ull){
		}
		void flush(){
			fwrite_unlocked(bg,1,p - bg,f),p=bg;
		}
		void chk(){
			if(__builtin_expect(p > ed_thre, 0)){
				flush();
			}
		}
		~Qoutf(){
			flush();delete[] bg;
		}
		void put4(u32 x){
			if(x > 99u){
				if(x > 999u){
					memcpy(p,_otab.tab + (x << 2) + 0,4),p += 4;
				}
				else{
					memcpy(p,_otab.tab + (x << 2) + 1,3),p += 3;
				}
			}
			else{
				if(x > 9u){
					memcpy(p,_otab.tab + (x << 2) + 2,2),p += 2;
				}
				else{
					*p++ = x^48;
				}
			}
		}
		void put2(u32 x){
			if(x > 9u){
				memcpy(p,_otab.tab + (x << 2) + 2,2),p += 2;
			}
			else{
				*p++ = x^48;
			}
		}
		Qoutf& write(const char* s,size_t count){
			if(count > 1024 || p + count > ed_thre){
				flush(),fwrite_unlocked(s,1,count,f);
			}
			else{
				memcpy(p,s,count),p+=count,chk();
			}
			return *this;
		}
        Qoutf& operator << (char ch){
            return *p++ = ch, *this;
        }
		Qoutf& operator << (u32 x){
			if(x > 99999999u){
				put2(x / 100000000u),x %= 100000000u;
				memcpy(p,_otab.tab + ((x / 10000u) << 2),4),p += 4;
				memcpy(p,_otab.tab + ((x % 10000u) << 2),4),p += 4;
			}
			else if(x > 9999u){
				put4(x / 10000u);
				memcpy(p,_otab.tab + ((x % 10000u) << 2),4),p += 4;
			}
			else{
				put4(x);
			}
			return chk() ,*this;
		}
		Qoutf& operator << (int x){
			if(x < 0){
				*p++ = '-', x = -x;
			}
			return *this << static_cast<u32>(x);
		}
	}qout(stdout);
}
namespace QIO{
	using QIO_I::Qinf;
	using QIO_I::qin;
	using QIO_O::Qoutf;
	using QIO_O::qout;
}

using namespace QIO;

void solve(){
    int N;
	std::string M;
	qin >> N >> M;
	auto F = alocS(N), G = alocS(N);
	scanP(F, N, qin);
	poly::Safe_Pow(F, N - 1, M, G);
	printP(G, N, qout);
	Stat_Info::report();
}

int main(){
	std::cin.tie(nullptr) -> sync_with_stdio(false);
	solve();
	return 0;
}
/*
Truth.
*/