#line 1 "oj/lc/persistent_range_affine_range_sum.test.cpp"
// verification-helper: PROBLEM https://judge.yosupo.jp/problem/persistent_range_affine_range_sum

#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#line 2 "tbd/container/persistent_lazy_segtree.hpp"

#line 4 "tbd/container/persistent_lazy_segtree.hpp"
#include <limits>
#include <span>
#line 7 "tbd/container/persistent_lazy_segtree.hpp"

#line 2 "tbd/algebraic/monoidal.hpp"

#include <concepts>

namespace tbd {

template<typename M>
concept Monoid = requires(const M& a, const M& b) {
    { M::e() } -> std::same_as<M>;
    { a.prod(b) } -> std::same_as<M>;
    requires std::movable<M>;
    requires std::copyable<M>;
};

template<typename M, typename X>
concept Act = requires(const M& e, const X& x) {
    { e.act(x) } -> std::same_as<X>;
    requires Monoid<M>;
};

} // namespace tbd
#line 9 "tbd/container/persistent_lazy_segtree.hpp"

namespace tbd {

template<typename T, typename E>
    requires Monoid<T> && Act<E, T>
class PersistentLazySegmentTree {
  public:
    explicit PersistentLazySegmentTree(size_t capacity = 0) {
        if (capacity > 0) {
            pool_.reserve(capacity);
        }
    }

    auto build(std::span<const T> data) -> size_t {
        if (data.size() == 1) {
            return make_node(T(data[0]), E::e(), NONE, NONE);
        }
        size_t m = data.size() / 2;
        return merge(build(data.subspan(0, m)), build(data.subspan(m)));
    }

    auto apply(size_t root, size_t l, size_t r, const E& f, size_t n)
        -> size_t {
        return apply_impl(root, 0, n, l, r, E::e(), f);
    }

    auto prod(size_t root, size_t l, size_t r, size_t n) const -> T {
        return prod_impl(root, 0, n, l, r, E::e());
    }

    auto copy(size_t src_i, size_t mut_i, size_t l, size_t r, size_t n)
        -> size_t {
        return copy_impl(src_i, mut_i, 0, n, l, r, E::e(), E::e());
    }

  private:
    struct Node {
        T val_;
        E lazy_;
        size_t left_;
        size_t right_;

        Node(T val, E lazy, size_t left, size_t right) :
            val_(std::move(val)),
            lazy_(std::move(lazy)),
            left_(left),
            right_(right) {}
    };

    auto make_node(T&& val, E&& lazy, size_t left, size_t right) -> size_t {
        pool_.emplace_back(
            std::forward<T>(val),
            std::forward<E>(lazy),
            left,
            right
        );
        return pool_.size() - 1;
    }

    auto apply_all(size_t i, const E& lazy) -> size_t {
        const Node& node = pool_[i];
        return make_node(
            lazy.act(node.val_),
            node.lazy_.prod(lazy),
            node.left_,
            node.right_
        );
    }

    auto merge(size_t left, size_t right) -> size_t {
        return make_node(
            pool_[left].val_.prod(pool_[right].val_),
            E::e(),
            left,
            right
        );
    }

    auto apply_impl(
        size_t i,
        size_t s,
        size_t e,
        size_t l,
        size_t r,
        const E& lazy,
        const E& f
    ) -> size_t {
        if (l <= s && e <= r) {
            return apply_all(i, lazy.prod(f));
        }
        if (r <= s || e <= l) {
            return apply_all(i, lazy);
        }

        size_t m = (s + e) / 2;
        const Node& node = pool_[i];
        E new_laz = node.lazy_.prod(lazy);
        return merge(
            apply_impl(node.left_, s, m, l, r, new_laz, f),
            apply_impl(node.right_, m, e, l, r, new_laz, f)
        );
    }

    auto
    prod_impl(size_t i, size_t s, size_t e, size_t l, size_t r, const E& lazy)
        const -> T {
        const Node& node = pool_[i];
        if (l <= s && e <= r) {
            return lazy.act(node.val_);
        }

        size_t m = (s + e) / 2;
        E new_lazy = node.lazy_.prod(lazy);
        if (r <= m) {
            return prod_impl(node.left_, s, m, l, r, new_lazy);
        }
        if (m <= l) {
            return prod_impl(node.right_, m, e, l, r, new_lazy);
        }
        return prod_impl(node.left_, s, m, l, r, new_lazy)
            .prod(prod_impl(node.right_, m, e, l, r, new_lazy));
    }

    auto copy_impl(
        size_t src_i,
        size_t mut_i,
        size_t s,
        size_t e,
        size_t l,
        size_t r,
        const E& src_lazy,
        const E& mut_lazy
    ) -> size_t {
        if (l <= s && e <= r) {
            return apply_all(mut_i, mut_lazy);
        }
        if (r <= s || e <= l) {
            return apply_all(src_i, src_lazy);
        }

        size_t m = (s + e) / 2;
        const Node& src_node = pool_[src_i];
        const Node& mut_node = pool_[mut_i];
        E new_src_lazy = src_node.lazy_.prod(src_lazy);
        E new_mut_lazy = mut_node.lazy_.prod(mut_lazy);
        return merge(
            copy_impl(
                src_node.left_,
                mut_node.left_,
                s,
                m,
                l,
                r,
                new_src_lazy,
                new_mut_lazy
            ),
            copy_impl(
                src_node.right_,
                mut_node.right_,
                m,
                e,
                l,
                r,
                new_src_lazy,
                new_mut_lazy
            )
        );
    }

    std::vector<Node> pool_;
    static constexpr size_t NONE = std::numeric_limits<size_t>::max();
};

} // namespace tbd
#line 2 "tbd/utility/scanner.hpp"

#include <sys/mman.h>
#include <sys/stat.h>
#line 6 "tbd/utility/scanner.hpp"

#include <cassert>
#line 9 "tbd/utility/scanner.hpp"
#include <cstring>
#include <string>
#include <type_traits>

namespace tbd {

/*
 * This is largely inspired by
 * <https://github.com/yosupo06/yosupo-library/blob/442845933609d69a3b213e17faa344740021a7f5/src/yosupo/fastio.hpp>
*/
struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    auto operator=(const Scanner&) -> Scanner& = delete;

    explicit Scanner(int fd) {
        struct stat buf;
        fstat(fd, &buf);
        len_ = buf.st_size;
        inp_ = static_cast<char*>(
            mmap(nullptr, len_, PROT_READ, MAP_PRIVATE, fd, 0)
        );
    }

    ~Scanner() {
        munmap(inp_, len_);
    }

    void read() {}

    template<class H, class... T>
    void read(H& h, T&... t) {
        read_single(h);
        read(t...);
    }

  private:
    static constexpr size_t SIZE = 1 << 15;
    static constexpr char SENTINEL = 0;
    static constexpr int MIN_LOOKAHEAD = 50;

    char* inp_;
    size_t cursor_ = 0;
    size_t len_ = 0;

    auto read_single(std::string& ref) {
        skip_space();
        ref = "";
        while (true) {
            char c = inp_[cursor_];
            if (c <= ' ') {
                break;
            }
            ref += c;
            cursor_++;
        }
    }

    auto read_single(double& ref) {
        skip_space();
        std::string s;
        read_single(s);
        ref = std::stod(s);
    }

    auto read_single(char& ref) {
        skip_space();
        ref = inp_[cursor_++];
    }

    template<class S>
        requires std::signed_integral<S>
    auto read_single(S& sref) {
        skip_space();
        bool neg = false;
        if (inp_[cursor_] == '-') {
            neg = true;
            cursor_++;
        }
        using U = std::make_unsigned_t<S>;
        U ref = 0;
        do {
            ref = U((10 * ref) + U(inp_[cursor_++] & 0x0f));
        } while (inp_[cursor_] >= '0');
        sref = S(neg ? -ref : ref);
    }

    template<class U>
        requires std::unsigned_integral<U>
    auto read_single(U& ref) {
        skip_space();
        ref = 0;
        do {
            ref = U((10 * ref) + U(inp_[cursor_++] & 0x0f));
        } while (inp_[cursor_] >= '0');
    }

    template<int TOKEN_LEN = 0>
    auto skip_space() -> void {
        while (inp_[cursor_] <= ' ') {
            cursor_++;
        }
    }
};

} // namespace tbd
#line 13 "oj/lc/persistent_range_affine_range_sum.test.cpp"

constexpr int32_t MOD = 998244353;

struct Pair {
    uint32_t value_;
    uint32_t size_;

    [[nodiscard]] constexpr auto prod(const Pair& o) const -> Pair {
        return {.value_ = (value_ + o.value_) % MOD, .size_ = size_ + o.size_};
    }

    constexpr static auto e() -> Pair {
        return {.value_ = 0, .size_ = 0};
    }
};

struct Affine {
    uint32_t a_;
    uint32_t b_;

    [[nodiscard]] constexpr auto prod(const Affine& other) const -> Affine {
        return {
            .a_ = uint32_t(uint64_t(a_) * other.a_ % MOD),
            .b_ = uint32_t(((uint64_t(b_) * other.a_) + other.b_) % MOD)
        };
    }

    constexpr static auto e() -> Affine {
        return {.a_ = 1, .b_ = 0};
    }

    [[nodiscard]] constexpr auto act(Pair x) const -> Pair {
        return {
            .value_ = uint32_t(
                (((uint64_t(a_) * x.value_) + (uint64_t(b_) * x.size_)) % MOD)
            ),
            .size_ = x.size_
        };
    }
};

auto main() -> int {
    std::ios_base::sync_with_stdio(false);
    using std::cout;

    auto sc = tbd::Scanner(STDIN_FILENO);

    size_t n;
    size_t q;
    sc.read(n, q);

    auto initial = std::vector<Pair>(n);
    for (auto& [v, s] : initial) {
        sc.read(v);
        s = 1;
    }

    auto tr = tbd::PersistentLazySegmentTree<Pair, Affine>(10000000);
    auto snapshots = std::vector<size_t> {tr.build(initial)};
    snapshots.reserve(q + 1);

    for (size_t _ = 0; _ < q; _++) {
        int op;
        int k;
        sc.read(op, k);

        if (op == 0) {
            size_t l;
            size_t r;
            uint32_t a;
            uint32_t b;
            sc.read(l, r, a, b);
            snapshots.push_back(
                tr.apply(snapshots[k + 1], l, r, {.a_ = a, .b_ = b}, n)
            );
        } else if (op == 1) {
            int s;
            size_t l;
            size_t r;
            sc.read(s, l, r);
            size_t new_root =
                tr.copy(snapshots[k + 1], snapshots[s + 1], l, r, n);
            snapshots.push_back(new_root);
        } else if (op == 2) {
            size_t l;
            size_t r;
            sc.read(l, r);
            cout << tr.prod(snapshots[k + 1], l, r, n).value_ << '\n';
            snapshots.push_back(snapshots.back());
        }
    }

    return 0;
}