# Matrix Number Connection Game 🧩

## Description
This project is a terminal-based, interactive puzzle game written in C. The main objective is to connect matching numbers on an NxN grid by drawing paths between them without overlapping. The game features a robust architecture including dynamic memory management, a recursive auto-solver, and a persistent leaderboard system.

## Key Features
* **Two Gameplay Modes:**
  * **Manual Mode:** Players input source and destination coordinates to draw paths. Includes a dynamic "Undo" feature to revert mistakes.
  * **Automatic Mode:** An integrated Auto-Solver uses a recursive backtracking algorithm to automatically find the correct paths for all numbers.
* **Board Generation:** Play on randomly generated, guaranteed-solvable NxN grids, or load custom matrix configurations from a text file.
* **Dynamic Memory & Undo System:** Board states are tracked using dynamic structs (`malloc`, `realloc`, `free`), allowing players to undo moves in manual mode safely.
* **Leaderboard & Scoring System:** The game dynamically calculates scores based on grid size, game mode, number of wrong moves, and empty blocks left. Scores are persistently saved and ranked in `user_score.csv`.

## Compilation & Execution
This project can be compiled and run using any standard C compiler (such as GCC).

```bash
# Compile the source code
gcc 24011023.c -o matrix_game

# Run the executable
./matrix_game
```

## How to Play
1. **Start:** Run the game and choose whether to generate a random board or load one from a file.
2. **Setup:** Enter your username and desired matrix size (N).
3. **Play:** * Select **Manual Mode** to play yourself. Enter the X and Y coordinates of the source number, followed by the destination coordinates. 
   * Select **Undo Move** if you make a mistake (Note: This slightly reduces your score).
   * Select **Automatic Mode** to watch the algorithm solve the puzzle.
4. **Endgame:** Once the board is solved, your final score is calculated and appended to the leaderboard. Check the "Show User Scores" option in the main menu to see your ranking!
