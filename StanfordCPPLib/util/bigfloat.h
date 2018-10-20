///// BEGIN BASE.H
//#pragma once

//#ifndef INTERNAL_INCLUDE
//#include "private/initstudent.h"   // insert necessary included code by student
//#endif // INTERNAL_INCLUDE

//#include <stddef.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <vector>
//#include <map>
//#include <stdexcept>
//#include <string>
//#include <sstream>

//#ifdef _MSC_VER
//# define INLINE __forceinline
//  typedef  unsigned __int64 u8;
//  typedef  unsigned __int32 u4;
//  typedef  unsigned __int16 u2;
//  typedef  unsigned __int8  u1;
//  typedef  __int64 s8;
//  typedef  __int32 s4;
//  typedef  __int16 s2;
//  typedef  __int8  s1;
//#else
//# include <stdint.h>
//# define INLINE inline
//  typedef  uint64_t  u8;
//  typedef  uint32_t  u4;
//  typedef  uint16_t  u2;
//  typedef  uint8_t   u1;
//  typedef  int64_t  s8;
//  typedef  int32_t  s4;
//  typedef  int16_t  s2;
//  typedef  int8_t   s1;
//#endif

//class CosmosException : public std::exception
//{
//public:
//    CosmosException(const char* m) : _msg(m) {}
//    CosmosException(const std::string& m) : _msg(m) {}
//    virtual ~CosmosException() throw() {}
//    virtual const char* what() const throw()
//    {
//        return _msg.c_str();
//    }
//protected:
//    std::string _msg;
//};

/*#define ASSERT(condition, ...) do { \
  if (!(condition)) Assert(__LINE__, __FILE__, #condition, ##__VA_ARGS__); \
} while(0) */

/*#define ASSERTFG(f,g) do { \
  double fr = (f); double gr = (g); double diff = (fr-gr); \
  if (diff > 1.0e-10 || diff < -1.0e-10) \
  Assert(__LINE__, __FILE__, #f "," #g, "%f, %f, %f", fr, gr, diff); \
} while(0) */

//static void Assert(
//    int line,
//    const char* filename,
//    const char* condition)
//{
//	static const int bufSize = 4000;
//    char y[bufSize];
//    sprintf(y, "line=[%d], file=[%s], condition=[%s]",
//            line, filename, condition);
//    throw CosmosException(y);
//}

//static void Assert(
//    int line,
//    const char* filename,
//    const char* condition,
//    const char* fmt,
//    ...)
//{
//	static const int bufSize = 4000;
//    char x[bufSize];
//    char y[bufSize];
//    va_list args;
//    va_start(args, fmt);
//    vsprintf(x, fmt, args);
//    va_end(args);
//    sprintf(y, "line=[%d], file=[%s], condition=[%s], %s",
//            line, filename, condition, x);
//    throw CosmosException(y);
//}

///// END BASE.H






////#pragma once
////#include "base.h"


//// This is not fast, but it has good accuracy.
//// I need this because orbital problems have a lot of nth-degree
//// polynomial approximations, and the coefficients of those polynomial
//// approximations are things like 48471792742212/237758976000.  The only
//// way to get the definitions of coefficients right is to do Gaussian
//// elimination on at least n equations with n unknowns, and a precision
//// about twice as great as the coefficients I want to end up with,
//// followed by continued fractions on the result to find the proper
//// fractional representation.
//class BigFloat
//{
//public:
//    BigFloat() { Zero(); }
//    BigFloat(const BigFloat& n) { Copy(n); }
//    BigFloat(s8 n) { FromInteger(n); }
//    BigFloat(s8 n, s8 exponent) { FromInteger(n, exponent); }
//    ~BigFloat() {}

//    // translation
//    BigFloat& FromInteger(s8 num, s8 exponent=0);
//    s8 ToInteger() const;  // it will truncate, but not overflow
//    static s8 RoundInteger(s8 value);  // round an s8 to right precision
//    double ToDouble() const;
//    bool IsNegative() const { return _isNegative; }
//    s8 ToExponent() const { return _exponent; }
//    u8 ToDigits() const;  // return digits filling an integer
//    void ToFraction(BigFloat& num, BigFloat& denom, int iter=1024) const;
//    void Print() const;
//    void PrintHex() const;
//    void PrintContinuedFraction() const;
//    void PrintDouble() const;

//    // arithmetic
//    BigFloat& PZero()
//    {
//        _exponent = c_zeroExponent;
//        _length = 0;
//        _isNegative = false;
//        return *this;
//    }
//    BigFloat& NZero()
//    {
//        _exponent = c_zeroExponent;
//        _length = 0;
//        _isNegative = true;
//        return *this;
//    }
//    BigFloat& Zero( bool neg = false) { return neg ? NZero() : PZero(); }
//    BigFloat& PInf()
//    {
//        _exponent = c_zeroExponent;
//        _length = 1;
//        _isNegative = false;
//        return *this;
//    }
//    BigFloat& NInf()
//    {
//        _exponent = c_zeroExponent;
//        _length = 1;
//        _isNegative = true;
//        return *this;
//    }
//    BigFloat& Inf( bool negative = false) { return negative ? NInf() : PInf(); }
//    BigFloat& NaN()
//    {
//        _exponent = c_zeroExponent;
//        _length = 2;
//        _isNegative = false;
//        return *this;
//    }

//    BigFloat& Copy(const BigFloat& n);
//    BigFloat& Negate();

//    // round to c_digits digits
//    //   carry=true: there should be an additional top digit of 1
//    //   previousDigit: what _d[c_digits] would have been, or 0
//    BigFloat& Round(bool carry, s8 previousDigit);
//    BigFloat& Round(u8 previousDigit);

//    // truncate to the nearest integer, towards zero
//    BigFloat& Trunc();

//    // -1 if |this|<|n|, 0 if |this|==|n|, 1 if |this|>|n|
//    int CompareAbsolute(const BigFloat& n) const;


//    // -1 if this<n, 0 if this==n, 1 if this>n
//    int Compare(const BigFloat& n) const;

//    bool IsZero() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 0;
//    }
//    bool IsPZero() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 0 &&
//            _isNegative == false;
//    }
//    bool IsNZero() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 0 &&
//            _isNegative == true;
//    }
//    bool IsInf() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 1;
//    }
//    bool IsPInf() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 1 &&
//            _isNegative == false;
//    }
//    bool IsNInf() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 1 &&
//            _isNegative == true;
//    }
//    bool IsNaN() const
//    {
//        return
//            _exponent == c_zeroExponent &&
//            _length == 2;
//    }
//    bool IsSpecial() const
//    {
//        return _exponent == c_zeroExponent;
//    }

//    BigFloat& Add(const BigFloat& n) { return AddOrSubtract(n, false); }
//    BigFloat& Sub(const BigFloat& n) { return AddOrSubtract(n, true); }
//    BigFloat& Mult(const BigFloat& n); // x => x*n
//    BigFloat& Div(const BigFloat& n); // x => x/n
//    BigFloat& Invert(); // x => 1/x
//    BigFloat& Sqrt(); // x => positive square root of x
//    BigFloat& Cos(); // x => cosine of x (x in radians)
//    BigFloat& Sin(); // x => sine of x (x in radians)
//    BigFloat& Sec(); // x => secant of x (x in radians)
//    BigFloat& Csc(); // x => cosecant of x (x in radians)
//    BigFloat& Tan(); // x => tangent of x (x in radians)
//    BigFloat& Exp(); // x => e to the xth power
//    BigFloat& ASin(); // x => arcsin of x (-1 => -pi/2, 1 => pi/2)
//    BigFloat& ACos(); // x => arccos of x (-1 => pi, 1 => 0)
//    BigFloat& ATan(); // x => arctan of x (-inf => -pi/2, inf => pi/2)
//    BigFloat& Ln();  // replaces x with the natural log of x
//    BigFloat& Log(const BigFloat& n);  // x => natural log of n base x
//    BigFloat& Power(const BigFloat& n); // replaces x with x to the nth
//    BigFloat& Rand(); // not impl: uniformly distributed value in [0,1)
//    BigFloat& RandNorm(); // not impl: pseudorandom normally-distributed value

//    // constants
//    static const BigFloat& Pi();  // length of unit circle, 3.14159...
//    static const BigFloat& E();  // the natural base for exponents, 2.71828...
//    static const BigFloat& ConstZero();
//    static const BigFloat& ConstOne();
//    static const BigFloat& ConstMinusOne();

//    // variations where arguments are signed integers
//    int Compare(s8 n, s8 exponent=0);
//    BigFloat& Add(s8 n, s8 exponent=0);
//    BigFloat& Sub(s8 n, s8 exponent=0);
//    BigFloat& Mult(s8 n, s8 exponent=0);
//    BigFloat& Div(s8 n, s8 exponent=0);
//    BigFloat& Power(s8 n, s8 exponent=0);  // not implemented

//    // Given an m*(m+1) matrix of BigFloat where the last col means =const,
//    // solve, and fill m[i][m] with the value for the ith variable.
//    static void GaussianElimination(BigFloat** m, s8 rows, s8 cols);
    
//    // assure that it works as expected
//    static void UnitTest();
        
//private:
//    // First, this => this mod 2pi.
//    // Return the quadrant (int)(this / (pi/4)), value 0..7
//    // this => (this + pi/4) mod pi/2 (positive), - pi/4.
//    // That means a negative value for odd quadrants and positive for even.
//    s8 Quadrant();
//    BigFloat& PartialSin();  // sin, but only for -pi/4 to pi/4
//    BigFloat& PartialCos();  // cos, but only for -pi/4 to pi/4
    
//    // this+n, or this-n if minus==true
//    BigFloat& AddOrSubtract(const BigFloat& n, bool minus);

//    // test whether this is the right representation of this integer
//    static void TestInteger(const BigFloat& n, s8 x);

//    // test addition and subtraction of two integers
//    static void TestAdd(s8 x, s8 y);

//    // test multiplication of two numbers
//    static void TestMult(s8 x, s8 ex, s8 y, s8 ey);

//    // test inverse of one number
//    static void TestInverse(s8 x, s8 ex);

//    // test sqrt of one number
//    static void TestSqrt(s8 x, s8 ex);

//    // representation: c_digits digits, each with range 0..c_range-1
//    // _d[0] is the most significant digit
//#ifdef BIGFLOAT_TEST
//    static const s8 c_digits = 4;
//    static const s8 c_log = 2;
//    static const s8 c_zeroExponent = -(((s8)1) << 4);
//#else
//    static const s8 c_digits = 10;
//    static const s8 c_log = 32;

//    // -1<<63 is a signed value, but 1<<63 is not, so 1<<62 then
//    static const s8 c_zeroExponent = -(((s8)1) << 62);
//#endif
//    static const s8 c_minExponent = c_zeroExponent + c_digits;
//    static const s8 c_maxExponent =  -c_zeroExponent;
//    static const u8 c_range = (((u8)1)<<c_log);

//    s8 _exponent;
//    u4 _d[c_digits];
//    bool _isNegative;
//    u2 _length; // number of digits used
//};

//class BigFloatCache
//{
//public:
//    static bool _isInitialized;
//    static BigFloat _zero;
//    static BigFloat _e;
//    static BigFloat *_ePower; // [i] is e^^(2^^i), i in -6..ePowerLen-1
//    static BigFloat *_eInvPower; // [i] is e^^-(2^^i), i in -6..ePowerLen-1
//    static s8 _ePowerLen;
//    static const s8 _ePowerNeg = -7;
//    static BigFloat _pi;
//    static BigFloat _twoPi;
//    static BigFloat _overTwoPi;
//    static BigFloat _piOverTwo;
//    static BigFloat _threePiOverTwo;
//    static BigFloat _piOverFour;
//    static s8 _overFactLen;
//    static BigFloat* _overFact;

//    // fill in _pi, _e, and various cached arrays
//    static void Init();
//};
