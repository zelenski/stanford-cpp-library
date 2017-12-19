//#include "bigfloat.h"
//#include <limits>

//bool BigFloatCache::_isInitialized = false;
//BigFloat BigFloatCache::_e;
//BigFloat BigFloatCache::_zero;
//BigFloat* BigFloatCache::_ePower;
//s8 BigFloatCache::_ePowerLen;
//BigFloat* BigFloatCache::_eInvPower;
//BigFloat BigFloatCache::_pi;
//BigFloat BigFloatCache::_twoPi;
//BigFloat BigFloatCache::_overTwoPi;
//BigFloat BigFloatCache::_piOverTwo;
//BigFloat BigFloatCache::_threePiOverTwo;
//BigFloat BigFloatCache::_piOverFour;
//s8 BigFloatCache::_overFactLen;
//BigFloat* BigFloatCache::_overFact;

//void BigFloatCache::Init()
//{
//    s8 i;
//    const int c_maxIter = 1000;

//    // fill _pi using Brent-Salamin algorithm
//    BigFloat a(1);

//    BigFloat b(2);
//    b.Sqrt();
//    b.Invert();

//    BigFloat t(4);
//    t.Invert();

//    BigFloat p(1);

//    BigFloat oldPi(3);
//    BigFloat newPi;

//    // for 32 32-bit digits, this stops after 8 iterations
//    for (i=0; i<c_maxIter; ++i)
//    {
//        BigFloat a2(a);
//        a2.Add(b);
//        a2.Div(2);

//        b.Mult(a);
//        b.Sqrt();

//        BigFloat t2(a);
//        t2.Sub(a2);
//        t2.Mult(t2);
//        t2.Mult(p);
//        t.Sub(t2);
        
//        p.Mult(2);

//        a.Copy(a2);

//        newPi.Copy(a);
//        newPi.Add(b);
//        newPi.Mult(newPi);
//        newPi.Div(t);
//        newPi.Div(4);

//        if (newPi.Compare(oldPi) == 0)
//            break;

//        oldPi.Copy(newPi);
//    }
//    ASSERT(i < c_maxIter);

//    oldPi.Copy(a);
//    oldPi.Add(b);
//    oldPi.Mult(oldPi);
//    BigFloat piDivisor(t);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _twoPi.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _pi.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _piOverTwo.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _piOverFour.Copy(newPi);  // higher precision than pi/2 or pi or 2pi

//    _threePiOverTwo.Copy(_pi);
//    _threePiOverTwo.Mult(3);
//    _threePiOverTwo.Div(2);
//    _overTwoPi.Copy(_twoPi);
//    _overTwoPi.Invert();

//    // fill _e using Brother's formula (7)
//    BigFloat e(0);
//    BigFloat oldE(-1);
//    BigFloat denom(1);

//    // for 32 32-bit digits, this takes 42 iterations
//    for (i=0; i<c_maxIter; ++i)
//    {
//        BigFloat term((8*i*i+1) * (8*i-4) + 5);
//        term.Div(denom);
//        e.Add(term);
//        if (e.Compare(oldE) == 0)
//        {
//            break;
//        }
//        oldE.Copy(e);
//        denom.Mult((4*i+1) * (4*i+2) * (4*i+3) * (4*i+4));
//    }
//    ASSERT(i < c_maxIter);
//    _e.Copy(e);
    
//    // precompute 1/n!, to reduce the cost of sin cos tan exp, etc.
//    _overFactLen = 4*i+32;
//    _overFact = new BigFloat[static_cast<unsigned int>(_overFactLen)];
//    BigFloat fact(1);
//    _overFact[0].Copy(1);
//    for (i = 1; i < _overFactLen; ++i)
//    {
//        fact.Mult(i);
//        _overFact[i].Copy(fact);
//        _overFact[i].Invert();
//    }

//    BigFloat power(_e);
//    for (i = 1; !power.IsSpecial(); ++i)
//    {
//        power.Mult(power);
//    }
//    _ePowerLen = i;
//    BigFloat* ePowerBuf = new BigFloat[static_cast<unsigned int>(_ePowerLen-_ePowerNeg)];
//    _ePower = &ePowerBuf[-_ePowerNeg];
//    _ePower[0].Copy(_e);
//    BigFloat* eInvPowerBuf = new BigFloat[static_cast<unsigned int>(_ePowerLen-_ePowerNeg)];
//    _eInvPower = &eInvPowerBuf[-_ePowerNeg];
//    _eInvPower[0].Copy(_e);
//    _eInvPower[0].Invert();

//    for (int i=1; i<_ePowerLen; ++i)
//    {
//        _ePower[i].Copy(_ePower[i-1]);
//        _ePower[i].Mult(_ePower[i]);
//        _eInvPower[i].Copy(_eInvPower[i-1]);
//        _eInvPower[i].Mult(_eInvPower[i]);
//    }

//    for (int i=-1; i>_ePowerNeg; --i)
//    {
//        _ePower[i].Copy(_ePower[1+i]);
//        _ePower[i].Sqrt();
//        _eInvPower[i].Copy(_eInvPower[1+i]);
//        _eInvPower[i].Sqrt();
//    }

//    _isInitialized = true;
//}


//// Do Gaussian elimination on m[rows][cols+1] (destructively).
//// Put the results in m[*][cols].
//void BigFloat::GaussianElimination(BigFloat** m, s8 rows, s8 cols)
//{
//    s8 i;  // currently working on ith column
//    s8 k;  // eliminating m[i] from m[k]
//    s8 j;  // eliminating m[i][j] from m[k][j]

//    // build a triangular matrix
//    for (i=0; i<cols; ++i)
//    {
//        // swap a row to i such that m[i][i] is nonzero
//        for (k=i; k<rows; ++k)
//        {
//            if (!m[k][i].IsZero())
//            {
//                break;
//            }
//        }
//        ASSERT(k < rows);
//        if (k != i)
//        {
//            BigFloat* temp = m[i];
//            m[i] = m[k];
//            m[k] = temp;
//        }

//        // do the elimination
//        for (k=i+1; k<rows; ++k)
//        {
//            BigFloat c(m[k][i]);
//            c.Div(m[i][i]);
//            for (j=i; j<cols+1; ++j)
//            {
//                BigFloat temp(m[i][j]);
//                temp.Mult(c);
//                m[k][j].Sub(temp);
//            }
//        }
//    }

//    // use the triangular matrix to find the results
//    for (i=cols; i--; )
//    {
//        for (j=i+1; j<cols; ++j)
//        {
//            BigFloat temp(m[i][j]);
//            temp.Mult(m[j][cols]);
//            m[i][cols].Sub(temp);
//        }
//        m[i][cols].Div(m[i][i]);
//    }
//}


//BigFloat& BigFloat::FromInteger(s8 origNum, s8 exponent)
//{
//    if (origNum == 0)
//    {
//        return Zero();
//    }

//    _isNegative = (origNum < 0);
//    u8 num = (u8)(_isNegative ? -origNum : origNum);

//    u8 shift = 64 - c_log;
//    _exponent = shift / c_log;
//    u8 mask = (c_range-1) << shift;
//    while (!(num & mask))
//    {
//        --_exponent;
//        num <<= c_log;
//    }

//    _length = 0;
//    _exponent += exponent;
//    while (num && _length < c_digits)
//    {
//        _d[_length++] = static_cast<u4>((num & mask) >> shift);
//        num <<= c_log;
//    }

//    // this shows up in test mode: allow truncation if too much precision
//    // ASSERT(!num, "integer had too much precision\n");

//    return *this;
//}


//// truncate if there is a fraction
//// throw an exception if it is bigger than an 8-byte signed integer
//// ignore the sign
//s8 BigFloat::ToInteger() const
//{
//    if (IsZero())
//        return 0;
    
//    ASSERT((_exponent < ((64-c_log)/c_log)) ||
//           (_exponent == (64-c_log)/c_log && _d[0] < c_range/2),
//           "exponent unsuitable for integer conversion");
    
//    s8 result = 0;
//    u8 pos = 0;
//    while (pos < _length)
//    {
//        result <<= c_log;
//        result += _d[pos++];
//    }
//    s8 shift = c_log * (_exponent + 1 - _length);
//    if (shift > 0)
//        result <<= shift;
//    else if (shift < 0)
//        result >>= -shift;
//    if (_isNegative)
//        result = -result;
//    return result;
//}


//// Ignore sign and exponent.
//// Stick the digits in a u8.
//// The highest digit is multiplied by (((u8)1) << (64-c_log))
//u8 BigFloat::ToDigits() const
//{
//    u8 result = 0;
//    for (int pos=_length; pos--; )
//    {
//        result >>= c_log;
//        result += (u8)_d[pos] << (64 - c_log);
//    }
//    return result;
//}


//// convert bigfloat to double
//double BigFloat::ToDouble() const
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return _isNegative ? -0.0 : 0.0;
//        else if (IsInf())
//            return _isNegative ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
//        else
//            return std::numeric_limits<double>::quiet_NaN();
//    }

//    double x = 0.0;

//    // handle the digits
//    for (int i=_length; i--; )
//    {
//        x /= (((u8)1) << c_log);
//        x += _d[i];
//    }

//    // handle the exponent
//    for (int i=0; i<_exponent; ++i)
//        x *= (((u8)1) << c_log);
//    for (int i=0; i>_exponent; --i)
//        x /= (((u8)1) << c_log);

//    // sign
//    if (_isNegative)
//        x = -x;
    
//    return x;
//}


//// print an accurate initializer for a double: (n / d) * 2^^x, where 1 <= n/d < 2
//void BigFloat::PrintContinuedFraction() const
//{
//    BigFloat n1; // numerator #1
//    BigFloat n2; // numerator #2
//    BigFloat d1; // denominator #1
//    BigFloat d2; // denominator #2
//    BigFloat r;  // remainder

//    if (IsZero())
//    {
//        printf("0.0");
//        return;
//    }
    
//    // find the power of 2 to put *this in [1.0, 2.0)
//    n1.Copy(*this);
//    s8 x = 0;
//    char sign[2] = {0, 0};
//    if (n1.IsNegative())
//    {
//        sign[0] = '-';
//        n1.Negate();
//    }

//    while (n1.Compare(2) >= 0)
//    {
//        n1.Div(2);
//        x += 1;
//    }
//    while (n1.Compare(1) < 0)
//    {
//        n1.Mult(2);
//        x -= 1;
//    }

//    if (x < -400)
//    {
//        printf("0.0");
//        return;
//    }
    
//    printf("(");
    
//    // fill n1/d1 with the first approximation
//    r.Copy(n1);
//    n1.Trunc();
//    d1.Copy(1);

//    // fill n2/d2 with the second approximation
//    r.Sub(n1);
//    if (r.IsZero())
//    {
//        printf("0x%llx)", n1.ToInteger());
//        return;
//    }
//    r.Invert();
//    d2.Copy(r).Trunc();
//    r.Sub(d2);
//    n2.Copy(n1).Mult(d2).Add(1);

//    // recurse until we get too precise: 1/r2 = c + r3, and n3/d3 = (c*n2 + n1)/(c*d2 + d1)
//    for (;;)
//    {
//        r.Invert();
//        BigFloat c(r);
//        c.Trunc();
//        r.Sub(c);

//        BigFloat temp(n2);
//        temp.Mult(c);
//        n1.Add(temp);

//        temp.Copy(d2);
//        temp.Mult(c);
//        d1.Add(temp);

//        if (r.IsZero() || n1._exponent*c_log >= 32)
//        {
//            printf("%sstatic_cast<double>(%llu) / %llu", sign, n2.ToInteger(), d2.ToInteger());
//            break;
//        }

//        r.Invert();
//        c.Copy(r).Trunc();
//        r.Sub(c);

//        temp.Copy(n1);
//        temp.Mult(c);
//        n2.Add(temp);

//        temp.Copy(d1);
//        temp.Mult(c);
//        d2.Add(temp);

//        if (r.IsZero() || n2._exponent*c_log >= 32)
//        {
//            printf("%sstatic_cast<double>(%llu) / %llu", sign, n1.ToInteger(), d1.ToInteger());
//            break;
//        }
//    }

//    if (0 < x && x < 32)
//    {
//        printf(" * 0x%llx", ((u8)1)<<x);
//    }
//    else if (0 > x && x > -32)
//    {
//        printf(" / 0x%llx", ((u8)1)<<(-x));
//    }
//    else
//    {
//        printf(" * PowerOfTwo(%d)", x);
//    }

//    printf(")");

//}


//void BigFloat::PrintDouble() const
//{
//    double x = ToDouble();
//    printf("%.19g", x);
//}


//// use continued fractions to convert this number to a ratio
//void BigFloat::ToFraction(BigFloat& num, BigFloat& denom, int iter) const
//{
//    BigFloat remainder(*this);
//    remainder._isNegative = false;
//    num.Copy(*this);
//    num.Trunc();
//    remainder.Sub(num);
//    if (remainder.IsZero() ||
//        remainder._exponent < -c_digits/2 ||
//        iter>=0)
//    {
//        denom.FromInteger(1);
//    }
//    else
//    {
//        remainder.Invert();
//        BigFloat subDenom;
//        remainder.ToFraction(denom, subDenom, iter-1);
//        num.Mult(denom);
//        num.Add(subDenom);
//    }
//    num._isNegative = this->_isNegative;
//}


//void BigFloat::Print() const
//{
//    printf("exp=%lld, %c ", _exponent, _isNegative ? '-' : '+');
//    for (int iPos=0; iPos < _length; ++iPos)
//        printf("%x ", _d[iPos]);
//    printf("\n");
//}


//void BigFloat::PrintHex() const
//{
//    if (_isNegative)
//        printf("-");
//    if (IsSpecial())
//    {
//        if (IsZero())
//            printf("0");
//        else if (IsInf())
//            printf("inf");
//        else if (IsNaN())
//            printf("NaN");
//    }
//    else if (_exponent+1 > _length || _exponent < 0)
//    {
//        printf("0x");
//        for (int i=0; i<_length; ++i)
//        {
//            if (i==0)
//            {
//                printf("%lx", _d[0]);
//            }
//            else
//            {
//                if (i==1)
//                    printf(".");
//                printf("%.8lx", _d[i]);
//            }
//        }
//        printf(":e%lld", _exponent);
//    }
//    else
//    {
//        printf("0x");
//        for (int i=0; i<_length; ++i)
//        {
//            if (i==0)
//            {
//                printf("%lx", _d[0]);
//            }
//            else
//            {
//                if (i == _exponent+1)
//                    printf(".");
//                printf("%.8lx", _d[i]);
//            }
//        }
//    }
//}


//BigFloat& BigFloat::Copy(const BigFloat& n)
//{
//    _exponent = n._exponent;
//    _isNegative = n._isNegative;
//    _length = n._length;
//    for (int iPos=_length; iPos--; )
//        _d[iPos] = n._d[iPos];
//    return *this;
//}


//BigFloat& BigFloat::Negate()
//{
//    _isNegative = !_isNegative;
//    return *this;
//}


//// -1 if |this|<|n|, 0 if |this|==|n|, 1 if |this|>|n|
//int BigFloat::CompareAbsolute(const BigFloat& n) const
//{
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsInf())
//            return n.IsInf() ? 0 : 1;
//        else if (n.IsInf())
//            return -1;
//        else if (IsZero())
//            return n.IsZero() ? 0 : -1;
//        else if (n.IsZero())
//            return 1;
//        else
//            return -1;
//    }
//    else if (_exponent > n._exponent)
//        return 1;
//    else if (_exponent < n._exponent)
//        return -1;
//    else // n._exponent == _exponent
//    {
//        u2 length = _length;
//        if (n._length < length)
//            length = n._length;
//        for (int iPos = 0;  iPos < length;  ++iPos)
//        {
//            u4 bd = n._d[iPos];
//            u4 ad = _d[iPos];
//            if (ad > bd)
//                return 1;
//            else if (ad < bd)
//                return -1;
//        }
//        if (_length > n._length)
//            return 1;
//        else if (_length < n._length)
//            return -1;
//    }
//    return 0;
//}

//// -1 if this<n, 0 if this==n, 1 if this>n
//int BigFloat::Compare(const BigFloat& n) const
//{
//    if (_isNegative != n._isNegative)
//        return _isNegative ? -1 : 1;
//    else if (_isNegative)
//        return -CompareAbsolute(n);
//    else
//        return CompareAbsolute(n);
//}


//BigFloat& BigFloat::Trunc()
//{
//    if (_exponent + 1 - _length >= 0)
//        ;
//    else if (_exponent < 0)
//        Zero();
//    else
//        _length = static_cast<u2>(_exponent + 1);

//    return *this;
//}

//// lots of operations want to round to c_digits digits of precision
//BigFloat& BigFloat::Round(bool carry, s8 previousDigit)
//{
//    s8 iPos;
//    bool round;

//    ASSERT(previousDigit == 0 || _length == c_digits);

//    if (-previousDigit > (s8)c_range/2)
//    {
//        iPos = c_digits-1;
//        round = true;
//        previousDigit += c_range;
//        while (round && iPos >= 0)
//        {
//            s8 sum = (u8)_d[iPos] - round;
//            round = (sum < 0);
//            if (round)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            ASSERT(carry);
//            carry = false;
//        }
//    }

//    if (carry)
//    {
//        bool round = false;
//        // need to shift all digits over by one and add a top digit
//        if (_length == c_digits)
//        {
//            // First, round bottom digit up if needed.
//            // Since the top digit carried already, it won't
//            // carry again.
//            iPos = c_digits-1;
//            round = (_d[iPos] > c_range/2 ||
//                     (_d[iPos] == c_range/2 && previousDigit >= 0));
//            iPos--;
//            while (round && iPos >= 0)
//            {
//                u8 sum = (u8)_d[iPos] + round;
//                round = (sum >= c_range);
//                if (round)
//                    sum -= c_range;
//                _d[iPos] = (u4)sum;
//                iPos--;
//            }
//        }
//        else
//        {
//            ++_length;
//        }
        
//        // now shift everyone over by one.
//        for (iPos = _length;  iPos-- > 1; )
//            _d[iPos] = _d[iPos-1];
        
//        // add the carry as the top new digit.
//        _d[0] = (u8)carry + (u8)round;
        
//        // increase the exponent by one.
//        if (_exponent == c_maxExponent)
//            return _isNegative ? NInf() : Inf();
//        ++_exponent;
//    }
//    else if (previousDigit >= (s8)c_range/2)
//    {
//        ASSERT(_length == c_digits);
        
//        // post-round only: 950+95 = 1000 not 1100 to 2 digits
//        round = true;
//        iPos = c_digits-1;
//        while (round && iPos >= 0)
//        {
//            u8 sum = (u8)_d[iPos] + round;
//            round = (sum >= c_range);
//            if (round)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            _length = 1;
//            _d[0] = 1;
//            if (_exponent == c_maxExponent)
//            {
//                return _isNegative ? NInf() : Inf();
//            }
//            ++_exponent;
//        }
//    }
    
//    // remove trailing zeros
//    if (_length)
//    {
//        while (_d[_length-1] == 0)
//        {
//            if (--_length == 0)
//            {
//                Zero(_isNegative);
//            }
//        }
//    }
    
//    // final postchecks
//    if (_exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (_exponent > c_maxExponent)
//    {
//        return Inf(_isNegative);
//    }

//    return *this;
//}


//// round up, no carry, 0 or positive previous digit
//BigFloat& BigFloat::Round(u8 previousDigit)
//{
//    s8 iPos;
//    bool round;

//    ASSERT(previousDigit == 0 || _length == c_digits);

//    if (previousDigit >= c_range/2)
//    {
//        ASSERT(_length == c_digits);
        
//        // post-round only: 950+95 = 1000 not 1100 to 2 digits
//        round = true;
//        iPos = c_digits-1;
//        while (round && iPos >= 0)
//        {
//            u8 sum = (u8)_d[iPos] + round;
//            round = (sum >= c_range);
//            if (round)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            _length = 1;
//            _d[0] = 1;
//            if (_exponent == c_maxExponent)
//            {
//                return _isNegative ? NInf() : Inf();
//            }
//            ++_exponent;
//        }
//    }
    
//    // remove trailing zeros
//    while (_length && _d[_length-1] == 0)
//    {
//        if (--_length == 0)
//		{
//			Zero(_isNegative);
//        }
//    }

//    // final postchecks
//    if (_exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (_exponent > c_maxExponent)
//    {
//        return Inf(_isNegative);
//    }

//    return *this;
//}


//// Ye gods, how could addition possibly be so complicated?
//BigFloat& BigFloat::AddOrSubtract(const BigFloat& n, bool minus)
//{
//    BigFloat m(*this);  // cache of this

//    if (IsSpecial() || n.IsSpecial())
//    {
//        BigFloat n2(n);
//        if (minus)
//            n2.Negate();

//        if (n2.IsZero())
//            return *this;
//        else if (IsZero())
//            return Copy(n2);
//        else if (IsNaN() || n.IsNaN())
//            return NaN();
//        else if (IsPInf())
//            return n2.IsNInf() ? NaN() : *this;
//        else if (IsNInf())
//            return n2.IsPInf() ? NaN() : *this;
//        else if (n2.IsPInf())
//            return PInf();
//        else if (n2.IsNInf())
//            return NInf();
//        else
//            ASSERT(false);
//    }
    
//    // make sure the absolute largest number of (a,b) is in a
//    const BigFloat* a = &m;
//    const BigFloat* b = &n;
//    bool signA = a->_isNegative;
//    bool signB = (b->_isNegative != minus);

//    // make sure a has the bigger absolute value
//    if (CompareAbsolute(n) < 0)
//    {
//        const BigFloat* temp = b;
//        b = a;
//        a = temp;
//        bool tempSign = signB;
//        signB = signA;
//        signA = tempSign;
//    }
//    bool add = (signA == signB);
//    _isNegative = signA;

//    // actually do the addition now
//    s8 delta = a->_exponent - b->_exponent;
//    if (delta > c_digits+1)
//    {
//        // b is too small to matter, just copy a
//        if (this != a)
//            Copy(*a);
//        _isNegative = signA;
//        return *this;
//    }
        
//    s8 previousDigit = 0;
    
//    _exponent = a->_exponent;
//    // do we need to carry one to the next digit?
//    bool carry = false;
    
//    // assume subtraction shifts the set of significant digits down by one?
//    bool shift = false;
    
//    // these digit positions are relative to the result assuming no carry
//    s8 iPos; // which digit position we are looking at right now
//    s8 leastA = a->_length; // least significant A position + 1
//    s8 leastB = b->_length + delta; // least signficant B position + 1
        
//    if (leastA > leastB)
//    {
//        iPos = leastA-1;
//        _length = static_cast<u2>(leastA);
        
//        // copy low-order digits from a
//        while (iPos >= leastB)
//        {
//            _d[iPos] = a->_d[iPos];
//            iPos--;
//        }
//    }
//    else
//    {
//        // assume we will have digits 1..digits not 0..digits-1.
//        if (!add && leastB > c_digits && a->_d[0]==1)
//        {
//            shift = true;
//            _exponent--;
//            leastB--;
//            leastA--;
//            delta--;
//        }
        
//        // copy low-order digits from b
//        s8 leastBUsed = leastB;
//        if (leastB > c_digits)
//        {
//            leastBUsed = c_digits;
//        }
        
//        iPos = leastBUsed-1;
//        _length = static_cast<u2>(leastBUsed);
        
//        if (iPos >= leastA) // either > or ==, we know not <
//        {
//            s8 stopB = delta > leastA ? delta : leastA;
//            if (add)
//            {
//                while (iPos >= stopB)
//                {
//                    _d[iPos] = b->_d[iPos-delta] + carry;
//                    if (carry && _d[iPos] != 0)
//                        carry = false;
//                    iPos--;
//                }
//            }
//            else
//            {
//                while (iPos >= stopB)
//                {
//					// the low-order digit is nonzero, and carry==false, so it is going to be in range.
//					// all the other digits might be zero, but they have carry==true, so they will also be in range.
//                    _d[iPos] = static_cast<u4>(c_range - b->_d[iPos-delta] - carry);
//                    carry = true;
//                    iPos--;
//                }
//            }
            
//            if (stopB > leastA)
//            {
//                // the biggest b is smaller than the smallest a
//                // fill in the digits in the gap
//                if (carry)
//                {
//                    if (add)
//                    {
//                        while (iPos >= leastA)
//                        {
//                            _d[iPos] = carry;
//                            carry = false;
//                            iPos--;
//                        }
//                    }
//                    else
//                    {
//                        while (iPos >= leastA)
//                        {
//                            _d[iPos] = c_range-1;
//                            iPos--;
//                        }
//                    }
//                }
//                else
//                {
//                    while (iPos >= leastA)
//                    {
//                        _d[iPos] = 0;
//                        iPos--;
//                    }
//                }
//            }
//        }
        
//        ASSERT(iPos == leastA-1);
//    }
        
    
//    if (add)
//    {
//        // do the sum
//        while (iPos-delta >= 0)
//        {
//            u8 sum = (u8)a->_d[iPos] + (u8)b->_d[iPos-delta] + carry;
//            carry = (sum >= c_range);
//            if (carry)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // ran out of digits, finish copying the rest of the digits
//        while (iPos >= 0)
//        {
//            u8 sum = (u8)a->_d[iPos] + carry;
//            carry = (sum >= c_range);
//            if (carry)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }

//        if (leastB >= c_digits+1)
//        {
//            previousDigit = b->_d[c_digits-delta];
//        }
//    }
//    else  // subtract, not add
//    {
//        // do the subtraction
//        while (iPos-delta >= 0)
//        {
//            s8 sum = (u8)a->_d[iPos+shift] - (u8)b->_d[iPos-delta] - carry;
//            carry = (sum < 0);
//            if (carry)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // ran out of digits in b, finish the addition
//        while (iPos >= 0)
//        {
//            s8 sum = (u8)a->_d[iPos+shift] - carry;
//            carry = (sum < 0);
//            if (carry)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // remember what the first dropped digit would have been
//        if (leastB >= c_digits+1)
//        {
//            previousDigit = -(s8)b->_d[c_digits-delta];
//            if (leastB > c_digits+1)
//                --previousDigit;
//        }
        
//        if (shift)
//        {
//            // we pre-carried, convert lack of negative carry to
//            // a positive carry
//            carry = !carry;
//        }
//        else
//        {
//            // |a|>|b|, so there should be no way to get a final carry
//            ASSERT(!carry);
//        }
        
//        if (!carry)
//        {
//			u2 pos;
//            // find out how many leading digits are zero
//            for (pos = 0;  pos < _length; ++pos)
//                if (_d[pos] > 0)
//                    break;
//            u2 adjust = pos;
            
//            // adjust the exponent and promote remaining nonzero digits
//            if (adjust == _length || _exponent - adjust < -c_maxExponent)
//            {
//                return Zero(_isNegative);
//            }
//            else if (adjust > 0)
//            {
//                _exponent -= adjust;
//                _length -= adjust;
//                for (pos = 0;  pos < _length;  pos++)
//                    _d[pos] = _d[pos+adjust];
//            }
//        }
//    }

//    return Round(carry, previousDigit);
//}



//// Either do the multiplication, or raise an exception.
//BigFloat& BigFloat::Mult(const BigFloat& n)
//{
//    // actually exponent will be exponent or exponent+1
//    s8 exponent = _exponent + n._exponent + 1;
//    _isNegative ^= n._isNegative;

//    // do rough prechecks
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsNaN() || n.IsNaN())
//            return NaN();
//        else if (IsZero() || n.IsZero())
//            return Zero(_isNegative);
//        else
//            return Inf(_isNegative);
//    }
//    else if (exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (exponent > c_maxExponent+1)
//    {
//        return Inf(_isNegative);
//    }
    
//    u8 length = _length + n._length;
//    if (length > c_digits)
//    {
//        length = c_digits+3;
//    }

//    u8 temp[2*c_digits];
//    for (int i=0; i<length; ++i)
//        temp[i] = 0;

//    // multiplications
//    for (int i=_length; i--; )
//    {
//        u8 result;
//        u8 start = n._length;
//        if (n._length > length - i)
//        {
//            start = length - i;
//                }
//        for (int j=start; j--; )
//        {
//            result = (u8)_d[i] * (u8)n._d[j];
//            temp[i + j + 1] += result & (c_range-1);
//            temp[i + j] += result >> c_log;
//        }
//    }

//    // carries
//    for (u8 i=length; i-- > 1; )
//    {
//        temp[i-1] += (temp[i] >> c_log);
//        temp[i] &= (c_range-1);
//    }
//    ASSERT(length > 0 && temp[0] < c_range);

//    u8* result = temp;
//    if (temp[0] == 0)
//    {
//        result = &temp[1];
//        --exponent;
//        --length;
//    }
//    u8 previousDigit = 0;
//    if (length > c_digits)
//    {
//        length = c_digits;
//        previousDigit = result[length];
//    }

//    // record the result
//    for (int i=0; i<length; ++i)
//        _d[i] = static_cast<u4>(result[i]);
//    _length = static_cast<u2>(length);
//    _exponent = exponent;
    
//    return Round(previousDigit);
//}


//BigFloat& BigFloat::Div(const BigFloat& n)
//{
//    // actually exponent will be exponent or exponent+1
//    s8 exponent = _exponent - n._exponent;

//    _isNegative = _isNegative ^ n._isNegative;

//    // do rough prechecks
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsNaN() || n.IsNaN() || n.IsZero())
//            return NaN();
//        else if (IsZero() || n.IsInf())
//            return Zero(_isNegative);
//        else
//            return Inf(_isNegative);
//    }
//    else if (exponent+1 < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (exponent > c_maxExponent+1)
//    {
//        return Inf(_isNegative);
//    }

//    ASSERT((c_log/2)*2 == c_log);
//	ASSERT(_length > 0);

//    // approximate result to only to this many half-digits, then round
//	// need at least one full previous digit, and one full digit of zeros in front
//    static const u8 limit = 2*(c_digits+2);

//    // numerator
//	static const u8 numerLimit = 2 * limit + 2;
//    s8 t[numerLimit];
//    for (int i=0; i<_length; ++i)
//    {
//        t[2*i] = _d[i]>>(c_log/2);
//        t[2*i+1] = _d[i]&((((s8)1)<<(c_log/2))-1);
//    }
//    for (int i=2*_length; i<numerLimit; ++i)
//        t[i] = 0;

//    // denominator (overallocate; only 2*n._length of this will be used)
//    s8 d[limit];
//    s8 ad = n._d[0];
//    s8 length = 2*n._length;
//    s8 shift = (ad < (1<<(c_log/2))) ? 1 : 0;
//    if (shift)
//    {
//        --length;
//        d[0] = ad;
//        ad <<= c_log/2;
//        for (int i=2; i<2*n._length; i+=2)
//        {
//            d[i-1] = n._d[i/2] >> (c_log/2);
//            d[i] = n._d[i/2] & ((1<<(c_log/2))-1);
//        }
//        if (n._length > 1)
//        {
//            ad += d[1];
//        }
//    }
//    else
//    {
//        for (int i=0; i<2*n._length; i+=2)
//        {
//            d[i] = n._d[i/2] >> (c_log/2);
//            d[i+1] = n._d[i/2] & ((1<<(c_log/2))-1);
//        }
//    }

//    // divide
//    s8 r[limit];
//    s8 an = t[0];
//    for (int i=0; i<limit; ++i)
//    {
//        an <<= c_log/2;
//        an += t[i+1];
//        s8 q = an/ad;
//        if (q)
//        {
//            // first 2 digits of d are already accounted for by ad
//            if (2 < length)
//            {
//                t[i+2] -= q*d[2];
//                for (int j=length; j-- > 3; )
//                {
//                    s8 p = t[i+j] - q*d[j];
//                    ASSERT(p < (((s8)1)<<(3*c_log/2)) && p > -(((s8)1)<<(3*c_log/2)));
//                    t[i+j-1] += p >> (c_log/2);
//                    t[i+j] = p & ((1<<(c_log/2))-1);
//                }
//            }
//        }
//        ASSERT(ad*q + (an%ad) == an);
//        an %= ad;
        
//        r[i] = q;
//    }

//    // carry
//    for (int i=limit; i-- > 1;)
//    {
//        s8 carry = r[i] >> (c_log/2);
//        if (carry != 0)
//        {
//            r[i] &= (1<<(c_log/2)) - 1;
//            r[i-1] += carry;
//        }
//    }
//    ASSERT((r[0] >> (c_log/2)) == 0);

//    s8* pr = r;
//    while (pr[0] == 0)
//    {
//        ++pr;
//        --shift;
//    }

//    // combine the half-digits and round
//    s8 previousDigit = 0;
//    if (!(shift & 1))
//    {
//        _d[0] = static_cast<u4>(pr[0]);
//        for (int i=1; i<c_digits; ++i)
//        {
//            _d[i] = static_cast<u4>((pr[2*i-1] << (c_log/2)) + pr[2*i]);
//        }
//        previousDigit = (pr[2*c_digits-1] << (c_log/2)) + pr[2*c_digits];
//    }
//    else
//    {
//        for (int i=0; i<c_digits; ++i)
//        {
//            _d[i] = static_cast<u4>((pr[2*i] << (c_log/2)) + pr[2*i+1]);
//        }
//        previousDigit = (pr[2*c_digits] << (c_log/2)) + pr[2*c_digits+1];
//    }
//    _length = c_digits;
//    _exponent = exponent + (shift>>1);
//    return Round(previousDigit);
//}


//int BigFloat::Compare(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Compare(b);
//}


//BigFloat& BigFloat::Add(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Add(b);
//}


//BigFloat& BigFloat::Sub(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Sub(b);
//}


//BigFloat& BigFloat::Mult(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Mult(b);
//}


//BigFloat& BigFloat::Div(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Div(b);
//}


//BigFloat& BigFloat::Invert()
//{
//    BigFloat i(1);
//    i.Div(*this);
//    return Copy(i);
//}


//BigFloat& BigFloat::Sqrt()
//{
//    if (_isNegative)
//    {
//        return NaN();
//    }
//    else if (IsSpecial())
//    {
//        return *this;
//    }

//    static const int c_intermediate = 2*c_digits+2;
//    static const int c_remainder = 2*c_intermediate;

//    // remainder stores the original value in half-digits
//    s8 r[c_remainder];

//    // answer is also in half-digits
//    s8 abuf[c_intermediate+1];
//    bool odd = (_exponent & 1);

//    s8* a = abuf;
//    if (!odd)
//    {
//        abuf[0] = 0;
//        a = &abuf[1];
//    }
    
//    for (int i=0; i<_length; ++i)
//    {
//        r[2*i] = _d[i]>>(c_log/2);
//        r[2*i+1] = _d[i] & ((1<<(c_log/2))-1);
//    }
//    for (int i=_length*2; i<c_remainder; ++i)
//        r[i] = 0;
//    for (int i=0; i<c_intermediate; ++i)
//        a[i] = 0;

//    // Find the first two half-digits of the root using integer arithmetic.
//    //
//    // This method sometimes gives an answer one too high, which is not
//    // important usually, but would cause the answer to not fit in two
//    // half-digits in the case of 1<<(2*c_log)-1.  I chose the starting
//    // root so that that doesn't happen.
//    u8 first =
//        (((u8)r[0])<<(3*c_log/2)) +
//        (((u8)r[1])<<(2*c_log/2)) +
//        (((u8)r[2])<<(1*c_log/2)) +
//        ((u8)r[3]);
//    r[0] = 0;
//    r[1] = 0;
//    r[2] = 0;
//    r[3] = 0;
//    u8 root = ((u8)1) << c_log;
//    for (;;)
//    {
//        u8 oldroot = root;
//        root = (root + (first/root)) / 2;
//        if (oldroot == root || oldroot == root+1)
//            break;
//    }
//    a[0] = root >> (c_log/2);
//    ASSERT(a[0] >= 1);
//    ASSERT(a[0] < (1<<(c_log/2)));
//    a[1] = root & ((1<<(c_log/2))-1);

//    u8 square = root*root;
//    s8 remainder = (s8)(first - root*root);
//    ASSERT(remainder < ((s8)2)<<c_log);
//    ASSERT(remainder > ((s8)-2)<<c_log);
//    s8 denominator = -2*(s8)root;
//    for (int i=2; i<c_intermediate; i++)
//    {
//        // new approximate remainder
//        remainder <<= c_log/2;
//        remainder += r[i+2];
//        r[i+2] = 0;

//        // new approximate delta
//        s8 delta = remainder/denominator;

//        // adjust the remainder
//        remainder -= denominator * delta;
//        ASSERT(remainder < ((s8)2)<<c_log);
//        ASSERT(remainder > ((s8)-2)<<c_log);
//        ASSERT(r[i] == 0);

//        if (2*i+1 < c_intermediate)
//        {
//            square = delta*delta;
//            r[2*i+1] -= square;
//        }
//        int j = (2*i+1 >= c_intermediate) ? c_intermediate-i : i;
//        if (j > 2)
//        {
//            while (j-- > 3)
//            {
//                s8 p = r[i+j+1] + 2 * a[j] * delta;
//                r[i+j+1] = p & ((1<<(c_log/2))-1);
//                r[i+j] += p >> (c_log/2);
//            }
//            r[i+j+1] += 2 * a[j] * delta;
//        }

//        a[i] = -delta;
//    }

//    // carries, make all the digits between 0 and 1<<(c_log/2)
//    for (int i=c_intermediate; i-- > 1; )
//    {
//        s8 top = a[i] >> c_log/2;
//        a[i-1] += top;
//        a[i] -= top << c_log/2;
//    }

//    // combine the half digits into the final answer
//    _exponent >>= 1;
//    _length = c_digits;
//    s8 previousDigit = (a[2*c_digits] << (c_log/2)) + a[2*c_digits+1];
//	s8 x = (abuf[0] << (c_log/2)) + abuf[1];
//    ASSERT(x >= 1);
//    ASSERT(x < (((s8)1)<<c_log));
//	_d[0] = static_cast<u4>(x);
//    for (int i=1; i<c_digits; ++i)
//    {
//        x = (abuf[2*i] << c_log/2) + abuf[2*i+1];
//        ASSERT(x >= 0);
//        ASSERT(x < (((s8)1)<<c_log));
//		_d[i] = static_cast<u4>(x);
//    }
//    return Round(previousDigit);
//}


//const BigFloat& BigFloat::Pi()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_pi;
//}


//const BigFloat& BigFloat::E()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_e;
//}


//const BigFloat& BigFloat::ConstZero()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_zero;
//}


//// given x in [-pi/4, pi/4], replace it with sin(x)
//BigFloat& BigFloat::PartialSin()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x6(x4);
//    x6.Mult(x2);

//    BigFloat x8(x6);
//    x8.Mult(x2);

//    BigFloat sin(0);
//    BigFloat oldSin(0);
//    BigFloat power(*this);
//    s8 i = 7;
//    ASSERT(BigFloatCache::_overFactLen < (1<<(63/6)));
//    for (; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat sum(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        BigFloat term(x2);
//        term.Mult(i*(i-1)*(i-2)*(i-3));
//        sum.Sub(term);

//        term.Copy(x4);
//        term.Mult(i*(i-1));
//        sum.Add(term);

//        sum.Sub(x6);
            

//        sum.Mult(BigFloatCache::_overFact[i]);
//        sum.Mult(power);
//        sin.Add(sum);

//        if (sin.Compare(oldSin) == 0)
//            break;

//        oldSin.Copy(sin);
//        power.Mult(x8);
//    }

//    ASSERT(i < BigFloatCache::_overFactLen);
//    return Copy(sin);
//}



//// given x in [-pi/4, pi/4], replace it with cos(x)
//BigFloat& BigFloat::PartialCos()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x6(x4);
//    x6.Mult(x2);

//    BigFloat x8(x6);
//    x8.Mult(x2);

//    BigFloat cos(0);
//    BigFloat oldCos(0);
//    BigFloat power(1);
//    s8 i = 6;
//    ASSERT(BigFloatCache::_overFactLen < (1<<(63/6)));
//    for (; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat sum(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        BigFloat term(x2);
//        term.Mult(i*(i-1)*(i-2)*(i-3));
//        sum.Sub(term);

//        term.Copy(x4);
//        term.Mult(i*(i-1));
//        sum.Add(term);

//        sum.Sub(x6);
            
//        sum.Mult(BigFloatCache::_overFact[i]);
//        sum.Mult(power);
//        cos.Add(sum);

//        if (cos.Compare(oldCos) == 0)
//            break;

//        oldCos.Copy(cos);
//        power.Mult(x8);
//    }

//    ASSERT(i < BigFloatCache::_overFactLen);
//    return Copy(cos);
//}


//// Lots of functions would prefer inputs only in [-pi/4, pi/4).
//s8 BigFloat::Quadrant()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat div2pi(*this);

//    // modulo 2pi, don't care about the rest
//    div2pi.Mult(BigFloatCache::_overTwoPi);
//    BigFloat extra(div2pi);
//    extra.Trunc();
//    div2pi.Sub(extra);
//    extra.Mult(BigFloatCache::_twoPi);
//    Sub(extra);

//    div2pi.Mult(8);
//    div2pi.Trunc();
//    s8 quadrant = div2pi.ToInteger();
//    switch (quadrant)
//    {
//    case 7:
//        Sub(BigFloatCache::_twoPi);
//        break;
//    case 6:
//    case 5:
//        Sub(BigFloatCache::_threePiOverTwo);
//        break;
//    case 4:
//    case 3:
//        Sub(BigFloatCache::_pi);
//        break;
//    case 2:
//    case 1:
//        Sub(BigFloatCache::_piOverTwo);
//        break;
//    case 0:
//        if (_isNegative)
//            quadrant += 7;
//        break;
//    case -1:
//    case -2:
//        quadrant += 7;
//        Add(BigFloatCache::_piOverTwo);
//        break;
//    case -3:
//    case -4:
//        quadrant += 7;
//        Add(BigFloatCache::_pi);
//        break;
//    case -5:
//    case -6:
//        quadrant += 7;
//        Add(BigFloatCache::_threePiOverTwo);
//        break;
//    case -7:
//        quadrant += 7;
//        Add(BigFloatCache::_twoPi);
//        break;
//    default:
//        ASSERT(false, "bad quadrant %d", quadrant);
//    }

//    BigFloat limit(3217);
//    limit.Div(4096);    // limit is a little bigger than pi/4, there are sometimes rounding issues
//    ASSERT(CompareAbsolute(limit) <= 0);
//    return quadrant;
//}


//BigFloat& BigFloat::Sin()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(0);
//        else
//            return NaN();
//    }

//    s8 quadrant = Quadrant();
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//        PartialSin();
//        break;
//    case 1:
//    case 2:
//        PartialCos();
//        break;
//    case 3:
//    case 4:
//        PartialSin();
//        Negate();
//        break;
//    case 5:
//    case 6:
//        PartialCos();
//        Negate();
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Csc()
//{
//    Sin();
//    Invert();
//    return *this;
//}


//BigFloat& BigFloat::Cos()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(1);
//        else
//            return NaN();
//    }
    
//    s8 quadrant = Quadrant();
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//        PartialCos();
//        break;
//    case 1:
//    case 2:
//        PartialSin();
//        Negate();
//        break;
//    case 3:
//    case 4:
//        PartialCos();
//        Negate();
//        break;
//    case 5:
//    case 6:
//        PartialSin();
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Sec()
//{
//    Cos();
//    Invert();
//    return *this;
//}


//BigFloat& BigFloat::Tan()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(0);
//        else
//            return NaN();
//    }

//    s8 quadrant = Quadrant();
//    BigFloat sin2;
//    BigFloat cos;
//    BigFloat cos2;
//    BigFloat sin;
//    bool negate = false;
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//    case 3:
//    case 4:
//        // sine is more accurate, derive cosine
//        PartialSin();
//        sin2.Copy(*this);
//        sin2.Mult(sin2);
//        cos.Copy(1);
//        cos.Sub(sin2);
//        cos.Sqrt();
//        Div(cos);
//        break;
//    case 2:
//    case 6:
//        negate = true;
//    case 1:
//    case 5:
//        // cosine is more accurate, derive sine
//        PartialCos();
//        cos2.Copy(*this);
//        cos2.Mult(cos2);
//        sin.Copy(1);
//        sin.Sub(cos2);
//        sin.Sqrt();
//        if (negate)
//            sin._isNegative = !sin._isNegative;
//        Div(sin);
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Exp()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    if (IsSpecial())
//    {
//        if (IsNaN())
//            return *this;
//        else if (IsZero())
//            return Copy(1);
//        else if (IsPInf())
//            return *this;
//        else if (IsNInf())
//            return Zero();
//    }

//    // only calculate e on [-1/16, 1/16].
//    // For the rest use multiples of e^^-16.
//    BigFloat whole(*this);
//    whole.Mult(1<<(-BigFloatCache::_ePowerNeg-1));
//    whole.Trunc();
//    whole.Div(1<<(-BigFloatCache::_ePowerNeg-1));
//    Sub(whole);

//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x8(x4);
//    x8.Mult(x4);

//    // sum the series (8 terms at a time, to reduce bignum multiplications)
//    // calculating only the fractional part mod 1/16
//    BigFloat exp(1);
//    BigFloat oldExp(1);
//    BigFloat power(*this);
//    for (s8 i=8; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat even(i*(i-1)*(i-2)*(i-3)*(i-4));

//        BigFloat term(i);
//        term.Mult(x4);
//        even.Add(term);
//        even.Mult(x2);

//        even.Add(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5)*(i-6));

//        term.Copy(i*(i-1)*(i-2));
//        term.Mult(x4);
//        even.Add(term);

//        BigFloat odd(i*(i-1)*(i-2)*(i-3));

//        term.Copy(1);
//        term.Mult(x4);
//        odd.Add(term);
//        odd.Mult(x2);
        
//        odd.Add(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        term.Copy(i*(i-1));
//        term.Mult(x4);
//        odd.Add(term);

//        odd.Mult(*this);

//        odd.Add(even);
//        odd.Mult(BigFloatCache::_overFact[i]);
//        odd.Mult(power);

//        exp.Add(odd);

//        if (exp.Compare(oldExp) == 0)
//            break;

//        oldExp.Copy(exp);
//        power.Mult(x8);
        
//    }
 
//    // now handle the whole number multiple of 1/16
//    if (!whole._isNegative)
//    {
//        for (s8 i=0; i<whole._length; ++i)
//        {
//            for (s8 j=0; j<c_log; ++j)
//            {
//                if (whole._d[i] & (1<<j))
//                {
//                    s8 index = c_log*(whole._exponent-i)+j;
//                    if (index < BigFloatCache::_ePowerLen)
//                    {
//                        ASSERT(index > BigFloatCache::_ePowerNeg);
//                        exp.Mult(BigFloatCache::_ePower[index]);
//                    }
//                    else
//                        exp.Inf();
//                }
//            }
//        }
//    }
//    else if (whole._isNegative)
//    {
//        for (s8 i=0; i<whole._length; ++i)
//        {
//            for (s8 j=0; j<c_log; ++j)
//            {
//                if (whole._d[i] & (1<<j))
//                {
//                    s8 index = c_log*(whole._exponent-i)+j;
//                    if (index < BigFloatCache::_ePowerLen)
//                    {
//                        ASSERT(index > BigFloatCache::_ePowerNeg);
//                        exp.Mult(BigFloatCache::_eInvPower[index]);
//                    }
//                    else
//                        exp.Zero();
//                }
//            }
//        }
//    }
    
//    return Copy(exp);
//}


//BigFloat& BigFloat::ASin()
//{
//    BigFloat denom(*this);
//    denom.Mult(denom);
//    denom.Sub(1);
//    denom.Negate();
//    denom.Sqrt();
//    denom.Add(1);
//    Div(denom);
//    ATan();
//    return Mult(2);
//}


//BigFloat& BigFloat::ACos()
//{
//    BigFloat num(*this);
//    num.Mult(num);
//    num.Sub(1);
//    num.Negate();
//    num.Sqrt();
//    BigFloat denom(*this);
//    denom.Add(1);
//    num.Div(denom);
//    num.ATan();
//    num.Mult(2);
//    return Copy(num);
//}


//BigFloat& BigFloat::ATan()
//{
//    if (IsNaN())
//        return *this;

//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();

//    bool flip = (CompareAbsolute(1) > 0);
//    if (flip)
//    {
//        Invert();
//    }

//    bool reduce = false;

//    // tan(x) = 2x/(1+sqrt(1+xx))
//    static const int c_iter = 2;
//    for (int i=0; i<c_iter; ++i)
//    {
//        BigFloat denom(*this);
//        denom.Mult(denom);
//        denom.Add(1);
//        denom.Sqrt();
//        denom.Add(1);
//        Div(denom);
//    }

//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);
    
//    BigFloat aTan(0);
//    BigFloat oldATan(0);
//    BigFloat power(*this);
//    for (s8 i=1; ; i+=4)
//    {
//        BigFloat term(i);
//        term.Mult(x2);
//        term.Sub(i+2);
//        term.Mult(power);
//        term.Div(i*(i+2));
//        aTan.Sub(term);

//        if (aTan.Compare(oldATan) == 0)
//            break;

//        oldATan.Copy(aTan);
//        power.Mult(x4);
//    }
//    Copy(aTan);

//    // multiply the result by 1<<c_iter to make up for all those half-angle reductions earlier
//    Mult(1<<c_iter);
//    if (flip)
//    {
//        Negate();
//        if (_isNegative)
//        {
//            Sub(BigFloatCache::_piOverTwo);
//        }
//        else
//        {
//            Add(BigFloatCache::_piOverTwo);
//        }
//    }

//    return *this;
//}


//BigFloat& BigFloat::Ln()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
    
//    if (_isNegative)
//        return NaN();
//    else if (IsSpecial())
//    {
//        if (IsZero())
//            return NInf();
//        else if (IsInf())
//            return Inf();
//        else
//            return NaN();
//    }

//    // get a remainder in [1-delta, delta] for some small delta
//    BigFloat whole;
//    static const s8 minDigit = BigFloatCache::_ePowerNeg/c_log - 2;
//    static const s8 offset = minDigit * c_log;  // offset to keep i positive
//    if (_exponent >= 0)
//    {
//        s8 i=BigFloatCache::_ePowerNeg+1;
//        while (i < BigFloatCache::_ePowerLen && Compare(BigFloatCache::_ePower[i]) >= 0)
//            ++i;
//        whole._exponent = minDigit + (i-1-offset)/c_log;
//        whole._length = (u2)(whole._exponent - minDigit);
//        if (whole._length > c_digits)
//            whole._length = c_digits;
//        for (int j=0; j<whole._length; ++j)
//            whole._d[j] = 0;
//        while (i-- > BigFloatCache::_ePowerNeg+1)
//        {
//            if (Compare(BigFloatCache::_ePower[i]) >= 0)
//            {
//                Mult(BigFloatCache::_eInvPower[i]);
//                whole._d[whole._exponent - (i-offset)/c_log - minDigit] += (1 << ((i+offset)%c_log));
//            }
//        }
//    }
//    else
//    {
//        whole._isNegative = true;
//        s8 i=BigFloatCache::_ePowerNeg+1;
//        while (i < BigFloatCache::_ePowerLen && Compare(BigFloatCache::_eInvPower[i]) <= 0)
//            ++i;
//        whole._exponent = minDigit + (i-1-offset)/c_log;
//		s8 len = whole._exponent - minDigit;
//		ASSERT(len >= 0);
//		ASSERT(len < (u2)~0);
//        whole._length = static_cast<u2>(len);
//        if (whole._length > c_digits)
//            whole._length = c_digits;
//        for (int j=0; j<whole._length; ++j)
//            whole._d[j] = 0;
        
//        while (i-- > BigFloatCache::_ePowerNeg+1)
//        {
//            if (Compare(BigFloatCache::_eInvPower[i]) <= 0)
//            {
//                Mult(BigFloatCache::_ePower[i]);
//                whole._d[whole._exponent - (i-offset)/c_log - minDigit] += (1 << ((i+offset)%c_log));
//            }
//        }
//    }

//    // ok here goes
//    BigFloat x(*this);
//    x.Sub(1);
//    BigFloat x2(x);
//    x2.Mult(x);
//    BigFloat power(x);
//    BigFloat oldLn(1);
//    BigFloat ln(0);
//    for (s8 i=1; ; i+= 2)
//    {
//        BigFloat term(i);
//        term.Mult(x);
//        term.Sub(i+1);
//        term.Div(i*(i+1));
//        term.Mult(power);
//        ln.Sub(term);
//        if (oldLn.Compare(ln) == 0)
//            break;

//        oldLn.Copy(ln);
//        power.Mult(x2);
//    }
    
//    // add in whole
//    ln.Add(whole);
    
//    return Copy(ln);
//}


//BigFloat& BigFloat::Power(const BigFloat& n)
//{
//    Ln();
//    Mult(n);
//    return Exp();
//}


//BigFloat& BigFloat::Log(const BigFloat& n)
//{
//    BigFloat lnn(n);
//    lnn.Ln();

//    Ln();
//    Invert();
    
//    return Mult(lnn);
//}


//s8 BigFloat::RoundInteger(s8 value)
//{
//    u8 x;

//    // find if it is negative
//    bool negative = (value < 0);

//    x = (u8)(negative ? -value : value);

//    // find the exponent
//    int exp=0;
//    for (; exp<64; exp+=c_log)
//    {
//        if (x < ((u8)c_range)<<exp)
//            break;
//    }

//    // round to the nearest legal int
//    u8 precision = c_log*(c_digits-1);
//    if (exp > precision)
//    {
//        // add 1/2 so it rounds right
//        x += ((u8)1)<<(exp - precision - 1);

//        // truncate x to the legal precision
//        x >>= (exp - precision);
//        x <<= (exp - precision);
//    }

//    return negative ? -(s8)x : (s8)x;
//}


//void BigFloat::TestInteger(const BigFloat& n, s8 value)
//{
//    s8 x = RoundInteger(value);
//    ASSERT(n.ToInteger() == x, "n=%d, x=%d", (s4)n.ToInteger(), (s4)x);
//}


//void BigFloat::TestAdd(s8 x, s8 y)
//{
//    BigFloat bx(x);
//    BigFloat by(y);
//    BigFloat bz(bx);

//    TestInteger(bz.Add(by), x+y);
//    TestInteger(bz.Copy(bx).Sub(by), x-y);
//}


//void BigFloat::TestMult(s8 x, s8 ex, s8 y, s8 ey)
//{
//    s8 px = x<0 ? -x : x;
//    s8 py = y<0 ? -y : y;
//    ASSERT(px < (((s8)1)<<31));
//    ASSERT(py < (((s8)1)<<31));
//    s8 pz = RoundInteger(px*py);

//    BigFloat bx(x, ex);
//    BigFloat by(y, ey);
//    BigFloat bz(bx);

//    if (pz == 0)
//    {
//        bz.Mult(by);
//        ASSERT(bz.ToInteger() == 0);
//        return;
//    }

//    int i;
//    for (i=0; pz < (((u8)1)<<(64-c_log)) && i<64; ++i)
//    {
//        pz <<= c_log;
//    }
//    s8 exponent = ex+ey+(64/c_log - i - 1);

//    if (exponent > c_maxExponent)
//        return;  // doing the multiplication would have raised an exception

//    bz.Mult(by);
//    if (exponent < c_minExponent)
//    {
//        ASSERT(bz.CompareAbsolute(0) == 0);
//    }
//    else
//    {
//        ASSERT(bz.ToDigits() == pz);
//        ASSERT(bz.ToExponent() == exponent);
//        ASSERT(bz.IsNegative() == ((x<0) != (y<0)));
//    }
//}

//void BigFloat::TestInverse(s8 x, s8 ex)
//{
//    BigFloat bx(x, ex);

//    BigFloat bi(bx);
//    bi.Invert();

//    BigFloat bq(bi);
//    bq.Mult(bx);
//    if (bx.ToExponent() >= -c_maxExponent && bi.IsZero())
//        return;

//    bq.Sub(1);
//    ASSERT(bq.Compare(0) == 0 ||
//           bq.ToExponent() <= 1-c_digits);
//}


//void BigFloat::TestSqrt(s8 x, s8 ex)
//{
//    BigFloat bx(x, ex);

//    BigFloat br(bx);  // root
//    br.Sqrt();

//    // check (bx-br*br)/bx
//    BigFloat bd(br);  // delta
//    bd.Mult(bd);
//    bd.Sub(bx);
//    BigFloat bo(bx);  // ratio of delta to original
//    bo.Invert();
//    bo.Mult(bd);
//    ASSERT(bo.Compare(0) == 0 ||
//           bo.ToExponent() <= 1-c_digits);


//    // check (br-bx/br)/br
//    // this tests division in interesting ways too
//    bd.Copy(bx);  // delta
//    bd.Div(br);
//    bd.Sub(br);
//    bo.Copy(br);  // ratio of delta to original
//    bo.Invert();
//    bo.Mult(bd);
//    ASSERT(bo.Compare(0) == 0 ||
//           bo.ToExponent() <= 1-c_digits);
//}


//#ifdef BIGFLOAT_TEST
//void BigFloat::UnitTest()
//{
//    BigFloat b;
//    BigFloat zero;
//    ASSERT(b.Compare(zero) == 0);

//    // b is 1
//    b.FromInteger(1);
//    ASSERT(b.Compare(zero) == 1);
//    ASSERT(b.Compare(b) == 0);

//    // b is -1
//    b.FromInteger(-1);
//    ASSERT(b.Compare(zero) == -1);
//    ASSERT(b.Compare(b) == 0);
//    BigFloat c;

//    // c is -c_range
//    c.FromInteger(-(s8)c_range);
//    TestInteger(c, -(s8)c_range);

//    TestSqrt(2, 0);

//    static const u8 c_maxInt = ((u8)1) << (c_log * c_digits);

//    // check if inverse works
//    for (s8 ex=(s8)(c_minExponent-c_digits+1);
//         ex<=(s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            TestInverse(x, ex);
//            TestInverse(-x, ex);
//        }
//    }
//    printf("Inverse works\n");


//    // check if square root works
//    for (s8 ex = (s8)(c_minExponent-c_digits+1);
//         ex < (s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            TestSqrt(x, ex);
//        }
//    }
//    printf("Square root works\n");

//    // check if addition and subtraction work
//    u4 coef = 1;
//    for (int q=0; q<2*c_digits; ++q)
//    {
//        for (s8 x=0; x<coef*c_maxInt; x += coef)
//        {
//            for (s8 y=0; y<c_maxInt; ++y)
//            {
//                TestAdd(x,y);
//                TestAdd(x,-y);
//                TestAdd(-x,y);
//                TestAdd(-x,-y);
//            }
//        }
//        coef *= c_range;
//    }
//    printf("Addition and Subtraction work\n");

//    // check if multiplication works
//    TestMult(0, 0, 1, 0);
//    TestMult(-1, 0, 0, 0);
//    for (s8 ex=(s8)(c_minExponent-c_digits+1);
//         ex<=(s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            for (s8 ey=(s8)(c_minExponent-c_digits+1);
//                 ey<=(s8)(c_maxExponent-c_digits+1);
//                 ++ey)
//            {
//                for (s8 y=(((s8)1)<<(c_log*(c_digits-1))); y<c_maxInt; y++)
//                {
//                    TestMult(x, ex, y, ey);
//                    TestMult(x, ex, -y, ey);
//                    TestMult(-x, ex, y, ey);
//                    TestMult(-x, ex, -y, ey);
//                }
//            }
//        }
//    }
//    printf("Multiplication works\n");
//}

//int main(int argc, const char **argv)
//{
//    BigFloat* m[2];
//    m[0] = new BigFloat[3];
//    m[1] = new BigFloat[3];
//    m[0][1].Copy(1);
//    m[1][0].Copy(1);
//    m[1][1].Copy(1);
//    m[0][2].Copy(1);
//    m[1][2].Copy(2);
//    BigFloat::GaussianElimination(m, 2, 2);
//    printf("%d, %d\n", (s4)m[0][2].ToInteger(), (s4)m[1][2].ToInteger());


//    printf("%f\n", BigFloat::Pi().ToDouble());
    
//    try
//    {
//        BigFloat::UnitTest();
//    }
//    catch( const std::exception & ex )
//    {
//        fprintf(stderr, "%s\n", ex.what());
//    }

//    return 0;
//}
//#endif
