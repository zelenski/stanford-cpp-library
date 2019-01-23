/*
 * File: hashset.h
 * ---------------
 * This file exports the <code>HashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 * 
 * @version 2018/03/10
 * - added methods front, back
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via HashMap)
 * @version 2016/12/06
 * - slight speedup bug fix in equals() method
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/11
 * - added containsAll, isSupersetOf methods
 * @version 2016/08/10
 * - added support for std initializer_list usage, such as {1, 2, 3}
 *   in constructor, addAll, containsAll, isSubsetOf, isSupersetOf, removeAll,
 *   retainAll, and operators +, +=, -, -=, *, *=
 * - bug fix in hashCode function
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added template hashCode function
 * @version 2014/10/10
 * - removed foreach patch
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _hashset_h
#define _hashset_h

#include <initializer_list>
#include <iostream>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "hashmap.h"
#undef INTERNAL_INCLUDE

/* Traits type for the HashSet, which wraps an underlying HashMap. */
namespace stanfordcpplib {
    namespace collections {
        template <typename T> struct HashSetTraits {
            using ValueType = T;
            using MapType   = HashMap<T, bool>;
            static std::string name() {
                return "HashSet";
            }
        };
    }
}

/*
 * A set of elements stored in no particular order. Elements can only be stored here
 * if they support a function
 *
 *     int hashCode(ValueType);
 *
 * that returns a nonnegative integer, along with equality comparison using ==.
 */
template <typename ValueType>
    using HashSet = stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::HashSetTraits<ValueType>>;

#endif // _hashset_h
