#include <toy/big_division.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    BigInteger<false> a, b, q, r;
    BigDivision<false> work;
    usize cases = in.read<u32, 7>();
    while (cases--) {
        auto x = in.token(), y = in.token();
        if (x.size() <= 18 && y.size() <= 18) {
            u64 av = decltype(a)::small_decimal(x), bv = decltype(a)::small_decimal(y);
            out.write(av / bv, ' ');
            out.write(av % bv);
            continue;
        }
        if (x.size() < y.size()) {
            out.write(0, ' ');
            out.append(x);
            out.put('\n');
            continue;
        }
        a.assign(x);
        b.assign(y);
        work.divmod(a, b, q, r);
        q.write(out, ' ');
        r.write(out);
    }
}
