/*
 * Copyright (c) 2009, Fredrik Orderud
 * License: BSD licence (http://www.opensource.org/licenses/bsd-license.php)
 *
 * @author Marty Stepp (made changes to F.Orderud version)
 * @version 2018/10/22
 * - bug fix for STL vector vs Stanford Vector
 * @version 2018/10/18
 * - added addr2line_functionName to resolve some function names not in backtrace
 * @version 2016/12/01
 * - bug fixes for call stack line number retrieval
 * - slight refactor of entry class
 * @version 2015/07/05
 * - replaced pragma once with ifndef/define for greater compatibility
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _call_stack_h
#define _call_stack_h
#include <string>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

namespace stacktrace {

int execAndCapture(std::string cmd, std::string& output);
int addr2line(void* addr, std::string& line);
int addr2line_all(Vector<void*> addrsVector, std::string& output);
int addr2line_all(void** addrs, int length, std::string& output);
std::string addr2line_clean(std::string line);
std::string addr2line_functionName(std::string line);

/*
 * Function to get/set a fake call stack pointer for use in printing a stack trace.
 * Called on Windows only after a signal / SEH handler is invoked to get a stack pointer.
 */
void*& fakeCallStackPointer();

/** Call-stack entry datastructure. */
struct entry {
public:
    /** Default constructor that clears all fields. */
    entry() : line(0), address(nullptr) {
        // empty
    }

    std::string file;       // filename
    size_t      line;       // line number
    std::string lineStr;    // line number string (not always set)
    std::string function;   // name of function or method
    void* address;          // memory address of stack pointer (raw)
    void* address2;         // memory address of stack pointer (from dladdr; data segment offset subtracted)

    /** Serialize entry into a text string. */
    std::string toString() const {
        std::ostringstream os;
        os << "file=\"" << file << "\"";
        if (line > 0) {
            os << " (line=" << line << ")";
        } else if (!lineStr.empty()) {
            os << " (lineStr=\"" << lineStr << "\")";
        }
        os << " function=\"" << function << "\"";
        return os.str();
    }
};

std::ostream& operator <<(std::ostream& out, const entry& ent);

/** Stack-trace base class, for retrieving the current call-stack. */
class call_stack {
public:
    /** Stack-trace consructor.
     \param num_discard - number of stack entries to discard at the top. */
    call_stack(const size_t num_discard = 0);

    virtual ~call_stack() throw();

    /** Serializes the entire call-stack into a text string. */
    std::string to_string() const {
        std::ostringstream os;
        for (int i = 0; i < stack.size(); i++)
            os << stack[i].toString() << std::endl;
        return os.str();
    }

    /** Call stack. */
    Vector<entry> stack;
};

} // namespace stacktrace

#endif // _call_stack_h
