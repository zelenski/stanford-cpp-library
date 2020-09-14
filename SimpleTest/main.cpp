#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
#include "search.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

int main()
{
    cout << "hello" << endl;
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    Grid<bool> maze;
    if (readMazeFile("res/13x39.maze", maze)) {
        solveMaze(maze);
    }

    searchEngine("res/website.txt");

    cout << "All done, exiting" << endl;
    return 0;
}

