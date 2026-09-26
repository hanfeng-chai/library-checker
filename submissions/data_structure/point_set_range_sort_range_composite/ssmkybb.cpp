#include <bits/stdc++.h>
using namespace std;

struct FIstream{ static constexpr unsigned SIZ = 1 << 17; char buf[SIZ], *p1 = buf, *p2 = buf; inline char _getchar(){ if(p1 == p2){ p2 = (p1 = buf) + fread(buf, 1, SIZ, stdin); if(p1 == p2) [[unlikely]] assert(0&&"EOF"); } return *p1++; } inline char ignore_space(){ char c; while((c = _getchar()) <= 0x20); return c; } template<typename T> inline void _read(T& res){ T x = 0, f = 1; char c = ignore_space(); if(c == '-'){ f = -1; c = _getchar(); } while('0' <= c && c <= '9'){ x = x*10 + (c-'0'); c = _getchar(); } res = x*f; } template<typename T> inline FIstream& operator>>(T& x){_read(x); return *this;} inline FIstream& operator>>(char& x){x = ignore_space(); return *this;} inline FIstream& operator>>(string& x){ string().swap(x); char c = ignore_space(); while(c > 0x20){ x.push_back(c); c = _getchar(); } return *this; } } _cin; struct FOstream_Pre{ char num[10000][4]; constexpr FOstream_Pre():num(){ for(int i = 0; i < 10000; i++){ int x = i; for(int j = 3; j >= 0; j--){ num[i][j] = x%10 + '0'; x /= 10; } } } } constexpr _FOstream_pre; struct FOstream{ static constexpr unsigned SIZ = 1 << 17; char buf[SIZ], *p1 = buf, *p2 = buf+SIZ; inline void _write(){ fwrite(buf, 1, p1-buf, stdout); p1 = buf; } inline void _putchar(char c){ if(p1 == p2) [[unlikely]] { _write(); } *p1++ = c; } template<typename T> void _write_i(T x){ constexpr int DIGIT_SIZ = 40; static_assert(DIGIT_SIZ <= SIZ); char num[DIGIT_SIZ], *idxp = num+DIGIT_SIZ; if(x < 0){ _putchar('-'); x = -x; } if(p2 - p1 < DIGIT_SIZ) _write(); while(x >= 10000){ idxp -= 4; memcpy(idxp, _FOstream_pre.num[size_t(x%10000)], 4); x /= 10000; } if(x >= 1000){ memcpy(p1, _FOstream_pre.num[size_t(x)], 4); p1 += 4; } else if(x >= 100){ memcpy(p1, _FOstream_pre.num[size_t(x)]+1, 3); p1 += 3; } else if(x >= 10){ memcpy(p1, _FOstream_pre.num[size_t(x)]+2, 2); p1 += 2; } else *p1++ = char(x)+'0'; memcpy(p1, idxp, num+DIGIT_SIZ - idxp); p1 += num+DIGIT_SIZ - idxp; } template<typename T> FOstream &operator<<(const T &x) {_write_i(x); return *this;} FOstream &operator<<(char x) {_putchar(x); return *this;} FOstream &operator<<(const char *x) { while(*x) _putchar(*x++); return *this; } FOstream &operator<<(char *x) {return *this << const_cast<const char*>(x);} FOstream &operator<<(double x) { if(isnan(x)) [[unlikely]] return *this << "nan"; char _b[70]; snprintf(_b, sizeof(_b), "%.*f", 15, x); return *this << const_cast<const char*>(_b); } FOstream &operator<<(long double x) { if(isnan(x)) [[unlikely]] return *this << "nan"; char _b[330]; snprintf(_b, sizeof(_b), "%.*Lf", 15, x); return *this << const_cast<const char*>(_b); } FOstream& operator<<(const string& x){ for(char i : x) _putchar(i); return *this; } ~FOstream(){ if(p1 != buf){ fwrite(buf, 1, p1 - buf, stdout); } } } _cout;
#define cin _cin
#define istream FIstream
#define cout _cout
#define ostream FOstream

namespace smkyb {

template<typename T, typename S, auto op, auto e>
struct meldable_binary_trie{
    static_assert(is_unsigned_v<T>);
    
    static constexpr T one = 1;
    static constexpr int bit_width = sizeof(T) * 8;
    
    struct node_t{
        T value;
        S sum;
        int width;
        int count;
        array<node_t*, 2> child;
        node_t() = default;
        node_t(T v, S s, int w, int c) : value(v), sum(s), width(w), count(c), child{&nil, &nil} {}
        node_t(T v, S s, int w, int c, node_t* c0, node_t* c1) : value(v), sum(s), width(w), count(c), child{c0, c1} {}
        static node_t nil;
    };
    
    struct ref_node_t{
        T val;
        bool exist;
        ref_node_t(T x, bool _e) : val{x}, exist{_e} {}
    };
    
    struct Pool {
        constexpr static int SIZ = 1<<17;
        node_t *ptr = nullptr, *en = nullptr;
        vector<node_t*> reuse;
        Pool() {
            ptr = new node_t[SIZ];
            en = ptr + SIZ;
        }
        node_t *get() {
            if(!reuse.empty()) {
                auto ptr = reuse.back();
                reuse.pop_back();
                return ptr;
            }
            if(ptr == en) {
                ptr = new node_t[SIZ];
                en = ptr + SIZ;
            }
            return ptr++;
        }
        void push(node_t *p) {reuse.push_back(p);}
    };
    
    static Pool pool;
    node_t* root;
    bool rev = false;
    
    template<typename _Tp>
    inline static int clz(_Tp x) {
        if constexpr(sizeof(_Tp) == 8ull) return __builtin_clzll(x);
        else return __builtin_clz(x);
    }
    
    inline static T mask(int l, int r) {
        if(r >= bit_width){
            if(l >= bit_width) return 0;
            else return -(one<<l);
        }
        return (one<<r) - (one<<l);
    }
    
    inline static T masked(T v, int l, int r) {
        return mask(l, r) & v;
    }
    
    inline static int diff_bit(T x, T y) {
        return bit_width - clz(x^y);
    }
    
    //value, sum, width, count, child
    template<typename... Args>
    static inline node_t* make_node(Args ...args) {
        return &((*pool.get()) = node_t(forward<Args>(args)...));
    }
    
    public:
    
    meldable_binary_trie(){
        root = make_node(0, e(), bit_width, 0);
    }
    meldable_binary_trie(T p, const S &x){
        root = make_node(p, x, bit_width, 1);
    }
    meldable_binary_trie(node_t *x){
        root = x;
    }
    
    meldable_binary_trie& operator=(meldable_binary_trie&& o) noexcept = default;
    
    meldable_binary_trie(meldable_binary_trie&& o) noexcept = default;
    
    int size() const {
        return root->count;
    }
    
    static node_t *meld(meldable_binary_trie &l, meldable_binary_trie &r){
        return meld(l.root, r.root, bit_width);
    }
    static node_t *meld(node_t *l, node_t *r){
        return meld(l, r, bit_width);
    }
    static node_t *meld(node_t *l, node_t *r, int bit){
        if(l->count == 0){
            return r;
        }
        if(r->count == 0){
            return l;
        }
        T mlv = masked(l->value, min(bit-l->width, bit-r->width), bit);
        T mrv = masked(r->value, min(bit-l->width, bit-r->width), bit);
        if(mlv == mrv){
            if(l->width == r->width){
                l->count += r->count;
                if(bit == l->width){
                    assert(false);
                } else {
                    l->child[0] = meld(l->child[0], r->child[0], bit - l->width);
                    l->child[1] = meld(l->child[1], r->child[1], bit - l->width);
                    pool.push(r);
                }
            } else {
                if(l->width > r->width) swap(l, r);
                l->count += r->count;
                bit -= l->width;
                r->width -= l->width;
                bool b = (r->value>>(bit-1))&1;
                l->child[b] = meld(l->child[b], r, bit);
            }
        } else {
            if(l->width > r->width) swap(l, r);
            int diff = diff_bit(mlv, mrv);
            if(l->width <= bit - diff){
                l->count += r->count;
                r->width -= l->width;
                bit -= l->width;
                bool b = (r->value>>(bit-1))&1;
                l->child[b] = meld(l->child[b], r, bit);
            } else {
                node_t *ptr = make_node(l->value, l->sum, l->width-bit+diff, l->count, l->child[0], l->child[1]);
                r->width -= bit - diff;
                l->width = bit - diff;
                l->count += r->count;
                l->child[ptr->value>>(diff-1)&1] = ptr;
                l->child[r->value>>(diff-1)&1] = r;
            }
        }
        l->sum = op(l->child[0]->sum, l->child[1]->sum);
        return l;
    }
    
    pair<meldable_binary_trie, meldable_binary_trie> split(int p){
        if(rev) p = root->count-p;
        auto res = split(root, p);
        if(rev){
            meldable_binary_trie f(res.second), s(res.first);
            f.rev = true; s.rev = true;
            return make_pair(move(f), move(s));
        } else return {meldable_binary_trie(res.first), meldable_binary_trie(res.second)};
    }
    
    static pair<node_t*, node_t*> split(node_t *l, int p){
        if(p == 0) return {&node_t::nil, l};
        if(p == l->count) return {l, &node_t::nil};
        
        if(p == l->child[0]->count) {
            l->child[0]->width += l->width;
            l->child[1]->width += l->width;
            pool.push(l);
            return {l->child[0], l->child[1]};
        } else if(p < l->child[0]->count) {
            auto [subl, subr] = split(l->child[0], p);
            
            subl->width += l->width;
            l->child[0] = subr;
            l->count = l->child[0]->count + l->child[1]->count;
            l->sum = op(l->child[0]->sum, l->child[1]->sum);
            return {subl, l};
        } else {
            auto [subl, subr] = split(l->child[1], p - l->child[0]->count);
            
            subr->width += l->width;
            l->child[1] = subl;
            l->count = l->child[0]->count + l->child[1]->count;
            l->sum = op(l->child[0]->sum, l->child[1]->sum);
            return {l, subr};
        }
    }
    
    meldable_binary_trie split_one(){
        if(rev){
            meldable_binary_trie res(split_one_rev(root));
            res.rev = true;
            return move(res);
        } else {
            return meldable_binary_trie(split_one(root));
        }
    }
    node_t *split_one(node_t *l){
        if(l->count == 1){
            pool.push(l);
            return &node_t::nil;
        }
        
        l->count--;
        if(l->child[0]->count > 0){
            l->child[0] = split_one(l->child[0]);
            l->sum = op(l->child[0]->sum, l->child[1]->sum);
        } else {
            l->child[1] = split_one(l->child[1]);
            l->sum = l->child[1]->sum;
        }
        return l;
    }
    node_t *split_one_rev(node_t *l){
        if(l->count == 1){
            pool.push(l);
            return &node_t::nil;
        }
        
        l->count--;
        if(l->child[1]->count > 0){
            l->child[1] = split_one_rev(l->child[1]);
            l->sum = op(l->child[0]->sum, l->child[1]->sum);
        } else {
            l->child[0] = split_one_rev(l->child[0]);
            l->sum = l->child[0]->sum;
        }
        return l;
    }
    
    S prod_l(int n) {
        return _prod_l(n, root);
    }
    
    S _prod_l(int n, node_t *pos) {
        if(n == 0) return e();
        if(n == pos->count) return pos->sum;
        
        if(n < pos->child[0]->count) return _prod_l(n, pos->child[0]);
        else if(n == pos->child[0]->count) return pos->child[0]->sum;
        else return op(pos->child[0]->sum, _prod_l(n - pos->child[0]->count, pos->child[1]));
    }
    
    S prod_r(int n) {
        return _prod_r(n, root);
    }
    
    S _prod_r(int n, node_t *pos) {
        if(n == 0) return e();
        if(n == pos->count) return pos->sum;
        
        if(n < pos->child[1]->count) return _prod_r(n, pos->child[1]);
        else if(n == pos->child[1]->count) return pos->child[1]->sum;
        else return op(_prod_r(n - pos->child[1]->count, pos->child[0]), pos->child[1]->sum);
    }
    
    S prod_lr(int l, int r) {
        return _prod_lr(l, r, root);
    }
    
    S _prod_lr(int l, int r, node_t *pos) {
        if(l == r) return e();
        if(l == 0) return _prod_l(r, pos);
        if(r == pos->count) return _prod_r(r-l, pos);
        
        const auto &child = pos->child;
        
        if(l == child[0]->count) return _prod_l(r-l, child[1]);
        if(r == child[0]->count) return _prod_r(r-l, child[0]);
        
        if(l < child[0]->count){
            if(r < child[0]->count) return _prod_lr(l, r, child[0]);
            else return op(_prod_r(child[0]->count-l, child[0]), _prod_l(r-child[0]->count, child[1]));
        } else {
            return _prod_lr(l-child[0]->count, r-child[0]->count, child[1]);
        }
    }
    
    S all_prod() const {return root->sum;}
    
    void init(T p, const S &x){
        root = make_node(p, x, bit_width, 1);
    }
    
    static int get_size(node_t *ptr){
        if(ptr == &node_t::nil) return 0;
        int res = 1;
        for(int i = 0; i < 2; i++) res += get_size(ptr->child[i]);
        return res;
    }
};
template<typename T, typename S, auto op, auto e> typename meldable_binary_trie<T, S, op, e>::node_t meldable_binary_trie<T, S, op, e>::node_t::nil = meldable_binary_trie<T, S, op, e>::node_t(0, e(), bit_width, 0);
template<typename T, typename S, auto op, auto e> typename meldable_binary_trie<T, S, op, e>::Pool meldable_binary_trie<T, S, op, e>::pool = meldable_binary_trie<T, S, op, e>::Pool();

struct fastset{
    using ull = unsigned long long;
    using uint = unsigned;
    
    int siz;
    vector<ull> node;
    
    fastset(int _n){
        int n = 1;
        while((n<<6) < _n) n <<= 6;
        siz = (n+1)/63;
        node.resize((n+1)/63 + (_n+63)/64);
    }
    
    void insert(int x){
        uint idx = siz + (x/64);
        x &= 63;
        while(true){
            if((node[idx]>>x) & 1ull) return;
            node[idx] |= (1ull<<x);
            if(idx == 0) return;
            idx--;
            x = idx & 63;
            idx /= 64;
        }
    }
    
    void erase(int x){
        int idx = siz + (x/64);
        x &= 63;
        while(true){
            node[idx] &= ~(1ull<<x);
            if(idx == 0 || node[idx]) return;
            idx--;
            x = idx & 63;
            idx /= 64;
        }
    }
    
    bool count(int x) const {
        return (node[siz+x/64]>>(x&63u))&1;
    }
    
    int lower_bound(int x){
        if(count(x)) return x;
        int idx = siz + (x/64);
        x &= 63;
        while(true){
            if(node[idx] & ~(((1ull<<x)<<1) - 1ull)){
                x = __builtin_ctzll(node[idx] & ~(((1ull<<x)<<1) - 1ull));
                if(idx >= siz) return (idx-siz)*64+x;
                break;
            }
            if(idx == 0) return -1;
            idx--;
            x = idx & 63;
            idx /= 64;
        }
        
        idx = idx*64 + x+1;
        while(idx < siz) idx = idx*64 + __builtin_ctzll(node[idx])+1;
        return (idx-siz)*64 + __builtin_ctzll(node[idx]);
    }
    
    int less_bound(int x){
        if(count(x)) return x;
        int idx = siz + (x/64);
        x &= 63;
        while(true){
            if(node[idx] & ((1ull<<x) - 1ull)){
                x = 63 - __builtin_clzll(node[idx] & ((1ull<<x) - 1ull));
                if(idx >= siz) return (idx-siz)*64+x;
                break;
            }
            if(idx == 0) return -1;
            idx--;
            x = idx & 63;
            idx /= 64;
        }
        
        idx = idx*64 + x+1;
        while(idx < siz) idx = idx*64 + 64 - __builtin_clzll(node[idx]);
        return (idx-siz)*64 + 63 - __builtin_clzll(node[idx]);
    }
};
template<typename S, auto op, auto e>
struct segtree{
    int siz;
    vector<S> node;
    
    segtree() = default;
    segtree(unsigned n) : siz(std::bit_ceil(n)), node(siz*2, e()){}
    segtree(const vector<S>& v){init(v);}
    void init(const vector<S>& v){
        siz = bit_ceil(v.size()); node.resize(siz*2, e());
        for(int i = 0; i < (int)v.size(); i++) node[i+siz] = v[i];
        for(int i = siz-1; i >= 1; i--) node[i] = op(node[i*2], node[i*2+1]);
    }
    
    const S &operator[](int pos) const {return node[pos+siz];}
    
    const S &get(int pos) const {return node[pos+siz];}
    
    void set(int pos, const S &x){
        assert(0 <= pos && pos < siz);
        pos += siz;
        node[pos] = x;
        while(pos>>=1) node[pos] = op(node[pos<<1], node[(pos<<1)+1]);
    }
    
    void add(int pos, const S &x){
        assert(0 <= pos && pos < siz);
        pos += siz;
        node[pos] = op(node[pos], x);
        while(pos>>=1) node[pos] = op(node[pos], x);
    }
    
    S prod(int left, int right){
        left = max(0, left); right = min(right, siz);
        S l_ans = e(), r_ans = e();
        for(left+=siz, right+=siz; left < right; left>>=1, right>>=1){
            if(left&1) l_ans = op(l_ans, node[left++]);
            if(right&1) r_ans = op(node[--right], r_ans);
        }
        return op(l_ans, r_ans);
    }
    
    S all_prod(){
        return node[1];
    }
};

} //namespace smkyb

template<typename T, typename S, auto op, auto e>
struct sortable_segtree {
    struct SS {
        S ltor, rtol;
        SS() = default;
        SS(const S &o) : ltor(o), rtol(o) {}
        SS(const S &l, const S &r) : ltor(l), rtol(r) {}
        static SS SS_op(const SS &l, const SS &r) {return SS{op(l.ltor, r.ltor), op(r.rtol, l.rtol)};}
        static SS SS_e() {return SS{e()};}
    };
    
    int n;
    smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e> *trie;
    smkyb::fastset fset;
    smkyb::segtree<S, op, e> seg;
    
    sortable_segtree(const vector<pair<T, S>> &v) : n(v.size()), fset(n+1), seg(n) {
        trie = new smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>[n];
        for(int i = 0; i <= n; i++) fset.insert(i);
        for(int i = 0; i < n; i++) trie[i] = smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>(v[i].first, v[i].second);
        for(int i = 0; i < n; i++) seg.set(i, v[i].second);
    }
    sortable_segtree(sortable_segtree<T, S, op, e> &&o) = default;
    
    void set(int i, T k, S x) {
        int l = fset.less_bound(i);
        int r = fset.lower_bound(i+1);
        if(l < i){
            auto res = trie[l].split(i-l);
            trie[l] = move(res.first); trie[i] = move(res.second);
            fset.insert(i);
            seg.set(l, trie[l].rev ? trie[l].all_prod().rtol : trie[l].all_prod().ltor);
        }
        if(i+1 < r){
            auto res = trie[i].split_one();
            trie[i+1] = move(res);
            fset.insert(i+1);
            seg.set(i+1, trie[i+1].rev ? trie[i+1].all_prod().rtol : trie[i+1].all_prod().ltor);
        }
        trie[i].init(k, x);
        seg.set(i, trie[i].rev ? trie[i].all_prod().rtol : trie[i].all_prod().ltor);
    }
    
    S prod(int l, int r) {
        int lbl = fset.less_bound(l);
        int lbr = lbl + trie[lbl].size();
        int rbl = fset.less_bound(r-1);
        int rbr = rbl + trie[rbl].size();
        if(lbl == l){
            if(rbr == r) return seg.prod(l, r);
            else {
                if(lbl == rbl) return (trie[lbl].rev ? trie[lbl].prod_r(r-l).rtol : trie[lbl].prod_l(r-l).ltor);
                else return op(seg.prod(l, rbl), (trie[rbl].rev ? trie[rbl].prod_r(r-rbl).rtol : trie[rbl].prod_l(r-rbl).ltor));
            }
        } else {
            if(rbr == r){
                if(lbl == rbl) return (trie[lbl].rev ? trie[lbl].prod_l(r-l).rtol : trie[lbl].prod_r(r-l).ltor);
                else return op((trie[lbl].rev ? trie[lbl].prod_l(lbr-l).rtol : trie[lbl].prod_r(lbr-l).ltor), seg.prod(lbr, r));
            } else {
                if(lbl == rbl) return (trie[lbl].rev ? trie[lbl].prod_lr(rbr-r, rbr-l).rtol : trie[lbl].prod_lr(l-lbl, r-lbl).ltor);
                else return op(op((trie[lbl].rev ? trie[lbl].prod_l(lbr-l).rtol : trie[lbl].prod_r(lbr-l).ltor), seg.prod(lbr, rbl)), (trie[rbl].rev ? trie[rbl].prod_r(r-rbl).rtol : trie[rbl].prod_l(r-rbl).ltor));
            }
        }
    }
    
    void sort(int l, int r) {
        int lb = fset.less_bound(l);
        if(lb < l){
            auto res = trie[lb].split(l-lb);
            trie[lb] = move(res.first); trie[l] = move(res.second);
            fset.insert(l);
            seg.set(lb, trie[lb].rev ? trie[lb].all_prod().rtol : trie[lb].all_prod().ltor);
        }
        int rb = fset.less_bound(r);
        if(rb < r){
            auto res = trie[rb].split(r-rb);
            trie[rb] = move(res.first); trie[r] = move(res.second);
            fset.insert(r);
            seg.set(r, trie[r].rev ? trie[r].all_prod().rtol : trie[r].all_prod().ltor);
        }
        lb = fset.lower_bound(l+1);
        while(lb < r){
            auto res = smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>::meld(trie[l], trie[lb]);
            trie[l] = smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>(res);
            fset.erase(lb);
            seg.set(lb, e());
            lb = fset.lower_bound(lb);
        }
        trie[l].rev = false;
        seg.set(l, trie[l].all_prod().ltor);
    }
    
    void sort_rev(int l, int r) {
        int lb = fset.less_bound(l);
        if(lb < l){
            auto res = trie[lb].split(l-lb);
            trie[lb] = move(res.first); trie[l] = move(res.second);
            fset.insert(l);
            seg.set(lb, trie[lb].rev ? trie[lb].all_prod().rtol : trie[lb].all_prod().ltor);
        }
        int rb = fset.less_bound(r);
        if(rb < r){
            auto res = trie[rb].split(r-rb);
            trie[rb] = move(res.first); trie[r] = move(res.second);
            fset.insert(r);
            seg.set(r, trie[r].rev ? trie[r].all_prod().rtol : trie[r].all_prod().ltor);
        }
        lb = fset.lower_bound(l+1);
        while(lb < r){
            auto res = smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>::meld(trie[l], trie[lb]);
            trie[l] = smkyb::meldable_binary_trie<T, SS, SS::SS_op, SS::SS_e>(res);
            fset.erase(lb);
            seg.set(lb, e());
            lb = fset.lower_bound(lb);
        }
        trie[l].rev = true;
        seg.set(l, trie[l].all_prod().rtol);
    }
};

#include <atcoder/modint>
using mint9 = atcoder::modint998244353;
using uint = unsigned;

struct S{
    mint9 first, second;
    S() : first(mint9::raw(1)), second(mint9::raw(0)) {}
    S(const S &o) = default;
    S(const mint9 &l, const mint9 &r) : first(l), second(r) {}
    static S e() {return {1, 0};}
    static S op(const S &l, const S &r) {return {r.first*l.first, r.first*l.second + r.second};}
};

int main(){
    uint n, q;
    cin >> n >> q;
    vector<pair<uint, S>> A(n);
    for(uint i = 0; i < n; i++){
        uint a, b;
        cin >> A[i].first >> a >> b;
        A[i].second = {a, b};
    }
    
    sortable_segtree<uint, S, S::op, S::e> trie(A);
    
    uint i{}, p{}, a{}, b{}, l{}, r{}, x{};
    S res{};
    while(q--){
        uint t;
        cin >> t;
        switch(t){
            case 0:
                cin >> i >> p >> a >> b;
                trie.set(i, p, {a, b});
                break;
            case 1:
                cin >> l >> r >> x;
                res = trie.prod(l, r);
                cout << (res.first*x + res.second).val() << '\n';
                break;
            case 2:
                cin >> l >> r;
                trie.sort(l, r);
                break;
            case 3:
                cin >> l >> r;
                trie.sort_rev(l, r);
                break;
        }
    }
}
