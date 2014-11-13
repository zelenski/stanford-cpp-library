/*
 * File: basicgraph.h
 * ------------------
 * This file contains the declaration of some useful graph types,
 * specifically the Vertex and Edge structures used in the typical graph.
 * together in lecture.  We also declare BasicGraph, an instantiation of
 * Stanford's Graph class using Vertex and Edge as its type parameters.
 *
 * See BasicGraph.cpp for implementation of each member.
 *
 * @author Marty Stepp
 * @version 2014/11/13
 * - added iterator begin(), end() support so that students can directly
 *   for-each over the vertices of a graph.
 * - added comparison operators ==, !=, <, etc.
 * - added template hashCode function
 * - bug fix to Edge to add move/copy = operators
 * @version 2014/10/20
 * - converted functions to accept const string& rather than string for speed
 * @version 2014/08/16
 * - added Vertex 'weight' alias for 'cost'
 * - added Edge 'weight' alias for 'cost, and 'end' alias for 'finish'
 */

#ifndef _basicgraph_h
#define _basicgraph_h

#include <string>
#include "graph.h"
#include "grid.h"
#include "observable.h"
#include "set.h"

/*
 * Forward declarations of Vertex/Edge structures so that they can refer
 * to each other mutually.
 */
struct Vertex;
struct Edge;

/*
 * Canonical Vertex (Node) structure implementation needed by Graph class template.
 * Each Vertex structure represents a single vertex in the graph.
 */
struct Vertex : public Observable {
public:
    std::string name;    // required by Stanford Graph; vertex's name
    Set<Edge*> arcs;     // edges outbound from this vertex; to neighbors
    Set<Edge*>& edges;   // alias of arcs; preferred name

    /*
     * The following three fields are 'supplementary data' inside each vertex.
     * You can use them in your path-searching algorithms to store various
     * information related to the vertex
     */
    double cost;        // cost to reach this vertex (initially 0; you can set this)
    double& weight;     // alias of cost; they are the same field
    bool visited;       // whether this vertex has been visited before (initally false; you can set this)
    Vertex* previous;   // vertex that comes before this one (initially NULL; you can set this)

    /*
     * The following pointer can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    void* extraData;

    /*
     * Constructs a vertex with the given name.
     */
    Vertex(const std::string& name = "");

    /*
     * Copy constructor (rule of three).
     */
    Vertex(const Vertex& other);

    /*
     * Frees up any memory dynamically allocated by this vertex.
     */
    ~Vertex();

    /*
     * Returns the color of this vertex, if any.  Initially WHITE.
     */
    /* Color */ int getColor() const;

    /*
     * Wipes the supplementary data of this vertex back to its initial state.
     * Specifically, sets cost to 0, visited to false, and previous to NULL.
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
    Vertex& operator =(const Vertex& other);

    /*
     * Move assignment operator (rule of three).
     */
    Vertex& operator =(Vertex&& other);

private:
    /* Color */ int m_color;   // vertex's color as passed to setColor
};

/*
 * Makes a vertex printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing a vertex is not the same as printing a vertex pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
std::ostream& operator<<(std::ostream& out, const Vertex& v);

/*
 * You can refer to a Vertex as a Node if you prefer.
 */
#define Node Vertex

/*
 * Canonical Edge (Arc) structure implementation needed by Graph class template.
 * Each Edge structure represents a single edge in the graph.
 */
struct Edge {
public:
    Vertex* start;    // edge's starting vertex (required by Graph)
    Vertex* finish;   // edge's ending vertex (required by Graph)
    Vertex*& end;     // alias of finish; they are the same field
    double cost;      // edge weight (required by Graph)
    double& weight;   // alias of cost; they are the same field
    bool visited;     // whether this edge has been visited before (initally false; you can set this)

    /*
     * The following pointer can point to any extra data needed by the vertex.
     * This field is generally not necessary and can be ignored.
     */
    void* extraData;

    /*
     * Constructs a new edge between the given start/end vertices with
     * the given cost.
     */
    Edge(Vertex* start = NULL, Vertex* finish = NULL, double cost = 0.0);

    /*
     * Frees up any memory dynamically allocated by this edge.
     */
    ~Edge();

    /*
     * Wipes the supplementary data of this vertex back to its initial state.
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
    Edge& operator =(const Edge& other);

    /*
     * Move assignment operator (rule of three).
     */
    Edge& operator =(Edge&& other);
};

/*
 * Makes an arc printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing an arc is not the same as printing an arc pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
std::ostream& operator<<(std::ostream& out, const Edge& edge);

/*
 * You can refer to an Edge as an Arc if you prefer.
 */
#define Arc Edge


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
class BasicGraph : public Graph<Vertex, Edge> {
public:
    /*
     * Newly added behavior in BasicGraph.
     */
    BasicGraph();
    void clearArcs();
    void clearEdges();
    bool containsArc(Vertex* v1, Vertex* v2) const;
    bool containsArc(const std::string& v1, const std::string& v2) const;
    bool containsArc(Edge* edge) const;
    bool containsEdge(Vertex* v1, Vertex* v2) const;
    bool containsEdge(const std::string& v1, const std::string& v2) const;
    bool containsEdge(Edge* edge) const;
    bool containsNode(const std::string& name) const;
    bool containsNode(Vertex* v) const;
    bool containsVertex(const std::string& name) const;
    bool containsVertex(Vertex* v) const;
    Edge* getArc(Vertex* v1, Vertex* v2) const;
    Edge* getArc(const std::string& v1, const std::string& v2) const;
    Edge* getEdge(Vertex* v1, Vertex* v2) const;
    Edge* getEdge(const std::string& v1, const std::string& v2) const;
    Edge* getInverseArc(Edge* edge) const;
    Edge* getInverseEdge(Edge* edge) const;
    bool isNeighbor(const std::string& v1, const std::string& v2) const;
    bool isNeighbor(Vertex* v1, Vertex* v2) const;
    void resetData();
    void setResetEnabled(bool enabled);
    virtual void scanArcData(TokenScanner& scanner, Edge* edge, Edge* inverse);
    virtual void writeArcData(std::ostream& out, Edge* edge) const;

    /*
     * The members below are mirrors of ones from Graph but with 'Node' changed
     * to 'Vertex' and/or 'Arc' changed to 'Edge', with identical behavior,
     * and so they are not documented in detail.  See Graph documentation.
     */
    Edge* addEdge(const std::string& v1, const std::string& v2, double cost = 0.0, bool directed = true);
    Edge* addEdge(Vertex* v1, Vertex* v2, double cost = 0.0, bool directed = true);
    Edge* addEdge(Edge* e, bool directed = true);
    Vertex* addVertex(const std::string& name);
    Vertex* addVertex(Vertex* v);
    const Set<Edge*>& getEdgeSet() const;
    const Set<Edge*>& getEdgeSet(Vertex* v) const;
    const Set<Edge*>& getEdgeSet(const std::string& v) const;
    Vertex* getVertex(const std::string& name) const;
    const Set<Vertex*>& getVertexSet() const;
    void removeEdge(const std::string& v1, const std::string& v2, bool directed = true);
    void removeEdge(Vertex* v1, Vertex* v2, bool directed = true);
    void removeEdge(Edge* e, bool directed = true);
    void removeVertex(const std::string& name);
    void removeVertex(Vertex* v);

private:
    bool m_resetEnabled;
};

#endif
