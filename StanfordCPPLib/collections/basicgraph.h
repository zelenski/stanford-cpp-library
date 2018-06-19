/*
 * File: basicgraph.h
 * ------------------
 * This file contains the declaration of some useful graph types,
 * specifically the Vertex and Edge structures used in the typical graph.
 * together in lecture.  We also declare BasicGraph, an instantiation of
 * Stanford's Graph class using Vertex and Edge as its type parameters.
 *
 * Most members are implemented in this file, since the class is a template.
 * See BasicGraph.cpp for implementation of some non-template members.
 *
 * @author Marty Stepp
 * @version 2018/03/10
 * - added methods front, back, toMap
 * - added operator << for various collections of Vertex* and Edge*
 * @version 2018/02/28
 * - compiler flag to enable/disable Vertex fields like visited, previous
 * @version 2017/11/14
 * - fix missing "this->" on some methods
 * - added getVertexNames, vertexCount, edgeCount
 * @version 2017/10/24
 * - print nullptr instead of null in uppercase
 * @version 2017/10/18
 * - fix compiler warnings
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via Graph)
 * @version 2016/12/04
 * - bug fix in resetData method (was referring to Vertex* and Edge*)
 * @version 2016/12/01
 * - bug fix in addEdge method (now correctly adds vertexes if not present)
 * - fixed bugs in some node/arc methods (should not crash on empty/null args)
 * @version 2016/11/26
 * - added BasicGraphV, VertexV, EdgeV macros
 * - added getInverseEdgeSet
 * @version 2016/11/19
 * - changed addVertex to not throw when adding duplicates
 * - added operator []
 * @version 2016/11/07
 * - small bug fix for printing cost in operator << (courtesy GitHub @hsivnightbeam)
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - bug fix for hashCode function
 * @version 2016/08/12
 * - added initializer_list functionality
 * @version 2014/12/04
 * - bug fix: Edge end reference was not properly pointing at finish field
 * @version 2014/11/21
 * - bug fix: Edge weight reference was not properly pointing at cost field
 * @version 2014/11/13
 * - added iterator begin(), end() support so that students can directly
 *   for-each over the vertices of a graph.
 * - added comparison operators ==, !=, <, etc.
 * - bug fix to Edge to add move/copy = operators
 * @version 2014/10/31
 * - bug fix: extraData was being nulled accidentally in Vertex::resetData()
 * @version 2014/10/23
 * - bug fixes based on cppcheck lint output
 * @version 2014/10/20
 * - converted functions to accept const string& rather than string for speed
 * @version 2014/08/16
 * - added Vertex 'weight' alias for 'cost'
 * - added Edge 'weight' alias for 'cost, and 'end' alias for 'finish'
 */

#ifndef _basicgraph_h
#define _basicgraph_h

#include <initializer_list>
#include <sstream>
#include <string>
#include <utility>
#include "gmath.h"
#include "graph.h"
#include "grid.h"
#include "hashset.h"
#include "linkedlist.h"
#include "observable.h"
#include "set.h"
#include "vector.h"

/*
 * Forward declarations of Vertex/Edge structures so that they can refer
 * to each other mutually.
 */
template <typename V = void*, typename E = void*>
class EdgeGen;

/*
 * Canonical Vertex (Node) structure implementation needed by Graph class template.
 * Each Vertex structure represents a single vertex in the graph.
 */
template <typename V = void*, typename E = void*>
class VertexGen : public Observable<int> {
public:
    std::string name;             // required by Stanford Graph; vertex's name
    Set<EdgeGen<V, E>*> arcs;     // required by Stanford Graph; edges outbound from this vertex; to neighbors
    Set<EdgeGen<V, E>*>& edges;   // alias of arcs; preferred name

#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    /*
     * The following three fields are 'supplementary data' inside each vertex.
     * You can use them in your path-searching algorithms to store various
     * information related to the vertex
     */
    double cost;        // cost to reach this vertex (initially 0; you can set this)
    double& weight;     // alias of cost; they are the same field
    bool visited;       // whether this vertex has been visited before (initally false; you can set this)
    VertexGen* previous;   // vertex that comes before this one (initially nullptr; you can set this)
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

    /*
     * The following pointer can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    V data;
    V& extraData;       // alias of data; they are the same field

    /*
     * Constructs a vertex with the given name.
     */
    VertexGen(const std::string& name = "");

    /*
     * Copy constructor (rule of three).
     */
    VertexGen(const VertexGen& other);

    /*
     * Frees up any memory dynamically allocated by this vertex.
     */
    ~VertexGen();

    /*
     * Returns the color of this vertex, if any.  Initially WHITE.
     */
    /* Color */ int getColor() const;

    /*
     * Wipes the supplementary data of this vertex back to its initial state.
     * Specifically, sets cost to 0, visited to false, and previous to nullptr.
     */
    void resetData();

    /*
     * Sets the color of this vertex to be the given color.
     * The color must be one of WHITE, GRAY, YELLOW, or GREEN.
     * Future calls to getColor will return the color you pass here.
     */
    void setColor(/* Color */ int c);

    /*
     * Returns a string representation of this vertex for debugging, such as
     * "Vertex{name=r13c42, cost=11, visited=true, previous=r12c41, neighbors={r12c41, r12c43}}".
     */
    std::string toString() const;

    /*
     * Copy assignment operator (rule of three).
     */
    VertexGen& operator =(const VertexGen& other);

    /*
     * Move assignment operator (rule of three).
     */
    VertexGen& operator =(VertexGen&& other);

private:
    /* Color */ int m_color;   // vertex's color as passed to setColor
};

/*
 * Makes a vertex printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing a vertex is not the same as printing a vertex pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const VertexGen<V, E>& v);

/*
 * Canonical Edge (Arc) structure implementation needed by Graph class template.
 * Each Edge structure represents a single edge in the graph.
 */
template <typename V, typename E>
class EdgeGen {
public:
    VertexGen<V, E>* start;    // required by Stanford Graph; edge's starting vertex
    VertexGen<V, E>* finish;   // required by Stanford Graph; edge's ending vertex
    VertexGen<V, E>*& end;     // alias of finish; they are the same field
    double cost;               // required by Stanford Graph; edge weight
    double& weight;            // alias of cost; they are the same field

#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    bool visited;     // whether this edge has been visited before (initally false; you can set this)
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

    /*
     * The following pointer can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    void* data;
    void*& extraData;          // alias of data; they are the same field

    /*
     * Constructs a new edge between the given start/end vertices with
     * the given cost.
     */
    EdgeGen(VertexGen<V, E>* start = nullptr, VertexGen<V, E>* finish = nullptr, double cost = 0.0);

    /*
     * Frees up any memory dynamically allocated by this edge.
     */
    ~EdgeGen();

    /*
     * Wipes the supplementary data of this edge back to its initial state.
     * Specifically, sets visited to false.
     */
    void resetData();

    /*
     * Returns a string representation of this edge for debugging, such as
     * "Arc{start=r12c42, finish=r12c41, cost=0.75}".
     */
    std::string toString() const;

    /*
     * Copy assignment operator (rule of three).
     */
    EdgeGen& operator =(const EdgeGen& other);

    /*
     * Move assignment operator (rule of three).
     */
    EdgeGen& operator =(EdgeGen&& other);
};

/*
 * Makes an arc printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing an arc is not the same as printing an arc pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const EdgeGen<V, E>& edge);


/*
 * BasicGraph is just basically an instantiation of Graph using Vertex and Edge
 * as its template parameters.  It also adds a few convenience functions such
 * as mirroring members like "addArc" with an equivalent more familiar name
 * like "addEdge".
 *
 * There are a few convenience functions related to neighbors, like isNeighbor.
 * BasicGraph contains a DFS implementation called isReachable, not found
 * in the normal Stanford Graph class.
 *
 * There are also a few functions added to make edges more convenient to work with:
 * getEdge(v1, v2) to get the edge between a given pair of vertices,
 * and getInverseEdge(edge) to get the edge v2 -> v1 for a given edge v1 -> v2.
 */
template <typename V = void*, typename E = void*>
class BasicGraphGen : public Graph<VertexGen<V, E>, EdgeGen<V, E> > {
public:
    /*
     * Newly added behavior in BasicGraphGen.
     */
    BasicGraphGen();
    BasicGraphGen(std::initializer_list<std::string> vertexList);
    void clearEdges();
    void clearEdges(VertexGen<V, E>* v);
    void clearEdges(const std::string& v);
    bool containsEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const;
    bool containsEdge(const std::string& v1, const std::string& v2) const;
    bool containsEdge(EdgeGen<V, E>* edge) const;
    bool containsVertex(const std::string& name) const;
    bool containsVertex(VertexGen<V, E>* v) const;
    EdgeGen<V, E>* getEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const;
    EdgeGen<V, E>* getEdge(const std::string& v1, const std::string& v2) const;
    EdgeGen<V, E>* getInverseArc(EdgeGen<V, E>* edge) const;
    EdgeGen<V, E>* getInverseEdge(EdgeGen<V, E>* edge) const;
    void resetData();
    void setResetEnabled(bool enabled);
    virtual void scanArcData(TokenScanner& scanner, EdgeGen<V, E>* edge, EdgeGen<V, E>* inverse);
    virtual void writeArcData(std::ostream& out, EdgeGen<V, E>* edge) const;

    /*
     * The members below are mirrors of ones from Graph but with 'Node' changed
     * to 'Vertex' and/or 'Arc' changed to 'Edge', with identical behavior,
     * and so they are not documented in detail.  See Graph documentation.
     */
    EdgeGen<V, E>* addEdge(const std::string& v1, const std::string& v2, double cost = 0.0, bool directed = true);
    EdgeGen<V, E>* addEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, double cost = 0.0, bool directed = true);
    EdgeGen<V, E>* addEdge(EdgeGen<V, E>* e, bool directed = true);
    VertexGen<V, E>* addVertex(const std::string& name);
    VertexGen<V, E>* addVertex(VertexGen<V, E>* v);
    int edgeCount() const;
    const Set<EdgeGen<V, E>*>& getEdgeSet() const;
    const Set<EdgeGen<V, E>*>& getEdgeSet(VertexGen<V, E>* v) const;
    const Set<EdgeGen<V, E>*>& getEdgeSet(const std::string& v) const;
    const Set<EdgeGen<V, E>*> getInverseEdgeSet(VertexGen<V, E>* v) const;
    const Set<EdgeGen<V, E>*> getInverseEdgeSet(const std::string& v) const;
    VertexGen<V, E>* getVertex(const std::string& name) const;
    Set<std::string> getVertexNames() const;
    const Set<VertexGen<V, E>*>& getVertexSet() const;
    void removeEdge(const std::string& v1, const std::string& v2, bool directed = true);
    void removeEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, bool directed = true);
    void removeEdge(EdgeGen<V, E>* e, bool directed = true);
    void removeVertex(const std::string& name);
    void removeVertex(VertexGen<V, E>* v);

    /*
     * Returns a Map representing an adjacency list equivalent to this graph.
     * Each vertex's name is a key in the map, and its neighboring vertexes' names
     * are stored in a Set as the value associated with that key.
     * It should be noted that this member does not preserve the weights of the edges
     * between the neighboring vertexes, so it is not ideal for use with weighted graphs.
     */
    Map<std::string, Set<std::string>> toMap() const;

    int vertexCount() const;

    /*
     * Operator: []
     * Usage: graph[vertexName]
     * ------------------------
     * Overloads <code>[]</code> return vertex pointers by vertex name.
     */
    VertexGen<V, E>* operator [](const std::string& name);
    const VertexGen<V, E>* operator [](const std::string& name) const;

private:
    bool m_resetEnabled;
};

/*
 * Hash function for BasicGraphGen.
 */
template <typename V, typename E>
int hashCode(const BasicGraphGen<V, E>& graph);

/*
 * Defines a Vertex to be a VertexGen with its data bound to be a void*.
 * Retained for backward compatibility.
 */
typedef VertexGen<void*, void*> Vertex;
#define VertexV VertexGen

/*
 * You can refer to a Vertex as a Node if you prefer.
 */
#define Node Vertex

/*
 * Defines an Edge to be an EdgeGen with its data bound to be a void*.
 * Retained for backward compatibility.
 */
typedef EdgeGen<void*, void*> Edge;
#define EdgeV EdgeGen

/*
 * You can refer to an Edge as an Arc if you prefer.
 */
#define Arc Edge

/*
 * Defines a BasicGraph to be a BasicGraphGen with its data bound to be a void*.
 * Retained for backward compatibility.
 */
typedef BasicGraphGen<void*, void*> BasicGraph;
#define BasicGraphV BasicGraphGen

/*
 * Hash function for BasicGraph.
 */
int hashCode(const BasicGraph& graph);


/*
 * Vertex member implementations
 */
template <typename V, typename E>
VertexGen<V, E>::VertexGen(const std::string& theName)
        : name(theName),
          edges(arcs),
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
          weight(cost),
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
          data(V()),
          extraData(data) {
    resetData();
}

template <typename V, typename E>
VertexGen<V, E>::VertexGen(const VertexGen& other)
    : name(other.name),
      arcs(other.arcs),
      edges(arcs),
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
      cost(other.cost),
      weight(cost),
      visited(other.visited),
      previous(other.previous),
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
      data(other.data),
      extraData(data),
      m_color(other.m_color) {
    // empty
}

template <typename V, typename E>
VertexGen<V, E>::~VertexGen() {
    // empty
}

template <typename V, typename E>
int VertexGen<V, E>::getColor() const {
    return m_color;
}

template <typename V, typename E>
void VertexGen<V, E>::resetData() {
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    cost = 0.0;
    previous = nullptr;
    visited = false;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    m_color = /* UNCOLORED */ 0;
}

template <typename V, typename E>
void VertexGen<V, E>::setColor(int c) {
    m_color = c;
    notifyObservers();
}

template <typename V, typename E>
std::string VertexGen<V, E>::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

template <typename V, typename E>
VertexGen<V, E>& VertexGen<V, E>::operator =(const VertexGen& other) {
    if (this != &other) {
        name = other.name;
        arcs = other.arcs;
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
        cost = other.cost;
        visited = other.visited;
        previous = other.previous;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
        data = other.data;
        m_color = other.m_color;
    }
    return *this;
}

template <typename V, typename E>
VertexGen<V, E>& VertexGen<V, E>::operator =(VertexGen&& other) {
    if (this != &other) {
        name = other.name;
        arcs = other.arcs;
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
        cost = other.cost;
        visited = other.visited;
        previous = other.previous;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
        data = other.data;
        m_color = other.m_color;
    }
    return *this;
}

template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const VertexGen<V, E>& v) {
    out << "Vertex{name=" << v.name;

#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    if (!floatingPointEqual(v.cost, 0.0)) {
        out << ", cost=" << v.cost;
    }
    out << ", visited=" << (v.visited ? "true" : "false");
    out << ", previous=" << (v.previous == nullptr ? std::string("nullptr") : v.previous->name);
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

    // print neighbors
    out << ", neighbors={";
    int i = 0;
    for (EdgeGen<V, E>* edge : v.edges) {
        if (i > 0) {
            out << ", ";
        }
        i++;
        if (edge->finish) {
            out << edge->finish->name;
        } else {
            out << "nullptr";
        }
    }
    out << "}";   // end of neighbors

    out << "}";   // end of Vertex
    return out;
}


/*
 * Edge member implementations
 */
template <typename V, typename E>
EdgeGen<V, E>::EdgeGen(VertexGen<V, E>* theStart, VertexGen<V, E>* theFinish, double theCost)
        : start(theStart),
          finish(theFinish),
          end(this->finish),
          cost(theCost),
          weight(this->cost),
          data(nullptr),
          extraData(data) {
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    this->extraData = nullptr;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    this->resetData();
}

template <typename V, typename E>
EdgeGen<V, E>::~EdgeGen() {
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    if (this->extraData != nullptr) {
        this->extraData = nullptr;
        // delete this->extraData;
    }
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
}

template <typename V, typename E>
void EdgeGen<V, E>::resetData() {
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    this->visited = false;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
}

template <typename V, typename E>
std::string EdgeGen<V, E>::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

template <typename V, typename E>
EdgeGen<V, E>& EdgeGen<V, E>::operator =(const EdgeGen& other) {
    if (this != &other) {
        start = other.start;
        finish = other.finish;
        cost = other.cost;
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
        visited = other.visited;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    }
    return *this;
}

template <typename V, typename E>
EdgeGen<V, E>& EdgeGen<V, E>::operator =(EdgeGen&& other) {
    start = other.start;
    finish = other.finish;
    cost = other.cost;
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    visited = other.visited;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    return *this;
}

template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const EdgeGen<V, E>& edge) {
    out << "Edge{start=";
    if (!edge.start) {
        out << "nullptr";
    } else {
        out << edge.start->name;
    }
    out << ", finish=";
    if (!edge.finish) {
        out << "nullptr";
    } else {
        out << edge.finish->name;
    }
    if (!floatingPointEqual(edge.cost, 0.0)) {
        out << ", cost=" << edge.cost;
    }
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    if (edge.visited) {
        out << ", visited=" << (edge.visited ? "true" : "false");
    }
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    out << "}";
    return out;
}


/*
 * BasicGraph member implementations
 */
template <typename V, typename E>
BasicGraphGen<V, E>::BasicGraphGen() : Graph<VertexGen<V, E>, EdgeGen<V, E> >() {
    m_resetEnabled = true;
}

template <typename V, typename E>
BasicGraphGen<V, E>::BasicGraphGen(std::initializer_list<std::string> vertexList)
        : Graph<VertexGen<V, E>, EdgeGen<V, E> >() {
    m_resetEnabled = true;
    for (const std::string& vertexName : vertexList) {
        this->addVertex(vertexName);
    }
}

template <typename V, typename E>
void BasicGraphGen<V, E>::clearEdges() {
    this->clearArcs();
}

template <typename V, typename E>
void BasicGraphGen<V, E>::clearEdges(VertexGen<V, E>* v) {
    this->clearArcs(v);
}

template <typename V, typename E>
void BasicGraphGen<V, E>::clearEdges(const std::string& v) {
    this->clearArcs(v);
}

template <typename V, typename E>
bool BasicGraphGen<V, E>::containsEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const {
    return this->containsArc(v1, v2);
}

template <typename V, typename E>
bool BasicGraphGen<V, E>::containsEdge(const std::string& v1, const std::string& v2) const {
    return this->containsArc(v1, v2);
}

template <typename V, typename E>
bool BasicGraphGen<V, E>::containsEdge(EdgeGen<V, E>* edge) const {
    return this->containsArc(edge);
}

template <typename V, typename E>
bool BasicGraphGen<V, E>::containsVertex(const std::string& name) const {
    return this->containsNode(name);
}

template <typename V, typename E>
bool BasicGraphGen<V, E>::containsVertex(VertexGen<V, E>* v) const {
    return this->containsNode(v);
}

template <typename V, typename E>
int BasicGraphGen<V, E>::edgeCount() const {
    return this->arcCount();
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::getEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const {
    return this->getArc(v1, v2);
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::getEdge(const std::string& v1, const std::string& v2) const {
    return this->getArc(v1, v2);
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::getInverseArc(EdgeGen<V, E>* edge) const {
    return this->getArc(edge->finish, edge->start);
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::getInverseEdge(EdgeGen<V, E>* edge) const {
    return this->getInverseArc(edge);
}

template <typename V, typename E>
void BasicGraphGen<V, E>::resetData() {
    if (m_resetEnabled) {
        for (VertexGen<V, E>* v : this->getVertexSet()) {
            v->resetData();
        }
        for (EdgeGen<V, E>* e : this->getEdgeSet()) {
            e->resetData();
        }
    }
}

template <typename V, typename E>
void BasicGraphGen<V, E>::setResetEnabled(bool enabled) {
    m_resetEnabled = enabled;
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::addEdge(const std::string& v1, const std::string& v2, double cost, bool directed) {
    if (!this->containsVertex(v1)) {
        this->addVertex(v1);
    }
    if (!this->containsVertex(v2)) {
        this->addVertex(v2);
    }
    return this->addEdge(this->getVertex(v1), this->getVertex(v2), cost, directed);
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::addEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, double cost, bool directed) {
    EdgeGen<V, E>* e = new EdgeGen<V, E>(v1, v2, cost);
    return this->addEdge(e, directed);
}

template <typename V, typename E>
EdgeGen<V, E>* BasicGraphGen<V, E>::addEdge(EdgeGen<V, E>* e, bool directed) {
    EdgeGen<V, E>* result = this->addArc(e);
    if (!directed) {
        EdgeGen<V, E>* result2 = this->addArc(e->finish, e->start);
        result2->cost = e->cost;
    }
    return result;
}

template <typename V, typename E>
VertexGen<V, E>* BasicGraphGen<V, E>::addVertex(const std::string& name) {
    return this->addNode(name);
}

template <typename V, typename E>
VertexGen<V, E>* BasicGraphGen<V, E>::addVertex(VertexGen<V, E>* v) {
    return this->addNode(v);
}

template <typename V, typename E>
const Set<EdgeGen<V, E>*>& BasicGraphGen<V, E>::getEdgeSet() const {
    return this->getArcSet();
}

template <typename V, typename E>
const Set<EdgeGen<V, E>*>& BasicGraphGen<V, E>::getEdgeSet(VertexGen<V, E>* v) const {
    return this->getArcSet(v);
}

template <typename V, typename E>
const Set<EdgeGen<V, E>*>& BasicGraphGen<V, E>::getEdgeSet(const std::string& v) const {
    return this->getArcSet(v);
}

template <typename V, typename E>
const Set<EdgeGen<V, E>*> BasicGraphGen<V, E>::getInverseEdgeSet(VertexGen<V, E>* v) const {
    return this->getInverseArcSet(v);
}

template <typename V, typename E>
const Set<EdgeGen<V, E>*> BasicGraphGen<V, E>::getInverseEdgeSet(const std::string& v) const {
    return this->getInverseArcSet(v);
}


template <typename V, typename E>
VertexGen<V, E>* BasicGraphGen<V, E>::getVertex(const std::string& name) const {
    return this->getNode(name);
}

template <typename V, typename E>
Set<std::string> BasicGraphGen<V, E>::getVertexNames() const {
    return this->getNodeNames();
}


template <typename V, typename E>
const Set<VertexGen<V, E>*>& BasicGraphGen<V, E>::getVertexSet() const {
    return this->getNodeSet();
}

template <typename V, typename E>
void BasicGraphGen<V, E>::removeEdge(const std::string& v1, const std::string& v2, bool directed) {
    this->removeEdge(this->getVertex(v1), this->getVertex(v2), directed);
}

template <typename V, typename E>
void BasicGraphGen<V, E>::removeEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, bool directed) {
    this->removeArc(v1, v2);
    if (!directed) {
        this->removeArc(v2, v1);
    }
}

template <typename V, typename E>
void BasicGraphGen<V, E>::removeEdge(EdgeGen<V, E>* e, bool directed) {
    this->removeArc(e);
    if (!directed) {
        this->removeArc(e->finish, e->start);
    }
}

template <typename V, typename E>
void BasicGraphGen<V, E>::removeVertex(const std::string& name) {
    this->removeNode(name);
}

template <typename V, typename E>
void BasicGraphGen<V, E>::removeVertex(VertexGen<V, E>* v) {
    this->removeNode(v);
}

template <typename V, typename E>
Map<std::string, Set<std::string>> BasicGraphGen<V, E>::toMap() const {
    Map<std::string, Set<std::string>> result;
    for (Vertex* v : this->getVertexSet()) {
        Set<std::string> neighborSet;
        for (Vertex* neighbor : this->getNeighbors(v)) {
            neighborSet += neighbor->name;
        }
        result[v->name] = neighborSet;
    }
    return result;
}

template <typename V, typename E>
int BasicGraphGen<V, E>::vertexCount() const {
    return this->nodeCount();
}

template <typename V, typename E>
VertexGen<V, E>* BasicGraphGen<V, E>::operator [](const std::string& name) {
    return this->getVertex(name);
}

template <typename V, typename E>
const VertexGen<V, E>* BasicGraphGen<V, E>::operator [](const std::string& name) const {
    return this->getVertex(name);
}

template <typename V, typename E>
void BasicGraphGen<V, E>::scanArcData(TokenScanner& scanner, EdgeGen<V, E>* edge, EdgeGen<V, E>* inverse) {
    std::string colon = scanner.nextToken();   // ":", skip over
    if (colon == ":") {
        std::string costStr = scanner.nextToken();
        edge->cost = stringToReal(costStr);
        if (inverse) {
            inverse->cost = edge->cost;
        }
    } else {
        // no cost for this edge (cost 0); un-read the colon token because
        // it probably wasn't actually a colon
        scanner.saveToken(colon);
    }
}

template <typename V, typename E>
void BasicGraphGen<V, E>::writeArcData(std::ostream& out, EdgeGen<V, E>* edge) const {
    if (!floatingPointEqual(edge->cost, 0.0)) {
        out << " : ";
        out << edge->cost;
    }
}

template <typename V, typename E>
int hashCode(const BasicGraphGen<V, E>& graph) {
    int code = hashSeed();
    for (VertexGen<V, E>* v : graph) {
        code = hashMultiplier() * code + hashCode(v->name);
    }
    for (EdgeGen<V, E>* e : graph.getEdgeSet()) {
        code = hashMultiplier() * code + hashCode(e->start->name);
        code = hashMultiplier() * code + hashCode(e->finish->name);
    }
    return (code & hashMask());
}

/*
 * Overloaded operator to print a set of edge pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const HashSet<EdgeGen<V, E>*>& sete) {
    out << "{";
    if (!sete.isEmpty()) {
        bool first = true;
        for (EdgeGen<V, E>* e : sete) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (e) {
                if (e->start) {
                    out << e->start->name;
                } else {
                    out << "null";
                }
                out << " -> ";
                if (e->finish) {
                    out << e->finish->name;
                } else {
                    out << "null";
                }
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a set of vertex pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const HashSet<VertexGen<V, E>*>& setv) {
    out << "{";
    if (!setv.isEmpty()) {
        bool first = true;
        for (VertexGen<V, E>* v : setv) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (v) {
                out << v->name;
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a list of edge pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const LinkedList<EdgeGen<V, E>*>& liste) {
    out << "{";
    if (!liste.isEmpty()) {
        bool first = true;
        for (EdgeGen<V, E>* e : liste) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (e) {
                if (e->start) {
                    out << e->start->name;
                } else {
                    out << "null";
                }
                out << " -> ";
                if (e->finish) {
                    out << e->finish->name;
                } else {
                    out << "null";
                }
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a list of vertex pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const LinkedList<VertexGen<V, E>*>& lst) {
    out << "{";
    if (!lst.isEmpty()) {
        bool first = true;
        for (VertexGen<V, E>* v : lst) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (v) {
                out << v->name;
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a set of edge pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const Set<EdgeGen<V, E>*>& sete) {
    out << "{";
    if (!sete.isEmpty()) {
        bool first = true;
        for (EdgeGen<V, E>* e : sete) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (e) {
                if (e->start) {
                    out << e->start->name;
                } else {
                    out << "null";
                }
                out << " -> ";
                if (e->finish) {
                    out << e->finish->name;
                } else {
                    out << "null";
                }
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a set of vertex pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const Set<VertexGen<V, E>*>& setv) {
    out << "{";
    if (!setv.isEmpty()) {
        bool first = true;
        for (VertexGen<V, E>* v : setv) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (v) {
                out << v->name;
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a vector of edge pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const Vector<EdgeGen<V, E>*>& vece) {
    out << "{";
    if (!vece.isEmpty()) {
        bool first = true;
        for (EdgeGen<V, E>* e : vece) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (e) {
                if (e->start) {
                    out << e->start->name;
                } else {
                    out << "null";
                }
                out << " -> ";
                if (e->finish) {
                    out << e->finish->name;
                } else {
                    out << "null";
                }
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

/*
 * Overloaded operator to print a vector of vertex pointers.
 * Normally it is unwise to override operators for printing pointers,
 * because the pointer could be null or garbage.
 * But in this case we have decided that it is better for students if their
 * attempts to print collections of vertexes and edges are easy to read.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const Vector<VertexGen<V, E>*>& vec) {
    out << "{";
    if (!vec.isEmpty()) {
        bool first = true;
        for (VertexGen<V, E>* v : vec) {
            if (!first) {
                out << ", ";
            }
            first = false;
            if (v) {
                out << v->name;
            } else {
                out << "null";
            }
        }
    }
    out << "}";
    return out;
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _basicgraph_h
