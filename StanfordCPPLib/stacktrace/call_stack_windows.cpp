/* Windows implementation of the call_stack class. */
#ifdef _WIN32
#include "call_stack.h"
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <errno.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#define _NO_CVCONST_H
#include <dbghelp.h>
#include <imagehlp.h>
#include "error.h"
#include "exceptions.h"
#include "strlib.h"
#include <cxxabi.h>

// TODO: REMOVE ***
#include "simpio.h"

namespace stacktrace {

#define WIN_STACK_FRAMES_TO_SKIP 0
#define WIN_STACK_FRAMES_MAX 60

std::string getLastWindowsError() {
    DWORD lastErrorCode = ::GetLastError();
    char* errorMsg = NULL;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    ::FormatMessageA(
                   /* dwFlags */ FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   /* lpSource */ NULL,
                   /* dwMessageId */ lastErrorCode,
                   /* dwLanguageId */ LANG_NEUTRAL,
                   /* lpBuffer */ (LPSTR)&errorMsg,
                   /* dwSize */ 0,
                   /* arguments */ NULL);
    if (errorMsg) {
        return std::string(errorMsg);
    } else {
        return "NULL";
    }
}

// line = "ZNK6VectorIiE3getEi at vector.h:587"
//         <FUNCTION> at <LINESTR>
void injectAddr2lineInfo(entry& ent, const std::string& line) {
    ent.line     = 0;
    int atIndex = stringIndexOf(line, " at ");
    if (atIndex >= 0) {
        if (ent.function.empty()) {
            ent.function = line.substr(0, atIndex);
        }
        ent.lineStr  = line.substr(atIndex + 4);

        int colonIndex = stringIndexOf(ent.lineStr, ":");
        if (colonIndex >= 0) {
            ent.file = ent.lineStr.substr(0, colonIndex);
            std::string rest = ent.lineStr.substr(colonIndex + 1);
            if (stringIsInteger(rest)) {
                ent.line = stringToInteger(rest);
            }
        }
    } else {
        if (ent.function.empty()) {
            ent.function = line;
        }
    }

    // demangle function name if necessary
    if (startsWith(ent.function, "Z")) {
        ent.function = "_" + ent.function;
    }

    if (startsWith(ent.function, "_Z")) {
        int status;
        char* demangled = abi::__cxa_demangle(ent.function.c_str(), NULL, 0, &status);
        if (status == 0 && demangled) {
            ent.function = demangled;
        }
    }
}

call_stack::call_stack(const size_t /*num_discard = 0*/) {
    // getting a stack trace on Windows / MinGW is loads of fun (not)
    HANDLE process = GetCurrentProcess();
    if (!::SymSetOptions(
                         // ::SymGetOptions()
                           SYMOPT_DEBUG
                         | SYMOPT_DEFERRED_LOADS
                         | SYMOPT_INCLUDE_32BIT_MODULES
                         // | SYMOPT_UNDNAME
                         | SYMOPT_CASE_INSENSITIVE
                         | SYMOPT_LOAD_LINES)) {
        std::cout << "SymSetOptions failed!" << std::endl;
        return;
    }
    if (!::SymInitialize(
            /* process */ process,
            /* user-defined search path */ NULL,
            /* include current process */ TRUE)) {
        std::cout << "SymInitialize failed!" << std::endl;
        return;
    }

    void* trace[100];
    USHORT frameCount = ::CaptureStackBackTrace(
                /* framesToSkip */ WIN_STACK_FRAMES_TO_SKIP,
                /* framesToCapture; must be < 63 */ WIN_STACK_FRAMES_MAX,
                trace,
                /* hash */ NULL
                );
    // StackWalk function?

    // try to load module symbol information; this always fails for me  :-/
    DWORD64 BaseAddr = 0;
    DWORD   FileSize = 0;
    const char* progFileC = exceptions::getProgramNameForStackTrace().c_str();
    char* progFile = (char*) progFileC;
    DWORD64 ModBase = ::SymLoadModule(
                            process,      // Process handle of the current process
                            NULL,         // Handle to the module's image file (not needed)
                            progFile,     // Path/name of the file
                            NULL,         // User-defined short name of the module (it can be NULL)
                            BaseAddr,     // Base address of the module (cannot be NULL if .PDB file is used, otherwise it can be NULL)
                            FileSize      // Size of the file (cannot be NULL if .PDB file is used, otherwise it can be NULL)
                        );
    if (ModBase == 0) {
        // std::cout << "Error: SymLoadModule() failed: " << getLastWindowsError() << std::endl;
        // return;
    }

    // let's also try to get the line numbers via an external command-line process 'addr2line'
    // (ought to be able to get this information through C function 'backtrace', but for some
    // reason, Qt Creator's shipped version of MinGW does not include this functionality, argh)
    std::string addr2lineOutput;
    std::vector<std::string> addr2lineLines;
    if (frameCount > 0) {
        int result = addr2line_all(trace, frameCount, addr2lineOutput);
        if (result == 0) {
            addr2lineLines = stringSplit(addr2lineOutput, "\n");
        }
    }

    SYMBOL_INFO* symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 1024 * sizeof(char), 1);
    symbol->MaxNameLen   = 1020;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for (int i = 0; i < frameCount; ++i) {
        entry ent;
        if (::SymFromAddr(process, (DWORD64) trace[i], 0, symbol)) {
            ent.function = symbol->Name;
        }
        // internal stuff failed, so load from external process
        if (i < addr2lineLines.size()) {
            injectAddr2lineInfo(ent, addr2line_clean(addr2lineLines[i]));
        } else {
            injectAddr2lineInfo(ent, "");
        }

        if (!ent.function.empty() || ent.line > 0) {
            stack.push_back(ent);
        }
    }
    free(symbol);
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32
