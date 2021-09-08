/*
 * LinkedIntList is an example of a second implementation of a basic data structure.
 * A LinkedIntList is a sequential collection of integers stored with 0-based integer
 * indexes and internally represented as a list of linked node structures.
 *
 * @version 2019/05/17
 * - added AssassinNode
 * @version 2018/03/19
 * - changed nullptr output to {}
 * @version 2016/11/11
 * - made into template class
 * @version 2016/08/23
 * - added initializer_list support to match other lib collections
 */

#ifndef _linkedintlist_h
#define _linkedintlist_h

#include <initializer_list>
#include <iostream>
#include <string>
#include "collections.h"
#include "error.h"
#include "hashset.h"
#include "strlib.h"
#include "vector.h"

/*
 * The internal structure representing a single node.
 */
template <typename T>
class ListNodeGen {
public:
    // global count of how many nodes have been created/destroyed
    static int s_allocated;
    static int s_freed;

    T data;           // element stored in each node
    ListNodeGen<T>* next;   // pointer to the next node (null if none)

    ListNodeGen(T d = T(), ListNodeGen<T>* n = nullptr);
    ~ListNodeGen();

    static void printChain(ListNodeGen<T>* list, std::string name = "list", int maxLength = 10);
};

template <typename T>
std::ostream& operator <<(std::ostream& out, ListNodeGen<T>* front);

template <typename T>
std::istream& operator >>(std::istream& input, ListNodeGen<T>*& front);


// begin "cpp" section of ListNode

template <typename T>
int ListNodeGen<T>::s_allocated = 0;

template <typename T>
int ListNodeGen<T>::s_freed = 0;

template <typename T>
ListNodeGen<T>::ListNodeGen(T d, ListNodeGen<T>* n) {
    data = d;
    next = n;
    s_allocated++;
}

template <typename T>
ListNodeGen<T>::~ListNodeGen() {
    s_freed++;
    next = nullptr;
}

template <typename T>
void ListNodeGen<T>::printChain(ListNodeGen<T>* list, std::string name, int maxLength) {
    std::cout << name << ": ";
    if (!list) {
        std::cout << "nullptr" << std::endl;
    } else {
        ListNodeGen<T>* curr = list;
        bool hasCycle = false;
        for (int i = 0; curr && (maxLength <= 0 || i < maxLength); i++, curr = curr->next) {
            std::cout << curr->data;
            if (curr->next) {
                std::cout << " -> ";
            }
            if (i == maxLength - 1) {
                std::cout << " ... (cycle)";
                hasCycle = true;
            }
        }
        if (!hasCycle) {
            std::cout << " /";
        }
        std::cout << std::endl;
    }
}

template <typename T>
std::ostream& operator <<(std::ostream& out, ListNodeGen<T>* front) {
    if (!front) {
        // out << "nullptr";
        out << "{}";
    } else {
        out << "{";
        if (front) {
            HashSet<ListNodeGen<T>*> visited;
            writeGenericValue(out, front->data, /* forceQuotes */ true);
            visited.add(front);

            ListNodeGen<T>* curr = front->next;
            while (curr) {
                out << ", ";
                writeGenericValue(out, curr->data, /* forceQuotes */ true);
                if (visited.contains(curr)) {
                    out << " (cycle!)";
                    break;
                }

                visited.add(curr);
                curr = curr->next;
            }
        }
        out << "}";
    }
    return out;
}

template <typename T>
std::istream& operator >>(std::istream& input, ListNodeGen<T>*& front) {
    // read into a vector first (yeah yeah, wastes some space, oh well)
    Vector<T> vec;
    T element;
    stanfordcpplib::collections::readCollection(input, vec, element, /* descriptor */ "ListNode::operator >>");

    // now convert into a linked list
    if (vec.isEmpty()) {
        front = nullptr;
    } else {
        front = new ListNodeGen<T>(vec[0]);
        ListNodeGen<T>* curr = front;
        for (int i = 1; i < vec.size(); i++) {
            curr->next = new ListNodeGen<T>(vec[i]);
            curr = curr->next;
        }
    }

    return input;
}

typedef ListNodeGen<int> ListNode;
typedef ListNodeGen<int> ListNodeInt;
typedef ListNodeGen<double> ListNodeDouble;
typedef ListNodeGen<std::string> ListNodeString;
typedef ListNodeGen<std::string> AssassinNode;

// end "cpp" section of ListNode


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

    // declared here so they can access front
    friend std::ostream& operator <<(std::ostream& out, const LinkedIntList& list);
    friend std::istream& operator >>(std::istream& input, LinkedIntList& list);

private:
    ListNode* front;   // pointer to front node in list;  null if empty

    bool m_locked;     // whether to forbid method calls (for problems)

    void checkIndex(int index, int min, int max) const;
    void checkLocked(std::string memberName = "") const;
};

#endif // _linkedintlist_h
