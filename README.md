Implementation of recursive backtracking alorithm, that solves unfinished sudoku board.
Algorithm tries a number on an open space and then recurively calls itself for all other open spaces, when no number 1-9 fits in the column, the alorithm comes comes back one step from recursion and tries another nummber. 
This will ultimately result in finding the right solution or returns false, if there is no viable solution.

The time comlexity of this algorithm is O(9^(n*n)) but in reality this algorithm will be much faster than naive solution, since it will rule out options systematically.

The user can input grid by hand, or load it from a file.
The algorithm solves the sudoku.
If the user wishes to try and solve the sudoku, it can, but there are also options to only print the finished sudoku or export the finished sudoku to user specified file.
