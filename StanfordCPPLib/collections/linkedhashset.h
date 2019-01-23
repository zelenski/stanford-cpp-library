/*
 * File: linkedhashset.h
 * ---------------------
 * This file exports the <code>LinkedHashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 * 
 * @author Marty Stepp
 * @version 2018/03/10
 * - added methods front, back
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - added containsAll, isSupersetOf methods
 * - added support for std initializer_list usage, such as {1, 2, 3}
 *   in constructor, addAll, containsAll, isSubsetOf, isSupersetOf, removeAll,
 *   retainAll, and operators +, +=, -, -=, *, *=
 * - bug fix in hashCode function
 * - added operators <, <=, >, >=
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/10/26
 * - initial version
 * @since 2015/10/26
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _linkedhashset_h
#define _linkedhashset_h

#include <initializer_list>
#include <iostream>
#include <functional>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "linkedhashmap.h"
#undef INTERNAL_INCLUDE

/* Traits type for the LinkedHashSet, which wraps an underlying LinkedHashMap. */
namespace stanfordcpplib {
    namespace collections {
        template <typename T> struct LinkedHashSetTraits {
            using ValueType = T;
            using MapType   = LinkedHashMap<T, bool>;
            static std::string name() {
                return "LinkedHashSet";
            }
        };
    }
}

/*
 * A set of elements that remembers their insertion order during iteration. To use
 * this type, the underlying elements must support a function
 *
 *     int hashCode(ValueType);
 *
 * that returns a nonnegative integer, along with equality comparison using ==.
 */
template <typename ValueType>
    using LinkedHashSet = stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::LinkedHashSetTraits<ValueType>>;

#endif // _linkedhashset_h
