/*
 * File: tokenpatch.h
 * ------------------
 * This file renames TokenType and WORD to avoid conflict with the
 * <windows.h> header.
 */

#ifdef _MSC_VER
#  define TokenType TokenTypeT
#  define WORD WORD_TC
#endif
