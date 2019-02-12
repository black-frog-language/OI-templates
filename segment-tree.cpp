#include <vector>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
using std::vector;
using std::initializer_list;
using std::min;
using std::max;
using std::shared_ptr;
using std::make_shared;
using std::out_of_range;
using std::string;
using std::to_string;

// get: get_value; get_range_sum, get_range_min, get_range_max;
// set: set_value, add_value, sub_value, and_value, or_value, not_value; add_range, sub_range;
// build: segment_tree, operator=, build
template <typename T>
class segment_tree {
    
private:
    template <typename U>
    struct node {
        size_t from, to;
        U sum;
        U cmp;
        U add_lazy = static_cast<U>(0), mul_lazy = static_cast<U>(1);
    };
    
    shared_ptr<vector<node<T>>> tree = make_shared<vector<node<T>>>();
    size_t _size = 0;
    
    void assign(size_t pos, T val) {
        (*tree)[pos].sum = val;
        (*tree)[pos].max = val;
        (*tree)[pos].min = val;
    }
    void assign_add_lazy(size_t pos, T val) {
        (*tree)[pos].add_lazy += val;
        (*tree)[pos].sum += ((*tree)[pos << 1].to - (*tree)[pos << 1].from + 1) * val;
        (*tree)[pos].max += val;
        (*tree)[pos].min += val;
    }
    void assign_mul_lazy(size_t pos, T val) {
        (*tree)[pos].mul_lazy *= val;
        (*tree)[pos].sum *= val;
        (*tree)[pos].max *= val;
        (*tree)[pos].min *= val;
    }
    void assign_lazy(size_t pos, node<T> &val) {
        assign_add_lazy(pos, val.add_lazy);
        assign_mul_lazy(pos, val.mul_lazy);
    }
    void clear_lazy(size_t pos) {
        (*tree)[pos].add_lazy = 0;
        (*tree)[pos].mul_lazy = 1;
    }
    void push_up(size_t pos) {
        (*tree)[pos].sum = cmp(((*tree)[pos << 1].sum, (*tree)[pos << 1 | 1].sum));
        
    }
    void push_down(size_t pos) {
        if ((*tree)[pos].add_lazy || (*tree)[pos].mul_lazy) {
            assign_lazy(pos << 1, (*tree)[pos]);
            assign_lazy(pos << 1 | 1, (*tree)[pos]);
            clear_lazy(pos);
        }
    }
    
    void assert_pos(size_t pos) {
        if (pos < 0 || pos >= _size)
            throw out_of_range(string("out of range: position ") + to_string(pos));
    }
    
    void assert_range(size_t from, size_t to) {
        if (from > to || from < 0 || to < 0 || from >= _size || to >= _size)
            throw out_of_range(string("out of range: from ") + to_string(from) + string(" to ") + to_string(to));
    }
    
public:
    segment_tree() {}
    segment_tree(segment_tree<T> &init) {
        build(init);
    }
    segment_tree(vector<T> &init) {
        build(init);
    }
    segment_tree(size_t size) {
        build(size);
    }
    segment_tree(size_t size, T val) {
        build(size, val);
    }
    segment_tree(initializer_list<T> init) {
        build(*make_shared<vector<T>>(init));
    }
    
    void build(segment_tree<T> &init) { tree = make_shared<vector<T>>(init.tree); }
    void build(vector<T> &init, size_t from, size_t to, size_t root,U cmp) {
        assert_range(from, to);
        (*tree)[root].from = from;
        (*tree)[root].to = to;
        (*tree)[root].cmp = cmp;
        if (from == to) assign(root, init[from]);
        else {
            size_t mid = (from + to) >> 1;
            build(init, from, mid, root << 1);
            build(init, mid + 1, to, root << 1 | 1);
            push_up(root);
        }
    }
    void build(vector<T> &init) {
        _size = init.size();
        tree->resize(init.size() << 4);
        build(init, 0, init.size() - 1, 1);
    }
    void build(size_t size, T val) { build(*make_shared<vector<T>>(size, val)); }
    void build(size_t size) { build(size, 0); }
    
    void operator=(segment_tree<T> &init) { build(init); }
    void operator=(vector<T> &init) { build(init); }
    
    T get_value(size_t pos, size_t root = 1) {
        assert_pos(pos);
        if ((*tree)[root].from == (*tree)[root].to) return (*tree)[root].sum;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (pos <= mid) return get_value(pos, root << 1);
        else return get_value(pos, root << 1 | 1);
    }
    
    T set_value(size_t pos, T (&transform)(T original), size_t root = 1) {
        assert_pos(pos);
        T val = transform((*tree)[root].sum);
        if ((*tree)[root].from == (*tree)[root].to) {
            assign(root, val);
            return val;
        }
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        T result;
        if (pos <= mid) result = set_value(pos, val, root << 1);
        else result = set_value(pos, val, root << 1 | 1);
        push_up(root);
        return result;
    }
    T set_value(size_t pos, T val) { return set_value(pos, [val](T original) { return val; }); }
    
    T add_value(size_t pos, T val = 1) { return set_value(pos, [val](T original) { return original + val; }); }
    T sub_value(size_t pos, T val = 1) { return add_value(pos, -val); }
    
    T and_value(size_t pos, T val) { return set_value(pos, [val](T original) { return original & val; }); }
    T or_value(size_t pos, T val) { return set_value(pos, [val](T original) { return original | val; }); }
    T not_value(size_t pos) { return set_value(pos, [](T original) { return ~original; }); }
    
    T get_range_sum(size_t from, size_t to, size_t root = 1) {
        assert_range(from, to);
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].sum;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_sum(from, to, root << 1);
        else if (from > mid) return get_range_sum(from, to, root << 1 | 1);
        else return get_range_sum(from, mid, root << 1) + get_range_sum(mid + 1, to, root << 1 | 1);
    }
    
    T get_range_max(size_t from, size_t to, size_t root = 1) {
        assert_range(from, to);
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].max;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_max(from, to, root << 1);
        else if (from > mid) return get_range_max(from, to, root << 1 | 1);
        else return max(get_range_max(from, mid, root << 1), get_range_max(mid + 1, to, root << 1 | 1));
    }
    
    T get_range_min(size_t from, size_t to, size_t root = 1) {
        assert_range(from, to);
        if (from == (*tree)[root].from && to == (*tree)[root].to) return (*tree)[root].min;
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) return get_range_min(from, to, root << 1);
        else if (from > mid) return get_range_min(from, to, root << 1 | 1);
        else return min(get_range_min(from, mid, root << 1), get_range_min(mid + 1, to, root << 1 | 1));
    }
    
    void add_range(size_t from, size_t to, T val = 1, size_t root = 1) {
        assert_range(from, to);
        if (from == (*tree)[root].from && to == (*tree)[root].to) {
            assign_add_lazy(root, val);
            return;
        }
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) add_range(from, to, val, root << 1);
        else if (from > mid) add_range(from, to, val, root << 1 | 1);
        else {
            add_range(from, mid, val, root << 1);
            add_range(mid + 1, to, val, root << 1 | 1);
        }
        push_up(root);
    }
    void sub_range(size_t from, size_t to, T val = 1, size_t root = 1) { return add_range(from, to, -val, root); }
    
    void mul_range(size_t from, size_t to, T val, size_t root = 1) {
        assert_range(from, to);
        if (from == (*tree)[root].from && to == (*tree)[root].to) {
            assign_mul_lazy(root, val);
            return;
        }
        push_down(root);
        size_t mid = ((*tree)[root].from + (*tree)[root].to) >> 1;
        if (to <= mid) mul_range(from, to, val, root << 1);
        else if (from > mid) mul_range(from, to, val, root << 1 | 1);
        else {
            mul_range(from, mid, val, root << 1);
            mul_range(mid + 1, to, val, root << 1 | 1);
        }
        push_up(root);
    }
    void div_range(size_t from, size_t to, T val = 1, size_t root = 1) { return mul_range(from, to, static_cast<T>(1.0) / val, root); }
    
};
