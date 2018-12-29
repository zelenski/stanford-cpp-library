/*
 * File: multimain.cpp
 * -------------------
 * ...
 *
 * @version 2018/11/14
 * - added Cancel option to multimain popup dialog
 * @version 2018/10/18
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "private/multimain.h"
#include <iomanip>
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "goptionpane.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "simpio.h"
#undef INTERNAL_INCLUDE

// helper to store [name => main func pointer]
STATIC_VARIABLE_DECLARE_MAP_EMPTY(Map, std::string, GThunkInt, funcMap);

extern int main();

namespace stanfordcpplib {

// whether the popup list of mains should be graphical
static bool& mainSelectorIsGraphical() {
    static bool _isGraphical = false;
    return _isGraphical;
}

int registerMainFunction(const std::string& name, int (*mainFunc)()) {
    GThunkInt mainThunk = mainFunc;
    STATIC_VARIABLE(funcMap)[name] = mainThunk;
    return 0;
}

MultiMainRegisterStaticInitializer::MultiMainRegisterStaticInitializer(
        const std::string& name, int (* mainFunc)(), bool graphical) {
    STATIC_VARIABLE(funcMap)[name] = mainFunc;
    mainSelectorIsGraphical() |= graphical;
}

#ifndef SPL_AUTOGRADER_MODE

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)(void));
extern void shutdownLibrary();

int selectMainFunction() {
    Map<std::string, GThunkInt>& funcMap = STATIC_VARIABLE(funcMap);
    if (funcMap.isEmpty()) {

        //////////////////////////////////////////////////////////////////////////
        /// NOTE TO STUDENT!                                                   ///
        /// If you are directed here by a compiler error,                      ///
        /// it means that you have not written a main function or that it      ///
        /// has the wrong parameters.                                          ///
        /// The heading of your main function must be:                         ///
        ///                                                                    ///
        /// int main() { ... }                                                 ///
        ///                                                                    ///
        /// (Our library secretly renames your main function to "qMain"        ///
        ///  so that we can actually control the main flow of execution.)      ///
        ///                                                                    ///
        /// You may also need to include a .h header from the Stanford         ///
        /// C++ library in the file of your project that contains the          ///
        /// 'main' function.                                                   ///
        //////////////////////////////////////////////////////////////////////////
        stanfordcpplib::runMainInThread(main);

    } else if (funcMap.size() == 1) {
        // just one main, so just run it
        std::string mainFuncName = funcMap.front();
        GThunkInt mainFunc = funcMap[mainFuncName];
        stanfordcpplib::runMainInThread(mainFunc);
    } else {
        // multiple mains; ask user which one to run
        if (stanfordcpplib::mainSelectorIsGraphical()) {
            GThunkInt mainFuncWrapper = [&funcMap]() {
                Vector<std::string> options;
                for (std::string functionName : funcMap) {
                    options.add(functionName);
                }

                // add a "Cancel" option so we can press Escape to abort
                options.add("Cancel");

                std::string choice = "";
                choice = GOptionPane::showOptionDialog(
                        /* message */ "main functions available:",
                        /* options */ options,
                        /* title   */ "Choose main function to run");
                if (choice.empty()) {
                    return 0;
                }
                GThunkInt mainFunc = funcMap[choice];
                if (mainFunc) {
                    return mainFunc();
                } else {
                    return 0;
                }
            };
            stanfordcpplib::runMainInThread(mainFuncWrapper);
        } else {
            // plain text console
            GThunkInt mainFuncWrapper = [&funcMap]() {
                // print list of mains
                std::cout << "main functions available:" << std::endl;
                int i = 1;
                Map<int, std::string> numberToName;
                for (std::string functionName : funcMap) {
                    std::cout << std::setw(2) << i << ") " << functionName << std::endl;
                    numberToName[i] = functionName;
                    i++;
                }
                int choice = getIntegerBetween("Your choice? ", 0, funcMap.size());
                if (choice == 0) {
                    return 0;
                }
                GThunkInt mainFunc = funcMap[numberToName[choice]];
                return mainFunc();
            };
            stanfordcpplib::runMainInThread(mainFuncWrapper);
        }
    }

    stanfordcpplib::shutdownLibrary();
    return 0;
}
#endif // SPL_AUTOGRADER_MODE

} // namespace stanfordcpplib
