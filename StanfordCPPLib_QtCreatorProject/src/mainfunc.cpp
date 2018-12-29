/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include "console.h"
#include "map.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"
#include "strlib.h"
using namespace std;

int multimain(collectionTest) {
    // empty?
    Stack<int> s;
    s.push(46);
    Queue<int> q;
    q.enqueue(43);
    Map<int, int> m;
    m.put(42, 43);

    Vector<int> v2;
    v2.add(42);
    // std::vector<int> v;

    cout << "v is " << to_string(v2) << endl;


    return 0;
}

int main() {
    return 0;
}
