#pragma once

#include <bits/extc++.h>

namespace toy {

template <uint32_t Mod>
struct Affine {
    uint32_t a = 1;
    uint32_t b = 0;

    uint32_t operator()(uint32_t x) const { return ((uint64_t)a * x + b) % Mod; }
};

template <uint32_t Mod>
struct ComposeAffine {
    Affine<Mod> operator()(Affine<Mod> first, Affine<Mod> second) const {
        return {(uint32_t)((uint64_t)second.a * first.a % Mod),
                (uint32_t)(((uint64_t)second.a * first.b + second.b) % Mod)};
    }
};

template <uint32_t Mod>
class AffineSegmentTree {
    using Function = Affine<Mod>;
    int count;
    int size;
    std::vector<Function> tree;

    static Function compose(Function first, Function second) {
        return ComposeAffine<Mod>{}(first, second);
    }

  public:
    explicit AffineSegmentTree(int length)
        : count(length), size(std::bit_ceil((unsigned)std::max(length, 1))), tree(2 * size) {}

    explicit AffineSegmentTree(const std::vector<Function> &values)
        : count(values.size()),
          size(std::bit_ceil((unsigned)std::max<std::size_t>(values.size(), 1))), tree(2 * size) {
        std::copy(values.begin(), values.end(), tree.begin() + size);
        for (int node = size - 1; node; --node)
            tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
    }

    void set(int index, Function function) {
        int node = size + index;
        tree[node] = function;
        while (node >>= 1) tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
    }

    Function fold(int left, int right) const {
        Function before, after;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) before = compose(before, tree[left++]);
            if (right & 1) after = compose(tree[--right], after);
        }
        return compose(before, after);
    }

    uint32_t apply(int left, int right, uint32_t value) const {
        assert(0 <= left && left < right && right <= count);
        uint32_t l = left + size - 1;
        uint32_t r = right + size;
        unsigned width = std::bit_width(l ^ r) - 1;
        uint32_t mask = (uint32_t(1) << width) - 1;

        uint32_t cursor = l;
        for (l = ~l & mask; l;) {
            unsigned shift = std::countr_zero(l);
            l ^= uint32_t(1) << shift;
            value = tree[(cursor >> shift) ^ 1](value);
        }
        cursor = r;
        for (r &= mask; r;) {
            unsigned shift = std::bit_width(r) - 1;
            r ^= uint32_t(1) << shift;
            value = tree[(cursor >> shift) ^ 1](value);
        }
        return value;
    }
};

template <uint32_t Mod>
class AffineEvaluationSegmentTree {
    using Function = Affine<Mod>;
    int size;
    std::vector<Function> tree;

    static Function reverse_compose(Function left, Function right) {
        return ComposeAffine<Mod>{}(right, left);
    }

  public:
    explicit AffineEvaluationSegmentTree(const std::vector<Function> &values)
        : size(values.size()), tree(2 * size) {
        for (int i = 0; i < size; ++i) tree[2 * size - 1 - i] = values[i];
        for (int node = size - 1; node; --node)
            tree[node] = reverse_compose(tree[node * 2], tree[node * 2 + 1]);
    }

    void set(int index, Function function) {
        int node = 2 * size - 1 - index;
        tree[node] = function;
        while (node >>= 1) tree[node] = reverse_compose(tree[node * 2], tree[node * 2 + 1]);
    }

    uint32_t apply(int left, int right, uint32_t value) const {
        int l = 2 * size - 1 - right;
        int r = 2 * size - left;
        unsigned width = std::bit_width((unsigned)(l ^ r)) - 1;
        int boundary = r >> width;
        for (r = (r >> std::countr_zero((unsigned)r)) ^ 1; r > boundary;
             r = (r >> std::countr_zero((unsigned)r)) ^ 1) {
            value = tree[r](value);
        }
        unsigned remaining = ~unsigned(l) & ((unsigned(1) << width) - 1);
        while (remaining) {
            unsigned shift = std::bit_width(remaining) - 1;
            remaining -= unsigned(1) << shift;
            value = tree[(l >> shift) ^ 1](value);
        }
        return value;
    }
};

template <class T, class Operation>
class FoldableQueue {
    struct Node {
        T value;
        T aggregate;
    };

    T identity;
    Operation operation;
    std::vector<Node> front;
    std::vector<Node> back;

    void transfer() {
        while (!back.empty()) {
            T value = back.back().value;
            back.pop_back();
            T aggregate = front.empty() ? value : operation(value, front.back().aggregate);
            front.push_back({value, aggregate});
        }
    }

  public:
    explicit FoldableQueue(T identity_value, Operation combine = {})
        : identity(identity_value), operation(combine) {}

    void push(T value) {
        T aggregate = back.empty() ? value : operation(back.back().aggregate, value);
        back.push_back({value, aggregate});
    }

    void pop() {
        if (front.empty()) transfer();
        front.pop_back();
    }

    T fold() const {
        if (front.empty()) return back.empty() ? identity : back.back().aggregate;
        if (back.empty()) return front.back().aggregate;
        return operation(front.back().aggregate, back.back().aggregate);
    }
};

template <class T, class Operation, std::size_t Capacity>
class FixedFoldableQueue {
    struct Node {
        T value;
        T aggregate;
    };
    static_assert(std::is_trivially_destructible_v<Node>);

    T identity;
    Operation operation;
    using Slot = std::aligned_storage_t<sizeof(Node), alignof(Node)>;
    std::array<Slot, Capacity> front_storage, back_storage;
    std::size_t front_size = 0;
    std::size_t back_size = 0;

    Node &front(std::size_t index) { return *reinterpret_cast<Node *>(&front_storage[index]); }
    const Node &front(std::size_t index) const {
        return *reinterpret_cast<const Node *>(&front_storage[index]);
    }
    Node &back(std::size_t index) { return *reinterpret_cast<Node *>(&back_storage[index]); }
    const Node &back(std::size_t index) const {
        return *reinterpret_cast<const Node *>(&back_storage[index]);
    }
    static void store(Node &destination, T value, T aggregate) {
        std::construct_at(&destination, Node{value, aggregate});
    }

    void transfer() {
        while (back_size) {
            T value = back(--back_size).value;
            T aggregate = front_size ? operation(value, front(front_size - 1).aggregate) : value;
            store(front(front_size++), value, aggregate);
        }
    }

  public:
    explicit FixedFoldableQueue(T identity_value, Operation combine = {})
        : identity(identity_value), operation(combine) {}

    void push(T value) {
        assert(front_size + back_size < Capacity);
        T aggregate = back_size ? operation(back(back_size - 1).aggregate, value) : value;
        store(back(back_size++), value, aggregate);
    }

    void pop() {
        if (!front_size) transfer();
        --front_size;
    }

    T fold() const {
        if (!front_size) return back_size ? back(back_size - 1).aggregate : identity;
        if (!back_size) return front(front_size - 1).aggregate;
        return operation(front(front_size - 1).aggregate, back(back_size - 1).aggregate);
    }
};

template <class T, class Operation>
class FoldableDeque {
    struct Node {
        T value;
        T aggregate;
    };

    T identity;
    Operation operation;
    std::vector<Node> left;
    std::vector<Node> right;

    void append_left(std::vector<Node> &stack, T value) {
        T aggregate = stack.empty() ? value : operation(value, stack.back().aggregate);
        stack.push_back({value, aggregate});
    }

    void append_right(std::vector<Node> &stack, T value) {
        T aggregate = stack.empty() ? value : operation(stack.back().aggregate, value);
        stack.push_back({value, aggregate});
    }

    void rebuild_left() {
        std::size_t count = (right.size() + 1) / 2;
        std::vector<Node> new_left;
        std::vector<Node> new_right;
        new_left.reserve(count);
        new_right.reserve(right.size() - count);
        for (std::size_t i = count; i-- > 0;) append_left(new_left, right[i].value);
        for (std::size_t i = count; i < right.size(); ++i) append_right(new_right, right[i].value);
        left.swap(new_left);
        right.swap(new_right);
    }

    void rebuild_right() {
        std::size_t count = (left.size() + 1) / 2;
        std::vector<Node> new_left;
        std::vector<Node> new_right;
        new_left.reserve(left.size() - count);
        new_right.reserve(count);
        for (std::size_t i = count; i < left.size(); ++i) append_left(new_left, left[i].value);
        for (std::size_t i = count; i-- > 0;) append_right(new_right, left[i].value);
        left.swap(new_left);
        right.swap(new_right);
    }

  public:
    explicit FoldableDeque(T identity_value, Operation combine = {})
        : identity(identity_value), operation(combine) {}

    void push_front(T value) { append_left(left, value); }
    void push_back(T value) { append_right(right, value); }

    void pop_front() {
        if (left.empty()) rebuild_left();
        left.pop_back();
    }

    void pop_back() {
        if (right.empty()) rebuild_right();
        right.pop_back();
    }

    T fold() const {
        if (left.empty()) return right.empty() ? identity : right.back().aggregate;
        if (right.empty()) return left.back().aggregate;
        return operation(left.back().aggregate, right.back().aggregate);
    }
};

template <class T, class Operation, std::size_t MaxOperations>
class FixedFoldableDeque {
    struct Node {
        T value;
        T aggregate;
    };
    static_assert(std::is_trivially_destructible_v<Node>);

    T identity;
    Operation operation;
    using Slot = std::aligned_storage_t<sizeof(Node), alignof(Node)>;
    std::array<Slot, 2 * MaxOperations> storage;
    std::size_t begin = MaxOperations;
    std::size_t middle = MaxOperations;
    std::size_t end = MaxOperations;

    Node &data(std::size_t index) { return *reinterpret_cast<Node *>(&storage[index]); }
    const Node &data(std::size_t index) const {
        return *reinterpret_cast<const Node *>(&storage[index]);
    }
    static void store(Node &destination, T value, T aggregate) {
        std::construct_at(&destination, Node{value, aggregate});
    }

    void rebuild_left() {
        middle += (end - middle + 1) / 2;
        data(middle - 1).aggregate = data(middle - 1).value;
        for (std::size_t i = middle - 1; i-- > begin;)
            data(i).aggregate = operation(data(i).value, data(i + 1).aggregate);
        if (middle < end) {
            data(middle).aggregate = data(middle).value;
            for (std::size_t i = middle + 1; i < end; ++i)
                data(i).aggregate = operation(data(i - 1).aggregate, data(i).value);
        }
    }

    void rebuild_right() {
        middle -= (middle - begin + 1) / 2;
        if (begin < middle) {
            data(middle - 1).aggregate = data(middle - 1).value;
            for (std::size_t i = middle - 1; i-- > begin;)
                data(i).aggregate = operation(data(i).value, data(i + 1).aggregate);
        }
        data(middle).aggregate = data(middle).value;
        for (std::size_t i = middle + 1; i < end; ++i)
            data(i).aggregate = operation(data(i - 1).aggregate, data(i).value);
    }

  public:
    explicit FixedFoldableDeque(T identity_value, Operation combine = {})
        : identity(identity_value), operation(combine) {}

    void push_front(T value) {
        assert(begin);
        T aggregate = begin < middle ? operation(value, data(begin).aggregate) : value;
        --begin;
        store(data(begin), value, aggregate);
    }

    void push_back(T value) {
        assert(end < storage.size());
        T aggregate = middle < end ? operation(data(end - 1).aggregate, value) : value;
        store(data(end++), value, aggregate);
    }

    void pop_front() {
        if (begin == middle) rebuild_left();
        ++begin;
    }

    void pop_back() {
        if (middle == end) rebuild_right();
        --end;
    }

    T fold() const {
        if (begin == middle) return middle == end ? identity : data(end - 1).aggregate;
        if (middle == end) return data(begin).aggregate;
        return operation(data(begin).aggregate, data(end - 1).aggregate);
    }
};

template <uint32_t Mod>
class AffineDualSegmentTree {
    using Function = Affine<Mod>;
    int size;
    int height;
    std::vector<uint32_t> values;
    std::vector<Function> lazy;

    void apply_node(int node, Function function) {
        lazy[node] = ComposeAffine<Mod>{}(lazy[node], function);
    }

    void push(int node) {
        if (lazy[node].a == 1 && lazy[node].b == 0) return;
        apply_node(node * 2, lazy[node]);
        apply_node(node * 2 + 1, lazy[node]);
        lazy[node] = {};
    }

  public:
    explicit AffineDualSegmentTree(std::vector<uint32_t> initial_values)
        : size(std::bit_ceil((unsigned)std::max<std::size_t>(initial_values.size(), 1))),
          height(std::countr_zero((unsigned)size)), values(std::move(initial_values)),
          lazy(2 * size) {}

    void apply(int left, int right, Function function) {
        int l = left + size;
        int r = right + size;
        for (int shift = height; shift; --shift) {
            if ((l >> shift) << shift != l) push(l >> shift);
            if ((r >> shift) << shift != r) push((r - 1) >> shift);
        }
        while (l < r) {
            if (l & 1) apply_node(l++, function);
            if (r & 1) apply_node(--r, function);
            l >>= 1;
            r >>= 1;
        }
    }

    uint32_t get(int index) {
        int node = size + index;
        for (int shift = height; shift; --shift) push(node >> shift);
        return lazy[node](values[index]);
    }
};

template <uint32_t Mod>
class AffineLazySegmentTree {
    using Function = Affine<Mod>;
    int size;
    std::vector<uint32_t> sum;
    std::vector<Function> lazy;

    void apply_node(int node, int length, Function function) {
        sum[node] = ((uint64_t)function.a * sum[node] + (uint64_t)function.b * length) % Mod;
        lazy[node] = ComposeAffine<Mod>{}(lazy[node], function);
    }

    void push(int node, int length) {
        if (lazy[node].a == 1 && lazy[node].b == 0) return;
        apply_node(node * 2, length / 2, lazy[node]);
        apply_node(node * 2 + 1, length / 2, lazy[node]);
        lazy[node] = {};
    }

    void range_apply(int node, int left, int right, int query_left, int query_right,
                     Function function) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_node(node, right - left, function);
            return;
        }
        push(node, right - left);
        int middle = (left + right) / 2;
        range_apply(node * 2, left, middle, query_left, query_right, function);
        range_apply(node * 2 + 1, middle, right, query_left, query_right, function);
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
        if (sum[node] >= Mod) sum[node] -= Mod;
    }

    uint32_t fold(int node, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return 0;
        if (query_left <= left && right <= query_right) return sum[node];
        push(node, right - left);
        int middle = (left + right) / 2;
        uint32_t result = fold(node * 2, left, middle, query_left, query_right) +
                          fold(node * 2 + 1, middle, right, query_left, query_right);
        return result >= Mod ? result - Mod : result;
    }

  public:
    explicit AffineLazySegmentTree(const std::vector<uint32_t> &values)
        : size(std::bit_ceil((unsigned)values.size())), sum(2 * size), lazy(2 * size) {
        std::copy(values.begin(), values.end(), sum.begin() + size);
        for (int node = size - 1; node; --node) {
            sum[node] = sum[node * 2] + sum[node * 2 + 1];
            if (sum[node] >= Mod) sum[node] -= Mod;
        }
    }

    void apply(int left, int right, Function function) {
        range_apply(1, 0, size, left, right, function);
    }

    uint32_t fold(int left, int right) { return fold(1, 0, size, left, right); }

    uint32_t get(int index) { return fold(index, index + 1); }
};

template <uint32_t Mod>
class RangeSetCompositeTree {
    using Function = Affine<Mod>;
    int size;
    int depth;
    std::vector<Function> aggregate;
    std::vector<int> lazy_base;
    std::vector<Function> powers;

    void apply(int node, int level, int base) {
        aggregate[node] = powers[base + level];
        lazy_base[node] = base;
    }
    void push(int node, int level) {
        if (lazy_base[node] < 0) return;
        apply(node * 2, level - 1, lazy_base[node]);
        apply(node * 2 + 1, level - 1, lazy_base[node]);
        lazy_base[node] = -1;
    }
    void pull(int node) {
        aggregate[node] = ComposeAffine<Mod>{}(aggregate[node * 2], aggregate[node * 2 + 1]);
    }

  public:
    explicit RangeSetCompositeTree(const std::vector<Function> &values,
                                   int expected_assignments = 0)
        : size(std::bit_ceil((unsigned)std::max<std::size_t>(values.size(), 1))),
          depth(std::countr_zero((unsigned)size)), aggregate(2 * size), lazy_base(2 * size, -1) {
        powers.reserve((std::size_t)expected_assignments * (depth + 1));
        std::copy(values.begin(), values.end(), aggregate.begin() + size);
        for (int node = size - 1; node; --node)
            aggregate[node] = ComposeAffine<Mod>{}(aggregate[node * 2], aggregate[node * 2 + 1]);
    }
    void set(int left, int right, Function function) {
        int levels = std::bit_width((unsigned)(right - left));
        int base = powers.size();
        powers.push_back(function);
        for (int level = 1; level < levels; ++level)
            powers.push_back(ComposeAffine<Mod>{}(powers.back(), powers.back()));

        int l = left + size;
        int r = right + size;
        int original_left = l;
        int original_right = r;
        for (int level = depth; level; --level) {
            if ((original_left >> level) << level != original_left)
                push(original_left >> level, level);
            if ((original_right >> level) << level != original_right)
                push((original_right - 1) >> level, level);
        }
        for (int level = 0; l < r; ++level, l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, level, base);
            if (r & 1) apply(--r, level, base);
        }
        for (int level = 1; level <= depth; ++level) {
            if ((original_left >> level) << level != original_left) pull(original_left >> level);
            if ((original_right >> level) << level != original_right)
                pull((original_right - 1) >> level);
        }
    }
    Function fold(int left, int right) {
        int l = left + size;
        int r = right + size;
        for (int level = depth; level; --level) {
            if ((l >> level) << level != l) push(l >> level, level);
            if ((r >> level) << level != r) push((r - 1) >> level, level);
        }
        Function before, after;
        while (l < r) {
            if (l & 1) before = ComposeAffine<Mod>{}(before, aggregate[l++]);
            if (r & 1) after = ComposeAffine<Mod>{}(aggregate[--r], after);
            l >>= 1;
            r >>= 1;
        }
        return ComposeAffine<Mod>{}(before, after);
    }
};

template <uint32_t Mod, std::size_t Capacity>
class FixedAffineRangeSumTree {
    using Function = Affine<Mod>;
    struct Node {
        Function lazy;
        uint32_t length = 0;
        uint32_t sum = 0;
    };

    int count = 0;
    std::array<Node, 2 * Capacity> nodes;

    static uint32_t add(uint32_t first, uint32_t second) {
        uint32_t result = first + second;
        return result >= Mod ? result - Mod : result;
    }

    void apply_node(int node, Function function) {
        nodes[node].lazy = ComposeAffine<Mod>{}(nodes[node].lazy, function);
        nodes[node].sum =
            ((uint64_t)function.a * nodes[node].sum + (uint64_t)function.b * nodes[node].length) %
            Mod;
    }

    void push(int node) {
        Function function = nodes[node].lazy;
        if (function.a == 1 && function.b == 0) return;
        nodes[node].lazy = {};
        apply_node(node * 2, function);
        apply_node(node * 2 + 1, function);
    }

    void push_path(int node) {
        for (int shift = std::bit_width((unsigned)node) - 1; shift; --shift) push(node >> shift);
    }

    void pull_path(int node) {
        while (node >>= 1) nodes[node].sum = add(nodes[node * 2].sum, nodes[node * 2 + 1].sum);
    }

  public:
    void reset(const std::vector<uint32_t> &values) {
        count = values.size();
        assert(count <= (int)Capacity);
        for (int i = 0; i < count; ++i) nodes[count + i] = {{}, 1, values[i]};
        for (int node = count - 1; node; --node) {
            nodes[node].lazy = {};
            nodes[node].length = nodes[node * 2].length + nodes[node * 2 + 1].length;
            nodes[node].sum = add(nodes[node * 2].sum, nodes[node * 2 + 1].sum);
        }
    }

    void apply(int left, int right, Function function) {
        int l = count + left;
        int r = count + right - 1;
        push_path(l--);
        push_path(r++);
        unsigned width = std::bit_width((unsigned)(l ^ r)) - 1;
        unsigned mask = (unsigned(1) << width) - 1;

        unsigned remaining = ~unsigned(l) & mask;
        int shift = 31;
        while (remaining) {
            shift = std::bit_width(remaining) - 1;
            remaining -= unsigned(1) << shift;
            apply_node((l >> shift) ^ 1, function);
        }
        pull_path(l >> shift);

        remaining = unsigned(r) & mask;
        shift = 31;
        while (remaining) {
            shift = std::bit_width(remaining) - 1;
            remaining -= unsigned(1) << shift;
            apply_node((r >> shift) ^ 1, function);
        }
        pull_path(r >> shift);
    }

    uint32_t fold(int left, int right) const {
        int l = count + left - 1;
        int r = count + right;
        uint64_t left_length = 0, left_sum = 0;
        uint64_t right_length = 0, right_sum = 0;
        while ((l ^ r) != 1) {
            if (~l & 1) {
                left_length += nodes[l ^ 1].length;
                left_sum += nodes[l ^ 1].sum;
            }
            if (r & 1) {
                right_length += nodes[r ^ 1].length;
                right_sum += nodes[r ^ 1].sum;
            }
            l >>= 1;
            r >>= 1;
            left_sum = (nodes[l].lazy.a * left_sum + nodes[l].lazy.b * left_length) % Mod;
            right_sum = (nodes[r].lazy.a * right_sum + nodes[r].lazy.b * right_length) % Mod;
        }
        uint64_t sum = add(left_sum, right_sum);
        uint64_t length = left_length + right_length;
        for (l >>= 1; l; l >>= 1) sum = (nodes[l].lazy.a * sum + nodes[l].lazy.b * length) % Mod;
        return sum;
    }
};

template <uint32_t Mod>
class CompressedAffineSumTree {
    using Function = Affine<Mod>;
    int size;
    std::vector<uint32_t> sum, length;
    std::vector<Function> lazy;
    void apply(int node, Function f) {
        sum[node] = ((uint64_t)f.a * sum[node] + (uint64_t)f.b * length[node]) % Mod;
        lazy[node] = ComposeAffine<Mod>{}(lazy[node], f);
    }
    void push(int node) {
        if (lazy[node].a == 1 && lazy[node].b == 0) return;
        apply(node * 2, lazy[node]);
        apply(node * 2 + 1, lazy[node]);
        lazy[node] = {};
    }
    void apply(int node, int left, int right, int ql, int qr, Function f) {
        if (qr <= left || right <= ql) return;
        if (ql <= left && right <= qr) {
            apply(node, f);
            return;
        }
        push(node);
        int middle = (left + right) / 2;
        apply(node * 2, left, middle, ql, qr, f);
        apply(node * 2 + 1, middle, right, ql, qr, f);
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
        if (sum[node] >= Mod) sum[node] -= Mod;
    }
    uint32_t fold(int node, int left, int right, int ql, int qr) {
        if (qr <= left || right <= ql) return 0;
        if (ql <= left && right <= qr) return sum[node];
        push(node);
        int middle = (left + right) / 2;
        uint32_t result =
            fold(node * 2, left, middle, ql, qr) + fold(node * 2 + 1, middle, right, ql, qr);
        return result >= Mod ? result - Mod : result;
    }

  public:
    explicit CompressedAffineSumTree(const std::vector<int> &coordinates)
        : size(std::bit_ceil((unsigned)(coordinates.size() - 1))), sum(2 * size), length(2 * size),
          lazy(2 * size) {
        for (int i = 0; i + 1 < (int)coordinates.size(); ++i)
            length[size + i] = coordinates[i + 1] - coordinates[i];
        for (int i = size - 1; i; --i) length[i] = length[i * 2] + length[i * 2 + 1];
    }
    void apply(int left, int right, Function f) { apply(1, 0, size, left, right, f); }
    uint32_t fold(int left, int right) { return fold(1, 0, size, left, right); }
};

} // namespace toy
