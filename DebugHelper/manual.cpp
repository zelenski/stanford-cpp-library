#include <iostream>
#include "console.h"
#include "map.h"
#include "priorityqueue.h"
#include "set.h"
#include "vector.h"
using namespace std;


struct Node {
    char letter;
    Node *left, *right;
};

void manaul()
{
    Node *ptr = new Node {'A', nullptr, nullptr};
    PriorityQueue<Node *> pq {{4.0, ptr}};
    Vector<Node *> v = {ptr, ptr, nullptr, nullptr};
    Set<Node *> s = {ptr, nullptr};
    Map<char, Node *> m = {{'a', ptr}, {'b', ptr},
                           {'c', nullptr}};

    std::vector<bool> stdvb = {true, true, false, false, true, true, true, true};
    std::vector<Node *> stdv = {ptr, nullptr, ptr};
    std::map<char, Node *> stdm = {{'a', ptr}, {'b', ptr},
                                   {'c', nullptr}};

    cout << pq << endl;
    cout << v << endl;
    cout << m << endl;
}


#include "generated"