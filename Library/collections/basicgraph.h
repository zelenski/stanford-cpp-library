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

/**
 * Forward declarations of Vertex/Edge structures so that they can refer
 * to each other mutually.
 * @private
 */
template <typename V = void*, typename E = void*>
class EdgeGen;

/**
 * Canonical Vertex (Node) structure implementation needed by Graph class template.
 * Each Vertex structure represents a single vertex in the graph.
 */
template <typename V = void*, typename E = void*>
class VertexGen : public Observable<int> {
public:
    /**
     * The vertex's name as a string.
     */
    std::string name;

    /**
     * The edges outbound from this vertex to its neighbors.
     */
    Set<EdgeGen<V, E>*> arcs;     // required by Stanford Graph;

    /**
     * The edges outbound from this vertex to its neighbors;
     * an alias of the 'arcs' member.
     */
    Set<EdgeGen<V, E>*>& edges;

#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    /**
     * The cost to reach this vertex; initially 0.
     */
    double cost;

    /**
     * An alias for the 'cost' member.
     */
    double& weight;

    /**
     * Whether this vertex has been visited before; initally false.
     */
    bool visited;

    /**
     * The vertex that comes before this one; initially null.
     */
    VertexGen* previous;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

    /**
     * The following pointer can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    V data;

    /**
     * @private
     */
    V& extraData;       // alias of data; they are the same field

    /**
     * Constructs a vertex with the given name.
     */
    VertexGen(const std::string& name = "");

    /**
     * Copy constructor.
     */
    VertexGen(const VertexGen& other);

    /**
     * Frees up any memory dynamically allocated by this vertex.
     */
    ~VertexGen();

    /**
     * Returns the color of this vertex, if any.  Initially WHITE.
     */
    /* Color */ int getColor() const;

    /**
     * Wipes the supplementary data of this vertex back to its initial state.
     * Specifically, sets cost to 0, visited to false, and previous to nullptr.
     */
    void resetData();

    /**
     * Sets the color of this vertex to be the given color.
     * The color must be one of WHITE, GRAY, YELLOW, or GREEN.
     * Future calls to getColor will return the color you pass here.
     */
    void setColor(/* Color */ int c);

    /**
     * Returns a string representation of this vertex for debugging, such as
     * "Vertex{name=r13c42, cost=11, visited=true, previous=r12c41, neighbors={r12c41, r12c43}}".
     */
    std::string toString() const;

    /**
     * Copy assignment operator.
     */
    VertexGen& operator =(const VertexGen& other);

    /**
     * Move assignment operator.
     */
    VertexGen& operator =(VertexGen&& other);

private:
    /* Color */ int _color;   // vertex's color as passed to setColor
};

/**
 * Makes a vertex printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing a vertex is not the same as printing a vertex pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const VertexGen<V, E>& v);

/**
 * Canonical Edge (Arc) structure implementation needed by Graph class template.
 * Each Edge structure represents a single edge in the graph.
 */
template <typename V, typename E>
class EdgeGen {
public:
    /**
     * The edge's starting vertex.
     */
    VertexGen<V, E>* start;

    /**
     * The edge's ending vertex.
     */
    VertexGen<V, E>* finish;   // required by Stanford Graph;

    /**
     * An alias of finish; they are the same field.
     */
    VertexGen<V, E>*& end;

    /**
     * The edge's weight; default 0.
     */
    double cost;

    /**
     * An alias for the 'cost' member.
     */
    double& weight;

#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    /**
     * Whether this edge has been visited before; initally false.
     */
    bool visited;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

    /**
     * A pointer that can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    void* data;

    /**
     * An alias for the 'data' member.
     */
    void*& extraData;

    /**
     * Constructs a new edge between the given start/end vertices with
     * the given cost.
     */
    EdgeGen(VertexGen<V, E>* start = nullptr, VertexGen<V, E>* finish = nullptr, double cost = 0.0);

    /**
     * Frees up any memory dynamically allocated by this edge.
     */
    ~EdgeGen();

    /**
     * Wipes the supplementary data of this edge back to its initial state.
     */
    void resetData();

    /**
     * Returns a string representation of this edge for debugging, such as
     * "Arc{start=r12c42, finish=r12c41, cost=0.75}".
     */
    std::string toString() const;

    /**
     * Copy assignment operator.
     */
    EdgeGen& operator =(const EdgeGen& other);

    /**
     * Move assignment operator.
     */
    EdgeGen& operator =(EdgeGen&& other);
};

/**
 * Makes an edge printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing an edge is not the same as printing an edge pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
template <typename V, typename E>
std::ostream& operator <<(std::ostream& out, const EdgeGen<V, E>& edge);


/**
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
class BasicGraphGen : public Graph<VertexGen<V, E>, EdgeGen<V, E>> {
public:
    /**
     * Constructs a new empty graph.
     * @bigoh O(1)
     */
    BasicGraphGen();

    /**
     * Constructs a new graph containing the given vertexes.
     * @bigoh O(V)
     */
    BasicGraphGen(std::initializer_list<std::string> vertexList);

    /**
     * Adds a directed edge to the graph from vertex v1 to vertex v2.
     * Note that it is allowed to have multiple edges between the same pair of vertexes.
     *
     * Returns a pointer to the edge, though clients need not store that pointer;
     * you can get the pointer again later by calling getEdge and passing the two vertexes.
     *
     * Equivalent to addNode.
     *
     * @return a pointer to the edge created
     * @bigoh O(log V + log E)
     */
    EdgeGen<V, E>* addEdge(const std::string& v1, const std::string& v2, double cost = 0.0, bool directed = true);

    /**
     * Adds a directed edge to the graph from vertex v1 to vertex v2.
     * If either vertex is not found in the graph, said vertex will be added to the graph.
     * Note that it is allowed to have multiple edges between the same pair of vertexes.
     *
     * Returns a pointer to the edge, though clients need not store that pointer;
     * you can get the pointer again later by calling getEdge and passing the two vertexes.
     *
     * Equivalent to addNode.
     *
     * @return a pointer to the edge created
     * @throw ErrorException if either vertex is null
     * @bigoh O(log V + log E)
     */
    EdgeGen<V, E>* addEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, double cost = 0.0, bool directed = true);

    /**
     * Adds the given directed edge to the graph from vertex v1 to vertex v2.
     * If either vertex is not found in the graph, said vertex will be added to the graph.
     * Note that it is allowed to have multiple edges between the same pair of vertexes.
     *
     * Returns a pointer to the edge, though clients need not store that pointer;
     * you can get the pointer again later by calling getEdge and passing the two vertexes.
     *
     * Equivalent to addNode.
     *
     * @return a pointer to the edge created
     * @throw ErrorException if either vertex is null
     * @bigoh O(log V + log E)
     */
    EdgeGen<V, E>* addEdge(EdgeGen<V, E>* e, bool directed = true);

    /**
     * Adds a vertex to the graph, if no vertex with that name already exists in the graph.
     * This version of this method accepts a string for the vertex's name,
     * creates a new vertex of the appropriate type and initializes its fields.
     * Returns a pointer to the vertex, though clients need not store that pointer;
     * you can get the pointer again later by calling getVertex and passing the same name.
     *
     * The vertexes in a graph must have unique names.
     * If this graph already contains a vertex with the given name,
     * the vertex will not be added and the graph's state will not change.
     *
     * Equivalent to addNode.
     *
     * @return a pointer to the vertex created
     * @bigoh O(log V)
     */
    VertexGen<V, E>* addVertex(const std::string& name);

    /**
     * Adds a vertex to the graph, if no vertex with that name already exists in the graph.
     * This version of this method accepts a string for the vertex's name,
     * creates a new vertex of the appropriate type and initializes its fields.
     * The other accepts a structure representing the vertex and its data.
     * Returns a pointer to the vertex, though clients need not store that pointer;
     * you can get the pointer again later by calling getVertex and passing the same name.
     *
     * The vertexes in a graph must have unique names.
     * If this graph already contains a vertex with the given name,
     * the vertex will not be added and the graph's state will not change.
     *
     * When calling this function, you are relinquishing ownership of the Vertex
     * structure's lifecycle to the graph; our graph will free it when done with it.
     *
     * Equivalent to addNode.
     *
     * @return a pointer to the vertex created
     * @throw ErrorException if vertex is null
     * @bigoh O(log V)
     */
    VertexGen<V, E>* addVertex(VertexGen<V, E>* v);

    /**
     * Removes all edges from the graph.
     * Frees any edge objects that were internally allocated as heap storage.
     *
     * Equivalent to clearArcs.
     * @bigoh O(E log E)
     */
    void clearEdges();

    /**
     * Removes all outbound edges of the given vertex from the graph.
     * The vertex itself is not removed.
     * If the vertex is null or is not found in the graph, has no effect.
     *
     * Equivalent to clearArcs.
     * @bigoh O(E log E)
     * @bigoh O(log V + E)
     */
    void clearEdges(VertexGen<V, E>* v);

    /**
     * Removes all outbound edges of the given vertex from the graph.
     * The vertex itself is not removed.
     * If the vertex is not found in the graph, has no effect.
     *
     * Equivalent to clearArcs.
     * @bigoh O(E log E)
     */
    void clearEdges(const std::string& v);

    /**
     * Returns true if the graph has an edge from v1 to v2 in the graph.
     * If either of the vertexes supplied is null or is not found in the graph, returns false.
     *
     * Equivalent to containsArc.
     * @bigoh O(log E)
     */
    bool containsEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const;

    /**
     * Returns true if the graph has an edge from v1 to v2 in the graph.
     * If either of the vertexes supplied is not found in the graph, returns false.
     *
     * Equivalent to containsArc.
     * @bigoh O(log E)
     */
    bool containsEdge(const std::string& v1, const std::string& v2) const;

    /**
     * Returns true if the graph contains the given edge.
     * If the edge structure is null, returns false.
     *
     * Equivalent to containsArc.
     * @bigoh O(log E)
     */
    bool containsEdge(EdgeGen<V, E>* edge) const;

    /**
     * Returns whether the graph contains a vertex with the given name.
     *
     * Equivalent to containsNode.
     * @bigoh O(log V)
     */
    bool containsVertex(const std::string& name) const;

    /**
     * Returns whether the graph contains the given vertex.
     * If a null pointer is passed, returns false.
     *
     * Equivalent to containsNode.
     * @bigoh O(log V)
     */
    bool containsVertex(VertexGen<V, E>* v) const;

    /**
     * Returns the number of edges in the graph.
     *
     * Equivalent to arcCount.
     * @bigoh O(log V)
     */
    int edgeCount() const;

    /**
     * Returns the structure representing the edge from v1 to v2 in the graph.
     * If either of the vertexes supplied is null or is not found in the graph, the function will return nullptr.
     * If there are multiple edges between the given pair of vertexes, which of the edges will be returned is unspecified.
     *
     * Equivalent to getArc.
     *
     * @param v1 start vertex
     * @param v2 end vertex
     * @return edge from v1 to v2, or nullptr if there is no such edge
     * @bigoh O(log V + log E)
     */
    EdgeGen<V, E>* getEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2) const;

    /**
     * Returns the structure representing the edge from v1 to v2 in the graph.
     * If either of the vertexes supplied is not found in the graph, the function will return nullptr.
     * If there are multiple edges between the given pair of vertexes, which of the edges will be returned is unspecified.
     *
     * Equivalent to getArc.
     *
     * @param v1 start vertex
     * @param v2 end vertex
     * @return edge from v1 to v2, or nullptr if there is no such edge
     * @bigoh O(log V + log E)
     */
    EdgeGen<V, E>* getEdge(const std::string& v1, const std::string& v2) const;

    /**
     * Returns the set of all edges in the graph.
     *
     * Equivalent to getArcSet.
     * @bigoh O(1)
     */
    const Set<EdgeGen<V, E>*>& getEdgeSet() const;

    /**
     * Returns the set of all edges that start at the specified vertex.
     * If the vertex supplied is null or is not found in the graph,
     * the function will return an empty set.
     *
     * Equivalent to getArcSet.
     * @bigoh O(1)
     */
    const Set<EdgeGen<V, E>*>& getEdgeSet(VertexGen<V, E>* v) const;

    /**
     * Returns the set of all edges that start at the specified vertex.
     * If the vertex supplied is not found in the graph,
     * the function will return an empty set.
     *
     * Equivalent to getArcSet.
     * @bigoh O(1)
     */
    const Set<EdgeGen<V, E>*>& getEdgeSet(const std::string& v) const;

    /**
     * Returns the edge that is the opposite of the given edge;
     * that is, if the specified edge e starts at v1 and ends at v2,
     * will return the edge that starts at v2 and ends at v1,
     * if such an edge exists in the graph.
     *
     * If the edge supplied is nullptr, is not found in the graph,
     * or has no inverse, the function will return nullptr.
     *
     * If there are multiple edges between the given pair of vertexes,
     * which of the edges will be returned is unspecified.
     *
     * Equivalent to getInverseEdge.
     * @bigoh O(E)
     */
    EdgeGen<V, E>* getInverseArc(EdgeGen<V, E>* edge) const;

    /**
     * Returns the edge that is the opposite of the given edge;
     * that is, if the specified edge e starts at v1 and ends at v2,
     * will return the edge that starts at v2 and ends at v1,
     * if such an edge exists in the graph.
     *
     * If the edge supplied is nullptr, is not found in the graph,
     * or has no inverse, the function will return nullptr.
     *
     * If there are multiple edges between the given pair of vertexes,
     * which of the edges will be returned is unspecified.
     *
     * Equivalent to getInverseArc.
     * @bigoh O(E)
     */
    EdgeGen<V, E>* getInverseEdge(EdgeGen<V, E>* edge) const;

    /**
     * Returns the set of all edges in the graph that end at the specified vertex.
     * If the vertex supplied is null or is not found in the graph,
     * the function will return an empty set.
     *
     * Equivalent to getInverseArcSet.
     * @bigoh O(E)
     */
    const Set<EdgeGen<V, E>*> getInverseEdgeSet(VertexGen<V, E>* v) const;

    /**
     * Returns the set of all edges in the graph that end at the specified vertex.
     * If the vertex supplied is null or is not found in the graph,
     * the function will return an empty set.
     *
     * Equivalent to getInverseArcSet.
     * @bigoh O(E)
     */
    const Set<EdgeGen<V, E>*> getInverseEdgeSet(const std::string& v) const;

    /**
     * Looks up a vertex in the graph by name and returns a pointer to
     * its internal data structure.
     * If no vertex with the specified name exists, returns nullptr.
     * @bigoh O(log V)
     */
    VertexGen<V, E>* getVertex(const std::string& name) const;

    /**
     * Returns a set of the names of all vertexes in the graph.
     * The vertexes will be sorted by name in case-sensitive alphabetical order.
     * @bigoh O(V log V)
     */
    Set<std::string> getVertexNames() const;

    /**
     * Returns the set of all vertexes in the graph.
     * The vertexes will be sorted by name in case-sensitive alphabetical order.
     * @bigoh O(V log V)
     */
    const Set<VertexGen<V, E>*>& getVertexSet() const;

    /**
     * Removes the edge from v1 to v2 from the graph.
     * If more than one edge connects the specified endpoints, all of them are removed.
     * If either of the vertexes supplied is not found in the graph,
     * calling this function will have no effect on the graph.
     * @bigoh O(E + log V)
     */
    void removeEdge(const std::string& v1, const std::string& v2, bool directed = true);

    /**
     * Removes the edge from v1 to v2 from the graph.
     * If more than one edge connects the specified endpoints, all of them are removed.
     * If either of the vertexes supplied is null or not found in the graph,
     * calling this function will have no effect on the graph.
     * @bigoh O(E + log V)
     */
    void removeEdge(VertexGen<V, E>* v1, VertexGen<V, E>* v2, bool directed = true);

    /**
     * Removes the given edge from the graph.
     * If the edge supplied is not found in the graph,
     * calling this function will have no effect on the graph.
     * @bigoh O(log E + log V)
     */
    void removeEdge(EdgeGen<V, E>* e, bool directed = true);

    /**
     * Removes the given vertex from the graph.
     * If the vertex is not found in the graph, the call has no effect.
     * @bigoh O(E + log V)
     */
    void removeVertex(const std::string& name);

    /**
     * Removes the given vertex from the graph.
     * If the vertex is null or is not found in the graph, the call has no effect.
     * @bigoh O(E + log V)
     */
    void removeVertex(VertexGen<V, E>* v);

    /**
     * Sets the data stored in each vertex and edge back to its original value
     * by calling resetData on every vertex and edge.
     * @private
     * @bigoh O(V + E)
     */
    void resetData();

    /**
     * Sets whether the resetData method is enabled (default true).
     * @private
     * @bigoh O(1)
     */
    void setResetEnabled(bool enabled);

    /* @inherit */
    virtual void scanArcData(TokenScanner& scanner, EdgeGen<V, E>* edge, EdgeGen<V, E>* inverse);

    /* @inherit */
    virtual void writeArcData(std::ostream& out, EdgeGen<V, E>* edge) const;

    /**
     * Returns a Map representing an adjacency list equivalent to this graph.
     * Each vertex's name is a key in the map, and its neighboring vertexes' names
     * are stored in a Set as the value associated with that key.
     * It should be noted that this member does not preserve the weights of the edges
     * between the neighboring vertexes, so it is not ideal for use with weighted graphs.
     * @bigoh O(V + E)
     */
    Map<std::string, Set<std::string>> toMap() const;

    /**
     * Returns the number of vertexes in the graph.
     * Equivalent to size.
     * @bigoh O(1)
     */
    int vertexCount() const;

    /**
     * Overloads <code>[]</code> to return vertex pointers by vertex name.
     * @bigoh O(log V)
     */
    VertexGen<V, E>* operator [](const std::string& name);

    /**
     * Overloads <code>[]</code> to return vertex pointers by vertex name.
     * @bigoh O(log V)
     */
    const VertexGen<V, E>* operator [](const std::string& name) const;

private:
    bool m_resetEnabled;
};

/**
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

/**
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
      _color(other._color) {
    // empty
}

template <typename V, typename E>
VertexGen<V, E>::~VertexGen() {
    // empty
}

template <typename V, typename E>
int VertexGen<V, E>::getColor() const {
    return _color;
}

template <typename V, typename E>
void VertexGen<V, E>::resetData() {
#ifdef SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    cost = 0.0;
    previous = nullptr;
    visited = false;
#endif // SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS
    _color = /* UNCOLORED */ 0;
}

template <typename V, typename E>
void VertexGen<V, E>::setColor(int c) {
    _color = c;
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
        _color = other._color;
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
        _color = other._color;
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

/**
 * Writes the given edge structure to the given output stream.
 */
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
BasicGraphGen<V, E>::BasicGraphGen() : Graph<VertexGen<V, E>, EdgeGen<V, E>>() {
    m_resetEnabled = true;
}

template <typename V, typename E>
BasicGraphGen<V, E>::BasicGraphGen(std::initializer_list<std::string> vertexList)
        : Graph<VertexGen<V, E>, EdgeGen<V, E>>() {
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

/**
 * Template hash function for graphs.
 */
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

/**
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

/**
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

/**
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

/**
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

/**
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

/**
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

/**
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

/**
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

#endif // _basicgraph_h
