/*
 * File: graph.h
 * -------------
 * This file exports a parameterized Graph class used to represent graphs,
 * which consist of a set of nodes (vertices) and a set of arcs (edges).
 * 
 * @version 2018/09/07
 * - reformatted doc-style comments
 * @version 2018/03/10
 * - added methods front, back
 * - fixed compiler issue with getArcSet call
 * @version 2017/11/14
 * - added nodeCount, arcCount, getNodeNames
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via Map)
 * - fixed bug in getInverseNeighborNames function
 * @version 2016/12/01
 * - removed memory leaks of graph node and arc structures
 * - fixed bug in containsNode method (was returning false positives)
 * - fixed bugs in some node/arc methods (should not crash on empty/null args)
 * @version 2016/11/29
 * - added getNeighborNames, getInverseNeighborNames
 * @version 2016/11/26
 * - added getInverseArcSet
 * @version 2016/11/19
 * - changed addNode to not throw when adding duplicates
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added iterator begin(), end() support so that students can directly
 *   for-each over the vertices of a graph.
 * - added comparison operators ==, !=, <, etc.
 * - added template hashCode function
 * @version 2014/10/20
 * - converted functions to accept const string& rather than string for speed
 * - added iterator/for-each support over nodes; begin() / end() members
 * - updated various methods that accept node/arc pointers to verify
 *   that those nodes/arcs are part of the graph first, and to check for null
 * @version 2014/10/10
 * - removed use of __foreach macro
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _graph_h
#define _graph_h

#include <string>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#define INTERNAL_INCLUDE 1
#include "tokenscanner.h"
#undef INTERNAL_INCLUDE

/**
 * This class represents a graph with the specified node and arc types.
 * The <code>NodeType</code> and <code>ArcType</code> parameters indicate
 * the structure type or class used for nodes and arcs, respectively.
 * These types can contain any fields or methods required by the client,
 * but must contain the following fields required by the <code>Graph</code>
 * package itself:
 *
 * <p>The <code>NodeType</code> definition must include:
 * <ul>
 *   <li>A <code>string</code> field called <code>name</code>
 *   <li>A <code>Set&lt;ArcType *&gt;</code> field called <code>arcs</code>
 * </ul>
 *
 * <p>The <code>ArcType</code> definition must include:
 * <ul>
 *   <li>A <code>NodeType *</code> field called <code>start</code>
 *   <li>A <code>NodeType *</code> field called <code>finish</code>
 * </ul>
 */
template <typename NodeType, typename ArcType>
class Graph {
public:
    /**
     * Creates an empty graph.
     * @bigoh O(1)
     */
    Graph() = default;
    
    /**
     * Frees the internal storage allocated to represent the graph.
     * @bigoh O(V + E)
     */
    virtual ~Graph();
    
    /**
     * Adds a directed arc to the graph from node n1 to n2.
     * If either node is not found in the graph, said node will be added to the graph.
     * Returns a pointer to the arc in case the client needs to capture this value.
     * @bigoh O(log V + log E)
     */
    ArcType* addArc(const std::string& n1, const std::string& n2);

    /**
     * Adds a directed arc to the graph from node n1 to n2.
     * If either node is not found in the graph, said node will be added to the graph.
     * Returns a pointer to the arc in case the client needs to capture this value.
     *
     * @throw ErrorException if any pointer passed is null
     * @bigoh O(log V + log E)
     */
    ArcType* addArc(NodeType* n1, NodeType* n2);

    /**
     * Adds the given arc to the graph.
     * If the start/finish nodes passed are not already part of the graph,
     * they are added to the graph.
     * Returns a pointer to the arc in case the client needs to capture this value.
     *
     * Memory management: Once you hand me this ArcType* pointer, our code owns it.
     * We will delete/free it when done with it.
     * You do not need to (and should not) free it yourself.
     *
     * @throw ErrorException if any pointer passed is null
     * @bigoh O(log V + log E)
     */
    ArcType* addArc(ArcType* arc);

    /**
     * Adds a node to the graph.  The first version of this method
     * creates a new node of the appropriate type and initializes its
     * fields; the second assumes that the client has already created
     * the node and simply adds it to the graph.
     * Returns a pointer to the node.
     * If a node with the given name is already present, does nothing.
     *
     * Memory management: Once you hand me this NodeType* pointer, our code owns it.
     * We will delete/free it when done with it.
     * You do not need to (and should not) free it yourself.
     *
     * @throw ErrorException if any pointer passed is null
     * @bigoh O(log V)
     */
    NodeType* addNode(const std::string& name);

    /**
     * Adds a node to the graph.
     * This version assumes that the client has already created the node structure
     * and simply adds it to the graph.
     * Returns a pointer to the node.
     * If a node with the given name is already present, does nothing.
     *
     * Memory management: Once you hand me this NodeType* pointer, our code owns it.
     * We will delete/free it when done with it.
     * You do not need to (and should not) free it yourself.
     *
     * @throw ErrorException if any pointer passed is null
     * @bigoh O(log V)
     */
    NodeType* addNode(NodeType* node);

    /**
     * Returns the number of arcs in the graph.
     * @bigoh O(1)
     */
    int arcCount() const;

    /**
     * Returns the last node in the graph in the order as would be returned by
     * a for-each loop or iterator.
     * @throw ErrorException if the graph is empty
     * @bigoh O(1)
     */
    NodeType* back() const;

    /**
     * Reinitializes the graph to be empty, removing all nodes and arcs
     * and freeing any heap storage used by their corresponding internal structures.
     * @bigoh O(V + E)
     */
    void clear();

    /**
     * Removes all arcs from the graph, freeing the heap storage used by their
     * corresponding internal structures. The graph's nodes remain intact.
     * @bigoh O(E)
     */
    void clearArcs();

    /**
     * Removes all arcs from the graph that start from the given node,
     * freeing the heap storage used by their corresponding internal structures.
     * The graph's nodes remain intact.
     * If the given node pointer is null or not found in the graph, has no effect.
     * @bigoh O(log V + E)
     */
    void clearArcs(NodeType* node);

    /**
     * Removes all arcs from the graph that start from the given node,
     * freeing the heap storage used by their corresponding internal structures.
     * The graph's nodes remain intact.
     * If the given node is not found in the graph, has no effect.
     * @bigoh O(E log E)
     */
    void clearArcs(const std::string& name);

    /**
     * Returns true if there exists an arc directly between the given two nodes.
     * If either node is null or is not contained in this graph, returns false.
     * @bigoh O(log E)
     */
    bool containsArc(NodeType* node1, NodeType* node2) const;

    /**
     * Returns true if there exists an arc directly between the given two nodes.
     * If either node is not contained in this graph, returns false.
     * @bigoh O(log E)
     */
    bool containsArc(const std::string& node1, const std::string& node2) const;

    /**
     * Returns true if the given arc exists in this graph.
     * If the given arc is null or either of its nodes are not contained in
     * this graph, returns false.
     * @bigoh O(log E)
     */
    bool containsArc(ArcType* arc) const;

    /**
     * Returns true if there exists a node in this graph with the given name.
     * @bigoh O(log V)
     */
    bool containsNode(const std::string& name) const;

    /**
     * Returns true if the given node is part of this graph.
     * If the pointer passed is null, returns false.
     * @bigoh O(log V)
     */
    bool containsNode(NodeType* node) const;

    /**
     * Compares two graphs for equality.
     * Returns <code>true</code> if this graph contains exactly the same
     * nodes, arcs, and connections as the given other graph.
     * Identical in behavior to the == operator.
     * @bigoh O(V log V + E log E)
     */
    bool equals(const Graph<NodeType, ArcType>& graph2) const;
    
    /**
     * Returns the first node in the graph in the order as would be returned by
     * a for-each loop or iterator.
     * @throw ErrorException if the graph is empty
     * @bigoh O(1)
     */
    NodeType* front() const;

    /**
     * Returns the arc, if any, from node1 to node2.
     * If multiple arcs exist between the given two nodes, which is returned is unspecified.
     * If either pointer passed is null or no such arc exists, returns a null pointer.
     * @bigoh O(log V + log E)
     */
    ArcType* getArc(NodeType* node1, NodeType* node2) const;

    /**
     * Returns the arc, if any, from node1 to node2.
     * If multiple arcs exist between the given two nodes, which is returned is unspecified.
     * If no such arc exists, returns a null pointer.
     * @bigoh O(log V + log E)
     */
    ArcType* getArc(const std::string& node1, const std::string& node2) const;

    /**
     * Returns the set of all arcs in the graph.
     * @bigoh O(1)
     */
    const Set<ArcType*>& getArcSet() const;

    /**
     * Returns the set of all arcs that start at the specified node,
     * indicated as a pointer to its node structure.
     * If the pointer passed is null or the given node is not found in the graph,
     * returns an empty set.
     * @bigoh O(1)
     */
    const Set<ArcType*>& getArcSet(NodeType* node) const;

    /**
     * Returns the set of all arcs that start at the specified node.
     * If the given node is not found in the graph, returns an empty set.
     * @bigoh O(1)
     */
    const Set<ArcType*>& getArcSet(const std::string& name) const;

    /**
     * Returns the set of outbound arcs to the given node from other nodes.
     * In other words, getInverseArcSet(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If any pointer passed is null, or if the given node is not found
     * in this graph, returns an empty set.
     * @bigoh O(E)
     */
    const Set<ArcType*> getInverseArcSet(NodeType* node) const;

    /**
     * Returns the set of outbound arcs to the given node from other nodes.
     * In other words, getInverseArcSet(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If the given node is not found in this graph, returns an empty set.
     * @bigoh O(E)
     */
    const Set<ArcType*> getInverseArcSet(const std::string& name) const;

    /**
     * Returns the set of strings of names of nodes that are neighbors of the
     * given node.
     * In other words, getInverseNeighborNames(n1) is the set of all strings n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If any pointer passed is null, or if the given node is not found
     * in this graph, returns an empty set.
     * @bigoh O(E)
     */
    Set<std::string> getInverseNeighborNames(NodeType* node) const;

    /**
     * Returns the set of strings of names of nodes that are neighbors of the
     * given node.
     * In other words, getInverseNeighborNames(n1) is the set of all strings n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If the given node is not found in this graph, returns an empty set.
     * @bigoh O(E)
     */
    Set<std::string> getInverseNeighborNames(const std::string& node) const;

    /**
     * Returns the set of nodes that are neighbors of the specified node.
     * In other words, getInverseNeighbors(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If any pointer passed is null, or if the given node is not found
     * in this graph, returns an empty set.
     * @bigoh O(E)
     */
    Set<NodeType*> getInverseNeighbors(NodeType* node) const;

    /**
     * Returns the set of nodes that are neighbors of the specified node.
     * In other words, getInverseNeighbors(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n2 and ending at n1.
     *
     * If the given node is not found in this graph, returns an empty set.
     * @bigoh O(E)
     */
    Set<NodeType*> getInverseNeighbors(const std::string& node) const;

    /**
     * Returns the set of node names that are neighbors of the specified node.
     * In other words, getNeighbors(n1) is the set of all strings n2
     * such that there exists an arc E starting from n1 and ending at n2.
     *
     * If any pointer passed is null, or if the given node is not found
     * in this graph, returns an empty set.
     * @bigoh O(log V)
     */
    Set<std::string> getNeighborNames(NodeType* node) const;

    /**
     * Returns the set of node names that are neighbors of the specified node.
     * In other words, getNeighbors(n1) is the set of all strings n2
     * such that there exists an arc E starting from n1 and ending at n2.
     *
     * If the given node is not found in this graph, returns an empty set.
     * @bigoh O(log V)
     */
    Set<std::string> getNeighborNames(const std::string& node) const;

    /**
     * Returns the set of nodes that are neighbors of the specified node.
     * In other words, getNeighbors(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n1 and ending at n2.
     *
     * If any pointer passed is null, or if the given node is not found
     * in this graph, returns an empty set.
     * @bigoh O(log V)
     */
    Set<NodeType*> getNeighbors(NodeType* node) const;

    /**
     * Returns the set of nodes that are neighbors of the specified node.
     * In other words, getNeighbors(n1) is the set of all nodes n2
     * such that there exists an arc E starting from n1 and ending at n2.
     *
     * If the given node is not found in this graph, returns an empty set.
     * @bigoh O(log V)
     */
    Set<NodeType*> getNeighbors(const std::string& node) const;

    /**
     * Looks up a node in the name table attached to the graph and
     * returns a pointer to that node.
     * If no node with the specified name exists, returns <code>nullptr</code>.
     * @bigoh O(log V)
     */
    NodeType* getNode(const std::string& name) const;

    /**
     * Returns the set of the names of all nodes in the graph.
     * Similar to getNodeSet but returns a set of strings rather than a set
     * of pointers to nodes.
     * @bigoh O(V log V)
     */
    Set<std::string> getNodeNames() const;
    
    /**
     * Returns the set of all nodes in the graph.
     * These are direct pointers to the internal NodeType* structures in the
     * graph, so any modifications you make to them will be reflected in the graph.
     * @bigoh O(1)
     */
    const Set<NodeType*>& getNodeSet() const;

    /**
     * Returns <code>true</code> if the graph contains an arc from
     * <code>n1</code> to <code>n2</code>.
     * If any pointer passed is null, or if either node is not contained
     * in this graph, returns false.
     * @bigoh O(log V)
     */
    bool isConnected(NodeType* n1, NodeType* n2) const;

    /**
     * Returns <code>true</code> if the graph contains an arc from
     * <code>n1</code> to <code>n2</code>.
     * If either node is not contained in this graph, returns false.
     * @bigoh O(log V)
     */
    bool isConnected(const std::string& s1, const std::string& s2) const;

    /**
     * Returns true if the graph contains an edge from v1 to v2.
     * If either of the vertexes supplied is not found in the graph, returns false.
     * @bigoh O(log V)
     */
    bool isNeighbor(const std::string& node1, const std::string& node2) const;

    /**
     * Returns true if the graph contains an edge from v1 to v2.
     * If either of the vertexes supplied is null or is not found in the graph, returns false.
     * @bigoh O(log V)
     */
    bool isNeighbor(NodeType* node1, NodeType* node2) const;

    /**
     * Returns <code>true</code> if the graph contains no vertexes.
     * @bigoh O(1)
     */
    bool isEmpty() const;

    /**
     * Returns the number of nodes in the graph.
     * Equivalent to size().
     * @bigoh O(1)
     */
    int nodeCount() const;

    /**
     * Removes an arc from v1 to v2 in the graph, specified by the names of its endpoints.
     * If more than one arc connects the specified endpoints, all of them are removed.
     * If no arc connects the given endpoints, or the given arc is not found,
     * the call has no effect.
     * @bigoh O(E + log V)
     */
    void removeArc(const std::string& s1, const std::string& s2);

    /**
     * Removes an arc from v1 to v2 in the graph, specified by the node pointers
     * at its endpoints.
     * If more than one arc connects the specified endpoints, all of them are removed.
     * If no arc connects the given endpoints, or the given arc is not found,
     * the call has no effect.
     * @bigoh O(E + log V)
     */
    void removeArc(NodeType* n1, NodeType* n2);

    /**
     * Removes the given arc from the graph, specified as an arc pointer.
     * If more than one arc connects the specified endpoints, all of them are removed.
     * If no arc connects the given endpoints, or the given arc is not found,
     * the call has no effect.
     *
     * Memory management: Our code will delete/free the ArcType* object when done with it.
     * You do not need to (and should not) free it yourself.
     * @bigoh O(log E + log V)
     */
    void removeArc(ArcType* arc);

    /**
     * Removes the node with the given name from the graph.
     * Removing a node also removes all arcs that contain that node.
     * If a node name is passed that is not part of the graph,
     * the call has no effect.
     * @bigoh O(E + log V)
     */
    void removeNode(const std::string& name);

    /**
     * Removes a node from the graph, specified as a pointer value.
     * Removing a node also removes all arcs that contain that node.
     * If a node or name is passed that is null or is not part of the graph,
     * the call has no effect.
     *
     * Memory management: Our code will delete/free the NodeType* object when done with it.
     * You do not need to (and should not) free it yourself.
     * @bigoh O(E + log V)
     */
    void removeNode(NodeType* node);

    /**
     * Reads the data for an arc from the scanner.
     * The <code>forward</code> argument points to the arc in the forward direction.
     * If the arc is undirected, <code>backward</code> points to the reverse arc;
     * for directed arcs, the <code>backward</code> pointer is <code>nullptr</code>.
     *
     * The default implementation of this method is empty.
     * Clients that want to initialize other fields in the arc must override
     * this method so that it initializes one or both arc, as appropriate.
     */
    virtual void scanArcData(TokenScanner &, ArcType* /*forward*/, ArcType* /*backward*/) {
        // empty
    }

    /**
     * This method reads one "entry" for the graph, which is either a node
     * description or an arc description.  The <code>scanGraphEntry</code>
     * method returns <code>true</code> if it reads an entry, and
     * <code>false</code> at the end of file or at text that cannot be
     * recognized as a graph entry.
     *
     * Node entries consist of the name of a node (which may be quoted
     * if it contains special characters), optionally followed by data for
     * the node.  Arc descriptions have one of the following forms:
     *
     * <pre>
     * n1 -> n2
     * n1 - n2
     * </pre>
     *
     * either of which can be followed by data for the arc.
     * The first form creates a single directed arc; the second creates two arcs,
     * one in each direction.
     *
     * Clients who want to read node or arc data must override the empty
     * versions of <code>scanNodeData</code> and <code>scanArcData</code>
     * included in this interface.
     */
    virtual bool scanGraphEntry(TokenScanner& scanner);

    /**
     * Reads the data for the specified node from the scanner.
     * The default implementation of this method is empty.
     * Clients that want to initialize other fields in the node from the token
     * stream must override this method.
     */
    virtual void scanNodeData(TokenScanner&, NodeType*) {
        /* Empty */
    }

    /**
     * Returns the number of nodes in the graph.
     * Equivalent to nodeCount.
     * @bigoh O(1)
     */
    int size() const;

    /**
     * Converts the graph to a printable string representation.
     * @return a string such as <code>"{A, B, C, D, A - B, B - D, C - D}"</code>.
     * @bigoh O(V + E)
     */
    std::string toString() const;
    
    /**
     * Writes the data for the arc to the output stream.
     * The default implementation of this method is empty.
     * Clients that want to store other fields from the arc must override this
     * method so that it writes that data in a form that scanArcData can read.
     */
    virtual void writeArcData(std::ostream&, ArcType*) const {
        // empty
    }

    /**
     * Writes the data for the node to the output stream.
     * The default implementation of this method is empty.
     * Clients that want to store other fields from the node must override this
     * method so that it writes that data in a form that scanNodeData can read.
     */
    virtual void writeNodeData(std::ostream&, NodeType*) const {
        // empty
    }

    using graph_iterator = typename Set<NodeType *>::const_iterator;
    
    /**
     * Returns an STL iterator positioned at the first vertex in the graph.
     * @bigoh O(1)
     */
    graph_iterator begin() const {
        return _nodes.begin();
    }

    /**
     * Returns an STL iterator positioned after the last vertex in the graph.
     * @bigoh O(1)
     */
    graph_iterator end() const {
        return _nodes.end();
    }
    
    /**
     * Relational operators to compare two graphs.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     * @bigoh O(V log V + E log E)
     */
    bool operator ==(const Graph& graph2) const;

    /**
     * Relational operators to compare two graphs.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     * @bigoh O(V log V + E log E)
     */
    bool operator !=(const Graph& graph2) const;

    /**
     * Relational operators to compare two graphs.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(V log V + E log E)
     */
    bool operator <(const Graph& graph2) const;

    /**
     * Relational operators to compare two graphs.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(V log V + E log E)
     */
    bool operator <=(const Graph& graph2) const;

    /**
     * Relational operators to compare two graphs.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(V log V + E log E)
     */
    bool operator >(const Graph& graph2) const;

    /**
     * Relational operators to compare two graphs.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     * @bigoh O(V log V + E log E)
     */
    bool operator >=(const Graph& graph2) const;

private:
    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /**
     * This template class establishes the ordering for nodes and arcs.
     * Nodes are processed in alphabetical order by node name; arcs are
     * compared in much the same way, looking first at the start node and
     * then continuing on to look at the finish node if the start nodes
     * match.  These functions, however, indicate equality only if the
     * arguments are identical, in the sense that they are at the same
     * address.  If two distinct arcs, for example, connect the same pair
     * of nodes (which is perfectly legal in the graph abstraction and can
     * be used, for example, to represent multiple modes of travel between
     * two nodes), those arcs are not the same.
     *
     * @private
     */
    class GraphComparator {
    public:
        bool operator ()(NodeType* n1, NodeType* n2) {
            return compare(n1, n2) < 0;
        }

        bool operator ()(ArcType* a1, ArcType* a2) {
            return compare(a1, a2) < 0;
        }
    };

    Set<NodeType*> _nodes{GraphComparator()}; /* The set of nodes in the graph */
    Set<ArcType*> _arcs{GraphComparator()};   /* The set of arcs in the graph  */
    Map<std::string, NodeType*> _nodeMap;     /* A map from names to nodes     */

public:
    /**
     * These functions are part of the public interface of the class but are
     * defined here to avoid adding confusion to the Graph class.
     *
     * @private
     */
    Graph& operator =(const Graph& src);

    /**
     * These functions are part of the public interface of the class but are
     * defined here to avoid adding confusion to the Graph class.
     *
     * @private
     */
    Graph(const Graph& src);

    /**
     * Compares two nodes for ordering within a set.
     *
     * @private
     */
    static int compare(NodeType* n1, NodeType* n2) {
        if (n1 == n2) {
            return 0;
        }
        if (n1->name < n2->name) {
            return -1;
        }
        if (n1->name > n2->name) {
            return +1;
        }
        return (n1 < n2) ? -1 : +1;
    }

    /**
     * Compares two arcs for ordering within a set.
     *
     * @private
     */
    static int compare(ArcType* a1, ArcType* a2) {
        if (a1 == a2) {
            return 0;
        }
        NodeType* n1 = a1->start;
        NodeType* n2 = a2->start;
        if (n1 != n2) {
            return compare(n1, n2);
        }
        n1 = a1->finish;
        n2 = a2->finish;
        if (n1 != n2) {
            return compare(n1, n2);
        }
        return (a1 < a2) ? -1 : +1;
    }

private:
    void deepCopy(const Graph& src);
    NodeType* getExistingNode(const std::string& name, const std::string& member = "") const;
    int graphCompare(const Graph& graph2) const;
    bool isExistingArc(ArcType* arc) const;
    bool isExistingNode(NodeType* node) const;
    void verifyExistingNode(NodeType* node, const std::string& member = "") const;
    void verifyNotNull(void* p, const std::string& member = "") const;
    NodeType* scanNode(TokenScanner& scanner);
};

/*
 * Implementation notes: Graph constructor
 * ---------------------------------------
 * Even though the body of the Graph constructor is empty, important
 * work is done by the initializers, which ensure that the nodes and
 * arcs set are given the correct comparison functions.
 */

template <typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::Graph(const Graph& src) {
    deepCopy(src);
}

/*
 * Implementation notes: Graph destructor
 * --------------------------------------
 * The destructor must free all heap storage used by this graph to
 * represent the nodes and arcs.  The clear method must also reclaim
 * this memory, which means that the destructor can simply call
 * clear to do the work.
 */
template <typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::~Graph() {
    clear();
}

/*
 * Implementation notes: addArc
 * ----------------------------
 * The addArc method appears in three forms, as described in the
 * interface.  The code for each form of the method, however, is
 * quite straightforward.
 */
template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(const std::string& s1, const std::string& s2) {
    return addArc(getExistingNode(s1, "addArc"), getExistingNode(s2, "addArc"));
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(NodeType* n1, NodeType* n2) {
    verifyExistingNode(n1, "addArc");
    verifyExistingNode(n2, "addArc");
    ArcType* arc = getArc(n1, n2);
    if (arc) {
        return arc;
    } else {
        arc = new ArcType();
        arc->start = n1;
        arc->finish = n2;
        return addArc(arc);
    }
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(ArcType* arc) {
    verifyNotNull(arc, "addArc");
    if (!isExistingNode(arc->start)) {
        addNode(arc->start);
    }
    if (!isExistingNode(arc->finish)) {
        addNode(arc->finish);
    }
    arc->start->arcs.add(arc);
    _arcs.add(arc);
    return arc;
}

/*
 * Implementation notes: addNode
 * -----------------------------
 * The addNode method appears in two forms: one that creates a node
 * from its name and one that assumes that the client has created
 * the new node.  In each case, the implementation must add the node
 * the set of nodes for the graph and add the name-to-node association
 * to the node map.
 */
template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::addNode(const std::string& name) {
    NodeType* node = getNode(name);
    if (node) {
        return node;   // vertex already exists
    }
    node = new NodeType();
    node->arcs = Set<ArcType*>(GraphComparator());
    node->name = name;
    return addNode(node);
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::addNode(NodeType* node) {
    verifyNotNull(node, "addNode");
    NodeType* existingNode = getNode(node->name);
    if (existingNode) {
        *existingNode = *node;   // copy state from parameter
        return existingNode;
    } else {
        _nodes.add(node);
        _nodeMap[node->name] = node;
        return node;
    }
}

template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::arcCount() const {
    return getArcSet().size();
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::back() const {
    if (this->isEmpty()) {
        error("Graph::back: graph is empty");
    }
    return this->_nodes.back();
}

/*
 * Implementation notes: clear
 * ---------------------------
 * The implementation of clear first frees the nodes and arcs in
 * their respective sets and then uses the Set class clear method
 * to ensure that these sets are empty.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clear() {
    for (NodeType* node : _nodes) {
        delete node;
    }
    for (ArcType* arc : _arcs) {
        delete arc;
    }
    _arcs.clear();
    _nodes.clear();
    _nodeMap.clear();
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clearArcs() {
    Set<ArcType*> arcsCopy = getArcSet();   // makes a copy
    for (ArcType* arc : arcsCopy) {
        removeArc(arc);
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clearArcs(NodeType* node) {
    if (isExistingNode(node)) {
        Set<ArcType*> arcsCopy = getArcSet(node);   // makes a copy
        for (ArcType* arc : arcsCopy) {
            removeArc(arc);
        }
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clearArcs(const std::string& name) {
    Set<ArcType*> arcsCopy = getArcSet(name);   // makes a copy
    for (ArcType* arc : arcsCopy) {
        removeArc(arc);
    }
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::containsArc(NodeType* node1, NodeType* node2) const {
    return getArc(node1, node2) != nullptr;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::containsArc(const std::string& node1, const std::string& node2) const {
    return getArc(node1, node2) != nullptr;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::containsArc(ArcType* arc) const {
    if (!arc) {
        return false;
    } else {
        return this->getArcSet().contains(arc);
    }
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::containsNode(const std::string& name) const {
    return _nodeMap.containsKey(name);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::containsNode(NodeType* node) const {
    if (node) {
        return _nodes.contains(node);
    } else {
        return false;
    }
}


template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::equals(const Graph<NodeType, ArcType>& graph2) const {
    return *this == graph2;
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::front() const {
    if (this->isEmpty()) {
        error("Graph::front: graph is empty");
    }
    return this->_nodes.front();
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::getArc(NodeType* node1, NodeType* node2) const {
    if (!containsNode(node1) || !containsNode(node2)) {
        return nullptr;
    }
    for (ArcType* arc : getArcSet(node1)) {
        if (arc->finish == node2) {
            return arc;
        }
    }
    return nullptr;
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::getArc(const std::string& node1, const std::string& node2) const {
    return getArc(getNode(node1), getNode(node2));
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>& Graph<NodeType, ArcType>::getArcSet() const {
    return _arcs;
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>& Graph<NodeType, ArcType>::getArcSet(NodeType* node) const {
    if (isExistingNode(node)) {
        return node->arcs;
    } else {
        static Set<ArcType*> set;   // empty
        return set;
    }
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>& Graph<NodeType, ArcType>::getArcSet(const std::string& name) const {
    return getArcSet(getNode(name));
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::getExistingNode(const std::string& name, const std::string& member) const {
    NodeType* node = _nodeMap.get(name);
    if (!node) {
        error("Graph::" + member + ": no node named " + name);
    }
    return node;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isExistingArc(ArcType* arc) const {
    return arc && _arcs.contains(arc);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isExistingNode(NodeType* node) const {
    return node && _nodeMap.containsKey(node->name) && _nodeMap.get(node->name) == node;
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::verifyExistingNode(NodeType* node, const std::string& member) const {
    verifyNotNull(node, member);
    if (!isExistingNode(node)) {
        error("Graph::" + member + ": node not found in graph");
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::verifyNotNull(void* p, const std::string& member) const {
    if (!p) {
        error("Graph::" + member + ": parameter cannot be null");
    }
}

template <typename NodeType, typename ArcType>
const Set<ArcType*> Graph<NodeType, ArcType>::getInverseArcSet(NodeType* node) const {
    Set<ArcType*> set;
    if (isExistingNode(node)) {
        for (ArcType* arc : getArcSet()) {
            if (arc->finish == node) {
                set.add(arc);
            }
        }
    }
    return set;
}

template <typename NodeType, typename ArcType>
const Set<ArcType*> Graph<NodeType, ArcType>::getInverseArcSet(const std::string& nodeName) const {
    return getInverseArcSet(getNode(nodeName));
}

template <typename NodeType, typename ArcType>
Set<std::string> Graph<NodeType, ArcType>::getInverseNeighborNames(NodeType* node) const {
    Set<std::string> set;
    if (isExistingNode(node)) {
        for (ArcType* arc : getArcSet()) {
            if (arc->finish == node) {
                set.add(arc->start->name);
            }
        }
    }
    return set;
}

template <typename NodeType, typename ArcType>
Set<std::string> Graph<NodeType, ArcType>::getInverseNeighborNames(const std::string& name) const {
    return getInverseNeighborNames(getNode(name));
}

template <typename NodeType, typename ArcType>
Set<NodeType*> Graph<NodeType, ArcType>::getInverseNeighbors(NodeType* node) const {
    Set<NodeType*> set;
    if (isExistingNode(node)) {
        for (ArcType* arc : getArcSet()) {
            if (arc->finish == node) {
                set.add(arc->start);
            }
        }
    }
    return set;
}

template <typename NodeType, typename ArcType>
Set<NodeType*> Graph<NodeType, ArcType>::getInverseNeighbors(const std::string& nodeName) const {
    return getInverseNeighbors(getNode(nodeName));
}

template <typename NodeType, typename ArcType>
Set<std::string> Graph<NodeType, ArcType>::getNeighborNames(NodeType* node) const {
    Set<std::string> neighbors;
    if (isExistingNode(node)) {
        for (ArcType* arc : node->arcs) {
            neighbors.add(arc->finish->name);
        }
    }
    return neighbors;
}

template <typename NodeType, typename ArcType>
Set<std::string> Graph<NodeType, ArcType>::getNeighborNames(const std::string& name) const {
    return getNeighborNames(getNode(name));
}

/*
 * Implementation notes: getNeighbors
 * ----------------------------------
 * This implementation recomputes the set each time, which is reasonably
 * efficient if the degree of the node is small.
 */
template <typename NodeType, typename ArcType>
Set<NodeType*> Graph<NodeType, ArcType>::getNeighbors(NodeType* node) const {
    Set<NodeType*> nodesResult{GraphComparator{}};
    if (isExistingNode(node)) {
        for (ArcType* arc : node->arcs) {
            nodesResult.add(arc->finish);
        }
    }
    return nodesResult;
}

template <typename NodeType, typename ArcType>
Set<NodeType*> Graph<NodeType, ArcType>::getNeighbors(const std::string& name) const {
    return getNeighbors(getNode(name));
}

/*
 * Implementation notes: getNode, getExistingNode
 * ----------------------------------------------
 * The getNode method simply looks up the name in the map, which correctly
 * returns nullptr if the name is not found.  Other methods in the
 * implementation call the private method getExistingNode instead,
 * which checks for a null value and signals an error.
 */
template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::getNode(const std::string& name) const {
    return _nodeMap.get(name);
}

template <typename NodeType, typename ArcType>
Set<std::string> Graph<NodeType, ArcType>::getNodeNames() const {
    Set<std::string> nodeNames;
    for (NodeType* node : _nodes) {
        nodeNames.add(node->name);
    }
    return nodeNames;
}

/*
 * Implementation notes: getNodeSet, getArcSet
 * -------------------------------------------
 * These methods simply return the set requested by the client.  The
 * sets are returned by reference for efficiency, because doing so
 * eliminates the need to copy the set.
 */
template <typename NodeType, typename ArcType>
const Set<NodeType*>& Graph<NodeType, ArcType>::getNodeSet() const {
    return _nodes;
}

/*
 * Implementation notes: isConnected
 * ---------------------------------
 * Node n1 is connected to n2 if any of the arcs leaving n1 finish at n2.
 * The two versions of this method allow nodes to be specified either as
 * node pointers or by name.
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isConnected(NodeType* n1, NodeType* n2) const {
    // don't call verifyExistingNode here because it will throw an error
    // if n1 or n2 is not found; should just make the call return false
    if (!isExistingNode(n1) || !isExistingNode(n2)) {
        return false;
    }
    for (ArcType* arc : n1->arcs) {
        if (arc->finish == n2) {
            return true;
        }
    }
    return false;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isConnected(const std::string& s1, const std::string& s2) const {
    // don't call getExistingNode here because it will throw an error
    // if s1 or s2 is not found; should just make the call return false
    return isConnected(_nodeMap.get(s1), _nodeMap.get(s2));
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isNeighbor(const std::string& node1, const std::string& node2) const {
    return isConnected(node1, node2);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isNeighbor(NodeType* node1, NodeType* node2) const {
    return isConnected(node1, node2);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isEmpty() const {
    return _nodes.isEmpty();
}

template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::nodeCount() const {
    return getNodeSet().size();
}

/*
 * Implementation notes: removeArc
 * -------------------------------
 * These methods remove arcs from the graph, which is ordinarily simply
 * a matter of removing the arc from two sets: the set of arcs in the
 * graph as a whole and the set of arcs in the starting node.  The
 * methods that remove an arc specified by its endpoints, however,
 * must take account of the fact that there might be more than one
 * such arc and delete all of them.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(const std::string& s1, const std::string& s2) {
    // don't call getExistingNode here because it will throw an error
    // if s1 or s2 is not found; should just make the call have no effect
    removeArc(_nodeMap.get(s1), _nodeMap.get(s2));
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(NodeType* n1, NodeType* n2) {
    // don't call verifyExistingNode here because it will throw an error
    // if n1 or n2 is not found; should just make the call have no effect
    if (!isExistingNode(n1) || !isExistingNode(n2)) {
        return;
    }
    Vector<ArcType*> toRemove;
    for (ArcType* arc : _arcs) {
        if (arc->start == n1 && arc->finish == n2) {
            toRemove.add(arc);
        }
    }
    for (ArcType* arc : toRemove) {
        removeArc(arc);
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(ArcType* arc) {
    if (!isExistingArc(arc)) {
        return;
    }
    arc->start->arcs.remove(arc);
    _arcs.remove(arc);
    delete arc;
}

/*
 * Implementation notes: removeNode
 * --------------------------------
 * The removeNode method must remove the specified node but must
 * also remove any arcs in the graph containing the node.  To avoid
 * changing the node set during iteration, this implementation creates
 * a vector of arcs that require deletion.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeNode(const std::string& name) {
    // don't call getExistingNode here because it will throw an error
    // if name is not found; should just make the call have no effect
    removeNode(_nodeMap.get(name));
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeNode(NodeType* node) {
    // don't call verifyExistingNode here because it will throw an error
    // if node is not found; should just make the call have no effect
    if (!isExistingNode(node)) {
        return;
    }
    Vector<ArcType*> toRemove;
    for (ArcType* arc : _arcs) {
        if (arc->start == node || arc->finish == node) {
            toRemove.add(arc);
        }
    }
    for (ArcType* arc : toRemove) {
        removeArc(arc);
    }
    _nodes.remove(node);
    _nodeMap.remove(node->name);
    delete node;
}

/*
 * Implementation notes: scanGraphEntry
 * ------------------------------------
 * The scanGraphEntry and its helper methods take a scanner that is
 * initialized to the input stream and has the options ignoreWhitespace,
 * scanStrings, and scanNumbers set.
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::scanGraphEntry(TokenScanner& scanner) {
    NodeType* n1 = scanNode(scanner);
    if (!n1) {
        return false;
    }
    std::string op = scanner.nextToken();
    if (op != "-" && op != "->") {
        scanner.saveToken(op);
        return true;
    }
    NodeType* n2 = scanNode(scanner);
    if (!n2) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("Graph::scanGraphEntry: Missing node after " + op);
#endif
        return false;
    }
    ArcType* forward = new ArcType();
    forward->start = n1;
    forward->finish = n2;
    addArc(forward);
    ArcType* backward = nullptr;
    if (op == "-") {
        backward = new ArcType();
        backward->start = n2;
        backward->finish = n1;
        addArc(backward);
    }
    scanArcData(scanner, forward, backward);
    return true;
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::scanNode(TokenScanner& scanner) {
    std::string token = scanner.nextToken();
    switch (scanner.getTokenType(token)) {
    case TokenScanner::WORD:
        break;
    case TokenScanner::STRING:
        token = scanner.getStringValue(token);
        break;
    default:
        scanner.saveToken(token);
        return nullptr;
    }
    NodeType* node = getNode(token);
    if (!node) {
        node = new NodeType();
        node->name = token;
        scanNodeData(scanner, node);
        addNode(node);
    }
    return node;
}

/*
 * Implementation notes: size, isEmpty
 * -----------------------------------
 * These methods are defined in terms of the node set, so the implementation
 * simply forwards the request there.  Note that it is impossible for a
 * graph to have arcs if it has no nodes.
 */
template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::size() const {
    return _nodes.size();
}

template <typename NodeType, typename ArcType>
std::string Graph<NodeType, ArcType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: operator =, copy constructor
 * -------------------------------------------------
 * These methods ensure that copying a graph creates an entirely new
 * parallel structure of nodes and arcs.
 */
template <typename NodeType, typename ArcType>
Graph<NodeType,ArcType>&
Graph<NodeType, ArcType>::operator =(const Graph& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

/*
 * Private method: deepCopy
 * ------------------------
 * Common code factored out of the copy constructor and operator= to
 * copy the contents from the other graph.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::deepCopy(const Graph& src) {
    for (NodeType* oldNode : src._nodes) {
        NodeType* newNode = new NodeType();
        *newNode = *oldNode;
        newNode->arcs.clear();
        addNode(newNode);
    }
    for (ArcType* oldArc : src._arcs) {
        ArcType* newArc = new ArcType();
        *newArc = *oldArc;
        newArc->start = getExistingNode(oldArc->start->name, "deepCopy");
        newArc->finish = getExistingNode(oldArc->finish->name, "deepCopy");
        addArc(newArc);
    }
}


/**
 * Compares two graphs for <, <=, ==, !=, >, >= relational operators.
 * Vertices are compared, including their neighboring arcs.
 *
 * @private
 */
template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::graphCompare(const Graph<NodeType, ArcType>& graph2) const {
    // optimization: if literally the same graph, return true
    if (this == &graph2) {
        return 0;
    }
    
    auto itr1 = begin();
    auto itr2 = graph2.begin();
    auto g1end = end();
    auto g2end = graph2.end();
    
    while (itr1 != g1end && itr2 != g2end) {
        // compare each pair of elements from iterators
        NodeType* node1 = *itr1;
        NodeType* node2 = *itr2;
        
        // optimization: if literally same node, equal; don't compare
        if (node1 != node2) {
            // first check names
            if (node1->name != node2->name) {
                return node1->name.compare(node2->name);
            }
            
            // then check all arcs, pairwise
            auto eitr1 = node1->arcs.begin();
            auto eitr2 = node2->arcs.begin();
            auto e1end = node1->arcs.end();
            auto e2end = node2->arcs.end();
            while (eitr1 != e1end && eitr2 != e2end) {
                ArcType* arc1 = *eitr1;
                ArcType* arc2 = *eitr2;
                
                // optimization: if literally same arc, equal; don't compare
                if (arc1 != arc2) {
                    // first check start vertex names, then end vertex names
                    if (arc1->start->name != arc2->start->name) {
                        return arc1->start->name.compare(arc2->start->name);
                    } else if (arc1->finish->name != arc2->finish->name) {
                        return arc1->finish->name.compare(arc2->finish->name);
                    }
                }
                eitr1++;
                eitr2++;
            }
            
            // if we get here, everything from me matched graph2, so either arcs equal,
            // or one is shorter than the other (fewer arcs) and is therefore less
            if (eitr1 == e1end && eitr2 == e2end) {
                // keep going
            } else if (eitr1 == e1end) {
                return -1;
            } else {
                return 1;
            }
        }
        
        // if we get here, those two vertices and their outbound arcs
        // were equal; so advance to next element
        itr1++;
        itr2++;
    }
    
    // if we get here, everything from me matched graph2, so either equal,
    // or one is shorter than the other (fewer vertices) and is therefore less
    if (itr1 == g1end && itr2 == g2end) {
        return 0;
    } else if (itr1 == g1end) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Operators
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator ==(const Graph& graph2) const {
    // optimization: if sizes not same, graphs not equal
    if (_nodes.size() != graph2._nodes.size()
            || _arcs.size() != graph2._arcs.size()
            || _nodeMap.size() != graph2._nodeMap.size()) {
        return false;
    }
    return graphCompare(graph2) == 0;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator !=(const Graph& graph2) const {
    return !(*this == graph2);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator <(const Graph& graph2) const {
    return graphCompare(graph2) < 0;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator <=(const Graph& graph2) const {
    return graphCompare(graph2) <= 0;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator >(const Graph& graph2) const {
    return graphCompare(graph2) > 0;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator >=(const Graph& graph2) const {
    return graphCompare(graph2) >= 0;
}

/**
 * Writes the given graph to the given output stream.
 * The insertion and extraction operators for graphs are more complicated
 * than for the standard collection types because the nodes and arcs can
 * contain client-specific data.  To ensure that this information is
 * correctly written and read by these operators, clients must override
 * the methods writeNodeData, writeArcData, scanNodeData, and scanArcData.
 */
template <typename NodeType, typename ArcType>
std::ostream& operator <<(std::ostream& os, const Graph<NodeType, ArcType>& g) {
    os << "{";
    bool started = false;
    for (NodeType* node : g.getNodeSet()) {
        if (started) {
            os << ", ";
        }
        writeGenericValue(os, node->name, stringIsInteger(node->name) || stringIsReal(node->name));
        g.writeNodeData(os, node);
        started = true;
    }
    for (ArcType* arc : g.getArcSet()) {
        os << ", ";
        writeGenericValue(os, arc->start->name, stringIsInteger(arc->start->name) || stringIsReal(arc->start->name));
        os << " -> ";
        writeGenericValue(os, arc->finish->name, stringIsInteger(arc->finish->name) || stringIsReal(arc->finish->name));
        g.writeArcData(os, arc);
    }
    return os << "}";
}

/**
 * Reads the given graph from the given input stream.
 * The insertion and extraction operators for graphs are more complicated
 * than for the standard collection types because the nodes and arcs can
 * contain client-specific data.  To ensure that this information is
 * correctly written and read by these operators, clients must override
 * the methods writeNodeData, writeArcData, scanNodeData, and scanArcData.
 */
template <typename NodeType, typename ArcType>
std::istream& operator >>(std::istream& is, Graph<NodeType, ArcType>& g) {
    TokenScanner scanner(is);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    scanner.addOperator("->");
    std::string token = scanner.nextToken();
    if (token != "{") {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("Graph::operator >>: Missing {");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }
    g.clear();
    while (g.scanGraphEntry(scanner)) {
        token = scanner.nextToken();
        if (token == "}") {
            scanner.saveToken(token);
        } else if (token != ",") {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
            error("Graph::operator >>: Unexpected token " + token);
#endif
            is.setstate(std::ios_base::failbit);
            return is;
        }
    }
    token = scanner.nextToken();
    if (token != "}") {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("Graph::operator >>: Missing }");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }
    return is;
}

/**
 * Template hash function for graphs.
 */
template <typename NodeType, typename ArcType>
int hashCode(const Graph<NodeType, ArcType>& graph) {
    int code = hashSeed();
    for (NodeType* node : graph) {
        code = hashMultiplier() * code + hashCode(node->name);
    }
    for (ArcType* arc : graph.getArcSet()) {
        code = hashMultiplier() * code + hashCode(arc->start->name);
        code = hashMultiplier() * code + hashCode(arc->finish->name);
    }
    return (code & hashMask());
}

#endif // _graph_h
