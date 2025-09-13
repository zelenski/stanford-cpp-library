#include "deque.h"
#include "grid.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"
#include "hashmap.h"
#include "hashset.h"
#include "map.h"
#include "set.h"
#include "priorityqueue.h"
#include "bits.h"
using namespace std;

#if defined(__APPLE__)
#define BREAKPOINT __builtin_debugtrap(); cout << "BREAK @" << __LINE__ << endl;
#elif defined(_WIN32)
#define BREAKPOINT __debugbreak(); cout << "BREAK @" << __LINE__ << endl;
#endif

struct Thing {
    int num;
    string label;
};
int hashCode(const Thing& t) {
    return t.num;
}
bool operator ==(const Thing& t1, const Thing& t2) {
    return t1.num == t2.num && t1.label == t2.label;
}
bool operator <(const Thing& t1, const Thing& t2) {
    return stanfordcpplib::collections::compareTo(t1.num, t2.num, t1.label, t2.label) < 0;
}

struct Node {
    char letter;
    Node *left, *right;
};

void stanford_linear() {
	{
		Deque<bool> deque_bool = {false, true};
		Deque<char> deque_char = {'b', 'B'};
		Deque<double> deque_double = {3.14159, -0.25};
		Deque<int> deque_int = {106, 1891};
		Deque<string> deque_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Deque<GridLocation> deque_gridlocation = {{3, 14}};
		Deque<Thing> deque_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Grid<bool> grid_bool = {{false, true}, {false, true}, {false, true}};
		Grid<char> grid_char = {{'b', 'B'}, {'b', 'B'}, {'b', 'B'}};
		Grid<double> grid_double = {{3.14159, -0.25}, {3.14159, -0.25}, {3.14159, -0.25}};
		Grid<int> grid_int = {{106, 1891}, {106, 1891}, {106, 1891}};
		Grid<string> grid_string = {{"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Grid<GridLocation> grid_gridlocation = {{{3, 14}}, {{3, 14}}, {{3, 14}}};
		Grid<Thing> grid_thing = {{(Thing){6, "purple"}}, {(Thing){6, "purple"}}, {(Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		Queue<bool> queue_bool = {false, true};
		Queue<char> queue_char = {'b', 'B'};
		Queue<double> queue_double = {3.14159, -0.25};
		Queue<int> queue_int = {106, 1891};
		Queue<string> queue_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Queue<GridLocation> queue_gridlocation = {{3, 14}};
		Queue<Thing> queue_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Stack<bool> stack_bool = {false, true};
		Stack<char> stack_char = {'b', 'B'};
		Stack<double> stack_double = {3.14159, -0.25};
		Stack<int> stack_int = {106, 1891};
		Stack<string> stack_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Stack<GridLocation> stack_gridlocation = {{3, 14}};
		Stack<Thing> stack_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Vector<bool> vector_bool = {false, true};
		Vector<char> vector_char = {'b', 'B'};
		Vector<double> vector_double = {3.14159, -0.25};
		Vector<int> vector_int = {106, 1891};
		Vector<string> vector_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Vector<GridLocation> vector_gridlocation = {{3, 14}};
		Vector<Thing> vector_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_hash() {
	{
		HashMap<bool, bool> hashmap_bool_bool = {{false, false}, {true, true}};
		HashMap<char, bool> hashmap_char_bool = {{'b', false}, {'B', true}};
		HashMap<double, bool> hashmap_double_bool = {{3.14159, false}, {-0.25, true}};
		HashMap<int, bool> hashmap_int_bool = {{106, false}, {1891, true}};
		HashMap<string, bool> hashmap_string_bool = {{"stanford", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		HashMap<GridLocation, bool> hashmap_gridlocation_bool = {{{3, 14}, false}};
		HashMap<Thing, bool> hashmap_thing_bool = {{(Thing){6, "purple"}, false}};
		HashMap<bool, char> hashmap_bool_char = {{false, 'b'}, {true, 'B'}};
		HashMap<char, char> hashmap_char_char = {{'b', 'b'}, {'B', 'B'}};
		HashMap<double, char> hashmap_double_char = {{3.14159, 'b'}, {-0.25, 'B'}};
		HashMap<int, char> hashmap_int_char = {{106, 'b'}, {1891, 'B'}};
		HashMap<string, char> hashmap_string_char = {{"stanford", 'b'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		HashMap<GridLocation, char> hashmap_gridlocation_char = {{{3, 14}, 'b'}};
		HashMap<Thing, char> hashmap_thing_char = {{(Thing){6, "purple"}, 'b'}};
		HashMap<bool, double> hashmap_bool_double = {{false, 3.14159}, {true, -0.25}};
		HashMap<char, double> hashmap_char_double = {{'b', 3.14159}, {'B', -0.25}};
		HashMap<double, double> hashmap_double_double = {{3.14159, 3.14159}, {-0.25, -0.25}};
		HashMap<int, double> hashmap_int_double = {{106, 3.14159}, {1891, -0.25}};
		HashMap<string, double> hashmap_string_double = {{"stanford", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		HashMap<GridLocation, double> hashmap_gridlocation_double = {{{3, 14}, 3.14159}};
		HashMap<Thing, double> hashmap_thing_double = {{(Thing){6, "purple"}, 3.14159}};
		HashMap<bool, int> hashmap_bool_int = {{false, 106}, {true, 1891}};
		HashMap<char, int> hashmap_char_int = {{'b', 106}, {'B', 1891}};
		HashMap<double, int> hashmap_double_int = {{3.14159, 106}, {-0.25, 1891}};
		HashMap<int, int> hashmap_int_int = {{106, 106}, {1891, 1891}};
		HashMap<string, int> hashmap_string_int = {{"stanford", 106}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 1891}};
		HashMap<GridLocation, int> hashmap_gridlocation_int = {{{3, 14}, 106}};
		HashMap<Thing, int> hashmap_thing_int = {{(Thing){6, "purple"}, 106}};
		HashMap<bool, string> hashmap_bool_string = {{false, "stanford"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<char, string> hashmap_char_string = {{'b', "stanford"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<double, string> hashmap_double_string = {{3.14159, "stanford"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<int, string> hashmap_int_string = {{106, "stanford"}, {1891, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<string, string> hashmap_string_string = {{"stanford", "stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<GridLocation, string> hashmap_gridlocation_string = {{{3, 14}, "stanford"}};
		HashMap<Thing, string> hashmap_thing_string = {{(Thing){6, "purple"}, "stanford"}};
		HashMap<bool, GridLocation> hashmap_bool_gridlocation = {{false, {3, 14}}};
		HashMap<char, GridLocation> hashmap_char_gridlocation = {{'b', {3, 14}}};
		HashMap<double, GridLocation> hashmap_double_gridlocation = {{3.14159, {3, 14}}};
		HashMap<int, GridLocation> hashmap_int_gridlocation = {{106, {3, 14}}};
		HashMap<string, GridLocation> hashmap_string_gridlocation = {{"stanford", {3, 14}}};
		HashMap<GridLocation, GridLocation> hashmap_gridlocation_gridlocation = {{{3, 14}, {3, 14}}};
		HashMap<Thing, GridLocation> hashmap_thing_gridlocation = {{(Thing){6, "purple"}, {3, 14}}};
		HashMap<bool, Thing> hashmap_bool_thing = {{false, (Thing){6, "purple"}}};
		HashMap<char, Thing> hashmap_char_thing = {{'b', (Thing){6, "purple"}}};
		HashMap<double, Thing> hashmap_double_thing = {{3.14159, (Thing){6, "purple"}}};
		HashMap<int, Thing> hashmap_int_thing = {{106, (Thing){6, "purple"}}};
		HashMap<string, Thing> hashmap_string_thing = {{"stanford", (Thing){6, "purple"}}};
		HashMap<GridLocation, Thing> hashmap_gridlocation_thing = {{{3, 14}, (Thing){6, "purple"}}};
		HashMap<Thing, Thing> hashmap_thing_thing = {{(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		HashSet<bool> hashset_bool = {false, true};
		HashSet<char> hashset_char = {'b', 'B'};
		HashSet<double> hashset_double = {3.14159, -0.25};
		HashSet<int> hashset_int = {106, 1891};
		HashSet<string> hashset_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		HashSet<GridLocation> hashset_gridlocation = {{3, 14}};
		HashSet<Thing> hashset_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_tree() {
	{
		Map<bool, bool> map_bool_bool = {{false, false}, {true, true}};
		Map<char, bool> map_char_bool = {{'b', false}, {'B', true}};
		Map<double, bool> map_double_bool = {{3.14159, false}, {-0.25, true}};
		Map<int, bool> map_int_bool = {{106, false}, {1891, true}};
		Map<string, bool> map_string_bool = {{"stanford", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		Map<GridLocation, bool> map_gridlocation_bool = {{{3, 14}, false}};
		Map<Thing, bool> map_thing_bool = {{(Thing){6, "purple"}, false}};
		Map<bool, char> map_bool_char = {{false, 'b'}, {true, 'B'}};
		Map<char, char> map_char_char = {{'b', 'b'}, {'B', 'B'}};
		Map<double, char> map_double_char = {{3.14159, 'b'}, {-0.25, 'B'}};
		Map<int, char> map_int_char = {{106, 'b'}, {1891, 'B'}};
		Map<string, char> map_string_char = {{"stanford", 'b'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		Map<GridLocation, char> map_gridlocation_char = {{{3, 14}, 'b'}};
		Map<Thing, char> map_thing_char = {{(Thing){6, "purple"}, 'b'}};
		Map<bool, double> map_bool_double = {{false, 3.14159}, {true, -0.25}};
		Map<char, double> map_char_double = {{'b', 3.14159}, {'B', -0.25}};
		Map<double, double> map_double_double = {{3.14159, 3.14159}, {-0.25, -0.25}};
		Map<int, double> map_int_double = {{106, 3.14159}, {1891, -0.25}};
		Map<string, double> map_string_double = {{"stanford", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		Map<GridLocation, double> map_gridlocation_double = {{{3, 14}, 3.14159}};
		Map<Thing, double> map_thing_double = {{(Thing){6, "purple"}, 3.14159}};
		Map<bool, int> map_bool_int = {{false, 106}, {true, 1891}};
		Map<char, int> map_char_int = {{'b', 106}, {'B', 1891}};
		Map<double, int> map_double_int = {{3.14159, 106}, {-0.25, 1891}};
		Map<int, int> map_int_int = {{106, 106}, {1891, 1891}};
		Map<string, int> map_string_int = {{"stanford", 106}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 1891}};
		Map<GridLocation, int> map_gridlocation_int = {{{3, 14}, 106}};
		Map<Thing, int> map_thing_int = {{(Thing){6, "purple"}, 106}};
		Map<bool, string> map_bool_string = {{false, "stanford"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<char, string> map_char_string = {{'b', "stanford"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<double, string> map_double_string = {{3.14159, "stanford"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<int, string> map_int_string = {{106, "stanford"}, {1891, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<string, string> map_string_string = {{"stanford", "stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<GridLocation, string> map_gridlocation_string = {{{3, 14}, "stanford"}};
		Map<Thing, string> map_thing_string = {{(Thing){6, "purple"}, "stanford"}};
		Map<bool, GridLocation> map_bool_gridlocation = {{false, {3, 14}}};
		Map<char, GridLocation> map_char_gridlocation = {{'b', {3, 14}}};
		Map<double, GridLocation> map_double_gridlocation = {{3.14159, {3, 14}}};
		Map<int, GridLocation> map_int_gridlocation = {{106, {3, 14}}};
		Map<string, GridLocation> map_string_gridlocation = {{"stanford", {3, 14}}};
		Map<GridLocation, GridLocation> map_gridlocation_gridlocation = {{{3, 14}, {3, 14}}};
		Map<Thing, GridLocation> map_thing_gridlocation = {{(Thing){6, "purple"}, {3, 14}}};
		Map<bool, Thing> map_bool_thing = {{false, (Thing){6, "purple"}}};
		Map<char, Thing> map_char_thing = {{'b', (Thing){6, "purple"}}};
		Map<double, Thing> map_double_thing = {{3.14159, (Thing){6, "purple"}}};
		Map<int, Thing> map_int_thing = {{106, (Thing){6, "purple"}}};
		Map<string, Thing> map_string_thing = {{"stanford", (Thing){6, "purple"}}};
		Map<GridLocation, Thing> map_gridlocation_thing = {{{3, 14}, (Thing){6, "purple"}}};
		Map<Thing, Thing> map_thing_thing = {{(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		Set<bool> set_bool = {false, true};
		Set<char> set_char = {'b', 'B'};
		Set<double> set_double = {3.14159, -0.25};
		Set<int> set_int = {106, 1891};
		Set<string> set_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Set<GridLocation> set_gridlocation = {{3, 14}};
		Set<Thing> set_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_other() {
	{
		PriorityQueue<bool> priorityqueue_bool = {{106, false}, {1891, true}};
		PriorityQueue<char> priorityqueue_char = {{106, 'b'}, {1891, 'B'}};
		PriorityQueue<double> priorityqueue_double = {{106, 3.14159}, {1891, -0.25}};
		PriorityQueue<int> priorityqueue_int = {{106, 106}, {1891, 1891}};
		PriorityQueue<string> priorityqueue_string = {{106, "stanford"}, {1891, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		PriorityQueue<GridLocation> priorityqueue_gridlocation = {{106, {3, 14}}};
		PriorityQueue<Thing> priorityqueue_thing = {{106, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
}


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
    GridLocationRange redistrict_Range = boggle_Grid.locations();
    GridLocation maze_Loc(2, 4);
    Stack<GridLocation> maze_Stack_Loc = { {1, 2}, {3, 4}, {5, 6}, {7, 8}};
    BREAKPOINT;
}

void node_pair() {
    // key align 1,4,8 and val align 1,4,8
    Thing t = {1891, "stanford"};
    GridLocation gl = {3, 14};
    {
        Map<char, char> treenode_11 = { {'B','b'} };
        Map<char, int> treenode_14 = { {'B',106} };
        Map<char, double> treenode_18 = { {'B',0.5} };
        Map<int, char> treenode_41 = { {106,'B'} };
        Map<int, int> treenode_44 = { {106,1891} };
        Map<int, double> treenode_48 = { {106,0.5} };
        Map<double, char> treenode_81 = { {0.5,'B'} };
        Map<double, int> treenode_84 = { {0.5,106} };
        Map<double, double> treenode_88 = { {0.5,0.25} };
        BREAKPOINT;
    }
    {
        HashMap<char, char> hashnode_11 = { {'B','b'} };
        HashMap<char, int> hashnode_14 = { {'B',106} };
        HashMap<char, double> hashnode_18 = { {'B',0.5} };
        HashMap<int, char> hashnode_41 = { {106,'B'} };
        HashMap<int, int> hashnode_44 = { {106,1891} };
        HashMap<int, double> hashnode_48 = { {106,0.5} };
        HashMap<double, char> hashnode_81 = { {0.5,'B'} };
        HashMap<double, int> hashnode_84 = { {0.5,106} };
        HashMap<double, double> hashnode_88 = { {0.5,0.25} };
        BREAKPOINT;
    }
    // struct with align 4,8 as key, again as val
    {
        Map<char, GridLocation> treenode_14 = { {'B',gl} };
        Map<char, Thing> treenode_18 = { {'B',t} };
        Map<int, GridLocation> treenode_44 = { {106,gl} };
        Map<int, Thing> treenode_48 = { {106,t} };
        Map<double, GridLocation> treenode_84 = { {0.5,gl} };
        Map<double, Thing> treenode_88 = { {0.5,t} };

        Map<GridLocation, char> treenode_41 = { {gl,'B'} };
        Map<Thing, char> treenode_81 = { {t,'B'} };
        Map<GridLocation, int> treenode_44s = { {gl,106} };
        Map<Thing, int> treenode_48s = { {t,106} };
        Map<GridLocation, double> treenode_84s = { {gl,0.5} };
        Map<Thing, double> treenode_88s = { {t,0.5} };
        BREAKPOINT;
    }
    {
        HashMap<char, GridLocation> hashnode_14 = { {'B',gl} };
        HashMap<char, Thing> hashnode_18 = { {'B',t} };
        HashMap<int, GridLocation> hashnode_44 = { {106,gl} };
        HashMap<int, Thing> hashnode_48 = { {106,t} };
        HashMap<double, GridLocation> hashnode_84 = { {0.5,gl} };
        HashMap<double, Thing> hashnode_88 = { {0.5,t} };

        HashMap<GridLocation, char> hashnode_41 = { {gl,'B'} };
        HashMap<Thing, char> hashnode_81 = { {t,'B'} };
        HashMap<GridLocation, int> hashnode_44s = { {gl,106} };
        HashMap<Thing, int> hashnode_48s = { {t,106} };
        HashMap<GridLocation, double> hashnode_84s = { {gl,0.5} };
        HashMap<Thing, double> hashnode_88s = { {t,0.5} };
        BREAKPOINT;
    }
}

void fixed_tests() {
    node_pair();
    assign_uses();
}

int main() {
    stanford_linear();
    stanford_hash();
    stanford_tree();
    stanford_other();
    fixed_tests();
    return 0;
}

