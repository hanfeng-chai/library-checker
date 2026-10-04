#include <toy/hash.h>
#include <toy/io.h>
#include <toy/majority.h>
#include <toy/offline_frequency.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>(), answers = 0;
    HashMap<u32, u32> ids(n + q);
    Buffer<u32> values(0, n + q);
    auto id = [&](u32 x) {
        auto &entry = ids[x];
        if (!entry) {
            values[values.n++] = x;
            entry = values.n;
        }
        return entry - 1;
    };
    Buffer<u32> initial(n), current(n);
    for (u32 i = 0; i < n; ++i) initial[i] = current[i] = id(in.read<u32, 10>());
    MajorityTree tree{std::span<const u32>(initial)};
    Buffer<FrequencyEvent> events(0, 2 * q);
    Buffer<u32> candidates(q), lengths(q);
    while (q--) {
        u32 type = in.read<u32, 1>(), l = in.read<u32, 6>();
        if (!type) {
            u32 value = id(in.read<u32, 10>());
            events[events.n++] = FrequencyEvent::change(current[l], l, false);
            current[l] = value;
            events[events.n++] = FrequencyEvent::change(value, l, true);
            tree.set(l, value);
        } else {
            u32 r = in.read<u32, 6>(), value = tree.candidate(l, r).value;
            candidates[answers] = value;
            lengths[answers] = r - l;
            events[events.n++] = FrequencyEvent::query(value, l, r, answers++);
        }
    }
    auto result =
        point_value_frequencies(values.n, std::span<const u32>(initial),
                                std::span<const u32>(current), std::move(events), answers);
    for (u32 i = 0; i < answers; ++i)
        out.write(2 * result[i] > lengths[i] ? i32(values[candidates[i]]) : -1);
}
