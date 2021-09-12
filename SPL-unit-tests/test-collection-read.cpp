#include "deque.h"
#include "grid.h"
#include "hashmap.h"
#include "hashset.h"
#include "map.h"
#include "priorityqueue.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "console.h"
#include "linkedlist.h"
#include "testing/SimpleTest.h"
using namespace std;

template <typename Collection> void readFromString(Collection& coll, string s)
{
    istringstream in(s);
    if (!(in >> coll)) error("Unable to parse original: " + s);

    ostringstream out;
    out << coll << endl;
    istringstream in2(out.str());
    Collection coll2;
    if (!(in2 >> coll2)) error("Unable to parse output: " + out.str() + " original: " + s);
    EXPECT_EQUAL(coll, coll2);
}


PROVIDED_TEST("Read list into Vector, Stack, Queue, Deqeue, LList, Set, HashSet")
{
	string l = "{\"red\", \"green\", \"blue\", \"yellow\", \"\", \"blue\"}";

	Vector<string> v;
	Stack<string> s;
	Queue<string> q;
	Set<string> set;
	HashSet<string> hset;
	LinkedList<string> ll;
	Deque<string> d;

	readFromString(v, l);
	readFromString(s, l);
	readFromString(q, l);
	readFromString(d, l);
	readFromString(ll, l);
	readFromString(set, l);
	readFromString(hset, l);
}

PROVIDED_TEST("Read pairs into Map, HashMap, PQueue")
{
	string p = "{0:\"cashew\", 3.14:\"kale\", 2:\"\", 2.5:\"tacos\"}";
	HashMap<double, string> hmap;
	Map<double, string> map;
	PriorityQueue<string> pq;

	readFromString(map, p);
	readFromString(hmap, p);
	readFromString(pq, p);
}

PROVIDED_TEST("Read groups into Set<Set> and Grid")
{
	string g = "{{\"red\", \"red\", \"red\"}, {\"apple\", \"\", \"pear\"}, {\"red\", \"blue\", \"green\"}}";
	Set<Set<string>> sets;
	HashSet<Set<string>> hsets;
	Grid<string> grid;

	readFromString(sets, g);
	readFromString(hsets, g);
	readFromString(grid, g);
}
