#include "testing/SimpleTest.h"
#include "stack.h"
#include "queue.h"
#include "map.h"
using namespace std;

// This function correctly reverses the elements of a queue
void reverse(Queue<int>& q)
{
    Stack<int> s;
    while (!q.isEmpty()) {
        int val = q.dequeue();
        s.push(val);
    }
    while (!s.isEmpty()) {
        int val = s.pop();
        q.enqueue(val);
    }
}

// This function is intended to modify a queue of characters to duplicate
// any negative numbers.
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
void stutterNegative(Queue<int>& q)
{
    int sz = q.size();
    for (int i = 0; i < sz; i++) {
        int cur = q.dequeue();
        q.enqueue(cur);
        if (cur < 0) {
            q.enqueue(cur);   // double up on negative numbers
        }
    }
}

// This function is intended to remove key/value pairs from
// map where key == value
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
void removeMatchPairs(Map<string, string>& map)
{
    for (string key: map) {
        if (map[key] == key) {
            map.remove(key);
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> a = {1, 2, 3, 4, 5};
    Queue<int> b = {5, 4, 3, 2, 1};

    reverse(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("stutterNegative, input has no negatives") {
    Queue<int> a = {2, 10};
    Queue<int> b = a;

    stutterNegative(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("stutterNegative, input has single negative") {
    Queue<int> a = {-6, 7};
    Queue<int> b = {-6, -6, 7};

    stutterNegative(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("stutterNegative, input has some negatives") {
    Queue<int> a = {-3, 4, -5, 10};
    Queue<int> b = {-3, -3, 4, -5, -5, 10};

    stutterNegative(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, no change") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Margaret", "Meg"}};
    Map<string, string> b = a;

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, remove one") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Jan", "Jan"}, {"Margaret", "Meg"}};
    Map<string, string> b = {{"Thomas", "Tom"},  {"Margaret", "Meg"}};

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}

