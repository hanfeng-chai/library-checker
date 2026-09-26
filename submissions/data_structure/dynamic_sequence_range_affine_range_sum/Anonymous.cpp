#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace fastio {
using namespace std;
static char* p;
void init() {
	struct stat st;
	fstat(0, &st);
	p = (char*)mmap(0, st.st_size, PROT_READ, MAP_SHARED, 0, 0);
}
template <typename T>
inline void rd(T& x) {
	x = 0;
	while (*p < '0' || *p > '9') p++;
	while (*p >= '0' && *p <= '9') {
		x = x * 10 + (*p - '0');
		p++;
	}
}
static char outbuf[1 << 20];
int out_p = 0;
inline void flush() {
	if (out_p) fwrite(outbuf, 1, out_p, stdout);
	out_p = 0;
}
inline void put(char c) {
	if (out_p == 1 << 20) flush();
	outbuf[out_p++] = c;
}
template <typename T>
inline void wt(T x) {
	if (x == 0) {
		if (out_p == 1 << 20) flush();
		outbuf[out_p++] = '0';
		return;
	}
	static char s[12];
	int i = 0;
	while (x) {
		s[i++] = x % 10 + '0';
		x /= 10;
	}
	while (i--) {
		if (out_p == 1 << 20) flush();
		outbuf[out_p++] = s[i];
	}
}
struct D {
	~D() {
		flush();
	}
} d;
}  // namespace fastio

using fastio::put;
using fastio::rd;
using fastio::wt;

namespace cho {
template <class M, int MAX_N = 1'000'100>
struct splaytree_array {
	using S = typename M::S;
	using F = typename M::F;
	using u32 = uint32_t;

	struct Node {
		u32 ch[2], sz;
		S val;
		F lazy;
	};

	Node* nodes;
	S* leafs;
	std::vector<int> p_node, p_leaf;

	static constexpr u32 nil = -1U;
	static constexpr u32 LEAF_BIT = 1U << 31;
	static constexpr u32 REV_BIT = 1U << 31;
	static constexpr u32 FILTER = (1U << 31) - 1;

	splaytree_array()
		: nodes(new Node[MAX_N]),
		  leafs(new S[MAX_N]),
		  p_node(MAX_N),
		  p_leaf(MAX_N) {
		std::iota(p_node.begin(), p_node.end(), 0);
		std::iota(p_leaf.begin(), p_leaf.end(), 0);
	}

	static bool is_leaf(u32 x) {
		return (x & LEAF_BIT);
	}
	int size(u32 x) {
		return is_leaf(x) ? 1 : (nodes[x].sz & FILTER);
	}
	S all_prod(u32 x) {
		return is_leaf(x) ? leafs[x ^ LEAF_BIT] : nodes[x].val;
	}
	void all_apply(u32 x, const F& f) {
		if (is_leaf(x)) {
			u32 id = x ^ LEAF_BIT;
			leafs[id] = M::mapping(f, leafs[id], 1);
		} else {
			nodes[x].val = M::mapping(f, nodes[x].val, (nodes[x].sz & FILTER));
			nodes[x].lazy = M::composition(f, nodes[x].lazy);
		}
	}
	void all_reverse(u32 x) {
		if (!is_leaf(x)) {
			nodes[x].sz ^= REV_BIT;
			std::swap(nodes[x].ch[0], nodes[x].ch[1]);
			M::flip(nodes[x].val);
		} else {
			// M::flip(leafs[x ^ LEAF_BIT]);
		}
	}
	void push(u32 x) {
		if (nodes[x].sz & REV_BIT) {
			all_reverse(nodes[x].ch[0]);
			all_reverse(nodes[x].ch[1]);
			nodes[x].sz ^= REV_BIT;
		}
		if (nodes[x].lazy != M::id()) {
			all_apply(nodes[x].ch[0], nodes[x].lazy);
			all_apply(nodes[x].ch[1], nodes[x].lazy);
			nodes[x].lazy = M::id();
		}
	}

	void update(u32 x) {
		u32 l = nodes[x].ch[0], r = nodes[x].ch[1];
		nodes[x].sz = size(l) + size(r);
		nodes[x].val = M::op(all_prod(l), all_prod(r));
	}

	void splay_kth(u32& t, int k) {
		static std::array<std::vector<u32>, 2> lr;
		int zig = -1;
		while (1) {
			push(t);
			u32 l = nodes[t].ch[0], r = nodes[t].ch[1];
			int lsz = size(l);
			if (k == lsz) break;
			if (k < lsz) {
				if (zig == 0) {
					u32 p = lr[1].back();
					lr[1].pop_back();
					nodes[p].ch[0] = r;
					update(p);
					nodes[t].ch[1] = p;
					zig = -1;
				} else zig = 0;
				lr[1].push_back(t);
				t = l;
			} else {
				k -= lsz;
				if (zig == 1) {
					u32 p = lr[0].back();
					lr[0].pop_back();
					nodes[p].ch[1] = l;
					update(p);
					nodes[t].ch[0] = p;
					zig = -1;
				} else zig = 1;
				lr[0].push_back(t);
				t = r;
			}
		}
		u32 l = nodes[t].ch[0], r = nodes[t].ch[1];
		while (!lr[0].empty()) {
			nodes[lr[0].back()].ch[1] = l;
			l = lr[0].back();
			update(l);
			lr[0].pop_back();
		}
		while (!lr[1].empty()) {
			nodes[lr[1].back()].ch[0] = r;
			r = lr[1].back();
			update(r);
			lr[1].pop_back();
		}
		nodes[t].ch[0] = l;
		nodes[t].ch[1] = r;
		update(t);
	}

	u32 merge(u32 l, u32 r) {
		if (l == nil) return r;
		if (r == nil) return l;
		u32 x = p_node.back();
		p_node.pop_back();
		nodes[x].ch[0] = l, nodes[x].ch[1] = r;
		nodes[x].lazy = M::id();
		update(x);
		return x;
	}
	std::pair<u32, u32> split(u32 t, int k) {
		if (k <= 0) return {nil, t};
		if (k >= size(t)) return {t, nil};
		splay_kth(t, k);
		u32 l = nodes[t].ch[0], r = nodes[t].ch[1];
		p_node.push_back(t);
		return {l, r};
	}
	u32 merge3(u32 t1, u32 t2, u32 t3) {
		return merge(merge(t1, t2), t3);
	}
	std::array<u32, 3> split3(u32 t, int l, int r) {
		auto [t_, t3] = split(t, r);
		auto [t1, t2] = split(t_, l);
		return {t1, t2, t3};
	}

	u32 build() {
		return nil;
	}
	u32 build(const S& val) {
		u32 id = p_leaf.back();
		p_leaf.pop_back();
		leafs[id] = val;
		return id | LEAF_BIT;
	}
	u32 build(const std::vector<S>& v) {
		if (v.empty()) return nil;
		auto dfs = [&](auto self, int l, int r) -> u32 {
			if (r - l == 1) return build(v[l]);
			int m = (l + r) >> 1;
			return merge(self(self, l, m), self(self, m, r));
		};
		return dfs(dfs, 0, v.size());
	}

	void insert(u32& nd, int i, S x) {
		assert(0 <= i && i <= size(nd));
		auto [l, r] = split(nd, i);
		nd = merge3(l, build(x), r);
	}
	void erase(u32& nd, int i) {
		assert(0 <= i && i < size(nd));
		auto [t1, t2, t3] = split3(nd, i, i + 1);
		p_leaf.push_back(t2 ^ LEAF_BIT);
		nd = merge(t1, t3);
	}
	void reverse(u32& nd, int l, int r) {
		assert(0 <= l && l <= r && r <= size(nd));
		if (l == r) return;
		auto [t1, t2, t3] = split3(nd, l, r);
		all_reverse(t2);
		nd = merge3(t1, t2, t3);
	}
	void apply(u32& nd, int l, int r, const F& f) {
		assert(0 <= l && l <= r && r <= size(nd));
		if (l == r) return;
		auto [t1, t2, t3] = split3(nd, l, r);
		all_apply(t2, f);
		nd = merge3(t1, t2, t3);
	}
	S prod(u32& nd, int l, int r) {
		assert(0 <= l && l <= r && r <= size(nd));
		if (l == r) return M::e();
		auto [t1, t2, t3] = split3(nd, l, r);
		S res = all_prod(t2);
		nd = merge3(t1, t2, t3);
		return res;
	}
};
}  // namespace cho

using namespace std;

constexpr int MOD = 998244353;
unsigned int ad(unsigned int a, unsigned int b) {
	a += b;
	return a >= MOD ? a - MOD : a;
}
unsigned int mu(unsigned int a, unsigned int b) {
	return (unsigned int)((unsigned long long)a * b % MOD);
}

struct MyMonoid {
	using S = unsigned int;
	using F = pair<unsigned int, unsigned int>;
	static S e() {
		return 0;
	}
	static S op(S a, S b) {
		return ad(a, b);
	}
	static S mapping(const F& f, S s, int sz) {
		return ad(mu(s, f.first), mu(f.second, sz));
	}
	static F composition(const F& f, const F& g) {
		return {mu(f.first, g.first), ad(mu(f.first, g.second), f.second)};
	}
	static F id() {
		return {1, 0};
	}
	static void flip(S&) {};
};

int main() {
	fastio::init();
	int n, q;
	rd(n);
	rd(q);
	vector<unsigned> v(n);
	for (int i = 0; i < n; i++) rd(v[i]);
	cho::splaytree_array<MyMonoid> tree;
	auto nd = tree.build(v);
	while (q--) {
		int ty, l, r, b, c;
		rd(ty);
		if (ty == 0) {
			rd(l);
			rd(r);
			tree.insert(nd, l, r);
		} else if (ty == 1) {
			rd(l);
			tree.erase(nd, l);
		} else if (ty == 2) {
			rd(l);
			rd(r);
			tree.reverse(nd, l, r);
		} else if (ty == 3) {
			rd(l);
			rd(r);
			rd(b);
			rd(c);
			tree.apply(nd, l, r, {b, c});
		} else {
			rd(l);
			rd(r);
			wt(tree.prod(nd, l, r));
			put('\n');
		}
	}
}
