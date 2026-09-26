#line 1 "verify/string/longest_common_substring.test.cpp"
// @brief Longest Common Substring
#define PROBLEM "https://judge.yosupo.jp/problem/longest_common_substring"
#include <bits/stdc++.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algo/structures/suffix_automaton.hpp"


#line 5 "cp-algo/structures/suffix_automaton.hpp"
#include <bit>
#include <cassert>
#line 14 "cp-algo/structures/suffix_automaton.hpp"
#include <string_view>
#line 1 "cp-algo/util/big_alloc.hpp"



#line 14 "cp-algo/util/big_alloc.hpp"

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, size_t Align = 32>
    class big_alloc {
        static_assert( Align >= alignof(void*), "Align must be at least pointer-size");
        static_assert(std::popcount(Align) == 1, "Align must be a power of two");
    public:
        using value_type = T;
        template <class U> struct rebind { using other = big_alloc<U, Align>; };
        constexpr bool operator==(const big_alloc&) const = default;
        constexpr bool operator!=(const big_alloc&) const = default;

        big_alloc() noexcept = default;
        template <typename U, std::size_t A>
        big_alloc(const big_alloc<U, A>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align = std::max<std::size_t>(alignof(T),  Align);
#if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) {
                void* raw = mmap(nullptr, padded,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                madvise(raw, padded, MADV_HUGEPAGE);
                return static_cast<T*>(raw);
            }
#endif
            return static_cast<T*>(::operator new(padded, std::align_val_t(align)));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            if (!p) return;
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align  = std::max<std::size_t>(alignof(T),  Align);
    #if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) { munmap(p, padded); return; }
    #endif
            ::operator delete(p, padded, std::align_val_t(align));
        }

    private:
        static constexpr std::size_t MEGABYTE = 1 << 20;
        static constexpr std::size_t round_up(std::size_t x) noexcept {
            return (x + Align - 1) / Align * Align;
        }
    };

    template<typename T> using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T> using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T> using big_deque = std::deque<T, big_alloc<T>>;
    template<typename T> using big_stack = std::stack<T, big_deque<T>>;
    template<typename T> using big_queue = std::queue<T, big_deque<T>>;
    template<typename T> using big_priority_queue = std::priority_queue<T, big_vector<T>>;
    template<typename T> using big_forward_list = std::forward_list<T, big_alloc<T>>;
    using big_string = big_basic_string<char>;

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    template<typename T, typename Compare = std::less<T>>
    using big_multiset = std::multiset<T, Compare, big_alloc<T>>;
    template<typename T, typename Compare = std::less<T>>
    using big_set = std::set<T, Compare, big_alloc<T>>;
}


#line 1 "cp-algo/util/checkpoint.hpp"


#line 8 "cp-algo/util/checkpoint.hpp"
namespace cp_algo {
#ifdef CP_ALGO_CHECKPOINT
    big_map<big_string, double> checkpoints;
    double last;
#endif
    template<bool final = false>
    void checkpoint([[maybe_unused]] auto const& _msg) {
#ifdef CP_ALGO_CHECKPOINT
        big_string msg = _msg;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size() && !final) {
            checkpoints[msg] += delta;
        }
        if(final) {
            for(auto const& [key, value] : checkpoints) {
                std::cerr << key << ": " << value * 1000 << " ms\n";
            }
            std::cerr << "Total: " << now * 1000 << " ms\n";
        }
#endif
    }
    template<bool final = false>
    void checkpoint() {
        checkpoint<final>("");
    }
}

#line 19 "cp-algo/structures/suffix_automaton.hpp"

namespace cp_algo::structures {
    auto suffix_array(std::string text);

    // An immutable substring index for lowercase Latin letters.
    // Packed state IDs support strings of fewer than 2^23 characters.
    class suffix_automaton {
    public:
        explicit suffix_automaton(std::string text): suffix_automaton(std::move(text), false) {}

        int64_t count_distinct() const { return distinct; }

        // Half-open intervals [a,b) in the indexed text and [c,d) in other.
        std::array<int, 4> longest_common_substring(std::string_view other) const {
            switch(width) {
                case 0: return match<0>(other);
                case 1: return match<1>(other);
                case 2: return match<2>(other);
                case 4: return match<4>(other);
                default: return match<-1>(other);
            }
        }

    private:
        friend auto suffix_array(std::string text);
        // Align large mappings to huge-page boundaries. This policy is local to SAM.
        template<class T> struct page_allocator {
            using value_type = T;
            template<class U> struct rebind { using other = page_allocator<U>; };
            page_allocator() = default;
            template<class U> page_allocator(page_allocator<U> const&) {}
            bool operator==(page_allocator const&) const { return true; }
            static constexpr size_t page = 1U << 21, threshold = 1U << 20;
            T* allocate(size_t count) {
                if(count > (std::numeric_limits<size_t>::max() - 2 * page) / sizeof(T))
                    throw std::bad_array_new_length();
                size_t bytes = count * sizeof(T);
#ifdef __linux__
                if(bytes >= threshold) {
                    bytes = (bytes + page - 1) & -page;
                    void *raw = mmap(nullptr, bytes + page, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    if(raw == MAP_FAILED) throw std::bad_alloc();
                    uintptr_t begin = (uintptr_t(raw) + page - 1) & -page;
                    size_t before = begin - uintptr_t(raw), after = page - before;
                    if(before) munmap(raw, before);
                    if(after) munmap(reinterpret_cast<void*>(begin + bytes), after);
                    madvise(reinterpret_cast<void*>(begin), bytes, MADV_HUGEPAGE);
                    return reinterpret_cast<T*>(begin);
                }
#endif
                auto p = static_cast<T*>(std::calloc(std::max(count, size_t(1)), sizeof(T)));
                if(!p) throw std::bad_alloc();
                return p;
            }
            void deallocate(T *p, size_t count) {
#ifdef __linux__
                if(count * sizeof(T) >= threshold) {
                    munmap(p, (count * sizeof(T) + page - 1) & -page);
                    return;
                }
#endif
                std::free(p);
            }
        };
        template<class T> using storage = std::vector<T, page_allocator<T>>;
        using row = std::array<int, 26>;
        // Newly mapped pages are zero. Begin row lifetimes without touching every page.
        template<class T> struct zero_storage {
            T *data = nullptr;
            size_t capacity = 0;
            zero_storage() = default;
            explicit zero_storage(size_t n): data(page_allocator<T>{}.allocate(n)), capacity(n) {
                std::uninitialized_default_construct_n(data, n);
            }
            zero_storage(zero_storage const &other): zero_storage(other.capacity) {
                if(capacity) std::copy_n(other.data, capacity, data);
            }
            zero_storage(zero_storage &&other) noexcept { swap(other); }
            zero_storage &operator=(zero_storage other) noexcept { swap(other); return *this; }
            void swap(zero_storage &other) noexcept {
                std::swap(data, other.data); std::swap(capacity, other.capacity);
            }
            ~zero_storage() { if(data) page_allocator<T>{}.deallocate(data, capacity); }
        };
        struct clone_data { int len, pos; };
        struct transitions { uint32_t a = 0, b = 0; };
        static constexpr uint32_t dense_tag = 0x80000000, id_mask = 0xFFFFFF;
        std::string word;
        int n = 0, alphabet = 26, width = 0;
        int64_t distinct = 0;
        std::array<int, 26> code{};
        storage<clone_data> clones;
        storage<int> link, flat;
        storage<transitions> to;
        storage<row> dense;
        zero_storage<row> clone_edges, prefix_edges;
        int prefix_bound = 0;

        suffix_automaton(std::string text, bool suffix_order): word(std::move(text)) {
            assert(word.size() < (1U << 23));
            n = (int)word.size();
            if(suffix_order) {
                for(int c = 0; c < 26; c++) code[c] = c;
            } else {
                code.fill(-1);
                uint32_t letters = 0;
                for(unsigned char c: word) {
                    assert(c >= 'a' && c <= 'z');
                    letters |= 1U << (c - 'a');
                }
                alphabet = 0;
                for(int c = 0; c < 26; c++) {
                    if(letters >> c & 1) code[c] = alphabet++;
                }
                if(alphabet != 26) {
                    for(char &c: word) c = char('a' + code[c - 'a']);
                }
                if(alphabet <= 1) width = 1;
                else if(alphabet <= 2) width = 2;
                else if(alphabet <= 4) width = 4;
                // Avoid tagged lookups while the ordinary dense rows fit in 32 MiB.
                else if(alphabet <= 16 || size_t(n) * alphabet <= (1U << 23)) width = -1;
            }
            clones.reserve(n);
            link.resize(2 * n + 1);
            if(suffix_order) {
                prefix_edges = zero_storage<row>(n + 1);
                clone_edges = zero_storage<row>(n);
            } else if(width == 0) {
                // At most one dense row per state, including clones.
                dense.reserve(link.size());
                to.reserve(link.size());
                to.resize(n + 1);
            } else {
                flat.resize(link.size() * (width > 0 ? width : alphabet));
            }
            checkpoint("init");
            if(suffix_order) {
                build<0, false, true>();
            }
            else switch(width) {
                case 0: build<0>(); break;
                case 1: build<1>(); break;
                case 2: build<2>(); break;
                case 4: build<4>(); break;
                default: build<-1>();
            }
            checkpoint("build");
        }

        int states() const { return n + 1 + (int)clones.size(); }
        // Ordinary state i represents prefix i; only clones need stored metadata.
        int length(int v) const { return v <= n ? v : clones[v - n - 1].len; }
        int position(int v) const { return v <= n ? v : clones[v - n - 1].pos; }

        static row &clone_row(int p, uintptr_t base) {
            return *reinterpret_cast<row*>(base + size_t(p) * sizeof(row));
        }
        template<int Width, bool CloneDense = false>
        int get(int p, int x, uintptr_t clone_base = 0) const {
            if constexpr(CloneDense) {
                if(p > n) return clone_row(p, clone_base)[x];
                // A prefix state's first edge is always p -> p+1 and never redirected.
                if(word[p] == char('a' + x)) return p + 1;
                return p <= prefix_bound ? prefix_edges.data[p][x] : 0;
            }
            if constexpr(Width != 0) {
                return flat[size_t(p) * (Width > 0 ? Width : alphabet) + x];
            } else {
                auto t = to[p];
                if(t.a & dense_tag) return dense[t.a & ~dense_tag][x];
                if((t.a >> 24) == unsigned(x + 1)) return int(t.a & id_mask);
                if((t.b >> 24) == unsigned(x + 1)) return int(t.b & id_mask);
                return 0;
            }
        }
        template<int Width, bool CloneDense = false>
        void set(int p, int x, int v, uintptr_t clone_base = 0) {
            if constexpr(CloneDense) {
                if(p > n) { clone_row(p, clone_base)[x] = v; return; }
                prefix_bound = std::max(prefix_bound, p);
                prefix_edges.data[p][x] = v;
                return;
            }
            if constexpr(Width != 0) {
                flat[size_t(p) * (Width > 0 ? Width : alphabet) + x] = v;
            } else {
                auto &t = to[p];
                auto encoded = (uint32_t(x + 1) << 24) | v;
                if(t.a & dense_tag) dense[t.a & ~dense_tag][x] = v;
                else if(!t.a || (t.a >> 24) == unsigned(x + 1)) t.a = encoded;
                else if(!t.b || (t.b >> 24) == unsigned(x + 1)) t.b = encoded;
                else {
                    std::array<int, 26> row{};
                    row[(t.a >> 24) - 1] = t.a & id_mask;
                    row[(t.b >> 24) - 1] = t.b & id_mask;
                    row[x] = v;
                    t.a = dense_tag | uint32_t(dense.size());
                    dense.push_back(row);
                }
            }
        }
        // In suffix-order mode the high byte caches immutable clone lengths.
        // Zero denotes an ordinary state; 255 falls back to full metadata.
        int tagged_length(uint32_t v) const {
            int len = v >> 24;
            return len == 255 ? length(v & id_mask) : len ? len : int(v & id_mask);
        }
        template<int Width, bool Count = true, bool CloneDense = false>
        void build() {
            auto state_id = [](uint32_t v) -> int {
                if constexpr(CloneDense) return v & id_mask;
                else return v;
            };
            auto state_length = [&](uint32_t v) {
                if constexpr(CloneDense) return tagged_length(v);
                else return length(v);
            };
            // Compute the ID-to-row adjustment once, outside the lookup dependency chain.
            uintptr_t clone_base = 0;
            if constexpr(CloneDense)
                clone_base = reinterpret_cast<uintptr_t>(clone_edges.data) - size_t(n + 1) * sizeof(row);
            int last = 0;
            for(unsigned char c: word) {
                int x = c - 'a';
                uint32_t current = last;
                if constexpr(CloneDense) {
                    // The first edge of the newest prefix is implicit.
                    current = link[last];
                }
                int p = state_id(current);
                ++last;
                uint32_t found;
                while(!(found = get<Width, CloneDense>(p, x, clone_base))) {
                    set<Width, CloneDense>(p, x, last, clone_base);
                    current = link[p];
                    p = state_id(current);
                }
                int q = state_id(found);
                if(q != last) {
                    int len = state_length(current) + 1;
                    if(state_length(found) == len) link[last] = found;
                    else {
                        int clone = states();
                        clones.push_back({len, position(q)});
                        if constexpr(CloneDense) {
                            auto &copy = clone_row(clone, clone_base);
                            if(q > n) copy = clone_row(q, clone_base);
                            else {
                                if(q <= prefix_bound) copy = prefix_edges.data[q];
                                copy[word[q] - 'a'] = q + 1;
                            }
                        } else if constexpr(Width != 0) {
                            int stride = Width > 0 ? Width : alphabet;
                            std::copy_n(flat.data() + size_t(q) * stride, stride,
                                        flat.data() + size_t(clone) * stride);
                        } else {
                            auto row = to[q];
                            if(row.a & dense_tag) {
                                auto copy = dense[row.a & ~dense_tag];
                                row.a = dense_tag | uint32_t(dense.size());
                                dense.push_back(copy);
                            }
                            to.push_back(row);
                        }
                        uint32_t tagged = clone;
                        if constexpr(CloneDense) tagged |= uint32_t(std::min(len, 255)) << 24;
                        link[last] = link[q] = tagged;
                        uint32_t next;
                        while(state_id(next = get<Width, CloneDense>(p, x, clone_base)) == q) {
                            set<Width, CloneDense>(p, x, tagged, clone_base);
                            current = link[p];
                            p = state_id(current);
                        }
                        // The first different end-position class is the clone's suffix link.
                        // Redirecting at the root makes that final transition the clone itself.
                        link[clone] = state_id(next) == clone ? 0 : next;
                    }
                }
                if constexpr(Count) distinct += last - length(link[last]);
            }
        }
        template<int Width>
        std::array<int, 4> match(std::string_view other) const {
            int v = 0, matched = 0, best = 0, end_s = 0, end_t = 0;
            for(int i = 0; i < (int)other.size(); i++) {
                unsigned c = (unsigned char)other[i] - unsigned('a');
                int x = c < 26 ? code[c] : -1;
                if(x < 0) { v = matched = 0; continue; }
                while(v && !get<Width>(v, x)) {
                    v = link[v];
                    matched = length(v);
                }
                v = get<Width>(v, x);
                matched = v ? matched + 1 : 0;
                if(matched > best) { best = matched; end_s = position(v); end_t = i + 1; }
            }
            checkpoint("query");
            return {end_s - best, end_s, end_t - best, end_t};
        }

        // Consumes a temporary SAM of the reversed input. No storage mutation is public.
        [[gnu::always_inline]] auto take_suffix_order() && {
            dense = decltype(dense){};
            clone_edges = zero_storage<row>{};
            prefix_edges = zero_storage<row>{};
            int size = states();
            const int n = this->n;
            auto links = link.data();
            auto text = word.data();
            auto clone = clones.data();
            auto position = [&](int v) { return v <= n ? v : clone[v - n - 1].pos; };
            auto tagged_length = [&](uint32_t v) {
                int len = v >> 24;
                return len == 255 ? (int(v & id_mask) <= n ? int(v & id_mask) : clone[(v & id_mask) - n - 1].len)
                                  : len ? len : int(v & id_mask);
            };
            struct tree_node { uint32_t offset, mask; };
            zero_storage<tree_node> tree(size + 1);
            zero_storage<int> children(size);
            uint32_t border = links[n];
            while((border & id_mask) > unsigned(n) && tagged_length(border) > prefix_bound)
                border = links[border & id_mask];
            int last_branch = std::max(prefix_bound, (border & id_mask) <= unsigned(n) ? int(border & id_mask) : 0);
            for(int i = 1; i < size; i++) {
                int p = links[i] & id_mask, x = text[position(i) - tagged_length(links[i]) - 1] - 'a';
                tree.data[p].mask |= 1U << x;
                links[i] = (x << 24) | p;
            }
            // Beyond the longest repeated prefix, ordinary states are leaves.
            // Their offset entries are never read, so skip that whole interval.
            for(int i = 0; i <= last_branch; i++)
                tree.data[i + 1].offset = tree.data[i].offset + std::popcount(tree.data[i].mask);
            tree.data[n + 1].offset = tree.data[last_branch + 1].offset;
            for(int i = n + 1; i < size; i++)
                tree.data[i + 1].offset = tree.data[i].offset + std::popcount(tree.data[i].mask);
            auto scatter = [&](int i, uint32_t entry) {
                int p = links[i] & id_mask, x = unsigned(links[i]) >> 24;
                int rank = std::popcount(tree.data[p].mask & ((1U << x) - 1));
                children.data[tree.data[p].offset + rank] = entry;
            };
            // Leaves carry answers; clones carry ranges; internal prefixes also emit themselves.
            for(int i = 1; i <= last_branch; i++) scatter(i, uint32_t(i) | dense_tag);
            for(int i = last_branch + 1; i <= n; i++) scatter(i, n - i);
            for(int i = n + 1; i < size; i++)
                scatter(i, (uint32_t(std::popcount(tree.data[i].mask)) << 24) | tree.data[i].offset);
            checkpoint("tree");
            int top = 0, count = 0, cursor = tree.data[0].offset, end = tree.data[1].offset;
            while(true) {
                if(cursor == end) {
                    if(!top) break;
                    end = tree.data[--top].mask;
                    cursor = tree.data[--top].mask;
                    continue;
                }
                if(end - cursor >= 2) {
                    uint64_t pair;
                    std::memcpy(&pair, children.data + cursor, sizeof(pair));
                    if(!(pair & 0xFF000000FF000000ULL)) {
                        std::memcpy(links + count, &pair, sizeof(pair));
                        count += 2; cursor += 2;
                        continue;
                    }
                }
                uint32_t entry = children.data[cursor++];
                if(!(entry >> 24)) { links[count++] = entry; continue; }
                int begin, finish;
                if(entry & dense_tag) {
                    int u = entry & id_mask;
                    links[count++] = n - u;
                    begin = tree.data[u].offset;
                    finish = tree.data[u + 1].offset;
                } else {
                    begin = entry & id_mask;
                    finish = begin + (entry >> 24);
                }
                if(cursor != end) {
                    tree.data[top++].mask = cursor;
                    tree.data[top++].mask = end;
                }
                cursor = begin;
                end = finish;
            }
            checkpoint("dfs");
            link.resize(n);
            return std::move(link);
        }
    };

    inline auto suffix_array(std::string text) {
        std::ranges::reverse(text);
        return suffix_automaton(std::move(text), true).take_suffix_order();
    }
}

#line 1 "blazingio/blazingio.min.hpp"
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
#line 41 "blazingio/blazingio.min.hpp"
#include $a(<arm_neon.h>,<immintrin.h>)
#line 43 "blazingio/blazingio.min.hpp"
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
#line 10 "verify/string/longest_common_substring.test.cpp"
using namespace cp_algo::structures;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s, t;
    std::cin >> s >> t;
    bool swapped = s.size() > t.size();
    if(swapped) std::swap(s, t);
    auto [a, b, c, d] = suffix_automaton(std::move(s)).longest_common_substring(t);
    if(swapped) { std::swap(a, c); std::swap(b, d); }
    std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    cp_algo::checkpoint<1>();
}
