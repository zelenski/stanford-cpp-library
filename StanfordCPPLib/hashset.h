/*
 * File: hashset.h
 * ---------------
 * This file exports the <code>HashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 * 
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added template hashCode function
 * @version 2014/10/10
 * - removed foreach patch
 */

#ifndef _hashset_h
#define _hashset_h

#include <iostream>
#include "error.h"
#include "hashcode.h"
#include "hashmap.h"
#include "vector.h"

/*
 * Class: HashSet<ValueType>
 * -------------------------
 * This class implements an efficient abstraction for storing sets
 * of distinct elements.  This class is identical to the <code>Set</code>
 * class except for the fact that it uses a hash table as its underlying
 * representation.  The advantage of the <code>HashSet</code> class is that
 * it operates in constant time, as opposed to the <i>O</i>(log <i>N</i>)
 * time for the <code>Set</code> class.  The disadvantage of
 * <code>HashSet</code> is that iterators return the values in a
 * seemingly random order.
 */
template <typename ValueType>
class HashSet {
public:
    /*
     * Constructor: HashSet
     * Usage: HashSet<ValueType> set;
     * ------------------------------
     * Initializes an empty set of the specified element type.
     */
    HashSet();

    /*
     * Destructor: ~HashSet
     * --------------------
     * Frees any heap storage associated with this set.
     */
    virtual ~HashSet();

    /*
     * Method: add
     * Usage: set.add(value);
     * ----------------------
     * Adds an element to this set, if it was not already there.  For
     * compatibility with the STL <code>set</code> class, this method
     * is also exported as <code>insert</code>.
     */
    void add(const ValueType& value);
    
    /*
     * Method: addAll
     * Usage: set.addAll(set2);
     * ----------------------
     * Adds all elements of the given other set to this set.
     * Returns a reference to this set.
     * Identical in behavior to the += operator.
     */
    HashSet<ValueType>& addAll(const HashSet<ValueType>& set);
    
    /*
     * Method: clear
     * Usage: set.clear();
     * -------------------
     * Removes all elements from this set.
     */
    void clear();
    
    /*
     * Method: contains
     * Usage: if (set.contains(value)) ...
     * -----------------------------------
     * Returns <code>true</code> if the specified value is in this set.
     */
    bool contains(const ValueType& value) const;
    
    /*
     * Method: equals
     * Usage: if (set.equals(set2)) ...
     * -----------------------------------
     * Returns <code>true</code> if this set contains exactly the same values
     * as the given other set.
     * Identical in behavior to the == operator.
     */
    bool equals(const HashSet<ValueType>& set2) const;
    
    /*
     * Method: first
     * Usage: ValueType value = set.first();
     * -------------------------------------
     * Returns the first value in the set in the order established by the
     * <code>foreach</code> macro.  If the set is empty, <code>first</code>
     * generates an error.
     */
    ValueType first() const;

    /*
     * Method: insert
     * Usage: set.insert(value);
     * -------------------------
     * Adds an element to this set, if it was not already there.  This
     * method is exported for compatibility with the STL <code>set</code> class.
     */
    void insert(const ValueType& value);

    /*
     * Method: isEmpty
     * Usage: if (set.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this set contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: isSubsetOf
     * Usage: if (set.isSubsetOf(set2)) ...
     * ------------------------------------
     * Implements the subset relation on sets.  It returns
     * <code>true</code> if every element of this set is
     * contained in <code>set2</code>.
     */
    bool isSubsetOf(const HashSet& set2) const;
    
    /*
     * Method: mapAll
     * Usage: set.mapAll(fn);
     * ----------------------
     * Iterates through the elements of the set and calls <code>fn(value)</code>
     * for each one.  The values are processed in ascending order, as defined
     * by the comparison function.
     */
    void mapAll(void (*fn)(ValueType)) const;
    void mapAll(void (*fn)(const ValueType&)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;
    
    /*
     * Method: remove
     * Usage: set.remove(value);
     * -------------------------
     * Removes an element from this set.  If the value was not
     * contained in the set, no error is generated and the set
     * remains unchanged.
     */
    void remove(const ValueType& value);
    
    /*
     * Method: removeAll
     * Usage: set.removeAll(set2);
     * ---------------------------
     * Removes all elements of the given other set from this set.
     * Returns a reference to this set.
     * Identical in behavior to the -= operator.
     */
    HashSet<ValueType>& removeAll(const HashSet<ValueType>& set);
    
    /*
     * Method: retainAll
     * Usage: set.retainAll(set2);
     * ----------------------
     * Removes all elements from this set that are not contained in the given
     * other set. Returns a reference to this set.
     * Identical in behavior to the *= operator.
     */
    HashSet<ValueType>& retainAll(const HashSet<ValueType>& set);

    /*
     * Method: size
     * Usage: count = set.size();
     * --------------------------
     * Returns the number of elements in this set.
     */
    int size() const;
    
    /*
     * Method: toString
     * Usage: string str = set.toString();
     * -----------------------------------
     * Converts the set to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: ==
     * Usage: set1 == set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * contain the same elements.
     */
    bool operator ==(const HashSet& set2) const;

    /*
     * Operator: !=
     * Usage: set1 != set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * are different.
     */
    bool operator !=(const HashSet& set2) const;

    /*
     * Operator: +
     * Usage: set1 + set2
     *        set1 + element
     * ---------------------
     * Returns the union of sets <code>set1</code> and <code>set2</code>, which
     * is the set of elements that appear in at least one of the two sets.  The
     * right hand set can be replaced by an element of the value type, in which
     * case the operator returns a new set formed by adding that element.
     */
    HashSet operator +(const HashSet& set2) const;
    HashSet operator +(const ValueType& element) const;

    /*
     * Operator: *
     * Usage: set1 * set2
     * ------------------
     * Returns the intersection of sets <code>set1</code> and <code>set2</code>,
     * which is the set of all elements that appear in both.
     */
    HashSet operator *(const HashSet& set2) const;

    /*
     * Operator: -
     * Usage: set1 - set2
     *        set1 - element
     * ---------------------
     * Returns the difference of sets <code>set1</code> and <code>set2</code>,
     * which is all of the elements that appear in <code>set1</code> but
     * not <code>set2</code>.  The right hand set can be replaced by an
     * element of the value type, in which case the operator returns a new
     * set formed by removing that element.
     */
    HashSet operator -(const HashSet& set2) const;
    HashSet operator -(const ValueType& element) const;

    /*
     * Operator: +=
     * Usage: set1 += set2;
     *        set1 += value;
     * ---------------------
     * Adds all of the elements from <code>set2</code> (or the single
     * specified value) to <code>set1</code>.  As a convenience, the
     * <code>HashSet</code> package also overloads the comma operator so
     * that it is possible to initialize a set like this:
     *
     *<pre>
     *    HashSet&lt;int&lt; digits;
     *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
     *</pre>
     */
    HashSet& operator +=(const HashSet& set2);
    HashSet& operator +=(const ValueType& value);

    /*
     * Operator: *=
     * Usage: set1 *= set2;
     * --------------------
     * Removes any elements from <code>set1</code> that are not present in
     * <code>set2</code>.
     */
    HashSet& operator *=(const HashSet& set2);

    /*
     * Operator: -=
     * Usage: set1 -= set2;
     *        set1 -= value;
     * ---------------------
     * Removes the elements from <code>set2</code> (or the single
     * specified value) from <code>set1</code>.  As a convenience, the
     * <code>HashSet</code> package also overloads the comma operator so
     * that it is possible to remove multiple elements from a set
     * like this:
     *
     *<pre>
     *    digits -= 0, 2, 4, 6, 8;
     *</pre>
     *
     * which removes the values 0, 2, 4, 6, and 8 from the set
     * <code>digits</code>.
     */
    HashSet& operator -=(const HashSet& set2);
    HashSet& operator -=(const ValueType& value);

    /*
     * Additional HashSet operations
     * -----------------------------
     * In addition to the methods listed in this interface, the HashSet
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * The iteration forms process the HashSet in an unspecified order.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    HashMap<ValueType, bool> map;        /* Map used to store the element     */
    bool removeFlag;                     /* Flag to differentiate += and -=   */

public:
    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support the comma operator, deep copying, and iteration.
     * Including these methods in the public interface would make
     * that interface more difficult to understand for the average client.
     */
    HashSet& operator ,(const ValueType& value) {
        if (this->removeFlag) {
            this->remove(value);
        } else {
            this->add(value);
        }
        return *this;
    }

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::iterator<std::input_iterator_tag,ValueType> {
    private:
        typename HashMap<ValueType,bool>::iterator mapit;

    public:
        iterator() {
            /* Empty */
        }

        iterator(typename HashMap<ValueType, bool>::iterator it) : mapit(it) {
            /* Empty */
        }

        iterator(const iterator& it) {
            mapit = it.mapit;
        }

        iterator& operator ++() {
            ++mapit;
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return mapit == rhs.mapit;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        ValueType& operator *() {
            return *mapit;
        }

        ValueType* operator ->() {
            return mapit;
        }
    };

    iterator begin() const {
        return iterator(map.begin());
    }

    iterator end() const {
        return iterator(map.end());
    }
};

template <typename ValueType>
HashSet<ValueType>::HashSet() : removeFlag(false) {
    /* Empty */
}

template <typename ValueType>
HashSet<ValueType>::~HashSet() {
    /* Empty */
}

template <typename ValueType>
void HashSet<ValueType>::add(const ValueType& value) {
    map.put(value, true);
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::addAll(const HashSet& set2) {
    for (ValueType value : set2) {
        this->add(value);
    }
    return *this;
}

template <typename ValueType>
void HashSet<ValueType>::clear() {
    map.clear();
}

template <typename ValueType>
bool HashSet<ValueType>::contains(const ValueType& value) const {
    return map.containsKey(value);
}

template <typename ValueType>
bool HashSet<ValueType>::equals(const HashSet<ValueType>& set2) const {
    // optimization: if literally same set, stop
    if (this == &set2) {
        return true;
    }
    
    if (size() != set2.size()) {
        return false;
    }
    
    return isSubsetOf(set2) && set2.isSubsetOf(*this);
}

template <typename ValueType>
ValueType HashSet<ValueType>::first() const {
    if (isEmpty()) {
        error("HashSet::first: set is empty");
    }
    return *begin();
}

template <typename ValueType>
void HashSet<ValueType>::insert(const ValueType& value) {
    map.put(value, true);
}

template <typename ValueType>
bool HashSet<ValueType>::isEmpty() const {
    return map.isEmpty();
}

template <typename ValueType>
bool HashSet<ValueType>::isSubsetOf(const HashSet& set2) const {
    iterator it = begin();
    iterator end = this->end();
    while (it != end) {
        if (!set2.map.containsKey(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(ValueType)) const {
    map.mapAll(fn);
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(const ValueType&)) const {
    map.mapAll(fn);
}

template <typename ValueType>
template <typename FunctorType>
void HashSet<ValueType>::mapAll(FunctorType fn) const {
    map.mapAll(fn);
}

template <typename ValueType>
void HashSet<ValueType>::remove(const ValueType& value) {
    map.remove(value);
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::removeAll(const HashSet& set2) {
    Vector<ValueType> toRemove;
    for (ValueType value : *this) {
        if (set2.map.containsKey(value)) {
            toRemove.add(value);
        }
    }
    for (ValueType value : toRemove) {
        this->remove(value);
    }
    return *this;
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::retainAll(const HashSet& set2) {
    Vector<ValueType> toRemove;
    for (ValueType value : *this) {
        if (!set2.map.containsKey(value)) {
            toRemove.add(value);
        }
    }
    for (ValueType value : toRemove) {
        this->remove(value);
    }
    return *this;
}

template <typename ValueType>
int HashSet<ValueType>::size() const {
    return map.size();
}

template <typename ValueType>
std::string HashSet<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: set operators
 * -----------------------------------
 * The implementations for the set operators use iteration to walk
 * over the elements in one or both sets.
 */
template <typename ValueType>
bool HashSet<ValueType>::operator ==(const HashSet& set2) const {
    return equals(set2);
}

template <typename ValueType>
bool HashSet<ValueType>::operator !=(const HashSet& set2) const {
    return !equals(set2);
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator +(const HashSet& set2) const {
    HashSet<ValueType> set = *this;
    set.addAll(set2);
    return set;
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator +(const ValueType& element) const {
    HashSet<ValueType> set = *this;
    set.add(element);
    return set;
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator *(const HashSet& set2) const {
    HashSet<ValueType> set = *this;
    return set.retainAll(set2);
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator -(const HashSet& set2) const {
    HashSet<ValueType> set = *this;
    return set.removeAll(set2);
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator -(const ValueType& element) const {
    HashSet<ValueType> set = *this;
    set.remove(element);
    return set;
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::operator +=(const HashSet& set2) {
    return addAll(set2);
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::operator +=(const ValueType& value) {
    add(value);
    removeFlag = false;
    return *this;
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::operator *=(const HashSet& set2) {
    return retainAll(set2);
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::operator -=(const HashSet& set2) {
    return removeAll(set2);
}

template <typename ValueType>
HashSet<ValueType>& HashSet<ValueType>::operator -=(const ValueType& value) {
    remove(value);
    removeFlag = true;
    return *this;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const HashSet<ValueType>& set) {
    os << "{";
    bool started = false;
    for (ValueType value : set) {
        if (started) {
            os << ", ";
        }
        writeGenericValue(os, value, true);
        started = true;
    }
    os << "}";
    return os;
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, HashSet<ValueType>& set) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
        error("HashSet::operator >>: Missing {");
    }
    set.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            ValueType value;
            readGenericValue(is, value);
            set += value;
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("HashSet::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

/*
 * Template hash function for hash sets.
 * Requires the element type in the HashSet to have a hashCode function.
 */
template <typename T>
int hashCode(const HashSet<T>& s) {
    int code = hashSeed();
    for (T n : s) {
        code = hashMultiplier() * code + hashCode(n);
    }
    return int(code & hashMask());
}

/*
 * Function: randomElement
 * Usage: element = randomElement(set);
 * ------------------------------------
 * Returns a randomly chosen element of the given set.
 * Throws an error if the set is empty.
 */
template <typename T>
const T& randomElement(const HashSet<T>& set) {
    if (set.isEmpty()) {
        error("randomElement: empty hash set was passed");
    }
    int index = randomInteger(0, set.size() - 1);
    int i = 0;
    for (const T& element : set) {
        if (i == index) {
            return element;
        }
        i++;
    }
    
    // this code will never be reached
    static T unused = set.first();
    return unused;
}

#endif
