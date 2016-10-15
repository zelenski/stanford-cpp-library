/*
 * File: collections.h
 * -------------------
 * Contains general-purpose functions for use with many collections.
 *
 * For example, we have functions for comparing any collections that have
 * a visible iterator (begin(), end()).
 * Used to implement comparison operators like < and >= on collections.
 *
 * @author Marty Stepp
 * @version 2016/09/24
 * - renamed compare.h to collections.h
 * - added printing functions
 * @version 2014/10/10
 * - initial version as compare.h
 * @since 2014/10/10
 */

#ifndef _collections_h
#define _collections_h

#include <iostream>
#include "error.h"
#include "hashcode.h"
#include "random.h"
#include "strlib.h"

namespace stanfordcpplib {
namespace collections {

/*
 * Performs a comparison for ordering between the given two collections
 * by comparing their elements pairwise to each other.
 * Returns -1 if collection 1 is "less than" collection 2;
 * Returns  1 if collection 1 is "greater than" collection 2;
 * Returns  0 if collection 1 is "equal to" collection 2.
 * The element type must have an operator <.
 */
template <typename CollectionType>
int compare(const CollectionType& coll1, const CollectionType& coll2) {
    // optimization: if they are the same object, then they are equal
    if (&coll1 == &coll2) {
        return 0;
    }
    
    auto itr1 = coll1.begin(),
            itr2 = coll2.begin(),
            end1 = coll1.end(),
            end2 = coll2.end();
    for (;
         itr1 != end1 && itr2 != end2;
         ++itr1, ++itr2) {
        // compare each pair of elements from iterators
        
        // TO STUDENT:
        // If the line below is failing to compile in your program, it probably
        // means that you are trying to make a nested collection
        // (e.g. Set<Vector<T>>) for some element type T that does not have a
        // less-than < operator.  That operator is *required* in order to make
        // a Set or Map of Vectors, so that the set/map knows how to sort the
        // elements into their ascending order.
        // You should either add a < operator to your class, or consider a
        // different nested collection solution.  Good luck!
        if (*itr1 < *itr2) {
            return -1;
        } else if (*itr2 < *itr1) {
            return 1;
        }
    }
    
    // if we get here, everything from v1 matched v2, so they are either equal,
    // or one is shorter than the other (fewer elements) and is therefore less
    if (itr1 == end1 && itr2 == end2) {
        return 0;
    } else if (itr1 == end1) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Performs a comparison for ordering between the given two maps
 * by comparing their key/value pairs pairwise to each other.
 * Returns -1 if collection 1 is "less than" collection 2;
 * Returns  1 if collection 1 is "greater than" collection 2;
 * Returns  0 if collection 1 is "equal to" collection 2.
 * Note that the order of keys and values in the maps matter here;
 * the same map with keys in different orders would produce a different result.
 * The key and value types must both have an operator <.
 */
template <typename MapType>
int compareMaps(const MapType& map1, const MapType& map2) {
    // optimization: if they are the same object, then they are equal
    if (&map1 == &map2) {
        return 0;
    }
    
    auto itr1 = map1.begin(),
            itr2 = map2.begin(),
            end1 = map1.end(),
            end2 = map2.end();
    for (;
         itr1 != end1 && itr2 != end2;
         ++itr1, ++itr2) {
        // compare each pair of elements from iterators
        
        // TO STUDENT:
        // If the line below is failing to compile in your program, it probably
        // means that you are trying to make a nested collection
        // (e.g. Set<Map<K, V>>) for some element type K or V that does not have a
        // less-than < operator.  That operator is *required* in order to make
        // a Set or Map of Maps, so that the set/map knows how to sort the
        // keys and values into their ascending order.
        // You should either add a < operator to your key/value types, or consider a
        // different nested collection solution.  Good luck!
        if (*itr1 < *itr2) {
            return -1;
        } else if (*itr2 < *itr1) {
            return 1;
        }
        
        // key1 == key2, so compare values
        auto value1 = map1[*itr1];
        auto value2 = map2[*itr2];
        if (value1 < value2) {
            return -1;
        } else if (value2 < value1) {
            return 1;
        }
    }
    
    // if we get here, everything from v1 matched v2, so they are either equal,
    // or one is shorter than the other (fewer elements) and is therefore less
    if (itr1 == end1 && itr2 == end2) {
        return 0;
    } else if (itr1 == end1) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Returns true if the two collections contain the same elements in the same order.
 * The element type must have an operator ==.
 */
template <typename CollectionType>
bool equals(const CollectionType& coll1, const CollectionType& coll2) {
    // optimization: if literally same collection, stop
    if (&coll1 == &coll2) {
        return true;
    }
    // optimization: if not same size, don't bother comparing pairwise
    if (coll1.size() != coll2.size()) {
        return false;
    }

    // check each pair of elements for equality
    auto itr1 = coll1.begin();
    auto end1 = coll1.end();
    auto itr2 = coll2.begin();
    auto end2 = coll1.end();
    while (itr1 != end1 && itr2 != end2) {
        if (!(*itr1 == *itr2)) {
            return false;
        }
        ++itr1;
        ++itr2;
    }
    return true;
}

/*
 * Returns true if the given two maps contain the same set of keys and each
 * key maps to the same value in both maps.
 * The order of the keys is not considered.
 * The key and value types must both have an operator ==.
 */
template <typename MapType>
bool equalsMap(const MapType& map1, const MapType& map2) {
    // optimization: if literally same map, stop
    if (&map1 == &map2) {
        return true;
    }
    // optimization: if not same size, don't bother comparing pairwise
    if (map1.size() != map2.size()) {
        return false;
    }

    // check each pair of key/value pairs for equality;
    // compare both ways; each must be subset of the other
    for (auto itr1 = map1.begin(), end1 = map1.end(); itr1 != end1; ++itr1) {
        if (!map2.containsKey(*itr1) || !(map1.get(*itr1) == map2.get(*itr1))) {
            return false;
        }
    }
    for (auto itr2 = map2.begin(), end2 = map2.end(); itr2 != end2; ++itr2) {
        if (!map1.containsKey(*itr2) || !(map1.get(*itr2) == map2.get(*itr2))) {
            return false;
        }
    }
    return true;
}

/*
 * Computes a hashCode for any iterable type.
 * Pass true for 'orderMatters' for collections that maintain some kind of element
 * ordering that is of importance.  This would be true for almost all collections
 * except hash-based ones where the ordering is semi-random and unimportant.
 */
template <typename IteratorType>
int hashCodeIterable(IteratorType begin, IteratorType end, bool orderMatters = true) {
    int code = hashSeed();
    while (begin != end) {
        if (orderMatters) {
            code *= hashMultiplier();
        }
        code += hashCode(*begin);
        ++begin;
    }
    return int(code & hashMask());
}

/*
 * Computes a hashCode for any type of collection.
 * The collection must have begin() and end()
 */
template <typename CollectionType>
int hashCodeCollection(const CollectionType& collection, bool orderMatters = true) {
    return hashCodeIterable(collection.begin(), collection.end(), orderMatters);
}

/*
 * Template hash function for hash maps.
 * Requires the key and value types in the HashMap to have a hashCode function.
 */
template <typename MapType>
int hashCodeMap(const MapType& map, bool orderMatters = true) {
    int code = hashSeed();
    auto begin = map.begin();
    auto end = map.end();
    while (begin != end) {
        if (orderMatters) {
            code *= hashMultiplier();
        }
        code += hashCode(*begin);

        if (orderMatters) {
            code *= hashMultiplier();
        }
        code += hashCode(map[*begin]);
        ++begin;
    }
    return int(code & hashMask());
}

/*
 * Returns a randomly chosen element of the given collection.
 * Throws an error if the set is empty.
 */
template <template <typename> class CollectionType, class ElementType>
const ElementType& randomElement(const CollectionType<ElementType>& collection) {
    if (collection.isEmpty()) {
        error("randomElement: empty collection was passed");
    }
    int index = randomInteger(0, collection.size() - 1);
    auto itr = collection.begin();
    for (int i = 0; i < index; i++) {
        ++itr;
    }
    return *itr;
}

/*
 * Returns a randomly chosen element of the given collection.
 * The collection must have an index [] operator.
 * Throws an error if the set is empty.
 */
template <template <typename> class CollectionType, class ElementType>
const ElementType& randomElementIndexed(const CollectionType<ElementType>& collection) {
    if (collection.isEmpty()) {
        error("randomElement: empty collection was passed");
    }
    int index = randomInteger(0, collection.size() - 1);
    return collection[index];
}

/*
 * Reads in any collection from the given input stream.
 * The collection must have an add() method that takes a single value,
 * and a clear() method that removes all elements from the collection.
 */
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
template <typename CollectionType, typename ElementType>
std::istream& readCollection(std::istream& input, CollectionType& collection, ElementType& element, std::string descriptor = "readIterable") {
#else
template <typename CollectionType, typename ElementType>
std::istream& readCollection(std::istream& input, CollectionType& collection, ElementType& element, std::string /* descriptor */) {
#endif
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error(descriptor + ": Missing {");
#endif
        input.setstate(std::ios_base::failbit);
        return input;
    }
    collection.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            if (!readGenericValue(input, element)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(descriptor + ": parse error");
#endif
                return input;
            }
            collection.add(element);
            input >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(std::string(descriptor + ": Unexpected character ") + ch);
#endif
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }
    return input;
}

/*
 * Reads in any Map collection from the given input stream.
 * The collection must have an add() method that takes a single value,
 * and a clear() method that removes all elements from the collection.
 */
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
template <typename MapType, typename KeyType, typename ValueType>
std::istream& readMap(std::istream& input, MapType& map, KeyType& key, ValueType& value, std::string descriptor = "readIterable") {
#else
template <typename MapType, typename KeyType, typename ValueType>
std::istream& readMap(std::istream& input, MapType& map, KeyType& key, ValueType& value, std::string /* descriptor */) {
#endif
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error(descriptor + ": Missing {");
#endif
        input.setstate(std::ios_base::failbit);
        return input;
    }
    map.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            if (!readGenericValue(input, key)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(descriptor + ": parse key error");
#endif
                return input;
            }
            input >> ch;
            if (ch != ':') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(descriptor + ": Missing colon after key");
#endif
                input.setstate(std::ios_base::failbit);
                return input;
            }
            if (!readGenericValue(input, value)) {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(descriptor + ": parse value error");
#endif
                return input;
            }
            map.put(key, value);
            input >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
                error(std::string(descriptor + ": Unexpected character ") + ch);
#endif
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }
    return input;
}

/*
 * Writes out any collection to the given output stream using its two iterators,
 * begin and end.
 */
template <typename IteratorType>
std::ostream& writeIterable(std::ostream& out, IteratorType begin, IteratorType end) {
    out << "{";
    bool first = true;
    while (begin != end) {
        if (!first) {
            out << ", ";
        }
        first = false;
        writeGenericValue(out, *begin, /* forceQuotes */ true);
        ++begin;
    }
    out << "}";
    return out;
}

/*
 * Writes out any collection to the given output stream using its two iterators,
 * begin and end.
 */
template <typename CollectionType>
std::ostream& writeCollection(std::ostream& out, CollectionType collection) {
    return writeIterable(out, collection.begin(), collection.end());
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename MapType>
std::ostream& writeMap(std::ostream& out, const MapType& map) {
    out << "{";
    auto begin = map.begin();
    auto end = map.end();
    bool first = true;
    while (begin != end) {
        if (!first) {
            out << ", ";
        }
        first = false;
        writeGenericValue(out, *begin, /* forceQuotes */ true);
        out << ":";
        writeGenericValue(out, map[*begin], /* forceQuotes */ true);
        ++begin;
    }
    out << "}";
    return out;
}



} // namespace collections
} // namespace stanfordcpplib

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif
