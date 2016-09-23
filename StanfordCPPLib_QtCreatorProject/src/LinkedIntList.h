/*
 * LinkedIntList is an example of a second implementation of a basic data structure.
 * A LinkedIntList is a sequential collection of integers stored with 0-based integer
 * indexes and internally represented as a list of linked node structures.
 *
 * @version 2016/08/23
 * - added initializer_list support to match other lib collections
 */

#ifndef _linkedintlist_h
#define _linkedintlist_h

#include <initializer_list>
#include <iostream>
#include <string>
using namespace std;

struct ListNode;   // forward declaration

class LinkedIntList {
public:
    LinkedIntList();
    LinkedIntList(std::initializer_list<int> list);
    ~LinkedIntList();

    void add(int value);
    void clear();
    int get(int index) const;
    void insert(int index, int value);
    bool isEmpty() const;
    void remove(int index);
    void set(int index, int value);
    int size() const;
    string toString() const;

    // section problems
    void setLocked(bool locked);
    int min() const;
    bool isSorted() const;
    int countDuplicates() const;
    void stutter();
    int deleteBack();
    void split();
    void removeAll(int value);
    void doubleList();
    void rotate();
    void reverse();
    void consume(LinkedIntList& list2);
    void transferEvens(LinkedIntList& list2);
    
    // exam questions
    void chopBothSides(int k);
    void combineDuplicates();
    void expand(int k);
    bool isSortedBy(int k) const;
    void mergeUpTo(int max);
    void partitionSort();
    bool removeRange(int min, int max);

    LinkedIntList& operator =(const LinkedIntList& src);

    // operators for reading/writing lists based on a front pointer
    // (these are used by the CodeStepByStep tool)
    friend ostream& operator <<(ostream& out, ListNode* front);
    friend istream& operator >>(istream& input, ListNode*& front);

private:
    ListNode* front;   // pointer to front node in list;  NULL if empty
    
    bool m_locked;     // whether to forbid method calls (for problems)

    void checkIndex(int index, int min, int max) const;
    void checkLocked(string memberName = "") const;
};

ostream& operator <<(ostream& out, const LinkedIntList& list);
istream& operator >>(istream& input, LinkedIntList& list);

/*
 * The internal structure representing a single node.
 */
struct ListNode {
    // global count of how many nodes have been created/destroyed
    static int s_allocated;
    static int s_freed;
    
    int data;         // element stored in each node
    ListNode* next;   // pointer to the next node (NULL if none)
    
    ListNode(int d = 0, ListNode* n = NULL);
    ~ListNode();
    
    static void printChain(ListNode* list, string name = "list", int maxLength = 10);
};

ostream& operator <<(ostream& out, ListNode* front);
istream& operator >>(istream& input, ListNode*& front);

#endif
