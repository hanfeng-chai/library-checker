#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    int n = in.read();
    in.read<i128>();
    in.read<i128>();
    u64 sum = 0;
    while (n--) {
        bool neg = *in.p == '-';
        in.p += neg;
        u64 x = 0;
        while (*in.p >= '0') x = x * 10 + *in.p++ - '0';
        ++in.p;
        sum += neg ? u64(0) - x : x;
    }
    out.write(sum);
}
