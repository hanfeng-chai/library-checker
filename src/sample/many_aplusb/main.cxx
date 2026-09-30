#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    int t = in.read();
    u64 sums[2048];
    while (t) {
        int n = min(t, 2048);
        for (int i = 0; i < n; ++i) {
            u64 a = in.read<u64, 19>(), b = in.read<u64, 19>();
            sums[i] = a + b;
        }
        out.write(span(sums, n));
        t -= n;
    }
}
