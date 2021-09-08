///*
// * Test file for verifying the Stanford C++ lib functionality.
// */

//#include <iostream>
//#include <iomanip>
//#include "bigfloat.h"
//#include "biginteger.h"
//#include "console.h"
//#include "error.h"
//#include "strlib.h"
//using namespace std;

//void testBigInteger();

//int mainBigInteger() {
//    cout << "Stanford C++ lib tester" << endl;
//    testBigInteger();
//    return 0;
//}

//bigint fact(int n) {
//    bigint result("1");
//    for (int i = 2; i <= n; i++) {
//        result *= BigInteger(integerToString(i));
//    }
//    return result;
//}

//void testBigInteger() {
//    {
//        BigInteger bi("-24");
//        cout << "abs = " << bi.abs() << endl;
//        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
//        bi = bi.abs();
//        cout << "pos: " << boolalpha << bi.isPositive() << ", neg: " << bi.isNegative() << ", nonneg: " << bi.isNonNegative() << endl;
//        cout << "pos: " << boolalpha << BigInteger::ZERO.isPositive() << ", neg: " << BigInteger::ZERO.isNegative() << ", nonneg: " << BigInteger::ZERO.isNonNegative() << endl;

//        BigInteger bi2("16");
//        cout << "gcd = " << bi.gcd(bi2) << endl << endl;

//        do {
//            cout << "int bi2 = " << bi2 << ", hex = " << bi2.toString(16) << ", oct = " << bi2.toString(8) << ", bin = " << bi2.toString(2) << endl;
//            bi2 *= BigInteger::TWO;
//        } while (bi2.isInt());

//        do {
//            cout << "long bi2 = " << bi2 << endl;
//            bi2 *= BigInteger::TEN;
//        } while (bi2.isLong());

//        cout << "max = " << bi.max(bi2) << ", min = " << bi.min(bi2) << endl;

//        cout << "modPow = " << BigInteger::TWO.modPow(10, 100) << endl;

//        cout << "pow = " << endl;
//        for (int i = 0; i < 20; i++) {
//            cout << "2 ^ " << i << " = " << BigInteger::TWO.pow(i) << endl;
//        }

//        // radix stuff
//        BigInteger biHex("0xff00", 16);
//        cout << "bihex = " << biHex << ", hex = " << biHex.toString(16) << endl;
//        BigInteger biBin("0b101010", 2);
//        cout << "bibin = " << biBin << ", bin = " << biBin.toString(2) << endl;

//        BigInteger biUnary("1111111", 1);
//        cout << "biunary = " << biUnary << ", unary " << biUnary.toString(1) << endl;

//        for (BigInteger bi = 1; bi <= 100; bi++) {
//            cout << "int bi = " << bi << ", hex = " << bi.toString(16) << ", oct = " << bi.toString(8) << ", bin = " << bi.toString(2) << endl;
//        }

//        // various int/long/double x BigInteger binary operations
//        cout << "bitwise stuff:" << endl;
//        BigInteger binary1("10100011", 2);
//        BigInteger binary2("01110110", 2);
//        cout << "a     = " << binary1.toString(2) << endl;
//        cout << "b     = " << setw(8) << binary2.toString(2) << endl;
//        cout << "a & b = " << setw(8) << (binary1 & binary2).toString(2) << endl;
//        cout << "a | b = " << setw(8) << (binary1 | binary2).toString(2) << endl;
//        cout << "a ^ b = " << setw(8) << (binary1 ^ binary2).toString(2) << endl;
//        cout << "~a    = " << setw(8) << (~binary1).toString(2) << endl;
//        cout << "!a    = " << setw(8) << (!binary1).toString(2) << endl;
//        cout << endl;

//        cout << "int +: " << (42 + bi) << endl;
//        cout << "int -: " << (42 - bi) << endl;
//        cout << "int *: " << (42 * bi) << endl;
//        cout << "int /: " << (42 / bi) << endl;
//        cout << "int %: " << (42 % bi) << endl;

//        cout << "long +: " << ((long)42 + bi) << endl;
//        cout << "long -: " << ((long)42 - bi) << endl;
//        cout << "long *: " << ((long)42 * bi) << endl;
//        cout << "long /: " << ((long)42 / bi) << endl;
//        cout << "long %: " << ((long)42 % bi) << endl;

////        cout << "double +: " << (42.5 + bi) << endl;
////        cout << "double -: " << (42.5 - bi) << endl;
////        cout << "double *: " << (42.5 * bi) << endl;
////        cout << "double /: " << (42.5 / bi) << endl;
////        cout << "double %: " << (42.5 % bi) << endl;
//    }

//    try {
//        BigInteger bad("-845lol098");
//        cout << bad << endl;
//    } catch (ErrorException& ex) {
//        cout << "correctly threw exception when creating bad integer: " << ex.getMessage() << endl;
//    }

//    // big division
//    BigInteger realbig(1000);
//    realbig *= realbig;
//    realbig *= realbig;
//    realbig *= realbig;
//    realbig *= realbig;
//    BigInteger almostAsBig(realbig);
//    realbig *= realbig;
//    cout << "realbig: " << realbig << endl;
//    cout << "almost : " << almostAsBig << endl;

//    try {
//        BigInteger quotient = realbig / almostAsBig;
//        cout << "quotient: " << quotient << endl;
//    } catch (ErrorException& ex) {
//        cout << "correctly threw exception when dividing by large integer" << endl;
//    }


//    cout << "Fib:" << endl;
//    BigInteger a(1);
//    BigInteger b(1);
//    for (int i = 0; i < 100; i++) {
//        BigInteger c = a + b;
//        cout << "#" << i << ": " << c << endl;
//        a = b;
//        b = c;
//    }

//    cout << "Fact:" << endl;
//    bigint f;
//    for (int i = 0; i < 50; i++) {
//        f = fact(i);
//        cout << "fact(" << i << ") = " << f << endl;
//    }

//    for (int i = 0; i < 19; i++) {
//        bigint g = fact(i);
//        bigint quotient = f / g;
//        bigint mod = f / g;

//        cout << "i = " << i << ", fact = " << g << ", quot = " << quotient << ", mod = " << mod << endl;
//    }
//}
