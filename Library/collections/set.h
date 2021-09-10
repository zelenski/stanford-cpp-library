/*
 * File: set.h
 * -----------
 * This file exports the <code>Set</code> class, which implements a
 * collection for storing a set of distinct elements.
 */

#ifndef _set_h
#define _set_h

#include <initializer_list>
#include <iostream>
#include <set>
#include <functional>
#include <type_traits>

#include "collections.h"
#include "map.h"

/* Traits type for the Set, which wraps an underlying Map. */
namespace stanfordcpplib {
    namespace collections {
        template <typename T> struct SetTraits {
            using ValueType = T;
            using MapType   = Map<T, bool>;
            static std::string name() {
                return "Set";
            }

            /* The Set type does allow you to construct the underlying Map by handing
             * along a std::function.
             */
            template <typename Function>
            static MapType construct(Function comparator) {
                static_assert(std::is_assignable<std::function<bool(const ValueType&, const ValueType&)>, Function>::value,
                              "Oops! Seems like you tried to initialize a Set incorrectly. Click here for details.");
                /*
                 * Hello student! If you are reading this message, it means that you tried to
                 * initialize a Set improperly. For example, you might have tried to write
                 * something like this:
                 *
                 *     Set<int> mySet = 137; // Oops!
                 *
                 * Here, for example, you're trying to assign an int to a Set<int>.
                 *
                 * or perhaps you had a function like this one:
                 *
                 *     void myFunction(Set<int>& mySet);
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
                return MapType(comparator);
            }

            /* You can also default-construct it. */
            static MapType construct() {
                return {};
            }

            /* However, you can't pass in any other arguments. */
            template <typename... Args>
            static void construct(MapType &, Args&&...) {
                static_assert(Fail<Args...>::value, "Oops! Seems like you tried to initialize a Set incorrectly. Click here for details.");

                /*
                 * Hello student! If you are reading this message, it means that you tried to
                 * initialize a Set improperly. For example, you might have tried to write
                 * something like this:
                 *
                 *     Set<int> mySet = 137; // Oops!
                 *
                 * Here, for example, you're trying to assign an int to a Set<int>.
                 *
                 * or perhaps you had a function like this one:
                 *
                 *     void myFunction(Set<int>& mySet);
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
                error("static_assert failed?");
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
