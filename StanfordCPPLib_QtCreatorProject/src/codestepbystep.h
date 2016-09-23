/*
 * ...
 */

#ifndef _codestepbystep_h
#define _codestepbystep_h

// basic standard library stuff
#include <cctype>
#include <cmath>
#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// STL
#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

// Stanford lib stuff
#include "strlib.h"
#include "ArrayIntList.h"
#include "basicgraph.h"
#include "BinaryTree.h"
#include "HashTableMap.h"
#include "HashTableSet.h"
#include "HeapPriorityQueue.h"
#include "LinkedIntList.h"

namespace CodeStepByStep {

std::string testToRun();
bool runAllTests();

std::string htmlDecode(const std::string& s);
std::string htmlEncode(const std::string& s);

template <typename T>
std::string htmlEncode(const T& obj) {
    std::ostringstream out;
    out << obj;
    std::string s = out.str();
    return ::htmlEncode(s);
}

std::ofstream& __getXmlOut();

void printXml(const std::string& s);

void printlnXml(const std::string& s);

void printXml(bool b);

void printlnXml(bool b);

template <typename T>
void printXml(const T& value) {
    __getXmlOut() << value;
}

template <typename T>
void printlnXml(const T& value) {
    __getXmlOut() << value << std::endl;
}

std::string __stackTraceToString();

void __printException(const std::string& type, const std::string& message = "",
                      const std::string& stacktrace = "", int lineNumber = -1);

void __openXmlOldWay(const std::string& filename);

void __closeXml();

void __printXml(const char* s);

void __printXml(const std::string& s);

void __printlnXml(const char* s);

void __printlnXml(const std::string& s);

// CODE FOR HANDLING SIGNALS (VARIOUS PROGRAM CRASHES)
#define SIGSTACK ((int) 0xdeadbeef)
#define SIGUNKNOWN ((int) 0xcafebabe)
#define SIGTIMEOUT ((int) 0xf00df00d)

void __codeStepByStepSignalHandler(int sig);
void __posixSignalHandler(int sig, siginfo_t* /*siginfo*/, void* /*context*/);

// functions to parse various collections from strings
void ArrayIntList_fromString(ArrayIntList& list, const std::string& str);
void BasicGraph_fromString(BasicGraph& graph, const std::string& str);
void BinaryTree_fromString(BinaryTree& tree, const std::string& str);
void BinaryTreeNode_fromString(BinaryTreeNode*& root, const std::string& str);
void BinaryTreeNodeptr_fromString(BinaryTreeNode*& root, const std::string& str);
void HeapPriorityQueue_fromString(HeapPriorityQueue& pqueue, const std::string& str);
void LinkedIntList_fromString(LinkedIntList& list, const std::string& str);
void ListNode_fromString(ListNode*& ptr, const std::string& str);
void ListNodeptr_fromString(ListNode*& ptr, const std::string& str);

// this one's body must be here in .h file because it's a template
template <typename T>
void Vector_fromString(Vector<T>& v, const std::string& str) {
    std::istringstream input(str);
    input >> v;
}

// helpers for parts of main() function
void main_begin(int argc, char** argv);
void main_end();

} // namespace CodeStepByStep

#endif // _codestepbystep_h
