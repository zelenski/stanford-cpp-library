/*
 * @version 2019/10/12
 * - add arrayToString template
 * @version 2019/05/17
 * - add AssassinNode support
 * @version 2019/04/24
 * - add setFromFile
 * - remove STL includes
 * @version 2017/10/06
 * - hid POSIX signal handler behind preprocessor macro
 * @version 2016/12/07
 * - added assert* methods
 */

#ifndef _codestepbystep_h
#define _codestepbystep_h

// basic standard library stuff
#include <cctype>
#include <climits>
#include <cmath>
#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//// STL
//#include <algorithm>
//#include <deque>
//#include <list>
//#include <map>
//#include <queue>
//#include <set>
//#include <stack>
//#include <vector>

// Stanford lib stuff
#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "ArrayIntList.h"
#define INTERNAL_INCLUDE 1
#include "basicgraph.h"
#define INTERNAL_INCLUDE 1
#include "BinaryTree.h"
#define INTERNAL_INCLUDE 1
#include "HashTableMap.h"
#define INTERNAL_INCLUDE 1
#include "HashTableSet.h"
#define INTERNAL_INCLUDE 1
#include "HeapPriorityQueue.h"
#define INTERNAL_INCLUDE 1
#include "LinkedIntList.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#undef INTERNAL_INCLUDE

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

void printXml(double d);

void printlnXml(double d);

void printXml(float f);

void printlnXml(float f);

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
// #define SIGSTACK ((int) 0xdeadbeef)
// #define SIGUNKNOWN ((int) 0xcafebabe)
// #define SIGTIMEOUT ((int) 0xf00df00d)

void __codeStepByStepSignalHandler(int sig);
#if !defined(_WIN32)
void __posixSignalHandler(int sig, siginfo_t* /*siginfo*/, void* /*context*/);
#endif // !defined(_WIN32)

// function to convert an array to string for printing
template <typename T>
std::string arrayToString(T a[], int length) {
    std::ostringstream out;
    out << "{";
    if (length > 0) {
        writeGenericValue(out, a[0], /* forceQuotes */ true);
        for (int i = 1; i < length; i++) {
            out << ", ";
            writeGenericValue(out, a[i], /* forceQuotes */ true);
        }
    }
    out << "}";
    return out.str();
}

// functions to parse various collections from strings
void ArrayIntList_fromString(ArrayIntList& list, const std::string& str);
void AssassinNode_fromString(AssassinNode*& ptr, const std::string& str);
void BasicGraph_fromString(BasicGraph& graph, const std::string& str);
void BinaryTree_fromString(BinaryTree& tree, const std::string& str);
void BinaryTreeNode_fromString(BinaryTreeNode*& root, const std::string& str);
void BinaryTreeNodeptr_fromString(BinaryTreeNode*& root, const std::string& str);
void BinaryTreeNodeChar_fromString(BinaryTreeNodeChar*& root, const std::string& str);
void BinaryTreeNodeCharptr_fromString(BinaryTreeNodeChar*& root, const std::string& str);
void BinaryTreeNodeDouble_fromString(BinaryTreeNodeDouble*& root, const std::string& str);
void BinaryTreeNodeString_fromString(BinaryTreeNodeString*& root, const std::string& str);
void HeapPriorityQueue_fromString(HeapPriorityQueue& pqueue, const std::string& str);
void LinkedIntList_fromString(LinkedIntList& list, const std::string& str);
void ListNode_fromString(ListNode*& ptr, const std::string& str);
void ListNodeptr_fromString(ListNode*& ptr, const std::string& str);
void ListNodeDouble_fromString(ListNodeDouble*& ptr, const std::string& str);
void ListNodeDoubleptr_fromString(ListNodeDouble*& ptr, const std::string& str);
void ListNodeString_fromString(ListNodeString*& ptr, const std::string& str);
void ListNodeStringptr_fromString(ListNodeString*& ptr, const std::string& str);

// this one's body must be here in .h file because it's a template
template <typename T>
void Vector_fromString(Vector<T>& v, const std::string& str) {
    std::istringstream input(str);
    input >> v;
}

// build a set of words from a file, a la Lexicon
// (we use this to initialize a set of words from a file in one line)
Set<string> setFromFile(const std::string& filename, bool cache = true);

// helpers for parts of main() function
void main_begin(int argc, char** argv);
void main_end();

// code for assertions (used in classes+objects problems)
#ifndef _codestepbystep_assertions
#define _codestepbystep_assertions
namespace Assertions {
void assertionPrint(const std::string& msg,
                    const std::string& assertType,
                    const std::string& valueType,
                    const std::string& expected,
                    const std::string& actual,
                    Map<std::string, std::string> attrs = {});

template <typename T>
void assertEquals(const std::string& msg, const std::string& type, T expected, T actual,
                  Map<std::string, std::string> attrs = {}) {
    std::string expStr = genericValueToString(expected);
    std::string actStr = genericValueToString(actual);
    assertionPrint(msg, "assertEquals", type, expStr, actStr, attrs);
}

template <typename T>
void assertEquals(const std::string& msg, T expected, T actual,
                  Map<std::string, std::string> attrs = {}) {
    assertEquals(msg, "T", expected, actual, attrs);
}

void assertEqualsBool(const std::string& msg, bool expected, bool actual);
void assertEqualsChar(const std::string& msg, char expected, char actual);
void assertEqualsDouble(const std::string& msg, double expected, double actual, double tolerance = 0.0001);
void assertEqualsInt(const std::string& msg, int expected, int actual);
void assertEqualsString(const std::string& msg, std::string expected, std::string actual);
void assertTrue(const std::string& msg, bool test);
void assertFalse(const std::string& msg, bool test);
void assertFail(const std::string& msg);
void setTestName(const std::string& name);
} // namespace CodeStepByStep::Assertions
#endif // _codestepbystep_assertions

} // namespace CodeStepByStep


#endif // _codestepbystep_h
