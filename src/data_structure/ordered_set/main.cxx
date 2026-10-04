#include <toy/io.h>
#include <toy/ordered_set.h>
#include <toy/radix_sort.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u32> initial(n), indices(n + q), types(q), values(n + q);
    for (auto &x : std::span(initial.p, initial.n)) x = in.read<u32, 10>();
    if (!n && q)
        while (*in.p <= ' ') ++in.p;
    struct Item {
        u32 value, slot;
    };
    Buffer<Item> items(q);
    usize used = 0;
    for (u32 i = 0; i < q; ++i) {
        types[i] = in.read<u32, 1>();
        u32 value = in.read<u32, 10>();
        indices[n + i] = value;
        if (types[i] != 2) items[used++] = {value, n + i};
    }
    items.n = used;
    radix_sort<30, 8>(std::span(items.p, items.n), [](Item x) { return x.value; });
    // The initial set is already sorted. Only query keys need a radix sort.
    usize a = 0, b = 0;
    u32 count = 0;
    while (a < n || b < used) {
        u32 x = std::min(a < n ? initial[a] : ~u32(0), b < used ? items[b].value : ~u32(0));
        values[count] = x;
        if (a < n && initial[a] == x) indices[a++] = count;
        while (b < used && items[b].value == x) indices[items[b++].slot] = count;
        ++count;
    }
    OrderedSet set(count, std::span<const u32>(indices.p, n));
    for (u32 i = 0; i < q; ++i) {
        u32 x = indices[n + i], type = types[i];
        if (!type)
            set.insert(x);
        else if (type == 1)
            set.erase(x);
        else if (type == 2)
            out.write(x <= set.size() ? i32(values[set.kth(x - 1)]) : -1);
        else if (type == 3)
            out.write(set.rank(x + 1));
        else {
            int at = type == 4 ? set.previous(x) : set.next(x);
            out.write(at < 0 ? -1 : i32(values[at]));
        }
    }
}
