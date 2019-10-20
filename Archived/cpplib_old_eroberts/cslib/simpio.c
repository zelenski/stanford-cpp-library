/*
 * File: simpio.c
 * Last modified on Thu Jul 15 11:43:07 2010 by eroberts
 * -----------------------------------------------------
 * This file implements the simpio.h interface.
 */

#include <stdio.h>
#include <string.h>
#include "cslib.h"
#include "simpio.h"
#include "strlib.h"

/*
 * Constants:
 * ----------
 * INITIAL_BUFFER_SIZE -- Initial buffer size
 */

#define INITIAL_BUFFER_SIZE 128

/* Exported entries */

/*
 * Functions: getInteger, getLong, getReal
 * ---------------------------------------
 * These functions first read a line and then call sscanf to
 * translate the number.  Reading an entire line is essential to
 * good error recovery, because the characters after the point of
 * error would otherwise remain in the input buffer and confuse
 * subsequent input operations.  The sscanf line allows white space
 * before and after the number but no other extraneous characters.
 */

int getInteger(void) {
   string line;
   int value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getInteger: unexpected end of file");
      switch (sscanf(line, " %d %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter an integer\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

long getLong(void) {
   string line;
   long value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getLong: unexpected end of file");
      switch (sscanf(line, " %ld %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter an integer\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

double getReal(void) {
   string line;
   double value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getReal: unexpected end of file");
      switch (sscanf(line, " %lf %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter a real number\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

/*
 * Function: getLine
 * -----------------
 * This function is a simple wrapper; all the work is done by
 * readLine.
 */

string getLine(void) {
   return readLine(stdin);
}

/*
 * Function: readLine
 * ------------------
 * This function operates by reading characters from the file
 * into a dynamically allocated buffer.  If the buffer becomes
 * full before the end of the line is reached, a new buffer
 * twice the size of the previous one is allocated.  This
 * function works correctly with any of the standard newline
 * sequences: "\n", "\r", or "\r\n".
 */

string readLine(FILE *infile) {
   string line, nline;
   int n, ch, size;

   n = 0;
   size = INITIAL_BUFFER_SIZE;
   line = (string) getBlock(size + 1);
   while (true) {
      ch = getc(infile);
      if (ch == '\n' || ch == EOF) break;
      if (ch == '\r') {
         ch = getc(infile);
         if (ch != '\n') ungetc(ch, infile);
         break;
      }
      if (n == size) {
         size *= 2;
         nline = (string) getBlock(size + 1);
         strncpy(nline, line, n);
         freeBlock(line);
         line = nline;
      }
      line[n++] = ch;
   }
   if (n == 0 && ch == EOF) {
      freeBlock(line);
      return NULL;
   }
   line[n] = '\0';
   nline = (string) getBlock(n + 1);
   strcpy(nline, line);
   freeBlock(line);
   return nline;
}

/*
 * Function: readLinesFromStream
 * -----------------------------
 * This function operates by reading lines from the file into a
 * dynamically allocated buffer that doubles in size whenever the
 * existing space is exhausted.
 */

string *readLinesFromStream(FILE *infile) {
   string *buffer, *nbuffer, line;
   int i, n, size;

   n = 0;
   size = INITIAL_BUFFER_SIZE;
   buffer = newArray(size, string);
   while (true) {
      line = readLine(infile);
      if (line == NULL) break;
      if (n == size) {
         size *= 2;
         nbuffer = newArray(size, string);
         for (i = 0; i < n; i++) {
            nbuffer[i] = buffer[i];
         }
         freeBlock(buffer);
         buffer = nbuffer;
      }
      buffer[n++] = line;
   }
   nbuffer = newArray(n + 1, string);
   for (i = 0; i < n; i++) {
      nbuffer[i] = buffer[i];
   }
   nbuffer[n] = NULL;
   freeBlock(buffer);
   return nbuffer;
}

/*
 * Function: readLinesFromFile
 * ---------------------------
 * This function is a wrapper for <code>readLinesFromStream</code>,
 * which does all the real work.
 */

string *readLinesFromFile(string filename) {
   FILE *infile;
   string *lines;

   if (stringEqual(filename, "-")) {
      infile = stdin;
   } else {
      infile = fopen(filename, "r");
      if (infile == NULL) error("Can't open %s", filename);
   }
   lines = readLinesFromStream(infile);
   if (infile != stdin) fclose(infile);
   return lines;
}
