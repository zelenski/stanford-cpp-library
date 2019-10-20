/*
 * File: graph.h
 * -------------
 * This interface exports a simple graph abstraction.  In this abstraction,
 * graphs closely model their mathematical formulation as a set of nodes
 * connected by a set of arcs.
 */

#ifndef _graph_h
#define _graph_h

#include "cslib.h"
#include "cmpfn.h"
#include "set.h"

/*
 * Type: Graph
 * -----------
 * This type represents the abstract type for a graph.  Conceptually, a
 * <code>Graph</code> consists of a set of <i>nodes</i> together with a
 * set of <i>arcs</i>.  Each arc connects two nodes in one direction.
 * Undirected graphs must include two arcs for each bidirectional
 * connection.
 */

typedef struct GraphCDT *Graph;

/*
 * Type: Node
 * ----------
 * This type is the abstract type for a node in a graph.  Clients
 * can store their own data in a node by using the functions
 * <code>getBlockData</code> and <code>setBlockData</code> described
 * in <a href="cslib.html"><code>cslib.h</code></a>.
 */

typedef struct NodeCDT *Node;

/*
 * Type: Arc
 * ---------
 * This type is the abstract type for an arc in a graph.  Clients
 * can store their own data in an arc by using the functions
 * <code>getBlockData</code> and <code>setBlockData</code> described
 * in <a href="cslib.html"><code>cslib.h</code></a>.
 */

typedef struct ArcCDT *Arc;

/*
 * Function: newGraph
 * Usage: g = newGraph();
 * ----------------------
 * Returns a new graph with no nodes or arcs.
 */

Graph newGraph(void);

/*
 * Function: freeGraph
 * Usage: freeGraph(g);
 * --------------------
 * Frees the storage for the graph, along with its nodes and arcs.
 */

void freeGraph(Graph g);

/*
 * Function: addNode
 * Usage: node = addNode(g, name);
 * -------------------------------
 * Adds a node to the graph with the specified name.  If there is already
 * a node with that name, <code>addNode</code> generates an error.  The
 * function returns the <code>Node</code> value.
 */

Node addNode(Graph g, string name);

/*
 * Function: removeNode
 * Usage: removeNode(g, node);
 * ---------------------------
 * Removes and frees the specified node from the graph, along with any
 * arcs that enter or leave that node.
 */

void removeNode(Graph g, Node node);

/*
 * Function: getNode
 * Usage: node = getNode(g, name);
 * -------------------------------
 * Returns the node in the graph that has the specified name.  If there is
 * no such node, <code>getNode</code> returns <code>NULL</code>.
 */

Node getNode(Graph g, string name);

/*
 * Function: addArc
 * Usage: arc = addArc(g, n1, n2);
 * -------------------------------
 * Adds a new arc to the graph connecting nodes <code>n1</code> and
 * <code>n2</code>.  The function returns the <code>Arc</code> value.
 */

Arc addArc(Graph g, Node n1, Node n2);

/*
 * Function: removeArc
 * Usage: removeArc(g, arc);
 * -------------------------
 * Removes and frees the specified arc.
 */

void removeArc(Graph g, Arc arc);

/*
 * Function: isConnected
 * Usage: if (isConnected(n1, n2)) . . .
 * -------------------------------------
 * Returns <code>true</code> if there is an arc from <code>n1</code>
 * to <code>n2</code>.
 */

bool isConnected(Node n1, Node n2);

/*
 * Function: getNodeSet
 * Usage: nodeSet = getNodeSet(g);
 * -------------------------------
 * Returns a set consisting of all nodes in the graph.  This function
 * is typically used in conjunction with the <code>foreach</code>
 * macro to initialize an iterator.  For example, the following
 * idiom iterates over the nodes in the specified graph:
 *
 *<pre>
 *    foreach (node in getNodeSet(g)) . . .
 *</pre>
 */

Set getNodeSet(Graph g);

/*
 * Function: getArcSet
 * Usage: arcSet = getArcSet(g);
 *        arcSet = getArcSet(node);
 * --------------------------------
 * Returns a set consisting of all arcs, either in the graph or starting
 * at the specified node.  This function is typically used in conjunction
 * with the <code>foreach</code> macro to initialize an iterator.  For
 * example, the following idiom iterates over the arcs in the specified
 * graph:
 *
 *<pre>
 *    foreach (arc in getArcSet(g)) . . .
 *</pre>
 */

Set getArcSet(void *arg);

/*
 * Function: getNeighbors
 * Usage: nodeSet = getNeighbors(node);
 * ------------------------------------
 * Returns a set consisting of the nodes to which a given node is
 * connected.  This function is typically used in conjunction with
 * the <code>foreach</code> macro to initialize an iterator.  For
 * example, the following idiom iterates over the nodes to which
 * the node <code>start</code> is connected:
 *
 *<pre>
 *    foreach (node in getNeighbors(start)) . . .
 *</pre>
 */

Set getNeighbors(Node node);

/*
 * Function: getName
 * Usage: str = getName(node);
 * ---------------------------
 * Returns the name of the node.
 */

string getName(Node node);

/*
 * Function: startOfArc
 * Usage: node = startOfArc(arc);
 * ------------------------------
 * Returns the node at the beginning of the specified arc.
 */

Node startOfArc(Arc arc);

/*
 * Function: endOfArc
 * Usage: node = endOfArc(arc);
 * ----------------------------
 * Returns the node at the end of the specified arc.
 */

Node endOfArc(Arc arc);

/*
 * Function: getCost
 * Usage: cost = getCost(arc);
 * ---------------------------
 * Returns the "cost" associated with traversing an arc.  This cost need not
 * be economic and will often refer to some other metric, such as distance.
 */

double getCost(Arc arc);

/*
 * Function: setCost
 * Usage: setCost(arc, cost);
 * --------------------------
 * Sets the cost of traversing the arc.
 */

void setCost(Arc arc, double cost);

/*
 * Function: setNodeOrdering
 * Usage: setNodeOrdering(graph, cmpFn);
 * -------------------------------------
 * Sets the comparison function used to order the nodes in a graph when
 * they are enumerated.  By default, nodes are sorted in alphabetical
 * order by name.
 */

void setNodeOrdering(Graph graph, CompareFn cmpFn);

/*
 * Function: setArcOrdering
 * Usage: setArcOrdering(graph, cmpFn);
 * ------------------------------------
 * Sets the comparison function used to order the arcs in a graph when
 * they are enumerated.  By default, arcs are sorted in alphabetical
 * order by the name of the start node followed by the name of the end node.
 */

void setArcOrdering(Graph graph, CompareFn cmpFn);

#endif
