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
 * @version 2017/12/12
 * - added equalsDouble for collections of double values (can't compare with ==)
 * @version 2017/10/18
 * - fix compiler warnings
 * @version 2017/09/29
 * - added compareTo1-5
 * @version 2016/12/09
 * - added checkVersion for iterators
 * @version 2016/09/24
 * - renamed compare.h to collections.h
 * - added printing functions
 * @version 2014/10/10
 * - initial version as compare.h
 * @since 2014/10/10
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _collections_h
#define _collections_h

#include <iostream>
#include <sstream>

#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "random.h"
#undef INTERNAL_INCLUDE

// begin global namespace string read/writing functions from strlib.h

/**
 * Reads the next string from infile into the reference parameter str.
 * If the first character (other than whitespace) is either a single
 * or a double quote, this function reads characters up to the
 * matching quote, processing standard escape sequences as it goes.
 * If not, readString reads characters up to any of the characters
 * in the string STRING_DELIMITERS in the implementation file.
 *
 * @private
 */
bool readQuotedString(std::istream& is, std::string& str, bool throwOnError = true);

/**
 * Writes the string str to outfile surrounded by double quotes, converting
 * special characters to escape sequences, as necessary.  If the optional
 * parameter forceQuotes is explicitly set to false, quotes are included
 * in the output only if necessary.
 *
 * @private
 */
std::ostream& writeQuotedString(std::ostream& os, const std::string& str,
                                bool forceQuotes = true);

/**
 * Checks whether the string needs quoting in order to be read correctly.
 * @private
 */
bool stringNeedsQuoting(const std::string& str);

/**
 * Writes a generic value to the output stream.  If that value is a string,
 * this function uses writeQuotedString to write the value.
 * @private
 */
template <typename ValueType>
std::ostream& writeGenericValue(std::ostream& os, const ValueType& value, bool) {
    os << std::boolalpha << value;
    return os;
}

template <>
inline std::ostream& writeGenericValue(std::ostream& os, const std::string& value,
                              bool forceQuotes) {
    return writeQuotedString(os, value, forceQuotes);
}

template <typename ValueType>
inline std::string genericValueToString(const ValueType& value,
                                        bool forceQuotes = false) {
    std::ostringstream os;
    writeGenericValue(os, value, forceQuotes);
    return os.str();
}

template <>
inline std::string genericValueToString(const std::string& value,
                                        bool forceQuotes) {
    std::ostringstream os;
    writeQuotedString(os, value, forceQuotes);
    return os.str();
}

/**
 * Reads a generic value from the input stream.  If that value is a string,
 * this function uses readQuotedString to read the value.
 * @private
 */
template <typename ValueType>
bool readGenericValue(std::istream& is, ValueType& value) {
    return (bool) (is >> value);
}

template <>
inline bool readGenericValue(std::istream& is, std::string& value) {
    return readQuotedString(is, value, /* throwOnError */ false);
}

// end of global namespace string read/writing functions from strlib.h
namespace stanfordcpplib {
namespace collections {

#ifdef SPL_THROW_ON_INVALID_ITERATOR
template <typename CollectionType, typename IteratorType>
void checkVersion(const CollectionType& coll, const IteratorType& itr,
                  const std::string& memberName = "") {
    unsigned int collVersion = coll.version();
    unsigned int itrVersion = itr.version();
    if (itrVersion != collVersion) {
        std::string msg = memberName;
        if (!msg.empty()) {
            msg += ": ";
        }
        msg += "Collection modified during iteration. Iterator is now invalid.\n";
        msg += "Do not modify a collection during a for-each loop or iterator traversal.";
        error(msg);
    }
}
#else // SPL_THROW_ON_INVALID_ITERATOR
template <typename CollectionType, typename IteratorType>
void checkVersion(const CollectionType&, const IteratorType&,
                  const std::string& = "") {
    // empty
}
#endif

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
 * Compares two values and returns an integer indicating their relative order,
 * in the general style of Java's compareTo method:
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The type passed must support a < less-than operator.
 */
template <typename T>
int compareTo(T t1, T t2) {
    if (t1 < t2) {
        return -1;
    } else if (t2 < t1) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Compares two pairs of values and returns an integer indicating their relative order,
 * in the general style of Java's compareTo method.
 * First the values t1 and t2 are compared.
 * If they are equal, ties are broken by comparing t3 and t4.
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The types passed must support a < less-than operator.
 */
template <typename T1, typename T2>
int compareTo2(T1 t1, T1 t2, T2 t3, T2 t4) {
    if (t3 < t4) {
        return -1;
    } else if (t4 < t3) {
        return 1;
    } else {
        return compareTo(t1, t2);
    }
}

/*
 * Compares three pairs of values and returns an integer indicating their relative order,
 * in the general style of Java's compareTo method.
 * First the values t1 and t2 are compared.
 * If t1 and t2 are equal, ties are broken by comparing t3 and t4.
 * If t3 and t4 are equal, ties are broken by comparing t5 and t6.
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The types passed must support a < less-than operator.
 */
template <typename T1, typename T2, typename T3>
int compareTo3(T1 t1, T1 t2, T2 t3, T2 t4, T3 t5, T3 t6) {
    if (t5 < t6) {
        return -1;
    } else if (t6 < t5) {
        return 1;
    } else {
        return compareTo2(t1, t2, t3, t4);
    }
}

/*
 * Compares four pairs of values and returns an integer indicating their relative order,
 * in the general style of Java's compareTo method.
 * First the values t1 and t2 are compared.
 * If t1 and t2 are equal, ties are broken by comparing t3 and t4.
 * If t3 and t4 are equal, ties are broken by comparing t5 and t6.
 * If t5 and t6 are equal, ties are broken by comparing t7 and t8.
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The types passed must support a < less-than operator.
 */
template <typename T1, typename T2, typename T3, typename T4>
int compareTo4(T1 t1, T1 t2, T2 t3, T2 t4, T3 t5, T3 t6, T4 t7, T4 t8) {
    if (t7 < t8) {
        return -1;
    } else if (t8 < t7) {
        return 1;
    } else {
        return compareTo3(t1, t2, t3, t4, t5, t6);
    }
}

/*
 * Compares five pairs of values and returns an integer indicating their relative order,
 * in the general style of Java's compareTo method.
 * First the values t1 and t2 are compared.
 * If t1 and t2 are equal, ties are broken by comparing t3 and t4.
 * If t3 and t4 are equal, ties are broken by comparing t5 and t6.
 * If t5 and t6 are equal, ties are broken by comparing t7 and t8.
 * If t7 and t8 are equal, ties are broken by comparing t9 and t10.
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The types passed must support a < less-than operator.
 */
template <typename T1, typename T2, typename T3, typename T4, typename T5>
int compareTo5(T1 t1, T1 t2, T2 t3, T2 t4, T3 t5, T3 t6, T4 t7, T4 t8, T5 t9, T5 t10) {
    if (t9 < t10) {
        return -1;
    } else if (t10 < t9) {
        return 1;
    } else {
        return compareTo4(t1, t2, t3, t4, t5, t6, t7, t8);
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
 * Returns true if the two collections contain the same elements in the same order.
 * The element type must be double, float, or any floating-point type.
 */
template <typename CollectionType>
bool equalsDouble(const CollectionType& coll1, const CollectionType& coll2) {
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
        if (!floatingPointEqual(*itr1, *itr2)) {
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
 * Writes out any collection to the given output stream using its two iterators,
 * begin and end.
 */
template <typename IteratorType>
std::ostream& writeIterableOfPointers(std::ostream& out, IteratorType begin, IteratorType end) {
    out << "{";
    bool first = true;
    while (begin != end) {
        if (!first) {
            out << ", ";
        }
        first = false;
        writeGenericValue(out, **begin, /* forceQuotes */ true);
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
std::ostream& writeCollectionOfPointers(std::ostream& out, CollectionType collection) {
    return writeIterableOfPointers(out, collection.begin(), collection.end());
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

#endif // _collections_h
