
#include <cstddef>

namespace canard {

template <typename T> using aggregate_type_t = typename T::aggregate_type;
template <typename T> using element_type_t = typename T::element_type;
template <typename T> using configuration_type_t = typename T::configuration_type;
template <typename T> using allocator_type_t = typename T::allocator_type;
template <typename T> using size_type_t = typename T::size_type;

// Direct associated-type projections: no cv/ref removal, fallback, or conversion.
// Missing members remain substitution failures in requires-expressions.
template <typename T> using value_type_t = typename T::value_type;

template <typename T> using monoid_type_t = typename T::monoid_type;

template <typename T> using tag_type_t = typename T::tag_type;

template <typename T> using summary_type_t = typename T::summary_type;

template <typename T> using update_type_t = typename T::update_type;

template <typename T> using action_type_t = typename T::action_type;

template <typename T> using leaf_type_t = typename T::leaf_type;

template <typename T> using branch_type_t = typename T::branch_type;

template <typename T> using word_type_t = typename T::word_type;

template <typename T> using representation_type_t = typename T::representation_type;

template <typename T> using execution_type_t = typename T::execution_type;

template <typename T> using delta_type_t = typename T::delta_type;

template <typename T> using prepared_action_t = typename T::prepared_action;

template <typename T> using subtree_change_t = typename T::subtree_change;

template <typename T> using local_change_t = typename T::local_change;

template <typename T> using edge_t = typename T::edge;

template <typename T> using field_t = typename T::field;

template <typename T> using pack_t = typename T::pack;

template <typename T> using fixed_multiplier_t = typename T::fixed_multiplier;

template <typename T> using blended_multiplier_t = typename T::blended_multiplier;

template <typename T, std::size_t Extent> using interval_t = typename T::template interval<Extent>;

template <typename T, std::size_t Count> using edit_type_t = typename T::template edit_type<Count>;

} // namespace canard
#include <concepts>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace canard::algebra {

// These concepts check EXPRESSIONS, not the semantic laws in docs/CONTRACTS.md.
// In particular, no syntactic trait is taken as proof of commutativity.
template <typename M>
concept monoid_expressions = requires(const M& m, const value_type_t<M>& x,
                                     const value_type_t<M>& y) {
    { m.identity() } -> std::same_as<value_type_t<M>>;
    { m.combine(x, y) } -> std::same_as<value_type_t<M>>;
};
// Compatibility concept used by the current value-owning structures.
template <typename M>
concept monoid = monoid_expressions<M> && std::copy_constructible<M> &&
                 std::copyable<value_type_t<M>>;

template <typename A, typename M>
concept action_for = monoid<M> && std::copy_constructible<A> &&
                     requires(const A& a,
                              const tag_type_t<A>& newer,
                              const tag_type_t<A>& older,
                              const value_type_t<M>& x,
                              std::size_t count) {
                         requires std::copyable<tag_type_t<A>>;
                         { a.identity() } -> std::same_as<tag_type_t<A>>;
                         { a.compose(newer, older) } -> std::same_as<tag_type_t<A>>;
                         { a.map(newer, x, count) } -> std::same_as<value_type_t<M>>;
                     };

// Convenient adaptation of an ordinary stateful callable and an explicit
// identity. The callable is stored without type erasure.
template <typename T, typename Combine> struct operation_monoid {
    using value_type = T;
    [[no_unique_address]] Combine operation;
    T unit;
    [[nodiscard]] T identity() const noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return unit;
    }
    [[nodiscard]] T combine(const T& x, const T& y) const
        noexcept(std::is_nothrow_invocable_r_v<T, const Combine&, const T&, const T&>) {
        return std::invoke(operation, x, y);
    }
};
template <typename Combine, typename T>
operation_monoid(Combine, T) -> operation_monoid<T, Combine>;

template <monoid M> struct reversed_monoid {
    using value_type = value_type_t<M>;
    [[no_unique_address]] M base;
    [[nodiscard]] value_type identity() const noexcept(noexcept(base.identity())) {
        return base.identity();
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(base.combine(y, x))) {
        return base.combine(y, x);
    }
};
template <typename M> reversed_monoid(M) -> reversed_monoid<M>;

// Componentwise product: a new payload composition does not need a new tree.
template <monoid Left, monoid Right> struct product_monoid {
    using value_type = std::pair<value_type_t<Left>, value_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] value_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(left.combine(x.first, y.first)) &&
                 noexcept(right.combine(x.second, y.second)) &&
                 std::is_nothrow_move_constructible_v<value_type>) {
        return {left.combine(x.first, y.first), right.combine(x.second, y.second)};
    }
};
template <typename L, typename R> product_monoid(L, R) -> product_monoid<L, R>;

template <typename Left, typename Right> struct product_action {
    using tag_type = std::pair<tag_type_t<Left>, tag_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] tag_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] tag_type compose(const tag_type& newer, const tag_type& older) const
        noexcept(noexcept(left.compose(newer.first, older.first)) &&
                 noexcept(right.compose(newer.second, older.second)) &&
                 std::is_nothrow_move_constructible_v<tag_type>) {
        return {left.compose(newer.first, older.first), right.compose(newer.second, older.second)};
    }
    template <typename X, typename Y>
    [[nodiscard]] std::pair<X, Y>
    map(const tag_type& f, const std::pair<X, Y>& x, std::size_t count) const
        noexcept(noexcept(left.map(f.first, x.first, count)) &&
                 noexcept(right.map(f.second, x.second, count)) &&
                 std::is_nothrow_move_constructible_v<std::pair<X, Y>>) {
        return {left.map(f.first, x.first, count), right.map(f.second, x.second, count)};
    }
};
template <typename L, typename R> product_action(L, R) -> product_action<L, R>;

// Used only internally to erase action storage and propagation from point trees.
struct no_action {
    struct tag_type {};
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type, tag_type) const noexcept {
        return {};
    }
    template <typename T>
    [[nodiscard]] constexpr T map(tag_type, const T& x, std::size_t) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return x;
    }
};

} // namespace canard::algebra
// ===== include/canard/algebra/basic.hpp =====
#include <algorithm>
#include <concepts>
#include <limits>

namespace canard::algebra {

template <std::integral T> struct sum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return a + b;
    }
};

template <std::integral T> struct minimum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::max();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::min(a, b);
    }
};

template <std::integral T> struct maximum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::lowest();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::max(a, b);
    }
};

template <std::integral T> struct add_to_sum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    // Newer is applied AFTER older. Addition happens to commute; others do not.
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        return x + f * static_cast<T>(count);
    }
};

template <std::integral T> struct add_to_extremum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        // Length distinguishes a real INT_MAX/INT_MIN leaf from an empty span.
        return count == 0 ? x : x + f;
    }
};

} // namespace canard::algebra
// ===== include/canard/algebra/modular_types.hpp =====
#include <cstdint>

namespace canard::algebra {

template <std::uint32_t Modulus> struct affine_map {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    std::uint32_t multiplier = 1;
    std::uint32_t translation = 0;
    [[nodiscard]] constexpr std::uint32_t operator()(std::uint32_t x) const noexcept {
        return (std::uint64_t(multiplier) * x + translation) % Modulus;
    }
    friend constexpr bool operator==(affine_map, affine_map) = default;
};

template <std::uint32_t Modulus> struct modular_sum {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    using value_type = std::uint32_t;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return 0;
    }
    [[nodiscard]] constexpr value_type combine(value_type a, value_type b) const noexcept {
        const auto x = a + b;
        return x >= Modulus ? x - Modulus : x;
    }
};

template <std::uint32_t Modulus> struct affine_composition {
    using value_type = affine_map<Modulus>;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return {};
    }
    // Array order: combine(left, right) means apply left first, then right.
    [[nodiscard]] constexpr value_type combine(value_type left, value_type right) const noexcept {
        return {
            std::uint32_t(std::uint64_t(right.multiplier) * left.multiplier % Modulus),
            std::uint32_t((std::uint64_t(right.multiplier) * left.translation + right.translation) %
                          Modulus)};
    }
};

template <std::uint32_t Modulus> struct affine_on_sum {
    using tag_type = affine_map<Modulus>;
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type newer, tag_type older) const noexcept {
        return affine_composition<Modulus>{}.combine(older, newer);
    }
    [[nodiscard]] constexpr std::uint32_t
    map(tag_type f, std::uint32_t x, std::size_t count) const noexcept {
        return (std::uint64_t(f.multiplier) * x +
                std::uint64_t(f.translation) * (count % Modulus)) %
               Modulus;
    }
};

} // namespace canard::algebra


namespace canard::algebra {
// Explicit semantic opt-in. A syntactic concept cannot prove this law.
// Spatial permutation does not preserve registration order.
template <typename M> inline constexpr bool enable_commutative_monoid = false;
template <std::integral T> inline constexpr bool enable_commutative_monoid<sum<T>> = true;
template <std::integral T> inline constexpr bool enable_commutative_monoid<minimum<T>> = true;
template <std::integral T> inline constexpr bool enable_commutative_monoid<maximum<T>> = true;
template <std::uint32_t P> inline constexpr bool enable_commutative_monoid<modular_sum<P>> = true;
template <monoid L, monoid R>
inline constexpr bool enable_commutative_monoid<product_monoid<L, R>> =
    enable_commutative_monoid<L> && enable_commutative_monoid<R>;
template <typename M>
concept commutative_monoid = monoid<M> && enable_commutative_monoid<M>;
} // namespace canard::algebra
// ===== include/canard/algebra/operation_traits.hpp =====
#include <type_traits>
#include <utility>
namespace canard::algebra {
template <monoid Monoid, typename Action>
    requires action_for<Action, Monoid>
inline constexpr bool nothrow_action_operations_v =
        std::is_nothrow_copy_constructible_v<value_type_t<Monoid>> &&
        std::is_nothrow_copy_assignable_v<value_type_t<Monoid>> &&
        std::is_nothrow_move_constructible_v<value_type_t<Monoid>> &&
        std::is_nothrow_move_assignable_v<value_type_t<Monoid>> &&
        std::is_nothrow_copy_constructible_v<tag_type_t<Action>> &&
        std::is_nothrow_copy_assignable_v<tag_type_t<Action>> &&
        std::is_nothrow_move_constructible_v<tag_type_t<Action>> &&
        std::is_nothrow_move_assignable_v<tag_type_t<Action>> &&
        noexcept(std::declval<const Monoid&>().identity()) &&
        noexcept(std::declval<const Monoid&>().combine(std::declval<const value_type_t<Monoid>&>(),
                                                       std::declval<const value_type_t<Monoid>&>())) &&
        noexcept(std::declval<const Action&>().identity()) &&
        noexcept(std::declval<const Action&>().compose(std::declval<const tag_type_t<Action>&>(),
                                                       std::declval<const tag_type_t<Action>&>())) &&
        noexcept(std::declval<const Action&>().map(
            std::declval<const tag_type_t<Action>&>(), std::declval<const value_type_t<Monoid>&>(), std::size_t{}));
}
// ===== include/canard/geometry/orthogonal.hpp =====
#include <algorithm>
#include <concepts>

namespace canard::geometry {
template <std::integral Coordinate> struct point2 {
    using coordinate_type = Coordinate;
    Coordinate x, y;
    friend constexpr bool operator==(point2, point2) = default;
};

// Half-open bounds; no increment of an upper coordinate is needed.
// Reversed and zero-width intervals represent the empty set.
template <std::integral Coordinate> struct rectangle2 {
    Coordinate left, down, right, up;
    [[nodiscard]] constexpr bool empty() const noexcept {
        return left >= right || down >= up;
    }
    [[nodiscard]] constexpr bool contains(point2<Coordinate> p) const noexcept {
        return left <= p.x && p.x < right && down <= p.y && p.y < up;
    }
};

// Closed extrema of a nonempty point set, not a half-open rectangle.
template <std::integral Coordinate> struct point_bounds2 {
    Coordinate min_x, min_y, max_x, max_y;
    [[nodiscard]] constexpr bool disjoint(rectangle2<Coordinate> r) const noexcept {
        return max_x < r.left || r.right <= min_x || max_y < r.down || r.up <= min_y;
    }
    [[nodiscard]] constexpr bool covered_by(rectangle2<Coordinate> r) const noexcept {
        return r.left <= min_x && max_x < r.right && r.down <= min_y && max_y < r.up;
    }
    [[nodiscard]] static constexpr point_bounds2 join(point_bounds2 a, point_bounds2 b) noexcept {
        return {std::min(a.min_x, b.min_x), std::min(a.min_y, b.min_y),
                std::max(a.max_x, b.max_x), std::max(a.max_y, b.max_y)};
    }
};
} // namespace canard::geometry
// ===== include/canard/spatial/configuration.hpp =====
// ===== include/canard/execution.hpp =====
namespace canard::execution {
// Source-level portable operations. Compiler target flags still govern auto-vectorization.
struct scalar {};
// Explicit opt-in backend; no runtime dispatch or implicit fallback.
struct avx2 {};
}
// ===== include/canard/structural/point_partition.hpp =====
#include <concepts>

namespace canard::structural {
namespace detail {
template <std::integral Coordinate, bool Adaptive> class point_kd_partition_plan;
}

// Lightweight construction choices. No payload, lazy action, query history,
// instruction-set choice, or persistent per-node policy data is involved.
namespace point_partition {
struct alternating {
    template <std::integral Coordinate>
    using plan_type = detail::point_kd_partition_plan<Coordinate, false>;
};
struct cardinality_coalesced {
    template <std::integral Coordinate>
    using plan_type = detail::point_kd_partition_plan<Coordinate, true>;
};
} // namespace point_partition

template <typename Partition, std::integral Coordinate>
using point_partition_plan_t = typename Partition::template plan_type<Coordinate>;
} // namespace canard::structural
#include <type_traits>
#include <bit>

namespace canard::spatial {
namespace representation {
struct ordinary {};
// Values/sums: ordinary residues in [0,2p). Tags: Montgomery-encoded.
struct modular_affine {};
}
template <unsigned LeafCapacity = 32,
          typename Representation = representation::ordinary,
          typename Execution = execution::scalar,
          typename Partition = structural::point_partition::alternating>
struct configuration {
    static_assert(LeafCapacity >= 8 && LeafCapacity <= 64 && std::has_single_bit(LeafCapacity));
    static constexpr unsigned leaf_capacity = LeafCapacity;
    using representation_type = Representation;
    using execution_type = Execution;
    using partition_type = Partition;
};
// Legacy custom profiles without the new axis retain alternating partitioning.
namespace detail {
template <typename Profile, typename = void> struct partition_for {
    using type = structural::point_partition::alternating;
};
template <typename Profile> struct partition_for<Profile, std::void_t<typename Profile::partition_type>> {
    using type = typename Profile::partition_type;
};
} // namespace detail
template <typename Profile>
using partition_for_t = typename detail::partition_for<Profile>::type;

template <typename Coordinate, typename Monoid, typename Action,
          typename Representation, typename Execution, unsigned Capacity>
struct kernel_binding {};
} // namespace canard::spatial
// ===== include/canard/utility/array.hpp =====
#include <array>
#include <utility>
#include <cstddef>
namespace canard::utility {
template <std::size_t N, typename T> [[nodiscard]] constexpr std::array<T, N> repeat(const T& x) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::array<T, N>{(static_cast<void>(I), x)...};
    }(std::make_index_sequence<N>{});
}
}
#include <array>
#include <bit>
#include <cstdint>

namespace canard::kernel {
template <std::integral Coordinate> struct spatial_scalar_selection {
    template <typename Geometry>
    [[nodiscard]] static std::uint64_t select(const Geometry& g,
        geometry::rectangle2<Coordinate> rectangle, std::uint64_t active) noexcept {
        std::uint64_t selected = 0;
        for (auto bits = active; bits; bits &= bits - 1) {
            const auto i = std::countr_zero(bits);
            if (rectangle.contains({g.xs[i], g.ys[i]})) selected |= std::uint64_t{1} << i;
        }
        return selected;
    }
};
template <std::integral Coordinate, algebra::commutative_monoid Monoid,
          typename Action, unsigned Capacity>
    requires(algebra::action_for<Action, Monoid> && algebra::nothrow_action_operations_v<Monoid, Action>)
struct spatial_scalar : spatial_scalar_selection<Coordinate> {
    using value_type = value_type_t<Monoid>;
    using summary_type = value_type;
    using update_type = tag_type_t<Action>;
    using action_type = update_type;
    using prepared_action = action_type;
    struct leaf_type { std::array<value_type, Capacity> values; };
    [[no_unique_address]] Monoid monoid;
    [[no_unique_address]] Action action;
    spatial_scalar(Monoid m, Action a)
        noexcept(std::is_nothrow_move_constructible_v<Monoid> && std::is_nothrow_move_constructible_v<Action>)
        : monoid(std::move(m)), action(std::move(a)) {}
    [[nodiscard]] summary_type read(const leaf_type& leaf, unsigned i) const noexcept { return leaf.values[i]; }
    void write(leaf_type& leaf, unsigned i, const summary_type& value) const noexcept { leaf.values[i] = value; }
    [[nodiscard]] leaf_type empty_leaf() const { return {utility::repeat<Capacity>(monoid.identity())}; }
    [[nodiscard]] summary_type identity() const noexcept { return monoid.identity(); }
    [[nodiscard]] summary_type combine(const summary_type& a, const summary_type& b) const noexcept { return monoid.combine(a, b); }
    [[nodiscard]] summary_type import_value(const value_type& v) const noexcept { return v; }
    [[nodiscard]] value_type export_value(const summary_type& v) const noexcept { return v; }
    [[nodiscard]] action_type identity_action() const noexcept { return action.identity(); }
    [[nodiscard]] action_type compose(const action_type& newer, const action_type& older) const noexcept { return action.compose(newer, older); }
    [[nodiscard]] summary_type map(const action_type& f, const summary_type& v, std::size_t count) const noexcept { return count ? action.map(f, v, count) : identity(); }
    [[nodiscard]] prepared_action prepare(const update_type& f) const noexcept { return f; }
    [[nodiscard]] action_type action_of(const prepared_action& f) const noexcept { return f; }
    [[nodiscard]] summary_type summarize(const leaf_type& leaf, std::uint64_t selected) const noexcept {
        auto total = identity();
        for (; selected; selected &= selected - 1) total = combine(total, leaf.values[std::countr_zero(selected)]);
        return total;
    }
    void materialize(leaf_type& leaf, std::uint64_t active, const action_type& f) const noexcept {
        for (; active; active &= active - 1) { auto& v = leaf.values[std::countr_zero(active)]; v = map(f, v, 1); }
    }
    [[nodiscard]] summary_type edit(leaf_type& leaf, std::uint64_t active, std::uint64_t selected,
        const prepared_action& f, const action_type& incoming, bool dirty, const summary_type&) const noexcept {
        if (dirty) materialize(leaf, active, incoming);
        materialize(leaf, selected, f);
        return summarize(leaf, active);
    }
};
} // namespace canard::kernel
namespace canard::spatial {
template <std::integral C, algebra::commutative_monoid M, typename A, unsigned B>
    requires(algebra::action_for<A, M> && algebra::nothrow_action_operations_v<M, A>)
struct kernel_binding<C, M, A, representation::ordinary, execution::scalar, B> {
    using type = kernel::spatial_scalar<C, M, A, B>;
};
} // namespace canard::spatial
// ===== include/canard/kernel/spatial_affine_sum.hpp =====
// ===== include/canard/numeric/montgomery32.hpp =====
#include <cstdint>
#include <limits>
namespace canard::numeric {
// Expert arithmetic policy. Words are deliberately raw: encode/decode mark
// representation changes; operators below preserve lazy residues in [0,2p).
// No primality assumption is needed for arithmetic or power().
template <std::uint32_t Modulus> struct montgomery32 {
    using word_type = std::uint32_t;
    using u32 = word_type;
    using u64 = std::uint64_t;
    static constexpr u32 modulus = Modulus;
    static_assert(Modulus > 1 && (Modulus & 1) && Modulus < (u32{1} << 30),
                  "Requires odd 1 < modulus < 2^30.");
    static constexpr u32 twice_modulus = 2 * modulus;
    static constexpr u32 negative_inverse = [] consteval {
        u32 inverse = 1;
        for (int step = 0; step < 5; ++step) {
            inverse *= 2 - modulus * inverse;
        }
        return -inverse;
    }();
    static constexpr u32 one = (u64{1} << 32) % modulus;
    static constexpr u32 radix_squared = u64{one} * one % modulus;
    static_assert(4 * u64{modulus} < (u64{1} << 32));
    // The fused reduction below also needs its 64-bit numerator not to overflow.
    static_assert(8 * u64{modulus} * modulus + u64{modulus} * 0xffff'ffffu <
                  std::numeric_limits<u64>::max());

    // REDC primitive: both words lie in [0, 2*modulus). Usually both are
    // encoded, producing an encoded result. One encoded and one ordinary
    // operand deliberately produce an ordinary result (encoded scaling).
    [[nodiscard]] static constexpr u32 multiply(u32 a, u32 b) noexcept {
        const u64 product = u64{a} * b;
        const u32 correction = static_cast<u32>(product) * negative_inverse;
        return static_cast<u32>((product + u64{correction} * modulus) >> 32);
    }

    [[nodiscard]] static constexpr u32 add(u32 a, u32 b) noexcept {
        const u32 sum = a + b;
        return sum >= twice_modulus ? sum - twice_modulus : sum;
    }

    [[nodiscard]] static constexpr u32 subtract(u32 a, u32 b) noexcept {
        return a >= b ? a - b : a + twice_modulus - b;
    }

    // Reduce a*b+c*d together. Both products must have the same encoding
    // degree: either encoded*encoded or encoded*ordinary for both terms.
    // The intermediate result is <3p; subtracting 2p
    // when necessary restores the [0,2p) invariant.
    [[nodiscard]] static constexpr u32 multiply_sum(u32 a, u32 b, u32 c, u32 d) noexcept {
        const u64 sum = u64{a} * b + u64{c} * d;
        const u32 correction = static_cast<u32>(sum) * negative_inverse;
        const u32 result = static_cast<u32>((sum + u64{correction} * modulus) >> 32);
        return result >= twice_modulus ? result - twice_modulus : result;
    }

    [[nodiscard]] static constexpr u32 encode(u32 value) noexcept {
        return multiply(value, radix_squared);
    }

    [[nodiscard]] static constexpr u32 decode(u32 value) noexcept {
        const u32 result = multiply(value, 1);
        return result >= modulus ? result - modulus : result;
    }

    [[nodiscard]] static constexpr u32 power(u32 base, u32 exponent) noexcept {
        u32 result = one;
        for (; exponent != 0; exponent >>= 1, base = multiply(base, base)) {
            if ((exponent & 1u) != 0) {
                result = multiply(result, base);
            }
        }
        return result;
    }
};
// Primality is checked at compile time only by algorithms requiring inversion.
[[nodiscard]] consteval bool is_prime32(std::uint32_t n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (std::uint32_t d = 3; std::uint64_t{d} * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

// Optional public scalar value type; exactly one word. The hot storage kernels
// use the policy above directly rather than aliasing arrays of these objects.
template <std::uint32_t Modulus> class montgomery_value {
    using arithmetic = montgomery32<Modulus>;
    std::uint32_t encoded_;
    struct encoded_tag {};
    constexpr montgomery_value(encoded_tag, std::uint32_t x) noexcept : encoded_(x) {}

  public:
    // Precondition: canonical < Modulus. No run-time checks or division here.
    explicit constexpr montgomery_value(std::uint32_t canonical = 0) noexcept
        : encoded_(arithmetic::encode(canonical)) {}
    [[nodiscard]] constexpr std::uint32_t value() const noexcept {
        return arithmetic::decode(encoded_);
    }
    friend constexpr montgomery_value operator+(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::add(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator-(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::subtract(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator*(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::multiply(a.encoded_, b.encoded_)};
    }
    friend constexpr bool operator==(montgomery_value a, montgomery_value b) noexcept {
        return a.value() == b.value();
    }
};
} // namespace canard::numeric
#include <array>

namespace canard::kernel {
// Shared invariant for scalar and AVX2 spatial execution. Uses the existing
// numeric policy; no dependency on sequence or fixed-index tree kernels.
template <std::integral Coordinate, std::uint32_t P, unsigned Capacity>
struct spatial_affine_sum : spatial_scalar_selection<Coordinate> {
    using field = numeric::montgomery32<P>;
    using value_type = std::uint32_t;
    using summary_type = value_type;
    using update_type = algebra::affine_map<P>;
    struct action_type { value_type multiplier, translation; };
    using prepared_action = action_type;
    struct alignas(32) leaf_type { std::array<value_type, Capacity> values{}; };
    spatial_affine_sum(algebra::modular_sum<P>, algebra::affine_on_sum<P>) noexcept {}
    [[nodiscard]] static summary_type read(const leaf_type& leaf, unsigned i) noexcept { return leaf.values[i]; }
    static void write(leaf_type& leaf, unsigned i, summary_type value) noexcept { leaf.values[i] = value; }
    [[nodiscard]] static leaf_type empty_leaf() noexcept { return {}; }
    [[nodiscard]] static summary_type identity() noexcept { return 0; }
    [[nodiscard]] static summary_type combine(summary_type a, summary_type b) noexcept { return field::add(a, b); }
    [[nodiscard]] static summary_type import_value(value_type x) noexcept { return x; }
    [[nodiscard]] static value_type export_value(summary_type x) noexcept { return x >= P ? x - P : x; }
    [[nodiscard]] static action_type identity_action() noexcept { return {field::one, 0}; }
    [[nodiscard]] static action_type compose(action_type newer, action_type older) noexcept {
        return {field::multiply(newer.multiplier, older.multiplier),
                field::add(field::multiply(newer.multiplier, older.translation), newer.translation)};
    }
    [[nodiscard]] static summary_type map(action_type f, summary_type x, std::size_t count) noexcept {
        return field::multiply_sum(f.multiplier, x, f.translation, static_cast<value_type>(count % P));
    }
    [[nodiscard]] static prepared_action prepare(update_type f) noexcept {
        return {field::encode(f.multiplier), field::encode(f.translation)};
    }
    [[nodiscard]] static action_type action_of(prepared_action f) noexcept { return f; }
    [[nodiscard]] static summary_type summarize(const leaf_type& leaf, std::uint64_t bits) noexcept {
        std::uint64_t total = 0;
        for (; bits; bits &= bits - 1) total += leaf.values[std::countr_zero(bits)];
        return static_cast<summary_type>(total % P);
    }
    static void materialize(leaf_type& leaf, std::uint64_t active, action_type f) noexcept {
        for (; active; active &= active - 1) { auto& value = leaf.values[std::countr_zero(active)]; value = map(f, value, 1); }
    }
    [[nodiscard]] static summary_type edit(leaf_type& leaf, std::uint64_t active, std::uint64_t selected,
        prepared_action f, action_type incoming, bool dirty, summary_type old) noexcept {
        if (dirty) materialize(leaf, active, incoming);
        const auto before = summarize(leaf, selected);
        materialize(leaf, selected, f);
        return field::add(old, field::subtract(map(f, before, std::popcount(selected)), before));
    }
    [[nodiscard]] static summary_type replace(leaf_type& leaf, unsigned lane, summary_type value,
        std::uint64_t before_active, summary_type old) noexcept {
        const auto before = (before_active >> lane & 1) ? leaf.values[lane] : 0;
        leaf.values[lane] = value;
        return field::add(old, field::subtract(value, before));
    }
};
} // namespace canard::kernel
namespace canard::spatial {
template <std::integral C, std::uint32_t P, unsigned B>
    requires(P > 1 && P < (1u << 30) && P % 2 == 1)
struct kernel_binding<C, algebra::modular_sum<P>, algebra::affine_on_sum<P>,
    representation::modular_affine, execution::scalar, B> {
    using type = kernel::spatial_affine_sum<C, P, B>;
};
} // namespace canard::spatial
// ===== include/canard/spatial/protocols.hpp =====
// ===== include/canard/structural/point_kd_layout.hpp =====
// ===== include/canard/structural/detail/point_kd_partition.hpp =====
#include <cstddef>
#include <cstdint>
#include <set>
#include <span>

namespace canard::structural::detail {
// Called only during immutable geometry construction. Exact cardinalities are
// needed only up to ceil(M / Capacity): higher counts stop the probe early.
// std::set gives deterministic comparison bounds even for adversarial integer
// patterns. Only one axis' temporary set is alive at a time.
template <std::integral Coordinate, bool Adaptive> class point_kd_partition_plan {
    int preferred_ = -1;
    using point_type = geometry::point2<Coordinate>;

    [[nodiscard]] static std::size_t bounded_cardinality(
        std::span<const point_type> points, bool y_axis, std::size_t limit) {
        std::set<Coordinate> distinct;
        for (const auto point : points) {
            distinct.insert(y_axis ? point.y : point.x);
            if (distinct.size() > limit) {
                break;
            }
        }
        return distinct.size();
    }

  public:
    explicit point_kd_partition_plan(std::span<const point_type> points, unsigned capacity) {
        if constexpr (Adaptive) {
            const auto limit = (points.size() + capacity - 1) / capacity;
            if (limit < 8) {
                return;
            }
            const auto nx = bounded_cardinality(points, false, limit);
            const auto ny = bounded_cardinality(points, true, limit);
            // At least roughly eight leaf capacities per distinct coordinate,
            // and an eightfold cardinality imbalance. A truncated count is a
            // lower bound; it cannot incorrectly prove the imbalance.
            if (nx <= limit / 8 && nx <= ny / 8) {
                preferred_ = 0;
            } else if (ny <= limit / 8 && ny <= nx / 8) {
                preferred_ = 1;
            }
        }
    }

    [[nodiscard]] unsigned operator()(std::span<const point_type> points,
                                      std::span<const std::uint32_t> ids,
                                      unsigned depth) const noexcept {
        if constexpr (Adaptive) {
            if (preferred_ >= 0) {
                const auto coordinate = [&](std::uint32_t id) noexcept {
                    return preferred_ ? points[id].y : points[id].x;
                };
                const auto first = coordinate(ids.front());
                for (const auto id : ids) {
                    if (coordinate(id) != first) {
                        return static_cast<unsigned>(preferred_);
                    }
                }
                // This node already lies on one equal-coordinate slab. Split
                // the remaining coordinate; never sacrifice the median balance.
                return 1u - static_cast<unsigned>(preferred_);
            }
        }
        return depth & 1u;
    }

    // Construction diagnostic: -1 means the alternating plan was retained.
    [[nodiscard]] int preferred_axis() const noexcept { return preferred_; }
};
} // namespace canard::structural::detail

namespace canard::structural {
// Expert extension point: Partition::plan_type<C> is constructed once from the
// registered coordinates and capacity, then selects axis 0 or 1 for each
// non-leaf span of registration IDs. Selection must not throw. Returning 0/1
// and retaining no dependencies beyond construction are semantic requirements.
template <typename Partition, typename Coordinate>
concept point_partition_for = std::integral<Coordinate> && requires(
    std::span<const geometry::point2<Coordinate>> points,
    std::span<const std::uint32_t> ids,
    const point_partition_plan_t<Partition, Coordinate>& plan) {
    point_partition_plan_t<Partition, Coordinate>{points, unsigned{64}};
    { plan(points, ids, unsigned{}) } noexcept -> std::same_as<unsigned>;
};
} // namespace canard::structural
#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <span>
#include <stdexcept>
#include <vector>

namespace canard::structural {
// Coordinate-block types do not depend on the construction policy. A kernel
// accepting this leaf type works with every admitted partition plan.
template <std::integral Coordinate, unsigned Capacity> struct point_kd_blocks {
    using bounds_type = geometry::point_bounds2<Coordinate>;
    static constexpr std::uint32_t none = std::numeric_limits<std::uint32_t>::max();
    struct node_type {
        bounds_type bounds;
        std::uint32_t first, last, right, leaf;
        [[nodiscard]] bool is_leaf() const noexcept { return leaf != none; }
    };
    struct alignas(32) leaf_type {
        std::array<Coordinate, Capacity> xs{}, ys{};
    };
};

// Registered immutable geometry: no payloads, activity flags, tags or ISA.
// The partition plan chooses an axis; the layout always splits by exact median
// count. Equal primary coordinates use the other axis, then registration ID.
template <std::integral Coordinate, unsigned Capacity,
          typename Partition = point_partition::alternating>
    requires point_partition_for<Partition, Coordinate>
class point_kd_layout {
    static_assert(Capacity >= 8 && Capacity <= 64 && std::has_single_bit(Capacity));
    using blocks_type = point_kd_blocks<Coordinate, Capacity>;
    using plan_type = point_partition_plan_t<Partition, Coordinate>;
  public:
    using point_type = geometry::point2<Coordinate>;
    using bounds_type = geometry::point_bounds2<Coordinate>;
    using node_type = typename blocks_type::node_type;
    using leaf_type = typename blocks_type::leaf_type;
    static constexpr auto none = blocks_type::none;
  private:
    std::vector<node_type> nodes_;
    std::vector<leaf_type> leaves_;
    std::vector<std::uint32_t> rank_, ids_;
    unsigned height_ = 0;

    std::uint32_t build(std::span<const point_type> points,
                        std::vector<std::uint32_t>& ids,
                        std::uint32_t first, std::uint32_t last, unsigned depth, const plan_type& partition) {
        height_ = std::max(height_, depth + 1);
        const auto index = static_cast<std::uint32_t>(nodes_.size());
        nodes_.push_back({{}, first, last, none, none});
        if (last - first <= Capacity) {
            const auto block = static_cast<std::uint32_t>(leaves_.size());
            leaves_.emplace_back();
            auto& leaf = leaves_.back();
            const auto p = points[ids[first]];
            bounds_type bounds{p.x, p.y, p.x, p.y};
            for (auto i = first; i < last; ++i) {
                const auto point = points[ids[i]];
                rank_[ids[i]] = i;
                leaf.xs[i - first] = point.x;
                leaf.ys[i - first] = point.y;
                bounds = bounds_type::join(bounds, {point.x, point.y, point.x, point.y});
            }
            nodes_[index].bounds = bounds;
            nodes_[index].leaf = block;
        } else {
            const auto middle = first + (last - first) / 2;
            const auto axis = partition(points, std::span<const std::uint32_t>{ids}.subspan(first, last - first), depth);
            assert(axis < 2);
            const auto less = [&](auto a, auto b) {
                const auto pa = points[a], pb = points[b];
                const auto ax = axis ? pa.y : pa.x;
                const auto bx = axis ? pb.y : pb.x;
                if (ax != bx) return ax < bx;
                const auto ay = axis ? pa.x : pa.y;
                const auto by = axis ? pb.x : pb.y;
                return ay != by ? ay < by : a < b;
            };
            std::nth_element(ids.begin() + first, ids.begin() + middle, ids.begin() + last, less);
            const auto left = build(points, ids, first, middle, depth + 1, partition);
            const auto right = build(points, ids, middle, last, depth + 1, partition);
            nodes_[index].right = right;
            nodes_[index].bounds = bounds_type::join(nodes_[left].bounds, nodes_[right].bounds);
        }
        return index;
    }
  public:
    point_kd_layout() = default;
    explicit point_kd_layout(std::span<const point_type> points) {
        if (points.size() > std::numeric_limits<std::uint32_t>::max() / 2)
            throw std::length_error("registered point geometry exceeds 32-bit indexing");
        rank_.resize(points.size());
        if (points.empty()) return;
        const auto blocks = (points.size() + Capacity - 1) / Capacity;
        nodes_.reserve(4 * blocks);
        leaves_.reserve(2 * blocks);
        std::vector<std::uint32_t> ids(points.size());
        std::iota(ids.begin(), ids.end(), 0u);
        const plan_type partition{points, Capacity};
        build(points, ids, 0, static_cast<std::uint32_t>(points.size()), 0, partition);
        ids_ = std::move(ids);
    }
    [[nodiscard]] std::size_t size() const noexcept { return rank_.size(); }
    [[nodiscard]] unsigned height() const noexcept { return height_; }
    [[nodiscard]] std::size_t node_count() const noexcept { return nodes_.size(); }
    [[nodiscard]] std::size_t leaf_count() const noexcept { return leaves_.size(); }
    [[nodiscard]] const node_type& node(std::uint32_t i) const noexcept { return nodes_[i]; }
    [[nodiscard]] const leaf_type& leaf(std::uint32_t i) const noexcept { return leaves_[i]; }
    [[nodiscard]] std::uint32_t rank(std::size_t id) const noexcept { assert(id < size()); return rank_[id]; }
    [[nodiscard]] std::uint32_t registration_id(std::uint32_t rank) const noexcept { return ids_[rank]; }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return nodes_.size() * sizeof(node_type) + leaves_.size() * sizeof(leaf_type) +
               (rank_.size() + ids_.size()) * sizeof(std::uint32_t);
    }
    void swap(point_kd_layout& other) noexcept {
        nodes_.swap(other.nodes_); leaves_.swap(other.leaves_);
        rank_.swap(other.rank_); ids_.swap(other.ids_); std::swap(height_, other.height_);
    }
};
} // namespace canard::structural
#include <concepts>
#include <type_traits>

namespace canard::spatial {
// Geometric permutation requires commutative aggregation. Every action must
// distribute over combining active points and respect newer-after-older order.
template <typename K, typename C, unsigned Capacity>
concept leaf_kernel = std::is_nothrow_copy_constructible_v<summary_type_t<K>> &&
    std::is_nothrow_copy_assignable_v<summary_type_t<K>> &&
    std::is_nothrow_copy_constructible_v<action_type_t<K>> &&
    std::is_nothrow_copy_assignable_v<action_type_t<K>> &&
    requires(const K& k, leaf_type_t<K>& leaf, const leaf_type_t<K>& observed,
    const value_type_t<K>& value, const summary_type_t<K>& sum, const action_type_t<K>& action,
    const prepared_action_t<K>& prepared, const update_type_t<K>& update,
    const typename structural::point_kd_layout<C, Capacity>::leaf_type& coordinates,
    geometry::rectangle2<C> rectangle, std::uint64_t bits) {
    { k.empty_leaf() } -> std::same_as<leaf_type_t<K>>;
    { k.identity() } noexcept -> std::same_as<summary_type_t<K>>;
    { k.combine(sum, sum) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.import_value(value) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.export_value(sum) } noexcept -> std::same_as<value_type_t<K>>;
    { k.identity_action() } noexcept -> std::same_as<action_type_t<K>>;
    { k.compose(action, action) } noexcept -> std::same_as<action_type_t<K>>;
    { k.map(action, sum, std::size_t{}) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.prepare(update) } noexcept -> std::same_as<prepared_action_t<K>>;
    { k.action_of(prepared) } noexcept -> std::same_as<action_type_t<K>>;
    { k.select(coordinates, rectangle, bits) } noexcept -> std::same_as<std::uint64_t>;
    { k.summarize(observed, bits) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.materialize(leaf, bits, action) } noexcept;
    { k.edit(leaf, bits, bits, prepared, action, true, sum) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.read(observed, 0u) } noexcept -> std::same_as<summary_type_t<K>>;
    { k.write(leaf, 0u, sum) } noexcept;
};
template <typename C, typename M, typename A, typename Profile>
concept supported_configuration = structural::point_partition_for<partition_for_t<Profile>, C> && requires {
    typename kernel_binding<C, M, A, representation_type_t<Profile>, execution_type_t<Profile>, Profile::leaf_capacity>::type;
} && leaf_kernel<typename kernel_binding<C, M, A, representation_type_t<Profile>,
    execution_type_t<Profile>, Profile::leaf_capacity>::type, C, Profile::leaf_capacity>;
template <typename C, typename M, typename A, typename Profile>
    requires supported_configuration<C, M, A, Profile>
using kernel_for_t = typename kernel_binding<C, M, A, representation_type_t<Profile>,
    execution_type_t<Profile>, Profile::leaf_capacity>::type;
} // namespace canard::spatial
// ===== include/canard/spatial/detail/registered_point_engine.hpp =====
// ===== include/canard/memory/point_kd_storage.hpp =====
#include <cstdint>
#include <vector>

namespace canard::memory {
// Fixed-sized after construction. Geometry owns addressing; the family engine
// owns construction and maintenance semantics. Operations perform no allocation.
template <typename Kernel> struct point_kd_storage {
    using summary_type = summary_type_t<Kernel>;
    using action_type = action_type_t<Kernel>;
    using leaf_type = leaf_type_t<Kernel>;
    struct state_type {
        summary_type sum;
        action_type pending;
        std::uint64_t active; // Only leaf states use this mask.
        std::uint32_t count;
        bool dirty;
    };
    std::vector<state_type> states;
    std::vector<leaf_type> leaves;
    point_kd_storage() = default;
    point_kd_storage(std::size_t nodes, std::size_t blocks, const Kernel& k)
        : states(nodes, state_type{k.identity(), k.identity_action(), 0, 0, false}),
          leaves(blocks, k.empty_leaf()) {}
    void swap(point_kd_storage& other) noexcept { states.swap(other.states); leaves.swap(other.leaves); }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return states.size() * sizeof(state_type) + leaves.size() * sizeof(leaf_type);
    }
};
} // namespace canard::memory
#include <bit>
#include <span>
#include <utility>

namespace canard::spatial::detail {
template <typename Coordinate, typename Kernel, unsigned Capacity, typename Partition = structural::point_partition::alternating> class registered_point_engine {
  public:
    using value_type = value_type_t<Kernel>;
    using summary_type = summary_type_t<Kernel>;
    using action_type = action_type_t<Kernel>;
    using prepared_action = prepared_action_t<Kernel>;
    using update_type = update_type_t<Kernel>;
    using point_type = geometry::point2<Coordinate>;
    using rectangle_type = geometry::rectangle2<Coordinate>;
    struct observation { summary_type value; std::uint32_t count; };
  private:
    [[no_unique_address]] Kernel kernel_;
    structural::point_kd_layout<Coordinate, Capacity, Partition> layout_;
    memory::point_kd_storage<Kernel> storage_;

    void pull(std::uint32_t index) noexcept {
        auto& s = storage_.states[index];
        const auto& l = storage_.states[index + 1];
        const auto& r = storage_.states[layout_.node(index).right];
        s.sum = kernel_.combine(l.sum, r.sum);
        s.count = l.count + r.count;
    }
    void apply_node(std::uint32_t i, const action_type& action) noexcept {
        auto& s = storage_.states[i];
        // An empty subtree must not retain this update for future activations.
        if (!s.count) return;
        s.sum = kernel_.map(action, s.sum, s.count);
        s.pending = s.dirty ? kernel_.compose(action, s.pending) : action;
        s.dirty = true;
    }
    void push(std::uint32_t index) noexcept {
        auto& s = storage_.states[index];
        if (!s.dirty) return;
        apply_node(index + 1, s.pending);
        apply_node(layout_.node(index).right, s.pending);
        s.dirty = false;
    }
    [[nodiscard]] observation fold_impl(std::uint32_t index, rectangle_type rectangle) const noexcept {
        const auto& n = layout_.node(index);
        const auto& s = storage_.states[index];
        if (!s.count || n.bounds.disjoint(rectangle)) return {kernel_.identity(), 0};
        if (n.bounds.covered_by(rectangle)) return {s.sum, s.count};
        observation part{kernel_.identity(), 0};
        if (n.is_leaf()) {
            const auto selected = kernel_.select(layout_.leaf(n.leaf), rectangle, s.active);
            part = {kernel_.summarize(storage_.leaves[n.leaf], selected), static_cast<std::uint32_t>(std::popcount(selected))};
        } else {
            const auto left = fold_impl(index + 1, rectangle);
            const auto right = fold_impl(n.right, rectangle);
            part = {kernel_.combine(left.value, right.value), left.count + right.count};
        }
        // Lift after combining children. A covered summary already includes
        // this node's action. Partial observations receive it exactly once.
        if (s.dirty && part.count) part.value = kernel_.map(s.pending, part.value, part.count);
        return part;
    }
    void apply_impl(std::uint32_t index, rectangle_type rectangle, const prepared_action& prepared) noexcept {
        const auto& n = layout_.node(index);
        auto& s = storage_.states[index];
        if (!s.count || n.bounds.disjoint(rectangle)) return;
        if (n.bounds.covered_by(rectangle)) { apply_node(index, kernel_.action_of(prepared)); return; }
        if (n.is_leaf()) {
            const auto selected = kernel_.select(layout_.leaf(n.leaf), rectangle, s.active);
            if (!selected) return;
            if (selected == s.active) { apply_node(index, kernel_.action_of(prepared)); return; }
            s.sum = kernel_.edit(storage_.leaves[n.leaf], s.active, selected, prepared, s.pending, s.dirty, s.sum);
            s.dirty = false;
        } else {
            push(index);
            apply_impl(index + 1, rectangle, prepared);
            apply_impl(n.right, rectangle, prepared);
            pull(index);
        }
    }
    void set_impl(std::uint32_t index, std::uint32_t position, const summary_type& value, bool activating) noexcept {
        const auto& n = layout_.node(index);
        auto& s = storage_.states[index];
        if (n.is_leaf()) {
            const unsigned lane = position - n.first;
            const auto bit = std::uint64_t{1} << lane;
            assert(activating ? !(s.active & bit) : bool(s.active & bit));
            auto& leaf = storage_.leaves[n.leaf];
            // Materialize OLD live lanes before setting the activation bit.
            // A newly inserted point sees no action from before its insertion.
            if (s.dirty) kernel_.materialize(leaf, s.active, s.pending);
            if constexpr (requires { { kernel_.replace(leaf, lane, value, s.active, s.sum) } noexcept -> std::same_as<summary_type>; }) {
                s.sum = kernel_.replace(leaf, lane, value, s.active, s.sum);
            } else {
                kernel_.write(leaf, lane, value);
                s.sum = kernel_.summarize(leaf, s.active | bit);
            }
            s.active |= bit;
            s.count += activating;
            s.dirty = false;
        } else {
            push(index);
            const auto child = position < layout_.node(n.right).first ? index + 1 : n.right;
            set_impl(child, position, value, activating);
            pull(index);
        }
    }
    [[nodiscard]] summary_type get_impl(std::uint32_t index, std::uint32_t position) const noexcept {
        const auto& n = layout_.node(index);
        const auto& s = storage_.states[index];
        auto result = kernel_.identity();
        if (n.is_leaf()) {
            assert((s.active >> (position - n.first)) & 1);
            result = kernel_.read(storage_.leaves[n.leaf], position - n.first);
        } else {
            const auto child = position < layout_.node(n.right).first ? index + 1 : n.right;
            result = get_impl(child, position);
        }
        return s.dirty ? kernel_.map(s.pending, result, 1) : result;
    }
    void build(std::uint32_t index, std::span<const value_type> initial) {
        const auto& n = layout_.node(index);
        auto& s = storage_.states[index];
        if (n.is_leaf()) {
            auto& leaf = storage_.leaves[n.leaf];
            for (auto rank = n.first; rank < n.last; ++rank) {
                const auto id = layout_.registration_id(rank);
                if (id >= initial.size()) continue;
                kernel_.write(leaf, rank - n.first, kernel_.import_value(initial[id]));
                s.active |= std::uint64_t{1} << (rank - n.first);
                ++s.count;
            }
            s.sum = kernel_.summarize(leaf, s.active);
        } else {
            build(index + 1, initial); build(n.right, initial); pull(index);
        }
    }
  public:
    registered_point_engine(std::span<const point_type> points, std::span<const value_type> initial, Kernel kernel)
        : kernel_(std::move(kernel)), layout_(points), storage_(layout_.node_count(), layout_.leaf_count(), kernel_) {
        if (initial.size() > points.size()) throw std::invalid_argument("more initial values than registered points");
        if (!points.empty()) build(0, initial);
    }
    registered_point_engine(const registered_point_engine&) = default;
    registered_point_engine(registered_point_engine&& other) noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_) { layout_.swap(other.layout_); storage_.swap(other.storage_); }
    registered_point_engine& operator=(registered_point_engine other) noexcept(std::is_nothrow_swappable_v<Kernel>)
        requires(std::is_nothrow_swappable_v<Kernel>) { swap(other); return *this; }
    void swap(registered_point_engine& other) noexcept requires(std::is_nothrow_swappable_v<Kernel>) {
        using std::swap; swap(kernel_, other.kernel_); layout_.swap(other.layout_); storage_.swap(other.storage_);
    }
    [[nodiscard]] std::size_t size() const noexcept { return layout_.size(); }
    [[nodiscard]] std::uint32_t active_size() const noexcept { return size() ? storage_.states[0].count : 0; }
    [[nodiscard]] bool empty() const noexcept { return size() == 0; }
    [[nodiscard]] unsigned height() const noexcept { return layout_.height(); }
    [[nodiscard]] std::size_t storage_bytes() const noexcept { return layout_.storage_bytes() + storage_.storage_bytes(); }
    [[nodiscard]] bool contains(std::size_t id) const noexcept {
        const auto position = layout_.rank(id);
        std::uint32_t index = 0;
        while (!layout_.node(index).is_leaf()) {
            const auto right = layout_.node(index).right;
            index = position < layout_.node(right).first ? index + 1 : right;
        }
        return (storage_.states[index].active >> (position - layout_.node(index).first)) & 1;
    }
    [[nodiscard]] value_type get(std::size_t id) const noexcept { return kernel_.export_value(get_impl(0, layout_.rank(id))); }
    void activate(std::size_t id, const value_type& value) noexcept { set_impl(0, layout_.rank(id), kernel_.import_value(value), true); }
    void set(std::size_t id, const value_type& value) noexcept { set_impl(0, layout_.rank(id), kernel_.import_value(value), false); }
    void apply(rectangle_type rectangle, const update_type& update) noexcept {
        if (!active_size() || rectangle.empty()) return;
        apply_impl(0, rectangle, kernel_.prepare(update));
    }
    [[nodiscard]] value_type fold(rectangle_type rectangle) const noexcept {
        if (empty() || rectangle.empty()) return kernel_.export_value(kernel_.identity());
        return kernel_.export_value(fold_impl(0, rectangle).value);
    }
    [[nodiscard]] value_type all_fold() const noexcept { return kernel_.export_value(empty() ? kernel_.identity() : storage_.states[0].sum); }
};
} // namespace canard::spatial::detail
// ===== include/canard/range/maintained_reference.hpp =====
#include <cstddef>
#include <concepts>
#include <format>
#include <string_view>
#include <utility>

namespace canard {

// A copied proxy copies the handle. Assigning a proxy writes a VALUE, never
// silently rebinds a handle. Snapshot the RHS before starting maintenance.
template <typename Owner> class point_reference {
    Owner* owner_;
    std::size_t position_;

  public:
    using value_type = value_type_t<Owner>;
    point_reference(Owner& owner, std::size_t position) noexcept
        : owner_(&owner), position_(position) {}
    point_reference(const point_reference&) noexcept = default;
    [[nodiscard]] value_type value() const {
        return owner_->get(position_);
    }
    operator value_type() const {
        return value();
    }
    const point_reference& operator=(const value_type& replacement) const noexcept(noexcept(owner_->set(position_, replacement)))
        requires requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); }
    {
        owner_->set(position_, replacement);
        return *this;
    }
    const point_reference& operator=(const point_reference& other) const
        requires requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); }
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Other>
    const point_reference& operator=(const point_reference<Other>& other) const
        requires(std::same_as<value_type, value_type_t<Other>> &&
                 requires(Owner& owner, const value_type& value) { owner.set(std::size_t{}, value); })
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
        owner_->apply(position_, position_ + 1, action);
    }
};

template <typename Owner> class interval_reference {
    Owner* owner_;
    std::size_t first_, last_;

  public:
    interval_reference(Owner& owner, std::size_t first, std::size_t last) noexcept
        : owner_(&owner), first_(first), last_(last) {}
    [[nodiscard]] auto fold() const {
        return owner_->fold(first_, last_);
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
        owner_->apply(first_, last_, action);
    }
    template <typename Value>
    void assign(const Value& value) const
        noexcept(noexcept(owner_->assign(first_, last_, value)))
        requires requires(Owner& owner) { owner.assign(first_, last_, value); } {
        owner_->assign(first_, last_, value);
    }
    void reverse() const noexcept(noexcept(owner_->reverse(first_, last_)))
        requires requires(Owner& owner) { owner.reverse(first_, last_); } {
        owner_->reverse(first_, last_);
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return last_ - first_;
    }
};

struct wide_tree_description {
    std::size_t size;
    unsigned fanout;
    unsigned height;
    std::size_t storage_bytes;
    bool lazy;
};

} // namespace canard

// One read per format operation. Numeric presentation comes from the value's
// own formatter. Public facades include this header, not an optional plugin.
template <typename Owner>
    requires std::formattable<canard::value_type_t<Owner>, char>
struct std::formatter<canard::point_reference<Owner>, char>
    : std::formatter<canard::value_type_t<Owner>, char> {
    template <typename Context>
    auto format(const canard::point_reference<Owner>& reference, Context& context) const {
        const auto snapshot = reference.value();
        return std::formatter<canard::value_type_t<Owner>, char>::format(snapshot, context);
    }
};

template <> struct std::formatter<canard::wide_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::wide_tree_description x, Context& context) const {
        return std::format_to(context.out(),
                              "{}(size={}, fanout={}, height={}, storage={} bytes)",
                              x.lazy ? "wide_lazy_segment_tree" : "wide_segment_tree",
                              x.size,
                              x.fanout,
                              x.height,
                              x.storage_bytes);
    }
};
// ===== include/canard/format/owner.hpp =====
#include <format>
#include <utility>
namespace canard::format {
// Delegation preserves the description's parser. Formatting an owner never
// walks its values; use an explicitly constructed snapshot for content output.
template <typename Owner> struct description_formatter {
    using description_type = decltype(std::declval<const Owner&>().description());
    std::formatter<description_type, char> base;
    constexpr auto parse(std::format_parse_context& context) {
        return base.parse(context);
    }
    template <typename Context>
    auto format(const Owner& owner, Context& context) const {
        return base.format(owner.description(), context);
    }
};
}
#include <format>
#include <span>

namespace canard::spatial {
template <typename Owner> class rectangle_reference {
    Owner* owner_;
    typename Owner::rectangle_type rectangle_;
  public:
    rectangle_reference(Owner& owner, typename Owner::rectangle_type rectangle) noexcept
        : owner_(&owner), rectangle_(rectangle) {}
    [[nodiscard]] auto fold() const noexcept { return owner_->fold(rectangle_); }
    template <typename Tag> void apply(const Tag& tag) const noexcept
        requires requires(Owner& o) { o.apply(rectangle_, tag); } { owner_->apply(rectangle_, tag); }
};
struct point_tree_description { std::size_t registered, active, bytes; unsigned capacity, height; };
} // namespace canard::spatial
namespace canard {
// Coordinates fixed at construction. Registration IDs survive kd permutation;
// duplicate locations remain independent. Activity and values change online.
template <std::integral Coordinate, algebra::commutative_monoid Monoid, typename Action,
          typename Configuration = spatial::configuration<>>
    requires(algebra::action_for<Action, Monoid> && spatial::supported_configuration<Coordinate, Monoid, Action, Configuration>)
class registered_point_tree : private spatial::detail::registered_point_engine<Coordinate,
    spatial::kernel_for_t<Coordinate, Monoid, Action, Configuration>, Configuration::leaf_capacity, spatial::partition_for_t<Configuration>> {
    using kernel = spatial::kernel_for_t<Coordinate, Monoid, Action, Configuration>;
    using base = spatial::detail::registered_point_engine<Coordinate, kernel, Configuration::leaf_capacity, spatial::partition_for_t<Configuration>>;
  public:
    using coordinate_type = Coordinate;
    using point_type = geometry::point2<Coordinate>;
    using rectangle_type = geometry::rectangle2<Coordinate>;
    using value_type = value_type_t<Monoid>;
    using element_type = value_type;
    using aggregate_type = value_type;
    using size_type = std::size_t;
    using tag_type = tag_type_t<Action>;
    using update_type = tag_type;
    using monoid_type = Monoid;
    using action_policy_type = Action;
    using configuration_type = Configuration;
    static constexpr bool nothrow_mutation = true;
    explicit registered_point_tree(std::span<const point_type> points, Monoid monoid = {}, Action action = {}, Configuration = {})
        : base(points, {}, kernel{std::move(monoid), std::move(action)}) {}
    // Initial values activate the first initial.size() registration IDs.
    registered_point_tree(std::span<const point_type> points, std::span<const value_type> initial,
        Monoid monoid = {}, Action action = {}, Configuration = {})
        : base(points, initial, kernel{std::move(monoid), std::move(action)}) {}
    registered_point_tree(const registered_point_tree&) = default;
    registered_point_tree(registered_point_tree&&) = default;
    registered_point_tree& operator=(const registered_point_tree&) = default;
    registered_point_tree& operator=(registered_point_tree&&) = default;
    using base::size; using base::empty; using base::active_size; using base::contains;
    using base::activate; using base::set; using base::get; using base::fold; using base::all_fold;
    using base::apply; using base::height; using base::storage_bytes;
    void swap(registered_point_tree& other) noexcept requires(std::is_nothrow_swappable_v<kernel>) { base::swap(other); }
    friend void swap(registered_point_tree& a, registered_point_tree& b) noexcept
        requires(std::is_nothrow_swappable_v<kernel>) { a.swap(b); }
    [[nodiscard]] point_reference<registered_point_tree> operator[](size_type id) & noexcept { return {*this, id}; }
    [[nodiscard]] value_type operator[](size_type id) const & noexcept { return this->get(id); }
    void operator[](size_type) && = delete;
    void operator[](size_type) const && = delete;
    [[nodiscard]] spatial::rectangle_reference<registered_point_tree> operator[](rectangle_type r) & noexcept { return {*this, r}; }
    [[nodiscard]] spatial::rectangle_reference<const registered_point_tree> operator[](rectangle_type r) const & noexcept { return {*this, r}; }
    void operator[](rectangle_type) && = delete;
    void operator[](rectangle_type) const && = delete;
    [[nodiscard]] spatial::point_tree_description description() const noexcept {
        return {this->size(), this->active_size(), this->storage_bytes(), Configuration::leaf_capacity, this->height()};
    }
};
template <std::integral C, typename M, typename A, typename P = spatial::configuration<>>
registered_point_tree(std::span<const geometry::point2<C>>, M, A, P = {}) -> registered_point_tree<C, M, A, P>;
template <std::integral C, typename T, typename M, typename A, typename P = spatial::configuration<>>
registered_point_tree(std::span<const geometry::point2<C>>, std::span<const T>, M, A, P = {}) -> registered_point_tree<C, M, A, P>;
} // namespace canard

template <> struct std::formatter<canard::spatial::point_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) { return context.begin(); }
    template <typename Context> auto format(canard::spatial::point_tree_description d, Context& context) const {
        return std::format_to(context.out(), "registered_point_tree(size={}, active={}, capacity={}, height={}, storage={} bytes)",
            d.registered, d.active, d.capacity, d.height, d.bytes);
    }
};
template <typename C, typename M, typename A, typename P>
struct std::formatter<canard::registered_point_tree<C,M,A,P>, char>
    : canard::format::description_formatter<canard::registered_point_tree<C,M,A,P>> {};
// ===== include/canard/io/buffered_writer.hpp =====
// ===== include/canard/io/protocols.hpp =====
// ===== include/canard/io/decimal/field.hpp =====
namespace canard::io::decimal {
struct field {
    const char* data;
    unsigned digits;
};
}

namespace canard::io::decimal {
struct u64_field {
    const char* source;
    unsigned digits;
};
}
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
namespace canard::io {
template <typename C>
concept unsigned64_codec = requires(const char* bytes, decimal::u64_field field) {
    requires std::same_as<value_type_t<C>, std::uint64_t>;
    { C::delimiters32(bytes) } noexcept -> std::same_as<std::uint32_t>;
    { C::decode(field) } noexcept -> std::same_as<std::uint64_t>;
    { C::decode_pair(field, field) } noexcept -> std::same_as<std::array<std::uint64_t, 2>>;
};
template <typename F, typename Word = value_type_t<F>>
concept integer_formatter = std::same_as<Word, value_type_t<F>> && requires(const F& f, char* p, Word x) {
    typename std::integral_constant<std::size_t, F::token_capacity>;
    requires (F::token_capacity > 0);
    { f(p, x) } noexcept -> std::same_as<char*>;
};
template <typename F, typename Word = value_type_t<F>>
concept paired_integer_formatter = integer_formatter<F, Word> && requires(const F& f, char* p, Word x) {
    { f.format_pair(p, x, x, char{}) } noexcept -> std::same_as<char*>;
};
} // namespace canard::io
// ===== include/canard/io/decimal_format.hpp =====
#include <array>
#include <bit>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <limits>
namespace canard::io::decimal {
inline constexpr auto four_digits = [] consteval {
    std::array<std::array<char, 4>, 10'000> result{};
    for (unsigned value = 0; value < result.size(); ++value) {
        result[value] = {static_cast<char>('0' + value / 1000),
                         static_cast<char>('0' + value / 100 % 10),
                         static_cast<char>('0' + value / 10 % 10),
                         static_cast<char>('0' + value % 10)};
    }
    return result;
}();

// Writes canonical decimal plus newline; at least 16 writable bytes supplied.
// Precondition: value <= MaxValue. Default handles all uint32 values;
// a nine-digit bound preserves the original solver's one-byte leading group.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
[[nodiscard]] inline char* format_u32_line(char* cursor, char* end, std::uint32_t value) noexcept {
    // Most answers have nine digits. Emit their eight low digits with
    // two fixed-size copies; use to_chars only for a short leading group.
    auto append_four = [&cursor](unsigned group) noexcept {
        std::memcpy(cursor, four_digits[group].data(), 4);
        cursor += 4;
    };
    if (value >= 100'000'000) {
        unsigned high = value / 100'000'000;
        value %= 100'000'000;
        if constexpr (MaxValue < 1'000'000'000) {
            *cursor++ = static_cast<char>('0' + high);
        } else {
            cursor = std::to_chars(cursor, end, high).ptr;
        }
        append_four(value / 10'000);
        append_four(value % 10'000);
    } else if (value >= 10'000) {
        cursor = std::to_chars(cursor, end, value / 10'000).ptr;
        append_four(value % 10'000);
    } else {
        cursor = std::to_chars(cursor, end, value).ptr;
    }
    *cursor++ = '\n';
    return cursor;
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// At least 24 writable bytes. Canonical uint64_t decimal, no delimiter.
// Reuses the shared base-10^4 table; first-group trimming uses one word copy.
[[nodiscard]] inline char* format_u64_token(char* cursor, std::uint64_t value) noexcept {
    if constexpr (std::endian::native != std::endian::little)
        return std::to_chars(cursor, cursor + 24, value).ptr;
    auto group = [&cursor](unsigned x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    auto first = [&cursor](unsigned x) {
        const unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
        std::uint32_t word;
        std::memcpy(&word, four_digits[x].data(), 4);
        word >>= 8 * skip;
        std::memcpy(cursor, &word, 4);
        cursor += 4 - skip;
    };
    if (value >= 10'000'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL, d = value / 10'000'000'000'000'000ULL;
        first(d);
        group(c - d * 10'000);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 1'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL;
        first(c);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 100'000'000) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000;
        first(b);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 10'000) {
        const std::uint64_t a = value / 10'000;
        first(a);
        group(value - a * 10'000);
    } else
        first(value);
    return cursor;
}

// Canonical decimal token, no delimiter. At least 16 writable bytes are required.
// The first group suppresses leading zeros; all following groups have four digits.
// Like the existing formatter, writes may touch unused bytes after the token.
[[nodiscard]] inline char* format_u32_compact(char* cursor, std::uint32_t value) noexcept {
    auto first = [&](std::uint32_t x) {
        if (x < 10) {
            *cursor++ = static_cast<char>('0' + x);
            return;
        }
        const unsigned skip = 3 - unsigned(x >= 10) - unsigned(x >= 100) - unsigned(x >= 1000);
        if constexpr (std::endian::native == std::endian::little) {
            std::uint32_t word;
            std::memcpy(&word, four_digits[x].data(), 4);
            word >>= 8 * skip;
            std::memcpy(cursor, &word, 4);
        } else {
            std::memcpy(cursor, four_digits[x].data() + skip, 4 - skip);
        }
        cursor += 4 - skip;
    };
    auto next = [&](std::uint32_t x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    if (value >= 100000000u) {
        first(value / 100000000u);
        next(value / 10000u % 10000u);
        next(value % 10000u);
    } else if (value >= 10000u) {
        first(value / 10000u);
        next(value % 10000u);
    } else
        first(value);
    return cursor;
}
} // namespace canard::io::decimal
#include <cstddef>
#include <concepts>
#include <type_traits>
#include <memory>
#include <span>
// ===== include/canard/io/output_buffer.hpp =====
// ===== include/canard/io/transfer.hpp =====
#include <cstddef>
#include <system_error>
namespace canard::io {
struct transfer_result {
    std::size_t count = 0;
    std::error_code error{};
};
}
#include <concepts>
#include <cstring>
#include <memory>
#include <span>
#include <utility>
namespace canard::io {
template <typename S>
concept byte_sink = (requires { requires S::assume_complete_writes; } &&
    requires(S& sink, const char* data, std::size_t n) { { sink.write_all(data, n) } noexcept; }) ||
    requires(S& sink, const char* data, std::size_t n) {
        { sink.write_some(data, n) } noexcept -> std::same_as<transfer_result>;
    };
// Shared byte ownership only. Decimal syntax and ISA are not part of this type.
// Explicit flush/finish reports errors; an error retains the unwritten suffix.
// Destruction attempts flush but cannot report errors: callers needing delivery
// confirmation must call finish() while the object is still alive.
template <byte_sink Sink, std::size_t Capacity = (1u << 16)>
class output_buffer {
  protected:
    static_assert(Capacity >= 64);
    static constexpr std::size_t capacity = Capacity;
    std::unique_ptr<char[]> storage_ = std::make_unique_for_overwrite<char[]>(Capacity);
    char* cursor_ = storage_.get();
    [[no_unique_address]] Sink sink_;
  public:
    explicit output_buffer(Sink sink = Sink{}) : sink_(std::move(sink)) {}
    output_buffer(const output_buffer&) = delete;
    output_buffer& operator=(const output_buffer&) = delete;
    ~output_buffer() noexcept { try { flush(); } catch (...) {} }
    void flush() {
        const auto size = static_cast<std::size_t>(cursor_ - storage_.get());
        if constexpr (requires { requires Sink::assume_complete_writes; }) {
            sink_.write_all(storage_.get(), size);
            cursor_ = storage_.get();
        } else {
            std::size_t sent = 0;
            while (sent != size) {
                const auto result = sink_.write_some(storage_.get() + sent, size - sent);
                // A sink must never report consuming bytes outside its input.
                if (result.count > size - sent)
                    std::terminate();
                sent += result.count;
                auto error = result.error;
                if (!error && result.count == 0)
                    error = std::make_error_code(std::errc::io_error);
                if (error) {
                    std::memmove(storage_.get(), storage_.get() + sent, size - sent);
                    cursor_ = storage_.get() + size - sent;
                    throw std::system_error(error, "canard output");
                }
            }
            cursor_ = storage_.get();
        }
    }
    void finish() {
        flush();
        if constexpr (requires { sink_.finish(); }) {
            if (auto error = sink_.finish())
                throw std::system_error(error, "canard output finish");
        }
    }
    [[nodiscard]] std::span<const char> pending_bytes() const & noexcept {
        return {storage_.get(), static_cast<std::size_t>(cursor_ - storage_.get())};
    }
    void pending_bytes() const && = delete;
    void write_bytes(std::span<const char> bytes) {
        while (!bytes.empty()) {
            auto room = static_cast<std::size_t>(storage_.get() + capacity - cursor_);
            if (room == 0) {
                flush();
                room = capacity;
            }
            const auto count = bytes.size() < room ? bytes.size() : room;
            std::memcpy(cursor_, bytes.data(), count);
            cursor_ += count;
            bytes = bytes.subspan(count);
        }
    }
};
}
// ===== include/canard/io/sink/stdio.hpp =====
#include <cerrno>
#include <cstdio>
#include <stdexcept>
namespace canard::io {
class stdio_sink {
    std::FILE* file_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit stdio_sink(std::FILE* file = stdout) : file_(file) {
        if (!file)
            throw std::invalid_argument("canard: null FILE handle");
    }
    transfer_result write_some(const char* data, std::size_t size) noexcept {
        for (;;) {
            errno = 0;
            const auto count = std::fwrite(data, 1, size, file_);
            if (!std::ferror(file_)) return {count, {}};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error == std::errc::interrupted) {
                std::clearerr(file_);
                if (count != 0) return {count, {}};
                continue;
            }
            return {count, error};
        }
    }
    std::error_code finish() noexcept {
        for (;;) {
            errno = 0;
            if (std::fflush(file_) == 0) return {};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error != std::errc::interrupted)
                return error;
            std::clearerr(file_);
        }
    }
};
}
namespace canard::io {
// Numeric formatting over the shared byte buffer. The default stdio sink is
// checked; trusted POSIX transport is an explicit Sink policy. Call finish()
// to observe transport failures; destructor flush is best effort.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max(), byte_sink Sink = stdio_sink>
class buffered_writer : private output_buffer<Sink> {
    using base = output_buffer<Sink>;
    using base::capacity;
    using base::storage_;
    using base::cursor_;
  public:
    explicit buffered_writer(Sink sink = Sink{}) : base(std::move(sink)) {}
    using base::flush;
    using base::finish;
    using base::pending_bytes;
    using base::write_bytes;
    // Canonical unsigned pair plus newline. Shares the existing buffer and
    // descriptor lifetime; no problem-specific I/O stack.
    void write_pair(std::uint64_t first, std::uint64_t second) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 48)
            flush();
        cursor_ = decimal::format_u64_token(cursor_, first);
        *cursor_++ = ' ';
        cursor_ = decimal::format_u64_token(cursor_, second);
        *cursor_++ = '\n';
    }

    // Fixed-size decimal batches reserve buffer space once. A conversion policy
    // supplies its maximum token footprint and returns the logical token end;
    // this owner still controls the byte buffer and selected transport.
    template <typename Word, std::size_t Extent, typename Formatter>
        requires integer_formatter<Formatter, Word>
    void write_batch(std::span<const Word, Extent> values, const Formatter& formatter) {
        constexpr std::size_t stride = Formatter::token_capacity + 1;
        static_assert(Formatter::token_capacity > 0 && Formatter::token_capacity < capacity);
        if constexpr (Extent != std::dynamic_extent && Extent <= capacity / stride) {
            if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(stride * Extent))
                flush();
            char* output = cursor_;
            if constexpr (paired_integer_formatter<Formatter, Word>) {
                std::size_t i = 0;
                for (; i + 1 < values.size(); i += 2) {
                    output = formatter.format_pair(output, values[i], values[i + 1], char(10));
                    *output++ = char(10);
                }
                if (i < values.size()) {
                    output = formatter(output, values[i]);
                    *output++ = char(10);
                }
            } else {
                for (const auto value : values) {
                    output = formatter(output, value);
                    *output++ = char(10);
                }
            }
            cursor_ = output;
        } else {
            for (const auto value : values) {
                if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(stride))
                    flush();
                cursor_ = formatter(cursor_, value);
                *cursor_++ = char(10);
            }
        }
    }

    void write_u64(std::uint64_t value) {
        if (storage_.get() + capacity - cursor_ < 24)
            flush();
        cursor_ = decimal::format_u64_token(cursor_, value);
        *cursor_++ = '\n';
    }

    // One newline per value, in input order. A small fixed-size batch reserves
    // buffer space once and commits the cursor once; larger/dynamic spans use
    // the same bounded scalar entry point. At most 16 bytes are touched per value.
    template <std::size_t Extent>
    void write_compact_batch(std::span<const std::uint32_t, Extent> values) {
        if constexpr (Extent != std::dynamic_extent && Extent <= capacity / 16) {
            if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(16 * Extent))
                flush();
            char* next = cursor_;
            for (const auto value : values) {
                next = decimal::format_u32_compact(next, value);
                *next++ = '\n';
            }
            cursor_ = next;
        } else {
            for (const auto value : values)
                write_compact_u32(value);
        }
    }

    // Canonical uint32 decimal through the shared base-10^4 table.
    void write_compact_u32(std::uint32_t value) {
        if (storage_.get() + capacity - cursor_ < 16)
            flush();
        cursor_ = decimal::format_u32_compact(cursor_, value);
        *cursor_++ = '\n';
    }

    void write(std::uint32_t value) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 16) {
            flush();
        }
        cursor_ = decimal::format_u32_line<MaxValue>(cursor_, end, value);
    }
};

} // namespace canard::io
// ===== include/canard/backend/avx2/spatial.hpp =====
// ===== include/canard/kernel/spatial_affine_sum_avx2.hpp =====
// ===== include/canard/numeric/montgomery32_avx2.hpp =====
// ===== include/canard/simd/avx2.hpp =====
// A deliberately small, concrete AVX2 vocabulary; not a std::simd emulation.
// No dispatch, allocation, bounds checks, or implicit scalar conversions.
#include <bit>
#include <cstdint>
#include <type_traits>
#include <immintrin.h>
#ifndef __AVX2__
#error "This backend requires AVX2 (e.g. -march=znver3 or -march=native)."
#endif
namespace canard::simd {
struct mask32x8;
namespace detail {
struct native_access;
}
class u32x8 {
    __m256i bits_;
    explicit u32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    using value_type = std::uint32_t;
    using mask_type = mask32x8;
    static constexpr int size() noexcept {
        return 8;
    }
    static constexpr int alignment = 32;
    // Default construction, like an intrinsic register, leaves lanes uninitialized.
    u32x8() = default;
    explicit u32x8(value_type value) noexcept
        : bits_(_mm256_set1_epi32(std::bit_cast<int>(value))) {}
};
// A broadcast invariant encoded in the type, not re-checked at runtime.
// Fixed-factor arithmetic can exploit identical lanes without extra shuffles.
class broadcast_u32x8 {
    u32x8 value_;

  public:
    broadcast_u32x8() = default;
    explicit broadcast_u32x8(std::uint32_t value) noexcept : value_(value) {}
    [[nodiscard]] u32x8 vector() const noexcept {
        return value_;
    }
};
struct mask32x8 {
  private:
    __m256i bits_;
    explicit mask32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    // Each lane is either all-zero or all-one, never an arbitrary byte mask.
    mask32x8() = default;
};
namespace detail {
struct native_access {
    static __m256i get(u32x8 v) noexcept {
        return v.bits_;
    }
    static __m256i get(mask32x8 v) noexcept {
        return v.bits_;
    }
    static u32x8 words(__m256i v) noexcept {
        return u32x8{v};
    }
    static mask32x8 mask(__m256i v) noexcept {
        return mask32x8{v};
    }
};
} // namespace detail
static_assert(sizeof(u32x8) == 32 && alignof(u32x8) == 32);
static_assert(sizeof(mask32x8) == 32);
static_assert(std::is_trivially_copyable_v<u32x8>);

// Full-width operations. Alignment and eight readable/writable words are
// preconditions. select() does NOT make an earlier memory access conditional.
[[nodiscard]] inline u32x8 load_aligned(const std::uint32_t* address) noexcept {
    return detail::native_access::words(
        _mm256_load_si256(reinterpret_cast<const __m256i*>(address)));
}
inline void store_aligned(std::uint32_t* address, u32x8 value) noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(address), detail::native_access::get(value));
}
[[nodiscard]] inline u32x8 select(mask32x8 mask, u32x8 when_true, u32x8 when_false) noexcept {
    using access = detail::native_access;
    return access::words(
        _mm256_blendv_epi8(access::get(when_false), access::get(when_true), access::get(mask)));
}
// Lane k is selected iff k >= first. Negative first selects every lane;
// first >= 8 selects none. Precondition: first > INT_MIN.
[[nodiscard]] inline mask32x8 lanes_from(int first) noexcept {
    return detail::native_access::mask(_mm256_cmpgt_epi32(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7),
                                                          _mm256_set1_epi32(first - 1)));
}
[[nodiscard]] inline u32x8 operator+(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_add_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 operator-(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_sub_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 min(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_min_epu32(access::get(a), access::get(b)));
}
// Explicitly distinguish multiplication modulo 2^32 from the high half of
// the widening product. Neither of these functions is modular-field multiply.
[[nodiscard]] inline u32x8 multiply_low(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_mullo_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b);
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), _mm256_srli_epi64(bv, 32));
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
// Same high-half multiplication with the second operand known to be a
// broadcast. AVX2's odd-lane stream can reuse it without a right shift.
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, broadcast_u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b.vector());
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), bv);
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
} // namespace canard::simd
namespace canard::numeric {
template <std::uint32_t Modulus> struct montgomery32_avx2 {
    using scalar = montgomery32<Modulus>;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using pack_type = simd::u32x8;
    static constexpr auto modulus = Modulus;
    static constexpr auto negative_inverse = scalar::negative_inverse;
    // A radix-2^32 reduction needs the complete 64-bit product for each lane.
    // AVX2 multiplies alternate 32-bit lanes, so use separate even/odd streams.
    [[nodiscard]] static inline simd::u32x8 multiply(simd::u32x8 left, simd::u32x8 right) noexcept {
        using access = simd::detail::native_access;
        const auto a = access::get(left), b = access::get(right);
        const auto even = _mm256_mul_epu32(a, b);
        const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(negative_inverse));
        const auto prime = _mm256_set1_epi32(static_cast<int>(modulus));
        const auto even_correction = _mm256_mul_epu32(even, inverse);
        const auto odd_correction = _mm256_mul_epu32(odd, inverse);
        const auto even_sum = _mm256_add_epi64(even, _mm256_mul_epu32(even_correction, prime));
        const auto odd_sum = _mm256_add_epi64(odd, _mm256_mul_epu32(odd_correction, prime));
        return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even_sum, 32), odd_sum, 0xaa));
    }

    // The factor is supplied encoded; internally Shoup-style reciprocal
    // reduction uses its canonical ordinary value. Input and output vectors
    // retain their representation (encoded or ordinary). The reciprocal is
    // prepared ONCE for all uses of this multiplier.
    class blended_multiplier;
    class fixed_multiplier {
        friend class blended_multiplier;
        simd::u32x8 factor_;
        simd::broadcast_u32x8 quotient_;

      public:
        explicit fixed_multiplier(u32 encoded) noexcept {
            const u32 value = scalar::decode(encoded);
            factor_ = simd::u32x8{value};
            quotient_ = simd::broadcast_u32x8{static_cast<u32>((u64{value} << 32) / modulus)};
        }
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            const auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    // Each lane chooses one of two already-prepared factors. Multiplication
    // retains its input representation and returns a lazy residue below 2p.
    // No divisions or reciprocal preparations occur inside the vector loop.
    class blended_multiplier {
        simd::u32x8 factor_, quotient_;

      public:
        blended_multiplier(simd::mask32x8 selected,
                           const fixed_multiplier& yes,
                           const fixed_multiplier& no) noexcept
            : factor_(simd::select(selected, yes.factor_, no.factor_)),
              quotient_(simd::select(selected, yes.quotient_.vector(), no.quotient_.vector())) {}
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    [[nodiscard]] static simd::u32x8 add(simd::u32x8 a, simd::u32x8 b) noexcept {
        const auto sum = a + b;
        return simd::min(sum, sum - simd::u32x8{scalar::twice_modulus});
    }
};
} // namespace canard::numeric
// ===== include/canard/simd/orthogonal_avx2.hpp =====
#include <bit>

namespace canard::simd {
// Every lane is all-zero or all-one. This does not mask memory accesses.
[[nodiscard]] inline mask32x8 lanes_from_bits(unsigned bits) noexcept {
    const auto selected = _mm256_set1_epi32(static_cast<int>(bits & 255));
    const auto positions = _mm256_setr_epi32(1,2,4,8,16,32,64,128);
    return detail::native_access::mask(_mm256_cmpeq_epi32(_mm256_and_si256(selected, positions), positions));
}
// Full uint32 coordinate domain; signed compares use biased operands.
struct rectangle_u32x8 {
    __m256i left, down, right, up;
    explicit rectangle_u32x8(geometry::rectangle2<std::uint32_t> r) noexcept {
        const auto broadcast = [](std::uint32_t x) {
            return _mm256_set1_epi32(std::bit_cast<int>(x ^ 0x80000000u));
        };
        left = broadcast(r.left); down = broadcast(r.down);
        right = broadcast(r.right); up = broadcast(r.up);
    }
    [[nodiscard]] unsigned select(const std::uint32_t* xs, const std::uint32_t* ys) const noexcept {
        const auto bias = _mm256_set1_epi32(std::bit_cast<int>(0x80000000u));
        const auto x = _mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const __m256i*>(xs)), bias);
        const auto y = _mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const __m256i*>(ys)), bias);
        const auto below = _mm256_or_si256(_mm256_cmpgt_epi32(left, x), _mm256_cmpgt_epi32(down, y));
        const auto upper = _mm256_and_si256(_mm256_cmpgt_epi32(right, x), _mm256_cmpgt_epi32(up, y));
        return static_cast<unsigned>(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_andnot_si256(below, upper))));
    }
};
} // namespace canard::simd
// ===== include/canard/simd/reduction_avx2.hpp =====
#include <cstdint>

namespace canard::simd {

// Lane k is selected exactly when first <= k < last. Negative boundaries and
// boundaries beyond lane 7 are permitted. first must be greater than INT_MIN.
[[nodiscard]] inline mask32x8 lanes_between(int first, int last) noexcept {
    using access = detail::native_access;
    return access::mask(
        _mm256_andnot_si256(access::get(lanes_from(last)), access::get(lanes_from(first))));
}

[[nodiscard]] inline u32x8 operator&(u32x8 left, u32x8 right) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_and_si256(access::get(left), access::get(right)));
}

// Exact sum of eight unsigned 32-bit lanes, without 32-bit overflow.
[[nodiscard]] inline std::uint64_t reduce_add_widened(u32x8 values) noexcept {
    const auto raw = detail::native_access::get(values);
    const auto pairs = _mm256_add_epi64(_mm256_and_si256(raw, _mm256_set1_epi64x(0xffff'ffffu)),
                                        _mm256_srli_epi64(raw, 32));
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(pairs), _mm256_extracti128_si256(pairs, 1));
    const auto total = _mm_add_epi64(halves, _mm_srli_si128(halves, 8));
    return static_cast<std::uint64_t>(_mm_cvtsi128_si64(total));
}

// Sum an arbitrary contiguous range without reading beyond its end.
// The mathematical total must fit in uint64_t.
[[nodiscard]] inline std::uint64_t sum_widened_u32(const std::uint32_t* first,
                                                   const std::uint32_t* last) noexcept {
    auto even = _mm256_setzero_si256(), odd = _mm256_setzero_si256();
    const auto low = _mm256_set1_epi64x(0xffff'ffffu);
    for (; last - first >= 8; first += 8) {
        const auto values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first));
        even = _mm256_add_epi64(even, _mm256_and_si256(values, low));
        odd = _mm256_add_epi64(odd, _mm256_srli_epi64(values, 32));
    }
    const auto total = _mm256_add_epi64(even, odd);
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(total), _mm256_extracti128_si256(total, 1));
    std::uint64_t answer = static_cast<std::uint64_t>(
        _mm_cvtsi128_si64(_mm_add_epi64(halves, _mm_srli_si128(halves, 8))));
    for (; first != last; ++first)
        answer += *first;
    return answer;
}

} // namespace canard::simd

namespace canard::kernel {
template <std::uint32_t P, unsigned Capacity>
struct spatial_affine_sum_avx2 : spatial_affine_sum<std::uint32_t, P, Capacity> {
    using base = spatial_affine_sum<std::uint32_t, P, Capacity>;
    using field = field_t<base>;
    using vectors = numeric::montgomery32_avx2<P>;
    using leaf_type = leaf_type_t<base>;
    using summary_type = summary_type_t<base>;
    using action_type = action_type_t<base>;
    using update_type = update_type_t<base>;
    using pack = simd::u32x8;
    struct prepared_action {
        action_type action;
        fixed_multiplier_t<vectors> scale;
        explicit prepared_action(action_type f) noexcept : action(f), scale(f.multiplier) {}
    };
    using base::base;
    [[nodiscard]] static prepared_action prepare(update_type f) noexcept { return prepared_action{base::prepare(f)}; }
    [[nodiscard]] static action_type action_of(const prepared_action& f) noexcept { return f.action; }
    template <typename Geometry>
    [[nodiscard]] static std::uint64_t select(const Geometry& g,
        geometry::rectangle2<std::uint32_t> r, std::uint64_t active) noexcept {
        const simd::rectangle_u32x8 rectangle{r};
        std::uint64_t result = 0;
        for (unsigned i = 0; i < Capacity; i += 8)
            result |= std::uint64_t{rectangle.select(g.xs.data() + i, g.ys.data() + i)} << i;
        return result & active;
    }
    [[nodiscard]] static summary_type summarize(const leaf_type& leaf, std::uint64_t bits) noexcept {
        if (std::popcount(bits) <= 2) return base::summarize(leaf, bits);
        pack total{0};
        for (unsigned i = 0; i < Capacity; i += 8) {
            const auto selected = static_cast<unsigned>((bits >> i) & 255);
            if (selected) total = vectors::add(total, simd::select(simd::lanes_from_bits(selected),
                simd::load_aligned(leaf.values.data() + i), pack{0}));
        }
        return static_cast<summary_type>(simd::reduce_add_widened(total) % P);
    }
    static void materialize(leaf_type& leaf, std::uint64_t active, action_type f) noexcept {
        const fixed_multiplier_t<vectors> scale{f.multiplier};
        const pack shift{field::decode(f.translation)};
        for (unsigned i = 0; i < Capacity; i += 8) {
            const unsigned bits = static_cast<unsigned>((active >> i) & 255);
            if (!bits) continue;
            const auto previous = simd::load_aligned(leaf.values.data() + i);
            const auto next = vectors::add(scale(previous), shift);
            simd::store_aligned(leaf.values.data() + i, simd::select(simd::lanes_from_bits(bits), next, previous));
        }
    }
    [[nodiscard]] static summary_type edit(leaf_type& leaf, std::uint64_t active, std::uint64_t selected,
        const prepared_action& f, action_type incoming, bool dirty, summary_type old) noexcept {
        const auto count = std::popcount(selected);
        auto before = summarize(leaf, selected);
        if (dirty) {
            before = base::map(incoming, before, count);
            const auto after = base::compose(f.action, incoming);
            const fixed_multiplier_t<vectors> g{incoming.multiplier}, h{after.multiplier};
            const pack old_shift{field::decode(incoming.translation)}, new_shift{field::decode(after.translation)};
            for (unsigned i = 0; i < Capacity; i += 8) {
                const unsigned live = static_cast<unsigned>((active >> i) & 255);
                if (!live) continue;
                const auto mask = simd::lanes_from_bits(static_cast<unsigned>(selected >> i));
                const blended_multiplier_t<vectors> scale{mask, h, g};
                const auto previous = simd::load_aligned(leaf.values.data() + i);
                const auto next = vectors::add(scale(previous), simd::select(mask, new_shift, old_shift));
                simd::store_aligned(leaf.values.data() + i, simd::select(simd::lanes_from_bits(live), next, previous));
            }
        } else {
            const pack shift{field::decode(f.action.translation)};
            for (unsigned i = 0; i < Capacity; i += 8) {
                const unsigned bits = static_cast<unsigned>((selected >> i) & 255);
                if (!bits) continue;
                const auto previous = simd::load_aligned(leaf.values.data() + i);
                const auto next = vectors::add(f.scale(previous), shift);
                simd::store_aligned(leaf.values.data() + i, simd::select(simd::lanes_from_bits(bits), next, previous));
            }
        }
        return field::add(old, field::subtract(base::map(f.action, before, count), before));
    }
};
} // namespace canard::kernel
namespace canard::spatial {
template <std::uint32_t P, unsigned B>
    requires(P > 1 && P < (1u << 30) && P % 2 == 1)
struct kernel_binding<std::uint32_t, algebra::modular_sum<P>, algebra::affine_on_sum<P>,
    representation::modular_affine, execution::avx2, B> {
    using type = kernel::spatial_affine_sum_avx2<P, B>;
};
}
// ===== include/canard/io/avx2/trusted_ascii_reader.hpp =====
// ===== include/canard/io/decimal_avx2.hpp =====
// ===== include/canard/config/compiler.hpp =====
#if defined(__GNUC__) || defined(__clang__)
# define CANARD_ALWAYS_INLINE [[gnu::always_inline]]
# define CANARD_NOINLINE [[gnu::noinline]]
#elif defined(_MSC_VER)
# define CANARD_ALWAYS_INLINE __forceinline
# define CANARD_NOINLINE __declspec(noinline)
#else
# define CANARD_ALWAYS_INLINE
# define CANARD_NOINLINE
#endif
namespace canard::detail {
template <int ReadWrite = 0, int Locality = 3>
inline void prefetch(const void* address) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(address, ReadWrite, Locality);
#else
    (void)address;
#endif
}
}
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#ifndef __AVX2__
#error "The packed decimal backend requires AVX2."
#endif
namespace canard::io::decimal {
static_assert(std::endian::native == std::endian::little);

// Borrowing field in padded ASCII storage. digits is 1..10, and the eight
// bytes preceding data+digits must be readable, even for a short field.


namespace detail {
inline constexpr int pair_weights = 0x010a;      // [10, 1]
inline constexpr int quad_weights = 0x00010064;  // [100, 1]
inline constexpr int octet_weights = 0x00012710; // [10000, 1]
inline constexpr auto nibble_mask = [] consteval {
    std::array<std::uint64_t, 11> masks{};
    for (unsigned digits = 1; digits <= 10; ++digits)
        masks[digits] = 0x0f0f'0f0f'0f0f'0f0full << (digits < 8 ? (8 - digits) * 8 : 0);
    return masks;
}();
inline constexpr std::array<unsigned, 10> ninth_digit_weight{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 100'000'000};
[[nodiscard]] inline std::uint64_t eight_byte_tail(field input) noexcept {
    std::uint64_t bytes;
    std::memcpy(&bytes, input.data + input.digits - 8, 8);
    return bytes;
}
[[nodiscard]] inline std::uint32_t ninth_digit(field input) noexcept {
    return static_cast<unsigned>(input.data[0] - '0') * ninth_digit_weight[input.digits];
}
template <unsigned MaxDigits>
[[nodiscard]] inline std::uint32_t leading_digits(field input) noexcept {
    if constexpr (MaxDigits <= 8)
        return 0;
    else if constexpr (MaxDigits == 9)
        return ninth_digit(input);
    else {
        if (input.digits <= 8)
            return 0;
        unsigned leading = static_cast<unsigned>(input.data[0] - '0');
        if (input.digits == 10)
            leading = leading * 10 + static_cast<unsigned>(input.data[1] - '0');
        return leading * 100'000'000;
    }
}
} // namespace detail

[[nodiscard]] inline std::uint32_t decode_one(field input) noexcept {
    auto digits = detail::eight_byte_tail(input) & detail::nibble_mask[input.digits];
    digits = digits * 10 + (digits >> 8);
    const auto eight = static_cast<std::uint32_t>(
        ((digits & 0x0000'00ff'0000'00ffull) * 0x000f'4240'0000'0064ull +
         ((digits >> 16) & 0x0000'00ff'0000'00ffull) * 0x0000'2710'0000'0001ull) >>
        32);
    return eight + detail::leading_digits<10>(input);
}

// ASCII -> digit pairs -> four-digit groups -> eight-digit groups.
// Four-digit groups are <=9999, so packing does not saturate and the final
// signed 16-bit multiply-add is exact. A ninth digit is added separately.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2>
decode_pair(field first, field second) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm_set_epi64x(std::bit_cast<long long>(eight_byte_tail(second)),
                                 std::bit_cast<long long>(eight_byte_tail(first)));
    digits = _mm_and_si128(digits,
                           _mm_set_epi64x(nibble_mask[second.digits], nibble_mask[first.digits]));
    const auto pairs = _mm_maddubs_epi16(digits, _mm_set1_epi16(pair_weights));
    const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(quad_weights));
    const auto compact = _mm_packus_epi32(quads, quads);
    const auto eights = _mm_madd_epi16(compact, _mm_set1_epi32(octet_weights));
    const auto leading_first = leading_digits<FirstMaxDigits>(first);
    const auto leading_second = leading_digits<SecondMaxDigits>(second);
    return {static_cast<std::uint32_t>(_mm_cvtsi128_si32(eights)) + leading_first,
            static_cast<std::uint32_t>(_mm_extract_epi32(eights, 1)) + leading_second};
}

template <unsigned FirstMaxDigits, unsigned SecondMaxDigits, unsigned ThirdMaxDigits>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept;

// FirstMaxDigits is a precondition, not a run-time validator. A known-small
// first field (e.g. an index) skips its ninth-digit calculation at compile time.
template <unsigned FirstMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_three(field first, field second, field third) noexcept {
    return decode_triplet<FirstMaxDigits, 9, 9>(first, second, third);
}

// The next 32 bytes must be readable and contain all requested delimiters.
// Only ASCII decimal input is supported, so signed-byte comparison suffices.
[[nodiscard]] inline std::uint32_t delimiter_mask(const char* data) noexcept {
    return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
        _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data)))));
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// Three bounded unsigned fields; total ASCII record (with delimiters) <=32
// bytes. Unlike decode_three's historical interface, every bound is explicit.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9, unsigned ThirdMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    static_assert(1 <= ThirdMaxDigits && ThirdMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(first)),
                                     std::bit_cast<long long>(eight_byte_tail(second)),
                                     std::bit_cast<long long>(eight_byte_tail(third)),
                                     0);
    digits = _mm256_and_si256(
        digits,
        _mm256_setr_epi64x(
            nibble_mask[first.digits], nibble_mask[second.digits], nibble_mask[third.digits], 0));
    const auto pairs = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(pair_weights));
    const auto quads = _mm256_madd_epi16(pairs, _mm256_set1_epi32(quad_weights));
    const auto compact = _mm256_packus_epi32(quads, quads);
    const auto eights = _mm256_madd_epi16(compact, _mm256_set1_epi32(octet_weights));
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 0)) +
                leading_digits<FirstMaxDigits>(first),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 1)) +
                leading_digits<SecondMaxDigits>(second),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 4)) +
                leading_digits<ThirdMaxDigits>(third)};
}

// Four fields of 1..7 decimal digits. Every eight-byte tail must be readable.
// The length/decimal alphabet are trusted preconditions, not runtime checks.
[[nodiscard]] inline std::array<std::uint32_t, 4>
decode_four_short(field a, field b, field c, field d) noexcept {
    using namespace detail;
    auto x = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(a)),
                                std::bit_cast<long long>(eight_byte_tail(b)),
                                std::bit_cast<long long>(eight_byte_tail(c)),
                                std::bit_cast<long long>(eight_byte_tail(d)));
    x = _mm256_and_si256(x,
                         _mm256_setr_epi64x(nibble_mask[a.digits],
                                            nibble_mask[b.digits],
                                            nibble_mask[c.digits],
                                            nibble_mask[d.digits]));
    x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(pair_weights));
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(quad_weights));
    x = _mm256_packus_epi32(x, x);
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(octet_weights));
    // AVX2 packing is local to each 128-bit half: fields occupy lanes 0,1,4,5.
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(x, 0)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 1)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 4)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 5))};
}
} // namespace canard::io::decimal
// ===== include/canard/io/detail/basic_trusted_ascii_reader.hpp =====
// ===== include/canard/io/padded_bytes_view.hpp =====
#include <cstddef>
namespace canard::io {
// Borrowed trusted storage. data[-8..size+31] must be readable. Padding is not
// automatically promised by string_view/span. The owner outlives every reader.
struct padded_bytes_view {
    const char* data;
    std::size_t size;
    static constexpr std::size_t prefix_padding = 8;
    static constexpr std::size_t suffix_padding = 32;
};
}
#include <array>
#include <bit>
#include <cstring>
#include <cstdint>
namespace canard::io {
// Borrowing reader over padded bytes; lifetime is controlled by the caller.
// Packed records require exactly one delimiter between fields, <=32 bytes
// including the terminating delimiter, and the stated maximum digit counts.
// read_tagged_triplet(): one-digit tag, then three fields. read_pair(): two
// 1..9-digit fields. read_u32() also accepts repeated leading whitespace.
template <typename Codec>
class basic_trusted_ascii_reader {
    const char* cursor_;
    void skip_whitespace() {
        while (static_cast<unsigned char>(*cursor_) <= ' ') {
            ++cursor_;
        }
    }

    template <typename Word> [[nodiscard]] static constexpr bool all_digits(Word word) noexcept {
        constexpr Word zeros =
            sizeof(Word) == 8 ? static_cast<Word>(0x3030'3030'3030'3030ull) : Word{0x3030'3030u};
        constexpr Word high =
            sizeof(Word) == 8 ? static_cast<Word>(0xf0f0'f0f0'f0f0'f0f0ull) : Word{0xf0f0'f0f0u};
        constexpr Word sixes =
            sizeof(Word) == 8 ? static_cast<Word>(0x0606'0606'0606'0606ull) : Word{0x0606'0606u};
        return (word & high) == zeros && ((word + sixes) & high) == zeros;
    }

    template <typename Word> [[nodiscard]] std::uint32_t read_unsigned() {
        if constexpr (std::endian::native != std::endian::little) {
            skip_whitespace();
            std::uint32_t value = 0;
            while (*cursor_ >= '0' && *cursor_ <= '9')
                value = value * 10 + (*cursor_++ - '0');
            return value;
        }
        skip_whitespace();
        Word word;
        std::memcpy(&word, cursor_, sizeof(word));
        std::uint32_t result = 0;
        if (all_digits(word)) {
            if constexpr (sizeof(Word) == 8) {
                word ^= 0x3030'3030'3030'3030ull;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ff'00ff'00ffull;
                word = (word * 100 + (word >> 16)) & 0x0000'ffff'0000'ffffull;
                word = (word * 10'000 + (word >> 32)) & 0x0000'0000'ffff'ffffull;
            } else {
                word ^= 0x3030'3030u;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ffu;
                word = (word * 100 + (word >> 16)) & 0x0000'ffffu;
            }
            result = static_cast<std::uint32_t>(word);
            cursor_ += sizeof(word);
        }
        // Valid judge input contains only values fitting in uint32_t.
        for (unsigned digit; (digit = static_cast<unsigned>(*cursor_ - '0')) < 10; ++cursor_) {
            result = result * 10 + digit;
        }
        return result;
    }

  public:
    // Two pairs of 1..MaxDigits unsigned fields, each followed by one delimiter.
    // The complete four-field record fits in 32 readable bytes; MaxDigits <= 7.
    template <unsigned MaxDigits = 6>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4>
    read_two_index_pairs() noexcept {
        static_assert(MaxDigits >= 1 && MaxDigits <= 7);
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned p0 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p1 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p2 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p3 = std::countr_zero(mask);
        const auto result = Codec::decode_four_short({cursor_, p0},
                                                       {cursor_ + p0 + 1, p1 - p0 - 1},
                                                       {cursor_ + p1 + 1, p2 - p1 - 1},
                                                       {cursor_ + p2 + 1, p3 - p2 - 1});
        cursor_ += p3 + 1;
        return result;
    }

    explicit basic_trusted_ascii_reader(padded_bytes_view input) noexcept : cursor_(input.data) {}
    // One codec delimiter mask per record; decimal reductions use packed
    // byte-to-pair, pair-to-quad, and quad-to-eight-digit multiply-adds.
    // A ninth leading digit is added separately. Field lengths are trusted.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2> read_pair() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second;
        ++cursor_;
        return {a, b};
    }
    template <unsigned FirstMaxDigits = 9,
              unsigned SecondMaxDigits = 9,
              unsigned ThirdMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3> read_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        const auto result =
            Codec::template decode_triplet<FirstMaxDigits, SecondMaxDigits, ThirdMaxDigits>(
                {cursor_, first},
                {cursor_ + first + 1, second - first - 1},
                {cursor_ + second + 1, third - second - 1});
        cursor_ += third + 1;
        return result;
    }

    // Exactly one one-digit tag and two bounded decimal fields.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
    read_tagged_pair() noexcept {
        skip_whitespace();
        const unsigned type = static_cast<unsigned>(cursor_[0] - '0');
        cursor_ += 2;
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second + 1;
        return {type, a, b};
    }

    template <unsigned FirstMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4> read_tagged_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned fourth = std::countr_zero(mask);
        const auto type = static_cast<unsigned>(cursor_[0] - '0');
        const auto [position, a, b] =
            Codec::template decode_three<FirstMaxDigits>({cursor_ + 2, second - 2},
                                                  {cursor_ + second + 1, third - second - 1},
                                                  {cursor_ + third + 1, fourth - third - 1});
        cursor_ += fourth;
        ++cursor_;
        return {type, position, a, b};
    }

    // Scalar/SWAR parser for general uint32 values, including ten digits.
    // MaxDigits selects a 4- or 8-byte fast prefix, not a run-time validator.
    template <unsigned MaxDigits = 10> [[nodiscard]] std::uint32_t read_u32() {
        static_assert(1 <= MaxDigits && MaxDigits <= 10);
        if constexpr (MaxDigits <= 6)
            return read_unsigned<std::uint32_t>();
        else
            return read_unsigned<std::uint64_t>();
    }
    // Signed input, including ten-digit magnitudes. Successful int32 parsing
    // is a precondition; unsigned subtraction also handles INT32_MIN exactly.
    [[nodiscard]] std::int32_t read_i32() {
        skip_whitespace();
        const bool negative = *cursor_ == '-';
        cursor_ += negative;
        const auto magnitude = read_unsigned<std::uint64_t>();
        return std::bit_cast<std::int32_t>(negative ? 0u - magnitude : magnitude);
    }
    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
};
} // namespace canard::io
namespace canard::io::decimal {
struct ascii_avx2_codec {
    CANARD_ALWAYS_INLINE static std::uint32_t delimiter_mask(const char* p) noexcept {
        return decimal::delimiter_mask(p);
    }
    template <unsigned A = 9, unsigned B = 9>
    CANARD_ALWAYS_INLINE static auto decode_pair(field a, field b) noexcept {
        return decimal::decode_pair<A, B>(a, b);
    }
    template <unsigned A = 9, unsigned B = 9, unsigned C = 9>
    CANARD_ALWAYS_INLINE static auto decode_triplet(field a, field b, field c) noexcept {
        return decimal::decode_triplet<A, B, C>(a, b, c);
    }
    template <unsigned A = 9>
    CANARD_ALWAYS_INLINE static auto decode_three(field a, field b, field c) noexcept {
        return decimal::decode_three<A>(a, b, c);
    }
    CANARD_ALWAYS_INLINE static auto decode_four_short(field a, field b, field c, field d) noexcept {
        return decimal::decode_four_short(a, b, c, d);
    }
};
}
namespace canard::io {
using avx2_ascii_reader = basic_trusted_ascii_reader<decimal::ascii_avx2_codec>;
}
// ===== include/canard/io/padded_file.hpp =====
// Compatibility include: trusted Linux whole-file mapping, no pipe fallback.
// ===== include/canard/io/source/linux_mapped_file.hpp =====
#if !defined(__linux__)
#error "canard Linux mapping requires Linux; use buffered_file_source on other platforms."
#endif
#include <cstddef>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
namespace canard::io {
// Trusted Linux regular file, positioned at its start; no pipe fallback or
// error checking. The descriptor remains caller-owned. Linux 4 KiB pages.
class padded_file {
    void* mapping_ = MAP_FAILED;
    std::size_t mapping_size_ = 0;
    std::size_t bytes_ = 0;
    const char* data_ = nullptr;

  public:
    explicit padded_file(int descriptor = STDIN_FILENO) noexcept {
        struct stat status;
        ::fstat(descriptor, &status);
        bytes_ = static_cast<std::size_t>(status.st_size);
        constexpr std::size_t page = 4096;
        mapping_size_ = ((bytes_ + page - 1) & ~(page - 1)) + 2 * page;
        mapping_ = ::mmap(nullptr, mapping_size_, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        ::mmap(static_cast<char*>(mapping_) + page,
               bytes_,
               PROT_READ,
               MAP_PRIVATE | MAP_FIXED,
               descriptor,
               0);
        data_ = static_cast<const char*>(mapping_) + page;
    }
    padded_file(const padded_file&) = delete;
    padded_file& operator=(const padded_file&) = delete;
    ~padded_file() {
        ::munmap(mapping_, mapping_size_);
    }
    [[nodiscard]] padded_bytes_view view() const & noexcept {
        return {data_, bytes_};
    }
    void view() const && = delete;
};
} // namespace canard::io
// ===== include/canard/io/sink/posix.hpp =====
#include <cerrno>
#include <unistd.h>
namespace canard::io {
// Expert sink retaining the old successful, complete-write precondition.
class trusted_posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = true;
    explicit trusted_posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    void write_all(const char* data, std::size_t size) const noexcept {
        ::write(descriptor_, data, size);
    }
};
class posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    transfer_result write_some(const char* data, std::size_t size) const noexcept {
        ssize_t count;
        do { count = ::write(descriptor_, data, size); } while (count < 0 && errno == EINTR);
        if (count < 0) return {0, {errno, std::generic_category()}};
        return {static_cast<std::size_t>(count), {}};
    }
};
}
using client_execution = canard::execution::avx2;
using client_sink = canard::io::trusted_posix_sink;
#include <vector>
#ifndef CANARD_SPATIAL_CAPACITY
#define CANARD_SPATIAL_CAPACITY 64
#endif
#ifdef CANARD_SPATIAL_SCALAR
using tree_execution = canard::execution::scalar;
#else
using tree_execution = client_execution;
#endif
#ifdef CANARD_SPATIAL_ALTERNATING
using client_partition = canard::structural::point_partition::alternating;
#else
using client_partition = canard::structural::point_partition::cardinality_coalesced;
#endif
namespace {
constexpr std::uint32_t modulus = 998244353;
using point = canard::geometry::point2<std::uint32_t>;
using profile = canard::spatial::configuration<CANARD_SPATIAL_CAPACITY,
    canard::spatial::representation::modular_affine, tree_execution,
    client_partition>;
struct operation { std::uint32_t type, a, b, c, d, e, f; };
}
int main() {
    canard::io::padded_file file;
    canard::io::avx2_ascii_reader input{file.view()};
    canard::io::buffered_writer<modulus - 1, client_sink> output;
    const auto [n, q] = input.read_pair();
    std::vector<point> points;
    points.reserve(n + q);
    std::vector<std::uint32_t> initial(n);
    for (unsigned i = 0; i < n; ++i) {
        const auto x = input.read_u32<10>(), y = input.read_u32<10>();
        initial[i] = input.read_u32<9>();
        points.push_back({x, y});
    }
    std::vector<operation> operations(q);
    for (auto& op : operations) {
        op.type = input.read_u32<1>();
        if (op.type == 0) {
            const auto x = input.read_u32<10>(), y = input.read_u32<10>();
            op.a = static_cast<std::uint32_t>(points.size());
            op.b = input.read_u32<9>();
            points.push_back({x, y});
        } else if (op.type == 1) {
            op.a = input.read_u32<6>(); op.b = input.read_u32<9>();
        } else {
            op.a = input.read_u32<10>(); op.b = input.read_u32<10>();
            op.c = input.read_u32<10>(); op.d = input.read_u32<10>();
            if (op.type == 3) { op.e = input.read_u32<9>(); op.f = input.read_u32<9>(); }
        }
    }
    canard::registered_point_tree tree{std::span<const point>{points}, std::span<const std::uint32_t>{initial},
        canard::algebra::modular_sum<modulus>{}, canard::algebra::affine_on_sum<modulus>{}, profile{}};
    // Coordinates only are preregistered; weights/actions are processed in
    // their original order and every fold returns an immediate result.
    for (const auto& op : operations) {
        if (op.type == 0) tree.activate(op.a, op.b);
        else if (op.type == 1) tree.set(op.a, op.b);
        else if (op.type == 2) output.write(tree.fold({op.a, op.b, op.c, op.d}));
        else tree.apply({op.a, op.b, op.c, op.d}, {op.e, op.f});
    }
}
