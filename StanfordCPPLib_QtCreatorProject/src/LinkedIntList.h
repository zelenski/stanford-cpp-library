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
    std::string toString() const;

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
    friend std::ostream& operator <<(std::ostream& out, ListNode* front);
    friend std::istream& operator >>(std::istream& input, ListNode*& front);

private:
    ListNode* front;   // pointer to front node in list;  null if empty
    
    bool m_locked;     // whether to forbid method calls (for problems)

    void checkIndex(int index, int min, int max) const;
    void checkLocked(std::string memberName = "") const;
};

std::ostream& operator <<(std::ostream& out, const LinkedIntList& list);
std::istream& operator >>(std::istream& input, LinkedIntList& list);

/*
 * The internal structure representing a single node.
 */
struct ListNode {
    // global count of how many nodes have been created/destroyed
    static int s_allocated;
    static int s_freed;

    int data;         // element stored in each node
    ListNode* next;   // pointer to the next node (null if none)

    ListNode(int d = 0, ListNode* n = nullptr);
    ~ListNode();

    static void printChain(ListNode* list, std::string name = "list", int maxLength = 10);
};

/*
 * The internal structure representing a single node.
 */
struct ListNodeDouble {
    // global count of how many nodes have been created/destroyed
    static int s_allocated;
    static int s_freed;

    double data;            // element stored in each node
    ListNodeDouble* next;   // pointer to the next node (null if none)

    ListNodeDouble(double d = 0.0, ListNodeDouble* n = nullptr);
    ~ListNodeDouble();

    static void printChain(ListNodeDouble* list, std::string name = "list", int maxLength = 10);
};

/*
 * The internal structure representing a single node.
 */
struct ListNodeString {
    // global count of how many nodes have been created/destroyed
    static int s_allocated;
    static int s_freed;

    std::string data;       // element stored in each node
    ListNodeString* next;   // pointer to the next node (null if none)

    ListNodeString(std::string d = "", ListNodeString* n = nullptr);
    ~ListNodeString();

    static void printChain(ListNodeString* list, std::string name = "list", int maxLength = 10);
};

std::ostream& operator <<(std::ostream& out, ListNode* front);
std::istream& operator >>(std::istream& input, ListNode*& front);
std::ostream& operator <<(std::ostream& out, ListNodeDouble* front);
std::istream& operator >>(std::istream& input, ListNodeDouble*& front);
std::ostream& operator <<(std::ostream& out, ListNodeString* front);
std::istream& operator >>(std::istream& input, ListNodeString*& front);

#endif
