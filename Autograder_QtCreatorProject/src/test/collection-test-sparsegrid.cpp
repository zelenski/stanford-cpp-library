/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "sparsegrid.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(SparseGridTests, "SparseGrid tests");

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
    SparseGrid<int> sgrid(4, 2);
    sgrid.fill(42);
    std::cout << "sparsegrid: " << sgrid << std::endl;
    for (int n : sgrid) {
        std::cout << n << std::endl;
    }
    
    Vector<int> v;
    v += 10, 20, 30, 40;
    std::cout << "vector: " << v << std::endl;
    for (int n : v) {
        std::cout << n << std::endl;
    }
    
    std::cout << "done!" << std::endl;
}

TIMED_TEST(SparseGridTests, hashCodeTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    HashSet<SparseGrid<int> > hashsparsegrid;
    SparseGrid<int> sparsegrid(2, 2);
    sparsegrid.fill(8);
    hashsparsegrid.add(sparsegrid);
    std::cout << "hashset of sparsegrid: " << hashsparsegrid << std::endl;
}

TIMED_TEST(SparseGridTests, initializerListTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};
    std::initializer_list<std::string> lexlist2 = {"twenty", "fifty"};
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    SparseGrid<int> sgrid {{1, 2, 3}, {4, 5, 6}};
    std::cout << "init list SparseGrid = " << sgrid << std::endl;
}

TIMED_TEST(SparseGridTests, randomElementTest_SparseGrid, TEST_TIMEOUT_DEFAULT) {
    Map<std::string, int> counts;
    int RUNS = 200;

    SparseGrid<std::string> sgrid;
    sgrid.resize(4, 3);
    sgrid[0][0] = "a";
    sgrid[0][1] = "b";
    sgrid[1][2] = "c";   // row 2 is blank
    sgrid[3][0] = "d";
    sgrid[1][1] = "e";
    sgrid[1][2] = "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(sgrid);
        std::cout << s << " ";
        counts[s]++;
    }
}
