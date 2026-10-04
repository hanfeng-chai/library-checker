#include <toy/io.h>
#include <toy/substring_lcs.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 q = in.read<u32, 6>();
    auto s = in.token(), t = in.token();
    PrefixSubstringLCS lcs(s, t);
    while (q--) {
        u32 a = in.read<u32, 4>(), b = in.read<u32, 4>(), c = in.read<u32, 4>();
        out.write(lcs.query(a, b, c));
    }
}
