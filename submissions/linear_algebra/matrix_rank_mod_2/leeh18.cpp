#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define USE_AVX2

#ifdef USE_AVX2
#include <immintrin.h>
#endif

using namespace std;

struct Matrix {
    const int rows, cols, stride;
    unsigned long long *data;
    Matrix(int r, int c) : rows(r), cols(c), stride((c + 63) / 64) {
        size_t total_bytes = (size_t)rows * stride * sizeof(unsigned long long);
        data = new unsigned long long[rows * stride];
        memset(data, 0, total_bytes);
    }
    ~Matrix() { delete[] data; }
    void set(int r, int c) { data[r * stride + (c / 64)] |= (1ULL << (c % 64)); }
    bool get(int r, int c) { return (data[r * stride + (c / 64)] >> (c % 64)) & 1; }
#ifdef USE_AVX2
    __attribute__((target("avx2"))) void xor_rows(unsigned long long *d, const unsigned long long *s, int start_idx) {
        int i = start_idx;
        for (; i + 16 <= stride; i += 16) {
            _mm256_storeu_si256((__m256i *)&d[i], _mm256_xor_si256(_mm256_loadu_si256((__m256i *)&d[i]),_mm256_loadu_si256((__m256i *)&s[i])));
            _mm256_storeu_si256((__m256i *)&d[i + 4], _mm256_xor_si256(_mm256_loadu_si256((__m256i *)&d[i + 4]),_mm256_loadu_si256((__m256i *)&s[i + 4])));
            _mm256_storeu_si256((__m256i *)&d[i + 8], _mm256_xor_si256(_mm256_loadu_si256((__m256i *)&d[i + 8]),_mm256_loadu_si256((__m256i *)&s[i + 8])));
            _mm256_storeu_si256((__m256i *)&d[i + 12], _mm256_xor_si256(_mm256_loadu_si256((__m256i *)&d[i + 12]),_mm256_loadu_si256((__m256i *)&s[i + 12])));
        }
        for (; i < stride; ++i) {
            d[i] ^= s[i];
        }
    }
#else
   void xor_rows(unsigned long long *d, const unsigned long long *s, int start_idx) {
        for (int i = start_idx; i < stride; ++i) {
            d[i] ^= s[i];
        }
    }
#endif
    int gaussian_elimination() { // O(min(N, M) * N * M / 64)
        int pivot_row = 0;
        for (int col = 0; col < cols && pivot_row < rows; ++col) {
            int chunk_idx = col / 64;
            unsigned long long bit_mask = (1ULL << (col % 64));
            int sel = -1;
            for (int i = pivot_row; i < rows; ++i) {
                if (data[i * stride + chunk_idx] & bit_mask) {
                    sel = i;
                    break;
                }
            }
            if (sel == -1) {
                continue;
            }
            if (pivot_row != sel)
                xor_rows(data + pivot_row * stride, data + sel * stride, chunk_idx);
            for (int i = pivot_row + 1; i < rows; ++i) {
                if (data[i * stride + chunk_idx] & bit_mask) {
                    xor_rows(data + i * stride, data + pivot_row * stride, chunk_idx);
                }
            }
            ++pivot_row;
        }
        return pivot_row;
    }
};

struct scanner {
    scanner() {
        struct stat st;
        fstat(0, &st);
        if (S_ISREG(st.st_mode)) {
            data_ = p_ = static_cast<char *>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0));
            size_ = st.st_size;
        } else {
            std::string buffer;
            char buf[65536];
            ssize_t n;
            while ((n = read(0, buf, sizeof(buf))) > 0) {
                buffer.append(buf, n);
            }
            data_ = new char[buffer.size() + 1];
            memcpy(data_, buffer.c_str(), buffer.size());
            data_[buffer.size()] = '\0';
            p_ = data_;
        }
    }
    template <typename... Args> void operator()(Args &...args) { (scan(args), ...); }
    std::string_view getline() {
        skip();
        auto first = p_;
        while (*p_ >= ' ') {
            ++p_;
        }
        return std::string_view(first, p_ - first);
    }

private:
    template <typename T> std::enable_if_t<std::is_integral_v<T>, void> scan(T &x) {
        skip();
        x = 0;
        auto is_negative = false;
        if (*p_ == '-') {
            ++p_;
            is_negative = true;
        }
        for (; *p_ > ' '; ++p_) {
            x = (x << 1) + (x << 3) + (*p_ & 15);
        }
        if (is_negative) {
            x = -x;
        }
    }
    template <typename T> std::enable_if_t<std::is_floating_point_v<T>, void> scan(T &x) {
        skip();
        auto first = p_;
        while (*p_ > ' ') {
            ++p_;
        }
        std::from_chars(first, p_, x);
    }
    void scan(char &x) {
        skip();
        x = *p_++;
    }
    void skip() {
        while (*p_ <= ' ') {
            ++p_;
        }
    }
    char *data_;
    char *p_;
    size_t size_;
};

scanner scan;

int main() {
    int n, m;
    scan(n, m);
    if (n < m) {
        Matrix mat(n, m);
        for (auto i = 0; i < n; ++i) {
            for (auto j = 0; j < m; ++j) {
                char c;
                scan(c);
                if (c == '1') {
                    mat.set(i, j);
                }
            }
        }
        cout << mat.gaussian_elimination() << endl;
    } else {
        Matrix mat(m, n);
        for (auto i = 0; i < n; ++i) {
            for (auto j = 0; j < m; ++j) {
                char c;
                scan(c);
                if (c == '1') {
                    mat.set(j, i);
                }
            }
        }
        cout << mat.gaussian_elimination() << endl;
    }
}
