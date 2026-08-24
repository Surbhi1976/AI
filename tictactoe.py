"""
Lab Assignment 1 - Problem 1
Tic-Tac-Toe solved using an input matrix (traditional / brute-force programming).

The program accepts a completed (or partially completed) N x N board as a
matrix of characters ('X', 'O', or '_' for empty) and determines the result:
    - 'X' wins
    - 'O' wins
    - Draw
    - Game not yet finished

APPROACH (traditional programming, not search/AI):
We simply scan every row, every column and the two diagonals and check
whether all N cells in that line contain the same non-empty symbol.

TIME COMPLEXITY:
    For a general N x N board:
        - Checking all N rows            -> N rows * N cells  = O(N^2)
        - Checking all N columns         -> N cols * N cells  = O(N^2)
        - Checking the 2 diagonals       -> O(N)
        - Checking for a draw (any '_')  -> O(N^2)
    Total: O(N^2)

    For the classic 3x3 Tic-Tac-Toe, N is a constant (3), so the algorithm
    runs in O(1) time and O(1) space in practice, but expressing it in terms
    of board size N gives O(N^2), which is the tight bound for the general
    N x N generalisation of the game.

SPACE COMPLEXITY: O(1) extra space (in-place checks on the given matrix).
"""

from typing import List, Optional


def check_winner(board: List[List[str]]) -> Optional[str]:
    """Return 'X' or 'O' if that player has won, else None."""
    n = len(board)

    # 1. Check rows            -> O(N^2)
    for row in board:
        if row.count(row[0]) == n and row[0] != '_':
            return row[0]

    # 2. Check columns         -> O(N^2)
    for c in range(n):
        col_vals = [board[r][c] for r in range(n)]
        if col_vals.count(col_vals[0]) == n and col_vals[0] != '_':
            return col_vals[0]

    # 3. Check main diagonal   -> O(N)
    main_diag = [board[i][i] for i in range(n)]
    if main_diag.count(main_diag[0]) == n and main_diag[0] != '_':
        return main_diag[0]

    # 4. Check anti-diagonal   -> O(N)
    anti_diag = [board[i][n - 1 - i] for i in range(n)]
    if anti_diag.count(anti_diag[0]) == n and anti_diag[0] != '_':
        return anti_diag[0]

    return None


def is_draw(board: List[List[str]]) -> bool:
    """Board is a draw if it's full and nobody has won."""
    return all(cell != '_' for row in board for cell in row)


def print_board(board: List[List[str]]) -> None:
    for row in board:
        print(' | '.join(row))
    print()


def evaluate(board: List[List[str]]) -> str:
    winner = check_winner(board)
    if winner:
        return f"Player '{winner}' wins!"
    elif is_draw(board):
        return "The game is a draw."
    else:
        return "Game is not finished yet."


def read_board_from_input(n: int = 3) -> List[List[str]]:
    """
    Reads an N x N board from the user.
    Enter each row as space separated symbols, e.g.:  X O _
    """
    print(f"Enter the {n}x{n} board row by row.")
    print("Use 'X', 'O' for the players and '_' for an empty cell.")
    board = []
    for r in range(n):
        while True:
            row = input(f"Row {r + 1}: ").strip().split()
            if len(row) == n and all(ch in ('X', 'O', '_') for ch in row):
                board.append(row)
                break
            print(f"Invalid row. Please enter exactly {n} values from X/O/_.")
    return board


if __name__ == "__main__":
    # ---- Demo with a hardcoded example (matches the classic X-wins case) ----
    demo_board = [
        ['X', 'O', 'X'],
        ['O', 'X', 'O'],
        ['O', 'O', 'X'],
    ]
    print("Demo board:")
    print_board(demo_board)
    print(evaluate(demo_board))
    print()

    # ---- Interactive mode ----
    try:
        user_board = read_board_from_input(3)
        print("\nYour board:")
        print_board(user_board)
        print(evaluate(user_board))
    except EOFError:
        # No interactive input available (e.g. running in a non-interactive shell)
        print("(Skipping interactive input - no stdin available.)")
