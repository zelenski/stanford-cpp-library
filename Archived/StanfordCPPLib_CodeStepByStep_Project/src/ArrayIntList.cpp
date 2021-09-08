/*
 * CS 106B, Marty Stepp
 * ArrayIntList.cpp implements the ArrayIntList class behavior declared in ArrayIntList.h.
 * 
 * @version 2016/08/23
 * - added initializer_list support to match other lib collections
 */

#define INTERNAL_INCLUDE 1
#include "ArrayIntList.h"
#include "error.h"
#include "strlib.h"
#undef INTERNAL_INCLUDE

/*
 * Constructs a new empty list (capacity 10).
 */
ArrayIntList::ArrayIntList() {
    elements = new int[10]();
    mysize = 0;
    capacity = 10;
}

/*
 * Constructs a new empty list with the given capacity.
 */
ArrayIntList::ArrayIntList(int cap) {
    elements = new int[cap]();
    mysize = 0;
    capacity = cap;
}

/*
 * Constructs a new empty list storing the given elements.
 */
ArrayIntList::ArrayIntList(std::initializer_list<int> list) {
    elements = new int[list.size()]();
    mysize = 0;
    capacity = list.size();
    for (int n : list) {
        add(n);
    }
}

/*
 * Destructor is called when an ArrayIntList object is destroyed
 * (when the closing } brace is reached in the function where
 * it is declared).
 */
ArrayIntList::~ArrayIntList() {
    delete[] elements;
}

/*
 * Appends the given value to the end of the list.
 */
void ArrayIntList::add(int value) {
    ensureCapacity(mysize + 1);
    elements[mysize] = value;
    mysize++;
}

/*
 * Removes all elements from the list.
 */
void ArrayIntList::clear() {
    mysize = 0;
}

/*
 * Returns the value at the given 0-based index of the list.
 */
int ArrayIntList::get(int index) const {
    checkIndex(index, 0, mysize - 1);
    return elements[index];
}

/*
 * Adds the given value just before the given 0-based index in the list,
 * shifting subsequent elements right as necessary to make room.
 * Throws a string exception if the index is out of bounds.
 */
void ArrayIntList::insert(int index, int value) {
    checkIndex(index, 0, mysize);
    ensureCapacity(mysize + 1);
    for (int i = mysize; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index] = value;
    mysize++;
}

/*
 * Returns true if there are no elements in the list.
 */
bool ArrayIntList::isEmpty() const {
    return mysize == 0;
}

/*
 * Removes the element at the given index from the list,
 * shifting elements left to make room.
 * Throws a string exception if the index is out of bounds.
 */
void ArrayIntList::remove(int index) {
    checkIndex(index, 0, mysize - 1);
    for (int i = index; i < mysize - 1; i++) {
        elements[i] = elements[i + 1];
    }
    mysize--;
}

/*
 * Stores the given value at the given index in the list.
 * Throws a string exception if the index is out of bounds.
 */
void ArrayIntList::set(int index, int value) {
    checkIndex(index, 0, mysize - 1);
    elements[index] = value;
}

/*
 * Returns the number of elements in the list.
 */
int ArrayIntList::size() const {
    return mysize;
}

/*
 * Prints the list to the given output stream, in a format such as:
 * {42, -7, 19, 106}
 */
ostream& operator <<(ostream& out, const ArrayIntList& list) {
    out << "{";
    if (!list.isEmpty()) {
        out << list.get(0);   // fencepost
        for (int i = 1; i < list.size(); i++) {
            out << ", " << list.get(i);
        }
    }
    out << "}";
    return out;
}

istream& operator >>(istream& input, ArrayIntList& list) {
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("ArrayIntList::operator >>: Missing {");
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
                error("ArrayIntList::operator >>: parse error");
#endif
                return input;
            }
            list.add(value);
            input >> ch;
            if (ch == '}') {
                break;
            } else if (ch != ',') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(std::string("ArrayIntList::operator >>: Unexpected character ") + ch);
#endif
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }
    return input;
}

/*
 * Throws a string exception if the given index is not between
 * min and max, inclusive.
 */
void ArrayIntList::checkIndex(int index, int min, int max) const {
    if (index < min || index > max) {
        throw "Index out of bounds: " + integerToString(index);
    }
}

/*
 * Grows the internal array to be at least as large as the given capacity.
 * Resizes by factors of 2 to ensure amortized O(1) add performance.
 */
void ArrayIntList::ensureCapacity(int cap) {
    if (capacity < cap) {
        while (capacity < cap) {
            capacity *= 2;
        }
        
        // copy all elements into a bigger array
        int* bigger = new int[capacity];
        for (int i = 0; i < mysize; i++) {
            bigger[i] = elements[i];
        }
        
        // swap in the new bigger array for the old one
        delete[] elements;
        elements = bigger;
    }
}

ArrayIntList& ArrayIntList::operator =(const ArrayIntList& src) {
    if (this != &src) {
        // deep copy
        clear();
        ensureCapacity(src.size());
        for (int i = 0; i < src.size(); i++) {
            add(src.get(i));
        }
    }
    return *this;
}
