/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include "base64.h"
#include "basicgraph.h"
#include "bitstream.h"
#include "console.h"
#include "dawglexicon.h"
//#include "deque.h"
#include "direction.h"
#include "error.h"
#include "filelib.h"
#include "foreach.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "gfilechooser.h"
#include "ginteractors.h"
#include "gmath.h"
#include "gobjects.h"
#include "goptionpane.h"
#include "graph.h"
#include "grid.h"
#include "gtimer.h"
#include "gtypes.h"
#include "gwindow.h"
//#include "hashcode.h"
#include "hashmap.h"
#include "hashset.h"
#include "lexicon.h"
#include "linkedlist.h"
#include "map.h"
#include "observable.h"
#include "platform.h"
#include "point.h"
#include "pqueue.h"
#include "queue.h"
#include "random.h"
#include "regexpr.h"
#include "set.h"
#include "shuffle.h"
#include "simpio.h"
#include "sound.h"
#include "sparsegrid.h"
#include "stack.h"
#include "strlib.h"
#include "timer.h"
#include "tokenscanner.h"
#include "urlstream.h"
#include "vector.h"

#include <iomanip>
#include <iostream>
#include <string>

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


// function prototype declarations
//void compareTest();
//void fileDialogTest();
//void foreachTest();
//void gbufferedImageTest();
//void goptionpaneTest();
//void hashCodeTest();
//void urlTest();

int main() {
    setConsoleSize(1000, 600);
    setConsoleLocation(GWindow::CENTER_MAGIC_VALUE, GWindow::CENTER_MAGIC_VALUE);
//    foreachTest();
//    fileDialogTest();
//    goptionpaneTest();
//    compareTest();
//    hashCodeTest();
//    urlTest();
//    foreachTest();
//    gbufferedImageTest();
    std::cout << "Complete." << std::endl;
    return 0;
}

//void compareTest() {
//    std::cout << "BEGIN COMPARE TEST" << std::endl;
//    Set<Vector<int> > sv;   // sad face
//    Vector<int> v1;
//    v1 += 1, 2, 4, 5;
    
//    Vector<int> v2;
//    v2 += 1, 3, 1, 4, 8;
    
//    Vector<int> v3;
//    v3 += 1, 1, 7;
    
//    Vector<int> v4;
//    v4 += 2, 0;
    
//    Vector<int> v5;
//    v5 += 1, 2, 4, 5, 6, 7;
    
//    Vector<int> v6;  // empty
    
//    sv += v1;
//    sv += v2;
//    sv += v3;
//    sv += v4;
//    sv += v6;
//    sv += v5;
    
//    std::cout << "sv: " << sv << std::endl;
    
//    // test on ==, < requirement on vectors
//    Vector<Student> vstud;
//    Student sam;
//    Student joe;
//    vstud.add(sam);
//    Vector<Student> vstud2;
//    vstud2.add(joe);
//    vstud2.add(sam);
//    std::cout << "vector of student: size " << vstud.size() << std::endl;
    
//    // this line should fail to compile
////    std::cout << "vector of student equal: " << std::boolalpha << (vstud == vstud2) << std::endl;
    
//    // this line should fail to compile
////    Set<Vector<Student> > sv2;   // sad face
////    sv2.add(vstud);
////    sv2.add(vstud2);
    
    
//    Set<LinkedList<int> > sll;   // sad face
//    LinkedList<int> ll1;
//    ll1 += 1, 2, 4, 5;
    
//    LinkedList<int> ll2;
//    ll2 += 1, 3, 1, 4, 8;
    
//    LinkedList<int> ll3;
//    ll3 += 1, 1, 7;
    
//    LinkedList<int> ll4;  // empty
    
//    sll += ll1;
//    sll += ll2;
//    sll += ll3;
//    sll += ll4;
    
//    std::cout << "sll: " << sll << std::endl;
    
    
//    Set<Set<int> > sset;
//    Set<int> set1;
//    set1 += 7, 5, 1, 2, 8;
    
//    Set<int> set2;
//    set2 += 1, 2, 3, 4;
    
//    Set<int> set3;
//    sset += set1;
//    sset += set2;
//    sset += set3;
//    std::cout << "sset: " << sset << std::endl;
    
    
//    Set<PriorityQueue<std::string> > spq;
//    PriorityQueue<std::string> pq;
//    pq.add("a", 4);
//    pq.add("bb", 3);
//    pq.add("c", 1);
//    pq.add("ddd", 6);
//    pq.add("e", 5);
//    pq.add("ffff", 2);
//    pq.add("ffff-2nd", 2);
//    pq.add("a-2nd", 4);
//    pq.add("e-2nd", 5);
//    PriorityQueue<std::string> pq2;  // dupe
//    pq2.add("a", 4);
//    pq2.add("bb", 3);
//    pq2.add("c", 1);
//    pq2.add("ddd", 6);
//    pq2.add("e", 5);
//    pq2.add("ffff", 2);
//    pq2.add("ffff-2nd", 2);
//    pq2.add("a-2nd", 4);
//    pq2.add("e-2nd", 5);
//    PriorityQueue<std::string> pq3;
//    pq3.add("a", 3);
//    pq3.add("b", 1);
//    pq3.add("x", 4);
    
//    spq.add(pq);
//    spq.add(pq2);
//    spq.add(pq3);
//    std::cout << "spq: " << spq << std::endl;
    

//    Graph<DumbNode, DumbEdge> graph;
//    graph.addNode("a");
//    graph.addNode("b");
//    graph.addNode("c");
//    graph.addNode("d");
//    graph.addNode("e");
//    graph.addArc("a", "b");
//    graph.addArc("a", "d");
//    graph.addArc("b", "c");
//    graph.addArc("b", "d");
//    graph.addArc("c", "b");
//    graph.addArc("c", "e");
//    std::cout << "graph1: " << graph << std::endl;
    
//    Graph<DumbNode, DumbEdge> graph2;
//    graph2.addNode("a");
//    graph2.addNode("b");
//    graph2.addNode("c");
//    graph2.addNode("d");
//    graph2.addNode("e");
//    graph2.addNode("f");
//    graph2.addArc("a", "b");
//    graph2.addArc("a", "d");
//    graph2.addArc("b", "c");
//    graph2.addArc("b", "d");
//    graph2.addArc("c", "b");
//    graph2.addArc("b", "e");
//    //graph2.addArc("c", "e");
//    std::cout << "graph2: " << graph2 << std::endl;
//    std::cout << "==: " << std::boolalpha << (graph == graph2) << std::endl;
//    std::cout << "!=: " << std::boolalpha << (graph != graph2) << std::endl;
//    std::cout << "<:  " << std::boolalpha << (graph < graph2) << std::endl;
//    std::cout << "<=: " << std::boolalpha << (graph <= graph2) << std::endl;
//    std::cout << ">:  " << std::boolalpha << (graph > graph2) << std::endl;
//    std::cout << ">=: " << std::boolalpha << (graph >= graph2) << std::endl;

//    BasicGraph bgraph;
//    bgraph.addNode("a");
//    bgraph.addNode("b");
//    bgraph.addNode("c");
//    bgraph.addNode("d");
//    bgraph.addNode("e");
//    bgraph.addArc("a", "b");
//    bgraph.addArc("a", "d");
//    bgraph.addArc("b", "c");
//    bgraph.addArc("b", "d");
//    bgraph.addArc("c", "b");
//    bgraph.addArc("c", "e");
//    std::cout << "bgraph1: " << bgraph << std::endl;
    
//    BasicGraph bgraph2;
//    bgraph2.addNode("a");
//    bgraph2.addNode("b");
//    bgraph2.addNode("c");
//    bgraph2.addNode("d");
//    bgraph2.addNode("e");
//    bgraph2.addNode("f");
//    bgraph2.addArc("a", "b");
//    bgraph2.addArc("a", "d");
//    bgraph2.addArc("b", "c");
//    bgraph2.addArc("b", "d");
//    bgraph2.addArc("c", "b");
//    bgraph2.addArc("b", "e");
//    //bgraph2.addArc("c", "e");
//    std::cout << "bgraph2: " << bgraph2 << std::endl;
//    std::cout << "==: " << std::boolalpha << (bgraph == bgraph2) << std::endl;
//    std::cout << "!=: " << std::boolalpha << (bgraph != bgraph2) << std::endl;
//    std::cout << "<:  " << std::boolalpha << (bgraph < bgraph2) << std::endl;
//    std::cout << "<=: " << std::boolalpha << (bgraph <= bgraph2) << std::endl;
//    std::cout << ">:  " << std::boolalpha << (bgraph > bgraph2) << std::endl;
//    std::cout << ">=: " << std::boolalpha << (bgraph >= bgraph2) << std::endl;
    
//    std::cout << "END COMPARE TEST" << std::endl << std::endl;
//}

//void fileDialogTest() {
//    // std::cout << "current dir should be \"" << getCurrentDirectory() << "\"" << std::endl;
//    std::ifstream input;
//    std::string filename = openFileDialog(input, "booyahtitle", "/usr/lib/eclipse");
//    std::cout << "you chose \"" << filename << "\"" << std::endl;
//}

//void foreachTest() {
//    std::cout << "foreach test: " << std::endl;
    
//    BasicGraph bgraph;
//    bgraph.addNode("a");
//    bgraph.addNode("b");
//    bgraph.addNode("c");
//    bgraph.addNode("d");
//    bgraph.addNode("e");
//    bgraph.addArc("a", "b");
//    bgraph.addArc("a", "d");
//    bgraph.addArc("b", "c");
//    bgraph.addArc("b", "d");
//    bgraph.addArc("c", "b");
//    bgraph.addArc("c", "e");
//    std::cout << "basicgraph: " << bgraph << std::endl;
//    for (Vertex* node : bgraph) {
//        std::cout << node->name << std::endl;
//    }
    
//    DawgLexicon dlex;
//    dlex.add("a");
//    dlex.add("cc");
//    dlex.add("bbb");
//    std::cout << "dawglexicon: " << dlex << std::endl;
//    for (std::string n : dlex) {
//        std::cout << n << std::endl;
//    }
    
//    Graph<DumbNode, DumbEdge> graph;
//    graph.addNode("a");
//    graph.addNode("b");
//    graph.addNode("c");
//    graph.addNode("d");
//    graph.addNode("e");
//    graph.addArc("a", "b");
//    graph.addArc("a", "d");
//    graph.addArc("b", "c");
//    graph.addArc("b", "d");
//    graph.addArc("c", "b");
//    graph.addArc("c", "e");
//    std::cout << "graph: " << graph << std::endl;
//    for (DumbNode* node : graph) {
//        std::cout << node->name << std::endl;
//    }
    
//    Grid<int> grid(4, 2);
//    grid.fill(42);
//    std::cout << "grid: " << grid << std::endl;
//    for (int n : grid) {
//        std::cout << n << std::endl;
//    }
    
//    HashMap<std::string, int> hmap;
//    hmap["a"] = 1;
//    hmap["bbbb"] = 2;
//    hmap["zz"] = 26;
//    std::cout << "hashmap: " << hmap << std::endl;
//    for (std::string k : hmap) {
//        std::cout << k << " => " << hmap[k] << std::endl;
//    }
    
//    HashSet<int> hset;
//    hset += 10, 20, 30, 40;
//    std::cout << "hashset: " << hset << std::endl;
//    for (int n : hset) {
//        std::cout << n << std::endl;
//    }

//    Lexicon lex;
//    lex.add("a");
//    lex.add("cc");
//    lex.add("bbb");
//    std::cout << "lexicon: " << lex << std::endl;
//    for (std::string n : lex) {
//        std::cout << n << std::endl;
//    }

//    LinkedList<int> ll;
//    ll += 10, 20, 30, 40;
//    std::cout << "linkedlist: " << ll << std::endl;
//    for (int n : ll) {
//        std::cout << n << std::endl;
//    }
    
//    Map<std::string, int> map;
//    map["a"] = 1;
//    map["bbbb"] = 2;
//    map["zz"] = 26;
//    std::cout << "map: " << map << std::endl;
//    for (std::string k : map) {
//        std::cout << k << " => " << map[k] << std::endl;
//    }
    
//    PriorityQueue<std::string> pq;
//    std::cout << "pq:" << pq << std::endl;
//    pq.add("a", 4);
//    pq.add("bb", 3);
//    pq.add("c", 1);
//    pq.add("ddd", 6);
//    pq.add("e", 5);
//    pq.add("ffff", 2);
    
//    // dupe priorities
//    pq.add("ffff-2nd", 2);
//    pq.add("a-2nd", 4);
//    pq.add("e-2nd", 5);
    
////    std::cout << "foreach loop: " << std::endl;
////    for (std::string str : pq) {
////        std::cout << "next element: " << str << std::endl;
////    }
    
//    Set<int> set;
//    set += 10, 20, 30, 40;
//    std::cout << "set: " << set << std::endl;
//    for (int n : set) {
//        std::cout << n << std::endl;
//    }
    
//    SparseGrid<int> sgrid(4, 2);
//    sgrid.fill(42);
//    std::cout << "sparsegrid: " << sgrid << std::endl;
//    for (int n : sgrid) {
//        std::cout << n << std::endl;
//    }
    
//    Vector<int> v;
//    v += 10, 20, 30, 40;
//    std::cout << "vector: " << v << std::endl;
//    for (int n : v) {
//        std::cout << n << std::endl;
//    }
    
//    std::cout << "done!" << std::endl;
//}

//void goptionpaneTest() {
//    GOptionPane::showMessageDialog("Hello, world");
//    GOptionPane::showMessageDialog("Here's one with title and type", "booyah", GOptionPane::ERROR);
//    GOptionPane::showMessageDialog("special chars [*+*&}{] && || !)@(*&#)(&^%!{ \" \" \" \"}) C:\\Program Files\\Test ;,;:\", ';\"\\//\\// ( ) test 1 2 3 $a $b %a %b %1 %2 http://foo.com/! end");
    
//    bool result = GOptionPane::showConfirmDialog("Are you sure?");
//    std::cout << "you chose: " << std::boolalpha << result << std::endl;
//    result = GOptionPane::showConfirmDialog("Here's one with title and type", "booyah", GOptionPane::OK_CANCEL);
//    std::cout << "you chose: " << std::boolalpha << result << std::endl;
//    result = GOptionPane::showConfirmDialog("Here's a second one with title and type", "booyah", GOptionPane::YES_NO_CANCEL);
//    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    
//    std::string input = GOptionPane::showInputDialog("Type something:");
//    std::cout << "you typed: \"" << input << "\"" << std::endl;
//    input = GOptionPane::showInputDialog("Here's one with a title", "booyah");
//    std::cout << "you typed: \"" << input << "\"" << std::endl;
    
//    Vector<std::string> choices;
//    choices += "a", "bb", "ccc", "dd";
//    std::string option = GOptionPane::showOptionDialog("choose!", choices, "booyah", "ccc");
//    std::cout << "you chose: \"" << option << "\"" << std::endl;
//}

//void border(GBufferedImage* img) {
//    int color = 0x0;
//    int w = img->getWidth();
//    int h = img->getHeight();
//    // top/bottom
//    for (int x = 0; x < w; x++) {
//        img->setRGB(x, 0, color);
//        img->setRGB(x, h-1, color);
//    }
//    // left/right
//    for (int y = 0; y < h; y++) {
//        img->setRGB(0, y, color);
//        img->setRGB(w-1, y, color);
//    }
//}

//void gbufferedImageTest() {
//    GWindow gw;
//    gw.setTitle("Test");

//    gw.add(new GLabel("test!"), 10, 10);

//    std::cout << "About to construct GBufferedImage." << std::endl;
//    GBufferedImage* img = new GBufferedImage(10, 50, 200, 250);
//    std::cout << "Done constructing GBufferedImage." << std::endl;
//    gw.add(img, 50, 50);
//    gw.setVisible(true);
//    pause(500);

//    // fill
//    img->fill(0xff00ff);  // purple

//    std::cout << "About to setRGB on GBufferedImage." << std::endl;
//    for (int y = 2; y < img->getHeight() - 2; y++) {
//        for (int x = 5; x <= img->getWidth() - 5; x++) {
//            img->setRGB(x, y, 0xffcc33);
//        }
//    }
//    std::cout << "Done setting RGB on GBufferedImage." << std::endl;
//    border(img);
//    pause(500);
    
//    std::cout << "About to resize on GBufferedImage." << std::endl;
//    img->resize(100, 50);
//    border(img);
//    pause(500);
    
//    std::cout << "About to resize on GBufferedImage." << std::endl;
//    img->resize(200, 80);
//    border(img);
//    pause(500);
    
//    std::cout << "About to setRGB on GBufferedImage." << std::endl;
//    for (int y = 10; y < img->getHeight() - 10; y++) {
//        for (int x = 10; x <= img->getWidth() - 10; x++) {
//            img->setRGB(x, y, 0xff33cc);
//        }
//    }
//    border(img);
//}

//// all collections:
//// BasicGraph, DawgLexicon, Graph, Grid, HashMap, HashSet, Lexicon, LinkedList,
//// Map, PriorityQueue, Queue, Set, SparseGrid, Stack, Vector

//void hashCodeTest() {
//    // BasicGraph
//    // TODO
    
//    // DawgLexicon
//    HashSet<DawgLexicon> hashdawg;
//    DawgLexicon dlex;
//    dlex.add("a");
//    dlex.add("abc");
//    hashdawg.add(dlex);
//    std::cout << "hashset of dawglexicon: " << hashdawg << std::endl;
    
//    // Deque
//    HashSet<Deque<int> > hashdeq;
//    Deque<int> deq;
//    deq.addBack(1);
//    deq.addFront(2);
//    deq.addBack(3);
//    deq.addFront(4);
//    hashdeq.add(deq);
//    std::cout << "hashset of deque: " << hashdeq << std::endl;

//    // Graph
//    // TODO
    
//    // Grid
//    HashSet<Grid<int> > hashgrid;
//    Grid<int> grid(2, 3);
//    grid.fill(42);
//    hashgrid.add(grid);
//    std::cout << "hashset of grid: " << hashgrid << std::endl;
    
//    // HashMap
//    HashSet<HashMap<int, int> > hashhashmap;
//    HashMap<int, int> hmap;
//    hmap.add(69, 96);
//    hmap.add(42, 24);
//    hashhashmap.add(hmap);
//    std::cout << "hashset of hashmap: " << hashhashmap << std::endl;
    
//    // HashSet
//    HashSet<HashSet<int> > hashhashset;
//    HashSet<int> hset;
//    hset.add(69);
//    hset.add(42);
//    hashhashset.add(hset);
//    std::cout << "hashset of hashset: " << hashhashset << std::endl;
    
//    // Lexicon
//    HashSet<Lexicon> hashlex;
//    Lexicon lex;
//    lex.add("a");
//    lex.add("abc");
//    hashlex.add(lex);
//    std::cout << "hashset of lexicon: " << hashlex << std::endl;
    
//    // LinkedList
//    HashSet<LinkedList<int> > hashlink;
//    LinkedList<int> llist;
//    llist.add(69);
//    llist.add(42);
//    hashlink.add(llist);
//    std::cout << "hashset of linkedlist: " << hashlink << std::endl;
    
//    // Map
//    HashSet<Map<int, int> > hashmap;
//    Map<int, int> map;
//    map.add(69, 96);
//    map.add(42, 24);
//    hashmap.add(map);
//    std::cout << "hashset of map: " << hashmap << std::endl;
    
//    // PriorityQueue
//    HashSet<PriorityQueue<std::string> > hashpq;
//    PriorityQueue<std::string> pq;
//    pq.add("a", 3);
//    pq.add("b", 2);
//    hashpq.add(pq);
//    std::cout << "hashset of priorityqueue: " << hashpq << std::endl;
    
//    // Queue
//    HashSet<Queue<int> > hashq;
//    Queue<int> q;
//    q.add(69);
//    q.add(42);
//    hashq.add(q);
//    std::cout << "hashset of queue: " << hashq << std::endl;
    
////    for (int n : q) {
////        std::cout << "queue element: " << n << std::endl;
////    }
    
//    // Set
//    HashSet<Set<int> > hashset;
//    Set<int> set;
//    set.add(69);
//    set.add(42);
//    hashset.add(set);
//    std::cout << "hashset of set: " << hashset << std::endl;
    
//    // SparseGrid
//    HashSet<SparseGrid<int> > hashsparsegrid;
//    SparseGrid<int> sparsegrid(2, 2);
//    sparsegrid.fill(8);
//    hashsparsegrid.add(sparsegrid);
//    std::cout << "hashset of sparsegrid: " << hashsparsegrid << std::endl;
    
//    // Stack
//    HashSet<Stack<int> > hashstack;
//    Stack<int> s;
//    s.add(69);
//    s.add(42);
    
////    for (int val : s) {
////        std::cout << "stack value: " << val << std::endl;
////    }
    
//    hashstack.add(s);
//    std::cout << "hashset of stack: " << hashstack << std::endl;
    
//    // Vector
//    HashSet<Vector<int> > hashvec;
//    Vector<int> v;
//    v.add(69);
//    v.add(42);
//    hashvec.add(v);
//    std::cout << "hashset of vector: " << hashvec << std::endl;
    
//    Stack<Student> vstud;
//    Student sam;
//    vstud.add(sam);
//    Vector <Student> vstud2;
//    vstud2.add(sam);
//    std::cout << "stack of student: size " << vstud.size() << std::endl;
    
    
//    std::cout << std::endl;
//    HashMap< HashSet<Vector<std::string> >, Vector<std::string> > ngram;
//    HashSet<Vector<std::string> > key;
//    Vector<std::string> value;
//    ngram.put(key, value);
//    std::cout << "ngram is " << ngram << std::endl;
//}

//void urlTest() {
//    std::cout << "Downloading ..." << std::endl;
//    iurlstream testurl;
//    testurl.open("http://martystepp.com/");
//    std::cout << "in the URL stream: " << std::endl;
//    std::cout << "====================" << std::endl;
//    std::string line;
//    while (getline(testurl, line)) {
//        std::cout << "line: " << line << std::endl;
//    }
//    testurl.close();
//    std::cout << "====================" << std::endl;
//}
