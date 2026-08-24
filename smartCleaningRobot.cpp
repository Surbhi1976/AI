/*
 * AI Lab 2 (11/08/26) - Q1 (C++ version)
 * Smart Cleaning Robot: Simple Reflex Agent vs Model-Based Reflex Agent
 *
 * GRID SYMBOLS:
 *   S -> Starting position of the robot
 *   D -> Dirty cell
 *   C -> Clean cell
 *   X -> Blocked area / wall
 *
 * The robot moves one cell at a time (Up, Down, Left, Right) through valid
 * (non-blocked) cells. When on a dirty cell it may perform a CLEAN action.
 *
 * ------------------------------------------------------------------------
 * AGENT 1: SIMPLE REFLEX AGENT
 * ------------------------------------------------------------------------
 * Decides its action purely from the CURRENT percept (the symbol on the
 * current cell):
 *     - percept == 'D' -> CLEAN
 *     - otherwise       -> move to a randomly chosen currently-valid
 *                           neighbouring cell
 * It keeps NO memory of which cells it has already visited or cleaned.
 * A purely fixed/deterministic priority order (always try Up, then Right,
 * then Down, then Left) can trap a memoryless agent in an infinite 2-cell
 * oscillation whenever two neighbouring cells are each other's only
 * currently valid move. The standard textbook fix (Russell & Norvig) is to
 * randomize the agent's choice among currently valid moves -- the decision
 * still depends only on the current percept, not on any memory of past
 * cells, so this remains a genuine simple reflex agent. A fixed random
 * seed is used so the run is reproducible.
 *
 * ------------------------------------------------------------------------
 * AGENT 2: MODEL-BASED REFLEX AGENT (Reflex Agent with State)
 * ------------------------------------------------------------------------
 * Maintains an internal model of the world:
 *     - a `visited` set          (cells already explored)
 *     - a `cleaned` set          (cells already cleaned)
 *     - a DFS-style path stack   (memory of how it got where it is)
 * Decision rule:
 *     - percept == 'D' and not yet cleaned -> CLEAN
 *     - otherwise prefer an UNVISITED valid neighbour (fixed priority
 *       Up, Right, Down, Left)
 *     - if every neighbour has already been visited, BACKTRACK one step
 *       along the path it came from (pop the stack)
 * This guarantees full exploration of the reachable region while only
 * "wasting" movements on backtracking, so it needs far fewer total
 * movements and repeated visits than the Simple Reflex Agent.
 *
 * Compile:  g++ -O2 -std=c++17 -o cleaning_robot cleaning_robot.cpp
 * Run:      ./cleaning_robot
 */
#include <bits/stdc++.h>
using namespace std;

// Grid symbols:
// S -> Start
// D -> Dirty
// C -> Clean
// X -> Wall

// Directions: Up, Right, Down, Left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};


// Check whether a cell is inside the grid
bool isValid(vector<string>& grid, int r, int c) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (r < 0 || r >= rows || c < 0 || c >= cols)
        return false;

    if (grid[r][c] == 'X')
        return false;

    return true;
}


// Find starting position
pair<int, int> findStart(vector<string>& grid) {

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {

            if (grid[i][j] == 'S') {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}


// Count total dirty cells
int countDirty(vector<string>& grid) {

    int count = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {

            if (grid[i][j] == 'D') {
                count++;
            }
        }
    }

    return count;
}


// Print grid
void printGrid(vector<string>& grid) {

    for (int i = 0; i < grid.size(); i++) {

        for (int j = 0; j < grid[0].size(); j++) {
            cout << grid[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;
}


// -----------------------------------------------------
// SIMPLE REFLEX AGENT
// -----------------------------------------------------

void simpleReflexAgent(vector<string> grid) {

    pair<int, int> start = findStart(grid);

    int r = start.first;
    int c = start.second;

    int totalDirty = countDirty(grid);
    int cleaned = 0;

    int movements = 0;
    int actions = 0;

    // To count how many times each cell is visited
    vector<vector<int>> visits(
        grid.size(),
        vector<int>(grid[0].size(), 0)
    );

    visits[r][c] = 1;

    // Random generator
    srand(42);

    int maxSteps = 5000;

    while (cleaned < totalDirty && actions < maxSteps) {

        char percept = grid[r][c];

        // If current cell is dirty -> CLEAN
        if (percept == 'D') {

            cout << "Robot at (" << r << "," << c
                 << ") -> CLEAN" << endl;

            grid[r][c] = 'C';

            cleaned++;
            actions++;

            continue;
        }


        // Store valid directions
        vector<int> possibleMoves;

        for (int i = 0; i < 4; i++) {

            int newR = r + dr[i];
            int newC = c + dc[i];

            if (isValid(grid, newR, newC)) {
                possibleMoves.push_back(i);
            }
        }


        // If no movement is possible
        if (possibleMoves.empty()) {
            cout << "Robot is trapped!" << endl;
            break;
        }


        // Choose a random valid direction
        int randomIndex = rand() % possibleMoves.size();

        int direction = possibleMoves[randomIndex];

        r = r + dr[direction];
        c = c + dc[direction];

        movements++;
        actions++;

        visits[r][c]++;

        cout << "Robot moved to ("
             << r << "," << c << ")" << endl;
    }


    // Count repeated visits
    int repeatedVisits = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {

            if (visits[i][j] > 1) {
                repeatedVisits += visits[i][j] - 1;
            }
        }
    }


    cout << "\n--- SIMPLE REFLEX AGENT REPORT ---\n";

    cout << "Dirty cells cleaned: "
         << cleaned << " / " << totalDirty << endl;

    cout << "Movements: "
         << movements << endl;

    cout << "Total actions: "
         << actions << endl;

    cout << "Repeated visits: "
         << repeatedVisits << endl;

    cout << "\nFinal Grid:\n";

    printGrid(grid);
}


// -----------------------------------------------------
// MODEL-BASED REFLEX AGENT
// -----------------------------------------------------

void modelBasedAgent(vector<string> grid) {

    pair<int, int> start = findStart(grid);

    int r = start.first;
    int c = start.second;

    int totalDirty = countDirty(grid);
    int cleaned = 0;

    int movements = 0;
    int actions = 0;


    // visited[i][j] tells whether robot has visited this cell
    vector<vector<bool>> visited(
        grid.size(),
        vector<bool>(grid[0].size(), false)
    );


    // visits[i][j] counts number of visits
    vector<vector<int>> visits(
        grid.size(),
        vector<int>(grid[0].size(), 0)
    );


    visited[r][c] = true;
    visits[r][c] = 1;


    // Stack stores the path of the robot
    stack<pair<int, int>> path;

    path.push({r, c});


    int maxSteps = 5000;


    while (cleaned < totalDirty && actions < maxSteps) {

        char percept = grid[r][c];


        // Rule 1:
        // If current cell is dirty, clean it
        if (percept == 'D') {

            cout << "Robot at (" << r << "," << c
                 << ") -> CLEAN" << endl;

            grid[r][c] = 'C';

            cleaned++;
            actions++;

            continue;
        }


        // Rule 2:
        // Find an unvisited neighbour
        bool foundNewCell = false;


        for (int i = 0; i < 4; i++) {

            int newR = r + dr[i];
            int newC = c + dc[i];


            if (isValid(grid, newR, newC) &&
                visited[newR][newC] == false) {

                // Move to new cell
                r = newR;
                c = newC;

                visited[r][c] = true;

                path.push({r, c});

                visits[r][c]++;

                movements++;
                actions++;

                cout << "Robot moved to new cell ("
                     << r << "," << c << ")" << endl;

                foundNewCell = true;

                break;
            }
        }


        // Rule 3:
        // If no unvisited neighbour, backtrack
        if (foundNewCell == false) {

            // If only starting cell is left,
            // exploration is complete
            if (path.size() == 1) {
                break;
            }


            // Remove current cell
            path.pop();


            // Go back to previous cell
            r = path.top().first;
            c = path.top().second;

            visits[r][c]++;

            movements++;
            actions++;

            cout << "Robot backtracked to ("
                 << r << "," << c << ")" << endl;
        }
    }


    // Count repeated visits
    int repeatedVisits = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {

            if (visits[i][j] > 1) {
                repeatedVisits += visits[i][j] - 1;
            }
        }
    }


    cout << "\n--- MODEL-BASED REFLEX AGENT REPORT ---\n";

    cout << "Dirty cells cleaned: "
         << cleaned << " / " << totalDirty << endl;

    cout << "Movements: "
         << movements << endl;

    cout << "Total actions: "
         << actions << endl;

    cout << "Repeated visits: "
         << repeatedVisits << endl;

    cout << "\nFinal Grid:\n";

    printGrid(grid);
}


// -----------------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------------

int main() {

    vector<string> grid = {

        "SCDCXCCDCC",
        "CXCCCCXCDC",
        "DCCXDCCCCC",
        "CCXCCCDXCC",
        "CDCCXCCCCD",
        "CCCDCCXCCC",
        "XCCCCDCCXC",
        "CCDXCCCDCC",
        "CXCCDCCCCC",
        "DCCCCXCCDC"
    };


    cout << "INITIAL GRID:\n";

    printGrid(grid);


    // Run Simple Reflex Agent
    simpleReflexAgent(grid);


    cout << "\n\n=================================\n\n";


    // Run Model-Based Reflex Agent
    modelBasedAgent(grid);


    return 0;
}
