#pragma GCC target("sse3,ssse3,sse4.1,sse4.2,lzcnt,popcnt")
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>

static const __m128i fast_in_uint32_dec_align[] = {
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1, -1, -1, /**/ -1, -1, -1, -1, -1, -1, -1, -1},
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1, -1, -1, /**/ -1, -1, -1, -1, -1, -1, -1,  0},
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1, -1,  0, /**/ -1, -1, -1, -1, -1, -1, -1,  1},
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1, -1,  1, /**/ -1, -1, -1, -1, -1, -1,  0,  2},
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1,  0,  2, /**/ -1, -1, -1, -1, -1, -1,  1,  3},
	(__m128i)__v16qi{-1, -1, -1, -1, -1, -1,  1,  3, /**/ -1, -1, -1, -1, -1,  0,  2,  4},
	(__m128i)__v16qi{-1, -1, -1, -1, -1,  0,  2,  4, /**/ -1, -1, -1, -1, -1,  1,  3,  5},
	(__m128i)__v16qi{-1, -1, -1, -1, -1,  1,  3,  5, /**/ -1, -1, -1, -1,  0,  2,  4,  6},
	(__m128i)__v16qi{-1, -1, -1, -1,  0,  2,  4,  6, /**/ -1, -1, -1, -1,  1,  3,  5,  7},
	(__m128i)__v16qi{-1, -1, -1, -1,  1,  3,  5,  7, /**/ -1, -1, -1,  0,  2,  4,  6,  8},
	(__m128i)__v16qi{-1, -1, -1,  0,  2,  4,  6,  8, /**/ -1, -1, -1,  1,  3,  5,  7,  9},
};

struct fast_in {
	static constexpr size_t page_size = 1 << 12;

	const char *buf, *end;
	fast_in() noexcept {
		struct stat st;
		int r = fstat(0, &st);
		if(r < 0) abort();
		size_t size = st.st_size;
		size += (-size) & (page_size-1);

		buf = (char*)mmap(NULL, size + page_size, PROT_READ, MAP_SHARED | MAP_POPULATE, 0, 0);
		if(buf == MAP_FAILED) abort();
		end = buf + size;

		void *guard = mmap((char*)end, page_size, PROT_READ, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
		if(guard == MAP_FAILED) abort();
	}

	[[noreturn]] inline static void invalid() noexcept {
		__builtin_unreachable();
	}

	inline char chr() noexcept {
		return *buf++;
	}

	inline void skip(char c) noexcept {
		if(*buf++ != c) {
			invalid();
		}
	}

	inline uint32_t uint32_dec() noexcept {
		__m128i data = _mm_sub_epi8(_mm_loadu_si128((__m128i*)buf), _mm_set1_epi32(0x30303030));
		__m128i not_digit = _mm_cmpeq_epi8(data, _mm_max_epu8(_mm_set1_epi32(0x0a0a0a0a), data));
		int not_digit_mask = _mm_movemask_epi8(not_digit);
		if((not_digit_mask & 2047) == 0) invalid();
		size_t ndig = __builtin_ctz(not_digit_mask);
		if(ndig == 0) invalid();
		if(ndig > 10) __builtin_unreachable();
		buf += ndig;
		data = _mm_shuffle_epi8(data, fast_in_uint32_dec_align[ndig]);
		uint64_t x = _mm_cvtsi128_si64(data);
		uint64_t y = _mm_extract_epi64(data, 1);
		x = 10 * x + y;
		x = (x & 0xFF00FF00FF00FF) * 100 + ((x >> 8) & 0xFF00FF00FF00FF); 
		x = (x & 0xFFFF0000FFFF) * 10000 + ((x >> 16) & 0xFFFF0000FFFF); 
		return (uint32_t)(x >> 32) + 100000000 * (uint32_t)x;
	}
};

static const char fast_out_num100[100][2] = {{48,48},{48,49},{48,50},{48,51},{48,52},{48,53},{48,54},{48,55},{48,56},{48,57},{49,48},{49,49},{49,50},{49,51},{49,52},{49,53},{49,54},{49,55},{49,56},{49,57},{50,48},{50,49},{50,50},{50,51},{50,52},{50,53},{50,54},{50,55},{50,56},{50,57},{51,48},{51,49},{51,50},{51,51},{51,52},{51,53},{51,54},{51,55},{51,56},{51,57},{52,48},{52,49},{52,50},{52,51},{52,52},{52,53},{52,54},{52,55},{52,56},{52,57},{53,48},{53,49},{53,50},{53,51},{53,52},{53,53},{53,54},{53,55},{53,56},{53,57},{54,48},{54,49},{54,50},{54,51},{54,52},{54,53},{54,54},{54,55},{54,56},{54,57},{55,48},{55,49},{55,50},{55,51},{55,52},{55,53},{55,54},{55,55},{55,56},{55,57},{56,48},{56,49},{56,50},{56,51},{56,52},{56,53},{56,54},{56,55},{56,56},{56,57},{57,48},{57,49},{57,50},{57,51},{57,52},{57,53},{57,54},{57,55},{57,56},{57,57}};

struct fast_out {
	static constexpr size_t buf_size = 1 << 16;

	size_t buf_len;
	char buf[buf_size];

	void flush(size_t limit = 0) noexcept {
		size_t at = 0;
		size_t l = buf_len;
		while(l > limit) {
			ssize_t r = write(1, buf + at, l);
			if(r <= 0) abort();
			at += (size_t)r;
			l -= (size_t)r;
		}
		memcpy(buf, buf + at, l);
		buf_len = l;
	}

	inline void ensure(size_t cap) noexcept {
		if(buf_len + cap <= buf_size) return;
		flush(buf_size / 2);
	}

	inline void chr(char c) noexcept {
		buf[buf_len++] = c;
	}

	inline void uint32_dec(uint32_t v) noexcept {
		size_t i = buf_len;
		uint32_t hi;
		if(v == 0) goto do_d1;
		switch(__builtin_clz(v)) {
			case 0:
			case 1:
				goto do_d10;
			case 2:
				if(v >= 1000000000) goto do_d10;
				goto do_d9;
			case 3:
			case 4:
				goto do_d9;
			case 5:
				if(v >= 100000000) goto do_d9;
				goto do_d8;
			case 6:
			case 7:
				goto do_d8;
			case 8:
				if(v >= 10000000) goto do_d8;
				goto do_d7;
			case 9:
			case 10:
			case 11:
				goto do_d7;
			case 12:
				if(v >= 1000000) goto do_d7;
				goto do_d6;
			case 13:
			case 14:
				goto do_d6;
			case 15:
				if(v >= 100000) goto do_d6;
				goto do_d5;
			case 16:
			case 17:
				goto do_d5;
			case 18:
				if(v >= 10000) goto do_d5;
				goto do_d4;
			case 19:
			case 20:
			case 21:
				goto do_d4;
			case 22:
				if(v >= 1000) goto do_d4;
				goto do_d3;
			case 23:
			case 24:
				goto do_d3;
			case 25:
				if(v >= 100) goto do_d3;
				goto do_d2;
			case 26:
			case 27:
				goto do_d2;
			case 28:
				if(v >= 10) goto do_d2;
				goto do_d1;
			case 29:
			case 30:
			case 31:
				goto do_d1;
			default:
				__builtin_unreachable();
		}

do_d10:
		hi = v / 100000000;
		v %= 100000000;
		buf[i++] = fast_out_num100[hi][0];
		buf[i++] = fast_out_num100[hi][1];
do_d8:
		if(v >= 100000000) __builtin_unreachable();
		hi = v / 1000000;
		v %= 1000000;
		buf[i++] = fast_out_num100[hi][0];
		buf[i++] = fast_out_num100[hi][1];
do_d6:
		if(v >= 1000000) __builtin_unreachable();
		hi = v / 10000;
		v %= 10000;
		buf[i++] = fast_out_num100[hi][0];
		buf[i++] = fast_out_num100[hi][1];
do_d4:
		if(v >= 10000) __builtin_unreachable();
		hi = v / 100;
		v %= 100;
		buf[i++] = fast_out_num100[hi][0];
		buf[i++] = fast_out_num100[hi][1];
do_d2:
		if(v >= 100) __builtin_unreachable();
		buf[i++] = fast_out_num100[v][0];
		buf[i++] = fast_out_num100[v][1];
		buf_len = i;
		return;
do_d9:
		if(v >= 1000000000) __builtin_unreachable();
		hi = v / 100000000;
		v %= 100000000;
		buf[i++] = '0' + hi;
		goto do_d8;
do_d7:
		if(v >= 10000000) __builtin_unreachable();
		hi = v / 1000000;
		v %= 1000000;
		buf[i++] = '0' + hi;
		goto do_d6;
do_d5:
		if(v >= 100000) __builtin_unreachable();
		hi = v / 10000;
		v %= 10000;
		buf[i++] = '0' + hi;
		goto do_d4;
do_d3:
		if(v >= 1000) __builtin_unreachable();
		hi = v / 100;
		v %= 100;
		buf[i++] = '0' + hi;
		goto do_d2;
do_d1:
		if(v >= 10) __builtin_unreachable();
		buf[i++] = '0' + v;
		buf_len = i;
		return;
	}
};

static fast_in inp;
static fast_out out;

struct alignas(64) leaf {
	union {
		uint64_t v64[8];
		__m128i v128[4];
	};
	inline bool is_empty() const noexcept {
		__m128i r = _mm_or_si128(_mm_or_si128(v128[0], v128[1]), _mm_or_si128(v128[2], v128[3]));
		return _mm_test_all_zeros(r, r);
	}
};

static uint32_t leaf_alloc;

static uint32_t mask_l0;
static uint64_t mask_l1[8];
static uint64_t mask_l2[8 * 64];
static uint64_t mask_l3[8 * 64 * 64];
static uint32_t ind_l3[8 * 64 * 64 * 64];

static leaf leaves[1 << 20];

inline static void n_ins(uint32_t val) {
	uint32_t i = ind_l3[val >> 9];
	if(i == 0) {
		ind_l3[val >> 9] = ((uint32_t)1 << 31) | (val & 511);
	} else if((int32_t)i < 0) {
		uint32_t val1 = (uint32_t)i ^ ((uint32_t)1 << 31);
		if(val1 >= 512) __builtin_unreachable();
		if(val1 == (val & 511)) return;
		i = ++leaf_alloc;
		ind_l3[val >> 9] = i;
		leaves[i].v64[(val >> 6) & 7] = (uint64_t)1 << (val & 63);
		leaves[i].v64[(val1 >> 6) & 7] |= (uint64_t)1 << (val1 & 63);
	} else {
		bool m = leaves[i].is_empty();
		leaves[i].v64[(val >> 6) & 7] |= (uint64_t)1 << (val & 63);
		if(!m) return;
	}

	mask_l0 |= (uint32_t)1 << (val >> 27);
	mask_l1[val >> 27] |= (uint64_t)1 << ((val >> 21) & 63);
	mask_l2[val >> 21] |= (uint64_t)1 << ((val >> 15) & 63);
	mask_l3[val >> 15] |= (uint64_t)1 << ((val >> 9) & 63);
}

inline static void n_del(uint32_t val) {
	uint32_t i = ind_l3[val >> 9];
	if(i == 0) return;

	if((int32_t)i < 0) {
		i ^= (uint32_t)1 << 31;
		if(i != (val & 511)) return;
		ind_l3[val >> 9] = 0;
	} else {
		uint64_t m = (uint64_t)1 << (val & 63);
		uint64_t &v = leaves[i].v64[(val >> 6) & 7];
		if(!(v & m)) return;
		v ^= m;
		if(!leaves[i].is_empty()) return;
	}
	{
		uint64_t m = (uint64_t)1 << ((val >> 9) & 63);
		uint64_t &v = mask_l3[val >> 15];
		if(!(v & m)) __builtin_unreachable();
		v ^= m;
		if(v) return;
	}
	{
		uint64_t m = (uint64_t)1 << ((val >> 15) & 63);
		uint64_t &v = mask_l2[val >> 21];
		if(!(v & m)) __builtin_unreachable();
		v ^= m;
		if(v) return;
	}
	{
		uint64_t m = (uint64_t)1 << ((val >> 21) & 63);
		uint64_t &v = mask_l1[val >> 27];
		if(!(v & m)) __builtin_unreachable();
		v ^= m;
		if(v) return;
	}
	uint32_t m = (uint32_t)1 << (val >> 27);
	if(!(mask_l0 & m)) __builtin_unreachable();
	mask_l0 ^= m;
}

static const uint64_t q3tab[256] = {
0, 0x0000000000000000, 0x0101010101010101, 0x0100010001000100, 0x0202020202020202, 0x0202000002020000, 0x0202010102020101, 0x0202010002020100, 0x0303030303030303, 0x0303000003030000, 0x0303010103030101, 0x0303010003030100, 0x0302030203020302, 0x0302000003020000, 0x0302010103020101, 0x0302010003020100, 0x0404040404040404, 0x0404040400000000, 0x0404040401010101, 0x0404040401000100, 0x0404040402020202, 0x0404040402020000, 0x0404040402020101, 0x0404040402020100, 0x0404040403030303, 0x0404040403030000, 0x0404040403030101, 0x0404040403030100, 0x0404040403020302, 0x0404040403020000, 0x0404040403020101, 0x0404040403020100, 0x0505050505050505, 0x0505050500000000, 0x0505050501010101, 0x0505050501000100, 0x0505050502020202, 0x0505050502020000, 0x0505050502020101, 0x0505050502020100, 0x0505050503030303, 0x0505050503030000, 0x0505050503030101, 0x0505050503030100, 0x0505050503020302, 0x0505050503020000, 0x0505050503020101, 0x0505050503020100, 0x0504050405040504, 0x0504050400000000, 0x0504050401010101, 0x0504050401000100, 0x0504050402020202, 0x0504050402020000, 0x0504050402020101, 0x0504050402020100, 0x0504050403030303, 0x0504050403030000, 0x0504050403030101, 0x0504050403030100, 0x0504050403020302, 0x0504050403020000, 0x0504050403020101, 0x0504050403020100, 0x0606060606060606, 0x0606060600000000, 0x0606060601010101, 0x0606060601000100, 0x0606060602020202, 0x0606060602020000, 0x0606060602020101, 0x0606060602020100, 0x0606060603030303, 0x0606060603030000, 0x0606060603030101, 0x0606060603030100, 0x0606060603020302, 0x0606060603020000, 0x0606060603020101, 0x0606060603020100, 0x0606040406060404, 0x0606040400000000, 0x0606040401010101, 0x0606040401000100, 0x0606040402020202, 0x0606040402020000, 0x0606040402020101, 0x0606040402020100, 0x0606040403030303, 0x0606040403030000, 0x0606040403030101, 0x0606040403030100, 0x0606040403020302, 0x0606040403020000, 0x0606040403020101, 0x0606040403020100, 0x0606050506060505, 0x0606050500000000, 0x0606050501010101, 0x0606050501000100, 0x0606050502020202, 0x0606050502020000, 0x0606050502020101, 0x0606050502020100, 0x0606050503030303, 0x0606050503030000, 0x0606050503030101, 0x0606050503030100, 0x0606050503020302, 0x0606050503020000, 0x0606050503020101, 0x0606050503020100, 0x0606050406060504, 0x0606050400000000, 0x0606050401010101, 0x0606050401000100, 0x0606050402020202, 0x0606050402020000, 0x0606050402020101, 0x0606050402020100, 0x0606050403030303, 0x0606050403030000, 0x0606050403030101, 0x0606050403030100, 0x0606050403020302, 0x0606050403020000, 0x0606050403020101, 0x0606050403020100, 0x0707070707070707, 0x0707070700000000, 0x0707070701010101, 0x0707070701000100, 0x0707070702020202, 0x0707070702020000, 0x0707070702020101, 0x0707070702020100, 0x0707070703030303, 0x0707070703030000, 0x0707070703030101, 0x0707070703030100, 0x0707070703020302, 0x0707070703020000, 0x0707070703020101, 0x0707070703020100, 0x0707040407070404, 0x0707040400000000, 0x0707040401010101, 0x0707040401000100, 0x0707040402020202, 0x0707040402020000, 0x0707040402020101, 0x0707040402020100, 0x0707040403030303, 0x0707040403030000, 0x0707040403030101, 0x0707040403030100, 0x0707040403020302, 0x0707040403020000, 0x0707040403020101, 0x0707040403020100, 0x0707050507070505, 0x0707050500000000, 0x0707050501010101, 0x0707050501000100, 0x0707050502020202, 0x0707050502020000, 0x0707050502020101, 0x0707050502020100, 0x0707050503030303, 0x0707050503030000, 0x0707050503030101, 0x0707050503030100, 0x0707050503020302, 0x0707050503020000, 0x0707050503020101, 0x0707050503020100, 0x0707050407070504, 0x0707050400000000, 0x0707050401010101, 0x0707050401000100, 0x0707050402020202, 0x0707050402020000, 0x0707050402020101, 0x0707050402020100, 0x0707050403030303, 0x0707050403030000, 0x0707050403030101, 0x0707050403030100, 0x0707050403020302, 0x0707050403020000, 0x0707050403020101, 0x0707050403020100, 0x0706070607060706, 0x0706070600000000, 0x0706070601010101, 0x0706070601000100, 0x0706070602020202, 0x0706070602020000, 0x0706070602020101, 0x0706070602020100, 0x0706070603030303, 0x0706070603030000, 0x0706070603030101, 0x0706070603030100, 0x0706070603020302, 0x0706070603020000, 0x0706070603020101, 0x0706070603020100, 0x0706040407060404, 0x0706040400000000, 0x0706040401010101, 0x0706040401000100, 0x0706040402020202, 0x0706040402020000, 0x0706040402020101, 0x0706040402020100, 0x0706040403030303, 0x0706040403030000, 0x0706040403030101, 0x0706040403030100, 0x0706040403020302, 0x0706040403020000, 0x0706040403020101, 0x0706040403020100, 0x0706050507060505, 0x0706050500000000, 0x0706050501010101, 0x0706050501000100, 0x0706050502020202, 0x0706050502020000, 0x0706050502020101, 0x0706050502020100, 0x0706050503030303, 0x0706050503030000, 0x0706050503030101, 0x0706050503030100, 0x0706050503020302, 0x0706050503020000, 0x0706050503020101, 0x0706050503020100, 0x0706050407060504, 0x0706050400000000, 0x0706050401010101, 0x0706050401000100, 0x0706050402020202, 0x0706050402020000, 0x0706050402020101, 0x0706050402020100, 0x0706050403030303, 0x0706050403030000, 0x0706050403030101, 0x0706050403030100, 0x0706050403020302, 0x0706050403020000, 0x0706050403020101, 0x0706050403020100
};

inline static uint8_t q6(uint64_t x, uint32_t i) {
	// on RISC-V + bitwise:
	//   return __builtin_ctzll(grev(x, i))^i;
	if(x == 0) __builtin_unreachable();
	size_t xhi = _mm_movemask_epi8(_mm_sign_epi8(_mm_set1_epi8(-128), _mm_cvtsi64_si128(x)));
	if(xhi == 0) __builtin_unreachable();
	uint8_t r = q3tab[xhi] >> (i & 56) << 3;
	x = (x >> r) & 255;
	if(x == 0) __builtin_unreachable();
	r += q3tab[x] >> ((i << 3) & 63);
	return r;
}

inline static uint8_t q3(size_t x, uint32_t i) {
	if(x == 0) __builtin_unreachable();
	if(i >= 8) __builtin_unreachable();
	return q3tab[x] >> (8 * i);
}

inline static uint32_t n_query(uint32_t val) {
	uint32_t r = q3(mask_l0, val >> 27);
	r = (r << 6) + q6(mask_l1[r], val >> 21);
	r = (r << 6) + q6(mask_l2[r], val >> 15);
	r = (r << 6) + q6(mask_l3[r], val >> 9);
	uint32_t i = ind_l3[r];
	r <<= 9;
	if(i == 0) __builtin_unreachable();
	if((int32_t)i < 0) {
		i ^= (uint32_t)1 << 31;
		if(i >= 512) __builtin_unreachable();
		r += i;
		return r ^ val;
	}
	size_t q = (val >> 6) & 7;
	for(size_t t = 0;; t++) {
		if(t >= 8) __builtin_unreachable();
		uint64_t m = leaves[i].v64[q ^ t];
		if(m) {
			r += (q ^ t) << 6;
			r += q6(m, val & 63);
			return r ^ val;
		}
	}
}

int main() {
	size_t q;
	q = inp.uint32_dec();
	inp.skip('\n');
	while(q--) {
		char op = inp.chr();
		inp.skip(' ');
		uint32_t val = inp.uint32_dec();
		inp.skip('\n');
		if(op == '0') {
			n_ins(val);
		} else if(op == '1') {
			n_del(val);
		} else if(op == '2') {
			out.ensure(11);
			out.uint32_dec(n_query(val));
			out.chr('\n');
		} else {
			__builtin_unreachable();
		}
	}
	out.flush();
	return 0;
}