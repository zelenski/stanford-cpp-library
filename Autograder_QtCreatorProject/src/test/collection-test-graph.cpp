/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "graph.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(GraphTests, "Graph tests");

// for testing Graph class
struct DumbNode;
struct DumbEdge;

struct DumbNode {
    std::string name;
    Set<DumbEdge*> arcs;
    double cost;
};

struct DumbEdge {
    DumbNode* start;
    DumbNode* finish;
    double cost;
};

TIMED_TEST(GraphTests, basicTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");

    Graph<DumbNode, DumbEdge> copy = graph;

    assertEqualsInt("graph size", 5, graph.size());
    assertEqualsInt("graph node count", 5, graph.getNodeSet().size());
    assertEqualsInt("graph arc count", 6, graph.getArcSet().size());

    assertNotNull("graph contains a", graph.getNode("a"));
    assertNotNull("graph contains b", graph.getNode("b"));
    assertNotNull("graph contains c", graph.getNode("c"));
    assertNotNull("graph contains d", graph.getNode("d"));
    assertNotNull("graph contains e", graph.getNode("e"));
    assertNull("graph contains f", graph.getNode("f"));
    assertNull("graph contains g", graph.getNode("g"));

    graph.removeArc("a", "b");
    assertEqualsInt("graph arc count", 5, graph.getArcSet().size());

    graph.removeNode("b");
    assertEqualsInt("graph node count", 4, graph.getNodeSet().size());
    assertEqualsInt("graph arc count", 2, graph.getArcSet().size());

    graph = copy;
    graph.clear();
    assertEqualsInt("graph size", 0, graph.size());
    assertEqualsInt("graph node count", 0, graph.getNodeSet().size());
    assertEqualsInt("graph arc count", 0, graph.getArcSet().size());
}

TIMED_TEST(GraphTests, compareTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");

    Graph<DumbNode, DumbEdge> graph2;
    graph2.addNode("a");
    graph2.addNode("b");
    graph2.addNode("c");
    graph2.addNode("d");
    graph2.addNode("e");
    graph2.addNode("f");
    graph2.addArc("a", "b");
    graph2.addArc("a", "d");
    graph2.addArc("b", "c");
    graph2.addArc("b", "d");
    graph2.addArc("c", "b");
    graph2.addArc("b", "e");

    Graph<DumbNode, DumbEdge> graph3;

    compareTestHelper(graph, graph2, "Graph", /* compareTo */ -1);
    compareTestHelper(graph, graph3, "Graph", /* compareTo */  1);
    compareTestHelper(graph2, graph, "Graph", /* compareTo */  1);
    compareTestHelper(graph, graph, "Graph", /* compareTo */ 0);

    Set<Graph<DumbNode, DumbEdge> > sgraph {graph, graph2, graph3};
    assertEqualsString("sgraph", "{{}, {a, b, c, d, e, a -> b, a -> d, b -> c, b -> d, c -> b, c -> e}, {a, b, c, d, e, f, a -> b, a -> d, b -> c, b -> d, b -> e, c -> b}}", sgraph.toString());
}

TIMED_TEST(GraphTests, forEachTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    Queue<std::string> expected {"a", "b", "c", "d", "e"};
    for (DumbNode* node : graph) {
        std::string exp = expected.dequeue();
        assertEqualsString("Graph foreach node name", exp, node->name);
    }
}

TIMED_TEST(GraphTests, frontBackTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    assertEqualsString("Graph front", "a", graph.front()->name);
    assertEqualsString("Graph back",  "e", graph.back()->name);
}

TIMED_TEST(GraphTests, hashCodeTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    assertEqualsInt("hashcode of self cgraph", hashCode(graph), hashCode(graph));

    Graph<DumbNode, DumbEdge> copy = graph;
    assertEqualsInt("hashcode of copy graph", hashCode(graph), hashCode(copy));

    Graph<DumbNode, DumbEdge> graph2;
    graph2.addNode("a");
    graph2.addNode("b");
    graph2.addNode("c");
    graph2.addNode("d");
    graph2.addNode("e");
    graph2.addNode("f");
    graph2.addArc("a", "b");
    graph2.addArc("a", "d");
    graph2.addArc("b", "c");
    graph2.addArc("b", "d");
    graph2.addArc("c", "b");
    graph2.addArc("b", "e");
    assertNotEqualsInt("hashcode of unequal graph", hashCode(graph), hashCode(graph2));

    HashSet<Graph<DumbNode, DumbEdge> > hashgraph;
    hashgraph.add(graph);
    hashgraph.add(graph2);
    assertEqualsInt("hashset of graph size", 2, hashgraph.size());
}

TIMED_TEST(GraphTests, initializerListTest_Graph, TEST_TIMEOUT_DEFAULT) {
    // TODO
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(GraphTests, iteratorVersionTest_Graph, TEST_TIMEOUT_DEFAULT) {
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");

    try {
        for (DumbNode* v : graph) {
            graph.removeNode(v);
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR
