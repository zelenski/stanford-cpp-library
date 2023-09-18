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
#include "bits.h"
using namespace std;


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

int main() {
	Deque<bool> Deque_bool = {false, true};
	Deque<char> Deque_char = {'a', 'b', 'c'};
	Deque<float> Deque_float = {1e-05, 1.37, 3.14159};
	Deque<int> Deque_int = {0, 1, 2};
	Deque<string> Deque_string = {"bananas", "ice cream", "nachos"};
	Deque<GridLocation> Deque_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	Deque<Thing> Deque_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
	Grid<bool> Grid_bool = {{false, true}, {false, true}, {false, true}};
	Grid<char> Grid_char = {{'a', 'b', 'c'}, {'a', 'b', 'c'}, {'a', 'b', 'c'}};
	Grid<float> Grid_float = {{1e-05, 1.37, 3.14159}, {1e-05, 1.37, 3.14159}, {1e-05, 1.37, 3.14159}};
	Grid<int> Grid_int = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	Grid<string> Grid_string = {{"bananas", "ice cream", "nachos"}, {"bananas", "ice cream", "nachos"}, {"bananas", "ice cream", "nachos"}};
	Grid<GridLocation> Grid_GridLocation = {{{0, 0}, {2, 4}, {5, 3}}, {{0, 0}, {2, 4}, {5, 3}}, {{0, 0}, {2, 4}, {5, 3}}};
	Grid<Thing> Grid_Thing = {{(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}, {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}, {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}}};
	HashMap<bool, bool> HashMap_bool_bool = {{false, false}, {true, true}};
	HashMap<char, bool> HashMap_char_bool = {{'a', false}, {'b', true}};
	HashMap<float, bool> HashMap_float_bool = {{1e-05, false}, {1.37, true}};
	HashMap<int, bool> HashMap_int_bool = {{0, false}, {1, true}};
	HashMap<string, bool> HashMap_string_bool = {{"bananas", false}, {"ice cream", true}};
	HashMap<GridLocation, bool> HashMap_GridLocation_bool = {{{0, 0}, false}, {{2, 4}, true}};
	HashMap<Thing, bool> HashMap_Thing_bool = {{(Thing){3, "red"}, false}, {(Thing){5, "green"}, true}};
	HashMap<bool, char> HashMap_bool_char = {{false, 'a'}, {true, 'b'}};
	HashMap<char, char> HashMap_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}};
	HashMap<float, char> HashMap_float_char = {{1e-05, 'a'}, {1.37, 'b'}, {3.14159, 'c'}};
	HashMap<int, char> HashMap_int_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	HashMap<string, char> HashMap_string_char = {{"bananas", 'a'}, {"ice cream", 'b'}, {"nachos", 'c'}};
	HashMap<GridLocation, char> HashMap_GridLocation_char = {{{0, 0}, 'a'}, {{2, 4}, 'b'}, {{5, 3}, 'c'}};
	HashMap<Thing, char> HashMap_Thing_char = {{(Thing){3, "red"}, 'a'}, {(Thing){5, "green"}, 'b'}, {(Thing){4, "blue"}, 'c'}};
	HashMap<bool, float> HashMap_bool_float = {{false, 1e-05}, {true, 1.37}};
	HashMap<char, float> HashMap_char_float = {{'a', 1e-05}, {'b', 1.37}, {'c', 3.14159}};
	HashMap<float, float> HashMap_float_float = {{1e-05, 1e-05}, {1.37, 1.37}, {3.14159, 3.14159}};
	HashMap<int, float> HashMap_int_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	HashMap<string, float> HashMap_string_float = {{"bananas", 1e-05}, {"ice cream", 1.37}, {"nachos", 3.14159}};
	HashMap<GridLocation, float> HashMap_GridLocation_float = {{{0, 0}, 1e-05}, {{2, 4}, 1.37}, {{5, 3}, 3.14159}};
	HashMap<Thing, float> HashMap_Thing_float = {{(Thing){3, "red"}, 1e-05}, {(Thing){5, "green"}, 1.37}, {(Thing){4, "blue"}, 3.14159}};
	HashMap<bool, int> HashMap_bool_int = {{false, 0}, {true, 1}};
	HashMap<char, int> HashMap_char_int = {{'a', 0}, {'b', 1}, {'c', 2}};
	HashMap<float, int> HashMap_float_int = {{1e-05, 0}, {1.37, 1}, {3.14159, 2}};
	HashMap<int, int> HashMap_int_int = {{0, 0}, {1, 1}, {2, 2}};
	HashMap<string, int> HashMap_string_int = {{"bananas", 0}, {"ice cream", 1}, {"nachos", 2}};
	HashMap<GridLocation, int> HashMap_GridLocation_int = {{{0, 0}, 0}, {{2, 4}, 1}, {{5, 3}, 2}};
	HashMap<Thing, int> HashMap_Thing_int = {{(Thing){3, "red"}, 0}, {(Thing){5, "green"}, 1}, {(Thing){4, "blue"}, 2}};
	HashMap<bool, string> HashMap_bool_string = {{false, "bananas"}, {true, "ice cream"}};
	HashMap<char, string> HashMap_char_string = {{'a', "bananas"}, {'b', "ice cream"}, {'c', "nachos"}};
	HashMap<float, string> HashMap_float_string = {{1e-05, "bananas"}, {1.37, "ice cream"}, {3.14159, "nachos"}};
	HashMap<int, string> HashMap_int_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	HashMap<string, string> HashMap_string_string = {{"bananas", "bananas"}, {"ice cream", "ice cream"}, {"nachos", "nachos"}};
	HashMap<GridLocation, string> HashMap_GridLocation_string = {{{0, 0}, "bananas"}, {{2, 4}, "ice cream"}, {{5, 3}, "nachos"}};
	HashMap<Thing, string> HashMap_Thing_string = {{(Thing){3, "red"}, "bananas"}, {(Thing){5, "green"}, "ice cream"}, {(Thing){4, "blue"}, "nachos"}};
	HashMap<bool, GridLocation> HashMap_bool_GridLocation = {{false, {0, 0}}, {true, {2, 4}}};
	HashMap<char, GridLocation> HashMap_char_GridLocation = {{'a', {0, 0}}, {'b', {2, 4}}, {'c', {5, 3}}};
	HashMap<float, GridLocation> HashMap_float_GridLocation = {{1e-05, {0, 0}}, {1.37, {2, 4}}, {3.14159, {5, 3}}};
	HashMap<int, GridLocation> HashMap_int_GridLocation = {{0, {0, 0}}, {1, {2, 4}}, {2, {5, 3}}};
	HashMap<string, GridLocation> HashMap_string_GridLocation = {{"bananas", {0, 0}}, {"ice cream", {2, 4}}, {"nachos", {5, 3}}};
	HashMap<GridLocation, GridLocation> HashMap_GridLocation_GridLocation = {{{0, 0}, {0, 0}}, {{2, 4}, {2, 4}}, {{5, 3}, {5, 3}}};
	HashMap<Thing, GridLocation> HashMap_Thing_GridLocation = {{(Thing){3, "red"}, {0, 0}}, {(Thing){5, "green"}, {2, 4}}, {(Thing){4, "blue"}, {5, 3}}};
	HashMap<bool, Thing> HashMap_bool_Thing = {{false, (Thing){3, "red"}}, {true, (Thing){5, "green"}}};
	HashMap<char, Thing> HashMap_char_Thing = {{'a', (Thing){3, "red"}}, {'b', (Thing){5, "green"}}, {'c', (Thing){4, "blue"}}};
	HashMap<float, Thing> HashMap_float_Thing = {{1e-05, (Thing){3, "red"}}, {1.37, (Thing){5, "green"}}, {3.14159, (Thing){4, "blue"}}};
	HashMap<int, Thing> HashMap_int_Thing = {{0, (Thing){3, "red"}}, {1, (Thing){5, "green"}}, {2, (Thing){4, "blue"}}};
	HashMap<string, Thing> HashMap_string_Thing = {{"bananas", (Thing){3, "red"}}, {"ice cream", (Thing){5, "green"}}, {"nachos", (Thing){4, "blue"}}};
	HashMap<GridLocation, Thing> HashMap_GridLocation_Thing = {{{0, 0}, (Thing){3, "red"}}, {{2, 4}, (Thing){5, "green"}}, {{5, 3}, (Thing){4, "blue"}}};
	HashMap<Thing, Thing> HashMap_Thing_Thing = {{(Thing){3, "red"}, (Thing){3, "red"}}, {(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){4, "blue"}, (Thing){4, "blue"}}};
	HashSet<bool> HashSet_bool = {false, true};
	HashSet<char> HashSet_char = {'a', 'b', 'c'};
	HashSet<float> HashSet_float = {1e-05, 1.37, 3.14159};
	HashSet<int> HashSet_int = {0, 1, 2};
	HashSet<string> HashSet_string = {"bananas", "ice cream", "nachos"};
	HashSet<GridLocation> HashSet_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	HashSet<Thing> HashSet_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
	Map<bool, bool> Map_bool_bool = {{false, false}, {true, true}};
	Map<char, bool> Map_char_bool = {{'a', false}, {'b', true}};
	Map<float, bool> Map_float_bool = {{1e-05, false}, {1.37, true}};
	Map<int, bool> Map_int_bool = {{0, false}, {1, true}};
	Map<string, bool> Map_string_bool = {{"bananas", false}, {"ice cream", true}};
	Map<GridLocation, bool> Map_GridLocation_bool = {{{0, 0}, false}, {{2, 4}, true}};
	Map<Thing, bool> Map_Thing_bool = {{(Thing){3, "red"}, false}, {(Thing){5, "green"}, true}};
	Map<bool, char> Map_bool_char = {{false, 'a'}, {true, 'b'}};
	Map<char, char> Map_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}};
	Map<float, char> Map_float_char = {{1e-05, 'a'}, {1.37, 'b'}, {3.14159, 'c'}};
	Map<int, char> Map_int_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	Map<string, char> Map_string_char = {{"bananas", 'a'}, {"ice cream", 'b'}, {"nachos", 'c'}};
	Map<GridLocation, char> Map_GridLocation_char = {{{0, 0}, 'a'}, {{2, 4}, 'b'}, {{5, 3}, 'c'}};
	Map<Thing, char> Map_Thing_char = {{(Thing){3, "red"}, 'a'}, {(Thing){5, "green"}, 'b'}, {(Thing){4, "blue"}, 'c'}};
	Map<bool, float> Map_bool_float = {{false, 1e-05}, {true, 1.37}};
	Map<char, float> Map_char_float = {{'a', 1e-05}, {'b', 1.37}, {'c', 3.14159}};
	Map<float, float> Map_float_float = {{1e-05, 1e-05}, {1.37, 1.37}, {3.14159, 3.14159}};
	Map<int, float> Map_int_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	Map<string, float> Map_string_float = {{"bananas", 1e-05}, {"ice cream", 1.37}, {"nachos", 3.14159}};
	Map<GridLocation, float> Map_GridLocation_float = {{{0, 0}, 1e-05}, {{2, 4}, 1.37}, {{5, 3}, 3.14159}};
	Map<Thing, float> Map_Thing_float = {{(Thing){3, "red"}, 1e-05}, {(Thing){5, "green"}, 1.37}, {(Thing){4, "blue"}, 3.14159}};
	Map<bool, int> Map_bool_int = {{false, 0}, {true, 1}};
	Map<char, int> Map_char_int = {{'a', 0}, {'b', 1}, {'c', 2}};
	Map<float, int> Map_float_int = {{1e-05, 0}, {1.37, 1}, {3.14159, 2}};
	Map<int, int> Map_int_int = {{0, 0}, {1, 1}, {2, 2}};
	Map<string, int> Map_string_int = {{"bananas", 0}, {"ice cream", 1}, {"nachos", 2}};
	Map<GridLocation, int> Map_GridLocation_int = {{{0, 0}, 0}, {{2, 4}, 1}, {{5, 3}, 2}};
	Map<Thing, int> Map_Thing_int = {{(Thing){3, "red"}, 0}, {(Thing){5, "green"}, 1}, {(Thing){4, "blue"}, 2}};
	Map<bool, string> Map_bool_string = {{false, "bananas"}, {true, "ice cream"}};
	Map<char, string> Map_char_string = {{'a', "bananas"}, {'b', "ice cream"}, {'c', "nachos"}};
	Map<float, string> Map_float_string = {{1e-05, "bananas"}, {1.37, "ice cream"}, {3.14159, "nachos"}};
	Map<int, string> Map_int_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	Map<string, string> Map_string_string = {{"bananas", "bananas"}, {"ice cream", "ice cream"}, {"nachos", "nachos"}};
	Map<GridLocation, string> Map_GridLocation_string = {{{0, 0}, "bananas"}, {{2, 4}, "ice cream"}, {{5, 3}, "nachos"}};
	Map<Thing, string> Map_Thing_string = {{(Thing){3, "red"}, "bananas"}, {(Thing){5, "green"}, "ice cream"}, {(Thing){4, "blue"}, "nachos"}};
	Map<bool, GridLocation> Map_bool_GridLocation = {{false, {0, 0}}, {true, {2, 4}}};
	Map<char, GridLocation> Map_char_GridLocation = {{'a', {0, 0}}, {'b', {2, 4}}, {'c', {5, 3}}};
	Map<float, GridLocation> Map_float_GridLocation = {{1e-05, {0, 0}}, {1.37, {2, 4}}, {3.14159, {5, 3}}};
	Map<int, GridLocation> Map_int_GridLocation = {{0, {0, 0}}, {1, {2, 4}}, {2, {5, 3}}};
	Map<string, GridLocation> Map_string_GridLocation = {{"bananas", {0, 0}}, {"ice cream", {2, 4}}, {"nachos", {5, 3}}};
	Map<GridLocation, GridLocation> Map_GridLocation_GridLocation = {{{0, 0}, {0, 0}}, {{2, 4}, {2, 4}}, {{5, 3}, {5, 3}}};
	Map<Thing, GridLocation> Map_Thing_GridLocation = {{(Thing){3, "red"}, {0, 0}}, {(Thing){5, "green"}, {2, 4}}, {(Thing){4, "blue"}, {5, 3}}};
	Map<bool, Thing> Map_bool_Thing = {{false, (Thing){3, "red"}}, {true, (Thing){5, "green"}}};
	Map<char, Thing> Map_char_Thing = {{'a', (Thing){3, "red"}}, {'b', (Thing){5, "green"}}, {'c', (Thing){4, "blue"}}};
	Map<float, Thing> Map_float_Thing = {{1e-05, (Thing){3, "red"}}, {1.37, (Thing){5, "green"}}, {3.14159, (Thing){4, "blue"}}};
	Map<int, Thing> Map_int_Thing = {{0, (Thing){3, "red"}}, {1, (Thing){5, "green"}}, {2, (Thing){4, "blue"}}};
	Map<string, Thing> Map_string_Thing = {{"bananas", (Thing){3, "red"}}, {"ice cream", (Thing){5, "green"}}, {"nachos", (Thing){4, "blue"}}};
	Map<GridLocation, Thing> Map_GridLocation_Thing = {{{0, 0}, (Thing){3, "red"}}, {{2, 4}, (Thing){5, "green"}}, {{5, 3}, (Thing){4, "blue"}}};
	Map<Thing, Thing> Map_Thing_Thing = {{(Thing){3, "red"}, (Thing){3, "red"}}, {(Thing){5, "green"}, (Thing){5, "green"}}, {(Thing){4, "blue"}, (Thing){4, "blue"}}};
	PriorityQueue<bool> PriorityQueue_bool = {{0, false}, {1, true}};
	PriorityQueue<char> PriorityQueue_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	PriorityQueue<float> PriorityQueue_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	PriorityQueue<int> PriorityQueue_int = {{0, 0}, {1, 1}, {2, 2}};
	PriorityQueue<string> PriorityQueue_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	PriorityQueue<GridLocation> PriorityQueue_GridLocation = {{0, {0, 0}}, {1, {2, 4}}, {2, {5, 3}}};
	PriorityQueue<Thing> PriorityQueue_Thing = {{0, (Thing){3, "red"}}, {1, (Thing){5, "green"}}, {2, (Thing){4, "blue"}}};
	Queue<bool> Queue_bool = {false, true};
	Queue<char> Queue_char = {'a', 'b', 'c'};
	Queue<float> Queue_float = {1e-05, 1.37, 3.14159};
	Queue<int> Queue_int = {0, 1, 2};
	Queue<string> Queue_string = {"bananas", "ice cream", "nachos"};
	Queue<GridLocation> Queue_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	Queue<Thing> Queue_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
	Set<bool> Set_bool = {false, true};
	Set<char> Set_char = {'a', 'b', 'c'};
	Set<float> Set_float = {1e-05, 1.37, 3.14159};
	Set<int> Set_int = {0, 1, 2};
	Set<string> Set_string = {"bananas", "ice cream", "nachos"};
	Set<GridLocation> Set_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	Set<Thing> Set_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
	Stack<bool> Stack_bool = {false, true};
	Stack<char> Stack_char = {'a', 'b', 'c'};
	Stack<float> Stack_float = {1e-05, 1.37, 3.14159};
	Stack<int> Stack_int = {0, 1, 2};
	Stack<string> Stack_string = {"bananas", "ice cream", "nachos"};
	Stack<GridLocation> Stack_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	Stack<Thing> Stack_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};
	Vector<bool> Vector_bool = {false, true};
	Vector<char> Vector_char = {'a', 'b', 'c'};
	Vector<float> Vector_float = {1e-05, 1.37, 3.14159};
	Vector<int> Vector_int = {0, 1, 2};
	Vector<string> Vector_string = {"bananas", "ice cream", "nachos"};
	Vector<GridLocation> Vector_GridLocation = {{0, 0}, {2, 4}, {5, 3}};
	Vector<Thing> Vector_Thing = {(Thing){3, "red"}, (Thing){5, "green"}, (Thing){4, "blue"}};

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
    Stack<GridLocation> maze_Stack_Loc = { {1, 2}, {1, 3}, {2, 3}, {2, 4}};


	return 0;
}
