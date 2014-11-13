/*
 * File: stack.h
 * -------------
 * This file exports the <code>Stack</code> class, which implements
 * a collection that processes values in a last-in/first-out (LIFO) order.
 * 
 * @version 2014/11/13
 * - added add() method as synonym for push()
 * - added remove() method as synonym for pop()
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 */

#ifndef _stack_h
#define _stack_h

#include <iterator>
#include <stack>
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
    Stack();

    /*
     * Destructor: ~Stack
     * ------------------
     * Frees any heap storage associated with this stack.
     */
    virtual ~Stack();
    
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
    Vector<ValueType> elements;

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public Vector<ValueType>::iterator {
    public:
        iterator() : Vector<ValueType>::iterator() {}
        iterator(const iterator& it) : Vector<ValueType>::iterator(it) {}
        iterator(const typename Vector<ValueType>::iterator& it) : Vector<ValueType>::iterator(it) {}
    };
    
    class const_iterator : public Vector<ValueType>::const_iterator {
    public:
        const_iterator() : Vector<ValueType>::const_iterator() {}
        const_iterator(const const_iterator& it) : Vector<ValueType>::const_iterator(it) {}
        const_iterator(const typename Vector<ValueType>::const_iterator& it) : Vector<ValueType>::const_iterator(it) {}
    };
    
    /*
     * Returns an iterator positioned at the first element of the list.
     */
    iterator begin() {
        return iterator(elements.begin());
    }

    /*
     * Returns an iterator positioned at the first element of the list.
     */
    const_iterator begin() const {
        auto itr = elements.begin();
        return const_iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the list.
     */
    iterator end() {
        auto itr = elements.end();
        return iterator(itr);
    }
    
    /*
     * Returns an iterator positioned at the last element of the list.
     */
    const_iterator end() const {
        auto itr = elements.end();
        return const_iterator(itr);
    }
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
void Stack<ValueType>::add(const ValueType& value) {
    push(value);
}

template <typename ValueType>
void Stack<ValueType>::clear() {
    elements.clear();
}

template <typename ValueType>
bool Stack<ValueType>::equals(const Stack<ValueType>& stack2) const {
    if (this == &stack2) {
		return true;
	}
	if (size() != stack2.size()) {
        return false;
    }
    for (int i = 0; i < size(); i++) {
        if (elements[i] != stack2.elements[i]) {
            return false;
        }
    }
    return true;
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
ValueType Stack<ValueType>::remove() {
    return pop();
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
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
bool Stack<ValueType>::operator ==(const Stack& stack2) const {
    return elements == stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator !=(const Stack & stack2) const {
    return elements != stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator <(const Stack & stack2) const {
    return elements < stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator <=(const Stack & stack2) const {
    return elements <= stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator >(const Stack & stack2) const {
    return elements > stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator >=(const Stack & stack2) const {
    return elements >= stack2.elements;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const Stack<ValueType>& stack) {
    return os << stack.elements;
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, Stack<ValueType>& stack) {
    char ch = '\0';
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

/*
 * Template hash function for stacks.
 * Requires the element type in the Stack to have a hashCode function.
 */
template <typename T>
int hashCode(const Stack<T>& s) {
    return hashCode(s.elements);
}

#endif
