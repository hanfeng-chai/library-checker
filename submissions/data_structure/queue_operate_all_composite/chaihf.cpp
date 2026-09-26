// BEGIN bundled: problems/data_structure/queue_operate_all_composite/two_stack_aggregate.cpp
// BEGIN bundled: include/toy/affine.hpp

#include <bits/extc++.h>

namespace toy {

template<uint32_t Mod>
struct Affine {
    uint32_t a = 1;
    uint32_t b = 0;

    uint32_t operator()(uint32_t x) const {
        return ((uint64_t)a * x + b) % Mod;
    }
};

template<uint32_t Mod>
struct ComposeAffine {
    Affine<Mod> operator()(Affine<Mod> first, Affine<Mod> second) const {
        return {
            (uint32_t)((uint64_t)second.a * first.a % Mod),
            (uint32_t)(((uint64_t)second.a * first.b + second.b) % Mod)
        };
    }
};

template<uint32_t Mod>
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
        : count(length), size(std::bit_ceil((unsigned)std::max(length, 1))),
          tree(2 * size) {}

    explicit AffineSegmentTree(const std::vector<Function>& values)
        : count(values.size()),
          size(std::bit_ceil((unsigned)std::max<std::size_t>(values.size(), 1))),
          tree(2 * size) {
        std::copy(values.begin(), values.end(), tree.begin() + size);
        for (int node = size - 1; node; --node)
            tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
    }

    void set(int index, Function function) {
        int node = size + index;
        tree[node] = function;
        while (node >>= 1)
            tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
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

template<uint32_t Mod>
class AffineEvaluationSegmentTree {
    using Function = Affine<Mod>;
    int size;
    std::vector<Function> tree;

    static Function reverse_compose(Function left, Function right) {
        return ComposeAffine<Mod>{}(right, left);
    }

public:
    explicit AffineEvaluationSegmentTree(const std::vector<Function>& values)
        : size(values.size()), tree(2 * size) {
        for (int i = 0; i < size; ++i)
            tree[2 * size - 1 - i] = values[i];
        for (int node = size - 1; node; --node)
            tree[node] = reverse_compose(tree[node * 2], tree[node * 2 + 1]);
    }

    void set(int index, Function function) {
        int node = 2 * size - 1 - index;
        tree[node] = function;
        while (node >>= 1)
            tree[node] = reverse_compose(tree[node * 2], tree[node * 2 + 1]);
    }

    uint32_t apply(int left, int right, uint32_t value) const {
        int l = 2 * size - 1 - right;
        int r = 2 * size - left;
        unsigned width = std::bit_width((unsigned)(l ^ r)) - 1;
        int boundary = r >> width;
        for (r = (r >> std::countr_zero((unsigned)r)) ^ 1;
             r > boundary;
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

template<class T, class Operation>
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

template<class T, class Operation, std::size_t Capacity>
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

    Node& front(std::size_t index) {
        return *reinterpret_cast<Node*>(&front_storage[index]);
    }
    const Node& front(std::size_t index) const {
        return *reinterpret_cast<const Node*>(&front_storage[index]);
    }
    Node& back(std::size_t index) {
        return *reinterpret_cast<Node*>(&back_storage[index]);
    }
    const Node& back(std::size_t index) const {
        return *reinterpret_cast<const Node*>(&back_storage[index]);
    }
    static void store(Node& destination, T value, T aggregate) {
        std::construct_at(&destination, Node{value, aggregate});
    }

    void transfer() {
        while (back_size) {
            T value = back(--back_size).value;
            T aggregate = front_size
                ? operation(value, front(front_size - 1).aggregate) : value;
            store(front(front_size++), value, aggregate);
        }
    }

public:
    explicit FixedFoldableQueue(T identity_value, Operation combine = {})
        : identity(identity_value), operation(combine) {}

    void push(T value) {
        assert(front_size + back_size < Capacity);
        T aggregate = back_size
            ? operation(back(back_size - 1).aggregate, value) : value;
        store(back(back_size++), value, aggregate);
    }

    void pop() {
        if (!front_size) transfer();
        --front_size;
    }

    T fold() const {
        if (!front_size)
            return back_size ? back(back_size - 1).aggregate : identity;
        if (!back_size) return front(front_size - 1).aggregate;
        return operation(front(front_size - 1).aggregate,
                         back(back_size - 1).aggregate);
    }
};

template<class T, class Operation>
class FoldableDeque {
    struct Node {
        T value;
        T aggregate;
    };

    T identity;
    Operation operation;
    std::vector<Node> left;
    std::vector<Node> right;

    void append_left(std::vector<Node>& stack, T value) {
        T aggregate = stack.empty() ? value : operation(value, stack.back().aggregate);
        stack.push_back({value, aggregate});
    }

    void append_right(std::vector<Node>& stack, T value) {
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
        for (std::size_t i = count; i < right.size(); ++i)
            append_right(new_right, right[i].value);
        left.swap(new_left);
        right.swap(new_right);
    }

    void rebuild_right() {
        std::size_t count = (left.size() + 1) / 2;
        std::vector<Node> new_left;
        std::vector<Node> new_right;
        new_left.reserve(left.size() - count);
        new_right.reserve(count);
        for (std::size_t i = count; i < left.size(); ++i)
            append_left(new_left, left[i].value);
        for (std::size_t i = count; i-- > 0;)
            append_right(new_right, left[i].value);
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

template<class T, class Operation, std::size_t MaxOperations>
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

    Node& data(std::size_t index) {
        return *reinterpret_cast<Node*>(&storage[index]);
    }
    const Node& data(std::size_t index) const {
        return *reinterpret_cast<const Node*>(&storage[index]);
    }
    static void store(Node& destination, T value, T aggregate) {
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
        T aggregate = begin < middle
            ? operation(value, data(begin).aggregate) : value;
        --begin;
        store(data(begin), value, aggregate);
    }

    void push_back(T value) {
        assert(end < storage.size());
        T aggregate = middle < end
            ? operation(data(end - 1).aggregate, value) : value;
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
        if (begin == middle)
            return middle == end ? identity : data(end - 1).aggregate;
        if (middle == end) return data(begin).aggregate;
        return operation(data(begin).aggregate, data(end - 1).aggregate);
    }
};

template<uint32_t Mod>
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
        : size(std::bit_ceil((unsigned)std::max<std::size_t>(
              initial_values.size(), 1))),
          height(std::countr_zero((unsigned)size)),
          values(std::move(initial_values)), lazy(2 * size) {}

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

template<uint32_t Mod>
class AffineLazySegmentTree {
    using Function = Affine<Mod>;
    int size;
    std::vector<uint32_t> sum;
    std::vector<Function> lazy;

    void apply_node(int node, int length, Function function) {
        sum[node] = ((uint64_t)function.a * sum[node] +
                     (uint64_t)function.b * length) % Mod;
        lazy[node] = ComposeAffine<Mod>{}(lazy[node], function);
    }

    void push(int node, int length) {
        if (lazy[node].a == 1 && lazy[node].b == 0) return;
        apply_node(node * 2, length / 2, lazy[node]);
        apply_node(node * 2 + 1, length / 2, lazy[node]);
        lazy[node] = {};
    }

    void range_apply(int node, int left, int right, int query_left,
                     int query_right, Function function) {
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
    explicit AffineLazySegmentTree(const std::vector<uint32_t>& values)
        : size(std::bit_ceil((unsigned)values.size())), sum(2 * size),
          lazy(2 * size) {
        std::copy(values.begin(), values.end(), sum.begin() + size);
        for (int node = size - 1; node; --node) {
            sum[node] = sum[node * 2] + sum[node * 2 + 1];
            if (sum[node] >= Mod) sum[node] -= Mod;
        }
    }

    void apply(int left, int right, Function function) {
        range_apply(1, 0, size, left, right, function);
    }

    uint32_t fold(int left, int right) {
        return fold(1, 0, size, left, right);
    }

    uint32_t get(int index) { return fold(index, index + 1); }
};

template<uint32_t Mod>
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
        aggregate[node] = ComposeAffine<Mod>{}(
            aggregate[node * 2], aggregate[node * 2 + 1]);
    }
public:
    explicit RangeSetCompositeTree(const std::vector<Function>& values,
                                   int expected_assignments = 0)
        : size(std::bit_ceil((unsigned)std::max<std::size_t>(values.size(), 1))),
          depth(std::countr_zero((unsigned)size)), aggregate(2 * size),
          lazy_base(2 * size, -1) {
        powers.reserve((std::size_t)expected_assignments * (depth + 1));
        std::copy(values.begin(), values.end(), aggregate.begin() + size);
        for (int node = size - 1; node; --node)
            aggregate[node] = ComposeAffine<Mod>{}(
                aggregate[node * 2], aggregate[node * 2 + 1]);
    }
    void set(int left, int right, Function function) {
        int levels = std::bit_width((unsigned)(right - left));
        int base = powers.size();
        powers.push_back(function);
        for (int level = 1; level < levels; ++level)
            powers.push_back(ComposeAffine<Mod>{}(
                powers.back(), powers.back()));

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
            if ((original_left >> level) << level != original_left)
                pull(original_left >> level);
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
            if (l & 1)
                before = ComposeAffine<Mod>{}(before, aggregate[l++]);
            if (r & 1)
                after = ComposeAffine<Mod>{}(aggregate[--r], after);
            l >>= 1;
            r >>= 1;
        }
        return ComposeAffine<Mod>{}(before, after);
    }
};

template<uint32_t Mod, std::size_t Capacity>
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
            ((uint64_t)function.a * nodes[node].sum +
             (uint64_t)function.b * nodes[node].length) % Mod;
    }

    void push(int node) {
        Function function = nodes[node].lazy;
        if (function.a == 1 && function.b == 0) return;
        nodes[node].lazy = {};
        apply_node(node * 2, function);
        apply_node(node * 2 + 1, function);
    }

    void push_path(int node) {
        for (int shift = std::bit_width((unsigned)node) - 1; shift; --shift)
            push(node >> shift);
    }

    void pull_path(int node) {
        while (node >>= 1)
            nodes[node].sum = add(nodes[node * 2].sum, nodes[node * 2 + 1].sum);
    }

public:
    void reset(const std::vector<uint32_t>& values) {
        count = values.size();
        assert(count <= (int)Capacity);
        for (int i = 0; i < count; ++i)
            nodes[count + i] = {{}, 1, values[i]};
        for (int node = count - 1; node; --node) {
            nodes[node].lazy = {};
            nodes[node].length =
                nodes[node * 2].length + nodes[node * 2 + 1].length;
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
            left_sum = (nodes[l].lazy.a * left_sum +
                        nodes[l].lazy.b * left_length) % Mod;
            right_sum = (nodes[r].lazy.a * right_sum +
                         nodes[r].lazy.b * right_length) % Mod;
        }
        uint64_t sum = add(left_sum, right_sum);
        uint64_t length = left_length + right_length;
        for (l >>= 1; l; l >>= 1)
            sum = (nodes[l].lazy.a * sum + nodes[l].lazy.b * length) % Mod;
        return sum;
    }
};

template<uint32_t Mod>
class CompressedAffineSumTree {
    using Function=Affine<Mod>;
    int size;
    std::vector<uint32_t> sum,length;
    std::vector<Function> lazy;
    void apply(int node,Function f){
        sum[node]=((uint64_t)f.a*sum[node]+(uint64_t)f.b*length[node])%Mod;
        lazy[node]=ComposeAffine<Mod>{}(lazy[node],f);
    }
    void push(int node){if(lazy[node].a==1&&lazy[node].b==0)return;apply(node*2,lazy[node]);apply(node*2+1,lazy[node]);lazy[node]={};}
    void apply(int node,int left,int right,int ql,int qr,Function f){
        if(qr<=left||right<=ql)return;
        if(ql<=left&&right<=qr){apply(node,f);return;}
        push(node);int middle=(left+right)/2;apply(node*2,left,middle,ql,qr,f);apply(node*2+1,middle,right,ql,qr,f);
        sum[node]=sum[node*2]+sum[node*2+1];if(sum[node]>=Mod)sum[node]-=Mod;
    }
    uint32_t fold(int node,int left,int right,int ql,int qr){
        if(qr<=left||right<=ql)return 0;
        if(ql<=left&&right<=qr)return sum[node];
        push(node);int middle=(left+right)/2;uint32_t result=fold(node*2,left,middle,ql,qr)+fold(node*2+1,middle,right,ql,qr);return result>=Mod?result-Mod:result;
    }
public:
    explicit CompressedAffineSumTree(const std::vector<int>& coordinates)
        :size(std::bit_ceil((unsigned)(coordinates.size()-1))),sum(2*size),length(2*size),lazy(2*size){
        for(int i=0;i+1<(int)coordinates.size();++i)length[size+i]=coordinates[i+1]-coordinates[i];
        for(int i=size-1;i;--i)length[i]=length[i*2]+length[i*2+1];
    }
    void apply(int left,int right,Function f){apply(1,0,size,left,right,f);}
    uint32_t fold(int left,int right){return fold(1,0,size,left,right);}
};

} // namespace toy
// END bundled: include/toy/affine.hpp
// BEGIN bundled: include/toy/io.hpp

// Linux x86-64 / GCC / C++23 only. Reader APIs encode token shape so fixed,
// bounded, full-range, and 128-bit inputs use independently benchmarked paths.
// Writer uses base-10^4 groups; signed 128-bit output is split at 10^19 so no
// compiler __divti3/__modti3 helper remains in the hot loop.
#include <bits/extc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef NDEBUG
#define toy_assert(expr) [[assume(expr)]]
#else
#define toy_assert(expr) assert(expr)
#endif

namespace toy {

using namespace std;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using isize = intptr_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

struct DirectMappingTag {};
inline constexpr DirectMappingTag direct_mapping;
using usize = uintptr_t;
using f32 = float;
using f64 = double;
using f80 = long double;

namespace detail {

inline void write_all(const char* p, usize n) {
    while (n) {
        isize z = ::write(1, p, n);
        if (z > 0) p += z, n -= z;
        else if (z < 0 && errno == EINTR) continue;
        else break;
    }
}

// High half of a 128x128 product, assembled from four 64x64 products.
inline u128 mulhi(u128 a, u128 b) {
    u64 al = (u64)a, ah = (u64)(a >> 64);
    u64 bl = (u64)b, bh = (u64)(b >> 64);
    u128 ll = (u128)al * bl, lh = (u128)al * bh;
    u128 hl = (u128)ah * bl, hh = (u128)ah * bh;
    u128 mid = (ll >> 64) + (u64)lh + (u64)hl;
    return hh + (lh >> 64) + (hl >> 64) + (mid >> 64);
}

// q=floor(x/10^19) via ceil(2^192/10^19); r=x-q*10^19.
inline pair<u64, u64> divmod_1e19(u128 x) {
    constexpr u64 B = 10'000'000'000'000'000'000ULL;
    constexpr u128 M = ((u128)0xd83c94fb6d2ac34aULL << 64) |
                       0x5663d3c7a0d865cbULL;
    if (x < B) return {0, (u64)x};
    u128 h = mulhi(x, M), s = x + h;
    u64 q = (u64)((s >> 64) + (s < x));
    return {q, (u64)(x - (u128)q * B)};
}

} // namespace detail

class Reader {
    const char* p;

    static const char* map_stdin() {
        struct stat st{};
        fstat(0, &st);
        usize page = (usize)sysconf(_SC_PAGESIZE);
        usize size = (usize)st.st_size;
        usize mapped = (size + page - 1) & -page;
        void* base = mmap(nullptr, mapped + page, PROT_READ,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        toy_assert(base != MAP_FAILED);
        if (size) {
            void* file = mmap(base, size, PROT_READ,
                              MAP_PRIVATE | MAP_FIXED, 0, 0);
            toy_assert(file == base);
        }
        return (const char*)base;
    }

    static const char* map_stdin_direct() {
        struct stat st{};
        fstat(0, &st);
        void* base = mmap(nullptr, (usize)st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
        toy_assert(base != MAP_FAILED);
        return (const char*)base;
    }

    static const array<array<char, 16>, 17>& masks() {
        static constexpr auto a = [] {
            array<array<char, 16>, 17> result{};
            for (int n = 0; n <= 16; ++n)
                for (int i = 0; i < 16; ++i)
                    result[n][i] = i < 16 - n ? (char)0x80 : (char)(i - 16 + n);
            return result;
        }();
        return a;
    }

    static u64 parse16(__m128i x) {
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010a));
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));
        __m128i a = _mm_mul_epu32(x, _mm_set_epi32(1, 10000, 1, 10000));
        x = _mm_add_epi64(a, _mm_srli_epi64(x, 32));
        return (u64)_mm_cvtsi128_si64(x) * 100000000ULL +
               (u64)_mm_extract_epi64(x, 1);
    }

    static bool all_digit(u64 x) {
        return !(((x ^ 0x3030303030303030ULL) & 0xf0f0f0f0f0f0f0f0ULL));
    }

    static bool all_digit4(u32 x) {
        return !(((x ^ 0x30303030U) & 0xf0f0f0f0U));
    }

    static u32 parse4(u32 x) {
        x ^= 0x30303030U;
        x = (x * 10 + (x >> 8)) & 0x00ff00ffU;
        return (x * 100 + (x >> 16)) & 0x0000ffffU;
    }

    static u64 parse8(u64 x) {
        x ^= 0x3030303030303030ULL;
        x = (x * 10 + (x >> 8)) & 0x00ff00ff00ff00ffULL;
        x = (x * 100 + (x >> 16)) & 0x0000ffff0000ffffULL;
        return (x * 10000 + (x >> 32)) & 0x00000000ffffffffULL;
    }

    static u128 parse32(__m256i x) {
        x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(0x010a));
        x = _mm256_madd_epi16(x, _mm256_set1_epi32(0x00010064));
        __m256i a = _mm256_mul_epu32(
            x, _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000));
        x = _mm256_add_epi64(a, _mm256_srli_epi64(x, 32));
        __m128i a0 = _mm256_castsi256_si128(x);
        __m128i a1 = _mm256_extracti128_si256(x, 1);
        u64 hi = (u64)_mm_cvtsi128_si64(a0) * 100000000ULL +
                 (u64)_mm_extract_epi64(a0, 1);
        u64 lo = (u64)_mm_cvtsi128_si64(a1) * 100000000ULL +
                 (u64)_mm_extract_epi64(a1, 1);
        return (u128)hi * 10000000000000000ULL + lo;
    }

    static const array<u64, 17>& powers10() {
        static constexpr auto a = [] {
            array<u64, 17> result{1};
            for (int i = 1; i <= 16; ++i) result[i] = result[i - 1] * 10;
            return result;
        }();
        return a;
    }

    [[gnu::always_inline]] static u64 parse_short16(__m128i x, u32 boundary,
                                                     int& digits) {
        digits = __builtin_ctz(boundary);
        x = _mm_shuffle_epi8(
            x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
        return parse16(x);
    }

    [[gnu::always_inline]] u64 read_swar_u64() {
        const char* q = p;
        u64 a, b;
        memcpy(&a, q, 8); memcpy(&b, q + 8, 8);
        if (all_digit(a)) {
            u64 v = parse8(a); q += 8;
            if (all_digit(b)) v = v * 100000000ULL + parse8(b), q += 8;
            while (*q >= '0') v = v * 10 + *q++ - '0';
            p = q + 1;
            return v;
        }
        u64 v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] u64 read_swar4_u64() {
        const char* q = p;
        u32 block;
        memcpy(&block, q, 4);
        u64 value = 0;
        if (all_digit4(block)) {
            value = parse4(block);
            q += 4;
        }
        while (*q >= '0') value = value * 10 + *q++ - '0';
        p = q + 1;
        return value;
    }

    [[gnu::always_inline]] u64 read_scalar_u64() {
        u64 v = 0;
        while (*p >= '0') v = v * 10 + *p++ - '0';
        ++p;
        return v;
    }

    [[gnu::always_inline]] u64 read_simd_u64() {
        const char* q = p;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (__builtin_expect(boundary != 0, 1)) {
            int digits = __builtin_ctz(boundary);
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[digits].data()));
            p = q + digits + 1;
            return parse16(x);
        }
        u64 v = parse16(x); q += 16;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        p = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_avx_u128(const char*& cursor) {
        const char* q = cursor;
        u128 v;
        __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)q),
                                     _mm256_set1_epi8('0'));
        __m256i hi = _mm256_and_si256(x, _mm256_set1_epi8((char)0xf0));
        u32 bad = ~((u32)_mm256_movemask_epi8(
            _mm256_cmpeq_epi8(hi, _mm256_setzero_si256())));
        if (!bad) {
            v = parse32(x); q += 32;
            u64 tail = 0, scale = 1;
            while (*q >= '0') {
                tail = tail * 10 + *q++ - '0';
                scale *= 10;
            }
            v = v * scale + tail;
            cursor = q + 1;
            return v;
        }
        int n = __builtin_ctz(bad);
        if (n) {
            if (n <= 16) {
                __m128i lo = _mm256_castsi256_si128(x);
                lo = _mm_shuffle_epi8(
                    lo, _mm_loadu_si128((const __m128i*)masks()[n].data()));
                v = parse16(lo);
            } else {
                alignas(32) u8 b[64]{};
                _mm256_storeu_si256((__m256i*)(b + 32 - n), x);
                v = parse32(_mm256_load_si256((const __m256i*)b));
            }
            cursor = q + n + 1;
            return v;
        }
        v = 0;
        while (*q >= '0') v = v * 10 + *q++ - '0';
        cursor = q + 1;
        return v;
    }

    [[gnu::always_inline]] static u128 read_staged_u128(const char*& cursor) {
        const char* q = cursor;
        __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                                  _mm_set1_epi8('0'));
        u32 boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u128 value = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value;
        }

        u128 value = parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        if (boundary) {
            int digits;
            u64 tail = parse_short16(x, boundary, digits);
            cursor = q + digits + 1;
            return value * powers10()[digits] + tail;
        }

        value = value * 10000000000000000ULL + parse16(x);
        q += 16;
        x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)q),
                         _mm_set1_epi8('0'));
        boundary = (u32)_mm_movemask_epi8(x);
        int digits;
        u64 tail = parse_short16(x, boundary, digits);
        cursor = q + digits + 1;
        return value * powers10()[digits] + tail;
    }

    template<int Digits, class U>
    [[gnu::always_inline]] U read_fixed_unsigned() {
        static_assert(1 <= Digits && Digits <= 37);
        U v;
        if constexpr (Digits <= 7) {
            v = 0;
            for (int i = 0; i < Digits; ++i) v = v * 10 + p[i] - '0';
        } else if constexpr (Digits == 8) {
            u64 x;
            memcpy(&x, p, 8);
            v = (U)parse8(x);
        } else if constexpr (Digits <= 16) {
            __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                      _mm_set1_epi8('0'));
            x = _mm_shuffle_epi8(
                x, _mm_loadu_si128((const __m128i*)masks()[Digits].data()));
            v = (U)parse16(x);
        } else if constexpr (Digits < 32) {
            if constexpr (Digits >= 24) {
                v = 0;
                int offset = 0;
                for (; offset + 8 <= Digits; offset += 8) {
                    u64 block;
                    memcpy(&block, p + offset, 8);
                    v = v * 100000000 + parse8(block);
                }
                for (; offset < Digits; ++offset) v = v * 10 + p[offset] - '0';
            } else {
                __m128i x = _mm_sub_epi8(_mm_loadu_si128((const __m128i*)p),
                                          _mm_set1_epi8('0'));
                v = (U)parse16(x);
                for (int i = 16; i < Digits; ++i) v = v * 10 + p[i] - '0';
            }
        } else {
            __m256i x = _mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)p),
                                         _mm256_set1_epi8('0'));
            v = (U)parse32(x);
            for (int i = 32; i < Digits; ++i) v = v * 10 + p[i] - '0';
        }
        p += Digits + 1;
        return v;
    }

    template<class T>
    [[gnu::always_inline]] T read_signed(auto read_unsigned) {
        bool neg = *p == '-'; p += neg;
        auto v = (this->*read_unsigned)();
        using U = make_unsigned_t<T>;
        U magnitude = (U)v;
        return neg ? (T)(U(0) - magnitude) : (T)magnitude;
    }

public:
    Reader(): p(map_stdin()) {}
    explicit Reader(DirectMappingTag): p(map_stdin_direct()) {}
    explicit Reader(const char* input): p(input) {}

    [[gnu::always_inline]] u32 read_digit() {
        u32 value = (u32)(*p - '0');
        p += 2;
        return value;
    }

    [[gnu::always_inline]] u32 read_u32() { return (u32)read_swar_u64(); }
    [[gnu::always_inline]] i32 read_i32() { return read_signed<i32>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u64 read_u64() { return read_simd_u64(); }
    [[gnu::always_inline]] i64 read_i64() { return read_signed<i64>(&Reader::read_simd_u64); }
    [[gnu::always_inline]] u128 read_u128() { return read_avx_u128(p); }
    [[gnu::always_inline]] i128 read_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_avx_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }
    [[gnu::always_inline]] i128 read_staged_i128() {
        bool neg = *p == '-'; p += neg;
        u128 value = read_staged_u128(p);
        return neg ? (i128)(u128(0) - value) : (i128)value;
    }

    template<int Digits, class T = u64>
    [[gnu::always_inline]] T read_fixed() {
        static_assert(is_integral_v<T> || same_as<T, i128> || same_as<T, u128>);
        if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value = read_fixed_unsigned<Digits, u128>();
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else if constexpr (same_as<T, u128>) {
            return read_fixed_unsigned<Digits, u128>();
        } else if constexpr (is_signed_v<T>) {
            bool neg = *p == '-'; p += neg;
            using U = make_unsigned_t<T>;
            U value = read_fixed_unsigned<Digits, U>();
            return neg ? (T)(U(0) - value) : (T)value;
        } else {
            return read_fixed_unsigned<Digits, T>();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_var() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else return read_u128();
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else value = read_avx_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        }
        else if constexpr (is_signed_v<T>) {
            if constexpr (MaxDigits <= 2)
                return read_signed<T>(&Reader::read_scalar_u64);
            else return read_signed<T>(&Reader::read_simd_u64);
        } else {
            if constexpr (MaxDigits <= 2) return (T)read_scalar_u64();
            else return (T)read_simd_u64();
        }
    }

    template<int MaxDigits, class T = u64>
    [[gnu::always_inline]] T read_uniform() {
        static_assert(1 <= MaxDigits && MaxDigits <= 37);
        if constexpr (same_as<T, u128>) {
            if constexpr (MaxDigits <= 19) return (u128)read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 34)
                return read_avx_u128(p);
            else return read_staged_u128(p);
        } else if constexpr (same_as<T, i128>) {
            bool neg = *p == '-'; p += neg;
            u128 value;
            if constexpr (MaxDigits <= 18) value = read_simd_u64();
            else if constexpr (32 <= MaxDigits && MaxDigits <= 36)
                value = read_avx_u128(p);
            else value = read_staged_u128(p);
            return neg ? (i128)(u128(0) - value) : (i128)value;
        } else {
            bool neg = false;
            if constexpr (is_signed_v<T>) neg = *p == '-', p += neg;
            u64 value;
            if constexpr (is_signed_v<T>) {
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
                else if constexpr (MaxDigits <= 13) value = read_swar_u64();
                else value = read_simd_u64();
            } else {
                if constexpr (MaxDigits <= 3) value = read_scalar_u64();
                else if constexpr (MaxDigits <= 7) value = read_swar4_u64();
                else if constexpr (MaxDigits <= 12) value = read_swar_u64();
                else value = read_simd_u64();
            }
            using U = make_unsigned_t<T>;
            U magnitude = (U)value;
            if constexpr (is_signed_v<T>)
                return neg ? (T)(U(0) - magnitude) : (T)magnitude;
            else return (T)magnitude;
        }
    }

    template<u32 Mod>
    [[gnu::always_inline]] u32 read_mod() {
        u32 value = read_u32();
        toy_assert(value < Mod);
        return value;
    }

    template<class T = u32>
    [[gnu::always_inline]] T read_index(T size) {
        T value = read<T>();
        toy_assert(value < size);
        return value;
    }

    string_view read_token() {
        const char* begin = p;
        while (*p > ' ') ++p;
        string_view token(begin, p);
        ++p;
        return token;
    }

    [[gnu::always_inline]] string_view read_token(usize length) {
        string_view token(p, length);
        p += length;
        toy_assert(*p <= ' ');
        ++p;
        return token;
    }

    [[gnu::always_inline]] void skip_spaces() {
        while (*p <= ' ') ++p;
    }

    template<class T> [[gnu::always_inline]] T read() {
        if constexpr (same_as<T, i128>) return read_i128();
        else if constexpr (same_as<T, u128>) return read_u128();
        else if constexpr (is_signed_v<T> && sizeof(T) <= 4) return (T)read_i32();
        else if constexpr (is_signed_v<T>) return (T)read_i64();
        else if constexpr (sizeof(T) <= 4) return (T)read_u32();
        else return (T)read_u64();
    }
};

template<usize N = 1 << 19, bool Compact = false>
class Writer {
    alignas(64) array<char, N> storage;
    char* begin = storage.data();
    char* p = begin;
    char* end = begin + N;

    static constexpr u32 packed_digits(int x) {
        return (u32)('0' + x / 1000) |
               (u32)('0' + x / 100 % 10) << 8 |
               (u32)('0' + x / 10 % 10) << 16 |
               (u32)('0' + x % 10) << 24;
    }

    static const array<u32, 10000>& lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) result[x] = packed_digits(x);
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& first_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                if (x == 0) value = (value & 0x00ffffffU) | (u32)' ' << 24;
                result[x] = value;
            }
            return result;
        }();
        return a;
    }

    static const array<u32, 10000>& negative_lut() {
        static constexpr auto a = [] {
            array<u32, 10000> result{};
            for (int x = 0; x < 10000; ++x) {
                u32 value = packed_digits(x);
                if (x < 1000) value = (value & 0xffffff00U) | ' ';
                if (x < 100) value = (value & 0xffff00ffU) | (u32)' ' << 8;
                if (x < 10) value = (value & 0xff00ffffU) | (u32)' ' << 16;
                unsigned offset = x >= 100 ? 0 : x >= 10 ? 8 : x ? 16 : 24;
                result[x] = (value & ~(0xffU << offset)) | (u32)'-' << offset;
            }
            return result;
        }();
        return a;
    }

    template<u64 Magic, int Shift>
    [[gnu::always_inline]] static u64 reciprocal_div(u64 x) {
        return (u64)(((u128)x * Magic) >> 64) >> Shift;
    }

    [[gnu::always_inline]] static u64 div_1e4(u64 x) {
        return reciprocal_div<0x346dc5d63886594bULL, 11>(x);
    }

    [[gnu::always_inline]] static u64 div_1e8(u64 x) {
        return reciprocal_div<0xabcc77118461cefdULL, 26>(x);
    }

    [[gnu::always_inline]] static u64 div_1e12(u64 x) {
        return reciprocal_div<0x232f33025bd42233ULL, 37>(x);
    }

    [[gnu::always_inline]] static u64 div_1e16(u64 x) {
        return reciprocal_div<0x39a5652fb1137857ULL, 51>(x);
    }

    [[gnu::always_inline]] void ensure(usize n) {
        if ((usize)(end - p) < n) flush();
    }

    [[gnu::always_inline]] static void group(char*& cursor, u32 s) {
        memcpy(cursor, &s, 4); cursor += 4;
    }

    [[gnu::always_inline]] static void first_group(char*& cursor, u64 x) {
        if constexpr (Compact) {
            unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
            u32 s = lut()[x] >> (skip * 8);
            memcpy(cursor, &s, 4);
            cursor += 4 - skip;
        } else {
            group(cursor, x ? first_lut()[x]
                            : (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
        }
    }

    [[gnu::always_inline]] static void u64_raw(char*& cursor, u64 x) {
        const auto& table = lut();
        if constexpr (!Compact) {
            if (x >= 10'000'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                u64 middle = high % 10000;
                u64 top = high / 10000;
                first_group(cursor, top / 10000);
                group(cursor, table[top % 10000]);
                group(cursor, table[middle]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 1'000'000'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                u64 high = x / 100'000'000ULL;
                first_group(cursor, high / 10000);
                group(cursor, table[high % 10000]);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 100'000'000ULL) {
                u64 low = x % 100'000'000ULL;
                first_group(cursor, x / 100'000'000ULL);
                group(cursor, table[low / 10000]);
                group(cursor, table[low % 10000]);
            } else if (x >= 10000ULL) {
                first_group(cursor, x / 10000);
                group(cursor, table[x % 10000]);
            } else if (x) {
                first_group(cursor, x);
            } else {
                group(cursor, (u32)' ' | (u32)' ' << 8 |
                              (u32)' ' << 16 | (u32)'0' << 24);
            }
        } else if (x > 9999'9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_group(cursor, q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_group(cursor, q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999'9999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_group(cursor, q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if (x > 9999ULL) {
            u64 q1 = div_1e4(x);
            first_group(cursor, q1);
            group(cursor, table[x - q1 * 10000]);
        } else {
            first_group(cursor, x);
        }
    }

    template<u64 Max>
    [[gnu::always_inline]] static void compact_u64_bounded(char*& cursor, u64 x) {
        static_assert(Max <= numeric_limits<u64>::max());
        const auto& table = lut();
        if constexpr (Max <= 9'999ULL) {
            first_group(cursor, x);
        } else if constexpr (Max <= 99'999'999ULL) {
            u64 q1 = div_1e4(x);
            if (q1) first_group(cursor, q1), group(cursor, table[x - q1 * 10000]);
            else first_group(cursor, x);
        } else if constexpr (Max <= 999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else if constexpr (Max <= 9'999'999'999'999'999ULL) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            if (q3) {
                first_group(cursor, q3);
                group(cursor, table[q2 - q3 * 10000]);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q2) {
                first_group(cursor, q2);
                group(cursor, table[q1 - q2 * 10000]);
                group(cursor, table[x - q1 * 10000]);
            } else if (q1) {
                first_group(cursor, q1);
                group(cursor, table[x - q1 * 10000]);
            } else {
                first_group(cursor, x);
            }
        } else {
            u64_raw(cursor, x);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void u64_fixed(char*& cursor, u64 x) {
        static_assert(1 <= Digits && Digits <= 20);
        constexpr unsigned groups = (Digits + 3) / 4;
        constexpr unsigned leading = Digits - 4 * (groups - 1);
        auto first_fixed = [&](u64 value) {
            u32 first = lut()[value] >> ((4 - leading) * 8);
            memcpy(cursor, &first, 4);
            cursor += leading;
        };
        const auto& table = lut();
        if constexpr (groups == 1) {
            first_fixed(x);
        } else if constexpr (groups == 2) {
            u64 q1 = div_1e4(x);
            first_fixed(q1);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 3) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            first_fixed(q2);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else if constexpr (groups == 4) {
            u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
            first_fixed(q3);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        } else {
            u64 q1 = div_1e4(x), q2 = div_1e8(x);
            u64 q3 = div_1e12(x), q4 = div_1e16(x);
            first_fixed(q4);
            group(cursor, table[q3 - q4 * 10000]);
            group(cursor, table[q2 - q3 * 10000]);
            group(cursor, table[q1 - q2 * 10000]);
            group(cursor, table[x - q1 * 10000]);
        }
    }

    template<unsigned Digits>
    [[gnu::always_inline]] static void i128_fixed(char*& cursor, i128 x) {
        static_assert(1 <= Digits && Digits <= 39);
        bool negative = x < 0;
        u128 magnitude = negative ? u128(0) - (u128)x : (u128)x;
        if (negative) *cursor++ = '-';
        if constexpr (Digits <= 20) {
            u64_fixed<Digits>(cursor, (u64)magnitude);
        } else {
            auto [high, low] = detail::divmod_1e19(magnitude);
            u64_fixed<Digits - 19>(cursor, high);
            fixed19(cursor, low);
        }
    }

    [[gnu::always_inline]] static void i64_raw(char*& cursor, u64 x) {
        if constexpr (Compact) {
            *cursor++ = '-';
            u64_raw(cursor, x);
        } else {
            const auto& next = lut();
            const auto& negative = negative_lut();
            if (x >= 10'000'000'000'000'000'000ULL) {
                *cursor++ = '-'; u64_raw(cursor, x);
            } else if (x > 999'9999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                u64 q3 = div_1e12(x), q4 = div_1e16(x);
                group(cursor, negative[q4]);
                group(cursor, next[q3 - q4 * 10000]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x), q3 = div_1e12(x);
                group(cursor, negative[q3]);
                group(cursor, next[q2 - q3 * 10000]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999'9999ULL) {
                u64 q1 = div_1e4(x), q2 = div_1e8(x);
                group(cursor, negative[q2]);
                group(cursor, next[q1 - q2 * 10000]);
                group(cursor, next[x - q1 * 10000]);
            } else if (x > 999ULL) {
                u64 q1 = div_1e4(x);
                group(cursor, negative[q1]);
                group(cursor, next[x - q1 * 10000]);
            } else {
                group(cursor, negative[x]);
            }
        }
    }

    [[gnu::always_inline]] static void fixed19(char*& cursor, u64 x) {
        const auto& next = lut();
        u64 q1 = div_1e4(x), q2 = div_1e8(x);
        u64 q3 = div_1e12(x), q4 = div_1e16(x);
        u32 first = next[q4] >> 8;
        memcpy(cursor, &first, 4); cursor += 3;
        group(cursor, next[q3 - q4 * 10000]);
        group(cursor, next[q2 - q3 * 10000]);
        group(cursor, next[q1 - q2 * 10000]);
        group(cursor, next[x - q1 * 10000]);
    }

public:
    ~Writer() { flush(); }
    void flush() { detail::write_all(begin, p - begin); p = begin; }
    void put(char c) {
        ensure(1);
        *p++ = c;
    }
    void write(string_view s) {
        while (!s.empty()) {
            usize n = min<usize>(s.size(), end - p);
            if (!n) {
                flush();
                continue;
            }
            memcpy(p, s.data(), n);
            p += n;
            s.remove_prefix(n);
        }
    }

    [[gnu::always_inline]] void write_padded_u32(u32 x) {
        ensure(16);
        char* cursor = p;
        *cursor++ = ' ';
        if (x > 99'999'999U) {
            u32 high = x / 100'000'000U;
            u32 middle = x / 10'000U % 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[middle]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x > 9'999U) {
            u32 high = x / 10'000U;
            group(cursor, first_lut()[high]);
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void write_token_u32_6(u32 x) {
        toy_assert(x < 1'000'000U);
        ensure(8);
        char* cursor = p;
        *cursor++ = ' ';
        if (x >= 10'000U) {
            u32 high = x / 10'000U;
            u32 digits = lut()[high];
            if (high >= 10) {
                digits >>= 16;
                memcpy(cursor, &digits, 2);
                cursor += 2;
            } else {
                *cursor++ = (char)(digits >> 24);
            }
            group(cursor, lut()[x % 10'000U]);
        } else if (x) {
            group(cursor, first_lut()[x]);
        } else {
            group(cursor, (u32)' ' | (u32)' ' << 8 |
                          (u32)' ' << 16 | (u32)'0' << 24);
        }
        p = cursor;
    }

    [[gnu::always_inline]] void writeln(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void writeln_i64(i64 x) {
        ensure(24);
        char* cursor = p;
        if (x < 0)
            i64_raw(cursor, u64(0) - (u64)x);
        else
            u64_raw(cursor, (u64)x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        u64_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_fixed<Digits>(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void writeln_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    [[gnu::always_inline]] void write_token(u64 x) {
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        u64_raw(cursor, x);
        p = cursor;
    }
    template<u64 Max>
    [[gnu::always_inline]] void write_token_bounded(u64 x) {
        toy_assert(x <= Max);
        ensure(24);
        char* cursor = p;
        *cursor++ = ' ';
        if constexpr (Compact) compact_u64_bounded<Max>(cursor, x);
        else u64_raw(cursor, x);
        p = cursor;
    }
    [[gnu::always_inline]] void writeln(i128 x) {
        ensure(48); bool neg = x < 0; u128 v = neg ? u128(0) - (u128)x : (u128)x;
        auto [q, r] = detail::divmod_1e19(v);
        char* cursor = p;
        if (neg) {
            if (q) i64_raw(cursor, q), fixed19(cursor, r); else i64_raw(cursor, r);
        } else {
            if (q) u64_raw(cursor, q), fixed19(cursor, r); else u64_raw(cursor, r);
        }
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void writeln_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        i128_fixed<Digits>(cursor, x);
        *cursor++ = '\n';
        p = cursor;
    }
    template<unsigned Digits>
    [[gnu::always_inline]] void write_token_fixed(i128 x) {
        ensure(48);
        char* cursor = p;
        *cursor++ = ' ';
        i128_fixed<Digits>(cursor, x);
        p = cursor;
    }
};

template<usize N = 1 << 19>
using CompactWriter = Writer<N, true>;

} // namespace toy
// END bundled: include/toy/io.hpp

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    static toy::FixedFoldableQueue<Function, toy::ComposeAffine<mod>, 500'000>
        queue(Function{}, toy::ComposeAffine<mod>{});
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            queue.push({a, b});
        } else if (type == 1) {
            queue.pop();
        } else {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(queue.fold()(x));
        }
    }
}
// END bundled: problems/data_structure/queue_operate_all_composite/two_stack_aggregate.cpp
