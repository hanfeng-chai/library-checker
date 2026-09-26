#include <bits/stdc++.h>
using namespace std;
static const int MOD = 998244353;
template<class T> constexpr T INF=numeric_limits<T>::max()/32*15+208;
using ll = long long;
using uint = unsigned;
using ull = unsigned long long;
constexpr int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1},dx8[8]={1,1,0,-1,-1,-1,0,1},dy8[8]={0,1,1,1,0,-1,-1,-1};
template<class T> T ifloor(T x,T y){return x/y-(x%y?(x<0)^(y<0):0);}
template<class T> T iceil(T x,T y){return x/y+(x%y?(x>=0)^(y<0):0);}
template<class T> bool chmax(T&a,T b){return a<b?(a=b,1):0;}
template<class T> bool chmin(T&a,T b){return a>b?(a=b,1):0;}

extern "C" int fileno(FILE *); extern "C" int isatty(int);
template<class T,class=void> struct has_fio_r:false_type{};
template<class T> struct has_fio_r<T,void_t<decltype(declval<T&>().begin()),decltype(declval<T&>().end())>>:true_type{};
template<class T,class=void> struct has_fio_v:false_type{};
template<class T> struct has_fio_v<T,void_t<decltype(declval<const T&>().value())>>:true_type{};
template<class T,class=void> struct has_fio_a:false_type{};
template<class T> struct has_fio_a<T,void_t<decltype(declval<T&>().assign(declval<const string&>()))>>:true_type{};
template<class T,class=void> struct has_fio_s:false_type{};
template<class T> struct has_fio_s<T,void_t<decltype(declval<const T&>().to_string())>>:true_type{};
template<bool B,class U=int> using en_if_t=enable_if_t<B,U>;
template<class T> constexpr bool is_rng_v=has_fio_r<T>::value&&!is_same_v<decay_t<T>,string>;
template<class T> constexpr bool has_val_v=!is_integral_v<T>&&!is_rng_v<T>&&!is_same_v<decay_t<T>,string>&&has_fio_v<T>::value;
template<class T> constexpr bool has_asn_v=!is_integral_v<T>&&!is_rng_v<T>&&!is_same_v<decay_t<T>,string>&&!has_fio_v<T>::value&&has_fio_a<T>::value;
template<class T> constexpr bool has_str_v=!is_integral_v<T>&&!is_rng_v<T>&&!is_same_v<decay_t<T>,string>&&!has_fio_v<T>::value&&has_fio_s<T>::value;
struct FastIOTb{char n[40000]{};constexpr FastIOTb(){for(int i=0;i<10000;++i){int x=i;for(int j=3;j>=0;--j)n[i*4+j]=char('0'+x%10),x/=10;}}};
struct Scanner{
    static constexpr int B=1<<17,O=64,Q=1024,D=16; char b[B+1]; int I=0,S=0;unsigned char M=isatty(fileno(stdin))?2:0;string nt;
    __attribute__((always_inline)) static inline uint p8(const char*p){ull x;memcpy(&x,p,8);
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        x=__builtin_bswap64(x);
#endif
        x-=0x3030303030303030ULL;x=(x*10+(x>>8))&0x00ff00ff00ff00ffULL;x=(x*100+(x>>16))&0x0000ffff0000ffffULL;x=(x*10000+(x>>32))&0xffffffffULL;return (uint)x;}
    __attribute__((always_inline)) static inline bool d8(const char*p){ull x;memcpy(&x,p,8);return (((x+0x4646464646464646ULL)|(x-0x3030303030303030ULL))&0x8080808080808080ULL)==0;}
    template<class U> __attribute__((noinline)) U lng(char c){const char*p=b+I-1,*e=b+S;U y=0;if(c>='0'&&e-p>=16&&p[15]>='0'&&d8(p)&&d8(p+8)){
            y=U(p8(p))*100000000+p8(p+8);p+=16;while(*p>='0')y=U(y*10+(*p&15)),++p;I=(int)(p-b)+1;return y;}while(c>='0')y=U(y*10+(c&15)),c=b[I++];return y;}
    inline void ld(){int l=S-I;memmove(b,b+I,l);if(M==2)S=l+(fgets(b+l,B+1-l,stdin)?(int)strlen(b+l):0);else{S=l+(int)fread(b+l,1,B-l,stdin);int n=min(S,Q),s=0,m=0;
            for(int i=0;i<n;++i){s+=b[i]<=' ';m+=b[i]=='-';}M=s*D<n-m;}I=0;b[S]=0;}
    inline void nd(){if(I+(M==2?1:O)>S) ld();} inline void bk(){for(nd();b[I]&&b[I]<=' ';++I)nd();} inline char skip(){bk(); return b[I++];}
    template<class T,en_if_t<is_integral_v<T>,int> = 0> void read(T&x){using V=conditional_t<is_same_v<T,bool>,uint,T>;using U=make_unsigned_t<V>;
        char c=skip();bool g=0;if constexpr(is_signed_v<T>)if(c=='-'){g=1;if(M==2)nd();c=b[I++];}U y=0;
        if(__builtin_expect(M,0)){if(M==1)y=lng<U>(c);else while(c>='0')y=U(y*10+(c&15)),nd(),c=b[I++];}else while(c>='0')y=U(y*10+(c&15)),c=b[I++];
        if constexpr(is_signed_v<T>){if(g&&y){x=-static_cast<T>(y-1);--x;return;}}x=static_cast<T>(y);}
    void read(double&x){read(nt);const char*f=nt.data(),*l=f+nt.size();auto r=from_chars(f,l,x);if(r.ec!=errc{}||r.ptr!=l)__builtin_trap();}
    template<class T,en_if_t<has_val_v<T>,int> = 0> void read(T&x){ll v; read(v); x=T(v);}
    template<class T,en_if_t<has_asn_v<T>,int> = 0> void read(T&x){string s;read(s);if(!x.assign(s))__builtin_trap();}
    template<class H,class N,class... T> void read(H&h,N&n,T&...t){read(h); read(n,t...);} template<class T,class U> void read(pair<T,U>&p){read(p.first,p.second);}
    template<class T,en_if_t<is_rng_v<T>,int> = 0> void read(T&a){for(auto&x:a) read(x);} void read(char &c){c=skip();}
    void read(string &s){s.clear();bk();for(;;){int l=I;while(I<S&&b[I]>' ')++I;s.append(b+l,I-l);if(I<S){++I;break;}ld();if(!S) break;}}
} din; template<class T> Scanner& operator>>(Scanner&in,T&x){ in.read(x); return in; }
struct Printer{
    static constexpr int B=1<<17,O=64,P=15;char b[B];int I=0;bool o=isatty(fileno(stdout));string nb;inline static constexpr FastIOTb Tb{};
    ~Printer(){flush();} inline void flush(){if(I) fwrite(b,1,I,stdout),I=0; }
    inline void pc(char c){if(I>B-O) flush(); b[I++]=c; if(o&&c=='\n') flush(); }
    inline void pr(const char*s,size_t n){while(n){if(I==B)flush();size_t k=min(n,(size_t)(B-I));memcpy(b+I,s,k);I+=(int)k;s+=k;n-=k;}}
    void print(bool x){pc(char('0'+x));}void print(char c){pc(c);}void print(const char* s){pr(s,strlen(s));}void print(const string&s){pr(s.data(),s.size());}
    inline char* wt(char*q,uint x){if(x>=1000)return memcpy(q,Tb.n+(x<<2),4),(q+4);if(x>=100)return memcpy(q,Tb.n+(x<<2)+1,3),(q+3);
        if(x>=10){uint y=(x*205)>>11;*q++=char('0'+y);*q++=char('0'+x-y*10);return q;}*q=char('0'+x);return q+1;}
    inline void w4(char*q,uint x){memcpy(q,Tb.n+(x<<2),4);}inline void w8(char*q,uint x){uint y=x/10000;w4(q,y);w4(q+4,x-y*10000);}
    inline char* w32(char*q,uint x){if(x>=100000000){uint y=x/100000000,z=x-y*100000000;q=wt(q,y);w8(q,z);return q+8;}
        if(x>=10000){uint y=x/10000,z=x-y*10000;q=wt(q,y);w4(q,z);return q+4;}return wt(q,x);}
    __attribute__((noinline)) inline char* w64(char*q,ull x){if(x<=0xffffffffULL)return w32(q,(uint)x);ull y=x/100000000;uint z=(uint)(x-y*100000000);
        if(y<=0xffffffffULL){q=w32(q,(uint)y);w8(q,z);return q+8;}uint t=(uint)(y/100000000),m=(uint)(y-(ull)t*100000000);q=w32(q,t);w8(q,m);w8(q+8,z);return q+16;}
    template<class T,en_if_t<is_integral_v<T>&& !is_same_v<T,bool>,int> = 0> void print(T x){ if(I>B-100) flush(); using U=make_unsigned_t<T>; U y;
        if constexpr(is_signed_v<T>){ if(x<0) b[I++]='-',y=U(0)-(U)x; else y=(U)x; } else y=x;
        if(!y){b[I++]='0';return;}char*q;
        if constexpr(sizeof(U)<=4)q=w32(b+I,(uint)y);else if constexpr(sizeof(U)<=8)q=w64(b+I,(ull)y);
        else{char W[3*sizeof(U)];int p=sizeof(W);while(y>=10000)p-=4,memcpy(W+p,Tb.n+(y%10000)*4,4),y/=10000;q=wt(b+I,(uint)y);memcpy(q,W+p,sizeof(W)-p);q+=sizeof(W)-p;}I=(int)(q-b);
    }
    void print_fixed(double x,int p=P){if(p<0)__builtin_trap();size_t z=(size_t)p+512;if(nb.size()<z)nb.resize(z);for(;;){char*f=nb.data(),*l=f+nb.size();auto r=to_chars(f,l,x,chars_format::fixed,p);
            if(r.ec==errc{})return pr(f,r.ptr-f);
            if(r.ec!=errc::value_too_large){__builtin_trap();}z=nb.size()*2;if(z<=nb.size())__builtin_trap();nb.resize(z);}}
    void print(double x){print_fixed(x);}
    template<class T,en_if_t<has_val_v<T>,int> = 0> void print(const T&x){ print(x.value()); }
    template<class T,en_if_t<has_str_v<T>,int> = 0> void print(const T&x){ print(x.to_string()); }
    template<class T,en_if_t<is_rng_v<T>,int> = 0> void print(const T&a){ bool f=0; for(auto&&x:a){ if(f) pc(' '); f=1; print(x); } }
    void puts(){ pc('\n'); } template<class T> void puts(const T&x){ print(x); pc('\n'); }
    template<class H,class... T> void puts(const H&h,const T&...t){ print(h); ((pc(' '),print(t)),...); pc('\n'); }
    void puts_fixed(double x,int p=P){print_fixed(x,p);pc('\n');}
} dout; template<class T> Printer& operator<<(Printer&out,const T&x){ out.print(x); return out; }

#line 1 "datastructure/wavelet_matrix.cpp"
#if defined(__GNUC__) && defined(__x86_64__)
#include <immintrin.h>
#endif

template <class T>
struct WaveletMatrix {
    int n, lg, blocks;
    vector<int> mid;
    vector<unsigned long long> bit;
    vector<int> pref;
    vector<T> vals;

    WaveletMatrix() : n(0), lg(0), blocks(0) {}
    explicit WaveletMatrix(const vector<T> &v) { build(v); }

    static inline void rank1_pair(const unsigned long long *row, const int *row_pref, int l, int r, int &l1, int &r1) {
        int l_block = l >> 6;
        l1 = row_pref[l_block];
        int l_rem = l & 63;
        if (l_rem) l1 += __builtin_popcountll(row[l_block] & ((1ULL << l_rem) - 1));

        int r_block = r >> 6;
        r1 = row_pref[r_block];
        int r_rem = r & 63;
        if (r_rem) r1 += __builtin_popcountll(row[r_block] & ((1ULL << r_rem) - 1));
    }

#if defined(__GNUC__) && defined(__x86_64__)
    __attribute__((target("popcnt,bmi2")))
    static inline void rank1_pair_bmi2(const unsigned long long *row, const int *row_pref, int l, int r,
                                       int &l1, int &r1) {
        int l_block = l >> 6;
        l1 = row_pref[l_block] + __builtin_popcountll(__builtin_ia32_bzhi_di(row[l_block], l & 63));

        int r_block = r >> 6;
        r1 = row_pref[r_block] + __builtin_popcountll(__builtin_ia32_bzhi_di(row[r_block], r & 63));
    }
#endif

    static int build_bit_row(const int *cur, int n, int blocks, int shift,
                             unsigned long long *row, int *row_pref) {
        int one_cnt = 0;
        for (int block = 0; block < blocks; ++block) {
            int begin = block << 6;
            int end = min(begin + 64, n);
            unsigned long long word = 0;
            for (int i = begin; i < end; ++i) {
                word |= (unsigned long long)((cur[i] >> shift) & 1) << (i - begin);
            }
            row[block] = word;
            one_cnt += __builtin_popcountll(word);
            row_pref[block + 1] = one_cnt;
        }
        return one_cnt;
    }

#if defined(__GNUC__) && defined(__x86_64__)
    __attribute__((target("avx2,popcnt")))
    static int build_bit_row_avx2(const int *cur, int n, int blocks, int shift,
                                  unsigned long long *row, int *row_pref) {
        int one_cnt = 0;
        __m128i shift_count = _mm_cvtsi32_si128(31 - shift);
        for (int block = 0; block < blocks; ++block) {
            int begin = block << 6;
            int end = min(begin + 64, n);
            unsigned long long word = 0;
            int i = begin;
            for (; i + 8 <= end; i += 8) {
                __m256i x = _mm256_loadu_si256((const __m256i *)(cur + i));
                __m256i shifted = _mm256_sll_epi32(x, shift_count);
                unsigned int mask = _mm256_movemask_ps(_mm256_castsi256_ps(shifted));
                word |= (unsigned long long)mask << (i - begin);
            }
            for (; i < end; ++i) {
                word |= (unsigned long long)((cur[i] >> shift) & 1) << (i - begin);
            }
            row[block] = word;
            one_cnt += __builtin_popcountll(word);
            row_pref[block + 1] = one_cnt;
        }
        return one_cnt;
    }

    struct PartitionTable8 {
        alignas(32) int perm[256][8];
        alignas(32) int rotate[9][8];
        alignas(32) int store[9][8];

        PartitionTable8() {
            for (int mask = 0; mask < 256; ++mask) {
                int pos = 0;
                for (int i = 0; i < 8; ++i) {
                    if (!((mask >> i) & 1)) perm[mask][pos++] = i;
                }
                for (int i = 0; i < 8; ++i) {
                    if ((mask >> i) & 1) perm[mask][pos++] = i;
                }
            }
            for (int zero_count = 0; zero_count <= 8; ++zero_count) {
                for (int i = 0; i < 8; ++i) {
                    rotate[zero_count][i] = zero_count + i < 8 ? zero_count + i : 0;
                    store[zero_count][i] = i < zero_count ? -1 : 0;
                }
            }
        }
    };

    __attribute__((target("avx2,popcnt")))
    static void stable_partition_avx2(const int *cur, int n, int shift, int zero_cnt,
                                      const unsigned long long *row, int *nxt) {
        static const PartitionTable8 table;
        int zi = 0, oi = zero_cnt;
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256i x = _mm256_loadu_si256((const __m256i *)(cur + i));
            unsigned int ones = (row[i >> 6] >> (i & 63)) & 0xffU;
            int one_count = __builtin_popcount(ones);
            int zero_count = 8 - one_count;
            __m256i perm = _mm256_load_si256((const __m256i *)table.perm[ones]);
            __m256i packed = _mm256_permutevar8x32_epi32(x, perm);
            __m256i one_perm = _mm256_load_si256((const __m256i *)table.rotate[zero_count]);
            __m256i one_values = _mm256_permutevar8x32_epi32(packed, one_perm);
            __m256i zero_store = _mm256_load_si256((const __m256i *)table.store[zero_count]);
            __m256i one_store = _mm256_load_si256((const __m256i *)table.store[one_count]);
            _mm256_maskstore_epi32(nxt + zi, zero_store, packed);
            _mm256_maskstore_epi32(nxt + oi, one_store, one_values);
            zi += zero_count;
            oi += one_count;
        }
        for (; i < n; ++i) {
            int x = cur[i];
            int b = (x >> shift) & 1;
            int dst = b ? oi : zi;
            nxt[dst] = x;
            zi += b ^ 1;
            oi += b;
        }
    }

    __attribute__((target("avx512f,popcnt")))
    static void stable_partition_avx512(const int *cur, int n, int shift, int zero_cnt,
                                        const unsigned long long *row, int *nxt) {
        int zi = 0, oi = zero_cnt;
        int i = 0;
        for (; i + 16 <= n; i += 16) {
            __m512i x = _mm512_loadu_si512((const void *)(cur + i));
            unsigned int ones = (row[i >> 6] >> (i & 63)) & 0xffffU;
            __mmask16 one_mask = (__mmask16)ones;
            __mmask16 zero_mask = (__mmask16)~one_mask;
            _mm512_mask_compressstoreu_epi32(nxt + zi, zero_mask, x);
            _mm512_mask_compressstoreu_epi32(nxt + oi, one_mask, x);
            int one_count = __builtin_popcount(ones);
            zi += 16 - one_count;
            oi += one_count;
        }
        for (; i < n; ++i) {
            int x = cur[i];
            int b = (x >> shift) & 1;
            int dst = b ? oi : zi;
            nxt[dst] = x;
            zi += b ^ 1;
            oi += b;
        }
    }

#endif

    template <class U>
    static auto encode_key(U x) -> typename make_unsigned<U>::type {
        using Key = typename make_unsigned<U>::type;
        Key key = static_cast<Key>(x);
        if constexpr (is_signed<U>::value) key ^= (Key(1) << (sizeof(U) * 8 - 1));
        return key;
    }

    void compress_generic(const vector<T> &v, vector<int> &cur) {
        vector<pair<T, int>> ord(n);
        for (int i = 0; i < n; ++i) ord[i] = {v[i], i};
        sort(ord.begin(), ord.end(), [](const pair<T, int> &a, const pair<T, int> &b) {
            return a.first < b.first;
        });
        vals.clear();
        vals.reserve(n);
        for (int i = 0; i < n; ++i) {
            if (vals.empty() || vals.back() < ord[i].first || ord[i].first < vals.back()) {
                vals.push_back(ord[i].first);
            }
            cur[ord[i].second] = (int)vals.size() - 1;
        }
    }

    void compress_integral(const vector<T> &v, vector<int> &cur) {
        using Key = typename make_unsigned<T>::type;
        vector<Key> keys(n);
        vector<int> ord(n), buf(n);
        Key min_key = encode_key(v[0]);
        Key max_key = min_key;
        for (int i = 0; i < n; ++i) {
            keys[i] = encode_key(v[i]);
            ord[i] = i;
            min_key = min(min_key, keys[i]);
            max_key = max(max_key, keys[i]);
        }

        const int B = 16;
        const int MASK = (1 << B) - 1;
        const int bucket_count = 1 << B;
        auto pass_count = [&](Key x) {
            int passes = 0;
            while (x) {
                ++passes;
                x >>= B;
            }
            return passes;
        };
        int passes = pass_count(min_key ^ max_key);
        int normalized_passes = pass_count(max_key - min_key);
        if (normalized_passes < passes) {
            for (int i = 0; i < n; ++i) keys[i] -= min_key;
            passes = normalized_passes;
        }

        vector<int> cnt(bucket_count);
        for (int pass = 0; pass < passes; ++pass) {
            fill(cnt.begin(), cnt.end(), 0);
            int shift = pass * B;
            for (int i = 0; i < n; ++i) ++cnt[(keys[ord[i]] >> shift) & MASK];
            int sum = 0;
            for (int i = 0; i < bucket_count; ++i) {
                int count = cnt[i];
                cnt[i] = sum;
                sum += count;
            }
            for (int i = 0; i < n; ++i) {
                int id = ord[i];
                buf[cnt[(keys[id] >> shift) & MASK]++] = id;
            }
            ord.swap(buf);
        }

        vals.clear();
        vals.reserve(n);
        bool has_prev = false;
        Key prev = 0;
        for (int i = 0; i < n; ++i) {
            int id = ord[i];
            if (!has_prev || keys[id] != prev) {
                vals.push_back(v[id]);
                prev = keys[id];
                has_prev = true;
            }
            cur[id] = (int)vals.size() - 1;
        }
    }

    void compress_values(const vector<T> &v, vector<int> &cur) {
        if constexpr (is_integral<T>::value && sizeof(T) <= 8) compress_integral(v, cur);
        else compress_generic(v, cur);
    }

    void build_from_index_internal(vector<int> cur) {
        n = (int)cur.size();
        if (n == 0) {
            lg = 0;
            blocks = 0;
            mid.clear();
            bit.clear();
            pref.clear();
            return;
        }

        int m = (int)vals.size();
        lg = 0;
        while ((1LL << lg) < m) ++lg;
        if (lg == 0) lg = 1;
        blocks = (n + 63) >> 6;

        mid.assign(lg, 0);
        bit.assign(lg * blocks + 1, 0);
        pref.assign(lg * (blocks + 1), 0);
        vector<int> nxt(n);

#if defined(__GNUC__) && defined(__x86_64__)
        bool use_avx2 = __builtin_cpu_supports("avx2") && __builtin_cpu_supports("popcnt");
        bool use_avx512 = __builtin_cpu_supports("avx512f") && __builtin_cpu_supports("popcnt");
#endif

        for (int d = 0, shift = lg - 1; d < lg; ++d, --shift) {
            auto *row = bit.data() + d * blocks;
            auto *row_pref = pref.data() + d * (blocks + 1);
            int one_cnt;
#if defined(__GNUC__) && defined(__x86_64__)
            if (use_avx2) one_cnt = build_bit_row_avx2(cur.data(), n, blocks, shift, row, row_pref);
            else one_cnt = build_bit_row(cur.data(), n, blocks, shift, row, row_pref);
#else
            one_cnt = build_bit_row(cur.data(), n, blocks, shift, row, row_pref);
#endif
            int zero_cnt = n - one_cnt;
            mid[d] = zero_cnt;

#if defined(__GNUC__) && defined(__x86_64__)
            if (use_avx512) stable_partition_avx512(cur.data(), n, shift, zero_cnt, row, nxt.data());
            else if (use_avx2) stable_partition_avx2(cur.data(), n, shift, zero_cnt, row, nxt.data());
            else
#endif
            {
                int zi = 0, oi = zero_cnt;
                for (int i = 0; i < n; ++i) {
                    int x = cur[i];
                    int b = (x >> shift) & 1;
                    int dst = b ? oi : zi;
                    nxt[dst] = x;
                    zi += b ^ 1;
                    oi += b;
                }
            }
            cur.swap(nxt);
        }
    }

    void build(const vector<T> &v) {
        n = (int)v.size();
        if (n == 0) {
            lg = 0;
            blocks = 0;
            vals.clear();
            mid.clear();
            bit.clear();
            pref.clear();
            return;
        }

        vector<int> cur(n);
        compress_values(v, cur);
        build_from_index_internal(move(cur));
    }

    void build_from_index(const vector<int> &idx, const vector<T> &sorted_vals) {
        vals = sorted_vals;
        build_from_index_internal(idx);
    }

private:
    int count_less_index_fallback(int l, int r, int xi) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        int res = 0;
        for (int d = 0, shift = lg - 1; d < lg; ++d, --shift) {
            int l1, r1;
            rank1_pair(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            if ((xi >> shift) & 1) {
                res += r0 - l0;
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            else {
                l = l0;
                r = r0;
            }
            if (l == r) break;
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return res;
    }

#if defined(__GNUC__) && defined(__x86_64__)
    __attribute__((target("popcnt,bmi2")))
    int count_less_index_bmi2(int l, int r, int xi) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        int res = 0;
        for (int d = 0, shift = lg - 1; d < lg; ++d, --shift) {
            int l1, r1;
            rank1_pair_bmi2(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            if ((xi >> shift) & 1) {
                res += r0 - l0;
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            else {
                l = l0;
                r = r0;
            }
            if (l == r) break;
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return res;
    }
#endif

    int count_equal_index_fallback(int l, int r, int xi) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        for (int d = 0, shift = lg - 1; d < lg; ++d, --shift) {
            int l1, r1;
            rank1_pair(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            if ((xi >> shift) & 1) {
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            else {
                l = l0;
                r = r0;
            }
            if (l == r) return 0;
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return r - l;
    }

#if defined(__GNUC__) && defined(__x86_64__)
    __attribute__((target("popcnt,bmi2")))
    int count_equal_index_bmi2(int l, int r, int xi) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        for (int d = 0, shift = lg - 1; d < lg; ++d, --shift) {
            int l1, r1;
            rank1_pair_bmi2(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            if ((xi >> shift) & 1) {
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            else {
                l = l0;
                r = r0;
            }
            if (l == r) return 0;
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return r - l;
    }
#endif

    int kth_smallest_index_fallback(int l, int r, int k) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        int idx = 0;
        for (int d = 0; d < lg; ++d) {
            int l1, r1;
            rank1_pair(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            int z = r0 - l0;
            idx <<= 1;
            if (k < z) {
                l = l0;
                r = r0;
            }
            else {
                k -= z;
                idx |= 1;
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return idx;
    }

#if defined(__GNUC__) && defined(__x86_64__)
    __attribute__((target("popcnt,bmi2")))
    int kth_smallest_index_bmi2(int l, int r, int k) const {
        const int *mid_data = mid.data();
        const auto *bit_data = bit.data();
        const int *pref_data = pref.data();
        int idx = 0;
        for (int d = 0; d < lg; ++d) {
            int l1, r1;
            rank1_pair_bmi2(bit_data, pref_data, l, r, l1, r1);
            int l0 = l - l1, r0 = r - r1;
            int z = r0 - l0;
            idx <<= 1;
            if (k < z) {
                l = l0;
                r = r0;
            }
            else {
                k -= z;
                idx |= 1;
                l = mid_data[d] + l1;
                r = mid_data[d] + r1;
            }
            bit_data += blocks;
            pref_data += blocks + 1;
        }
        return idx;
    }
#endif

    template <bool Prev, bool UseBmi2>
    __attribute__((always_inline))
    bool neighbor_index_impl(int l, int r, int xi, int &res) const {
        int prefix = 0;
        int candidate_l = 0, candidate_r = 0, candidate_d = -1, candidate_idx = 0;
        int d = 0;
        for (; d < lg && l < r; ++d) {
            const auto *row = bit.data() + d * blocks;
            const int *row_pref = pref.data() + d * (blocks + 1);
            int l1, r1;
#if defined(__GNUC__) && defined(__x86_64__)
            if constexpr (UseBmi2) rank1_pair_bmi2(row, row_pref, l, r, l1, r1);
            else rank1_pair(row, row_pref, l, r, l1, r1);
#else
            rank1_pair(row, row_pref, l, r, l1, r1);
#endif
            int l0 = l - l1, r0 = r - r1;
            int bit_value = (xi >> (lg - d - 1)) & 1;
            if constexpr (Prev) {
                if (bit_value) {
                    if (l0 < r0) {
                        candidate_l = l0;
                        candidate_r = r0;
                        candidate_d = d + 1;
                        candidate_idx = prefix << 1;
                    }
                    l = mid[d] + l1;
                    r = mid[d] + r1;
                    prefix = prefix << 1 | 1;
                }
                else {
                    l = l0;
                    r = r0;
                    prefix <<= 1;
                }
            }
            else {
                if (bit_value) {
                    l = mid[d] + l1;
                    r = mid[d] + r1;
                    prefix = prefix << 1 | 1;
                }
                else {
                    if (l1 < r1) {
                        candidate_l = mid[d] + l1;
                        candidate_r = mid[d] + r1;
                        candidate_d = d + 1;
                        candidate_idx = prefix << 1 | 1;
                    }
                    l = l0;
                    r = r0;
                    prefix <<= 1;
                }
            }
        }

        if constexpr (!Prev) {
            if (d == lg && l < r) {
                res = prefix;
                return true;
            }
        }
        if (candidate_d < 0) return false;

        l = candidate_l;
        r = candidate_r;
        prefix = candidate_idx;
        for (d = candidate_d; d < lg; ++d) {
            const auto *row = bit.data() + d * blocks;
            const int *row_pref = pref.data() + d * (blocks + 1);
            int l1, r1;
#if defined(__GNUC__) && defined(__x86_64__)
            if constexpr (UseBmi2) rank1_pair_bmi2(row, row_pref, l, r, l1, r1);
            else rank1_pair(row, row_pref, l, r, l1, r1);
#else
            rank1_pair(row, row_pref, l, r, l1, r1);
#endif
            int l0 = l - l1, r0 = r - r1;
            prefix <<= 1;
            if constexpr (Prev) {
                if (l1 < r1) {
                    prefix |= 1;
                    l = mid[d] + l1;
                    r = mid[d] + r1;
                }
                else {
                    l = l0;
                    r = r0;
                }
            }
            else {
                if (l0 < r0) {
                    l = l0;
                    r = r0;
                }
                else {
                    prefix |= 1;
                    l = mid[d] + l1;
                    r = mid[d] + r1;
                }
            }
        }
        res = prefix;
        return true;
    }

    template <bool Prev>
    bool neighbor_index_fallback(int l, int r, int xi, int &res) const {
        return neighbor_index_impl<Prev, false>(l, r, xi, res);
    }

#if defined(__GNUC__) && defined(__x86_64__)
    template <bool Prev>
    __attribute__((target("popcnt,bmi2")))
    bool neighbor_index_bmi2(int l, int r, int xi, int &res) const {
        return neighbor_index_impl<Prev, true>(l, r, xi, res);
    }
#endif

public:
    int count_less_index(int l, int r, int xi) const {
        if (xi <= 0 || l >= r || n == 0) return 0;
        if (xi >= (int)vals.size()) return r - l;
#if defined(__GNUC__) && defined(__x86_64__)
        if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2")) {
            return count_less_index_bmi2(l, r, xi);
        }
#endif
        return count_less_index_fallback(l, r, xi);
    }

    int count_less(int l, int r, const T &x) const {
        int xi = (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
        return count_less_index(l, r, xi);
    }

    int count_equal_index(int l, int r, int xi) const {
        if (l >= r || n == 0 || xi < 0 || xi >= (int)vals.size()) return 0;
#if defined(__GNUC__) && defined(__x86_64__)
        if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2")) {
            return count_equal_index_bmi2(l, r, xi);
        }
#endif
        return count_equal_index_fallback(l, r, xi);
    }

    vector<pair<int, int>> top_k_freq_index(int l, int r, int k) const {
        if (k <= 0 || l >= r || n == 0) return {};

#if defined(__GNUC__) && defined(__x86_64__)
        bool use_bmi2 = __builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2");
#endif

        struct Node {
            int l, r, d, idx;
            long long lower;
        };
        struct Item {
            int freq, idx;
        };

        auto item_better = [](const Item &a, const Item &b) {
            if (a.freq != b.freq) return a.freq > b.freq;
            return a.idx < b.idx;
        };
        auto node_worse = [](const Node &a, const Node &b) {
            int ca = a.r - a.l;
            int cb = b.r - b.l;
            if (ca != cb) return ca < cb;
            if (a.lower != b.lower) return a.lower > b.lower;
            return a.d < b.d;
        };

        vector<Node> heap;
        heap.push_back({l, r, 0, 0, 0});
        vector<Item> best;
        best.reserve(min(k, r - l));

        while (!heap.empty()) {
            if ((int)best.size() == k) {
                const Node &cur = heap.front();
                const Item &cut = best.front();
                int freq = cur.r - cur.l;
                if (freq < cut.freq) break;
                if (freq == cut.freq && cur.lower >= cut.idx) break;
            }

            pop_heap(heap.begin(), heap.end(), node_worse);
            Node cur = heap.back();
            heap.pop_back();

            if (cur.d == lg) {
                Item item{cur.r - cur.l, cur.idx};
                if ((int)best.size() < k) {
                    best.push_back(item);
                    push_heap(best.begin(), best.end(), item_better);
                }
                else if (item_better(item, best.front())) {
                    pop_heap(best.begin(), best.end(), item_better);
                    best.back() = item;
                    push_heap(best.begin(), best.end(), item_better);
                }
                continue;
            }

            const auto *row = bit.data() + cur.d * blocks;
            const int *row_pref = pref.data() + cur.d * (blocks + 1);
            int l1, r1;
#if defined(__GNUC__) && defined(__x86_64__)
            if (use_bmi2) rank1_pair_bmi2(row, row_pref, cur.l, cur.r, l1, r1);
            else rank1_pair(row, row_pref, cur.l, cur.r, l1, r1);
#else
            rank1_pair(row, row_pref, cur.l, cur.r, l1, r1);
#endif
            int l0 = cur.l - l1, r0 = cur.r - r1;
            int shift = lg - cur.d - 1;
            if (l0 < r0) {
                heap.push_back({l0, r0, cur.d + 1, cur.idx << 1, cur.lower});
                push_heap(heap.begin(), heap.end(), node_worse);
            }
            if (l1 < r1) {
                heap.push_back({
                                       mid[cur.d] + l1,
                                       mid[cur.d] + r1,
                                       cur.d + 1,
                                       cur.idx << 1 | 1,
                                       cur.lower + (1LL << shift)
                               });
                push_heap(heap.begin(), heap.end(), node_worse);
            }
        }

        sort(best.begin(), best.end(), item_better);
        vector<pair<int, int>> res;
        res.reserve(best.size());
        for (const auto &item : best) res.push_back({item.freq, item.idx});
        return res;
    }

    vector<pair<int, T>> top_k_freq(int l, int r, int k) const {
        auto idx_res = top_k_freq_index(l, r, k);
        vector<pair<int, T>> res;
        res.reserve(idx_res.size());
        for (const auto &p : idx_res) res.push_back({p.first, vals[p.second]});
        return res;
    }

    int range_freq(int l, int r, const T &lower, const T &upper) const {
        if (lower >= upper || l >= r) return 0;
        return count_less(l, r, upper) - count_less(l, r, lower);
    }

    int freq(int l, int r, const T &x) const {
        int xi = (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
        if (xi == (int)vals.size() || vals[xi] != x) return 0;
        return count_equal_index(l, r, xi);
    }

    T kth_smallest(int l, int r, int k) const {
#if defined(__GNUC__) && defined(__x86_64__)
        if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2")) {
            return vals[kth_smallest_index_bmi2(l, r, k)];
        }
#endif
        return vals[kth_smallest_index_fallback(l, r, k)];
    }

    T kth_largest(int l, int r, int k) const {
        return kth_smallest(l, r, r - l - 1 - k);
    }

    bool prev_value(int l, int r, const T &upper, T &res) const {
        if (l >= r || n == 0) return false;
        int xi = (int)(lower_bound(vals.begin(), vals.end(), upper) - vals.begin());
        if (xi <= 0) return false;
        if (xi >= (int)vals.size()) {
            res = kth_largest(l, r, 0);
            return true;
        }
#if defined(__GNUC__) && defined(__x86_64__)
        if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2")) {
            int idx;
            if (!neighbor_index_bmi2<true>(l, r, xi, idx)) return false;
            res = vals[idx];
            return true;
        }
#endif
        int idx;
        if (!neighbor_index_fallback<true>(l, r, xi, idx)) return false;
        res = vals[idx];
        return true;
    }

    bool next_value(int l, int r, const T &lower, T &res) const {
        if (l >= r || n == 0) return false;
        int xi = (int)(lower_bound(vals.begin(), vals.end(), lower) - vals.begin());
        if (xi >= (int)vals.size()) return false;
#if defined(__GNUC__) && defined(__x86_64__)
        if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("bmi2")) {
            int idx;
            if (!neighbor_index_bmi2<false>(l, r, xi, idx)) return false;
            res = vals[idx];
            return true;
        }
#endif
        int idx;
        if (!neighbor_index_fallback<false>(l, r, xi, idx)) return false;
        res = vals[idx];
        return true;
    }
};

/**
 * @brief Wavelet Matrix
 */


int main(){
    int n, q;
    din >> n >> q;
    vector<int> A(n);
    din >> A;
    WaveletMatrix<int> wm(A);
    while(q--){
        int l, r, k;
        din >> l >> r >> k;
        dout.puts(wm.kth_smallest(l, r, k));
    }
    return 0;
}