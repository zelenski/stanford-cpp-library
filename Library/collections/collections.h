/*
 * File: collections.h
 * -------------------
 * Contains general-purpose functions for use with many collections.
 *
 * For example, we have functions for comparing any collections that have
 * a visible iterator (begin(), end()).
 * Used to implement comparison operators like < and >= on collections.
 *
 * @version 2020/09/12
 * - simplify interface in preparation for install
 * @author Marty Stepp
 * @version 2019/10/12
 * - added ARRAY_LENGTH macro
 * @version 2019/04/12
 * - added GenericSet unionWith, intersect, difference methods
 * - added functions to read/write quoted char values
 * - changed comment formatting
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

#ifndef _collections_h
#define _collections_h

#include <iostream>
#include <sstream>
#include <type_traits>

#include "error.h"
#include "gmath.h"
#include "hashcode.h"
#include "random.h"

// begin global namespace string read/writing functions from strlib.h

/**
 * Reads the next char from infile into the reference parameter ch.
 * If the first character (other than whitespace) is either a single
 * or a double quote, this function reads characters up to the
 * matching quote, processing standard escape sequences as it goes.
 * If not, readQuotedChar reads characters up to any of the characters
 * in the string STRING_DELIMITERS in the implementation file.
 *
 * @private
 */
bool readQuotedChar(std::istream& is, char& ch, bool throwOnError = true);

/**
 * Reads the next string from infile into the reference parameter str.
 * If the first character (other than whitespace) is either a single
 * or a double quote, this function reads characters up to the
 * matching quote, processing standard escape sequences as it goes.
 * If not, readQuoted String reads characters up to any of the characters
 * in the string STRING_DELIMITERS in the implementation file.
 *
 * @private
 */
bool readQuotedString(std::istream& is, std::string& str, bool throwOnError = true);

/**
 * Writes the char ch to outfile surrounded by single quotes, converting
 * special characters to escape sequences, as necessary.  If the optional
 * parameter forceQuotes is explicitly set to false, quotes are included
 * in the output only if necessary.
 *
 * @private
 */
std::ostream& writeQuotedChar(std::ostream& os, char ch, bool forceQuotes = true);

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
    return os << std::boolalpha << value;
}

inline std::ostream& writeGenericValue(std::ostream& os, char value,
                                       bool forceQuotes) {
    return writeQuotedChar(os, value, forceQuotes);
}

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

inline bool readGenericValue(std::istream& is, char& value) {
    return readQuotedChar(is, value, /* throwOnError */ false);
}

inline bool readGenericValue(std::istream& is, std::string& value) {
    return readQuotedString(is, value, /* throwOnError */ false);
}

// end of global namespace string read/writing functions from strlib.h
namespace stanfordcpplib {
namespace collections {

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
        msg += "A collection was modified during iteration, which invalidates operation.\n";
        msg += "Do not modify a collection within a for-each loop!";
        error(msg);
    }
}

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
 * Template functions to compare two interleaved sequences of values, returning
 * -1 if the first value is less than the second,
 *  0 if the values are equal,
 *  1 if the first value is greater than the second.
 * The type passed must support a < less-than operator.
 */
inline int compareTo() {
    return 0;
}
template <typename T, typename... Rest>
int compareTo(const T& first, const T& second, const Rest&... rest) {
    if (first < second) return -1;
    if (second < first) return +1;
    return compareTo(rest...);
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
    auto end2 = coll2.end();
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
    auto end2 = coll2.end();
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

    // check whether each element in the first map is also in the second.
    // since the sizes are the same, if this is true, the maps are equal.
    for (auto itr1 = map1.begin(), end1 = map1.end(); itr1 != end1; ++itr1) {
        if (!map2.containsKey(*itr1) || !(map1.get(*itr1) == map2.get(*itr1))) {
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
template <typename Collection>
auto randomElement(const Collection& collection) -> const decltype(*collection.begin())& {
    if (collection.isEmpty()) {
        error("randomElement: empty collection was passed");
    }
    return *std::next(collection.begin(), randomInteger(0, collection.size() - 1));
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


template <typename CollectionType, typename ElementType>
    void readOne(CollectionType& collection, const ElementType& elem)
        { collection.add(elem); }
/*
 * Reads in any collection from the given input stream.
 * The collection must eiither have an add() method that takes a single value,
 * (or provide own callback to use as add operation)
 * and a clear() method that removes all elements from the collection.
 */
template <typename CollectionType, typename ElementType>
std::istream& readCollection(std::istream& input, CollectionType& collection, ElementType& element, std::string /* descriptor */,
        void (*fn)(CollectionType&, const ElementType&) = readOne<CollectionType,ElementType>)
{

    char ch = '\0';
    input >> ch;
    if (ch != '{') {
        input.setstate(std::ios_base::failbit);
        return input;
    }
    collection.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            if (!readGenericValue(input, element)) {
                return input;
            }
            fn(collection, element);
            input >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                input.setstate(std::ios_base::failbit);
                return input;
            }
        }
    }
    return input;
}


template <typename CollectionType, typename KeyType, typename ValueType>
    void readOne(CollectionType& collection, const KeyType& key, const ValueType& value)
        { collection.put(key, value); }
/*
 * Reads in any paired collection (Map or Priority Queue) from the given input stream.
 * The collection must have a put() method that takes key and value
 * (or provide a callback to use as add operation)
 * and a clear() method that removes all elements from the collection.
 */

template <typename CollectionType, typename KeyType, typename ValueType>
std::istream& readPairedCollection(std::istream& input, CollectionType& collection, KeyType& key, ValueType& value, std::string /* descriptor */,
        void (*fn)(CollectionType&, const KeyType&, const ValueType&) = readOne<CollectionType,KeyType,ValueType>)
{
    char ch = '\0';
    input >> ch;
    if (ch != '{') {
        input.setstate(std::ios_base::failbit);
        return input;
    }
    collection.clear();
    input >> ch;
    if (ch != '}') {
        input.unget();
        while (true) {
            if (!readGenericValue(input, key)) {
                return input;
            }
            input >> ch;
            if (ch != ':') {
                input.setstate(std::ios_base::failbit);
                return input;
            }
            if (!readGenericValue(input, value)) {
                return input;
            }
            fn(collection, key, value);
            input >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
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

/*
 * Type responsible for tracking the version of some object. This is factored out into
 * its own object with unusual copy functions so that any time the underlying object
 * is moved or assigned the underlying version number is updated.
 */
class VersionTracker {
public:
    /* Assigning a VersionTracker increments the underlying version number. */
    VersionTracker& operator= (VersionTracker) {
        ++_version;
        return *this;
    }

    /* Move-constructing a VersionTracker implements the version number of the
     * object being moved.
     */
    VersionTracker(VersionTracker&& rhs) {
        rhs._version++;
    }

    /* Use default constructor and default copy constructor. */
    VersionTracker() = default;
    VersionTracker(const VersionTracker &) = default;

    /* Marks that the version must be updated. */
    void update() {
        ++_version;
    }

    /* Returns the version number. */
    unsigned int version() const {
        return _version;
    }

private:
    unsigned int _version = 0;
};

/*
 * Checked iterator type that wraps an underlying iterator type, adding in bounds-checking
 * and version-checking.
 */
template <typename Iterator> class CheckedIterator {
public:
    /* We're whatever sort of iterator we're wrapping. */
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;

    /*
     * Default constructor must be explicitly declared so that the private constructor
     * doesn't shadow us.
     */
    CheckedIterator() = default;

    /* Constructs an iterator given information about the underlying container. */
    template <typename Container>
    CheckedIterator(const VersionTracker* owner, Iterator iter, Container& c)
        : _version(owner->version()), _owner(owner), _iter(iter), _begin(c.begin()), _end(c.end()) {

    }

    /*
     * We're friends with all other CheckedIterator types, allowing for cross-construction
     * and the like.
     */
    template <typename OtherItr> friend class CheckedIterator;

    /* Conversion constructor, when permitted. */
    template <typename OtherItr> CheckedIterator(const CheckedIterator<OtherItr>& rhs)
        : _version(rhs._version),
          _owner(rhs._owner),
          _iter(rhs._iter),
          _begin(rhs._begin),
          _end(rhs._end) {

    }

    template <typename OtherItr> operator CheckedIterator<OtherItr>() const {
        return CheckedIterator<OtherItr>{_version, _owner, _iter, _begin, _end};
    }

    /* All possible iterator functions. */

    /* Comparison operators. */
    template <typename OtherItr> bool operator ==(const CheckedIterator<OtherItr>& rhs) const {
        if (!_owner || !rhs._owner) {
            error("Cannot compare an uninitialized iterator.");
        }
        if ( _owner !=  rhs._owner) {
            error("Cannot compare iterators from two different containers.");
        }
        return _iter == rhs._iter;
    }
    template <typename OtherItr> bool operator !=(const CheckedIterator<OtherItr>& rhs) const {
        return !(*this == rhs);
    }

    /*
     * We report errors if the underlying owners are different, since otherwise
     * the behavior is undefined.
     */
    template <typename OtherItr> bool operator <(const CheckedIterator<OtherItr>& rhs) const {
        if (!_owner || !rhs._owner) {
            error("Cannot compare an uninitialized iterator.");
        }
        if (_owner != rhs._owner) {
            error("Cannot compare iterators from different containers.");
        }
        return _iter < rhs._iter;
    }
    template <typename OtherItr> bool operator >(const CheckedIterator<OtherItr>& rhs) const {
        return rhs < *this;
    }
    template <typename OtherItr> bool operator <=(const CheckedIterator<OtherItr>& rhs) const {
        return !(*this > rhs);
    }
    template <typename OtherItr> bool operator >=(const CheckedIterator<OtherItr>& rhs) const {
        return !(*this < rhs);
    }

    /* Random access. */
    reference operator [](difference_type index) const {
        if (!_owner) {
            error("Cannot access elements through an uninitialized iterator.");
        }
        ::stanfordcpplib::collections::checkVersion(*_owner, *this);
        if (index >= 0 &&  index >= _end - _iter) {
            error("Out of bounds.");
        }
        if (index <  0 && -index >  _iter - _begin) {
            error("Out of bounds.");
        }

        return _iter[index];
    }

    CheckedIterator& operator +=(difference_type index) {
        if (!_owner) error("Cannot advance uninitialized iterators.");
        ::stanfordcpplib::collections::checkVersion(*_owner, *this);
        _iter += index;
        return *this;
    }
    CheckedIterator& operator -=(difference_type index) {
        return *this += (-index);
    }

    CheckedIterator operator +(difference_type index) const {
        auto result = *this;
        return result += index;
    }
    CheckedIterator operator -(difference_type index) const {
        return *this + (-index);
    }

    template <typename OtherItr>
    difference_type operator -(const CheckedIterator<OtherItr>& rhs) const {
        if (!_owner || !rhs._owner) {
            error("Cannot subtract uninitialized iterators.");
        }

        ::stanfordcpplib::collections::checkVersion(*_owner, *this);
        if (_owner != rhs._owner) {
            error("Cannot subtract iterators from two different containers.");
        }

        return _iter - rhs._iter;
    }

    /* Forwards and backwards. */
    CheckedIterator& operator ++() {
        if (!_owner) {
            error("Cannot advance an uninitialized iterator.");
        }

        ::stanfordcpplib::collections::checkVersion(*_owner, *this);
        if (_iter == _end) {
            error("Cannot advance an iterator past end of range.");
        }
        ++_iter;
        return *this;
    }
    CheckedIterator operator ++(int) {
        auto result = *this;
        ++*this;
        return result;
    }

    CheckedIterator& operator --() {
        if (!_owner) {
            error("Cannot back up an uninitialized iterator.");
        }

        ::stanfordcpplib::collections::checkVersion(*_owner, *this);
        if (_iter == _begin) {
            error("Cannot back up an iteartor before start of range.");
        }

        --_iter;
        return *this;
    }
    CheckedIterator operator --(int) {
        auto result = *this;
        --*this;
        return result;
    }

    /* Dereferencing. */
    reference operator *() const {
        if (!_owner) {
            error("Cannot dereference an uninitialized iterator.");
        }
        ::stanfordcpplib::collections::checkVersion(*_owner, *this);

        if (_iter == _end) {
            error("Iterator out of range.");
        }
        return *_iter;
    }
    pointer operator ->() const {
        return &**this;
    }

    /* Direct version access. */
    unsigned int version() const {
        if (!_owner) {
            error("Cannot get version from an uninitialized iterator.");
        }
        return _version;
    }

private:
    unsigned int _version = 0;
    const VersionTracker* _owner = nullptr;
    Iterator _iter;
    Iterator _begin, _end;
};

/*
 * Iterator over a pairs that projects out the first component. Essentially, this turns an
 * iterator over pair<const Key, Value> into an iterator over const Key.
 *
 * All bounds-checking, error-handling, etc. are presumed to come from the underlying
 * iterator type.
 */
template <typename Iterator> class ProjectingIterator {
public:
    /* The sort of thing that we're wrapping. */
    using value_type       = typename std::remove_reference<decltype(std::declval<Iterator>()->first)>::type;

    /* We're whatever sort of iterator we're wrapping. */
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using pointer           = const value_type *;
    using reference         = const value_type &;

    /*
     * Default constructor must be explicitly declared so that the private constructor
     * doesn't shadow us.
     */
    ProjectingIterator() = default;

    /* Wraps an existing iterator. */
    explicit ProjectingIterator(Iterator iter) : _iter(iter) {
        // Empty
    }

    /*
     * We're friends with all other related types, allowing for cross-construction
     * and the like.
     */
    template <typename OtherItr> friend class ProjectingIterator;

    /* Conversion constructor, when permitted. */
    template <typename OtherItr> ProjectingIterator(const ProjectingIterator<OtherItr>& rhs)
        : _iter(rhs._iter) {
        // Empty
    }

    template <typename OtherItr> operator ProjectingIterator<OtherItr>() const {
        return ProjectingIterator<OtherItr>(_iter);
    }

    /* All possible iterator functions. */

    /* Comparison operators. */
    template <typename OtherItr> bool operator ==(const ProjectingIterator<OtherItr>& rhs) {
        return _iter == rhs._iter;
    }
    template <typename OtherItr> bool operator !=(const ProjectingIterator<OtherItr>& rhs) {
        return !(*this == rhs);
    }

    template <typename OtherItr> bool operator <(const ProjectingIterator<OtherItr>& rhs) {
        return _iter < rhs._iter;
    }
    template <typename OtherItr> bool operator >(const ProjectingIterator<OtherItr>& rhs) {
        return rhs < *this;
    }
    template <typename OtherItr> bool operator <=(const ProjectingIterator<OtherItr>& rhs) {
        return !(*this > rhs);
    }
    template <typename OtherItr> bool operator >=(const ProjectingIterator<OtherItr>& rhs) {
        return !(*this < rhs);
    }

    /* Random access. */
    reference operator [](difference_type index) const {
        return _iter[index];
    }
    ProjectingIterator& operator +=(difference_type index) {
        _iter += index;
        return *this;
    }
    ProjectingIterator& operator -=(difference_type index) {
        return *this += (-index);
    }
    ProjectingIterator operator +(difference_type index) const {
        auto result = *this;
        return result += index;
    }
    ProjectingIterator operator -(difference_type index) const {
        return *this + (-index);
    }
    template <typename OtherItr>
    difference_type operator -(const ProjectingIterator<OtherItr>& rhs) const {
        return _iter - rhs._iter;
    }

    /* Forwards and backwards. */
    ProjectingIterator& operator ++() {
        ++_iter;
        return *this;
    }
    ProjectingIterator operator ++(int) {
        auto result = *this;
        ++*this;
        return result;
    }

    ProjectingIterator& operator --() {
        --_iter;
        return *this;
    }
    ProjectingIterator operator --(int) {
        auto result = *this;
        --*this;
        return result;
    }

    /* Dereferencing. */
    reference operator *() const {
        return _iter->first;
    }
    pointer operator ->() const {
        return &**this;
    }

private:
    Iterator _iter;
};

/**
 * Class: GenericSet<SetTraits>
 * ----------------------------
 * This class stores a collection of distinct elements. SetTraits should be
 * a type containing the following:
 *
 *     typename ValueType:          whatever is stored in the map
 *     typename MapType:            should be a Map<ValueType, bool>
 *     static std::string name():   should return the name of the type.
 *
 * There's one more requirement: you need to define a function
 *
 *     template <typename... Args>
 *        static MapType construct(Args&&... args)
 *
 * that constructs an internal MapType object with the specified arguments.
 * This function should do something creative or clever to ensure that there
 * is a nice compiler error generated in the event that the arguments are
 * invalid, since otherwise the error is going to be deeply nested inside the
 * GenericSet template.
 *
 * This is not meant to be used directly by students.
 */
template <typename SetTraits>
class GenericSet {
public:
    /**
     * Utility alias to make things easier to work with.
     */
    using value_type = typename SetTraits::ValueType;

    /**
     * Constructor: GenericSet
     * Usage: GenericSet<ValueType, SetTraits> set;
     * --------------------------------------------
     * Initializes an empty set of the specified element type.
     */
    GenericSet() = default;

    /**
     * Constructor: GenericSet
     * Usage: GenericSet<ValueType, SetTraits> set {1, 2, 3};
     * ------------------------------------------------------
     * Initializes a new set that stores the given elements.
     */
    GenericSet(std::initializer_list<value_type> list);

    /**
     * Constructor: GenericSet
     * Usage: GenericSet<ValueType, SetTraits> set(... things for the map ...);
     * ------------------------------------------------------------------------
     * Forwards the specified arguments down to the underlying Map type.
     */
    template <typename... Args>
    explicit GenericSet(Args... args);

    /**
     * Constructor: GenericSet
     * Usage: GenericSet<ValueType, SetTraits> set({1, 2, 3}, ... things for the map ...);
     * -----------------------------------------------------------------------------------
     * Constructs a set using the specified elements, forwarding the arguments to the
     * underlying map.
     */
    template <typename... Args>
    GenericSet(std::initializer_list<value_type> list,
               Args... args);

    /**
     * Destructor: ~Set
     * ----------------
     * Frees any heap storage associated with this set.
     */
    virtual ~GenericSet() = default;

    /**
     * Method: add
     * Usage: set.add(value);
     * ----------------------
     * Adds an element to this set, if it was not already there.
     */
    void add(const value_type& value);

    /**
     * Method: back
     * Usage: ValueType value = set.last();
     * ------------------------------------
     * Returns the last value in the set in the order established by the
     * <code>for-each</code> loop.  If the set is empty, generates an error.
     */
    value_type last() const;

    /**
     * Method: clear
     * Usage: set.clear();
     * -------------------
     * Removes all elements from this set.
     */
    void clear();

    /**
     * Method: contains
     * Usage: if (set.contains(value)) ...
     * -----------------------------------
     * Returns <code>true</code> if the specified value is in this set.
     */
    bool contains(const value_type& value) const;

    /**
     * Method: difference
     * Usage: set.difference(set2);
     * ----------------------------
     * Removes all elements of the given other set from this set.
     * You can also pass an initializer list such as {1, 2, 3}.
     * Returns a reference to this set.
     *
     * Note that this function modifies the current set in place rather than
     * returning a new set. If you want a new set, consider using the - operator
     * instead (not -=), which returns a newly created copy set.
     *
     * Identical in behavior to the -= operator.
     */
    GenericSet& difference(const GenericSet& set);

    /**
     * Method: equals
     * Usage: if (set.equals(set2)) ...
     * --------------------------------
     * Returns <code>true</code> if this set contains exactly the same values
     * as the given other set.
     * Identical in behavior to the == operator.
     */
    bool equals(const GenericSet& set2) const;

    /**
     * Method: first
     * Usage: ValueType value = set.first();
     * -------------------------------------
     * Returns the first value in the set in the order established by the
     * <code>for-each</code> loop.  If the set is empty, <code>first</code>
     * generates an error.
     */
    value_type first() const;

    /**
     * Method: intersect
     * Usage: set.intersect(set2);
     * ---------------------------
     * Removes all elements from this set that are not contained in the given
     * other set.
     * You can also pass an initializer list such as {1, 2, 3}.
     * Returns a reference to this set.
     *
     * Note that this function modifies the current set in place rather than
     * returning a new set. If you want a new set, consider using the * operator
     * instead (not *=), which returns a newly created copy set.
     *
     * Identical in behavior to the *= operator.
     */
    GenericSet& intersect(const GenericSet& set);

    /**
     * Method: isEmpty
     * Usage: if (set.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this set contains no elements.
     */
    bool isEmpty() const;

    /**
     * Method: isSubsetOf
     * Usage: if (set.isSubsetOf(set2)) ...
     * ------------------------------------
     * Implements the subset relation on sets.  It returns
     * <code>true</code> if every element of this set is
     * contained in <code>set2</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     */
    bool isSubsetOf(const GenericSet& set2) const;

    /**
     * Method: isSupersetOf
     * Usage: if (set.isSupersetOf(set2)) ...
     * --------------------------------------
     * Implements the superset relation on sets.  It returns
     * <code>true</code> if every element of this set is
     * contained in <code>set2</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     */
    bool isSupersetOf(const GenericSet& set2) const;

    /**
     * Method: mapAll
     * Usage: set.mapAll(fn);
     * ----------------------
     * Iterates through the elements of the set and calls <code>fn(value)</code>
     * for each one.  The iteration order matches the underlying order in which
     * the elements are stored.  For Set, this is sorted order according to the
     * comparison function and for HashSet, this is whatever order the elements
     * happen to be in.
     */
    void mapAll(std::function<void (const value_type&)> fn) const;

    /**
     * Method: remove
     * Usage: set.remove(value);
     * -------------------------
     * Removes an element from this set.  If the value was not
     * contained in the set, no error is generated and the set
     * remains unchanged.
     */
    void remove(const value_type& value);

    /**
     * Method: size
     * Usage: count = set.size();
     * --------------------------
     * Returns the number of elements in this set.
     */
    int size() const;

    /**
     * Method: toString
     * Usage: string str = set.toString();
     * -----------------------------------
     * Converts the set to a printable string representation.
     */
    std::string toString() const;

    /**
     * Method: unionWith
     * Usage: set.unionWith(set2);
     * ---------------------------
     * Adds all elements of the given other set to this set.
     * You can also pass an initializer list such as {1, 2, 3}.
     * Returns a reference to this set.
     * Identical in behavior to the += operator.
     *
     * Note that this function modifies the current set in place rather than
     * returning a new set. If you want a new set, consider using the + operator
     * instead (not +=), which returns a newly created copy set.
     *
     * (Implementation note: This function cannot be named 'union' because
     * that is a C/C++ keyword.)
     */
    GenericSet& unionWith(const GenericSet& set);

    /**
     * Operator: ==
     * Usage: set1 == set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * contain the same elements.
     */
    bool operator ==(const GenericSet& set2) const;

    /**
     * Operator: !=
     * Usage: set1 != set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * are different.
     */
    bool operator !=(const GenericSet& set2) const;

    /**
     * Operators: <, >, <=, >=
     * Usage: if (set1 <= set2) ...
     * ...
     * ----------------------------
     * Relational operators to compare two sets.
     * The <, >, <=, >= operators require that the value type has a < operator
     * so that the elements can be compared pairwise.
     *
     * These are implemented as friend functions so that if we fully instantiate
     * this type, we don't get errors when using relational operators.
     */
    template <typename Traits>
    friend bool operator <(const GenericSet<Traits>& set1, const GenericSet<Traits>& set2);
    template <typename Traits>
    friend bool operator <=(const GenericSet<Traits>& set1, const GenericSet<Traits>& set2);
    template <typename Traits>
    friend bool operator >(const GenericSet<Traits>& set1, const GenericSet<Traits>& set2);
    template <typename Traits>
    friend bool operator >=(const GenericSet<Traits>& set1, const GenericSet<Traits>& set2);

    /**
     * Operator: +
     * Usage: set1 + set2
     *        set1 + element
     * ---------------------
     * Returns the union of sets <code>set1</code> and <code>set2</code>, which
     * is the set of elements that appear in at least one of the two sets.
     * You can also pass an initializer list such as {1, 2, 3}.
     * The right hand set can be replaced by an element of the value type, in
     * which case the operator returns a new set formed by adding that element.
     */
    GenericSet operator +(const GenericSet& set2) const;
    GenericSet operator +(const value_type& element) const;

    /**
     * Operator: *
     * Usage: set1 * set2
     * ------------------
     * Returns the intersection of sets <code>set1</code> and <code>set2</code>,
     * which is the set of all elements that appear in both.
     * You can also pass an initializer list such as {1, 2, 3}.
     */
    GenericSet operator *(const GenericSet& set2) const;

    /**
     * Operator: -
     * Usage: set1 - set2
     *        set1 - element
     * ---------------------
     * Returns the difference of sets <code>set1</code> and <code>set2</code>,
     * which is all of the elements that appear in <code>set1</code> but
     * not <code>set2</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     * The right hand set can be replaced by an element of the value type, in
     * which case the operator returns a new set formed by removing that element.
     */
    GenericSet operator -(const GenericSet& set2) const;
    GenericSet operator -(const value_type& element) const;

    /**
     * Operator: +=
     * Usage: set1 += set2;
     *        set1 += value;
     * ---------------------
     * Adds all of the elements from <code>set2</code> (or the single
     * specified value) to <code>set1</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     * As a convenience, the <code>Set</code> package also overloads the comma
     * operator so that it is possible to initialize a set like this:
     *
     *<pre>
     *    Set&lt;int&gt; digits;
     *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
     *</pre>
     */
    GenericSet& operator +=(const GenericSet& set2);
    GenericSet& operator +=(const value_type& value);

    /**
     * Operator: *=
     * Usage: set1 *= set2;
     * --------------------
     * Removes any elements from <code>set1</code> that are not present in
     * <code>set2</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     */
    GenericSet& operator *=(const GenericSet& set2);

    /**
     * Operator: -=
     * Usage: set1 -= set2;
     *        set1 -= value;
     * ---------------------
     * Removes the elements from <code>set2</code> (or the single
     * specified value) from <code>set1</code>.
     * You can also pass an initializer list such as {1, 2, 3}.
     * As a convenience, the <code>Set</code> package also overloads the comma
     * operator so that it is possible to remove multiple elements from a set
     * like this:
     *
     *<pre>
     *    digits -= 0, 2, 4, 6, 8;
     *</pre>
     *
     * which removes the values 0, 2, 4, 6, and 8 from the set
     * <code>digits</code>.
     */
    GenericSet& operator -=(const GenericSet& set2);
    GenericSet& operator -=(const value_type& value);

    /*
     * Additional Set operations
     * -------------------------
     * In addition to the methods listed in this interface, the Set
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * The iteration forms process the Set in ascending order.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    typename SetTraits::MapType _map = SetTraits::construct();  /* Map used to store the elements    */
    bool _removeFlag = false;                                   /* Flag to differentiate += and -=   */

public:
    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support the comma operator, copying, and iteration.
     *
     * Including these methods in the public interface would make
     * that interface more difficult to understand for the average client.
     */
    GenericSet& operator ,(const value_type& value) {
        if (this->_removeFlag) {
            this->remove(value);
        } else {
            this->add(value);
        }
        return *this;
    }

    using const_iterator = typename SetTraits::MapType::const_iterator;
    using iterator = const_iterator;

    iterator begin() const {
        return _map.begin();
    }

    iterator end() const {
        return _map.end();
    }

    friend int hashCode(const GenericSet& set) {
        return hashCode(set._map);
    }
};

template <typename SetTraits>
GenericSet<SetTraits>::GenericSet(std::initializer_list<value_type> list)
    : _map(SetTraits::construct()) {
    /* Can't do addAll because that would recursively try constructing a GenericSet.
     * Instead, directly add everything here. This becomes the focal point for
     * all initializer_list conversions.
     */
    for (const auto& elem: list) {
        add(elem);
    }
}

template <typename SetTraits>
template <typename... Args>
GenericSet<SetTraits>::GenericSet(Args... args) : GenericSet({}, std::move(args)...) {
    // Handled by other constructor
}

template <typename SetTraits>
template <typename... Args>
GenericSet<SetTraits>::GenericSet(std::initializer_list<value_type> list, Args... args)
    : _map(SetTraits::construct(std::move(args)...)) {

    /* Can't do addAll because that would recursively try constructing a GenericSet.
     * Instead, directly add everything here. This becomes the focal point for
     * all initializer_list conversions.
     */
    for (const auto& elem: list) {
        add(elem);
    }
}

template <typename SetTraits>
void GenericSet<SetTraits>::add(const value_type& value) {
    _map.put(value, true);
}

template <typename SetTraits>
typename GenericSet<SetTraits>::value_type
GenericSet<SetTraits>::last() const {
    if (isEmpty()) {
        error(SetTraits::name() + "::back: set is empty");
    }
    return _map.lastKey();
}

template <typename SetTraits>
void GenericSet<SetTraits>::clear() {
    _map.clear();
}

template <typename SetTraits>
bool GenericSet<SetTraits>::contains(const value_type& value) const {
    return _map.containsKey(value);
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::difference(const GenericSet<SetTraits>& set2) {
    _map.removeAll(set2._map);
    return *this;
}

template <typename SetTraits>
bool GenericSet<SetTraits>::equals(const GenericSet& set2) const {
    // optimization: if literally same set, stop
    if (this == &set2) {
        return true;
    }

    /* We are equal if we have the same size and we're a subset of the other
     * set.
     */
    if (size() != set2.size()) {
        return false;
    }
    return isSubsetOf(set2);
}

template <typename SetTraits>
typename GenericSet<SetTraits>::value_type
GenericSet<SetTraits>::first() const {
    if (isEmpty()) {
        error(SetTraits::name() + "::first: set is empty");
    }
    return _map.firstKey();
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::intersect(const GenericSet<SetTraits>& set2) {
    _map.retainAll(set2._map);
    return *this;
}

template <typename SetTraits>
bool GenericSet<SetTraits>::isEmpty() const {
    return _map.isEmpty();
}

template <typename SetTraits>
bool GenericSet<SetTraits>::isSubsetOf(const GenericSet& set2) const {
    for (const auto& elem: *this) {
        if (!set2.contains(elem)) {
            return false;
        }
    }
    return true;
}

template <typename SetTraits>
bool GenericSet<SetTraits>::isSupersetOf(const GenericSet& set2) const {
    for (const auto& value: set2) {
        if (!contains(value)) {
            return false;
        }
    }
    return true;
}

template <typename SetTraits>
void GenericSet<SetTraits>::mapAll(std::function<void (const value_type &)> fn) const {
    _map.mapAll([&](const value_type& elem, bool) {
        fn(elem);
    });
}

template <typename SetTraits>
void GenericSet<SetTraits>::remove(const value_type& value) {
    _map.remove(value);
}

template <typename SetTraits>
int GenericSet<SetTraits>::size() const {
    return _map.size();
}

template <typename SetTraits>
std::string GenericSet<SetTraits>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::unionWith(const GenericSet<SetTraits>& set2) {
    for (const auto& value : set2) {
        add(value);
    }
    return *this;
}


/*
 * Implementation notes: set operators
 * -----------------------------------
 * The implementations for the set operators use iteration to walk
 * over the elements in one or both sets.
 */
template <typename SetTraits>
bool GenericSet<SetTraits>::operator ==(const GenericSet& set2) const {
    return equals(set2);
}

template <typename SetTraits>
bool GenericSet<SetTraits>::operator !=(const GenericSet& set2) const {
    return !equals(set2);
}

template <typename SetTraits>
bool operator <(const GenericSet<SetTraits>& set1, const GenericSet<SetTraits>& set2) {
    return set1._map < set2._map;
}

template <typename SetTraits>
bool operator <=(const GenericSet<SetTraits>& set1, const GenericSet<SetTraits>& set2) {
    return set1._map <= set2._map;
}

template <typename SetTraits>
bool operator >(const GenericSet<SetTraits>& set1, const GenericSet<SetTraits>& set2) {
    return set1._map > set2._map;
}

template <typename SetTraits>
bool operator >=(const GenericSet<SetTraits>& set1, const GenericSet<SetTraits>& set2) {
    return set1._map >= set2._map;
}

template <typename SetTraits>
GenericSet<SetTraits> GenericSet<SetTraits>::operator +(const GenericSet& set2) const {
    return GenericSet(*this).unionWith(set2);
}

template <typename SetTraits>
GenericSet<SetTraits> GenericSet<SetTraits>::operator +(const value_type& element) const {
    GenericSet result = *this;
    result.add(element);
    return result;
}

template <typename SetTraits>
GenericSet<SetTraits> GenericSet<SetTraits>::operator *(const GenericSet& set2) const {
    return GenericSet(*this).intersect(set2);
}

template <typename SetTraits>
GenericSet<SetTraits> GenericSet<SetTraits>::operator -(const GenericSet& set2) const {
    return GenericSet(*this).difference(set2);
}

template <typename SetTraits>
GenericSet<SetTraits> GenericSet<SetTraits>::operator -(const value_type& element) const {
    GenericSet result = *this;
    result.remove(element);
    return result;
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::operator +=(const GenericSet& set2) {
    _removeFlag = false;
    return unionWith(set2);
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::operator +=(const value_type& value) {
    add(value);
    _removeFlag = false;
    return *this;
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::operator *=(const GenericSet& set2) {
    return intersect(set2);
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::operator -=(const GenericSet& set2) {
    _removeFlag = true;
    return difference(set2);
}

template <typename SetTraits>
GenericSet<SetTraits>& GenericSet<SetTraits>::operator -=(const value_type& value) {
    remove(value);
    _removeFlag = true;
    return *this;
}

template <typename SetTraits>
std::ostream& operator <<(std::ostream& os, const GenericSet<SetTraits>& set) {
    return stanfordcpplib::collections::writeCollection(os, set);
}

template <typename SetTraits>
std::istream& operator >>(std::istream& is, GenericSet<SetTraits>& set) {
    typename SetTraits::ValueType element;
    return stanfordcpplib::collections::readCollection(is, set, element, /* descriptor */ SetTraits::name() + "::operator >>");
}


/*
 * Types used to automatically check whether a type is comparable using
 * the < operator and whether a type supports operator== and hashCode.
 *
 * This is used to provide better compiler diagnostics to students when
 * they try to instantiate our times incorrectly.
 *
 * Later on, when C++20 concepts are rolled out, we should consider
 * replacing this code with concepts.
 */
template <typename T>
struct IsLessThanComparable {
private:
    /* Use SFNIAE overloading to detect which of these two options to pick. */
    struct Yes{};
    struct No {};

    template <typename U>
    static Yes check(int,
                     decltype(std::declval<U>() < std::declval<U>()) = 0);
    template <typename U> static No  check(...);

public:
    static constexpr bool value =
            std::conditional<std::is_same<decltype(check<T>(0)), Yes>::value,
                             std::true_type,
                             std::false_type>::type::value;
};

template <typename T>
struct IsHashable {
private:
    /* Use SFNIAE overloading to detect which of these two options to pick. */
    struct Yes{};
    struct No {};

    template <typename U>
    static Yes check(int,
                     decltype(hashCode(std::declval<U>())) = 0,
                     decltype(std::declval<U>() == std::declval<U>()) = 0);
    template <typename U> static No  check(...);

public:
    static constexpr bool value =
            std::conditional<std::is_same<decltype(check<T>(0)), Yes>::value,
                             std::true_type,
                             std::false_type>::type::value;
};

/*
 * Returns std::less<T>, except with a nice static assertion wrapped around it to
 * make sure that in the event that T isn't comparable via <, the error message is
 * more readable.
 */
template <typename T>
std::function<bool (const T&, const T&)> checkedLess() {
    static_assert(IsLessThanComparable<T>::value,
                  "Oops! You tried using a type as a key in our Map without making it comparable. Click this error for more details.");
    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it probably means that you tried making a Map with a custom struct or class type
     * as the key type or a Set with a custom struct as a value type.
     *
     * In order to have a type be a key type in a Map - or to have a type be a value type
     * in a Set - it needs to have be comparable using the < operator. By default, types in C++
     * can't be compared using the < operator, hence the error.
     *
     * There are two ways to fix this. The first option would simply be to not use your custom
     * type as a key in the Map or value in a Set. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define an operator< function for your custom
     * type. Here's the syntax for doing that:
     *
     *     bool operator< (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         return compareTo(lhs.data1, rhs.data1,
     *                          lhs.data2, rhs.data2,
     *                          ...
     *                          lhs.dataN, rhs.dataN);
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
     *     bool operator< (const MyType& lhs, const MyType& rhs) {
     *         return compareTo(lhs.myInt,    rhs.myInt,
     *                          lhs.myString, rhs.myString);
     *     }
     *
     * Hope this helps!
     */
    return std::less<T>();
}

/*
 * Utility traits type that always contains a value that's false.
 */
template <typename... Args> struct Fail {
    static constexpr bool value = false;
};

} // namespace collections
} // namespace stanfordcpplib

#endif // _collections_h
