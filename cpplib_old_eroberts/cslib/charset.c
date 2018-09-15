/*
 * File: charset.c
 * Last modified on Wed Dec 12 16:26:17 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the CharSet type defined in charset.h
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "charset.h"
#include "cslib.h"
#include "foreach.h"
#include "iterator.h"
#include "itertype.h"
#include "strlib.h"
#include "unittest.h"

/* Constants */

#ifdef _LP64
#  define BIT_SHIFT_SIZE 6
#  define BIT_ARRAY_SIZE 4
#  define BIT_SHIFT_MASK 0x3F
#else
#  define BIT_SHIFT_SIZE 5
#  define BIT_ARRAY_SIZE 8
#  define BIT_SHIFT_MASK 0x1F
#endif

/*
 * Type: CharSetCDT
 * ----------------
 * This type defines the concrete structure of a character set.
 */

struct CharSetCDT {
   IteratorHeader header;
   long bits[BIT_ARRAY_SIZE];
};

/* Private function prototypes */

static Iterator newCharSetIterator(void *collection);
static bool stepCharSetIterator(Iterator it, void *dst);
static void advanceCharSetIterator(Iterator it);

/* Exported entries */

/*
 * Function: newCharSet
 * Usage: set = newCharSet();
 * --------------------------
 * Creates an empty set of characters.
 */

CharSet newCharSet() {
   CharSet set;

   set = newBlock(CharSet);
   enableIteration(set, newCharSetIterator);
   clearCharSet(set);
   return set;
}

void freeCharSet(CharSet set) {
   freeBlock(set);
}

int sizeCharSet(CharSet set) {
   int ch, index, n;
   long mask;

   n = 0;
   index = 0;
   mask = 1L;
   for (ch = 0; ch < 256; ch++) {
      if ((set->bits[index] & mask) != 0) n++;
      mask <<= 1;
      if (mask == 0) {
         index++;
         mask = 1L;
      }
   }
   return n;
}

bool isEmptyCharSet(CharSet set) {
   int i;

   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      if (set->bits[i] != 0) return false;
   }
   return true;
}

void clearCharSet(CharSet set) {
   int i;

   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      set->bits[i] = 0;
   }
}

CharSet cloneCharSet(CharSet set) {
   CharSet newset;
   int i;

   newset = newBlock(CharSet);
   enableIteration(newset, newCharSetIterator);
   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      newset->bits[i] = set->bits[i];
   }
   return newset;
}

bool containsCharSet(CharSet set, char ch) {
   long mask;

   mask = 1L << (ch & BIT_SHIFT_MASK);
   return (set->bits[(ch & 0xFF) >> BIT_SHIFT_SIZE] & mask) != 0;
}

void addCharSet(CharSet set, char ch) {
   long mask;

   mask = 1L << (ch & BIT_SHIFT_MASK);
   set->bits[(ch & 0xFF) >> BIT_SHIFT_SIZE] |= mask;
}

void addString(CharSet set, string str) {
   int i, n;

   n = stringLength(str);
   for (i = 0; i < n; i++) {
      addCharSet(set, str[i]);
   }
}

void removeCharSet(CharSet set, char ch) {
   long mask;

   mask = 1L << (ch & BIT_SHIFT_MASK);
   set->bits[(ch & 0xFF) >> BIT_SHIFT_SIZE] &= ~mask;
}

bool equalsCharSet(CharSet s1, CharSet s2) {
   int i;

   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      if (s1->bits[i] != s2->bits[i]) return false;
   }
   return true;
}

bool isSubsetCharSet(CharSet s1, CharSet s2) {
   int i;

   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      if ((s1->bits[i] & ~s2->bits[i]) != 0) return false;
   }
   return true;
}

CharSet unionCharSet(CharSet s1, CharSet s2) {
   CharSet set;
   int i;

   set = newBlock(CharSet);
   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      set->bits[i] = s1->bits[i] | s2->bits[i];
   }
   return set;
}

CharSet intersectionCharSet(CharSet s1, CharSet s2) {
   CharSet set;
   int i;

   set = newCharSet();
   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      set->bits[i] = s1->bits[i] & s2->bits[i];
   }
   return set;
}

CharSet setDifferenceCharSet(CharSet s1, CharSet s2) {
   CharSet set;
   int i;

   set = newCharSet();
   for (i = 0; i < BIT_ARRAY_SIZE; i++) {
      set->bits[i] = s1->bits[i] & ~s2->bits[i];
   }
   return set;
}

/* Private functions */

static Iterator newCharSetIterator(void *collection) {
   Iterator it;

   it = newStepIterator(1, stepCharSetIterator);
   setCollection(it, collection);
   advanceCharSetIterator(it);
   return it;
}

static bool stepCharSetIterator(Iterator it, void *dst) {
   int *cp;

   cp = (int *) getIteratorData(it);
   if (cp == NULL) return false;
   *((char *) dst) = (char) *cp;
   advanceCharSetIterator(it);
   return true;
}

static void advanceCharSetIterator(Iterator it) {
   CharSet set;
   int *cp;
   long mask;

   set = (CharSet) getCollection(it);
   cp = (int *) getIteratorData(it);
   if (cp == NULL) {
      cp = newBlock(int *);
      *cp = -1;
   }
   while (++(*cp) < 256) {
      mask = 1L << (*cp & BIT_SHIFT_MASK);
      if (set->bits[*cp >> BIT_SHIFT_SIZE] & mask) break;
   }
   if (*cp == 256) {
      freeBlock(cp);
      setIteratorData(it, NULL);
   }
}

/**********************************************************************/
/* Unit test for the charset module                                   */
/**********************************************************************/

#ifndef _NOTEST_

/* Private function prototypes */

static void testStringCharSet(void);
static void testCharacterCharSet(void);
static CharSet createCharSet(string str);

/* Unit test */

void testCharSetModule(void) {
   CharSet vowels, consonants, lowercase, onePointTiles, onePointConsonants;
   CharSet set;

   trace(vowels = createCharSet("aeiou"));
   trace(consonants = createCharSet("bcdfghjklmnpqrstvwxyz"));
   trace(lowercase = createCharSet("abcdefghijklmnopqrstuvwxyz"));
   trace(onePointTiles = createCharSet("etaoinsrlu"));
   test(equals(onePointTiles, createCharSet("aeilnorstu")), true);
   test(isSubset(vowels, onePointTiles), true);
   test(isSubset(consonants, onePointTiles), false);
   test(equals(union(vowels, consonants), lowercase), true);
   trace(onePointConsonants = intersection(consonants, onePointTiles));
   test(equals(onePointConsonants, createCharSet("lnrst")), true);
   test(equals(setDifference(lowercase, consonants), vowels), true);
   trace(set = clone(vowels));
   test(equals(set, vowels), true);
   trace(remove(set, 'o'));
   test(equals(vowels, createCharSet("aeiou")), true);
   test(equals(set, createCharSet("aeiu")), true);
}

static CharSet createCharSet(string str) {
   CharSet set;

   set = newCharSet();
   addString(set, str);
   return set;
}

#endif
