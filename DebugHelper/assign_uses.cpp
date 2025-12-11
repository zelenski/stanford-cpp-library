#include "shared.h"
using namespace std;

void assign_uses() {
    Vector<Bit> huffBit_Vector = {0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1};
    Queue<Bit> huffBit_Queue =   {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    Map<char, Queue<Bit>> huffBit_Map_Q {{'a', {0, 1, 1, 1}}, {'b', {1, 0}}};
    Map<char, Vector<Bit>> huffBit_Map_V = {{'a', {1, 1, 1, 0}}, {'b', {0, 0}}};

    Node *ptr = new Node {'A', nullptr, nullptr};
    Node on_stack = {'Z', nullptr, nullptr};
    PriorityQueue<Node *> huffNodePtr_PQ {{4.0, ptr}};
    Vector<Node *> huffNodePtr_Vector = {ptr, &on_stack};
    Set<Node *> huffNodePtr_Set = {ptr};
    Map<char, Node *> huffNodePtr_Map = {{'a', ptr}, {'b', &on_stack}};

    Map<string, Set<string>> siteIndex_Map_Set =  {{"home", {"the", "end"}}, {"faq", {"what", "why"}}};
    Grid<char> boggle_Grid = {{'B', 'O', 'G'}, {'G', 'L', 'E'}};
    GridLocationRange redistrict_Range(0,0,3,6);
    GridLocation maze_Loc(2, 4);
    Stack<GridLocation> maze_Stack_Loc = { {1, 2}, {3, 4}, {5, 6}, {7, 8}};
    BREAKPOINT;
}
