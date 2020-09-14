#pragma once

#include "stack.h"
#include "grid.h"

// Prototypes to be shared with other modules

bool checkSolution(Grid<bool>& g, Stack<GridLocation> path);

bool readMazeFile(std::string filename, Grid<bool>& maze);

Stack<GridLocation> solveMaze(Grid<bool>& maze);
