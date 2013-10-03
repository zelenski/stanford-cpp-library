/*
 * File: hashset.h
 * ---------------
 * This file exports the <code>HashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 */

#ifndef _hashset_h
#define _hashset_h

#include <iostream>
#include "foreach.h"
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
 * Method: size
 * Usage: count = set.size();
 * --------------------------
 * Returns the number of elements in this set.
 */

   int size() const;

/*
 * Method: isEmpty
 * Usage: if (set.isEmpty()) ...
 * -----------------------------
 * Returns <code>true</code> if this set contains no elements.
 */

   bool isEmpty() const;

/*
 * Method: add
 * Usage: set.add(value);
 * ----------------------
 * Adds an element to this set, if it was not already there.  For
 * compatibility with the STL <code>set</code> class, this method
 * is also exported as <code>insert</code>.
 */

   void add(const ValueType & value);
   void insert(const ValueType & value);

/*
 * Method: remove
 * Usage: set.remove(value);
 * -------------------------
 * Removes an element from this set.  If the value was not
 * contained in the set, no error is generated and the set
 * remains unchanged.
 */

   void remove(const ValueType & value);

/*
 * Method: contains
 * Usage: if (set.contains(value)) ...
 * -----------------------------------
 * Returns <code>true</code> if the specified value is in this set.
 */

   bool contains(const ValueType & value) const;

/*
 * Method: isSubsetOf
 * Usage: if (set.isSubsetOf(set2)) ...
 * ------------------------------------
 * Implements the subset relation on sets.  It returns
 * <code>true</code> if every element of this set is
 * contained in <code>set2</code>.
 */

   bool isSubsetOf(const HashSet & set2) const;

/*
 * Method: clear
 * Usage: set.clear();
 * -------------------
 * Removes all elements from this set.
 */

   void clear();

/*
 * Operator: ==
 * Usage: set1 == set2
 * -------------------
 * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
 * contain the same elements.
 */

   bool operator==(const HashSet & set2) const;

/*
 * Operator: !=
 * Usage: set1 != set2
 * -------------------
 * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
 * are different.
 */

   bool operator!=(const HashSet & set2) const;

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

   HashSet operator+(const HashSet & set2) const;
   HashSet operator+(const ValueType & element) const;

/*
 * Operator: *
 * Usage: set1 * set2
 * ------------------
 * Returns the intersection of sets <code>set1</code> and <code>set2</code>,
 * which is the set of all elements that appear in both.
 */

   HashSet operator*(const HashSet & set2) const;

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

   HashSet operator-(const HashSet & set2) const;
   HashSet operator-(const ValueType & element) const;

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

   HashSet & operator+=(const HashSet & set2);
   HashSet & operator+=(const ValueType & value);

/*
 * Operator: *=
 * Usage: set1 *= set2;
 * --------------------
 * Removes any elements from <code>set1</code> that are not present in
 * <code>set2</code>.
 */

   HashSet & operator*=(const HashSet & set2);

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

   HashSet & operator-=(const HashSet & set2);
   HashSet & operator-=(const ValueType & value);

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
 * Method: toString
 * Usage: string str = set.toString();
 * -----------------------------------
 * Converts the set to a printable string representation.
 */

   std::string toString();

/*
 * Method: mapAll
 * Usage: set.mapAll(fn);
 * ----------------------
 * Iterates through the elements of the set and calls <code>fn(value)</code>
 * for each one.  The values are processed in ascending order, as defined
 * by the comparison function.
 */

   void mapAll(void (*fn)(ValueType)) const;
   void mapAll(void (*fn)(const ValueType &)) const;

   template <typename FunctorType>
   void mapAll(FunctorType fn) const;

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

   HashMap<ValueType,bool> map;        /* Map used to store the element     */
   bool removeFlag;                    /* Flag to differentiate += and -=   */

public:

/*
 * Hidden features
 * ---------------
 * The remainder of this file consists of the code required to
 * support the comma operator, deep copying, and iteration.
 * Including these methods in the public interface would make
 * that interface more difficult to understand for the average client.
 */

   HashSet & operator,(const ValueType & value) {
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

      iterator(typename HashMap<ValueType,bool>::iterator it) : mapit(it) {
         /* Empty */
      }

      iterator(const iterator & it) {
         mapit = it.mapit;
      }

      iterator & operator++() {
         ++mapit;
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      bool operator==(const iterator & rhs) {
         return mapit == rhs.mapit;
      }

      bool operator!=(const iterator & rhs) {
         return !(*this == rhs);
      }

      ValueType operator*() {
         return *mapit;
      }

      ValueType *operator->() {
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

extern void error(std::string msg);

template <typename ValueType>
HashSet<ValueType>::HashSet() {
   /* Empty */
}

template <typename ValueType>
HashSet<ValueType>::~HashSet() {
   /* Empty */
}

template <typename ValueType>
int HashSet<ValueType>::size() const {
   return map.size();
}

template <typename ValueType>
bool HashSet<ValueType>::isEmpty() const {
   return map.isEmpty();
}

template <typename ValueType>
void HashSet<ValueType>::add(const ValueType & value) {
   map.put(value, true);
}

template <typename ValueType>
void HashSet<ValueType>::insert(const ValueType & value) {
   map.put(value, true);
}

template <typename ValueType>
void HashSet<ValueType>::remove(const ValueType & value) {
   map.remove(value);
}

template <typename ValueType>
bool HashSet<ValueType>::contains(const ValueType & value) const {
   return map.containsKey(value);
}

template <typename ValueType>
void HashSet<ValueType>::clear() {
   map.clear();
}

template <typename ValueType>
bool HashSet<ValueType>::isSubsetOf(const HashSet & set2) const {
   iterator it = begin();
   iterator end = this->end();
   while (it != end) {
      if (!set2.map.containsKey(*it)) return false;
      ++it;
   }
   return true;
}

/*
 * Implementation notes: set operators
 * -----------------------------------
 * The implementations for the set operators use iteration to walk
 * over the elements in one or both sets.
 */

template <typename ValueType>
bool HashSet<ValueType>::operator==(const HashSet & set2) const {
   return this->isSubsetOf(set2) && set2.isSubsetOf(*this);
}

template <typename ValueType>
bool HashSet<ValueType>::operator!=(const HashSet & set2) const {
   return !(*this == set2);
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator+(const HashSet & set2) const {
   HashSet<ValueType> set = *this;
   foreach (ValueType value in set2) {
      set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator+(const ValueType & element) const {
   HashSet<ValueType> set = *this;
   set.add(element);
   return set;
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator*(const HashSet & set2) const {
   HashSet<ValueType> set;
   foreach (ValueType value in *this) {
      if (set2.map.containsKey(value)) set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator-(const HashSet & set2) const {
   HashSet<ValueType> set;
   foreach (ValueType value in *this) {
      if (!set2.map.containsKey(value)) set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator-(const ValueType & element) const {
   HashSet<ValueType> set = *this;
   set.remove(element);
   return set;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator+=(const HashSet & set2) {
   foreach (ValueType value in set2) {
      this->add(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator+=(const ValueType & value) {
   this->add(value);
   this->removeFlag = false;
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator*=(const HashSet & set2) {
   Vector<ValueType> toRemove;
   foreach (ValueType value in *this) {
      if (!set2.map.containsKey(value)) toRemove.add(value);
   }
   foreach (ValueType value in toRemove) {
      this->remove(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator-=(const HashSet & set2) {
   Vector<ValueType> toRemove;
   foreach (ValueType value in *this) {
      if (set2.map.containsKey(value)) toRemove.add(value);
   }
   foreach (ValueType value in toRemove) {
      this->remove(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator-=(const ValueType & value) {
   this->remove(value);
   this->removeFlag = true;
   return *this;
}

template <typename ValueType>
ValueType HashSet<ValueType>::first() const {
   if (isEmpty()) error("first: set is empty");
   return *begin();
}

template <typename ValueType>
std::string HashSet<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(ValueType)) const {
   map.mapAll(fn);
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(const ValueType &)) const {
   map.mapAll(fn);
}

template <typename ValueType>
template <typename FunctorType>
void HashSet<ValueType>::mapAll(FunctorType fn) const {
   map.mapAll(fn);
}

template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const HashSet<ValueType> & set) {
   os << "{";
   bool started = false;
   foreach (ValueType value in set) {
      if (started) os << ", ";
      writeGenericValue(os, value, true);
      started = true;
   }
   os << "}";
   return os;
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, HashSet<ValueType> & set) {
   char ch;
   is >> ch;
   if (ch != '{') error("operator >>: Missing {");
   set.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         set += value;
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

#endif
