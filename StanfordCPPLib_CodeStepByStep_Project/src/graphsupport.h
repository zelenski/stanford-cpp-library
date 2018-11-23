#ifndef _graphsupport_h
#define _graphsupport_h

#include <iostream>
#include <sstream>
#include "basicgraph.h"
#include "vector.h"
using namespace std;

// colors for graph problems
typedef int Color;
extern const Color UNCOLORED, WHITE, GRAY, YELLOW, GREEN, RED, BLUE;
extern const int NUM_COLORS;
extern const Color COLORS[7];
extern const std::string COLOR_NAMES[7];

bool graph_canReach(BasicGraph& graph, Vertex* start, Vertex* end, Set<Vertex*>& visited, Vector<Vertex*>* path = nullptr);
bool graph_isConnected(BasicGraph& graph, bool checkWeak = false);
bool graph_isCyclic(BasicGraph& graph);
void graph_printEdgeList(BasicGraph& graph);
void graph_printAdjacencyList(BasicGraph& graph);
void graph_printAdjacencyMatrix(BasicGraph& graph);
void graph_printVertexDegrees(BasicGraph& graph);
string graph_pathToString(const Vector<Vertex*>& path);
void graph_printPath(const Vector<Vertex*>& path);
void BasicGraph_fromString(BasicGraph& obj, string str);
ostream& operator <<(ostream& out, const Vector<Vertex*>& path);
ostream& operator <<(ostream& out, const Set<Vertex*>& set);

#endif //_graphsupport_h
