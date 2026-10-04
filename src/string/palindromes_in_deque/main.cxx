#include <toy/io_batch.h>
#include <toy/palindrome_deque.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 q = in.read<u32, 6>();
    PalindromeDeque tree(q);
    while (q--) {
        u32 kind = in.read<u32, 1>();
        if (kind < 2) {
            u32 c = *in.p - 'a';
            in.p += 2;
            if (kind)
                tree.push<true>(c);
            else
                tree.push<false>(c);
        } else if (kind == 2)
            tree.pop<false>();
        else
            tree.pop<true>();
        write6(out, tree.distinct, ' ');
        write6(out, tree.prefix(), ' ');
        write6(out, tree.suffix());
    }
}
