/* Windows implementation of the call_stack class. */
#ifdef _WIN32
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
#include "error.h"
#include "exceptions.h"
#include "platform.h"
#include "strlib.h"
#include <cxxabi.h>

namespace stacktrace {

static Platform* pp = getPlatform();

const int WIN_STACK_FRAMES_TO_SKIP = 0;
const int WIN_STACK_FRAMES_MAX = 20;

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
    std::vector<void*> traceVector;
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    void* fakeStackPtr = stacktrace::getFakeCallStackPointer();
    if (fakeStackPtr) {
        // set up fake stack for partial trace
        LPEXCEPTION_POINTERS exceptionInfo = (LPEXCEPTION_POINTERS) fakeStackPtr;
        if (exceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
            // can't do stack walking in Windows when a stack overflow happens :-/
            traceVector.push_back((void*) exceptionInfo->ContextRecord->Eip);
        } else {
            SymInitialize(GetCurrentProcess(), 0, TRUE);
            STACKFRAME frame = {0};
            frame.AddrPC.Offset    = exceptionInfo->ContextRecord->Eip;
            frame.AddrPC.Mode      = AddrModeFlat;
            frame.AddrStack.Offset = exceptionInfo->ContextRecord->Esp;
            frame.AddrStack.Mode   = AddrModeFlat;
            frame.AddrFrame.Offset = exceptionInfo->ContextRecord->Ebp;
            frame.AddrFrame.Mode   = AddrModeFlat;
            while ((int) traceVector.size() < WIN_STACK_FRAMES_MAX &&
                   StackWalk(IMAGE_FILE_MACHINE_I386,
                             process,
                             thread,
                             &frame,
                             exceptionInfo->ContextRecord,
                             0,
                             SymFunctionTableAccess,
                             SymGetModuleBase,
                             0)) {
                traceVector.push_back((void*) frame.AddrPC.Offset);
            }
        }
    } else {
        if (!::SymSetOptions(
                             // ::SymGetOptions()
                               SYMOPT_DEBUG
                             | SYMOPT_DEFERRED_LOADS
                             | SYMOPT_INCLUDE_32BIT_MODULES
                             // | SYMOPT_UNDNAME
                             | SYMOPT_CASE_INSENSITIVE
                             | SYMOPT_LOAD_LINES)) {
            // std::cout << "SymSetOptions failed!" << std::endl;
            // return;
        }
        if (!::SymInitialize(
                /* process */ process,
                /* user-defined search path */ NULL,
                /* include current process */ TRUE)) {
            // std::cout << "SymInitialize failed!" << std::endl;
            // return;
        }

        void* trace[WIN_STACK_FRAMES_MAX];
        USHORT frameCount = ::CaptureStackBackTrace(
                    /* framesToSkip */ WIN_STACK_FRAMES_TO_SKIP,
                    /* framesToCapture; must be < 63 */ WIN_STACK_FRAMES_MAX,
                    trace,
                    /* hash */ NULL
                    );
        for (int i = 0; i < frameCount; i++) {
            traceVector.push_back(trace[i]);
        }

        // try to load module symbol information; this always fails for me  :-/
        DWORD64 BaseAddr = 0;
        DWORD   FileSize = 0;
        const char* progFileC = exceptions::getProgramNameForStackTrace().c_str();
        char* progFile = (char*) progFileC;
        if (!::SymLoadModule(
                process,      // Process handle of the current process
                NULL,         // Handle to the module's image file (not needed)
                progFile,     // Path/name of the file
                NULL,         // User-defined short name of the module (it can be NULL)
                BaseAddr,     // Base address of the module (cannot be NULL if .PDB file is used, otherwise it can be NULL)
                FileSize)) {      // Size of the file (cannot be NULL if .PDB file is used, otherwise it can be NULL)
            // std::cout << "Error: SymLoadModule() failed: " << pp->os_getLastError() << std::endl;
            // return;
        }
    }

    // let's also try to get the line numbers via an external command-line process 'addr2line'
    // (ought to be able to get this information through C function 'backtrace', but for some
    // reason, Qt Creator's shipped version of MinGW does not include this functionality, argh)
    std::string addr2lineOutput;
    std::vector<std::string> addr2lineLines;
    if (!traceVector.empty()) {
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
        if (process && ::SymFromAddr(process, (DWORD64) traceVector[i], 0, symbol)) {
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
