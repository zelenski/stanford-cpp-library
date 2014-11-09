/* Copyright (c) 2009, Fredrik Orderud
   License: BSD licence (http://www.opensource.org/licenses/bsd-license.php)
   Based on: http://stupefydeveloper.blogspot.com/2008/10/cc-call-stack.html */

/* Linux/gcc implementation of the call_stack class. */
#ifndef _WIN32
#ifdef __GNUC__
#include <stdio.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <dlfcn.h>
#endif // __GNUC__
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "call_stack.h"
#include "exceptions.h"
#include "strlib.h"

#define MAX_DEPTH 32

namespace stacktrace {

/*
 * Run a sub-process and capture its output.
 */
int execAndCapture(std::string cmd, std::string& output) {
    cmd += " 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }
    char buffer[1024];
    output = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 1024, pipe) != NULL) {
            output += buffer;
        }
    }
    return pclose(pipe);
}

// "_Z4Mainv at /home/stepp/.../FooProject/src/mainfunc.cpp:131"
std::string addr2line_clean(std::string line) {
    if (line.find(" at ") != std::string::npos) {
        line = line.substr(line.rfind(" at ") + 4);
    }
    if (line.find('/') != std::string::npos) {
        line = line.substr(line.rfind('/') + 1);
    }
    
    // strip extra parenthesized info from the end
    if (line.find(" (") != std::string::npos) {
        line = line.substr(0, line.rfind(" ("));
    }
    line = trim(line);
    return line;
}

int addr2line_all(void** addrs, int length, std::string& output) {
    // have addr2line map the address to the relent line in the code
#if defined(__APPLE__)
    // Mac OS X version
    // TODO: implement/test
//    sprintf(addr2line_cmd, "atos -o %.256s %p", exceptions::getProgramNameForStackTrace().c_str(), addr); 
//    return execAndCapture(addr2line_cmd, line);
    return -1;
#elif defined(_WIN32)
    // not implemented on Windows yet
    return -1;
#else
    // Linux
    
    // make a space-separated string of all the addresses
    std::ostringstream out;
    out << "addr2line -f -C -s -p -e " << exceptions::getProgramNameForStackTrace();
    for (int i = 0; i < length; i++) {
        out << " " << std::hex << std::setfill('0') << addrs[i];
    }
    
    int result = execAndCapture(out.str(), output);
    return result;
#endif
}

/*
 * Resolve symbol name and source location given the path to the executable 
 * and an address
 */
int addr2line(void* addr, std::string& line) {
    void* addrs[1] = {addr};
    return addr2line_all(addrs, 1, line);
}

call_stack::call_stack (const size_t num_discard /*= 0*/) {
    using namespace abi;

    // retrieve call-stack
    void * trace[MAX_DEPTH];
    int stack_depth = backtrace(trace, MAX_DEPTH);

    // let's also try to get the line numbers via an external process
    std::string addr2lineOutput;
    std::vector<std::string> addr2lineLines;
    if (stack_depth > 0) {
        addr2line_all(trace, stack_depth, addr2lineOutput);
        addr2lineLines = stringSplit(addr2lineOutput, "\n");
    }
    
    for (int i = num_discard+1; i < stack_depth; i++) {
        Dl_info dlinfo;
        if(!dladdr(trace[i], &dlinfo))
            continue;

        const char * symname = dlinfo.dli_sname;

        int    status;
        char * demangled = abi::__cxa_demangle(symname, NULL, 0, &status);
        if(status == 0 && demangled)
            symname = demangled;

        //printf("entry: %s, %s\n", dlinfo.dli_fname, symname);

        // store entry to stack
        if (dlinfo.dli_fname && symname) {
            entry e;
            e.file     = dlinfo.dli_fname;
            e.line     = 0; // unsupported
            e.function = symname;
            
            // let's try to get the line number
            // addr2line(trace[i], e.lineStr);
            if (i < (int) addr2lineLines.size()) {
                e.lineStr = addr2line_clean(addr2lineLines[i]);
            }
            
            stack.push_back(e);
        } else {
            continue; // skip last entries below main
        }

        if (demangled)
            free(demangled);
    }
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#else // _WIN32

#include "call_stack.h"
#include <windows.h>
#include <tchar.h>

#include <assert.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#ifdef _WIN32
  #include <windows.h>
  #include <imagehlp.h>
#else
  #include <err.h>
  #include <execinfo.h>
#endif
#include "error.h"
#include "exceptions.h"
#include "strlib.h"


namespace stacktrace {



int execAndCapture(std::string cmd, std::string& output) {
    cmd += " 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }
    char buffer[1024];
    output = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 1024, pipe) != NULL) {
            output += buffer;
        }
    }
    return pclose(pipe);
}


// "_Z5errorSs at /home/stepp/Dropbox/data/docs/stanford/StanfordCPPLib/build/Autograder_QtCreatorProject-Desktop_Qt_5_3_GCC_64bit-Debug/../../Autograder_QtCreatorProject/lib/StanfordCPPLib/error.cpp:47 (discriminator 2)"
// _Z1cv at mainfunc.cpp:71 (discriminator 1)

// http://en.wikipedia.org/wiki/Name_mangling#Name_mangling_in_C.2B.2B

// MyNS::LoL::cCcCc_cCcCc123(int, char*, double, char, bool)
// becomes
// _ZN4MyNS3LoL14cCcCc_cCcCc123EiPcdcb

// _Z N4 MyNS 3 LoL 14 cCcCc_cCcCc123 EiPcdcb

/*
 * Resolve symbol name and source location given the path to the executable
 * and an address
 */
int addr2line(const void* const addr, std::string& line) {
    char addr2line_cmd[512] = {0};

    // have addr2line map the address to the relent line in the code
    std::string PROGRAM_NAME = exceptions::getProgramNameForStackTrace();
#if defined(_WIN32)
    sprintf(addr2line_cmd, "C:\\Programs\\Qt\\Tools\\mingw482_32\\bin\\addr2line.exe -f -p -e %.256s %p", PROGRAM_NAME.c_str(), addr);
#elif defined(__APPLE__)
    sprintf(addr2line_cmd, "atos -o %.256s %p", PROGRAM_NAME.c_str(), addr);
#else
    sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", PROGRAM_NAME.c_str(), addr);
#endif

    return execAndCapture(addr2line_cmd, line);
}

#ifdef _WIN32
#define MAX_STACK_FRAMES 64
static void* stack_traces[MAX_STACK_FRAMES];

void print_stack_trace() {
    int i, trace_size = 0;
    char **messages = (char **)NULL;

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);

    // skip the first couple stack frames (as they are this function and
    // our handler) and also skip the last frame as it's (always?) junk.
    // for (i = 3; i < (trace_size - 1); ++i)
    for (i = 0; i < trace_size; ++i) {
        std::string line;
        if (addr2line(stack_traces[i], line) == 0) {
            // *** line = demangle(line);
            if (!line.empty()) {
                // stop showing the stack trace when it gets down to the low-level functions
                if (stringContains(line, "at main.cpp") || stringContains(line, "at platform.cpp")) {
                    break;
                }
                std::cerr << " ***     " << line << std::endl;
            }
        } else {
            // printf("  error determining line # for: %s\n", messages[i]);
        }

    }
    if (messages) {
        free(messages);
    }
}
#endif







call_stack::call_stack(const size_t /*num_discard = 0*/) {
    // empty
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32
