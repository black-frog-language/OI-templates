// LuoguOI P3371
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int SAFE_INT_INF = 2147483647;

struct Edge {
    int to, len;
    Edge(int to, int len): to(to), len(len) {}
};

void spfa(vector<vector<Edge>> &map, int from, vector<int> &dist) {
    vector<bool> inQueue(map.size(), false);
    queue<int> bfsQueue;
    bfsQueue.push(from);
    inQueue[from] = true;
    while (bfsQueue.size()) {
        int front = bfsQueue.front();
        for (auto &i : map[front])
            if (!inQueue[i.to] && dist[i.to] > dist[front] + i.len) {
                dist[i.to] = dist[front] + i.len;
                bfsQueue.push(i.to);
            }
        inQueue[front] = false;
        bfsQueue.pop();
    }
}

int main() {
    int nNodes, nEdges, iStart;
    cin >> nNodes >> nEdges >> iStart;
    vector<vector<Edge>> map(nNodes);
    for (int i = 0; i < nEdges; i++) {
        int from, to, len;
        cin >> from >> to >> len;
        map[from - 1].push_back(Edge(to - 1, len));
    }
    vector<int> dist(nNodes, SAFE_INT_INF);
    dist[iStart - 1] = 0;
    spfa(map, iStart - 1, dist);
    for (auto i : dist) cout << i << ' ';
    return 0;
}