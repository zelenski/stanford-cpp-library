/*
 * File: hashmap.h
 * ---------------
 * This file exports the <code>HashMap</code> class, which stores
 * a set of <i>key</i>-<i>value</i> pairs.
 */

#ifndef _hashmap_h
#define _hashmap_h

#include <cstdlib>
#include <initializer_list>
#include <string>
#include <utility>
#include <unordered_map>
#include <functional>

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
    HashMap() = default;

    /*
     * Constructor: HashMap
     * Usage: HashMap<ValueType> map {{"a", 1}, {"b", 2}, {"c", 3}};
     * -------------------------------------------------------------
     * Initializes a new map that stores the given pairs.
     * Note that the pairs are stored in unpredictable order internally and not
     * necessarily the order in which they are written in the initializer list.
     */
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> list);

    /*
     * Destructor: ~HashMap
     * --------------------
     * Frees any heap storage associated with this map.
     */
    virtual ~HashMap() = default;

    /*
     * Method: lastKey
     * Usage: KeyType value = map.lastKey();
     * ------------------------------------
     * Returns the last key in the map in the order established by the
     * <code>for-each</code> loop.
     * Note that since the keys are stored in an unpredictable order,
     * this is not necessarily equal to the "largest" key value in any particular
     * sorting order; it is just the key that would happen to be emitted last
     * from a for-each loop.
     * If the map is empty, generates an error.
     */
    KeyType lastKey() const;

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
     * Method: firstKey
     * Usage: KeyType value = map.firstKey();
     * -------------------------------------
     * Returns the first key in the map in the order established by the
     * <code>for-each</code> loop.
     * Note that since the keys are stored in an unpredictable order,
     * this is not necessarily equal to the "smallest" key value in any particular
     * sorting order; it is just the key that would happen to be emitted first
     * from a for-each loop.
     * If the map is empty, generates an error.
     */
    KeyType firstKey() const;

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
    void mapAll(std::function<void(const KeyType&, const ValueType&)> fn) const;

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
     * Operator: +
     * Usage: map1 + map2
     * ------------------
     * Returns the union of the two maps, equivalent to a copy of the first map
     * with putAll called on it passing the second map as a parameter.
     * If the two maps both contain a mapping for the same key, the mapping
     * from the second map is favored.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator +(const HashMap& map2) const;

    /*
     * Operator: +=
     * Usage: map1 += map2;
     * --------------------
     * Adds all key/value pairs from the given map to this map.
     * Equivalent to calling putAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator +=(const HashMap& map2);

    /*
     * Operator: -
     * Usage: map1 - map2
     * ------------------
     * Returns the difference of the two maps, equivalent to a copy of the first map
     * with removeAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator -(const HashMap& map2) const;

    /*
     * Operator: -=
     * Usage: map1 -= map2;
     * --------------------
     * Removes all key/value pairs from the given map to this map.
     * Equivalent to calling removeAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator -=(const HashMap& map2);

    /*
     * Operator: *
     * Usage: map1 * map2
     * ------------------
     * Returns the intersection of the two maps, equivalent to a copy of the first map
     * with retainAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap operator *(const HashMap& map2) const;

    /*
     * Operator: *=
     * Usage: map1 *= map2;
     * ---------------------
     * Removes all key/value pairs that are not found in the given map from this map.
     * Equivalent to calling retainAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    HashMap& operator *=(const HashMap& map2);

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

private:
    static_assert(stanfordcpplib::collections::IsHashable<KeyType>::value,
                  "Oops! You tried using a type as a key in our HashMap without making it hashable. Click this error for more details.");
    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it probably means that you tried making a HashMap with a custom struct or class type
     * as the key type or a HashSet with a custom struct as a value type.
     *
     * In order to have a type be a key type in a HashMap - or to have a type be a value type
     * in a HashSet - it needs to have a hashCode function defined and be capable of being
     * compared using the == operator. If you were directed here, one of those two conditions
     * wasn't met.
     *
     * There are two ways to fix this. The first option would simply be to not use your custom
     * type as a key in the HashMap or value in a HashSet. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define a hashCode() and operator== function
     * for your type. To do so, first define hashCode as follows:
     *
     *     int hashCode(const YourCustomType& obj) {
     *         return hashCode(obj.data1, obj.data2, ..., obj.dataN);
     *     }
     *
     * where data1, data2, ... dataN are the data members of your type. For example, if you had
     * a custom type
     *
     *     struct MyType {
     *         int myInt;
     *         string myString;
     *     };
     *
     * you would define the function
     *
     *     int hashCode(const MyType& obj) {
     *         return hashCode(obj.myInt, obj.myString);
     *     }
     *
     * Second, define operator== as follows:
     *
     *     bool operator== (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         return lhs.data1 == rhs.data1 &&
     *                lhs.data2 == rhs.data2 &&
     *                         ...
     *                lhs.dataN == rhs.dataN;
     *     }
     *
     * Using the MyType example from above, we'd write
     *
     *     bool operator== (const MyType& lhs, const MyType& rhs) {
     *         return lhs.myInt == rhs.myInt && lhs.myString == rhs.myString;
     *     }
     *
     * Hope this helps!
     */

    struct Hasher {
        std::size_t operator()(const KeyType& key) const {
            return hashCode(key);
        }
    };

    std::unordered_map<KeyType, ValueType, Hasher> _elements;
    stanfordcpplib::collections::VersionTracker _version;

    /* Private methods */

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
     */

    using const_iterator = stanfordcpplib::collections::ProjectingIterator<stanfordcpplib::collections::CheckedIterator<typename std::unordered_map<KeyType, ValueType, Hasher>::const_iterator>>;
    using iterator = const_iterator;

    iterator begin() const;
    iterator end() const;

    /*
     * Hashing support.
     */
    bool operator== (const HashMap& rhs) const;
    bool operator!= (const HashMap& rhs) const;

    template <typename K, typename V>
    friend int hashCode(const HashMap<K, V>& map);
};

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> list)
        : _elements(list) {
}

template <typename KeyType, typename ValueType>
KeyType HashMap<KeyType, ValueType>::lastKey() const {
    if (isEmpty()) {
        error("HashMap::lastKey: map is empty");
    }

    return std::next(_elements.begin(), _elements.size() - 1)->first;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::clear() {
    _elements.clear();
    _version.update();
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return !!_elements.count(key);
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::equals(const HashMap<KeyType, ValueType>& map2) const {
    return stanfordcpplib::collections::equalsMap(*this, map2);
}

template <typename KeyType, typename ValueType>
KeyType HashMap<KeyType, ValueType>::firstKey() const {
    if (isEmpty()) {
        error("HashMap::firstKey: map is empty");
    }
    return *begin();
}

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType, ValueType>::get(const KeyType& key) const {
    auto itr = _elements.find(key);
    return itr == _elements.end()? ValueType() : itr->second;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::isEmpty() const {
    return _elements.empty();
}

template <typename KeyType, typename ValueType>
Vector<KeyType> HashMap<KeyType, ValueType>::keys() const {
    Vector<KeyType> keyset;
    for (const auto& entry: _elements) {
        keyset.add(entry.first);
    }
    return keyset;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::mapAll(std::function<void (const KeyType&, const ValueType&)> fn) const {
    for (const auto& entry: _elements) {
        fn(entry.first, entry.second);
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    int presize = size();
    _elements[key] = value;

    if (presize != size()) _version.update();
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::putAll(const HashMap& map2) {
    for (const KeyType& key : map2) {
        put(key, map2.get(key));
    }
    return *this;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::remove(const KeyType& key) {
    auto itr = _elements.find(key);
    if (itr != _elements.end()) {
        _elements.erase(itr);
        _version.update();
    }
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::removeAll(const HashMap& map2) {
    for (const KeyType& key : map2) {
        if (containsKey(key) && get(key) == map2.get(key)) {
            remove(key);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::retainAll(const HashMap& map2) {
    Vector<KeyType> toRemove;
    for (const KeyType& key : *this) {
        if (!map2.containsKey(key) || get(key) != map2.get(key)) {
            toRemove.add(key);
        }
    }
    for (const KeyType& key : toRemove) {
        remove(key);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
int HashMap<KeyType, ValueType>::size() const {
    return _elements.size();
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
    for (const auto& entry: _elements) {
        values.add(entry.second);
    }
    return values;
}

template <typename KeyType, typename ValueType>
ValueType& HashMap<KeyType, ValueType>::operator [](const KeyType& key) {
    int presize = size();
    ValueType& result = _elements[key];

    if (presize != size()) _version.update();
    return result;
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
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator +=(const HashMap& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator -(const HashMap& map2) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.removeAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator -=(const HashMap& map2) {
    return removeAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType> HashMap<KeyType, ValueType>::operator *(const HashMap& map2) const {
    HashMap<KeyType, ValueType> result = *this;
    return result.retainAll(map2);
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>& HashMap<KeyType, ValueType>::operator *=(const HashMap& map2) {
    return retainAll(map2);
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::begin() const {
    return iterator({ &_version, _elements.begin(), _elements });
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::end() const {
    return iterator({ &_version, _elements.end(), _elements });
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::operator == (const HashMap<KeyType, ValueType>& rhs) const {
    return stanfordcpplib::collections::equalsMap(*this, rhs);
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::operator != (const HashMap<KeyType, ValueType>& rhs) const {
    return !(*this == rhs);
}

template <typename KeyType, typename ValueType>
int hashCode(const HashMap<KeyType, ValueType>& map) {
    return stanfordcpplib::collections::hashCodeMap(map, false);
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
    return stanfordcpplib::collections::readPairedCollection(is, map, key, value, /* descriptor */ std::string("HashMap::operator >>"));
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
    return stanfordcpplib::collections::randomElement(map);
}

#endif // _hashmap_h
