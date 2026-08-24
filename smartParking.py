"""
Lab Assignment 1 - Problem 3
Smart Parking System using Breadth-First Search (BFS)

GRID SYMBOLS:
    E -> Entrance / starting position
    A -> Available parking space
    O -> Occupied parking space
    R -> Road (vehicle can pass through)
    X -> Blocked area / wall

The vehicle starts at 'E' and can move Up, Down, Left, Right (one cell at a
time) only through 'R' and 'A' cells. It cannot pass through 'O' or 'X'.

ALGORITHM (BFS):
    1. Locate E, push it into a queue, mark it visited.
    2. Pop the front of the queue and look at its 4 neighbours
       (up, down, left, right).
    3. Skip neighbours that are out of bounds, blocked (X), occupied (O),
       or already visited.
    4. For every valid neighbour, record its parent (for path
       reconstruction) and push it into the queue.
    5. The moment a neighbour is an available space 'A', stop -- because
       BFS explores level by level, this is guaranteed to be one of the
       nearest available spaces (minimum number of moves).
    6. Reconstruct the path by walking parent pointers backward from the
       goal to the entrance, then reverse it.

COMPLEXITY:
    Let R = number of rows, C = number of columns.
    Each cell is visited at most once and each visit examines 4 neighbours.
    Time  complexity: O(R * C)
    Space complexity: O(R * C)   (for the visited set / queue / parent map)
"""

from collections import deque


# Find the entrance position
def find_entrance(grid):

    rows = len(grid)
    cols = len(grid[0])

    for i in range(rows):

        for j in range(cols):

            if grid[i][j] == 'E':
                return (i, j)

    return None


# BFS to find nearest available parking
def bfs_nearest_parking(grid):

    rows = len(grid)
    cols = len(grid[0])

    # Find starting position
    start = find_entrance(grid)

    # Check if entrance exists
    if start is None:
        print("No entrance found.")
        return None, -1, []


    # Create queue
    queue = deque()

    # Add starting position
    queue.append(start)


    # Create visited set
    visited = set()

    # Mark starting position as visited
    visited.add(start)


    # Store parent of every position
    parent = {}

    # Start has no parent
    parent[start] = None


    # Possible movements
    moves = [
        (-1, 0),   # Up
        (1, 0),    # Down
        (0, -1),   # Left
        (0, 1)     # Right
    ]


    # BFS
    while queue:

        # Remove first position
        current = queue.popleft()

        r = current[0]
        c = current[1]


        # Check all 4 directions
        for move in moves:

            dr = move[0]
            dc = move[1]

            # Calculate new position
            nr = r + dr
            nc = c + dc


            # -----------------------------
            # Check if position is outside grid
            # -----------------------------
            if nr < 0 or nr >= rows or nc < 0 or nc >= cols:
                continue


            # New position
            next_position = (nr, nc)


            # -----------------------------
            # Skip if already visited
            # -----------------------------
            if next_position in visited:
                continue


            # Get value of the cell
            cell = grid[nr][nc]


            # -----------------------------
            # Cannot move through X or O
            # -----------------------------
            if cell == 'X' or cell == 'O':
                continue


            # Mark position as visited
            visited.add(next_position)


            # Store parent
            parent[next_position] = current


            # -----------------------------
            # Check if available parking found
            # -----------------------------
            if cell == 'A':

                path = []

                current_position = next_position


                # Go backwards from parking space
                # to entrance
                while current_position is not None:

                    path.append(current_position)

                    current_position = parent[current_position]


                # Reverse path
                path.reverse()


                # Number of moves
                number_of_moves = len(path) - 1


                return next_position, number_of_moves, path


            # Add position to queue
            queue.append(next_position)


    # No available parking found
    return None, -1, []


# Print the grid
def print_grid(grid):

    for row in grid:

        for cell in row:

            print(cell, end=" ")

        print()


# Print result
def print_result(goal, moves, path):

    if goal is None:

        print("No reachable available parking space found.")

    else:

        print()
        print("Nearest available parking space:", goal)

        print("Number of movements:", moves)

        print("Route:")

        for position in path:
            print(position)


# ---------------- MAIN PROGRAM ----------------


grid = [

    ['E', 'R', 'R', 'X', 'A', 'X', 'X', 'R', 'R', 'R'],

    ['A', 'R', 'R', 'X', 'O', 'R', 'R', 'O', 'R', 'A'],

    ['X', 'R', 'X', 'X', 'R', 'X', 'R', 'X', 'R', 'X'],

    ['R', 'R', 'R', 'R', 'R', 'X', 'R', 'R', 'R', 'R'],

    ['R', 'X', 'X', 'X', 'R', 'X', 'X', 'X', 'X', 'R'],

    ['R', 'R', 'R', 'O', 'R', 'R', 'R', 'A', 'R', 'R'],

    ['X', 'X', 'R', 'X', 'X', 'X', 'R', 'X', 'X', 'R'],

    ['A', 'R', 'R', 'R', 'O', 'R', 'R', 'R', 'R', 'R'],

    ['O', 'X', 'X', 'R', 'X', 'X', 'X', 'R', 'X', 'X'],

    ['R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'A'],

    ['X', 'X', 'R', 'X', 'X', 'X', 'R', 'X', 'X', 'X'],

    ['A', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R']

]


print("Parking Grid:")
print()

print_grid(grid)


# Find nearest parking
goal, number_of_moves, path = bfs_nearest_parking(grid)


# Print result
print_result(goal, number_of_moves, path)
