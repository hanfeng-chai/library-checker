// #include <algorithm>
// #include <array>
// #include <cstddef>
// #include <cstdint>
// #include <ranges>
// #include <span>
// #include <vector>
// #ifndef CANARD_DEBUG
// #define CANARD_DEBUG 0
// #endif
// #include "canard/algebra/actions/affine.h"
// #include "canard/algebra/monoids/sum.h"
// #include "canard/io.h"
// #include "canard/modular/montgomery_modular.h"
// #include "canard/tree/dual_segment_tree.h"
// using canard::io::input;
// using canard::io::output;
// using Count = canard::io::decimal::bounded_uint32_t<500'000>;
// using Kind = canard::io::decimal::bounded_uint32_t<1>;
// using Residue = canard::io::decimal::bounded_uint32_t<998'244'352>;
// using residue = canard::montgomery_modular<998'244'353>;
// constexpr std::size_t lookahead = 8;
// struct operation {
//     bool is_update;
//     std::uint32_t first;
//     std::uint32_t last;
//     std::uint32_t slope;
//     std::uint32_t intercept;
// };
// [[nodiscard]] operation read_operation(input &in) noexcept {
//     const auto kind = in.read<Kind>();
//     if (kind == 0) {
//         const auto [first, last] = in.read<Count, Count>();
//         const auto [slope, intercept] = in.read<Residue, Residue>();
//         return {.is_update = true,
//                 .first = first,
//                 .last = last,
//                 .slope = slope,
//                 .intercept = intercept};
//     }
//     const auto first = in.read<Count>();
//     return {.is_update = false,
//             .first = first,
//             .last = first + 1,
//             .slope = 0,
//             .intercept = 0};
// }
// void solve(input &in, output &out) {
//     const auto [size, count] = in.read<Count, Count>();
//     canard::tree::dual_segment_tree<canard::algebra::affine<residue>,
//                                     canard::algebra::sum<residue>>
//         tree(in.records<Residue>(size) | std::views::elements<0>);
//     constexpr std::size_t batch_size = 64;
//     std::array<operation, batch_size> operations{};
//     std::vector<std::uint32_t> answers;
//     answers.reserve(count);
//     const std::size_t operation_count = count;
//     for (std::size_t start = 0; start < operation_count;) {
//         const std::size_t current_count =
//             std::min(operations.size(), operation_count - start);
//         auto window = std::span{operations}.first(current_count);
//         for (auto &current : window) current = read_operation(in);
//         for (const auto [index, current] : std::views::enumerate(window)) {
//             if (const auto ahead = static_cast<std::size_t>(index) + lookahead;
//                 ahead < current_count) {
//                 tree.prefetch(operations[ahead].first, operations[ahead].last);
//             }
//             if (current.is_update) {
//                 tree.apply(current.first, current.last,
//                            {.slope = residue{current.slope},
//                             .intercept = residue{current.intercept}});
//             } else {
//                 answers.push_back(tree.get(current.first).value());
//             }
//         }
//         start += current_count;
//     }
//     out.write<Residue, "\n", "\n">(answers);
//     out.finish();
// }
// int main() {
//     input in{};
//     output out{};
//     solve(in, out);
// }

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <vector>
#include <tuple>
#include <utility>
#include <type_traits>
#include <concepts>
#include <bit>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <immintrin.h>
#include <functional>
#include <iterator>
#include <memory>
#include <string_view>
#include <string>
#include <cerrno>
#include <climits>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <xmmintrin.h>
#define z template
#define f const
#define F constexpr
#define m return
#define D noexcept
#define q requires
#define B class
#define P namespace
#define l using
#define X v<T,32
#define lt auto
#define my static
#define eq concept
#define bf inline
#define I operator
#define lx static_cast
#define mL v<T
#define lZ sizeof
#define Z struct
#define lL size()
#define lQ value_type
#define nJ ->g::same_as
#define nl value
#define nX H::K<a
#define pU H::K
#define lM av::ab
#define lI typename
#define at j a
#define lU G T
#define pV a<aY<T>&&aw<T
#define mf consteval
#define om 16>b
#define oO aw<T,4uz
#define nK g::get<0
#define on 16>a
#define mY c,16
#define ms 16uz
#define mt 1uz
#define pz br(am<p>{},aR<32
#define pW aW)(munmap(dq,bG));dq={};bG=0
#define lC z<B
#define R ah::
#define y ;}z
#define mm f lt
#define Y }P J::
#define C D{m
#define lD my F
#define mn z<lU
#define lG f j
#define mM z<at,lU>q(pV
#define lJ mL,16
#define mz g::ranges
#define qk j...b>(ao<b...>)D
#define mu F j
#define mo ;};z
#define mg b,a
#define oo f mL,om,f mL,on)D
#define mA lU>q
#define mh private
#define nY reinterpret_cast
#define id a>>
#define ql delete
#define lV lZ(T)
#define mZ H::al
#define is f au
#define oP aS::deallocate(bP,a,b)
#define lN f a&
#define mv false
#define lK ::lL
#define nm H::aP<a>([]
#define op ]<j...a>(ao<a...>
#define ph f mL,om
#define mC explicit
#define mD f nX>
#define mi public
#define ma b,f
#define nn &&q(f
#define ly j N
#define lW j b
#define mp friend
#define lT }D nJ
#define mj c,f
#define pi ->g::convertible_to<T>;
#define oq g::string_view
#define os g::countr_zero
#define mE X>b)D
#define oQ no_unique_address
#define na g::tuple
#define mF bf F
#define lO b){m
#define mG f X
#define mN for(j
#define nL a>eq
#define lR a){m
#define lS if(a
#define nM q(lZ...(a
#define no iterator
#define nN b.nl
#define lz F lt
#define pj g::allocator<au>>B
#define ot g::bit_cast
#define pA m H::as::hK<a>(H::bK<H::aJ
#define mO g::get
#define mP af::bX
#define mQ ax::ag
#define ou a.nl
#define oR H::as
#define mb Q<T
#define np z<aC a
#define nq aZ<T,N
#define mw f at
#define ov b=r,at=1>l
#define pB mL,on
#define mc f T
#define nr f r b
#define oS is_exhausted
#define oT cy::cb<b>(gU
#define pk g::derived_from
#define oU data
#define pC g::default_sentinel_t
#define ow g::memcpy
#define ns gE::aF
#define pX ak<ap::be
#define pl g::to_address
#define pD g::remove_cvref_t
#define pE g::has_single_bit
#define pF g::conditional_t
#define pY d=c.z load<a>(0);c.z store<a>(0
#define pZ g::is_nothrow_invocable_v<b&,a
#define oV fe.dL(0
#define qa q(a%2==1&&a>1&&a<(1u<<30
#define qb p.de,p.dW,p.cg?p.bw:0
#define dp ;lC
#define nZ lC nL
#define nb <R ae
#define lv ...>
#define lw ...)
#define W ;mm
#define lX lD j
#define nt lN d,lN e)
#define L );m
#define md ::max(
#define fd ));}
#define nu mg>>;q R
#define nv lC T
#define nw lC a
#define oa (mm&c,mm&b)
#define in {if(
#define lE a>&&
#define lF });}
#define lY )=ql
#define lP mg>
#define ob R ai
#define pG A&a,f M&ma O<A>&e,f O<M>
#define lA {mm
#define mq bf mu
#define oc R hu{mn
#define pH c,mA(mb,bm>&&Q<c,aO>)v
#define lH ;if(
#define oW mG>ma X>a)D
#define nO lM(mv);
#define nx ai<a
#define nP <mZ a>a
#define ny aq<V>::
#define nc O<a>
#define nd lD aV
#define od ou,nN)}
#define ne H::aE
#define nQ V,N
#define mB lG c
#define ox continue;
#define nR am<X>>
#define oy decltype
#define mx ax::
#define nS aq<T>::
#define mH true
#define mI pU<b
#define mJ ly>q
#define pm R cP<bc<N>>
#define oz =default
#define mr >(a)
#define oe ;},d,e)
#define nz a>Z
#define mR l lQ
#define of g::min
#define qc b,mA(mb,aO>&&(Q<b,ci>||Q<b,ar>)
#define oA nN,ou)}
#define pn q mb,hD>;mn
#define nA (d,c)
#define mk c,b
#define pI O<bE<id,bE<a>lK>
#define nB <nX>>
#define mS lG d
#define mT &&R
#define mU F ad
#define po ad ma ad c)
#define pp <g::uint32_t
#define og lZ...
#define nT R fu
#define nU first
#define pJ mz::input_range
#define oX ap::aB<V
#define nC a lK
#define pq b,at
#define nf lG b
#define ng lG e
#define nh lD r
#define pK allocator_type
#define oB oR::co
#define oC bo::bL
#define oD af::jw
#define nD q(mD
#define nE f ac
#define nF bf aW
#define oY nullptr
#define oh while
#define oi an<mY
#define nG aj<c
#define nH aj<a
#define pr R ee::hE
#define pL g::exchange(d
#define qd g::integral_constant<j,b
#define qe T&ma nq>&a){a*bY(b)+a;}my
#define qf g::invoke_result_t<b&,a
#define qg m H::cO<p>([](mm&d,mm&c
#define ps pB,mL,om
#define qh c,mA(mb,aO>&&Q<c,ar>)v
#define pt au('0'+a
#define pM g::integral
#define oZ lV==mt
#define qi v<b,32>bI(mG>mj X>a)D
#define pN lN b,lN c)D
#define pa for(lW
#define oE mF aV
#define pb z cT<N
#define pc z cx<N
#define oF J::io
#define pO lx<ac>(R hN
#define pd enum B
#define pP k.subspan(w
#define pQ g::size_t N
#define me dp nL
#define ml ;}dp
#define lB ap::
#define nI dp T
#define mV z nb
#define oG [&]<qk{
#define nV dp ls
#define nW lX lL
#define oj dp pq
#define mW oU()
#define mK )f{m
#define pe (pX<V>>{
#define ni a lv
#define mX a);}
#define nj };Z
#define nk (X>a
#define oH lZ(nc)
#define oI a,lx<r>
#define pf am<lJ>>
#define ok bF<nQ
#define oJ mx bQ::
#define pg bp.fX()
#define pR )lJ>bD(ph)D;
#define qj bN<O<id...c>q(og(c)==nC
#define ol dn<N>
#define oK aZ<nQ>
#define pu ao<b lv)
#define pv lZ(O<b>)
#define pw ob<an<lP
#define px ob<aL<lP
#define pS )lJ>aR(ph)D
#define oL pU<nx
#define oM mZ<lE
#define py };lM(av::
#define pT z<lW,B...nz
#define oN gy.dL
P g=std;l aV=bool;l ac=uint64_t;l j=size_t;l ci=int8_t;l bm=int32_t;l aO=int16_t;l ar=uint8_t;l hD=uint16_t;l r=uint32_t;l aW=void;l au=char;l jr=uintmax_t;l hy=ssize_t nV,B lu>eq Q=g::same_as<ls,lu>nV>l aM=g::numeric_limits<ls>nV>l iL=g::make_unsigned_t<ls>nV>l am=g::type_identity<ls>;z<j...ls>l ao=g::index_sequence<ls lv;z<j ls>l ak=g::make_index_sequence<ls>nV,j lu>l bd=g::array<ls,lu>nV>l bE=g::remove_cv_t<ls>nV>eq cX=g::unsigned_integral<ls>dp...ls>l cD=na<ls lv;z<j ls,B lu>l fI=g::tuple_element_t<ls,lu>nV,B lu>eq iy=g::convertible_to<ls,lu>;l hn=pC;P J{nv>eq ff=mb,pD<T>>nI,B U>eq bN=Q<pD<T>,pD<U>>nI>eq bq=ff<T>&&pM<T>&&!mb,aV>&&lV<=lZ(ac)nI>eq G=bq<T>&&pE(lV)nI>l O=lI T::lQ;Y ap{P kE{Z iw{nj iM{};}lC S>eq hG=q(f S&a,f O<S>&b){{a.combine(b,b)}nJ<O<S>>ml M>eq hC=hG<M>nn M&a){{a.dY()}nJ<O<M>>;};};
#define aa inline __attribute__((__always_inline__))
#define bu __attribute__((__flatten__))
l kk=__m128i;P J::R hu{P jT{Z lb{nd jS=mv;lX jC=ms;nj kF:lb{nd gg=mH;nj kj:kF{nj jU:kj{nj kG:jU{lX ft=32uz;};l iY=kG;}l J::G;mn,at>Z v;mn>mq aY=aM<iL<T>>::digits nI,at>eq aw=G<T>&&lV==a dp nz cv{mR=aV;a kH;nW D;};mn>Z lJ>{mR=T;l fg=cv<v>;kk nl;nW C ms/lV;}};mn>Z X>{mR=T;l fg=cv<v>;__m256i nl;nW C 32uz/lV;}nj jE:jT::iY{nd eK=gg;mn,at>l iZ=mL,of(a,ft)>;};Y oc>q(oZ)lJ>dE(pf,T lR{_mm_set1_epi8(ot<ci mr)}y<mA(oZ)X>dE(nR,T lR{_mm256_set1_epi8(ot<ci mr)}y<mA(lV==4uz)X>dE(nR,T lR{_mm256_set1_epi32(ot<bm mr)};}Y oc,bN<T>...a>nM)==lJ>lK&&oZ)lJ>iu(pf,a...lO{_mm_setr_epi8(ot<ci>(b)lw}y<lU,bN<T>...a>nM)==lJ>lK&&lV==2uz)lJ>iu(pf,a...lO{_mm_setr_epi16(ot<aO>(b)lw}y<lU,G U>lJ>br(pf,f v<U,on){m{ou}y<lU>X>ja(nR){m{_mm256_setzero_si256()}y<lU>X>hH(nR)lA a=ja(nR{}L{_mm256_cmpeq_epi8(ou,ou)}y<lU,G U>X>br(nR,f v<U,32>lR{ou};}Y oc>lJ>I&(ps)C{_mm_and_si128(od y<lU>lJ>I|(ps)D;mn>X>I&nk,X>b)C{_mm256_and_si256(od y<lU>X>I|nk,mE;mn>X>I^nk,X>b)C{_mm256_xor_si256(od y<lU>X>I~nk)C hH(nR{})^a y<lU,at>cv<mL,id I|(f cv<mL,id ma cv<mL,id c)D;Y oc>lJ>I*(ps)D q(aw<T,2uz>);mM,2uz>pR mM,4uz>pR mM,8uz>pR mM,2uz>pS;mM,4uz>pS;mM,8uz>pS dp qh<mY>aD(oo dp qc)v<b,16>bI(f lJ>mj pB)D dp pH<mY>aD(ph,f pB)C{_mm_packs_epi32(oA;}lC pH<mY>bI(oo;mn>lJ>aG(oo pn>lJ>bT(oo pn>v<ac,16>bU(oo q mb,r>;mn>lJ>aG(oo q mb,ac>{m{_mm_unpacklo_epi64(oA y<at>q(a<ms)kk eL(kk lO _mm_srli_si128(b, bm(a))y<at,mA(a<lJ>lK&&(aw<T,mt>||aw<T,2uz>||oO>))T ik(mL,om)lA c=a==0?nN:eL<a*lV>(nN L ot<T>(lx<iL<T>>(_mm_cvtsi128_si32(c)fd bf v<bm,16>bA(v<aO,om,v<aO,on)C{_mm_madd_epi16(oA y<at,mA(a==ms&&mb,ar>)lJ>bv(f lJ>c,ph)C{_mm_shuffle_epi8(c.nl,nN)};}bf v<aO,16>bs(v<ar,om,v<ci,on)C{_mm_maddubs_epi16(oA y<lU>lJ>I*(ps)D q(oO>);mn>X>I+nk,mE q(oO>){m{_mm256_add_epi32(od y<lU>X>I-nk,mE q(oO>){m{_mm256_sub_epi32(od y<lU>X>I*nk,mE q(aw<T,2uz>);mn>X>I*nk,mE q(oO>){m{_mm256_mullo_epi32(od y<at,mA(pV,2uz>)X>bD(f mE;mM,4uz>)X>bD(f mE;mM,8uz>)X>bD(f mE;mM,2uz>)X>aR(f mE;mM,4uz>)X>aR(f mE;mM,8uz>)X>aR(mG>b)C{_mm256_srli_epi64(nN,bm(a))};}lC qh<c,32>aD(oW dp qc)qi dp pH<c,32>aD(oW dp b,mA(mb,bm>&&(Q<b,aO>||Q<b,hD>))qi;mn>q mb,r>X>gh(mG>a,mG>b)C{_mm256_min_epu32(od y<mA mb,r>X>il(mG>a,f mE;mn>X>aG(oW pn>X>bT(oW pn>v<ac,32>bU(oW q mb,r>{m{_mm256_mul_epu32(oA y<mA mb,r>X>fB(mG>a,mG>lO{_mm256_blend_epi32(a.value, b.value, 0b1010'1010)}y<lU>X>bT(mG>i,mG>e)D q mb,r>{l w=v<ac,32>;l p=v<r,32>W a=bU(i,e)W d=pz>(br(am<w>{},i)))W c=pz>(br(am<w>{},e)))W b=bU(d,c L fB(pz mr),br(am<p>{},b))y<at,mA(a==ms&&mb,ar>)X>bv(mG>mj mE;bf v<aO,32>bs(v<ar,32>b,v<ci,32>a)D;bf v<bm,32>bA(v<aO,32>b,v<aO,32>a)D;Y R H{l gz=hu::jE;Y ah{nv>eq bS=G<T>;mq fh=H::gz::ft;z<bS T>mq hv=fh/lV;P H{Z hI{}dp a>l ix=lI a::fg;z<bS T,ly>l dZ=gz::iZ<T,lV*N>nI,ly>eq cc=bS<T>&&gz::gg&&pE(N)&&(lV*N==ms||lV*N==32uz||lV*N==64uz||lV*N==128uz)dp b,B...nL fJ=pZ lv&&g::is_trivially_copyable_v<qf...>>dp a,j...b>q(og(b)>0&&(fJ<a,qd>>&&lw)lt bK(a&c,pu{m g::array{g::invoke(c,qd>{})...}y<j c,B a>q(c>0)lt bK(a lO bK(b,ak<c>{lF nw,lW,B...c>q(b>0&&og(c)>0&&fJ<a,f c&lv)lt bV(a d,f bd<mk>&...e){m bK<b>([&](lt p)C g::invoke(d,e[p]lw;lF Z as y<bS T,ly=hv<T>>q H::cc<T,N>B fu:mi H::hI{mh:l hJ=H::dZ<T,N>;bd<hJ,N/hJ lK>cw;mp Z oR;mi:fu()D{}mR=T;nW C N;}}me aC=pk<bE<a>,H::hI>&&q{lI O<bE<id;q H::cc<pI;{bE<a>lK}nJ<j>ml a,B...b>eq dm=aC<lE(aC<b>&&lw&&((oH*nC==pv*b lK)&&lw;P H{np>l cY=fu<pI;np>l K=dZ<pI;np>l gA=ix<K<id;np>mq aJ=bE<a>lK/K<a>lK;np>l hw=bd<K<a>,aJ<id;np>l fv=bd<gA<a>,aJ<id me dS=aC<lE!g::is_volatile_v<lE pk<bE<a>,cY<id&&g::is_nothrow_destructible_v<a>me by=dS<lE ff<lE g::is_nothrow_default_constructible_v<lE g::is_nothrow_move_constructible_v<a>me gB=aC<lE gz::eK me aE=gB<lE dS<a>me al=aE<lE by<a>dp b,B c,B...nL dF=pZ lv&&Q<qf lv,c>y<aC a>B ea{mh:H::fv<a>cw;mp Z oR;mi:ea()D{}mR=aV;nW D;};P H{Z as{z<by a>my a hK(hw<a>c){a b{};lx<cY<a>&>(b).cw=c;m b y<dS a>my mm&co(lN lO lx<f cY<a>&>(b).cw y<aC b>my ea<b>im(fv<b>c){ea<b>a{};a.cw=c;m a y<aC b>my mm&co(f ea<b>&lR a.cw;}};}Y ah{lC b,B nL eM=aC<b>&&Q<O<b>,a>dp a,B...b>eq hq=aC<lE(aC<b>&&lw&&((nC==b lK)&&lw;z<aC b,bS a>l ai=fu<a,pv*b lK/lZ(a)>;np>l eb=fu<nc,nC/2uz>;P H{lC b,B nL gj=al<b>&&aE<lE oH==pv*2uz&&dm<lP&&aJ<b> ==aJ<a>dp a,B b,B e>eq cC=q(lN d,lN c){{bI<e>nA lT<b>ml a,B b,B e>eq cI=q(lN d,lN c){{aD<e>nA lT<b>;};}nZ gX=ne<lE q{lI eb<a>;}&&mZ<eb<id oj>eq bJ=eM<b,ar>&&mZ<b>&&a==ms&&mI>lK%a==j{0}nn mI>&d,f mI>&c){{bv<a>nA lT<mI>>ml a,B b>eq bn=aC<lE bS<b>&&H::gj<nx,b>,lE H::cC<nX>,oL,b>>,b>&&H::cI<nX>,oL,b>>,b>me cS=oM cX<O<id&&nD&c,mD&b){{bT(mk)lT nB;}me bO=eM<a,r>&&ne<lE mZ<nx,ac>>&&nD&c,mD&b){{bU(mk)lT<oL,ac>>>ml pq>eq bB=mZ<b>&&cX<O<b>>&&a==ms&&(mI>lK*pv)%a==j{0}nn mI>&d,f mI>&c){{aG nA lT<mI>>;}me cN=oM nD&b,mD&c){{b*c lT nB;}me cJ=oM nD&b){{bD<0>(b)lT nB;{aR<0>(b)lT nB;}me fK=eM<a,ar>&&ne<lE nD&mj oL,ci>>&b){{bs(mk)lT<oL,aO>>>;}me fL=eM<a,aO>&&ne<lE nD&b){{bA(b,b)lT<oL,bm>>>;};P H{z<by b,B a>q dF<a,K<b>,f K<b>&>b aP(a mj b&d){m as::hK<b>(bV(c,as::co(d)))y<by a,B b>q dF<b,K<a>,f K<a>&,f K<a>&>a aP(b c,nt{m as::hK<a>(bV(c,as::co(d),as::co(e)))y<by b,dS...a,B c>nM)>0&&dm<mg lv&&((aJ<b> ==aJ<a>)&&lw&&dF<c,K<b>,f K<a>&lv)b cO(c d,lN...e){m as::hK<b>(bV(d,as::co(e)lw);}}nw,lW>eq fM=oM H::aJ<a> ==mt&&nD&c){{part_shift_lanes_down<b>(c)lT nB;};P H{np>l fC=pF<(aJ<a> >mt),K<a>,K<eb<id>dp a>l cj=dZ<ac,nC*oH/lZ(ac)>me fD=(oH==8uz||(oH==4uz&&q(lN b){{part_gather_even_odd_in_groups(b)lT<a>;}))nn cj<a>&mj cj<a>&b){{aG(mk)lT<cj<id;{part_interleave_high_in_groups(mk)lT<cj<id;}&&(nC*oH==ms||q(lN b){{part_reorder_narrowed_groups(b)lT<a>;lF nZ fE=gX<lE H::fD<H::fC<id;z<at,bJ<a>b>b eN(f b&p,f b&e){m H::aP<b>([](f mI>&mj mI>&d)C bv<a>(c,d);},p,e)y<lW,bS e,bn<e>a>q(b==ms)nx,e>ec(lN i,lN w){l p=nx,e>;m H::cO<p>([](mD&d,mD&c)C aD<e>nA;},i,w)y<cS a>a hL(nt{m nm(mD&c,mD&b)C bT(mk)oe y<lW,bB<b>a>a eo(lN e,lN p){m nm(mD&d,mD&c)C aG nA;},e,p)y<fK a,eM<ci>b>q(ne<b>&&hq<a,b>)lt eE(lN i,f b&e)->nx,aO>{l p=nx,aO>;qg)C bs nA;},i,e)y<fL a,eM<aO>b>q(ne<b>&&hq<a,b>)lt cZ(lN i,f b&e)->nx,bm>{l p=nx,bm>;qg)C bA nA;},i,e)y nP I~(lN b)C nm(mm&c)C~c;},b)y nP I&(nt C nm oa C c&b oe y nP I|(pN;z nP I^(nt C nm oa C c^b oe y nP I+(nt C nm oa C c+b oe y nP I-(nt C nm oa C c-b oe y<cN a>a I*(nt C nm oa C c*b oe;}Y ah{z nP cP(nc c)lA b=dE(am nB{},c);pA<id([b](lt)C b;}fd Y oc>q(oZ)lI X>::fg fN nk,X>lO{{_mm256_cmpgt_epi8(oA}y<mA(g::signed_integral<T>&&oZ)lI X>::fg I<nk,X>b)C fN(a,b);}Y R hu{Y ah{nZ dw=ne<lE nD&b,mD&c){{b<c lT<H::gA<id mo<dw a>ea<a>I<(nt C oR::im<a>(H::bV([]oa C c<b;},oB(d),oB(e)fd Y R hu{z<at,j...b>F ac hx(nE c,pu{m(ac{}|...|(((c>>(b*a))&mt)<<b))y<lU>bf ac hM(f cv<X>>&b)lA a=ac(lx<r>(_mm256_movemask_epi8(b.kH.nl))L hx<lV>(a,ak<X>lK>{lF Y ah{z<H::gB b>ac hM(f ea<b>&c)lA&d=oB(c L[&op)C(ac{}|...|(hM(d[a])<<(a*mI>lK)fd(ak<H::aJ<b>>{lF Y ah{P H{}z<dw a>ea<a>I==(pN;P H{}Y ah{P H{z<al a,lW,qj&&b<aJ<a>)K<a>jo(f c&...e)lA p=g::tie(e lw;m[&]<j...d>(ao<d lv)C iu(am<K<id{},mO<b*K<a>lK+d>(p)lw;}(ak<K<a>lK>{lF}z<mZ a,qj)aa a jp(c...d){pA<id([&](lt b)C H::jo<a,oy(b)::nl>(d lw;}))y<mZ a,ne b>q dm<a,b>a gY(f b&c){m oR::hK<a>(H::bV([](mm&d)C br(am nB{},d);},oB(c)))y<j c,ne a>q(c<nC)nc hN(lN d)D{mu b=c/nX>lK;m ik<c%nX>lK>(oB(d)[b]);}Y oc>lJ>hO(pf,f aW*lR{_mm_loadu_si128(nY<f kk*mr)}y<lU>X>hO(nR,f aW*lR{_mm256_loadu_si256(nY<f __m256i*mr)};}Y ah{z nP load(f nc*b)D{l p=nX>;pA<id([b](lt c)C hO(am<p>{},b+c*p lK);}fd Y ah{P H{nw>mF a jq=lx<a>(~g::make_unsigned_t<a>{})dp a,ly>mf bd<bd<a,N>,N+1>ip(){bd<bd<a,N>,N+1>c{};pa=0;b<=N;++b)mN d=0;d<b;++d)c[b][d]=jq<a>;m c;}}np>a gC(nf){lD lt c=H::ip<nc,nC>(L load<a>(c[b].oU(fd Y ap{lC V>Z aq dp V>eq cp=q{lI ny ce mo<cp V>l aB=lI ny ce;z<cp V>mq be=g::tuple_size_v<aB<V>>;P es{nw,ly>Z dT dp...E,ly>Z dT<cD<E lv,N>{l jF=cD<nT<E,N>lv;}y<cp V,ly>l bF=lI es::dT<aB<V>,N>::jF;P es{nw,ly>oE dN=mv dp...E,ly>oE dN<cD<E lv,N> =(q{lI nT<E,N>;}&&lw;}lC V,ly>eq bb=cp<V>&&es::dN<aB<V>,N>&&q{lI ny z ol ml V,mJ bb<nQ>l aZ=lI ny z ol;z<ly,cp V>q bb<nQ>oK eO(f V&c){f aB<V>b=ny fP(c L[&op)C ny pb>(ok>{R cP<fI<a,ok>>>(mO<a>(b))...lF(ak<be<V>>{lF P es{nw>oE ep=mv dp...a>oE ep<cD<a...>> =(R fE<lE lw;}P es{nw,lW>oE fY=mv dp...a,lW>oE fY<cD<ni,b> =(R fM<a,b>&&lw y<cp V,mJ bb<nQ>oK iq(lG k,lG s,f oK&n,f oK&t){f ok>e=ny pc>(n);f ok>o=ny pc>(t)W p=[]<B b>(mB,mS,f b&i,f b&a)D{f b w=R gC<b>(d)&~R gC<b>(c L a^((i^a)&w);};m[&op)C ny pb>(ok>{p(k,s,mO<a>(e),mO<a>(o))...lF(ak<be<V>>{lF P es{}Y ap{nv>eq dx=g::regular<T>nn T&a,mc&b){{a+b}pi{a-b}pi{a*b}pi{-a}pi T(ac{lF nI>eq gZ=q(mc&a,nE b){a.jm(b)mo<dx T>F T jb(mc&a,nE b);z<dx T>q gZ<T>F T jb(mc&a,nE lO a.jm(b);}Y ap{z<dx T>Z kl:kE::iw,kE::iM{mR=T;lD T dY(){m T{};}lD T combine(mc&a,mc&lO a+b y<mJ bb<T,N>my bf nq>jG(f nq>&a,f nq>&b);lD T kI(mc&a);lD T lc(mc&a,nE b);};Y ap{nv>Z bi{T ed{T(mt)};T bZ{};mp F aV I==(f bi&,f bi&)oz mo<dx T>Z jV{mR=bi<T>;lD lQ dY(){m{};}lD lQ combine(f lQ&ma lQ&lR jH(mg);}lD g::pair<T,T>ld(f lQ&a);lD lQ ll(mc&ma T&a);lD T gk(f kl<T>&,f lQ&ma T&mj ac lR b.ed*c+(a==1?b.bZ:jb(b.bZ,a fd Z eF{lQ kJ;oy(bY(g::declval<mc&>()))ed;};lD eF cq(f lQ&b)q q(mc&a){bY(mX{m{b,bY(b.ed)}y<mJ bb<T,N>nn qe nq>gD(f kl<T>&,f eF&a,f nq>&ma ac c){m b*a.ed+eO<N>(jb(a.kJ.bZ,c))y<mJ bb<T,N>nn qe bi<nq>>fO(f bi<nq>>&ma eF&lR{b.ed*a.ed,b.bZ*a.ed+eO<N>(a.kJ.bZ)};}mh:lC U>lD bi<U>jH(f bi<U>&ma bi<U>&a)mo<cp T>q(be<T> ==1)Z aq<bi<T>>{l iN=fI<0,aB<T>>;l ce=cD<iN,iN>;lD ce fP(f bi<T>&lR{nK>(nS fP(a.ed)),nK>(nS fP(a.bZ))};}lD bi<T>jh(f ce&lR{nS jh(na{nK mr}),nS jh(na{mO<1 mr})}y<ly>l dn=bi<nq>>;z<ly>my ol cT(f lI es::dT<ce,N>::jF&lR{nS pb>(na{nK mr}),nS pb>(na{mO<1 mr})}y<ly>my lI es::dT<ce,N>::jF cx(f ol&lR{nK>(nS pc>(a.ed)),nK>(nS pc>(a.bZ))};}};Y gE{z<mJ(N>0)Z aF{bd<au,N>hr{};mf aF(is(&a)[N])D{mz::copy(a,hr.begin(fd nW C N-1;}nd empty()D;F au*mW D;F is*mW f C hr.mW;}F is*c_str()f D;F oq ji(mK{mW,lL};}F g::string lp()f mo<Q<au>a>aF(a)->aF<2>;}
#define dl MAP_ANONYMOUS
P oF{;Z bg{is*kK;j kL;lX eP=8;lX eQ=64;};P av{nF ha(){g::abort();}nF ab(f aV a)in!a){ha();}}bf aV fi(lG mj at,j&b){lS>aM<j>md)-c)m mv;b=c+a;m mH;}bf aV hs(lG mj at,j&e){ab(a!=0);nf=c%a;mS=b==0?0:a-b;m fi(c,d,e);}bf j hP(){f intmax_t a=sysconf(_SC_PAGESIZE);ab(a>0)W b=jr(a);ab(b<=jr(aM<j>md))L j(mX}nZ bj=Q<nc,au>;z<bj t=pj aU{mi:l cK=t;l aS=g::allocator_traits<cK>;l gF=lI aS::pointer;mC aU(FILE*d,f cK&c=cK{})D:bP(c){lM(d!=oY);f bm a=::fileno(d);lM(a>=0);Z stat b{};f bm e=::fstat(a,&b);lM(e==0)lH S_ISREG(b.st_mode))fZ(a,b);else gl(mX~aU()D in bG!=0){f bm a=munmap(dq,bG);lM(a==0);}}aU(f aU&lY;aU&I=(f aU&lY;aU(aU&&lY;aU&I=(aU&&lY;bg ji(mK jI;}mh:l hb=au*;Z hc{j size;j lm;};aW dO(f hc p)lA[i,w]=p;at{py fi(i,bg::eQ,a));j d{py hs(a,w,d));lW{py hs(bg::eP,w,b));j c{py fi(b,d,c));aW*e=mmap(oY,c,PROT_READ|PROT_WRITE,MAP_PRIVATE|dl,-1,0)lH e==MAP_FAILED||e==oY)nO dq=(au*)(e);bG=c;hd=dq+b;jI=bg{hd,i};}aW fZ(f bm e,f Z stat&i){lM(i.st_size>=0)W p=jr(i.st_size);lM(p<=jr(aM<j>md)));lG w=j(p);lG k=av::hP();dO({w,k});mw=w-(w%k);lS!=0){aW*d=mmap(hd,a,PROT_READ,MAP_PRIVATE|MAP_FIXED,e,0)lH d==MAP_FAILED||d!=hd){(pW;nO}}nf=w-a lH b==0)m;lM(a<=j(aM<off_t>md)));lM(k<=j(aM<hy>md)));j c{};oh(c<b){f hy d=pread(e,hd+a+mk-c,off_t(a+c))lH d<0)in errno==EINTR)ox(pW;nO}lM(d>0);lM(j(d)<=b-c);c+=j(d);}}aW gl(f bm s){mu w=64uz*1024uz;mB=aS::max_size(bP);lG p=j(aM<hy>md));lG i=c<p?c:p;lW=w;lM(b<=i);gF a=aS::allocate(bP,b);lS==gF{})nO au*o=pl(a);j k{};for(;;)in k==b){mS=b<=i/2?b*2:i lH d<=b){oP;nO}gF e=aS::allocate(bP,d)lH e==gF{}){oP;nO}au*u=pl(e);ow(u,o,k);oP;a=e;o=u;b=d;}f hy n=read(s,o+k,b-k)lH n<0)in errno==EINTR)ox oP;nO}if(n==0)break;lM(j(n)<=b-k);k+=j(n);}dO({k,av::hP()})lH k!=0)ow(hd,o,k);oP;}[[oQ]]cK bP{};hb dq{};j bG{};au*hd{};bg jI{}mo<bj d=pj aH{mi:l cL=d;l ck=g::allocator_traits<cL>;l iO=lI ck::pointer;lX da=64uz*1024uz;mC aH(FILE*c,at=da,f cL&b=cL{})D:dU(-1),et(b),fj(a){lM(c!=oY);lM(a!=0);lM(a<=ck::max_size(et));dU=::fileno(c);lM(dU>=0);hQ=ck::allocate(et,fj);lM(hQ!=iO{lF~aH()D{jc();ck::deallocate(et,hQ,fj);}aH(f aH&lY;aH&I=(f aH&lY;aH(aH&&lY;aH&I=(aH&&lY;bf au*kM(mw)D;bf j kA()f D;j ga(mK fj-gm;}nF kY(au*a);nF js(mw);au*he(){m pl(hQ)+gm;}aW gG(mw){gm+=a;}aW jc()in gm==0)m;au*p=pl(hQ);lW{};oh(b<gm){mw=gm-b;ng=a<j(aM<hy>md))?a:j(aM<hy>md));f hy c=write(dU,p+b,e)lH c<0)in errno==EINTR)ox nO}lM(c>0);lM(j(c)<=a);b+=j(c);}gm=0;}mh:bm dU;[[oQ]]cL et;iO hQ{};j fj;j gm{};};Y ah{P ee{Z jj{nd cr=mv;nj hE:jj{nd cr=mH;};}nZ ae=pk<a,ee::jj>&&q{q Q<oy(a::cr),f aV>;lI g::bool_constant<a::cr>;};Y oc>aW iP(mG>b,aW*a){_mm256_storeu_si256(nY<__m256i*mr,nN);}Y ah{P H{}z<ne b>aW km(O<b>*mk d)D{l p=mI>W&e=oB(d);[&op)D{(iP(e[a],c+a*p lK),lw;}(ak<H::aJ<b>>{lF Y ah{nZ dz=oM nD&b,mD&c){{gh(b,c)lT nB;{il(b,c)lT nB;}me gi=dz<a>||(oM dw<a>);z<gi a>a kN(pN;z<gi a>q dz<a>a kN(nt C nm oa C gh(mk)oe;}Y R hu{nF hR(f aW*a){_mm_prefetch((const char *)(a), _MM_HINT_T0);}Y ah{nF jt(f aW*a){hu::hR(mX Y io::mx gH{z<Q<ci>a>l eu=nT<a,64>me db=Q<a,ci>mT H::cc<a,64>mT dw<eu<id;z<db b>bf ac cl(is*d){l p=eu<b>W c=R load<p>(nY<f b*>(d))W a=R cP<p>(b{33}L R hM(c<mX lC pq>l an=nT<lP oj>l cs=pw,aO>oj>l fQ=ob<cs<lP,bm>oj>l bW=pw,bm>oj>l fF=pw,ac>oj>l fG=ob<bW<lP,r>oj>eq az=Q<b,ar>&&(a==ms||a==32uz)mT H::cc<lP mT fK<an<mg>>mT fL<cs<mg>>mT bn<fQ<lP,aO>mT bJ<an<lP,16>me dr=az<a,32>mT bO<fG<a,32>>mT cJ<fF<a,32>>;mF ar fk=ar{128};mq cm=20;z<j d,j c>mf bd<ar,16>er(){bd<ar,on{};a.fill(fk)W e=c<d?c:d W p=d-e;pa{};b<e;++b)a[p+b]=ar(b L a y<lW>mf lt eR(){m[op)C bd<bd<ar,16>,cm+1>{er<lP()...};}(ak<cm+1>{lF mF lt fl=eR<8>();mq ef=ms;z<at>q(a==8uz)F mm&gn(){m fl y<j c,at=0>q((c==8uz||c==ms)&&(a==0||(a>0&&a<=c)))F f bd<ar,16>&hf(nr){mS=a>0?a:b;m gn<c>()[d];}lC c=ar,j d=8,at=0>q((d==8uz||d==ms)&&(a==0||(a>0&&a<=d))&&az<mY>)bf oi>cE(is*k,nr)lA p=b>d?b-d:0 W*n=nY<f c*>(k+p)W i=R load<oi>>(n)W e=R load<oi>>(nY<f c*>(hf<d,a>(b).mW))W w=R eN<16>(i,e L w&R cP<oi>>(ar{15 lF nw,J::nc d,J::nc c>bu aa a cF(){m[]<j...b>(pu C R jp<a>(((b%2uz==j{0})?d:c)lw;}(ak<nC>{lF lC pq>q az<lP bf bW<lP fR(f an<lP d){l p=cs<lP W i=R eE(d,cF<pw,ci>,ci{10},ci{1}>())W c=R cZ(i,cF<p,aO{100},aO{1}>())W e=R ec<16,aO>(c,c L R cZ(e,cF<p,aO{10000},aO{1}>(fd lC ov aL=nT<mg*4>dp ov dP=px,ar>dp ov fm=px,ac>dp ov bz=px,hD>dp ov dV=px,bm>dp ov eg=ob<bz<lP,aO>dp b=r,at=1>eq cz=Q<b,r>&&(a==mt||a==2uz)mT H::cc<mg*4>&&q{q R mZ<aL<nu mZ<dP<nu bO<aL<nu cN<aL<nu cJ<aL<nu cJ<fm<nu cN<bz<nu cJ<bz<nu cS<bz<nu bn<dV<lP,aO>;q R bn<eg<lP,ar>;q R bB<bz<lP,ef>;q R bJ<dP<lP,ef>ml c=ar,at=0>q az<mY>bf ac go(is*e,nr)lA d=cE<c,8,a>(e,b L pO<0>(fR<mY>(d)fd lC c=ar,j i=0,j d=0>q az<mY>mT bB<ob<oi>,ac>,16>bf bW<mY>gp(is*w,nr,is*e,f r a){l p=ob<oi>,ac>W n=R gY<p>(cE<c,8,i>(w,b))W k=R gY<p>(cE<c,8,d>(e,a)L fR<mY>(R gY<oi>>(R eo<16>(n,k)fd Y io::ax{l J::bq;P bQ{Z dA{}y<bq T,T a,T b>q(a<=b)Z dG:bQ::dA{mR=T;lD T eS=a;lD T eT=b;};P bQ{nv>eq ct=ff<T>&&pk<T,dA>&&q{q bq<O<T>>;q Q<oy(T::eS),f O<T>>;q Q<oy(T::eT),f O<T>>;lI dG<O<T>,T::eS,T::eT>ml T,T...nz bH;z<bq T,T nz bH<T,a>{l ke=dG<T,aM<T>::lowest(),a>ml T,T...nL dc=q{lI bH<T,ni::ke;};}nv,T...a>q bQ::dc<T,ni l gq=lI bQ::bH<T,ni::ke nI>eq ag=bq<T>||bQ::ct<T>dp nz bk;z<bQ::ct nz bk<a>:dG<nc,a::eS,a::eT>{};z<ag a>l aj=O<bk<id;z<r...a>q bQ::dc<r,ni l dd=gq<r,ni;Y io::ax{Z bt{is*oU;r iQ mo<ly>l bl=bd<bt,N>;P bQ{mV a,Q<ci>b=ci>bf ac cl(is*c);mV a,Q<ci>b=ci>q(a::cr&&gH::db<b>)bf ac cl(is*c){m gH::cl<b>(c)y nb c>B fw{mi:mC fw(is*a,nf)D:dB(a),dC(a),dy(a+b),hz(a)in b!=0)gI()y<mJ(N>0)aa bl<N>kf()in dH<N>(aQ))m iR<N>();bl<N>a{};oG((a[b]=ju()),lw;}(ak<N>{}L a;}mh:lX cf=64;is*dB;is*dC;ac aQ{};is*dy;is*hz;j fn()f lA a=j(dy-dB L a<cf?a:cf;}aW gI()lA a=fn();aQ=cl<c>(dB);lS<cf)aQ&=ir(mX lD ac ir(mw){lS==0)m 0;lS==cf)m aM<ac>md L(mt<<a)-mt y<mJ(N==0)nd dH(ac){m mH y<mJ(N>0)nd dH(ac lR a!=0&&dH<N-1>(a&(a-1 fd bt fS(is*b,is*d)f lA a=j(d-b L{b,r(a)}y<mJ(N>0)aa bl<N>iR(){bl<N>d{};lt a=aQ;is*b=dC;[&]<j...e>(ao<e lv)D{(([&]()D lA i=os(a);a&=a-mt;is*p=dB+i;d[e]=fS(b,p);b=p+1;}()),lw;}(ak<N>{});dC=b;aQ=a;m d;}bt ju(){is*a=dC;oh(mH)in aQ!=0)lA e=os(aQ);aQ&=aQ-mt;is*b=dB+e;dC=b+1;m fS(a,b);}mm d=j(dy-dB)lH d>cf){dB+=cf;dC=dB;gI();ox}dC=dy;aQ=0;m fS(a,dC);}}};}pd bo:ar{bL,jv,kB};pd af:ar{jw,jJ,hS,bX,hg,jx};pd aN:ar{kO,le nj eU{bo dI;af dW;aN de;ar jD;ar bw;ar cA;aV cg ml T>l cU=iL<T>;z<cX T>F cU<T>ds(T lR a;}nv>Z hh{aV fo;cU<T>ds mo<bo a,bq T>q(a==oC)F hh<T>dJ(T lO{mv,lx<cU<T>>(b)}y<ag c>mf cU<nG>>eh(){l T=nG>;F T d=bk<c>::eS;F T e=bk<c>::eT;lz a=ds(d);lz b=ds(e L a>b?a:b y<ag a>mf cU<aj<id gr(){l T=nH>;F T b=bk<a>::eS;F T c=bk<a>::eT;{m 0;}}mf ar fx(ac b){ar a=1;oh(b>=10){b/=10;++a;}m a;}mf af hi(ar a){lS<=1)m oD;lS<=4)m af::jJ;lS<=8)m af::hS;lS<=10)m mP;lS<=16)m af::hg;m af::jx y<ag b>mf eU gs(){l T=aj<b>;F T p=bk<b>::eS;F T i=bk<b>::eT;lz c=fx(gr<b>());lz a=fx(eh<b>());F bo d=[]()C oC;}();lz e=eh<b>()<=aM<r>md)?aN::kO:aN::le;m{d,hi(a),e,c,a,lx<ar>(a+(d==oC?0:1)),c==a}y<ag a>mF eU aI=gs<a>();z<ag b>F aW hj(bt a){(aW)a;}Z bx{is*gJ;r fT;aV hk mo<bo b>q(b==oC)aa F bx df(bt lR{a.oU,a.iQ,mv};}F r bh(is*lR r(ar(a[0])-ar('0' fd mF r eG=4u;F r cB(is*d,r a)lA e=bh(d);lS==1u)m e W p=bh(d+mt)W c=e*10u+p;lS==2u)m c W w=bh(d+2uz)W b=c*10u+w;lS==3u)m b W i=bh(d+3uz L b*10u+i;}mF ac gK=100000000uz;mF ac gb=0x2386f26fc10000u;;;z<aN a>l aA=pF<a==aN::kO,r,ac>;z<r b,af a,r c>q((b==8&&a<=af::hS)||(b==16&&a<=af::hg))aa ac bC(is*,r){m 0 y<r b,af c,r d>q(!((b==8&&c<=af::hS)||(b==16&&c<=af::hg)))aa ac bC(is*e,r a){;;lS<=b)m 0;lz p=b==8?gK:gb;m ac(cB(e,a-b))*p;}nw,B b>eq eV=a::cr&&gH::az<b,16>;mV d,aN c,af e,r a,B p>q(eV<d,p>&&e<=mP)aa aA<c>gc(is*i,nr){lz w=of(a,8u)W k=gH::go<p,w>(i,b L lx<aA<c>>(bC<8,e,a>(i,b)+k)y nb d,aN a,af b,r c=0,B p=ar>q(b==oD)aa aA<a>ev(bx e){m lx<aA<id(bh(e.gJ))y nb d,aN c,af p,r a=0,B i=ar>q(p!=oD)aa aA<c>ev(bx e)lA b=a!=0?a:e.fT lH b<=eG){m lx<aA<c>>(cB(e.gJ,b fd m gc<d,c,p,a,i>(e.gJ,b);}P bQ{Z fy{ac kn;ac jW ml c,af b,af a,B d>eq fp=c::cr&&((b<=mP&&a<=mP&&gH::az<d,16>)||((b>mP||a>mP)&&gH::dr<d>));mV i,af d,r p,af c,r e,B o>q(fp<i,d,c,o>&&d<=mP&&c<=mP)aa fy gt(bx k,nr,bx w,f r a)lA n=gH::gp<o,of(p,8u),of(e,8u)>(k.gJ,b,w.gJ,a L{bC<8,d,p>(k.gJ,b)+pO<0>(n)),bC<8,c,e>(w.gJ,a)+pO<1>(n))}y nb w,af p,r d,af e,r c,B n=ar>q(p!=oD||e!=oD)aa fy gu(bx k,bx i)lA b=d!=0?d:k.fT W a=c!=0?c:i.fT lH(b|a)==1u)[[unlikely]]{m{bh(k.gJ),bh(i.gJ)};}m gt<w,p,d,e,c,n>(k,b,i,mX}z<bo a,B T,B U>q(a==oC)F T dg(U b,aV){m lx<T>(lx<cU<T>>(b fd P bQ{mV c,ag a>aa nH>jX(bt d){hj<a>(d);lz p=aI<a>;l T=nH>W b=df<p.dI>(d L dg<p.dI,T>(ev<c,qb>(b),b.hk)y nb i,ag d,ag c>aa lt iz(bt n,bt w)->g::pair<aj<d>,nG>>{hj<d>(n);hj<c>(w);lz b=aI<d>;lz a=aI<c>W p=df<b.dI>(n)W e=df<a.dI>(w)W k=gu<i,b.dW,b.cg?b.bw:0,a.dW,a.cg?a.bw:0>(p,e L{dg<b.dI,aj<d>>(k.kn,p.hk),dg<a.dI,nG>>(k.jW,e.hk)}y<aV b,ag...d>mf lt ew(){F bd<aV,og(d)>a{(aI<d>.dW==oD)...};mu p=lx<j>(mz::count(a,b));bd<j,p>c{};j i=0;mN e=0;e<a.lL;++e)lS[e]==b)c[i++]=e;m c y nb c,B a,j p,B b,ly>aa aW gv(b&d,f bl<N>&e);mV c,B a,j d,lW,B w,ly>aa aW gL(w&e,f bl<N>&p)lA i=iz<c,fI<d,a>,fI<mg>>(p[d],p[b]);mO<d>(e)=i.nU;mO<b>(e)=i.second y nb b,B a,lt i,lt c,j...d,j...w,B k,ly>aa aW gM(k&e,f bl<N>&p,ao<d lv,ao<w lv){(gv<mg,i[d]>(e,p),lw;(gL<mg,c[w*2],c[(w*2)+1]>(e,p),lw;y nb c,ag...a>nM)>0)aa lt hT(bl<og(a)>p)->na<nH>lv{lz b=ew<mH,ni();lz d=ew<mv,ni();cD<nH>lv e{};gM<c,cD<ni,b,d>(e,p,ak<b.lL>{},ak<d.lL/2>{}L e;}}z<ag a>F aW dK(nH>b){(aW)b;}mq ex=10000uz;mq cn=4uz;mF ac hl=10000uz;mf lt fq(){bd<bd<au,cn>,ex>b{};mz::transform(g::views::iota(j{},ex),b.begin(),[](at)C bd<au,cn>{pt/1000),pt/100%10),pt/10%10),pt%10)};}L b;}mF lt ey=fq();bf au*cQ(au*a,r c)lA&b=ey[c];ow(a,b.mW,b.lL L a+4;}bf au*cu(au*a,r b){f r e=1u+(b>=10)+(b>=100)+(b>=1000);r c{};ow(&c,ey[b].mW,lZ c);f r d=(cn-e)*8;c>>=d;ow(a,&c,lZ c L a+e y<af i,cX c>q(i<=mP)bf au*gN(c b,au*a){lz d=lx<c>(hl);lz p=lx<c>(100000000);in b>=p){f c w=b/p;f c e=b/d;a=cu(oI(w));a=cQ(oI(e-w*d)L cQ(oI(b-e*d fd}in b>=d){f c e=b/d;a=cu(oI(e)L cQ(oI(b-e*d fd}m cu(oI(b fd P bQ{z<bo b>q(b==oC)F au*iS(au*a,aV){m a;}bf au*hm(au*b,oq a)in!a.empty())ow(mg.mW,a.lL L b+a.lL;}pd ei:ar{ej,jk,lf,};mV c,af d,r b,B a>mF ei gO=[]{{m ei::ej;}}();mV e,bo w,aN i,af p,r d,B b=r>q(gO<e,p,d,b> ==ei::ej)bf au*dt(au*a,aA<i>k,aV c){a=iS<w>(a,c L gN<p>(k,a)y<ag e,aV d>mf j cG(){lz p=aI<e>;lz b=lx<j>(p.bw);lz a=lx<j>(p.cA);lz c=a-b;{m c+g md cn,b);}}mV b,ag c,B a=r>q(b::cr&&gH::cz<a>)mf j cG(){m cG<c,mH>()y nb d,ag a>bf au*jY(au*c,nH>e){dK<a>(e);lz p=aI<a>W b=dJ<p.dI>(e L dt<d,p.dI,qb>(c,lx<aA<p.de>>(b.ds),b.fo);}P jy{mV p,eU b,eU a>aa au*fH(au*mj aA<b.de>k,f aV e,f aA<a.de>w,f aV d,oq i){c=dt<p,b.dI,b.de,b.dW,b.cg?b.bw:0>(c,k,e);c=hm(c,i L dt<p,a.dI,a.de,a.dW,a.cg?a.bw:0>(c,w,d);}lC d,B c,eU b,eU nL eW=d::cr&&gH::cz<c,2>&&b.dW>mP&&a.dW>mP;mV i,ag b,ag a,B w=r>q(!eW<i,w,aI<b>,aI<id)bf au*iA(au*k,aj<b>s,nH>o,oq n){dK<b>(s);dK<a>(o);lz d=aI<b>;lz c=aI<a>W p=dJ<d.dI>(s)W e=dJ<c.dI>(o L fH<i,d,c>(k,lx<aA<d.de>>(p.ds),p.fo,lx<aA<c.de>>(e.ds),e.fo,n);}}}Y io{l R ae;P av{nZ kP=iy<lN,oq>me fU=pJ<lE mz::sized_range<lE bq<mz::range_value_t<id&&!kP<a>y<ae a=pr,bj b=pj aT;z<ae b,bj c,mQ...a>nM)>0)B ay;z<ae a=pr,bj b=pj aX;z<ae a,bj b>B aT{mi:l pK=b;mC aT(FILE*d=stdin,f pK&c=pK{})D:cw nA,hV(cw.ji().kK,cw.ji().kL){static_assert(bg::eP==8uz);static_assert(bg::eQ==64uz);}aT(f aT&lY;aT&I=(f aT&lY;aT(aT&&lY;aT&I=(aT&&lY;z<mQ c>aa mx nG>hU(){m oJ jX<a,c>(hV.z kf<1>()[0])y<mQ d,mQ c,mQ...e>aa lt hU()->na<mx aj<d>,mx nG>,mx aj<e>lv{m oJ hT<a,d,c,e lv(hV.z kf<og(e)+2>())y<mQ...c>q(og(c)>0)ay<a,b,c lv jK(mS){m ay<a,b,c lv(this,d);}mh:aU<b>cw;oJ fw<a>hV mo<ae b,bj c,mQ...a>nM)>0)B ay{mi:mR=cD<mx nH>lv;B no{mi:mR=ay::lQ;l difference_type=ptrdiff_t;l iterator_concept=g::input_iterator_tag;no()D oz;f lQ&I*()f C jd->eX;}aa no&I++()D{jd->advance(L*this;}B dh{mi:f bf lQ&I*()f D;mh:mp B no;mC dh(lQ d)D:jZ(d){}lQ jZ;};bf dh I++(int)D;mp aV I==(f no&d,pC)C d.oS();}mp aV I==(pC d,f no&e)C e==d;}mh:mp B ay;mC no(ay*d)D:jd(d){}bf aV oS()f;ay*jd{};};ay(f ay&lY;ay&I=(f ay&lY;ay(ay&&d)D:hF(pL.hF,oY)),cV(pL.cV,j{0})),eX(g::move(d.eX)),ez(pL.ez,mv)){}bf ay&I=(ay&&d)D;no begin()D in!ez&&!oS()){eX=eH();ez=mH;}m no(this);}hn end()f C{};}j lL f C cV;}mh:mp B aT<b,c>;mC ay(aT<b,c>*d,ng)D:hF(d),cV(e){}aV oS(mK cV==0;}aa lQ eH()nM)==1){m lQ{hF->z hU<ni()};}aa lQ eH()nM)>1);aa aW advance()D{--cV lH!oS())eX=eH();}aT<b,c>*hF;j cV;lQ eX{};aV ez{}mo<ae i,bj d>B aX{mi:l fz=d;mC aX(FILE*b=stdout,f fz&a=fz{})D:eY(b,aH<d>::da,a){}aX(f aX&lY;aX&I=(f aX&lY;aX(aX&&lY;aX&I=(aX&&lY;z<mQ c,ns b=" ",ns p="\n",av::fU n>q Q<mz::range_value_t<n>,mx nG>>&&mz::contiguous_range<n>bu aW ko(n&&o){l kp=mx nG>;ng=mz::size(o)lH e==0)m;F at=gd<mk,p>();f g::span<f kp>k(mz::oU(o),e);j w=0;for(;e-w>a;w+=a){fr<mk,b>(pP).z nU<a>(fd if(e-w==a){fr<mk,p>(pP).z nU<a>(fd else{fr<mk,p>(pP fd}aW ka(){eY.jc();}mh:lX ch=8;z<mQ c,ns a,ns e>my mf j gd()lA b=g md cR<c,a,a>(ch),cR<c,a,e>(ch)L b<=aH<d>::da?ch:2uz y<mQ b,ns c,ns w>lX cR(lG p)in p==0)m 0;lz a=lx<j>(mx aI<b>.cA);lz e=oJ cG<i,b>(L(p-1)*(a+c.lL)+g md e,a+w.lL);}au*hW(at)in eY.ga()<a)eY.jc(L eY.he();}nF kg(oq a);z<ns a>my au*ek(au*c){oG((c[b]=a.mW[b]),lw;}(ak<a.lL>{}L c+a.lL y<mQ c,ns b,ns n,j p>q(p==g::dynamic_extent||p>0)aW fr(f g::span<f mx nG>,p>e)in e.empty())m W w=e.lL W k=cR<mk,n>(w);au*f s=hW(k);au*a=s;mN o{};o<w/2;++o)in o!=0)a=ek<b mr W t=o*2;a=oJ jy::iA<i,c,c>(a,e[t],e[t+1],b.ji(fd if(w%2!=0)in w>1)a=ek<b mr;a=oJ jY<i,c>(a,e.back(fd a=ek<n mr;eY.gG(j(a-s fd aH<d>eY;};l iB=aT<pr>;l iC=aX<pr>;Y iT{mf r cd(nr){r a=1;for(bm c=0;c<5;++c)a*=2U-b*a;m 0U-a;}Y iT{nv,B e>F T kq(T d,ac a,T p,e b){T c=g::move(p);oh(a!=0)in(a&1U)!=0)c=b(c,d);a>>=1U;lS!=0)d=b(d,d);}m c;}Y iT{P kb{mF bd<r,3>ge{2,7,61};}F aV jz(f r a){lS<2)m mv;lS%2==0)m a==2 W e=r(os(a-1));f r i=(a-1)>>e W c=[a](nE ma ac d){m b*d%a;};for(f r p:kb::ge)in p%a==0)ox ac b=kq(ac{p},i,mt,c)lH b==1)ox for(r d=1;d<e&&b!=a-1;++d)b=c(b,b)lH b!=a-1)m mv;}m mH;}}P J{z<r a>q(a>1&&a<(1u<<31))Z bR{r kr;r jA;lD bR df(f r lO{b,r((ac{b}<<32U)/a)};}F r kC(f r x)f;};P fW{z<ly>l bc=nT<r,N>;z<ly>bc<N>iD(f bc<N>&a,f r lO R kN(a,a-pm(b))y<r a,ly>bc<N>hA(f bc<N>&mj bR<a>&w){f bc<N>b=R hL(c,pm(w.jA)L c*pm(w.kr)-b*pm(mX}P fW{}z<r a,ly>qa))B aK{mi:l gw=fW::bc<N>;nh jl=iT::cd(a);aK()oz;mC aK(f gw&b)D:iU(b){}nW D;f gw&ks(mK iU;}mp aK I+(f aK&ma aK&c)C aK{fW::iD(b.iU+c.iU,2*a)};}mp aK I*(f aK&ma bR<a>&c)C aK{fW::hA(b.iU,c)};}mh:gw iU;};}P J{P fW{z<cX T>F r hX(mc ma r lR r(lx<ac>(b)%mX}}P J{z<r a>qa))B ad{mi:nh kQ=a;nd eA=iT::jz(a);mU()D oz;z<pM T>q(!mb,aV>)F mC ad(mc b)D:eZ(iE(lx<ac>(ia(b))*hZ)){}F r nl()f D{nr=iE(eZ L b>=a?b-a:b;}F r ks(mK eZ;}lD ad iv(f r c){ad b;b.eZ=c;m b;}mU&I+=(f ad c)D{nr=eZ+c.eZ;eZ=of(b,b-hY L*this;}mU&I*=(f ad b)D{eZ=iE(ac(eZ)*b.eZ L*this;}mU I-()f C ad{}-*this;}mp mU I+(po C b+=c;}mp mU I-(po C b-=c;}mp mU I*(po C b*=c;}mU lq(nE b)f;mU jm(nE b mK iv(r(ac(eZ)*b%a fd mU jL()f q eA;mU jL()f q(!eA);mp F aV I==(f po C ib(b.eZ)==ib(c.eZ);}mp F bR<a>bY(f ad lO bR<a>::df(nN(fd nh iE(nE c)lA b=r(c)*gP;m r((c+ac(b)*a)>>32U);}nh hY=2*a;nh gP=iT::cd(a);nh jM=r((mt<<32U)%a);nh hZ=r(ac(jM)*jM%a);mh:z<pM T>nh ia(mc lO fW::hX(mg)y<cX T>q(lV<=lZ(r))nh ia(mc lO b;}nh ib(f r lO b>=a?b-a:b;}r eZ{};}y pp nz J::lB aq<J::ad<id{l ic=ad<a>;l ce=na pp>;lD ce fP(f ic lO ce{b.ks()};}lD ic jh(f ce&lO ic::iv(nK>(b))y<pQ>l dn=aK<a,N>;z<pQ>my ol cT(f na<nT pp,N>>&lO ol{nK>(b)}y<pQ>my na<nT pp,N>>cx(f ol&lO{b.ks()};}};P J::ap{lC A,B M>eq ie=q(f pG&c,ac d){{a.gk(b,e,c,d)}nJ<O<M>>ml A,B M>q ie<A,M>F O<M>gk(f pG&mj ac d){m a.gk(b,e,c,d);}lC A,B M>eq ig=hC<A>&&hC<M>nn pG&c,ac d){{lB gk(a,b,e,c,d)}nJ<O<M>>ml A,B M,ly>eq iF=ig<A,M>&&bb<O<M>,N>&&bb<O<A>,N>nn A&a,f M&ma O<A>&d,f aZ<O<M>,N>&mj aZ<O<A>,N>&p,ac e){a.cq(d);{a.z gD<N>(mg.cq(d),c,e)}nJ<aZ<O<M>,N>>;{a.z fO<N>(p,a.cq(d))}nJ<aZ<O<A>,N>>ml A>l gQ=oy(g::declval<f A&>().cq(g::declval<f O<A>&>()));Y ap{z<hG M,B T>F O<M>kR(f M&,mc&lR O<M mr;}Y iV{pd jn:ar{kS,kt,kc,nj span{j ku;j jN;j kT;jn ln;nj lo{j lg;j lh mo<lW>q(pE(b)&&b>=2&&b<=64)B ba{mi:lX kZ=b;lX eI=os(b);lX ca=b-1;lX fA=(aM<j>::digits+eI-1)/eI;F ba()D oz;mC F ba(mB)D:jO(c){at=c;gS[0]=a;fV=1;oh(a>b){a=(a+ca)>>eI;gS[fV]=a;++fV;}}mu lL f C jO;}mu fX(mK fV;}mu li(mw)f D;mu gR(mw mK(gS[a]+ca)>>eI;}mu kd(mw,mB)f;mu kd(mw,mB,mS)f dp e>F aW kv(at,j d,e&&p)f{mN c=0;a<d;++c)if(iG(a,d,c,p))m;}lC w>F aV iG(j&a,j&c,lG p,w&&i)f{lS>=c)m mH;f aV d=(c&ca)==0||c==gS[p]lH(id eI)==((c-1)>>eI)){f aV e=(a&ca)==0&&d lH!e||p+1==fV){i(span{p,a,c,jn::kc}L mH;}}else in(a&ca)!=0){ng=(a|ca)+1;i(span{p,a,e,jn::kS});a=e;}if(!d){ng=c&~ca;i(span{p,e,c,jn::kt});c=e;}}a>>=eI;c=(c+ca)>>eI;m a>=c;}mh:j jO{};j fV{};bd<j,fA>gS{};};;Y iV{P cy{mq eB=64 nI>mq dQ=lV;z<lB cp T>mq dQ<T> =[op){m g md{lZ(fI<a,lB aB<T>>)...lF(pX<T>>{});mf j iH(mw){m g::clamp<j>(g::bit_floor(eB/a),2,16);}}nv>mq gx=cy::iH(cy::dQ<T>);P cy{pT eJ{}dp a,lW>mq fa=g md alignof(a),of(eB,g::bit_floor(lZ(a)*b)));z<at,B b,B...c>Z eJ<a,b,c lv{alignas(fa<lP)bd<lP dn;[[oQ]]eJ<a,c lv kU mo<lW,B nz dR;pT dR<b,cD<a...>>{lX kw=((lZ(a)*b)+lw;lX je=kw%eB==0?eB:g md{fa<a,b>...})mo<j c,B a>q(c==0)lz&cb(a&lO b.dn y<at,B b>q(a>0)lz&cb(b&c){m cb<a-1>(c.kU)y<lW,B nz fb;pT fb<b,cD<a...>>{l kV=eJ<mg lv;}y<lB cp V,at>Z alignas(cy::dR<a,oX>>::je)fs{mR=V;l gT=lB aq<V>;lI cy::fb<a,oX>>::kV gU;nW D;V get(mB)f C[&]<j...b>(pu C gT::jh(oX>{oT)[c]...lF pe lF aW jp(ng,f V&c){f oX>d=gT::fP(c);oG((oT)[e]=mO<b>(d)),lw;}pe lF aW fill(f V&c)D{f oX>d=gT::fP(c);oG(oT).fill(mO<b>(d)),lw;}pe})y<mJ(lB bb<nQ>&&N<=a)lB oK load(mB)f C oG l p=lB ok>;m gT::pb>(p{R load<fI<b,p>>(oT).mW+c)...lF pe})y<mJ(lB bb<nQ>&&N<=a)aW store(lG mj lB oK&d)D{f lB ok>e=gT::pc>(d);oG(R km(oT).mW+c,mO<b>(e)),lw;}pe lF}dp V,at>Z fc;z<lB cp V,at>Z fc<V,a>{l kW=fs<V,a>ml V,at>l iW=lI fc<V,a>::kW;Z dX{j jP;j kh ml b>B di{mi:di()oz;z<j i>di(f ba<i>&mj dX a):jQ(a){j e=0;mN d=a.jP;d<a.kh;++d)e+=c.gR(d);iX=g::make_unique_for_overwrite<b[]>(e);b*p=iX.get();mN d=a.jP;d<a.kh;++d){ih[d]=p;p+=c.gR(d);}}b&dL(mw,mB){m ih[a][c];}f b&dL(mw,mB mK ih[a][c];}mh:g::unique_ptr<b[]>iX;bd<b*,ba<2>::fA>ih{};dX jQ{};};mq gf=256uz<<10U;z<j d>mu ho(f ba<d>&a,nf){j c=0;oh(c<a.fX()&&a.gR(c)*b>gf){++c;}m c;}Y iV{P cy{}P cy{}P cy{}lC V,ly,B b>b ht(mS,ng,f b&a,f b&c)in d==0&&e==N)m a;m lB iq<nQ>(d,e,a,c)y<lB cp T>aV hp(mc&ma T&a){l p=lB aq<T>;m p::fP(b)==p::fP(a)y<lB hC M,j c,pJ o,B w>aa aW iI(f M&a,f ba<c>&b,o&k,w n){lt e=mz::begin(k);mN d=0;d<b.gR(0);++d){lt&i=n(d);mN p=0;p<c;++p)in(d<<ba<c>::eI)+p<b.lL){i.jp(p,lB kR(a,*e));++e;}else{i.jp(p,a.dY(fd}}}lC c>aW cM(f c&d){mu b=64 W*e=nY<f unsigned char*>(&d);for(at=0;a<lZ(c);a+=b)R jt(e+mX Y iV{lC A,lB hC M,at=gx<O<A>>>q lB ig<A,M>B el{mi:l cW=O<M>;l bM=O<A>;l dj=ba<a>;z<pJ p>q mz::sized_range<p>mC el(p&&e,A i=A{},M c=M{}):bp(mz::size(e)),cH(ho(bp,iJ)),fe(bp,dX{0,1}),gy(bp,dX{1,pg}),dD(g::move(i)),eD(g::move(c)){iI(eD,bp,e,[this](nf)->lt&{m oV,b);});pa=1;b<pg;++b)mN d=0;d<bp.gR(b);++d)oN(b,d).fill(dD.dY(fd j lL f C bp.lL;}f bf dj&kD()f;f bf A&la()f;f bf M&kx()f;aa bu cW lj(nf)f{cW c=oV,b>>du).get(b&dv);j e=b;mN d=1;d<pg;++d){e>>=du;c=lB gk(dD,eD,lk(d,e),c,1);}m c;}nF jp(nf,cW c);bu aW gk(ng,lG p,f bM&n)in e==p)m;{mS=j(os(e))/du;mB=p==lL?pg-1:j(os(p))/du;pa=pg-1;b>0;--b){lG i=e>>(du*b);lG w=(p-1)>>(du*b)lH i==w)in b>d||b>c)jR(b,i);}else in b>d)jR(b,i)lH b>c)jR(b,w);}}}f dk k=df(n);bp.kv(e,p,[&](f span&b){dM(b.ku,b.jN>>du,k,b.jN&dv,((b.kT-1)&dv)+1);lF nF ky(f bM&b);aW jB(mS,lG p)f in d>=p||cH==0)m;j e=d>>du;j c=(p-1)>>du;cM(oV,e));cM(oV,c));pa=1;b<cH;++b){e>>=du;c>>=du;cM(oN(b,e));cM(oN(b,c fd}mh:lX du=dj::eI;lX dv=dj::ca;l eC=iW<cW,a>;l em=iW<bM,a>;nd en=lB iF<A,M,lE Q<eC,fs<cW,id&&Q<em,fs<bM,id;Z it{bM lr;lB gQ<A>dn;};l dk=pF<en,it,bM>;lX iJ=g md lZ(eC),lZ(em));dk df(f bM&b)f q en{m{b,dD.cq(b)};}bf dk df(f bM&b)f q(!en);bM lk(mB,nf mK oN(mk>>du).get(b&dv);}aV iK(f bM&b mK hp(b,dD.dY(fd aa bu aW dM(ng,lG p,f dk&b,lG i,lG w)q en in e==0){eC&c=oV,p)W pY,ht<cW,a>(i,w,dD.z gD<a>(eD,b.dn,d,1),d fd else{em&c=oN(e,p)W pY,ht<bM,a>(i,w,dD.z fO<a>(d,b.dn),d fd}aa bu aW dM(mB,mS,f dk&b,ng,lG p)q(!en);bu aW jR(mS,nf){em&e=oN(d,b>>du);f bM c=e.get(b&dv)lH iK(c))m;e.jp(b&dv,dD.dY());dM(d-1,b,df(c),0,mX dj bp;j cH;di<eC>fe;di<em>gy;[[oQ]]A dD;[[oQ]]M eD;};}l oF::iB;l oF::iC;l ii=oF::mx dd<500000>;l kX=oF::mx dd<1>;l gV=oF::mx dd<998244352>;l gW=J::ad<998244353>;mu jf=8;Z ij{aV jg;r nU;r ki;r ed;r bZ;};ij hB(iB&i)lA e=i.hU<kX>()lH e==0)lA[c,p]=i.hU<ii,ii>()W[d,a]=i.hU<gV,gV>(L{mH,c,p,d,a};}mm b=i.hU<ii>(L{mv,b,b+1,0,0};}aW kz(iB&x,iC&t)lA[u,n]=x.hU<ii,ii>();J::iV::el<J::lB jV<gW>,J::lB kl<gW>>o(x.jK<gV>(u)|g::views::elements<0>);mu p=64;bd<ij,p>b{};g::vector<r>e;e.reserve(n);mB=n;mN i=0;i<c;){mw=of(b.lL,c-i);lt k=g::span{b}.nU(a);for(lt&d:k)d=hB(x);for(mm[s,d]:g::views::enumerate(k))in mm w=j(s)+jf;w<a){o.jB(b[w].nU,b[w].ki);}if(d.jg){o.gk(d.nU,d.ki,{gW{d.ed},gW{d.bZ}lF else{e.push_back(o.lj(d.nU).nl(fd}i+=a;}t.ko<gV,"\n","\n">(e);t.ka();}int main(){iB b{};iC a{};kz(mg);}
