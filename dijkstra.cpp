// LuoguOI P3371
#include <iostream>
#include <vector>
using namespace std;
const int INT_INF = 2147483647;

struct Edge {
    int to, len;
    Edge(int to, int len): to(to), len(len) {}
};

int minUnvisited(vector<bool> &visited, vector<int> &dist) {
    int min = dist.size();
    dist.push_back(INT_INF);
    for (int i = 0; i < visited.size(); i++)
        if (dist[i] < dist[min] && !visited[i]) min = i;
    dist.pop_back();
    if (min == dist.size()) return -1;
    return min;
}

void dijkstra(vector<vector<Edge>> &map, int from, vector<int> &dist) {
    dist[from] = 0;
    vector<bool> visited(map.size(), false);
    for (int i = minUnvisited(visited, dist); i >= 0; i = minUnvisited(visited, dist)) {
        for (int j = 0; j < map[i].size(); j++) {
            if (!visited[map[i][j].to]) {
                dist[map[i][j].to] = min(dist[map[i][j].to], dist[i] + map[i][j].len);
            }
        }
        visited[i] = true;
    }
}

int main() {
    int nNodes, nEdges, from;
    cin >> nNodes >> nEdges >> from;
    vector<vector<Edge>> map(nNodes);
    for (int i = 0; i < nEdges; i++) {
        int from, to, len;
        cin >> from >> to >> len;
        map[from].push_back(Edge(to, len));
    }
    vector<int> dist(nNodes, INT_INF);
    dijkstra(map, from, dist);
    for (auto i : dist) cout << i << ' ';
    return 0;
}