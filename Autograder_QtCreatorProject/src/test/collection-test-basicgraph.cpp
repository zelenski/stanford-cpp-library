/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "basicgraph.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(BasicGraphTests, "BasicGraph tests");

TIMED_TEST(BasicGraphTests, basicTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");   // TODO: uncomment

    BasicGraph copy = bgraph;

    assertEqualsInt("basicgraph size", 5, bgraph.size());
    assertEqualsInt("basicgraph vertex count", 5, bgraph.getVertexSet().size());
    assertEqualsInt("basicgraph edge count", 6, bgraph.getEdgeSet().size());

    assertTrue("basicgraph contains a", bgraph.containsVertex("a"));
    assertTrue("basicgraph contains b", bgraph.containsVertex("b"));
    assertTrue("basicgraph contains c", bgraph.containsVertex("c"));
    assertTrue("basicgraph contains d", bgraph.containsVertex("d"));
    assertTrue("basicgraph contains e", bgraph.containsVertex("e"));
    assertFalse("basicgraph contains f", bgraph.containsVertex("f"));
    assertFalse("basicgraph contains g", bgraph.containsVertex("g"));

    assertTrue("basicgraph contains edge a -> b", bgraph.containsEdge("a", "b"));
    assertTrue("basicgraph contains edge b -> c", bgraph.containsEdge("b", "c"));
    assertTrue("basicgraph contains edge c -> e", bgraph.containsEdge("c", "e"));
    assertFalse("basicgraph contains edge a -> c", bgraph.containsEdge("a", "c"));
    assertFalse("basicgraph contains edge b -> a", bgraph.containsEdge("b", "a"));

    bgraph.removeArc("a", "b");
    assertEqualsInt("basicgraph edge count", 5, bgraph.getEdgeSet().size());
    assertFalse("basicgraph contains edge a -> b", bgraph.containsEdge("a", "b"));
    assertFalse("basicgraph contains edge a -> b", bgraph.containsEdge("a", "b"));

    // error("shiiiiiiit");

    bgraph.removeVertex("b");
    assertEqualsInt("basicgraph vertex count", 4, bgraph.getVertexSet().size());
    assertEqualsInt("basicgraph edge count", 2, bgraph.getEdgeSet().size());
    assertFalse("basicgraph contains edge a -> b", bgraph.containsEdge("a", "b"));
    assertFalse("basicgraph contains edge b -> c", bgraph.containsEdge("b", "c"));
    assertFalse("basicgraph contains edge b -> d", bgraph.containsEdge("b", "d"));
    assertFalse("basicgraph contains edge c -> b", bgraph.containsEdge("c", "b"));

    bgraph = copy;
    bgraph.clearEdges();
    assertEqualsInt("basicgraph size", 5, bgraph.size());
    assertEqualsInt("basicgraph vertex count", 5, bgraph.getVertexSet().size());
    assertEqualsInt("basicgraph edge count", 0, bgraph.getEdgeSet().size());

    bgraph = copy;
    bgraph.clear();
    assertEqualsInt("basicgraph size", 0, bgraph.size());
    assertEqualsInt("basicgraph vertex count", 0, bgraph.getVertexSet().size());
    assertEqualsInt("basicgraph edge count", 0, bgraph.getEdgeSet().size());
}

TIMED_TEST(BasicGraphTests, compareTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");

    BasicGraph bgraph2;
    bgraph2.addNode("a");
    bgraph2.addNode("b");
    bgraph2.addNode("c");
    bgraph2.addNode("d");
    bgraph2.addNode("e");
    bgraph2.addNode("f");
    bgraph2.addArc("a", "b");
    bgraph2.addArc("a", "d");
    bgraph2.addArc("b", "c");
    bgraph2.addArc("b", "d");
    bgraph2.addArc("c", "b");
    bgraph2.addArc("b", "e");

    BasicGraph bgraph3;

    compareTestHelper(bgraph, bgraph2, "BasicGraph", /* compareTo */ -1);
    compareTestHelper(bgraph, bgraph3, "BasicGraph", /* compareTo */  1);
    compareTestHelper(bgraph2, bgraph, "BasicGraph", /* compareTo */  1);
    compareTestHelper(bgraph, bgraph, "BasicGraph", /* compareTo */  0);

    Set<BasicGraph> sbgraph {bgraph, bgraph2, bgraph3};
    assertEqualsString("sbgraph", "{{}, {a, b, c, d, e, a -> b, a -> d, b -> c, b -> d, c -> b, c -> e}, {a, b, c, d, e, f, a -> b, a -> d, b -> c, b -> d, b -> e, c -> b}}", sbgraph.toString());
}

TIMED_TEST(BasicGraphTests, forEachTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    Queue<std::string> expected {"a", "b", "c", "d", "e"};
    for (Vertex* node : bgraph) {
        std::string exp = expected.dequeue();
        assertEqualsString("BasicGraph foreach vertex name", exp, node->name);
    }
}

TIMED_TEST(BasicGraphTests, frontBackTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    assertEqualsString("BasicGraph front", "a", bgraph.front()->name);
    assertEqualsString("BasicGraph back",  "e", bgraph.back()->name);
}

TIMED_TEST(BasicGraphTests, hashCodeTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    assertEqualsInt("hashcode of self basicgraph", hashCode(bgraph), hashCode(bgraph));

    BasicGraph copy = bgraph;
    assertEqualsInt("hashcode of copy basicgraph", hashCode(bgraph), hashCode(copy));

    BasicGraph bgraph2;
    bgraph2.addNode("a");
    bgraph2.addNode("b");
    bgraph2.addNode("c");
    bgraph2.addNode("d");
    bgraph2.addNode("e");
    bgraph2.addNode("f");
    bgraph2.addArc("a", "b");
    bgraph2.addArc("a", "d");
    bgraph2.addArc("b", "c");
    bgraph2.addArc("b", "d");
    bgraph2.addArc("c", "b");
    bgraph2.addArc("b", "e");
    assertNotEqualsInt("hashcode of unequal basicgraph", hashCode(bgraph), hashCode(bgraph2));

    HashSet<BasicGraph> hashbgraph;
    hashbgraph.add(bgraph);
    hashbgraph.add(bgraph2);
    assertEqualsInt("hashset of basicgraph size", 2, hashbgraph.size());

    Vertex* v1 = new Vertex("v1");
    Vertex* v2 = new Vertex("v2");
    Vertex* v3 = new Vertex("v3");
    Vertex* v4 = new Vertex("v4");
    Vertex* v5 = new Vertex("v5");
    Vertex* v6 = new Vertex("v6");

    HashSet<HashSet<Vertex*>> hhset2;
    HashSet<Vertex*> hset3;
    hset3.add(v1);
    hset3.add(v2);
    hset3.add(v3);
    hhset2.add(hset3);
    HashSet<Vertex*> hset4;
    hset4.add(v4);
    hset4.add(v5);
    hset4.add(v6);
    hhset2.add(hset4);
    assertEqualsInt("hashset of hashset of vertex size", 2, hhset2.size());

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete v5;
    delete v6;
}

TIMED_TEST(BasicGraphTests, initializerListTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph graph {"a", "b", "c", "d"};
    assertEqualsString("init list BasicGraph", "{a, b, c, d}", graph.toString());
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(BasicGraphTests, iteratorVersionTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");

    try {
        for (Vertex* v : bgraph) {
            bgraph.removeNode(v);
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR

//static void temp(BasicGraph /*bg*/) {
//    // empty
//}

//static Set<Edge*> temp2() {
//    BasicGraph bgraph;
//    bgraph.addNode("a");
//    bgraph.addNode("b");
//    bgraph.addNode("c");
//    bgraph.addNode("d");
//    bgraph.addNode("e");
//    bgraph.addArc("a", "b");
//    bgraph.addArc("a", "d");
//    bgraph.addArc("b", "c");
//    bgraph.addArc("b", "d");
//    bgraph.addArc("c", "b");
//    bgraph.addArc("c", "e");

//    Set<Edge*> edges;
//    for (Vertex* v : bgraph.getVertexSet()) {
//        edges += v->arcs;
//    }

////    {
////        BasicGraph bgraph2 = bgraph;
////        temp(bgraph2);

////        // let it fall out of scope; should free memory (oops!)
////    }

//    return edges;
//}

//TIMED_TEST(BasicGraphTests, operatorEqualsTest_BasicGraph, TEST_TIMEOUT_DEFAULT) {
//    Set<Edge*> edges = temp2();

//    // this code should crash
//    int count = 0;
//    for (Edge* e : edges) {
//        if (e->start || e->finish) {
//            std::cout << "e is " << e->start->name
//                      << " -> " << e->finish->name << std::endl;
//            count++;
//        }
//    }

//    assertPass("Graph didn't crash after operator =");
//}
