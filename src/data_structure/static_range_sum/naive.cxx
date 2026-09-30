#include <toy/io.h>
using namespace toy;

u64 prefix[500001];

int main() {
    Reader in;
    Writer out;
    int n = in.read<u32, 6>(), q = in.read<u32, 6>();
    for (int i = 1; i <= n; ++i) prefix[i] = prefix[i - 1] + in.read<u64, 10>();
    while (q--) {
        u32 l = in.read<u32, 6>(), r = in.read<u32, 6>();
        out.write(prefix[r] - prefix[l]);
    }
}
