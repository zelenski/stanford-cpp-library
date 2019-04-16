/*
 * File: stack.h
 * -------------
 * This file exports the <code>Stack</code> class, which implements
 * a collection that processes values in a last-in/first-out (LIFO) order.
 * 
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via Vector)
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * - made const iterators public
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {1, 2, 3}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2014/11/13
 * - added add() method as synonym for push()
 * - added remove() method as synonym for pop()
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _stack_h
#define _stack_h

#include <initializer_list>

#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

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
     * Method: add
     * Usage: stack.add(value);
     * -------------------------
     * Pushes the specified value onto the top of this stack.
     * A synonym for the push method.
     */
    void add(const ValueType& value);
    
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
     * Pushes the specified value onto the top of this stack.
     */
    void push(const ValueType& value);

    /*
     * Method: remove
     * Usage: ValueType top = stack.remove();
     * -----------------------------------
     * Removes the top element from this stack and returns it.
     * A synonym for the pop method.
     */
    ValueType remove();

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
void Stack<ValueType>::add(const ValueType& value) {
    push(value);
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
ValueType Stack<ValueType>::peek() const {
    if (isEmpty()) {
        error("Stack::peek: Attempting to peek at an empty stack");
    }
    return _elements.back();
}

template <typename ValueType>
ValueType Stack<ValueType>::pop() {
    if (isEmpty()) {
        error("Stack::pop: Attempting to pop an empty stack");
    }
    return _elements.pop_back();
}

template <typename ValueType>
void Stack<ValueType>::push(const ValueType& value) {
    _elements.push_back(value);
}

template <typename ValueType>
ValueType Stack<ValueType>::remove() {
    return pop();
}

template <typename ValueType>
int Stack<ValueType>::size() const {
    return _elements.size();
}

template <typename ValueType>
ValueType & Stack<ValueType>::top() {
    if (isEmpty()) {
        error("Stack::top: Attempting to read top of an empty stack");
    }
    return _elements.back();
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
std::istream& operator >>(std::istream& is, Stack<ValueType>& stack) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, stack, element, /* descriptor */ "Stack::operator >>");
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
