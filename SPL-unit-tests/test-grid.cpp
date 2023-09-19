/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "grid.h"
#include "hashset.h"
#include "queue.h"
#include "set.h"
#include "common.h"
#include "SimpleTest.h"
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
    EXPECT_EQUAL(sgrid.toString(), "{{}, {{0, 0}, {0, 0}}, {{0, 0, 0}, {0, 0, 0}}, {{0, 0}, {0, 0}, {0, 0}}}");
}

PROVIDED_TEST("Grid, forEach") {
    Grid<int> grid(4, 2);
    grid.fill(42);
    grid[2][0] = 17;
    grid[3][1] = 0;

    /* Version 1: Range-based for loop. */
    Queue<int> expected {42, 42, 42, 42, 17, 42, 42, 0};
    for (int n : grid) {
        int exp = expected.dequeue();
        EXPECT_EQUAL( exp, n);
    }

    /* Make sure we snagged everything. */
    EXPECT(expected.isEmpty());

    /* Version 2: Explicit .locations() loop. */
    expected = {42, 42, 42, 42, 17, 42, 42, 0};
    Queue<GridLocation> locs = { {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}, {3, 0}, {3, 1} };
    for (GridLocation loc: grid.locations()) {
        EXPECT_EQUAL(loc, locs.dequeue());
        EXPECT_EQUAL(grid[loc], expected.dequeue());
    }
    EXPECT(expected.isEmpty());

    /* Version 3: .locations() loop in col-major order. */
    expected = {42, 42, 17, 42, 42, 42, 42, 0};
    locs = { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1} };
    for (GridLocation loc: grid.locations(false)) {
        EXPECT_EQUAL(loc, locs.dequeue());
        EXPECT_EQUAL(grid[loc], expected.dequeue());
    }
    EXPECT(expected.isEmpty());
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

void fillDuring(Grid<int>& g) {
    for (int m : g) {
        (void)m;
        g.fill(0);
    }
}

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

PROVIDED_TEST("Grid<T>, locations() GridLocationRange iterator, begin/end/distance") {
    Grid<int> grid;
    GridLocationRange range;
    Set<GridLocation> set;

    /* 5 x 1 */
    grid = Grid<int>(5, 1);
    range = grid.locations();
    set.clear();
    for (auto loc: range)
        set.add(loc);
    EXPECT_EQUAL(set, {{0,0},{1,0},{2,0},{3,0},{4,0}});
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), set.size());

    /* 1 x 5 */
    grid = Grid<int>(1, 5);
    range = grid.locations();
    set.clear();
    for (auto loc: range)
        set.add(loc);
    EXPECT_EQUAL(set, {{0,0},{0,1},{0,2},{0,3},{0,4}});
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), set.size());

    /* 2 x 3 */
    grid = Grid<int>(2, 3);
    range = grid.locations();
    set.clear();
    for (auto loc: range)
        set.add(loc);
    EXPECT_EQUAL(set, {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}});
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), set.size());
}

PROVIDED_TEST("Grid<T>, locations() works on an empty Grid.") {
    Grid<int> empty;
    GridLocationRange range;

    /* 0 x 0 */
    empty = Grid<int>(0, 0);
    range = empty.locations();
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), 0);

    /* 0 x N */
    empty = Grid<int>(0, 137);
    range = empty.locations();
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), 0);

    /* N x 0 */
    empty = Grid<int>(137, 0);
    range = empty.locations();
    EXPECT_EQUAL(std::distance(range.begin(), range.end()), 0);
}

PROVIDED_TEST("GridLocationRange, contains") {
    GridLocationRange range(1, 1, 2, 2);
    EXPECT(range.contains({1, 1}));
    EXPECT(range.contains({1, 2}));
    EXPECT(range.contains({2, 1}));
    EXPECT(range.contains({2, 2}));

    EXPECT(!range.contains({1, 0}));
    EXPECT(!range.contains({1, 3}));
    EXPECT(!range.contains({2, 0}));
    EXPECT(!range.contains({2, 3}));
    EXPECT(!range.contains({0, 1}));
    EXPECT(!range.contains({0, 2}));
    EXPECT(!range.contains({3, 1}));
    EXPECT(!range.contains({3, 2}));
    EXPECT(!range.contains({0, 0}));
    EXPECT(!range.contains({0, 3}));
    EXPECT(!range.contains({3, 0}));
    EXPECT(!range.contains({3, 3}));

    /* Now, try this with empty ranges. */
    range = GridLocationRange(1, 1, 0, 1); // No rows
    EXPECT(!range.contains({0, 0}));
    EXPECT(!range.contains({0, 1}));
    EXPECT(!range.contains({1, 0}));
    EXPECT(!range.contains({1, 1}));
    EXPECT(range.isEmpty());

    range = GridLocationRange(1, 1, 1, 0); // No columns
    EXPECT(!range.contains({0, 0}));
    EXPECT(!range.contains({0, 1}));
    EXPECT(!range.contains({1, 0}));
    EXPECT(!range.contains({1, 1}));
    EXPECT(range.isEmpty());

    range = GridLocationRange(1, 1, 0, 0); // No rows, columns
    EXPECT(!range.contains({0, 0}));
    EXPECT(!range.contains({0, 1}));
    EXPECT(!range.contains({1, 0}));
    EXPECT(!range.contains({1, 1}));
    EXPECT(range.isEmpty());
}


PROVIDED_TEST("GridLocationRange dimensions,size,isEmpty ") {
    Grid<int> grid;
    GridLocationRange range;

    /* 5 x 1 */
    grid = Grid<int>(5, 1);
    range = grid.locations();
    EXPECT(!range.isEmpty());
    EXPECT_EQUAL(range.numRows(), grid.numRows());
    EXPECT_EQUAL(range.numCols(), grid.numCols());
    EXPECT_EQUAL(range.size(), std::distance(range.begin(), range.end()));

    /* 1 x 5 */
    grid = Grid<int>(1, 5);
    range = grid.locations();
    EXPECT(!range.isEmpty());
    EXPECT_EQUAL(range.numRows(), grid.numRows());
    EXPECT_EQUAL(range.numCols(), grid.numCols());
    EXPECT_EQUAL(range.size(), std::distance(range.begin(), range.end()));

    /* 2 x 3 */
    grid = Grid<int>(2, 3);
    range = grid.locations();
    EXPECT(!range.isEmpty());
    EXPECT_EQUAL(range.numRows(), grid.numRows());
    EXPECT_EQUAL(range.numCols(), grid.numCols());
    EXPECT_EQUAL(range.size(), std::distance(range.begin(), range.end()));

    GridLocationRange origin;
    EXPECT(!origin.isEmpty());
    EXPECT_EQUAL(origin.numRows(), 1);
    EXPECT_EQUAL(origin.numCols(),1);
    EXPECT_EQUAL(origin.size(), 1);
    EXPECT(origin.contains({0, 0}));

    GridLocationRange empty(10, 5, 2, 1);
    EXPECT(empty.isEmpty());
    EXPECT_EQUAL(empty.numRows(), 0);
    EXPECT_EQUAL(empty.numCols(),0);
    EXPECT_EQUAL(empty.size(), 0);
    EXPECT(!empty.contains(empty.startLocation()));
    EXPECT(!empty.contains(empty.endLocation()));
}
