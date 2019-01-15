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
    return os << std::boolalpha << value;
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

/*
 * Type responsible for tracking the version of some object. This is factored out into
 * its own object with unusual copy functions so that any time the underlying object
 * is moved or assigned the underlying version number is updated.
 */
class VersionTracker {
public:
    /* Assigning a VersionTracker increments the underlying version number. */
    VersionTracker& operator= (VersionTracker) {
        ++mVersion;
        return *this;
    }

    /* Move-constructing a VersionTracker implements the version number of the
     * object being moved.
     */
    VersionTracker(VersionTracker&& rhs) {
        rhs.mVersion++;
    }

    /* Use default constructor and default copy constructor. */
    VersionTracker() = default;
    VersionTracker(const VersionTracker &) = default;

    /* Marks that the version must be updated. */
    void update() {
        ++mVersion;
    }

    /* Returns the version number. */
    unsigned int version() const {
        return mVersion;
    }

private:
    unsigned int mVersion = 0;
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
        : mVersion(owner->version()), mOwner(owner), mIter(iter), mBegin(c.begin()), mEnd(c.end()) {

    }

    /*
     * We're friends with all other CheckedIterator types, allowing for cross-construction
     * and the like.
     */
    template <typename OtherItr> friend class CheckedIterator;

    /* Conversion constructor, when permitted. */
    template <typename OtherItr> CheckedIterator(const CheckedIterator<OtherItr>& rhs)
        : mVersion(rhs.mVersion),
          mOwner(rhs.mOwner),
          mIter(rhs.mIter),
          mBegin(rhs.mBegin),
          mEnd(rhs.mEnd) {

    }

    template <typename OtherItr> operator CheckedIterator<OtherItr>() const {
        return CheckedIterator<OtherItr>{mVersion, mOwner, mIter, mBegin, mEnd};
    }

    /* All possible iterator functions. */

    /* Comparison operators. */
    template <typename OtherItr> bool operator ==(const CheckedIterator<OtherItr>& rhs) const {
        if (!mOwner || !rhs.mOwner) error("Cannot compare an uninitialized iterator.");
        if ( mOwner !=  rhs.mOwner) error("Cannot compare iterators from two different containers.");
        return mIter == rhs.mIter;
    }
    template <typename OtherItr> bool operator !=(const CheckedIterator<OtherItr>& rhs) const {
        return !(*this == rhs);
    }

    /*
     * We report errors if the underlying owners are different, since otherwise
     * the behavior is undefined.
     */
    template <typename OtherItr> bool operator <(const CheckedIterator<OtherItr>& rhs) const {
        if (!mOwner || !rhs.mOwner) {
            error("Cannot compare an uninitialized iterator.");
        }
        if (mOwner != rhs.mOwner) {
            error("Cannot compare iterators from different containers.");
        }
        return mIter < rhs.mIter;
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
        if (!mOwner) {
            error("Cannot access elements through an uninitialized iterator.");
        }
        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);
        if (index >= 0 &&  index >= mEnd - mIter) {
            error("Out of bounds.");
        }
        if (index <  0 && -index >  mIter - mBegin) {
            error("Out of bounds.");
        }

        return mIter[index];
    }

    CheckedIterator& operator +=(difference_type index) {
        if (!mOwner) error("Cannot advance uninitialized iterators.");
        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);
        mIter += index;
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
        if (!mOwner || !rhs.mOwner) {
            error("Cannot subtract uninitialized iterators.");
        }

        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);
        if (mOwner != rhs.mOwner) {
            error("Cannot subtract iterators from two different containers.");
        }

        return mIter - rhs.mIter;
    }

    /* Forwards and backwards. */
    CheckedIterator& operator ++() {
        if (!mOwner) {
            error("Cannot advance an uninitialized iterator.");
        }

        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);
        if (mIter == mEnd) {
            error("Cannot advance an iterator past end of range.");
        }
        ++mIter;
        return *this;
    }
    CheckedIterator operator ++(int) {
        auto result = *this;
        ++*this;
        return result;
    }

    CheckedIterator& operator --() {
        if (!mOwner) {
            error("Cannot back up an uninitialized iterator.");
        }

        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);
        if (mIter == mBegin) {
            error("Cannot back up an iteartor before start of range.");
        }

        --mIter;
        return *this;
    }
    CheckedIterator operator --(int) {
        auto result = *this;
        --*this;
        return result;
    }

    /* Dereferencing. */
    reference operator *() const {
        if (!mOwner) {
            error("Cannot dereference an uninitialized iterator.");
        }
        ::stanfordcpplib::collections::checkVersion(*mOwner, *this);

        if (mIter == mEnd) {
            error("Iterator out of range.");
        }
        return *mIter;
    }
    pointer operator ->() const {
        return &**this;
    }

    /* Direct version access. */
    unsigned int version() const {
        if (!mOwner) {
            error("Cannot get version from an uninitialized iterator.");
        }
        return mVersion;
    }

private:
    unsigned int mVersion = 0;
    const VersionTracker* mOwner = nullptr;
    Iterator mIter;
    Iterator mBegin, mEnd;
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
    explicit ProjectingIterator(Iterator iter) : mIter(iter) {
        // Empty
    }

    /*
     * We're friends with all other related types, allowing for cross-construction
     * and the like.
     */
    template <typename OtherItr> friend class ProjectingIterator;

    /* Conversion constructor, when permitted. */
    template <typename OtherItr> ProjectingIterator(const ProjectingIterator<OtherItr>& rhs)
        : mIter(rhs.mIter) {
        // Empty
    }

    template <typename OtherItr> operator ProjectingIterator<OtherItr>() const {
        return ProjectingIterator<OtherItr>(mIter);
    }

    /* All possible iterator functions. */

    /* Comparison operators. */
    template <typename OtherItr> bool operator ==(const ProjectingIterator<OtherItr>& rhs) {
        return mIter == rhs.mIter;
    }
    template <typename OtherItr> bool operator !=(const ProjectingIterator<OtherItr>& rhs) {
        return !(*this == rhs);
    }

    template <typename OtherItr> bool operator <(const ProjectingIterator<OtherItr>& rhs) {
        return mIter < rhs.mIter;
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
        return mIter[index];
    }
    ProjectingIterator& operator +=(difference_type index) {
        mIter += index;
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
        return mIter - rhs.mIter;
    }

    /* Forwards and backwards. */
    ProjectingIterator& operator ++() {
        ++mIter;
        return *this;
    }
    ProjectingIterator operator ++(int) {
        auto result = *this;
        ++*this;
        return result;
    }

    ProjectingIterator& operator --() {
        --mIter;
        return *this;
    }
    ProjectingIterator operator --(int) {
        auto result = *this;
        --*this;
        return result;
    }

    /* Dereferencing. */
    reference operator *() const {
        return mIter->first;
    }
    pointer operator ->() const {
        return &**this;
    }

private:
    Iterator mIter;
};

} // namespace collections
} // namespace stanfordcpplib

#endif // _collections_h
