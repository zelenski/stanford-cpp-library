#pragma once
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

struct Thing {
    int num;
    std::string label;
};
int hashCode(const Thing& t);
bool operator ==(const Thing& t1, const Thing& t2);
bool operator <(const Thing& t1, const Thing& t2);

struct Node {
    char letter;
    Node *left, *right;
};

#if defined(__APPLE__)
#define BREAKPOINT __builtin_debugtrap(); cout << "BREAK @" << __LINE__ << endl;
#elif defined(_WIN32)
#define BREAKPOINT __debugbreak(); cout << "BREAK @" << __LINE__ << endl;
#else
raise(SIGTRAP); cout << "BREAK @" << __LINE__ << endl;
#endif