/*
 * File: typeproperties.h
 * -------------------
 * Internal helper functions used to check whether various types
 * have various properties
 */
#pragma once

#include "hashcode.h"
#include <type_traits>
#include <iostream>

/*
 * Checks that various operations are defined, giving nice error messages if
 * they aren't.
 */
#define ASSERT_IS_COMPARABLE(...) \
    static_assert(::stanfordcpplib::collections::IsLessThanComparable<__VA_ARGS__>::value,\
                  "Oops! You tried comparing two objects that aren't comparable. Click this error for more details.")
    /*
     * Hello students! If you got directed to this line of code in a compiler error,
     * it probably means that you wrote code that requires comparing two objects
     * with the < operator that aren't comparable. This could be from trying to use
     * the < operator directory, from trying to sort a list of objects, from using
     * your type as the key in a Map, or from storing your object in a Set. By
     * default, objects in C++ aren't comparable by <, hence the error.
     *
     * There are two ways to fix this. The first option would simply be to not use
     * the object in a way that requires the comparison.
     *
     * The second way to fix this is to explicitly define an operator< function for your custom
     * type. Here's the syntax for doing that:
     *
     *     bool operator< (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         using namespace stanfordcpplib::collections;
     *         return compareTo(lhs.data1, rhs.data1,
     *                          lhs.data2, rhs.data2,
     *                          ...
     *                          lhs.dataN, rhs.dataN) == -1; // -1 signals less than
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
     *         using namespace stanfordcpplib::collections;
     *         return compareTo(lhs.myInt,    rhs.myInt,
     *                          lhs.myString, rhs.myString) == -1;
     *     }
     *
     * Hope this helps!
     */

#define ASSERT_HAS_EQUALITY(...) \
    static_assert(::stanfordcpplib::collections::IsEqualityComparable<__VA_ARGS__>::value, \
                  "Oops! You tried comparing two objects that aren't comparable. Click this error for more details.")

    /*
     * Hello students! If you got directed to this line of code in a compiler error,
     * it probably means that you wrote code that requires comparing two objects
     * with the == operator that aren't comparable. This could be from trying to use
     * the == operator directory, from using EXPECT_EQUAL or EXPECT_NOT_EQUAL on
     * custom types, etc.
     *
     * There are two ways to fix this. The first option would simply be to not use
     * the object in a way that requires the comparison.
     *
     * The second way to fix this is to explicitly define an operator< function for your custom
     * type. Here's the syntax for doing that:
     *
     *     bool operator== (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         using namespace stanfordcpplib::collections;
     *         return equalTo(lhs.data1, rhs.data1,
     *                        lhs.data2, rhs.data2,
     *                        ...
     *                        lhs.dataN, rhs.dataN);
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
     *     bool operator== (const MyType& lhs, const MyType& rhs) {
     *         using namespace stanfordcpplib::collections;
     *         return equalTo(lhs.myInt,    rhs.myInt,
     *                        lhs.myString, rhs.myString);
     *     }
     *
     * Hope this helps!
     */

#define ASSERT_IS_HASHABLE(...) \
    static_assert(::stanfordcpplib::collections::IsHashable<__VA_ARGS__>::value, \
                  "Oops! You tried hashing an object that isn't hashable. Click this error for more details.")
    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it means your code needs to hash something that isn't hashable.
     *
     * In order to compute hash codes - either because you're storing a custom type in
     * a HashMap or HashSet, or because you explicitly needed to compute a hash code -
     * you need to define a hashCode() and operator== function for your custom type.
     *
     * There are two ways to fix this. The first option would simply be to not to do whatever
     * operation requires hashing. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define a hashCode() and operator== function
     * for your type. To do so, first define hashCode as follows:
     *
     *     int hashCode(const YourCustomType& obj) {
     *         return hashCode(obj.data1, obj.data2, ..., obj.dataN);
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
     *     int hashCode(const MyType& obj) {
     *         return hashCode(obj.myInt, obj.myString);
     *     }
     *
     * Second, define operator== as follows:
     *
     *     bool operator== (const YourCustomType& lhs, const YourCustomType& rhs) {
     *         using namespace stanfordcpplib::collections;
     *         return equalTo(lhs.data1, rhs.data1,
     *                        lhs.data2, rhs.data2,
     *                        ...
     *                        lhs.dataN, rhs.dataN);
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
     *     bool operator== (const MyType& lhs, const MyType& rhs) {
     *         using namespace stanfordcpplib::collections;
     *         return equalTo(lhs.myInt,    rhs.myInt,
     *                        lhs.myString, rhs.myString);
     *     }
     *
     * Hope this helps!
     */

#define ASSERT_STREAM_INSERTABLE(...) \
    static_assert(::stanfordcpplib::collections::IsStreamInsertable<__VA_ARGS__>::value,\
                  "Oops! You tried printing an object that isn't printable. Click this error for more details.")

    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it means your code needs to print something that isn't printable.
     *
     * In order to print a value to cout, or to use SimpleTest on a custom value, that
     * value needs to have a function called operator<< defined for it. If this doesn't
     * exist, then C++ will cause an error.
     *
     * There are two ways to fix this. The first option would simply be to not to do whatever
     * operation requires printing the value. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define the operator<< function for your
     * type. To do this, write a function that looks like this:
     *
     *     std::ostream& operator<< (std::ostream& out, const YourCustomType& obj) {
     *         // ... write code here to print out the contents of your custom
     *         // ... type. Instead of printing to cout, though, print to out.
     *
     *         return out;
     *     }
     *
     * For example, if you had a custom type
     *
     *     struct MyType {
     *         int myInt;
     *         string myString;
     *     };
     *
     * you might define the function
     *
     *     std::ostream& operator<< (std::ostream& out, const MyType& obj) {
     *         out << "{" << obj.myInt << ", " << obj.myString << "}";
     *         return out;
     *     }
     *
     * Hope this helps!
     */

#define ASSERT_STREAM_EXTRACTABLE(...) \
    static_assert(::stanfordcpplib::collections::IsStreamExtractable<__VA_ARGS__>::value,\
                  "Oops! You tried reading an object that isn't readable. Click this error for more details.")
    /*
     * Hello CS106 students! If you got directed to this line of code in a compiler error,
     * it means your code needs to read something that isn't readable.
     *
     * In order to print a value to cout, or to use SimpleTest on a custom value, that
     * value needs to have a function called operator>> defined for it. If this doesn't
     * exist, then C++ will cause an error.
     *
     * There are two ways to fix this. The first option would simply be to not to do whatever
     * operation requires printing the value. This is probably the easiest option.
     *
     * The second way to fix this is to explicitly define the operator>> function for your
     * type. To do this, write a function that looks like this:
     *
     *     std::istream& operator>> (std::istream& in, YourCustomType& obj) {
     *         // ... write code here to read a YourCustomType. Instead of using
     *         // ... cin here, though, use in.
     *
     *         return in;
     *     }
     *
     * Hope this helps!
     */

namespace stanfordcpplib {
    namespace collections {
        /*
         * The types below are used to check whether a type has various properties
         * (comparable via <, hashable, comparable via ==, capable of being output
         * to a stream, capable of being read from a stream, etc.
         *
         * This is used to provide better compiler diagnostics to students when
         * they try to instantiate our types incorrectly.
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
                             decltype(std::declval<U>() < std::declval<U>()) = false);
            template <typename U> static No  check(...);

        public:
            static constexpr bool value =
                    std::conditional<std::is_same<decltype(check<T>(0)), Yes>::value,
            std::true_type,
            std::false_type>::type::value;
        };

        template <typename T>
        struct IsEqualityComparable {
        private:
            /* Use SFNIAE overloading to detect which of these two options to pick. */
            struct Yes{};
            struct No {};

            template <typename U>
            static Yes check(int,
                             decltype(std::declval<U>() == std::declval<U>()) = false);
            template <typename U> static No  check(...);

        public:
            static constexpr bool value =
                    std::conditional<std::is_same<decltype(check<T>(0)), Yes>::value,
            std::true_type,
            std::false_type>::type::value;
        };

        template <typename T>
        struct IsStreamInsertable {
        private:
            /* Use SFNIAE overloading to detect which of these two options to pick. */
            struct Yes{};
            struct No {};

            template <typename U>
            static Yes check(typename std::remove_reference<decltype(std::cout << std::declval<U>())>*);
            template <typename U> static No  check(...);

        public:
            static constexpr bool value =
                    std::conditional<std::is_same<decltype(check<T>(nullptr)), Yes>::value,
            std::true_type,
            std::false_type>::type::value;
        };

        template <typename T>
        struct IsStreamExtractable {
        private:
            /* Use SFNIAE overloading to detect which of these two options to pick. */
            struct Yes{};
            struct No {};

            template <typename U>
            static Yes check(typename std::remove_reference<decltype(std::cin >> std::declval<U&>())>*);
            template <typename U> static No  check(...);

        public:
            static constexpr bool value =
                    std::conditional<std::is_same<decltype(check<T>(nullptr)), Yes>::value,
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
                             decltype(std::declval<U>() == std::declval<U>()) = false);
            template <typename U> static No  check(...);

        public:
            static constexpr bool value =
                    std::conditional<std::is_same<decltype(check<T>(0)), Yes>::value,
            std::true_type,
            std::false_type>::type::value;
        };
    }
}
