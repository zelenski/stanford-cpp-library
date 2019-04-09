/*
 * File: linkedhashset.h
 * ---------------------
 * This file exports the <code>LinkedHashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 * 
 * @author Marty Stepp
 * @version 2019/02/04
 * - changed internal implementation to wrap std collections
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

            /* You can default-construct a LinkedHashSet. */
            static MapType construct() {
                return {};
            }

            /* However, you can't pass in any other arguments. */
            template <typename... Args>
            static void construct(Args&&...) {
                static_assert(Fail<Args...>::value, "Oops! Seems like you tried to initialize a LinkedHashSet incorrectly. Click here for details.");

                /*
                 * Hello student! If you are reading this message, it means that you tried to
                 * initialize a LinkedHashSet improperly. For example, you might have tried to
                 * write something like this:
                 *
                 *     LinkedHashSet<int> mySet = 137; // Oops!
                 *
                 * Here, for example, you're trying to assign an int to a LinkedHashSet<int>.
                 *
                 * or perhaps you had a function like this one:
                 *
                 *     void myFunction(LinkedHashSet<int>& mySet);
                 *
                 * and you called it by writing
                 *
                 *     myFunction(someSet + someOtherSet); // Oops!
                 *     myFunction({ });                    // Oops!
                 *
                 * In these cases, you're trying to pass a value into a function that takes
                 * its argument by (non-const) reference. C++ doesn't allow you to do this.
                 *
                 * To see where the actual error comes from, look in the list of error messages
                 * in Qt Creator. You should see a line that says "required from here" that
                 * points somewhere in your code. That's the actual line you wrote that caused
                 * the problem, so double-click on that error message and see where it takes
                 * you. Now you know where to look!
                 *
                 * Hope this helps!
                 */
                error("static_assert succeeded?");
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
