/*
 * File: static.h
 * --------------
 * This file provides several macros for declaring static variables and functions
 * in ways that are safe for usage during the C++ static initiialization phase.
 * These macros should be used to declare all not-inside-function static data
 * used by the library, since a lot of library code runs during the
 * static initialization phase.
 */

#ifndef _static_h
#define _static_h

// declare static var/func and assign it the given value
#define STATIC_VARIABLE_DECLARE(type, name, value) \
    static type & s_##name() { \
        static type __##name = (value); \
        return __##name; \
    }

// declare but don't assign a value (use type's default value)
#define STATIC_VARIABLE_DECLARE_BLANK(type, name) \
    static type & s_##name() { \
        static type __##name; \
        return __##name; \
    }

// declare static const var/func and assign it the given value
#define STATIC_CONST_VARIABLE_DECLARE(type, name, value) \
    static const type & s_##name() { \
        static const type __##name = (value); \
        return __##name; \
    }

// declare static var/func collection and assign it the given elements
// (these seem to not work for multi-template collections like Map<K, V>  :-/
//  but they work for single-template collections like Vector<E>)
#define STATIC_VARIABLE_DECLARE_COLLECTION(type, name, ...) \
    static type & s_##name() { \
        static type __##name = { __VA_ARGS__ }; \
        return __##name; \
    }

// declare static var/func collection and leave it empty
#define STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(type, name) \
    static type & s_##name() { \
        static type __##name; \
        return __##name; \
    }

#define STATIC_VARIABLE_DECLARE_MAP_EMPTY(maptype, keytype, valuetype, name) \
    static maptype < keytype , valuetype > & s_##name() { \
        static maptype < keytype , valuetype > __##name; \
        return __##name; \
    }

// declare static const var/func collection and assign it the given elements
#define STATIC_CONST_VARIABLE_DECLARE_COLLECTION(type, name, ...) \
    static const type & s_##name() { \
        static const type __##name { __VA_ARGS__ }; \
        return __##name; \
    }

// look up the value of the given static variable (by calling its static s_ function)
#define STATIC_VARIABLE(name) \
    (s_##name())

#endif // _static_h
