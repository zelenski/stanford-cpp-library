/*
 * File: process.h
 * ---------------
 * This file declares a Process class to help launch external processes in
 * a platform-neutral way.
 *
 * NOTE: THIS IMPLEMENTATION IS INCOMPLETE AND CURRENTLY DISABLED.
 * 
 * @author Marty Stepp
 * @version 2017/10/07
 * - initial version
 * @since 2017/10/07
 */

#ifdef PROCESS_H_ENABLED   // won't be enabled
#ifndef _process_h
#define _process_h

#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include "pstream.h"
#include "vector.h"

/**
 * A Process object contains functionality to help launch an external process
 * in a platform-neutral way.
 *
 * Example usage:
 *
 * <pre>
 * Process proc("/whatever/myprog -a -b -c foo.txt");
 * proc.setTimeout(5000);
 * proc.startAndWait();
 * string output = proc.output();
 * ...
 * </pre>
 */
class Process {
public:
    static const int TIMEOUT_MS_DEFAULT;

    /**
     * Kills the process with the given PID and sends it the given signal.
     * If there is no such process, has no effect.
     */
    static void kill(int pid, int sig = 9);

    /**
     * Runs the given command as an external process, captures its console output,
     * and stores the output as a string in the 'output' parameter.
     * If timeoutMS > 0 is passed, times out the process after the given number of ms.
     * Returns the process's integer exit code.
     * If the process times out, returns -1.
     */
    static int runAndCaptureOutput(const std::string& commandLine,
                                   std::string& output,
                                   int timeout = 0);

    /**
     * Constructs a new process wrapper.
     */
    Process(const std::string& commandLine = "");

    /**
     * Adds the given command-line argument to this process's command line.
     */
    void addCommandLineArg(const std::string& arg);

    /**
     * Adds the given command-line arguments to this process's command line.
     */
    void addCommandLineArgs(std::initializer_list<std::string> args);

    /**
     * Returns a single string containing all of the command-line arguments
     * to this process, separated by spaces.
     */
    std::string commandLine() const;

    /**
     * Returns the system process exit code returned by this process on termination.
     * Most processes return 0 for a success code and non-0 for failure codes.
     * If the process has not yet terminated, returns 0.
     */
    int exitCode() const;

    /**
     * Returns any console output that was printed to stdout by this process.
     * If the process has not yet run or produced no output, returns an empty string.
     */
    std::string output() const;

    /**
     * Returns the process ID for this process.
     * This method is OS-dependent and actually only works on *nix systems.
     * If it is called on non-*nix systems, it will return -1.
     * You must start the process (as per start()) before calling this function,
     * else the process doesn't exist yet, so this will return -1.
     */
    int pid() const;

    /**
     * Returns true if this process has been started, is still running,
     * and hasn't been stopped/killed yet.
     */
    bool running() const;

    /**
     * Sets the complete command-line for this process, splitting the given
     * string by spaces into individual process arguments.
     */
    void setCommandLine(const std::string& commandLine);

    /**
     * Sets the command-line arguments for this process.
     * Similar to addCommandLineArgs except that this replaces any args that were there before.
     */
    void setCommandLineArgs(std::initializer_list<std::string> args);

    /**
     * Sets console input that should be fed to the process's standard input (stdin/cin).
     */
    void setInput(const std::string& input);

    /**
     * Sets the number of milliseconds until this process should be forcibly stopped and killed.
     */
    void setTimeout(int timeout);

    /**
     * Sets the current/working directory that this process should use.
     */
    void setWorkingDirectory(const std::string& workingDirectory);

    /**
     * Runs this process.
     * This call is non-blocking; the other process runs in the background.
     * After calling this, getPID() should work.
     */
    void start();

    /**
     * Runs this process and waits for its given timeout for it to finish running.
     * If no timeout has been specified, waits indefinitely.
     * Equivalent to calling start() and then wait().
     */
    void startAndWait();

    /**
     * Forcibly terminates this process, sending it a 'kill' signal in the operating system.
     */
    void stop();

    /**
     * Whether this process has finished running.
     * If the process has not been run yet, returns false.
     */
    bool terminated() const;

    /**
     * The number of milliseconds that this process will be allowed to run before killing it.
     * By default, this is 0 ms, indicating no timeout enforced.
     */
    int timeout() const;

    /**
     * Whether this process exceeded its timeout.
     * If the process has not been run or has not terminated yet, returns false.
     */
    bool timedOut() const;

    /**
     * Returns a string representation of this process.
     */
    std::string toString() const;

    /**
     * Lets the process run for the given number of milliseconds and waits
     * for it to complete.
     * If the ms is omitted or nonpositive, waits for an amount of ms equal
     * to the process's timeout setting.
     * If the process terminates, grabs its console output.
     * If it doesn't terminate, stops the process and marks it as timed out.
     */
    void wait(int ms = 0);

    /**
     * The current/working directory that this process should use.
     * Defaults to the system 'temp' directory as known to C++.
     */
    const std::string& workingDirectory() const;

private:
    Vector<std::string> m_commandLineArgs;
    std::string m_input;
    std::ostringstream m_outputStream;
    std::string m_workingDirectory;
    int m_exitCode;
    int m_pid;
    int m_timeoutMS;
    bool m_running;
    bool m_terminated;
    bool m_timedOut;

#ifdef _WIN32
    // TODO
#else // _WIN32
    redi::ipstream* m_instream;
#endif

    static long getCurrentTimeMillis();
};

/**
 * Displays a string representation of this process to the given output stream.
 */
std::ostream& operator <<(std::ostream& out, const Process& process);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _process_h
#endif // PROCESS_H_ENABLED   // won't be enabled
