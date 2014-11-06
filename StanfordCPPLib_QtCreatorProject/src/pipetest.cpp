//#include <iostream>
//#include <string>
//#include "console.h"
//#include "regexpr.h"
//using namespace std;

//extern int mainWrapper(int argc, char **argv);
//extern int startupMain(int argc, char **argv);
//extern void startupMainDontRunMain(int argc, char **argv);   // added to platform.cpp by Marty
//extern int studentMain();
//extern int _mainFlags;
//int studentMain() {return 0;}
//int Main() {return 0;}

//#undef main
//int main(int argc, char** argv) {
//    // initialize Stanford libraries and graphical console
//    _mainFlags = GRAPHICS_FLAG + CONSOLE_FLAG;
//    startupMainDontRunMain(argc, argv);
    
//    string slong = "abcbcde";
//    while (slong.length() < 1024*64) {
//        slong += slong;
//    }
//    cout << "about to regex replace on long string (length " << slong.length() << ")" << endl;
//    slong = regexReplace(slong, "bc", "XX");
//    cout << "done with regex replace on long string (length " << slong.length() << ")." << endl;
//    cout << slong.substr(0, 60) << " ..." << endl;
//    cout << "Exiting main." << endl;
//    return 0;
//}
