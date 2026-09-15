#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

const int INF = numeric_limits<int>::max();

// ----------------------------------------------------------------------
// 1. GRAPH DEFINITION  <-- REPLACE THIS WITH YOUR ACTUAL GRAPH
// ----------------------------------------------------------------------

// Adjacency list: node -> list of (neighbor, edge_weight)
map<string, vector<pair<string, int>>> graph = {
    {"a", {{"b", 4}, {"c", 2}}},
    {"b", {{"a", 4}, {"d", 5}, {"e", 10}}},
    {"c", {{"a", 2}, {"d", 8}, {"f", 12}}},
    {"d", {{"b", 5}, {"c", 8}, {"e", 2}, {"f", 6}}},
    {"e", {{"b", 10}, {"d", 2}, {"f", 3}}},
    {"f", {{"c", 12}, {"d", 6}, {"e", 3}}},
};

// Heuristic h(n): estimated distance from each node to the goal 'f'
// EDIT THESE VALUES to match the table given in your exercise
map<string, int> heuristic = {
    {"a", 11},
    {"b", 9},
    {"c", 8},
    {"d", 4},
    {"e", 3},
    {"f", 0},
};

const string START = "a";
const string GOAL = "f";

// ----------------------------------------------------------------------
// Helper: reconstruct and print a path from the parent map
// ----------------------------------------------------------------------

vector<string> reconstructPath(map<string, string>& parent, const string& goal) {
    vector<string> path;
    string node = goal;
    while (true) {
        path.push_back(node);
        auto it = parent.find(node);
        if (it == parent.end() || it->second == "") break;
        node = it->second;
    }
    reverse(path.begin(), path.end());
    return path;
}

void printPath(const vector<string>& path) {
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n";
}

// ----------------------------------------------------------------------
// 2. DIJKSTRA'S ALGORITHM
// ----------------------------------------------------------------------

void dijkstra(const string& start, const string& goal) {
    map<string, int> dist;
    map<string, string> parent;
    map<string, bool> visited;

    for (auto& kv : graph) dist[kv.first] = INF;
    dist[start] = 0;
    parent[start] = "";

    // min-heap of (distance, node)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, start});

    cout << "Step  Node Visited   Distance\n";
    int step = 0;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        step++;
        cout << step << "     " << u << "              " << d << "\n";

        if (u == goal) break;

        for (auto& [v, weight] : graph[u]) {
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[goal] == INF) {
        cout << "\nNo path found.\n";
        return;
    }

    vector<string> path = reconstructPath(parent, goal);
    cout << "\nShortest path: ";
    printPath(path);
    cout << "Total distance: " << dist[goal] << "\n";
}

// ----------------------------------------------------------------------
// 3. A* SEARCH
// ----------------------------------------------------------------------

void astar(const string& start, const string& goal) {
    map<string, int> gScore;
    map<string, string> parent;
    map<string, bool> visited;

    gScore[start] = 0;
    parent[start] = "";

    // min-heap of (fScore, node)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({heuristic[start], start});

    cout << "Step  Node Expanded  g(n)    h(n)    f(n)\n";
    int step = 0;

    while (!pq.empty()) {
        auto [f, u] = pq.top();
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        step++;
        cout << step << "     " << u << "              "
             << gScore[u] << "       " << heuristic[u] << "       " << f << "\n";

        if (u == goal) break;

        for (auto& [v, weight] : graph[u]) {
            int newG = gScore[u] + weight;
            if (gScore.find(v) == gScore.end() || newG < gScore[v]) {
                gScore[v] = newG;
                parent[v] = u;
                int fScore = newG + heuristic[v];
                pq.push({fScore, v});
                cout << "       generate " << v << ": g=" << newG
                     << " h=" << heuristic[v] << " f=" << fScore << "\n";
            }
        }
    }

    if (gScore.find(goal) == gScore.end()) {
        cout << "\nNo path found.\n";
        return;
    }

    vector<string> path = reconstructPath(parent, goal);
    cout << "\nShortest path: ";
    printPath(path);
    cout << "Total distance: " << gScore[goal] << "\n";
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------

int main() {
    cout << string(60, '=') << "\n";
    cout << "(a) Dijkstra's Algorithm\n";
    cout << string(60, '=') << "\n";
    dijkstra(START, GOAL);

    cout << "\n" << string(60, '=') << "\n";
    cout << "(b) A* Search\n";
    cout << string(60, '=') << "\n";
    astar(START, GOAL);

    return 0;
}
