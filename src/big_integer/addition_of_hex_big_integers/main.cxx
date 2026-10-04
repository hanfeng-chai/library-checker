#include <toy/big_integer.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    BigInteger<true> a, b, sum;
    usize cases = in.read<u32, 6>();
    while (cases--) {
        auto x = in.token(), y = in.token();
        if (x.size() - (x.front() == '-') <= 31 && y.size() - (y.front() == '-') <= 31) {
            sum.assign(decltype(a)::small_hex(x) + decltype(a)::small_hex(y));
            sum.write(out);
            continue;
        }
        a.assign(x);
        b.assign(y);
        decltype(a)::add(a, b, sum);
        sum.write(out);
    }
}
