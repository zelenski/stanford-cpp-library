/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include "map.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"

int main() {
    // empty?
    Stack<int> s;
    s.push(42);
    Queue<int> q;
    q.enqueue(43);
    Map<int, int> m;
    m.put(42, 43);

    Vector<int> v2;
    v2.add(42);
    // std::vector<int> v;

    return 0;
}
