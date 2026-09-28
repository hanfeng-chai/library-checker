#include <toy/io.hpp>
#include <toy/range.hpp>

struct Vote { int value = 0, balance = 0; };
struct MergeVote {
    Vote operator()(Vote a, Vote b) const {
        if (!a.balance) return b;
        if (!b.balance) return a;
        if (a.value == b.value) return {a.value, a.balance + b.balance};
        if (a.balance > b.balance) return {a.value, a.balance - b.balance};
        return {b.value, b.balance - a.balance};
    }
};
struct Query { int type, first, second; };

int main() {
    toy::Reader input(toy::direct_mapping); toy::Writer output;
    int n = input.read_uniform<6, toy::u32>(), q = input.read_uniform<6, toy::u32>();
    std::vector<int> values(n), coordinates;
    for (int& x : values) x = input.read_uniform<10, int>(), coordinates.push_back(x);
    std::vector<Query> queries(q);
    for (auto& query : queries) {
        query.type = input.read_fixed<1, toy::u32>();
        query.first = input.read_uniform<6, toy::u32>();
        query.second = input.read_uniform<10, toy::u32>();
        if (!query.type) coordinates.push_back(query.second);
    }
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    int m = coordinates.size();
    std::vector<std::vector<int>> positions(m);
    for (int i = 0; i < n; ++i)
        positions[std::lower_bound(coordinates.begin(), coordinates.end(), values[i]) -
                  coordinates.begin()].push_back(i);
    for (auto query : queries) if (!query.type)
        positions[std::lower_bound(coordinates.begin(), coordinates.end(), query.second) -
                  coordinates.begin()].push_back(query.first);
    std::vector<std::vector<int>> bit(m);
    for (int i = 0; i < m; ++i) {
        std::sort(positions[i].begin(), positions[i].end());
        positions[i].erase(std::unique(positions[i].begin(), positions[i].end()),
                           positions[i].end());
        bit[i].resize(positions[i].size() + 1);
    }
    auto add = [&](int id, int position, int delta) {
        int at = std::lower_bound(positions[id].begin(), positions[id].end(), position) -
                 positions[id].begin() + 1;
        for (; at < (int)bit[id].size(); at += at & -at) bit[id][at] += delta;
    };
    auto prefix = [&](int id, int position) {
        int at = std::lower_bound(positions[id].begin(), positions[id].end(), position) -
                 positions[id].begin(), result = 0;
        for (; at; at -= at & -at) result += bit[id][at];
        return result;
    };
    std::vector<int> ids(n);
    std::vector<Vote> leaves(n);
    for (int i = 0; i < n; ++i) {
        ids[i] = std::lower_bound(coordinates.begin(), coordinates.end(), values[i]) -
                 coordinates.begin();
        add(ids[i], i, 1); leaves[i] = {ids[i], 1};
    }
    toy::SegmentTree tree(leaves, Vote{}, MergeVote{});
    for (auto query : queries) {
        if (!query.type) {
            int id = std::lower_bound(coordinates.begin(), coordinates.end(), query.second) -
                     coordinates.begin();
            add(ids[query.first], query.first, -1);
            ids[query.first] = id; add(id, query.first, 1);
            tree.set(query.first, {id, 1});
        } else {
            int left = query.first, right = query.second;
            int id = tree.fold(left, right).value;
            int count = prefix(id, right) - prefix(id, left);
            if (count * 2 > right - left) output.writeln((toy::u64)coordinates[id]);
            else output.write("-1\n");
        }
    }
}
