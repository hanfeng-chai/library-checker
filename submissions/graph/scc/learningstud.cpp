// Synthesized by "Claude Opus 5 Thinking" from multiple of my experiments and its own final optimizations.
#include <concepts>
#pragma GCC optimize "Ofast"
// no exception tables: nothing references libstdc++, so only libc is loaded at startup
#pragma GCC optimize "no-exceptions"
#pragma GCC target "avx2,bmi,bmi2"
#ifndef	DEBUG
#define	NDEBUG
#endif
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <immintrin.h>
#include <sys/uio.h>
#include <string_view>
#include <cstdlib>
#include <cerrno>
#include <utility>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace {

[[noreturn, gnu::cold, gnu::noinline]] void
fail() noexcept { std::abort(); }

[[nodiscard, gnu::always_inline]] inline std::string_view
read_file(const int fd) noexcept {
	struct ::stat stat;
	if (::fstat(fd, &stat) == -1) [[unlikely]] fail();
	const std::size_t len = stat.st_size;
	if (len == 0) [[unlikely]] fail();
#ifndef	MAP_POPULATE
#define	MAP_POPULATE MAP_FILE
#endif
	const auto s = ::mmap(nullptr, len, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	if (s == MAP_FAILED) [[unlikely]] fail();
	return std::string_view(static_cast<const char*>(s), len);
}

[[gnu::always_inline]] inline void
write_file(const int fd, const std::string_view s) noexcept {
	auto buf = s.data();
	auto nbyte = s.size();
	while (nbyte > 0) {
		const auto m = ::write(fd, buf, nbyte);
		if (m == -1) [[unlikely]] {
			if (errno == EINTR) continue;
			fail();
		}
		buf += m;
		nbyte -= m;
	}
}

[[gnu::always_inline]] inline void
populate(void *const address, const std::size_t length) noexcept {
	const auto first = reinterpret_cast<std::uintptr_t>(address) & -std::uintptr_t{4096};
	::madvise(reinterpret_cast<void *>(first), reinterpret_cast<std::uintptr_t>(address) + length - first, MADV_POPULATE_WRITE);
}

// Byte-at-a-time parse for the header and the last few bytes; never reads past last.
[[nodiscard, gnu::always_inline]] inline std::uint32_t
parse_unsigned(const char *&c, const char *const last) noexcept {
	std::uint32_t value = 0;
	for (; c != last && static_cast<unsigned char>(*c - '0') < 10; ++c) value = value * 10 + (*c - '0');
	if (c != last) ++c;
	return value;
}

// Bit i is set iff c[i] is a delimiter: subtracting '0' sets the high bit of every byte below '0'.
[[nodiscard, gnu::always_inline]] inline std::uint64_t
delimiter_mask(const char *const c) noexcept {
	const auto zero = _mm256_set1_epi8('0');
	const auto lo = _mm256_sub_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(c)), zero);
	const auto hi = _mm256_sub_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(c + 32)), zero);
	return std::uint64_t{static_cast<std::uint32_t>(_mm256_movemask_epi8(lo))}
	     ^ std::uint64_t{static_cast<std::uint32_t>(_mm256_movemask_epi8(hi))} << 32;
}

// pshufb controls right-aligning u (digit_count_u bytes at 0) into bytes 0..7 and v (digit_count_v
// bytes after the space) into bytes 8..15; 0x80 zeroes the leading bytes.
constexpr auto line_shuffle = [] constexpr noexcept {
	std::array<std::array<std::uint8_t, 16>, 8 * 8> table{};
	for (int ku = 0; ku != 8; ++ku) for (int kv = 0; kv != 8; ++kv) {
		auto &control = table[ku * 8 + kv];
		for (int j = 0; j != 16; ++j) control[j] = 0x80;
		for (int j = 0; j != ku; ++j) control[8 - ku + j] = j;
		for (int j = 0; j != kv; ++j) control[16 - kv + j] = ku + 1 + j;
	}
	return table;
}();

// Parses the line "u v" at c given both digit counts (each at most 6); loads 16 bytes at c.
// Both numbers go through one maddubs/madd/packus/madd chain as two 8-digit lanes.
[[nodiscard, gnu::always_inline]] inline std::pair<std::uint32_t, std::uint32_t>
parse_line(const char *const c, const std::uint32_t digit_count_u, const std::uint32_t digit_count_v) noexcept {
	assert(1 <= digit_count_u && digit_count_u <= 6);
	assert(1 <= digit_count_v && digit_count_v <= 6);
	const auto digits = _mm_sub_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i *>(c)), _mm_set1_epi8('0'));
	const auto control = _mm_loadu_si128(reinterpret_cast<const __m128i *>(line_shuffle[digit_count_u * 8 + digit_count_v].data()));
	const auto aligned = _mm_shuffle_epi8(digits, control);
	const auto pairs = _mm_maddubs_epi16(aligned, _mm_set1_epi16(0x010a));
	const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(0x0001'0064));
	const auto octets = _mm_madd_epi16(_mm_packus_epi32(quads, quads), _mm_set1_epi32(0x0001'2710));
	return {static_cast<std::uint32_t>(_mm_cvtsi128_si32(octets)), static_cast<std::uint32_t>(_mm_extract_epi32(octets, 1))};
}

// Writes ' ' followed by the digits of n so that the token ends at p; returns the token's start.
// Always stores the 8 bytes ending at p, so the 8 bytes before p must be writable.
[[nodiscard, gnu::always_inline]] inline char *
format_unsigned6_backward(char *const p, const std::uint32_t n, const char prefix) noexcept {
	static constexpr auto pair = [] constexpr noexcept {
		std::array<std::uint16_t, 100> table;
		for (unsigned i = 0; i != table.size(); ++i) {
			table[i] = ('0' + i / 10) << __CHAR_BIT__ * 0 ^ ('0' + i % 10) << __CHAR_BIT__ * 1;
		}
		return table;
	}();
	const auto digit_count = 1 + (n >= 10) + (n >= 100) + (n >= 1'000) + (n >= 10'000) + (n >= 100'000);
	auto swar = std::uint64_t{pair[n / 10'000]} << __CHAR_BIT__ * 2
	          ^ std::uint64_t{pair[n / 100 % 100]} << __CHAR_BIT__ * 4
	          ^ std::uint64_t{pair[n % 100]} << __CHAR_BIT__ * 6;           // "HHMMLL" in bytes 2..7
	swar &= ~std::uint64_t{0} << (6 - digit_count + 2) * __CHAR_BIT__;      // keep the last digit_count digits
	swar |= std::uint64_t{static_cast<unsigned char>(prefix)} << (7 - digit_count) * __CHAR_BIT__;
	std::memcpy(p - 8, &swar, sizeof(swar));
	return p - digit_count - 1;
}

/*
record (edge[i], 64 bits) = [ a : 21 | b : 21 | c : 21 | unused : 1 ]
	unexamined edge:  a = next edge,            b = target
	dfs frame:        a = next edge (iterator), b = previous frame, c = parent
	S node:           a = next node,            b = vertex
The record freed when a vertex finishes (its final frame) becomes its S node.

vertex[v] (64 bits)
	< opened:            unvisited; its first record inlined as (a = head, b = first target, c = next of head),
	                     so that descending needs no dependent load of edge[head]; 0: sink, no out-edges
	opened | i << 1 | r: open (active or on S); i = low link, r = root bit
	done:                assigned to an emitted component
done compares greater than every open value, so min(low) ignores it.
*/
constexpr int field_width = 21;
constexpr std::uint64_t field_mask = (1 << field_width) - 1;
using state_t = std::uint64_t;
constexpr state_t opened = state_t{1} << 63;
constexpr state_t done = ~state_t{0};

[[nodiscard, gnu::always_inline]] constexpr std::uint32_t
field_a(const std::uint64_t record) noexcept { return record & field_mask; }

[[nodiscard, gnu::always_inline]] constexpr std::uint32_t
field_b(const std::uint64_t record) noexcept { return record >> field_width & field_mask; }

[[nodiscard, gnu::always_inline]] constexpr std::uint32_t
field_c(const std::uint64_t record) noexcept { return record >> field_width * 2; }

[[nodiscard, gnu::always_inline]] constexpr std::uint64_t
pack(const std::uint32_t a, const std::uint32_t b, const std::uint32_t c = 0) noexcept {
	assert(a <= field_mask);
	assert(b <= field_mask);
	assert(c <= field_mask);
	return a ^ std::uint64_t{b} << field_width ^ std::uint64_t{c} << field_width * 2;
}


// Tarjan emits components in reverse topological order, so each component is formatted
// backwards as it pops, ending at `text`; the finished text reads in topological order.
// Returns the start of the text.
[[nodiscard, gnu::always_inline]] inline char *
strongly_connected_components(const std::uint32_t n, state_t *const vertex, std::uint64_t *const edge,
                              char *text, std::uint32_t &component_count) noexcept {
	state_t counter = opened;
	std::uint32_t stack = 0;
	component_count = 0;
	const auto emit_singleton = [&](const std::uint32_t v) {
		*--text = '\n';
		text = format_unsigned6_backward(text, v, ' ');
		*--text = '1';
		++component_count;
	};
	for (std::uint32_t r = 0; r != n; ++r) {
		if (const auto x = vertex[r]; x >= opened) continue;
		else if (x == 0) {
			vertex[r] = done;
			emit_singleton(r);
			continue;
		}
		for (auto u = r, v = r, frame = std::uint32_t{0};;) {
			if (const auto y = vertex[v]; y < opened) {
				if (y != 0) {
					vertex[v] = counter | 1;
					counter += 2;
					// y inlines v's first record, so edge[head] is only stored to, never loaded,
					// on the descent path; the second record is prefetched for the first slide
					const auto head = field_a(y);
					__builtin_prefetch(&edge[field_c(y)]);
					edge[head] = pack(field_c(y), frame, u);
					frame = head;
					u = v;
					v = field_b(y);
					continue;
				}
				vertex[v] = done;
				emit_singleton(v);
			} else if ((y | 1) < vertex[u]) {
				vertex[u] = y & ~state_t{1};
			}
			const auto f = edge[frame];
			if (const auto next = field_a(f)) {
				const auto e = edge[next];
				// look one sibling ahead: its record was prefetched by the previous slide (or descent);
				// prefetch its target's state and the record after it
				if (const auto after = field_a(e)) {
					const auto e2 = edge[after];
					__builtin_prefetch(&vertex[field_b(e2)]);
					__builtin_prefetch(&edge[field_a(e2)]);
				}
				edge[next] = (e & field_mask) ^ (f & ~field_mask);
				frame = next;
				v = field_b(e);
				continue;
			}
			if (const auto x = vertex[u]; x & 1) {
				const auto bound = x - 1;
				*--text = '\n';
				text = format_unsigned6_backward(text, u, ' ');
				std::uint32_t size = 1, s = stack;
				for (; s != 0; ++size) {
					const auto node = edge[s];
					const auto w = field_b(node);
					if (vertex[w] < bound) break;
					vertex[w] = done;
					text = format_unsigned6_backward(text, w, ' ');
					s = field_a(node);
				}
				text = format_unsigned6_backward(text, size, ' ') + 1;   // drop the leading space
				stack = s;
				vertex[u] = done;
				counter = bound;
				++component_count;
			} else {
				edge[frame] = pack(stack, u);
				stack = frame;
			}
			const auto parent = field_c(f);
			if (parent == u) break;
			v = u;
			u = parent;
			frame = field_b(f);
		}
	}
	return text;
}

}

[[gnu::no_stack_protector, gnu::hot]] int
main() {
	constexpr int max_size = 500'000;
	static_assert(max_size + 1 <= field_mask);
	static_assert((opened | state_t{max_size} << 1 | 1) < done - 1);

#ifndef	MADV_HUGEPAGE
#define	MADV_HUGEPAGE MADV_NORMAL
#else
	alignas(1 << 21)
#endif
	static constinit struct {
		state_t vertex[max_size]{}; // MUST be completely cleared for correctness
		std::uint64_t edge[max_size + 1]; // CAN be completely uninitialized
	} storage{};
	::madvise(&storage, sizeof(storage), MADV_HUGEPAGE);
#ifndef	MADV_POPULATE_WRITE
#define	MADV_POPULATE_WRITE MADV_NORMAL
#endif
	::madvise(&storage, sizeof(storage), MADV_POPULATE_WRITE);
	const auto input = read_file(STDIN_FILENO);
	assert(!input.empty());
	auto c = input.begin();
	const auto last = input.end();
	const int n = parse_unsigned(c, last);
	assert(1 <= n);
	assert(n <= max_size);
	const int m = parse_unsigned(c, last);
	assert(0 <= m);
	assert(m <= max_size);
	const auto vertex = storage.vertex;
	const auto edge = storage.edge;
	populate(vertex, n * sizeof(*vertex));
	populate(edge, (m + 1) * sizeof(*edge));
	const auto link = [&](const int i, const std::uint32_t u, const std::uint32_t v) {
		assert(u < static_cast<std::uint32_t>(n));
		assert(v < static_cast<std::uint32_t>(n));
		const auto old_head = field_a(vertex[u]);
		edge[i] = pack(old_head, v);
		vertex[u] = pack(i, v, old_head);
	};
	int i = 1;
	{
		// Line boundaries come from 64-byte delimiter masks, so no parse waits on the previous one,
		// and vertex[u] is prefetched `delay` edges before its read-modify-write in link.
		constexpr int delay = 16;
		std::uint32_t pending_u[delay], pending_v[delay];
		const auto push = [&](const std::uint32_t u, const std::uint32_t v) {
			__builtin_prefetch(&vertex[u], 1);
			const int slot = i % delay;
			if (i > delay) link(i - delay, pending_u[slot], pending_v[slot]);
			pending_u[slot] = u;
			pending_v[slot] = v;
			++i;
		};
		const char *line = c;          // start of the current line
		const char *space = nullptr;   // its space, if found in the previous block
		for (const char *p = c; i <= m && last - p >= 64 + 16; p += 64) {   // 16: parse_line's load
			auto mask = delimiter_mask(p);
			if (space != nullptr && mask != 0) {
				const char *const newline = p + std::countr_zero(mask);
				mask &= mask - 1;
				const auto [u, v] = parse_line(line, space - line, newline - space - 1);
				push(u, v);
				line = newline + 1;
				space = nullptr;
			}
			while ((mask & (mask - 1)) != 0 && i <= m) {
				const char *const blank = p + std::countr_zero(mask);
				mask &= mask - 1;
				const char *const newline = p + std::countr_zero(mask);
				mask &= mask - 1;
				const auto [u, v] = parse_line(line, blank - line, newline - blank - 1);
				push(u, v);
				line = newline + 1;
			}
			if (mask != 0 && space == nullptr) space = p + std::countr_zero(mask);
		}
		for (int k = i > delay ? i - delay : 1; k != i; ++k) link(k, pending_u[k % delay], pending_v[k % delay]);
		c = line;
	}
	for (; i <= m; ++i) {   // the last < 80 bytes
		const auto u = parse_unsigned(c, last);
		const auto v = parse_unsigned(c, last);
		link(i, u, v);
	}
	// 4'388'897: worst-case output (all singletons); 8 bytes of slack for the backward 8-byte stores
	constexpr std::size_t text_size = 8 + 4'388'897;
	alignas(1 << 21) static char text[text_size];
	::madvise(text, sizeof(text), MADV_HUGEPAGE);
	{
		// Prefault only the tail the text can reach. All singletons is the longest output:
		// "1 v\n" per vertex, plus the count line (<= 7 bytes) and 8 bytes of store slack.
		std::size_t needed = 8 + 7 + 3 * std::size_t(n) + n;                                    // + one digit each
		for (std::size_t power = 10; power < std::size_t(n); power *= 10) needed += n - power;   // + further digits
		populate(text + text_size - needed, needed);
	}
	std::uint32_t component_count;
	auto first = strongly_connected_components(n, vertex, edge, text + text_size, component_count);
	*--first = '\n';
	first = format_unsigned6_backward(first, component_count, ' ') + 1;
	{
		// stdout is a pipe: hand the pages to it instead of copying; text is never modified afterwards
		iovec iov{first, static_cast<std::size_t>(text + text_size - first)};
		while (iov.iov_len != 0) {
			const auto r = ::vmsplice(STDOUT_FILENO, &iov, 1, 0);
			if (r == -1) {
				write_file(STDOUT_FILENO, {static_cast<const char *>(iov.iov_base), iov.iov_len});
				break;
			}
			iov.iov_base = static_cast<char *>(iov.iov_base) + r;
			iov.iov_len -= r;
		}
	}
	::_exit(0);   // skip exit handlers
}
