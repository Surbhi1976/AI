"""
Lab Assignment 1 - Problem 2
Water Jug Problem: Given a 4-litre jug and a 3-litre jug (no markings) and an
unlimited water supply, measure out exactly 2 litres in the 4-litre jug.

STATE REPRESENTATION:
    A state is a pair (x, y):
        x = amount of water currently in the 4L jug   (0 <= x <= 4)
        y = amount of water currently in the 3L jug   (0 <= y <= 3)

POSSIBLE OPERATIONS (from any state (x, y)):
    1. Fill 4L jug           -> (4, y)
    2. Fill 3L jug           -> (x, 3)
    3. Empty 4L jug          -> (0, y)
    4. Empty 3L jug          -> (x, 0)
    5. Pour 4L jug -> 3L jug -> (x - t, y + t)   where t = min(x, 3 - y)
    6. Pour 3L jug -> 4L jug -> (x + t, y - t)   where t = min(y, 4 - x)

GOAL TEST: x == 2   (4L jug contains exactly 2 litres)

MANUAL SOLUTION (one shortest path):
    Step |   Action                              | State (4L, 3L)
    -----|----------------------------------------|----------------
      0  | Start                                  | (0, 0)
      1  | Fill the 4L jug                        | (4, 0)
      2  | Pour 4L jug into 3L jug                 | (1, 3)
      3  | Empty the 3L jug                        | (1, 0)
      4  | Pour the 1L from the 4L jug into 3L jug | (0, 1)
      5  | Fill the 4L jug                         | (4, 1)
      6  | Pour 4L jug into 3L jug (3L jug takes 2)| (2, 3)  <-- GOAL: 2L in the 4L jug

This program solves the problem generally using Breadth-First Search (BFS)
over the state space so it will find a *shortest* sequence of operations,
and prints the steps.
"""

from collections import deque


# Capacity of jugs
jug1_capacity = 4
jug2_capacity = 3

# Goal: get 2 litres in jug 1
goal = 2


# Function to generate all possible next states
def get_successors(state):

    x = state[0]   # Water in 4L jug
    y = state[1]   # Water in 3L jug

    successors = []

    # 1. Fill 4L jug
    if x < jug1_capacity:
        new_state = (jug1_capacity, y)
        successors.append((new_state, "Fill the 4L jug"))

    # 2. Fill 3L jug
    if y < jug2_capacity:
        new_state = (x, jug2_capacity)
        successors.append((new_state, "Fill the 3L jug"))

    # 3. Empty 4L jug
    if x > 0:
        new_state = (0, y)
        successors.append((new_state, "Empty the 4L jug"))

    # 4. Empty 3L jug
    if y > 0:
        new_state = (x, 0)
        successors.append((new_state, "Empty the 3L jug"))

    # 5. Pour water from 4L jug to 3L jug
    if x > 0 and y < jug2_capacity:

        amount = min(x, jug2_capacity - y)

        new_x = x - amount
        new_y = y + amount

        new_state = (new_x, new_y)

        successors.append(
            (new_state, "Pour water from 4L jug into 3L jug")
        )

    # 6. Pour water from 3L jug to 4L jug
    if y > 0 and x < jug1_capacity:

        amount = min(y, jug1_capacity - x)

        new_x = x + amount
        new_y = y - amount

        new_state = (new_x, new_y)

        successors.append(
            (new_state, "Pour water from 3L jug into 4L jug")
        )

    return successors


def bfs_water_jug():

    # Starting state
    start = (0, 0)

    # Queue for BFS
    queue = deque()

    # Add starting state
    queue.append(start)

    # Keep track of visited states
    visited = set()
    visited.add(start)

    # Store parent of every state
    parent = {}

    # Starting state has no parent
    parent[start] = (None, "Start")


    # Continue while queue is not empty
    while queue:

        # Take first state from queue
        current = queue.popleft()

        # Get all possible next states
        successors = get_successors(current)

        # Check every next state
        for item in successors:

            next_state = item[0]
            action = item[1]

            # If already visited, skip it
            if next_state in visited:
                continue

            # Mark as visited
            visited.add(next_state)

            # Store how we reached this state
            parent[next_state] = (current, action)

            # Check goal
            if next_state[0] == goal:

                path = []

                current_state = next_state

                # Go backwards from goal to start
                while current_state is not None:

                    previous_state = parent[current_state][0]
                    action_taken = parent[current_state][1]

                    path.append(
                        (current_state, action_taken)
                    )

                    current_state = previous_state

                # Reverse path because we collected it backwards
                path.reverse()

                return path

            # Add new state to queue
            queue.append(next_state)

    return None


def solve():

    # Find solution
    path = bfs_water_jug()

    # If no solution exists
    if path is None:
        print("No solution found.")
        return


    print("Water Jug Problem")
    print("-------------------------")

    # Print every step
    step = 0

    for item in path:

        state = item[0]
        action = item[1]

        print("Step:", step)
        print("Action:", action)
        print("State:", state)

        print()

        step = step + 1


# Start the program
solve()
