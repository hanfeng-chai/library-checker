#include <toy/big_division.h>
using namespace toy;
int main() {
    Reader in; Writer out; BigInteger<true> a,b,q,r; BigDivision<true> work;
    usize t=in.read<u32,7>();
    while(t--) {
        auto x=in.token(), y=in.token();
        if(x.size()<=16 && y.size()<=16) {
            u64 av=decltype(a)::hexadecimal_padded(x.data(),x.size()), bv=decltype(a)::hexadecimal_padded(y.data(),y.size());
            u64 quot=av/bv; q.assign(i128(quot)); r.assign(i128(av-quot*bv));
        } else if(x.size()<=31 && y.size()<=31) {
            u128 av=decltype(a)::small_hex(x),bv=decltype(a)::small_hex(y),quot=av/bv;
            q.assign(i128(quot));r.assign(i128(av-quot*bv));
        } else if(x.size()<y.size()) { out.write(0,' ');out.append(x);out.put('\n');continue; }
        else { a.assign(x);b.assign(y);work.divmod(a,b,q,r); }
        q.write(out,' ');r.write(out);
    }
}
