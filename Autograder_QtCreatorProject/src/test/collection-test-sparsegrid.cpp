/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "sparsegrid.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(SparseGridTests, "SparseGrid tests");

/* Force instantiation of the template on a type to ensure that we don't have anything
 * invidious lurking that just didn't get compiled.
 */
template class SparseGrid<int>;
template class SparseGrid<std::string>;

TIMED_TEST(SparseGridTests, compareTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> sgrid1;
    sgrid1.resize(2, 2);
    SparseGrid<int> sgrid2;
    sgrid2.resize(2, 3);
    SparseGrid<int> sgrid3;
    sgrid3.resize(3, 2);
    compareTestHelper(sgrid1, sgrid2, "SparseGrid", /* compareTo */ -1);
    compareTestHelper(sgrid2, sgrid1, "SparseGrid", /* compareTo */  1);
    compareTestHelper(sgrid1, sgrid3, "SparseGrid", /* compareTo */ -1);
    compareTestHelper(sgrid3, sgrid1, "SparseGrid", /* compareTo */  1);
    compareTestHelper(sgrid2, sgrid3, "SparseGrid", /* compareTo */  1);
    compareTestHelper(sgrid3, sgrid2, "SparseGrid", /* compareTo */ -1);
    compareTestHelper(sgrid1, sgrid1, "SparseGrid", /* compareTo */  0);

    Set<SparseGrid<int> > ssgrid {sgrid1, sgrid2, sgrid3};
    assertEqualsString("ssgrid", "{{}, 2 x 2, {}, 3 x 2, {}, 2 x 3}", ssgrid.toString());
}

TIMED_TEST(SparseGridTests, forEachTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> grid(4, 2);
    grid.fill(42);
    grid[2][0] = 17;
    grid[3][1] = 0;
    Queue<int> expected {42, 42, 42, 42, 17, 42, 42, 0};
    for (int n : grid) {
        int exp = expected.dequeue();
        assertEqualsInt("SparseGrid foreach", exp, n);
    }
}

TIMED_TEST(SparseGridTests, frontBackTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> grid {{10, 20, 30}, {40, 50, 60}};
    assertEqualsInt("SparseGrid front", 10, grid.front());
    assertEqualsInt("SparseGrid back",  60, grid.back());
}

TIMED_TEST(SparseGridTests, hashCodeTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> grid(2, 3);
    grid.fill(42);
    assertEqualsInt("hashcode of self SparseGrid", hashCode(grid), hashCode(grid));

    SparseGrid<int> copy = grid;
    assertEqualsInt("hashcode of copy SparseGrid", hashCode(grid), hashCode(copy));

    SparseGrid<int> empty;   // empty
    HashSet<SparseGrid<int> > hashgrid {grid, copy, empty, empty};

    assertEqualsInt("hashset of SparseGrid size", 2, hashgrid.size());
}

TIMED_TEST(SparseGridTests, initializerListTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> grid {{10, 20, 30}, {40, 50, 60}};
    assertEqualsInt("init list SparseGrid numRows", 2, grid.numRows());
    assertEqualsInt("init list SparseGrid numCols", 3, grid.numCols());
    assertEqualsInt("init list SparseGrid size", 6, grid.size());
    assertEqualsInt("init list SparseGrid[0][0]", 10, grid[0][0]);
    assertEqualsInt("init list SparseGrid[0][1]", 20, grid[0][1]);
    assertEqualsInt("init list SparseGrid[0][2]", 30, grid[0][2]);
    assertEqualsInt("init list SparseGrid[1][0]", 40, grid[1][0]);
    assertEqualsInt("init list SparseGrid[1][1]", 50, grid[1][1]);
    assertEqualsInt("init list SparseGrid[1][2]", 60, grid[1][2]);
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(SparseGridTests, iteratorVersionTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    SparseGrid<int> grid {{10, 20, 30}, {40, 50, 60}};
    try {
        for (int n : grid) {
            if (n % 2 == 0) {
                grid.fill(42);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

TIMED_TEST(SparseGridTests, randomElementTest_Grid, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    SparseGrid<std::string> grid;
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
        assertTrue("must choose " + s + " sometimes", counts[s] > 0);
    }
}
