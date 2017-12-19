/*
 * File: complex.cpp
 * -----------------
 * This file defines a type representing complex numbers.
 * See complex.h for declarations and documentation of class and members.
 *
 * @version 2017/10/18
 * - initial version
 */

#include "complex.h"
#include <cctype>
#include <cmath>
#include <sstream>
#include "gmath.h"
#include "hashcode.h"

Complex::Complex(double a, double b) {
    this->a = a;
    this->b = b;
}

double Complex::abs() const {
   return sqrt(a*a + b*b);
}

double Complex::imag() const {
    return b;
}

double Complex::real() const {
    return a;
}

std::string Complex::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const Complex& c) {
    return hashCode2(c.real(), c.imag());
}

Complex operator *(const Complex& m, const Complex& n) {
   double realP = m.a * n.a - m.b * n.b;
   double imagP = m.b * n.a + m.a * n.b;
   return Complex(realP, imagP);
}

Complex operator +(const Complex& m, const Complex& n) {
   double realP = m.a + n.a;
   double imagP = m.b + n.b;
   return Complex(realP, imagP);
}

Complex operator -(const Complex& m, const Complex& n) {
   double realP = m.a - n.a;
   double imagP = m.b - n.b;
   return Complex(realP, imagP);
}

bool operator <(const Complex& c1, const Complex& c2) {
    return c1.real() < c2.real() ||
            (floatingPointEqual(c1.real(), c2.real()) && c1.imag() < c2.imag());
}

bool operator <=(const Complex& c1, const Complex& c2) {
    return c1 < c2 || c1 == c2;
}

bool operator >(const Complex& c1, const Complex& c2) {
    return c2 < c1;
}

bool operator >=(const Complex& c1, const Complex& c2) {
    return c2 < c1 || c2 == c1;
}

bool operator ==(const Complex& c1, const Complex& c2) {
    return floatingPointEqual(c1.real(), c2.real())
            && floatingPointEqual(c1.imag(), c2.imag());
}

bool operator !=(const Complex& c1, const Complex& c2) {
    return !(c1 == c2);
}

std::ostream& operator <<(std::ostream& out, const Complex& c) {
    out << c.real();
    if (!floatingPointEqual(c.imag(), 0.0)) {
        out << (c.imag() > 0 ? "+" : "") << c.imag() << "i";
    }
    return out;
}

// This code is pretty ugly.
// I don't remember some of the ins and outs of the istream API.
// Oh, well.
std::istream& operator >>(std::istream& input, Complex& c) {
    input >> std::ws;   // skip whitespace

    // optional '('
    bool needClosingParen = false;
    if (input.peek() == '(') {
        input.get();
        needClosingParen = true;
    }

    // read real part
    input >> std::ws;
    double real = 0.0;
    bool readReal = false;

    std::streampos pos = input.tellg();
    if (input >> real) {
        readReal = true;
    } else {
        input.clear();
        input.seekg(pos);
    }

    // read imag part
    input >> std::ws;
    double imag = 0.0;
    bool readImag = false;

    pos = input.tellg();
    if (input >> imag) {
        readImag = true;
    } else {
        input.clear();
        input.seekg(pos);
        if (input.peek() == 'i') {
            imag = 1.0;
            readImag = true;
        } else if (input.peek() == '-' || input.peek() == '+') {
            bool isNegative = input.peek() == '-';
            input.get();
            input >> std::ws;
            pos = input.tellg();

            if (input >> imag) {
                if (isNegative) {
                    imag = -imag;
                }
                readImag = true;
            } else {
                input.clear();
                input.seekg(pos);
                if (input.peek() == 'i') {
                    imag = isNegative ? -1.0 : 1.0;
                    readImag = true;
                } else {
                    input.clear();
                    input.unget();
                }
            }
        }
    }

    // read 'i'
    input >> std::ws;
    if (input.peek() == 'i') {
        input.get();
    }

    // optional ')'
    input >> std::ws;
    if (input.peek() == ')') {
        input.get();
    } else if (needClosingParen) {
        // invalid input; saw opening ( but no closing )
        input.setstate(std::ios_base::failbit);
        return input;
    }

    if (readReal || readImag) {
        input.clear();   // unset fail state
        c.a = real;
        c.b = imag;
    } else {
        input.setstate(std::ios_base::failbit);
    }

    return input;
}
