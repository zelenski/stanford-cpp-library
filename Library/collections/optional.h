#pragma once

#include <type_traits>
#include <ostream>
#include <algorithm>
#include <memory>
#include "collections.h"

/* Allows for overloading on Nothing. */
struct nothing_t { };
extern nothing_t Nothing;

/* These template shenanigans are designed to detect whether a series of template args
 * accidentally would shadow the move or copy constructor, or would select an initializer
 * list constructor when it shouldn't.
 */
namespace OptionalInternals {
    template <typename T> struct IsInitializerList {
        static const bool result = false;
    };

    template <typename T> struct IsInitializerList<std::initializer_list<T>> {
        static const bool result = true;
    };

    template <typename Target, typename... Args> struct AvoidOverride {
        static const bool result = true;
    };
    template <typename Target, typename Singleton> struct AvoidOverride<Target, Singleton> {
        static const bool result =
                !std::is_same<typename std::remove_cv<typename std::remove_reference<Singleton>::type>::type, Target>::value &&
                !IsInitializerList<typename std::remove_reference<Singleton>::type>::result;

    };

    template <typename T> struct IsString {
        static const bool result = std::is_convertible<T, std::string>::value;
    };
}

template <typename T> class Optional {
public:
    /* Construct from a list of parameters - as long as you aren't trying to
     * override the copy/move constructor.
     */
    template <typename... Args, typename = typename std::enable_if<OptionalInternals::AvoidOverride<Optional, Args...>::result, int>::type>
    Optional(Args&&... args) : data_(new T(std::forward<Args&&>(args)...)) {
        // Handled in initializer
    }

    /* BUG: The Set type will incorrectly try ingesting an initializer_list<const char*> through the
     * wrong constructor. Allow initializer lists only if they don't contain strings.
     */
    template <typename Arg, typename = typename std::enable_if<!OptionalInternals::IsString<Arg>::result, int>::type>
    Optional(std::initializer_list<Arg> args) : data_(new T(args)) {
        // Handled in initializer
    }

    /* And handle strings separately. */
    Optional(std::initializer_list<std::string> args) : data_(new T(args)) {
        // Handled in initializer
    }

    /* Build empty. */
    Optional(nothing_t) {
        // Default ctor for unique_ptr does what we need it to do.
    }

    /* Copy/move constructor. */
    Optional(const Optional& rhs) {
        /* Something there? Then copy it. */
        if (rhs.data_) {
            data_.reset(new T(*rhs.data_));
        }
    }
    Optional(Optional&& rhs) : data_(std::move(rhs.data_)) {
        // Use initialization list.
    }

    /* Assignment operator. */
    Optional& operator= (Optional rhs) {
        std::swap(data_, rhs.data_);
        return *this;
    }

    /* Has a value? */
    bool hasValue() const {
        return (bool) data_;
    }

    /* Get value. */
    T& value() {
        return const_cast<T&>(static_cast<const Optional*>(this)->value());
    }
    const T& value() const {
        if (!hasValue()) {
            error("Optional: Tried getting the value of Nothing.");
        }
        return *data_;
    }

private:
    std::unique_ptr<T> data_;
};

/* == nothing is equivalent to asking whether we have a value. */
template <typename T> bool operator== (const Optional<T>& lhs, nothing_t) {
    return !lhs.hasValue();
}

template <typename T> bool operator== (nothing_t, const Optional<T>& rhs) {
    return rhs == Nothing;
}

template <typename T> bool operator!= (const Optional<T>& lhs, nothing_t) {
    return !(lhs == Nothing);
}

template <typename T> bool operator!= (nothing_t, const Optional<T>& rhs) {
    return !(rhs == Nothing);
}

/* Relational operators */
template <typename T> bool operator== (const Optional<T>& lhs, const Optional<T>& rhs) {
    /* If either of these has no value, see if they both do. */
    if (!lhs.hasValue() || !rhs.hasValue()) {
        return lhs.hasValue() == rhs.hasValue();
    }

    /* Otherwise compare the values. */
    return lhs.value() == rhs.value();
}

template <typename T> bool operator!= (const Optional<T>& lhs, const Optional<T>& rhs) {
    return !(lhs == rhs);
}

/* Relational operators put Nothing first. */
template <typename T> bool operator< (const Optional<T>& lhs, const Optional<T>& rhs) {
    static_assert(stanfordcpplib::collections::IsLessThanComparable<T>::value, "Oops! You can't compare Optional<T>s if the underlying type T is not comparable.");

    /* Nothing comes first. */
    if (!lhs.hasValue() || !rhs.hasValue()) {
        return !!lhs.hasValue() < !!rhs.hasValue();
    }

    /* Otherwise compare the rest. */
    return lhs.value() < rhs.value();
}

template <typename T> bool operator>= (const Optional<T>& lhs, const Optional<T>& rhs) {
    return !(lhs < rhs);
}

template <typename T> bool operator> (const Optional<T>& lhs, const Optional<T>& rhs) {
    return rhs < lhs;
}

template <typename T> bool operator<= (const Optional<T>& lhs, const Optional<T>& rhs) {
    return !(rhs > lhs);
}

inline std::ostream& operator<< (std::ostream& out, nothing_t) {
    return out << "Nothing";
}

template <typename T> std::ostream& operator<< (std::ostream& out, const Optional<T>& rhs) {
    if (!rhs.hasValue()) return out << Nothing;
    else return out << rhs.value();
}
