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
#define BREAKPOINT __builtin_debugtrap()
#elif defined(_WIN32)
#define BREAKPOINT __debugbreak()
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
		Deque<char> Deque_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		Deque<float> Deque_float = {3.14159, 137.0, -0.5};
		Deque<int> Deque_int = {1, 3, 5, 7, 9};
		Deque<string> Deque_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Deque<GridLocation> Deque_GridLocation = {{3, 14}, {18, 91}};
		Deque<Thing> Deque_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
	{
		Grid<bool> Grid_bool = {{false, true}, {false, true}, {false, true}};
		Grid<char> Grid_char = {{'a', 'b', 'c', 'A', 'B', 'C'}, {'a', 'b', 'c', 'A', 'B', 'C'}, {'a', 'b', 'c', 'A', 'B', 'C'}};
		Grid<float> Grid_float = {{3.14159, 137.0, -0.5}, {3.14159, 137.0, -0.5}, {3.14159, 137.0, -0.5}};
		Grid<int> Grid_int = {{1, 3, 5, 7, 9}, {1, 3, 5, 7, 9}, {1, 3, 5, 7, 9}};
		Grid<string> Grid_string = {{"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}, {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Grid<GridLocation> Grid_GridLocation = {{{3, 14}, {18, 91}}, {{3, 14}, {18, 91}}, {{3, 14}, {18, 91}}};
		Grid<Thing> Grid_Thing = {{(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}, {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}, {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}};
		BREAKPOINT;
	}
	{
		Queue<bool> Queue_bool = {false, true};
		Queue<char> Queue_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		Queue<float> Queue_float = {3.14159, 137.0, -0.5};
		Queue<int> Queue_int = {1, 3, 5, 7, 9};
		Queue<string> Queue_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Queue<GridLocation> Queue_GridLocation = {{3, 14}, {18, 91}};
		Queue<Thing> Queue_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
	{
		Stack<bool> Stack_bool = {false, true};
		Stack<char> Stack_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		Stack<float> Stack_float = {3.14159, 137.0, -0.5};
		Stack<int> Stack_int = {1, 3, 5, 7, 9};
		Stack<string> Stack_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Stack<GridLocation> Stack_GridLocation = {{3, 14}, {18, 91}};
		Stack<Thing> Stack_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
	{
		Vector<bool> Vector_bool = {false, true};
		Vector<char> Vector_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		Vector<float> Vector_float = {3.14159, 137.0, -0.5};
		Vector<int> Vector_int = {1, 3, 5, 7, 9};
		Vector<string> Vector_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Vector<GridLocation> Vector_GridLocation = {{3, 14}, {18, 91}};
		Vector<Thing> Vector_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
}

void stanford_hash() {
	{
		HashMap<bool, bool> HashMap_bool_bool = {{false, false}, {true, true}};
		HashMap<char, bool> HashMap_char_bool = {{'a', false}, {'b', true}};
		HashMap<float, bool> HashMap_float_bool = {{3.14159, false}, {137.0, true}};
		HashMap<int, bool> HashMap_int_bool = {{1, false}, {3, true}};
		HashMap<string, bool> HashMap_string_bool = {{"tree", false}, {"cardinal", true}};
		HashMap<GridLocation, bool> HashMap_GridLocation_bool = {{{3, 14}, false}, {{18, 91}, true}};
		HashMap<Thing, bool> HashMap_Thing_bool = {{(Thing){3, "red"}, false}, {(Thing){5, "green"}, true}};
		HashMap<bool, char> HashMap_bool_char = {{false, 'a'}, {true, 'b'}};
		HashMap<char, char> HashMap_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}, {'A', 'A'}, {'B', 'B'}, {'C', 'C'}};
		HashMap<float, char> HashMap_float_char = {{3.14159, 'a'}, {137.0, 'b'}, {-0.5, 'c'}};
		HashMap<int, char> HashMap_int_char = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'A'}, {9, 'B'}};
		HashMap<string, char> HashMap_string_char = {{"tree", 'a'}, {"cardinal", 'b'}, {"Stanford", 'c'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'A'}};
		HashMap<GridLocation, char> HashMap_GridLocation_char = {{{3, 14}, 'a'}, {{18, 91}, 'b'}};
		HashMap<Thing, char> HashMap_Thing_char = {{(Thing){3, "red"}, 'a'}, {(Thing){5, "green"}, 'b'}, {(Thing){4, "blue"}, 'c'}};
		HashMap<bool, float> HashMap_bool_float = {{false, 3.14159}, {true, 137.0}};
		HashMap<char, float> HashMap_char_float = {{'a', 3.14159}, {'b', 137.0}, {'c', -0.5}};
		HashMap<float, float> HashMap_float_float = {{3.14159, 3.14159}, {137.0, 137.0}, {-0.5, -0.5}};
		HashMap<int, float> HashMap_int_float = {{1, 3.14159}, {3, 137.0}, {5, -0.5}};
		HashMap<string, float> HashMap_string_float = {{"tree", 3.14159}, {"cardinal", 137.0}, {"Stanford", -0.5}};
		HashMap<GridLocation, float> HashMap_GridLocation_float = {{{3, 14}, 3.14159}, {{18, 91}, 137.0}};
		HashMap<Thing, float> HashMap_Thing_float = {{(Thing){3, "red"}, 3.14159}, {(Thing){5, "green"}, 137.0}, {(Thing){4, "blue"}, -0.5}};
		HashMap<bool, int> HashMap_bool_int = {{false, 1}, {true, 3}};
		HashMap<char, int> HashMap_char_int = {{'a', 1}, {'b', 3}, {'c', 5}, {'A', 7}, {'B', 9}};
		HashMap<float, int> HashMap_float_int = {{3.14159, 1}, {137.0, 3}, {-0.5, 5}};
		HashMap<int, int> HashMap_int_int = {{1, 1}, {3, 3}, {5, 5}, {7, 7}, {9, 9}};
		HashMap<string, int> HashMap_string_int = {{"tree", 1}, {"cardinal", 3}, {"Stanford", 5}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 7}};
		HashMap<GridLocation, int> HashMap_GridLocation_int = {{{3, 14}, 1}, {{18, 91}, 3}};
		HashMap<Thing, int> HashMap_Thing_int = {{(Thing){3, "red"}, 1}, {(Thing){5, "green"}, 3}, {(Thing){4, "blue"}, 5}};
		HashMap<bool, string> HashMap_bool_string = {{false, "tree"}, {true, "cardinal"}};
		HashMap<char, string> HashMap_char_string = {{'a', "tree"}, {'b', "cardinal"}, {'c', "Stanford"}, {'A', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<float, string> HashMap_float_string = {{3.14159, "tree"}, {137.0, "cardinal"}, {-0.5, "Stanford"}};
		HashMap<int, string> HashMap_int_string = {{1, "tree"}, {3, "cardinal"}, {5, "Stanford"}, {7, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<string, string> HashMap_string_string = {{"tree", "tree"}, {"cardinal", "cardinal"}, {"Stanford", "Stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<GridLocation, string> HashMap_GridLocation_string = {{{3, 14}, "tree"}, {{18, 91}, "cardinal"}};
		HashMap<Thing, string> HashMap_Thing_string = {{(Thing){3, "red"}, "tree"}, {(Thing){5, "green"}, "cardinal"}, {(Thing){4, "blue"}, "Stanford"}};
		HashMap<bool, GridLocation> HashMap_bool_GridLocation = {{false, {3, 14}}, {true, {18, 91}}};
		HashMap<char, GridLocation> HashMap_char_GridLocation = {{'a', {3, 14}}, {'b', {18, 91}}};
		HashMap<float, GridLocation> HashMap_float_GridLocation = {{3.14159, {3, 14}}, {137.0, {18, 91}}};
		HashMap<int, GridLocation> HashMap_int_GridLocation = {{1, {3, 14}}, {3, {18, 91}}};
		HashMap<string, GridLocation> HashMap_string_GridLocation = {{"tree", {3, 14}}, {"cardinal", {18, 91}}};
		HashMap<GridLocation, GridLocation> HashMap_GridLocation_GridLocation = {{{3, 14}, {3, 14}}, {{18, 91}, {18, 91}}};
		HashMap<Thing, GridLocation> HashMap_Thing_GridLocation = {{(Thing){3, "red"}, {3, 14}}, {(Thing){5, "green"}, {18, 91}}};
		HashMap<bool, Thing> HashMap_bool_Thing = {{false, (Thing){3, "red"}}, {true, (Thing){5, "green"}}};
		HashMap<char, Thing> HashMap_char_Thing = {{'a', (Thing){3, "red"}}, {'b', (Thing){5, "green"}}, {'c', (Thing){4, "blue"}}};
		HashMap<float, Thing> HashMap_float_Thing = {{3.14159, (Thing){3, "red"}}, {137.0, (Thing){5, "green"}}, {-0.5, (Thing){4, "blue"}}};
		HashMap<int, Thing> HashMap_int_Thing = {{1, (Thing){3, "red"}}, {3, (Thing){5, "green"}}, {5, (Thing){4, "blue"}}};
		HashMap<string, Thing> HashMap_string_Thing = {{"tree", (Thing){3, "red"}}, {"cardinal", (Thing){5, "green"}}, {"Stanford", (Thing){4, "blue"}}};
		HashMap<GridLocation, Thing> HashMap_GridLocation_Thing = {{{3, 14}, (Thing){3, "red"}}, {{18, 91}, (Thing){5, "green"}}};
		HashMap<Thing, Thing> HashMap_Thing_Thing = {{(Thing){3, "red"}, (Thing){3, "red"}}, {(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){4, "blue"}, (Thing){4, "blue"}}};
		BREAKPOINT;
	}
	{
		HashSet<bool> HashSet_bool = {false, true};
		HashSet<char> HashSet_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		HashSet<float> HashSet_float = {3.14159, 137.0, -0.5};
		HashSet<int> HashSet_int = {1, 3, 5, 7, 9};
		HashSet<string> HashSet_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		HashSet<GridLocation> HashSet_GridLocation = {{3, 14}, {18, 91}};
		HashSet<Thing> HashSet_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
}

void stanford_tree() {
	{
		Map<bool, bool> Map_bool_bool = {{false, false}, {true, true}};
		Map<char, bool> Map_char_bool = {{'a', false}, {'b', true}};
		Map<float, bool> Map_float_bool = {{3.14159, false}, {137.0, true}};
		Map<int, bool> Map_int_bool = {{1, false}, {3, true}};
		Map<string, bool> Map_string_bool = {{"tree", false}, {"cardinal", true}};
		Map<GridLocation, bool> Map_GridLocation_bool = {{{3, 14}, false}, {{18, 91}, true}};
		Map<Thing, bool> Map_Thing_bool = {{(Thing){3, "red"}, false}, {(Thing){5, "green"}, true}};
		Map<bool, char> Map_bool_char = {{false, 'a'}, {true, 'b'}};
		Map<char, char> Map_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}, {'A', 'A'}, {'B', 'B'}, {'C', 'C'}};
		Map<float, char> Map_float_char = {{3.14159, 'a'}, {137.0, 'b'}, {-0.5, 'c'}};
		Map<int, char> Map_int_char = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'A'}, {9, 'B'}};
		Map<string, char> Map_string_char = {{"tree", 'a'}, {"cardinal", 'b'}, {"Stanford", 'c'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'A'}};
		Map<GridLocation, char> Map_GridLocation_char = {{{3, 14}, 'a'}, {{18, 91}, 'b'}};
		Map<Thing, char> Map_Thing_char = {{(Thing){3, "red"}, 'a'}, {(Thing){5, "green"}, 'b'}, {(Thing){4, "blue"}, 'c'}};
		Map<bool, float> Map_bool_float = {{false, 3.14159}, {true, 137.0}};
		Map<char, float> Map_char_float = {{'a', 3.14159}, {'b', 137.0}, {'c', -0.5}};
		Map<float, float> Map_float_float = {{3.14159, 3.14159}, {137.0, 137.0}, {-0.5, -0.5}};
		Map<int, float> Map_int_float = {{1, 3.14159}, {3, 137.0}, {5, -0.5}};
		Map<string, float> Map_string_float = {{"tree", 3.14159}, {"cardinal", 137.0}, {"Stanford", -0.5}};
		Map<GridLocation, float> Map_GridLocation_float = {{{3, 14}, 3.14159}, {{18, 91}, 137.0}};
		Map<Thing, float> Map_Thing_float = {{(Thing){3, "red"}, 3.14159}, {(Thing){5, "green"}, 137.0}, {(Thing){4, "blue"}, -0.5}};
		Map<bool, int> Map_bool_int = {{false, 1}, {true, 3}};
		Map<char, int> Map_char_int = {{'a', 1}, {'b', 3}, {'c', 5}, {'A', 7}, {'B', 9}};
		Map<float, int> Map_float_int = {{3.14159, 1}, {137.0, 3}, {-0.5, 5}};
		Map<int, int> Map_int_int = {{1, 1}, {3, 3}, {5, 5}, {7, 7}, {9, 9}};
		Map<string, int> Map_string_int = {{"tree", 1}, {"cardinal", 3}, {"Stanford", 5}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 7}};
		Map<GridLocation, int> Map_GridLocation_int = {{{3, 14}, 1}, {{18, 91}, 3}};
		Map<Thing, int> Map_Thing_int = {{(Thing){3, "red"}, 1}, {(Thing){5, "green"}, 3}, {(Thing){4, "blue"}, 5}};
		Map<bool, string> Map_bool_string = {{false, "tree"}, {true, "cardinal"}};
		Map<char, string> Map_char_string = {{'a', "tree"}, {'b', "cardinal"}, {'c', "Stanford"}, {'A', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<float, string> Map_float_string = {{3.14159, "tree"}, {137.0, "cardinal"}, {-0.5, "Stanford"}};
		Map<int, string> Map_int_string = {{1, "tree"}, {3, "cardinal"}, {5, "Stanford"}, {7, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<string, string> Map_string_string = {{"tree", "tree"}, {"cardinal", "cardinal"}, {"Stanford", "Stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<GridLocation, string> Map_GridLocation_string = {{{3, 14}, "tree"}, {{18, 91}, "cardinal"}};
		Map<Thing, string> Map_Thing_string = {{(Thing){3, "red"}, "tree"}, {(Thing){5, "green"}, "cardinal"}, {(Thing){4, "blue"}, "Stanford"}};
		Map<bool, GridLocation> Map_bool_GridLocation = {{false, {3, 14}}, {true, {18, 91}}};
		Map<char, GridLocation> Map_char_GridLocation = {{'a', {3, 14}}, {'b', {18, 91}}};
		Map<float, GridLocation> Map_float_GridLocation = {{3.14159, {3, 14}}, {137.0, {18, 91}}};
		Map<int, GridLocation> Map_int_GridLocation = {{1, {3, 14}}, {3, {18, 91}}};
		Map<string, GridLocation> Map_string_GridLocation = {{"tree", {3, 14}}, {"cardinal", {18, 91}}};
		Map<GridLocation, GridLocation> Map_GridLocation_GridLocation = {{{3, 14}, {3, 14}}, {{18, 91}, {18, 91}}};
		Map<Thing, GridLocation> Map_Thing_GridLocation = {{(Thing){3, "red"}, {3, 14}}, {(Thing){5, "green"}, {18, 91}}};
		Map<bool, Thing> Map_bool_Thing = {{false, (Thing){3, "red"}}, {true, (Thing){5, "green"}}};
		Map<char, Thing> Map_char_Thing = {{'a', (Thing){3, "red"}}, {'b', (Thing){5, "green"}}, {'c', (Thing){4, "blue"}}};
		Map<float, Thing> Map_float_Thing = {{3.14159, (Thing){3, "red"}}, {137.0, (Thing){5, "green"}}, {-0.5, (Thing){4, "blue"}}};
		Map<int, Thing> Map_int_Thing = {{1, (Thing){3, "red"}}, {3, (Thing){5, "green"}}, {5, (Thing){4, "blue"}}};
		Map<string, Thing> Map_string_Thing = {{"tree", (Thing){3, "red"}}, {"cardinal", (Thing){5, "green"}}, {"Stanford", (Thing){4, "blue"}}};
		Map<GridLocation, Thing> Map_GridLocation_Thing = {{{3, 14}, (Thing){3, "red"}}, {{18, 91}, (Thing){5, "green"}}};
		Map<Thing, Thing> Map_Thing_Thing = {{(Thing){3, "red"}, (Thing){3, "red"}}, {(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){4, "blue"}, (Thing){4, "blue"}}};
		BREAKPOINT;
	}
	{
		Set<bool> Set_bool = {false, true};
		Set<char> Set_char = {'a', 'b', 'c', 'A', 'B', 'C'};
		Set<float> Set_float = {3.14159, 137.0, -0.5};
		Set<int> Set_int = {1, 3, 5, 7, 9};
		Set<string> Set_string = {"tree", "cardinal", "Stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Set<GridLocation> Set_GridLocation = {{3, 14}, {18, 91}};
		Set<Thing> Set_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
		BREAKPOINT;
	}
}

void stanford_other() {
	{
		PriorityQueue<bool> PriorityQueue_bool = {{1, false}, {3, true}};
		PriorityQueue<char> PriorityQueue_char = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'A'}, {9, 'B'}};
		PriorityQueue<float> PriorityQueue_float = {{1, 3.14159}, {3, 137.0}, {5, -0.5}};
		PriorityQueue<int> PriorityQueue_int = {{1, 1}, {3, 3}, {5, 5}, {7, 7}, {9, 9}};
		PriorityQueue<string> PriorityQueue_string = {{1, "tree"}, {3, "cardinal"}, {5, "Stanford"}, {7, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		PriorityQueue<GridLocation> PriorityQueue_GridLocation = {{1, {3, 14}}, {3, {18, 91}}};
		PriorityQueue<Thing> PriorityQueue_Thing = {{1, (Thing){3, "red"}}, {3, (Thing){5, "green"}}, {5, (Thing){4, "blue"}}};
		BREAKPOINT;
	}
}


void assign_uses() {
    Node *ptr = new Node {'A', nullptr, nullptr};
    PriorityQueue<Node *> huff_PQ_NodePtr {{4.0, ptr}};
    Vector<Node *> huff_Vector_NodePtr = {ptr, ptr, nullptr, nullptr};
    Set<Node *> huff_Set_NodePtr = {ptr, nullptr};
    Map<char, Node *> huff_Map_NodePtr = {{'a', ptr}, {'b', ptr}, {'c', nullptr}};

    Vector<Bit> huff_Vector_Bit = {0, 1, 1, 1, 0};
    Map<char, Queue<Bit>> huff_Map_Queue_Bit {{'a', {0, 1, 1, 1}}, {'b', {1, 0}}};
    Map<char, Vector<Bit>> huff_Map_Vector_Bit = {{'a', {1, 1, 1, 0}}, {'b', {0, 0}}};
    Map<string, Set<string>> siteIndex_Map_Set =  {{"home", {"the", "end"}}, {"faq", {"what", "why"}}};

    Grid<char> boggle_Grid = {{'a', 'n', 'r'}, {'p', 'e', 't'}, {'c', 'o', 'b'}};
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
