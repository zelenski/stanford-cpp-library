/*
 * File: hashmap.h
 * ---------------
 * This file exports the <code>HashMap</code> class, which stores
 * a set of <i>key</i>-<i>value</i> pairs.
 * 
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/10
 * - added support for std initializer_list usage, such as
 *   {{"a", 1}, {"b", 2}, {"c", 3}} in constructor, putAll, removeAll, retainAll,
 *   operators +, +=, -, -=, *, *=
 * - added addAll method
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * - fixed bug where string quotes would not show when map was printed
 * @version 2015/06/19
 * - fixed deepCopy code that was causing copies to have different hash code than
 *   the original they were copied from (credit to SL Wen Zhang for finding the bug)
 * @version 2014/11/13
 * - added add() method as synonym for put()
 * - added template hashCode function
 * - moved hashCode functions to hashcode.h/cpp
 * @version 2014/10/29
 * - moved hashCode functions out to hashcode.h
 * @version 2014/10/10
 * - added comparison operators ==, !=
 */

#ifndef _hashmap_h
#define _hashmap_h

#include <cstdlib>
#include <initializer_list>
#include <map>
#include <string>
#include <utility>
#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "vector.h"

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
     * Constructor: HashMap
     * Usage: HashMap<ValueType> map {{"a", 1}, {"b", 2}, {"c", 3}};
     * -------------------------------------------------------------
     * Initializes a new map that stores the given pairs.
     * Note that the pairs are stored in unpredictable order internally and not
     * necessarily the order in which they are written in the initializer list.
     */
    HashMap(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Destructor: ~HashMap
     * --------------------
     * Frees any heap storage associated with this map.
     */
    virtual ~HashMap();

    /*
     * Method: add
     * Usage: map.add(key, value);
     * ---------------------------
     * Associates <code>key</code> with <code>value</code> in this map.
     * A synonym for the put method.
     */
    void add(const KeyType& key, const ValueType& value);

    /*
     * Method: addAll
     * Usage: map.addAll(map2);
     * ------------------------
     * Adds all key/value pairs from the given map to this map.
     * If both maps contain a pair for the same key, the one from map2 will
     * replace the one from this map.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     * Returns a reference to this map.
     * Identical in behavior to putAll.
     */
    HashMap& addAll(const HashMap& map2);
    HashMap& addAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Method: clear
     * Usage: map.clear();
     * -------------------
     * Removes all entries from this map.
     */
    void clear();

    /*
     * Method: containsKey
     * Usage: if (map.containsKey(key)) ...
     * ------------------------------------
     * Returns <code>true</code> if there is an entry for <code>key</code>
     * in this map.
     */
    bool containsKey(const KeyType& key) const;

    /*
     * Method: equals
     * Usage: if (map.equals(map2)) ...
     * --------------------------------
     * Returns <code>true</code> if the two maps contain exactly the same
     * key/value pairs, and <code>false</code> otherwise.
     */
    bool equals(const HashMap& map2) const;

    /*
     * Method: get
     * Usage: ValueType value = map.get(key);
     * --------------------------------------
     * Returns the value associated with <code>key</code> in this map.
     * If <code>key</code> is not found, <code>get</code> returns the
     * default value for <code>ValueType</code>.
     */
    ValueType get(const KeyType& key) const;

    /*
     * Method: isEmpty
     * Usage: if (map.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this map contains no entries.
     */
    bool isEmpty() const;
    
    /*
     * Method: keys
     * Usage: Vector<KeyType> keys = map.keys();
     * -----------------------------------------
     * Returns a collection containing all keys in this map.
     * Note that this implementation makes a deep copy of the keys,
     * so it is inefficient to call on large maps.
     */
    Vector<KeyType> keys() const;

    /*
     * Method: mapAll
     * Usage: map.mapAll(fn);
     * ----------------------
     * Iterates through the map entries and calls <code>fn(key, value)</code>
     * for each one.  The keys are processed in an undetermined order.
     */
    void mapAll(void (*fn)(KeyType, ValueType)) const;
    void mapAll(void (*fn)(const KeyType&, const ValueType&)) const;
    
    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    /*
     * Method: put
     * Usage: map.put(key, value);
     * ---------------------------
     * Associates <code>key</code> with <code>value</code> in this map.
     * Any previous value associated with <code>key</code> is replaced
     * by the new value.
     */
    void put(const KeyType& key, const ValueType& value);

    /*
     * Method: putAll
     * Usage: map.putAll(map2);
     * ---------------------------
     * Adds all key/value pairs from the given map to this map.
     * If both maps contain a pair for the same key, the one from map2 will
     * replace the one from this map.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     * Returns a reference to this map.
     */
    HashMap& putAll(const HashMap& map2);
    HashMap& putAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Method: remove
     * Usage: map.remove(key);
     * -----------------------
     * Removes any entry for <code>key</code> from this map.
     * If the given key is not found, has no effect.
     */
    void remove(const KeyType& key);

    /*
     * Method: removeAll
     * Usage: map.removeAll(map2);
     * ---------------------------
     * Removes all key/value pairs from this map that are contained in the given map.
     * If both maps contain the same key but it maps to different values, that
     * mapping will not be removed.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     * Returns a reference to this map.
     */
    HashMap& removeAll(const HashMap& map2);
    HashMap& removeAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Method: retainAll
     * Usage: map.retainAll(map2);
     * ---------------------------
     * Removes all key/value pairs from this map that are not contained in the given map.
     * If both maps contain the same key but it maps to different values, that
     * mapping will be removed.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     * Returns a reference to this map.
     */
    HashMap& retainAll(const HashMap& map2);
    HashMap& retainAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Method: size
     * Usage: int nEntries = map.size();
     * ---------------------------------
     * Returns the number of entries in this map.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = map.toString();
     * -----------------------------------
     * Converts the map to a printable string representation.
     */
    std::string toString() const;

    /*
     * Method: values
     * Usage: Vector<ValueType> values = map.values();
     * -----------------------------------------------
     * Returns a collection containing all values in this map.
     * Note that this implementation makes a deep copy of the values,
     * so it is inefficient to call on large maps.
     */
    Vector<ValueType> values() const;

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
    ValueType& operator [](const KeyType& key);
    ValueType operator [](const KeyType& key) const;

    /*
     * Operator: ==
     * Usage: if (map1 == map2) ...
     * ----------------------------
     * Compares two maps for equality.
     */
    bool operator ==(const HashMap& map2) const;

    /*
     * Operator: !=
     * Usage: if (map1 != map2) ...
     * ----------------------------
     * Compares two maps for inequality.
     */
    bool operator !=(const HashMap& map2) const;

    /*
     * Operator: +
     * Usage: map1 + map2
     * ------------------
     * Returns the union of the two maps, equivalent to a copy of the first map
     * with addAll called on it passing the second map as a parameter.
     * If the two maps both contain a mapping for the same key, the mapping
     * from the second map is favored.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator +(const HashMap& map2) const;
    HashMap operator +(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: +=
     * Usage: map1 += map2;
     * --------------------
     * Adds all key/value pairs from the given map to this map.
     * Equivalent to calling addAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator +=(const HashMap& map2);
    HashMap& operator +=(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Operator: -
     * Usage: map1 - map2
     * ------------------
     * Returns the difference of the two maps, equivalent to a copy of the first map
     * with removeAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator -(const HashMap& map2) const;
    HashMap operator -(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: -=
     * Usage: map1 -= map2;
     * --------------------
     * Removes all key/value pairs from the given map to this map.
     * Equivalent to calling removeAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator -=(const HashMap& map2);
    HashMap& operator -=(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Operator: *
     * Usage: map1 * map2
     * ------------------
     * Returns the intersection of the two maps, equivalent to a copy of the first map
     * with retainAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator *(const HashMap& map2) const;
    HashMap operator *(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: *=
     * Usage: map1 *= map2;
     * ---------------------
     * Removes all key/value pairs that are not found in the given map from this map.
     * Equivalent to calling retainAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator *=(const HashMap& map2);
    HashMap& operator *=(std::initializer_list<std::pair<KeyType, ValueType> > list);

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
        Cell* next;
    };

    /* Instance variables */
    Vector<Cell*> buckets;
    int nBuckets;
    int numEntries;

    /* Private methods */

    /*
     * Private method: createBuckets
     * Usage: createBuckets(nBuckets);
     * -------------------------------
     * Sets up the vector of buckets to have nBuckets entries, each null.
     * If asked to make empty vector, makes one bucket just to simplify
     * handling elsewhere.
     */
    void createBuckets(int nBuckets) {
        if (nBuckets == 0) {
            nBuckets = 1;
        }
        buckets = Vector<Cell*>(nBuckets, nullptr);
        this->nBuckets = nBuckets;
        numEntries = 0;
    }

    /*
     * Private method: deleteBuckets
     * Usage: deleteBuckets(buckets);
     * ------------------------------
     * Deletes all the cells in the linked lists contained in vector.
     */
    void deleteBuckets(Vector<Cell*>& buckets) {
        for (int i = 0; i < buckets.size(); i++) {
            Cell* cp = buckets[i];
            while (cp) {
                Cell* np = cp->next;
                delete cp;
                cp = np;
            }
            buckets[i] = nullptr;
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
        Vector<Cell*> oldBuckets = buckets;
        createBuckets(oldBuckets.size() * 2 + 1);
        for (int i = 0; i < oldBuckets.size(); i++) {
            for (Cell* cp = oldBuckets[i]; cp != nullptr; cp = cp->next) {
                put(cp->key, cp->value);
            }
        }
        deleteBuckets(oldBuckets);
    }

    /*
     * Private method: findCell
     * Usage: Cell* cp = findCell(bucket, key);
     *        Cell* cp = findCell(bucket, key, parent);
     * ------------------------------------------------
     * Finds a cell in the chain for the specified bucket that matches key.
     * If a match is found, the return value is a pointer to the cell containing
     * the matching key.  If no match is found, the function returns nullptr.
     * If the optional third argument is supplied, it is filled in with the
     * cell preceding the matching cell to allow the client to splice out
     * the target cell in the delete call.  If parent is null, it indicates
     * that the cell is the first cell in the bucket chain.
     */
    Cell* findCell(int bucket, const KeyType& key) const {
        Cell *dummy;
        return findCell(bucket, key, dummy);
    }

    Cell* findCell(int bucket, const KeyType& key, Cell*& parent) const {
        parent = nullptr;
        Cell* cp = buckets.get(bucket);
        while (cp && !(key == cp->key)) {
            parent = cp;
            cp = cp->next;
        }
        return cp;
    }

    void deepCopy(const HashMap& src) {
        createBuckets(src.nBuckets);
        for (int i = 0; i < src.nBuckets; i++) {
            // BUGFIX: was just calling put(), which reversed the chains;
            // now deep-copy the chains exactly as they were to preserve hashcode
            Cell* endOfChain = nullptr;
            for (Cell* cp = src.buckets.get(i); cp != nullptr; cp = cp->next) {
                // put(cp->key, cp->value);
                
                // copy the cell and put at end of bucket list
                Cell* copy = new Cell();
                copy->key = cp->key;
                copy->value = cp->value;
                copy->next = nullptr;
                if (!endOfChain) {
                    // first node in bucket
                    buckets.set(i, copy);
                } else {
                    // not first node; put after existing node
                    endOfChain->next = copy;
                    endOfChain = copy;
                }
                endOfChain = copy;
                numEntries++;
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
    HashMap& operator =(const HashMap& src) {
        if (this != &src) {
            clear();
            deepCopy(src);
        }
        return *this;
    }

    HashMap(const HashMap& src) {
        deepCopy(src);
    }

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::iterator<std::input_iterator_tag, KeyType> {
    private:
        const HashMap* mp;           /* Pointer to the map           */
        int bucket;                  /* Index of current bucket      */
        Cell* cp;                    /* Current cell in bucket chain */

    public:
        iterator() : mp(nullptr), bucket(0), cp(0) {
            /* Empty */
        }

        iterator(const HashMap* mp, bool end) {
            this->mp = mp;
            if (end) {
                bucket = mp->nBuckets;
                cp = nullptr;
            } else {
                bucket = 0;
                cp = mp->buckets.get(bucket);
                while (!cp && ++bucket < mp->nBuckets) {
                    cp = mp->buckets.get(bucket);
                }
            }
        }

        iterator(const iterator& it) {
            mp = it.mp;
            bucket = it.bucket;
            cp = it.cp;
        }

        iterator& operator ++() {
            cp = cp->next;
            while (!cp && ++bucket < mp->nBuckets) {
                cp = mp->buckets.get(bucket);
            }
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return mp == rhs.mp && bucket == rhs.bucket && cp == rhs.cp;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        KeyType& operator *() {
            return cp->key;
        }

        KeyType* operator ->() {
            return &cp->key;
        }

        friend class HashMap;
    };

    /*
     * Returns an iterator positioned at the first key of the map.
     */
    iterator begin() const {
        return iterator(this, /* end */ false);
    }

    /*
     * Returns an iterator positioned at the last key of the map.
     */
    iterator end() const {
        return iterator(this, /* end */ true);
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
template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::HashMap() {
    createBuckets(INITIAL_BUCKET_COUNT);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::HashMap(std::initializer_list<std::pair<KeyType, ValueType> > list) {
    createBuckets(INITIAL_BUCKET_COUNT);
    putAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::~HashMap() {
    deleteBuckets(buckets);
    numEntries = 0;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    put(key, value);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::addAll(const HashMap& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::addAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return putAll(list);
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::clear() {
    deleteBuckets(buckets);
    numEntries = 0;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return findCell(hashCode(key) % nBuckets, key) != nullptr;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::equals(const HashMap<KeyType, ValueType>& map2) const {
    return stanfordcpplib::collections::equalsMap(*this, map2);
}

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType, ValueType>::get(const KeyType& key) const {
    Cell* cp = findCell(hashCode(key) % nBuckets, key);
    if (!cp) {
        return ValueType();
    }
    return cp->value;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::isEmpty() const {
    return size() == 0;
}

template <typename KeyType, typename ValueType>
Vector<KeyType> HashMap<KeyType, ValueType>::keys() const {
    Vector<KeyType> keyset;
    for (KeyType key : *this) {
        keyset.add(key);
    }
    return keyset;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::mapAll(void (*fn)(KeyType, ValueType)) const {
    for (int i = 0; i < buckets.size(); i++) {
        for (Cell* cp = buckets.get(i); cp != nullptr; cp = cp->next) {
            fn(cp->key, cp->value);
        }
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::mapAll(void (*fn)(const KeyType&,
                                                   const ValueType&)) const {
    for (int i = 0; i < buckets.size(); i++) {
        for (Cell* cp = buckets.get(i); cp != nullptr; cp = cp->next) {
            fn(cp->key, cp->value);
        }
    }
}

template <typename KeyType, typename ValueType>
template <typename FunctorType>
void HashMap<KeyType, ValueType>::mapAll(FunctorType fn) const {
    for (int i = 0; i < buckets.size(); i++) {
        for (Cell* cp = buckets.get(i); cp != nullptr; cp = cp->next) {
            fn(cp->key, cp->value);
        }
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    (*this)[key] = value;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::putAll(const HashMap& map2) {
    for (KeyType key : map2) {
        put(key, map2.get(key));
    }
    return *this;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::putAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    for (std::pair<KeyType, ValueType> pair : list) {
        put(pair.first, pair.second);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::remove(const KeyType& key) {
    int bucket = hashCode(key) % nBuckets;
    Cell *parent;
    Cell* cp = findCell(bucket, key, parent);
    if (cp) {
        if (!parent) {
            buckets[bucket] = cp->next;
        } else {
            parent->next = cp->next;
        }
        delete cp;
        numEntries--;
    }
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::removeAll(const HashMap& map2) {
    for (KeyType key : map2) {
        if (containsKey(key) && get(key) == map2.get(key)) {
            remove(key);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::removeAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    for (std::pair<KeyType, ValueType> pair : list) {
        if (containsKey(pair.first) && get(pair.first) == pair.second) {
            remove(pair.first);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::retainAll(const HashMap& map2) {
    Vector<KeyType> toRemove;
    for (KeyType key : *this) {
        if (!map2.containsKey(key) || get(key) != map2.get(key)) {
            toRemove.add(key);
        }
    }
    for (KeyType key : toRemove) {
        remove(key);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::retainAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    HashMap<KeyType, ValueType> map2(list);
    retainAll(map2);
    return *this;
}

template <typename KeyType, typename ValueType>
int HashMap<KeyType, ValueType>::size() const {
    return numEntries;
}

template <typename KeyType, typename ValueType>
std::string HashMap<KeyType, ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename KeyType, typename ValueType>
Vector<ValueType> HashMap<KeyType, ValueType>::values() const {
    Vector<ValueType> values;
    for (KeyType key : *this) {
        values.add(this->get(key));
    }
    return values;
}

template <typename KeyType, typename ValueType>
ValueType& HashMap<KeyType, ValueType>::operator [](const KeyType& key) {
    int bucket = hashCode(key) % nBuckets;
    Cell* cp = findCell(bucket, key);
    if (!cp) {
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

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType, ValueType>::operator [](const KeyType& key) const {
    return get(key);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator +(const HashMap& map2) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.putAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator +(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.putAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator +=(const HashMap& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator +=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return putAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator -(const HashMap& map2) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.removeAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator -(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.removeAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator -=(const HashMap& map2) {
    return removeAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator -=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return removeAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator *(const HashMap& map2) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.retainAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator *(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.retainAll(list);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator *=(const HashMap& map2) {
    return retainAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator *=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return retainAll(list);
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::operator ==(const HashMap& map2) const {
    return equals(map2);
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::operator !=(const HashMap& map2) const {
    return !equals(map2);   // BUGFIX 2016/09/24
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename KeyType, typename ValueType>
std::ostream& operator <<(std::ostream& os,
                          const HashMap<KeyType, ValueType>& map) {
    return stanfordcpplib::collections::writeMap(os, map);
}

template <typename KeyType, typename ValueType>
std::istream& operator >>(std::istream& is,
                          HashMap<KeyType, ValueType>& map) {
    KeyType key;
    ValueType value;
    return stanfordcpplib::collections::readMap(is, map, key, value, /* descriptor */ std::string("HashMap::operator >>"));
}

/*
 * Template hash function for hash maps.
 * Requires the key and value types in the HashMap to have a hashCode function.
 */
template <typename K, typename V>
int hashCode(const HashMap<K, V>& map) {
    return stanfordcpplib::collections::hashCodeMap(map, /* orderMatters */ false);
}

/*
 * Function: randomKey
 * Usage: element = randomKey(map);
 * --------------------------------
 * Returns a randomly chosen key of the given map.
 * Throws an error if the map is empty.
 */
template <typename K, typename V>
const K& randomKey(const HashMap<K, V>& map) {
    if (map.isEmpty()) {
        error("randomKey: empty hash map was passed");
    }
    int index = randomInteger(0, map.size() - 1);
    int i = 0;
    for (const K& key : map) {
        if (i == index) {
            return key;
        }
        i++;
    }
    
    // this code will never be reached
    static Vector<K> v = map.keys();
    return v[0];
}

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _hashmap_h
