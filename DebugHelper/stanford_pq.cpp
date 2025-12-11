#include "shared.h"
using namespace std;

void stanford_pq() {
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

