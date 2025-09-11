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
		Deque<bool> Deque_bool = {false, true};
		Deque<char> Deque_char = {'a', 'B', 'c'};
		Deque<float> Deque_float = {3.14159, -0.25};
		Deque<int> Deque_int = {1, 5, 10};
		Deque<string> Deque_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Deque<GridLocation> Deque_GridLocation = {{3, 14}, {18, 91}};
		Deque<Thing> Deque_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Grid<bool> Grid_bool = {{false, true}, {false, true}, {false, true}};
		Grid<char> Grid_char = {{'a', 'B', 'c'}, {'a', 'B', 'c'}, {'a', 'B', 'c'}};
		Grid<float> Grid_float = {{3.14159, -0.25}, {3.14159, -0.25}, {3.14159, -0.25}};
		Grid<int> Grid_int = {{1, 5, 10}, {1, 5, 10}, {1, 5, 10}};
		Grid<string> Grid_string = {{"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Grid<GridLocation> Grid_GridLocation = {{{3, 14}, {18, 91}}, {{3, 14}, {18, 91}}, {{3, 14}, {18, 91}}};
		Grid<Thing> Grid_Thing = {{(Thing){5, "green"}, (Thing){6, "purple"}}, {(Thing){5, "green"}, (Thing){6, "purple"}}, {(Thing){5, "green"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		Queue<bool> Queue_bool = {false, true};
		Queue<char> Queue_char = {'a', 'B', 'c'};
		Queue<float> Queue_float = {3.14159, -0.25};
		Queue<int> Queue_int = {1, 5, 10};
		Queue<string> Queue_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Queue<GridLocation> Queue_GridLocation = {{3, 14}, {18, 91}};
		Queue<Thing> Queue_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Stack<bool> Stack_bool = {false, true};
		Stack<char> Stack_char = {'a', 'B', 'c'};
		Stack<float> Stack_float = {3.14159, -0.25};
		Stack<int> Stack_int = {1, 5, 10};
		Stack<string> Stack_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Stack<GridLocation> Stack_GridLocation = {{3, 14}, {18, 91}};
		Stack<Thing> Stack_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
	{
		Vector<bool> Vector_bool = {false, true};
		Vector<char> Vector_char = {'a', 'B', 'c'};
		Vector<float> Vector_float = {3.14159, -0.25};
		Vector<int> Vector_int = {1, 5, 10};
		Vector<string> Vector_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Vector<GridLocation> Vector_GridLocation = {{3, 14}, {18, 91}};
		Vector<Thing> Vector_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_hash() {
	{
		HashMap<bool, bool> HashMap_bool_bool = {{false, false}, {true, true}};
		HashMap<char, bool> HashMap_char_bool = {{'a', false}, {'B', true}};
		HashMap<float, bool> HashMap_float_bool = {{3.14159, false}, {-0.25, true}};
		HashMap<int, bool> HashMap_int_bool = {{1, false}, {5, true}};
		HashMap<string, bool> HashMap_string_bool = {{"tree", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		HashMap<GridLocation, bool> HashMap_GridLocation_bool = {{{3, 14}, false}, {{18, 91}, true}};
		HashMap<Thing, bool> HashMap_Thing_bool = {{(Thing){5, "green"}, false}, {(Thing){6, "purple"}, true}};
		HashMap<bool, char> HashMap_bool_char = {{false, 'a'}, {true, 'B'}};
		HashMap<char, char> HashMap_char_char = {{'a', 'a'}, {'B', 'B'}, {'c', 'c'}};
		HashMap<float, char> HashMap_float_char = {{3.14159, 'a'}, {-0.25, 'B'}};
		HashMap<int, char> HashMap_int_char = {{1, 'a'}, {5, 'B'}, {10, 'c'}};
		HashMap<string, char> HashMap_string_char = {{"tree", 'a'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		HashMap<GridLocation, char> HashMap_GridLocation_char = {{{3, 14}, 'a'}, {{18, 91}, 'B'}};
		HashMap<Thing, char> HashMap_Thing_char = {{(Thing){5, "green"}, 'a'}, {(Thing){6, "purple"}, 'B'}};
		HashMap<bool, float> HashMap_bool_float = {{false, 3.14159}, {true, -0.25}};
		HashMap<char, float> HashMap_char_float = {{'a', 3.14159}, {'B', -0.25}};
		HashMap<float, float> HashMap_float_float = {{3.14159, 3.14159}, {-0.25, -0.25}};
		HashMap<int, float> HashMap_int_float = {{1, 3.14159}, {5, -0.25}};
		HashMap<string, float> HashMap_string_float = {{"tree", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		HashMap<GridLocation, float> HashMap_GridLocation_float = {{{3, 14}, 3.14159}, {{18, 91}, -0.25}};
		HashMap<Thing, float> HashMap_Thing_float = {{(Thing){5, "green"}, 3.14159}, {(Thing){6, "purple"}, -0.25}};
		HashMap<bool, int> HashMap_bool_int = {{false, 1}, {true, 5}};
		HashMap<char, int> HashMap_char_int = {{'a', 1}, {'B', 5}, {'c', 10}};
		HashMap<float, int> HashMap_float_int = {{3.14159, 1}, {-0.25, 5}};
		HashMap<int, int> HashMap_int_int = {{1, 1}, {5, 5}, {10, 10}};
		HashMap<string, int> HashMap_string_int = {{"tree", 1}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 5}};
		HashMap<GridLocation, int> HashMap_GridLocation_int = {{{3, 14}, 1}, {{18, 91}, 5}};
		HashMap<Thing, int> HashMap_Thing_int = {{(Thing){5, "green"}, 1}, {(Thing){6, "purple"}, 5}};
		HashMap<bool, string> HashMap_bool_string = {{false, "tree"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<char, string> HashMap_char_string = {{'a', "tree"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<float, string> HashMap_float_string = {{3.14159, "tree"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<int, string> HashMap_int_string = {{1, "tree"}, {5, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<string, string> HashMap_string_string = {{"tree", "tree"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<GridLocation, string> HashMap_GridLocation_string = {{{3, 14}, "tree"}, {{18, 91}, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<Thing, string> HashMap_Thing_string = {{(Thing){5, "green"}, "tree"}, {(Thing){6, "purple"}, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<bool, GridLocation> HashMap_bool_GridLocation = {{false, {3, 14}}, {true, {18, 91}}};
		HashMap<char, GridLocation> HashMap_char_GridLocation = {{'a', {3, 14}}, {'B', {18, 91}}};
		HashMap<float, GridLocation> HashMap_float_GridLocation = {{3.14159, {3, 14}}, {-0.25, {18, 91}}};
		HashMap<int, GridLocation> HashMap_int_GridLocation = {{1, {3, 14}}, {5, {18, 91}}};
		HashMap<string, GridLocation> HashMap_string_GridLocation = {{"tree", {3, 14}}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", {18, 91}}};
		HashMap<GridLocation, GridLocation> HashMap_GridLocation_GridLocation = {{{3, 14}, {3, 14}}, {{18, 91}, {18, 91}}};
		HashMap<Thing, GridLocation> HashMap_Thing_GridLocation = {{(Thing){5, "green"}, {3, 14}}, {(Thing){6, "purple"}, {18, 91}}};
		HashMap<bool, Thing> HashMap_bool_Thing = {{false, (Thing){5, "green"}}, {true, (Thing){6, "purple"}}};
		HashMap<char, Thing> HashMap_char_Thing = {{'a', (Thing){5, "green"}}, {'B', (Thing){6, "purple"}}};
		HashMap<float, Thing> HashMap_float_Thing = {{3.14159, (Thing){5, "green"}}, {-0.25, (Thing){6, "purple"}}};
		HashMap<int, Thing> HashMap_int_Thing = {{1, (Thing){5, "green"}}, {5, (Thing){6, "purple"}}};
		HashMap<string, Thing> HashMap_string_Thing = {{"tree", (Thing){5, "green"}}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", (Thing){6, "purple"}}};
		HashMap<GridLocation, Thing> HashMap_GridLocation_Thing = {{{3, 14}, (Thing){5, "green"}}, {{18, 91}, (Thing){6, "purple"}}};
		HashMap<Thing, Thing> HashMap_Thing_Thing = {{(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		HashSet<bool> HashSet_bool = {false, true};
		HashSet<char> HashSet_char = {'a', 'B', 'c'};
		HashSet<float> HashSet_float = {3.14159, -0.25};
		HashSet<int> HashSet_int = {1, 5, 10};
		HashSet<string> HashSet_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		HashSet<GridLocation> HashSet_GridLocation = {{3, 14}, {18, 91}};
		HashSet<Thing> HashSet_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_tree() {
	{
		Map<bool, bool> Map_bool_bool = {{false, false}, {true, true}};
		Map<char, bool> Map_char_bool = {{'a', false}, {'B', true}};
		Map<float, bool> Map_float_bool = {{3.14159, false}, {-0.25, true}};
		Map<int, bool> Map_int_bool = {{1, false}, {5, true}};
		Map<string, bool> Map_string_bool = {{"tree", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		Map<GridLocation, bool> Map_GridLocation_bool = {{{3, 14}, false}, {{18, 91}, true}};
		Map<Thing, bool> Map_Thing_bool = {{(Thing){5, "green"}, false}, {(Thing){6, "purple"}, true}};
		Map<bool, char> Map_bool_char = {{false, 'a'}, {true, 'B'}};
		Map<char, char> Map_char_char = {{'a', 'a'}, {'B', 'B'}, {'c', 'c'}};
		Map<float, char> Map_float_char = {{3.14159, 'a'}, {-0.25, 'B'}};
		Map<int, char> Map_int_char = {{1, 'a'}, {5, 'B'}, {10, 'c'}};
		Map<string, char> Map_string_char = {{"tree", 'a'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		Map<GridLocation, char> Map_GridLocation_char = {{{3, 14}, 'a'}, {{18, 91}, 'B'}};
		Map<Thing, char> Map_Thing_char = {{(Thing){5, "green"}, 'a'}, {(Thing){6, "purple"}, 'B'}};
		Map<bool, float> Map_bool_float = {{false, 3.14159}, {true, -0.25}};
		Map<char, float> Map_char_float = {{'a', 3.14159}, {'B', -0.25}};
		Map<float, float> Map_float_float = {{3.14159, 3.14159}, {-0.25, -0.25}};
		Map<int, float> Map_int_float = {{1, 3.14159}, {5, -0.25}};
		Map<string, float> Map_string_float = {{"tree", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		Map<GridLocation, float> Map_GridLocation_float = {{{3, 14}, 3.14159}, {{18, 91}, -0.25}};
		Map<Thing, float> Map_Thing_float = {{(Thing){5, "green"}, 3.14159}, {(Thing){6, "purple"}, -0.25}};
		Map<bool, int> Map_bool_int = {{false, 1}, {true, 5}};
		Map<char, int> Map_char_int = {{'a', 1}, {'B', 5}, {'c', 10}};
		Map<float, int> Map_float_int = {{3.14159, 1}, {-0.25, 5}};
		Map<int, int> Map_int_int = {{1, 1}, {5, 5}, {10, 10}};
		Map<string, int> Map_string_int = {{"tree", 1}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 5}};
		Map<GridLocation, int> Map_GridLocation_int = {{{3, 14}, 1}, {{18, 91}, 5}};
		Map<Thing, int> Map_Thing_int = {{(Thing){5, "green"}, 1}, {(Thing){6, "purple"}, 5}};
		Map<bool, string> Map_bool_string = {{false, "tree"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<char, string> Map_char_string = {{'a', "tree"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<float, string> Map_float_string = {{3.14159, "tree"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<int, string> Map_int_string = {{1, "tree"}, {5, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<string, string> Map_string_string = {{"tree", "tree"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<GridLocation, string> Map_GridLocation_string = {{{3, 14}, "tree"}, {{18, 91}, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<Thing, string> Map_Thing_string = {{(Thing){5, "green"}, "tree"}, {(Thing){6, "purple"}, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<bool, GridLocation> Map_bool_GridLocation = {{false, {3, 14}}, {true, {18, 91}}};
		Map<char, GridLocation> Map_char_GridLocation = {{'a', {3, 14}}, {'B', {18, 91}}};
		Map<float, GridLocation> Map_float_GridLocation = {{3.14159, {3, 14}}, {-0.25, {18, 91}}};
		Map<int, GridLocation> Map_int_GridLocation = {{1, {3, 14}}, {5, {18, 91}}};
		Map<string, GridLocation> Map_string_GridLocation = {{"tree", {3, 14}}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", {18, 91}}};
		Map<GridLocation, GridLocation> Map_GridLocation_GridLocation = {{{3, 14}, {3, 14}}, {{18, 91}, {18, 91}}};
		Map<Thing, GridLocation> Map_Thing_GridLocation = {{(Thing){5, "green"}, {3, 14}}, {(Thing){6, "purple"}, {18, 91}}};
		Map<bool, Thing> Map_bool_Thing = {{false, (Thing){5, "green"}}, {true, (Thing){6, "purple"}}};
		Map<char, Thing> Map_char_Thing = {{'a', (Thing){5, "green"}}, {'B', (Thing){6, "purple"}}};
		Map<float, Thing> Map_float_Thing = {{3.14159, (Thing){5, "green"}}, {-0.25, (Thing){6, "purple"}}};
		Map<int, Thing> Map_int_Thing = {{1, (Thing){5, "green"}}, {5, (Thing){6, "purple"}}};
		Map<string, Thing> Map_string_Thing = {{"tree", (Thing){5, "green"}}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", (Thing){6, "purple"}}};
		Map<GridLocation, Thing> Map_GridLocation_Thing = {{{3, 14}, (Thing){5, "green"}}, {{18, 91}, (Thing){6, "purple"}}};
		Map<Thing, Thing> Map_Thing_Thing = {{(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		Set<bool> Set_bool = {false, true};
		Set<char> Set_char = {'a', 'B', 'c'};
		Set<float> Set_float = {3.14159, -0.25};
		Set<int> Set_int = {1, 5, 10};
		Set<string> Set_string = {"tree", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Set<GridLocation> Set_GridLocation = {{3, 14}, {18, 91}};
		Set<Thing> Set_Thing = {(Thing){5, "green"}, (Thing){6, "purple"}};
		BREAKPOINT;
	}
}

void stanford_other() {
	{
		PriorityQueue<bool> PriorityQueue_bool = {{1, false}, {5, true}};
		PriorityQueue<char> PriorityQueue_char = {{1, 'a'}, {5, 'B'}, {10, 'c'}};
		PriorityQueue<float> PriorityQueue_float = {{1, 3.14159}, {5, -0.25}};
		PriorityQueue<int> PriorityQueue_int = {{1, 1}, {5, 5}, {10, 10}};
		PriorityQueue<string> PriorityQueue_string = {{1, "tree"}, {5, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		PriorityQueue<GridLocation> PriorityQueue_GridLocation = {{1, {3, 14}}, {5, {18, 91}}};
		PriorityQueue<Thing> PriorityQueue_Thing = {{1, (Thing){5, "green"}}, {5, (Thing){6, "purple"}}};
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

int main() {
    stanford_linear();
    stanford_hash();
    stanford_tree();
    stanford_other();
    assign_uses();
    return 0;
}

