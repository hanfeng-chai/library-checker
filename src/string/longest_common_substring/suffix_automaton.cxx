#include <toy/io.h>
#include <toy/string_basic.h>
#include <toy/suffix_automaton.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto s = in.token(), t = in.token();
    bool flipped = s.size() > t.size();
    if (flipped) std::swap(s, t);
    std::array<u32, 4> result{};
    bool ready = false;
    if (common_prefix(s.data(), t.data(), s.size()) == s.size()) {
        result = {0, u32(s.size()), 0, u32(s.size())};
        ready = true;
    } else if (s.size() < t.size())
        if (const void *found = memmem(t.data(), t.size(), s.data(), s.size())) {
            u32 at = (const char *)found - t.data();
            result = {0, u32(s.size()), at, at + u32(s.size())};
            ready = true;
        }
    if (!ready) {
        u32 a = 0, b = 0;
        for (char c : s) a |= 1u << (c - 'a');
        for (char c : t) b |= 1u << (c - 'a');
        if (!(a & b))
            ready = true;
        else if (std::has_single_bit(a) || std::has_single_bit(b)) {
            bool swap = std::has_single_bit(b) && !std::has_single_bit(a);
            auto text = swap ? s : t;
            u32 limit = swap ? t.size() : s.size(), length = 0, best = 0, end = 0;
            char c = 'a' + std::countr_zero(swap ? b : a);
            for (u32 i = 0; i < text.size(); ++i) {
                length = text[i] == c ? length + 1 : 0;
                if (length > best) {
                    best = std::min(length, limit);
                    end = i + 1;
                    if (best == limit) break;
                }
            }
            result = swap ? std::array<u32, 4>{end - best, end, 0, best}
                          : std::array<u32, 4>{0, best, end - best, end};
            ready = true;
        }
    }
    if (!ready) {
        SuffixAutomaton index(s);
        result = index.longest_common_substring(t);
    }
    if (flipped) {
        std::swap(result[0], result[2]);
        std::swap(result[1], result[3]);
    }
    for (u32 x : result) out.write(x, ' ');
    out.put('\n');
}
