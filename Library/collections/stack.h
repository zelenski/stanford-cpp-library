/*
 * File: stack.h
 * -------------
 * This file exports the <code>Stack</code> class, which implements
 * a collection that processes values in a last-in/first-out (LIFO) order.
 */

#ifndef _stack_h
#define _stack_h

#include <initializer_list>

#include "error.h"
#include "hashcode.h"
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
    Stack() = default;

    /*
     * Constructor: Stack
     * Usage: Stack<ValueType> stack {1, 2, 3};
     * ----------------------------------------
     * Initializes a new stack that stores the given elements from bottom-top.
     */
    Stack(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~Stack
     * ------------------
     * Frees any heap storage associated with this stack.
     */
    virtual ~Stack() = default;

    /*
     * Method: clear
     * Usage: stack.clear();
     * ---------------------
     * Removes all elements from this stack.
     */
    void clear();

    /*
     * Method: equals
     * Usage: if (stack.equals(stack2)) ...
     * ------------------------------------
     * Returns <code>true</code> if this stack contains exactly the same values
     * as the given other stack.
     * Identical in behavior to the == operator.
     */
    bool equals(const Stack<ValueType>& stack2) const;

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
    const ValueType& peek() const;

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
     * Pushes the specified value onto the top of this stack.
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

    /*
     * Operators: <, >, <=, >=
     * Usage: if (stack1 < stack2) ...
     * -------------------------------
     * Relational operators to compare two stacks.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Stack& stack2) const;
    bool operator <=(const Stack& stack2) const;
    bool operator >(const Stack& stack2) const;
    bool operator >=(const Stack& stack2) const;

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

    template <typename T>
    friend int hashCode(const Stack<T>& s);

    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const Stack<T>& stack);

private:
    Vector<ValueType> _elements;
};

/*
 * Stack class implementation
 * --------------------------
 * The Stack is internally managed using a Vector.  This layered design
 * makes the implementation extremely simple, to the point that most
 * methods can be implemented in as single line.
 */

template <typename ValueType>
Stack<ValueType>::Stack(std::initializer_list<ValueType> list) : _elements(list) {
}

template <typename ValueType>
void Stack<ValueType>::clear() {
    _elements.clear();
}

template <typename ValueType>
bool Stack<ValueType>::equals(const Stack<ValueType>& stack2) const {
    return stanfordcpplib::collections::equals(_elements, stack2._elements);
}

template <typename ValueType>
bool Stack<ValueType>::isEmpty() const {
    return size() == 0;
}

template <typename ValueType>
const ValueType& Stack<ValueType>::peek() const {
    if (isEmpty()) {
        error("Stack::peek: Attempting to peek at an empty stack");
    }
    return _elements[_elements.size() -1];
}

template <typename ValueType>
ValueType Stack<ValueType>::pop() {
    if (isEmpty()) {
        error("Stack::pop: Attempting to pop an empty stack");
    }
    return _elements.remove(_elements.size() -1);
}

template <typename ValueType>
void Stack<ValueType>::push(const ValueType& value) {
    _elements.add(value);
}

template <typename ValueType>
int Stack<ValueType>::size() const {
    return _elements.size();
}

template <typename ValueType>
std::string Stack<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
bool Stack<ValueType>::operator ==(const Stack& stack2) const {
    return _elements == stack2._elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator !=(const Stack & stack2) const {
    return _elements != stack2._elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator <(const Stack & stack2) const {
    return _elements < stack2._elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator <=(const Stack & stack2) const {
    return _elements <= stack2._elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator >(const Stack & stack2) const {
    return _elements > stack2._elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator >=(const Stack & stack2) const {
    return _elements >= stack2._elements;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Stack<ValueType>& stack) {
    return os << stack._elements;
}

template <typename ValueType>
    void readOne(Stack<ValueType>& stack, const ValueType& value)
        { stack.push(value); }

template <typename ValueType>
std::istream& operator >>(std::istream& is, Stack<ValueType>& stack) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, stack, element, /* descriptor */ "Stack::operator >>", readOne<ValueType>);
}

/*
 * Template hash function for stacks.
 * Requires the element type in the Stack to have a hashCode function.
 */
template <typename T>
int hashCode(const Stack<T>& s) {
    return hashCode(s._elements);
}

#endif // _stack_h
