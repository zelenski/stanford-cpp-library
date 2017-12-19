/*
 * File: process.cpp
 * -----------------
 * This file implements a Process class to help launch external processes in
 * a platform-neutral way.
 * See process.h for class declarations and documentation.
 *
 * NOTE: THIS IMPLEMENTATION IS INCOMPLETE AND CURRENTLY DISABLED.
 *
 * @author Marty Stepp
 * @version 2017/10/07
 * - initial version
 * @since 2017/10/07
 */

#ifdef PROCESS_H_ENABLED   // won't be enabled
#include "process.h"
#include <csignal>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include "strlib.h"
#ifdef _WIN32
#  include <windows.h>
#  include <tchar.h>
#else // _WIN32
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/resource.h>
#  include <dirent.h>
#  include <errno.h>
#  include <pwd.h>
#  include <stdint.h>
#  include <unistd.h>
#endif // _WIN32


const int Process::TIMEOUT_MS_DEFAULT = 5000;

void Process::kill(int pid, int sig) {
    Process proc;
    proc.addCommandLineArgs({
        "kill",
        "-" + integerToString(sig),
        integerToString(pid)
    });
    proc.startAndWait();
}

int Process::runAndCaptureOutput(const std::string& commandLine,
                                 std::string& output,
                                 int timeoutMS) {
    output = "";
    Process proc(commandLine);
    proc.setTimeout(timeoutMS);
    proc.startAndWait();
    if (proc.timedOut()) {
        proc.stop();
    }
    output = proc.output();
    return proc.exitCode();
}

long Process::getCurrentTimeMillis() {
    struct timeval tp;
    if (gettimeofday(&tp, nullptr)) {
        return -1;
    }
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

Process::Process(const std::string& commandLine) :
    m_exitCode(-1),
    m_pid(-1),
    m_timeoutMS(0),
    m_running(false),
    m_terminated(false),
    m_timedOut(false)
{
#ifdef _WIN32

#else // _WIN32
    m_instream = nullptr;
#endif

    setCommandLine(commandLine);
}

void Process::addCommandLineArg(const std::string& arg) {
    m_commandLineArgs.add(arg);
}

void Process::addCommandLineArgs(std::initializer_list<std::string> args) {
    m_commandLineArgs.addAll(args);
}

std::string Process::commandLine() const {
    std::ostringstream out;
    bool first = true;
    for (const std::string& arg : m_commandLineArgs) {
        if (!first) {
            out << " ";
        }
        out << arg;
        first = false;
    }
    return out.str();
}

int Process::exitCode() const {
    return m_exitCode;
}

std::string Process::output() const {
    return m_outputStream.str();
}

int Process::pid() const {
    // TODO: lookup if running
    return m_pid;
}

bool Process::running() const {
    // TODO
    return m_running || (m_instream != nullptr && !m_instream->eof());
}

void Process::setCommandLine(const std::string& commandLine) {
    m_commandLineArgs.clear();
    m_commandLineArgs.addAll(stringSplit(commandLine, " "));
}

void Process::setCommandLineArgs(std::initializer_list<std::string> args) {
    m_commandLineArgs.clear();
    m_commandLineArgs.addAll(args);
}

void Process::setInput(const std::string& input) {
    m_input = input;
}

void Process::setTimeout(int timeoutMS) {
    m_timeoutMS = timeoutMS;
}

void Process::setWorkingDirectory(const std::string& workingDirectory) {
    m_workingDirectory = workingDirectory;
}

void Process::start() {
    if (running()) {
        return;
    }

    m_running = true;
    m_outputStream.str("");

#ifdef _WIN32
    // TODO
#else // _WIN32
    // *nix/Apple systems
    // merge stdout and stderr
    m_instream = new redi::ipstream(commandLine(),
                                    redi::pstreams::pstdout | redi::pstreams::pstderr);
    m_pid = m_instream->pid();
#endif // _WIN32
}

void Process::startAndWait() {
    start();
    wait();
}

void Process::stop() {
    if (!running()) {
        return;
    }

#ifdef _WIN32
    // TODO
#else // _WIN32
    if (!m_instream) {
        return;
    }
    if (m_pid > 0) {
        kill(m_pid);
    }
    m_instream->close();
    m_exitCode = WEXITSTATUS(m_instream->rdbuf()->status());
    delete m_instream;
    m_instream = nullptr;
#endif // _WIN32

    m_running = false;
}

bool Process::terminated() const {
    return m_terminated || (m_instream != nullptr && m_instream->eof());
}

int Process::timeout() const {
    return m_timeoutMS;
}

bool Process::timedOut() const {
    return m_timedOut;
}

std::string Process::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

void Process::wait(int ms) {
    if (!running()) {
        return;
    }
    if (ms <= 0) {
        ms = m_timeoutMS;
    }

#ifdef _WIN32

#else // _WIN32
    long startTime = getCurrentTimeMillis();

    while (true) {
        if (m_instream->rdbuf()->in_avail()) {
            int ch = m_instream->get();
            if (ch < 0) {
                if (m_instream->eof()) {
                    m_terminated = true;
                    m_instream->close();
                    m_exitCode = WEXITSTATUS(m_instream->rdbuf()->status());
                    delete m_instream;
                    m_instream = nullptr;
                    break;
                } else {
                }
            } else {
                m_outputStream.put(ch);
            }
        } else {
            // no input available; wait for a short time
            usleep(10000);
            if (ms > 0) {
                long elapsed = getCurrentTimeMillis() - startTime;
                if (elapsed >= ms) {
                    m_terminated = false;
                    m_timedOut = true;
                    break;
                }
            }
        }
    }
#endif // _WIN32
}

const std::string& Process::workingDirectory() const {
    return m_workingDirectory;
}

std::ostream& operator <<(std::ostream& out, const Process& process) {
    out << "Process{"
        << "pid=" << process.pid()
        << ",commandLine=\"" << process.commandLine() << "\""
        << ",running=" << std::boolalpha << process.running()
        << ",terminated=" << std::boolalpha << process.terminated()
        << ",timedOut=" << std::boolalpha << process.timedOut()
        << "}";
    return out;
}
#endif // PROCESS_H_ENABLED   // won't be enabled
