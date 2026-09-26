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
    requires std::is_nothrow_move_constructible_v<M>;
    requires std::is_nothrow_copy_constructible_v<M>;
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
    using Index = uint32_t;

    explicit PersistentLazySegmentTree(Index capacity = 0) {
        if (capacity > 0) {
            pool_.reserve(capacity);
        }
    }

    auto build(std::span<const T> data) -> Index {
        const auto size = static_cast<Index>(data.size());
        if (size == 1) {
            return make_node(T(data[0]), E::e(), NONE, NONE);
        }
        Index m = size / 2;
        return merge(build(data.subspan(0, m)), build(data.subspan(m)));
    }

    auto apply(Index root, Index l, Index r, const E& f, Index n) -> Index {
        return apply_impl(root, 0, n, l, r, E::e(), f);
    }

    auto prod(Index root, Index l, Index r, Index n) const noexcept -> T {
        return prod_impl(root, 0, n, l, r, E::e());
    }

    auto copy(Index src_i, Index mut_i, Index l, Index r, Index n) -> Index {
        return copy_impl(src_i, mut_i, 0, n, l, r, E::e(), E::e());
    }

  private:
    struct Node {
        T val_;
        E lazy_;
        Index left_;
        Index right_;

        Node(T&& val, E&& lazy, Index left, Index right) noexcept :
            val_(std::move(val)),
            lazy_(std::move(lazy)),
            left_(left),
            right_(right) {}
    };

    static_assert(std::is_nothrow_move_constructible_v<Node>);

    auto make_node(T&& val, E&& lazy, Index left, Index right) -> Index {
        pool_.emplace_back(std::move(val), std::move(lazy), left, right);
        return static_cast<Index>(pool_.size() - 1);
    }

    auto apply_all(Index i, const E& lazy) -> Index {
        const Node& node = pool_[i];
        return make_node(
            lazy.act(node.val_),
            node.lazy_.prod(lazy),
            node.left_,
            node.right_
        );
    }

    auto merge(Index left, Index right) -> Index {
        return make_node(
            pool_[left].val_.prod(pool_[right].val_),
            E::e(),
            left,
            right
        );
    }

    auto apply_impl(
        Index i,
        Index s,
        Index e,
        Index l,
        Index r,
        const E& lazy,
        const E& f
    ) -> Index {
        if (l <= s && e <= r) {
            return apply_all(i, lazy.prod(f));
        }
        if (r <= s || e <= l) {
            return apply_all(i, lazy);
        }

        Index m = (s + e) / 2;
        Index left = pool_[i].left_;
        Index right = pool_[i].right_;
        E new_laz = pool_[i].lazy_.prod(lazy);
        return merge(
            apply_impl(left, s, m, l, r, new_laz, f),
            apply_impl(right, m, e, l, r, new_laz, f)
        );
    }

    auto prod_impl(Index i, Index s, Index e, Index l, Index r, const E& lazy)
        const noexcept -> T {
        const Node& node = pool_[i];
        if (l <= s && e <= r) {
            return lazy.act(node.val_);
        }

        Index m = (s + e) / 2;
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
        Index src_i,
        Index mut_i,
        Index s,
        Index e,
        Index l,
        Index r,
        const E& src_lazy,
        const E& mut_lazy
    ) -> Index {
        if (l <= s && e <= r) {
            return apply_all(mut_i, mut_lazy);
        }
        if (r <= s || e <= l) {
            return apply_all(src_i, src_lazy);
        }

        Index m = (s + e) / 2;
        Index src_left = pool_[src_i].left_;
        Index src_right = pool_[src_i].right_;
        Index mut_left = pool_[mut_i].left_;
        Index mut_right = pool_[mut_i].right_;
        E new_src_lazy = pool_[src_i].lazy_.prod(src_lazy);
        E new_mut_lazy = pool_[mut_i].lazy_.prod(mut_lazy);
        return merge(
            copy_impl(
                src_left,
                mut_left,
                s,
                m,
                l,
                r,
                new_src_lazy,
                new_mut_lazy
            ),
            copy_impl(
                src_right,
                mut_right,
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
    static constexpr Index NONE = std::numeric_limits<Index>::max();
};

} // namespace tbd
#line 2 "tbd/utility/unsafe_scanner.hpp"

#include <sys/mman.h>
#include <sys/stat.h>

#line 8 "tbd/utility/unsafe_scanner.hpp"
#include <string>
#include <type_traits>

namespace tbd {

struct UnsafeScanner {
  public:
    UnsafeScanner(const UnsafeScanner&) = delete;
    auto operator=(const UnsafeScanner&) -> UnsafeScanner& = delete;

    explicit UnsafeScanner(int fd) {
        {
            struct stat buf;
            fstat(fd, &buf);
            len_ = buf.st_size;
        }
        inp_ = static_cast<char*>(
            mmap(nullptr, len_, PROT_READ, MAP_PRIVATE, fd, 0)
        );
    }

    ~UnsafeScanner() {
        munmap(static_cast<void*>(inp_), len_);
    }

    void read() noexcept {}

    template<typename H, typename... T>
    void read(H& h, T&... t) noexcept {
        skip_whitespaces();
        read_single(h);
        read(t...);
    }

  private:
    char* inp_;
    size_t cursor_ = 0;
    size_t len_;

    auto read_single(std::string& ref) noexcept {
        ref = "";
        while (true) {
            if (char c = inp_[cursor_]; c > ' ') {
                ref += c;
                cursor_++;
            } else {
                break;
            }
        }
    }

    auto read_single(double& ref) noexcept {
        std::string s;
        read_single(s);
        ref = std::stod(s);
    }

    auto read_single(char& ref) noexcept {
        ref = inp_[cursor_++];
    }

    template<typename S>
        requires std::signed_integral<S>
    auto read_single(S& sref) noexcept {
        bool neg = false;
        if (inp_[cursor_] == '-') {
            neg = true;
            cursor_++;
        }
        using U = std::make_unsigned_t<S>;
        auto ref = U(0);
        do {
            ref = U((10 * ref) + U(inp_[cursor_++] & 0x0f));
        } while (inp_[cursor_] >= '0');
        sref = S(neg ? -ref : ref);
    }

    template<typename U>
        requires std::unsigned_integral<U>
    auto read_single(U& ref) noexcept {
        ref = 0;
        do {
            ref = U((10 * ref) + U(inp_[cursor_++] & 0x0f));
        } while (inp_[cursor_] >= '0');
    }

    auto skip_whitespaces() noexcept -> void {
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

    [[nodiscard]] constexpr auto prod(const Pair& o) const noexcept -> Pair {
        return {.value_ = (value_ + o.value_) % MOD, .size_ = size_ + o.size_};
    }

    constexpr static auto e() noexcept -> Pair {
        return {.value_ = 0, .size_ = 0};
    }
};

struct Affine {
    uint32_t a_;
    uint32_t b_;

    [[nodiscard]] constexpr auto prod(const Affine& other) const noexcept
        -> Affine {
        return {
            .a_ = uint32_t(uint64_t(a_) * other.a_ % MOD),
            .b_ = uint32_t(((uint64_t(b_) * other.a_) + other.b_) % MOD)
        };
    }

    constexpr static auto e() noexcept -> Affine {
        return {.a_ = 1, .b_ = 0};
    }

    [[nodiscard]] constexpr auto act(Pair x) const noexcept -> Pair {
        return {
            .value_ = uint32_t(
                (((uint64_t(a_) * x.value_) + (uint64_t(b_) * x.size_)) % MOD)
            ),
            .size_ = x.size_
        };
    }
};

auto main() noexcept -> int {
    std::ios_base::sync_with_stdio(false);
    using std::cout;

    auto sc = tbd::UnsafeScanner(STDIN_FILENO);

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