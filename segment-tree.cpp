#include <vector>
#include <initializer_list>
#include <algorithm>
using std::vector;
using std::initializer_list;
using std::min;
using std::max;

template <typename T>
class segment_tree {
    
private:
    template <typename U>
    struct segment_tree_node {
        size_t from, to;
        U sum, max, min;
    };
    
    vector<segment_tree_node<T>> tree;
    
    T assign(size_t pos, T val) {
        tree[pos].sum = val;
        tree[pos].max = val;
        tree[pos].min = val;
        return val;
    }
    void push_up(size_t pos) {
        tree[pos].sum = tree[pos << 1].sum + tree[pos << 1 | 1].sum;
        tree[pos].max = max(tree[pos << 1].max, tree[pos << 1 | 1].max);
        tree[pos].min = min(tree[pos << 1].min, tree[pos << 1 | 1].min);
    }
    
public:
    segment_tree() {}
    segment_tree(vector<T> &init):
    tree(vector<segment_tree_node<T>>(init.size() << 2)) {
        build(init);
    }
    segment_tree(size_t size):
    tree(vector<segment_tree_node<T>>(size << 2)){
        build(vector<T>(size));
    }
    segment_tree(size_t size, T val):
    tree(vector<segment_tree_node<T>>(size << 2)){
        build(vector<T>(size, val));
    }
    segment_tree(initializer_list<T> &init):
    tree(vector<segment_tree_node<T>>(init.size() << 2)){
        build(vector<T>(init));
    }
    
    void reserve(size_t size) { tree.reserve(size << 2); }
    void resize(size_t size) { tree.resize(size << 2); }
    
    void build(vector<T> &init, size_t from, size_t to, size_t root) {
        resize(init.size());
        tree[root].from = from;
        tree[root].to = to;
        if (from == to) assign(root, init[from]);
        else {
            size_t mid = (from + to) >> 1;
            build(init, from, mid, root << 1);
            build(init, mid + 1, to, root << 1 | 1);
            push_up(root);
        }
    }
    void build(vector<T> &init) { build(init, 0, init.size() - 1, 1); }
    
    T get_value(size_t pos, size_t root) const {
        if (tree[root].from == tree[root].to) return tree[root].sum;
        size_t mid = (tree[root].from + tree[root].to) >> 1;
        if (pos <= mid) return get_value(pos, root << 1);
        else return get_value(pos, root << 1 | 1);
    }
    T get_value(size_t pos) const { return get_value(pos, 1); }
    
    T set_value(size_t pos, T val, size_t root) {
        if (tree[root].from == tree[root].to) {
            assign(root, val);
            return val;
        }
        size_t mid = (tree[root].from + tree[root].to) >> 1;
        T result;
        if (pos <= mid) result = set_value(pos, val, root << 1);
        else result = set_value(pos, val, root << 1 | 1);
        push_up(root);
        return result;
    }
    T set_value(size_t pos, T val) { return set_value(pos, val, 1); }
    
    T add_value(size_t pos, T val = 1) { return set_value(pos, get_value(pos) + val); }
    T sub_value(size_t pos, T val = 1) { return add_value(pos, -val); }
    
    T get_range_sum(size_t from, size_t to, size_t root) const {
        if (from == tree[root].from && to == tree[root].to) return tree[root].sum;
        size_t mid = (tree[root].from + tree[root].to) >> 1;
        if (to <= mid) return get_range_sum(from, to, root << 1);
        else if (from > mid) return get_range_sum(from, to, root << 1 | 1);
        else return get_range_sum(from, mid, root << 1) + get_range_sum(mid + 1, to, root << 1 | 1);
    }
    T get_range_sum(size_t from, size_t to) const { return get_range_sum(from, to, 1); }
    
    T get_range_max(size_t from, size_t to, size_t root) const {
        if (from == tree[root].from && to == tree[root].to) return tree[root].max;
        size_t mid = (tree[root].from + tree[root].to) >> 1;
        if (to <= mid) return get_range_max(from, to, root << 1);
        else if (from > mid) return get_range_max(from, to, root << 1 | 1);
        else return max(get_range_max(from, mid, root << 1), get_range_max(mid + 1, to, root << 1 | 1));
    }
    T get_range_max(size_t from, size_t to) const { return get_range_max(from, to, 1); }
    
    T get_range_min(size_t from, size_t to, size_t root) const {
        if (from == tree[root].from && to == tree[root].to) return tree[root].min;
        size_t mid = (tree[root].from + tree[root].to) >> 1;
        if (to <= mid) return get_range_min(from, to, root << 1);
        else if (from > mid) return get_range_min(from, to, root << 1 | 1);
        else return min(get_range_min(from, mid, root << 1), get_range_min(mid + 1, to, root << 1 | 1));
    }
    T get_range_min(size_t from, size_t to) const { return get_range_min(from, to, 1); }
    
};
