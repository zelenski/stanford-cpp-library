#include "recursion.h"
#include "exceptions.h"
#include "call_stack.h"

int getRecursionIndentLevel() {
    // constructing the following object jumps into fancy code in call_stack_gcc/windows.cpp
    // to rebuild the stack trace; implementation differs for each operating system
    stacktrace::call_stack trace;
    std::vector<stacktrace::entry> entries = trace.stack;

    std::string currentFunction = "";
    int currentFunctionCount = 0;
    for (size_t i = 0; i < entries.size(); ++i) {
        // remove references to std:: namespace
        if (exceptions::shouldFilterOutFromStackTrace(entries[i].function)
                || entries[i].function.find("recursionIndent(") != std::string::npos
                || entries[i].function.find("getRecursionIndentLevel(") != std::string::npos) {
            continue;
        } else if (currentFunction.empty()) {
            currentFunction = entries[i].function;
            currentFunctionCount = 1;
        } else if (entries[i].function == currentFunction) {
            currentFunctionCount++;
        } else {
            break;
        }
    }
    return currentFunctionCount;
}

std::string recursionIndent(const std::string& indenter) {
    int indent = getRecursionIndentLevel();
    std::string result = "";
    for (int i = 0; i < indent - 1; i++) {
        result += indenter;
    }
    return result;
}
