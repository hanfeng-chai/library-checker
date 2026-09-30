#include <toy/io.h>
#include <toy/partition_queue.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<i32> a(n, n + q);
    for (auto& x : span(a.p, a.n)) x = in.read<i32, 10>();
    if (!n && q) while (*in.p <= ' ') ++in.p;
    PartitionQueue heap(std::move(a));
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (!type) heap.push(in.read<i32, 10>());
        else out.write(type == 1 ? heap.pop_min() : heap.pop_max());
    }
}
