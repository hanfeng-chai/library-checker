#line 1 ".verify-helper/cache/sam25b/shared_avx2.cpp"
#pragma GCC target("popcnt")
#pragma GCC optimize("O3,unroll-loops")
#define CP_ALGO_CHECKPOINT
#include <bits/stdc++.h>
#pragma GCC push_options
#pragma GCC target("avx2,bmi,bmi2,popcnt")
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

#line 8 ".verify-helper/cache/sam25b/shared_avx2.cpp"
#pragma GCC pop_options
using namespace std;
namespace FastIO
{
#define USE_FastIO
// ------------------------------
// #define DISABLE_MMAP
// ------------------------------
#if ( defined(LOCAL) || defined(_WIN32) ) && !defined(DISABLE_MMAP)
#define DISABLE_MMAP
#endif
#ifdef LOCAL
	inline void _chk_i() {}
	inline char _gc_nochk() { return getchar(); }
	inline char _gc() { return getchar(); }
	inline void _chk_o() {}
	inline void _pc_nochk(char c) { putchar(c); }
	inline void _pc(char c) { putchar(c); }
	template < int n > inline void _pnc_nochk(const char *c) { for ( int i = 0 ; i < n ; i++ ) putchar(c[i]); }
#else
#ifdef DISABLE_MMAP
	inline constexpr int _READ_SIZE = 1 << 18; inline static char _read_buffer[_READ_SIZE + 40], *_read_ptr = nullptr, *_read_ptr_end = nullptr; static inline bool _eof = false;
	inline void _chk_i() { if ( __builtin_expect(!_eof, true) && __builtin_expect(_read_ptr_end - _read_ptr < 40, false) ) { int sz = _read_ptr_end - _read_ptr; if ( sz ) memcpy(_read_buffer, _read_ptr, sz); char *beg = _read_buffer + sz; _read_ptr = _read_buffer, _read_ptr_end = beg + fread(beg, 1, _READ_SIZE, stdin); if ( __builtin_expect(_read_ptr_end != beg + _READ_SIZE, false) ) _eof = true, *_read_ptr_end = EOF; } }
	inline char _gc_nochk() { return __builtin_expect(_eof && _read_ptr == _read_ptr_end, false) ? EOF : *_read_ptr++; }
	inline char _gc() { _chk_i(); return _gc_nochk(); }
#else
#include<sys/mman.h>
#include<sys/stat.h>
	inline static char *_read_ptr = (char *)mmap(nullptr, [] { struct stat s; return fstat(0, &s), s.st_size; } (), 1, 2, 0, 0);
	inline void _chk_i() {}
	inline char _gc_nochk() { return *_read_ptr++; }
	inline char _gc() { return *_read_ptr++; }
#endif
	inline constexpr int _WRITE_SIZE = 1 << 18; inline static char _write_buffer[_WRITE_SIZE + 40], *_write_ptr = _write_buffer;
	inline void _chk_o() { if ( __builtin_expect(_write_ptr - _write_buffer > _WRITE_SIZE, false) ) fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout), _write_ptr = _write_buffer; }
	inline void _pc_nochk(char c) { *_write_ptr++ = c; }
	inline void _pc(char c) { *_write_ptr++ = c, _chk_o(); }
	template < int n > inline void _pnc_nochk(const char *c) { memcpy(_write_ptr, c, n), _write_ptr += n; }
	inline struct _auto_flush { inline ~_auto_flush() { fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout); } } _auto_flush;
#endif
#define println println_ // don't use C++23 std::println
	template < class T > inline constexpr bool _is_signed = numeric_limits < T >::is_signed;
	template < class T > inline constexpr bool _is_unsigned = numeric_limits < T >::is_integer && !_is_signed < T >;
#if __SIZEOF_LONG__ == 64
	template <> inline constexpr bool _is_signed < __int128 > = true;
	template <> inline constexpr bool _is_unsigned < __uint128_t > = true;
#endif
	inline bool _isgraph(char c) { return c >= 33; }
	inline bool _isdigit(char c) { return 48 <= c && c <= 57; } // or faster, remove c <= 57
	constexpr struct _table {
#ifndef LOCAL
	int i[65536];
#endif
	char o[40000]; constexpr _table() :
#ifndef LOCAL
	i{},
#endif
	o{} {
#ifndef LOCAL
	for ( int x = 0 ; x < 65536 ; x++ ) i[x] = -1; for ( int x = 0 ; x <= 9 ; x++ ) for ( int y = 0 ; y <= 9 ; y++ ) i[x + y * 256 + 12336] = x * 10 + y;
#endif
	for ( int x = 0 ; x < 10000 ; x++ ) for ( int y = 3, z = x ; ~y ; y-- ) o[x * 4 + y] = z % 10 + 48, z /= 10; } } _table;
	template < class T, int digit > inline constexpr T _pw10 = 10 * _pw10 < T, digit - 1 >;
	template < class T > inline constexpr T _pw10 < T, 0 > = 1;
	inline void read(char &c) { do c = _gc(); while ( !_isgraph(c) ); }
	inline void read_cstr(char *s) { char c = _gc(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) *s++ = c, c = _gc(); *s = 0; }
	inline void read(string &s) { char c = _gc(); s.clear(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) s.push_back(c), c = _gc(); }
	template < class T, bool neg >
#ifndef LOCAL
	__attribute__((no_sanitize("undefined")))
#endif
	inline void _read_int_suf(T &x) { _chk_i(); char c; while
#ifndef LOCAL
	( ~_table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)] ) if constexpr ( neg ) x = x * 100 - _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; else x = x * 100 + _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; if
#endif
	( _isdigit(c = _gc_nochk()) ) if constexpr ( neg ) x = x * 10 - ( c & 15 ); else x = x * 10 + ( c & 15 ); }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ) if ( c == 45 ) { _read_int_suf < T, true >(x = -( _gc_nochk() & 15 )); return; } _read_int_suf < T, false >(x = c & 15); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ); _read_int_suf < T, false >(x = c & 15); }
	inline void write(bool x) { _pc(x | 48); }
	inline void write(char c) { _pc(c); }
	inline void write_cstr(const char *s) { while ( *s ) _pc(*s++); }
	inline void write(const string &s) { for ( char c : s ) _pc(c); }
	template < class T, bool neg, int digit > inline void _write_int_suf(T x) { if constexpr ( digit == 4 ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _write_int_suf < T, neg, digit / 2 >(x / _pw10 < T, digit / 2 >), _write_int_suf < T, neg, digit / 2 >(x % _pw10 < T, digit / 2 >); }
	template < class T, bool neg, int digit > inline void _write_int_pre(T x) { if constexpr ( digit <= 4 ) if ( digit >= 3 && ( neg ? x <= -100 : x >= 100 ) ) if ( digit >= 4 && ( neg ? x <= -1000 : x >= 1000 ) ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _pnc_nochk < 3 >(_table.o + ( neg ? -x : x ) * 4 + 1); else if ( digit >= 2 && ( neg ? x <= -10 : x >= 10 ) ) _pnc_nochk < 2 >(_table.o + ( neg ? -x : x ) * 4 + 2); else _pc_nochk(( neg ? -x : x ) | 48); else { constexpr int cur = 1 << __lg(digit - 1); if ( neg ? x <= -_pw10 < T, cur > : x >= _pw10 < T, cur > ) _write_int_pre < T, neg, digit - cur >(x / _pw10 < T, cur >), _write_int_suf < T, neg, cur >(x % _pw10 < T, cur >); else _write_int_pre < T, neg, cur >(x); } }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void write(T x) { if ( x >= 0 ) _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x); else _pc_nochk(45), _write_int_pre < T, true, numeric_limits < T >::digits10 + 1 >(x); _chk_o(); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void write(T x) { _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x), _chk_o(); }
	template < size_t N, class ...T > inline void _read_tuple(tuple < T... > &x) { read(get < N >(x)); if constexpr ( N + 1 != sizeof...(T) ) _read_tuple < N + 1, T... >(x); }
	template < size_t N, class ...T > inline void _write_tuple(const tuple < T... > &x) { write(get < N >(x)); if constexpr ( N + 1 != sizeof...(T) ) _pc(32), _write_tuple < N + 1, T... >(x); }
	template < class ...T > inline void read(tuple < T... > &x) { _read_tuple < 0, T... >(x); }
	template < class ...T > inline void write(const tuple < T... > &x) { _write_tuple < 0, T... >(x); }
	template < class T1, class T2 > inline void read(pair < T1, T2 > &x) { read(x.first), read(x.second); }
	template < class T1, class T2 > inline void write(const pair < T1, T2 > &x) { write(x.first), _pc(32), write(x.second); }
	template < class T > inline auto read(T &x) -> decltype(x.read(), void()) { x.read(); }
	template < class T > inline auto write(const T &x) -> decltype(x.write(), void()) { x.write(); }
	template < class T1, class ...T2 > inline void read(T1 &x, T2 &...y) { read(x), read(y...); }
	template < class ...T > inline void read_cstr(char *x, T *...y) { read_cstr(x), read_cstr(y...); }
	template < class T1, class ...T2 > inline void write(const T1 &x, const T2 &...y) { write(x), write(y...); }
	template < class ...T > inline void write_cstr(const char *x, const T *...y) { write_cstr(x), write_cstr(y...); }
	template < class T > inline void print(const T &x) { write(x); }
	inline void print_cstr(const char *x) { write_cstr(x); }
	template < class T1, class ...T2 > inline void print(const T1 &x, const T2 &...y) { write(x), _pc(32), print(y...); }
	template < class ...T > inline void print_cstr(const char *x, const T *...y) { write_cstr(x), _pc(32), print_cstr(y...); }
	inline void println() { _pc(10); }
	inline void println_cstr() { _pc(10); }
	template < class ...T > inline void println(const T &...x) { print(x...), _pc(10); }
	template < class ...T > inline void println_cstr(const T *...x) { print_cstr(x...), _pc(10); }
}	using FastIO::read, FastIO::read_cstr, FastIO::write, FastIO::write_cstr, FastIO::println, FastIO::println_cstr;
int main() {
    static char input[500009];
    read_cstr(input);
    auto answer = cp_algo::structures::suffix_array(std::string(input));
    for(int i: answer) write(i, ' ');
    println();
    cp_algo::checkpoint("write");
    cp_algo::checkpoint<1>();
}
