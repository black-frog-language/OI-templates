#include <iostream>
#include <vector>
#include <queue>
using namespace std;
const int INT_INF = 0x3f3f3f3f;

struct Edge {
    int to, len;
    Edge(int to, int len): to(to), len(len) {}
};

struct Node {
    int id, dist;
    Node(int id, int dist): id(id), dist(dist) {}
    bool operator <(const Node &rhs) const { return dist > rhs.dist; }
};

void dijkstra(vector<vector<Edge>> &map, int from, vector<int> &dist) {
    dist[from] = 0;
    vector<bool> visited(map.size(), false);
    priority_queue<Node> heap;
    heap.push(Node(from, dist[from]));
    for (Node i = heap.top(); !heap.empty(); i = heap.top()) {
        heap.pop();
        if (visited[i.id]) continue;
        for (auto &j : map[i.id]) {
            if (!visited[j.to]) {
                if (dist[i.id] + j.len < dist[j.to]) {
                    dist[j.to] = dist[i.id] + j.len;
                    heap.push(Node(j.to, dist[j.to]));
                }
            }
        }
        visited[i.id] = true;
    }
}

int main() {
    int nNodes, nEdges, from;
    cin >> nNodes >> nEdges >> from;
    vector<vector<Edge>> map(nNodes);
    for (int i = 0; i < nEdges; i++) {
        int from, to, len;
        cin >> from >> to >> len;
        map[from - 1].push_back(Edge(to - 1, len));
    }
    vector<int> dist(nNodes, INT_INF);
    dijkstra(map, from - 1, dist);
    for (auto i : dist) cout << i << ' ';
    return 0;
}