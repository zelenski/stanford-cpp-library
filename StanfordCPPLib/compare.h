/*
 * File: compare.h
 * ---------------
 * Contains general-purpose functions for comparing any collections that have
 * a visible iterator (begin(), end()).
 * Used to implement comparison operators like < and >= on collections.
 * 
 * @author Marty Stepp
 * @version 2014/10/10
 * @since 2014/10/10
 */

#ifndef _compare_h
#define _compare_h

namespace compare {
template <typename Collection>
int compare(const Collection& v1, const Collection& v2) {
    // optimization: if they are the same object, then they are equal
    if (&v1 == &v2) {
        return 0;
    }
    
    auto itr1 = v1.begin();
    auto itr2 = v2.begin();
    auto v1end = v1.end();
    auto v2end = v2.end();
    
    while (itr1 != v1end && itr2 != v2end) {
        // compare each pair of elements from iterators
        auto val1 = *itr1;
        auto val2 = *itr2;
        
        // TO STUDENT:
        // If the line below is failing to compile in your program, it probably
        // means that you are trying to make a nested collection
        // (e.g. Set<Vector<T>>) for some element type T that does not have a
        // less-than < operator.  That operator is *required* in order to make
        // a Set or Map of Vectors, so that the set/map knows how to sort the
        // elements into their ascending order.
        // You should either add a < operator to your class, or consider a
        // different nested collection solution.  Good luck!
        if (val1 < val2) {
            return -1;
        } else if (val2 < val1) {
            return 1;
        }
        
        // advance to next element
        itr1++;
        itr2++;
    }
    
    // if we get here, everything from v1 matched v2, so they are either equal,
    // or one is shorter than the other (fewer elements) and is therefore less
    if (itr1 == v1end && itr2 == v2end) {
        return 0;
    } else if (itr1 == v1end) {
        return -1;
    } else {
        return 1;
    }
}

template <typename MapType>
int compareMaps(const MapType& v1, const MapType& v2) {
    // optimization: if they are the same object, then they are equal
    if (&v1 == &v2) {
        return 0;
    }
    
    auto itr1 = v1.begin();
    auto itr2 = v2.begin();
    auto v1end = v1.end();
    auto v2end = v2.end();
    
    while (itr1 != v1end && itr2 != v2end) {
        // compare each pair of elements from iterators
        auto key1 = *itr1;
        auto key2 = *itr2;
        
        auto value1 = v1[key1];
        auto value2 = v2[key2];
        
        // TO STUDENT:
        // If the line below is failing to compile in your program, it probably
        // means that you are trying to make a nested collection
        // (e.g. Set<Map<K, V>>) for some element type K or V that does not have a
        // less-than < operator.  That operator is *required* in order to make
        // a Set or Map of Maps, so that the set/map knows how to sort the
        // keys and values into their ascending order.
        // You should either add a < operator to your key/value types, or consider a
        // different nested collection solution.  Good luck!
        if (key1 < key2) {
            return -1;
        } else if (key2 < key1) {
            return 1;
        }
        
        // key1 == key2
        if (value1 < value2) {
            return -1;
        } else if (value2 < value1) {
            return 1;
        }
        
        // advance to next element
        itr1++;
        itr2++;
    }
    
    // if we get here, everything from v1 matched v2, so they are either equal,
    // or one is shorter than the other (fewer elements) and is therefore less
    if (itr1 == v1end && itr2 == v2end) {
        return 0;
    } else if (itr1 == v1end) {
        return -1;
    } else {
        return 1;
    }
}
} // namespace compare

#endif
