// LuoguOI P3366
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class disjoint_set {
private:
    vector<long> tree;
public:
    disjoint_set() {}
    disjoint_set(long size) {
        resize(size);
    }
    void resize(long size) {
        tree.resize(size);
        for (int i = 0; i < size; i++) tree[i] = i;
    }
    // 'find(...)' returns a node's root.
    long find(long pos) { return (tree[pos] == pos) ? pos : tree[pos] = find(tree[pos]); }
    // 'combine(...)' makes two nodes' roots the same.
    void combine(long a, long b) { tree[find(b)] = find(a); }
    // 'is(a, b)' is a shorthand for find(a) == find(b).
    bool is(long a, long b) { return find(a) == find(b); }
};

struct edge {
    int from, to, weight;
    edge() {}
    edge(int from, int to, int weight): from(from), to(to), weight(weight) {}
};

vector<edge> edges;
disjoint_set nodes;

int main() {
    int nNodes, nEdges;
    cin >> nNodes >> nEdges;
    nodes.resize(nNodes);
    edges.resize(nEdges);
    for (int i = 0; i < nEdges; i++) {
        int from, to, weight;
        cin >> from >> to >> weight;
        edges[i] = edge(from - 1, to - 1, weight);
    }
    sort(edges.begin(), edges.end(), [](edge &a, edge &b) { return a.weight < b.weight; });
    int weight = 0;
    int count = 0;
    bool flag = false;
    for (int i = 0; i < nEdges; i++) {
        if (!nodes.is(edges[i].from, edges[i].to)) {
            weight += edges[i].weight;
            nodes.combine(edges[i].from, edges[i].to);
            count++;
        }
        if (count == nNodes - 1) {
            flag = true;
            break;
        }
    }
    if (flag) cout << weight;
    else cout << -1;
    return 0;
}
