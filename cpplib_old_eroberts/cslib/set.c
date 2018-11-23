/*
 * File: set.c
 * Last modified on Wed Dec 12 16:33:08 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the set abstraction defined in set.h.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"
#include "cmpfn.h"
#include "cslib.h"
#include "exception.h"
#include "foreach.h"
#include "generic.h"
#include "iterator.h"
#include "itertype.h"
#include "map.h"
#include "set.h"
#include "strlib.h"
#include "unittest.h"

/*
 * Type: SetCDT
 * ------------
 * This type defines the concrete structure of a set.
 */

struct SetCDT {
   IteratorHeader header;      /* Header to enable iteration            */
   BST bst;                    /* The underlying binary search tree     */
   string baseType;            /* The name of the base type             */
   int baseTypeSize;           /* Size of the base type in bytes        */
   CompareFn cmpFn;            /* Function to compare two keys          */
   FetchFn fetchFn;            /* Function to fetch an argument         */
   StoreFn storeFn;            /* Function to store a value             */
   ToStringFn toStringFn;      /* Function to convert key to a string   */
};

/* Private function prototypes */

static string checkBaseTypes(Set s1, Set s2);
static Iterator newSetIterator(void *collection);
static bool stepSetIterator(Iterator iterator, void *dst);

/* Exported entries */

Set newSetFromType(string baseType) {
   Set set;

   set = newBlock(Set);
   enableIteration(set, newSetIterator);
   set->baseType = baseType;
   set->fetchFn = getFetchFnForType(baseType);
   set->storeFn = getStoreFnForType(baseType);
   set->cmpFn = getCompareFnForType(baseType);
   set->toStringFn = getToStringFn(baseType);
   set->baseTypeSize = getTypeSizeForType(baseType);
   set->bst = newBSTFromType(baseType);
   return set;
}

void freeSet(Set set) {
   freeBST(set->bst);
   freeBlock(set);
}

int sizeSet(Set set) {
   return sizeBST(set->bst);
}

bool isEmptySet(Set set) {
   return isEmptyBST(set->bst);
}

void clearSet(Set set) {
   clearBST(set->bst);
}

Set cloneSet(Set set) {
   Set newset;

   newset = newBlock(Set);
   enableIteration(newset, newSetIterator);
   newset->baseType = set->baseType;
   newset->baseTypeSize = set->baseTypeSize;
   newset->fetchFn = set->fetchFn;
   newset->storeFn = set->storeFn;
   newset->cmpFn = set->cmpFn;
   newset->bst = cloneBST(set->bst);
   return newset;
}

bool containsSet(Set set, ...) {
   va_list args;
   bool result;

   va_start(args, set);
   result = containsSetFromArgs(set, args);
   va_end(args);
   return result;
}

bool containsSetFromArgs(Set set, va_list args) {
   GenericType any;

   set->fetchFn(args, &any);
   return findBSTNodeFromArg(set->bst, any) != NULL;
}

void addSet(Set set, ...) {
   va_list args;

   va_start(args, set);
   addSetFromArgs(set, args);
   va_end(args);
}

void addSetFromArgs(Set set, va_list args) {
   GenericType any;

   set->fetchFn(args, &any);
   insertBSTNodeFromArg(set->bst, any);
}

void removeSet(Set set, ...) {
   va_list args;

   va_start(args, set);
   removeSetFromArgs(set, args);
   va_end(args);
}

void removeSetFromArgs(Set set, va_list args) {
   GenericType any;

   set->fetchFn(args, &any);
   removeBSTNodeFromArg(set->bst, any);
}

bool equalsSet(Set s1, Set s2) {
   checkBaseTypes(s1, s2);
   return isSubset(s1, s2) && isSubset(s2, s1);
}

bool isSubsetSet(Set s1, Set s2) {
   Iterator it;
   BSTNode node;
   bool result;

   checkBaseTypes(s1, s2);
   result = true;
   it = newNodeIterator(s1->bst, INORDER);
   while (result && stepIterator(it, &node)) {
      if (!findBSTNodeFromArg(s2->bst, getKey(node))) result = false;
   }
   freeIterator(it);
   return result;
}

Set unionSet(Set s1, Set s2) {
   Iterator it;
   BSTNode node;
   Set set;

   set = newSetFromType(checkBaseTypes(s1, s2));
   setCompareFn(set, getCompareFn(s1));
   it = newNodeIterator(s1->bst, INORDER);
   while (stepIterator(it, &node)) {
      insertBSTNodeFromArg(set->bst, getKey(node));
   }
   freeIterator(it);
   it = newNodeIterator(s2->bst, INORDER);
   while (stepIterator(it, &node)) {
      insertBSTNodeFromArg(set->bst, getKey(node));
   }
   freeIterator(it);
   return set;
}

Set intersectionSet(Set s1, Set s2) {
   Iterator it;
   GenericType any;
   BSTNode node;
   Set set;

   set = newSetFromType(checkBaseTypes(s1, s2));
   setCompareFn(set, getCompareFn(s1));
   it = newNodeIterator(s1->bst, INORDER);
   while (stepIterator(it, &node)) {
      any = getKey(node);
      if (findBSTNodeFromArg(s2->bst, any) != NULL) {
         insertBSTNodeFromArg(set->bst, any);
      }
   }
   freeIterator(it);
   return set;
}

Set setDifferenceSet(Set s1, Set s2) {
   Iterator it;
   BSTNode node;
   GenericType any;
   Set set;

   set = newSetFromType(checkBaseTypes(s1, s2));
   setCompareFn(set, getCompareFn(s1));
   it = newNodeIterator(s1->bst, INORDER);
   while (stepIterator(it, &node)) {
      any = getKey(node);
      if (findBSTNodeFromArg(s2->bst, any) == NULL) {
         insertBSTNodeFromArg(set->bst, any);
      }
   }
   freeIterator(it);
   return set;
}

void setCompareFn(Set set, CompareFn cmpFn) {
   set->cmpFn = cmpFn;
   setCompareFnBST(set->bst, cmpFn);
}

CompareFn getCompareFn(Set set) {
   return getCompareFnBST(set->bst);
}

/* Private functions */

static string checkBaseTypes(Set s1, Set s2) {
   string type;

   type = getBaseTypeBST(s1->bst);
   if (!stringEqual(type, getBaseTypeBST(s2->bst))) {
      error("Sets have different base types");
   }
   if (getCompareFnBST(s1->bst) != getCompareFnBST(s2->bst)) {
      error("Sets have different comparison functions");
   }
   return type;
}

static Iterator newSetIterator(void *collection) {
   Iterator iterator;
   Set set;

   set = (Set) collection;
   iterator = newStepIterator(set->baseTypeSize, stepSetIterator);
   setCollection(iterator, collection);
   setIteratorData(iterator, newNodeIterator(set->bst, INORDER));
   return iterator;
}

static bool stepSetIterator(Iterator iterator, void *dst) {
   Set set;
   BSTNode node;
   Iterator nodeIterator;
   bool result;

   set = (Set) getCollection(iterator);
   nodeIterator = (Iterator) getIteratorData(iterator);
   result = stepIterator(nodeIterator, &node);
   if (result) {
      set->storeFn(getKey(node), dst);
   } else {
      freeIterator(nodeIterator);
   }
   return result;
}

/**********************************************************************/
/* Unit test for the set module                                       */
/**********************************************************************/

#ifndef _NOTEST_

/* Constant tables */

static string OSPD3[] = {
   "aa", "ab", "ad", "ae", "ag", "ah", "ai", "al", "am", "an", "ar",
   "as", "at", "aw", "ax", "ay", "ba", "be", "bi", "bo", "by", "de",
   "do", "ed", "ef", "eh", "el", "em", "en", "er", "es", "et", "ex",
   "fa", "go", "ha", "he", "hi", "hm", "ho", "id", "if", "in", "is",
   "it", "jo", "ka", "la", "li", "lo", "ma", "me", "mi", "mm", "mo",
   "mu", "my", "na", "ne", "no", "nu", "od", "oe", "of", "oh", "om",
   "on", "op", "or", "os", "ow", "ox", "oy", "pa", "pe", "pi", "re",
   "sh", "si", "so", "ta", "ti", "to", "uh", "um", "un", "up", "us",
   "ut", "we", "wo", "xi", "xu", "ya", "ye", "yo"
};
static int N_OSPD3 = sizeof OSPD3 / sizeof OSPD3[0];

static string OSPD4[] = {
   "aa", "ab", "ad", "ae", "ag", "ah", "ai", "al", "am", "an", "ar",
   "as", "at", "aw", "ax", "ay", "ba", "be", "bi", "bo", "by", "de",
   "do", "ed", "ef", "eh", "el", "em", "en", "er", "es", "et", "ex",
   "fa", "fe", "go", "ha", "he", "hi", "hm", "ho", "id", "if", "in",
   "is", "it", "jo", "ka", "ki", "la", "li", "lo", "ma", "me", "mi",
   "mm", "mo", "mu", "my", "na", "ne", "no", "nu", "od", "oe", "of",
   "oh", "oi", "om", "on", "op", "or", "os", "ow", "ox", "oy", "pa",
   "pe", "pi", "qi", "re", "sh", "si", "so", "ta", "ti", "to", "uh",
   "um", "un", "up", "us", "ut", "we", "wo", "xi", "xu", "ya", "ye",
   "yo", "za"
};
static int N_OSPD4 = sizeof OSPD4 / sizeof OSPD4[0];

static string NEW_WORDS[] = {
   "fe", "ki", "oi", "qi", "za"
};
static int N_NEW_WORDS = sizeof NEW_WORDS / sizeof NEW_WORDS[0];

/* Private function prototypes */

static void testCharacterSet(void);
static Set createCharSet(string str);
static void testStringSet();
static void checkStringSet(Set set, string array[]);
static void addStringsToSet(Set set, string array[], int n);
static void testIntegerSet();
static Set createDigitSet(string str);

/* Unit test */

void testSetModule(void) {
   testCharacterSet();
   testStringSet();
   testIntegerSet();
}

static void testCharacterSet(void) {
   Set vowels, consonants, lowercase, onePointTiles, onePointConsonants;

   trace(vowels = createCharSet("aeiou"));
   trace(consonants = createCharSet("bcdfghjklmnpqrstvwxyz"));
   trace(lowercase = createCharSet("abcdefghijklmnopqrstuvwxyz"));
   trace(onePointTiles = createCharSet("etaoinsrlu"));
   trace(equals(onePointTiles, createCharSet("aeilnorstu")));
   test(isSubset(vowels, onePointTiles), true);
   test(isSubset(consonants, onePointTiles), false);
   test(equals(union(vowels, consonants), lowercase), true);
   trace(onePointConsonants = intersection(consonants, onePointTiles));
   test(equals(onePointConsonants, createCharSet("lnrst")), true);
   test(equals(setDifference(lowercase, consonants), vowels), true);
}

static Set createCharSet(string str) {
   Set set;
   int i;

   set = newSet(char);
   for (i = 0; i < stringLength(str); i++) {
      add(set, str[i]);
   }
   return set;
}

static void testStringSet() {
   Set ospd3, ospd4, newWords, set;

   trace(ospd3 = newSet(string));
   test(isEmpty(ospd3), true);
   trace(addStringsToSet(ospd3, OSPD3, N_OSPD3));
   test(isEmpty(ospd3), false);
   test(size(ospd3), (int) N_OSPD3);
   trace(checkStringSet(ospd3, OSPD3));
   trace(ospd4 = newSet(string));
   trace(addStringsToSet(ospd4, OSPD4, N_OSPD4));
   trace(newWords = newSet(string));
   trace(set = clone(ospd4));
   test(equals(set, ospd4), true);
   trace(addStringsToSet(newWords, NEW_WORDS, N_NEW_WORDS));
   test(equals(union(ospd3, newWords), ospd4), true);
   test(equals(intersection(ospd3, ospd4), ospd3), true);
   test(equals(setDifference(ospd4, ospd3), newWords), true);
   test(isSubset(ospd3, ospd4), true);
   test(isSubset(ospd4, ospd3), false);
}

static void checkStringSet(Set set, string array[]) {
   string str;
   int index;

   index = 0;
   foreach (str in set) {
      if (!stringEqual(str, array[index++])) {
         reportError("Incorrect value: %s", str);
      }
   }
}

static void addStringsToSet(Set set, string array[], int n) {
   int i;

   for (i = 0; i < n; i++) {
      add(set, array[i]);
   }
}

static void testIntegerSet() {
   Set primes, evens, odds, set;

   trace(primes = createDigitSet("2357"));
   trace(evens = createDigitSet("02468"));
   trace(odds = createDigitSet("13579"));
   test(equals(union(evens, primes), createDigitSet("02345678")), true);
   test(equals(intersection(evens, primes), createDigitSet("2")), true);
   test(isEmpty(intersection(odds, evens)), true);
   test(equals(setDifference(primes, evens), createDigitSet("357")), true);
   trace(set = clone(primes));
   test(equals(set, primes), true);
}

static Set createDigitSet(string str) {
   Set set;
   int i, n;

   set = newSet(int);
   n = stringLength(str);
   for (i = 0; i < n; i++) {
      add(set, str[i] - '0');
   }
   return set;
}

#endif
