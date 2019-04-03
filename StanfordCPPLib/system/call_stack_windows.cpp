/*
 * File: call_stack_windows.cpp
 * ----------------------------
 * Windows implementation of the call_stack class.
 *
 * @author Marty Stepp
 * @version 2019/04/03
 * - fixed compiler errors for 64-bit Windows MinGW compiler (context struct)
 * @version 2018/10/22
 * - bug fix for STL vector vs Stanford Vector
 * @version 2018/09/12
 * - fixed compiler errors with os_getLastError and other misc warnings
 * @version 2017/10/24
 * - removed SYMOPT_DEBUG from SymSetOptions to avoid spurious console output
 * @version 2017/10/20
 * - changed null/0s to nullptr to remove compiler warnings
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2015/05/28
 */

#ifdef _WIN32
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#include <windows.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
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
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#include <cxxabi.h>
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace stacktrace {
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_TO_SKIP, 0)
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_MAX, 20)

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
        char* demangled = abi::__cxa_demangle(ent.function.c_str(), nullptr, nullptr, &status);
        if (status == 0 && demangled) {
            ent.function = demangled;
        }
    }
}

std::ostream& operator <<(std::ostream& out, const entry& ent) {
    return out << ent.toString();
}

call_stack::call_stack(const size_t /*num_discard = 0*/) {
    // getting a stack trace on Windows / MinGW is loads of fun (not)
    Vector<void*> traceVector;
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    // dear student: if you get a compiler error about 'Eip' not being found here,
    // it means you're using a 64-bit compiler like the MS Visual C++ compiler,
    // and not the 32-bit MinGW compiler we instructed you to install.
    // Please re-install Qt Creator with the proper compiler (MinGW 32-bit) enabled.

    void* fakeStackPtr = stacktrace::fakeCallStackPointer();
    if (fakeStackPtr) {
        // set up fake stack for partial trace
        LPEXCEPTION_POINTERS exceptionInfo = (LPEXCEPTION_POINTERS) fakeStackPtr;
        if (exceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
            // can't do stack walking in Windows when a stack overflow happens :-/
#if _WIN64
            traceVector.push_back((void*) exceptionInfo->ContextRecord->Rip);
#else
            traceVector.push_back((void*) exceptionInfo->ContextRecord->Eip);
#endif // _WIN64
        } else {
            SymInitialize(GetCurrentProcess(), nullptr, TRUE);
            STACKFRAME frame = {0};
#if _WIN64
            frame.AddrPC.Offset    = exceptionInfo->ContextRecord->Rip;
            frame.AddrStack.Offset = exceptionInfo->ContextRecord->Rsp;
            frame.AddrFrame.Offset = exceptionInfo->ContextRecord->Rbp;
#else
            frame.AddrPC.Offset    = exceptionInfo->ContextRecord->Eip;
            frame.AddrStack.Offset = exceptionInfo->ContextRecord->Esp;
            frame.AddrFrame.Offset = exceptionInfo->ContextRecord->Ebp;
#endif // _WIN64
            frame.AddrPC.Mode      = AddrModeFlat;
            frame.AddrStack.Mode   = AddrModeFlat;
            frame.AddrFrame.Mode   = AddrModeFlat;
            while ((int) traceVector.size() < STATIC_VARIABLE(STACK_FRAMES_MAX) &&
                   StackWalk(IMAGE_FILE_MACHINE_I386,
                             process,
                             thread,
                             &frame,
                             exceptionInfo->ContextRecord,
                             nullptr,
                             SymFunctionTableAccess,
                             SymGetModuleBase,
                             nullptr)) {
                traceVector.push_back((void*) frame.AddrPC.Offset);
            }
        }
    } else {
        if (!::SymSetOptions(
                             // ::SymGetOptions()
                             // SYMOPT_DEBUG
                               SYMOPT_DEFERRED_LOADS
                             | SYMOPT_INCLUDE_32BIT_MODULES
                             // | SYMOPT_UNDNAME
                             | SYMOPT_CASE_INSENSITIVE
                             | SYMOPT_LOAD_LINES)) {
            // std::cout << "SymSetOptions failed!" << std::endl;
            // return;
        }
        if (!::SymInitialize(
                /* process */ process,
                /* user-defined search path */ nullptr,
                /* include current process */ TRUE)) {
            // std::cout << "SymInitialize failed!" << std::endl;
            // return;
        }

        void* trace[STATIC_VARIABLE(STACK_FRAMES_MAX)];
        USHORT frameCount = ::CaptureStackBackTrace(
                    /* framesToSkip */ STATIC_VARIABLE(STACK_FRAMES_TO_SKIP),
                    /* framesToCapture; must be < 63 */ STATIC_VARIABLE(STACK_FRAMES_MAX),
                    trace,
                    /* hash */ nullptr
                    );
        for (int i = 0; i < frameCount; i++) {
            traceVector.push_back(trace[i]);
        }

        // try to load module symbol information; this always fails for me  :-/
        DWORD BaseAddr = 0;
        DWORD FileSize = 0;
        const char* progFileC = exceptions::getProgramNameForStackTrace().c_str();
        char* progFile = const_cast<char*>(progFileC);
        if (!::SymLoadModule(
                process,      // Process handle of the current process
                nullptr,      // Handle to the module's image file (not needed)
                progFile,     // Path/name of the file
                nullptr,      // User-defined short name of the module (it can be null)
                BaseAddr,     // Base address of the module (cannot be null if .PDB file is used, otherwise it can be null)
                FileSize)) {      // Size of the file (cannot be null if .PDB file is used, otherwise it can be null)
            // std::cerr << "Error: SymLoadModule() failed: " << platform::os_getLastError() << std::endl;
            // return;
        }
    }

    // let's also try to get the line numbers via an external command-line process 'addr2line'
    // (ought to be able to get this information through C function 'backtrace', but for some
    // reason, Qt Creator's shipped version of MinGW does not include this functionality, argh)
    std::string addr2lineOutput;
    Vector<std::string> addr2lineLines;
    if (!traceVector.isEmpty()) {
        int result = addr2line_all(traceVector, addr2lineOutput);
        if (result == 0) {
            addr2lineLines = stringSplit(addr2lineOutput, "\n");
        }
    }

    SYMBOL_INFO* symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 1024 * sizeof(char), 1);
    symbol->MaxNameLen   = 1020;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for (int i = 0; i < (int) traceVector.size(); ++i) {
        entry ent;
        ent.address = traceVector[i];
        if (process && ::SymFromAddr(process, (DWORD64) traceVector[i], nullptr, symbol)) {
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
