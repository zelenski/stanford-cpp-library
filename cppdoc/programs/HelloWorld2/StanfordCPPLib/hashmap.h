/*
 * File: hashmap.h
 * ---------------
 * This file exports the <code>HashMap</code> class, which stores
 * a set of <i>key</i>-<i>value</i> pairs.
 */

#ifndef _hashmap_h
#define _hashmap_h

#include <cstdlib>
#include <string>
#include "foreach.h"
#include "vector.h"

/*
 * Function: hashCode
 * Usage: int hash = hashCode(key);
 * --------------------------------
 * Returns a hash code for the specified key, which is always a
 * nonnegative integer.  This function is overloaded to support
 * all of the primitive types and the C++ <code>string</code> type.
 */

int hashCode(const std::string & key);
int hashCode(int key);
int hashCode(char key);
int hashCode(long key);
int hashCode(double key);

/*
 * Class: HashMap<KeyType,ValueType>
 * ---------------------------------
 * This class implements an efficient association between
 * <b><i>keys</i></b> and <b><i>values</i></b>.  This class is
 * identical to the <a href="Map-class.html"><code>Map</code></a> class
 * except for the fact that it uses a hash table as its underlying
 * representation.  Although the <code>HashMap</code> class operates in
 * constant time, the iterator for <code>HashMap</code> returns the
 * values in a seemingly random order.
 */

template <typename KeyType, typename ValueType>
class HashMap {

public:

/*
 * Constructor: HashMap
 * Usage: HashMap<KeyType,ValueType> map;
 * --------------------------------------
 * Initializes a new empty map that associates keys and values of
 * the specified types.  The type used for the key must define
 * the <code>==</code> operator, and there must be a free function
 * with the following signature:
 *
 *<pre>
 *    int hashCode(KeyType key);
 *</pre>
 *
 * that returns a positive integer determined by the key.  This interface
 * exports <code>hashCode</code> functions for <code>string</code> and
 * the C++ primitive types.
 */

   HashMap();

/*
 * Destructor: ~HashMap
 * --------------------
 * Frees any heap storage associated with this map.
 */

   virtual ~HashMap();

/*
 * Method: size
 * Usage: int nEntries = map.size();
 * ---------------------------------
 * Returns the number of entries in this map.
 */

   int size() const;

/*
 * Method: isEmpty
 * Usage: if (map.isEmpty()) ...
 * -----------------------------
 * Returns <code>true</code> if this map contains no entries.
 */

   bool isEmpty() const;

/*
 * Method: put
 * Usage: map.put(key, value);
 * ---------------------------
 * Associates <code>key</code> with <code>value</code> in this map.
 * Any previous value associated with <code>key</code> is replaced
 * by the new value.
 */

   void put(KeyType key, ValueType value);

/*
 * Method: get
 * Usage: ValueType value = map.get(key);
 * --------------------------------------
 * Returns the value associated with <code>key</code> in this map.
 * If <code>key</code> is not found, <code>get</code> returns the
 * default value for <code>ValueType</code>.
 */

   ValueType get(KeyType key) const;

/*
 * Method: containsKey
 * Usage: if (map.containsKey(key)) ...
 * ------------------------------------
 * Returns <code>true</code> if there is an entry for <code>key</code>
 * in this map.
 */

   bool containsKey(KeyType key) const;

/*
 * Method: remove
 * Usage: map.remove(key);
 * -----------------------
 * Removes any entry for <code>key</code> from this map.
 */

   void remove(KeyType key);

/*
 * Method: clear
 * Usage: map.clear();
 * -------------------
 * Removes all entries from this map.
 */

   void clear();

/*
 * Operator: []
 * Usage: map[key]
 * ---------------
 * Selects the value associated with <code>key</code>.  This syntax
 * makes it easy to think of a map as an "associative array"
 * indexed by the key type.  If <code>key</code> is already present
 * in the map, this function returns a reference to its associated
 * value.  If key is not present in the map, a new entry is created
 * whose value is set to the default for the value type.
 */

   ValueType & operator[](KeyType key);
   ValueType operator[](KeyType key) const;

/*
 * Method: toString
 * Usage: string str = map.toString();
 * -----------------------------------
 * Converts the map to a printable string representation.
 */

   std::string toString();

/*
 * Method: mapAll
 * Usage: map.mapAll(fn);
 * ----------------------
 * Iterates through the map entries and calls <code>fn(key, value)</code>
 * for each one.  The keys are processed in an undetermined order.
 */

   void mapAll(void (*fn)(KeyType, ValueType)) const;
   void mapAll(void (*fn)(const KeyType &, const ValueType &)) const;
   template <typename FunctorType>
   void mapAll(FunctorType fn) const;

/*
 * Additional HashMap operations
 * -----------------------------
 * In addition to the methods listed in this interface, the HashMap
 * class supports the following operations:
 *
 *   - Stream I/O using the << and >> operators
 *   - Deep copying for the copy constructor and assignment operator
 *   - Iteration using the range-based for statement and STL iterators
 *
 * The HashMap class makes no guarantees about the order of iteration.
 */

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Implementation notes:
 * ---------------------
 * The HashMap class is represented using a hash table that uses
 * bucket chaining to resolve collisions.
 */

private:

/* Constant definitions */

   static const int INITIAL_BUCKET_COUNT = 101;
   static const int MAX_LOAD_PERCENTAGE = 70;

/* Type definition for cells in the bucket chain */

   struct Cell {
      KeyType key;
      ValueType value;
      Cell *next;
   };

/* Instance variables */

   Vector<Cell *> buckets;
   int nBuckets;
   int numEntries;

/* Private methods */

/*
 * Private method: createBuckets
 * Usage: createBuckets(nBuckets);
 * -------------------------------
 * Sets up the vector of buckets to have nBuckets entries, each NULL.
 * If asked to make empty vector, makes one bucket just to simplify
 * handling elsewhere.
 */

   void createBuckets(int nBuckets) {
      if (nBuckets == 0) nBuckets = 1;
      buckets = Vector<Cell *>(nBuckets, NULL);
      this->nBuckets = nBuckets;
      numEntries = 0;
   }

/*
 * Private method: deleteBuckets
 * Usage: deleteBuckets(buckets);
 * ------------------------------
 * Deletes all the cells in the linked lists contained in vector.
 */

   void deleteBuckets(Vector <Cell *> & buckets) {
      for (int i = 0; i < buckets.size(); i++) {
         Cell *cp = buckets[i];
         while (cp != NULL) {
            Cell *np = cp->next;
            delete cp;
            cp = np;
         }
         buckets[i] = NULL;
      }
   }

/*
 * Private method: expandAndRehash
 * Usage: expandAndRehash();
 * -------------------------
 * This method is used to increase the number of buckets in the map
 * and then rehashes all existing entries and adds them into new buckets.
 * This operation is used when the load factor (i.e. the number of cells
 * per bucket) has increased enough to warrant this O(N) operation to
 * enlarge and redistribute the entries.
 */

   void expandAndRehash() {
      Vector<Cell *>oldBuckets = buckets;
      createBuckets(oldBuckets.size() * 2 + 1);
      for (int i = 0; i < oldBuckets.size(); i++) {
         for (Cell *cp = oldBuckets[i]; cp != NULL; cp = cp->next) {
            put(cp->key, cp->value);
         }
      }
      deleteBuckets(oldBuckets);
   }

/*
 * Private method: findCell
 * Usage: Cell *cp = findCell(bucket, key);
 *        Cell *cp = findCell(bucket, key, parent);
 * ------------------------------------------------
 * Finds a cell in the chain for the specified bucket that matches key.
 * If a match is found, the return value is a pointer to the cell containing
 * the matching key.  If no match is found, the function returns NULL.
 * If the optional third argument is supplied, it is filled in with the
 * cell preceding the matching cell to allow the client to splice out
 * the target cell in the delete call.  If parent is NULL, it indicates
 * that the cell is the first cell in the bucket chain.
 */

   Cell *findCell(int bucket, KeyType key) const {
      Cell *dummy;
      return findCell(bucket, key, dummy);
   }

   Cell *findCell(int bucket, KeyType key, Cell * & parent) const {
      parent = NULL;
      Cell *cp = buckets.get(bucket);
      while (cp != NULL && key != cp->key) {
         parent = cp;
         cp = cp->next;
      }
      return cp;
   }

   void deepCopy(const HashMap & src) {
      createBuckets(src.nBuckets);
      for (int i = 0; i < src.nBuckets; i++) {
         for (Cell *cp = src.buckets.get(i); cp != NULL; cp = cp->next) {
            put(cp->key, cp->value);
         }
      }
   }

public:

/*
 * Hidden features
 * ---------------
 * The remainder of this file consists of the code required to
 * support deep copying and iteration.  Including these methods
 * in the public interface would make that interface more
 * difficult to understand for the average client.
 */

/*
 * Deep copying support
 * --------------------
 * This copy constructor and operator= are defined to make a
 * deep copy, making it possible to pass/return maps by value
 * and assign from one map to another.
 */

   HashMap & operator=(const HashMap & src) {
      if (this != &src) {
         clear();
         deepCopy(src);
      }
      return *this;
   }

   HashMap(const HashMap & src) {
      deepCopy(src);
   }

/*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input
 * iterators so that they work symmetrically with respect to the
 * corresponding STL classes.
 */

   class iterator : public std::iterator<std::input_iterator_tag,KeyType> {

   private:

      const HashMap *mp;           /* Pointer to the map           */
      int bucket;                  /* Index of current bucket      */
      Cell *cp;                    /* Current cell in bucket chain */

   public:

      iterator() {
        /* Empty */
      }

      iterator(const HashMap *mp, bool end) {
         this->mp = mp;
         if (end) {
            bucket = mp->nBuckets;
            cp = NULL;
         } else {
            bucket = 0;
            cp = mp->buckets.get(bucket);
            while (cp == NULL && ++bucket < mp->nBuckets) {
               cp = mp->buckets.get(bucket);
            }
         }
      }

      iterator(const iterator & it) {
         mp = it.mp;
         bucket = it.bucket;
         cp = it.cp;
      }

      iterator & operator++() {
         cp = cp->next;
         while (cp == NULL && ++bucket < mp->nBuckets) {
            cp = mp->buckets.get(bucket);
         }
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      bool operator==(const iterator & rhs) {
         return mp == rhs.mp && bucket == rhs.bucket && cp == rhs.cp;
      }

      bool operator!=(const iterator & rhs) {
         return !(*this == rhs);
      }

      KeyType operator*() {
         return cp->key;
      }

      KeyType *operator->() {
         return &cp->key;
      }

      friend class HashMap;

   };

   iterator begin() const {
      return iterator(this, false);
   }

   iterator end() const {
      return iterator(this, true);
   }

};

/*
 * Implementation notes: HashMap class
 * -----------------------------------
 * In this map implementation, the entries are stored in a hashtable.
 * The hashtable keeps a vector of "buckets", where each bucket is a
 * linked list of elements that share the same hash code (i.e. hash
 * collisions are resolved by chaining). The buckets are dynamically
 * allocated so that we can change the the number of buckets (rehash)
 * when the load factor becomes too high. The map should provide O(1)
 * performance on the put/remove/get operations.
 */

template <typename KeyType,typename ValueType>
HashMap<KeyType,ValueType>::HashMap() {
   createBuckets(INITIAL_BUCKET_COUNT);
}

template <typename KeyType,typename ValueType>
HashMap<KeyType,ValueType>::~HashMap() {
   deleteBuckets(buckets);
}

template <typename KeyType,typename ValueType>
int HashMap<KeyType,ValueType>::size() const {
   return numEntries;
}

template <typename KeyType,typename ValueType>
bool HashMap<KeyType,ValueType>::isEmpty() const {
   return size() == 0;
}

template <typename KeyType,typename ValueType>
void HashMap<KeyType,ValueType>::put(KeyType key, ValueType value) {
   (*this)[key] = value;
}

template <typename KeyType,typename ValueType>
ValueType HashMap<KeyType,ValueType>::get(KeyType key) const {
   Cell *cp = findCell(hashCode(key) % nBuckets, key);
   if (cp == NULL) return ValueType();
   return cp->value;
}

template <typename KeyType,typename ValueType>
bool HashMap<KeyType,ValueType>::containsKey(KeyType key) const {
   return findCell(hashCode(key) % nBuckets, key) != NULL;
}

template <typename KeyType,typename ValueType>
void HashMap<KeyType,ValueType>::remove(KeyType key) {
   int bucket = hashCode(key) % nBuckets;
   Cell *parent;
   Cell *cp = findCell(bucket, key, parent);
   if (cp != NULL) {
      if (parent == NULL) {
         buckets[bucket] = cp->next;
      } else {
         parent->next = cp->next;
      }
      delete cp;
      numEntries--;
   }
}

template <typename KeyType,typename ValueType>
void HashMap<KeyType,ValueType>::clear() {
   deleteBuckets(buckets);
   numEntries = 0;
}

template <typename KeyType,typename ValueType>
ValueType & HashMap<KeyType,ValueType>::operator[](KeyType key) {
   int bucket = hashCode(key) % nBuckets;
   Cell *cp = findCell(bucket, key);
   if (cp == NULL) {
      if (numEntries > MAX_LOAD_PERCENTAGE * nBuckets / 100.0) {
         expandAndRehash();
         bucket = hashCode(key) % nBuckets;
      }
      cp = new Cell;
      cp->key = key;
      cp->value = ValueType();
      cp->next = buckets[bucket];
      buckets[bucket] = cp;
      numEntries++;
   }
   return cp->value;
}

template <typename KeyType,typename ValueType>
void HashMap<KeyType,ValueType>::mapAll(void (*fn)(KeyType, ValueType)) const {
   for (int i = 0 ; i < buckets.size(); i++) {
      for (Cell *cp = buckets.get(i); cp != NULL; cp = cp->next) {
         fn(cp->key, cp->value);
      }
   }
}

template <typename KeyType,typename ValueType>
void HashMap<KeyType,ValueType>::mapAll(void (*fn)(const KeyType &,
                                                   const ValueType &)) const {
   for (int i = 0 ; i < buckets.size(); i++) {
      for (Cell *cp = buckets.get(i); cp != NULL; cp = cp->next) {
         fn(cp->key, cp->value);
      }
   }
}

template <typename KeyType,typename ValueType>
template <typename FunctorType>
void HashMap<KeyType,ValueType>::mapAll(FunctorType fn) const {
   for (int i = 0 ; i < buckets.size(); i++) {
      for (Cell *cp = buckets.get(i); cp != NULL; cp = cp->next) {
         fn(cp->key, cp->value);
      }
   }
}

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType,ValueType>::operator[](KeyType key) const {
   return get(key);
}

template <typename KeyType, typename ValueType>
std::string HashMap<KeyType,ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */

template <typename KeyType, typename ValueType>
std::ostream & operator<<(std::ostream & os,
                          const HashMap<KeyType,ValueType> & map) {
   os << "{";
   typename HashMap<KeyType,ValueType>::iterator begin = map.begin();
   typename HashMap<KeyType,ValueType>::iterator end = map.end();
   typename HashMap<KeyType,ValueType>::iterator it = begin;
   while (it != end) {
      if (it != begin) os << ", ";
      writeGenericValue(os, *it, false);
      os << ":";
      writeGenericValue(os, map[*it], false);
      ++it;
   }
   return os << "}";
}

template <typename KeyType, typename ValueType>
std::istream & operator>>(std::istream & is,
                          HashMap<KeyType,ValueType> & map) {
   char ch;
   is >> ch;
   if (ch != '{') error("operator >>: Missing {");
   map.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         KeyType key;
         readGenericValue(is, key);
         is >> ch;
         if (ch != ':') error("operator >>: Missing colon after key");
         ValueType value;
         readGenericValue(is, value);
         map[key] = value;
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
