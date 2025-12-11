#include "shared.h"
using namespace std;

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

