/*
 * File: cmdscan.c
 * Last modified on Mon Dec  3 01:43:13 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the cmdscan.h interface.
 */

#include <stdio.h>
#include "cmdscan.h"
#include "cslib.h"
#include "exception.h"
#include "generic.h"
#include "hashmap.h"
#include "iterator.h"
#include "itertype.h"
#include "simpio.h"
#include "strlib.h"
#include "tokenscanner.h"

/*
 * Type: CommandScannerCDT
 * -----------------------
 * This type defines the concrete structure of the command scanner.
 */

struct CommandScannerCDT {
   IteratorHeader header;
   string line;
   string cmdName;
   HashMap cmdTable;
   TokenScanner scanner;
   void *data;
   bool quit;
};

/*
 * Type: CommandEntry
 * ------------------
 * This type defines the entry for an individual command.
 */

typedef struct {
   string cmdName;
   CommandFn cmdFn;
} *CommandEntry;

/* Private function prototypes */

static Iterator newCommandIterator(void *collection);
static void addCommandToIterator(string key, void *value, void *data);

/* Exported entries */

CommandScanner newCommandScanner(void) {
   CommandScanner cs;

   cs = newBlock(CommandScanner);
   cs->cmdTable = newHashMap();
   cs->scanner = newTokenScanner();
   ignoreSpaces(cs->scanner);
   cs->data = NULL;
   cs->quit = false;
   return cs;
}

void freeCommandScanner(CommandScanner cs) {
   freeSymbolTable(cs->cmdTable);
   freeTokenScanner(cs->scanner);
   freeBlock(cs);
}

void defineCommand(CommandScanner cs, string cmdName, CommandFn cmdFn) {
   CommandEntry entry;

   entry = newBlock(CommandEntry);
   entry->cmdName = copyString(cmdName);
   entry->cmdFn = cmdFn;
   put(cs->cmdTable, cmdName, entry);
}

void setCommandData(CommandScanner cs, void *data) {
   cs->data = data;
}

void *getCommandData(CommandScanner cs) {
   return cs->data;
}

string readCommandToken(CommandScanner cs) {
   return nextToken(cs->scanner);
}

string getCommandLine(CommandScanner cs) {
   return copyString(cs->line);
}

string getCommandName(CommandScanner cs) {
   return copyString(cs->cmdName);
}

TokenScanner getTokenScanner(CommandScanner cs) {
   return cs->scanner;
}

void commandLoop(CommandScanner cs, string prompt) {
   while (!cs->quit) {
      printf("%s", prompt);
      try {
         if (!executeCommand(cs, getLine())) {
            printf("Illegal command: %s\n", cs->line);
         }
      } catch (ErrorException) {
         printf("error: %s\n", (string) getExceptionValue());
      } endtry
   }
}

bool executeCommand(CommandScanner cs, string line) {
   CommandEntry entry;

   cs->line = line;
   setInputString(cs->scanner, line);
   cs->cmdName = nextToken(cs->scanner);
   entry = (CommandEntry) get(cs->cmdTable, cs->cmdName);
   if (entry == NULL) return false;
   entry->cmdFn(cs);
   return true;
}

void quitCommand(CommandScanner cs) {
   cs->quit = true;
}

/* Private functions to enable iteration */

/*
 * Implementation notes: newCommandIterator, addCommandToIterator
 * --------------------------------------------------------------
 * These functions implement the polymorphic iterator facility
 * for command scanners.  For details on the general strategy,
 * see the comments in itertype.h interface.
 */

static Iterator newCommandIterator(void *collection) {
   CommandScanner cs;
   Iterator iterator;

   cs = (CommandScanner) collection;
   iterator = newListIterator(sizeof (string), stringCmpFn);
   mapSymbolTable(addCommandToIterator, cs->cmdTable, iterator);
   return iterator;
}

static void addCommandToIterator(string key, void *value, void *data) {
   string newCommand;

   newCommand = copyString(key);
   addToIteratorList((Iterator) data, &newCommand);
}
