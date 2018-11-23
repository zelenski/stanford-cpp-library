/*
 * File: map.h
 * -----------
 * This interface defines a map abstraction that associates string-valued
 * keys with values.  In contrast to the <code>HashMap</code> type defined
 * in the <code>hashmap.h</code> interface, the implementation of the
 * <code>Map</code> type uses a balanced binary tree, which offers
 * logarithmic performance and sorted iteration.
 *
 * <p>In most applications, the restriction of keys to strings is easy
 * to circumvent.  The simplest strategy is to convert key values to
 * strings before inserting them into the map.  A more general strategy
 * is to use the <code>bst.h</code> interface instead.
 */

#ifndef _map_h
#define _map_h

#include "cslib.h"
#include "generic.h"
#include "iterator.h"

/*
 * Type: Map
 * ---------
 * This type is the ADT used to represent the map.
 */

typedef struct MapCDT *Map;

/* Exported entries */

/*
 * Function: newMap
 * Usage: map = newMap();
 * ----------------------
 * Allocates a new map with no entries.
 */

Map newMap();

/*
 * Function: freeMap
 * Usage: freeMap(map);
 * --------------------
 * Frees the storage associated with the map.
 */

void freeMap(Map map);

/*
 * Function: size
 * Usage: n = size(map);
 * ---------------------
 * Returns the number of elements in the map.
 */

int sizeMap(Map map);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(map)) . . .
 * ------------------------------
 * Returns <code>true</code> if the map has no entries.
 */

bool isEmptyMap(Map map);

/*
 * Function: clear
 * Usage: clear(map);
 * ------------------
 * Removes all entries from the map.
 */

void clearMap(Map map);

/*
 * Function: clone
 * Usage: newmap = clone(map);
 * ---------------------------
 * Creates a copy of the map.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

Map cloneMap(Map map);

/*
 * Function: put
 * Usage: put(map, key, value);
 * ----------------------------
 * Associates <code>key</code> with <code>value</code> in the map.
 * Each call to <code>put</code> supersedes any previous definition
 * for <code>key</code>.
 */

void putMap(Map map, string key, void *value);

/*
 * Function: get
 * Usage: void *value = get(map, key);
 * -----------------------------------
 * Returns the value associated with <code>key</code> in the map,
 * or <code>NULL</code>, if no such value exists.
 */

void *getMap(Map map, string key);

/*
 * Function: containsKey
 * Usage: if (containsKey(map, key)) . . .
 * ---------------------------------------
 * Checks to see if the map contains the specified key.
 */

bool containsKeyMap(Map map, string key);

/*
 * Function: remove
 * Usage: remove(map, key);
 * ------------------------
 * Removes the key and its value from the map.
 */

void removeMap(Map map, string key);

/*
 * Function: map
 * Usage: map(map, fn, data);
 * --------------------------
 * Iterates through the map and calls the function <code>fn</code> on
 * each entry.  The callback function takes the following arguments:
 *
 *<ul>
 *  <li>The key
 *  <li>The associated value
 *  <li>The <code>data</code> pointer
 *</ul>
 *
 * The <code>data</code> pointer allows the client to pass state
 * information to the function <code>fn</code>, if necessary.  If no such
 * information is required, this argument should be <code>NULL</code>.
 */

void mapMap(Map map, proc fn, void *data);

#endif
