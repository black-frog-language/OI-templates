#include <vector>
#include <initializer_list>
#include <algorithm>
#include <memory>
using std::vector;
using std::initializer_list;
using std::min;
using std::max;
using std::shared_ptr;
using std::make_shared;

// get: get_value; get_range_sum, get_range_min, get_range_max;
// set: set_value, add_value, sub_value, and_value, or_value, not_value; ;
// build: segment_tree, operator=, build
template <typename T>
class segment_value_tree {
    
private:
    template <typename U>
    struct node {
        size_t from, to;
        U sum, max, min;
    };
    
    shared_ptr<vector<node<T>>> tree = make_shared<vector<node<T>>>();
    
    T assign(size_t pos, T val) {
        (*tree)[pos].sum = val;
        (*tree)[pos].max = val;
        (*tree)[pos].min = val;
        return val;
    }
    void push_up(size_t pos) {
        (*tree)[pos].sum = (*tree)[pos << 1].sum + (*tree)[pos << 1 | 1].sum;
        (*tree)[pos].max = max((*tree)[pos << 1].max, (*tree)[pos << 1 | 1].max);
        (*tree)[pos].min = min((*tree)[pos << 1].min, (*tree)[pos << 1 | 1].min);
    }
    
public:
    segment_value_tree() {}
    segment_value_tree(segment_value_tree<T> &init) {
        build(init);
    }
    segment_value_tree(vector<T> &init) {
        build(init);
    }
    segment_value_tree(size_t size) {
        build(size);
    }
    segment_value_tree(size_t size, T val) {
        build(size, val);
    }
    segment_value_tree(initializer_list<T> &init) {
        build(*make_shared<vector<T>>(init));
    }
    
    void build(segment_value_tree<T> &init) { tree = make_shared<vector<T>>(init.tree); }
    void build(vector<T> &init, size_t from, size_t to, size_t root) {
        tree->resize(init.size() * 4);
        (*tree)[root].from = from;
        (*tree)[root].to = to;
        if (from == to) assign(root, init[from]);
        else {
            size_t mid = (from + to) >> 1;
            build(init, from, mid, root << 1);
            build(init, mid + 1, to, root << 1 | 1);
            push_up(root);
        }
    }
    void build(vector<T> &init) { build(init, 0, init.size() - 1, 1); }
    void build(size_t size, T val) { build(*make_shared<vector<T>>(size, val)); }
    void build(size_t size) { build(size, 0); }
    
    void operator=(segment_value_tree<T> &init) { build(init); }
    void operator=(vector<T> &init) { build(init); }
    
    T get_value(size_t pos, size_t root) const {
        if ((*tree)[root].from == (*tree)[root].to) return (*tree)[root].sum;
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (pos <= mid) return get_value(pos, root << 1);
        else return get_value(pos, root << 1 | 1);
    }
    T get_value(size_t pos) const { return get_value(pos, 1); }
    
    T set_value(size_t pos, T val, size_t root) {
        if ((*tree)[root].from == (*tree)[root].to) {
            assign(root, val);
            return val;
        }
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
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
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].sum;
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_sum(from, to, root << 1);
        else if (from > mid) return get_range_sum(from, to, root << 1 | 1);
        else return get_range_sum(from, mid, root << 1) + get_range_sum(mid + 1, to, root << 1 | 1);
    }
    T get_range_sum(size_t from, size_t to) const { return get_range_sum(from, to, 1); }
    
    T get_range_max(size_t from, size_t to, size_t root) const {
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].max;
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_max(from, to, root << 1);
        else if (from > mid) return get_range_max(from, to, root << 1 | 1);
        else return max(get_range_max(from, mid, root << 1), get_range_max(mid + 1, to, root << 1 | 1));
    }
    T get_range_max(size_t from, size_t to) const { return get_range_max(from, to, 1); }
    
    T get_range_min(size_t from, size_t to, size_t root) const {
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].min;
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_min(from, to, root << 1);
        else if (from > mid) return get_range_min(from, to, root << 1 | 1);
        else return min(get_range_min(from, mid, root << 1), get_range_min(mid + 1, to, root << 1 | 1));
    }
    T get_range_min(size_t from, size_t to) const { return get_range_min(from, to, 1); }
    
};



#include <vector>
#include <initializer_list>
#include <algorithm>
#include <memory>
using std::vector;
using std::initializer_list;
using std::min;
using std::max;
using std::shared_ptr;
using std::make_shared;

// get: get_value; get_range_sum;
// set: set_value, add_value, sub_value, and_value, or_value, not_value; add_range, sub_range;
// build: segment_sum_tree, operator=, build;
template <typename T>
class segment_sum_tree {
    
private:
    template <typename U>
    struct node {
        size_t from, to;
        U sum, lazy;
    };
    
    shared_ptr<vector<node<T>>> tree = make_shared<vector<node<T>>>();
    
    T assign(size_t pos, T val) {
        (*tree)[pos].sum = val;
        return val;
    }
    void push_up(size_t pos) {
        (*tree)[pos].sum = (*tree)[pos << 1].sum + (*tree)[pos << 1 | 1].sum;
    }
    void push_down(size_t pos) {
        if ((*tree)[pos].lazy) {
            (*tree)[pos << 1].lazy += (*tree)[pos].lazy;
            (*tree)[pos << 1].sum += ((*tree)[pos << 1].to - (*tree)[pos << 1].from + 1) * (*tree)[pos].lazy;
            (*tree)[pos << 1 | 1].lazy += (*tree)[pos].lazy;
            (*tree)[pos << 1 | 1].sum += ((*tree)[pos << 1 | 1].to - (*tree)[pos << 1 | 1].from + 1) * (*tree)[pos].lazy;
            (*tree)[pos].lazy = 0;
        }
    }
    
public:
    segment_sum_tree() {}
    segment_sum_tree(segment_sum_tree<T> &init) {
        build(init);
    }
    segment_sum_tree(vector<T> &init) {
        build(init);
    }
    segment_sum_tree(size_t size) {
        build(size);
    }
    segment_sum_tree(size_t size, T val) {
        build(size, val);
    }
    segment_sum_tree(initializer_list<T> &init) {
        build(*make_shared<vector<T>>(init));
    }
    
    void build(segment_sum_tree<T> &init) { tree = make_shared<vector<T>>(init.tree); }
    void build(vector<T> &init, size_t from, size_t to, size_t root) {
        tree->resize(init.size() * 4);
        (*tree)[root].from = from;
        (*tree)[root].to = to;
        if (from == to) assign(root, init[from]);
        else {
            size_t mid = (from + to) >> 1;
            build(init, from, mid, root << 1);
            build(init, mid + 1, to, root << 1 | 1);
            push_up(root);
        }
    }
    void build(vector<T> &init) { build(init, 0, init.size() - 1, 1); }
    void build(size_t size, T val) { build(*make_shared<vector<T>>(size, val)); }
    void build(size_t size) { build(size, 0); }
    
    void operator=(segment_sum_tree<T> &init) { build(init); }
    void operator=(vector<T> &init) { build(init); }
    
    T get_value(size_t pos, size_t root = 1) {
        if ((*tree)[root].from == (*tree)[root].to) return (*tree)[root].sum;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (pos <= mid) return get_value(pos, root << 1);
        else return get_value(pos, root << 1 | 1);
    }
    
    T set_value(size_t pos, T (&transform)(T original, T val) = [] (T original, T val) { return val; }, size_t root = 1) {
        if ((*tree)[root].from == (*tree)[root].to) {
            int val = transform((*tree)[root].sum);
            assign(root, val);
            return val;
        }
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        T result;
        if (pos <= mid) result = set_value(pos, transform, root << 1);
        else result = set_value(pos, transform, root << 1 | 1);
        push_up(root);
        return result;
    }
    
    T add_value(size_t pos, T val = 1) { return set_value(pos, val, [] (T original, T val) { return original + val; }); }
    T sub_value(size_t pos, T val = 1) { return add_value(pos, -val); }
    
    T and_value(size_t pos, T val) { return set_value(pos, val, [val] (T original) { return original & val; }); }
    T or_value(size_t pos, T val) { return set_value(pos, val, [val] (T original) { return original | val; }); }
    T not_value(size_t pos) { return set_value(pos, [] (T original) { return ~original; }); }
    
    T get_range_sum(size_t from, size_t to, size_t root = 1) {
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].sum;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_sum(from, to, root << 1);
        else if (from > mid) return get_range_sum(from, to, root << 1 | 1);
        else return get_range_sum(from, mid, root << 1) + get_range_sum(mid + 1, to, root << 1 | 1);
    }
    
    void add_range(size_t from, size_t to, T val, size_t root = 1) {
        if (from == (*tree)[root].from && to == (*tree)[root].to) {
            (*tree)[root].sum += ((*tree)[root].to - (*tree)[root].from + 1) * val;
            (*tree)[root].lazy += val;
            return;
        }
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) / 2;
        if (to <= mid) add_range(from, to, val, root << 1);
        else if (from > mid) add_range(from, to, val, root << 1 | 1);
        else {
            add_range(from, mid, val, root << 1);
            add_range(mid + 1, to, val, root << 1 | 1);
        }
        push_up(root);
    }
    void sub_range(size_t from, size_t to, T val, size_t root = 1) { return add_range(from, to, -val, root); }
};

