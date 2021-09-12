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
using namespace std;

int main() {
	Deque<bool> Deque_bool = {false, true};
	Deque<char> Deque_char = {'a', 'b', 'c'};
	Deque<float> Deque_float = {1e-05, 1.37, 3.14159};
	Deque<int> Deque_int = {0, 1, 2};
	Deque<string> Deque_string = {"bananas", "ice cream", "nachos"};
	Grid<bool> Grid_bool = {{false, true}, {false, true}, {false, true}};
	Grid<char> Grid_char = {{'a', 'b', 'c'}, {'a', 'b', 'c'}, {'a', 'b', 'c'}};
	Grid<float> Grid_float = {{1e-05, 1.37, 3.14159}, {1e-05, 1.37, 3.14159}, {1e-05, 1.37, 3.14159}};
	Grid<int> Grid_int = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	Grid<string> Grid_string = {{"bananas", "ice cream", "nachos"}, {"bananas", "ice cream", "nachos"}, {"bananas", "ice cream", "nachos"}};
	HashMap<bool, bool> HashMap_bool_bool = {{false, false}, {true, true}};
	HashMap<char, bool> HashMap_char_bool = {{'a', false}, {'b', true}};
	HashMap<float, bool> HashMap_float_bool = {{1e-05, false}, {1.37, true}};
	HashMap<int, bool> HashMap_int_bool = {{0, false}, {1, true}};
	HashMap<string, bool> HashMap_string_bool = {{"bananas", false}, {"ice cream", true}};
	HashMap<bool, char> HashMap_bool_char = {{false, 'a'}, {true, 'b'}};
	HashMap<char, char> HashMap_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}};
	HashMap<float, char> HashMap_float_char = {{1e-05, 'a'}, {1.37, 'b'}, {3.14159, 'c'}};
	HashMap<int, char> HashMap_int_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	HashMap<string, char> HashMap_string_char = {{"bananas", 'a'}, {"ice cream", 'b'}, {"nachos", 'c'}};
	HashMap<bool, float> HashMap_bool_float = {{false, 1e-05}, {true, 1.37}};
	HashMap<char, float> HashMap_char_float = {{'a', 1e-05}, {'b', 1.37}, {'c', 3.14159}};
	HashMap<float, float> HashMap_float_float = {{1e-05, 1e-05}, {1.37, 1.37}, {3.14159, 3.14159}};
	HashMap<int, float> HashMap_int_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	HashMap<string, float> HashMap_string_float = {{"bananas", 1e-05}, {"ice cream", 1.37}, {"nachos", 3.14159}};
	HashMap<bool, int> HashMap_bool_int = {{false, 0}, {true, 1}};
	HashMap<char, int> HashMap_char_int = {{'a', 0}, {'b', 1}, {'c', 2}};
	HashMap<float, int> HashMap_float_int = {{1e-05, 0}, {1.37, 1}, {3.14159, 2}};
	HashMap<int, int> HashMap_int_int = {{0, 0}, {1, 1}, {2, 2}};
	HashMap<string, int> HashMap_string_int = {{"bananas", 0}, {"ice cream", 1}, {"nachos", 2}};
	HashMap<bool, string> HashMap_bool_string = {{false, "bananas"}, {true, "ice cream"}};
	HashMap<char, string> HashMap_char_string = {{'a', "bananas"}, {'b', "ice cream"}, {'c', "nachos"}};
	HashMap<float, string> HashMap_float_string = {{1e-05, "bananas"}, {1.37, "ice cream"}, {3.14159, "nachos"}};
	HashMap<int, string> HashMap_int_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	HashMap<string, string> HashMap_string_string = {{"bananas", "bananas"}, {"ice cream", "ice cream"}, {"nachos", "nachos"}};
	HashSet<bool> HashSet_bool = {false, true};
	HashSet<char> HashSet_char = {'a', 'b', 'c'};
	HashSet<float> HashSet_float = {1e-05, 1.37, 3.14159};
	HashSet<int> HashSet_int = {0, 1, 2};
	HashSet<string> HashSet_string = {"bananas", "ice cream", "nachos"};
	Map<bool, bool> Map_bool_bool = {{false, false}, {true, true}};
	Map<char, bool> Map_char_bool = {{'a', false}, {'b', true}};
	Map<float, bool> Map_float_bool = {{1e-05, false}, {1.37, true}};
	Map<int, bool> Map_int_bool = {{0, false}, {1, true}};
	Map<string, bool> Map_string_bool = {{"bananas", false}, {"ice cream", true}};
	Map<bool, char> Map_bool_char = {{false, 'a'}, {true, 'b'}};
	Map<char, char> Map_char_char = {{'a', 'a'}, {'b', 'b'}, {'c', 'c'}};
	Map<float, char> Map_float_char = {{1e-05, 'a'}, {1.37, 'b'}, {3.14159, 'c'}};
	Map<int, char> Map_int_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	Map<string, char> Map_string_char = {{"bananas", 'a'}, {"ice cream", 'b'}, {"nachos", 'c'}};
	Map<bool, float> Map_bool_float = {{false, 1e-05}, {true, 1.37}};
	Map<char, float> Map_char_float = {{'a', 1e-05}, {'b', 1.37}, {'c', 3.14159}};
	Map<float, float> Map_float_float = {{1e-05, 1e-05}, {1.37, 1.37}, {3.14159, 3.14159}};
	Map<int, float> Map_int_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	Map<string, float> Map_string_float = {{"bananas", 1e-05}, {"ice cream", 1.37}, {"nachos", 3.14159}};
	Map<bool, int> Map_bool_int = {{false, 0}, {true, 1}};
	Map<char, int> Map_char_int = {{'a', 0}, {'b', 1}, {'c', 2}};
	Map<float, int> Map_float_int = {{1e-05, 0}, {1.37, 1}, {3.14159, 2}};
	Map<int, int> Map_int_int = {{0, 0}, {1, 1}, {2, 2}};
	Map<string, int> Map_string_int = {{"bananas", 0}, {"ice cream", 1}, {"nachos", 2}};
	Map<bool, string> Map_bool_string = {{false, "bananas"}, {true, "ice cream"}};
	Map<char, string> Map_char_string = {{'a', "bananas"}, {'b', "ice cream"}, {'c', "nachos"}};
	Map<float, string> Map_float_string = {{1e-05, "bananas"}, {1.37, "ice cream"}, {3.14159, "nachos"}};
	Map<int, string> Map_int_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	Map<string, string> Map_string_string = {{"bananas", "bananas"}, {"ice cream", "ice cream"}, {"nachos", "nachos"}};
	PriorityQueue<bool> PriorityQueue_bool = {{0, false}, {1, true}};
	PriorityQueue<char> PriorityQueue_char = {{0, 'a'}, {1, 'b'}, {2, 'c'}};
	PriorityQueue<float> PriorityQueue_float = {{0, 1e-05}, {1, 1.37}, {2, 3.14159}};
	PriorityQueue<int> PriorityQueue_int = {{0, 0}, {1, 1}, {2, 2}};
	PriorityQueue<string> PriorityQueue_string = {{0, "bananas"}, {1, "ice cream"}, {2, "nachos"}};
	Queue<bool> Queue_bool = {false, true};
	Queue<char> Queue_char = {'a', 'b', 'c'};
	Queue<float> Queue_float = {1e-05, 1.37, 3.14159};
	Queue<int> Queue_int = {0, 1, 2};
	Queue<string> Queue_string = {"bananas", "ice cream", "nachos"};
	Set<bool> Set_bool = {false, true};
	Set<char> Set_char = {'a', 'b', 'c'};
	Set<float> Set_float = {1e-05, 1.37, 3.14159};
	Set<int> Set_int = {0, 1, 2};
	Set<string> Set_string = {"bananas", "ice cream", "nachos"};
	Stack<bool> Stack_bool = {false, true};
	Stack<char> Stack_char = {'a', 'b', 'c'};
	Stack<float> Stack_float = {1e-05, 1.37, 3.14159};
	Stack<int> Stack_int = {0, 1, 2};
	Stack<string> Stack_string = {"bananas", "ice cream", "nachos"};
	Vector<bool> Vector_bool = {false, true};
	Vector<char> Vector_char = {'a', 'b', 'c'};
	Vector<float> Vector_float = {1e-05, 1.37, 3.14159};
	Vector<int> Vector_int = {0, 1, 2};
	Vector<string> Vector_string = {"bananas", "ice cream", "nachos"};

    struct Node {
        char letter;
        Node *left, *right;
    };

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



	return 0;
}
