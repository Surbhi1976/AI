#include <bits/stdc++.h>
using namespace std;

int main()
{
    // Hospital grid
    vector<string> grid = {
        "SCCCCCCCCC",
        "XXCXXXXXXC",
        "CCCECCCCCC",
        "CXCXXXXXXC",
        "CCCCCCCCCC",
        "XXXXXXXCXX",
        "CCCCCCCECC",
        "CXXXXXXXCC",
        "CCCCCCCCCC",
        "CCCCCCCCCC"
    };

    int rows = grid.size();
    int cols = grid[0].size();

    // Find starting position S
    pair<int, int> start;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 'S')
            {
                start = {i, j};
            }
        }
    }

    // Up, Down, Left, Right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    // BFS
    queue<pair<int, int>> q;

    set<pair<int, int>> visited;

    // Store previous cell
    map<pair<int, int>, pair<int, int>> parent;

    // Store distance
    map<pair<int, int>, int> distance;

    q.push(start);
    visited.insert(start);
    distance[start] = 0;

    pair<int, int> emergencyRoom = {-1, -1};

    while (!q.empty())
    {
        pair<int, int> current = q.front();
        q.pop();

        int r = current.first;
        int c = current.second;

        // If we reached an emergency room
        if (grid[r][c] == 'E')
        {
            emergencyRoom = current;
            break;
        }

        // Check all 4 directions
        for (int i = 0; i < 4; i++)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check boundary
            if (nr < 0 || nr >= rows ||
                nc < 0 || nc >= cols)
            {
                continue;
            }

            // Can move only through C and E
            if (grid[nr][nc] != 'C' &&
                grid[nr][nc] != 'E')
            {
                continue;
            }

            pair<int, int> next = {nr, nc};

            // If not visited
            if (visited.count(next) == 0)
            {
                visited.insert(next);

                parent[next] = current;

                distance[next] =
                    distance[current] + 1;

                q.push(next);
            }
        }
    }

    // Check if emergency room was found
    if (emergencyRoom.first == -1)
    {
        cout << "No emergency room is reachable." << endl;
        return 0;
    }

    // ------------------------------------------------
    // Print answer
    // ------------------------------------------------

    cout << "Nearest available emergency room: "
         << "(" << emergencyRoom.first
         << ", " << emergencyRoom.second << ")"
         << endl;

    cout << "Minimum number of movements: "
         << distance[emergencyRoom]
         << endl;

    // Create route
    vector<pair<int, int>> path;

    pair<int, int> current = emergencyRoom;

    while (current != start)
    {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(start);

    // Reverse the path
    reverse(path.begin(), path.end());

    cout << "Route: ";

    for (int i = 0; i < path.size(); i++)
    {
        cout << "(" << path[i].first
             << "," << path[i].second << ")";

        if (i != path.size() - 1)
        {
            cout << " -> ";
        }
    }

    cout << endl;

    // Complexity
    cout << "\nTime Complexity: O(R * C)" << endl;
    cout << "Space Complexity: O(R * C)" << endl;

    return 0;
}
