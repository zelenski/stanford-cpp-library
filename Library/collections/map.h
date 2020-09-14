/*
 * File: map.h
 * -----------
 * This file exports the template class <code>Map</code>, which
 * maintains a collection of <i>key</i>-<i>value</i> pairs.
 *
 * @version 2020/09/12
 * - simplify interface in preparation for install
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
 * @version 2018/03/19
 * - added constructors that accept a comparison function
 * @version 2018/03/10
 * - added methods front, back
 * @version 2017/10/18
 * - fix compiler warnings
 * @version 2016/12/09
 * - added iterator version checking support
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - bug fix for operators <, <=, >, >= (was comparing only keys!)
 * @version 2016/08/10
 * - added support for std initializer_list usage, such as
 *   {{"a", 1}, {"b", 2}, {"c", 3}} in constructor, addAll, putAll,
 *   removeAll, retainAll, and operators +, +=, -, -=, *, *=
 * - added addAll method
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/10/13
 * - nulled out pointer fields in destructor after deletion to avoid double-free
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * - fixed bug where string quotes would not show when map was printed
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added add() method as synonym for put()
 * - added template hashCode function
 * @version 2014/10/10
 * - removed usage of __foreach macro
 */


#ifndef _map_h
#define _map_h

#include <cstdlib>
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <map>
#include <functional>

#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "stack.h"
#include "vector.h"

/*
 * Class: Map<KeyType,ValueType>
 * -----------------------------
 * This class maintains an association between <b><i>keys</i></b> and
 * <b><i>values</i></b>.  The types used for keys and values are
 * specified using templates, which makes it possible to use
 * this structure with any data type.
 */
template <typename KeyType, typename ValueType>
class Map {
public:
    /*
     * Constructor: Map
     * Usage: Map<KeyType,ValueType> map;
     * ----------------------------------
     * Initializes a new empty map that associates keys and values of the
     * specified types.
     */
    Map();

    /*
     * Constructor: Map
     * Usage: Map<KeyType,ValueType> map(lessFunc);
     * --------------------------------------------
     * Initializes a new empty map that associates keys and values of the
     * specified types, using the given "less-than" comparison function
     * to order any keys that will be later added to it.
     * The function can accept the two keys to compare either by value
     * or by const reference.
     */
    Map(std::function<bool (const KeyType&, const KeyType&)> lessFunc);

    /*
     * Constructor: Map
     * Usage: Map<ValueType> map {{"a", 1}, {"b", 2}, {"c", 3}};
     * ---------------------------------------------------------
     * Initializes a new map that stores the given pairs.
     * Note that the pairs are stored in key-sorted order internally and not
     * necessarily the order in which they are written in the initializer list.
     */
    Map(std::initializer_list<std::pair<const KeyType, ValueType>> list);

    /*
     * Constructor: Map
     * Usage: Map<ValueType> map({{"a", 1}, {"b", 2}, {"c", 3}}, lessFunc);
     * --------------------------------------------------------------------
     * Initializes a new empty map that associates keys and values of the
     * specified types, using the given "less-than" comparison function
     * to order any keys that will be later added to it.
     * The function can accept the two keys to compare either by value
     * or by const reference.
     */
    Map(std::initializer_list<std::pair<const KeyType, ValueType>> list,
        std::function<bool (const KeyType&, const KeyType&)> lessFunc);

    /*
     * Destructor: ~Map
     * ----------------
     * Frees any heap storage associated with this map.
     */
    virtual ~Map() = default;

    /*
     * Method: lastKey
     * Usage: KeyType value = map.lastKey();
     * ------------------------------------
     * Returns the last key in the map in the order established by the
     * <code>for-each</code> loop.  If the map is empty, generates an error.
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
    bool equals(const Map& map2) const;

    /*
     * Method: firstKey
     * Usage: KeyType value = map.firstKey();
     * --------------------------------------
     * Returns the first key in the map in the order established by the
     * <code>for-each</code> loop.  If the map is empty, generates an error.
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
     * for each one.  The keys are processed in ascending order, as defined
     * by the comparison function.
     */
    void mapAll(std::function<void (const KeyType&, const ValueType&)> fn) const;

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
     * ------------------------
     * Adds all key/value pairs from the given map to this map.
     * If both maps contain a pair for the same key, the one from map2 will
     * replace the one from this map.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     * Returns a reference to this map.
     */
    Map& putAll(const Map& map2);

    /*
     * Method: remove
     * Usage: map.remove(key);
     * -----------------------
     * Removes any entry for <code>key</code> from this map.
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
    Map& removeAll(const Map& map2);

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
    Map& retainAll(const Map& map2);

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
    bool operator ==(const Map& map2) const;

    /*
     * Operator: !=
     * Usage: if (map1 != map2) ...
     * ----------------------------
     * Compares two maps for inequality.
     */
    bool operator !=(const Map& map2) const;

    /*
     * Operators: <, <=, >, >=
     * Usage: if (map1 < map2) ...
     * ---------------------------
     * Relational operators to compare two maps.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Map& map2) const;
    bool operator <=(const Map& map2) const;
    bool operator >(const Map& map2) const;
    bool operator >=(const Map& map2) const;

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
    Map operator +(const Map& map2) const;

    /*
     * Operator: +=
     * Usage: map1 += map2;
     * --------------------
     * Adds all key/value pairs from the given map to this map.
     * Equivalent to calling putAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    Map& operator +=(const Map& map2);

    /*
     * Operator: -
     * Usage: map1 - map2
     * ------------------
     * Returns the difference of the two maps, equivalent to a copy of the first map
     * with removeAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    Map operator -(const Map& map2) const;

    /*
     * Operator: -=
     * Usage: map1 -= map2;
     * --------------------
     * Removes all key/value pairs from the given map to this map.
     * Equivalent to calling removeAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    Map& operator -=(const Map& map2);

    /*
     * Operator: *
     * Usage: map1 * map2
     * ------------------
     * Returns the intersection of the two maps, equivalent to a copy of the first map
     * with retainAll called on it passing the second map as a parameter.
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    Map operator *(const Map& map2) const;

    /*
     * Operator: *=
     * Usage: map1 *= map2;
     * ---------------------
     * Removes all key/value pairs that are not found in the given map from this map.
     * Equivalent to calling retainAll(map2).
     * You can also pass an initializer list of pairs such as {{"a", 1}, {"b", 2}, {"c", 3}}.
     */
    Map& operator *=(const Map& map2);

    /*
     * Additional Map operations
     * -------------------------
     * In addition to the methods listed in this interface, the Map
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * All iteration is guaranteed to proceed in the order established by
     * the comparison function passed to the constructor, which ordinarily
     * matches the order of the key type.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    using MapType = std::map<KeyType, ValueType, std::function<bool(const KeyType&, const KeyType&)>>;
    MapType _elements;
    stanfordcpplib::collections::VersionTracker _version;

public:
    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support deep copying and iteration.  Including these methods in
     * the public portion of the interface would make that interface more
     * difficult to understand for the average client.
     */

    using const_iterator = stanfordcpplib::collections::ProjectingIterator<stanfordcpplib::collections::CheckedIterator<typename MapType::const_iterator>>;
    using iterator = const_iterator;

    const_iterator begin() const;
    const_iterator end() const;
};

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map() : _elements(stanfordcpplib::collections::checkedLess<KeyType>()) {
    // Handled in initializer
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(std::function<bool(const KeyType&, const KeyType&)> lessFunc)
        : _elements(lessFunc) {
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(std::initializer_list<std::pair<const KeyType, ValueType>> list)
        : _elements(list, stanfordcpplib::collections::checkedLess<KeyType>()) {
    // Handled in initializer
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(std::initializer_list<std::pair<const KeyType, ValueType>> list,
                             std::function<bool(const KeyType&, const KeyType&)> lessFunc)
        : _elements(list, lessFunc) {
}

template <typename KeyType, typename ValueType>
KeyType Map<KeyType, ValueType>::lastKey() const {
    if (isEmpty()) {
        error("Map::lastKey: map is empty");
    }
    return _elements.rbegin()->first;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::clear() {
    _elements.clear();
    _version.update();
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return !!_elements.count(key);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::equals(const Map<KeyType, ValueType>& map2) const {
    return stanfordcpplib::collections::equalsMap(*this, map2);
}

template <typename KeyType, typename ValueType>
KeyType Map<KeyType, ValueType>::firstKey() const {
    if (isEmpty()) {
        error("Map::firstKey: map is empty");
    }
    return _elements.begin()->first;
}

template <typename KeyType, typename ValueType>
ValueType Map<KeyType, ValueType>::get(const KeyType& key) const {
    auto itr = _elements.find(key);
    return itr == _elements.end()? ValueType() : itr->second;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::isEmpty() const {
    return _elements.empty();
}

template <typename KeyType,typename ValueType>
Vector<KeyType> Map<KeyType, ValueType>::keys() const {
    Vector<KeyType> keyset;
    for (const auto& entry: _elements) {
        keyset.add(entry.first);
    }
    return keyset;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::mapAll(std::function<void (const KeyType&, const ValueType&)> fn) const {
    for (const auto& entry: _elements) {
        fn(entry.first, entry.second);
    }
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::put(const KeyType& key,
                                  const ValueType& value) {
    int presize = size();
    _elements[key] = value;
    if (presize != size()) _version.update();
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::putAll(const Map& map2) {
    for (const KeyType& key : map2) {
        put(key, map2.get(key));
    }
    return *this;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::remove(const KeyType& key) {
    _elements.erase(key);
    _version.update();
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::removeAll(const Map& map2) {
    for (const KeyType& key : map2) {
        if (containsKey(key) && get(key) == map2.get(key)) {
            remove(key);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::retainAll(const Map& map2) {
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
int Map<KeyType, ValueType>::size() const {
    return _elements.size();
}

template <typename KeyType, typename ValueType>
std::string Map<KeyType, ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename KeyType,typename ValueType>
Vector<ValueType> Map<KeyType, ValueType>::values() const {
    Vector<ValueType> values;
    for (const auto& entry: _elements) {
        values.add(entry.second);
    }
    return values;
}

template <typename KeyType, typename ValueType>
ValueType& Map<KeyType, ValueType>::operator [](const KeyType& key) {
    auto presize = size();
    auto& result = _elements[key];

    /* If the size was updated, we must have inserted something. */
    if (presize != size()) _version.update();
    return result;
}

template <typename KeyType, typename ValueType>
ValueType Map<KeyType, ValueType>::operator [](const KeyType& key) const {
    return get(key);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator +(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.putAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator +=(const Map& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator -(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.removeAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator -=(const Map& map2) {
    return removeAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator *(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.retainAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator *=(const Map& map2) {
    return retainAll(map2);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator ==(const Map& map2) const {
    return equals(map2);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator !=(const Map& map2) const {
    return !equals(map2);   // BUGFIX 2016/01/27, thanks to O. Zeng
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator <(const Map& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) < 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator <=(const Map& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) <= 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator >(const Map& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) > 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator >=(const Map& map2) const {
    return stanfordcpplib::collections::compareMaps(*this, map2) >= 0;
}

template <typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator Map<KeyType, ValueType>::begin() const {
    return iterator({ &_version, _elements.begin(), _elements });
}

template <typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator Map<KeyType, ValueType>::end() const {
    return iterator({ &_version, _elements.end(), _elements });
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
                          const Map<KeyType, ValueType>& map) {
    return stanfordcpplib::collections::writeMap(os, map);
}

template <typename KeyType, typename ValueType>
std::istream& operator >>(std::istream& is, Map<KeyType,ValueType>& map) {
    KeyType key;
    ValueType value;
    return stanfordcpplib::collections::readPairedCollection(is, map, key, value, /* descriptor */ std::string("Map::operator >>"));
}

/*
 * Template hash function for maps.
 * Requires the key and value types in the Map to have a hashCode function.
 */
template <typename K, typename V>
int hashCode(const Map<K, V>& map) {
    return stanfordcpplib::collections::hashCodeMap(map);
}

/*
 * Function: randomKey
 * Usage: element = randomKey(map);
 * --------------------------------
 * Returns a randomly chosen key of the given map.
 * Throws an error if the map is empty.
 */
template <typename K, typename V>
const K& randomKey(const Map<K, V>& map) {
    return stanfordcpplib::collections::randomElement(map);
}

#endif // _map_h
