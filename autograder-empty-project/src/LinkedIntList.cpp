/*
 * LinkedIntList.cpp implements the LinkedIntList class behavior declared in LinkedIntList.h.
 *
 * @version 2016/11/11
 * - made into template class
 */

#define INTERNAL_INCLUDE 1
#include "LinkedIntList.h"
#undef INTERNAL_INCLUDE

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
    std::ostringstream out;
    out << *this;
    return out.str();
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
    if (list.isEmpty()) {
        out << "{}";
    } else {
        out << list.front;
    }
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
