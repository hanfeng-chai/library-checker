// Resubmit after new tests. Original:
// https://judge.yosupo.jp/submission/354636

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <queue>
using namespace std;

// NOLINTBEGIN
// clang-format off
// DO NOT REMOVE THIS MESSAGE. The mess that follows is a minified build of
// https://github.com/purplesyringa/blazingio. Refer to the repository for
// a human-readable version and documentation.
// Options: cbfoiedrhWLMXaIaAn
#define M$(x,...)_mm256_##x##_epi8(__VA_ARGS__)
#define $u(...)__VA_ARGS__
#if __APPLE__
#define $m(A,B)A
#else
#define $m(A,B)B
#endif
#if _WIN32
#define $w(A,B)A
#else
#define $w(A,B)B
#endif
#if __i386__|_M_IX86
#define $H(A,B)A
#else
#define $H(A,B)B
#endif
#if __aarch64__
#define $a(A,B)A
#else
#define $a(A,B)B
#endif
#define $P(x)void F(x K){
#define $T template<$c T
#define $c class
#define $C constexpr
#define $R return
#define $O operator
#define u$ uint64_t
#define $r $R*this;
#include<array>
#include<bitset>
#include<complex>
#include<cstring>
#include $a(<arm_neon.h>,<immintrin.h>)
#include<stdint.h>
#include $w(<windows.h>,<sys/mman.h>)
#include<sys/stat.h>
#include $w(<io.h>,<unistd.h>)
#include $w(<ios>,<sys/resource.h>)
#if _MSC_VER
#define __builtin_add_overflow(a,b,c)_addcarry_u64(0,a,b,c)
#define $s
#else
$H(,u$ _umul128(u$ a,u$ b,u$*D){auto x=(__uint128_t)a*b;*D=u$(x>>64);$R(u$)x;})
#define $s $a(,__attribute__((target("avx2"))))
#endif
#define $z $a(16,32)
#define $t $a(uint8x16_t,__m256i)
#define $I $w(__forceinline,__attribute__((always_inline)))
#define $F M(),
#define E$(x)if(!(x))abort();
$w(LONG WINAPI $x(_EXCEPTION_POINTERS*);,)namespace $f{using namespace std;struct B{enum $c A:char{}c;B&$O=(char x){c=A{x};$r}$O char(){$R(char)c;}};$C u$ C=~0ULL/255;struct D{string&K;};static B E[65568];template<int F>struct G{B*H,*S;void K(off_t C){$w(char*D=(char*)VirtualAlloc(0,(C+8191)&-4096,8192,1);E$(D)E$(VirtualFree(D,0,32768))DWORD A=C&-65536;E$(!A||MapViewOfFileEx(CreateFileMapping(GetStdHandle(-10),0,2,0,A,0),4,0,0,0,D)==D)E$(VirtualAlloc(D+A,65536,12288,4)==D+A)E$(~_lseek(0,A,0))DWORD E=0;ReadFile(GetStdHandle(-10),D+A,65536,&E,0);,int A=getpagesize();char*D=(char*)mmap(0,C+A,3,2,0,0);E$(D!=(void*)-1)E$(mmap(D+((C+A-1)&-A),A,3,$m(4114,50),-1,0)!=(void*)-1))H=(B*)D+C;*H=10;H[1]=48;H[2]=0;S=(B*)D;}void L(){H=S=E;}$I void M(){if(F&&S==H){$w(DWORD A=0;ReadFile(GetStdHandle(-10),S=E,65536,&A,0);,$a($u(register long A asm("x0")=0,D asm("x1")=(long)E,G asm("x2")=65536,C asm($m("x16","x8"))=$m(3,63);asm volatile("svc 0" $m("x80",):"+r"(A),"+r"(D):"r"(C),"r"(G));S=launder(E);),off_t A=$H(3,$m(33554435,0));B*D=E;asm volatile($H("int $128","syscall"):"+a"(A),$H("+c"(D):"b","+S"(D):"D")(0),"d"(65536)$H(,$u(:"rcx","r11")));S=D;))H=S+A;*H=10;if(!A)E[1]=48,E[2]=0;}}$T>$I void N(T&x){while($F(*S&240)==48)x=T(x*10+(*S++-48));}$T>$I decltype((void)~T{1})O(T&x){M();int A=is_signed_v<T>&&*S==45;S+=A;N(x=0);x=A?1+~x:x;}$T>$I decltype((void)T{1.})O(T&x){M();int A=*S==45;S+=A;$F S+=*S==43;u$ n=0;int i=0;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48;int B=20;int C=*S==46;S+=C;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48,B-=C;x=(T)n;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;if(*S==46)S++,C=1;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;int D;if((*S|32)==101)S++,$F S+=*S==43,O(D),B+=D;static $C auto E=[](){array<T,41>E{};T x=1;for(int i=21;i--;)E[40-i]=x,E[i]=1/x,x*=10;$R E;}();while(B>40)x*=(T)1e10,B-=10;while(B<0)x*=(T)1e-10,B+=10;x*=E[B];x=A?-x:x;}$I void O(bool&x){$F x=*S++==49;}$I void O(char&x){$F x=*S++;}$I void O(uint8_t&x){$F x=*S++;}$I void O(int8_t&x){$F x=*S++;}$T>$s void P(string&K,T C){M();B*G=S;C();K.assign((char*)G,S-G);while(F&&S==H&&($F H!=E)){C();K.append(E,S);}}$s void O(string&K){P(K,[&]()$s{B*p=S;$w(ULONG R;,)$t x;$a(uint64x2_t A;while(memcpy(&x,p,16),A=uint64x2_t(x<33),!(A[0]|A[1]))p+=16;S=p+(A[0]?0:8)+$w((_BitScanForward64(&R,A[0]?A[0]:A[1]),R),__builtin_ctzll(A[0]?A[0]:A[1]))/8;,int J;$t C=M$(set1,32);while(memcpy(&x,p,32),!(J=M$(movemask,M$(cmpeq,C,_mm256_max_epu8(C,x)))))p+=32;S=p+$w((_BitScanForward(&R,J),R),__builtin_ctz(J));)});}$s void O(D&A){P(A.K,[&](){S=(B*)memchr(S,10,H-S+1);});if(A.K.size()&&A.K.back()==13)A.K.pop_back();if(A.K.empty()||S<H)S+=*S==10;}$T>$I void O(complex<T>&K){T A,B{};if($F*S==40){S++;O(A);if($F*S++==44)Q(B),S++;}else O(A);K={A,B};}template<size_t N>$s void O(bitset<N>&K){if(N>4095&&!*this)$R;ptrdiff_t i=N;while(i)if($F i%$z||H-S<$z)K[--i]=*S++==49;else{B*p=S;for(int64_t j=0;j<min(i,H-S)/$z;j++){i-=$z;$t x;memcpy(&x,p,$z);$a(auto B=(uint8x16_t)vdupq_n_u64(~2ULL/254)&(48-x);auto C=vzip_u8(vget_high_u8(B),vget_low_u8(B));auto y=vaddvq_u16((uint16x8_t)vcombine_u8(C.val[0],C.val[1]));,u$ a=~0ULL/65025;auto y=$w(_byteswap_ulong,__builtin_bswap32)(M$(movemask,M$(shuffle,_mm256_slli_epi32(x,7),_mm256_set_epi64x(a+C*24,a+C*16,a+C*8,a))));)p+=$z;memcpy((char*)&K+i/8,&y,$z/8);}S=p;}}$T>$I void Q(T&K){if(!is_same_v<T,D>)while($F(uint8_t)*S<33)S++;O(K);}$O bool(){$R!!*this;}bool $O!(){$R S>H;}};struct U{G<0>A;G<1>B;U(){struct stat D;E$(~fstat(0,&D))(D.st_mode>>12)==8?A.K(D.st_size):B.L();}U*tie(nullptr_t){$R this;}void sync_with_stdio(bool){}$T>$I U&$O>>(T&K){A.S?A.Q(K):B.Q(K);$r}$O bool(){$R!!*this;}bool $O!(){$R A.S?!A:!B;}};short A[100];char L[64]{1};struct
V{char*D;B*S;int J;V(){$w(E$(D=(char*)VirtualAlloc(0,536870912,8192,4))E$(VirtualAlloc(D,4096,4096,260))AddVectoredExceptionHandler(1,$x);,size_t C=536870912;$m(,rlimit E;getrlimit(RLIMIT_AS,&E);if(~E.rlim_cur)C=25165824;)D=(char*)mmap(0,C,3,$m(4162,16418),-1,0);E$(D!=(void*)-1))S=(B*)D;for(int i=0;i<100;i++)A[i]=short((48+i/10)|((48+i%10)<<8));for(int i=1;i<64;i++)L[i]=L[i-1]+(0x8922489224892249>>i&1);}~V(){flush($w(!J,));}void flush($w(int F=0,)){$w(J=1;auto E=GetStdHandle(-11);auto C=F?ReOpenFile(E,1073741824,7,2684354560):(void*)-1;DWORD A;E$(C==(void*)-1?WriteFile(E,D,DWORD((char*)S-D),&A,0):(WriteFile(C,D,DWORD(((char*)S-D+4095)&-4096),&A,0)&&~_chsize(1,int((char*)S-D)))),auto G=D;ssize_t A;while((A=write(1,G,(char*)S-G))>0)G+=A;E$(~A))S=(B*)D;}$P(char)*S++=K;}$P(uint8_t)*S++=K;}$P(int8_t)*S++=K;}$P(bool)*S++=48+K;}$T>decltype((void)~T{1})F(T K){using D=make_unsigned_t<T>;D C=K;if(K<0)F('-'),C=1+~C;static $C auto N=[](){array<D,5*sizeof(T)/2>N{};D n=1;for(size_t i=1;i<N.size();i++)n*=10,N[i]=n;$R N;}();$w(ULONG M;,)int G=L[$w(($H(_BitScanReverse(&M,ULONG((int64_t)C>>32))?M+=32:_BitScanReverse(&M,(ULONG)C|1),_BitScanReverse64(&M,C|1)),M),63^__builtin_clzll(C|1))];G-=C<N[G-1];short H[20];if $C(sizeof(T)==2){auto n=33555U*C-C/2;u$ H=A[n>>25];n=(n&33554431)*25;H|=A[n>>23]<<16;H|=u$(48+((n&8388607)*5>>22))<<32;H>>=40-G*8;memcpy(S,&H,8);}else if $C(sizeof(T)==4){auto n=1441151881ULL*C;$H(n>>=25;n++;for(int i=0;i<5;i++){H[i]=A[n>>32];n=(n&~0U)*100;},int K=57;auto J=~0ULL>>7;for(int i=0;i<5;i++){H[i]=A[n>>K];n=(n&J)*25;K-=2;J/=4;})memcpy(S,(B*)H+10-G,16);}else{$H($u(if(C<(1ULL<<32)){$R F((uint32_t)C);}auto J=(u$)1e10;auto x=C/J,y=C%J;int K=100000,b[]{int(x/K),int(x%K),int(y/K),int(y%K)};B H[40];for(int i=0;i<4;i++){int n=int((429497ULL*b[i]>>7)+1);B*p=H+i*5;*p=48+char(n>>25);n=(n&~0U>>7)*25;memcpy(p+1,A+(n>>23),2);memcpy(p+3,A+((n&~0U>>9)*25>>21),2);}),$u(u$ D,E=_umul128(18,C,&D),F;_umul128(0x725dd1d243aba0e8,C,&F);D+=__builtin_add_overflow(E,F+1,&E);for(int i=0;i<10;i++)H[i]=A[D],E=_umul128(100,E,&D);))memcpy(S,(B*)H+20-G,20);}S+=G;}$T>decltype((void)T{1.})F(T K){if(K<0)F('-'),K=-K;auto G=[&](){auto x=u$(K*1e12);$H($u(x-=x>999999999999;uint32_t n[]{uint32_t(x/1000000*429497>>7)+1,uint32_t(x%1000000*429497>>7)+1};int K=25,J=~0U>>7;for(int i=0;i<3;i++){for(int j=0;j<2;j++)memcpy(S+i*2+j*6,A+(n[j]>>K),2),n[j]=(n[j]&J)*25;K-=2;J/=4;}S+=12;),$u(u$ D,E=_umul128(472236648287,x,&D)>>8;E|=D<<56;D>>=8;E++;for(int i=0;i<6;i++)memcpy(S,A+D,2),S+=2,E=_umul128(100,E,&D);))};if(K==0)$R F('0');if(K>=1e16){K*=(T)1e-16;int B=16;while(K>=1)K*=(T).1,B++;F("0.");G();F('e');F(B);}else if(K>=1){auto B=(u$)K;F(B);if((K-=(T)B)>0)F('.'),G();}else F("0."),G();}$P(const char*)$w(size_t A=strlen(K);memcpy((char*)S,K,A);S+=A;,S=(B*)stpcpy((char*)S,K);)}$P(const uint8_t*)F((char*)K);}$P(const int8_t*)F((char*)K);}$P(string_view)memcpy(S,K.data(),K.size());S+=K.size();}$T>$P(complex<T>)*this<<'('<<K.real()<<','<<K.imag()<<')';}template<size_t N>$s $P(const bitset<N>&)auto i=N;while(i%$z)*S++=48+K[--i];B*p=S;while(i){i-=$z;$a(short,int)x;memcpy(&x,(char*)&K+i/8,$z/8);$a(auto A=(uint8x8_t)vdup_n_u16(x);vst1q_u8((uint8_t*)p,48-vtstq_u8(vcombine_u8(vuzp2_u8(A,A),vuzp1_u8(A,A)),(uint8x16_t)vdupq_n_u64(~2ULL/254)));,auto b=_mm256_set1_epi64x(~2ULL/254);_mm256_storeu_si256(($t*)p,M$(sub,M$(set1,48),M$(cmpeq,_mm256_and_si256(M$(shuffle,_mm256_set1_epi32(x),_mm256_set_epi64x(0,C,C*2,C*3)),b),b)));)p+=$z;}S=p;}$T>V&$O<<(const T&K){F(K);$r}V&$O<<(V&(*A)(V&)){$R A(*this);}};struct W{$T>W&$O<<(const T&K){$r}W&$O<<(W&(*A)(W&)){$R A(*this);}};}namespace std{$f::U i$;$f::V o$;$f::W e$;$f::U&getline($f::U&B,string&K){$f::D A{K};$R B>>A;}$f::V&flush($f::V&B){if(!i$.A.S)B.flush();$R B;}$f::V&endl($f::V&B){$R B<<'\n'<<flush;}$f::W&endl($f::W&B){$R B;}$f::W&flush($f::W&B){$R B;}}$w(LONG WINAPI $x(_EXCEPTION_POINTERS*A){auto C=A->ExceptionRecord;auto B=C->ExceptionInformation[1];if(C->ExceptionCode==2147483649&&B-(ULONG_PTR)std::o$.D<0x40000000){E$(VirtualAlloc((char*)B,16777216,4096,4)&&VirtualAlloc((char*)(B+16777216),4096,4096,260))$R-1;}$R 0;},)
#define freopen(...)if(freopen(__VA_ARGS__)==stdin)std::i$=$f::U{}
#define cin i$
#define cout o$
#ifdef ONLINE_JUDGE
#define cerr e$
#define clog e$
#endif
// End of blazingio
// NOLINTEND
// clang-format on

using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;

u64 get_time() {
    return chrono::steady_clock::now().time_since_epoch().count();
}

u32 inv(u32 a, u32 m) {
    if (a == 0) return 0;
    // assumption: gcd(a, m) = 1, 0 < a < m < 2^31
    u32 ori_m = m;
    i32 b = 1, c = 0;
    while (a != 1) {
        u32 d = m / a;
        c -= b * d;
        m -= a * d;
        swap(a, m);
        swap(b, c);
    }
    return b < 0 ? b + ori_m : b;
}

u32 modex(u32 b, i32 e, u32 m) {
    u32 r = 1;
    while (e) {
        if (e & 1) {
            r = (u64) r * b % m;
        }
        b = (u64) b * b % m;
        e >>= 1;
    }
    return r;
}

u32 crt(u32 a1, u32 m1, u32 a2, u32 m2) {
    // assumption: gcd(m1, m2) = 1, 0 < m1 * m2 < 2^31, a1 < m1, a2 < m2, 
    return (inv(m1 % m2, m2) * m1 * (u64) a2 + inv(m2 % m1, m1) * m2 * (u64) a1) % (m1 * m2);
}

const u32 MAXP = 999999937;
// Threshold to not precomputing everything
const u32 MAGIC0 = 2000000;
// Number of buckets for Farey fractions
const u32 MAGIC1 = 1000000; // 500000 - 2000000
// Farey fractions guaranteeing conversion to values less than this
const u32 MAGIC2 = 1300000; // 800000 - 2000000
// Factorisations up to this number are precomputed
const u32 MAGIC3 = 31624; // 31622 - MAGIC2
// Compute batch discrete logs for this number of smallest primes
const u32 MAGIC4 = 25;
// Update smooth net up to this number
const u32 MAGIC5 = 100;

// compute parameters to quickly *g%p
using mulgp_prec_t = tuple<u32, u32, u32>;
mulgp_prec_t mulgp_prec(u32 g, u32 p) {
    u32 neg_inv = p;
    neg_inv *= (2 - neg_inv * p);
    neg_inv *= (2 - neg_inv * p);
    neg_inv *= (2 - neg_inv * p);
    neg_inv *= (2 - neg_inv * p);
    return {((u64) g << 32) % p, -neg_inv, p};
}

// compute *g%p quickly
inline u32 mulgp(u32 x, mulgp_prec_t &prec) {
    u32 ori = x;
    u64 y = u64(x) * get<0>(prec);
    x = y + u64(u32(y) * get<1>(prec)) * get<2>(prec) >> 32;
    return x - (x >= get<2>(prec)) * get<2>(prec);
}

class fast_table {
public:
	explicit fast_table(u32 n) : cap(buckets(n)), mask(cap - 1), tags(cap, 0), keys(cap, 0), values(cap, -1) {}
	void insert(u32 key, u32 val) {
		u32 idx = key & mask;
		while (true) {
			if (!tags[idx]) {
				tags[idx] = true;
				keys[idx] = key;
				values[idx] = val;
				return;
			}
			idx = (idx + 1) & mask;
		}
	}
	u32 lookup(u32 key) const {
		u32 idx = key & mask;
		while (true) {
			if (!tags[idx]) {
				return -1;
			}
			if (keys[idx] == key) {
				return values[idx];
			}
			idx = (idx + 1) & mask;
		}
	}
private:
	u32 cap;
	u32 mask;
	std::vector<bool> tags;
	std::vector<u32> keys;
	std::vector<u32> values;
	static inline u32 buckets(u32 n) {
		n /= 0.8;
		if (n < 16) return 16;
		return 1u << 32 - countl_zero(n - 1);
	}
};

// g does not have to be a primitive root
// only does bsgs
class batch_discrete_log {
public:
    batch_discrete_log(u32 p, u32 g, u32 order, u32 _step_size = 0) : p(p), g(g), order(order), step_size(compute_step_size(order, _step_size)), lookup(step_size) {
        // assumption: p is prime, g has order order, g < p < 2^31
        auto prec = mulgp_prec(g, p);
        for (u32 i = 0, j = 1; i < step_size; ++i, j = mulgp(j, prec)) {
            lookup.insert(j, i);
        }
        invstep = modex(g, order - step_size, p);
        invstepprec = mulgp_prec(invstep, p);
    }

    u32 operator()(u32 h) {
        u32 ret = 0;
        while (lookup.lookup(h) == -1) {
            ret += step_size;
            h = mulgp(h, invstepprec);
        }
        return ret + lookup.lookup(h);
    }

private:
    u32 p, g, order, step_size, invstep;
    mulgp_prec_t invstepprec;
    fast_table lookup;
    static u32 compute_step_size(u32 order, u32 given_step_size) {
        if (given_step_size == 0) {
            given_step_size = sqrt(order);
        }
        if (given_step_size < 100) {
            given_step_size = order;
        }
        return given_step_size;
    }
};

// assumes g is a primitive root
class fixed_discrete_log {
    u32 p, g, pm1, halfp;
    vector<u32> dlog_lookup;
    vector<u32> farey_lookup;
public:
    fixed_discrete_log(u32 p, u32 g) : p(p), g(g), pm1(p-1), halfp(p/2) {
        // precompute everything for small p
        if (p < MAGIC0) {
            dlog_lookup.assign(p, 0);
            auto prec = mulgp_prec(g, p);
            u32 i = 1, j = g;
            while (j != 1) {
                dlog_lookup[j] = i;
                j = mulgp(j, prec);
                i += 1;
            }
            return;
        }
        dlog_lookup.assign(MAGIC2 + 1, 0);
        // precompute farey fractions to express x = a/b (mod p) where a < MAGIC2 and b < around 1000.
        auto farey = [&](auto self, u32 f1, u32 f2, u32 x, u32 y) -> void {
            u32 f3 = f1 + f2;
            u32 l = (((u64) MAXP * (f3 >> 16) - MAGIC2) * MAGIC1 - 1) / ((u64) MAXP * (f3 & 0xffff)) + 1;
            u32 r = (((u64) MAXP * (f3 >> 16) + MAGIC2) * MAGIC1) / ((u64) MAXP * (f3 & 0xffff));
            l = max(l, x);
            r = min(r, y);
            if (x < l) {
                self(self, f1, f3, x, l);
            }
            fill(farey_lookup.begin() + l, farey_lookup.begin() + r, f3);
            if (r < y) {
                self(self, f3, f2, r, y);
            }
        };
        constexpr u32 first_x = (u64) MAGIC2 * MAGIC1 / MAXP;
        constexpr u32 first_y = ((u64) (MAXP - MAGIC2) * MAGIC1 - 1) / (MAXP * 2) + 1;
        farey_lookup.assign(MAGIC1, 0);
        fill(farey_lookup.begin(), farey_lookup.begin() + first_x, 1);
        farey(farey, 1, 0x10002, first_x, first_y);
        fill(farey_lookup.begin() + first_y, farey_lookup.begin() + MAGIC1/2, 0x10002);
        for (u32 i = MAGIC1/2; i < MAGIC1; ++i) {
            farey_lookup[i] = (farey_lookup[MAGIC1 - 1 - i] * 0xffff0001u ^ 0xffff0000u) + 0x10000u;
        }
        // precompute factorisation up to MAGIC3
        vector<u32> spf(MAGIC3+1, 0), primes;
        primes.reserve(3401); // update this when updating MAGIC3
        for (u32 i = 2; i <= MAGIC3; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int j = 0; primes[j] * i <= MAGIC3; ++j) {
                spf[primes[j] * i] = primes[j];
                if (primes[j] == spf[i]) {
                    break;
                }
            }
        }
        // dlog stage 1: the first MAGIC4 primes
        vector<u32> stage1(primes.begin(), primes.begin() + MAGIC4);
        vector<u32> stage1_ans(MAGIC4, 0);
        u32 stage1_mod = 1;
        // Do Pohlig-Hellman. The difference matters even with safe prime p
        vector<pair<u32, u32>> group_size_factorisation;
        u32 group_size = pm1;
        for (u32 pp : primes) {
            if (pp * pp >= group_size) {
                break;
            }
            if (group_size % pp == 0) {
                u32 e = 0;
                do {
                    ++e;
                    group_size /= pp;
                } while (group_size % pp == 0);
                group_size_factorisation.emplace_back(pp, e);
            }
        }
        if (group_size > 1) {
            group_size_factorisation.emplace_back(group_size, 1);
        }
        u32 ginv = inv(g, p);
        for (auto [pp, e] : group_size_factorisation) {
            // handle each subgroup of order pp separately
            u32 n = modex(pp, e, p);
            u32 ee = p/n;
            u32 gg = modex(ginv, ee, p);
            batch_discrete_log ph(p, modex(g, p/pp, p), pp, sqrt(1.0 * pp * MAGIC4));
            for (u32 i = 0; i < MAGIC4; ++i) {
                u32 h = modex(stage1[i], ee, p);
                u32 pe = n;
                i32 ans = 0;
                u32 mul = 1;
                for (u32 j = 0; j < e; ++j) {
                    pe /= pp;
                    ans += ph(modex((u64) h * modex(gg, ans, p) % p, pe, p)) * mul;
                    mul *= pp;
                }
                stage1_ans[i] = crt(stage1_ans[i], stage1_mod, ans, n);
            }
            stage1_mod *= n;
        }
        for (u32 i = 0; i < MAGIC4; ++i) {
            dlog_lookup[stage1[i]] = stage1_ans[i];
        }
        // dlog stage 2: primes up to MAGIC3 and MAGIC5-smooth numbers up to MAGIC2
        vector<u32> smooth{1};
        smooth.reserve(4825); // udpate this when updating MAGIC5
        for (u32 pp : primes) {
            if (!dlog_lookup[pp]) {
                // dlog stage 2a: primes up to MAGIC3
                u32 h = pp;
                u32 ans = 0;
                while (true) {
                    auto [num, den] = get_frac(h);
                    if ((den != 1 && dlog_lookup[den] == 0) || (abs(num) != 1 && dlog_lookup[abs(num)] == 0)) {
                        // fail, retry with a different h
                        ++ans;
                        h = (u64) h * ginv % p;
                        continue;
                    }
                    u32 dlog = (num < 0 ? dlog_lookup[-num] + halfp : dlog_lookup[num]) + pm1 - dlog_lookup[den];
                    dlog_lookup[pp] = (ans + dlog) % pm1;
                    break;
                }
            }
            // dlog stage 2b: the smooth numbers currently calculable up to MAGIC5
            if (pp > MAGIC5) continue;
            smooth.erase(remove_if(smooth.begin(), smooth.end(), [&](u32 v) {return v > MAGIC2/pp;}), smooth.end());
            u32 n = smooth.size();
            for (u32 j = 0; j < n; ++j) {
                u32 new_smooth = smooth[j] * pp;
                do {
                    if (new_smooth <= MAGIC2/pp) {
                        smooth.push_back(new_smooth);
                    }
                    dlog_lookup[new_smooth] = (dlog_lookup[new_smooth / pp] + dlog_lookup[pp]) % pm1;
                    new_smooth *= pp;
                } while (new_smooth <= MAGIC2);
            }
        }
        // dlog stage 3: all up to MAGIC3
        for (u32 i = 2; i <= MAGIC3; ++i) {
            if (dlog_lookup[i]) continue;
            dlog_lookup[i] = (dlog_lookup[i / spf[i]] + dlog_lookup[spf[i]]) % pm1;
        }
        // dlog stage 5: all up to MAGIC2
        for (u32 i = MAGIC3 + 1; i <= MAGIC2; ++i) {
            if (dlog_lookup[i]) continue;
            dlog_lookup[i] = (dlog_lookup[p % i] + halfp + pm1 - dlog_lookup[p / i]) % pm1;
        }
    }

    inline pair<i32, u32> get_frac(u32 h) const {
        u32 bucket = u64(h) * MAGIC1 / p;
        u32 frac = farey_lookup[bucket];
        u32 den = frac & 0xffff;
        // this might overflow, but it's fine
        i32 num = den * h - (frac >> 16) * p;
        return {num, den};
    };

    u32 operator()(u32 h) const {
        if (h < dlog_lookup.size()) {
            return dlog_lookup[h];
        }
        auto [num, den] = get_frac(h);
        u32 ans = (num < 0 ? dlog_lookup[-num] + halfp : dlog_lookup[num]) + pm1 - dlog_lookup[den];
        ans -= (ans >= pm1) * pm1;
        ans -= (ans >= pm1) * pm1;
        return ans;
    }
};

int main() {
    u32 p, g, n;
    cin >> p >> g >> n;
    u64 tm = get_time();
    fixed_discrete_log ds(p, g);
    u64 tm2 = get_time();
    fprintf(stderr, "precompute took %lfms\n", (tm2-tm)/1e6);
    while (n--) {
        u32 a;
        cin >> a;
        cout << ds(a) << '\n';
    }
}