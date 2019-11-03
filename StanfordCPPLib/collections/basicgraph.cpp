/*
 * File: basicgraph.cpp
 * --------------------
 * This file implements any non-template functionality used by
 * the BasicGraph class.
 *
 * @version 2016/12/01
 */

#include "basicgraph.h"

int hashCode(const BasicGraph& graph) {
    int code = hashSeed();
    for (Vertex* v : graph) {
        code = hashMultiplier() * code + hashCode(v->name);
    }
    for (Edge* e : graph.getEdgeSet()) {
        code = hashMultiplier() * code + hashCode(e->start->name);
        code = hashMultiplier() * code + hashCode(e->finish->name);
    }
    return (code & hashMask());
}
