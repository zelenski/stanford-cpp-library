/*
 * @version 2017/10/24
 * - replaced null in uppercase with nullptr
 */

#define INTERNAL_INCLUDE 1
#include "graphsupport.h"
#include <iomanip>
#include "gmath.h"
#undef INTERNAL_INCLUDE

// definitions for colors
const int NUM_COLORS = 7;

const Color UNCOLORED = 0;
const Color WHITE = 1;
const Color GRAY = 2;
const Color YELLOW = 3;
const Color GREEN = 4;
const Color RED = 5;
const Color BLUE = 6;

const Color COLORS[7] = {
    UNCOLORED,
    WHITE,
    GRAY,
    YELLOW,
    GREEN,
    RED,
    BLUE
};

const string COLOR_NAMES[7] = {
    "uncolored",
    "white",
    "gray",
    "yellow",
    "green",
    "red",
    "blue"
};

bool graph_canReach(BasicGraph& graph, Vertex* start, Vertex* end, Set<Vertex*>& visited, Vector<Vertex*>* path) {
    // cout << "    canReach(graph, start=" << start->name << ", end=" << end->name << ")" << endl;
    if (start == end) {
        return true;
    } else if (visited.contains(start)) {
        return false;
    } else {
        visited.add(start);
        if (path) {
            path->add(start);
        }
        for (Vertex* neighbor : graph.getNeighbors(start->name)) {
            if (graph_canReach(graph, neighbor, end, visited, path)) {
                return true;
            }
        }
        visited.remove(start);
        if (path) {
            path->remove(path->size() - 1);
        }
        return false;
    }
}

bool graph_isConnected(BasicGraph& graph, bool checkWeak) {
    bool weak = false;
    for (Vertex* v1 : graph.getVertexSet()) {
        for (Vertex* v2 : graph.getVertexSet()) {
            if (v1 == v2) {
                continue;
            }
            // cout << "    " << v1->name << " ... " << v2->name << endl;
            graph.resetData();
            Set<Vertex*> visited;
            if (!graph_canReach(graph, v1, v2, visited)) {
                if (checkWeak && graph_canReach(graph, v2, v1, visited)) {
                    weak = true;
                    continue;
                }
                cout << "vertex " << v1->name << " cannot reach " << v2->name << endl;
                return false;
            }
        }
    }
    
    if (weak) {
        cout << "(weakly connected)" << endl;
    }
    return !weak;
}

bool graph_isCyclic(BasicGraph& graph) {
    for (Vertex* v : graph.getVertexSet()) {
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            Vector<Vertex*> path;
            graph.resetData();
            Set<Vertex*> visited;
            if (graph_canReach(graph, neighbor, v, visited, &path)) {
                path.insert(0, v);
                path.add(v);
                cout << "cycle starting from " << v->name << ": " << graph_pathToString(path) << endl;
                return true;
            }
        }
    }
    
    return false;
}

void graph_printEdgeList(BasicGraph& graph) {
    cout << "edge list:" << endl;
    Vector<string> edgeList;
    for (Edge* edge : graph.getEdgeSet()) {
        string edgeStr = "" + edge->start->name + " -> " + edge->finish->name;
        if (!floatingPointEqual(edge->weight, 0.0)) {
            edgeStr += " : " + realToString(edge->weight);
        }
        cout << "  " << edgeStr << endl;
    }
}

void graph_printAdjacencyList(BasicGraph& graph) {
    cout << "adjacency list:" << endl;
    for (Vertex* v : graph.getVertexSet()) {
        cout << "  " << v->name << " : ";
        int count = 0;
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (count > 0) {
                cout << ", ";
            }
            cout << "(" << neighbor->name;
            Edge* edge = graph.getEdge(v, neighbor);
            if (!floatingPointEqual(edge->weight, 0.0)) {
                cout << ":" << edge->weight;
            }
            count++;
            cout << ")";
        }
        cout << endl;
    }
}

void graph_printAdjacencyMatrix(BasicGraph& graph) {
    cout << "adjacency matrix:" << endl;
    
    const int COL_WIDTH = 5;
    
    // column headers
    cout << setw(COL_WIDTH) << "";
    for (Vertex* v1 : graph.getVertexSet()) {
        cout << setw(COL_WIDTH) << v1->name;
    }
    cout << endl;
    
    for (Vertex* v1 : graph.getVertexSet()) {
        // row header
        cout << setw(COL_WIDTH) << v1->name;
        
        for (Vertex* v2 : graph.getVertexSet()) {
            double toPrint = 0;
            if (graph.isNeighbor(v1, v2)) {
                Edge* edge = graph.getEdge(v1, v2);
                if (!floatingPointEqual(edge->weight, 0.0)) {
                    toPrint = edge->weight;
                } else {
                    toPrint = 1;
                }
            }
            cout << setw(COL_WIDTH) << toPrint;
        }
        cout << endl;
    }
}

void graph_printVertexDegrees(BasicGraph& graph) {
    cout << "vertex degrees:" << endl;
    for (Vertex* v : graph.getVertexSet()) {
        cout << "  " << v->name << " : ";
        int inDegree = 0;
        for (Vertex* v2 : graph.getVertexSet()) {
            if (graph.isNeighbor(v2, v)) {
                inDegree++;
            }
        }
        cout << "in-degree: " << inDegree;
        cout << ", out-degree: " << v->edges.size() << endl;
    }
}

string graph_pathToString(const Vector<Vertex*>& path) {
    Vector<string> path2;
    for (Vertex* v : path) {
        if (v) {
            path2.add(v->name);
        } else {
            path2.add("nullptr");
        }
    }
    return path2.toString();
}

void graph_printPath(const Vector<Vertex*>& path) {
    cout << "{";
    int i = 0;
    for (Vertex* v : path) {
        if (i > 0) {
            cout << ", ";
        }
        if (!v) {
            cout << "nullptr";
        } else {
            cout << v->name;
        }
        i++;
    }
    cout << "}" << endl;
}

void BasicGraph_fromString(BasicGraph& obj, string str) {
    istringstream input(str);
    input >> obj;
}

ostream& operator <<(ostream& out, const Vector<Vertex*>& path) {
    out << "{";
    int i = 0;
    for (Vertex* v : path) {
        if (i > 0) {
            out << ", ";
        }
        if (!v) {
            out << "nullptr";
        } else {
            out << v->name;
        }
        i++;
    }
    out << "}";
    return out;
}

ostream& operator <<(ostream& out, const Set<Vertex*>& set) {
    out << "{";
    int i = 0;
    for (Vertex* v : set) {
        if (i > 0) {
            out << ", ";
        }
        if (!v) {
            out << "nullptr";
        } else {
            out << v->name;
        }
        i++;
    }
    out << "}";
    return out;
}
