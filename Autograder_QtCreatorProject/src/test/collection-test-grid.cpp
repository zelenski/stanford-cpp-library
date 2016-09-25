/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "grid.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(GridTests, "Grid tests");

TIMED_TEST(GridTests, compareTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Grid<int> grid1;
    grid1.resize(2, 2);
    Grid<int> grid2;
    grid2.resize(2, 3);
    Grid<int> grid3;
    grid3.resize(3, 2);
    Grid<int> grid4;
    compareTestHelper(grid1, grid2, "Grid", /* compareTo */ -1);
    compareTestHelper(grid2, grid1, "Grid", /* compareTo */  1);
    compareTestHelper(grid1, grid3, "Grid", /* compareTo */ -1);
    compareTestHelper(grid3, grid1, "Grid", /* compareTo */  1);
    compareTestHelper(grid2, grid3, "Grid", /* compareTo */  1);
    compareTestHelper(grid3, grid2, "Grid", /* compareTo */ -1);
    compareTestHelper(grid1, grid1, "Grid", /* compareTo */  0);

    Set<Grid<int> > sgrid {grid1, grid2, grid3, grid4};
    assertEqualsString("sgrid", "{{}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}}, {{0, 0, 0}, {0, 0, 0}}}", sgrid.toString());
}

TIMED_TEST(GridTests, forEachTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Grid<int> grid(4, 2);
    grid.fill(42);
    grid[2][0] = 17;
    grid[3][1] = 0;
    Queue<int> expected {42, 42, 42, 42, 17, 42, 42, 0};
    for (int n : grid) {
        int exp = expected.dequeue();
        assertEqualsInt("Grid foreach", exp, n);
    }
}

TIMED_TEST(GridTests, hashCodeTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Grid<int> grid(2, 3);
    grid.fill(42);
    assertEqualsInt("hashcode of self grid", hashCode(grid), hashCode(grid));

    Grid<int> copy = grid;
    assertEqualsInt("hashcode of copy grid", hashCode(grid), hashCode(copy));

    Grid<int> empty;   // empty
    HashSet<Grid<int> > hashgrid {grid, copy, empty, empty};

    assertEqualsInt("hashset of grid size", 2, hashgrid.size());
}

TIMED_TEST(GridTests, initializerListTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Grid<int> grid {{10, 20, 30}, {40, 50, 60}};
    assertEqualsInt("init list Grid numRows", 2, grid.numRows());
    assertEqualsInt("init list Grid numCols", 3, grid.numCols());
    assertEqualsInt("init list Grid size", 6, grid.size());
    assertEqualsInt("init list Grid[0][0]", 10, grid[0][0]);
    assertEqualsInt("init list Grid[0][1]", 20, grid[0][1]);
    assertEqualsInt("init list Grid[0][2]", 30, grid[0][2]);
    assertEqualsInt("init list Grid[1][0]", 40, grid[1][0]);
    assertEqualsInt("init list Grid[1][1]", 50, grid[1][1]);
    assertEqualsInt("init list Grid[1][2]", 60, grid[1][2]);
}

TIMED_TEST(GridTests, randomElementTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

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

    assertTrue("must choose a sometimes", counts["a"] > 0);
    assertTrue("must choose b sometimes", counts["b"] > 0);
    assertTrue("must choose c sometimes", counts["c"] > 0);
    assertTrue("must choose d sometimes", counts["d"] > 0);
    assertTrue("must choose e sometimes", counts["e"] > 0);
    assertTrue("must choose f sometimes", counts["f"] > 0);
}
