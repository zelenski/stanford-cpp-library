/*
 * File: biginteger.h
 * ------------------
 * This file exports a class for arbitrary-size integer arithmetic.
 * It is meant to help get around the max/min value limit for types
 * such as int and long.
 *
 * In general, a BigInteger supports the standard operators and operations
 * that you would expect to be able to use on an int or long value.
 *
 * Example usage:
 *
 * BigInteger bi("1234567890123456789");
 * for (int i = 0; i < 10; i++) {
 *     bi *= 12345678;
 * }
 * cout << "really big number is: " << bi << endl;
 *
 * Implementation notes:
 * This code is heavily based on a BigInteger library taken from:
 * https://github.com/panks/BigInteger
 *
 * The implementation stores the big integer as a string of base-10 digits
 * along with a sign bit represented as a bool.
 * In general the operations are not heavily optimized; this class should not
 * be used in situations where performance is critical.
 * Note that better Big Integer libraries exist in other places for more
 * serious work and can be found using your favorite search engine.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/10/28
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _biginteger_h
#define _biginteger_h

#include <iostream>
#include <string>

#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

class BigInteger {
public:
    /**
     * Constants to represent very commonly used big integer values
     */
    static const BigInteger NEGATIVE_ONE;
    static const BigInteger ZERO;
    static const BigInteger ONE;
    static const BigInteger TWO;
    static const BigInteger TEN;
    static const BigInteger MAX_INT;
    static const BigInteger MIN_INT;
    static const BigInteger MAX_UINT;
    static const BigInteger MAX_LONG;
    static const BigInteger MIN_LONG;
    static const BigInteger MAX_ULONG;
    static const BigInteger MAX_SHORT;
    static const BigInteger MIN_SHORT;
    static const BigInteger MAX_USHORT;

    /**
     * Constructs a new big integer set to zero.
     *
     * @example BigInteger bi;
     */
    BigInteger();

    /**
     * Constructs a new big integer whose value is a copy of another big integer.
     *
     * @example BigInteger bi1(bi2);
     */
    BigInteger(const BigInteger& other);

    /**
     * Constructs a new big integer set to the given value.
     *
     * @example BigInteger bi("1234567890123456789");
     */
    BigInteger(const std::string& s, int radix = 10);

    /**
     * Constructs a new big integer set to the given value.
     *
     * @example BigInteger bi(12345);
     * @example BigInteger bi2(-42);
     */
    BigInteger(long n);

    /**
     * Returns a new BigInteger whose value is the absolute value of this one.
     */
    BigInteger abs() const;

    /**
     * Returns the greatest common divisor of this and the given other big integer.
     * For example, gcd(24, 16) is 8.
     */
    BigInteger gcd(const BigInteger& other) const;

    /**
     * Returns true if this BigInteger's value is within the range of values
     * that can be stored as an int.
     */
    bool isInt() const;

    /**
     * Returns true if this BigInteger's value is within the range of values
     * that can be stored as a long.
     */
    bool isLong() const;

    /**
     * Returns true if this BigInteger represents a negative number < 0.
     * You could just perform the check yourself by testing whether number > 0,
     * but this member is faster because it doesn't need to create a temporary
     * BigInteger instance for doing the comparison.
     */
    bool isNegative() const;

    /**
     * Returns true if this BigInteger represents a non-negative number >= 0.
     * You could just perform the check yourself by testing whether number > 0,
     * but this member is faster because it doesn't need to create a temporary
     * BigInteger instance for doing the comparison.
     */
    bool isNonNegative() const;

    /**
     * Returns true if this BigInteger represents a positive number > 0.
     * You could just perform the check yourself by testing whether number > 0,
     * but this member is faster because it doesn't need to create a temporary
     * BigInteger instance for doing the comparison.
     */
    bool isPositive() const;

    /**
     * Returns whichever is larger between this big integer and the given
     * other big integer.
     */
    const BigInteger& max(const BigInteger& other) const;

    /**
     * Returns whichever is smaller between this big integer and the given
     * other big integer.
     */
    const BigInteger& min(const BigInteger& other) const;

    /**
     * Returns a new BigInteger whose value is (this ^^ exp) % m.
     * @throw ErrorException if exp is negative or if m is 0.
     */
    BigInteger modPow(const BigInteger& exp, const BigInteger& m) const;

    /**
     * Returns a new BigInteger whose value is the value of this BigInteger
     * raised to the given exponent.
     * @throw ErrorException if the exponent is negative.
     */
    BigInteger pow(long exp) const;

    /**
     * Returns a new BigInteger whose value is the value of this BigInteger
     * raised to the given exponent.
     * @throw ErrorException if the exponent is negative.
     */
    BigInteger pow(const BigInteger& exp) const;

    /**
     * Returns an int representation of this BigInteger, such as
     * -12345678.
     * @throw ErrorException if this BigInteger is out of the range of int.
     */
    int toInt() const;

    /**
     * Returns a long representation of this BigInteger, such as
     * -123456789.
     * @throw ErrorException if this BigInteger is out of the range of long.
     */
    long toLong() const;

    /**
     * Returns a string representation of this BigInteger, such as
     * "-1234567890123456789".
     */
    std::string toString(int radix = 10) const;

    /**
     * Increases the value of this BigInteger by 1 (prefix).
     */
    BigInteger& operator ++(); // prefix

    /**
     * Increases the value of this BigInteger by 1 (posfix).
     */
    BigInteger  operator ++(int); // postfix

    /**
     * Decreases the value of this BigInteger by 1 (prefix).
     */
    BigInteger& operator --(); // prefix

    /**
     * Decreases the value of this BigInteger by 1 (postfix).
     */
    BigInteger  operator --(int); // postfix

    /**
     * Assigns this BigInteger to store the sum of itself
     * and the given other BigInteger.
     */
    BigInteger& operator +=(const BigInteger& b);

    /**
     * Assigns this BigInteger to store the result of subtracting
     * the given other BigInteger from this BigInteger.
     */
    BigInteger& operator -=(const BigInteger& b);

    /**
     * Assigns this BigInteger to store the product of itself
     * and the given other BigInteger.
     */
    BigInteger& operator *=(const BigInteger& b);

    /**
     * Assigns this BigInteger to store the quotient of dividing
     * itself by the given other BigInteger.
     * @throw ErrorException if denominator is 0.
     * @throw ErrorException if denominator is not within the range of type long.
     */
    BigInteger& operator /=(const BigInteger& b);

    /**
     * Assigns this BigInteger to store the remainder of dividing
     * itself by the given other BigInteger.
     * @throw ErrorException if denominator is 0.
     * @throw ErrorException if denominator is not within the range of type long.
     */
    BigInteger& operator %=(const BigInteger& b);

    /**
     * Sets this big integer to a bitwise AND between this integer and the given other integer,
     * retaining only bits that are set in both.
     */
    BigInteger& operator &=(const BigInteger& b);

    /**
     * Sets this big integer to a bitwise OR between this integer and the given other integer,
     * retaining bits that are set in this integer or the other integer or both.
     */
    BigInteger& operator |=(const BigInteger& b);

    /**
     * Sets this big integer to a bitwise XOR between this integer and the given other integer,
     * retaining bits that are set in this integer or the other integer but not both.
     */
    BigInteger& operator ^=(const BigInteger& b);

    /**
     * Performs a bitwise NOT on this integer,
     * inverting the values of all of its bits.
     */
    BigInteger operator ~() const;

    /**
     * Performs a logical NOT on this integer,
     * setting it to 0 if non-zero, or to 1 if zero.
     */
    BigInteger operator !() const;

    /**
     * Sets this BigInteger to store the same value as the given other big integer.
     */
    BigInteger& operator =(const BigInteger& other);

    /**
     * Unary negation; returns a new BigInteger that is
     * the negative of this BigInteger.
     */
    BigInteger operator -() const;

    /**
     * Returns a new big integer whose value is equal to the value of
     * this big integer bit-shifted left by the given number of bits.
     * Equivalent to multiplying by 2 ^ shift.
     */
    BigInteger operator <<(unsigned int shift) const;

    /**
     * Modifies this big integer to be bit-shifted left by the given number of bits.
     * Equivalent to multiplying by 2 ^ shift.
     */
    BigInteger& operator <<=(unsigned int shift);

    /**
     * Returns a new big integer whose value is equal to the value of
     * this big integer bit-shifted right by the given number of bits.
     * Equivalent to dividing by 2 ^ shift.
     */
    BigInteger operator >>(unsigned int shift) const;

    /**
     * Modifies this big integer to be bit-shifted right by the given number of bits.
     * Equivalent to dividing by 2 ^ shift.
     */
    BigInteger& operator >>=(unsigned int shift);

    /**
     * Converts this BigInteger into a boolean value.
     * The value will be false if this BigInteger stores 0, or true otherwise.
     */
    explicit operator bool() const;

    /**
     * Converts this BigInteger into a double.
     * @throw ErrorException if this big integer is not within the range of type long.
     */
    // explicit operator double() const;

    /**
     * Converts this BigInteger into a float.
     * @throw ErrorException if this big integer is not within the range of type long.
     */
    // explicit operator float() const;

    /**
     * Converts this BigInteger into an integer.
     * @throw ErrorException if this big integer is not within the range of type int.
     */
    explicit operator int() const;

    /**
     * Converts this BigInteger into a long.
     * @throw ErrorException if this big integer is not within the range of type long.
     */
    explicit operator long() const;

    /**
     * Converts this BigInteger into a string.
     */
    explicit operator std::string() const;

private:
    /*
     * Constructs a new big integer set to the given value,
     * with optional sign (true=negative, false=positive).
     *
     * Example:
     * BigInteger bi("1234567890123456789", true);   // negative
     */
    BigInteger(const std::string& s, bool sin); // "string" constructor

    // add two big integers as string and return result; used by operator +
    static std::string add(const std::string& number1, const std::string& number2);

    // checks that the given string is in the proper format that it could be
    // interpreted as an integer in the given base; if not, issues an error()
    static void checkStringIsNumeric(const std::string& s, int radix = 10);

    // TODO: make work for big integer division
    // divide n by den and return result; used by operator /
    static std::pair<std::string, long> divide(const std::string& n, long den);
    static std::pair<BigInteger, BigInteger> divideBig(const BigInteger& numer, const BigInteger& denom);

    // return true if two BigIntegers are equal; used by operator ==
    static bool equals(const BigInteger& n1, const BigInteger& n2);

    // checks for -0 case and changes to 0
    void fixNegativeZero();

    /*
     * Returns the number stored by this BigInteger as a string.
     * If this BigInteger represents a negative number, the sign will be omitted.
     * Example: "1234567890123456789"
     */
    const std::string& getNumber() const; // retrieves the number

    /*
     * Returns the sign of this BigInteger; true if negative, false if not.
     */
    bool getSign() const;

    // return true if n1 > n2; used by operator >
    static bool greater(const BigInteger& n1, const BigInteger& n2);

    // return true if n1 < n2; used by operator <
    static bool less(const BigInteger& n1, const BigInteger& n2);

    // multiply two big integers as string and return result; used by operator *
    static std::string multiply(const std::string& n1, const std::string& n2);

    // removes leading 0s except one from given number
    static std::string removeLeadingZeros(const std::string& s);

    /*
     * Sets the number and the sign stored by this BigInteger.
     */
    void setValue(const std::string& s, int radix = 10);

    /*
     * Sets the number stored by this BigInteger.
     */
    void setNumber(const std::string& s, int radix = 10);

    /*
     * Sets the sign stored by this BigInteger.
     * The value true means negative; false means positive.
     */
    void setSign(bool s);

    // e.g. "0xfff" => "fff"
    static std::string stripNumberPrefix(const std::string& num, int radix = 10);

    // subtract num2 from num1 as string and return result; used by operator -
    static std::string subtract(const std::string& number1, const std::string& number2);

    // convert a string value into a long
    static long toLong(const std::string& s);

    // convert a long value to a string
    static std::string toString(long n);

    // TODO: remove?
    BigInteger& operator [](int n);

    friend int hashCode(const BigInteger& b);
    friend BigInteger operator +(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator -(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator *(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator /(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator %(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator &(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator |(const BigInteger& b1, const BigInteger& b2);
    friend BigInteger operator ^(const BigInteger& b1, const BigInteger& b2);
    friend bool operator ==(const BigInteger& b1, const BigInteger& b2);
    friend bool operator !=(const BigInteger& b1, const BigInteger& b2);
    friend bool operator >(const BigInteger& b1, const BigInteger& b2);
    friend bool operator <(const BigInteger& b1, const BigInteger& b2);
    friend bool operator >=(const BigInteger& b1, const BigInteger& b2);
    friend bool operator <=(const BigInteger& b1, const BigInteger& b2);
    friend std::istream& operator >>(std::istream& input, BigInteger& b);
    friend std::ostream& operator <<(std::ostream& out, const BigInteger& b);

    // member variables
    std::string number;   // string representation of this big integer in base-10, e.g. "1234567890123456789"
    bool sign;            // true if number is negative
};

/**
 * Returns a string representation of the given big integer.
 * Equivalent to calling bi.toString().
 * Provided for consistency with the other lib functions like integerToString.
 */
std::string bigIntegerToString(const BigInteger& bi, int radix = 10);

/**
 * Returns an integer hash code for the given BigInteger.
 */
int hashCode(const BigInteger& b);

/**
 * Returns a new BigInteger that is the sum of this BigInteger
 * and the given other BigInteger.
 */
BigInteger operator +(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns a new BigInteger that is the result of subtracting
 * the given other BigInteger from this BigInteger.
 */
BigInteger operator -(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns a new BigInteger that is the product of this BigInteger
 * and the given other BigInteger.
 */
BigInteger operator *(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns a new BigInteger that is the quotient of dividing
 * this BigInteger by the given other BigInteger.
 * @throw ErrorException if denominator is 0.
 * @throw ErrorException if denominator is not within the range of type long.
 */
BigInteger operator /(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns a new BigInteger that is the remainder of dividing
 * this BigInteger by the given other BigInteger.
 * @throw ErrorException if denominator is 0.
 * @throw ErrorException if denominator is not within the range of type long.
 */
BigInteger operator %(const BigInteger& b1, const BigInteger& b2);

/**
 * Performs a bitwise AND between this integer and the given other integer,
 * retaining only bits that are set in both.
 */
BigInteger operator &(const BigInteger& b1, const BigInteger& b2);

/**
 * Performs a bitwise OR between this integer and the given other integer,
 * retaining bits that are set in this integer or the other integer or both.
 */
BigInteger operator |(const BigInteger& b1, const BigInteger& b2);

/**
 * Performs a bitwise XOR between this integer and the given other integer,
 * retaining bits that are set in this integer or the other integer but not both.
 */
BigInteger operator ^(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if two BigIntegers store the same value.
 */
bool operator ==(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if two BigIntegers do not store the same value.
 */
bool operator !=(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if this BigInteger stores a larger value than the given other one.
 */
bool operator >(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if this BigInteger stores a smaller value than the given other one.
 */
bool operator <(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if this BigInteger stores a value that is
 * greater than or equal to the given other one.
 */
bool operator >=(const BigInteger& b1, const BigInteger& b2);

/**
 * Returns true if this BigInteger stores a value that is
 * less than or equal to the given other one.
 */
bool operator <=(const BigInteger& b1, const BigInteger& b2);

/**
 * Reads a BigInteger from the given input stream.
 */
std::istream& operator >>(std::istream& input, BigInteger& b);

/**
 * Writes this BigInteger to the given output stream.
 */
std::ostream& operator <<(std::ostream& out, const BigInteger& b);

/*
 * Free-standing arithmetic operators between int/long and BigInteger.
 * (Commenting these out because when present they seem to also match double,
 * producing bad results.)
 */
//BigInteger operator +(long n, const BigInteger& b);
//BigInteger operator -(long n, const BigInteger& b);
//BigInteger operator *(long n, const BigInteger& b);
//BigInteger operator /(long n, const BigInteger& b);
//BigInteger operator %(long n, const BigInteger& b);

// aliases for BigInteger for those who like abbreviations and lowercase
typedef BigInteger BigInt;
typedef BigInteger bigint;

#endif // _biginteger_h
