#include <vector>
#include <cassert>
#include <array>
#include <iostream>


#include <unistd.h>

// サイズは空白や改行も含めた文字数
template<int size_in = 1 << 25, int size_out = 1 << 25>
struct fast_io {
    char ibuf[size_in], obuf[size_out];
    char *ip, *op;

    fast_io() : ip(ibuf), op(obuf) {
        int t = 0, k = 0;
        while ((k = read(STDIN_FILENO, ibuf + t, sizeof(ibuf) - t)) > 0) {
            t += k;
        }
    }
    
    ~fast_io() {
        int t = 0, k = 0;
        while ((k = write(STDOUT_FILENO, obuf + t, op - obuf - t)) > 0) {
            t += k;
        }
    }
  
    long long in() {
        long long x = 0;
        bool neg = false;
        for (; *ip < '+'; ip++) ;
        if (*ip == '-'){ neg = true; ip++;}
        else if (*ip == '+') ip++;
        for (; *ip >= '0'; ip++) x = 10 * x + *ip - '0';
        if (neg) x = -x;
        return x;
    }

    unsigned long long inu64() {
        unsigned long long x = 0;
        for (; *ip < '+'; ip++) ;
        if (*ip == '+') ip++;
        for (; *ip >= '0'; ip++) x = 10 * x + *ip - '0';
        return x;
    }

    char in_char() {
        for (; *ip < '!'; ip++) ;
        return *ip++;
    }
  
    void out(long long x, char c = 0) {
        static char tmp[20];
        if (!x) {
            *op++ = '0';
        } else {
            int i;
            if (x < 0) {
                *op++ = '-';
                x = -x;
            }
            for (i = 0; x; i++) {
                tmp[i] = x % 10;
                x /= 10;
            }
            for (i--; i >= 0; i--) *op++ = tmp[i] + '0';
        }
        if (c) *op++ = c;
    }

    void outu64(unsigned long long x, char c = 0) {
        static char tmp[20];
        if (!x) {
            *op++ = '0';
        } else {
            int i;
            for (i = 0; x; i++) {
                tmp[i] = x % 10;
                x /= 10;
            }
            for (i--; i >= 0; i--) *op++ = tmp[i] + '0';
        }
        if (c) *op++ = c;
    }

    void out_char(char x, char c = 0){
        *op++ = x;
        if (c) *op++ = c;
    }

    long long memory_size() {
        return (long long)(size_in + size_out) * sizeof(char);
    }
};



using T = int;
static constexpr int M = 500001;
using Ver = int;

Ver now = 1;
std::array<Ver, M> par, last;
std::array<int, M> len, lenL;
std::array<T, M> val;

struct R {
    Ver u;
    std::vector<Ver> v;
    R(int _u) : u(_u) {}

    void build() {
        int ver = last[u], size = lenL[u];
        v.resize(size);
        for (int i = 0; i < size; i++) {
            v[i] = ver;
            ver = par[ver];
        }
    }
};

std::vector<R*> rev(M, nullptr);

void check(Ver v) {
    if (len[now] == lenL[now] * 2 + 1) {
        lenL[now] = len[now];
        rev[now] = new R(now);
    } else {
        rev[now] = rev[v];
    }
}

Ver push_back(Ver v, T x) {
    last[now] = now;
    val[now] = x;
    par[now] = last[v];
    len[now] = len[v] + 1;
    lenL[now] = lenL[v];
    check(v);
    return now++;
}

Ver pop_front(Ver v) {
    last[now] = last[v];
    len[now] = len[v] - 1;
    lenL[now] = lenL[v] - 1;
    check(v);
    return now++;
}

T front(Ver v) {
    if (rev[v]->v.empty()) rev[v]->build();
    v = rev[v]->v[lenL[v] - 1];
    return val[v];
}

int main() {
    fast_io<1 << 25, 1 << 24> io;
    int Q, x, y, z;
    Q = io.in();
    for (int i = 0; i < Q; i++) {
        x = io.in();
        y = io.in();
        if (x == 0) {
            z = io.in();
            push_back(y + 1, z);
        } else {
            int z = front(y + 1);
            pop_front(y + 1);
            io.out(z, '\n');
        }
    }
}
