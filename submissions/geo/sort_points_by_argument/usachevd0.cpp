#line 1 "sol-3.cpp"
#include <bits/stdc++.h>

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
#line 43 "sol-3.cpp"
#include $a(<arm_neon.h>,<immintrin.h>)
#line 45 "sol-3.cpp"
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

    // BEGIN
    // //////////////////////////////////////////////////////////////////////

    ; //

/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2021 Orson Peters

    This software is provided 'as-is', without any express or implied warranty.
   In no event will the authors be held liable for any damages arising from the
   use of this software.

    Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software in a
   product, an acknowledgment in the product documentation would be appreciated
   but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PDQSORT_H
#define PDQSORT_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

#if __cplusplus >= 201103L
#include <cstdint>
#include <type_traits>
#define PDQSORT_PREFER_MOVE(x) std::move(x)
#else
#define PDQSORT_PREFER_MOVE(x) (x)
#endif

namespace pdqsort_detail {
enum {
  // Partitions below this size are sorted using insertion sort.
  insertion_sort_threshold = 24,

  // Partitions above this size use Tukey's ninther to select the pivot.
  ninther_threshold = 128,

  // When we detect an already sorted partition, attempt an insertion sort
  // that allows this
  // amount of element moves before giving up.
  partial_insertion_sort_limit = 8,

  // Must be multiple of 8 due to loop unrolling, and < 256 to fit in unsigned
  // char.
  block_size = 64,

  // Cacheline size, assumes power of two.
  cacheline_size = 64

};

#if __cplusplus >= 201103L
template <class T>
struct is_default_compare : std::false_type {};
template <class T>
struct is_default_compare<std::less<T>> : std::true_type {};
template <class T>
struct is_default_compare<std::greater<T>> : std::true_type {};
#endif

// Returns floor(log2(n)), assumes n > 0.
template <class T>
inline int log2(T n) {
  int log = 0;
  while (n >>= 1)
    ++log;
  return log;
}

// Sorts [begin, end) using insertion sort with the given comparison function.
template <class Iter, class Compare>
inline void insertion_sort(Iter begin, Iter end, Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  if (begin == end)
    return;

  for (Iter cur = begin + 1; cur != end; ++cur) {
    Iter sift = cur;
    Iter sift_1 = cur - 1;

    // Compare first so we can avoid 2 moves for an element already positioned
    // correctly.
    if (comp(*sift, *sift_1)) {
      T tmp = PDQSORT_PREFER_MOVE(*sift);

      do {
        *sift-- = PDQSORT_PREFER_MOVE(*sift_1);
      } while (sift != begin && comp(tmp, *--sift_1));

      *sift = PDQSORT_PREFER_MOVE(tmp);
    }
  }
}

// Sorts [begin, end) using insertion sort with the given comparison function.
// Assumes
// *(begin - 1) is an element smaller than or equal to any element in [begin,
// end).
template <class Iter, class Compare>
inline void unguarded_insertion_sort(Iter begin, Iter end, Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  if (begin == end)
    return;

  for (Iter cur = begin + 1; cur != end; ++cur) {
    Iter sift = cur;
    Iter sift_1 = cur - 1;

    // Compare first so we can avoid 2 moves for an element already positioned
    // correctly.
    if (comp(*sift, *sift_1)) {
      T tmp = PDQSORT_PREFER_MOVE(*sift);

      do {
        *sift-- = PDQSORT_PREFER_MOVE(*sift_1);
      } while (comp(tmp, *--sift_1));

      *sift = PDQSORT_PREFER_MOVE(tmp);
    }
  }
}

// Attempts to use insertion sort on [begin, end). Will return false if more
// than partial_insertion_sort_limit elements were moved, and abort sorting.
// Otherwise it will successfully sort and return true.
template <class Iter, class Compare>
inline bool partial_insertion_sort(Iter begin, Iter end, Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  if (begin == end)
    return true;

  std::size_t limit = 0;
  for (Iter cur = begin + 1; cur != end; ++cur) {
    Iter sift = cur;
    Iter sift_1 = cur - 1;

    // Compare first so we can avoid 2 moves for an element already positioned
    // correctly.
    if (comp(*sift, *sift_1)) {
      T tmp = PDQSORT_PREFER_MOVE(*sift);

      do {
        *sift-- = PDQSORT_PREFER_MOVE(*sift_1);
      } while (sift != begin && comp(tmp, *--sift_1));

      *sift = PDQSORT_PREFER_MOVE(tmp);
      limit += cur - sift;
    }

    if (limit > partial_insertion_sort_limit)
      return false;
  }

  return true;
}

template <class Iter, class Compare>
inline void sort2(Iter a, Iter b, Compare comp) {
  if (comp(*b, *a))
    std::iter_swap(a, b);
}

// Sorts the elements *a, *b and *c using comparison function comp.
template <class Iter, class Compare>
inline void sort3(Iter a, Iter b, Iter c, Compare comp) {
  sort2(a, b, comp);
  sort2(b, c, comp);
  sort2(a, b, comp);
}

template <class T>
inline T *align_cacheline(T *p) {
#if defined(UINTPTR_MAX) && __cplusplus >= 201103L
  std::uintptr_t ip = reinterpret_cast<std::uintptr_t>(p);
#else
  std::size_t ip = reinterpret_cast<std::size_t>(p);
#endif
  ip = (ip + cacheline_size - 1) & -cacheline_size;
  return reinterpret_cast<T *>(ip);
}

template <class Iter>
inline void swap_offsets(Iter first, Iter last, unsigned char *offsets_l,
                         unsigned char *offsets_r, size_t num, bool use_swaps) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  if (use_swaps) {
    // This case is needed for the descending distribution, where we need
    // to have proper swapping for pdqsort to remain O(n).
    for (size_t i = 0; i < num; ++i) {
      std::iter_swap(first + offsets_l[i], last - offsets_r[i]);
    }
  } else if (num > 0) {
    Iter l = first + offsets_l[0];
    Iter r = last - offsets_r[0];
    T tmp(PDQSORT_PREFER_MOVE(*l));
    *l = PDQSORT_PREFER_MOVE(*r);
    for (size_t i = 1; i < num; ++i) {
      l = first + offsets_l[i];
      *r = PDQSORT_PREFER_MOVE(*l);
      r = last - offsets_r[i];
      *l = PDQSORT_PREFER_MOVE(*r);
    }
    *r = PDQSORT_PREFER_MOVE(tmp);
  }
}

// Partitions [begin, end) around pivot *begin using comparison function comp.
// Elements equal to the pivot are put in the right-hand partition. Returns
// the position of the pivot after partitioning and whether the passed
// sequence already was correctly partitioned. Assumes the pivot is a median
// of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long. Uses branchless partitioning.
template <class Iter, class Compare>
inline std::pair<Iter, bool> partition_right_branchless(Iter begin, Iter end,
                                                        Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;

  // Move pivot into local for speed.
  T pivot(PDQSORT_PREFER_MOVE(*begin));
  Iter first = begin;
  Iter last = end;

  // Find the first element greater than or equal than the pivot (the median
  // of 3 guarantees this exists).
  while (comp(*++first, pivot))
    ;

  // Find the first element strictly smaller than the pivot. We have to guard
  // this search if there was no element before *first.
  if (first - 1 == begin)
    while (first < last && !comp(*--last, pivot))
      ;
  else
    while (!comp(*--last, pivot))
      ;

  // If the first pair of elements that should be swapped to partition are the
  // same element, the passed in sequence already was correctly partitioned.
  bool already_partitioned = first >= last;
  if (!already_partitioned) {
    std::iter_swap(first, last);
    ++first;

    // The following branchless partitioning is derived from "BlockQuicksort:
    // How Branch Mispredictions don’t affect Quicksort" by Stefan Edelkamp
    // and Armin Weiss, but heavily micro-optimized.
    unsigned char offsets_l_storage[block_size + cacheline_size];
    unsigned char offsets_r_storage[block_size + cacheline_size];
    unsigned char *offsets_l = align_cacheline(offsets_l_storage);
    unsigned char *offsets_r = align_cacheline(offsets_r_storage);

    Iter offsets_l_base = first;
    Iter offsets_r_base = last;
    size_t num_l, num_r, start_l, start_r;
    num_l = num_r = start_l = start_r = 0;

    while (first < last) {
      // Fill up offset blocks with elements that are on the wrong side.
      // First we determine how much elements are considered for each offset
      // block.
      size_t num_unknown = last - first;
      size_t left_split =
          num_l == 0 ? (num_r == 0 ? num_unknown / 2 : num_unknown) : 0;
      size_t right_split = num_r == 0 ? (num_unknown - left_split) : 0;

      // Fill the offset blocks.
      if (left_split >= block_size) {
        for (size_t i = 0; i < block_size;) {
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
        }
      } else {
        for (size_t i = 0; i < left_split;) {
          offsets_l[num_l] = i++;
          num_l += !comp(*first, pivot);
          ++first;
        }
      }

      if (right_split >= block_size) {
        for (size_t i = 0; i < block_size;) {
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
        }
      } else {
        for (size_t i = 0; i < right_split;) {
          offsets_r[num_r] = ++i;
          num_r += comp(*--last, pivot);
        }
      }

      // Swap elements and update block sizes and first/last boundaries.
      size_t num = std::min(num_l, num_r);
      swap_offsets(offsets_l_base, offsets_r_base, offsets_l + start_l,
                   offsets_r + start_r, num, num_l == num_r);
      num_l -= num;
      num_r -= num;
      start_l += num;
      start_r += num;

      if (num_l == 0) {
        start_l = 0;
        offsets_l_base = first;
      }

      if (num_r == 0) {
        start_r = 0;
        offsets_r_base = last;
      }
    }

    // We have now fully identified [first, last)'s proper position. Swap the
    // last elements.
    if (num_l) {
      offsets_l += start_l;
      while (num_l--)
        std::iter_swap(offsets_l_base + offsets_l[num_l], --last);
      first = last;
    }
    if (num_r) {
      offsets_r += start_r;
      while (num_r--)
        std::iter_swap(offsets_r_base - offsets_r[num_r], first), ++first;
      last = first;
    }
  }

  // Put the pivot in the right place.
  Iter pivot_pos = first - 1;
  *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
  *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

  return std::make_pair(pivot_pos, already_partitioned);
}

// Partitions [begin, end) around pivot *begin using comparison function comp.
// Elements equal to the pivot are put in the right-hand partition. Returns
// the position of the pivot after partitioning and whether the passed
// sequence already was correctly partitioned. Assumes the pivot is a median
// of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long.
template <class Iter, class Compare>
inline std::pair<Iter, bool> partition_right(Iter begin, Iter end,
                                             Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;

  // Move pivot into local for speed.
  T pivot(PDQSORT_PREFER_MOVE(*begin));

  Iter first = begin;
  Iter last = end;

  // Find the first element greater than or equal than the pivot (the median
  // of 3 guarantees this exists).
  while (comp(*++first, pivot))
    ;

  // Find the first element strictly smaller than the pivot. We have to guard
  // this search if there was no element before *first.
  if (first - 1 == begin)
    while (first < last && !comp(*--last, pivot))
      ;
  else
    while (!comp(*--last, pivot))
      ;

  // If the first pair of elements that should be swapped to partition are the
  // same element, the passed in sequence already was correctly partitioned.
  bool already_partitioned = first >= last;

  // Keep swapping pairs of elements that are on the wrong side of the pivot.
  // Previously swapped pairs guard the searches, which is why the first
  // iteration is special-cased above.
  while (first < last) {
    std::iter_swap(first, last);
    while (comp(*++first, pivot))
      ;
    while (!comp(*--last, pivot))
      ;
  }

  // Put the pivot in the right place.
  Iter pivot_pos = first - 1;
  *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
  *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

  return std::make_pair(pivot_pos, already_partitioned);
}

// Similar function to the one above, except elements equal to the pivot are
// put to the left of the pivot and it doesn't check or return if the passed
// sequence already was partitioned. Since this is rarely used (the many equal
// case), and in that case pdqsort already has O(n) performance, no block
// quicksort is applied here for simplicity.
template <class Iter, class Compare>
inline Iter partition_left(Iter begin, Iter end, Compare comp) {
  typedef typename std::iterator_traits<Iter>::value_type T;

  T pivot(PDQSORT_PREFER_MOVE(*begin));
  Iter first = begin;
  Iter last = end;

  while (comp(pivot, *--last))
    ;

  if (last + 1 == end)
    while (first < last && !comp(pivot, *++first))
      ;
  else
    while (!comp(pivot, *++first))
      ;

  while (first < last) {
    std::iter_swap(first, last);
    while (comp(pivot, *--last))
      ;
    while (!comp(pivot, *++first))
      ;
  }

  Iter pivot_pos = last;
  *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
  *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

  return pivot_pos;
}

template <class Iter, class Compare, bool Branchless>
inline void pdqsort_loop(Iter begin, Iter end, Compare comp, int bad_allowed,
                         bool leftmost = true) {
  typedef typename std::iterator_traits<Iter>::difference_type diff_t;

  // Use a while loop for tail recursion elimination.
  while (true) {
    diff_t size = end - begin;

    // Insertion sort is faster for small arrays.
    if (size < insertion_sort_threshold) {
      if (leftmost)
        insertion_sort(begin, end, comp);
      else
        unguarded_insertion_sort(begin, end, comp);
      return;
    }

    // Choose pivot as median of 3 or pseudomedian of 9.
    diff_t s2 = size / 2;
    if (size > ninther_threshold) {
      sort3(begin, begin + s2, end - 1, comp);
      sort3(begin + 1, begin + (s2 - 1), end - 2, comp);
      sort3(begin + 2, begin + (s2 + 1), end - 3, comp);
      sort3(begin + (s2 - 1), begin + s2, begin + (s2 + 1), comp);
      std::iter_swap(begin, begin + s2);
    } else
      sort3(begin + s2, begin, end - 1, comp);

    // If *(begin - 1) is the end of the right partition of a previous
    // partition operation there is no element in [begin, end) that is smaller
    // than *(begin - 1). Then if our pivot compares equal to *(begin - 1) we
    // change strategy, putting equal elements in the left partition, greater
    // elements in the right partition. We do not have to recurse on the left
    // partition, since it's sorted (all equal).
    if (!leftmost && !comp(*(begin - 1), *begin)) {
      begin = partition_left(begin, end, comp) + 1;
      continue;
    }

    // Partition and get results.
    std::pair<Iter, bool> part_result =
        Branchless ? partition_right_branchless(begin, end, comp)
                   : partition_right(begin, end, comp);
    Iter pivot_pos = part_result.first;
    bool already_partitioned = part_result.second;

    // Check for a highly unbalanced partition.
    diff_t l_size = pivot_pos - begin;
    diff_t r_size = end - (pivot_pos + 1);
    bool highly_unbalanced = l_size < size / 8 || r_size < size / 8;

    // If we got a highly unbalanced partition we shuffle elements to break
    // many patterns.
    if (highly_unbalanced) {
      // If we had too many bad partitions, switch to heapsort to guarantee
      // O(n log n).
      if (--bad_allowed == 0) {
        std::make_heap(begin, end, comp);
        std::sort_heap(begin, end, comp);
        return;
      }

      if (l_size >= insertion_sort_threshold) {
        std::iter_swap(begin, begin + l_size / 4);
        std::iter_swap(pivot_pos - 1, pivot_pos - l_size / 4);

        if (l_size > ninther_threshold) {
          std::iter_swap(begin + 1, begin + (l_size / 4 + 1));
          std::iter_swap(begin + 2, begin + (l_size / 4 + 2));
          std::iter_swap(pivot_pos - 2, pivot_pos - (l_size / 4 + 1));
          std::iter_swap(pivot_pos - 3, pivot_pos - (l_size / 4 + 2));
        }
      }

      if (r_size >= insertion_sort_threshold) {
        std::iter_swap(pivot_pos + 1, pivot_pos + (1 + r_size / 4));
        std::iter_swap(end - 1, end - r_size / 4);

        if (r_size > ninther_threshold) {
          std::iter_swap(pivot_pos + 2, pivot_pos + (2 + r_size / 4));
          std::iter_swap(pivot_pos + 3, pivot_pos + (3 + r_size / 4));
          std::iter_swap(end - 2, end - (1 + r_size / 4));
          std::iter_swap(end - 3, end - (2 + r_size / 4));
        }
      }
    } else {
      // If we were decently balanced and we tried to sort an already
      // partitioned sequence try to use insertion sort.
      if (already_partitioned &&
          partial_insertion_sort(begin, pivot_pos, comp) &&
          partial_insertion_sort(pivot_pos + 1, end, comp))
        return;
    }

    // Sort the left partition first using recursion and do tail recursion
    // elimination for the right-hand partition.
    pdqsort_loop<Iter, Compare, Branchless>(begin, pivot_pos, comp, bad_allowed,
                                            leftmost);
    begin = pivot_pos + 1;
    leftmost = false;
  }
}
}

template <class Iter, class Compare>
inline void pdqsort(Iter begin, Iter end, Compare comp) {
  if (begin == end)
    return;

#if __cplusplus >= 201103L
  pdqsort_detail::pdqsort_loop<
      Iter, Compare,
      pdqsort_detail::is_default_compare<
          typename std::decay<Compare>::type>::value &&
          std::is_arithmetic<
              typename std::iterator_traits<Iter>::value_type>::value>(
      begin, end, comp, pdqsort_detail::log2(end - begin));
#else
  pdqsort_detail::pdqsort_loop<Iter, Compare, false>(
      begin, end, comp, pdqsort_detail::log2(end - begin));
#endif
}

template <class Iter>
inline void pdqsort(Iter begin, Iter end) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  pdqsort(begin, end, std::less<T>());
}

template <class Iter, class Compare>
inline void pdqsort_branchless(Iter begin, Iter end, Compare comp) {
  if (begin == end)
    return;
  pdqsort_detail::pdqsort_loop<Iter, Compare, true>(
      begin, end, comp, pdqsort_detail::log2(end - begin));
}

template <class Iter>
inline void pdqsort_branchless(Iter begin, Iter end) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  pdqsort_branchless(begin, end, std::less<T>());
}

#undef PDQSORT_PREFER_MOVE

#endif

using namespace std;

void run([[maybe_unused]] int testNo) {}

constexpr int MaxN = 2e5 + 5;
__uint128_t a[MaxN];

uint64_t _div(uint32_t u, uint32_t v) {
  bool rev = false;
  if (u > v) {
    swap(u, v);
    rev = true;
  }
  uint64_t q = (static_cast<__uint128_t>(u) << 60) / v;
  return !rev ? q : (1ull << 61) - q;
}

uint64_t tag(int x, int y) {
  if (x < 0 && y < 0) {
    return _div(-y, -x);
  } else if (x == 0 && y < 0) {
    return 0x1ull << 61;
  } else if (x > 0 && y < 0) {
    return (0x2ull << 61) | _div(x, -y);
  } else if (x >= 0 && y == 0) {
    return 0x3ull << 61;
  } else if (x > 0 && y > 0) {
    return (0x4ull << 61) | _div(y, x);
  } else if (x == 0 && y > 0) {
    return 0x5ull << 61;
  } else if (x < 0 && y > 0) {
    return (0x6ull << 61) | _div(-x, y);
  } else if (x < 0 && y == 0) {
    return 0x7ull << 61;
  }
}

__uint128_t pack(int x, int y, uint64_t t) {
  uint32_t ux = x;
  uint32_t uy = y;
  // return (static_cast<__uint128_t>(uy) << 96) |
  //        (static_cast<__uint128_t>(ux) << 64) | t;
  return static_cast<__uint128_t>(t) << 64 | (static_cast<uint64_t>(uy) << 32) |
         ux;
}

std::pair<int, int> unpack(__uint128_t v) {
  uint64_t xy = v; // v >> 64;
  uint32_t ux = xy;
  uint32_t uy = xy >> 32;
  return {static_cast<int32_t>(ux), static_cast<int32_t>(uy)};
}

int32_t main() {
  int N;
  cin >> N;
  for (int i = 0; i < N; ++i) {
    int x, y;
    cin >> x >> y;
    uint64_t t = tag(x, y);
    a[i] = pack(x, y, t);
  }
  // std::sort(a, a + N);
  pdqsort_branchless(a, a + N, [](__uint128_t a, __uint128_t b) {
    return static_cast<uint64_t>(a >> 64) < static_cast<uint64_t>(b >> 64);
  });
  for (int i = 0; i < N; ++i) {
    auto [x, y] = unpack(a[i]);
    cout << x << " " << y << "\n";
  }
}
