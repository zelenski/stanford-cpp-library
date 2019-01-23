/*
 * File: set.h
 * -----------
 * This file exports the <code>Set</code> class, which implements a
 * collection for storing a set of distinct elements.
 * 
 * @version 2018/03/19
 * - added constructors that accept a comparison function
 * @version 2018/03/10
 * - added methods front, back
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via Map)
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
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added template hashCode function
 * @version 2014/10/10
 * - removed use of __foreach macro
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _set_h
#define _set_h

#include <initializer_list>
#include <iostream>
#include <set>
#include <functional>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#undef INTERNAL_INCLUDE

/* Traits type for the Set, which wraps an underlying Map. */
namespace stanfordcpplib {
    namespace collections {
        template <typename T> struct SetTraits {
            using ValueType = T;
            using MapType   = Map<T, bool>;
            static std::string name() {
                return "Set";
            }
        };
    }
}

/*
 * A set of elements stored in sorted order. The elements must be comparable
 * using the < operator in order to be stored here.
 */
template <typename ValueType>
    using Set = stanfordcpplib::collections::GenericSet<stanfordcpplib::collections::SetTraits<ValueType>>;

#endif // _set_h
