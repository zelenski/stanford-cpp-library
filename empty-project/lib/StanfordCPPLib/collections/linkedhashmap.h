/*
 * File: linkedhashmap.h
 * ---------------------
 * This file exports the <code>LinkedHashMap</code> class, which stores
 * a set of <i>key</i>-<i>value</i> pairs.
 * Identical to a HashMap except that upon iteration using a for-each loop
 * or << / toString call, it will emit its key/value pairs in the order they
 * were originally inserted.  This is provided at a runtime and memory
 * cost due to needing to store an extra copy of the keys.
 * 
 * @author Marty Stepp
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - added support for std initializer_list usage, such as
 *   {{"a", 1}, {"b", 2}, {"c", 3}} in constructor, putAll, removeAll, retainAll,
 *   operators +, +=, -, -=, *, *=
 * - added addAll method
 * - added operators <, <=, >, >=
 * @version 2016/08/12
 * - bug fix in operator >>
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/10/26
 * - initial version
 * @since 2015/10/26
 */

#ifndef _linkedhashmap_h
#define _linkedhashmap_h

#include <initializer_list>
#include <iterator>
#include <string>
#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "hashmap.h"
#include "vector.h"

/*
 * Class: LinkedHashMap<KeyType,ValueType>
 * ---------------------------------------
 * 
 */
template <typename KeyType, typename ValueType>
class LinkedHashMap {
public:
    /*
     * Constructor: LinkedHashMap
     * Usage: LinkedHashMap<KeyType, ValueType> map;
     * ---------------------------------------------
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
    LinkedHashMap();

    /*
     * Constructor: LinkedHashMap
     * Usage: LinkedHashMap<ValueType> map {{"a", 1}, {"b", 2}, {"c", 3}};
     * -------------------------------------------------------------------
     * Initializes a new map that stores the given pairs.
     * Note that the pairs are stored in unpredictable order internally and not
     * necessarily the order in which they are written in the initializer list.
     */
    LinkedHashMap(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Destructor: ~HashMap
     * --------------------
     * Frees any heap storage associated with this map.
     */
    virtual ~LinkedHashMap();

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
    LinkedHashMap& addAll(const LinkedHashMap& map2);
    LinkedHashMap& addAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

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
    bool equals(const LinkedHashMap& map2) const;

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
     * This implementation already retains an internal copy of the keys,
     * so it is efficient to call this function even on large maps.
     */
    const Vector<KeyType>& keys() const;

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
     * Returns a reference to this map.
     */
    LinkedHashMap& putAll(const LinkedHashMap& map2);
    LinkedHashMap& putAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

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
     * Returns a reference to this map.
     */
    LinkedHashMap& removeAll(const LinkedHashMap& map2);
    LinkedHashMap& removeAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Method: retainAll
     * Usage: map.retainAll(map2);
     * ---------------------------
     * Removes all key/value pairs from this map that are not contained in the given map.
     * If both maps contain the same key but it maps to different values, that
     * mapping will be removed.
     * Returns a reference to this map.
     */
    LinkedHashMap& retainAll(const LinkedHashMap& map2);
    LinkedHashMap& retainAll(std::initializer_list<std::pair<KeyType, ValueType> > list);

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
    // ValueType& operator [](const KeyType& key);
    ValueType operator [](const KeyType& key) const;

    /*
     * Operator: ==
     * Usage: if (map1 == map2) ...
     * ----------------------------
     * Compares two maps for equality.
     */
    bool operator ==(const LinkedHashMap& map2) const;

    /*
     * Operator: !=
     * Usage: if (map1 != map2) ...
     * ----------------------------
     * Compares two maps for inequality.
     */
    bool operator !=(const LinkedHashMap& map2) const;

    /*
     * Operators: <, <=, >, >=
     * Usage: if (map1 < map2) ...
     * ---------------------------
     * Relational operators to compare two maps.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const LinkedHashMap& map2) const;
    bool operator <=(const LinkedHashMap& map2) const;
    bool operator >(const LinkedHashMap& map2) const;
    bool operator >=(const LinkedHashMap& map2) const;

    /*
     * Operator: +
     * Usage: map1 + map2
     * ------------------
     * Returns the union of the two maps, equivalent to a copy of the first map
     * with addAll called on it passing the second map as a parameter.
     * If the two maps both contain a mapping for the same key, the mapping
     * from the second map is favored.
     */
    LinkedHashMap operator +(const LinkedHashMap& map2) const;
    LinkedHashMap operator +(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: +=
     * Usage: map1 += map2;
     * --------------------
     * Adds all key/value pairs from the given map to this map.
     * Equivalent to calling addAll(map2).
     */
    LinkedHashMap& operator +=(const LinkedHashMap& map2);
    LinkedHashMap& operator +=(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Operator: -
     * Usage: map1 - map2
     * ------------------
     * Returns the difference of the two maps, equivalent to a copy of the first map
     * with removeAll called on it passing the second map as a parameter.
     */
    LinkedHashMap operator -(const LinkedHashMap& map2) const;
    LinkedHashMap operator -(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: -=
     * Usage: map1 -= map2;
     * --------------------
     * Removes all key/value pairs from the given map to this map.
     * Equivalent to calling removeAll(map2).
     */
    LinkedHashMap& operator -=(const LinkedHashMap& map2);
    LinkedHashMap& operator -=(std::initializer_list<std::pair<KeyType, ValueType> > list);

    /*
     * Operator: *
     * Usage: map1 * map2
     * ------------------
     * Returns the intersection of the two maps, equivalent to a copy of the first map
     * with retainAll called on it passing the second map as a parameter.
     */
    LinkedHashMap operator *(const LinkedHashMap& map2) const;
    LinkedHashMap operator *(std::initializer_list<std::pair<KeyType, ValueType> > list) const;

    /*
     * Operator: *=
     * Usage: map1 *= map2;
     * ---------------------
     * Removes all key/value pairs that are not found in the given map from this map.
     * Equivalent to calling retainAll(map2).
     */
    LinkedHashMap& operator *=(const LinkedHashMap& map2);
    LinkedHashMap& operator *=(std::initializer_list<std::pair<KeyType, ValueType> > list);

    template <typename K, typename V>
    friend std::ostream& operator <<(std::ostream& os, const LinkedHashMap<K, V>& map);
    
    /*
     * Additional LinkedHashMap operations
     * -----------------------------------
     * In addition to the methods listed in this interface, the HashMap
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * During iteration, the LinkedHashMap class returns its keys in the
     * order they were added.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes:
     * ---------------------
     * The HashMap class is represented using a hash map along with a
     * Vector to remember the order of insertion.
    */
private:
    HashMap<KeyType, ValueType> innerMap;
    Vector<KeyType> keyVector;

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
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public Vector<KeyType>::iterator {
    public:
        iterator() : Vector<KeyType>::iterator() {}
        iterator(const iterator& it) : Vector<KeyType>::iterator(it) {}
        iterator(const typename Vector<KeyType>::iterator& it) : Vector<KeyType>::iterator(it) {}
    };

    /*
     * Returns an iterator positioned at the first key of the map.
     */
    iterator begin() const {
        return iterator(keyVector.begin());
    }

    /*
     * Returns an iterator positioned at the last key of the map.
     */
    iterator end() const {
        return iterator(keyVector.end());
    }
};

/*
 * Implementation notes: LinkedHashMap class
 * -----------------------------------------
 * ...
 */
template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>::LinkedHashMap() {
    // empty
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>::LinkedHashMap(std::initializer_list<std::pair<KeyType, ValueType> > list) {
    putAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>::~LinkedHashMap() {
    // empty
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    put(key, value);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::addAll(const LinkedHashMap& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::addAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return putAll(list);
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::clear() {
    innerMap.clear();
    keyVector.clear();
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return innerMap.containsKey(key);
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::equals(const LinkedHashMap<KeyType, ValueType>& map2) const {
    return stanfordcpplib::collections::equalsMap(*this, map2);
}

template <typename KeyType, typename ValueType>
ValueType LinkedHashMap<KeyType, ValueType>::get(const KeyType& key) const {
    return innerMap.get(key);
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::isEmpty() const {
    return innerMap.isEmpty();
}

template <typename KeyType, typename ValueType>
const Vector<KeyType>& LinkedHashMap<KeyType, ValueType>::keys() const {
    return keyVector;
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::mapAll(void (*fn)(KeyType, ValueType)) const {
    innerMap.mapAll(fn);
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::mapAll(void (*fn)(const KeyType&,
                                                   const ValueType&)) const {
    innerMap.mapAll(fn);
}

template <typename KeyType, typename ValueType>
template <typename FunctorType>
void LinkedHashMap<KeyType, ValueType>::mapAll(FunctorType fn) const {
    innerMap.mapAll(fn);
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    innerMap.put(key, value);
    keyVector.add(key);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::putAll(const LinkedHashMap& map2) {
    for (KeyType key : map2) {
        put(key, map2.get(key));
    }
    return *this;
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::putAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    for (std::pair<KeyType, ValueType> pair : list) {
        put(pair.first, pair.second);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
void LinkedHashMap<KeyType, ValueType>::remove(const KeyType& key) {
    innerMap.remove(key);
    for (int i = 0, sz = keyVector.size(); i < sz; i++) {
        if (keyVector[i] == key) {
            keyVector.remove(i);
            break;
        }
    }
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::removeAll(const LinkedHashMap& map2) {
    for (KeyType key : map2) {
        if (containsKey(key) && get(key) == map2.get(key)) {
            remove(key);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::removeAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    for (std::pair<KeyType, ValueType> pair : list) {
        if (containsKey(pair.first) && get(pair.first) == pair.second) {
            remove(pair.first);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::retainAll(const LinkedHashMap& map2) {
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
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::retainAll(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    LinkedHashMap<KeyType, ValueType> map2(list);
    retainAll(map2);
    return *this;
}

template <typename KeyType, typename ValueType>
int LinkedHashMap<KeyType, ValueType>::size() const {
    return innerMap.size();
}

template <typename KeyType, typename ValueType>
std::string LinkedHashMap<KeyType, ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename KeyType, typename ValueType>
Vector<ValueType> LinkedHashMap<KeyType, ValueType>::values() const {
    Vector<ValueType> values;
    for (KeyType key : *this) {
        values.add(get(key));
    }
    return values;
}

template <typename KeyType, typename ValueType>
ValueType LinkedHashMap<KeyType, ValueType>::operator [](const KeyType& key) const {
    return innerMap[key];
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator +(const LinkedHashMap& map2) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.putAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator +(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.putAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator +=(const LinkedHashMap& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator +=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return putAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator -(const LinkedHashMap& map2) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.removeAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator -(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.removeAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator -=(const LinkedHashMap& map2) {
    return removeAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator -=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return removeAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator *(const LinkedHashMap& map2) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.retainAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType> LinkedHashMap<KeyType, ValueType>::operator *(
        std::initializer_list<std::pair<KeyType, ValueType> > list) const {
    LinkedHashMap<KeyType, ValueType> result = *this;
    return result.retainAll(list);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator *=(const LinkedHashMap& map2) {
    return retainAll(map2);
}

template <typename KeyType, typename ValueType>
LinkedHashMap<KeyType, ValueType>& LinkedHashMap<KeyType, ValueType>::operator *=(
        std::initializer_list<std::pair<KeyType, ValueType> > list) {
    return retainAll(list);
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator ==(const LinkedHashMap& map2) const {
    return equals(map2);
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator !=(const LinkedHashMap& map2) const {
    return !equals(map2);
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator <(const LinkedHashMap& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) < 0;
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator <=(const LinkedHashMap& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) <= 0;
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator >(const LinkedHashMap& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) > 0;
}

template <typename KeyType, typename ValueType>
bool LinkedHashMap<KeyType, ValueType>::operator >=(const LinkedHashMap& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) >= 0;
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
                          const LinkedHashMap<KeyType, ValueType>& map) {
    return stanfordcpplib::collections::writeMap(os, map);
}

template <typename KeyType, typename ValueType>
std::istream& operator >>(std::istream& is,
                          LinkedHashMap<KeyType, ValueType>& map) {
    KeyType key;
    ValueType value;
    return stanfordcpplib::collections::readMap(is, map, key, value, /* descriptor */ std::string("LinkedHashMap::operator >>"));
}

/*
 * Template hash function for hash maps.
 * Requires the key and value types in the LinkedHashMap to have a hashCode function.
 */
template <typename K, typename V>
int hashCode(const LinkedHashMap<K, V>& map) {
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
const K& randomKey(const LinkedHashMap<K, V>& map) {
    if (map.isEmpty()) {
        error("randomKey: empty map was passed");
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

#endif // _linkedhashmap_h
