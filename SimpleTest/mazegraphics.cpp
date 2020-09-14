/*
 * File: mazegraphics.cpp
 * ----------------------
 * @author Julie Zelenski, Spring 2020
 * Implementation of the graphics support routines. You are
 * welcome to read over this code, but you will not need to edit it.
 */

#include "gwindow.h"
#include <iomanip>  // for setw, setfill
#include "error.h"
#include "gthread.h"
#include "map.h"
#include "mazegraphics.h"
using namespace std;

namespace MazeGraphics {

static bool intializedOnce = false;
static GWindow *window;
static Map<bool, std::string> colors;

struct cellT {
    GRect *square;
    GOval *dot;
    bool marked;
};
static Grid<cellT> cells;

void initialize()
{
    window = new GWindow(1, 1);
    colors[false] = "Dark Gray";
    colors[true] = "White";
    window->setVisible(true);
    window->setWindowTitle("Maze Graphics");
    window->setRepaintImmediately(false);
    window->setAutoRepaint(false);
    window->setExitOnClose(false);
    window->setLocation(10, 10);
    window->setResizable(false);
    intializedOnce = true;
}

void changeDimensions(int numRows, int numCols)
{
    static const int kMinWindowSize = 200, kMaxWindowSize = 800;
    static const int kDefaultCellSize = 10;

    if (!intializedOnce) initialize();
    window->clear();
    cells.clear();
    cells.resize(numRows, numCols);
    int cellSize = kDefaultCellSize;
    if (kDefaultCellSize*min(numRows, numCols) < kMinWindowSize)
        cellSize = min(kMinWindowSize/min(numRows, numCols), kMaxWindowSize/max(numRows, numCols));
    int dotSize = int(cellSize * .6);
    int margin = (cellSize - dotSize)/2;
    window->setCanvasSize(numCols*cellSize, numRows*cellSize);
    for (const auto& loc : cells.locations()) {
        cells[loc].square = new GRect(loc.col * cellSize, loc.row * cellSize, cellSize, cellSize);
        cells[loc].square->setVisible(false);
        cells[loc].dot = new GOval(loc.col * cellSize + margin, loc.row * cellSize + margin, dotSize , dotSize);
        cells[loc].dot->setVisible(false);
        cells[loc].marked = false;
        window->add(cells[loc].square);
        window->add(cells[loc].dot);
    }
}

void drawGrid(const Grid<bool>& g)
{
    if (g.numRows() != cells.numRows() || g.numCols() != cells.numCols()) {
        changeDimensions(g.numRows(), g.numCols());
    }
    for (const auto& loc : cells.locations()) {
        int val = g[loc];
        if (val) {
            cells[loc].square->setVisible(false);
       } else {
            cells[loc].square->setColor(colors[val]);
            cells[loc].square->setFillColor(colors[val]);
            cells[loc].square->setVisible(true);
        }
    }
    GThread::runOnQtGuiThread([] {  window->repaint(); });
}

void highlightPath(Stack<GridLocation> path, string color)
{
    if (!cells.numRows() || !cells.numCols()) error("MazeGraphics asked to highlight path before grid configured.");
    for (auto& c : cells) c.marked = false;
    while (!path.isEmpty()) {
        GridLocation loc = path.pop();
        if (!cells.inBounds(loc)) error("MazeGraphics asked to highlight path location: " + loc.toString() + " that is out of bounds for grid.");
        cells[loc].marked = true;
    }
    for (auto& c : cells) {
        if (c.marked) {
            c.dot->setVisible(true);
            c.dot->setColor(color);
            c.dot->setFillColor(color);
        } else {
            c.dot->setVisible(false);
        }
    }
    GThread::runOnQtGuiThread([] {  window->repaint(); });
}

void printMaze() {
    for (const auto& loc : cells.locations()) {
        char ch = (cells[loc].square->isVisible()) ? '@' : ' ';
        if (cells[loc].marked) ch = '+';
        cout << setw(3) << setfill(' ') << ch;
        if (loc.col == cells.numCols()-1) cout << endl;
    }
}

}
