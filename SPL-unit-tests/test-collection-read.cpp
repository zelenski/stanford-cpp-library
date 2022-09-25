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
#include "SimpleTest.h"
using namespace std;

template <typename Collection> void readFromString(Collection& coll, string s)
{
    istringstream in(s);
     if (!(in >> coll)) error("Unable to read collection from: " + s);

    ostringstream out;
    out << coll << endl;
    istringstream in2(out.str());
    Collection coll2;
    if (!(in2 >> coll2)) error("Unable to read collection that was written: " + out.str());
    EXPECT_EQUAL(coll, coll2);
}


PROVIDED_TEST("Correctly read list into Vector, Stack, Queue, Deqeue, LList, Set, HashSet")
{
    string l = "{red, green, blue, yellow, \"\", blue}";
    //string l = "{\"red\", \"green\", \"blue\", \"yellow\", \"\", \"blue\"}";

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

    string empty = "{}";
    readFromString(v, empty);
    EXPECT(v.isEmpty());
}

PROVIDED_TEST("Confirm errors (malformed list)")
{
    Vector<int> quotes;
    EXPECT_ERROR(readFromString(quotes, "{apple\"}")); // no open "
    EXPECT_ERROR(readFromString(quotes, "{\"apple}")); // no close "

    Vector<int> malformed;
    EXPECT_ERROR(readFromString(malformed, "3}")); // no open {
    EXPECT_ERROR(readFromString(malformed, "{3")); // no close }
    EXPECT_ERROR(readFromString(malformed, "{3 4}")); // no comma
    EXPECT_ERROR(readFromString(malformed, "{3ab}")); // not a number
}

PROVIDED_TEST("Correctly read pairs into Map, HashMap, PQueue")
{
//	string p = "{0:\"cashew\", 3.14:\"kale\", 2:\"\", 2.5:\"tacos\"}";
    string p = "{0:cashew, 3.14:kale, 2:\"\", 2.5:tacos}";

    HashMap<double, string> hmap;
	Map<double, string> map;
	PriorityQueue<string> pq;

	readFromString(map, p);
	readFromString(hmap, p);
	readFromString(pq, p);

    string empty = "{}";
    readFromString(map, empty);
    EXPECT(map.isEmpty());

}

PROVIDED_TEST("Confirm errors (malformed pair)")
{
    Map<int, int> malformed;
    EXPECT_ERROR(readFromString(malformed, "{3,4}}")); // no open {
    EXPECT_ERROR(readFromString(malformed, "{{3,4}")); // no close }
    EXPECT_ERROR(readFromString(malformed, "{{3 4}}")); // no comma
}

PROVIDED_TEST("Correctly read groups into Set<Set> and Grid")
{
//	string g = "{{\"red\", \"red\", \"red\"}, {\"apple\", \"\", \"pear\"}, {\"red\", \"blue\", \"green\"}}";
    string g = "{{red, red, red}, {apple, \"\", pear}, {red, blue, green}}";

    Set<Set<string>> sets;
	HashSet<Set<string>> hsets;
	Grid<string> grid;

	readFromString(sets, g);
	readFromString(hsets, g);
    readFromString(grid, g);

    string empty = "{}";
    readFromString(sets, empty);
    EXPECT(sets.isEmpty());
}

PROVIDED_TEST("Confirm Grid error (non-rectangular input)")
{
    Grid<int> malformed;
    EXPECT_ERROR(readFromString(malformed, "{{3, 4}, {1, 2}, {5, 6, 7}}"));
    EXPECT_ERROR(readFromString(malformed, "{{3, 4}, {1, 2}, {5}}"));

    EXPECT_ERROR( (malformed = {{3, 4}, {1, 2}, {5, 6, 7}}) );
    EXPECT_ERROR( (malformed = {{3, 4}, {1, 2}, {5}}) );
}

void confirmGood(string s, string expected = "INPUT")
{
    string t;
    string result = expected == "INPUT" ? s : expected;

    istringstream in(s);
    EXPECT(readQuotedString(in, t, true));
    EXPECT_EQUAL(t, result);

    istringstream in2(s);
    EXPECT(readQuotedString(in2, t, false));
    EXPECT_EQUAL(t, result);
}

void confirmBad(string s)
{
    string t;

    istringstream in(s);
    EXPECT(!readQuotedString(in, t, false));

    istringstream in2(s);
    EXPECT_ERROR(readQuotedString(in2, t, true));
}

PROVIDED_TEST("correct readQuotedString (double and single quotes)")
{
    confirmGood("\"apple\"", "apple");
    confirmGood("'apple'", "apple");
    confirmGood("''", "");
    confirmGood("\"\"", "");
}

PROVIDED_TEST("correct readQuotedString (no quotes)")
{
    confirmGood("apple");
    confirmGood("");
    confirmGood("apple banana cherry");
    // surprise -- stops at delimiter
    confirmGood("apple:\nbanana", "apple");
    confirmGood("apple, banana, cherry", "apple");
}

PROVIDED_TEST("errors readQuotedString (open quote no/mismatch close)")
{
    confirmBad("'apple");
    confirmBad("\"apple");
    confirmBad("'apple\"");
    confirmBad("'apple\"");
    confirmBad("'apple;");
}

PROVIDED_TEST("correct readQuotedString (valid escapes)")
{
    confirmGood("'\\t'", "\t");
    confirmGood("'\\101'" ,"A");
    confirmGood("'\\x41'" ,"A");
    confirmGood("'\\t\\n'" ,"\t\n");
    confirmGood("'\\101\\102'" ,"AB");
    confirmGood("'\\x41\\x4a'" ,"AJ");
    confirmGood("'\\t\\x41\\102\\x4a\\n'" ,"\tABJ\n");
}

PROVIDED_TEST("errors readQuotedString (invalid escapes)")
{
    confirmBad("'\\");
    confirmBad("'\\123");
    confirmBad("'\\xab");
//    confirmBad("'\\8'"); // not valid octal
}


