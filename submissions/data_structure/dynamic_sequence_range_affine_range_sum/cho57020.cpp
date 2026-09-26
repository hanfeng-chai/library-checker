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
template <class M> struct weightbalancedtree_sequence {
	using S = typename M::S;
	using F = typename M::F;
	using u32 = uint32_t;
	struct Node {
		u32 ch[2], sz;
		S val;
		F lazy;
	};

	std::vector<Node> nodes;
	std::vector<S> leafs;
	std::vector<u32> p_node, p_leaf;

	static constexpr u32 nil = 0;
	static constexpr u32 LEAF_BIT = 1U << 31;
	static constexpr u32 REV_BIT = 1U << 31;
	static constexpr u32 FILTER = (1U << 31) - 1;

	struct Tree {
		u32 id;
		explicit Tree(u32 _id = nil) : id(_id) {};
		bool empty() const {
			return id == nil;
		}
	};

	u32 pop_node() {
		u32 res = p_node.back();
		p_node.pop_back();
		return res;
	}
	void push_node(u32 x) {
		p_node.push_back(x);
	}

	weightbalancedtree_sequence(int sz = 1)
		: nodes(sz), leafs(sz), p_node(sz - 1), p_leaf(sz) {
		for (int i = 1; i < sz; i++) p_node[i] = i;
		for (int i = 0; i < sz; i++) p_leaf[i] = i;

		nodes[nil].ch[0] = nodes[nil].ch[1] = nil;
		nodes[nil].sz = 0;
		nodes[nil].val = M::e();
		nodes[nil].lazy = M::id();
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
	void all_apply(u32 x, F f) {
		if (is_leaf(x)) {
			u32 id = x ^ LEAF_BIT;
			leafs[id] = M::mapping(f, leafs[id], 1);
		} else {
			nodes[x].val = M::mapping(f, nodes[x].val, nodes[x].sz & FILTER);
			nodes[x].lazy = M::composition(f, nodes[x].lazy);
		}
	}
	void all_reverse(u32 x) {
		if (!is_leaf(x)) {
			nodes[x].sz ^= REV_BIT;
			std::swap(nodes[x].ch[0], nodes[x].ch[1]);
			M::flip(nodes[x].val);
		} else M::flip(leafs[x ^ LEAF_BIT]);
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
	void update_with_size(u32 x, int lsz, int rsz) {
		nodes[x].sz = lsz + rsz;
		nodes[x].val =
			M::op(all_prod(nodes[x].ch[0]), all_prod(nodes[x].ch[1]), lsz, rsz);
	}

	static constexpr int DELTA = 12;
	static constexpr bool can_merge_directly(int sx, int sy) {
		return (sx * DELTA) >= sy;
	}
	static constexpr bool is_balanced(int sl, int sr) {
		return can_merge_directly(sl, sr) && can_merge_directly(sr, sl);
	}
	template <bool d> u32 balance_heavy(u32 t, u32 c, int sc) {
		push(c);
		int scc0 = size(nodes[c].ch[1 ^ d]);
		int scc1 = size(nodes[c].ch[d]);
		if (can_merge_directly(scc1, sc) &&
			can_merge_directly(scc0, scc1 + sc)) {
			u32 cc = nodes[c].ch[d];
			nodes[t].ch[1 ^ d] = cc;
			if constexpr (d) update_with_size(t, scc1, sc);
			else update_with_size(t, sc, scc1);
			nodes[c].ch[d] = t;
			if constexpr (d) update_with_size(c, scc0, scc1 + sc);
			else update_with_size(c, scc1 + sc, scc0);
			return c;
		}
		u32 cc = nodes[c].ch[d];
		push(cc);
		Node& nd_cc = nodes[cc];
		u32 ccl = nd_cc.ch[d], ccr = nd_cc.ch[1 ^ d];
		int sccl = size(ccl), sccr = size(ccr);
		nodes[t].ch[1 ^ d] = ccl;
		if constexpr (d) update_with_size(t, sccl, sc);
		else update_with_size(t, sc, sccl);
		nodes[c].ch[d] = ccr;
		if constexpr (d) update_with_size(c, scc0, sccr);
		else update_with_size(c, sccr, scc0);
		nd_cc.ch[d] = t;
		nd_cc.ch[1 ^ d] = c;
		if constexpr (d) update_with_size(cc, scc0 + sccr, sccl + sc);
		else update_with_size(cc, sc + sccl, sccr + scc0);
		return cc;
	}
	u32 balance(u32 t) {
		u32 l = nodes[t].ch[0], r = nodes[t].ch[1];
		int sl = size(l), sr = size(r);
		if (is_balanced(sl, sr)) {
			update_with_size(t, sl, sr);
			return t;
		}
		if (sl > sr) return balance_heavy<1>(t, l, sr);
		else return balance_heavy<0>(t, r, sl);
	}

	u32 merge_impl(u32 l, u32 r, u32 sp) {
		int sl = size(l), sr = size(r);
		if (is_balanced(sl, sr)) {
			nodes[sp].ch[0] = l;
			nodes[sp].ch[1] = r;
			nodes[sp].lazy = M::id();
			update_with_size(sp, sl, sr);
			return sp;
		}
		if (sl > sr) {
			push(l);
			nodes[l].ch[1] = merge_impl(nodes[l].ch[1], r, sp);
			return balance(l);
		} else {
			push(r);
			nodes[r].ch[0] = merge_impl(l, nodes[r].ch[0], sp);
			return balance(r);
		}
	}
	std::array<u32, 3> split_impl(u32 x, int k) {
		push(x);
		u32 l = nodes[x].ch[0], r = nodes[x].ch[1];
		int lsz = size(l);
		if (k == lsz) {
			return {l, x, r};
		} else if (k < lsz) {
			auto [t1, sp, t2] = split_impl(l, k);
			return {t1, sp, merge_impl(t2, r, x)};
		} else {
			auto [t1, sp, t2] = split_impl(r, k - lsz);
			return {merge_impl(l, t1, x), sp, t2};
		}
	}
	std::array<u32, 3> split3_impl(u32 t, int l, int r) {
		push(t);
		u32 ch0 = nodes[t].ch[0], ch1 = nodes[t].ch[1];
		int lsz = size(ch0);
		if (r == lsz) {
			auto [t1, sp, t2] = split_impl(ch0, l);
			push_node(t), push_node(sp);
			return {t1, t2, ch1};
		}
		if (l == lsz) {
			auto [t2, sp, t3] = split_impl(ch1, r - lsz);
			push_node(t), push_node(sp);
			return {ch0, t2, t3};
		}
		if (r < lsz) {
			auto ar = split3_impl(ch0, l, r);
			ar[2] = merge_impl(ar[2], ch1, t);
			return ar;
		}
		if (l > lsz) {
			auto ar = split3_impl(ch1, l - lsz, r - lsz);
			ar[0] = merge_impl(ch0, ar[0], t);
			return ar;
		}
		auto [t1, sp1, tl] = split_impl(ch0, l);
		auto [tr, sp2, t3] = split_impl(ch1, r - lsz);
		push_node(sp1), push_node(sp2);
		return {t1, merge_impl(tl, tr, t), t3};
	}

	u32 insert_impl(u32 t, int i, u32 x) {
		if (is_leaf(t)) {
			if (i == 0) return merge_impl(x, t, pop_node());
			return merge_impl(t, x, pop_node());
		}
		push(t);
		Node& nd = nodes[t];
		u32 l = nd.ch[0], r = nd.ch[1];
		int lsz = size(l);
		if (i < lsz) nd.ch[0] = insert_impl(l, i, x);
		else nd.ch[1] = insert_impl(r, i - lsz, x);
		return balance(t);
	}
	u32 erase_impl(u32 t, int i) {
		push(t);
		Node& nd = nodes[t];
		u32 l = nd.ch[0], r = nd.ch[1];
		int lsz = size(l);
		if (i < lsz) {
			if (is_leaf(l)) {
				p_leaf.push_back(l ^ LEAF_BIT);
				push_node(t);
				return r;
			}
			nd.ch[0] = erase_impl(l, i);
		} else {
			if (is_leaf(r)) {
				p_leaf.push_back(r ^ LEAF_BIT);
				push_node(t);
				return l;
			}
			nd.ch[1] = erase_impl(r, i - lsz);
		}
		return balance(t);
	}
	void set_impl(u32 t, int i, S x) {
		if (is_leaf(t)) {
			leafs[t ^ LEAF_BIT] = x;
			return;
		}
		push(t);
		Node& nd = nodes[t];
		u32 l = nd.ch[0], r = nd.ch[1];
		int lsz = size(l);
		if (i < lsz) set_impl(l, i, x);
		else set_impl(r, i - lsz, x);
		nd.val = M::op(all_prod(nd.ch[0]), all_prod(nd.ch[1]), lsz, size(r));
	}

	S prod_impl(u32 t, int sz, int ql, int qr) {
		if (ql <= 0 && sz <= qr) return all_prod(t);
		push(t);
		u32 ch0 = nodes[t].ch[0], ch1 = nodes[t].ch[1];
		int lsz = size(ch0);
		if (qr <= lsz) return prod_impl(ch0, lsz, ql, qr);
		if (ql >= lsz) return prod_impl(ch1, sz - lsz, ql - lsz, qr - lsz);
		return M::op(prod_impl(ch0, lsz, ql, lsz),
					 prod_impl(ch1, sz - lsz, 0, qr - lsz), lsz - ql, qr - lsz);
	}
	void apply_impl(u32 t, int sz, int ql, int qr, const F& f) {
		if (ql <= 0 && sz <= qr) {
			all_apply(t, f);
			return;
		}
		push(t);
		u32 ch0 = nodes[t].ch[0], ch1 = nodes[t].ch[1];
		int lsz = size(ch0);
		if (qr <= lsz) apply_impl(ch0, lsz, ql, qr, f);
		else if (ql >= lsz) apply_impl(ch1, sz - lsz, ql - lsz, qr - lsz, f);
		else {
			apply_impl(ch0, lsz, ql, lsz, f);
			apply_impl(ch1, sz - lsz, 0, qr - lsz, f);
		}
		nodes[t].val = M::op(all_prod(ch0), all_prod(ch1), lsz, size(ch1));
	}
	u32 exp_val(const S& val) {
		p_node.emplace_back(nodes.size());
		nodes.emplace_back();
		leafs.emplace_back(val);
		return (leafs.size() - 1) | LEAF_BIT;
	}
	u32 build_val(const S& val) {
		u32 id = p_leaf.back();
		p_leaf.pop_back();
		leafs[id] = val;
		return id | LEAF_BIT;
	}

   public:
	void reserve_nodes(int sz) {
		int n = leafs.size();
		if (n < sz) {
			nodes.resize(sz);
			leafs.resize(sz);
			for (int i = n; i < sz; i++) p_node.push_back(i);
			for (int i = n; i < sz; i++) p_leaf.push_back(i);
		}
	}
	void clear() {
		int sz = leafs.size();
		for (int i = 1; i < sz; i++) p_node[i] = i;
		for (int i = 0; i < sz; i++) p_leaf[i] = i;
	}
	Tree build() {
		return Tree();
	}
	Tree build(const S& val) {
		return Tree(p_leaf.empty() ? exp_val(val) : build_val(val));
	}
	Tree build(const std::vector<S>& v) {
		int n = v.size(), nw = p_leaf.size();
		if (n > nw) reserve_nodes(leafs.size() + n - nw);
		auto dfs = [&](auto self, int l, int r) -> u32 {
			if (r - l <= 0) return nil;
			if (r - l == 1) return build_val(v[l]);
			int m = (l + r) >> 1;
			return merge_impl(self(self, l, m), self(self, m, r), pop_node());
		};
		return Tree(dfs(dfs, 0, n));
	}

	int size(Tree t) {
		return size(t.id);
	}
	S all_prod(Tree t) {
		return all_prod(t.id);
	}
	void all_apply(Tree t, F f) {
		if (t.empty()) return;
		all_apply(t.id, f);
	}
	void all_reverse(Tree t) {
		if (t.empty()) return;
		all_reverse(t.id);
	}

	Tree merge(Tree l, Tree r) {
		if (l.empty()) return r;
		if (r.empty()) return l;
		return Tree(merge_impl(l.id, r.id, pop_node()));
	}
	std::pair<Tree, Tree> split(Tree t, int k) {
		if (k <= 0) return {nil, t};
		if (k >= size(t.id)) return {t, nil};
		auto [l, sp, r] = split_impl(t.id, k);
		push_node(sp);
		return {Tree(l), Tree(r)};
	}
	Tree merge3(Tree t1, Tree t2, Tree t3) {
		if (size(t1.id) > size(t3.id)) return merge(t1, merge(t2, t3));
		return merge(merge(t1, t2), t3);
	}
	std::array<Tree, 3> split3(Tree t, int l, int r) {
		assert(l <= r);
		int sz = size(t.id);
		if (l <= 0 && r <= 0) return {Tree(), Tree(), t};
		if (l <= 0 && r >= sz) return {Tree(), t, Tree()};
		if (l >= sz && r >= sz) return {t, Tree(), Tree()};
		if (l <= 0) {
			auto [t2, x, t3] = split_impl(t.id, r);
			push_node(x);
			return {Tree(), Tree(t2), Tree(t3)};
		}
		if (l == r) {
			auto [t1, x, t3] = split_impl(t.id, r);
			push_node(x);
			return {Tree(t1), Tree(), Tree(t3)};
		}
		if (r >= sz) {
			auto [t1, x, t2] = split_impl(t.id, l);
			push_node(x);
			return {Tree(t1), Tree(t2), Tree()};
		}
		auto [t1, t2, t3] = split3_impl(t.id, l, r);
		return {Tree(t1), Tree(t2), Tree(t3)};
	}

	void insert(Tree& t, int i, S x) {
		assert(0 <= i && i <= size(t.id));
		u32 id = p_leaf.empty() ? exp_val(x) : build_val(x);
		if (t.empty()) t.id = id;
		else t.id = insert_impl(t.id, i, id);
	}
	void erase(Tree& t, int i) {
		assert(0 <= i && i < size(t.id));
		if (is_leaf(t.id)) {
			p_leaf.push_back(t.id ^ LEAF_BIT);
			t.id = nil;
		} else t.id = erase_impl(t.id, i);
	}
	void push_front(Tree& t, S x) {
		insert(t, 0, x);
	}
	void push_back(Tree& t, S x) {
		insert(t, size(t.id), x);
	}
	void pop_front(Tree& t) {
		erase(t, 0);
	}
	void pop_back(Tree& t) {
		erase(t, size(t.id) - 1);
	}

	void set(Tree t, int i, S x) {
		assert(0 <= i && i < size(t.id));
		set_impl(t.id, i, x);
	}
	S get(Tree tree, int i) {
		u32 t = tree.id;
		assert(0 <= i && i < size(t));
		while (1) {
			if (is_leaf(t)) return leafs[t ^ LEAF_BIT];
			push(t);
			int lsz = size(nodes[t].ch[0]);
			if (i < lsz) t = nodes[t].ch[0];
			else {
				t = nodes[t].ch[1];
				i -= lsz;
			}
		}
	}

	S prod(Tree tree, int l, int r) {
		assert(0 <= l && l <= r && r <= size(tree.id));
		if (l == r) return M::e();
		return prod_impl(tree.id, size(tree.id), l, r);
	}
	void apply(Tree& tree, int l, int r, F f) {
		assert(0 <= l && l <= r && r <= size(tree.id));
		if (l == r) return;
		apply_impl(tree.id, size(tree.id), l, r, f);
	}
	void reverse(Tree& tree, int l, int r) {
		assert(0 <= l && l <= r && r <= size(tree.id));
		if (l == r) return;
		auto [t1, t2, t3] = split3(tree, l, r);
		all_reverse(t2.id);
		tree = merge3(t1, t2, t3);
	}

	std::vector<S> dump(Tree root) {
		std::vector<S> res;
		auto dfs = [&](auto self, u32 t) -> void {
			if (is_leaf(t)) res.push_back(leafs[t ^ LEAF_BIT]);
			else {
				push(t);
				self(self, nodes[t].ch[0]);
				self(self, nodes[t].ch[1]);
			}
		};
		res.reserve(size(root.id));
		if (!root.empty()) dfs(dfs, root.id);
		return res;
	}
};
}  // namespace cho

using namespace std;
using ll = long long;
#define rep(i, s, t) for (ll i = s; i < (ll)(t); i++)
#define all(x) begin(x), end(x)
template <class T> bool chmin(T& x, T y) {
	return x > y ? (x = y, true) : false;
}
template <class T> bool chmax(T& x, T y) {
	return x < y ? (x = y, true) : false;
}

constexpr int MOD = 998244353;
inline unsigned int ad(unsigned int a, unsigned int b) {
	a += b;
	return a >= MOD ? a - MOD : a;
}
inline unsigned int mu(unsigned int a, unsigned int b) {
	return (unsigned int)((unsigned long long)a * b % MOD);
}

struct MyMonoid {
	using S = long;
	using F = pair<unsigned int, unsigned int>;
	static S e() {
		return 0;
	}
	static S op(S l, S r, int, int) {
		return ad(l, r);
	}
	static S mapping(F f, S x, int sz) {
		return ad(mu(x, f.first), mu(f.second, sz));
	}
	static F composition(F f, const F& g) {
		return {mu(f.first, g.first), ad(mu(f.first, g.second), f.second)};
	}
	static F id() {
		return {1, 0};
	}
	static void flip(S&) {};
};

using S = MyMonoid::S;
using F = MyMonoid::F;

int main() {
	fastio::init();
	int n, q;
	rd(n);
	rd(q);
	vector<S> v(n);
	for (int i = 0; i < n; i++) rd(v[i]);
	cho::weightbalancedtree_sequence<MyMonoid> tree;
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
