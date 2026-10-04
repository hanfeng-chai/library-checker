#include <toy/big_integer.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    IntegerFFT workspace;
    BigInteger<false> a, b, product;
    usize cases = in.read<u32, 6>();
    while (cases--) {
        a.assign(in.token());
        b.assign(in.token());
        decltype(a)::multiply(a, b, product, &workspace);
        product.write(out);
    }
}
