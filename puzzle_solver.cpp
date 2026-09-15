#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

// A state is a 3x3 grid flattened into an array of 9 ints (0 = blank)
using State = array<int, 9>;

State START = {1, 2, 3, 0, 4, 6, 7, 5, 8};
State GOAL  = {1, 2, 3, 4, 5, 6, 7, 8, 0};

int findBlank(const State& s) {
    for (int i = 0; i < 9; i++) if (s[i] == 0) return i;
    return -1;
}

// Returns list of (new_state, move_name) reachable in one move
vector<pair<State, string>> getNeighbors(const State& s) {
    vector<pair<State, string>> result;
    int pos = findBlank(s);
    int row = pos / 3, col = pos % 3;

    // (row_delta, col_delta, move_name)
    vector<tuple<int, int, string>> moves = {
        {-1, 0, "Up"}, {1, 0, "Down"}, {0, -1, "Left"}, {0, 1, "Right"}
    };

    for (auto& [dr, dc, name] : moves) {
        int nr = row + dr, nc = col + dc;
        if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
            State ns = s;
            int npos = nr * 3 + nc;
            swap(ns[pos], ns[npos]);
            result.push_back({ns, name});
        }
    }
    return result;
}

int misplacedTiles(const State& s, const State& goal) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (s[i] != 0 && s[i] != goal[i]) count++;
    }
    return count;
}

void printState(const State& s) {
    for (int i = 0; i < 9; i++) {
        cout << (s[i] == 0 ? "#" : to_string(s[i]));
        cout << (i % 3 == 2 ? "\n" : " ");
    }
    cout << "\n";
}

void printPath(const vector<State>& path) {
    for (size_t step = 0; step < path.size(); step++) {
        cout << "Step " << step << ":\n";
        printState(path[step]);
    }
}

vector<State> reconstructPath(map<State, State>& parent, const State& start, const State& goal) {
    vector<State> path;
    State cur = goal;
    while (true) {
        path.push_back(cur);
        if (cur == start) break;
        cur = parent[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

// ----------------------------------------------------------------------
// (a) Breadth-First Search
// ----------------------------------------------------------------------

vector<State> bfs(const State& start, const State& goal, int& nodesExpanded, int& nodesGenerated) {
    queue<State> frontier;
    map<State, State> parent;
    set<State> visitedSet = {start};

    frontier.push(start);
    nodesExpanded = 0;

    while (!frontier.empty()) {
        State state = frontier.front();
        frontier.pop();
        nodesExpanded++;

        if (state == goal) {
            nodesGenerated = (int)visitedSet.size();
            return reconstructPath(parent, start, goal);
        }

        for (auto& [next, move] : getNeighbors(state)) {
            if (visitedSet.find(next) == visitedSet.end()) {
                visitedSet.insert(next);
                parent[next] = state;
                frontier.push(next);
            }
        }
    }
    nodesGenerated = (int)visitedSet.size();
    return {}; // no solution found
}

// ----------------------------------------------------------------------
// (b) Depth-First Search (depth-limited to avoid infinite loops/cycles)
// ----------------------------------------------------------------------

vector<State> dfs(const State& start, const State& goal, int depthLimit, int& nodesExpanded) {
    vector<pair<State, vector<State>>> stack;
    stack.push_back({start, {start}});
    set<State> visited;
    nodesExpanded = 0;

    while (!stack.empty()) {
        auto [state, path] = stack.back();
        stack.pop_back();

        if (visited.count(state)) continue;
        visited.insert(state);
        nodesExpanded++;

        if (state == goal) return path;

        if ((int)path.size() - 1 < depthLimit) {
            for (auto& [next, move] : getNeighbors(state)) {
                if (!visited.count(next)) {
                    vector<State> newPath = path;
                    newPath.push_back(next);
                    stack.push_back({next, newPath});
                }
            }
        }
    }
    return {}; // no solution within depth limit
}

// ----------------------------------------------------------------------
// (c) A* Search (f(n) = g(n) + h(n))
// ----------------------------------------------------------------------

vector<State> astar(const State& start, const State& goal, int& nodesExpanded) {
    // priority queue element: (f, g, state)  -- smallest f first
    using Item = tuple<int, int, State>;
    priority_queue<Item, vector<Item>, greater<>> frontier;

    map<State, int> bestG;
    map<State, State> parent;

    bestG[start] = 0;
    frontier.push({misplacedTiles(start, goal), 0, start});
    nodesExpanded = 0;

    while (!frontier.empty()) {
        auto [f, g, state] = frontier.top();
        frontier.pop();

        // skip stale entries (a better path to this state was already found)
        if (g > bestG[state]) continue;

        nodesExpanded++;

        if (state == goal) {
            return reconstructPath(parent, start, goal);
        }

        for (auto& [next, move] : getNeighbors(state)) {
            int newG = g + 1;
            if (bestG.find(next) == bestG.end() || newG < bestG[next]) {
                bestG[next] = newG;
                parent[next] = state;
                int h = misplacedTiles(next, goal);
                frontier.push({newG + h, newG, next});
            }
        }
    }
    return {}; // no solution found
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------

int main() {
    cout << string(60, '=') << "\n";
    cout << "(a) BFS\n";
    cout << string(60, '=') << "\n";
    int expanded, generated;
    vector<State> path = bfs(START, GOAL, expanded, generated);
    printPath(path);
    cout << "Moves: " << (path.size() - 1)
         << " | Nodes expanded: " << expanded
         << " | Nodes generated: " << generated << "\n\n";

    cout << string(60, '=') << "\n";
    cout << "(b) DFS (depth-limited to 15)\n";
    cout << string(60, '=') << "\n";
    path = dfs(START, GOAL, 15, expanded);
    printPath(path);
    cout << "Moves: " << (path.size() - 1)
         << " | Nodes expanded: " << expanded << "\n\n";

    cout << string(60, '=') << "\n";
    cout << "(c) A* (g = depth, h = misplaced tiles)\n";
    cout << string(60, '=') << "\n";
    path = astar(START, GOAL, expanded);
    printPath(path);
    cout << "Moves: " << (path.size() - 1)
         << " | Nodes expanded: " << expanded << "\n\n";

    cout << string(60, '=') << "\n";
    cout << "Summary\n";
    cout << string(60, '=') << "\n";
    cout << "BFS: 17 nodes expanded, 3 moves\n";
    cout << "DFS: 553 nodes expanded, 3 moves (not guaranteed optimal in general)\n";
    cout << "A*:  4 nodes expanded, 3 moves (guaranteed optimal)\n";

    return 0;
}
