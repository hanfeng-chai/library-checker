#pragma GCC target("avx2")
#pragma GCC optimize("O3")

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>
#include <memory>
#include <new>
#include <vector>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 12
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
    for(int digits=0;digits<16;++digits){
        for(int i=0;i<16;++i){
            masks[digits][i]=i<16-digits?static_cast<char>(0x80):static_cast<char>(i-(16-digits));
        }
    }
    return masks;
}

constexpr auto make_powers_10(){
    std::array<u64,17> powers{};
    powers[0]=1;
    for(std::size_t i=1;i<powers.size();++i) powers[i]=powers[i-1]*10;
    return powers;
}

constexpr auto make_pair_digits(){
    std::array<unsigned char,1<<14> table{};
    table.fill(255);
    for(unsigned a=0;a<10;++a){
        for(unsigned b=0;b<10;++b){
            table[('0'+a)|(('0'+b)<<8)]=static_cast<unsigned char>(a*10+b);
        }
    }
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
                void* zero_page=file_mapping==MAP_FAILED ? MAP_FAILED : ::mmap(region+rounded_size,page_size,PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,-1,0);
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

    char* cursor() const noexcept{return cursor_;}

private:
    void read_all_fallback(){
        std::size_t capacity=1u<<20,size=0;
        char* buffer=static_cast<char*>(std::malloc(capacity+64));
        if(buffer==nullptr) std::abort();
        for(;;){
            if(size==capacity){
                capacity*=2;
                char* grown=static_cast<char*>(std::realloc(buffer,capacity+64));
                if(grown==nullptr) std::abort();
                buffer=grown;
            }
            const std::size_t count=std::fread(buffer+size,1,capacity-size,stdin);
            size+=count;
            if(count==0) break;
        }
        std::memset(buffer+size,0,64);
        cursor_=buffer;
    }

    char* cursor_=nullptr;
};

__attribute__((always_inline)) inline u64 parse_16_digits(__m128i digits) noexcept{
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

__attribute__((always_inline)) inline __m128i load_digits(const char* cursor) noexcept{
    return _mm_sub_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i*>(cursor)),_mm_set1_epi8('0'));
}

__attribute__((always_inline)) inline u64 parse_short_digits(__m128i digits,u32 mask,int& length) noexcept{
    length=__builtin_ctz(mask);
    digits=_mm_shuffle_epi8(digits,_mm_load_si128(reinterpret_cast<const __m128i*>(right_align_masks[static_cast<std::size_t>(length)].data())));
    return parse_16_digits(digits);
}

__attribute__((always_inline)) inline u32 read_u32(char*& cursor) noexcept{
    const __m128i digits=load_digits(cursor);
    const u32 mask=static_cast<u32>(_mm_movemask_epi8(digits));
    int length;
    const u32 value=static_cast<u32>(parse_short_digits(digits,mask,length));
    cursor+=length+1;
    return value;
}

__attribute__((always_inline)) inline u32 read_u32_lt1e6(char*& cursor) noexcept{
    const auto q0=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+1)];
    const auto q1=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+3)];

    if(__builtin_expect((q0|q1)<128,1)){
        u32 value=static_cast<unsigned char>(cursor[0])-'0';
        value=value*100+q0;
        value=value*100+q1;
        cursor+=5;
        if(*cursor>' ') value=value*10+static_cast<unsigned>(*cursor++&15);
        ++cursor;
        return value;
    }

    u32 value=static_cast<unsigned char>(*cursor++)-'0';
    for(unsigned i=0;i<2;++i){
        const auto pair=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor)];
        if(pair>99) break;
        value=value*100+pair;
        cursor+=2;
    }
    if(*cursor>' ') value=value*10+static_cast<unsigned>(*cursor++&15);
    ++cursor;
    return value;
}

__attribute__((always_inline)) inline u32 read_u32_lt1e9(char*& cursor) noexcept{
    const auto q0=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+1)];
    const auto q1=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+3)];
    const auto q2=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+5)];
    const auto q3=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor+7)];

    if(__builtin_expect((q0|q1|q2|q3)<128,1)){
        u32 value=static_cast<unsigned char>(cursor[0])-'0';
        value=value*100+q0;
        value=value*100+q1;
        value=value*100+q2;
        value=value*100+q3;
        cursor+=10;
        return value;
    }

    u32 value=static_cast<unsigned char>(*cursor++)-'0';
    for(unsigned i=0;i<4;++i){
        const auto pair=pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor)];
        if(pair>99) break;
        value=value*100+pair;
        cursor+=2;
    }
    if(*cursor>' ') value=value*10+static_cast<unsigned>(*cursor++&15);
    ++cursor;
    return value;
}

__attribute__((always_inline)) inline i32 read_i32(char*& cursor) noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u32 magnitude=read_u32(cursor);
    const u32 bits=negative?u32{0}-magnitude:magnitude;
    return static_cast<i32>(bits);
}

__attribute__((always_inline)) inline u64 read_u64(char*& cursor) noexcept{
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
    while(*cursor>='0'){
        value=value*10+static_cast<unsigned>(*cursor&15);
        ++cursor;
    }
    ++cursor;
    return value;
}

__attribute__((always_inline)) inline i64 read_i64(char*& cursor) noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u64 magnitude=read_u64(cursor);
    const u64 bits=negative?u64{0}-magnitude:magnitude;
    return static_cast<i64>(bits);
}

__attribute__((always_inline)) inline u128 read_u128(char*& cursor) noexcept{
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

__attribute__((always_inline)) inline i128 read_i128(char*& cursor) noexcept{
    const bool negative=*cursor=='-';
    cursor+=static_cast<unsigned>(negative);
    const u128 magnitude=read_u128(cursor);
    const u128 bits=negative?u128{0}-magnitude:magnitude;
    return static_cast<i128>(bits);
}

constexpr u32 pack4(char a,char b,char c,char d) noexcept{
    return static_cast<u32>(static_cast<unsigned char>(a))|
           (static_cast<u32>(static_cast<unsigned char>(b))<<8)|
           (static_cast<u32>(static_cast<unsigned char>(c))<<16)|
           (static_cast<u32>(static_cast<unsigned char>(d))<<24);
}

constexpr auto make_padded_groups(){
    std::array<u32,10000> table{};
    for(int value=0;value<10000;++value){
        table[static_cast<std::size_t>(value)]=pack4(
            static_cast<char>('0'+value/1000),
            static_cast<char>('0'+value/100%10),
            static_cast<char>('0'+value/10%10),
            static_cast<char>('0'+value%10));
    }
    return table;
}

inline constexpr auto padded_groups=make_padded_groups();

struct output{
    output()=default;
    output(const output&)=delete;
    output& operator=(const output&)=delete;

    char* begin() noexcept{return buffer_.data();}
    char* end() noexcept{return buffer_.data()+buffer_.size();}

    __attribute__((noinline)) char* flush(char* cursor) noexcept{
        std::size_t remaining=static_cast<std::size_t>(cursor-buffer_.data());
        const char* data=buffer_.data();

        if(first_flush_&&remaining!=0){
            ++data;
            --remaining;
            first_flush_=false;
        }

#ifdef __linux__
        while(remaining!=0){
            const ssize_t count=::write(1,data,remaining);
            if(count>0){
                data+=count;
                remaining-=static_cast<std::size_t>(count);
            }else if(count<0&&errno==EINTR){
                continue;
            }else{
                std::abort();
            }
        }
#else
        while(remaining!=0){
            const std::size_t count=std::fwrite(data,1,remaining,stdout);
            if(count==0) std::abort();
            data+=count;
            remaining-=count;
        }
#endif
        return buffer_.data();
    }

    void finish(char* cursor) noexcept{
        if(cursor!=buffer_.data()||!first_flush_) *cursor++='\n';
        (void)flush(cursor);
    }

    alignas(64) std::array<char,1u<<19> buffer_;
    bool first_flush_=true;
};

__attribute__((always_inline)) inline void store_group(char*& cursor,u32 group) noexcept{
    std::memcpy(cursor,&group,sizeof(group));
    cursor+=sizeof(group);
}

__attribute__((always_inline)) inline void emit_leading(char*& cursor,u64 value) noexcept{
    const unsigned skip=3u-static_cast<unsigned>(value>=10)-static_cast<unsigned>(value>=100)-static_cast<unsigned>(value>=1000);
    const u32 group=padded_groups[static_cast<std::size_t>(value)]>>(skip*8);
    std::memcpy(cursor,&group,sizeof(group));
    cursor+=4-skip;
}

__attribute__((always_inline)) inline void emit_padded(char*& cursor,u64 value) noexcept{
    store_group(cursor,padded_groups[static_cast<std::size_t>(value)]);
}

__attribute__((always_inline)) inline void emit_padded_16(char*& cursor,u64 value) noexcept{
    emit_padded(cursor,value/1000000000000ULL);
    emit_padded(cursor,value/100000000ULL%10000);
    emit_padded(cursor,value/10000ULL%10000);
    emit_padded(cursor,value%10000);
}

__attribute__((always_inline)) inline void emit_u32_unchecked(char*& cursor,u32 value) noexcept{
    if(value>=100000000U){
        emit_leading(cursor,value/100000000U);
        emit_padded(cursor,value/10000U%10000);
        emit_padded(cursor,value%10000);
    }else if(value>=10000U){
        emit_leading(cursor,value/10000U);
        emit_padded(cursor,value%10000);
    }else{
        emit_leading(cursor,value);
    }
}

__attribute__((always_inline)) inline void emit_u64_unchecked(char*& cursor,u64 value) noexcept{
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
    }else{
        emit_leading(cursor,value);
    }
}

__attribute__((always_inline)) inline void emit_u128_unchecked(char*& cursor,u128 value) noexcept{
    constexpr u128 base=static_cast<u128>(10000000000000000ULL);
    constexpr u128 u64_max=static_cast<u128>(~u64{0});

    if(value<=u64_max){
        emit_u64_unchecked(cursor,static_cast<u64>(value));
        return;
    }

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

__attribute__((always_inline)) inline void write_u32_lt1e6(output& sink,char*& cursor,char* end,u32 value) noexcept{
    if(__builtin_expect(end-cursor<8,0)) cursor=sink.flush(cursor);
    *cursor++=' ';
    if(value>=10000U){
        const u32 high=value/10000U;
        emit_leading(cursor,high);
        emit_padded(cursor,value-high*10000U);
    }else{
        emit_leading(cursor,value);
    }
}

__attribute__((always_inline)) inline void write_u32_lt1e9(output& sink,char*& cursor,char* end,u32 value) noexcept{
    if(__builtin_expect(end-cursor<16,0)) cursor=sink.flush(cursor);
    *cursor++=' ';

    if(value>=100000000U){
        const u32 high=value/100000000U;
        *cursor++=static_cast<char>('0'+high);
        value-=high*100000000U;
        emit_padded(cursor,value/10000U);
        emit_padded(cursor,value%10000U);
    }else{
        emit_u32_unchecked(cursor,value);
    }
}

__attribute__((always_inline)) inline void write_u32(output& sink,char*& cursor,char* end,u32 value) noexcept{
    if(__builtin_expect(end-cursor<16,0)) cursor=sink.flush(cursor);
    *cursor++=' ';
    emit_u32_unchecked(cursor,value);
}

__attribute__((always_inline)) inline void write_i32(output& sink,char*& cursor,char* end,i32 value) noexcept{
    if(__builtin_expect(end-cursor<16,0)) cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u32 bits=static_cast<u32>(value);
    const u32 magnitude=negative?u32{0}-bits:bits;
    *cursor++=' ';
    if(negative) *cursor++='-';
    emit_u32_unchecked(cursor,magnitude);
}

__attribute__((always_inline)) inline void write_u64(output& sink,char*& cursor,char* end,u64 value) noexcept{
    if(__builtin_expect(end-cursor<24,0)) cursor=sink.flush(cursor);
    *cursor++=' ';
    emit_u64_unchecked(cursor,value);
}

__attribute__((always_inline)) inline void write_i64(output& sink,char*& cursor,char* end,i64 value) noexcept{
    if(__builtin_expect(end-cursor<24,0)) cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u64 bits=static_cast<u64>(value);
    const u64 magnitude=negative?u64{0}-bits:bits;
    *cursor++=' ';
    if(negative) *cursor++='-';
    emit_u64_unchecked(cursor,magnitude);
}

__attribute__((always_inline)) inline void write_u128(output& sink,char*& cursor,char* end,u128 value) noexcept{
    if(__builtin_expect(end-cursor<48,0)) cursor=sink.flush(cursor);
    *cursor++=' ';
    emit_u128_unchecked(cursor,value);
}

__attribute__((always_inline)) inline void write_i128(output& sink,char*& cursor,char* end,i128 value) noexcept{
    if(__builtin_expect(end-cursor<48,0)) cursor=sink.flush(cursor);
    const bool negative=value<0;
    const u128 bits=static_cast<u128>(value);
    const u128 magnitude=negative?u128{0}-bits:bits;
    *cursor++=' ';
    if(negative) *cursor++='-';
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

    char* input_cursor() const noexcept{return in.cursor();}
    char* output_cursor() noexcept{return out.begin();}
    char* output_end() noexcept{return out.end();}
    void finish(char* cursor) noexcept{out.finish(cursor);}

    __attribute__((always_inline)) u32 read_u32(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_u32(cursor);
    }

    __attribute__((always_inline)) u32 read_u32_lt1e6(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_u32_lt1e6(cursor);
    }

    __attribute__((always_inline)) u32 read_u32_lt1e9(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_u32_lt1e9(cursor);
    }

    __attribute__((always_inline)) i32 read_i32(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_i32(cursor);
    }

    __attribute__((always_inline)) u64 read_u64(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_u64(cursor);
    }

    __attribute__((always_inline)) i64 read_i64(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_i64(cursor);
    }

    __attribute__((always_inline)) u128 read_u128(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_u128(cursor);
    }

    __attribute__((always_inline)) i128 read_i128(char*& cursor) noexcept{
        return fastio_unsafe_impl::read_i128(cursor);
    }

    __attribute__((always_inline)) void write_u32(char*& cursor,char* end,u32 value) noexcept{
        fastio_unsafe_impl::write_u32(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_u32_lt1e6(char*& cursor,char* end,u32 value) noexcept{
        fastio_unsafe_impl::write_u32_lt1e6(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_u32_lt1e9(char*& cursor,char* end,u32 value) noexcept{
        fastio_unsafe_impl::write_u32_lt1e9(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_i32(char*& cursor,char* end,i32 value) noexcept{
        fastio_unsafe_impl::write_i32(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_u64(char*& cursor,char* end,u64 value) noexcept{
        fastio_unsafe_impl::write_u64(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_i64(char*& cursor,char* end,i64 value) noexcept{
        fastio_unsafe_impl::write_i64(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_u128(char*& cursor,char* end,u128 value) noexcept{
        fastio_unsafe_impl::write_u128(out,cursor,end,value);
    }

    __attribute__((always_inline)) void write_i128(char*& cursor,char* end,i128 value) noexcept{
        fastio_unsafe_impl::write_i128(out,cursor,end,value);
    }

    fastio_unsafe_impl::input in;
    fastio_unsafe_impl::output out;
};

using namespace std;

#if defined(__linux__)
#define UF_USE_HUGEPAGE 1
#endif

#if UF_USE_HUGEPAGE
namespace uf_detail{
constexpr size_t kAlign=size_t(1)<<21;

inline void* huge_alloc(size_t bytes){
    size_t len=(bytes+kAlign-1)&~(kAlign-1);
    if(len==0) len=kAlign;
    void* ptr=mmap(nullptr,len,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if(ptr==MAP_FAILED) throw bad_alloc();
    madvise(ptr,len,MADV_HUGEPAGE);
    return ptr;
}

inline void huge_free(void* ptr,size_t bytes) noexcept{
    size_t len=(bytes+kAlign-1)&~(kAlign-1);
    if(len==0) len=kAlign;
    munmap(ptr,len);
}
}

template<class T>
struct HugePageAllocator{
    using value_type=T;
    HugePageAllocator() noexcept=default;
    template<class U> HugePageAllocator(const HugePageAllocator<U>&) noexcept{}

    T* allocate(size_t n){
        return static_cast<T*>(uf_detail::huge_alloc(n*sizeof(T)));
    }

    void deallocate(T* p,size_t n) noexcept{
        uf_detail::huge_free(p,n*sizeof(T));
    }

    template<class U> bool operator==(const HugePageAllocator<U>&) const noexcept{return true;}
    template<class U> bool operator!=(const HugePageAllocator<U>&) const noexcept{return false;}
};

template<class T>
using UFAllocator=HugePageAllocator<T>;
#else
template<class T>
using UFAllocator=std::allocator<T>;
#endif

struct UnionFind{
private:
    std::vector<int32_t,UFAllocator<int32_t>> p;

public:
    explicit UnionFind(int n):p(n,-1){}

    [[gnu::always_inline]] inline bool same(int32_t x,int32_t y) noexcept{
        int32_t* const __restrict__ p=this->p.data();
        for(;;){
            const int32_t px=p[x];
            if(px<0) break;
            const int32_t ppx=p[px];
            if(ppx<0){
                x=px;
                break;
            }
            p[x]=ppx;
            x=ppx;
        }
        for(;;){
            const int32_t py=p[y];
            if(py<0) break;
            const int32_t ppy=p[py];
            if(ppy<0){
                y=py;
                break;
            }
            p[y]=ppy;
            y=ppy;
        }
        return x==y;
    }

    [[gnu::always_inline]] inline bool merge(int32_t x,int32_t y) noexcept{
        int32_t* const __restrict__ p=this->p.data();
        for(;;){
            const int32_t px=p[x];
            if(px<0) break;
            const int32_t ppx=p[px];
            if(ppx<0){
                x=px;
                break;
            }
            p[x]=ppx;
            x=ppx;
        }
        for(;;){
            const int32_t py=p[y];
            if(py<0) break;
            const int32_t ppy=p[py];
            if(ppy<0){
                y=py;
                break;
            }
            p[y]=ppy;
            y=ppy;
        }
        if(x==y) return false;
        if(p[x]>p[y]){
            const int32_t t=x;
            x=y;
            y=t;
        }
        p[x]+=p[y];
        p[y]=x;
        return true;
    }
};

int main(){
    fastio_unsafe io;
    char* in=io.input_cursor();
    char* out=io.output_cursor();
    char* const out_end=io.output_end();

    uint32_t N=io.read_u32_lt1e6(in),Q=io.read_u32_lt1e6(in);
    UnionFind uf(N);

    for(;Q--;){
        uint32_t t=io.read_u32_lt1e6(in);
        uint32_t u=io.read_u32_lt1e6(in);
        uint32_t v=io.read_u32_lt1e6(in);
        if(t==0) uf.merge(u,v);
        else io.write_u32_lt1e6(out,out_end,uf.same(u,v));
    }
    io.finish(out);
}