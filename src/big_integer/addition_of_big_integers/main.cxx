#include <toy/big_integer.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    BigInteger<false> a, b, sum;
    usize cases = in.read<u32, 6>();
    while (cases--) {
        auto x = in.token(), y = in.token();
        if (x.size() - (x.front() == '-') <= 18 && y.size() - (y.front() == '-') <= 18) {
            out.write(decltype(a)::small_decimal(x) + decltype(a)::small_decimal(y));
            continue;
        }
        a.assign(x);
        b.assign(y);
        decltype(a)::add(a, b, sum);
        sum.write(out);
    }
}
