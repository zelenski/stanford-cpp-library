/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

// all collections:
// BasicGraph, DawgLexicon, Graph, Grid, HashMap, HashSet, Lexicon, LinkedList,
// Map, PriorityQueue, Queue, Set, SparseGrid, Stack, Vector

#include "testcases.h"
#include "basicgraph.h"
#include "dawglexicon.h"
#include "deque.h"
#include "graph.h"
#include "grid.h"
#include "hashcode.h"
#include "hashmap.h"
#include "hashset.h"
#include "lexicon.h"
#include "linkedhashmap.h"
#include "linkedhashset.h"
#include "linkedlist.h"
#include "map.h"
#include "pqueue.h"
#include "queue.h"
#include "set.h"
#include "sparsegrid.h"
#include "stack.h"
#include "vector.h"
#include <iostream>
#include <string>
using namespace std;

// for testing collections with non- ==, < elements
class Student {
public:
    std::string name;
};

// for testing Graph class
struct DumbNode;
struct DumbEdge;
struct DumbNode {
    std::string name;
    Set<DumbEdge*> arcs;
    double cost;
};

struct DumbEdge {
    DumbNode* start;
    DumbNode* finish;
    double cost;
};

template <typename T>
static void compareTestHelper(const T& o1, const T& o2, const std::string& type = "") {
    std::cout << "compare " << type << " " << o1 << "  to  " << o2 << ":" << std::endl;
    std::cout << "<:  " << std::boolalpha << (o1 <  o2) << std::endl;
    std::cout << "<=: " << std::boolalpha << (o1 <= o2) << std::endl;
    std::cout << "==: " << std::boolalpha << (o1 == o2) << std::endl;
    std::cout << "!=: " << std::boolalpha << (o1 != o2) << std::endl;
    std::cout << ">:  " << std::boolalpha << (o1 >  o2) << std::endl;
    std::cout << ">=: " << std::boolalpha << (o1 >= o2) << std::endl;
    std::cout << std::endl;
}

void randomElementTest() {
    std::cout << "randomElement/Key test:" << std::endl;
    Map<std::string, int> counts;
    int RUNS = 200;
    
    std::cout << "Grid: ";
    Grid<std::string> grid;
    grid.resize(2, 3);
    grid[0][0] = "a";
    grid[0][1] = "b";
    grid[0][2] = "c";
    grid[1][0] = "d";
    grid[1][1] = "e";
    grid[1][2] = "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(grid);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "HashMap: ";
    HashMap<std::string, int> hmap;
    hmap["a"] = 50;
    hmap["b"] = 40;
    hmap["c"] = 30;
    hmap["d"] = 20;
    hmap["e"] = 10;
    hmap["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(hmap);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "HashSet: ";
    HashSet<std::string> hset;
    hset += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(hset);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "LinkedList: ";
    LinkedList<std::string> list;
    list += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(list);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "Map: ";
    Map<std::string, int> map;
    map["a"] = 50;
    map["b"] = 40;
    map["c"] = 30;
    map["d"] = 20;
    map["e"] = 10;
    map["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(map);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "Set: ";
    Set<std::string> set;
    set += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(set);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    
    std::cout << "SparseGrid: ";
    SparseGrid<std::string> sgrid;
    sgrid.resize(4, 3);
    sgrid[0][0] = "a";
    sgrid[0][1] = "b";
    sgrid[1][2] = "c";   // row 2 is blank
    sgrid[3][0] = "d";
    sgrid[1][1] = "e";
    sgrid[1][2] = "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(sgrid);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;

    std::cout << "Vector: ";
    Vector<std::string> v;
    v += "a", "b", "c", "d", "e", "f";
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomElement(v);
        std::cout << s << " ";
        counts[s]++;
    }
    std::cout << std::endl;
    std::cout << "counts:" << counts << std::endl << std::endl;
}

void compareTest() {
    std::cout << "BEGIN COMPARE TEST" << std::endl;
    
    // testing some toString stuff and collections of objects
    Map<string, int> m;
    m["a"] = 2;
    m["b"] = 4;
    cout << "map is: " << m << endl;
    
    Vector<string> v;
    v += "a", "b";
    cout << "v is: " << v << endl;
    
//    Vector<Domino> vd;
//    string s = "{(1:4), (2:6), (4:5), (1:5), (3:5)}";
//    istringstream input(s);
//    input >> vd;
//    cout << "vd = " << vd << endl;
    
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    BasicGraph bgraph2;
    bgraph2.addNode("a");
    bgraph2.addNode("b");
    bgraph2.addNode("c");
    bgraph2.addNode("d");
    bgraph2.addNode("e");
    bgraph2.addNode("f");
    bgraph2.addArc("a", "b");
    bgraph2.addArc("a", "d");
    bgraph2.addArc("b", "c");
    bgraph2.addArc("b", "d");
    bgraph2.addArc("c", "b");
    bgraph2.addArc("b", "e");
    compareTestHelper(bgraph, bgraph2, "BasicGraph");
    
    DawgLexicon dawg;
    dawg.add("a");
    dawg.add("b");
    dawg.add("c");
    DawgLexicon dawg2;
    dawg2.add("a");
    dawg2.add("ab");
    dawg2.add("bc");
    compareTestHelper(dawg, dawg2, "DawgLexicon");
    
    Deque<std::string> deq;
    deq.add("a");
    deq.add("b");
    deq.add("c");
    Deque<std::string> deq2;
    deq2.add("a");
    deq2.add("ab");
    deq2.add("bc");
    compareTestHelper(deq, deq2, "Deque");
    
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    Graph<DumbNode, DumbEdge> graph2;
    graph2.addNode("a");
    graph2.addNode("b");
    graph2.addNode("c");
    graph2.addNode("d");
    graph2.addNode("e");
    graph2.addNode("f");
    graph2.addArc("a", "b");
    graph2.addArc("a", "d");
    graph2.addArc("b", "c");
    graph2.addArc("b", "d");
    graph2.addArc("c", "b");
    graph2.addArc("b", "e");
    //graph2.addArc("c", "e");
    compareTestHelper(graph, graph2, "Graph");
    
    Grid<int> grid1;
    grid1.resize(2, 2);
    Grid<int> grid2;
    grid2.resize(2, 3);
    Grid<int> grid3;
    grid3.resize(3, 2);
    compareTestHelper(grid1, grid2, "Grid");
    compareTestHelper(grid1, grid3);
    compareTestHelper(grid2, grid3);
    compareTestHelper(grid1, grid1);
    
    // (dawg?)lex contains "", "foo?" etc
    
    Lexicon lex;
    lex.add("a");
    lex.add("ab");
    lex.add("bc");
    Lexicon lex2;
    lex2.add("a");
    lex2.add("b");
    lex2.add("c");
    compareTestHelper(lex, lex2, "Lexicon");
    
    LinkedList<int> ll1;
    ll1 += 1, 2, 4, 5;
    LinkedList<int> ll2;
    ll2 += 1, 3, 1, 4, 8;
    LinkedList<int> ll3;
    ll3 += 1, 1, 7;
    LinkedList<int> ll4;  // empty
    compareTestHelper(ll1, ll2, "LinkedList");
//    compareTestHelper(ll1, ll3);
//    compareTestHelper(ll1, ll4);
//    compareTestHelper(ll2, ll3);
//    compareTestHelper(ll2, ll4);
//    compareTestHelper(ll3, ll4);
    Set<LinkedList<int> > sll;
    sll += ll1;
    sll += ll2;
    sll += ll3;
    sll += ll4;
    std::cout << "sll: " << sll << std::endl;

    PriorityQueue<std::string> pq;
    pq.add("a", 4);
    pq.add("bb", 3);
    pq.add("c", 1);
    pq.add("ddd", 6);
    pq.add("e", 5);
    pq.add("ffff", 2);
    pq.add("ffff-2nd", 2);
    pq.add("a-2nd", 4);
    pq.add("e-2nd", 5);
    PriorityQueue<std::string> pq2;  // dupe
    pq2.add("a", 4);
    pq2.add("bb", 3);
    pq2.add("c", 1);
    pq2.add("ddd", 6);
    pq2.add("e", 5);
    pq2.add("ffff", 2);
    pq2.add("ffff-2nd", 2);
    pq2.add("a-2nd", 4);
    pq2.add("e-2nd", 5);
    PriorityQueue<std::string> pq3;
    pq3.add("a", 3);
    pq3.add("b", 1);
    pq3.add("x", 4);
    PriorityQueue<std::string> pq4;
    pq4.add("a", 333);
    pq4.add("b", 111);
    pq4.add("x", 444);
    // does not compile; PQ comparison operators removed
    // compareTestHelper(pq, pq2, "PriorityQueue");
//    compareTestHelper(pq, pq3);
//    compareTestHelper(pq2, pq3);
//    compareTestHelper(pq3, pq4);
//    Set<PriorityQueue<std::string> > spq;
//    spq.add(pq);
//    spq.add(pq2);
//    spq.add(pq3);
//    std::cout << "spq: " << spq << std::endl;
    
    Queue<int> q1;
    q1.add(1);
    q1.add(2);
    q1.add(1);
    q1.add(5);
    Queue<int> q2;
    q2.add(1);
    q2.add(2);
    q2.add(1);
    q2.add(4);
    q2.add(7);
    compareTestHelper(q1, q2);
    Set<Queue<int> > sq;
    sq.add(q1);
    sq.add(q2);
    std::cout << "sq: " << sq << std::endl;
    
    Set<int> set1;
    set1 += 7, 5, 1, 2, 8;
    Set<int> set2;
    set2 += 1, 2, 3, 4;
    Set<int> set3;
    compareTestHelper(set1, set2, "Set");
    compareTestHelper(set1, set3);
    compareTestHelper(set2, set3);
    Set<Set<int> > sset;
    sset += set1;
    sset += set2;
    sset += set3;
    std::cout << "sset: " << sset << std::endl;
    
    
    SparseGrid<int> sgrid1;
    sgrid1.resize(2, 2);
    SparseGrid<int> sgrid2;
    sgrid2.resize(2, 3);
    SparseGrid<int> sgrid3;
    sgrid3.resize(3, 2);
    compareTestHelper(sgrid1, sgrid2, "SparseGrid");
    compareTestHelper(sgrid1, sgrid3);
    compareTestHelper(sgrid2, sgrid3);
    compareTestHelper(sgrid1, sgrid1);

    Vector<int> v1;
    v1 += 1, 2, 4, 5;
    Vector<int> v2;
    v2 += 1, 3, 1, 4, 8;
    Vector<int> v3;
    v3 += 1, 1, 7;
    Vector<int> v4;
    v4 += 2, 0;
    Vector<int> v5;
    v5 += 1, 2, 4, 5, 6, 7;
    Vector<int> v6;  // empty
    compareTestHelper(v1, v2, "Vector");
//    compareTestHelper(v1, v3);
//    compareTestHelper(v1, v4);
//    compareTestHelper(v1, v5);
//    compareTestHelper(v1, v6);
//    compareTestHelper(v2, v3);
//    compareTestHelper(v2, v4);
//    compareTestHelper(v2, v5);
//    compareTestHelper(v2, v6);
//    compareTestHelper(v3, v4);
//    compareTestHelper(v3, v5);
//    compareTestHelper(v3, v6);
//    compareTestHelper(v4, v5);
//    compareTestHelper(v4, v6);
    compareTestHelper(v5, v6);
    Set<Vector<int> > sv;
    sv += v1;
    sv += v2;
    sv += v3;
    sv += v4;
    sv += v6;
    sv += v5;
    std::cout << "sv: " << sv << std::endl;
    
    // test on ==, < requirement on vectors
    Vector<Student> vstud;
    Student sam;
    Student joe;
    vstud.add(sam);
    Vector<Student> vstud2;
    vstud2.add(joe);
    vstud2.add(sam);
    std::cout << "vector of student: size " << vstud.size() << std::endl;
    
    // this line should fail to compile
//    std::cout << "vector of student equal: " << std::boolalpha << (vstud == vstud2) << std::endl;
    // this line should fail to compile
//    Set<Vector<Student> > sv2;
//    sv2.add(vstud);
//    sv2.add(vstud2);
    
    std::cout << "END COMPARE TEST" << std::endl << std::endl;
}

void foreachTest() {
    std::cout << "foreach test: " << std::endl;
    
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    std::cout << "basicgraph: " << bgraph << std::endl;
    for (Vertex* node : bgraph) {
        std::cout << node->name << std::endl;
    }
    
    DawgLexicon dlex;
    dlex.add("a");
    dlex.add("cc");
    dlex.add("bbb");
    std::cout << "dawglexicon: " << dlex << std::endl;
    for (std::string n : dlex) {
        std::cout << n << std::endl;
    }
    
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    std::cout << "graph: " << graph << std::endl;
    for (DumbNode* node : graph) {
        std::cout << node->name << std::endl;
    }
    
    Grid<int> grid(4, 2);
    grid.fill(42);
    std::cout << "grid: " << grid << std::endl;
    for (int n : grid) {
        std::cout << n << std::endl;
    }
    
    HashMap<std::string, int> hmap;
    hmap["a"] = 1;
    hmap["bbbb"] = 2;
    hmap["zz"] = 26;
    std::cout << "hashmap: " << hmap << std::endl;
    for (std::string k : hmap) {
        std::cout << k << " => " << hmap[k] << std::endl;
    }
    
    HashSet<int> hset;
    hset += 10, 20, 30, 40;
    std::cout << "hashset: " << hset << std::endl;
    for (int n : hset) {
        std::cout << n << std::endl;
    }

    Lexicon lex;
    lex.add("a");
    lex.add("cc");
    lex.add("bbb");
    std::cout << "lexicon: " << lex << std::endl;
    for (std::string n : lex) {
        std::cout << n << std::endl;
    }

    std::cout << std::endl;
    LinkedHashMap<std::string, int> lhmap;
//    lhmap["zz"] = 26;
//    lhmap["a"] = 1;
//    lhmap["ss"] = 97;
//    lhmap["bbbb"] = 2;
//    lhmap["E"] = 4;
//    lhmap["yZ44"] = 33;
    lhmap.put("zz", 26);
    lhmap.put("a", 1);
    lhmap.put("ss", 97);
    lhmap.put("bbbb", 2);
    lhmap.put("E", 4);
    lhmap.put("yZ44", 33);
    std::cout << "linkedhashmap: " << lhmap << std::endl;
    for (std::string k : lhmap) {
        std::cout << k << " => " << lhmap[k] << std::endl;
    }
    lhmap.remove("ss");
    lhmap.remove("zz");
    lhmap.remove("yZ44");
    lhmap.remove("notfound");
    std::cout << "after removes:" << std::endl;
    std::cout << "linkedhashmap: " << lhmap << std::endl;
    for (std::string k : lhmap) {
        std::cout << k << " => " << lhmap[k] << std::endl;
    }
    std::cout << std::endl;
    
    LinkedHashSet<int> lhset;
    lhset += 30, 10, 40, 66, -1, 42, 99;
    std::cout << "linkedhashset: " << lhset << std::endl;
    for (int n : lhset) {
        std::cout << n << std::endl;
    }
    lhset -= 40, 30, 99;
    std::cout << "linkedhashset: " << lhset << std::endl;
    for (int n : lhset) {
        std::cout << n << std::endl;
    }
    std::cout << std::endl;

    LinkedList<int> ll;
    ll += 10, 20, 30, 40;
    std::cout << "linkedlist: " << ll << std::endl;
    for (int n : ll) {
        std::cout << n << std::endl;
    }
    
    Map<std::string, int> map;
    map["a"] = 1;
    map["bbbb"] = 2;
    map["zz"] = 26;
    std::cout << "map: " << map << std::endl;
    for (std::string k : map) {
        std::cout << k << " => " << map[k] << std::endl;
    }
    
    PriorityQueue<std::string> pq;
    pq.add("a", 4);
    pq.add("bb", 3);
    pq.add("c", 1);
    pq.add("ddd", 6);
    pq.add("e", 5);
    pq.add("ffff", 2);
    
    // dupe priorities
    pq.add("ffff-2nd", 2);
    pq.add("a-2nd", 4);
    pq.add("e-2nd", 5);
    std::cout << "pq: " << pq << std::endl;

    // this should not compile since PQ has no public iterator
//    std::cout << "foreach loop: " << std::endl;
//    for (std::string str : pq) {
//        std::cout << "next element: " << str << std::endl;
//    }
    
    Queue<std::string> q;
    q.add("a");
    q.add("bb");
    q.add("c");
    q.add("ddd");
    q.add("e");
    q.add("ffff");
    std::cout << "q: " << q << std::endl;
    
    // this should not compile since PQ has no public iterator
//    std::cout << "foreach loop: " << std::endl;
//    for (std::string str : q) {
//        std::cout << "next element: " << str << std::endl;
//    }
    
    Set<int> set;
    set += 10, 20, 30, 40;
    std::cout << "set: " << set << std::endl;
    for (int n : set) {
        std::cout << n << std::endl;
    }
    
    SparseGrid<int> sgrid(4, 2);
    sgrid.fill(42);
    std::cout << "sparsegrid: " << sgrid << std::endl;
    for (int n : sgrid) {
        std::cout << n << std::endl;
    }
    
    Vector<int> v;
    v += 10, 20, 30, 40;
    std::cout << "vector: " << v << std::endl;
    for (int n : v) {
        std::cout << n << std::endl;
    }
    
    std::cout << "done!" << std::endl;
}

void hashCodeTest() {
    std::cout << "================= BEGIN HASHCODE TESTS =================" << std::endl;
    
    // BasicGraph
    BasicGraph bgraph;
    bgraph.addNode("a");
    bgraph.addNode("b");
    bgraph.addNode("c");
    bgraph.addNode("d");
    bgraph.addNode("e");
    bgraph.addArc("a", "b");
    bgraph.addArc("a", "d");
    bgraph.addArc("b", "c");
    bgraph.addArc("b", "d");
    bgraph.addArc("c", "b");
    bgraph.addArc("c", "e");
    BasicGraph bgraph2;
    bgraph2.addNode("a");
    bgraph2.addNode("b");
    bgraph2.addNode("c");
    bgraph2.addNode("d");
    bgraph2.addNode("e");
    bgraph2.addNode("f");
    bgraph2.addArc("a", "b");
    bgraph2.addArc("a", "d");
    bgraph2.addArc("b", "c");
    bgraph2.addArc("b", "d");
    bgraph2.addArc("c", "b");
    bgraph2.addArc("b", "e");
    HashSet<BasicGraph> hashbgraph;
    hashbgraph.add(bgraph);
    hashbgraph.add(bgraph2);
    std::cout << "hashset of basicgraph: " << hashbgraph << std::endl;
    
    // DawgLexicon
    HashSet<DawgLexicon> hashdawg;
    DawgLexicon dlex;
    dlex.add("a");
    dlex.add("abc");
    hashdawg.add(dlex);
    std::cout << "hashset of dawglexicon: " << hashdawg << std::endl;
    
    // Deque
    HashSet<Deque<int> > hashdeq;
    Deque<int> deq;
    deq.addBack(1);
    deq.addFront(2);
    deq.addBack(3);
    deq.addFront(4);
    hashdeq.add(deq);
    std::cout << "hashset of deque: " << hashdeq << std::endl;

    // Graph
    Graph<DumbNode, DumbEdge> graph;
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addArc("a", "b");
    graph.addArc("a", "d");
    graph.addArc("b", "c");
    graph.addArc("b", "d");
    graph.addArc("c", "b");
    graph.addArc("c", "e");
    Graph<DumbNode, DumbEdge> graph2;
    graph2.addNode("a");
    graph2.addNode("b");
    graph2.addNode("c");
    graph2.addNode("d");
    graph2.addNode("e");
    graph2.addNode("f");
    graph2.addArc("a", "b");
    graph2.addArc("a", "d");
    graph2.addArc("b", "c");
    graph2.addArc("b", "d");
    graph2.addArc("c", "b");
    graph2.addArc("b", "e");
    //graph2.addArc("c", "e");
    HashSet<Graph<DumbNode, DumbEdge> > hashgraph;
    hashgraph.add(graph);
    hashgraph.add(graph2);
    std::cout << "hashset of graph: " << hashgraph << std::endl;
    
    // Grid
    HashSet<Grid<int> > hashgrid;
    Grid<int> grid(2, 3);
    grid.fill(42);
    hashgrid.add(grid);
    std::cout << "hashset of grid: " << hashgrid << std::endl;
    
    // HashMap
    HashSet<HashMap<int, int> > hashhashmap;
    HashMap<int, int> hmap;
    hmap.add(69, 96);
    hmap.add(42, 24);
    hashhashmap.add(hmap);
    std::cout << "hashset of hashmap: " << hashhashmap << std::endl;
    
    // HashSet
    HashSet<HashSet<int> > hashhashset;
    HashSet<int> hset;
    hset.add(69);
    hset.add(42);
    hashhashset.add(hset);
    std::cout << "hashset of hashset: " << hashhashset << std::endl;
    
    // Lexicon
    HashSet<Lexicon> hashlex;
    Lexicon lex;
    lex.add("a");
    lex.add("abc");
    hashlex.add(lex);
    std::cout << "hashset of lexicon: " << hashlex << std::endl;
    
    // LinkedList
    HashSet<LinkedList<int> > hashlink;
    LinkedList<int> llist;
    llist.add(69);
    llist.add(42);
    hashlink.add(llist);
    std::cout << "hashset of linkedlist: " << hashlink << std::endl;
    
    // Map
    HashSet<Map<int, int> > hashmap;
    Map<int, int> map;
    map.add(69, 96);
    map.add(42, 24);
    hashmap.add(map);
    std::cout << "hashset of map: " << hashmap << std::endl;
    
    // PriorityQueue
    HashSet<PriorityQueue<std::string> > hashpq;
    PriorityQueue<std::string> pq;
    pq.add("a", 3);
    pq.add("b", 2);
    hashpq.add(pq);
    std::cout << "hashset of priorityqueue: " << hashpq << std::endl;
    
    // Queue
    HashSet<Queue<int> > hashq;
    Queue<int> q;
    q.add(69);
    q.add(42);
    hashq.add(q);
    std::cout << "hashset of queue: " << hashq << std::endl;
    
//    for (int n : q) {
//        std::cout << "queue element: " << n << std::endl;
//    }
    
    // Set
    HashSet<Set<int> > hashset;
    Set<int> set;
    set.add(69);
    set.add(42);
    hashset.add(set);
    std::cout << "hashset of set: " << hashset << std::endl;
    
    // SparseGrid
    HashSet<SparseGrid<int> > hashsparsegrid;
    SparseGrid<int> sparsegrid(2, 2);
    sparsegrid.fill(8);
    hashsparsegrid.add(sparsegrid);
    std::cout << "hashset of sparsegrid: " << hashsparsegrid << std::endl;
    
    // Stack
    HashSet<Stack<int> > hashstack;
    Stack<int> s;
    s.add(69);
    s.add(42);
    
//    for (int val : s) {
//        std::cout << "stack value: " << val << std::endl;
//    }
    
    hashstack.add(s);
    std::cout << "hashset of stack: " << hashstack << std::endl;
    
    // Vector
    HashSet<Vector<int> > hashvec;
    Vector<int> v;
    v.add(69);
    v.add(42);
    hashvec.add(v);
    std::cout << "hashset of vector: " << hashvec << std::endl;
    
    Stack<Student> vstud;
    Student sam;
    vstud.add(sam);
    Vector <Student> vstud2;
    vstud2.add(sam);
    std::cout << "stack of student: size " << vstud.size() << std::endl;
    
    HashSet<HashSet<int>> hhset;
    HashSet<int> hset1;
    hset1.add(42);
    hset1.add(17);
    hset1.add(318);
    hhset.add(hset1);
    HashSet<int> hset2;
    hset2.add(99);
    hset2.add(19);
    hset2.add(59);
    hset2.add(999);
    hhset.add(hset2);
    std::cout << "hashset of hashset of int: " << hhset << std::endl;
    
    Vertex* v1 = new Vertex("v1");
    Vertex* v2 = new Vertex("v2");
    Vertex* v3 = new Vertex("v3");
    Vertex* v4 = new Vertex("v4");
    Vertex* v5 = new Vertex("v5");
    Vertex* v6 = new Vertex("v6");
    
    HashSet<HashSet<Vertex*>> hhset2;
    HashSet<Vertex*> hset3;
    hset3.add(v1);
    hset3.add(v2);
    hset3.add(v3);
    hhset2.add(hset3);
    HashSet<Vertex*> hset4;
    hset4.add(v4);
    hset4.add(v5);
    hset4.add(v6);
    hhset2.add(hset4);
    std::cout << "hashset of hashset of Vertex*: " << hhset2 << std::endl;
    
    std::cout << std::endl;
    HashMap< HashSet<Vector<std::string> >, Vector<std::string> > ngram;
    HashSet<Vector<std::string> > key;
    Vector<std::string> value;
    ngram.put(key, value);
    std::cout << "ngram is " << ngram << std::endl;
    
    
    // hash code of hash collections after they are deep-copied
    HashMap<int, int> hmapcode;
    for (int i = 0; i < 99; i++) {
        int rand = randomInteger(-9999, 9999);
        hmapcode[rand] = i*i;
    }
    std::cout << "HashMap deep-copy hashCode tests:" << std::endl;
    std::cout << "hashcode of copy 1 is " << hashCode(hmapcode) << std::endl;
    HashMap<int, int> hmapcode2 = hmapcode;
    std::cout << "hashcode of copy 2 is " << hashCode(hmapcode2) << std::endl;
    std::cout << "map 1 equals map 2? " << std::boolalpha << (hmapcode == hmapcode2) << std::endl;
    std::cout << "sizes: " << hmapcode.size() << ", " << hmapcode2.size() << std::endl;

    HashSet<int> hsetcode;
    for (int i = 0; i < 99; i++) {
        int rand = randomInteger(-9999, 9999);
        hsetcode.add(rand);
    }
    std::cout << std::endl << "HashSet deep-copy hashCode tests:" << std::endl;
    std::cout << "hashcode of copy 1 is " << hashCode(hsetcode) << std::endl;
    HashSet<int> hsetcode2 = hsetcode;
    std::cout << "hashcode of copy 2 is " << hashCode(hsetcode2) << std::endl;
    std::cout << "set 1 equals set 2? " << std::boolalpha << (hsetcode == hsetcode2) << std::endl;
    std::cout << "sizes: " << hsetcode.size() << ", " << hsetcode2.size() << std::endl;

    std::cout << "================== END HASHCODE TESTS ==================" << std::endl;
}
