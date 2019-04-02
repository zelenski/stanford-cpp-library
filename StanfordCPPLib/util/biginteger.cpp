/*
 * File: biginteger.cpp
 * --------------------
 * This file implements members of a class for arbitrary-size integer arithmetic.
 * It is meant to help get around the max/min value limit for types
 * such as int and long.
 * See biginteger.h for declarations and documentation of each member.
 *
 * @version 2017/11/05
 * - fixed compiler error on some older clang versions about string insert call
 * @version 2017/10/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "biginteger.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static int STRING_SIZE_MAX = 10000;   // for strings

const BigInteger BigInteger::NEGATIVE_ONE("-1");
const BigInteger BigInteger::ZERO("0");
const BigInteger BigInteger::ONE("1");
const BigInteger BigInteger::TWO("2");
const BigInteger BigInteger::TEN("10");
const BigInteger BigInteger::MAX_INT("2147483647");
const BigInteger BigInteger::MIN_INT("-2147483648");
const BigInteger BigInteger::MAX_UINT("4294967295");
const BigInteger BigInteger::MAX_LONG("9223372036854775807");
const BigInteger BigInteger::MIN_LONG("-9223372036854775808");
const BigInteger BigInteger::MAX_ULONG("18446744073709551615");
const BigInteger BigInteger::MAX_SHORT("32767");
const BigInteger BigInteger::MIN_SHORT("-32768");
const BigInteger BigInteger::MAX_USHORT("65535");

BigInteger::BigInteger()
    : number("0"),
      sign(false) {
    // empty
}

BigInteger::BigInteger(const BigInteger& other)
    : number(other.number),
      sign(other.sign) {
    // empty
}

BigInteger::BigInteger(const std::string& s, int radix) {
    setValue(s, radix);
}

BigInteger::BigInteger(const std::string& s, bool sin) {
    setValue(s);
    setSign(sin);
}

BigInteger::BigInteger(long n) {
    setValue(toString(n));
}

BigInteger BigInteger::abs() const {
    return BigInteger(getNumber());
}

std::string BigInteger::add(const std::string& n1, const std::string& n2) {
    // sum variable to store result;
    // left-pad numbers with 0s to be the same length as needed
    int digits = (int) (n1.length() > n2.length() ? n1.length() : n2.length());
    std::string sum = padLeft("", digits);
    std::string n1copy = padLeft(n1, digits);
    std::string n2copy = padLeft(n2, digits);

    // add each digit pair
    int carry = 0;
    for (int i = n1copy.length() - 1; i >= 0; --i) {
        sum[i] = (carry + (n1copy[i] - '0') + (n2copy[i] - '0')) + '0';
        if (i != 0) {
            if (sum[i] > '9') {
                sum[i] -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }
    }

    if (sum[0] > '9') {
        // top digit carried over; make an additional digit at front
        sum[0] -= 10;
        sum.insert(0, (size_t) 1, '1');
    }

    return sum;
}

void BigInteger::checkStringIsNumeric(const std::string& s, int radix) {
    std::string scopy = stripNumberPrefix(s, radix);

    // first char can optionally be + or - sign
    if (scopy.empty()) {
        error("Empty string cannot be converted into an integer");
    }

    int start = 0;
    if (scopy[0] == '+' || scopy[0] == '-') {
        start++;
    }
    for (int i = start, len = (int) scopy.length(); i < len; i++) {
        char ch = tolower(scopy[i]);
        bool good = false;
        if (radix == 1) {
            good = ch == '1';
        } else if (radix <= 10) {
            good = ch >= '0' && ch < '0' + radix;
        } else if (radix <= 16) {
            good = isdigit(ch) || (ch >= 'a' && ch < radix - 10 + 'a');
        } else {
            // don't know how to verify other bases very well
            good = isalnum(ch);
        }

        if (!good) {
            error("Non-numeric string passed: \"" + scopy + "\"");
        }
    }
}

std::pair<std::string, long> BigInteger::divide(const std::string& n, long den) {
    if (den == 0) {
        error("Cannot divide by zero");
    }

    long rem = 0;
    std::string result;
    result.resize(STRING_SIZE_MAX);

    for (int indx = 0, len = (int) n.length(); indx < len; ++indx) {
        rem = (rem * 10) + (n[indx] - '0');
        result[indx] = rem / den + '0';
        rem %= den;
    }

    result.resize(n.length());
    result = removeLeadingZeros(result);

    return make_pair(result, rem);
}

// Returns (quotient, remainder) as a pair 2-tuple.
// Implementation note: This is a stupid implementation.
// It is quite slow and there are much better algorithms.  But it works.
// see also:
// https://en.wikipedia.org/wiki/Division_algorithm#Division_by_repeated_subtraction
// pre: b2 != 0
std::pair<BigInteger, BigInteger> BigInteger::divideBig(const BigInteger& numerator, const BigInteger& denominator) {
    if (denominator == ZERO) {
        error("Cannot divide by zero");
    } else if (denominator.isNegative()) {
        std::pair<BigInteger, BigInteger> result = divideBig(numerator, -denominator);
        result.second = -result.second;
        return result;
    } else if (numerator.isNegative()) {
        std::pair<BigInteger, BigInteger> result = divideBig(-numerator, denominator);
        if (result.second == ZERO) {
            result.first = -result.first;
        } else {
            result.first = -result.first - 1;
            result.second = denominator - result.second;
        }
        return result;
    } else {
        // TODO: implement a proper division algorithm
        error("Denominator too large to divide: " + denominator.toString());
    }

    /* TODO: Uncomment this section and properly implement it.
    // now b1 >= 0 && b2 > 0
    BigInteger quotient(ZERO);
    BigInteger remainder(numerator);
    while (remainder >= denominator) {
        ++quotient;
        remainder -= denominator;
    }
    std::pair<BigInteger, BigInteger> result {quotient, remainder};
    return result;
    */
}

bool BigInteger::equals(const BigInteger& n1, const BigInteger& n2) {
    return n1.getNumber() == n2.getNumber()
        && n1.getSign() == n2.getSign();
}

void BigInteger::fixNegativeZero() {
    if (number == "0") {
        // avoid (-0) problem
        sign = false;
    }
}

BigInteger BigInteger::gcd(const BigInteger& other) const {
    BigInteger a(*this);
    BigInteger b(other);
    while (b != ZERO) {
        BigInteger temp(b);
        b = a % b;
        a = temp;
    }
    return a;
}

const std::string& BigInteger::getNumber() const {
    return number;
}

bool BigInteger::getSign() const {
    return sign;
}

bool BigInteger::greater(const BigInteger& n1, const BigInteger& n2) {
    return less(n2, n1);
}

bool BigInteger::isInt() const {
    return stringIsInteger(toString());
}

bool BigInteger::isLong() const {
    return stringIsLong(toString());
}

bool BigInteger::isNegative() const {
    return sign;   // true means negative
}

bool BigInteger::isNonNegative() const {
    return !sign;   // true means negative
}

bool BigInteger::isPositive() const {
    return !sign && number[0] != '0';
}

bool BigInteger::less(const BigInteger& n1, const BigInteger& n2) {
    bool sign1 = n1.getSign();
    bool sign2 = n2.getSign();

    if (sign1 && ! sign2) {
        // n1 is -ve and n2 is +ve
        return true;
    } else if (!sign1 && sign2) {
        // n1 is +ve and n2 is -ve
        return false;
    } else if (!sign1) {
        // both +ve
        if (n1.getNumber().length() < n2.getNumber().length()) {
            return true;
        } else if (n1.getNumber().length() > n2.getNumber().length()) {
            return false;
        } else {
            return n1.getNumber() < n2.getNumber();
        }
    } else {
        // both -ve
        if (n1.getNumber().length() > n2.getNumber().length()) {
            return true;
        } else if (n1.getNumber().length() < n2.getNumber().length()) {
            return false;
        } else {
            // greater with -ve sign is LESS
            return n1.getNumber().compare(n2.getNumber()) > 0;
        }
    }
}

const BigInteger& BigInteger::max(const BigInteger& other) const {
    if (*this < other) {
        return other;
    } else {
        return *this;
    }
}

const BigInteger& BigInteger::min(const BigInteger& other) const {
    if (*this > other) {
        return other;
    } else {
        return *this;
    }
}

BigInteger BigInteger::modPow(const BigInteger& exp, const BigInteger& m) const {
    BigInteger result = ONE;
    for (BigInteger i = ZERO; i < exp; i++) {
        result = (result * *this) % m;
    }
    return result;
}

std::string BigInteger::multiply(const std::string& n1, const std::string& n2) {
    std::string n1copy = n1;
    std::string n2copy = n2;

    if (n1copy.length() > n2copy.length()) {
        n1copy.swap(n2copy);
    }

    std::string product = "0";
    for (int i = n1copy.length() - 1; i >= 0; --i) {
        std::string temp = n2copy;
        int currentDigit = n1copy[i] - '0';
        int carry = 0;
        for (int j = temp.length() - 1; j >= 0; --j) {
            temp[j] = ((temp[j] - '0') * currentDigit) + carry;
            if (temp[j] > 9) {
                carry = (temp[j] / 10);
                temp[j] -= (carry * 10);
            } else {
                carry = 0;
            }

            temp[j] += '0';   // back to string mode
        }

        if (carry > 0) {
            temp.insert(0, (size_t) 1, (char) (carry + '0'));
        }

        // as like mult by 10, 100, 1000, 10000 and so on
        temp.append((n1copy.length() - i - 1), '0');

        product = add(product, temp);   // O(n)
    }

    // erase leading zeros
    product = removeLeadingZeros(product);
    return product;
}

BigInteger BigInteger::pow(long exp) const {
    if (exp < 0) {
        error("negative exponent: " + std::to_string(exp));
    } else if (exp == 0) {
        return ONE;
    } else if (exp == 1) {
        return *this;
    }

    // optimization for even exponents
    BigInteger thisCopy(*this);
    BigInteger result(ONE);
    while (exp > 0) {
        if (exp % 2 == 0) {
            thisCopy *= thisCopy;
            exp /= 2;
        } else {
            result *= thisCopy;
            exp--;
        }
    }
    return result;
}

BigInteger BigInteger::pow(const BigInteger& exp) const {
    if (exp < ZERO) {
        error("negative exponent: " + exp.toString());
    } else if (exp == ZERO) {
        return ONE;
    } else if (exp == ONE) {
        return *this;
    }

    // optimization for even exponents
    BigInteger thisCopy(*this);
    BigInteger expCopy(exp);
    BigInteger result(ONE);
    while (expCopy > ZERO) {
        if (expCopy % TWO == ZERO) {
            thisCopy *= thisCopy;
            expCopy /= TWO;
        } else {
            result *= thisCopy;
            expCopy--;
        }
    }
    return result;
}

std::string BigInteger::removeLeadingZeros(const std::string& s) {
    std::string result = s;
    while (result[0] == '0' && (int) result.length() > 1) {
        result.erase(0, 1);
    }
    if ((int) result.length() == 0) {
        result = "0";
    }
    return result;
}

void BigInteger::setValue(const std::string& s, int radix) {
    std::string scopy = stripNumberPrefix(s, radix);
    checkStringIsNumeric(scopy, radix);
    if (scopy[0] == '+' || scopy[0] == '-') {
        // signed value; separate sign from number
        setNumber(scopy.substr(1), radix);
        sign = (scopy[0] == '-');
    } else {
        // not signed; positive value
        setNumber(scopy, radix);
        sign = false;
    }
    fixNegativeZero();
}

// TODO: throw error on negative/too-large radixes

void BigInteger::setNumber(const std::string& s, int radix) {
    // accept hex as 0x???, bin as 0b???, oct as 0o???
    std::string scopy = stripNumberPrefix(s, radix);
    checkStringIsNumeric(scopy, radix);
    if (radix == 10) {
        number = scopy;
    } else if (scopy == "0") {
        number = scopy;   // 0 is 0 in any base
    } else if (radix == 1) {
        std::ostringstream out;
        for (int i = 0; i < radix; i++) {
            out << '1';
        }
        *this = BigInteger(out.str());
    } else {
        // convert to base-10 by plucking off a "digit" char,
        // and adding it to a cumulative sum BigInteger variable
        BigInteger result(ZERO);
        BigInteger power(ONE);
        BigInteger biRadix(std::to_string(radix));
        for (int i = (int) scopy.length() - 1; i >= 0; i--) {
            std::string ch = scopy.substr(i, 1);
            int val = stringToInteger(ch, radix);
            result += power * BigInteger(std::to_string(val));
            power *= biRadix;
        }
        *this = result;
    }
    fixNegativeZero();
}

void BigInteger::setSign(bool s) {
    sign = s;
    fixNegativeZero();
}

std::string BigInteger::stripNumberPrefix(const std::string& num, int radix) {
    std::string result;
    if (radix == 2 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'b') {
        result = num.substr(2);
    } else if (radix == 8 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'o') {
        result = num.substr(2);
    } else if (radix == 16 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'x') {
        result = num.substr(2);
    } else {
        result = num;
    }
    result = stringReplace(result, ",", "");
    result = stringReplace(result, "_", "");
    result = stringReplace(result, " ", "");
    return result;
}

std::string BigInteger::subtract(const std::string& n1, const std::string& n2) {
    // sub variable to store result;
    // left-pad numbers with 0s to be the same length as needed
    int digits = (int) (n1.length() > n2.length() ? n1.length() : n2.length());
    std::string sub = padLeft("", digits);
    std::string n1copy = padLeft(n1, digits);
    std::string n2copy = padLeft(n2, digits);

    for (int i = (int) n1copy.length() - 1; i >= 0; --i) {
        if (n1copy[i] < n2copy[i]) {
            n1copy[i] += 10;
            n1copy[i - 1]--;
        }
        sub[i] = ((n1copy[i] - '0') - (n2copy[i] - '0')) + '0';
    }

    // erase leading zeros
    sub = removeLeadingZeros(sub);

    return sub;
}

long BigInteger::toLong(const std::string& s) {
    long sum = 0;
    long prevSum = 0;
    for (int i = 0; i < (int) s.length(); i++) {
        prevSum = sum;
        sum = (sum * 10) + (s[i] - '0');
        if (sum < prevSum) {
            error("numeric overflow when converting string to long: \"" + s + "\"");
        }
    }
    return sum;
}

int BigInteger::toInt() const {
    return (int) toLong(number);
}

long BigInteger::toLong() const {
    return toLong(number);
}

std::string BigInteger::toString(int radix) const {
    if (radix == 10) {
        return std::string(*this);
    } else if (radix <= 0) {
        error("Illegal radix value: " + std::to_string(radix));
    } else if (number == "0") {
        return "0";
    } else if (radix == 1) {
        std::ostringstream out;
        for (BigInteger i = ZERO; i < *this; i++) {
            out << "1";
        }
        return out.str();
    } else {
        // write string (reversed) into an output stream, digit by digit
        std::ostringstream out;
        out << std::setbase(radix);
        BigInteger copy(this->abs());
        BigInteger biRadix(std::to_string(radix));
        while (copy.isPositive()) {
            int lastDigit = (copy % biRadix).toInt();
            out << lastDigit;
            copy /= biRadix;
        }
        if (isNegative()) {
            out << '-';
        }

        // reverse and return the string
        std::string str = out.str();
        std::reverse(str.begin(), str.end());
        return str;
    }
    return "0";
}

std::string BigInteger::toString(long n) {
    std::stringstream ss;
    std::string temp;
    ss << n;
    ss >> temp;
    return temp;
}

BigInteger& BigInteger::operator =(const BigInteger& b) {
    setNumber(b.getNumber());
    setSign(b.getSign());
    return *this;
}

BigInteger& BigInteger::operator ++() {
    (*this) = (*this) + ONE;
    return (*this);
}

BigInteger BigInteger::operator ++(int) {
    BigInteger before = (*this);
    (*this) = (*this) + ONE;
    return before;
}

BigInteger& BigInteger::operator --() {
    (*this) = (*this) - ONE;
    return (*this);
}

BigInteger BigInteger::operator --(int) {
    BigInteger before = (*this);
    (*this) = (*this) - ONE;
    return before;
}

BigInteger& BigInteger::operator +=(const BigInteger& b) {
    (*this) = (*this) + b;
    return (*this);
}

BigInteger& BigInteger::operator -=(const BigInteger& b) {
    (*this) = (*this) - b;
    return (*this);
}

BigInteger& BigInteger::operator *=(const BigInteger& b) {
    (*this) = (*this) * b;
    return (*this);
}

BigInteger& BigInteger::operator /=(const BigInteger& b) {
    (*this) = (*this) / b;
    return (*this);
}

BigInteger& BigInteger::operator %=(const BigInteger& b) {
    (*this) = (*this) % b;
    return (*this);
}

BigInteger& BigInteger::operator &=(const BigInteger& b) {
    *this = (*this & b);
    return *this;
}

BigInteger& BigInteger::operator |=(const BigInteger& b) {
    *this = (*this | b);
    return *this;
}

BigInteger& BigInteger::operator ^=(const BigInteger& b) {
    *this = (*this ^ b);
    return *this;
}

BigInteger BigInteger::operator ~() const {
    std::string s1 = this->toString(2);

    // loop over each bit, performing ~ operation
    for (int i = 0, len = (int) s1.length(); i < len; i++) {
        bool b1 = s1[i] == '1';
        s1[i] = b1 ? '0' : '1';
    }
    s1 = removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger BigInteger::operator !() const {
    if (*this == ZERO) {
        return ONE;
    } else {
        return ZERO;
    }
}

BigInteger& BigInteger::operator [](int n) {
    return *(this + (n * sizeof(BigInteger)));
}

BigInteger BigInteger::operator -() const {
    return BigInteger(getNumber(), !getSign());
}

//BigInteger& BigInteger::operator =(const BigInteger& other) {
//    this->number = other.number;
//    this->sign = other.sign;
//}

BigInteger BigInteger::operator <<(unsigned int shift) const {
    BigInteger result(*this);
    for (unsigned int i = 0; i < shift; i++) {
        result *= TWO;
    }
    return result;
}

BigInteger& BigInteger::operator <<=(unsigned int shift) {
    (*this) = (*this) << shift;
    return *this;
}

BigInteger BigInteger::operator >>(unsigned int shift) const {
    BigInteger result(*this);
    for (unsigned int i = 0; i < shift; i++) {
        result /= TWO;
    }
    return result;
}

BigInteger& BigInteger::operator >>=(unsigned int shift) {
    (*this) = (*this) >> shift;
    return *this;
}

BigInteger::operator bool() const {
    return *this != ZERO;
}

//BigInteger::operator double() const {
//    return (double) toLong();
//}

//BigInteger::operator float() const {
//    return (float) toLong();
//}

BigInteger::operator int() const {
    return toInt();
}

BigInteger::operator long() const {
    return toLong();
}

BigInteger::operator std::string() const {
    // if positive, don't print + sign
    std::string signedString = (getSign()) ? "-" : "";
    signedString += number;
    return signedString;
}

std::string bigIntegerToString(const BigInteger& bi, int radix) {
    return bi.toString(radix);
}

int hashCode(const BigInteger& b) {
    return hashCode(b.getNumber(), b.getSign());
}

BigInteger operator +(const BigInteger& b1, const BigInteger& b2) {
    BigInteger sum;
    if (b1.getSign() == b2.getSign()) {
        // both +ve or -ve
        sum.setNumber(BigInteger::add(b1.getNumber(), b2.getNumber()));
        sum.setSign(b1.getSign());
    } else {
        // sign different
        if (b1.abs() > b2.abs()) {
            sum.setNumber(BigInteger::subtract(b1.getNumber(), b2.getNumber()));
            sum.setSign(b1.getSign());
        } else {
            sum.setNumber(BigInteger::subtract(b2.getNumber(), b1.getNumber()));
            sum.setSign(b2.getSign());
        }
    }
    if (sum.getNumber() == "0") {
        // avoid (-0) problem
        sum.setSign(false);
    }
    return sum;
}

BigInteger operator -(const BigInteger& b1, const BigInteger& b2) {
    // x - y = x + (-y)
    BigInteger bcopy = b2;
    bcopy.setSign(!b2.getSign());
    return b1 + bcopy;
}

BigInteger operator *(const BigInteger& b1, const BigInteger& b2) {
    BigInteger product;
    product.setNumber(BigInteger::multiply(b1.getNumber(), b2.getNumber()));
    product.setSign(b1.getSign() != b2.getSign());
    return product;
}

BigInteger operator /(const BigInteger& b1, const BigInteger& b2) {
    if (b2 == BigInteger::ZERO) {
        error("Division by zero");
    } else if (BigInteger::MIN_LONG <= b2 && b2 <= BigInteger::MAX_LONG) {
        long den = BigInteger::toLong(b2.getNumber());
        BigInteger div;
        div.setNumber(BigInteger::divide(b1.getNumber(), den).first);
        div.setSign(b1.getSign() != b2.getSign());
        return div;
    }

    // else large denominator; compute through naive repeated subtraction
    // (this is not a smart way to do large integer division, but this
    // library is not meant to be particularly clever or performant)
    std::pair<BigInteger, BigInteger> result = BigInteger::divideBig(b1, b2);
    return result.first;
}

BigInteger operator %(const BigInteger& b1, const BigInteger& b2) {
    if (b2 == BigInteger::ZERO) {
        error("Division by zero");
    } else if (BigInteger::MIN_LONG <= b2 && b2 <= BigInteger::MAX_LONG) {
        long den = BigInteger::toLong(b2.getNumber());
        BigInteger div;
        div.setNumber(BigInteger::divide(b1.getNumber(), den).first);
        div.setSign(b1.getSign() != b2.getSign());
        return div;
    }

    // else large denominator; compute through naive repeated subtraction
    // (this is not a smart way to do large integer division, but this
    // library is not meant to be particularly clever or performant)
    std::pair<BigInteger, BigInteger> result = BigInteger::divideBig(b1, b2);
    return result.second;
}

BigInteger operator &(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing & operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 && b2) ? '1' : '0';
    }
    s1 = BigInteger::removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger operator |(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing | operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 || b2) ? '1' : '0';
    }
    // don't need to check for leading 0s because | can only make more 1s than before
    // s1 = removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger operator ^(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing ^ operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 ^ b2) ? '1' : '0';
    }
    s1 = BigInteger::removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

bool operator ==(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2);
}

bool operator !=(const BigInteger& b1, const BigInteger& b2) {
    return !BigInteger::equals(b1, b2);
}

bool operator >(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::greater(b1, b2);
}

bool operator <(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::less(b1, b2);
}

bool operator >=(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2)
            || BigInteger::greater(b1, b2);
}

bool operator <=(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2)
            || BigInteger::less(b1, b2);
}

std::istream& operator >>(std::istream& input, BigInteger& b) {
    std::string s;
    input >> s;
    b.setValue(s);
    return input;
}

std::ostream& operator <<(std::ostream& out, const BigInteger& b) {
    return out << std::string(b);
}

//BigInteger operator +(int n, const BigInteger& b) {
//    return BigInteger(n) + b;
//}

//BigInteger operator -(int n, const BigInteger& b) {
//    return BigInteger(n) - b;
//}

//BigInteger operator *(int n, const BigInteger& b) {
//    return BigInteger(n) * b;
//}

//BigInteger operator /(int n, const BigInteger& b) {
//    return BigInteger(n) / b;
//}

//BigInteger operator %(int n, const BigInteger& b) {
//    return BigInteger(n) % b;
//}
