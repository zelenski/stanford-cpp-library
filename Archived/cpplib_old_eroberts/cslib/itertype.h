/*
 * File: itertype.h
 * ----------------
 * This interface allows package designers to export new collection
 * types that support iteration.  Clients of those collection types
 * who merely want to use iterators do not need to import this interface.
 */

#ifndef _itertype_h
#define _itertype_h

#include "cslib.h"
#include "iterator.h"
#include "cmpfn.h"

/*
 * General overview
 * ----------------
 * In order to create a collection type that supports iteration,
 * the implementor of the type must first allocate space at the
 * beginning of the concrete record for an <code>IteratorHeader</code>
 * containing a function pointer that allows <code>newIterator</code>
 * to make a new iterator for that collection type.  The implementor
 * must initialize this header in the constructor function by calling
 * <code>enableIteration</code>, which takes the collection pointer
 * and a type-specific callback to create the iterator.
 *
 * There are two different strategies for creating the iterator in
 * each type.  The general strategy (which is useful for iterators
 * that are not really collections, such as scanners that return
 * a series of tokens) is to call <code>newStepIterator</code>,
 * which takes a pointer to a <code>stepIterator</code> function
 * specific for that type.  The second strategy creates the entire
 * list of values at the beginning.  Such iterators are created by
 * calling <code>newListIterator</code> to create an empty iterator
 * and the calling <code>addToIteratorList</code> for each element.
 *
 * Elements in the iterator may be sorted or unsorted depending on
 * the comparison function passed to <code>newListIterator</code>.
 */

/*
 * Type: NewIteratorFn
 * -------------------
 * Represents the class of functions that create new iterators.
 */

typedef Iterator (*NewIteratorFn)(void *collection);

/*
 * Type: StepIteratorFn
 * --------------------
 * Represents the class of functions that step from one element to the next.
 */

typedef bool (*StepIteratorFn)(Iterator iterator, void *dst);

/*
 * Type: IteratorHeader
 * --------------------
 * This structure must appear at the beginning of any concrete
 * structure that supports iteration.
 */

typedef struct {
    unsigned long password;
    NewIteratorFn newFn;
} IteratorHeader;

/*
 * Function: enableIteration
 * Usage: enableIteration(collection, newFn);
 * ------------------------------------------
 * Enables iteration for the collection.  The type-specific function
 * pointer <code>newFn</code> is used to initialize an iterator
 * for this type.
 */

void enableIteration(void *collection, NewIteratorFn newFn);

/*
 * Function: newStepIterator
 * Usage: iterator = newStepIterator(size, stepFn);
 * ------------------------------------------------
 * Creates a new iterator that uses an implementation-specific
 * step function.
 */

Iterator newStepIterator(int size, StepIteratorFn stepFn);

/*
 * Function: newListIterator
 * Usage: iterator = newListIterator(size, cmpFn);
 * -----------------------------------------------
 * Creates a new iterator with an empty iterator list.  The first
 * argument is the size of the element type,  which makes it possible
 * for the package to allocate storage for values of that type.
 * The <code>cmpFn</code> is the comparison function used to sort
 * the iterator elements.  If <code>cmpFn</code> is the constant
 * <code>NULL</code>, the elements are entered in the order in
 * which <code>addToIteratorList</code> is called.
 */

Iterator newListIterator(int size, CompareFn cmpFn);

/*
 * Function: addToIteratorList
 * Usage: addToIteratorList(iterator, dst);
 * ----------------------------------------
 * Adds a new element to a list-style iterator.  The order of the
 * elements is determined by the comparison function specified
 * when the iterator was created.
 */

void addToIteratorList(Iterator iterator, void *dst);

/*
 * Function: setCollection
 * Usage: setCollection(iterator, collection);
 * -------------------------------------------
 * Sets the collection to which an iterator belongs.
 */

void setCollection(Iterator iterator, void *collection);

/*
 * Function: getCollection
 * Usage: collection = getCollection(iterator);
 * --------------------------------------------
 * Returns the collection to which an iterator belongs.
 */

void *getCollection(Iterator iterator);

/*
 * Function: setIteratorData
 * Usage: setIteratorData(iterator, data);
 * ---------------------------------------
 * Sets a data pointer in the iterator for use by clients.
 */

void setIteratorData(Iterator iterator, void *data);

/*
 * Function: getIteratorData
 * Usage: data = getIteratorData(iterator);
 * ----------------------------------------
 * Returns the client data pointer from the iterator.
 */

void *getIteratorData(Iterator iterator);

#endif
