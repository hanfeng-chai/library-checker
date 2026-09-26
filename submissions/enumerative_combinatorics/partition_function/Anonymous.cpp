// #include "common_test.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <immintrin.h>

// How much does avx512 even help?
#undef __AVX512F__

int64_t mini_strtoll(const char** c) {
	int64_t a = 0;

	const char* b = *c;
	
	while (1) {
		char k = *b;
		b++;	
		
		if (k >= '0' && k <= '9')
			a = a * 10 + k - '0';
		else
			break;	
	}

	*c = b;

	return a;
}

void mini_9utoa(char** c, uint32_t a) {
	char* b = *c;

	if (__builtin_expect(a == 0, 0)) {
		*b = '0';
		*c++;
	}

	const __m256i pow10 = _mm256_setr_epi32(10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000);

	const int cmp = _mm256_movemask_ps(
			_mm256_castsi256_ps(
				_mm256_cmpgt_epi32(pow10,
					_mm256_set1_epi32(a)))) + 0x100;

	int len = __tzcnt_u16(cmp);
	*c += len + 1;

	for (char* i = *c - 1; i >= b; --i) {
		*i = (char)(a % 10) + '0';
		a /= 10;
	}
}

#define INT __asm__ volatile( "int3;" ::: "memory" );
#define MODULUS 998244353ULL

void calculate_partitions(uint32_t* b, int N) {
	for (int k = 2; k < N; ++k) {
		int64_t sum = 0;

		int d = 0;
		int cn = -1;

		for (;;) {
			d += cn + 2;
			cn++;
			
			if (d > k) break;
			sum += b[k - d];

			d += 1 + (cn >> 1);
			cn++;

			if (d > k) break;
			sum += b[k - d];

			d += cn + 2;
			cn++;
			
			if (d > k) break;
			sum += MODULUS - b[k - d];

			d += 1 + (cn >> 1);
			cn++;

			if (d > k) break;
			sum += MODULUS - b[k - d];
		}

		b[k] = sum % MODULUS;
	}
}

uint32_t stored_pentagonal[1200];

void fill_p() {
	for (int i = 0; i < 1200; ++i) {
		int n = (i + 1) >> 1;
		stored_pentagonal[i] = (n * (3 * n - 2 * (i % 2) + 1)) >> 1;
	}
}

inline uint32_t pentagonal(uint32_t i) {
	return stored_pentagonal[i];
}

void avx2_calculate_partitions(uint32_t* b, int N, int precomputed) {
#ifdef __AVX512F__
#define W 7
#define BW 128
#define IW 16
#define VT __m512i
#else
#define W 6
#define BW 64
#define IW 8
#define VT __m256i
#endif

	N = ((N >> W) << W) + BW;

	int groups = N >> W;

#ifdef __AVX512F__
	const __m512i mod = _mm512_set1_epi32(MODULUS);
#else
	const __m256i mod = _mm256_set1_epi32(MODULUS);
#endif

	for (int k = (precomputed >> W) << W; k < N; k += BW) {
#ifdef __AVX512F__
#define INIT(n) __m512i n = _mm512_setzero_si512();
#else
#define INIT(n) __m256i n = _mm256_setzero_si256();
#endif
		INIT(accum1) 
		INIT(accum2)
		INIT(accum3)
		INIT(accum4)
		INIT(accum5)
		INIT(accum6)
		INIT(accum7)
		INIT(accum8)

#ifdef __AVX512F__
#define ADDV _mm512_add_epi32
#define LOADU _mm512_loadu_si512
#define LOADA _mm512_load_si512
#define SUBV _mm512_sub_epi32
#define MINU _mm512_min_epu32
#define STORE _mm512_storeu_si512
#else
#define ADDV _mm256_add_epi32
#define LOADU _mm256_loadu_si256
#define LOADA _mm256_load_si256
#define SUBV _mm256_sub_epi32
#define MINU _mm256_min_epu32
#define STORE _mm256_storeu_si256
#endif

#define ADD_MOD(a, addr, reduce) { \
		VT addb = ADDV(a, LOADU((const VT *) (addr))); \
		VT subb = SUBV(addb, mod); \
		a = reduce ? MINU(addb, subb) : addb; }

#define REDUCE(a) a = MINU(MINU(ADDV(a, mod), a), SUBV(a, mod));

#define SUB_MOD(a, addr, reduce) { \
		VT subb = SUBV(a, LOADU((const VT *) (addr))); \
		VT addb = ADDV(subb, mod); \
		a = reduce ? MINU(addb, subb) : addb; }

#ifdef __AVX512F__
#define START 21
#else
#define START 13
#endif

		int i = START;
		int p, next_p = pentagonal(i);
		uint32_t* base, *next_base = b + k - next_p;

		// Main loop
		for (;;) {
#define PREFETCH_BASE \
			_mm_prefetch(next_base, _MM_HINT_T0); \
			_mm_prefetch(next_base + 16, _MM_HINT_T0); \
			_mm_prefetch(next_base + 32, _MM_HINT_T0); \
			_mm_prefetch(next_base + 48, _MM_HINT_T0);

#ifdef __AVX512F__
#define PREFETCH_SEQ PREFETCH_BASE \
			_mm_prefetch(next_base + 64, _MM_HINT_T0); \
			_mm_prefetch(next_base + 80, _MM_HINT_T0); \
			_mm_prefetch(next_base + 96, _MM_HINT_T0); \
			_mm_prefetch(next_base + 112, _MM_HINT_T0);
#else
#define PREFETCH_SEQ PREFETCH_BASE
#endif

#define TEXTIFY(A) #A
#define ADD_SEQ(TYPE, reduce) \
 \
			base = next_base; \
			p = next_p; \
			if (base < b - BW) break; \
 \
			/*printf("k: %i, p: %i, i: %i, base: %llu, b: %llu, type: " TEXTIFY(TYPE) "\n" , k, p, i, base, b);*/ \
\
			next_p = pentagonal(i + 1); \
			next_base = b + k- next_p; \
\
			TYPE(accum1, base, reduce); \
			TYPE(accum2, base + IW, reduce); \
			TYPE(accum3, base + 2 * IW, reduce); \
			TYPE(accum4, base + 3 * IW, reduce); \
			TYPE(accum5, base + 4 * IW, reduce); \
			TYPE(accum6, base + 5 * IW, reduce); \
			TYPE(accum7, base + 6 * IW, reduce); \
			TYPE(accum8, base + 7 * IW, reduce); \
			++i; 

			ADD_SEQ(ADD_MOD, 1)
			ADD_SEQ(ADD_MOD, 1)
			ADD_SEQ(SUB_MOD, 1)
			ADD_SEQ(SUB_MOD, 1)
		}

		// Merge accumulators (pain)
#define STORE_ACCUM(a, addr) STORE((VT *)(addr), a);

		base = b + k;
		
		STORE_ACCUM(accum1, base); 
		STORE_ACCUM(accum2, base + IW); 
		STORE_ACCUM(accum3, base + 2 * IW); 
		STORE_ACCUM(accum4, base + 3 * IW); 
		STORE_ACCUM(accum5, base + 4 * IW); 
		STORE_ACCUM(accum6, base + 5 * IW); 
		STORE_ACCUM(accum7, base + 6 * IW); 
		STORE_ACCUM(accum8, base + 7 * IW);

		for (int j = 0; j < BW; ++j) {
			int64_t sum = *base;

			for (int i = START - 1; ;) {
				int n = pentagonal(i);

				n = pentagonal(i);
				sum += MODULUS - base[-n];
				--i;

				if (i <= 0) break;

				n = pentagonal(i);
				sum += MODULUS - base[-n];
				--i;

				if (i <= 0) break;

				n = pentagonal(i);
				sum += base[-n];
				--i;

				if (i <= 0) break;

				n = pentagonal(i);
				sum += base[-n];
				--i;

				if (i <= 0) break;
			}

			*base = sum % MODULUS;	
			base++;
		}
	}
}

int main() {
	fill_p();

	struct stat st;
	fstat(0, &st);

	size_t input_size;

	const char* input = (const char*)mmap(0, input_size = st.st_size + 64, PROT_READ, MAP_SHARED | MAP_POPULATE, 0, 0);
	if (input == MAP_FAILED) abort();

	const int N = mini_strtoll(&input) + 1;
	uint32_t* partitions = (uint32_t*)calloc((N + 128) * sizeof(uint32_t), 1);

	partitions += 128;

	char* output = (char*)malloc(N * 10) + 32;
	char* wp = output;
	
	partitions[0] = 1; partitions[1] = 1;

	calculate_partitions(partitions, 256);
	avx2_calculate_partitions(partitions, N, 256);

	for (int k = 0; k < N; ++k) {
		mini_9utoa(&wp, partitions[k]);
		*wp++ = '\n';
	}	

	write(1, output, wp - output);
	return 0;
}