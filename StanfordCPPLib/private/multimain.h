/*
 * File: multimain.h
 * -----------------
 * ...
 *
 * @version 2018/10/18
 * - initial version
 */

#ifndef _multimain_h
#define _multimain_h

#include <string>

namespace stanfordcpplib {

/**
 * Adds a new main() function with the given name to the list of multi-mains
 * that will be presented when the program runs.
 */
int registerMainFunction(const std::string& name, int (*mainFunc)());
int selectMainFunction();

/**
 * A class for registering a main function at static initialization time,
 * before the real main() runs.
 */
class MultiMainRegisterStaticInitializer {
public:
    MultiMainRegisterStaticInitializer(const std::string& name, int (* mainFunc)(), bool graphical = false);
};

} // namespace stanfordcpplib

#endif // _multimain_h


// only define initstudent if needed by SPL QT GUI or graphical console
//#if defined(SPL_QT_GUI_IN_USE) || defined(SPL_GRAPHICAL_CONSOLE_IN_USE)
#ifndef QT_NEEDS_QMAIN
#ifdef SPL_REPLACE_MAIN_FUNCTION

#ifndef main
#define main qMain
#endif // main

#endif // SPL_REPLACE_MAIN_FUNCTION
#endif // QT_NEEDS_QMAIN

// replace "multimain(foo)" definition with:
//   1) prototype of multimain_foo()
//   2) call to MainRegisterStaticInitializer that maps "foo" to multimain_foo function
//   3) definition of multimain_foo()
#undef multimain
#if defined(SPL_QT_GUI_IN_USE)
#define multimain(name) \
    main_##name (); \
    static ::stanfordcpplib::MultiMainRegisterStaticInitializer _main_init_##name ((#name), main_##name, true); \
int main_##name ()
#else
#define multimain(name) \
    main_##name (); \
    static ::stanfordcpplib::MultiMainRegisterStaticInitializer _main_init_##name ((#name), main_##name); \
int main_##name ()
#endif // defined(SPL_QT_GUI_IN_USE)

//#endif // defined(SPL_QT_GUI_IN_USE) || defined(SPL_GRAPHICAL_CONSOLE_IN_USE)
