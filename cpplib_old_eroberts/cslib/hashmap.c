/*
 * File: hashmap.c
 * Last modified on Wed Dec 12 16:32:22 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the <code>HashMap</code> class.
 */

#include <stdio.h>
#include "cmpfn.h"
#include "cslib.h"
#include "exception.h"
#include "foreach.h"
#include "generic.h"
#include "hashmap.h"
#include "iterator.h"
#include "itertype.h"
#include "strlib.h"
#include "unittest.h"

/* Constants */

#define INITIAL_BUCKET_COUNT 101
#define HASH_SEED 5381
#define HASH_MULTIPLIER 33
#define HASH_MASK ((unsigned) -1 >> 1)

/*
 * Type: Cell
 * ----------
 * This type defines a linked list cell for the map.
 */

typedef struct Cell {
   string key;
   void *value;
   struct Cell *link;
} Cell;

/*
 * Type: HashMapCDT
 * ----------------
 * This type defines the underlying concrete representation for a
 * HashMap.  These details are not relevant to and therefore
 * not exported to the client.  In this implementation, the
 * underlying structure is a hash table organized as an array of
 * "buckets," in which each bucket is a linked list of elements
 * that share the same hash code.
 */

struct HashMapCDT {
   IteratorHeader header;
   Cell **buckets;
   int nBuckets;
   int count;
};

/* Private function prototypes */

static int hashCode(string str);
static void rehash(HashMap map, int nBuckets);
static void freeBucketChain(Cell *cp);
static Cell *findCell(Cell *cp, string s);
static Iterator newMapIterator(void *collection);
static void addKeyToIterator(string key, void *value, void *data);

/* Public entries */

HashMap newHashMap(void) {
   HashMap map;
   int i;

   map = newBlock(HashMap);
   enableIteration(map, newMapIterator);
   map->count = 0;
   rehash(map, INITIAL_BUCKET_COUNT);
   return map;
}

void freeHashMap(HashMap map) {
   clearHashMap(map);
   freeBlock(map);
}

int sizeHashMap(HashMap map) {
   return map->count;
}

bool isEmptyHashMap(HashMap map) {
   return map->count == 0;
}

void clearHashMap(HashMap map) {
   int i;

   for (i = 0; i < map->nBuckets; i++) {
      freeBucketChain(map->buckets[i]);
      map->buckets[i] = NULL;
   }
   map->count = 0;
}

HashMap cloneHashMap(HashMap map) {
   HashMap newmap;
   Cell *cp;
   int i;

   newmap = newHashMap();
   for (i = 0; i < map->nBuckets; i++) {
      for (cp = map->buckets[i]; cp != NULL; cp = cp->link) {
         putHashMap(newmap, cp->key, cp->value);
      }
   }
   return newmap;
}

void putHashMap(HashMap map, string key, void *value) {
   int bucket;
   Cell *cp;

   bucket = hashCode(key) % map->nBuckets;
   cp = findCell(map->buckets[bucket], key);
   if (cp == NULL) {
      cp = newBlock(Cell *);
      cp->key = copyString(key);
      cp->link = map->buckets[bucket];
      map->buckets[bucket] = cp;
      map->count++;
   }
   cp->value = value;
}

void *getHashMap(HashMap map, string key) {
   int bucket;
   Cell *cp;

   bucket = hashCode(key) % map->nBuckets;
   cp = findCell(map->buckets[bucket], key);
   if (cp == NULL) return NULL;
   return cp->value;
}

void removeHashMap(HashMap map, string key) {
   int bucket;
   Cell **cpp, *cp;

   bucket = hashCode(key) % map->nBuckets;
   cpp = &map->buckets[bucket];
   while (*cpp != NULL && !stringEqual((*cpp)->key, key)) {
      cpp = &(*cpp)->link;
   }
   if (*cpp != NULL) {
      cp = *cpp;
      *cpp = cp->link;
      freeBlock(cp->key);
      freeBlock(cp);
      map->count--;
   }
}

bool containsKeyHashMap(HashMap map, string key) {
   int bucket;

   bucket = hashCode(key) % map->nBuckets;
   return findCell(map->buckets[bucket], key) != NULL;
}

void mapHashMap(HashMap map, proc fn, void *data) {
   int i;
   Cell *cp;

   for (i = 0; i < map->nBuckets; i++) {
      for (cp = map->buckets[i]; cp != NULL; cp = cp->link) {
         if (cp->value != NULL) fn(cp->key, cp->value, data);
      }
   }
}

/* Private functions */

static int hashCode(string str) {
   unsigned hash;
   int i, n;

   hash = HASH_SEED;
   n = stringLength(str);
   for (i = 0; i < n; i++) {
      hash = HASH_MULTIPLIER * hash + str[i];
   }
   return (int) (hash & HASH_MASK);
}

static void rehash(HashMap map, int nBuckets) {
   Cell **oldBuckets, *cp, *np;
   int oldNBuckets, bucket, i;

   if (map->count != 0) {
      oldBuckets = map->buckets;
      oldNBuckets = map->nBuckets;
   }
   map->buckets = newArray(nBuckets, Cell *);
   map->nBuckets = nBuckets;
   for (i = 0; i < nBuckets; i++) {
      map->buckets[i] = NULL;
   }
   if (map->count != 0) {
      for (i = 0; i < oldNBuckets; i++) {
         cp = oldBuckets[i];
         while (cp != NULL) {
            np = cp->link;
            bucket = hashCode(cp->key) % nBuckets;
            cp->link = map->buckets[bucket];
            map->buckets[bucket] = cp;
            cp = np;
         }
      }
   }
}

/*
 * Implementation notes: freeBucketChain
 * -------------------------------------
 * This function takes a chain pointer and frees all the cells
 * in that chain.  Because the package makes copies of the keys,
 * this function must free the string storage as well.
 */

static void freeBucketChain(Cell *cp) {
   Cell *next;

   while (cp != NULL) {
      next = cp->link;
      freeBlock(cp->key);
      freeBlock(cp);
      cp = next;
   }
}

static Cell *findCell(Cell *cp, string key) {
   while (cp != NULL && !stringEqual(cp->key, key)) {
      cp = cp->link;
   }
   return cp;
}

/*
 * Implementation notes: newMapIterator, addKeyToIterator
 * ------------------------------------------------------
 * These functions implement the polymorphic iterator facility
 * for maps.  For details on the general strategy, see
 * the comments in the <code>itertype.h</code> interface.
 */

static Iterator newMapIterator(void *collection) {
   HashMap map;
   Iterator iterator;

   map = (HashMap) collection;
   iterator = newListIterator(sizeof (string), NULL);
   mapHashMap(map, addKeyToIterator, iterator);
   return iterator;
}

static void addKeyToIterator(string key, void *value, void *data) {
   if (value != NULL) addToIteratorList((Iterator) data, &key);
}

/**********************************************************************/
/* Unit test for the hashmap module                                   */
/**********************************************************************/

#ifndef _NOTEST_

/* Private function prototypes */

static void markElement(string name, int *bitSet);

/* Unit test */

void testHashMapModule(void) {
   HashMap map, map2;
   string key;
   int bits;

   trace(map = newHashMap());
   test(size(map), 0);
   test(isEmpty(map), true);
   trace(put(map, "H", "Hydrogen"));
   test(size(map), 1);
   test(isEmpty(map), false);
   trace(put(map, "He", "Helium"));
   trace(put(map, "Al", "Aluminum"));
   test(containsKey(map, "H"), true);
   test(get(map, "H"), "Hydrogen");
   test(get(map, "He"), "Helium");
   test(get(map, "Al"), "Aluminum");
   test(containsKey(map, "Li"), false);
   test(get(map, "Li"), NULL);
   trace(put(map, "Al", "Aluminium"));
   test(get(map, "Al"), "Aluminium");
   trace(remove(map, "Al"));
   test(containsKey(map, "Al"), false);
   test(get(map, "He"), "Helium");
   trace(put(map, "Li", "Lithium"));
   test(get(map, "Li"), "Lithium");
   trace(put(map, "Be", "Beryllium"));
   test(size(map), 4);
   trace(bits = 0);
   trace(foreach (key in map) markElement(key, &bits));
   test(bits, 15);
   trace(map2 = clone(map));
   trace(bits = 0);
   trace(foreach (key in map2) markElement(key, &bits));
   test(bits, 15);
}

/* Private functions */

static void markElement(string name, int *bits) {
   if (stringEqual(name, "H")) {
      *bits |= 1;
   } else if (stringEqual(name, "He")) {
      *bits |= 2;
   } else if (stringEqual(name, "Li")) {
      *bits |= 4;
   } else if (stringEqual(name, "Be")) {
      *bits |= 8;
   } else {
      *bits = -1;
   }
}

#endif
