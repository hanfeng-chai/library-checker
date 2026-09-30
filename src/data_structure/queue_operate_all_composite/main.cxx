#include <toy/io.h>
#include <toy/affine.h>
#include <toy/fold.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize q = in.read<u32, 6>(); FoldQueue<Affine<>, ComposeAffine<>> queue(q);
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (!type) { u32 a = in.read<u32, 9>(), b = in.read<u32, 9>(); queue.push({a, b}); }
        else if (type == 1) queue.pop();
        else out.write(queue.fold()(in.read<u32, 9>()));
    }
}
