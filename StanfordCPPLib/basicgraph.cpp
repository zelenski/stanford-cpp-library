/*
 * File: basicgraph.cpp
 * --------------------
 * This file contains the implementation of some useful graph types,
 * specifically the Vertex and Edge structures used in the typical graph.
 * together in lecture.  We also implement BasicGraph, an instantiation of
 * Stanford's Graph class using Vertex and Edge as its type parameters.
 *
 * See BasicGraph.h for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2014/10/31
 * - bug fix: extraData was being NULLed accidentally in Vertex::resetData()
 * @version 2014/10/23
 * - bug fixes based on cppcheck lint output
 * @version 2014/10/20
 * - converted functions to accept const string& rather than string for speed
 * @version 2014/08/16
 * - added Vertex 'weight' alias for 'cost'
 * - added Edge 'weight' alias for 'cost, and 'end' alias for 'finish'
 */

#include "basicgraph.h"
#include <sstream>

/*
 * Vertex member implementations
 */
Vertex::Vertex(const std::string& name) : name(name), edges(arcs), weight(cost), extraData(NULL) {
    resetData();
}

Vertex::Vertex(const Vertex &other) : name(other.name), arcs(other.arcs),
        edges(arcs), cost(other.cost), weight(cost), visited(other.visited),
        previous(other.previous), extraData(other.extraData), m_color(other.m_color) {
    // empty
}

Vertex::~Vertex() {
    if (extraData != NULL) {
        // delete extraData;
    }
}

int Vertex::getColor() const {
    return m_color;
}

void Vertex::resetData() {
    cost = 0.0;
    previous = NULL;
    visited = false;
    m_color = /* UNCOLORED */ 0;
    // extraData = NULL;
}

void Vertex::setColor(int c) {
    m_color = c;
    notifyObservers();
}

std::string Vertex::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

Vertex& Vertex::operator =(const Vertex& other) {
    name = other.name;
    arcs = other.arcs;
    cost = other.cost;
    visited = other.visited;
    previous = other.previous;
    extraData = other.extraData;
    m_color = other.m_color;
    return *this;
}

Vertex& Vertex::operator =(Vertex&& other) {
    name = other.name;
    arcs = other.arcs;
    cost = other.cost;
    visited = other.visited;
    previous = other.previous;
    extraData = other.extraData;
    m_color = other.m_color;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Vertex& v) {
    out << "Vertex{name=" << v.name;
    if (v.cost != 0.0) {
        out << ", cost=" << v.cost;
    }
    out << ", cost=" << v.cost;
    out << ", visited=" << (v.visited ? "true" : "false");
    out << ", previous=" << (v.previous == NULL ? std::string("NULL") : v.previous->name);

    out << ", neighbors={";
    int i = 0;
    for (Edge* edge : v.edges) {
        if (i > 0) {
            out << ", ";
        }
        i++;
        if (edge->finish) {
            out << edge->finish->name;
        } else {
            out << "NULL";
        }
    }
    out << "}";
    out << "}";
    return out;
}


/*
 * Edge member implementations
 */
Edge::Edge(Vertex* start, Vertex* finish, double cost)
        : start(start), finish(finish), end(finish), cost(cost), weight(cost) {
    this->extraData = NULL;
    this->resetData();
}

Edge::~Edge() {
    if (this->extraData != NULL) {
        // delete this->extraData;
    }
}

void Edge::resetData() {
    this->visited = false;
}

std::string Edge::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

Edge& Edge::operator =(const Edge& other) {
    start = other.start;
    finish = other.finish;
    cost = other.cost;
    visited = other.visited;
    return *this;
}

Edge& Edge::operator =(Edge&& other) {
    start = other.start;
    finish = other.finish;
    cost = other.cost;
    visited = other.visited;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
    out << "Edge{start=";
    if (edge.start == NULL) {
        out << "NULL";
    } else {
        out << edge.start->name;
    }
    out << ", finish=";
    if (edge.finish == NULL) {
        out << "NULL";
    } else {
        out << edge.finish->name;
    }
    if (edge.cost != 0.0) {
        out << ", cost=" << edge.cost;
    }
    if (edge.visited) {
        out << ", visited=" << (edge.visited ? "true" : "false");
    }
    out << "}";
    return out;
}


/*
 * BasicGraph member implementations
 */
BasicGraph::BasicGraph() : Graph<Vertex, Edge>() {
    m_resetEnabled = true;
}

void BasicGraph::clearArcs() {
    clearEdges();
}

void BasicGraph::clearEdges() {
    Set<Edge*> edges = getEdgeSet();   // makes a copy
    for (Edge* edge : edges) {
        removeEdge(edge);
    }
}

bool BasicGraph::containsArc(Vertex* v1, Vertex* v2) const {
    return this->getArc(v1, v2) != NULL;
}

bool BasicGraph::containsArc(const std::string& v1, const std::string& v2) const {
    return this->getArc(v1, v2) != NULL;
}

bool BasicGraph::containsArc(Edge* edge) const {
    if (edge == NULL) {
        return false;
    } else {
        return this->getEdgeSet().contains(edge);
    }
}

bool BasicGraph::containsEdge(Vertex* v1, Vertex* v2) const {
    return this->containsArc(v1, v2);
}

bool BasicGraph::containsEdge(const std::string& v1, const std::string& v2) const {
    return this->containsArc(v1, v2);
}

bool BasicGraph::containsEdge(Edge* edge) const {
    return this->containsArc(edge);
}

bool BasicGraph::containsNode(const std::string& name) const {
    return this->getNode(name) != NULL;
}

bool BasicGraph::containsNode(Vertex* v) const {
    if (v == NULL) {
        return false;
    } else {
        return this->getNodeSet().contains(v);
    }
}

bool BasicGraph::containsVertex(const std::string& name) const {
    return this->containsNode(name);
}

bool BasicGraph::containsVertex(Vertex* v) const {
    return this->containsNode(v);
}

Edge* BasicGraph::getArc(Vertex* v1, Vertex* v2) const {
    for (Edge* edge : this->getEdgeSet(v1)) {
        if (edge->finish == v2) {
            return edge;
        }
    }
    return NULL;
}

Edge* BasicGraph::getArc(const std::string& v1, const std::string& v2) const {
    return this->getArc(this->getVertex(v1), this->getVertex(v2));
}

Edge* BasicGraph::getEdge(Vertex* v1, Vertex* v2) const {
    return this->getArc(v1, v2);
}

Edge* BasicGraph::getEdge(const std::string& v1, const std::string& v2) const {
    return this->getArc(v1, v2);
}

Edge* BasicGraph::getInverseArc(Edge* edge) const {
    return this->getArc(edge->finish, edge->start);
}

Edge* BasicGraph::getInverseEdge(Edge* edge) const {
    return this->getInverseArc(edge);
}

bool BasicGraph::isNeighbor(const std::string& v1, const std::string& v2) const {
    return this->isNeighbor(this->getVertex(v1), this->getVertex(v2));
}

bool BasicGraph::isNeighbor(Vertex* v1, Vertex* v2) const {
    for (Edge* edge : this->getEdgeSet(v1)) {
        if (edge->finish == v2) {
            return true;
        }
    }
    return false;
}

void BasicGraph::resetData() {
    if (m_resetEnabled) {
        for (Vertex* v : getVertexSet()) {
            v->resetData();
        }
        for (Edge* e : getEdgeSet()) {
            e->resetData();
        }
    }
}

void BasicGraph::setResetEnabled(bool enabled) {
    m_resetEnabled = enabled;
}

// members below are just mirrors of ones from Graph

Edge* BasicGraph::addEdge(const std::string& v1, const std::string& v2, double cost, bool directed) {
    return this->addEdge(getVertex(v1), getVertex(v2), cost, directed);
}

Edge* BasicGraph::addEdge(Vertex* v1, Vertex* v2, double cost, bool directed) {
    Edge* e = new Edge(v1, v2, cost);
    return addEdge(e, directed);
}

Edge* BasicGraph::addEdge(Edge* e, bool directed) {
    Edge* result = this->addArc(e);
    if (!directed) {
        Edge* result2 = this->addArc(e->finish, e->start);
        result2->cost = e->cost;
    }
    return result;
}

Vertex* BasicGraph::addVertex(const std::string& name) {
    return this->addNode(name);
}

Vertex* BasicGraph::addVertex(Vertex* v) {
    return this->addNode(v);
}

const Set<Edge*>& BasicGraph::getEdgeSet() const {
    return this->getArcSet();
}

const Set<Edge*>& BasicGraph::getEdgeSet(Vertex* v) const {
    return this->getArcSet(v);
}

const Set<Edge*>& BasicGraph::getEdgeSet(const std::string& v) const {
    return this->getArcSet(v);
}

Vertex* BasicGraph::getVertex(const std::string& name) const {
    return this->getNode(name);
}

const Set<Vertex*>& BasicGraph::getVertexSet() const {
    return this->getNodeSet();
}

void BasicGraph::removeEdge(const std::string& v1, const std::string& v2, bool directed) {
    this->removeEdge(this->getVertex(v1), this->getVertex(v2), directed);
}

void BasicGraph::removeEdge(Vertex* v1, Vertex* v2, bool directed) {
    this->removeArc(v1, v2);
    if (!directed) {
        this->removeArc(v2, v1);
    }
}

void BasicGraph::removeEdge(Edge* e, bool directed) {
    this->removeArc(e);
    if (!directed) {
        this->removeArc(e->finish, e->start);
    }
}

void BasicGraph::removeVertex(const std::string& name) {
    this->removeNode(name);
}

void BasicGraph::removeVertex(Vertex* v) {
    this->removeNode(v);
}

void BasicGraph::scanArcData(TokenScanner& scanner, Edge* edge, Edge* inverse) {
    std::string colon = scanner.nextToken();   // ":", skip over
    if (colon == ":") {
        std::string costStr = scanner.nextToken();
        edge->cost = stringToReal(costStr);
        if (inverse != NULL) {
            inverse->cost = edge->cost;
        }
    } else {
        // no cost for this edge (cost 0); un-read the colon token because
        // it probably wasn't actually a colon
        scanner.saveToken(colon);
    }
}

void BasicGraph::writeArcData(std::ostream& out, Edge* edge) const {
    if (edge->cost != 0) {
        out << " : ";
        out << edge->cost;
    }
}
