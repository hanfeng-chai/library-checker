#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <initializer_list>

#include <tuple>
#include <utility>
#include <type_traits>
#include <bit>

const __m128i dz = _mm_set1_epi8('0');
const __m128i indices = _mm_setr_epi8(-0x10, -0xf, -0xe, -0xd, -0xc, -0xb, -0xa, -0x9, -0x8, -0x7, -0x6, -0x5, -0x4, -0x3, -0x2, -0x1);
const __m128i z = _mm_setzero_si128();

const __m128i mul_10 = _mm_set1_epi16(10 + 1 * (1 << 8));
const __m128i mul_100 = _mm_set1_epi32(100 + 1 * (1 << 16));
const __m128i mul_10000 = _mm_set1_epi32(10000 + 1 * (1 << 16));

const uint64_t msk_a[4] = {
	0, 0, (uint64_t)-1, (uint64_t)-1
};

inline void rd_integer_pair_len7(const char** ep, int* a, int* b) {
	__m128i digits = _mm_loadu_si128((const __m128i*) *ep);
	__m128i digits_sub = _mm_subs_epu8(digits, dz);
	__m128i is_ndigit = _mm_cmpgt_epi8(dz, digits);

	uint16_t is_nd_msk = _mm_movemask_epi8(is_ndigit);
	int len1 = __tzcnt_u16(is_nd_msk);
	is_nd_msk >>= len1 + 1;
	int len2 = __tzcnt_u16(is_nd_msk);

	*ep += len1 + len2 + 2;

	__m128i new_shift = _mm_add_epi8(indices, _mm_set1_epi8(len1));
	__m128i new_shift2 = _mm_add_epi8(new_shift, _mm_set1_epi8(len2 + 1));
	__m128i digits1 = _mm_shuffle_epi8(digits_sub, new_shift);
#ifdef __AVX512F__
	__m128i digits2 = _mm_maskz_shuffle_epi8((uint16_t)(-1) << (16 - len2), digits_sub, new_shift2);
#else
	__m128i digits2 = _mm_shuffle_epi8(digits_sub, new_shift2) & _mm_loadu_si128((const __m128i*)((const char*)msk_a + len2));
#endif

	__m128i m1 = _mm_maddubs_epi16(digits1, mul_10);
	__m128i m12 = _mm_maddubs_epi16(digits2, mul_10);
	__m128i m2 = _mm_packs_epi32(_mm_madd_epi16(m1, mul_100), _mm_madd_epi16(m12, mul_100));
	__m128i m3 = _mm_madd_epi16(m2, mul_10000);

	*a = (int)(_mm_extract_epi32(m3, 1));
	*b = (int)(_mm_extract_epi32(m3, 3));
}

// BCD16 addition algorithm

constexpr uint64_t NIBBLE_LO = 0x0f0f0f0f0f0f0f0f;
constexpr uint64_t NIBBLE_HI = 0xf0f0f0f0f0f0f0f0;
constexpr uint64_t BCD_OFFS = 0x6666666666666666;
constexpr uint64_t BCD_OFFS_P1 = 0x0707070707070707;

inline uint64_t even_overflow_sum_mask(uint64_t b, uint64_t a, uint64_t real_sum) {
	// find if nibbles 0f0f0f0f overflow
	// Condition: sum + 1 overflows and the corresponding real nibble is not 0xf
	
	b &= NIBBLE_LO;
	a &= NIBBLE_LO;

	// possible overflow, stored in higher nibbles
	uint64_t po = (a + b + BCD_OFFS_P1) & NIBBLE_HI;

	real_sum = NIBBLE_LO - (real_sum & NIBBLE_LO);  // nonzero nibble here means 0xf not attained

	real_sum = (real_sum | (real_sum >> 1));  // merge pairs of bits
	real_sum = (real_sum | (real_sum >> 2));  // merge nibbles

	return 0xf * (real_sum & (po >> 4));
}

struct BCD16 {
	uint64_t data;

	BCD16 () {
		
	}

	BCD16 (uint64_t d) {
		data = d;
	}

	inline BCD16 operator+(const BCD16& b) const {
		uint64_t bd = b.data;
		uint64_t bd_biased = bd + BCD_OFFS;    // no overflow

		uint64_t full_sum = data + bd_biased;  // may need to mask some digits

		uint64_t msk = even_overflow_sum_mask(bd, data, full_sum);
		uint64_t oddm = even_overflow_sum_mask(bd >> 4, data >> 4, full_sum >> 4) << 4;
		
		msk |= oddm;

		return (msk & full_sum) + ((~msk & full_sum) - (~msk & BCD_OFFS));
	}

	inline BCD16& operator+=(const BCD16& b) {
	       	*this = *this + b;
		return *this;
	}

	inline BCD16 operator-(const BCD16& b) const {
		uint64_t bd = b.data;

		// 9s complement
		bd = 0x9999'9999'9999'9999 - bd;
		
		return BCD16{bd+1} + *this;
	}

	static inline BCD16 from_int(uint64_t v) {
		// Extract digit by digit
		uint64_t result = 0;

		for (int i = 0; i < 16; ++i) {
			result >>= 4;
			result |= (v % 10) << 60;
			v /= 10;
		}

		return BCD16{result};
	}

	uint64_t to_int() const {
		uint64_t result = 0;
		uint64_t d = data;

		for (int i = 0; i < 16; ++i) {
			result *= 10;
			result += (d & (0xf000'0000'0000'0000)) >> 60;
			d <<= 4;
		}

		return result;
	}

	// Read integer into 128 bit bcd
	static inline BCD16 rd_str(const char** c) {
		__m128i digits = _mm_loadu_si128((const __m128i*) *c);
		__m128i digits_sub = _mm_subs_epu8(digits, dz);
		__m128i is_ndigit = _mm_cmpgt_epi8(dz, digits);

		uint16_t is_nd_msk = _mm_movemask_epi8(is_ndigit);
		int len1 = __tzcnt_u16(is_nd_msk);

		*c += len1 + 1;  // extra char for space

		digits_sub = _mm_shuffle_epi8(digits_sub, _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));

		uint64_t hi = _mm_extract_epi64(digits_sub, 1), lo = _mm_cvtsi128_si64(digits_sub);

		uint64_t r = _pext_u64(lo, NIBBLE_LO) + (_pext_u64(hi, NIBBLE_LO) << 32);
		r >>= (64 - len1 * 4);

		return BCD16{r};
	}

	inline void write_to(char** w) const {
		constexpr uint64_t zeros = '0' * 0x0101010101010101;
		int lz = _lzcnt_u64(data) / 4;

		if (lz == 16) lz = 15;  // 0

		uint64_t d = data;	
		d <<= lz * 4;

		*((uint64_t*)*w) = __bswap_64(_pdep_u64(d >> 32, NIBBLE_LO) + zeros);
		*((uint64_t*)*w + 1) = __bswap_64(_pdep_u64(d, NIBBLE_LO) + zeros);

		*w += 16 - lz;
		**w = '\n';
		*w += 1;
	}

};

#ifdef __AVX2__
#ifdef __AVX512F__
#define VT __m512i
#define v_set1_epi64 _mm512_set1_epi64
#define v_set1_epi8 _mm512_set1_epi8
#define v_add_epi8 _mm512_add_epi8
#define v_add_epi64 _mm512_add_epi64
#define v_sub_epi8 _mm512_sub_epi8
#define v_sub_epi64 _mm512_sub_epi64
#define v_subs_epu8 _mm512_subs_epu8
#define v_mullo_epi16 _mm512_mullo_epi16
#define v_loadu _mm512_loadu_si512
#define v_storeu _mm512_storeu_si512

const VT V_NIBBLE_HI = v_set1_epi64(NIBBLE_HI);

__m512i v_sr_epi8(__m512i a, const int i) {
	return _mm512_srli_epi16(a & V_NIBBLE_HI, i);
}
#else
#define VT __m256i
#define v_set1_epi64 _mm256_set1_epi64x
#define v_set1_epi8 _mm256_set1_epi8
#define v_add_epi8 _mm256_add_epi8
#define v_add_epi64 _mm256_add_epi64
#define v_sub_epi8 _mm256_sub_epi8
#define v_sub_epi64 _mm256_sub_epi64
#define v_subs_epu8 _mm256_subs_epu8
#define v_mullo_epi16 _mm256_mullo_epi16
#define v_loadu _mm256_loadu_si256
#define v_storeu _mm256_storeu_si256

const VT V_NIBBLE_HI = v_set1_epi64(NIBBLE_HI);

__m256i v_sr_epi8(__m256i a, const int i) {
	return _mm256_srli_epi16(a & V_NIBBLE_HI, i);
}
#endif
	const VT V_BCD_OFFS = v_set1_epi64(BCD_OFFS);
	const VT V_BCD_OFFS_P1 = v_set1_epi64(BCD_OFFS_P1);
	const VT V_NIBBLE_LO = v_set1_epi64(NIBBLE_LO);
	const VT V_NINES = v_set1_epi64(0x9999'9999'9999'999a);


struct BCD16V {


	VT data;

	BCD16V () {
#ifdef __AVX512F__
		data = _mm512_setzero_si512();
#else
		data = _mm256_setzero_si256();
#endif
	}

	BCD16V (VT d) {
		data = d;
	}

	BCD16V (std::initializer_list<BCD16> k) {
#ifdef __AVX512F__
		data = _mm512_loadu_si512((const __m512i*) k.begin());
#else
		data = _mm256_loadu_si256((const __m256i*) k.begin());
#endif
	}

	template <int idx>
	inline void insert_bcd16(BCD16 v) {
#ifdef __AVX512F__
		data = _mm512_mask_set1_epi32(data, 1 << idx, v.data);
#else
		data = _mm256_insert_epi32(data, v.data, idx);
#endif
	}

	template <bool SR4>
	static inline VT overflow_sum_mask(VT a, VT b, VT real_sum) {
		// assumes a, b, and real_sum are already masked properly. Returns results in lowest bit of each 8-bit int
		// possible overflow and real_sum != 0xf required
		
		VT possible_overflow = v_add_epi8(v_add_epi8(V_BCD_OFFS_P1, a), b);
		if constexpr (!SR4) {
			possible_overflow = v_sr_epi8(possible_overflow, 4);
			real_sum = v_subs_epu8(real_sum, v_set1_epi8(0xe));

			return possible_overflow & ~real_sum;
		} else {
			possible_overflow &= NIBBLE_HI;
			return possible_overflow & ~v_add_epi8(real_sum, v_set1_epi8(1));
		}
	}

	inline BCD16V operator+(const BCD16V& b) const {
		VT bd = b.data;	
		VT bd_biased = bd + V_BCD_OFFS;

		VT full_sum = v_add_epi64(data, bd_biased);

		// Calculate overflow sum mask
		VT msk_even = BCD16V::overflow_sum_mask<false>(bd & V_NIBBLE_LO, data & V_NIBBLE_LO, full_sum & V_NIBBLE_LO);  // mask out which nibbles have overflowed
		VT msk_odd = BCD16V::overflow_sum_mask<true>(v_sr_epi8(bd, 4), v_sr_epi8(data, 4), v_sr_epi8(full_sum, 4));
		VT msk = v_mullo_epi16(msk_even | msk_odd, v_set1_epi64(0xf000f000f000f));

		VT no_overflow = v_sub_epi8(~msk & full_sum, ~msk & V_BCD_OFFS);

		return (msk & full_sum) | no_overflow;
	}

	inline BCD16V& operator+=(const BCD16V& b) {
		*this = *this + b;
		return *this;
	}

	inline BCD16V operator-(const BCD16V& b) const {
		VT bd = b.data;

		bd = v_sub_epi64(V_NINES, bd);

		return BCD16V{bd} + *this;
	}

	inline void extract_to_arr(BCD16* v) const {
#ifdef __AVX512F__
	_mm512_storeu_si512((__m512i*) v, data);	
#else
	_mm256_storeu_si256((__m256i*) v, data);	
#endif
	}

	inline void write_to(char** w) const {
		// vpcompressb would be excellent here, but we don't have access. Perform 4 to 8 16-byte stores instead

		constexpr int cnt = sizeof(VT) / sizeof(BCD16);
		
#ifdef __AVX512F__
		uint64_t lz[cnt];

		VT lz_v = _mm512_srli_epi32(_mm512_lzcnt_epi64(data), 2);
		lz_v = _mm512_min_epi32(lz_v, _mm512_set1_epi64(15));
		_mm512_storeu_si512((__m512i*)lz, lz_v);
	
		VT shifted_bcd = _mm512_sllv_epi64(data, _mm512_slli_epi32(lz_v, 2));

		VT low_bcd = shifted_bcd & NIBBLE_LO;
		VT hi_bcd = v_sr_epi8(shifted_bcd, 4);

		__m512i entr1_to_4 = _mm512_unpackhi_epi8(low_bcd, hi_bcd);
		__m512i entr5_to_8 = _mm512_unpacklo_epi8(low_bcd, hi_bcd);

		__m512i swp = _mm512_set_epi8(0, 1, 2, 3, 4, 5, 6, 7,
				8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
				8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
				8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
				8, 9, 10, 11, 12, 13, 14, 15);

		entr1_to_4 = _mm512_shuffle_epi8(entr1_to_4, swp);
		entr5_to_8 = _mm512_shuffle_epi8(entr5_to_8, swp);

		const __m512i ZEROS = _mm512_set1_epi8('0');

		entr1_to_4 = _mm512_add_epi8(entr1_to_4, ZEROS);
		entr5_to_8 = _mm512_add_epi8(entr5_to_8, ZEROS);

#define STORE_NEXT(idx, entr, entr_idx) \
		_mm_storeu_si128((__m128i*) *w, _mm512_extracti64x2_epi64(entr, entr_idx)); \
		*w += 16 - lz[idx]; \
		**w = '\n'; \
		(*w)++;

		STORE_NEXT(0, entr5_to_8, 0)
		STORE_NEXT(1, entr1_to_4, 0)
		STORE_NEXT(2, entr5_to_8, 1)
		STORE_NEXT(3, entr1_to_4, 1)

		STORE_NEXT(4, entr5_to_8, 2)
		STORE_NEXT(5, entr1_to_4, 2)
		STORE_NEXT(6, entr5_to_8, 3)
		STORE_NEXT(7, entr1_to_4, 3)

#undef STORE_NEXT
#else // unused
		BCD16 scalar[cnt];
		_mm256_storeu_si256((__m256i*) scalar, data);

		for (int i = 0; i < cnt; ++i) {
			scalar[i].write_to(w);
		}
#endif

	}
};
#endif

const int CHK = 32768;

BCD16 sums[500001];
char output[CHK * 17];

int main() {
	struct stat st;
	fstat(0, &st);

	size_t input_size;

	const char* input = (const char*)mmap(0, input_size = st.st_size + 64, PROT_READ, MAP_SHARED | MAP_POPULATE, 0, 0);
	if (input == MAP_FAILED) abort();

	int N, Q;
	rd_integer_pair_len7(&input, &N, &Q);

	// bcd
	BCD16 running{0};

	char* wp = output;

	for (int i = 0; i < N; ++i) {
		sums[i] = running;
		running += BCD16::rd_str(&input);
	}

	sums[N] = running;

	BCD16 diffs[CHK];

	uint64_t start;

#ifdef VT
	constexpr int sub_chk = sizeof(VT) / sizeof(uint64_t);
#endif

	int i = 0;
	for (;;) {
		int j = 0;

#ifdef __AVX2__
		for (; i < Q - sub_chk + 1 && j < CHK; j += sub_chk, i += sub_chk) {
			BCD16 begins[sub_chk];
			BCD16 ends[sub_chk];

			for (int m = 0; m < sub_chk; ++m) {
				int begin, end;
				rd_integer_pair_len7(&input, &begin, &end);

				_mm_prefetch(&sums[begin], _MM_HINT_T0);
				_mm_prefetch(&sums[end], _MM_HINT_T0);

				*(uint64_t*)(begins + m) = (uint64_t)(sums + begin);
				*(uint64_t*)(ends + m) = (uint64_t)(sums + end);
			}

			for (int m = 0; m < sub_chk; ++m) {
				begins[m] = *((uint64_t*)(begins[m].data));
				ends[m] = *((uint64_t*)(ends[m].data));
			}

			BCD16V ends_v = BCD16V{v_loadu((const VT*) ends)};
			BCD16V begins_v = BCD16V{v_loadu((const VT*) begins)};

			BCD16V diff = ends_v - begins_v;
			
			v_storeu((VT*) &diffs[j], diff.data);
		}
#endif

		for (; i < Q && j < CHK; ++j, ++i) {
			int begin, end;
			rd_integer_pair_len7(&input, &begin, &end);

			diffs[j] = sums[end] - sums[begin];
		}

		int k = 0;
#ifdef __AVX512F__
		for (; k < j - sub_chk + 1; k += sub_chk) {
			BCD16V v = BCD16V{v_loadu((const VT*)&diffs[k])};	

			v.write_to(&wp);
		}
#endif

		for (; k < j; ++k) {
			diffs[k].write_to(&wp);
		}

		write(1, output, wp - output);
		wp = output;

		if (j != CHK) break;
	}

	return 0;
}
