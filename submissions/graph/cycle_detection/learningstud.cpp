// Synthesized by "Claude Opus 5 Thinking" from submission 404896 and exploiting long cycle.
#include <concepts>
#pragma GCC optimize "Ofast"
#pragma GCC optimize "no-exceptions" // keep beside Ofast; nothing then needs libstdc++
#pragma GCC target "avx2,bmi,bmi2"
#ifndef	DEBUG
#define	NDEBUG
#endif
#include <array>
#include <bit>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <immintrin.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/uio.h>

namespace {

constexpr auto operator""_Ki(unsigned long long nbyte) noexcept {return nbyte << 10;}
constexpr auto operator""_Mi(unsigned long long nbyte) noexcept {return nbyte << 20;}

#ifdef	ONLINE_JUDGE
// AMD EPYC 7B13 is proprietary OEM for GCP which is closest to
// https://www.amd.com/en/products/processors/server/epyc/7003-series/amd-epyc-7713.html
// https://www.reddit.com/r/LocalLLaMA/comments/1mjv9r8/comment/n7dz3y8/
constexpr std::size_t level1_icache_size	= 32_Ki;	// _SC_LEVEL1_ICACHE_SIZE
constexpr std::size_t level1_icache_assoc	= 8;		// _SC_LEVEL1_ICACHE_ASSOC
constexpr std::size_t level1_icache_linesize	= 64;		// _SC_LEVEL1_ICACHE_LINESIZE
constexpr std::size_t level1_dcache_size	= 32_Ki;	// _SC_LEVEL1_DCACHE_SIZE
constexpr std::size_t level1_dcache_assoc	= 8;		// _SC_LEVEL1_DCACHE_ASSOC
constexpr std::size_t level1_dcache_linesize	= 64;		// _SC_LEVEL1_DCACHE_LINESIZE
constexpr std::size_t level2_cache_size		= 512_Ki;	// _SC_LEVEL2_CACHE_SIZE
constexpr std::size_t level2_cache_assoc	= 8;		// _SC_LEVEL2_CACHE_ASSOC
constexpr std::size_t level2_cache_linesize	= 64;		// _SC_LEVEL2_CACHE_LINESIZE
constexpr std::size_t level3_cache_size		= 32_Mi;	// _SC_LEVEL3_CACHE_SIZE
constexpr std::size_t level3_cache_assoc	= 16;		// _SC_LEVEL3_CACHE_ASSOC
constexpr std::size_t level3_cache_linesize	= 64;		// _SC_LEVEL3_CACHE_LINESIZE
#endif

// false if a and a + distance share a cache set (for every a) in a cache of this geometry
[[nodiscard]] constexpr bool
set_disjoint(const std::size_t distance, const std::size_t size, const std::size_t assoc, const std::size_t linesize) noexcept {
	const auto stride = size / assoc, offset = distance % stride;
	return linesize <= offset && offset <= stride - linesize;
}

[[nodiscard, gnu::always_inline]] inline std::pair<const char *, const char *>
read_file(const int fd) noexcept {
	struct ::stat stat;
	if (::fstat(fd, &stat) == -1 || stat.st_size == 0) [[unlikely]] std::abort();
#ifndef	MAP_POPULATE
#define	MAP_POPULATE MAP_FILE
#endif
	const auto s = ::mmap(nullptr, stat.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	if (s == MAP_FAILED) [[unlikely]] std::abort();
	return {static_cast<const char *>(s), static_cast<const char *>(s) + stat.st_size};
}

// vmsplice when stdout is a pipe; the buffer must not change afterwards
[[gnu::always_inline]] inline void
write_file(const int fd, const char *const buf, const std::size_t nbyte) noexcept {
	for (iovec iov{const_cast<char *>(buf), nbyte}; iov.iov_len != 0;) {
		auto m = ::vmsplice(fd, &iov, 1, 0);
		if (m == -1) m = ::write(fd, iov.iov_base, iov.iov_len);
		if (m == -1) [[unlikely]] {
			if (errno == EINTR) continue;
			std::abort();
		}
		iov.iov_base = static_cast<char *>(iov.iov_base) + m;
		iov.iov_len -= m;
	}
}

[[nodiscard, gnu::always_inline]] inline int
parse_unsigned(const char *&c, const char *const last) noexcept {
	int value = 0;
	for (; c != last && '0' <= *c; ++c) value = value * 10 + (*c - '0');
	if (c != last) ++c;
	return value;
}

// bit i: c[i] < '0'
[[nodiscard, gnu::always_inline]] inline std::uint64_t
delimiter_mask(const char *const c) noexcept {
	const auto zero = _mm256_set1_epi8('0');
	const std::uint32_t lo = _mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(c)), zero));
	const std::uint32_t hi = _mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(c + 32)), zero));
	return lo ^ std::uint64_t{hi} << 32;
}

// "u v" with u, v < 10^6 right-aligned into two 8-digit lanes, then converted together
[[nodiscard, gnu::always_inline]] inline std::pair<int, int>
parse_line(const char *const c, const int u_digit_count, const int v_digit_count) noexcept {
	static constexpr auto shuffle = [] constexpr noexcept {
		std::array<std::array<char, 16>, 8 * 8> table;
		for (int ku = 0; ku != 8; ++ku) for (int kv = 0; kv != 8; ++kv) {
			auto &control = table[ku * 8 + kv];
			control.fill(0x80);
			for (int j = 0; j != ku; ++j) control[8 - ku + j] = j;
			for (int j = 0; j != kv; ++j) control[16 - kv + j] = ku + 1 + j;
		}
		return table;
	}();
	assert(1 <= u_digit_count && u_digit_count <= 6);
	assert(1 <= v_digit_count && v_digit_count <= 6);
	auto a = _mm_sub_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i *>(c)), _mm_set1_epi8('0'));
	a = _mm_shuffle_epi8(a, _mm_loadu_si128(reinterpret_cast<const __m128i *>(&shuffle[u_digit_count * 8 + v_digit_count])));
	a = _mm_maddubs_epi16(a, _mm_set1_epi16(0x010a));
	a = _mm_madd_epi16(a, _mm_set1_epi32(0x0001'0064));
	a = _mm_madd_epi16(_mm_packus_epi32(a, a), _mm_set1_epi32(0x0001'2710));
	return {_mm_cvtsi128_si32(a), _mm_extract_epi32(a, 1)};
}

// writes the digits of n (< 10^6) and then delimiter, ending at c; stores the 8 bytes before c
[[nodiscard, gnu::always_inline]] inline char *
format_unsigned6(char *const c, const char delimiter, const unsigned n) noexcept {
	static constexpr auto table = [] constexpr noexcept {
		std::array<std::uint16_t, 100> table;
		for (unsigned i = 0; i != table.size(); ++i) table[i] = ('0' + i / 10) ^ ('0' + i % 10) << __CHAR_BIT__;
		return table;
	}();
	assert(n < 1'000'000);
	const int digit_count = 1 + (n >= 10) + (n >= 100) + (n >= 1'000) + (n >= 10'000) + (n >= 100'000);
	auto swar = std::uint64_t{table[n / 10'000]} << __CHAR_BIT__ * 1
	          ^ std::uint64_t{table[n / 100 % 100]} << __CHAR_BIT__ * 3
	          ^ std::uint64_t{table[n % 100]} << __CHAR_BIT__ * 5;
	swar &= ~std::uint64_t{0} << (7 - digit_count) * __CHAR_BIT__;
	swar ^= std::uint64_t{static_cast<unsigned char>(delimiter)} << __CHAR_BIT__ * 7;
	std::memcpy(c - 8, &swar, sizeof(swar));
	return c - digit_count - 1;
}

/*
edge[i] = a | b << 21
	a = next edge, b = target
vertex[v]
	0:         no out-edge, or finished
	< active:  unvisited, first edge inlined as {a = edge, b = its target, c = its next}
	active:    on the DFS path
path[k] = a | b << 21 | c << 42
	a = next edge of c to explore, b = edge taken from c, c = the k-th vertex of the DFS path
*/
constexpr int width = 21;
constexpr std::uint64_t mask = (1 << width) - 1;
constexpr std::uint64_t active = ~std::uint64_t{0};

[[nodiscard, gnu::always_inline]] constexpr unsigned
a(const std::uint64_t e) noexcept {return e & mask;}
[[nodiscard, gnu::always_inline]] constexpr unsigned
b(const std::uint64_t e) noexcept {return e >> width & mask;}
[[nodiscard, gnu::always_inline]] constexpr unsigned
c(const std::uint64_t e) noexcept {return e >> width * 2;}
[[nodiscard, gnu::always_inline]] constexpr std::uint64_t
pack(const std::uint64_t a, const std::uint64_t b, const std::uint64_t c = 0) noexcept {
	assert(a <= mask);
	assert(b <= mask);
	assert(c <= mask);
	return a ^ b << width ^ c << width * 2;
}

// formats the output backwards ending at text (only path[] is read meanwhile):
// a back edge u → v closes the cycle of the path from v to u
[[nodiscard, gnu::always_inline]] inline char *
cycle_detection(const unsigned n, std::uint64_t *const vertex, const std::uint64_t *const edge, std::uint64_t *const path, char *text) noexcept {
	auto top = path; // back at path after every tree
	for (unsigned i = 0; i != n; ++i) for (auto v = i, u = v;;) {
		if (const auto y = vertex[v]; y == active) {
			unsigned cycle_length = 0;
			for (;; --top) {
				text = format_unsigned6(text, '\n', b(*top) - 1);
				++cycle_length;
				if (c(*top) == v) return format_unsigned6(text, '\n', cycle_length);
			}
		} else if (y != 0) {
			vertex[v] = active;
			__builtin_prefetch(&edge[c(y)]);
			*++top = pack(c(y), a(y), v);
			u = std::exchange(v, b(y));
			continue;
		}
		if (top == path) break;
		if (const auto next = a(*top)) {
			const auto e = edge[next];
			if (a(e)) {
				const auto e2 = edge[a(e)];
				__builtin_prefetch(&vertex[b(e2)]);
				__builtin_prefetch(&edge[a(e2)]);
			}
			*top = pack(a(e), next, u);
			v = b(e);
			continue;
		}
		vertex[u] = 0;
		v = u;
		u = c(*--top);
	}
	return static_cast<char *>(std::memcpy(text - 3, "-1\n", 3));
}

}

[[gnu::no_stack_protector, gnu::hot]] int
main() {
	constexpr int max_size = 500'000;
	static_assert(max_size < mask);
#ifndef	MADV_HUGEPAGE
#define	MADV_HUGEPAGE MADV_NORMAL
#define	huge
#else
#define	huge alignas(1 << 21)
#endif
#ifndef	MADV_POPULATE_WRITE
#define	MADV_POPULATE_WRITE MADV_NORMAL
#endif
	huge static constinit struct {
		std::uint64_t vertex[max_size]{}; // MUST be completely cleared for correctness
		std::uint64_t edge[max_size + 1]; // CAN be completely uninitialized; offset from vertex avoids 4K aliasing
		std::uint64_t path[max_size + 1]; // CAN be completely uninitialized
	} storage{};
#ifdef	ONLINE_JUDGE
	// vertex[i] and edge[i] are touched together: keep them out of each other's L1/L2 sets,
	// and the whole working set, input included, within L3
	static_assert(set_disjoint(sizeof(storage.vertex), level1_dcache_size, level1_dcache_assoc, level1_dcache_linesize));
	static_assert(set_disjoint(sizeof(storage.vertex), level2_cache_size, level2_cache_assoc, level2_cache_linesize));
	static_assert(sizeof(storage) + 14 * max_size <= level3_cache_size);
#endif
	const auto vertex = storage.vertex;
	const auto edge = storage.edge;
	::madvise(&storage, sizeof(storage), MADV_HUGEPAGE);
	::madvise(&storage, sizeof(storage), MADV_POPULATE_WRITE);
	auto [c, last] = read_file(STDIN_FILENO);
	const auto n = parse_unsigned(c, last);
	assert(1 <= n);
	assert(n <= max_size);
	const auto m = parse_unsigned(c, last);
	assert(0 <= m);
	assert(m <= max_size);

	// edge[i] = {u, v} until linked; linking lags parsing so that vertex[u] is prefetched first
	constexpr int delay = 16;
	const auto link = [&](const int i) noexcept {
		const auto u = a(edge[i]), v = b(edge[i]);
		assert(u < unsigned(n));
		assert(v < unsigned(n));
		const auto head = a(vertex[u]);
		edge[i] = pack(head, v);
		vertex[u] = pack(i, v, head);
	};
	int i = 1;
	const auto push = [&](const int u, const int v) noexcept {
		__builtin_prefetch(&vertex[u], 1);
		edge[i] = pack(u, v);
		if (i > delay) link(i - delay);
		++i;
	};
	while (i <= m && last - c >= 64 + 16) {
		const auto block = c; // starts at a line; 64 bytes hold at least 4 whole lines
		for (auto d = delimiter_mask(block); (d & (d - 1)) != 0 && i <= m; d &= d - 1, d &= d - 1) {
			const auto space = block + std::countr_zero(d), newline = block + std::countr_zero(d & (d - 1));
			const auto [u, v] = parse_line(c, space - c, newline - space - 1);
			push(u, v);
			c = newline + 1;
		}
	}
	while (i <= m) {
		const auto u = parse_unsigned(c, last);
		push(u, parse_unsigned(c, last));
	}
	for (int k = i > delay ? i - delay : 1; k != i; ++k) link(k);

	// the output is formatted into vertex[], prefaulted and dead once a cycle is found
	static_assert(8 + 7 * (max_size + 1) <= sizeof(storage.vertex));
	const auto text = reinterpret_cast<char *>(storage.edge);
	const auto first = cycle_detection(n, vertex, edge, storage.path, text);
	write_file(STDOUT_FILENO, first, text - first);
	::_exit(0);
}
