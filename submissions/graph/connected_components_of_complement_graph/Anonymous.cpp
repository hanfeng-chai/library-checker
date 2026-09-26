#include <queue>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

class scanner {
public:
	static constexpr int buffer_size = 1 << 17;
	static constexpr int buffer_capacity = buffer_size + (1 << 6);
private:
	int pos;
	char buf[buffer_capacity];
public:
	// constructor
	scanner() : pos(buffer_capacity) {}
	// read a character
	char read() {
		if (pos >= buffer_size) {
			int rem = buffer_capacity - pos;
			memcpy(buf, buf + pos, rem);
			fread(buf + rem, sizeof(char), pos, stdin);
			pos = 0;
		}
		return buf[pos++];
	}
	// read an int64_t integer
	int64_t readint() {
		if (pos >= buffer_size) {
			int rem = buffer_capacity - pos;
			memcpy(buf, buf + pos, rem);
			fread(buf + rem, sizeof(char), pos, stdin);
			pos = 0;
		}
		int64_t x = 0;
		char c = buf[pos++], isplus = 1;
		if (c == '-') {
			isplus = 0;
			c = buf[pos++];
		}
		while ('0' <= c && c <= '9') {
			x = x * 10 + int(c - '0');
			c = buf[pos++];
		}
		return (isplus ? x : -x);
	}
};

class printer {
public:
	static constexpr int buffer_size = 1 << 17;
	static constexpr int buffer_capacity = buffer_size + (1 << 6);
private:
	int pos;
	char buf[buffer_capacity];
	char num_table[40000];
public:
	// number of digits (x >= 0)
	int num_digits(int64_t x) {
		if (x < 10) return 1;
		if (x < 100) return 2;
		if (x < 1000) return 3;
		if (x < 10000) return 4;
		if (x < 100000) return 5;
		if (x < 1000000) return 6;
		if (x < 10000000) return 7;
		if (x < 100000000) return 8;
		if (x < 1000000000) return 9;
		if (x < 10000000000) return 10;
		if (x < 100000000000) return 11;
		if (x < 1000000000000) return 12;
		if (x < 10000000000000) return 13;
		if (x < 100000000000000) return 14;
		if (x < 1000000000000000) return 15;
		if (x < 10000000000000000) return 16;
		if (x < 100000000000000000) return 17;
		if (x < 1000000000000000000) return 18;
		return 19;
	}
	// constructor
	printer() : pos(0) {
		for (int i = 0; i < 10000; i++) {
			int x = i;
			for (int j = 3; j >= 0; j--) {
				num_table[i * 4 + j] = x % 10 + '0';
				x /= 10;
			}
		}
	}
	// destructor
	~printer() {
		flush();
	}
	// flush stream
	void flush() {
		fwrite(buf, sizeof(char), pos, stdout);
		pos = 0;
	}
	// write a character
	void write(char c) {
		buf[pos++] = c;
		if (pos >= buffer_size) {
			flush();
		}
	}
	// write an int64_t integer
	void writeint(int64_t x, char end = '\0') {
		if (x < 0) {
			buf[pos++] = '-';
			x = -x;
		}
		int d = num_digits(x);
		for (int i = d - 4; i >= 1; i -= 4) {
			int g = x % 10000;
			x /= 10000;
			memcpy(buf + pos + i, num_table + g * 4, 4);
		}
		int rem = (d - 1) % 4 + 1;
		memcpy(buf + pos, num_table + int(x) * 4 + (4 - rem), rem);
		pos += d;
		if (end != '\0') {
			buf[pos++] = end;
		}
		if (pos >= buffer_size) {
			flush();
		}
	}
};

struct edge {
	int s, t;
};

class graph {
private:
	int n, m;
	vector<edge> es;
	vector<int> index;
	vector<int> adj;
public:
	graph() : es(), adj() {}
	graph(int n_, const vector<edge>& es_) : n(n_), m(es_.size()), es(es_) {
		index.resize(max(n, m) + 1);
		adj.resize(m);
		for (int i = 0; i < m; i++) {
			index[es[i].s]++;
		}
		for (int i = 0; i < n; i++) {
			index[i + 1] += index[i];
		}
		for (int i = m - 1; i >= 0; i--) {
			adj[--index[es[i].s]] = i;
		}
	}
	int V() const { return n; }
	int E() const { return m; }
	int degree(int x) const { return index[x + 1] - index[x]; }
	edge get_edge(int id) const { return es[id]; }
	int get_id(int x, int rank) const { return adj[index[x] + rank]; }
	edge get_edge(int x, int rank) const { return es[adj[index[x] + rank]]; }
};

pair<int, vector<int> > complement_components(const graph& G) {
	int num = 0;
	vector<int> comp(G.V(), -1);
	vector<int> que(G.V());
	vector<int> remain(G.V());
	for (int i = 0; i < G.V(); i++) {
		remain[i] = i;
	}
	int remsize = G.V();
	for (int i = 0; i < G.V(); i++) {
		if (comp[i] < 0) {
			int ql = 0, qr = 0;
			que[qr++] = i;
			comp[i] = num;
			while (ql != qr) {
				int u = que[ql++];
				for (int j = 0; j < G.degree(u); j++) {
					int t = G.get_edge(u, j).t;
					if (comp[t] < 0) {
						comp[t] = -(u + 2);
					}
				}
				int next_remsize = 0;
				for (int j = 0; j < remsize; j++) {
					int t = remain[j];
					if (comp[t] != -(u + 2)) {
						comp[t] = num;
						que[qr++] = t;
					} else {
						remain[next_remsize++] = t;
					}
				}
				remsize = next_remsize;
			}
			num++;
		}
	}
	return {num, comp};
}

int main() {
	scanner sc;
	printer pr;
	int N = sc.readint();
	int M = sc.readint();
	vector<edge> es(2 * M);
	for (int i = 0; i < M; i++) {
		es[i].s = sc.readint();
		es[i].t = sc.readint();
		es[i + M] = edge{es[i].t, es[i].s};
	}
	graph G(N, es);
	auto [K, comp] = complement_components(G);
	vector<int> cnt(K + 1);
	for (int i = 0; i < N; i++) {
		cnt[comp[i]]++;
	}
	for (int i = 0; i < K; i++) {
		cnt[i + 1] += cnt[i];
	}
	vector<int> ans(N);
	for (int i = N - 1; i >= 0; i--) {
		ans[--cnt[comp[i]]] = i;
	}
	pr.writeint(K, '\n');
	for (int i = 0; i < K; i++) {
		pr.writeint(cnt[i + 1] - cnt[i], ' ');
		for (int j = cnt[i]; j < cnt[i + 1]; j++) {
			pr.writeint(ans[j], j + 1 != cnt[i + 1] ? ' ' : '\n');
		}
	}
	return 0;
}