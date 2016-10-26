/*
 * LinkedIntList.cpp implements the LinkedIntList class behavior declared in LinkedIntList.h.
 *
 * @version 2015/08/16
 */

#include "LinkedIntList.h"
#include "error.h"
#include "hashset.h"
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

int ListNodeDouble::s_allocated = 0;
int ListNodeDouble::s_freed = 0;

ListNodeDouble::ListNodeDouble(double d, ListNodeDouble* n) {
    data = d;
    next = n;
    s_allocated++;
}

ListNodeDouble::~ListNodeDouble() {
    s_freed++;
}

int ListNodeString::s_allocated = 0;
int ListNodeString::s_freed = 0;

ListNodeString::ListNodeString(std::string d, ListNodeString* n) {
    data = d;
    next = n;
    s_allocated++;
}

ListNodeString::~ListNodeString() {
    s_freed++;
}

void ListNode::printChain(ListNode* list, std::string name, int maxLength) {
    std::cout << name << ": ";
    if (list == NULL) {
        std::cout << "NULL" << std::endl;
    } else {
        ListNode* curr = list;
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

void ListNodeDouble::printChain(ListNodeDouble* list, std::string name, int maxLength) {
    std::cout << name << ": ";
    if (list == NULL) {
        std::cout << "NULL" << std::endl;
    } else {
        ListNodeDouble* curr = list;
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

void ListNodeString::printChain(ListNodeString* list, std::string name, int maxLength) {
    std::cout << name << ": ";
    if (list == NULL) {
        std::cout << "NULL" << std::endl;
    } else {
        ListNodeString* curr = list;
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

std::ostream& operator <<(std::ostream& out, ListNode* front) {
    LinkedIntList* list = new LinkedIntList();
    list->front = front;
    out << *list;
    list->front = nullptr;   // avoid double-free
    return out;
}

std::ostream& operator <<(std::ostream& out, ListNodeDouble* front) {
    out << "{";
    if (front) {
        ListNodeDouble* curr = front;
        bool hasCycle = false;
        int maxLength = 100;
        for (int i = 0; curr && (maxLength <= 0 || i < maxLength); i++, curr = curr->next) {
            out << curr->data;
            if (curr->next) {
                out << ", ";
            }
            if (i == maxLength - 1) {
                out << " ... (cycle)";
                hasCycle = true;
                break;
            }
        }
    }
    out << "}";
    return out;
}

std::ostream& operator <<(std::ostream& out, ListNodeString* front) {
    out << "{";
    if (front) {
        ListNodeString* curr = front;
        bool hasCycle = false;
        int maxLength = 100;
        for (int i = 0; curr && (maxLength <= 0 || i < maxLength); i++, curr = curr->next) {
            out << curr->data;
            if (curr->next) {
                out << ", ";
            }
            if (i == maxLength - 1) {
                out << " ... (cycle)";
                hasCycle = true;
                break;
            }
        }
    }
    out << "}";
    return out;
}

std::istream& operator >>(std::istream& input, ListNode*& front) {
    LinkedIntList* list = new LinkedIntList();
    input >> *list;
    if (input.fail()) {
        front = nullptr;
    } else {
        front = list->front;
    }
    list->front = nullptr;   // avoid double-free
    return input;
}

std::istream& operator >>(std::istream& input, ListNodeDouble*& front) {
    // parse as vector, e.g. {1.0, 2.5, 3.5}, then convert to LL of double
    Vector<double> vec;
    if (!(input >> vec)) {
        return input;
    }

    // convert vector to LL
    if (vec.isEmpty()) {
        front = nullptr;
    } else {
        front = new ListNodeDouble(vec[0]);
        ListNodeDouble* curr = front;
        for (int i = 1; i < vec.size(); i++) {
            curr->next = new ListNodeDouble(vec[i]);
            curr = curr->next;
        }
    }

    return input;
}

std::istream& operator >>(std::istream& input, ListNodeString*& front) {
    // parse as vector, e.g. {1.0, 2.5, 3.5}, then convert to LL of double
    Vector<std::string> vec;
    if (!(input >> vec)) {
        return input;
    }

    // convert vector to LL
    if (vec.isEmpty()) {
        front = nullptr;
    } else {
        front = new ListNodeString(vec[0]);
        ListNodeString* curr = front;
        for (int i = 1; i < vec.size(); i++) {
            curr->next = new ListNodeString(vec[i]);
            curr = curr->next;
        }
    }

    return input;
}

LinkedIntList::LinkedIntList()
        : front(nullptr), m_locked(false) {
    // empty
}

/*
 * Constructs a new empty list storing the given elements.
 */
LinkedIntList::LinkedIntList(std::initializer_list<int> list)
        : front(nullptr), m_locked(false) {
    for (int n : list) {
        add(n);
    }
}

LinkedIntList::~LinkedIntList() {
    m_locked = false;
    clear();
}

void LinkedIntList::add(int value) {
    checkLocked("add");
    if (!front) {
        // empty list: add this node as the new front
        front = new ListNode(value);
    } else {
        // non-empty list: move to end, attach new node
        ListNode* current = front;
        while (current->next) {
            current = current->next;
        }
        current->next = new ListNode(value);
    }
}

void LinkedIntList::clear() {
    checkLocked("clear");
    while (front) {
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
    return front == nullptr;
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
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// TODO: put printing code into operator <<, call that from toString
std::string LinkedIntList::toString() const {
    std::string result = "{";
    if (!isEmpty()) {
        HashSet<ListNode*> visited;
        result += integerToString(front->data);
        visited.add(front);

        ListNode* curr = front->next;
        while (curr) {
            result += ", " + integerToString(curr->data);
            if (visited.contains(curr)) {
                result += " (cycle!)";
                break;
            }

            visited.add(curr);
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
        throw std::string("Invalid index");
    }
}

void LinkedIntList::checkLocked(std::string memberName) const {
    if (m_locked) {
        error("LinkedIntList forbidden from calling member function " + memberName);
    }
}

LinkedIntList& LinkedIntList::operator =(const LinkedIntList& src) {
    if (this != &src) {
        // deep copy
        clear();
        for (int i = 0; i < src.size(); i++) {
            add(src.get(i));
        }
    }
    return *this;
}

std::ostream& operator <<(std::ostream& out, const LinkedIntList& list) {
    out << list.toString();
    return out;
}

std::istream& operator >>(std::istream& input, LinkedIntList& list) {
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("LinkedIntList::operator >>: Missing {");
#endif
        input.setstate(std::ios_base::failbit);
        return input;
    }
    list.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            int value;
            if (!readGenericValue(input, value)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error("LinkedIntList::operator >>: parse error");
#endif
                return input;
            }
            list.add(value);
            input >> ch;
            if (ch == '}') {
                break;
            } else if (ch != ',') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(std::string("LinkedIntList::operator >>: Unexpected character ") + ch);
#endif
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }
    return input;
}
