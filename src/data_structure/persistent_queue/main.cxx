#include <toy/io.h>
#include <toy/version_tree.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize q = in.read<u32, 6>(); VersionTree tree(q);
    struct Operation { int answer; u32 value; };
    auto ops = version_storage<Operation>(q + 1); Buffer<u32> values(q), answers(q); int count = 0, front = 0, back = 0;
    for (usize i = 1; i <= q; ++i) {
        u32 type = in.read<u32, 1>(); int base = in.read<i32, 6>() + 1; tree.add(base);
        if (!type) ops[i] = {-1, in.read<u32, 10>()}; else ops[i] = {count++, 0};
    }
    tree.visit([&](int i) { auto op = ops[i]; if (op.answer < 0) values[back++] = op.value; else answers[op.answer] = values[front++]; },
               [&](int i) { auto op = ops[i]; if (op.answer < 0) --back; else values[--front] = answers[op.answer]; });
    out.write(std::span(answers.p, count));
}
