/*
 * ...
 */

#include "codestepbystep.h"
#include "exceptions.h"
#include "private/platform.h"

extern void startupMainDontRunMain(int argc, char** argv);

namespace CodeStepByStep {
static std::string __testToRun = "";
static bool __runAllTests = false;
static std::string __xmlOutFilename;
static std::ofstream* __xout = nullptr;
static void (*old_terminate)() = nullptr;

// only use these if we can't use __getXmlOut (e.g. in a signal handler)
static FILE* __xfout = nullptr;

std::string testToRun() {
    return __testToRun;
}

bool runAllTests() {
    return __runAllTests;
}

std::string htmlDecode(const std::string& s) {
    return ::htmlDecode(s);
}

std::string htmlEncode(const std::string& s) {
    return ::htmlEncode(s);
}

std::ofstream& __getXmlOut() {
    if (!__xout) {
        __xout = new std::ofstream();
    }
    return *__xout;
}

void printXml(const std::string& s) {
    __getXmlOut() << s;
}

void printlnXml(const std::string& s) {
    __getXmlOut() << s << std::endl;
}

void printXml(bool b) {
    __getXmlOut() << std::boolalpha << b;
}

void printlnXml(bool b) {
    __getXmlOut() << std::boolalpha << b << std::endl;
}

std::string __stackTraceToString() {
    std::ostringstream out;
    exceptions::printStackTrace(out);
    return out.str();
}

void __printException(const std::string& type, const std::string& message,
                      const std::string& stacktrace, int lineNumber) {
    CodeStepByStep::__getXmlOut() << "<exception>\n"
            << "<type>" << type << "</type>" << std::endl
            << "<message>" << htmlEncode(message) << "</message>" << std::endl
            << "<line>" << lineNumber << "</line>" << std::endl
            << "<stacktrace>" << htmlEncode(stacktrace) << "</stacktrace>" << std::endl
            << "</exception>" << std::endl;
    CodeStepByStep::__getXmlOut().flush();
}

void __openXmlOldWay(const std::string& filename) {
    if (__xout) {
        __getXmlOut().flush();
        __getXmlOut().close();
        __xout = nullptr;
    }
    __xfout = fopen(filename.c_str(), "a+");   // open for appending
}

void __closeXml() {
    if (__xout) {
        __getXmlOut().close();
    }
    if (__xfout) {
        fclose(__xfout);
        __xfout = nullptr;
    }
}

void __printXml(const char* s) {
    fputs(s, __xfout);
    fflush(__xfout);
}

void __printXml(const std::string& s) {
    fputs(s.c_str(), __xfout);
    fflush(__xfout);
}

void __printlnXml(const char* s) {
    fputs(s, __xfout);
    fputs("\n", __xfout);
    fflush(__xfout);
}

void __printlnXml(const std::string& s) {
    fputs(s.c_str(), __xfout);
    fputs("\n", __xfout);
    fflush(__xfout);
}


// functions to parse various collections from strings
void ArrayIntList_fromString(ArrayIntList& list, const std::string& str) {
    std::istringstream input(str);
    input >> list;
}

void BasicGraph_fromString(BasicGraph& graph, const std::string& str) {
    std::istringstream input(str);
    input >> graph;
}

void BinaryTree_fromString(BinaryTree& tree, const std::string& str) {
    std::istringstream input(str);
    input >> tree;
}

void BinaryTreeNode_fromString(BinaryTreeNode*& root, const std::string& str) {
    std::istringstream input(str);
    input >> root;
}

void BinaryTreeNodeptr_fromString(BinaryTreeNode*& root, const std::string& str) {
    std::istringstream input(str);
    input >> root;
}

void HeapPriorityQueue_fromString(HeapPriorityQueue& pqueue, const std::string& str) {
    std::istringstream input(str);
    input >> pqueue;
}

void LinkedIntList_fromString(LinkedIntList& list, const std::string& str) {
    std::istringstream input(str);
    input >> list;
}

void ListNode_fromString(ListNode*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}

void ListNodeptr_fromString(ListNode*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}

void ListNodeDouble_fromString(ListNodeDouble*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}

void ListNodeDoubleptr_fromString(ListNodeDouble*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}

void ListNodeString_fromString(ListNodeString*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}

void ListNodeStringptr_fromString(ListNodeString*& ptr, const std::string& str) {
    std::istringstream input(str);
    input >> ptr;
}


// CODE FOR HANDLING SIGNALS (VARIOUS PROGRAM CRASHES)
void __codeStepByStepSignalHandler(int sig) {
    signal(sig, SIG_DFL);   // turn signal handler off

    // tailor the error message to the kind of signal that occurred
    CodeStepByStep::__closeXml();
    __openXmlOldWay(__xmlOutFilename);
    if (sig == SIGSEGV) {
        __printXml("<exception>\n<type>SIGSEGV</type>\n<message>segmentation fault</message>\n</exception>\n");
    } else if (sig == SIGABRT) {
        __printXml("<exception>\n<type>SIGABRT</type>\n<message>abort</message>\n</exception>\n");
    } else if (sig == SIGILL) {
        __printXml("<exception>\n<type>SIGILL</type>\n<message>illegal instruction</message>\n</exception>\n");
    } else if (sig == SIGFPE) {
        __printXml("<exception>\n<type>SIGFPE</type>\n<message>arithmetic error</message>\n</exception>\n");
    } else if (sig == SIGINT) {
        __printXml("<exception>\n<type>SIGINT</type>\n<message>interrupt error</message>\n</exception>\n");
    } else if (sig == SIGSTACK) {
        __printXml("<exception>\n<type>SIGSTACK</type>\n<message>stack overflow</message>\n</exception>\n");
    } else if (sig == SIGTERM) {
        __printXml("<exception>\n<type>SIGTERM</type>\n<message>terminated</message>\n</exception>\n");
    } else {
        __printXml("<exception>\n<type>SIGFATAL</type>\n<message>fatal error</message>\n</exception>\n");
    }

    if (__runAllTests) {
        __printXml("</test>\n");
        __printXml("</tests>\n");
    }

    __closeXml();
    std::exit(1);
}

void __posixSignalHandler(int sig, siginfo_t* /*siginfo*/, void* /*context*/) {
    __codeStepByStepSignalHandler(sig);
}

static void __setupSignalHandler() {
    bool handled = false;
#ifdef SHOULD_USE_SIGNAL_STACK
#if !defined(_WIN32)
    // alternate stack on Linux for stack overflows
    static uint8_t alternate_stack[SIGSTKSZ];
    stack_t ss = {};
    ss.ss_sp = (void*) alternate_stack;
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    sigaltstack(&ss, nullptr);

    struct sigaction sig_action = {};
    sig_action.sa_sigaction = __posixSignalHandler;
    sigemptyset(&sig_action.sa_mask);
#ifdef __APPLE__
    // backtrace() doesn't work on OS X when we use an alternate stack
    sig_action.sa_flags = SA_SIGINFO;
#else
    sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif // __APPLE__
    sigaction(SIGSEGV, &sig_action, nullptr);
    sigaction(SIGFPE,  &sig_action, nullptr);
    sigaction(SIGILL,  &sig_action, nullptr);
    sigaction(SIGTERM, &sig_action, nullptr);
    sigaction(SIGINT,  &sig_action, nullptr);
    sigaction(SIGABRT, &sig_action, nullptr);
    handled = true;
#endif
#endif // SHOULD_USE_SIGNAL_STACK

    if (!handled) {
        signal(SIGABRT, CodeStepByStep::__codeStepByStepSignalHandler);
        signal(SIGFPE,  CodeStepByStep::__codeStepByStepSignalHandler);
        signal(SIGILL,  CodeStepByStep::__codeStepByStepSignalHandler);
        signal(SIGINT,  CodeStepByStep::__codeStepByStepSignalHandler);
        signal(SIGSEGV, CodeStepByStep::__codeStepByStepSignalHandler);
        signal(SIGTERM, CodeStepByStep::__codeStepByStepSignalHandler);
    }
}

static void __disableSignalHandler() {
    signal(SIGABRT, SIG_DFL);
    signal(SIGFPE,  SIG_DFL);
    signal(SIGILL,  SIG_DFL);
    signal(SIGINT,  SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

static void __terminateHandler() {
    __disableSignalHandler();   // don't want both a signal AND a terminate() call

    std::string stackTrace;
    try {
        std::ostringstream out;
        exceptions::printStackTrace(out);
        stackTrace = out.str();
        throw;   // re-throws the exception that already occurred
    } catch (int value) {
        CodeStepByStep::__printException("int", integerToString(value), stackTrace);
    } catch (long value) {
        CodeStepByStep::__printException("long", longToString(value), stackTrace);
    } catch (float value) {
        CodeStepByStep::__printException("float", doubleToString((double) value), stackTrace);
    } catch (double value) {
        CodeStepByStep::__printException("double", doubleToString(value), stackTrace);
    } catch (bool value) {
        CodeStepByStep::__printException("bool", boolToString(value), stackTrace);
    } catch (char value) {
        CodeStepByStep::__printException("char", "'" + charToString(value) + "'", stackTrace);
    } catch (std::string value) {
        CodeStepByStep::__printException("string", value, stackTrace);
    } catch (const char* value) {
        CodeStepByStep::__printException("string", value, stackTrace);
    } catch (ErrorException value) {
        CodeStepByStep::__printException("ErrorException", value.what(), stackTrace);
    } catch (std::exception& value) {
        CodeStepByStep::__printException("exception", value.what(), stackTrace);
    } catch (...) {
        CodeStepByStep::__printException("unknown", "", stackTrace);
    }

    if (CodeStepByStep::__runAllTests) {
        CodeStepByStep::__getXmlOut() << "</test>" << std::endl;
        CodeStepByStep::__getXmlOut() << "</tests>" << std::endl;
    }
    CodeStepByStep::__closeXml();
}

void main_begin(int argc, char** argv) {
    // initialize Stanford library
    __initializeStanfordCppLibrary(argc, argv);

    // set up signal handler to process serious errors
    CodeStepByStep::__setupSignalHandler();
    CodeStepByStep::old_terminate = std::set_terminate(CodeStepByStep::__terminateHandler);

    // echo console input
    // plainconsole::setEcho(true);

    // parse command-line args
    CodeStepByStep::__testToRun = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (i < argc - 1 && arg == "--test") {
            CodeStepByStep::__testToRun = argv[i + 1];
        } else if (arg == "--all") {
            CodeStepByStep::__testToRun = "--all";
            CodeStepByStep::__runAllTests = true;
        } else if (i < argc - 1 && arg == "--xml") {
            CodeStepByStep::__xmlOutFilename = argv[i + 1];
            CodeStepByStep::__getXmlOut().open(argv[i + 1]);
            // CodeStepByStep::__openXml(__xmlOutFilename);
        }
    }

    if (CodeStepByStep::__runAllTests) {
        // output XML prolog
        CodeStepByStep::__getXmlOut() << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
                << "<tests>" << std::endl;
        CodeStepByStep::__getXmlOut().flush();
    }
}

void main_end() {
    if (CodeStepByStep::__runAllTests) {
        CodeStepByStep::__getXmlOut() << "</tests>" << std::endl;
        CodeStepByStep::__getXmlOut().flush();
    }

    CodeStepByStep::__closeXml();
}

} // namespace CodeStepByStep
