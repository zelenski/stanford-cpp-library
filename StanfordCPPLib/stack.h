/*
 * File: stack.h
 * -------------
 * This file exports the <code>Stack</code> class, which implements
 * a collection that processes values in a last-in/first-out (LIFO) order.
 */

#ifndef _stack_h
#define _stack_h

#include <stack>
#include "error.h"
#include "vector.h"

/*
 * Class: Stack<ValueType>
 * -----------------------
 * This class models a linear structure called a <b><i>stack</i></b>
 * in which values are added and removed only from one end.
 * This discipline gives rise to a last-in/first-out behavior (LIFO)
 * that is the defining feature of stacks.  The fundamental stack
 * operations are <code>push</code> (add to top) and <code>pop</code>
 * (remove from top).
 */
template <typename ValueType>
class Stack {
public:
    /*
     * Constructor: Stack
     * Usage: Stack<ValueType> stack;
     * ------------------------------
     * Initializes a new empty stack.
     */
    Stack();

    /*
     * Destructor: ~Stack
     * ------------------
     * Frees any heap storage associated with this stack.
     */
    virtual ~Stack();
    
    /*
     * Method: clear
     * Usage: stack.clear();
     * ---------------------
     * Removes all elements from this stack.
     */
    void clear();
    
    /*
     * Method: isEmpty
     * Usage: if (stack.isEmpty()) ...
     * -------------------------------
     * Returns <code>true</code> if this stack contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: peek
     * Usage: ValueType top = stack.peek();
     * ------------------------------------
     * Returns the value of top element from this stack, without removing
     * it.  This method signals an error if called on an empty stack.
     */
    ValueType peek() const;
    
    /*
     * Method: pop
     * Usage: ValueType top = stack.pop();
     * -----------------------------------
     * Removes the top element from this stack and returns it.  This
     * method signals an error if called on an empty stack.
     */
    ValueType pop();
    
    /*
     * Method: push
     * Usage: stack.push(value);
     * -------------------------
     * Pushes the specified value onto this stack.
     */
    void push(const ValueType& value);

    /*
     * Method: size
     * Usage: int n = stack.size();
     * ----------------------------
     * Returns the number of values in this stack.
     */
    int size() const;
    
    /*
     * Method: top
     * Usage: ValueType top = stack.top();
     * ------------------------------------
     * Returns the value of top element from this stack, without removing
     * it.  This method signals an error if called on an empty stack.  For
     * compatibility with the STL classes, this method is exported
     * under the name <code>top</code>, in which case it returns the value
     * by reference.
     */
    ValueType& top();
    
    /*
     * Returns an STL stack object with the same elements as this Stack.
     */
    std::stack<ValueType> toStlStack() const;

    /*
     * Method: toString
     * Usage: string str = stack.toString();
     * -------------------------------------
     * Converts the stack to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: ==
     * Usage: stack1 == stack2
     * -----------------------
     * Returns <code>true</code> if <code>stack1</code> and <code>stack2</code>
     * contain the same elements.
     */
    bool operator ==(const Stack& stack2) const;

    /*
     * Operator: !=
     * Usage: stack1 != stack2
     * -----------------------
     * Returns <code>true</code> if <code>stack1</code> and <code>stack2</code>
     * do not contain the same elements.
     */
    bool operator !=(const Stack& stack2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes: Stack data structure
     * ------------------------------------------
     * The easiest way to implement a stack is to store the elements in a
     * Vector.  Doing so means that the problems of dynamic memory allocation
     * and copy assignment are already solved by the implementation of the
     * underlying Vector class.
     */

private:
    Vector<ValueType> elements;
};

/*
 * Stack class implementation
 * --------------------------
 * The Stack is internally managed using a Vector.  This layered design
 * makes the implementation extremely simple, to the point that most
 * methods can be implemented in as single line.
 */

template <typename ValueType>
Stack<ValueType>::Stack() {
    /* Empty */
}

template <typename ValueType>
Stack<ValueType>::~Stack() {
    /* Empty */
}

template <typename ValueType>
void Stack<ValueType>::clear() {
    elements.clear();
}

template <typename ValueType>
bool Stack<ValueType>::isEmpty() const {
    return size() == 0;
}

template <typename ValueType>
ValueType Stack<ValueType>::peek() const {
    if (isEmpty()) {
        error("Stack::peek: Attempting to peek at an empty stack");
    }
    return elements.get(elements.size() - 1);
}

template <typename ValueType>
ValueType Stack<ValueType>::pop() {
    if (isEmpty()) {
        error("Stack::pop: Attempting to pop an empty stack");
    }
    ValueType top = elements[elements.size() - 1];
    elements.remove(elements.size() - 1);
    return top;
}

template <typename ValueType>
void Stack<ValueType>::push(const ValueType& value) {
    elements.add(value);
}

template <typename ValueType>
int Stack<ValueType>::size() const {
    return elements.size();
}

template <typename ValueType>
ValueType & Stack<ValueType>::top() {
    if (isEmpty()) {
        error("Stack::top: Attempting to read top of an empty stack");
    }
    return elements[elements.size() - 1];
}

template <typename ValueType>
std::stack<ValueType> Stack<ValueType>::toStlStack() const {
    std::stack<ValueType> result;
    for (int i = 0; i < size(); i++) {
        result.push(this->elements[i]);
    }
    return result;
}

template <typename ValueType>
std::string Stack<ValueType>::toString() const {
    ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
bool Stack<ValueType>::operator ==(const Stack& stack2) const {
    if (size() != stack2.size()) {
        return false;
    }
    for (int i = 0; i < size(); i++) {
        if (this->elements[i] != stack2.elements[i]) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
bool Stack<ValueType>::operator !=(const Stack & stack2) const {
    return !(*this == stack2);
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Stack<ValueType>& stack) {
    os << "{";
    Stack<ValueType> copy = stack;
    Stack<ValueType> reversed;
    while (!copy.isEmpty()) {
        reversed.push(copy.pop());
    }
    int len = stack.size();
    for (int i = 0; i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        writeGenericValue(os, reversed.pop(), true);
    }
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Stack<ValueType>& stack) {
    char ch;
    is >> ch;
    if (ch != '{') {
        error("Stack::operator >>: Missing {");
    }
    stack.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            ValueType value;
            readGenericValue(is, value);
            stack.push(value);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("Stack::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

// hashing functions for stacks;  defined in hashmap.cpp
int hashCode(const Stack<std::string>& s);
int hashCode(const Stack<int>& s);
int hashCode(const Stack<char>& s);
int hashCode(const Stack<long>& s);
int hashCode(const Stack<double>& s);

#endif
