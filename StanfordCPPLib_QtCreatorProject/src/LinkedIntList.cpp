/*
 * LinkedIntList.cpp implements the LinkedIntList class behavior declared in LinkedIntList.h.
 *
 * @version 2015/07/23
 */

#include "LinkedIntList.h"
#include "error.h"
#include "strlib.h"

int ListNode::s_allocated = 0;
int ListNode::s_freed = 0;

ListNode::ListNode(int d, ListNode* n) {
    data = d;
    next = n;
    s_allocated++;
}

ListNode::~ListNode() {
    s_freed++;
}

void ListNode::printChain(ListNode* list, string name, int maxLength) {
    cout << name << ": ";
    if (list == NULL) {
        cout << "NULL" << endl;
    } else {
        ListNode* curr = list;
        bool hasCycle = false;
        for (int i = 0; curr != NULL && (maxLength <= 0 || i < maxLength); i++, curr = curr->next) {
            cout << curr->data;
            if (curr->next != NULL) {
                cout << " -> ";
            }
            if (i == maxLength - 1) {
                cout << " ... (cycle)";
                hasCycle = true;
            }
        }
        if (!hasCycle) {
            cout << " /";
        }
        cout << endl;
    }
}

LinkedIntList::LinkedIntList()
        : front(NULL), m_locked(false) {
    // empty
}

LinkedIntList::~LinkedIntList() {
    clear();
}

void LinkedIntList::add(int value) {
    checkLocked("add");
    if (front == NULL) {
        // empty list: add this node as the new front
        front = new ListNode(value, NULL);
    } else {
        // non-empty list: move to end, attach new node
        ListNode* current = front;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new ListNode(value, NULL);
    }
}

void LinkedIntList::clear() {
    checkLocked("clear");
    while (front != NULL) {
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }
}

int LinkedIntList::get(int index) const {
    checkLocked("get");
    checkIndex(index, 0, size() - 1);
    ListNode* current = front;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void LinkedIntList::insert(int index, int value) {
    checkLocked("insert");
    checkIndex(index, 0, size());
    if (index == 0) {
        ListNode* temp = front;
        front = new ListNode(value, temp);
    } else {
        ListNode* current = front;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        ListNode* temp = current->next;
        current->next = new ListNode(value, temp);
    }
}

bool LinkedIntList::isEmpty() const {
    return front == NULL;
}

void LinkedIntList::remove(int index) {
    checkLocked("remove");
    checkIndex(index, 0, size() - 1);
    ListNode* trash;
    if (index == 0) {
        trash = front;
        front = front->next;
    } else {
        ListNode* curr = front;
        for (int i = 0; i < index-1; i++) {
            curr = curr->next;
        }
        trash = curr->next;
        curr->next = curr->next->next;
    }
    delete trash;
}

void LinkedIntList::set(int index, int value) {
    checkLocked("set");
    checkIndex(index, 0, size() - 1);
    ListNode* current = front;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    current->data = value;
}

int LinkedIntList::size() const {
    checkLocked("size");
    int count = 0;
    ListNode* current = front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

string LinkedIntList::toString() const {
    string result = "{";
    if (!isEmpty()) {
        result += integerToString(front->data);
        ListNode* curr = front->next;
        while (curr != NULL) {
            result += ", " + integerToString(curr->data);
            curr = curr->next;
        }
    }
    return result + "}";
}

void LinkedIntList::setLocked(bool locked) {
    m_locked = locked;
}

void LinkedIntList::checkIndex(int index, int min, int max) const {
    if (index < min || index > max) {
        throw string("Invalid index");
    }
}

void LinkedIntList::checkLocked(string memberName) const {
    if (m_locked) {
        error("LinkedIntList forbidden from calling member function " + memberName);
    }
}

ostream& operator <<(ostream& out, const LinkedIntList& list) {
    out << list.toString();
    return out;
}

istream& operator >>(istream& input, LinkedIntList& list) {
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
        error("LinkedIntList::operator >>: Missing {");
    }
    list.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            int value;
            readGenericValue(input, value);
            list.add(value);
            input >> ch;
            if (ch == '}') {
                break;
            } else if (ch != ',') {
                error(std::string("LinkedIntList::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return input;
}
