/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

// all collections:
// DawgLexicon, Grid, HashMap, HashSet, Lexicon, LinkedList,
// Map, PriorityQueue, Queue, Set, Stack, Vector


#include "basicgraph.h"
#include "deque.h"
#include "graph.h"
#include "grid.h"
#include "hashcode.h"
#include "hashmap.h"
#include "hashset.h"
#include "lexicon.h"
#include "linkedlist.h"
#include "map.h"
#include "priorityqueue.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;





PROVIDED_TEST("collection initializer lists") {
    auto list = {60, 70};
    auto list2 = {20, 50};
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};
    std::initializer_list<std::string> lexlist2 = {"twenty", "fifty"};
    std::initializer_list<std::pair<std::string, int> > pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<std::string, int> > pairlist2 = {{"b", 20}, {"e", 50}};

    Grid<int> grid {{1, 2, 3}, {4, 5, 6}};
    cout << "init list Grid = " << grid << endl;

    HashMap<string, int> hmap {{"a", 10}, {"b", 20}, {"c", 30}};
    cout << "init list HashMap = " << hmap << endl;
    hmap += {{"d", 40}, {"e", 50}};
    cout << "after +=, HashMap = " << hmap << endl;
    //cout << "HashMap + {} list = " << (hmap + pairlist) << endl;
    //cout << "HashMap - {} list = " << (hmap - pairlist2) << endl;
    //cout << "HashMap * {} list = " << (hmap * pairlist2) << endl;
    hmap -= {{"b", 20}, {"e", 50}, {"a", 999}};
    cout << "HashMap -={} list = " << hmap << endl;
    hmap *= {{"z", 0}, {"a", 10}, {"d", 40}, {"x", 99}};
    cout << "HashMap *={} list = " << hmap << endl;
    cout << "at end,   HashMap = " << hmap << endl;

    HashSet<int> hset {10, 20, 30};
    cout << "init list HashSet = " << hset << endl;
    hset += {40, 50};
    cout << "after +=, HashSet = " << hset << endl;
    cout << "HashSet + {} list = " << (hset + list) << endl;
    cout << "HashSet - {} list = " << (hset - list2) << endl;
    cout << "HashSet * {} list = " << (hset * list2) << endl;
    hset -= {20, 50};
    cout << "HashSet -={} list = " << hset << endl;
    hset *= {0, 10, 40, 99};
    cout << "HashSet *={} list = " << hset << endl;
    cout << "at end,   HashSet = " << hset << endl;

    std::initializer_list<std::string> lexallwords = {
        "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy"
    };
    Lexicon lex {"ten", "twenty", "thirty"};
    cout << "init list Lexicon = " << lex << ", size " << lex.size() << endl;
    for (std::string s : lexallwords) { cout << boolalpha << lex.contains(s) << " "; }
    cout << endl;
    lex += "forty", "fifty";
    cout << "after +=, Lexicon = " << lex << ", size " << lex.size() << endl;
    lex -= "forty", "fifty";
    cout << "after -=, Lexicon = " << lex << ", size " << lex.size() << endl;
    lex += {"forty", "fifty"};
    cout << "after +=, Lexicon = " << lex << ", size " << lex.size() << endl;
    for (std::string s : lexallwords) { cout << boolalpha << lex.contains(s) << " "; }
    cout << endl;
    cout << "Lexicon + {} list = " << (lex + lexlist) << endl;
    cout << "Lexicon - {} list = " << (lex - lexlist2) << endl;
    cout << "Lexicon * {} list = " << (lex * lexlist2) << endl;
    lex -= {"twenty", "fifty"};
    cout << "Lexicon -={} list = " << lex << ", size " << lex.size() << endl;
    lex *= {"zero", "ten", "forty", "ninetynine"};
    cout << "Lexicon *={} list = " << lex << ", size " << lex.size() << endl;
    cout << "at end,   Lexicon = " << lex << ", size " << lex.size() << endl;

    LinkedList<int> llist {10, 20, 30};
    cout << "init list LinkedList = " << llist << endl;
    llist += {40, 50};
    cout << "after +=, LinkedList = " << llist << endl;
    cout << "LinkedList + {} list = " << (llist + list) << endl;
    cout << "at end,   LinkedList = " << llist << endl;

    Map<string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    cout << "init list Map = " << map << endl;
    map += {{"d", 40}, {"e", 50}};
    cout << "after +=, Map = " << map << endl;
    //cout << "Map + {} list = " << (map + pairlist) << endl;
    //cout << "Map - {} list = " << (map - pairlist2) << endl;
    //cout << "Map * {} list = " << (map * pairlist2) << endl;
    map -= {{"b", 20}, {"e", 50}, {"a", 999}};
    cout << "Map -={} list = " << map << endl;
    map *= {{"z", 0}, {"a", 10}, {"d", 40}, {"x", 99}};
    cout << "Map *={} list = " << map << endl;
    cout << "at end,   Map = " << map << endl;

    PriorityQueue<string> pqueue {{40.0, "Marty"}, {20.0, "Eric"}, {30.0, "Mehran"}};
    cout << "init list PQueue = " << pqueue << endl;
    while (!pqueue.isEmpty()) {
        cout << "  " << pqueue.peekPriority() << " " << pqueue.peek() << endl;
        pqueue.dequeue();
    }

    Queue<int> queue {10, 20, 30};
    cout << "init list Queue = " << queue << endl;

    Deque<int> deque {10, 20, 30};
    cout << "init list Deque = " << deque << endl;

    Set<int> set {10, 20, 30};
    cout << "init list Set = " << set << endl;
    set += {40, 50};
    cout << "after +=, Set = " << set << endl;
    cout << "Set + {} list = " << (set + list) << endl;
    cout << "Set - {} list = " << (set - list2) << endl;
    cout << "Set * {} list = " << (set * list2) << endl;
    set -= {20, 50};
    cout << "Set -={} list = " << set << endl;
    set *= {0, 10, 40, 99};
    cout << "Set *={} list = " << set << endl;
    cout << "at end,   Set = " << set << endl;

    Stack<int> stack {10, 20, 30};
    cout << "init list Stack = " << stack << endl;

    Vector<int> v {10, 20, 30};
    cout << "init list Vector = " << v << endl;
    v += {40, 50};
    cout << "after +=, Vector = " << v << endl;
    cout << "Vector + {} list = " << (v + list) << endl;
    cout << "at end,   Vector = " << v << endl;
    v = {999, 888, 777};
    cout << "on =,     Vector = " << v << endl;
    v.clear();
    v.add(777);
    std::initializer_list<int> sevenlist = {777};
    if (v == sevenlist) {
        cout << "op ==, Vector equal" << endl;
    } else {
        cout << "op ==, Vector not equal" << endl;
    }
}
