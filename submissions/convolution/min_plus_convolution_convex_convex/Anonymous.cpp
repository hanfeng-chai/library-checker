#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <cstdint>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

constexpr int OUT_BUF_SIZE = 1 << 25;
alignas(64) char out_buf[OUT_BUF_SIZE];
alignas(64) uint32_t table[10000];

constexpr long long SPLIT_VAL = 100000000;

struct Init {
    Init() {
        for (int i = 0; i < 10000; ++i) {
            unsigned char b[4];
            int x = i;
            b[3] = x % 10 + '0';
            x /= 10;
            b[2] = x % 10 + '0';
            x /= 10;
            b[1] = x % 10 + '0';
            x /= 10;
            b[0] = x % 10 + '0';
            memcpy(&table[i], b, 4);
        }
    }
} _init1;

struct FastReader {
    char* p;
    FastReader() {
        struct stat st;
        fstat(0, &st);
        p = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }

    inline __attribute__((always_inline)) int nextInt() {
        while (*p <= ' ')
            p++;
        bool neg = false;
        if (*p == '-') {
            neg = true;
            p++;
        }
        unsigned int x = 0;
        while (*p >= '0') {
            x = x * 10 + (*p - '0');
            p++;
        }
        return neg ? -(int)x : (int)x;
    }
};

inline int write_full(char* buf, long long n) {
    if (n == 0) {
        *buf = '0';
        return 1;
    }
    char temp[24];
    int tp = 0;
    while (n > 0) {
        temp[tp++] = n % 10;
        n /= 10;
    }
    for (int i = 0; i < tp; ++i) {
        buf[i] = temp[tp - 1 - i] + '0';
    }
    return tp;
}

int main() {
    FastReader fr;

    int N = fr.nextInt();
    int M = fr.nextInt();

    int* diffA = new int[N];
    int* diffB = new int[M];

    int a_prev = fr.nextInt();
    long long current_val = a_prev;

    for (int i = 0; i < N - 1; ++i) {
        int cur = fr.nextInt();
        diffA[i] = cur - a_prev;
        a_prev = cur;
    }
    diffA[N - 1] = 2000000007;

    int b_prev = fr.nextInt();
    current_val += b_prev;

    for (int i = 0; i < M - 1; ++i) {
        int cur = fr.nextInt();
        diffB[i] = cur - b_prev;
        b_prev = cur;
    }
    diffB[M - 1] = 2000000007;

    char* op = out_buf;

    op += write_full(op, current_val);

    int* pA = diffA;
    int* pB = diffB;
    int limit = N + M - 2;

    long long cached_high = -1;
    char high_str[32];
    int high_len = 0;

    for (int i = 0; i < limit; ++i) {
        *op++ = ' ';

        int da = *pA;
        int db = *pB;
        bool take_a = (da <= db);
        int diff = take_a ? da : db;
        pA += take_a;
        pB += (take_a ^ 1);

        current_val += diff;

        if (current_val >= SPLIT_VAL) {
            long long high = current_val / SPLIT_VAL;
            int low = current_val % SPLIT_VAL;

            if (high == cached_high) {
                if (high_len == 1)
                    *op = *high_str;
                else if (high_len == 2)
                    *(uint16_t*)op = *(uint16_t*)high_str;
                else if (high_len == 3) {
                    *(uint16_t*)op = *(uint16_t*)high_str;
                    op[2] = high_str[2];
                } else if (high_len == 4)
                    *(uint32_t*)op = *(uint32_t*)high_str;
                else
                    memcpy(op, high_str, high_len);
                op += high_len;

                *(uint32_t*)op = table[low / 10000];
                op += 4;
                *(uint32_t*)op = table[low % 10000];
                op += 4;
            } else {
                cached_high = high;
                high_len = write_full(high_str, high);

                memcpy(op, high_str, high_len);
                op += high_len;

                *(uint32_t*)op = table[low / 10000];
                op += 4;
                *(uint32_t*)op = table[low % 10000];
                op += 4;
            }
        } else {
            cached_high = -1;
            op += write_full(op, current_val);
        }
    }

    *op++ = '\n';
    write(1, out_buf, op - out_buf);

    _exit(0);
}