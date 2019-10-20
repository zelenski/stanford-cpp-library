/*
 * File: graph.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * This file implements the graph.h interface using sets to represent the
 * nodes and arcs.
 */

#include <stdio.h>
#include "foreach.h"
#include "cslib.h"
#include "generic.h"
#include "graph.h"
#include "hashmap.h"
#include "iterator.h"
#include "itertype.h"
#include "set.h"
#include "strlib.h"
#include "unittest.h"

/*
 * Type: GraphCDT
 * --------------
 * This definition provides the concrete type for a Graph,
 * which is implemented as a set of nodes and a set of arcs.
 */

struct GraphCDT {
   IteratorHeader header;       /* Header to make graphs iterable     */
   Set nodes;                   /* The set of all nodes in the graph  */
   Set arcs;                    /* The set of all arcs in the graph   */
   CompareFn nodeCmpFn;         /* Comparison function to order nodes */
   CompareFn arcCmpFn;          /* Comparison function to order arcs  */
   HashMap nameMap;             /* Map from names to nodes            */
};

/*
 * Type: NodeCDT
 * -------------
 * This type defines the concrete structure of a graph node.  All nodes
 * have a name, which is used to identify the node.
 */

struct NodeCDT {
   string name;                 /* Name identifying the node          */
   Set arcs;                    /* Set of arcs leaving this node      */
   Graph graph;                 /* The graph containing this node     */
};

/*
 * Type: ArcCDT
 * ------------
 * The concrete type for an arc consists of its endpoints plus a cost.
 * This cost need not be economic and will often refer to some other
 * metric, such as distance.
 */

struct ArcCDT {
   Node start;                  /* The starting node for the arc      */
   Node end;                    /* The ending node for the arc        */
   double cost;                 /* The "cost" of travesing the arc    */
   Graph graph;                 /* The graph containing this arc      */
};

/* Private function prototypes */

static Iterator newGraphIterator(void *collection);
static int defaultNodeOrdering(const void *p1, const void *p2);
static int defaultArcOrdering(const void *p1, const void *p2);

/* Exported entries */

Graph newGraph(void) {
   Graph g;

   g = newBlock(Graph);
   enableIteration(g, newGraphIterator);
   g->nodes = newSet(Node);
   g->arcs = newSet(Arc);
   g->nodeCmpFn = defaultNodeOrdering;
   g->arcCmpFn = defaultArcOrdering;
   setCompareFn(g->nodes, defaultNodeOrdering);
   setCompareFn(g->arcs, defaultArcOrdering);
   g->nameMap = newHashMap();
   return g;
}

void freeGraph(Graph g) {
   Node node;
   Arc arc;

   foreach (node in g->nodes) {
      freeBlock(node);
   }
   foreach (arc in g->arcs) {
      freeBlock(arc);
   }
   freeSet(g->nodes);
   freeSet(g->arcs);
   freeHashMap(g->nameMap);
   freeBlock(g);
}

Node addNode(Graph g, string name) {
   Node node;

   node = newBlock(Node);
   node->name = name;
   node->arcs = newSet(Arc);
   node->graph = g;
   setCompareFn(node->arcs, g->arcCmpFn);
   addSet(g->nodes, node);
   if (containsKeyHashMap(g->nameMap, name)) {
      error("addNode: Duplicate node %s", name);
   }
   putHashMap(g->nameMap, name, node);
   return node;
}

void removeNode(Graph g, Node node) {
   Arc arc;

   foreach (arc in g->arcs) {
      if (arc->start == node || arc->end == node) removeArc(g, arc);
   }
   freeSet(node->arcs);
   removeSet(g->nodes, node);
   removeHashMap(g->nameMap, node->name);
   freeBlock(node);
}

Node getNode(Graph g, string name) {
   return getHashMap(g->nameMap, name);
}

Arc addArc(Graph g, Node n1, Node n2) {
   Arc arc;

   arc = newBlock(Arc);
   arc->start = n1;
   arc->end = n2;
   arc->cost = 0;
   arc->graph = g;
   addSet(g->arcs, arc);
   addSet(n1->arcs, arc);
   return arc;
}

void removeArc(Graph g, Arc arc) {
   removeSet(g->arcs, arc);
   removeSet(arc->start->arcs, arc);
   freeBlock(arc);
}

bool isConnected(Node n1, Node n2) {
   Iterator it;
   Arc arc;
   bool result;

   result = false;
   it = newIterator(n1->arcs);
   while (!result && stepIterator(it, &arc)) {
      if (arc->end == n2) result = true;
   }
   freeIterator(it);
   return result;
}

Set getNodeSet(Graph g) {
   return g->nodes;
}

Set getArcSet(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "Graph")) {
      return ((Graph) arg)->arcs;
   } else if (endsWith(type, "Node")) {
      return ((Node) arg)->arcs;
   } else {
      error("getArcSet: Unsupported type %s", type);
      return NULL;
   }
}

Set getNeighbors(Node node) {
   Set result;
   Arc arc;

   result = newSet(Node);
   setCompareFn(result, node->graph->nodeCmpFn);
   foreach (arc in node->arcs) {
      addSet(result, arc->end);
   }
   return result;
}

string getName(Node node) {
   return node->name;
}

Node startOfArc(Arc arc) {
   return arc->start;
}

Node endOfArc(Arc arc) {
   return arc->end;
}

double getCost(Arc arc) {
   return arc->cost;
}

void setCost(Arc arc, double cost) {
   arc->cost = cost;
}

void setNodeOrdering(Graph g, CompareFn cmpFn) {
   // Add isEmpty check
   g->nodeCmpFn = cmpFn;
   setCompareFn(g->nodes, cmpFn);
}

void setArcOrdering(Graph g, CompareFn cmpFn) {
   // Add isEmpty check
   g->arcCmpFn = cmpFn;
   setCompareFn(g->arcs, cmpFn);
}

/* Private functions */

/*
 * Function: newGraphIterator
 * Usage: iterator = newGraphIterator(graph);
 * ------------------------------------------
 * Creates a new iterator for a graph, which iterates over its nodes.
 */

static Iterator newGraphIterator(void *collection) {
   return newIterator(((Graph) collection)->nodes);
}

static int defaultNodeOrdering(const void *p1, const void *p2) {
   string s1, s2;

   s1 = getName(*((Node *) p1));
   s2 = getName(*((Node *) p2));
   return stringCompare(s1, s2);
}

static int defaultArcOrdering(const void *p1, const void *p2) {
   Arc a1, a2;
   int cmp;

   a1 = *((Arc *) p1);
   a2 = *((Arc *) p2);
   cmp = stringCompare(getName(a1->start), getName(a2->start));
   if (cmp != 0) return cmp;
   cmp = stringCompare(getName(a1->end), getName(a2->end));
   if (cmp != 0) return cmp;
   if (a1->cost < a2->cost) return -1;
   if (a1->cost > a2->cost) return +1;
   if (a1 < a2) return -1;
   if (a1 > a2) return +1;
   return 0;
}

/**********************************************************************/
/* Unit test for the graph module                                     */
/**********************************************************************/

#ifndef _NOTEST_

/* Desired output stream */

static string nodeScript[] = { "A", "B", "C", "D", "X" };
static string arcScript[] = {
   "A -> B",
   "A -> C",
   "A -> D",
   "B -> C",
   "B -> D",
   "C -> D",
   "X"
};
static string neighborScript[] = { "B", "C", "D", "X" };
static string arcsFromScript[] = { "A -> B", "A -> C", "A -> D", "X" };

/* Private function prototypes */

static Graph createTestGraph(void);
static void createNode(Graph g, string name);
static void linkNodesAlphabetically(Graph g);
static void testNodeIterator(Graph g);
static void testArcIterator(Graph g);
static void testArcsFrom(Graph g);
static void testNeighbors(Graph g);
static string arcString(Arc arc);

/* Unit test */

void testGraphModule(void) {
   Graph g;

   g = createTestGraph();
   testNodeIterator(g);
   testArcIterator(g);
   testArcsFrom(g);
   testNeighbors(g);
}

/* Private functions */

static Graph createTestGraph(void) {
   Graph g;

   reportMessage("createTestGraph();");
   g = newGraph();
   addNode(g, "A");
   addNode(g, "B");
   addNode(g, "C");
   addNode(g, "D");
   linkNodesAlphabetically(g);
   return g;
}

static void linkNodesAlphabetically(Graph g) {
   Node n1, n2;
   string s1, s2;

   foreach (n1 in getNodeSet(g)) {
      s1 = getName(n1);
      foreach (n2 in getNodeSet(g)) {
         s2 = getName(n2);
         if (stringCompare(s1, s2) < 0) {
            addArc(g, n1, n2);
         }
      }
   }
}

static void testNodeIterator(Graph g) {
   Node node;
   int count;

   count = 0;
   reportMessage("foreach (node in getNodes(g)) {");
   adjustReportIndentation(+3);
   foreach (node in getNodeSet(g)) {
      test(getName(node), (string) nodeScript[count++]);
   }
   adjustReportIndentation(-3);
   reportMessage("}");
   if (!stringEqual(nodeScript[count], "X")) {
      reportError("Node iterator has too few elements");
   }
}

static void testArcIterator(Graph g) {
   Arc arc;
   int count;

   count = 0;
   reportMessage("foreach (arc in getArcSet(g)) {");
   adjustReportIndentation(+3);
   foreach (arc in getArcSet(g)) {
      test(arcString(arc), (string) arcScript[count++]);
   }
   adjustReportIndentation(-3);
   reportMessage("}");
   if (!stringEqual(arcScript[count], "X")) {
      reportError("Arc iterator has too few elements");
   }
}

static void testArcsFrom(Graph g) {
   Arc arc;
   int count;

   count = 0;
   reportMessage("foreach (arc in getArcSet(getNode(g, \"A\"))) {");
   adjustReportIndentation(+3);
   foreach (arc in getArcSet(getNode(g, "A"))) {
      test(arcString(arc), (string) arcsFromScript[count++]);
   }
   adjustReportIndentation(-3);
   reportMessage("}");
   if (!stringEqual(arcsFromScript[count], "X")) {
      reportError("Arc iterator has too few elements");
   }
}

static void testNeighbors(Graph g) {
   Node node;
   int count;

   count = 0;
   reportMessage("foreach (node in getNeighbors(getNode(g, \"A\"))) {");
   adjustReportIndentation(+3);
   foreach (node in getNeighbors(getNode(g, "A"))) {
      test(getName(node), (string) neighborScript[count++]);
   }
   adjustReportIndentation(-3);
   reportMessage("}");
   if (!stringEqual(neighborScript[count], "X")) {
      reportError("Node iterator has too few elements");
   }
}

static string arcString(Arc arc) {
   return concat(getName(arc->start), concat(" -> ", getName(arc->end)));
}

#endif
