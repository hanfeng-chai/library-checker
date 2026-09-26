

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
// BEGIN bundled: include/toy/sortable.hpp

#include <bits/extc++.h>
// BEGIN bundled: include/toy/ds.hpp

#include <bits/extc++.h>
#include <immintrin.h>

namespace toy {

class DisjointSetUnion {
    std::vector<int> parent_or_size;

public:
    explicit DisjointSetUnion(int n) : parent_or_size(n, -1) {}

    int leader(int x) {
        int root = x;
        while (parent_or_size[root] >= 0) root = parent_or_size[root];
        while (x != root) {
            int parent = parent_or_size[x];
            parent_or_size[x] = root;
            x = parent;
        }
        return root;
    }

    bool merge(int a, int b) {
        a = leader(a);
        b = leader(b);
        if (a == b) return false;
        if (parent_or_size[a] > parent_or_size[b]) std::swap(a, b);
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = a;
        return true;
    }

    bool same(int a, int b) { return leader(a) == leader(b); }
    int size(int x) { return -parent_or_size[leader(x)]; }
};

class RollbackUnionFind {
    struct Change {
        int parent;
        int child;
        int child_size;
    };

    std::vector<int> parent_or_size;
    std::vector<Change> history;

public:
    explicit RollbackUnionFind(int n, std::size_t capacity = 0)
        : parent_or_size(n, -1) {
        history.reserve(capacity);
    }

    int leader(int vertex) const {
        while (parent_or_size[vertex] >= 0)
            vertex = parent_or_size[vertex];
        return vertex;
    }

    bool same(int first, int second) const {
        return leader(first) == leader(second);
    }

    bool merge(int first, int second) {
        first = leader(first);
        second = leader(second);
        if (first == second) {
            history.push_back({-1, -1, 0});
            return false;
        }
        if (parent_or_size[first] > parent_or_size[second])
            std::swap(first, second);
        history.push_back({first, second, parent_or_size[second]});
        parent_or_size[first] += parent_or_size[second];
        parent_or_size[second] = first;
        return true;
    }

    void undo() {
        Change change = history.back();
        history.pop_back();
        if (change.parent < 0) return;
        parent_or_size[change.parent] -= change.child_size;
        parent_or_size[change.child] = change.child_size;
    }

    std::size_t snapshot() const { return history.size(); }

    void rollback(std::size_t state) {
        while (history.size() > state) undo();
    }
};

template<class T>
class FenwickTree {
    std::vector<T> data;

public:
    explicit FenwickTree(int n) : data(n + 1) {}

    template<class Range>
    explicit FenwickTree(const Range& values) : data(values.size() + 1) {
        for (int i = 0; i < (int)values.size(); ++i) data[i + 1] += values[i];
        for (int i = 1; i < (int)data.size(); ++i) {
            int parent = i + (i & -i);
            if (parent < (int)data.size()) data[parent] += data[i];
        }
    }

    void add(int index, T delta) {
        for (++index; index < (int)data.size(); index += index & -index)
            data[index] += delta;
    }

    T prefix_sum(int end) const {
        T result{};
        for (; end; end -= end & -end) result += data[end];
        return result;
    }

    T sum(int left, int right) const {
        return prefix_sum(right) - prefix_sum(left);
    }

    int lower_bound(T target) const {
        if (target <= T{}) return 0;
        int index = 0;
        for (int step = std::bit_floor((unsigned)data.size()); step; step >>= 1) {
            int next = index + step;
            if (next < (int)data.size() && data[next] < target) {
                index = next;
                target -= data[next];
            }
        }
        return index;
    }
};

class FenwickBitset {
    std::vector<uint64_t> bits;
    std::vector<int> block_counts;

    static uint64_t low_bits(unsigned width) {
        return (uint64_t{1} << width) - 1;
    }

    int prefix_blocks(int end) const {
        int result = 0;
        for (; end; end -= end & -end) result += block_counts[end - 1];
        return result;
    }

public:
    explicit FenwickBitset(int size)
        : bits((size + 63) / 64 + 1),
          block_counts((size + 63) / 64) {}

    bool contains(int index) const {
        return bits[index >> 6] >> (index & 63) & 1;
    }

    void set(int index, bool value) {
        int block = index >> 6;
        uint64_t mask = uint64_t{1} << (index & 63);
        if (bool(bits[block] & mask) == value) return;
        bits[block] ^= mask;
        int delta = value ? 1 : -1;
        for (++block; block <= (int)block_counts.size();
             block += block & -block)
            block_counts[block - 1] += delta;
    }

    int count(int left, int right) const {
        int left_block = left >> 6;
        int right_block = right >> 6;
        int result =
            std::popcount(bits[right_block] & low_bits(right & 63)) -
            std::popcount(bits[left_block] & low_bits(left & 63));
        return result + prefix_blocks(right_block) -
               prefix_blocks(left_block);
    }
};

template<class T, std::size_t Capacity>
class FixedCenteredDeque {
    std::array<T, 2 * Capacity + 1> data{};
    std::size_t left = Capacity;
    std::size_t right = Capacity;

public:
    bool empty() const { return left == right; }
    std::size_t size() const { return right - left; }

    void push_front(const T& value) { data[--left] = value; }
    void push_back(const T& value) { data[right++] = value; }
    void pop_front() { ++left; }
    void pop_back() { --right; }

    T& operator[](std::size_t index) { return data[left + index]; }
    const T& operator[](std::size_t index) const {
        return data[left + index];
    }
};

class PredecessorSet {
    std::vector<std::vector<uint64_t>> levels;
    int universe;

    static int next_in_word(const std::vector<uint64_t>& words, int index) {
        int word = index >> 6;
        if (word >= (int)words.size()) return -1;
        uint64_t candidates = words[word] & (~0ULL << (index & 63));
        if (candidates) return word * 64 + std::countr_zero(candidates);
        return -1;
    }

    static int previous_in_word(const std::vector<uint64_t>& words, int index) {
        if (index < 0) return -1;
        int word = std::min<int>(index >> 6, words.size() - 1);
        uint64_t mask = (index & 63) == 63 ? ~0ULL : (1ULL << ((index & 63) + 1)) - 1;
        uint64_t candidates = words[word] & mask;
        if (candidates) return word * 64 + 63 - std::countl_zero(candidates);
        return -1;
    }

public:
    explicit PredecessorSet(int n) : universe(n) {
        for (int size = n; ; size = (size + 63) / 64) {
            levels.emplace_back((size + 63) / 64);
            if (size <= 64) break;
        }
    }

    void assign(std::string_view bits) {
        for (auto& level : levels) std::fill(level.begin(), level.end(), 0);
        int index = 0;
        for (; index + 64 <= universe; index += 64) {
            __m256i ones = _mm256_set1_epi8('1');
            uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
            uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
            levels[0][index / 64] = low | (uint64_t)high << 32;
        }
        for (; index < universe; ++index)
            if (bits[index] == '1') levels[0][index / 64] |= 1ULL << (index % 64);
        for (int level = 1; level < (int)levels.size(); ++level)
            for (int child = 0; child < (int)levels[level - 1].size(); ++child)
                if (levels[level - 1][child])
                    levels[level][child / 64] |= 1ULL << (child % 64);
    }

    bool contains(int x) const {
        return levels[0][x >> 6] >> (x & 63) & 1;
    }

    void insert(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            uint64_t bit = 1ULL << (x & 63);
            if (word & bit) break;
            word |= bit;
            x >>= 6;
        }
    }

    void erase(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            word &= ~(1ULL << (x & 63));
            if (word) break;
            x >>= 6;
        }
    }

    int next(int x) const {
        if (x >= universe) return -1;
        int position = x;
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = next_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + std::countr_zero(word);
                }
                return found < universe ? found : -1;
            }
            position = (position >> 6) + 1;
        }
        return -1;
    }

    int previous(int x) const {
        if (universe == 0 || x < 0) return -1;
        int position = std::min(x, universe - 1);
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = previous_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + 63 - std::countl_zero(word);
                }
                return found;
            }
            position = (position >> 6) - 1;
        }
        return -1;
    }
};

template <std::size_t MaxUniverse>
class BoundedPredecessorSet {
        static_assert(MaxUniverse <= (1ULL << 24));
        static constexpr std::size_t leaf_count = (MaxUniverse + 63) / 64;
        static constexpr std::size_t level1_count = (leaf_count + 63) / 64;
        static constexpr std::size_t level2_count = (level1_count + 63) / 64;

        std::array<uint64_t, leaf_count> leaf{};
        std::array<uint64_t, level1_count> level1{};
        std::array<uint64_t, level2_count> level2{};
        uint64_t root = 0;
        std::size_t assigned_leaves = 0;

    public:
        void assign(std::string_view bits) {
            level1.fill(0);
            level2.fill(0);
            root = 0;
            std::size_t index = 0;
            const __m256i ones = _mm256_set1_epi8('1');
            for (; index + 64 <= bits.size(); index += 64) {
                uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
                uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
                leaf[index / 64] = low | (uint64_t)high << 32;
            }
            if (index < bits.size()) {
                leaf[index / 64] = 0;
                for (; index < bits.size(); ++index)
                    if (bits[index] == '1') leaf[index / 64] |= 1ULL << (index % 64);
            }
            std::size_t used_leaves = (bits.size() + 63) / 64;
            if (used_leaves < assigned_leaves)
                std::fill(leaf.begin() + used_leaves, leaf.begin() + assigned_leaves, 0);
            assigned_leaves = used_leaves;
            for (std::size_t i = 0; i < used_leaves; ++i)
                level1[i / 64] |= uint64_t(leaf[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (leaf_count + 63) / 64; ++i)
                level2[i / 64] |= uint64_t(level1[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (level1_count + 63) / 64; ++i)
                root |= uint64_t(level2[i] != 0) << i;
        }

        void insert(unsigned x) {
            leaf[x >> 6] |= 1ULL << (x & 63);
            level1[x >> 12] |= 1ULL << ((x >> 6) & 63);
            level2[x >> 18] |= 1ULL << ((x >> 12) & 63);
            root |= 1ULL << (x >> 18);
        }

        void erase(unsigned x) {
            if (!(leaf[x >> 6] &= ~(1ULL << (x & 63))))
                if (!(level1[x >> 12] &= ~(1ULL << ((x >> 6) & 63))))
                    if (!(level2[x >> 18] &= ~(1ULL << ((x >> 12) & 63))))
                        root &= ~(1ULL << (x >> 18));
        }

        bool contains(unsigned x) const {
            return leaf[x >> 6] >> (x & 63) & 1;
        }

        int successor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & (~0ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | std::countr_zero(word));
            word = level1[x >> 12] & (-2ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | std::countr_zero(word);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = level2[x >> 18] & (-2ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | std::countr_zero(word);
                answer = answer << 6 | std::countr_zero(level1[answer]);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = root & (-2ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = std::countr_zero(word);
            answer = answer << 6 | std::countr_zero(level2[answer]);
            answer = answer << 6 | std::countr_zero(level1[answer]);
            return int(answer << 6 | std::countr_zero(leaf[answer]));
        }

        int predecessor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & ~(-2ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | (63 - std::countl_zero(word)));
            word = level1[x >> 12] & ~(~0ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | (63 - std::countl_zero(word));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = level2[x >> 18] & ~(~0ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | (63 - std::countl_zero(word));
                answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = root & ~(~0ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = 63 - std::countl_zero(word);
            answer = answer << 6 | (63 - std::countl_zero(level2[answer]));
            answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
            return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
        }
};

} // namespace toy
// END bundled: include/toy/ds.hpp
// BEGIN bundled: include/toy/range.hpp

#include <bits/extc++.h>

namespace toy {

template<class T>
class PrefixSum {
    std::vector<T> prefix;

public:
    template<class Range>
    explicit PrefixSum(const Range& values) : prefix(values.size() + 1) {
        std::partial_sum(values.begin(), values.end(), prefix.begin() + 1);
    }

    T sum(int left, int right) const { return prefix[right] - prefix[left]; }
};

template<class T, class Operation>
class SegmentTree {
    int size;
    T identity;
    Operation operation;
    std::vector<T> data;

public:
    template<class Range>
    SegmentTree(const Range& values, T identity_value, Operation combine = {})
        : size(std::bit_ceil((unsigned)values.size())), identity(identity_value),
          operation(combine), data(2 * size, identity_value) {
        std::copy(values.begin(), values.end(), data.begin() + size);
        for (int i = size - 1; i; --i) data[i] = operation(data[2 * i], data[2 * i + 1]);
    }

    void set(int index, T value) {
        data[index += size] = value;
        while (index >>= 1) data[index] = operation(data[2 * index], data[2 * index + 1]);
    }

    T get(int index) const { return data[size + index]; }

    T fold(int left, int right) const {
        T lhs = identity, rhs = identity;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) lhs = operation(lhs, data[left++]);
            if (right & 1) rhs = operation(data[--right], rhs);
        }
        return operation(lhs, rhs);
    }
};

template<class T, class Operation>
class SparseTable {
    Operation operation;
    std::vector<std::vector<T>> table;

public:
    template<class Range>
    explicit SparseTable(const Range& values, Operation combine = {})
        : operation(combine), table(std::bit_width(values.size())) {
        table[0].assign(values.begin(), values.end());
        for (int level = 1; level < (int)table.size(); ++level) {
            int half = 1 << (level - 1);
            int count = values.size() - (1 << level) + 1;
            table[level].resize(std::max(count, 0));
            for (int i = 0; i < count; ++i)
                table[level][i] = operation(table[level - 1][i],
                                            table[level - 1][i + half]);
        }
    }

    T fold_idempotent(int left, int right) const {
        int level = std::bit_width((unsigned)(right - left)) - 1;
        return operation(table[level][left], table[level][right - (1 << level)]);
    }
};

class RangeAddMinTree {
    static constexpr int64_t infinity =
        4'000'000'000'000'000'000LL;

    struct PrefixMinimum {
        int64_t sum = 0;
        int64_t minimum = infinity;
    };

    int length;
    int size;
    std::vector<int64_t> difference;
    std::vector<PrefixMinimum> data;

    static PrefixMinimum combine(
        const PrefixMinimum& left, const PrefixMinimum& right) {
        return {
            left.sum + right.sum,
            std::min(left.minimum, left.sum + right.minimum)};
    }

    void set(int index) {
        int node = size + index;
        data[node] = {difference[index], difference[index]};
        while (node >>= 1)
            data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    PrefixMinimum fold(int left, int right) const {
        uint32_t lower = left + size - 1;
        uint32_t upper = right + size;
        int width = std::bit_width(lower ^ upper) - 1;
        uint32_t mask = (uint32_t(1) << width) - 1;
        PrefixMinimum result;

        uint32_t bits = ~lower & mask;
        while (bits != 0) {
            int level = std::countr_zero(bits);
            bits ^= uint32_t(1) << level;
            result = combine(
                result, data[(lower >> level) ^ 1]);
        }
        bits = upper & mask;
        while (bits != 0) {
            int level = std::bit_width(bits) - 1;
            bits ^= uint32_t(1) << level;
            result = combine(
                result, data[(upper >> level) ^ 1]);
        }
        return result;
    }

    int64_t prefix_sum(int end) const {
        int64_t result = 0;
        for (uint32_t node = size + end; node > 1; node >>= 1)
            if (node & 1) result += data[node - 1].sum;
        return result;
    }

public:
    explicit RangeAddMinTree(const std::vector<int64_t>& values)
        : length(values.size()),
          size(std::bit_ceil((unsigned)values.size())),
          difference(values.size()), data(2 * size) {
        int64_t previous = 0;
        for (int i = 0; i < length; ++i) {
            difference[i] = values[i] - previous;
            previous = values[i];
            data[size + i] = {difference[i], difference[i]};
        }
        for (int node = size - 1; node; --node)
            data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    void add(int left, int right, int64_t value) {
        difference[left] += value;
        set(left);
        if (right < length) {
            difference[right] -= value;
            set(right);
        }
    }

    int64_t minimum_of(int left, int right) const {
        return prefix_sum(left) + fold(left, right).minimum;
    }
};

class RecursiveRangeClampAddSumTree {
    static constexpr int64_t infinity = std::numeric_limits<int64_t>::max();
    struct Node {
        int64_t maximum = -infinity, second_maximum = -infinity;
        int64_t minimum = infinity, second_minimum = infinity;
        int64_t sum = 0, lazy_add = 0;
        int maximum_count = 0, minimum_count = 0;
    };

    int size;
    std::vector<Node> data;

    void pull(int node) {
        const Node& left = data[node * 2];
        const Node& right = data[node * 2 + 1];
        Node& current = data[node];
        current.sum = left.sum + right.sum;
        current.maximum = std::max(left.maximum, right.maximum);
        current.maximum_count =
            (left.maximum == current.maximum ? left.maximum_count : 0) +
            (right.maximum == current.maximum ? right.maximum_count : 0);
        current.second_maximum =
            std::max(left.maximum == current.maximum ? left.second_maximum : left.maximum,
                     right.maximum == current.maximum ? right.second_maximum : right.maximum);
        current.minimum = std::min(left.minimum, right.minimum);
        current.minimum_count =
            (left.minimum == current.minimum ? left.minimum_count : 0) +
            (right.minimum == current.minimum ? right.minimum_count : 0);
        current.second_minimum =
            std::min(left.minimum == current.minimum ? left.second_minimum : left.minimum,
                     right.minimum == current.minimum ? right.second_minimum : right.minimum);
        current.lazy_add = 0;
    }
    void apply_add(int node, int length, int64_t value) {
        Node& current = data[node];
        current.sum += value * length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity) current.second_maximum += value;
        if (current.second_minimum != infinity) current.second_minimum += value;
        current.lazy_add += value;
    }
    void apply_chmin(int node, int64_t value) {
        Node& current = data[node];
        current.sum += (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum) current.minimum = value;
        else if (current.second_minimum == current.maximum) current.second_minimum = value;
        current.maximum = value;
    }
    void apply_chmax(int node, int64_t value) {
        Node& current = data[node];
        current.sum += (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum) current.maximum = value;
        else if (current.second_maximum == current.minimum) current.second_maximum = value;
        current.minimum = value;
    }
    void push(int node, int left_length, int right_length) {
        Node& current = data[node];
        if (current.lazy_add) {
            apply_add(node * 2, left_length, current.lazy_add);
            apply_add(node * 2 + 1, right_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum)
            apply_chmin(node * 2, current.maximum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2].minimum < current.minimum)
            apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }
    void build(int node, int left, int right, const std::vector<int64_t>& values) {
        if (right - left == 1) {
            int64_t value = values[left];
            data[node] = {value, -infinity, value, infinity, value, 0, 1, 1};
            return;
        }
        int middle = (left + right) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        pull(node);
    }
    void chmin(int node, int left, int right, int query_left, int query_right,
               int64_t value) {
        if (query_right <= left || right <= query_left || data[node].maximum <= value)
            return;
        if (query_left <= left && right <= query_right &&
            data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmin(node * 2, left, middle, query_left, query_right, value);
        chmin(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void chmax(int node, int left, int right, int query_left, int query_right,
               int64_t value) {
        if (query_right <= left || right <= query_left || value <= data[node].minimum)
            return;
        if (query_left <= left && right <= query_right &&
            value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmax(node * 2, left, middle, query_left, query_right, value);
        chmax(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void add(int node, int left, int right, int query_left, int query_right,
             int64_t value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_add(node, right - left, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        add(node * 2, left, middle, query_left, query_right, value);
        add(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    int64_t sum(int node, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return 0;
        if (query_left <= left && right <= query_right) return data[node].sum;
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        return sum(node * 2, left, middle, query_left, query_right) +
               sum(node * 2 + 1, middle, right, query_left, query_right);
    }

public:
    explicit RecursiveRangeClampAddSumTree(
        const std::vector<int64_t>& values)
        : size(values.size()), data(values.size() * 4) {
        build(1, 0, size, values);
    }
    void chmin(int left, int right, int64_t value) {
        chmin(1, 0, size, left, right, value);
    }
    void chmax(int left, int right, int64_t value) {
        chmax(1, 0, size, left, right, value);
    }
    void add(int left, int right, int64_t value) {
        add(1, 0, size, left, right, value);
    }
    int64_t sum(int left, int right) {
        return sum(1, 0, size, left, right);
    }
};

class RangeClampAddSumTree {
    static constexpr int64_t infinity =
        std::numeric_limits<int64_t>::max();

    struct Node {
        int64_t maximum = -infinity;
        int64_t second_maximum = -infinity;
        int64_t minimum = infinity;
        int64_t second_minimum = infinity;
        int64_t sum = 0;
        int64_t lazy_add = 0;
        uint32_t maximum_count = 0;
        uint32_t minimum_count = 0;
    };

    int length;
    uint32_t capacity;
    uint32_t depth;
    std::vector<Node> data;

    void pull(uint32_t node) {
        const Node& left = data[node * 2];
        const Node& right = data[node * 2 + 1];
        Node& current = data[node];
        current.sum = left.sum + right.sum;
        if (left.maximum == right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum =
                std::max(left.second_maximum, right.second_maximum);
            current.maximum_count =
                left.maximum_count + right.maximum_count;
        } else if (left.maximum > right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum =
                std::max(left.second_maximum, right.maximum);
            current.maximum_count = left.maximum_count;
        } else {
            current.maximum = right.maximum;
            current.second_maximum =
                std::max(left.maximum, right.second_maximum);
            current.maximum_count = right.maximum_count;
        }
        if (left.minimum == right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum =
                std::min(left.second_minimum, right.second_minimum);
            current.minimum_count =
                left.minimum_count + right.minimum_count;
        } else if (left.minimum < right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum =
                std::min(left.second_minimum, right.minimum);
            current.minimum_count = left.minimum_count;
        } else {
            current.minimum = right.minimum;
            current.second_minimum =
                std::min(left.minimum, right.second_minimum);
            current.minimum_count = right.minimum_count;
        }
        current.lazy_add = 0;
    }

    void apply_add(uint32_t node, uint32_t node_length, int64_t value) {
        Node& current = data[node];
        current.sum += value * node_length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity)
            current.second_maximum += value;
        if (current.second_minimum != infinity)
            current.second_minimum += value;
        current.lazy_add += value;
    }

    void apply_chmin(uint32_t node, int64_t value) {
        Node& current = data[node];
        current.sum +=
            (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum)
            current.minimum = value;
        else if (current.second_minimum == current.maximum)
            current.second_minimum = value;
        current.maximum = value;
    }

    void apply_chmax(uint32_t node, int64_t value) {
        Node& current = data[node];
        current.sum +=
            (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum)
            current.maximum = value;
        else if (current.second_maximum == current.minimum)
            current.second_maximum = value;
        current.minimum = value;
    }

    void push(uint32_t node, uint32_t node_length) {
        Node& current = data[node];
        uint32_t child_length = node_length >> 1;
        if (current.lazy_add != 0) {
            apply_add(node * 2, child_length, current.lazy_add);
            apply_add(node * 2 + 1, child_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum)
            apply_chmin(node * 2, current.maximum);
        if (data[node * 2].minimum < current.minimum)
            apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }

    void apply_chmin_subtree(
        uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].maximum <= value) return;
        if (data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        push(node, node_length);
        apply_chmin_subtree(node * 2, node_length >> 1, value);
        apply_chmin_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    void apply_chmax_subtree(
        uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].minimum >= value) return;
        if (value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        push(node, node_length);
        apply_chmax_subtree(node * 2, node_length >> 1, value);
        apply_chmax_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    template<class Apply>
    void apply_range(int left, int right, Apply apply) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            apply(lower, 1);
            while (lower >>= 1) pull(lower);
            return;
        }

        uint32_t split_level =
            std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }

        apply(lower, 1);
        apply(upper, 1);
        node_length = 1;
        while ((lower >> 1) < (upper >> 1)) {
            if ((lower & 1) == 0)
                apply(lower + 1, node_length);
            pull(lower >>= 1);
            if (upper & 1)
                apply(upper - 1, node_length);
            pull(upper >>= 1);
            node_length <<= 1;
        }
        while (lower >>= 1) pull(lower);
    }

    void push_boundary_paths(uint32_t lower, uint32_t upper) {
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            return;
        }
        uint32_t split_level =
            std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }
    }

public:
    explicit RangeClampAddSumTree(
        const std::vector<int64_t>& values)
        : length(values.size()),
          capacity(std::bit_ceil((uint32_t)values.size())),
          depth(std::bit_width(capacity) - 1),
          data(2 * capacity) {
        for (uint32_t i = 0; i < values.size(); ++i) {
            int64_t value = values[i];
            data[capacity + i] = {
                value, -infinity, value, infinity,
                value, 0, 1, 1};
        }
        for (uint32_t node = capacity - 1; node != 0; --node)
            pull(node);
    }

    void chmin(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_chmin_subtree(node, node_length, value);
            });
    }

    void chmax(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_chmax_subtree(node, node_length, value);
            });
    }

    void add(int left, int right, int64_t value) {
        apply_range(
            left, right,
            [&](uint32_t node, uint32_t node_length) {
                apply_add(node, node_length, value);
            });
    }

    int64_t sum(int left, int right) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        push_boundary_paths(lower, upper);
        int64_t left_sum = data[lower].sum;
        if (lower == upper) return left_sum;
        int64_t right_sum = data[upper].sum;
        while ((lower >> 1) != (upper >> 1)) {
            if ((lower & 1) == 0)
                left_sum += data[lower + 1].sum;
            if (upper & 1)
                right_sum += data[upper - 1].sum;
            lower >>= 1;
            upper >>= 1;
        }
        return left_sum + right_sum;
    }
};

} // namespace toy
// END bundled: include/toy/range.hpp

namespace toy {

template<class T, class Operation>
class SortableSegmentTree {
    static constexpr int pool_capacity = 4'000'000;
    struct Node {
        T forward;
        T backward;
        int count;
        int left;
        int right;
    };

    int length;
    int key_limit;
    T identity;
    Operation operation;
    std::vector<Node> pool;
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;

    int make_node(T value) {
        pool.push_back({value, value, 1, 0, 0});
        return pool.size() - 1;
    }

    int make_singleton(int key, T value) {
        int left = 0;
        int right = key_limit;
        std::array<bool, 32> direction;
        int depth = 0;
        while (right - left > 1) {
            int middle = (left + right) / 2;
            if (key < middle) {
                direction[depth++] = false;
                right = middle;
            } else {
                direction[depth++] = true;
                left = middle;
            }
        }
        int node = make_node(value);
        while (depth--) {
            int parent = make_node(value);
            if (direction[depth])
                pool[parent].right = node;
            else
                pool[parent].left = node;
            node = parent;
        }
        return node;
    }

    void pull(int index) {
        Node& node = pool[index];
        if (!node.left && !node.right) return;
        if (!node.left) {
            node.forward = pool[node.right].forward;
            node.backward = pool[node.right].backward;
            node.count = pool[node.right].count;
        } else if (!node.right) {
            node.forward = pool[node.left].forward;
            node.backward = pool[node.left].backward;
            node.count = pool[node.left].count;
        } else {
            node.forward =
                operation(pool[node.left].forward, pool[node.right].forward);
            node.backward =
                operation(pool[node.right].backward, pool[node.left].backward);
            node.count = pool[node.left].count + pool[node.right].count;
        }
    }

    void insert_key(int node, int left, int right, int key, T value) {
        if (right - left == 1) {
            pool[node].forward = pool[node].backward = value;
            return;
        }
        int middle = (left + right) / 2;
        if (key < middle) {
            if (!pool[node].left)
                pool[node].left = make_node(identity);
            insert_key(pool[node].left, left, middle, key, value);
        } else {
            if (!pool[node].right)
                pool[node].right = make_node(identity);
            insert_key(pool[node].right, middle, right, key, value);
        }
        pull(node);
    }

    std::pair<int, int> split(int node, int count) {
        if (!count) return {0, node};
        if (count == pool[node].count) return {node, 0};
        int left_count =
            pool[node].left ? pool[pool[node].left].count : 0;
        int suffix = make_node(identity);
        if (count <= left_count) {
            auto [first, second] = split(pool[node].left, count);
            pool[suffix].left = second;
            pool[suffix].right = pool[node].right;
            pool[node].left = first;
            pool[node].right = 0;
        } else {
            auto [first, second] =
                split(pool[node].right, count - left_count);
            pool[node].right = first;
            pool[suffix].right = second;
        }
        pull(node);
        pull(suffix);
        return {node, suffix};
    }

    int merge(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        pool[first].left = merge(pool[first].left, pool[second].left);
        pool[first].right = merge(pool[first].right, pool[second].right);
        pull(first);
        return first;
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward
                               : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] = split(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] = split(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void initialize(const std::vector<int>& keys, const std::vector<T>& values) {
        boundaries.assign(std::string(length + 1, '1'));
        reversed.assign(length, false);
        roots.assign(length, 0);
        std::vector<T> leaves(length, identity);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_singleton(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(
            leaves, identity, operation);
    }

    void rebuild() {
        std::vector<int> keys;
        std::vector<T> values;
        keys.reserve(length);
        values.reserve(length);
        auto visit = [&](auto&& self, int node, int left, int right,
                         bool reverse) -> void {
            if (!node) return;
            if (right - left == 1) {
                keys.push_back(left);
                values.push_back(pool[node].forward);
                return;
            }
            int middle = (left + right) / 2;
            if (!reverse) {
                self(self, pool[node].left, left, middle, reverse);
                self(self, pool[node].right, middle, right, reverse);
            } else {
                self(self, pool[node].right, middle, right, reverse);
                self(self, pool[node].left, left, middle, reverse);
            }
        };
        for (int begin = boundaries.next(0); begin < length;
             begin = boundaries.next(begin + 1))
            visit(visit, roots[begin], 0, key_limit, reversed[begin]);
        pool.clear();
        pool.push_back({});
        initialize(keys, values);
    }

    void ensure_capacity() {
        if (pool.size() > pool_capacity * 9 / 10) rebuild();
    }

public:
    SortableSegmentTree(int key_count, const std::vector<int>& keys,
                        const std::vector<T>& values, T identity_value,
                        Operation combine = {})
        : length(keys.size()), key_limit(key_count), identity(identity_value),
          operation(combine), boundaries(length + 1) {
        pool.reserve(pool_capacity);
        pool.push_back({});
        initialize(keys, values);
    }

    void set(int index, int key, T value) {
        ensure_capacity();
        split_at(index);
        split_at(index + 1);
        reversed[index] = false;
        roots[index] = make_singleton(key, value);
        aggregate->set(index, value);
    }
    T fold(int left, int right) {
        ensure_capacity();
        split_at(left);
        split_at(right);
        return aggregate->fold(left, right);
    }
    void sort_ascending(int left, int right) {
        ensure_capacity();
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = merge(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }
    void sort_descending(int left, int right) {
        sort_ascending(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

template<class T, class Operation>
class TreapSortableSegmentTree {
    struct Node {
        T value;
        T forward;
        T backward;
        uint32_t priority;
        int key;
        int minimum_key;
        int maximum_key;
        int count;
        int left;
        int right;
    };

    int length;
    T identity;
    Operation operation;
    std::vector<Node> pool{{}};
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;
    uint64_t random_state = 0x243f6a8885a308d3ULL;

    uint32_t random() {
        random_state += 0x9e3779b97f4a7c15ULL;
        uint64_t value = random_state;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }

    int count(int node) const { return node ? pool[node].count : 0; }
    T forward(int node) const {
        return node ? pool[node].forward : identity;
    }
    T backward(int node) const {
        return node ? pool[node].backward : identity;
    }

    int make_node(int key, T value) {
        pool.push_back(
            {value, value, value, random(), key, key, key, 1, 0, 0});
        return pool.size() - 1;
    }

    void pull(int index) {
        Node& node = pool[index];
        node.count = 1 + count(node.left) + count(node.right);
        node.minimum_key =
            node.left ? pool[node.left].minimum_key : node.key;
        node.maximum_key =
            node.right ? pool[node.right].maximum_key : node.key;
        node.forward =
            operation(operation(forward(node.left), node.value),
                      forward(node.right));
        node.backward =
            operation(operation(backward(node.right), node.value),
                      backward(node.left));
    }

    std::pair<int, int> split_rank(int node, int prefix_count) {
        if (!prefix_count) return {0, node};
        if (prefix_count == count(node)) return {node, 0};
        int left_count = count(pool[node].left);
        if (prefix_count <= left_count) {
            auto [first, second] =
                split_rank(pool[node].left, prefix_count);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_rank(
            pool[node].right, prefix_count - left_count - 1);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    std::pair<int, int> split_key(int node, int key) {
        if (!node) return {0, 0};
        if (key <= pool[node].minimum_key) return {0, node};
        if (pool[node].maximum_key < key) return {node, 0};
        if (key <= pool[node].key) {
            auto [first, second] = split_key(pool[node].left, key);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_key(pool[node].right, key);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    int meld(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        if (pool[first].priority < pool[second].priority)
            std::swap(first, second);
        auto [left, right] = split_key(second, pool[first].key);
        pool[first].left = meld(pool[first].left, left);
        pool[first].right = meld(pool[first].right, right);
        pull(first);
        return first;
    }

    T fold_rank(int node, int left, int right, bool reverse) {
        if (left == 0 && right == pool[node].count)
            return reverse ? pool[node].backward : pool[node].forward;

        int first_child =
            reverse ? pool[node].right : pool[node].left;
        int second_child =
            reverse ? pool[node].left : pool[node].right;
        int first_count = count(first_child);
        T result = identity;
        if (left < first_count)
            result = operation(
                result,
                fold_rank(
                    first_child, left, std::min(right, first_count),
                    reverse));
        if (left <= first_count && first_count < right)
            result = operation(result, pool[node].value);
        if (right > first_count + 1)
            result = operation(
                result,
                fold_rank(
                    second_child,
                    std::max(0, left - first_count - 1),
                    right - first_count - 1, reverse));
        return result;
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward
                               : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] =
                split_rank(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] =
                split_rank(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void sort_ascending_impl(int left, int right) {
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = meld(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }

public:
    TreapSortableSegmentTree(const std::vector<int>& keys,
                             const std::vector<T>& values,
                             T identity_value, Operation combine = {})
        : length(keys.size()), identity(identity_value), operation(combine),
          boundaries(length + 1), reversed(length), roots(length) {
        pool.reserve(2 * length + 1);
        boundaries.assign(std::string(length + 1, '1'));
        std::vector<T> leaves(length);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_node(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(
            leaves, identity, operation);
    }

    void set(int index, int key, T value) {
        split_at(index);
        split_at(index + 1);
        roots[index] = make_node(key, value);
        reversed[index] = false;
        aggregate->set(index, value);
    }

    T fold(int left, int right) {
        int first = boundaries.previous(left);
        int last = boundaries.previous(right - 1);
        if (first == last)
            return fold_rank(
                roots[first], left - first, right - first,
                reversed[first]);

        T result = fold_rank(
            roots[first], left - first, pool[roots[first]].count,
            reversed[first]);
        int middle_left = boundaries.next(first + 1);
        result = operation(
            result, aggregate->fold(middle_left, last));
        return operation(
            result,
            fold_rank(
                roots[last], 0, right - last, reversed[last]));
    }

    void sort_ascending(int left, int right) {
        sort_ascending_impl(left, right);
    }

    void sort_descending(int left, int right) {
        sort_ascending_impl(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

template<class T, class Operation>
class PatriciaSortableSegmentTree {
    struct Node {
        T forward;
        T backward;
        uint32_t key;
        int bit;
        int count;
        int left;
        int right;
    };

    int length;
    T identity;
    Operation operation;
    std::vector<Node> pool{{}};
    std::vector<int> free_nodes;
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;

    int allocate(Node node) {
        if (free_nodes.empty()) {
            pool.push_back(node);
            return pool.size() - 1;
        }
        int index = free_nodes.back();
        free_nodes.pop_back();
        pool[index] = node;
        return index;
    }

    void release(int node) { free_nodes.push_back(node); }

    int make_leaf(uint32_t key, T value) {
        return allocate({value, value, key, -1, 1, 0, 0});
    }

    int count(int node) const { return node ? pool[node].count : 0; }

    void pull(int node) {
        Node& current = pool[node];
        current.key = pool[current.left].key;
        current.count =
            pool[current.left].count + pool[current.right].count;
        current.forward = operation(
            pool[current.left].forward,
            pool[current.right].forward);
        current.backward = operation(
            pool[current.right].backward,
            pool[current.left].backward);
    }

    int make_branch(int bit, int first, int second) {
        int left;
        int right;
        if (pool[first].key >> bit & 1)
            left = second, right = first;
        else
            left = first, right = second;
        int node = allocate(
            {identity, identity, pool[left].key, bit, 0, left, right});
        pull(node);
        return node;
    }

    static int highest_bit(uint32_t value) {
        return std::bit_width(value) - 1;
    }

    int meld(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        int difference =
            highest_bit(pool[first].key ^ pool[second].key);
        int first_bit = pool[first].bit;
        int second_bit = pool[second].bit;
        if (difference > std::max(first_bit, second_bit))
            return make_branch(difference, first, second);

        if (first_bit == second_bit) {
            pool[first].left =
                meld(pool[first].left, pool[second].left);
            pool[first].right =
                meld(pool[first].right, pool[second].right);
            release(second);
        } else if (first_bit > second_bit) {
            if (pool[second].key >> first_bit & 1)
                pool[first].right = meld(pool[first].right, second);
            else
                pool[first].left = meld(pool[first].left, second);
        } else {
            if (pool[first].key >> second_bit & 1)
                pool[second].right = meld(first, pool[second].right);
            else
                pool[second].left = meld(first, pool[second].left);
            first = second;
        }
        pull(first);
        return first;
    }

    std::pair<int, int> split_rank(int node, int prefix_count) {
        if (!prefix_count) return {0, node};
        if (prefix_count == pool[node].count) return {node, 0};
        int left_count = pool[pool[node].left].count;
        if (prefix_count == left_count) {
            int left = pool[node].left;
            int right = pool[node].right;
            release(node);
            return {left, right};
        }
        if (prefix_count < left_count) {
            auto [first, second] =
                split_rank(pool[node].left, prefix_count);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_rank(
            pool[node].right, prefix_count - left_count);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    T fold_rank(int node, int left, int right, bool reverse) {
        if (left == 0 && right == pool[node].count)
            return reverse ? pool[node].backward : pool[node].forward;
        int first_child =
            reverse ? pool[node].right : pool[node].left;
        int second_child =
            reverse ? pool[node].left : pool[node].right;
        int first_count = pool[first_child].count;
        if (right <= first_count)
            return fold_rank(
                first_child, left, right, reverse);
        if (first_count <= left)
            return fold_rank(
                second_child, left - first_count,
                right - first_count, reverse);
        return operation(
            fold_rank(
                first_child, left, first_count, reverse),
            fold_rank(
                second_child, 0, right - first_count, reverse));
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward
                               : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] =
                split_rank(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] =
                split_rank(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void sort_ascending_impl(int left, int right) {
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = meld(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }

public:
    PatriciaSortableSegmentTree(
        const std::vector<uint32_t>& keys,
        const std::vector<T>& values,
        T identity_value, Operation combine = {})
        : length(keys.size()), identity(identity_value), operation(combine),
          boundaries(length + 1), reversed(length), roots(length) {
        pool.reserve(2 * length + 1);
        free_nodes.reserve(length);
        boundaries.assign(std::string(length + 1, '1'));
        std::vector<T> leaves(length);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_leaf(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(
            leaves, identity, operation);
    }

    void set(int index, uint32_t key, T value) {
        split_at(index);
        split_at(index + 1);
        release(roots[index]);
        roots[index] = make_leaf(key, value);
        reversed[index] = false;
        aggregate->set(index, value);
    }

    T fold(int left, int right) {
        int first = boundaries.previous(left);
        int last = boundaries.previous(right - 1);
        if (first == last)
            return fold_rank(
                roots[first], left - first, right - first,
                reversed[first]);
        T result = fold_rank(
            roots[first], left - first, pool[roots[first]].count,
            reversed[first]);
        int middle_left = boundaries.next(first + 1);
        result = operation(
            result, aggregate->fold(middle_left, last));
        return operation(
            result,
            fold_rank(
                roots[last], 0, right - last, reversed[last]));
    }

    void sort_ascending(int left, int right) {
        sort_ascending_impl(left, right);
    }

    void sort_descending(int left, int right) {
        sort_ascending_impl(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

} // namespace toy
// END bundled: include/toy/sortable.hpp

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> keys(n);
    std::vector<Function> functions(n);
    for (int i = 0; i < n; ++i) {
        keys[i] = input.read_uniform<10, toy::u32>();
        functions[i].a = input.read_uniform<9, toy::u32>();
        functions[i].b = input.read_uniform<9, toy::u32>();
    }

    toy::PatriciaSortableSegmentTree tree(
        keys, functions, Function{}, toy::ComposeAffine<mod>{});
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<10, toy::u32>();
        if (type == 0) {
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            tree.set(first, second, Function{a, b});
        } else if (type == 1) {
            toy::u32 value = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(
                tree.fold(first, second)(value));
        } else if (type == 2) {
            tree.sort_ascending(first, second);
        } else {
            tree.sort_descending(first, second);
        }
    }
}

