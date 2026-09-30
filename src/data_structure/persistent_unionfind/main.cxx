#include <toy/io.h>
#include <toy/version_tree.h>
#include <toy/rollback_dsu.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); VersionTree tree(q); RollbackDSU dsu(n);
    struct Operation { int a, b, answer; };
    auto ops = version_storage<Operation>(q + 1); Buffer<RollbackDSU::Change> changes(q + 1); Buffer<u32> answers(q); int count = 0;
    for (usize i = 1; i <= q; ++i) {
        u32 type = in.read<u32, 1>(); int base = in.read<i32, 6>() + 1; auto [a, b] = in.read_pair<6>();
        tree.add(base); ops[i] = {int(a), int(b), type ? count++ : -1};
    }
    tree.visit([&](int i) { auto op = ops[i]; if (op.answer < 0) changes[i] = dsu.merge(op.a, op.b); else answers[op.answer] = dsu.same(op.a, op.b); },
               [&](int i) { if (ops[i].answer < 0) dsu.undo(changes[i]); });
    for (int i = 0; i < count; ++i) { out.put('0' + answers[i]); out.put('\n'); }
}
