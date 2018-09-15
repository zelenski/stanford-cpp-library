/*
 * File: cmdscan.h
 * ---------------
 * This interface exports a simple command-line scanner.
 */

#ifndef _cmdscan_h
#define _cmdscan_h

#include "cslib.h"
#include "tokenscanner.h"

/*
 * Type: CommandScanner
 * --------------------
 * The <code>CommandScanner</code> type offers a simple command-scanning
 * abstraction that is primarily used to construct test programs.  The
 * typical pattern of use requires the following steps:
 *
 *<ol>
 * <li>Call <code>newCommandScanner</code> to create an empty command scanner.
 * <li>Call <code>defineCommand</code> to associate commands with functions.
 * <li>Call <code>commandLoop</code> to execute a command loop.
 *</ol>
 *
 * Command scanners are implemented as a collection type, so that
 * iterating over the scanner returns the name of each command in
 * lexicographic order.
 */

typedef struct CommandScannerCDT *CommandScanner;

/*
 * Type: CommandFn
 * ---------------
 * This type represents the space of functions that can be used
 * as commands.  Each command takes the entire command scanner
 * as an argument, which gives the callback function access to
 * the various data fields associated with the scanner.
 */

typedef void (*CommandFn)(CommandScanner cs);

/*
 * Function: newCommandScanner
 * Usage: cs = newCommandScanner();
 * --------------------------------
 * Allocates a new command scanner.
 */

CommandScanner newCommandScanner(void);

/*
 * Function: freeCommandScanner
 * Usage: freeCommandScanner(cs);
 * ------------------------------
 * Frees the specified command scanner.
 */

void freeCommandScanner(CommandScanner cs);

/*
 * Function: defineCommand
 * Usage: defineCommand(cs, cmdName, cmdFn);
 * -----------------------------------------
 * Adds an entry to the internal command table for the command scanner
 * by associating the specified command name with the corresponding function.
 */

void defineCommand(CommandScanner cs, string cmdName, CommandFn cmdFn);

/*
 * Function: commandLoop
 * Usage: commandLoop(cs, prompt);
 * -------------------------------
 * Executes a loop that reads and executes commands from the user.  On
 * each cycle, <code>commandLoop</code> performs the following operations:
 *
 * <p><ol>
 * <li>Prints the specified prompt
 * <li>Reads in a line from the user
 * <li>Checks to see if the first token on the line is a command
 * <li>Executes the function associated with that command
 * </ol><p>
 *
 * If a command is undefined, <code>commandLoop</code> displays a message
 * to that effect and allows the user to enter a new command.  If any
 * errors occur in the command processing, they are caught by the
 * command loop.
 */

void commandLoop(CommandScanner cs, string prompt);

/*
 * Function: executeCommand
 * Usage: ok = executeCommand(cs, line);
 * -------------------------------------
 * Executes a command line as if it were entered by the user.
 * The function returns <code>true</code> if the command is
 * defined, and <code>false</code> otherwise.
 */

bool executeCommand(CommandScanner cs, string line);

/*
 * Function: setCommandData
 * Usage: setCommandData(cs, data);
 * --------------------------------
 * Associates a data block with the command scanner.  The pointer to the
 * data block is passed to the callback functions for the various commands.
 */

void setCommandData(CommandScanner cs, void *data);

/*
 * Function: getCommandData
 * Usage: data = getCommandData(cs);
 * ---------------------------------
 * Returns the data block associated with the command scanner.
 */

void *getCommandData(CommandScanner cs);

/*
 * Function: readCommandToken
 * Usage: token = readCommandToken(cs);
 * ------------------------------------
 * Reads the next token from the command line, which is returned as a
 * string.  If no more tokens exist, <code>readCommandToken</code>
 * returns the empty string.
 */

string readCommandToken(CommandScanner cs);

/*
 * Function: getCommandLine
 * Usage: line = getCommandLine(cs);
 * ---------------------------------
 * Returns the current command line.
 */

string getCommandLine(CommandScanner cs);

/*
 * Function: getCommandName
 * Usage: name = getCommandName(cs);
 * ---------------------------------
 * Returns the current command name.
 */

string getCommandName(CommandScanner cs);

/*
 * Function: getTokenScanner
 * Usage: scanner = getTokenScanner(cs);
 * -------------------------------------
 * Returns the <code>TokenScanner</code> used internally to parse tokens.
 */

TokenScanner getTokenScanner(CommandScanner cs);

/*
 * Function: quitCommand
 * Usage: defineCommand(cs, "quit", quitCommand);
 * ----------------------------------------------
 * This callback function exits from the command scanner.
 */

void quitCommand(CommandScanner cs);

#endif
