/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "grid.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "set.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

/*
 * Force instantiation of the template on a type to ensure that we don't have anything
 * insidious lurking that just didn't get compiled.
 */
template class Grid<int>;
template class Grid<std::string>;

PROVIDED_TEST("Grid, compare") {
    Grid<int> grid1;
    grid1.resize(2, 2);
    Grid<int> grid2;
    grid2.resize(2, 3);
    Grid<int> grid3;
    grid3.resize(3, 2);
    Grid<int> grid4;

    /* Comparison is lexicographic by rows, then cols, then
     * elements.
     */
    testCompareOperators(grid1, grid2, LessThan);
    testCompareOperators(grid2, grid1, GreaterThan);
    testCompareOperators(grid1, grid3, LessThan);
    testCompareOperators(grid3, grid1, GreaterThan);
    testCompareOperators(grid2, grid3, LessThan);
    testCompareOperators(grid3, grid2, GreaterThan);
    testCompareOperators(grid1, grid1, EqualTo);

    Set<Grid<int>> sgrid {grid1, grid2, grid3, grid4};
    EXPECT_EQUAL( "{{}, {{0, 0}, {0, 0}}, {{0, 0, 0}, {0, 0, 0}}, {{0, 0}, {0, 0}, {0, 0}}}", sgrid.toString());
}

PROVIDED_TEST("Grid, forEach") {
    Grid<int> grid(4, 2);
    grid.fill(42);
    grid[2][0] = 17;
    grid[3][1] = 0;
    Queue<int> expected {42, 42, 42, 42, 17, 42, 42, 0};
    for (int n : grid) {
        int exp = expected.dequeue();
        EXPECT_EQUAL( exp, n);
    }
}

PROVIDED_TEST("Grid, access corners") {
    Grid<int> grid {{10, 20, 30}, {40, 50, 60}};
    EXPECT_EQUAL( 10, grid[0][0]);
    EXPECT_EQUAL(  60, grid[grid.numRows()-1][grid.numCols()-1]);
}

PROVIDED_TEST("Grid, hashCode") {
    Grid<int> grid(2, 3);
    grid.fill(42);
    EXPECT_EQUAL( hashCode(grid), hashCode(grid));

    Grid<int> copy = grid;
    EXPECT_EQUAL( hashCode(grid), hashCode(copy));

    Grid<int> empty;   // empty
    HashSet<Grid<int> > hashgrid {grid, copy, empty, empty};

    EXPECT_EQUAL( 2, hashgrid.size());
}

PROVIDED_TEST("Grid, initializerList") {
    Grid<int> grid {{10, 20, 30}, {40, 50, 60}};
    EXPECT_EQUAL( 2, grid.numRows());
    EXPECT_EQUAL( 3, grid.numCols());
    EXPECT_EQUAL( 6, grid.size());
    EXPECT_EQUAL( 10, grid[0][0]);
    EXPECT_EQUAL( 20, grid[0][1]);
    EXPECT_EQUAL( 30, grid[0][2]);
    EXPECT_EQUAL( 40, grid[1][0]);
    EXPECT_EQUAL( 50, grid[1][1]);
    EXPECT_EQUAL( 60, grid[1][2]);
}

void fillDuring(Grid<int>& g) { for (int m : g) g.fill(0); }

PROVIDED_TEST("Grid, error on modify during iterate") {
    Grid<int> rowSort = {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };   EXPECT_ERROR(fillDuring(rowSort));
}


PROVIDED_TEST("Grid, mapAll") {
    Grid<int> rowSort = {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };

    int lastRowVal = -1;
    rowSort.mapAll([&](const int& val) {
        EXPECT( lastRowVal < val);
        lastRowVal = val;
    });
}

PROVIDED_TEST("Grid, randomElement") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Grid<std::string> grid;
    grid.resize(2, 3);
    grid[0][0] = "a";
    grid[0][1] = "b";
    grid[0][2] = "c";
    grid[1][0] = "d";
    grid[1][1] = "e";
    grid[1][2] = "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(grid);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0);
    }
}
