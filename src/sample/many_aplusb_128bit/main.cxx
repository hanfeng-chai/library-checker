#include <toy/io.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    int t = in.read();
    i128 sums[2048];
    while (t) {
        int n = min(t, 2048);
        for (int i = 0; i < n; ++i) {
            i128 a = in.read<i128, 38>(), b = in.read<i128, 38>();
            sums[i] = a + b;
        }
        out.write(span(sums, n)); t -= n;
    }
}
