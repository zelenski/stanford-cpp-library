// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "vector.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;

bool areConnected(GridLocation one, GridLocation two)
{
    return (abs(one.row-two.row) + abs(one.col-two.col)) == 1;
}

bool checkSolution(Grid<bool>& g, Stack<GridLocation> path)
{
    GridLocation entry = {0, 0};
    GridLocation exit = {g.numRows()-1,  g.numCols()-1};
    GridLocation cur = path.pop();
    if (cur != exit) error("path does not end at exit");
    while (!path.isEmpty()) {
        GridLocation prev = path.pop();
        if (!g[prev]) error("path walks through wall");
        if (!areConnected(prev, cur)) error("path teleports");
        cur = prev;
    }
    if (cur != entry) error("path does not begin at entry");
    return true;
}

/*
 * The given readMaze function correctly reads a well-formed
 * maze from a file. However, there are cases of malformed
 * inputs which it does not correctly handle. As part of the
 * assignment warmup, you will add complete error-checking to
 so that the function is fully robust.
 */
bool readMazeFile(string filename, Grid<bool>& maze)
{
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);
    string lastLine = lines.removeBack(); // last line is solution if present

    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {
                maze[r][c] = false;
            } else if (ch == '-') {
                maze[r][c] = true;
            }
        }
    }
    Stack<GridLocation> solution;
    istringstream istr(lastLine); // Stack read does its own error-checking
    if (istr >> solution) {// if successfully read
        return checkSolution(maze, solution);
    }
    return true; // else no solution; nothing more to check
 }


GridLocation neighbor(GridLocation loc, int dRow, int dCol)
{
    GridLocation next = {loc.row + dRow, loc.col + dCol};
    return next;
}

Vector<GridLocation> neighbors(Grid<bool>&g, GridLocation loc, int hop = 1)
{
    Vector<GridLocation> v = { neighbor(loc, -hop,0), neighbor(loc, hop,0),
                               neighbor(loc, 0,-hop), neighbor(loc, 0,hop)};
    for (int i = 0; i < v.size(); ) {
        if (!g.inBounds(v[i]))
            v.remove(i);
        else
            i++;
    }
    return v;
}

Stack<GridLocation> solveMaze(Grid<bool> &maze)
{
    MazeGraphics::drawGrid(maze);
    GridLocation entry = {0, 0};
    GridLocation exit = {maze.numRows()-1, maze.numCols()-1};

    Queue<Stack<GridLocation>> q;
    Stack<GridLocation> curPath;
    Set<GridLocation> visited;

    curPath.push(entry);
    q.enqueue(curPath);
    while (!q.isEmpty()) {
        curPath = q.dequeue();
        GridLocation pathEnd = curPath.peek();
        visited.add(pathEnd);
        MazeGraphics::highlightPath(curPath, "blue");
        if (pathEnd == exit) {
            return curPath;
        }
        for (auto n : neighbors(maze, pathEnd)) {
            if (!visited.contains(n) && maze[n] != false) {
                auto extPath = curPath;
                extPath.push(n);
                q.enqueue(extPath);
            }
        }
    }
    error("Should never get here, maze had no solution");
}


void createNewMazeAB(Grid<bool> &maze)
{
    int numRows = 2*randomInteger(5, 12) + 1;
    int numCols = 2*randomInteger(10, 20) + 1;
    maze.resize(numRows, numCols);

    Set<GridLocation> included;
    for (GridLocation loc: maze.locations()) { maze[loc] = ((loc.row % 2) || (loc.col % 2))? false: true; }
    GridLocation upperLeft = {0,0};
    GridLocation cur = upperLeft;
    included.add(upperLeft);

    while (included.size() != (numRows+1)*(numCols+1)/4) {
        Vector<GridLocation> un = neighbors(maze, cur, 2);
        GridLocation chosen = randomElement(un);
        if (!included.contains(chosen)) {
            maze[(cur.row + chosen.row)/2][(cur.col+chosen.col)/2] = true;
            included.add(chosen);
            MazeGraphics::drawGrid(maze);
        }
        cur = chosen;
    }
}

void writeMaze(Grid<bool> &g, Stack<GridLocation> &soln)
{
    ofstream out(string("res/") + integerToString(g.numRows()) + "x" + integerToString(g.numCols()));
    for (int r = 0; r < g.numRows(); r++) {
        for (int c = 0; c < g.numCols(); c++) {
            out << (g[r][c] ? '-' : '@');
        }
        out << endl;
    }
    out << soln << endl;
}


void doMaze(string filename)
{
    Grid<bool> maze;

    readMazeFile(filename, maze);
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "green");
    checkSolution(maze, soln);
}

void doMaze()
{
    Grid<bool> maze;

    createNewMazeAB(maze);
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::highlightPath(soln, "green");
    checkSolution(maze, soln);
    writeMaze(maze, soln);
    MazeGraphics::printMaze();
}

/*
    Vector<string> allfiles = listDirectory("res");
    for (string f : allfiles) {
        if (stringContains(f, ".maze"))
            doMaze("res/" + f);
    }*/

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> g = {{true, false}, {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT(checkSolution(g, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> g = {{true, false}, {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

   // checkSolution(g, go_through_wall);
    EXPECT_ERROR(checkSolution(g, not_end_at_exit));
    EXPECT_ERROR(checkSolution(g, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(g, go_through_wall));
    EXPECT_ERROR(checkSolution(g, teleport));

}

PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> g = {{true, false}, {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(g, not_end_at_exit));
    EXPECT_ERROR(checkSolution(g, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(g, go_through_wall));
    EXPECT_ERROR(checkSolution(g, teleport));
}


PROVIDED_TEST("Test readMaze on example file") {
    Grid<bool> g;

    EXPECT(readMazeFile("res/5x7.maze", g));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2") {
    Grid<bool> g;

    EXPECT(readMazeFile("res/2x2.maze", g));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

