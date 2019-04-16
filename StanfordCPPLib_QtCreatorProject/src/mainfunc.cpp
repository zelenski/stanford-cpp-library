/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include "console.h"
#include "filelib.h"
#include "map.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"
#include "strlib.h"
using namespace std;

int multimain(collections) {
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

    Vector<string> files = listDirectory(".");
    for (string file : files) {
        cout << "a file: " << file << endl;
    }


    return 0;
}

// this is only here so the project will compile
int main() {
    return 0;
}
