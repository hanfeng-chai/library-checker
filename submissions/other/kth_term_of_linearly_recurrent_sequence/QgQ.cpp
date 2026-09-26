#line 1 "y.cpp"
#include <bits/stdc++.h>
#include <immintrin.h>
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("O3")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#endif

#line 2 "IO/fastio_unsafe.hpp"
#line 11 "IO/fastio_unsafe.hpp"
#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 14
#endif

namespace fastio_unsafe_impl{
using i32=std::int32_t;
using u32=std::uint32_t;
using i64=std::int64_t;
using u64=std::uint64_t;
using i128=__int128_t;
using u128=__uint128_t;

constexpr auto make_right_align_masks(){
    std::array<std::array<char,16>,16> masks{};
    for(int digits=0;digits<16;++digits)for(int i=0;i<16;++i)
        masks[digits][i]=i<16-digits?static_cast<char>(0x80):static_cast<char>(i-(16-digits));
    return masks;
}
constexpr auto make_powers_10(){
    std::array<u64,17> powers{};
    powers[0]=1;
    for(std::size_t i=1;i<powers.size();++i)powers[i]=powers[i-1]*10;
    return powers;
}
constexpr auto make_pair_digits(){
    std::array<unsigned char,1<<14> table{};
    table.fill(255);
    for(unsigned a=0;a<10;++a)for(unsigned b=0;b<10;++b)
        table[('0'+a)|(('0'+b)<<8)]=static_cast<unsigned char>(a*10+b);
    return table;
}
alignas(16) inline constexpr auto right_align_masks=make_right_align_masks();
inline constexpr auto powers_10=make_powers_10();
inline constexpr auto pair_digits=make_pair_digits();

struct input{
    input(){
#ifdef __linux__
        struct stat info{};
        if(::fstat(0,&info)==0&&S_ISREG(info.st_mode)&&info.st_size>0){
            const off_t current=::lseek(0,0,SEEK_CUR);
            const std::size_t file_size=static_cast<std::size_t>(info.st_size);
            const std::size_t page_size=static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
            const std::size_t rounded_size=(file_size+page_size-1)/page_size*page_size;
            const std::size_t reserved_size=rounded_size+page_size;
            char* region=static_cast<char*>(::mmap(nullptr,reserved_size,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
            if(region!=MAP_FAILED){
                void* file_mapping=::mmap(region,rounded_size,PROT_READ,MAP_PRIVATE|MAP_FIXED,0,0);
                void* zero_page=file_mapping==MAP_FAILED?MAP_FAILED: ::mmap(region+rounded_size,page_size,PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,-1,0);
                if(file_mapping!=MAP_FAILED&&zero_page!=MAP_FAILED){
                    const std::size_t offset=current>0?std::min(static_cast<std::size_t>(current),file_size):0;
                    cursor_=region+offset;
                    return;
                }
                ::munmap(region,reserved_size);
            }
        }
#endif
        read_all_fallback();
    }
    input(const input&)=delete;
    input& operator=(const input&)=delete;
    char* cursor()const noexcept{return cursor_;}
private:
    void read_all_fallback(){
        std::size_t capacity=1u<<20,size=0;
        char* buffer=static_cast<char*>(std::malloc(capacity+64));
        if(buffer==nullptr)std::abort();
        for(;;){
            if(size==capacity){
                capacity*=2;
                char* grown=static_cast<char*>(std::realloc(buffer,capacity+64));
                if(grown==nullptr)std::abort();
                buffer=grown;
            }
            const std::size_t count=std::fread(buffer+size,1,capacity-size,stdin);
            size+=count;
            if(count==0)break;
        }
        std::memset(buffer+size,0,64);
        cursor_=buffer;
    }
    char* cursor_=nullptr;
};

__attribute__((always_inline)) inline u64 parse_16_digits(__m128i digits)noexcept{
    const __m128i pair_weights=_mm_set1_epi16(0x010A);
    const __m128i quad_weights=_mm_set1_epi32(0x00010064);
    const __m128i oct_weights=_mm_set_epi32(1,10000,1,10000);
    const __m128i pairs=_mm_maddubs_epi16(digits,pair_weights);
    const __m128i quads=_mm_madd_epi16(pairs,quad_weights);
    const __m128i products=_mm_mul_epu32(quads,oct_weights);
    const __m128i odd=_mm_srli_epi64(quads,32);
    const __m128i octets=_mm_add_epi64(products,odd);
    const u64 high=static_cast<u64>(_mm_cvtsi128_si64(octets));
    const u64 low=static_cast<u64>(_mm_extract_epi64(octets,1));
    return high*100000000ULL+low;
}
__attribute__((always_inline)) inline __m128i load_digits(const char* cursor)noexcept{
    return _mm_sub_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i*>(cursor)),_mm_set1_epi8('0'));
}
__attribute__((always_inline)) inline u64 parse_short_digits(__m128i digits,u32 mask,int& length)noexcept{
    length=__builtin_ctz(mask);
    digits=_mm_shuffle_epi8(digits,_mm_load_si128(reinterpret_cast<const __m128i*>(right_align_masks[static_cast<std::size_t>(length)].data())));
    return parse_16_digits(digits);
}
__attribute__((always_inline)) inline u32 read_u32(char*& cursor)noexcept{
    const __m128i digits=load_digits(cursor);
    const u32 mask=static_cast<u32>(_mm_movemask_epi8(digits));
    int length;
    const u32 value=static_cast<u32>(parse_short_digits(digits,mask,length));
    cursor+=length+1;
    return value;
}
__attribute__((always_inline)) inline u32 read_u32_lt1e9(char*& cursor)noexcept{
    const auto q0=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+1)];
    const auto q1=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+3)];
    const auto q2=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+5)];
    const auto q3=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+7)];
    if(__builtin_expect((q0|q1|q2|q3)<128,1)){
        u32 value=static_cast<unsigned char>(cursor[0])-'0';
        value=value*100+q0;value=value*100+q1;value=value*100+q2;value=value*100+q3;
        cursor+=10;
        return value;
    }
    u32 value=static_cast<unsigned char>(*cursor++)-'0';
    for(unsigned i=0;i<4;++i){
        const auto pair=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor)];
        if(pair>99)break;
        value=value*100+pair;
        cursor+=2;
    }
    if(*cursor>' ')value=value*10+static_cast<unsigned>(*cursor++&15);
    ++cursor;
    return value;
}
__attribute__((always_inline)) inline i32 read_i32(char*& cursor)noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u32 magnitude=read_u32(cursor);
    const u32 bits=negative?u32{0}-magnitude:magnitude;
    return static_cast<i32>(bits);
}
__attribute__((always_inline)) inline u64 read_u64(char*& cursor)noexcept{
    __m128i digits=load_digits(cursor);
    const u32 mask=static_cast<u32>(_mm_movemask_epi8(digits));
    if(__builtin_expect(mask!=0,1)){
        int length;
        const u64 value=parse_short_digits(digits,mask,length);
        cursor+=length+1;
        return value;
    }
    u64 value=parse_16_digits(digits);
    cursor+=16;
    while(*cursor>='0'){value=value*10+static_cast<unsigned>(*cursor&15);++cursor;}
    ++cursor;
    return value;
}
__attribute__((always_inline)) inline i64 read_i64(char*& cursor)noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u64 magnitude=read_u64(cursor);
    const u64 bits=negative?u64{0}-magnitude:magnitude;
    return static_cast<i64>(bits);
}
__attribute__((always_inline)) inline u128 read_u128(char*& cursor)noexcept{
    __m128i digits=load_digits(cursor);
    u32 mask=static_cast<u32>(_mm_movemask_epi8(digits));
    if(__builtin_expect(mask!=0,0)){
        int length;
        const u128 value=parse_short_digits(digits,mask,length);
        cursor+=length+1;
        return value;
    }
    u128 value=parse_16_digits(digits);
    cursor+=16;
    digits=load_digits(cursor);
    mask=static_cast<u32>(_mm_movemask_epi8(digits));
    if(mask!=0){
        int length;
        const u64 tail=parse_short_digits(digits,mask,length);
        cursor+=length+1;
        return value*powers_10[static_cast<std::size_t>(length)]+tail;
    }
    value=value*static_cast<u128>(10000000000000000ULL)+parse_16_digits(digits);
    cursor+=16;
    digits=load_digits(cursor);
    mask=static_cast<u32>(_mm_movemask_epi8(digits));
    int length;
    const u64 tail=parse_short_digits(digits,mask,length);
    cursor+=length+1;
    return value*powers_10[static_cast<std::size_t>(length)]+tail;
}
__attribute__((always_inline)) inline i128 read_i128(char*& cursor)noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u128 magnitude=read_u128(cursor);
    const u128 bits=negative?u128{0}-magnitude:magnitude;
    return static_cast<i128>(bits);
}

constexpr u32 pack4(char a,char b,char c,char d)noexcept{
    return static_cast<u32>(static_cast<unsigned char>(a))|
           (static_cast<u32>(static_cast<unsigned char>(b))<<8)|
           (static_cast<u32>(static_cast<unsigned char>(c))<<16)|
           (static_cast<u32>(static_cast<unsigned char>(d))<<24);
}
constexpr auto make_padded_groups(){
    std::array<u32,10000> table{};
    for(int value=0;value<10000;++value)table[static_cast<std::size_t>(value)]=pack4(
        static_cast<char>('0'+value/1000),static_cast<char>('0'+value/100%10),
        static_cast<char>('0'+value/10%10),static_cast<char>('0'+value%10));
    return table;
}
inline constexpr auto padded_groups=make_padded_groups();

struct output{
    output()=default;
    output(const output&)=delete;
    output& operator=(const output&)=delete;
    char* begin()noexcept{return buffer_.data();}
    char* end()noexcept{return buffer_.data()+buffer_.size();}
    __attribute__((noinline)) char* flush(char* cursor)noexcept{
        std::size_t remaining=static_cast<std::size_t>(cursor-buffer_.data());
        const char* data=buffer_.data();
        if(first_flush_&&remaining!=0){++data;--remaining;first_flush_=false;}
#ifdef __linux__
        while(remaining!=0){
            const ssize_t count=::write(1,data,remaining);
            if(count>0){data+=count;remaining-=static_cast<std::size_t>(count);}
            else if(count<0&&errno==EINTR)continue;
            else std::abort();
        }
#else
        while(remaining!=0){
            const std::size_t count=std::fwrite(data,1,remaining,stdout);
            if(count==0)std::abort();
            data+=count;remaining-=count;
        }
#endif
        return buffer_.data();
    }
    void finish(char* cursor)noexcept{
        if(cursor!=buffer_.data()||!first_flush_)*cursor++='\n';
        (void)flush(cursor);
    }
    alignas(64) std::array<char,1u<<19> buffer_;
    bool first_flush_=true;
};

__attribute__((always_inline)) inline void store_group(char*& cursor,u32 group)noexcept{
    std::memcpy(cursor,&group,sizeof(group));cursor+=sizeof(group);
}
__attribute__((always_inline)) inline void emit_leading(char*& cursor,u64 value)noexcept{
    const unsigned skip=3u-static_cast<unsigned>(value>=10)-static_cast<unsigned>(value>=100)-static_cast<unsigned>(value>=1000);
    const u32 group=padded_groups[static_cast<std::size_t>(value)]>>(skip*8);
    std::memcpy(cursor,&group,sizeof(group));cursor+=4-skip;
}
__attribute__((always_inline)) inline void emit_padded(char*& cursor,u64 value)noexcept{
    store_group(cursor,padded_groups[static_cast<std::size_t>(value)]);
}
__attribute__((always_inline)) inline void emit_padded_16(char*& cursor,u64 value)noexcept{
    emit_padded(cursor,value/1000000000000ULL);
    emit_padded(cursor,value/100000000ULL%10000);
    emit_padded(cursor,value/10000ULL%10000);
    emit_padded(cursor,value%10000);
}
__attribute__((always_inline)) inline void emit_u32_unchecked(char*& cursor,u32 value)noexcept{
    if(value>=100000000U){
        emit_leading(cursor,value/100000000U);
        emit_padded(cursor,value/10000U%10000);
        emit_padded(cursor,value%10000);
    }else if(value>=10000U){
        emit_leading(cursor,value/10000U);
        emit_padded(cursor,value%10000);
    }else emit_leading(cursor,value);
}
__attribute__((always_inline)) inline void emit_u64_unchecked(char*& cursor,u64 value)noexcept{
    if(value>=10000000000000000ULL){
        emit_leading(cursor,value/10000000000000000ULL);
        emit_padded_16(cursor,value%10000000000000000ULL);
    }else if(value>=1000000000000ULL){
        emit_leading(cursor,value/1000000000000ULL);
        emit_padded(cursor,value/100000000ULL%10000);
        emit_padded(cursor,value/10000ULL%10000);
        emit_padded(cursor,value%10000);
    }else if(value>=100000000ULL){
        emit_leading(cursor,value/100000000ULL);
        emit_padded(cursor,value/10000ULL%10000);
        emit_padded(cursor,value%10000);
    }else if(value>=10000ULL){
        emit_leading(cursor,value/10000);
        emit_padded(cursor,value%10000);
    }else emit_leading(cursor,value);
}
__attribute__((always_inline)) inline void emit_u128_unchecked(char*& cursor,u128 value)noexcept{
    constexpr u128 base=static_cast<u128>(10000000000000000ULL);
    constexpr u128 u64_max=static_cast<u128>(~u64{0});
    if(value<=u64_max){emit_u64_unchecked(cursor,static_cast<u64>(value));return;}
    const u64 low=static_cast<u64>(value%base);
    const u128 upper=value/base;
    if(upper<=u64_max){
        emit_u64_unchecked(cursor,static_cast<u64>(upper));
        emit_padded_16(cursor,low);
        return;
    }
    const u64 middle=static_cast<u64>(upper%base);
    const u32 high=static_cast<u32>(upper/base);
    emit_u32_unchecked(cursor,high);
    emit_padded_16(cursor,middle);
    emit_padded_16(cursor,low);
}
__attribute__((always_inline)) inline void write_u32_lt1e9(output& sink,char*& cursor,char* end,u32 value)noexcept{
    if(__builtin_expect(end-cursor<16,0))cursor=sink.flush(cursor);
    *cursor++=' ';
    if(value>=100000000U){
        const u32 high=value/100000000U;
        *cursor++=static_cast<char>('0'+high);
        value-=high*100000000U;
        emit_padded(cursor,value/10000U);
        emit_padded(cursor,value%10000U);
    }else emit_u32_unchecked(cursor,value);
}
__attribute__((always_inline)) inline void write_u32(output& sink,char*& cursor,char* end,u32 value)noexcept{
    if(__builtin_expect(end-cursor<16,0))cursor=sink.flush(cursor);
    *cursor++=' ';emit_u32_unchecked(cursor,value);
}
__attribute__((always_inline)) inline void write_i32(output& sink,char*& cursor,char* end,i32 value)noexcept{
    if(__builtin_expect(end-cursor<16,0))cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u32 bits=static_cast<u32>(value),magnitude=negative?u32{0}-bits:bits;
    *cursor++=' ';if(negative)*cursor++='-';
    emit_u32_unchecked(cursor,magnitude);
}
__attribute__((always_inline)) inline void write_u64(output& sink,char*& cursor,char* end,u64 value)noexcept{
    if(__builtin_expect(end-cursor<24,0))cursor=sink.flush(cursor);
    *cursor++=' ';emit_u64_unchecked(cursor,value);
}
__attribute__((always_inline)) inline void write_i64(output& sink,char*& cursor,char* end,i64 value)noexcept{
    if(__builtin_expect(end-cursor<24,0))cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u64 bits=static_cast<u64>(value),magnitude=negative?u64{0}-bits:bits;
    *cursor++=' ';if(negative)*cursor++='-';
    emit_u64_unchecked(cursor,magnitude);
}
__attribute__((always_inline)) inline void write_u128(output& sink,char*& cursor,char* end,u128 value)noexcept{
    if(__builtin_expect(end-cursor<48,0))cursor=sink.flush(cursor);
    *cursor++=' ';emit_u128_unchecked(cursor,value);
}
__attribute__((always_inline)) inline void write_i128(output& sink,char*& cursor,char* end,i128 value)noexcept{
    if(__builtin_expect(end-cursor<48,0))cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u128 bits=static_cast<u128>(value),magnitude=negative?u128{0}-bits:bits;
    *cursor++=' ';if(negative)*cursor++='-';
    emit_u128_unchecked(cursor,magnitude);
}
}

struct fastio_unsafe{
    using i32=fastio_unsafe_impl::i32;
    using u32=fastio_unsafe_impl::u32;
    using i64=fastio_unsafe_impl::i64;
    using u64=fastio_unsafe_impl::u64;
    using i128=fastio_unsafe_impl::i128;
    using u128=fastio_unsafe_impl::u128;
    fastio_unsafe()=default;
    fastio_unsafe(const fastio_unsafe&)=delete;
    fastio_unsafe& operator=(const fastio_unsafe&)=delete;
    char* input_cursor()const noexcept{return in.cursor();}
    char* output_cursor()noexcept{return out.begin();}
    char* output_end()noexcept{return out.end();}
    void finish(char* cursor)noexcept{out.finish(cursor);}
    __attribute__((always_inline)) u32 read_u32(char*& cursor)noexcept{return fastio_unsafe_impl::read_u32(cursor);}
    __attribute__((always_inline)) u32 read_u32_lt1e9(char*& cursor)noexcept{return fastio_unsafe_impl::read_u32_lt1e9(cursor);}
    __attribute__((always_inline)) i32 read_i32(char*& cursor)noexcept{return fastio_unsafe_impl::read_i32(cursor);}
    __attribute__((always_inline)) u64 read_u64(char*& cursor)noexcept{return fastio_unsafe_impl::read_u64(cursor);}
    __attribute__((always_inline)) i64 read_i64(char*& cursor)noexcept{return fastio_unsafe_impl::read_i64(cursor);}
    __attribute__((always_inline)) u128 read_u128(char*& cursor)noexcept{return fastio_unsafe_impl::read_u128(cursor);}
    __attribute__((always_inline)) i128 read_i128(char*& cursor)noexcept{return fastio_unsafe_impl::read_i128(cursor);}
    __attribute__((always_inline)) void write_u32(char*& cursor,char* end,u32 value)noexcept{fastio_unsafe_impl::write_u32(out,cursor,end,value);}
    __attribute__((always_inline)) void write_u32_lt1e9(char*& cursor,char* end,u32 value)noexcept{fastio_unsafe_impl::write_u32_lt1e9(out,cursor,end,value);}
    __attribute__((always_inline)) void write_i32(char*& cursor,char* end,i32 value)noexcept{fastio_unsafe_impl::write_i32(out,cursor,end,value);}
    __attribute__((always_inline)) void write_u64(char*& cursor,char* end,u64 value)noexcept{fastio_unsafe_impl::write_u64(out,cursor,end,value);}
    __attribute__((always_inline)) void write_i64(char*& cursor,char* end,i64 value)noexcept{fastio_unsafe_impl::write_i64(out,cursor,end,value);}
    __attribute__((always_inline)) void write_u128(char*& cursor,char* end,u128 value)noexcept{fastio_unsafe_impl::write_u128(out,cursor,end,value);}
    __attribute__((always_inline)) void write_i128(char*& cursor,char* end,i128 value)noexcept{fastio_unsafe_impl::write_i128(out,cursor,end,value);}
    fastio_unsafe_impl::input in;
    fastio_unsafe_impl::output out;
};

#line 2 "convolution/ntt998.hpp"
#if ((defined(__GNUC__)||defined(__clang__))&&(defined(__x86_64__)||defined(__i386__)))||defined(_M_AVX2)
#define EEZ_NTT998_USE_AVX2 1
#else
#error "eez::ntt998 requires an x86 target with AVX2 support"
#endif
#line 11 "convolution/ntt998.hpp"
#include <bit>
#line 17 "convolution/ntt998.hpp"
#include <span>
#line 19 "convolution/ntt998.hpp"
#include <type_traits>

#line 1 "math/modint998.hpp"
#line 5 "math/modint998.hpp"
#line 7 "math/modint998.hpp"
#line 9 "math/modint998.hpp"
#line 11 "math/modint998.hpp"

struct modint998{
    using u32=std::uint32_t;
    using i32=std::int32_t;
    using u64=std::uint64_t;
    static constexpr u32 MOD=998244353u;
    static constexpr u32 MOD2=MOD*2;
    static constexpr u32 primitive_root=3;
    static constexpr int max_power_of_two=23;
private:
    static constexpr u32 R=3296722945u;
    static constexpr u32 N2=932051910u;
    struct montgomery_tag{};
    constexpr modint998(u32 x,montgomery_tag):a(x){}
    static constexpr u32 reduce(u64 x){
        return static_cast<u32>((x+u64(static_cast<u32>(x)*u32(-R))*MOD)>>32);
    }
public:
    u32 a;
    static_assert(MOD<(u32(1)<<30));
    static_assert((MOD&1)!=0);
    static_assert(R*MOD==1);
    constexpr modint998():a(0){}
    template<class T,std::enable_if_t<std::is_integral_v<T>&&std::is_signed_v<T>,int> =0>
    constexpr modint998(T x):a(0){
        const std::int64_t y=static_cast<std::int64_t>(x)%std::int64_t(MOD)+MOD;
        a=reduce(u64(y)*N2);
    }
    template<class T,std::enable_if_t<std::is_integral_v<T>&&std::is_unsigned_v<T>,int> =0>
    constexpr modint998(T x):a(reduce(((u64(x)%MOD)+MOD)*N2)){}
    static constexpr modint998 raw(u32 x){return modint998(reduce(u64(x)*N2),montgomery_tag{});}
    static constexpr modint998 montgomery_raw(u32 x){return modint998(x,montgomery_tag{});}
    static constexpr u32 mod(){return MOD;}
    static constexpr u32 get_mod(){return MOD;}
    constexpr u32 val()const{const u32 x=reduce(a);return x>=MOD?x-MOD:x;}
    constexpr u32 get()const{return val();}
    constexpr modint998& operator+=(const modint998& rhs){a+=rhs.a-MOD2;if(i32(a)<0)a+=MOD2;return *this;}
    constexpr modint998& operator-=(const modint998& rhs){a-=rhs.a;if(i32(a)<0)a+=MOD2;return *this;}
    constexpr modint998& operator*=(const modint998& rhs){a=reduce(u64(a)*rhs.a);return *this;}
    constexpr modint998& operator/=(const modint998& rhs){return *this*=rhs.inv();}
    constexpr modint998 operator+()const{return *this;}
    constexpr modint998 operator-()const{return modint998()-*this;}
    friend constexpr modint998 operator+(modint998 lhs,const modint998& rhs){return lhs+=rhs;}
    friend constexpr modint998 operator-(modint998 lhs,const modint998& rhs){return lhs-=rhs;}
    friend constexpr modint998 operator*(modint998 lhs,const modint998& rhs){return lhs*=rhs;}
    friend constexpr modint998 operator/(modint998 lhs,const modint998& rhs){return lhs/=rhs;}
    friend constexpr bool operator==(const modint998& lhs,const modint998& rhs){
        const u32 x=lhs.a>=MOD?lhs.a-MOD:lhs.a,y=rhs.a>=MOD?rhs.a-MOD:rhs.a;
        return x==y;
    }
    friend constexpr bool operator!=(const modint998& lhs,const modint998& rhs){return !(lhs==rhs);}
    constexpr modint998& operator++(){return *this+=raw(1);}
    constexpr modint998 operator++(int){modint998 old=*this;++*this;return old;}
    constexpr modint998& operator--(){return *this-=raw(1);}
    constexpr modint998 operator--(int){modint998 old=*this;--*this;return old;}
    constexpr modint998 pow(u64 e)const{
        if(e==0)return raw(1);
        if(a==0)return raw(0);
        if(e>=MOD-1)e%=MOD-1;
        if(e==0)return raw(1);
        const u32 n=static_cast<u32>(e);
        if(n==1)return *this;
        const modint998 x=*this,p2=x*x;
        if(n==2)return p2;
        const modint998 p3=p2*x;
        if(n==3)return p3;
        const modint998 p4=p2*p2,p5=p4*x,p6=p3*p3,p7=p4*p3;
        const modint998 t[8]={raw(1),x,p2,p3,p4,p5,p6,p7};
        const unsigned s=((std::bit_width(n)-1)/3)*3;
        modint998 r=t[(n>>s)&7];
        auto step=[&](unsigned k)constexpr{
            r*=r;r*=r;r*=r;
            const u32 d=(n>>k)&7;
            if(d)r*=t[d];
        };
        switch(s){
            case 27:step(24);[[fallthrough]];
            case 24:step(21);[[fallthrough]];
            case 21:step(18);[[fallthrough]];
            case 18:step(15);[[fallthrough]];
            case 15:step(12);[[fallthrough]];
            case 12:step(9);[[fallthrough]];
            case 9:step(6);[[fallthrough]];
            case 6:step(3);[[fallthrough]];
            case 3:step(0);[[fallthrough]];
            default:break;
        }
        return r;
    }
    constexpr modint998 inv()const{
        assert(val()!=0);
        const modint998 x=*this;
        const modint998 a2=x*x,a4=a2*a2,a5=a4*x,a9=a5*a4,a18=a9*a9,a36=a18*a18,a72=a36*a36,a144=a72*a72;
        const modint998 a288=a144*a144,a293=a288*a5,a586=a293*a293,a879=a586*a293,a1023=a879*a144;
        modint998 r=a1023*a879;
        r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;
        r*=a1023;
        r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;r*=r;
        return r*a1023;
    }
    constexpr modint998 inverse()const{return inv();}
    friend std::ostream& operator<<(std::ostream& os,const modint998& x){return os<<x.val();}
    friend std::istream& operator>>(std::istream& is,modint998& x){std::int64_t value;is>>value;x=modint998(value);return is;}
};
static_assert(sizeof(modint998)==4);
static_assert(std::is_trivially_copyable_v<modint998>);
using mint998=modint998;

#line 22 "convolution/ntt998.hpp"
#if defined(__GNUC__)&&!defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma GCC push_options
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma clang attribute push(__attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))),apply_to=function)
#endif
#if defined(_MSC_VER)
#define EEZ_NTT998_ALWAYS_INLINE __forceinline
#define EEZ_NTT998_RESTRICT __restrict
#elif defined(__GNUC__)||defined(__clang__)
#define EEZ_NTT998_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_NTT998_RESTRICT __restrict__
#else
#define EEZ_NTT998_ALWAYS_INLINE inline
#define EEZ_NTT998_RESTRICT
#endif

namespace eez::ntt998{
using mint=modint998;
using u32=std::uint32_t;
using usize=std::size_t;
inline constexpr u32 mod=mint::MOD;
inline constexpr usize max_ntt_size=usize(1)<<23;
inline constexpr usize max_convolution_size=usize(1)<<25;
inline constexpr usize max_size=max_ntt_size;
inline constexpr usize naive_cutoff=48;
inline void forward(std::span<mint> a)noexcept;
inline void inverse(std::span<mint> a)noexcept;
inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b);
inline std::vector<mint> square(std::span<const mint> a);
inline std::vector<mint> convolution(const std::vector<mint>& a,const std::vector<mint>& b){
    return convolution(std::span<const mint>(a.data(),a.size()),std::span<const mint>(b.data(),b.size()));
}
inline std::vector<mint> square(const std::vector<mint>& a){return square(std::span<const mint>(a.data(),a.size()));}

namespace detail{
template<class T>class aligned_allocator{
public:
    using value_type=T;
    using is_always_equal=std::true_type;
    aligned_allocator()noexcept=default;
    template<class U>constexpr aligned_allocator(const aligned_allocator<U>&)noexcept{}
    [[nodiscard]] T* allocate(usize n){return static_cast<T*>(::operator new(n*sizeof(T),std::align_val_t{64}));}
    void deallocate(T* p,usize)noexcept{::operator delete(p,std::align_val_t{64});}
    template<class U>struct rebind{using other=aligned_allocator<U>;};
};
template<class T,class U>constexpr bool operator==(const aligned_allocator<T>&,const aligned_allocator<U>&)noexcept{return true;}
template<class T,class U>constexpr bool operator!=(const aligned_allocator<T>&,const aligned_allocator<U>&)noexcept{return false;}
using aligned_vector=std::vector<mint,aligned_allocator<mint>>;
}

class workspace{
public:
    workspace()=default;
    explicit workspace(usize n){reserve(n);}
    void reserve(usize n){if(a_.size()<n)a_.resize(n);if(b_.size()<n)b_.resize(n);}
    [[nodiscard]] usize capacity()const noexcept{return std::min(a_.size(),b_.size());}
private:
    friend void convolution_to(std::span<const mint>,std::span<const mint>,std::span<mint>,workspace&);
    friend void square_to(std::span<const mint>,std::span<mint>,workspace&);
    detail::aligned_vector a_,b_;
};
inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws);
inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws);

class frequency_buffer{
public:
    frequency_buffer()=default;
    [[nodiscard]] usize size()const noexcept{return data_.size();}
private:
    friend void forward_to(std::span<const mint>,frequency_buffer&,usize);
    friend void pointwise_multiply(frequency_buffer&,const frequency_buffer&);
    friend void pointwise_square(frequency_buffer&);
    friend void inverse_to(frequency_buffer&,std::span<mint>);
    std::vector<mint> data_;
};
inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n);
inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs);
inline void pointwise_square(frequency_buffer& a);
inline void inverse_to(frequency_buffer& src,std::span<mint> out);

constexpr usize convolution_size(usize n,usize m)noexcept{return n&&m?n+m-1:0;}
constexpr usize transform_size(usize n,usize m)noexcept{
    if(!n||!m)return 0;
    if(n>max_ntt_size||m>max_ntt_size)return 0;
    if(n>max_ntt_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;while(x<z)x<<=1;
    return x;
}
constexpr usize convolution_transform_size(usize n,usize m)noexcept{
    if(!n||!m)return 0;
    if(n>max_convolution_size||m>max_convolution_size)return 0;
    if(n>max_convolution_size-m+1)return 0;
    const usize z=n+m-1;
    usize x=1;while(x<z)x<<=1;
    return x;
}
constexpr bool valid_ntt_size(usize n)noexcept{return n!=0&&(n&(n-1))==0&&n<=max_ntt_size;}
constexpr bool valid_convolution_transform_size(usize n)noexcept{return n>=32&&(n&(n-1))==0&&n<=max_convolution_size;}

namespace detail{
using word=u32;
using u64=std::uint64_t;
inline constexpr word mod=mint::MOD;
inline constexpr word mod2=2*mod;
inline constexpr unsigned max_log=23;
inline constexpr word montgomery_ninv=998244351u;
inline constexpr word montgomery_one=mint::raw(1).a;
static_assert(mod<(word(1)<<30));
static_assert(word(mod*montgomery_ninv)==~word(0));
static_assert(sizeof(mint)==sizeof(word));
EEZ_NTT998_ALWAYS_INLINE constexpr word raw(const mint& x)noexcept{return x.a;}
EEZ_NTT998_ALWAYS_INLINE constexpr mint from_raw(word x)noexcept{return mint::montgomery_raw(x);}
EEZ_NTT998_ALWAYS_INLINE constexpr word mul(word a,word b)noexcept{
    const u64 x=u64(a)*b;
    const word q=static_cast<word>(x)*montgomery_ninv;
    return static_cast<word>((x+u64(q)*mod)>>32);
}
EEZ_NTT998_ALWAYS_INLINE constexpr word add(word a,word b)noexcept{const word x=a+b;return x>=mod2?x-mod2:x;}
EEZ_NTT998_ALWAYS_INLINE constexpr word sub(word a,word b)noexcept{return a>=b?a-b:a+mod2-b;}
EEZ_NTT998_ALWAYS_INLINE constexpr word canonicalize(word a)noexcept{return a>=mod?a-mod:a;}

struct twiddle_table{
    std::array<word,max_log+1> root{},iroot{},rate1{},rate3{},irate3{};
    constexpr twiddle_table(){
        root[max_log]=mint::raw(mint::primitive_root).pow((mod-1)>>max_log).a;
        iroot[max_log]=mint::montgomery_raw(root[max_log]).inv().a;
        for(int i=int(max_log)-1;i>=0;--i){
            root[usize(i)]=mul(root[usize(i+1)],root[usize(i+1)]);
            iroot[usize(i)]=mul(iroot[usize(i+1)],iroot[usize(i+1)]);
        }
        word prod=montgomery_one;
        for(unsigned i=0;i+1<=max_log;++i){rate1[i]=mul(root[i+1],prod);prod=mul(prod,iroot[i+1]);}
        prod=montgomery_one;
        word iprod=montgomery_one;
        for(unsigned i=0;i+3<=max_log;++i){
            rate3[i]=mul(root[i+3],prod);
            irate3[i]=mul(iroot[i+3],iprod);
            prod=mul(prod,iroot[i+3]);
            iprod=mul(iprod,root[i+3]);
        }
    }
};
inline constexpr twiddle_table twiddles{};
EEZ_NTT998_ALWAYS_INLINE word forward_rate1(unsigned i)noexcept{return twiddles.rate1[i];}
EEZ_NTT998_ALWAYS_INLINE word forward_rate3(unsigned i)noexcept{return twiddles.rate3[i];}
EEZ_NTT998_ALWAYS_INLINE word inverse_rate3(unsigned i)noexcept{return twiddles.irate3[i];}
EEZ_NTT998_ALWAYS_INLINE unsigned twiddle_index(u32 block)noexcept{return static_cast<unsigned>(std::countr_zero(~block));}

#if EEZ_NTT998_USE_AVX2
using vec=__m256i;
EEZ_NTT998_ALWAYS_INLINE vec load8(const mint* p)noexcept{return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));}
EEZ_NTT998_ALWAYS_INLINE void store8(mint* p,vec x)noexcept{_mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);}
EEZ_NTT998_ALWAYS_INLINE vec broadcast(word x)noexcept{return _mm256_set1_epi32(static_cast<int>(x));}

/* 変更①: signed-mask 補正から unsigned min 補正へ */
EEZ_NTT998_ALWAYS_INLINE vec add8(vec a,vec b)noexcept{
    const vec x=_mm256_add_epi32(a,b);
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));
}
EEZ_NTT998_ALWAYS_INLINE vec sub8(vec a,vec b)noexcept{
    const vec x=_mm256_sub_epi32(a,b);
    return _mm256_min_epu32(x,_mm256_add_epi32(x,broadcast(mod2)));
}
// Lazy sums are used before multiplication only with a multiplier below mod.
// Input < 4*mod and multiplier < mod imply Montgomery output < 2*mod.
EEZ_NTT998_ALWAYS_INLINE vec lazy_add8(vec a,vec b)noexcept{return _mm256_add_epi32(a,b);}
EEZ_NTT998_ALWAYS_INLINE vec lazy_sub8(vec a,vec b)noexcept{return _mm256_add_epi32(a,_mm256_sub_epi32(broadcast(mod2),b));}
EEZ_NTT998_ALWAYS_INLINE vec mul8(vec a,vec b)noexcept{
    const vec ninv=broadcast(montgomery_ninv),prime=broadcast(mod);
    const vec product_even=_mm256_mul_epu32(a,b);
    const vec product_odd=_mm256_mul_epu32(_mm256_srli_epi64(a,32),_mm256_srli_epi64(b,32));
    const vec q_even=_mm256_mul_epu32(product_even,ninv),q_odd=_mm256_mul_epu32(product_odd,ninv);
    const vec reduced_even=_mm256_add_epi64(product_even,_mm256_mul_epu32(q_even,prime));
    const vec reduced_odd=_mm256_add_epi64(product_odd,_mm256_mul_epu32(q_odd,prime));
    return _mm256_or_si256(_mm256_srli_epi64(reduced_even,32),reduced_odd);
}
EEZ_NTT998_ALWAYS_INLINE vec mul8_fixed(vec a,vec b,vec bninv)noexcept{
    const vec prime=broadcast(mod),odd_a=_mm256_srli_epi64(a,32);
    const vec product_even=_mm256_mul_epu32(a,b),product_odd=_mm256_mul_epu32(odd_a,b);
    const vec q_even=_mm256_mul_epu32(a,bninv),q_odd=_mm256_mul_epu32(odd_a,bninv);
    const vec reduced_even=_mm256_add_epi64(product_even,_mm256_mul_epu32(q_even,prime));
    const vec reduced_odd=_mm256_add_epi64(product_odd,_mm256_mul_epu32(q_odd,prime));
    return _mm256_or_si256(_mm256_srli_epi64(reduced_even,32),reduced_odd);
}
EEZ_NTT998_ALWAYS_INLINE vec canonicalize8(vec x)noexcept{
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod)));
}
EEZ_NTT998_ALWAYS_INLINE vec pack_four(word x0,word x1)noexcept{
    return _mm256_setr_epi32((int)x0,(int)x0,(int)x0,(int)x0,(int)x1,(int)x1,(int)x1,(int)x1);
}
EEZ_NTT998_ALWAYS_INLINE vec load2x4(const mint* p0,const mint* p1)noexcept{
    const __m128i lo=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p0)));
    const __m128i hi=_mm_loadu_si128(reinterpret_cast<const __m128i*>(static_cast<const void*>(p1)));
    return _mm256_set_m128i(hi,lo);
}
EEZ_NTT998_ALWAYS_INLINE void store2x4(mint* p0,mint* p1,vec x)noexcept{
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),_mm256_castsi256_si128(x));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),_mm256_extracti128_si256(x,1));
}
EEZ_NTT998_ALWAYS_INLINE void transpose_8x4_to_4x8(vec v0,vec v1,vec v2,vec v3,vec& x0,vec& x1,vec& x2,vec& x3)noexcept{
    const vec t0=_mm256_unpacklo_epi32(v0,v1),t1=_mm256_unpackhi_epi32(v0,v1);
    const vec t2=_mm256_unpacklo_epi32(v2,v3),t3=_mm256_unpackhi_epi32(v2,v3);
    const vec perm=_mm256_setr_epi32(0,4,1,5,2,6,3,7);
    x0=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,t2),perm);
    x1=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,t2),perm);
    x2=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,t3),perm);
    x3=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,t3),perm);
}
EEZ_NTT998_ALWAYS_INLINE void transpose_4x8_to_8x4(vec x0,vec x1,vec x2,vec x3,vec& v0,vec& v1,vec& v2,vec& v3)noexcept{
    const vec perm=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    const vec q0=_mm256_permutevar8x32_epi32(x0,perm),q1=_mm256_permutevar8x32_epi32(x1,perm);
    const vec q2=_mm256_permutevar8x32_epi32(x2,perm),q3=_mm256_permutevar8x32_epi32(x3,perm);
    const vec t0=_mm256_unpacklo_epi64(q0,q1),t2=_mm256_unpackhi_epi64(q0,q1);
    const vec t1=_mm256_unpacklo_epi64(q2,q3),t3=_mm256_unpackhi_epi64(q2,q3);
    v0=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(2,0,2,0)));
    v1=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(3,1,3,1)));
    v2=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(2,0,2,0)));
    v3=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(3,1,3,1)));
}
#endif

EEZ_NTT998_ALWAYS_INLINE void forward_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3)noexcept{
    const word x0=raw(b[i]),x1=mul(raw(b[stride+i]),r1),x2=mul(raw(b[2*stride+i]),r2),x3=mul(raw(b[3*stride+i]),r3);
    const word s02=add(x0,x2),d02=sub(x0,x2),s13=add(x1,x3),t=mul(sub(x1,x3),twiddles.root[2]);
    b[i]=from_raw(add(s02,s13));b[stride+i]=from_raw(sub(s02,s13));
    b[2*stride+i]=from_raw(add(d02,t));b[3*stride+i]=from_raw(sub(d02,t));
}
EEZ_NTT998_ALWAYS_INLINE void inverse_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3)noexcept{
    const word x0=raw(b[i]),x1=raw(b[stride+i]),x2=raw(b[2*stride+i]),x3=raw(b[3*stride+i]);
    const word s01=add(x0,x1),d01=sub(x0,x1),s23=add(x2,x3),t=mul(sub(x2,x3),twiddles.iroot[2]);
    b[i]=from_raw(add(s01,s23));b[stride+i]=from_raw(mul(add(d01,t),r1));
    b[2*stride+i]=from_raw(mul(sub(s01,s23),r2));b[3*stride+i]=from_raw(mul(sub(d01,t),r3));
}

inline void forward_radix4_scalar(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize stride)noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]),x1=raw(b[stride+i]),x2=raw(b[2*stride+i]),x3=raw(b[3*stride+i]);
            const word s02=add(x0,x2),d02=sub(x0,x2),s13=add(x1,x3),t=mul(sub(x1,x3),twiddles.root[2]);
            b[i]=from_raw(add(s02,s13));b[stride+i]=from_raw(sub(s02,s13));
            b[2*stride+i]=from_raw(add(d02,t));b[3*stride+i]=from_raw(sub(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)forward_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}
inline void inverse_radix4_scalar(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize stride)noexcept{
    {
        mint* const b=a;
        for(usize i=0;i<stride;++i){
            const word x0=raw(b[i]),x1=raw(b[stride+i]),x2=raw(b[2*stride+i]),x3=raw(b[3*stride+i]);
            const word s01=add(x0,x1),d01=sub(x0,x1),s23=add(x2,x3),t=mul(sub(x2,x3),twiddles.iroot[2]);
            b[i]=from_raw(add(s01,s23));b[stride+i]=from_raw(add(d01,t));
            b[2*stride+i]=from_raw(sub(s01,s23));b[3*stride+i]=from_raw(sub(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    for(usize s=1;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        mint* const b=a+s*4*stride;
        for(usize i=0;i<stride;++i)inverse_butterfly(b,stride,i,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}

#if EEZ_NTT998_USE_AVX2
EEZ_NTT998_ALWAYS_INLINE void forward_radix4_large_block(mint* EEZ_NTT998_RESTRICT b,usize stride,vec imag,word r1,word r2,word r3)noexcept{
    imag=canonicalize8(imag);
    const vec w1=broadcast(canonicalize(r1)),w2=broadcast(canonicalize(r2)),w3=broadcast(canonicalize(r3));
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i),x1=mul8_fixed(load8(b+stride+i),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))),x2=mul8_fixed(load8(b+2*stride+i),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))),x3=mul8_fixed(load8(b+3*stride+i),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv)));
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8_fixed(lazy_sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));
        store8(b+i,add8(s02,s13));store8(b+stride+i,sub8(s02,s13));
        store8(b+2*stride+i,add8(d02,t));store8(b+3*stride+i,sub8(d02,t));
    }
}
EEZ_NTT998_ALWAYS_INLINE void inverse_radix4_large_block(mint* EEZ_NTT998_RESTRICT b,usize stride,vec iimag,word r1,word r2,word r3)noexcept{
    iimag=canonicalize8(iimag);
    const vec w1=broadcast(canonicalize(r1)),w2=broadcast(canonicalize(r2)),w3=broadcast(canonicalize(r3));
    for(usize i=0;i<stride;i+=8){
        const vec x0=load8(b+i),x1=load8(b+stride+i),x2=load8(b+2*stride+i),x3=load8(b+3*stride+i);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8_fixed(lazy_sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
        store8(b+i,add8(s01,s23));store8(b+stride+i,mul8_fixed(lazy_add8(d01,t),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))));
        store8(b+2*stride+i,mul8_fixed(lazy_sub8(s01,s23),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))));store8(b+3*stride+i,mul8_fixed(lazy_sub8(d01,t),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv))));
    }
}
inline void forward_radix4_large(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize stride)noexcept{
    const vec imag=broadcast(canonicalize(twiddles.root[2]));
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i),x1=load8(b+stride+i),x2=load8(b+2*stride+i),x3=load8(b+3*stride+i);
            const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8_fixed(lazy_sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));
            store8(b+i,add8(s02,s13));store8(b+stride+i,sub8(s02,s13));
            store8(b+2*stride+i,add8(d02,t));store8(b+3*stride+i,sub8(d02,t));
        }
    }
    if(blocks==1)return;
    word rot=forward_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1)),w2=mul8(w1,w1),w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);_mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)forward_radix4_large_block(a+(s+lane)*4*stride,stride,imag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        forward_radix4_large_block(a+s*4*stride,stride,imag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}
inline void inverse_radix4_large(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize stride)noexcept{
    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));
    {
        mint* const b=a;
        for(usize i=0;i<stride;i+=8){
            const vec x0=load8(b+i),x1=load8(b+stride+i),x2=load8(b+2*stride+i),x3=load8(b+3*stride+i);
            const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8_fixed(lazy_sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
            store8(b+i,add8(s01,s23));store8(b+stride+i,add8(d01,t));
            store8(b+2*stride+i,sub8(s01,s23));store8(b+3*stride+i,sub8(d01,t));
        }
    }
    if(blocks==1)return;
    word rot=inverse_rate3(0);
    usize s=1;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8],r2[8],r3[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1)),w2=mul8(w1,w1),w3=mul8(w2,w1);
        _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);_mm256_store_si256(reinterpret_cast<vec*>(r3),w3);
        for(unsigned lane=0;lane<8;++lane)inverse_radix4_large_block(a+(s+lane)*4*stride,stride,iimag,r1[lane],r2[lane],r3[lane]);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        inverse_radix4_large_block(a+s*4*stride,stride,iimag,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}
inline void forward_radix4_p4(mint* EEZ_NTT998_RESTRICT a,usize blocks)noexcept{
    if(blocks<2){forward_radix4_scalar(a,blocks,4);return;}
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11),w2=mul8(w1,w1),w3=mul8(w2,w1);
        mint* const b0=a+s*16;mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1),x1=mul8_fixed(load2x4(b0+4,b1+4),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))),x2=mul8_fixed(load2x4(b0+8,b1+8),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))),x3=mul8_fixed(load2x4(b0+12,b1+12),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv)));
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8_fixed(sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s02,s13));store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));store2x4(b0+12,b1+12,sub8(d02,t));
        if(s+2<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}
inline void inverse_radix4_p4(mint* EEZ_NTT998_RESTRICT a,usize blocks)noexcept{
    if(blocks<2){inverse_radix4_scalar(a,blocks,4);return;}
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    for(usize s=0;s<blocks;s+=2){
        const word r10=rot;
        rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
        const word r11=rot;
        const vec w1=pack_four(r10,r11),w2=mul8(w1,w1),w3=mul8(w2,w1);
        mint* const b0=a+s*16;mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1),x1=load2x4(b0+4,b1+4),x2=load2x4(b0+8,b1+8),x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8_fixed(sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s01,s23));store2x4(b0+4,b1+4,mul8_fixed(add8(d01,t),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))));
        store2x4(b0+8,b1+8,mul8_fixed(sub8(s01,s23),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))));store2x4(b0+12,b1+12,mul8_fixed(sub8(d01,t),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv))));
        if(s+2<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+1))));
    }
}
inline void forward_radix4_p1(mint* EEZ_NTT998_RESTRICT a,usize blocks)noexcept{
    const vec imag=broadcast(twiddles.root[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1)),w2=mul8(w1,w1),w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);x2=mul8(x2,w2);x3=mul8(x3,w3);
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8(sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        forward_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));
    }
}
inline void inverse_radix4_p1(mint* EEZ_NTT998_RESTRICT a,usize blocks)noexcept{
    const vec iimag=broadcast(twiddles.iroot[2]);
    word rot=montgomery_one;
    usize s=0;
    for(;s+8<=blocks;s+=8){
        alignas(32) word r1[8];
        for(unsigned lane=0;lane<8;++lane){
            r1[lane]=rot;
            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));
        }
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1)),w2=mul8(w1,w1),w3=mul8(w2,w1);
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8(sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(add8(d01,t),w1),mul8(sub8(s01,s23),w2),mul8(sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));
    }
}
#endif

inline void forward_radix2_first(mint* EEZ_NTT998_RESTRICT a,usize n)noexcept{
    const usize half=n>>1;
    usize i=0;
#if EEZ_NTT998_USE_AVX2
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i),y=load8(a+half+i);
        store8(a+i,add8(x,y));store8(a+half+i,sub8(x,y));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]),y=raw(a[half+i]);
        a[i]=from_raw(add(x,y));a[half+i]=from_raw(sub(x,y));
    }
}
inline void forward_radix4_stage(mint* EEZ_NTT998_RESTRICT a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
#if EEZ_NTT998_USE_AVX2
    if(stride>=8)forward_radix4_large(a,blocks,stride);
    else if(stride==4)forward_radix4_p4(a,blocks);
    else if(stride==1)forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
#else
    forward_radix4_scalar(a,blocks,stride);
#endif
}
inline void inverse_radix4_stage(mint* EEZ_NTT998_RESTRICT a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
#if EEZ_NTT998_USE_AVX2
    if(stride>=8)inverse_radix4_large(a,blocks,stride);
    else if(stride==4)inverse_radix4_p4(a,blocks);
    else if(stride==1)inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
#else
    inverse_radix4_scalar(a,blocks,stride);
#endif
}
inline void final_radix2_scale(mint* EEZ_NTT998_RESTRICT a,usize n,word scale_mont)noexcept{
    const usize half=n>>1;
    usize i=0;
#if EEZ_NTT998_USE_AVX2
    const vec scale=broadcast(scale_mont);
    for(;i+8<=half;i+=8){
        const vec x=load8(a+i),y=load8(a+half+i);
        store8(a+i,mul8(add8(x,y),scale));store8(a+half+i,mul8(sub8(x,y),scale));
    }
#endif
    for(;i<half;++i){
        const word x=raw(a[i]),y=raw(a[half+i]);
        a[i]=from_raw(mul(add(x,y),scale_mont));a[half+i]=from_raw(mul(sub(x,y),scale_mont));
    }
}
inline void final_radix4_scale(mint* EEZ_NTT998_RESTRICT a,usize n,word scale_mont)noexcept{
    const usize stride=n>>2;
    usize i=0;
#if EEZ_NTT998_USE_AVX2
    const vec iimag=broadcast(twiddles.iroot[2]),scale=broadcast(scale_mont);
    for(;i+8<=stride;i+=8){
        const vec x0=load8(a+i),x1=load8(a+stride+i),x2=load8(a+2*stride+i),x3=load8(a+3*stride+i);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8_fixed(sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
        store8(a+i,mul8(add8(s01,s23),scale));store8(a+stride+i,mul8(add8(d01,t),scale));
        store8(a+2*stride+i,mul8(sub8(s01,s23),scale));store8(a+3*stride+i,mul8(sub8(d01,t),scale));
    }
#endif
    for(;i<stride;++i){
        const word x0=raw(a[i]),x1=raw(a[stride+i]),x2=raw(a[2*stride+i]),x3=raw(a[3*stride+i]);
        const word s01=add(x0,x1),d01=sub(x0,x1),s23=add(x2,x3),t=mul(sub(x2,x3),twiddles.iroot[2]);
        a[i]=from_raw(mul(add(s01,s23),scale_mont));a[stride+i]=from_raw(mul(add(d01,t),scale_mont));
        a[2*stride+i]=from_raw(mul(sub(s01,s23),scale_mont));a[3*stride+i]=from_raw(mul(sub(d01,t),scale_mont));
    }
}
inline void forward_dif(mint* EEZ_NTT998_RESTRICT a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){forward_radix2_first(a,n);stage=1;}
    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);
}
inline void inverse_dit(mint* EEZ_NTT998_RESTRICT a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

#if EEZ_NTT998_USE_AVX2
EEZ_NTT998_ALWAYS_INLINE vec load8_aligned(const mint* p)noexcept{return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const void*>(p)));}
EEZ_NTT998_ALWAYS_INLINE void store8_aligned(mint* p,vec x)noexcept{_mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);}
EEZ_NTT998_ALWAYS_INLINE vec shrink4_to_2(vec x)noexcept{return _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));}
EEZ_NTT998_ALWAYS_INLINE word shrink4_to_2_scalar(word x)noexcept{return x>=mod2?x-mod2:x;}
inline void copy_shrink4_to_2(const mint* EEZ_NTT998_RESTRICT src,mint* EEZ_NTT998_RESTRICT dst,usize n)noexcept{
    usize i=0;
    for(;i+8<=n;i+=8)store8(dst+i,shrink4_to_2(load8_aligned(src+i)));
    for(;i<n;++i)dst[i]=from_raw(shrink4_to_2_scalar(raw(src[i])));
}
inline void shrink4_to_2_inplace(mint* EEZ_NTT998_RESTRICT a,usize n)noexcept{
    usize i=0;
    for(;i+8<=n;i+=8)store8_aligned(a+i,shrink4_to_2(load8_aligned(a+i)));
    for(;i<n;++i)a[i]=from_raw(shrink4_to_2_scalar(raw(a[i])));
}


template<bool trivial_twiddle>
inline void forward_radix4_block_lazy(mint* b,usize stride,word r1)noexcept{
    const word imag=canonicalize(twiddles.root[2]);
    const vec vimag=broadcast(imag),vimag_ninv=broadcast(imag*montgomery_ninv);
    const vec vr1=broadcast(r1),vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2),vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3),vr3_ninv=broadcast(r3*montgomery_ninv);
    for(usize i=0;i<stride;i+=8){
        vec x0=shrink4_to_2(load8_aligned(b+i));
        vec x1=load8_aligned(b+stride+i),x2=load8_aligned(b+2*stride+i),x3=load8_aligned(b+3*stride+i);
        if constexpr(!trivial_twiddle){
            x1=mul8_fixed(x1,vr1,vr1_ninv);x2=mul8_fixed(x2,vr2,vr2_ninv);x3=mul8_fixed(x3,vr3,vr3_ninv);
        }else{x1=shrink4_to_2(x1);x2=shrink4_to_2(x2);x3=shrink4_to_2(x3);}
        vec s02=lazy_add8(x0,x2),d02=lazy_sub8(x0,x2),s13=lazy_add8(x1,x3);
        const vec t=mul8_fixed(lazy_sub8(x1,x3),vimag,vimag_ninv);
        s02=shrink4_to_2(s02);d02=shrink4_to_2(d02);s13=shrink4_to_2(s13);
        store8_aligned(b+i,lazy_add8(s02,s13));store8_aligned(b+stride+i,lazy_sub8(s02,s13));
        store8_aligned(b+2*stride+i,lazy_add8(d02,t));store8_aligned(b+3*stride+i,lazy_sub8(d02,t));
    }
}
template<bool trivial_twiddle>
EEZ_NTT998_ALWAYS_INLINE void forward_radix4_block_pair_lazy(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize stride,word r1)noexcept{
    forward_radix4_block_lazy<trivial_twiddle>(a,stride,r1);
    forward_radix4_block_lazy<trivial_twiddle>(b,stride,r1);
}
template<bool trivial_twiddle,bool apply_scale>
inline void inverse_radix4_block_lazy(mint* b,usize stride,word r1,word scale)noexcept{
    const word iimag=canonicalize(twiddles.iroot[2]);
    const vec viimag=broadcast(iimag),viimag_ninv=broadcast(iimag*montgomery_ninv);
    const vec vr1=broadcast(r1),vr1_ninv=broadcast(r1*montgomery_ninv);
    const word r2=canonicalize(mul(r1,r1));
    const vec vr2=broadcast(r2),vr2_ninv=broadcast(r2*montgomery_ninv);
    const word r3=canonicalize(mul(r2,r1));
    const vec vr3=broadcast(r3),vr3_ninv=broadcast(r3*montgomery_ninv);
    const word scale_canonical=canonicalize(scale);
    for(usize i=0;i<stride;i+=8){
        const vec x0=shrink4_to_2(load8_aligned(b+i)),x1=shrink4_to_2(load8_aligned(b+stride+i));
        const vec x2=shrink4_to_2(load8_aligned(b+2*stride+i)),x3=shrink4_to_2(load8_aligned(b+3*stride+i));
        vec s01=lazy_add8(x0,x1),d01=lazy_sub8(x0,x1),s23=lazy_add8(x2,x3);
        const vec t=mul8_fixed(lazy_sub8(x2,x3),viimag,viimag_ninv);
        s01=shrink4_to_2(s01);d01=shrink4_to_2(d01);s23=shrink4_to_2(s23);
        vec y0=lazy_add8(s01,s23),y1=lazy_add8(d01,t),y2=lazy_sub8(s01,s23),y3=lazy_sub8(d01,t);
        if constexpr(apply_scale){
            const word s0=scale_canonical;
            const word s1=trivial_twiddle?s0:canonicalize(mul(s0,r1));
            const word s2=trivial_twiddle?s0:canonicalize(mul(s0,r2));
            const word s3=trivial_twiddle?s0:canonicalize(mul(s0,r3));
            y0=mul8_fixed(y0,broadcast(s0),broadcast(s0*montgomery_ninv));
            y1=mul8_fixed(y1,broadcast(s1),broadcast(s1*montgomery_ninv));
            y2=mul8_fixed(y2,broadcast(s2),broadcast(s2*montgomery_ninv));
            y3=mul8_fixed(y3,broadcast(s3),broadcast(s3*montgomery_ninv));
        }else if constexpr(!trivial_twiddle){
            y1=mul8_fixed(y1,vr1,vr1_ninv);y2=mul8_fixed(y2,vr2,vr2_ninv);y3=mul8_fixed(y3,vr3,vr3_ninv);
        }
        store8_aligned(b+i,y0);store8_aligned(b+stride+i,y1);
        store8_aligned(b+2*stride+i,y2);store8_aligned(b+3*stride+i,y3);
    }
}

inline unsigned adaptive_leaf_log(usize n)noexcept{
    const unsigned h=static_cast<unsigned>(std::countr_zero(n));
    return (h&1u)?3u:4u;
}
EEZ_NTT998_ALWAYS_INLINE void forward_cache_node(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    const usize stride=block_size>>2;
    if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}
inline void forward_cache_block(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    forward_cache_node(base,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const child_base=base+child*child_size;
        const usize child_block=block*4+child;
        forward_cache_node(child_base,child_size,layer+1,child_block,blocks_at_layer*4,rotation);
        const usize grandchild_size=child_size>>2;
        for(usize grandchild=0;grandchild<4;++grandchild)
            forward_cache_node(child_base+grandchild*grandchild_size,grandchild_size,layer+2,child_block*4+grandchild,blocks_at_layer*16,rotation);
    }
}
inline void forward_cache_dfs(mint* EEZ_NTT998_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    if(block_size==leaf_size*64){forward_cache_block(base,block_size,layer,block,blocks_at_layer,rotation);return;}
    const usize stride=block_size>>2;
    if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);
    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)
        forward_cache_dfs(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}
EEZ_NTT998_ALWAYS_INLINE void forward_cache_pair_node(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    const usize stride=block_size>>2;
    if(block==0)forward_radix4_block_pair_lazy<true>(a,b,stride,montgomery_one);
    else forward_radix4_block_pair_lazy<false>(a,b,stride,rotation[layer]);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));
}
inline void forward_cache_pair_block(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    forward_cache_pair_node(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    for(usize child=0;child<4;++child){
        mint* const child_a=a+child*child_size;
        mint* const child_b=b+child*child_size;
        const usize child_block=block*4+child;
        forward_cache_pair_node(child_a,child_b,child_size,layer+1,child_block,blocks_at_layer*4,rotation);
        const usize grandchild_size=child_size>>2;
        for(usize grandchild=0;grandchild<4;++grandchild)
            forward_cache_pair_node(child_a+grandchild*grandchild_size,child_b+grandchild*grandchild_size,grandchild_size,layer+2,child_block*4+grandchild,blocks_at_layer*16,rotation);
    }
}
inline void forward_cache_pair_dfs(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)noexcept{
    if(block_size==leaf_size*64){forward_cache_pair_block(a,b,block_size,layer,block,blocks_at_layer,rotation);return;}
    forward_cache_pair_node(a,b,block_size,layer,block,blocks_at_layer,rotation);
    const usize child_size=block_size>>2;
    if(child_size==leaf_size)return;
    for(usize child=0;child<4;++child)
        forward_cache_pair_dfs(a+child*child_size,b+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);
}
template<bool apply_scale>
EEZ_NTT998_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation)noexcept{
    const usize stride=block_size>>2;
    if(block==0)inverse_radix4_block_lazy<true,apply_scale>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,apply_scale>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}
template<bool scale_leaf>
inline void inverse_cache_block(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation)noexcept{
    const usize child_size=block_size>>2,grandchild_size=child_size>>2;
    for(usize child=0;child<4;++child){
        mint* const child_base=base+child*child_size;
        const usize child_block=block*4+child;
        for(usize grandchild=0;grandchild<4;++grandchild)
            inverse_cache_node<scale_leaf>(child_base+grandchild*grandchild_size,grandchild_size,layer+2,child_block*4+grandchild,blocks_at_layer*16,scale,rotation);
        inverse_cache_node<false>(child_base,child_size,layer+1,child_block,blocks_at_layer*4,scale,rotation);
    }
    inverse_cache_node<false>(base,block_size,layer,block,blocks_at_layer,scale,rotation);
}
template<bool scale_leaf>
inline void inverse_cache_dfs(mint* EEZ_NTT998_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>& rotation)noexcept{
    if(block_size==leaf_size*64){inverse_cache_block<scale_leaf>(base,block_size,layer,block,blocks_at_layer,scale,rotation);return;}
    const usize child_size=block_size>>2;
    if(child_size!=leaf_size)for(usize child=0;child<4;++child)
        inverse_cache_dfs<scale_leaf>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,scale,rotation);
    const usize stride=block_size>>2;
    if constexpr(scale_leaf){
        if(child_size==leaf_size){
            if(block==0)inverse_radix4_block_lazy<true,true>(base,stride,montgomery_one,scale);
            else inverse_radix4_block_lazy<false,true>(base,stride,rotation[layer],scale);
        }else if(block==0)inverse_radix4_block_lazy<true,false>(base,stride,montgomery_one,scale);
        else inverse_radix4_block_lazy<false,false>(base,stride,rotation[layer],scale);
    }else if(block==0)inverse_radix4_block_lazy<true,false>(base,stride,montgomery_one,scale);
    else inverse_radix4_block_lazy<false,false>(base,stride,rotation[layer],scale);
    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));
}

inline void forward_adaptive(mint* a,usize n,unsigned leaf_log)noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size)return;
    std::array<word,max_log/2+1> rotation{};rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);
        forward_cache_dfs(a,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_dfs(a+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_dfs(a,n,leaf_size,0,0,1,rotation);
}
inline void forward_adaptive_pair(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize n,unsigned leaf_log)noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size)return;
    std::array<word,max_log/2+1> rotation{};rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        forward_radix2_first(a,n);forward_radix2_first(b,n);
        forward_cache_pair_dfs(a,b,n>>1,leaf_size,0,0,2,rotation);
        forward_cache_pair_dfs(a+(n>>1),b+(n>>1),n>>1,leaf_size,0,1,2,rotation);
        return;
    }
    forward_cache_pair_dfs(a,b,n,leaf_size,0,0,1,rotation);
}
inline void inverse_adaptive(mint* a,usize n,unsigned leaf_log)noexcept{
    const usize leaf_size=usize(1)<<leaf_log;
    if(n==leaf_size)return;
    const word scale=mint::raw(static_cast<word>(n>>leaf_log)).inv().a;
    std::array<word,max_log/2+1> rotation{};rotation.fill(canonicalize(montgomery_one));
    if((std::countr_zero(n)-leaf_log)&1u){
        inverse_cache_dfs<true>(a,n>>1,leaf_size,0,0,2,scale,rotation);
        inverse_cache_dfs<true>(a+(n>>1),n>>1,leaf_size,0,1,2,scale,rotation);
        const usize half=n>>1;
        for(usize i=0;i<half;i+=8){
            const vec x=shrink4_to_2(load8_aligned(a+i)),y=shrink4_to_2(load8_aligned(a+half+i));
            store8_aligned(a+i,lazy_add8(x,y));store8_aligned(a+half+i,lazy_sub8(x,y));
        }
        return;
    }
    inverse_cache_dfs<true>(a,n,leaf_size,0,0,1,scale,rotation);
}

EEZ_NTT998_ALWAYS_INLINE __m128i reduce_four_accumulators(vec x)noexcept{
    const vec ninv=broadcast(montgomery_ninv),prime=broadcast(mod);
    const vec q=_mm256_mul_epu32(x,ninv);
    const vec sum=_mm256_add_epi64(x,_mm256_mul_epu32(q,prime));
    const vec high=_mm256_bsrli_epi128(sum,4);
    const vec packed=_mm256_permutevar8x32_epi32(high,_mm256_setr_epi32(0,2,4,6,0,0,0,0));
    return _mm256_castsi256_si128(packed);
}
EEZ_NTT998_ALWAYS_INLINE vec reduce_eight_accumulators(vec even,vec odd)noexcept{
    const vec ninv=broadcast(montgomery_ninv),prime=broadcast(mod);
    const vec q_even=_mm256_mul_epu32(even,ninv),q_odd=_mm256_mul_epu32(odd,ninv);
    const vec reduced_even=_mm256_add_epi64(even,_mm256_mul_epu32(q_even,prime));
    const vec reduced_odd=_mm256_add_epi64(odd,_mm256_mul_epu32(q_odd,prime));
    return shrink4_to_2(_mm256_or_si256(_mm256_srli_epi64(reduced_even,32),reduced_odd));
}
EEZ_NTT998_ALWAYS_INLINE void leaf_product8x4(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize first_block,const std::array<word,4>& modulus)noexcept{
    alignas(64) word lhs[4][16];
    alignas(64) vec even[4]{},odd[4]{};
    for(unsigned k=0;k<4;++k){
        const usize offset=(first_block+k)*8;
        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+offset)));
        const vec y=canonicalize8(shrink4_to_2(load8_aligned(b+offset)));
        const word w=canonicalize(modulus[k]);
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));
        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);
        store8_aligned(b+offset,y);
    }
    for(unsigned i=0;i<8;++i)for(unsigned k=0;k<4;++k){
        const usize offset=(first_block+k)*8;
        const vec y=broadcast(raw(b[offset+i]));
        const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));
        even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));
        odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
    }
    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));
}
EEZ_NTT998_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint* EEZ_NTT998_RESTRICT a,const mint* EEZ_NTT998_RESTRICT b,usize first_block,const std::array<word,2>& modulus)noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    alignas(64) word lhs[6][16],rhs[6][8];
    alignas(64) vec even[6]{},odd[6]{};
    word w[2];
    for(unsigned k=0;k<2;++k){
        const usize offset=(first_block+k)*16;
        const vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+offset))),split);
        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+offset+8))),split);
        const vec bx0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+offset))),split);
        const vec bx1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+offset+8))),split);
        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20),ao=_mm256_permute2x128_si256(ax0,ax1,0x31);
        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20),bo=_mm256_permute2x128_si256(bx0,bx1,0x31);
        const vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))},ry[3]{be,bo,canonicalize8(add8(be,bo))};
        w[k]=canonicalize(modulus[k]);
        const vec vw=broadcast(w[k]),vwninv=broadcast(w[k]*montgomery_ninv);
        for(unsigned j=0;j<3;++j){
            const unsigned p=k*3+j;
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vwninv));
            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);
            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);
        }
    }
    for(unsigned i=0;i<8;++i)for(unsigned p=0;p<6;++p){
        const vec y=broadcast(rhs[p][i]),x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]+8-i));
        even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));
        odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));
    }
    for(unsigned k=0;k<2;++k){
        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);
        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);
        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);
        const vec ce=add8(p0,yp1),co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co),hi=_mm256_unpackhi_epi32(ce,co);
        const usize offset=(first_block+k)*16;
        store8_aligned(a+offset,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+offset+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}
EEZ_NTT998_ALWAYS_INLINE word twice(word x)noexcept{return x+x;}
EEZ_NTT998_ALWAYS_INLINE vec pack4_u32(word x0,word x1,word x2,word x3)noexcept{
    const __m128i x=_mm_setr_epi32(static_cast<int>(x0),static_cast<int>(x1),static_cast<int>(x2),static_cast<int>(x3));
    return _mm256_cvtepu32_epi64(x);
}
EEZ_NTT998_ALWAYS_INLINE vec mul4_u32(word a0,word b0,word a1,word b1,word a2,word b2,word a3,word b3)noexcept{
    return _mm256_mul_epu32(pack4_u32(a0,a1,a2,a3),pack4_u32(b0,b1,b2,b3));
}
EEZ_NTT998_ALWAYS_INLINE u64 hsum4_u64(vec x)noexcept{
    __m128i s=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));
    s=_mm_add_epi64(s,_mm_srli_si128(s,8));
    return static_cast<u64>(_mm_cvtsi128_si64(s));
}
EEZ_NTT998_ALWAYS_INLINE vec square8_packed(vec vx,word w)noexcept{
    alignas(32) word x[8],xw[8];
    vx=canonicalize8(shrink4_to_2(vx));w=canonicalize(w);
    _mm256_store_si256(reinterpret_cast<vec*>(x),vx);
    _mm256_store_si256(reinterpret_cast<vec*>(xw),mul8_fixed(vx,broadcast(w),broadcast(w*montgomery_ninv)));
    u64 a0=hsum4_u64(mul4_u32(x[0],x[0],twice(xw[1]),x[7],twice(xw[2]),x[6],twice(xw[3]),x[5]));
    const u64 a1=hsum4_u64(mul4_u32(twice(x[0]),x[1],twice(xw[2]),x[7],twice(xw[3]),x[6],twice(xw[4]),x[5]));
    u64 a2=hsum4_u64(mul4_u32(twice(x[0]),x[2],x[1],x[1],twice(xw[3]),x[7],twice(xw[4]),x[6]));
    const u64 a3=hsum4_u64(mul4_u32(twice(x[0]),x[3],twice(x[1]),x[2],twice(xw[4]),x[7],twice(xw[5]),x[6]));
    u64 a4=hsum4_u64(mul4_u32(twice(x[0]),x[4],twice(x[1]),x[3],x[2],x[2],twice(xw[5]),x[7]));
    const u64 a5=hsum4_u64(mul4_u32(twice(x[0]),x[5],twice(x[1]),x[4],twice(x[2]),x[3],twice(xw[6]),x[7]));
    u64 a6=hsum4_u64(mul4_u32(twice(x[0]),x[6],twice(x[1]),x[5],twice(x[2]),x[4],x[3],x[3]));
    const u64 a7=hsum4_u64(mul4_u32(twice(x[0]),x[7],twice(x[1]),x[6],twice(x[2]),x[5],twice(x[3]),x[4]));
    const vec extra=mul4_u32(xw[4],x[4],xw[5],x[5],xw[6],x[6],xw[7],x[7]);
    alignas(32) u64 e[4];_mm256_store_si256(reinterpret_cast<vec*>(e),extra);
    a0+=e[0];a2+=e[1];a4+=e[2];a6+=e[3];
    const vec lo=_mm256_setr_epi64x(static_cast<long long>(a0),static_cast<long long>(a1),static_cast<long long>(a2),static_cast<long long>(a3));
    const vec hi=_mm256_setr_epi64x(static_cast<long long>(a4),static_cast<long long>(a5),static_cast<long long>(a6),static_cast<long long>(a7));
    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),reduce_four_accumulators(lo)));
}
EEZ_NTT998_ALWAYS_INLINE void leaf_square8x4(mint* EEZ_NTT998_RESTRICT a,usize first_block,const std::array<word,4>& modulus)noexcept{
    for(unsigned k=0;k<4;++k){
        const usize offset=(first_block+k)*8;
        store8_aligned(a+offset,square8_packed(load8_aligned(a+offset),modulus[k]));
    }
}
EEZ_NTT998_ALWAYS_INLINE void leaf_square16x2_karatsuba(mint* EEZ_NTT998_RESTRICT a,usize first_block,const std::array<word,2>& modulus)noexcept{
    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    for(unsigned k=0;k<2;++k){
        const usize offset=(first_block+k)*16;
        const vec x0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+offset))),split);
        const vec x1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+offset+8))),split);
        const vec xe=_mm256_permute2x128_si256(x0,x1,0x20),xo=_mm256_permute2x128_si256(x0,x1,0x31);
        const vec xs=canonicalize8(add8(xe,xo));
        const word w=canonicalize(modulus[k]);
        const vec p0=square8_packed(xe,w),p1=square8_packed(xo,w),p2=square8_packed(xs,w);
        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));
        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w)),0);
        const vec ce=add8(p0,yp1),co=sub8(sub8(p2,p0),p1);
        const vec lo=_mm256_unpacklo_epi32(ce,co),hi=_mm256_unpackhi_epi32(ce,co);
        store8_aligned(a+offset,_mm256_permute2x128_si256(lo,hi,0x20));
        store8_aligned(a+offset+8,_mm256_permute2x128_si256(lo,hi,0x31));
    }
}
template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_products(mint* a,mint* b,usize n)noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_product8x4(a,b,s,modulus);
        else leaf_product16x2_karatsuba(a,b,s,modulus);
    }
}
template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_squares(mint* a,usize n)noexcept{
    const usize blocks=n/leaf_size;
    word w=montgomery_one;
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));
        }
        if constexpr(leaf_size==8)leaf_square8x4(a,s,modulus);
        else leaf_square16x2_karatsuba(a,s,modulus);
    }
}
inline void convolution_adaptive_inplace(mint* a,mint* b,usize n)noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3)leaf_products<8,4>(a,b,n);else leaf_products<16,2>(a,b,n);
    inverse_adaptive(a,n,leaf_log);
}
inline void square_adaptive_inplace(mint* a,usize n)noexcept{
    const unsigned leaf_log=adaptive_leaf_log(n);
    forward_adaptive(a,n,leaf_log);
    if(leaf_log==3)leaf_squares<8,4>(a,n);else leaf_squares<16,2>(a,n);
    inverse_adaptive(a,n,leaf_log);
}
#endif

inline void pointwise_multiply(mint* EEZ_NTT998_RESTRICT a,const mint* EEZ_NTT998_RESTRICT b,usize n)noexcept{
    usize i=0;
#if EEZ_NTT998_USE_AVX2
    for(;i+8<=n;i+=8)store8(a+i,mul8(load8(a+i),load8(b+i)));
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(b[i])));
}
inline void pointwise_square(mint* EEZ_NTT998_RESTRICT a,usize n)noexcept{
    usize i=0;
#if EEZ_NTT998_USE_AVX2
    for(;i+8<=n;i+=8){const vec x=load8(a+i);store8(a+i,mul8(x,x));}
#endif
    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(a[i])));
}
}

inline void forward(std::span<mint> a)noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::forward_dif(a.data(),a.size());
}
inline void inverse(std::span<mint> a)noexcept{
    if(a.size()<=1)return;
    assert(valid_ntt_size(a.size()));
    detail::inverse_dit(a.data(),a.size());
}

namespace detail{
inline std::vector<mint> convolution_naive(std::span<const mint> a,std::span<const mint> b){
    std::vector<mint> result(convolution_size(a.size(),b.size()));
    for(usize i=0;i<a.size();++i)for(usize j=0;j<b.size();++j)result[i+j]+=a[i]*b[j];
    return result;
}
inline std::vector<mint> square_naive(std::span<const mint> a){
    std::vector<mint> result(convolution_size(a.size(),a.size()));
    for(usize i=0;i<a.size();++i){
        result[2*i]+=a[i]*a[i];
        for(usize j=i+1;j<a.size();++j){const mint product=a[i]*a[j];result[i+j]+=product+product;}
    }
    return result;
}
inline usize checked_transform_size(usize n,usize m){
    const usize result=convolution_transform_size(n,m);
    if(n&&m&&!result)throw std::length_error("eez::ntt998: convolution exceeds the 2^25 transform limit");
    return result;
}
inline void require_ntt_size(usize n){
    if(!valid_ntt_size(n))throw std::invalid_argument("eez::ntt998: transform length must be a power of two in [1, 2^23]");
}
}

#if EEZ_NTT998_USE_AVX2
using convolution_buffer=detail::aligned_vector;
inline void convolution_inplace(convolution_buffer& a,convolution_buffer& b){
    if(a.size()!=b.size()||!valid_convolution_transform_size(a.size()))
        throw std::invalid_argument("eez::ntt998::convolution_inplace: buffer sizes must match and be a power of two in [32, 2^25]");
    detail::convolution_adaptive_inplace(a.data(),b.data(),a.size());
    detail::shrink4_to_2_inplace(a.data(),a.size());
}
#endif

inline std::vector<mint> convolution(std::span<const mint> a,std::span<const mint> b){
    if(a.empty()||b.empty())return {};
    if(a.data()==b.data()&&a.size()==b.size())return square(a);
    if(std::min(a.size(),b.size())<=naive_cutoff)return detail::convolution_naive(a,b);
    const usize result_size=convolution_size(a.size(),b.size());
    const usize n=detail::checked_transform_size(a.size(),b.size());
    detail::aligned_vector fa(n),fb(n);
    std::copy(a.begin(),a.end(),fa.begin());std::copy(b.begin(),b.end(),fb.begin());
    detail::convolution_adaptive_inplace(fa.data(),fb.data(),n);
    std::vector<mint> result(result_size);
    detail::copy_shrink4_to_2(fa.data(),result.data(),result_size);
    return result;
}
inline std::vector<mint> square(std::span<const mint> a){
    if(a.empty())return {};
    if(a.size()<=naive_cutoff)return detail::square_naive(a);
    const usize result_size=convolution_size(a.size(),a.size());
    const usize n=detail::checked_transform_size(a.size(),a.size());
    detail::aligned_vector fa(n);
    std::copy(a.begin(),a.end(),fa.begin());
    detail::square_adaptive_inplace(fa.data(),n);
    std::vector<mint> result(result_size);
    detail::copy_shrink4_to_2(fa.data(),result.data(),result_size);
    return result;
}
inline void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint> out,workspace& ws){
    if(a.empty()||b.empty())return;
    const usize result_size=convolution_size(a.size(),b.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt998::convolution_to: output span is too small");
    if(a.data()==b.data()&&a.size()==b.size()){square_to(a,out,ws);return;}
    if(std::min(a.size(),b.size())<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i)for(usize j=0;j<b.size();++j)out[i+j]+=a[i]*b[j];
        return;
    }
    const usize n=detail::checked_transform_size(a.size(),b.size());
    ws.reserve(n);
    std::fill_n(ws.a_.begin(),n,mint{});std::fill_n(ws.b_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());std::copy(b.begin(),b.end(),ws.b_.begin());
    detail::convolution_adaptive_inplace(ws.a_.data(),ws.b_.data(),n);
    detail::copy_shrink4_to_2(ws.a_.data(),out.data(),result_size);
}
inline void square_to(std::span<const mint> a,std::span<mint> out,workspace& ws){
    if(a.empty())return;
    const usize result_size=convolution_size(a.size(),a.size());
    if(out.size()<result_size)throw std::invalid_argument("eez::ntt998::square_to: output span is too small");
    if(a.size()<=naive_cutoff){
        std::fill_n(out.begin(),result_size,mint{});
        for(usize i=0;i<a.size();++i){
            out[2*i]+=a[i]*a[i];
            for(usize j=i+1;j<a.size();++j){const mint product=a[i]*a[j];out[i+j]+=product+product;}
        }
        return;
    }
    const usize n=detail::checked_transform_size(a.size(),a.size());
    ws.reserve(n);std::fill_n(ws.a_.begin(),n,mint{});
    std::copy(a.begin(),a.end(),ws.a_.begin());
    detail::square_adaptive_inplace(ws.a_.data(),n);
    detail::copy_shrink4_to_2(ws.a_.data(),out.data(),result_size);
}
inline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize n){
    detail::require_ntt_size(n);
    if(src.size()>n)throw std::invalid_argument("eez::ntt998::forward_to: source is longer than transform");
    dst.data_.assign(n,mint{});
    std::copy(src.begin(),src.end(),dst.data_.begin());
    detail::forward_dif(dst.data_.data(),n);
}
inline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer& rhs){
    if(lhs.size()!=rhs.size())throw std::invalid_argument("eez::ntt998::pointwise_multiply: transform sizes differ");
    detail::pointwise_multiply(lhs.data_.data(),rhs.data_.data(),lhs.data_.size());
}
inline void pointwise_square(frequency_buffer& a){detail::pointwise_square(a.data_.data(),a.data_.size());}
inline void inverse_to(frequency_buffer& src,std::span<mint> out){
    if(src.data_.empty()){
        if(!out.empty())throw std::invalid_argument("eez::ntt998::inverse_to: empty transform");
        return;
    }
    if(out.size()>src.data_.size())throw std::invalid_argument("eez::ntt998::inverse_to: output is longer than transform");
    detail::inverse_dit(src.data_.data(),src.data_.size());
    std::copy_n(src.data_.begin(),out.size(),out.begin());
}
}

#undef EEZ_NTT998_ALWAYS_INLINE
#undef EEZ_NTT998_RESTRICT
#undef EEZ_NTT998_USE_AVX2
#if defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma clang attribute pop
#elif defined(__GNUC__)&&(defined(__x86_64__)||defined(__i386__))
#pragma GCC pop_options
#endif

#line 11 "y.cpp"
using namespace std;
namespace nt=eez::ntt998;
namespace nd=eez::ntt998::detail;
using mint=nt::mint;
using usize=size_t;
using u32=uint32_t;
using u64=uint64_t;

namespace bm_ntt{
using namespace eez::ntt998::detail;

struct stage_rotations{vector<word,aligned_allocator<word>> r1,r2,r3;};
template<bool inverse>
inline const stage_rotations& cached_rotations(usize blocks){
    static array<stage_rotations,max_log+1> tables;
    auto& t=tables[countr_zero(blocks)];
    if(t.r1.empty()){
        t.r1.resize(blocks);t.r2.resize(blocks);t.r3.resize(blocks);
        word w=montgomery_one;
        for(usize s=0;s<blocks;s++){
            t.r1[s]=canonicalize(w);t.r2[s]=canonicalize(mul(w,w));t.r3[s]=canonicalize(mul(t.r2[s],w));
            if(s+1<blocks)w=mul(w,inverse?inverse_rate3(twiddle_index(u32(s))):forward_rate3(twiddle_index(u32(s))));
        }
    }
    return t;
}
inline void bm_forward_radix4_p4(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    if(blocks<2){forward_radix4_scalar(a,blocks,4);return;}
    const vec imag=broadcast(canonicalize(twiddles.root[2]));
    const auto& table=cached_rotations<false>(total?total:blocks);
    for(usize s=0;s<blocks;s+=2){
        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]),w2=pack_four(table.r2[first+s],table.r2[first+s+1]),w3=pack_four(table.r3[first+s],table.r3[first+s+1]);
        mint* const b0=a+s*16;mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1),x1=mul8_fixed(load2x4(b0+4,b1+4),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))),x2=mul8_fixed(load2x4(b0+8,b1+8),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))),x3=mul8_fixed(load2x4(b0+12,b1+12),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv)));
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8_fixed(lazy_sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s02,s13));store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));store2x4(b0+12,b1+12,sub8(d02,t));
    }
}
inline void bm_inverse_radix4_p4(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    if(blocks<2){inverse_radix4_scalar(a,blocks,4);return;}
    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));
    const auto& table=cached_rotations<true>(total?total:blocks);
    for(usize s=0;s<blocks;s+=2){
        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]),w2=pack_four(table.r2[first+s],table.r2[first+s+1]),w3=pack_four(table.r3[first+s],table.r3[first+s+1]);
        mint* const b0=a+s*16;mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1),x1=load2x4(b0+4,b1+4),x2=load2x4(b0+8,b1+8),x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8_fixed(lazy_sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s01,s23));store2x4(b0+4,b1+4,mul8_fixed(lazy_add8(d01,t),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))));
        store2x4(b0+8,b1+8,mul8_fixed(lazy_sub8(s01,s23),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))));store2x4(b0+12,b1+12,mul8_fixed(lazy_sub8(d01,t),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv))));
    }
}
inline void bm_forward_radix4_p1(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    const vec imag=broadcast(canonicalize(twiddles.root[2]));
    const auto& table=cached_rotations<false>(total?total:blocks);
    word rot=table.r1[first];
    usize s=0;
    for(;s+8<=blocks;s+=8){
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(table.r1.data()+first+s));
        const vec w2=_mm256_load_si256(reinterpret_cast<const vec*>(table.r2.data()+first+s));
        const vec w3=_mm256_load_si256(reinterpret_cast<const vec*>(table.r3.data()+first+s));
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);x2=mul8(x2,w2);x3=mul8(x3,w3);
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8(lazy_sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        forward_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(first+s))));
    }
}
inline void bm_inverse_radix4_p1(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));
    const auto& table=cached_rotations<true>(total?total:blocks);
    word rot=table.r1[first];
    usize s=0;
    for(;s+8<=blocks;s+=8){
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(table.r1.data()+first+s));
        const vec w2=_mm256_load_si256(reinterpret_cast<const vec*>(table.r2.data()+first+s));
        const vec w3=_mm256_load_si256(reinterpret_cast<const vec*>(table.r3.data()+first+s));
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8(lazy_sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(lazy_add8(d01,t),w1),mul8(lazy_sub8(s01,s23),w2),mul8(lazy_sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word rot2=mul(rot,rot),rot3=mul(rot2,rot);
        inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);
        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(first+s))));
    }
}
inline void bm_forward_radix4_stage(mint* __restrict__ a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
    if(stride>=8)forward_radix4_large(a,blocks,stride);
    else if(stride==4)bm_forward_radix4_p4(a,blocks);
    else if(stride==1)bm_forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
}
inline void bm_inverse_radix4_stage(mint* __restrict__ a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
    if(stride>=8)inverse_radix4_large(a,blocks,stride);
    else if(stride==4)bm_inverse_radix4_p4(a,blocks);
    else if(stride==1)bm_inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
}
inline void bm_forward_dif(mint* __restrict__ a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){forward_radix2_first(a,n);stage=1;}
    for(;stage<h;stage+=2)bm_forward_radix4_stage(a,n,stage);
}
inline void bm_inverse_dit(mint* __restrict__ a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)bm_inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)bm_inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

// 4096 coefficients per tile (16 KiB).
inline constexpr usize ntt_tile=4096;
template<bool inv>
inline void stage_range(mint* a,usize stride,usize first,usize count,usize total){
    if(stride==1){
        if constexpr(inv)bm_inverse_radix4_p1(a+first*4,count,first,total);
        else bm_forward_radix4_p1(a+first*4,count,first,total);
    }else if(stride==4){
        if constexpr(inv)bm_inverse_radix4_p4(a+first*16,count,first,total);
        else bm_forward_radix4_p4(a+first*16,count,first,total);
    }else{
        const auto& t=cached_rotations<inv>(total);
        const vec imag=broadcast(inv?twiddles.iroot[2]:twiddles.root[2]);
        for(usize s=first;s<first+count;s++){
            if(s==0){
                if constexpr(inv)inverse_radix4_large(a,1,stride);
                else forward_radix4_large(a,1,stride);
            }else{
                if constexpr(inv)inverse_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);
                else forward_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);
            }
        }
    }
}
inline int tile_stage(usize n,int h){
    int stage=h&1;
    while((n>>stage)>ntt_tile)stage+=2;
    return stage;
}
inline void forward_tiled(mint* a,usize n){
    if(n<=1)return;
    const int h=int(countr_zero(n));
    if(n<=ntt_tile){bm_forward_dif(a,n);return;}
    const int split=tile_stage(n,h);
    int stage=0;
    if(h&1){forward_radix2_first(a,n);stage=1;}
    for(;stage<split;stage+=2)bm_forward_radix4_stage(a,n,stage);
    for(usize b=0;b<(usize(1)<<split);b++)for(int s=split;s<h;s+=2){
        const usize count=usize(1)<<(s-split);
        stage_range<false>(a,n>>(s+2),b*count,count,usize(1)<<s);
    }
}
inline void inverse_prefix_tiled(mint* a,usize n){
    const int h=int(countr_zero(n)),last=(h&1)?1:2;
    if(n<=ntt_tile){
        for(int s=h-2;s>=last;s-=2)bm_inverse_radix4_stage(a,n,s);
        return;
    }
    const int split=tile_stage(n,h);
    for(usize b=0;b<(usize(1)<<split);b++)for(int s=h-2;s>=split;s-=2){
        const usize count=usize(1)<<(s-split);
        stage_range<true>(a,n>>(s+2),b*count,count,usize(1)<<s);
    }
    for(int s=split-2;s>=last;s-=2)bm_inverse_radix4_stage(a,n,s);
}
inline void inverse_tiled(mint* a,usize n){
    if(n<=1)return;
    if(n<=ntt_tile){bm_inverse_dit(a,n);return;}
    inverse_prefix_tiled(a,n);
    const word scale=mint::raw(u32(n)).inv().a;
    if(countr_zero(n)&1)final_radix2_scale(a,n,scale);
    else final_radix4_scale(a,n,scale);
}
inline void forward_pair(mint* a,mint* b,usize n){
    if(n<=1)return;
    forward_tiled(a,n);forward_tiled(b,n);
}
inline void inverse_pair(mint* a,mint* b,usize n){
    if(n<=1)return;
    inverse_tiled(a,n);inverse_tiled(b,n);
}
}

static constexpr int BM_NAIVE_CUTOFF=24;
static mint Bostan_Mori_naive(const vector<mint>& A,const vector<mint>& Q0,u64 N){
    int d=(int)A.size();
    vector<mint> Q=Q0;
    vector<mint> P=nt::convolution(Q,A);P.resize(d);
    vector<mint> Qm(d+1),R(2*d),S(2*d+1);
    while(N){
        for(int i=0;i<=d;i++)Qm[i]=(i&1)?-Q[i]:Q[i];
        fill(R.begin(),R.end(),mint{});fill(S.begin(),S.end(),mint{});
        for(int i=0;i<d;i++)for(int j=0;j<=d;j++)R[i+j]+=P[i]*Qm[j];
        for(int i=0;i<=d;i++)for(int j=0;j<=d;j++)S[i+j]+=Q[i]*Qm[j];
        int b=(int)(N&1);
        for(int i=0;i<d;i++)P[i]=R[2*i+b];
        for(int i=0;i<=d;i++)Q[i]=S[2*i];
        N>>=1;
    }
    return P[0];
}

static inline nd::vec half8(nd::vec x)noexcept{
    x=nd::canonicalize8(x);
    const nd::vec one=_mm256_set1_epi32(1),prime=_mm256_set1_epi32((int)mint::MOD);
    nd::vec mask=_mm256_sub_epi32(_mm256_setzero_si256(),_mm256_and_si256(x,one));
    x=_mm256_add_epi32(x,_mm256_and_si256(mask,prime));
    return _mm256_srli_epi32(x,1);
}
static inline void split16(nd::vec a,nd::vec b,nd::vec& e,nd::vec& o)noexcept{
    const nd::vec idx=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    a=_mm256_permutevar8x32_epi32(a,idx);b=_mm256_permutevar8x32_epi32(b,idx);
    e=_mm256_permute2x128_si256(a,b,0x20);
    o=_mm256_permute2x128_si256(a,b,0x31);
}

/* 変更②: P側の1/2を各反復では掛けず、最後にまとめて補正 */
template<bool Odd>
static inline void make_half(const mint* qf,const mint* pf,mint* qh,mint* ph,const mint* twist,usize m)noexcept{
    usize i=0;
    for(;i+8<=m;i+=8){
        nd::vec qe,qo,pe,po;
        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);
        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);
        nd::store8(qh+i,nd::mul8(qe,qo));
        nd::vec a=nd::mul8(pe,qo),b=nd::mul8(po,qe);
        if constexpr(Odd)nd::store8(ph+i,nd::mul8(nd::sub8(a,b),nd::load8(twist+i)));
        else nd::store8(ph+i,nd::add8(a,b));
    }
    for(;i<m;i++){
        mint qe=qf[2*i],qo=qf[2*i+1],pe=pf[2*i],po=pf[2*i+1];
        qh[i]=qe*qo;
        if constexpr(Odd)ph[i]=(pe*qo-po*qe)*twist[i];
        else ph[i]=pe*qo+po*qe;
    }
}

static mint make_final_odd_constant(const mint* qf,const mint* pf,const mint* twist,usize m)noexcept{
    nd::vec acc=_mm256_setzero_si256();
    usize i=0;
    for(;i+8<=m;i+=8){
        nd::vec qe,qo,pe,po;
        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);
        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);
        nd::vec v=nd::canonicalize8(nd::mul8(nd::sub8(nd::mul8(pe,qo),nd::mul8(po,qe)),nd::load8(twist+i)));
        acc=nd::add8(acc,v);
    }
    alignas(32) nd::word s8[8];
    nd::store8(reinterpret_cast<mint*>(s8),acc);
    nd::word s=0;
    for(int j=0;j<8;j++)s=nd::add(s,s8[j]);
    for(;i<m;i++){
        mint v=(pf[2*i]*qf[2*i+1]-pf[2*i+1]*qf[2*i])*twist[i];
        s=nd::add(s,nd::canonicalize(nd::raw(v)));
    }
    nd::word im=mint::raw((u32)m).inv().a;
    return nd::from_raw(nd::mul(s,im));
}

struct BMPlan{usize z;bool top_alias;};
static inline BMPlan bm_plan(usize plen,usize qlen){
    usize qdeg=qlen-1;
    if(qdeg&&has_single_bit(qdeg)&&plen<=qdeg){
        usize z=qdeg<<1;
        if(z<=nt::max_ntt_size)return {z,true};
    }
    usize need=max(plen+qlen-1,usize(2)*qlen-1);
    usize z=max(usize(2),bit_ceil(need));
    if(z>nt::max_ntt_size)throw length_error("Bostan_Mori");
    return {z,false};
}
static inline usize truncated_len(usize len,u64 N)noexcept{
    if(N>=u64(len-1))return len;
    return usize(N)+1;
}
static inline usize trim_length(const mint* a,usize n)noexcept{
    while(n>1&&a[n-1]==mint{})--n;
    return n;
}

/*
half_twist[bitrev(k)] = 1/w^k
double_twist[i]       = w^i/m
w: primitive z-th root, z=2m
*/
static void build_bm_twists(mint* half_twist,mint* double_twist,usize z)noexcept{
    usize m=z>>1;
    unsigned h=(unsigned)countr_zero(z);
    nd::word w=nd::twiddles.root[h],iw=nd::twiddles.iroot[h];
    nd::word fp=mint::raw((u32)m).inv().a,ip=nd::montgomery_one;
    usize rev=0;
    for(usize k=0;k<m;k++){
        double_twist[k]=nd::from_raw(fp);
        half_twist[rev]=nd::from_raw(ip);
        fp=nd::mul(fp,w);ip=nd::mul(ip,iw);
        if(k+1<m){
            usize bit=m>>1;
            while(rev&bit){rev^=bit;bit>>=1;}
            rev^=bit;
        }
    }
}

static void inverse_twisted(mint* __restrict__ a,usize n,const mint* __restrict__ tw)noexcept{
    if(n==1){a[0]*=tw[0];return;}
    const int h=int(countr_zero(n));
    if(h&1){
        bm_ntt::inverse_prefix_tiled(a,n);
        const usize half=n/2;
        usize i=0;
        for(;i+8<=half;i+=8){
            const auto x=nd::load8(a+i),y=nd::load8(a+half+i);
            nd::store8(a+i,nd::mul8(nd::add8(x,y),nd::load8(tw+i)));
            nd::store8(a+half+i,nd::mul8(nd::sub8(x,y),nd::load8(tw+half+i)));
        }
        for(;i<half;i++){
            mint x=a[i],y=a[half+i];
            a[i]=(x+y)*tw[i];a[half+i]=(x-y)*tw[half+i];
        }
    }else{
        bm_ntt::inverse_prefix_tiled(a,n);
        const usize stride=n/4;
        const auto imag=nd::broadcast(nd::twiddles.iroot[2]);
        usize i=0;
        for(;i+8<=stride;i+=8){
            const auto x0=nd::load8(a+i),x1=nd::load8(a+stride+i),x2=nd::load8(a+2*stride+i),x3=nd::load8(a+3*stride+i);
            const auto s01=nd::add8(x0,x1),d01=nd::sub8(x0,x1),s23=nd::add8(x2,x3),t=nd::mul8_fixed(nd::sub8(x2,x3),imag,_mm256_mul_epu32(imag,nd::broadcast(nd::montgomery_ninv)));
            nd::store8(a+i,nd::mul8(nd::add8(s01,s23),nd::load8(tw+i)));
            nd::store8(a+stride+i,nd::mul8(nd::add8(d01,t),nd::load8(tw+stride+i)));
            nd::store8(a+2*stride+i,nd::mul8(nd::sub8(s01,s23),nd::load8(tw+2*stride+i)));
            nd::store8(a+3*stride+i,nd::mul8(nd::sub8(d01,t),nd::load8(tw+3*stride+i)));
        }
        const mint ii=nd::from_raw(nd::twiddles.iroot[2]);
        for(;i<stride;i++){
            mint x0=a[i],x1=a[stride+i],x2=a[2*stride+i],x3=a[3*stride+i];
            mint s01=x0+x1,d01=x0-x1,s23=x2+x3,t=(x2-x3)*ii;
            a[i]=(s01+s23)*tw[i];a[stride+i]=(d01+t)*tw[stride+i];
            a[2*stride+i]=(s01-s23)*tw[2*stride+i];a[3*stride+i]=(d01-t)*tw[3*stride+i];
        }
    }
}

static inline void ntt_double_pair(const mint* qh,const mint* ph,mint* qf,mint* pf,const mint* double_twist,usize m,bool top_alias,mint qtop)noexcept{
    if(qf!=qh)memcpy(qf,qh,m*sizeof(mint));
    if(pf!=ph)memcpy(pf,ph,m*sizeof(mint));
    memcpy(qf+m,qh,m*sizeof(mint));memcpy(pf+m,ph,m*sizeof(mint));
    inverse_twisted(qf+m,m,double_twist);
    inverse_twisted(pf+m,m,double_twist);
    if(top_alias)qf[m]-=qtop+qtop;
    bm_ntt::forward_pair(qf+m,pf+m,m);
}

static mint Bostan_Mori(const vector<mint>& A,const vector<mint>& Q0,u64 N){
    usize d=A.size();
    if(N<d)return A[usize(N)];
    if(d<=BM_NAIVE_CUTOFF)return Bostan_Mori_naive(A,Q0,N);

    vector<mint> Pv=nt::convolution(Q0,A);Pv.resize(d);
    usize plen=d,qlen=d+1;
    while(plen>1&&Pv[plen-1]==mint{})--plen;
    while(qlen>1&&Q0[qlen-1]==mint{})--qlen;

    BMPlan plan=bm_plan(plen,qlen);
    usize z=plan.z,m=z>>1;
    bool top_alias=plan.top_alias;
    const usize max_z=z,max_m=m;
    nd::aligned_vector qf(max_z),pf(max_z),qh(max_m),ph(max_m);
    nd::aligned_vector half_twist(max_m),double_twist(max_m);

    copy_n(Q0.begin(),qlen,qf.begin());
    copy_n(Pv.begin(),plen,pf.begin());
    fill(qf.begin()+qlen,qf.begin()+z,mint{});
    fill(pf.begin()+plen,pf.begin()+z,mint{});
    bm_ntt::forward_pair(qf.data(),pf.data(),z);
    build_bm_twists(half_twist.data(),double_twist.data(),z);

    mint qtop=top_alias?Q0[qlen-1]:mint{};
    mint p_inv_scale=mint::raw(1);
    const mint inv2=mint::raw(499122177);

    while(N){
        p_inv_scale*=inv2;
        if(N==1)return make_final_odd_constant(qf.data(),pf.data(),half_twist.data(),m)*p_inv_scale;

        const unsigned bit=(unsigned)(N&1);
        const usize pdeg=plen-1,qdeg=qlen-1,product_deg=pdeg+qdeg;
        usize next_plen=product_deg>=bit?(product_deg-bit)/2+1:1;
        usize next_qlen=qlen;

        if(top_alias){
            mint t=qtop*qtop;
            if(qdeg&1)t=-t;
            qtop=t;
        }

        N>>=1;
        usize cut_plen=truncated_len(next_plen,N);
        usize cut_qlen=truncated_len(next_qlen,N);
        BMPlan desired=bm_plan(cut_plen,cut_qlen);

        if(desired.z<z){
            if(bit)make_half<true>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);
            else make_half<false>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);
            usize old_m=m;
            bm_ntt::inverse_pair(qh.data(),ph.data(),old_m);

            if(top_alias)qh[0]-=qtop;

            usize qcopy=min(cut_qlen,old_m);
            memcpy(qf.data(),qh.data(),qcopy*sizeof(mint));
            if(top_alias&&cut_qlen>old_m)qf[old_m]=qtop;
            memcpy(pf.data(),ph.data(),cut_plen*sizeof(mint));

            plen=trim_length(pf.data(),cut_plen);
            qlen=trim_length(qf.data(),cut_qlen);

            plan=bm_plan(plen,qlen);
            z=plan.z;m=z>>1;
            top_alias=plan.top_alias;
            qtop=top_alias?qf[qlen-1]:mint{};

            fill(qf.begin()+qlen,qf.begin()+z,mint{});
            fill(pf.begin()+plen,pf.begin()+z,mint{});
            bm_ntt::forward_pair(qf.data(),pf.data(),z);
            build_bm_twists(half_twist.data(),double_twist.data(),z);
        }else{
            plen=next_plen;qlen=next_qlen;
            if(bit)make_half<true>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);
            else make_half<false>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);
            ntt_double_pair(qf.data(),pf.data(),qf.data(),pf.data(),double_twist.data(),m,top_alias,qtop);
        }
    }
    return {};
}

int main(){
    fastio_unsafe io;
    char* in=io.input_cursor();
    u32 d=io.read_u32_lt1e9(in);
    u64 k=io.read_u64(in);

    vector<mint> A(d);
    for(auto& x:A)x=mint::raw(io.read_u32_lt1e9(in));

    if(k<d){
        printf("%u\n",A[(usize)k].val());
        return 0;
    }

    vector<mint> Q(usize(d)+1);
    Q[0]=mint::raw(1);
    for(u32 i=0;i<d;i++)Q[usize(i)+1]=-mint::raw(io.read_u32_lt1e9(in));

    printf("%u\n",Bostan_Mori(A,Q,k).val());
    return 0;
}