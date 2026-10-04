#include <toy/bitset.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 8>(), q = in.read<u32, 7>();
    BitSet set{std::string_view(in.p, n)};
    in.p += n + 1;
    while (q--) {
        u32 type = in.read<u32, 1>(), x = in.read<u32, 7>();
        if (!type)
            set.insert(x);
        else if (type == 1)
            set.erase(x);
        else if (type == 2) {
            out.put('0' + set.contains(x));
            out.put('\n');
        } else
            out.write(set.bound(x, type == 4));
    }
}
