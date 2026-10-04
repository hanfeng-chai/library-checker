#include <toy/hash.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 q = in.read<u32, 7>();
    HashMap<u64, u64> map(q);
    struct Query {
        u64 key, value;
        u32 type;
    };
    Query queries[128];
    u64 answers[128];
    while (q) {
        usize count = std::min<u32>(q, 128), used = 0;
        for (usize i = 0; i < count; ++i) {
            u32 type = in.read<u32, 1>();
            u64 key = in.read<u64, 19>(), value = type ? 0 : in.read<u64, 19>();
            queries[i] = {key, value, type};
            __builtin_prefetch(map.table.p + map.bucket(key));
        }
        for (usize i = 0; i < count; ++i) {
            auto [key, value, type] = queries[i];
            if (type)
                answers[used++] = map.get(key);
            else if (value)
                map[key] = value;
            else if (auto *entry = map.find(key))
                *entry = 0;
        }
        out.write(std::span(answers, used));
        q -= count;
    }
}
