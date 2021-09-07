/*
 * File: complex.h
 * ---------------
 * This file exports a type representing complex numbers.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/10/18
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _complex_h
#define _complex_h

#include <iostream>
#include <string>

/**
 * A Complex object represents a complex number of the form a + bi.
 */
class Complex {
public:
    /**
     * Constructs a new imaginary number: (a + b * i).
     */
    Complex(double a = 0.0, double b = 0.0);

    /**
     * Returns the absolute value of the complex number.
     */
    double abs() const;

    /**
     * Returns the imaginary value of the complex number.
     */
    double imag() const;

    /**
     * Returns the real value of the complex number.
     */
    double real() const;

    /**
     * Returns a string representation of this complex number,
     * such as "4+3i" or "-2-4i" or "0".
     */
    std::string toString() const;

    /**
     * Adds two complex numbers and returns
     * the result as a new complex number.
     */
    friend Complex operator +(const Complex& m, const Complex& n);

    /**
     * Subtracts two complex numbers and returns
     * the result as a new complex number.
     */
    friend Complex operator -(const Complex& m, const Complex& n);

    /**
     * Multiplies two complex numbers and returns
     * the result as a new complex number.
     */
    friend Complex operator *(const Complex& m, const Complex& n);

    friend std::istream& operator >>(std::istream& out, Complex& c);

private:
    double a;   // real value
    double b;   // imag value
};

/**
 * Returns an integer hash code for complex numbers so that they
 * can be stored in HashSet and HashMap collections.
 */
int hashCode(const Complex& c);

/**
 * These relational operators define a total ordering for complex numbers.
 * Mathematically this is not well-defined, but we provide this simply so
 * that Complex objects can be stored in a Set or Map collection.
 */
bool operator <(const Complex& c1, const Complex& c2);
bool operator <=(const Complex& c1, const Complex& c2);
bool operator >(const Complex& c1, const Complex& c2);
bool operator >=(const Complex& c1, const Complex& c2);
bool operator ==(const Complex& c1, const Complex& c2);
bool operator !=(const Complex& c1, const Complex& c2);

/**
 * Writes the complex number to an output stream.
 */
std::ostream& operator <<(std::ostream& out, const Complex& c);

/**
 * Reads a complex number from an output stream.
 */
std::istream& operator >>(std::istream& input, Complex& c);

#endif // _complex_h
