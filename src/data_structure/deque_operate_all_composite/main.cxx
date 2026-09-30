#include <toy/io.h>
#include <toy/affine.h>
#include <toy/fold.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize q = in.read<u32, 6>(); FoldDeque<Affine<>, ComposeAffine<>> deque(q);
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (type < 2) { u32 a = in.read<u32, 16>(), b = in.read<u32, 16>(); if (!type) deque.push_front({a, b}); else deque.push_back({a, b}); }
        else if (type == 2) deque.pop_front();
        else if (type == 3) deque.pop_back();
        else out.write(deque.fold()(in.read<u32, 16>()));
    }
}
