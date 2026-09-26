#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
// https://nyaannyaan.github.io/library/misc/fastio.hpp
namespace nyaan {
namespace internal {
using namespace std;
template <typename T>
using is_broadly_integral =
	typename conditional_t<is_integral_v<T> || is_same_v<T, __int128_t> ||
							   is_same_v<T, __uint128_t>,
						   true_type,
						   false_type>::type;

template <typename T>
using is_broadly_signed =
	typename conditional_t<is_signed_v<T> || is_same_v<T, __int128_t>,
						   true_type,
						   false_type>::type;

template <typename T>
using is_broadly_unsigned =
	typename conditional_t<is_unsigned_v<T> || is_same_v<T, __uint128_t>,
						   true_type,
						   false_type>::type;

#define ENABLE_VALUE(x)   \
	template <typename T> \
	constexpr bool x##_v = x<T>::value;

ENABLE_VALUE(is_broadly_integral);
ENABLE_VALUE(is_broadly_signed);
ENABLE_VALUE(is_broadly_unsigned);
#undef ENABLE_VALUE

#define ENABLE_HAS_TYPE(var)                                     \
	template <class, class = void>                               \
	struct has_##var : false_type {};                            \
	template <class T>                                           \
	struct has_##var<T, void_t<typename T::var>> : true_type {}; \
	template <class T>                                           \
	constexpr auto has_##var##_v = has_##var<T>::value;

#define ENABLE_HAS_VAR(var)                                       \
	template <class, class = void>                                \
	struct has_##var : false_type {};                             \
	template <class T>                                            \
	struct has_##var<T, void_t<decltype(T::var)>> : true_type {}; \
	template <class T>                                            \
	constexpr auto has_##var##_v = has_##var<T>::value;
}  // namespace internal

namespace fastio {
using namespace std;
static constexpr int SZ = 1 << 17;
static constexpr int offset = 64;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre {
	char num[40000];
	constexpr Pre() : num() {
		for (int i = 0; i < 10000; i++) {
			int n = i;
			for (int j = 3; j >= 0; j--) {
				num[i * 4 + j] = n % 10 + '0';
				n /= 10;
			}
		}
	}
} constexpr pre;

void load() {
	int len = in_right - in_left;
	memmove(inbuf, inbuf + in_left, len);
	in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
	in_left = 0;
}
void flush() {
	fwrite(outbuf, 1, out_right, stdout);
	out_right = 0;
}
void skip_space() {
	if (in_left + offset > in_right) load();
	while (inbuf[in_left] <= ' ') in_left++;
}

void single_read(char& c) {
	if (in_left + offset > in_right) load();
	skip_space();
	c = inbuf[in_left++];
}
void single_read(string& S) {
	skip_space();
	while (true) {
		if (in_left == in_right) load();
		int i = in_left;
		for (; i != in_right; i++) {
			if (inbuf[i] <= ' ') break;
		}
		copy(inbuf + in_left, inbuf + i, back_inserter(S));
		in_left = i;
		if (i != in_right) break;
	}
}
template <typename T,
		  enable_if_t<internal::is_broadly_integral_v<T>>* = nullptr>
void single_read(T& x) {
	if (in_left + offset > in_right) load();
	skip_space();
	char c = inbuf[in_left++];
	[[maybe_unused]] bool minus = false;
	if constexpr (internal::is_broadly_signed_v<T>) {
		if (c == '-') minus = true, c = inbuf[in_left++];
	}
	x = 0;
	while (c >= '0') {
		x = x * 10 + (c & 15);
		c = inbuf[in_left++];
	}
	if constexpr (internal::is_broadly_signed_v<T>) {
		if (minus) x = -x;
	}
}
void rd() {
}
template <typename Head, typename... Tail>
void rd(Head& head, Tail&... tail) {
	single_read(head);
	rd(tail...);
}

void single_write(const char& c) {
	if (out_right > SZ - offset) flush();
	outbuf[out_right++] = c;
}
void single_write(const bool& b) {
	if (out_right > SZ - offset) flush();
	outbuf[out_right++] = b ? '1' : '0';
}
void single_write(const string& S) {
	flush(), fwrite(S.data(), 1, S.size(), stdout);
}
void single_write(const char* p) {
	flush(), fwrite(p, 1, strlen(p), stdout);
}
template <typename T,
		  enable_if_t<internal::is_broadly_integral_v<T>>* = nullptr>
void single_write(const T& _x) {
	if (out_right > SZ - offset) flush();
	if (_x == 0) {
		outbuf[out_right++] = '0';
		return;
	}
	T x = _x;
	if constexpr (internal::is_broadly_signed_v<T>) {
		if (x < 0) outbuf[out_right++] = '-', x = -x;
	}
	constexpr int buffer_size = sizeof(T) * 10 / 4;
	char buf[buffer_size];
	int i = buffer_size;
	while (x >= 10000) {
		i -= 4;
		memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
		x /= 10000;
	}
	if (x < 100) {
		if (x < 10) {
			outbuf[out_right] = '0' + x;
			++out_right;
		} else {
			uint32_t q = (uint32_t(x) * 205) >> 11;
			uint32_t r = uint32_t(x) - q * 10;
			outbuf[out_right] = '0' + q;
			outbuf[out_right + 1] = '0' + r;
			out_right += 2;
		}
	} else {
		if (x < 1000) {
			memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
			out_right += 3;
		} else {
			memcpy(outbuf + out_right, pre.num + (x << 2), 4);
			out_right += 4;
		}
	}
	memcpy(outbuf + out_right, buf + i, buffer_size - i);
	out_right += buffer_size - i;
}
void wt() {
}
template <typename Head, typename... Tail>
void wt(const Head& head, const Tail&... tail) {
	single_write(head);
	if constexpr (sizeof...(tail)) wt(' ');
	wt(std::forward<const Tail>(tail)...);
}
template <typename... Args>
void wtn(const Args&... x) {
	wt(std::forward<const Args>(x)...);
	wt('\n');
}
struct Dummy {
	Dummy() {
		atexit(flush);
	}
} dummy;
#define cin CANNOT_USE_WITH_FASTIO
#define cout CANNOT_USE_WITH_FASTIO
}  // namespace fastio
}  // namespace nyaan

using nyaan::fastio::rd;
using nyaan::fastio::skip_space;
using nyaan::fastio::wt;
using nyaan::fastio::wtn;

namespace cho {
template <class S,
		  auto op,
		  class F,
		  auto mapping,
		  auto composition,
		  auto id,
		  auto flip>
struct splay_node {
	splay_node *l, *r, *p;
	S val, prod;
	F lazy;
	int size;
	bool rev;
	splay_node(const S& x = {}) {
		l = r = p = nullptr;
		val = prod = x;
		lazy = id();
		size = 1;
		rev = 0;
	};
	bool is_root() const {
		return ((!p) || (p->l != this && p->r != this));
	}
	void all_apply(F f) {
		val = mapping(f, val);
		prod = mapping(f, prod);
		if (size > 1) lazy = composition(f, lazy);
	}
	void toggle() {
		rev ^= 1;
		std::swap(l, r);
		flip(prod);
	}
	void push() {
		if (lazy != id()) {
			if (l) l->all_apply(lazy);
			if (r) r->all_apply(lazy);
			lazy = id();
		}
		if (rev) {
			if (l) l->toggle();
			if (r) r->toggle();
			rev = 0;
		}
	}
	void update() {
		size = 1, prod = val;
		if (l) prod = op(l->prod, prod), size += l->size;
		if (r) prod = op(prod, r->prod), size += r->size;
	}
	int pos() {
		if (p && p->l == this) return -1;
		if (p && p->r == this) return 1;
		return 0;
	}
	void rotate() {
		auto x = p;
		size = x->size, prod = x->prod;
		if (pos() == -1) {
			if ((x->l = r)) r->p = x;
			r = x;
		} else {
			if ((x->r = l)) l->p = x;
			l = x;
		}
		p = x->p;
		x->p = this;
		x->update();
		if (p && p->l == x) p->l = this;
		if (p && p->r == x) p->r = this;
	}
	void splay() {
		while (!is_root()) {
			if (p->is_root()) {
				p->push(), push();
				rotate();
			} else {
				p->p->push(), p->push(), push();
				if (pos() == p->pos()) p->rotate(), rotate();
				else rotate(), rotate();
			}
		}
		push();
	}
};

template <class TREE_NODE>
TREE_NODE* merge(TREE_NODE* l, TREE_NODE* r) {
	if (!l) return r;
	if (!r) return l;
	l->push();
	while (l->r) l = l->r, l->push();
	l->splay();
	l->r = r;
	r->p = l;
	l->update();
	return l;
}

template <class TREE_NODE>
std::pair<TREE_NODE*, TREE_NODE*> split(TREE_NODE* t, int k) {
	if (!t) return {nullptr, nullptr};
	if (k <= 0) return {nullptr, t};
	if (k >= t->size) return {t, nullptr};
	while (1) {
		t->push();
		int lsz = t->l ? t->l->size : 0;
		if (k == lsz) break;
		else if (k < lsz) t = t->l;
		else {
			k -= lsz + 1;
			t = t->r;
		}
	}
	t->splay();
	auto l = t->l;
	t->l = l->p = nullptr;
	t->update();
	return {l, t};
}
}  // namespace cho

namespace cho {
template <class S,
		  auto op,
		  class F,
		  auto mapping,
		  auto composition,
		  auto id,
		  auto flip>
struct linkcut_tree {
	using node = splay_node<S, op, F, mapping, composition, id, flip>;
	std::vector<node> vertex;
	int n;
	linkcut_tree() {};
	linkcut_tree(int n_, S e) : linkcut_tree(std::vector<S>(n_, e)) {};
	linkcut_tree(std::vector<S>& v) : vertex(v.size()), n(v.size()) {
		for (int i = 0; i < n; i++) vertex[i].val = vertex[i].prod = v[i];
	}
	int add_vertex(S val) {
		vertex.emplace_back(val);
		return n++;
	}
	node* expose(int v) {
		node* t = &vertex[v];
		node* bf = nullptr;
		for (; t; t = t->p) {
			t->splay();
			t->r = bf;
			t->update();
			bf = t;
		}
		return bf;
	}
	void reroot(int t) {
		expose(t)->toggle();
	}
	void link(int u, int v) {
		reroot(u);
		vertex[u].splay();
		// expose(v);
		vertex[u].p = &vertex[v];
	}
	void cut(int u, int v) {
		reroot(u);
		vertex[u].splay();
		expose(v);
		vertex[v].splay();
		vertex[v].l = vertex[u].p = nullptr;
	}
	// u->v のパス
	S prod(int u, int v) {
		reroot(u);
		return expose(v)->prod;
	}
	void apply(int u, int v, F f) {
		reroot(u);
		expose(v)->all_apply(f);
	}
	void set(int v, S val) {
		vertex[v].splay();
		vertex[v].val = val;
		vertex[v].update();
	}
	S get(int v) {
		return vertex[v].val;
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

using u32 = unsigned int;
struct S {
	u32 a, b, rb;
};

S b_S(u32 a, u32 b) {
	return S{a, b, b};
}

S op(S l, S r) {
	return {mu(l.a, r.a), ad(mu(l.a, r.b), l.b), ad(mu(r.a, l.rb), r.rb)};
}
using F = bool;
S mapping(F, S) {
	return {};
}
F composition(F, F) {
	return 0;
}
F id() {
	return 0;
}
void flip(S& a) {
	swap(a.b, a.rb);
}

using lct = cho::linkcut_tree<S, op, F, mapping, composition, id, flip>;

int main() {
	u32 n, q;
	rd(n), rd(q);
	vector<S> tmp(n);
	rep(i, 0, n) {
		u32 a, b;
		rd(a), rd(b);
		tmp[i] = b_S(a, b);
	}
	lct g(tmp);
	rep(i, 0, n - 1) {
		u32 u, v;
		rd(u), rd(v);
		g.link(u, v);
	}
	u32 t, a, b, c, d;
	rep(Qi, 0, q) {
		rd(t);
		if (t == 0) {
			rd(a), rd(b), rd(c), rd(d);
			g.cut(a, b);
			g.link(c, d);
		} else if (t == 1) {
			rd(a), rd(b), rd(c);
			g.set(a, b_S(b, c));
		} else {
			rd(a), rd(b), rd(c);
			auto res = g.prod(a, b);
			wtn(ad(mu(res.a, c), res.rb));
		}
	}
}
