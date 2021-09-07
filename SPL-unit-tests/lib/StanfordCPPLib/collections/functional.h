/*
 * File: functional.h
 * ------------------
 * Contains a few functions for functional programming.
 *
 * The map, filter, and reduce operations are quite common in functional
 * programming systems.  They allow for elegant functional solutions to many
 * common programming tasks involving collections of data.
 *
 * There are similar functions in the C++ STL algorithm package.
 * But those functions often operate on STL iterators.
 * By contrast, these functions accept collections as parameters instead.
 * This is preferable for introductory students who are just learning.
 *
 * Implementation detail: Each function has several overloads.
 * Some of these are to allow for functions and predicates that accept their
 * arguments either by value or by const reference.
 * Other overloads allow either returning the result collection or passing it in
 * by reference as an output parameter to be filled in.
 *
 * We do not support "in-place" versions of these functions, partly because
 * there is not a perfect standard "remove" member across all collections that
 * accepts a value to remove as its parameter, and partly because the more common
 * functional style is to expect a new collection result to be returned.
 *
 * @author Marty Stepp
 * @version 2018/03/10
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _functional_h
#define _functional_h

#include <functional> // The standard header one. :-)

namespace functional {

/*
 * Performs a filter operation on the given collection,
 * returning a new collection that retains only the elements
 * for which the given predicate function returns true.
 */
template <typename CollectionType, typename ElementType>
CollectionType filter(CollectionType collection,
                      bool (*predicate)(ElementType)) {
    CollectionType result;
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/*
 * Performs a filter operation on the given collection,
 * returning a new collection that retains only the elements
 * for which the given predicate function returns true.
 */
template <typename CollectionType, typename ElementType>
CollectionType filter(CollectionType collection,
                      bool (*predicate)(const ElementType&)) {
    CollectionType result;
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/*
 * Performs a filter operation on the given collection,
 * building a new collection that retains only the elements
 * for which the given predicate function returns true.
 * The new collection is stored in the given 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType>
CollectionType& filter(CollectionType collection,
                       bool (*predicate)(ElementType),
                       CollectionType& result) {
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/*
 * Performs a filter operation on the given collection,
 * building a new collection that retains only the elements
 * for which the given predicate function returns true.
 * The new collection is stored in the given 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType>
CollectionType& filter(CollectionType collection,
                       bool (*predicate)(const ElementType&),
                       CollectionType& result) {
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/**
 * Performs a filter operation on the given collection,
 * returning a new collection that retains only the elements
 * for which the given predicate function returns true.
 */
template <typename CollectionType, typename ElementType>
CollectionType filter(CollectionType collection,
                      std::function<bool (const ElementType &)> predicate) {
    CollectionType result;
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/**
 * Performs a filter operation on the given collection,
 * building a new collection that retains only the elements
 * for which the given predicate function returns true.
 * The new collection is stored in the given 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType>
CollectionType& filter(CollectionType collection,
                       std::function<bool (const ElementType &)> predicate,
                       CollectionType& result) {
    for (const ElementType& element : collection) {
        if (predicate(element)) {
            result.add(element);
        }
    }
    return result;
}

/*
 * Applies the given function to each element of the given collection,
 * producing and returning a new collection containing the results.
 */
template <typename CollectionType, typename ElementType>
CollectionType map(CollectionType collection,
                   ElementType (*fn)(ElementType)) {
    CollectionType result;
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/*
 * Applies the given function to each element of the given collection,
 * producing and returning a new collection containing the results.
 */
template <typename CollectionType, typename ElementType>
CollectionType map(CollectionType collection,
                   ElementType (*fn)(const ElementType&)) {
    CollectionType result;
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/*
 * Applies the given function to each element of the given collection,
 * producing a new collection containing the results.
 * The new collection is stored in the 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType,
          typename CollectionType2, typename ElementType2>
CollectionType2& map(CollectionType collection,
                     ElementType2 (*fn)(ElementType),
                     CollectionType2& result) {
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/*
 * Applies the given function to each element of the given collection,
 * producing a new collection containing the results.
 * The new collection is stored in the 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType,
          typename CollectionType2, typename ElementType2>
CollectionType2& map(CollectionType collection,
                     ElementType2 (*fn)(const ElementType&),
                     CollectionType2& result) {
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/**
 * Applies the given function to each element of the given collection,
 * producing and returning a new collection containing the results.
 */
template <typename CollectionType, typename ElementType>
CollectionType map(CollectionType collection,
                   std::function<ElementType (const ElementType &)> fn) {
    CollectionType result;
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/**
 * Applies the given function to each element of the given collection,
 * producing a new collection containing the results.
 * The new collection is stored in the 'result' variable.
 * A reference to that same result is returned for convenience.
 */
template <typename CollectionType, typename ElementType,
          typename CollectionType2, typename ElementType2>
CollectionType2& map(CollectionType collection,
                     std::function<ElementType (const ElementType &)> fn,
                     CollectionType2& result) {
    for (const ElementType& element : collection) {
        result.add(fn(element));
    }
    return result;
}

/*
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   ElementType (*fn)(ElementType e1, ElementType e2),
                   ElementType startValue) {
    ElementType prev = startValue;
    for (const ElementType& element : collection) {
        prev = fn(prev, element);
    }
    return prev;
}

/*
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   ElementType (*fn)(ElementType e1, ElementType e2)) {
    bool first = true;
    ElementType prev;
    for (const ElementType& element : collection) {
        if (first) {
            prev = element;
            first = false;
        } else {
            prev = fn(prev, element);
        }
    }
    return prev;
}

/*
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 * Begins the reduction with the given starting value, which is then merged
 * with each value from the collection one at a time.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   ElementType (*fn)(const ElementType& e1, const ElementType& e2),
                   ElementType startValue) {
    ElementType prev = startValue;
    for (const ElementType& element : collection) {
        prev = fn(prev, element);
    }
    return prev;
}

/*
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 * Begins the reduction with the given starting value, which is then merged
 * with each value from the collection one at a time.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   ElementType (*fn)(const ElementType& e1, const ElementType& e2)) {
    bool first = true;
    ElementType prev;
    for (const ElementType& element : collection) {
        if (first) {
            prev = element;
            first = false;
        } else {
            prev = fn(prev, element);
        }
    }
    return prev;
}

/**
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 * Begins with a "default" value of the starting type.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   std::function<ElementType (const ElementType&, const ElementType &)> fn) {
    ElementType prev;
    for (const ElementType& element : collection) {
        prev = fn(prev, element);
    }
    return prev;
}

/**
 * Performs a reduction operation, applying a function to each neighboring pair
 * of elements of the collection until they are all combined (reduced) into a
 * single value, which is then returned.
 * Begins the reduction with the given starting value, which is then merged
 * with each value from the collection one at a time.
 */
template <typename CollectionType, typename ElementType>
ElementType reduce(CollectionType collection,
                   std::function<ElementType (const ElementType&, const ElementType &)> fn,
                   ElementType startValue) {
    ElementType prev = startValue;
    for (const ElementType& element : collection) {
        prev = fn(prev, element);
    }
    return prev;
}

} // namespace functional

#endif // _functional_h
