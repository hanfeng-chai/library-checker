#include <bits/stdc++.h>

using namespace std;

namespace fastio {

const int buf_size = 1 << 14;
char buf_read[buf_size];
char buf_write[buf_size + 30];
char *ptr_read = buf_read + buf_size;
char *ptr_write = buf_write;

long long read_int() {
    auto getChar = []() {
        if (ptr_read == buf_read + buf_size){
            fread(buf_read, 1, buf_size, stdin);
            ptr_read = buf_read;
        }
        return *ptr_read++;
    };
    char c = getChar();
    while (c && (c < '0' || c > '9') && c != '-') {
        c = getChar();
    }
    long long z = 1;
    if (c == '-') {
        z = -1;
        c = getChar();
    }
    long long res = 0;
    while (c >= '0' && c <= '9'){
        res = res * 10 + (c - '0');
        c = getChar();
    }
    return z * res;
}

void write_flush() {
    fwrite(buf_write, 1, ptr_write - buf_write, stdout);
    ptr_write = buf_write;
}

void write_int(long long x) {
    if (x < 0) {
        *ptr_write++ = '-';
        x = -x;
    }
    char *start = ptr_write;
    if (!x) {
        *ptr_write++ = '0';
    }
    while (x) {
        *ptr_write++ = x % 10 + '0';
        x /= 10;
    }
    reverse(start, ptr_write);
    if (ptr_write >= buf_write + buf_size) {
        write_flush();
    }
}

void write_char(char c) {
    *ptr_write++ = c;
    if (ptr_write >= buf_write + buf_size) {
        write_flush();
    }
}

void write_string(const string &s) {
    for (char c : s) {
        write_char(c);
    }
}

}

template<typename Edge>
class GraphIterator {
public:
    class OutgoingEdges {
    public:
        OutgoingEdges(const GraphIterator *g, int l, int r): g(g), l(l), r(r) {
        }

        const Edge* begin() const {
            if (l == r) {
                return 0;
            }
            return &g->prepared_edges[l];
        }

        const Edge* end() const {
            if (l == r) {
                return 0;
            }
            return &g->prepared_edges[r];
        }

    private:
        int l, r;
        const GraphIterator *g;
    };

    void clear() {
        prepared_edges.clear();
        edges.clear();
        start.clear();
        prepared = false;
    }

    void add_edge(int from, const Edge &e) {
        assert(!prepared && from >= 0);
        edges.push_back({from, e});
    }

    void prepare() {
        assert(!prepared);
        int n = 0;
        for (const auto &e : edges) {
            n = max(n, e.first);
        }
        n += 2;
        start.resize(n);
        for (const auto &e : edges) {
            ++start[e.first + 1];
        }
        for (int i = 1; i < n; ++i) {
            start[i] += start[i - 1];
        }
        prepared_edges.resize(edges.size() + 1);
        auto counter = start;
        for (const auto &e : edges) {
            prepared_edges[counter[e.first]++] = e.second;
        }
        prepared = true;
    }

    OutgoingEdges operator [] (int from) const {
        assert(prepared);
        if (from < 0 || from + 1 >= start.size()) {
            return {this, 0, 0};
        }
        return {this, start[from], start[from + 1]};
    }

private:
    vector<Edge> prepared_edges;
    vector<pair<int, Edge>> edges;
    vector<int> start;
    bool prepared = false;
};

class two_sat {
public:
    two_sat(int n): answer(n, -1) {
    }

    void add_or_clause(int x, bool valx, int y, bool valy) {
        x = x * 2 + valx;
        y = y * 2 + valy;
        g.add_edge(x ^ 1, y);
        g.add_edge(y ^ 1, x);
    }

    pair<bool, vector<int>> solve() {
        vector<int> order(answer.size());
        iota(order.begin(),order.end(),0);
        mt19937 gen(123);
        shuffle(order.begin(),order.end(),gen);
        g.prepare();
        for (auto i:order) {
            if (answer[i] == -1) {
                lastv.clear();
                if (!dfs(2 * i)) {
                    for (int v : lastv) {
                        answer[v] = -1;
                    }
                    if (!dfs(2 * i + 1)) {
                        return {false, {}};
                    }
                }
            }
        }
        return {true, answer};
    }

    void clear() {
        g.clear();
        lastv.clear();
        fill(answer.begin(), answer.end(), -1);
    }

private:
    bool dfs(int v) {
        lastv.push_back(v / 2);
        answer[v / 2] = v % 2;
        for (int to : g[v]) {
            if ((answer[to / 2] != -1 && answer[to / 2] != to % 2) || (answer[to / 2] == -1 && !dfs(to))) {
                return false;
            }
        }
        return true;
    }

    vector<int> answer, lastv;
    GraphIterator<int> g;
};

int n, m;

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    n = fastio::read_int();
    m = fastio::read_int();
    two_sat ts(n);
    while (m--) {
        int a, c, x;
        a = fastio::read_int();
        c = fastio::read_int();
        x = fastio::read_int();
        bool b = a > 0;
        bool d = c > 0;
        a = abs(a) - 1;
        c = abs(c) - 1;
        ts.add_or_clause(a, b, c, d);
    }
    auto ans = ts.solve();
    if (ans.first) {
        fastio::write_string("s SATISFIABLE\nv ");
        for (int i = 0; i < n; ++i) {
            int x = i + 1;
            if (!ans.second[i]) {
                x = -x;
            }
            fastio::write_int(x);
            fastio::write_char(' ');
        }
        fastio::write_string("0\n");
    } else {
        fastio::write_string("s UNSATISFIABLE");
    }
    fastio::write_flush();
    return 0;
}
