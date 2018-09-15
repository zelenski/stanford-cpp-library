/*
 * File: generic.h
 * ---------------
 * This interface defines those functions that apply to more than one type.
 */

#ifndef _generic_h
#define _generic_h

#include "cmpfn.h"
#include "cslib.h"

/* Avoid various name conflicts */

#define remove xremove

#ifdef __macosx__
#  define dequeue _dequeue
#  define enqueue _enqueue
#endif

#define union(s1, s2) xunion(s1, s2)
#define getX(arg) getXGeneric(sizeof arg, arg)
#define getY(arg) getYGeneric(sizeof arg, arg)
#define getWidth(arg) getWidthGeneric(sizeof arg, arg)
#define getHeight(arg) getHeightGeneric(sizeof arg, arg)
#define toString(arg) toStringGeneric(sizeof arg, arg)
#define isEmpty(arg) isEmptyGeneric(sizeof arg, arg)
#define contains(arg, value) containsGeneric(sizeof arg, arg, value)

/*
 * Type: GenericType
 * -----------------
 * Union capable of holding any of the primitive types.
 */

typedef union {
    int intRep;
    short shortRep;
    long longRep;
    float floatRep;
    double doubleRep;
    char charRep;
    bool boolRep;
    unsigned unsignedRep;
    unsigned short unsignedShortRep;
    unsigned long unsignedLongRep;
    unsigned char unsignedCharRep;
    void *pointerRep;
} GenericType;

/*
 * Friend type: FetchFn
 * --------------------
 * This function type fetches an argument from an argument list into the
 * address indicated by <code>dst</code>.
 */

typedef void (*FetchFn)(va_list args, GenericType *dst);

/*
 * Friend type: StoreFn
 * --------------------
 * This function type stores a value from the generic object into the
 * address indicated by <code>dst</code>.
 */

typedef void (*StoreFn)(GenericType src, void *dst);

/*
 * Friend type: ToStringFn
 * -----------------------
 * This function type converts a generic argument to a string.
 */

typedef string (*ToStringFn)(GenericType dst);

/*
 * Function: size
 * Usage: n = size(arg);
 * ---------------------
 * Returns the number of elements in the collection.
 */

int size(void *arg);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(arg)) . . .
 * ------------------------------
 * Returns <code>true</code> if the collection is empty.
 */

bool isEmptyGeneric(int size, ...);

/*
 * Function: clear
 * Usage: clear(arg);
 * ------------------
 * Removes all elements from the collection.
 */

void clear(void *arg);

/*
 * Function: clone
 * Usage: clone(arg);
 * ------------------
 * Creates a copy of the collection.
 */

void *clone(void *arg);

/*
 * Function: get
 * Usage: value = get(arg, ...);
 * -----------------------------
 * Gets a value from the collection.  The format of the argument list
 * depends on the collection type.  See the documentation of the
 * individual type for details.
 */

void *get(void *arg, ...);

/*
 * Function: set
 * Usage: set(arg, ...);
 * ---------------------
 * Sets a specific element in the collection.  The format of the argument
 * list depends on the collection type.  See the documentation of the
 * individual type for details.
 */

void set(void *arg, ...);

/*
 * Function: put
 * Usage: put(arg, ...);
 * ---------------------
 * Puts a value into the collection.  By convention, this verb is used
 * in preference to <code>set</code> or <code>add</code> for maps.  The
 * format of the argument list depends on the collection type.  See the
 * documentation of the individual type for details.
 */

void put(void *arg, ...);

/*
 * Function: containsKey
 * Usage: if (containsKey(arg, key)) . . .
 * ---------------------------------------
 * Checks whether the specified key is in the map.
 */

bool containsKey(void *arg, void *key);

/*
 * Function: contains
 * Usage: if (contains(arg, ...)) . . .
 * ------------------------------------
 * Checks whether a set contains the value specified by the argument list,
 * which may vary according to the base type.
 */

bool containsGeneric(int size, ...);

/*
 * Function: add
 * Usage: add(arg, ...);
 * ---------------------
 * Adds a value to the collection.  The format of the argument list depends
 * on the collection type.  See the documentation of the individual type
 * for details.
 */

void add(void *arg, ...);

/*
 * Function: remove
 * Usage: remove(arg, ...);
 * ------------------------
 * Removes a value from the collection.  The format of the argument list
 * depends on the collection type.  See the documentation of the individual
 * type for details.
 */

void remove(void *arg, ...);

/*
 * Function: enqueue
 * Usage: enqueue(arg, ...);
 * -------------------------
 * Adds a new value to the queue.  The format of the argument list differs
 * slightly for the <code>Queue</code> and <code>PriorityQueue</code> types.
 * See the documentation of the individual type for details.
 */

void enqueue(void *arg, ...);

/*
 * Function: dequeue
 * Usage: value = dequeue(arg);
 * ----------------------------
 * Removes and returns the first item in the queue.
 */

void *dequeue(void *arg);

/*
 * Function: peek
 * Usage: peek(arg);
 * -----------------
 * Returns the first item in the collection without removing it.
 */

void *peek(void *arg);

/*
 * Function: equals
 * Usage: if (equals(s1, s2)) . . .
 * --------------------------------
 * Returns <code>true</code> if the sets are equal.
 */

bool equals(void *s1, void *s2);

/*
 * Function: isSubset
 * Usage: if (isSubset(s1, s2)) . . .
 * ----------------------------------
 * Returns <code>true</code> if <code>s1</code> is a subset of
 * <code>s2</code>.
 */

bool isSubset(void *s1, void *s2);

/*
 * Function: union
 * Usage: set = union(s1, s2);
 * ---------------------------
 * Returns the union of <code>s1</code> and <code>s2</code>.
 */

void *union(void *s1, void *s2);

/*
 * Function: intersection
 * Usage: set = intersection(s1, s2);
 * ----------------------------------
 * Returns the intersection of the <code>s1</code> and <code>s2</code>.
 */

void *intersection(void *s1, void *s2);

/*
 * Function: setDifference
 * Usage: set = setDifference(s1, s2);
 * -----------------------------------
 * Returns the set difference of <code>s1</code> and <code>s2</code>.
 */

void *setDifference(void *s1, void *s2);

/*
 * Function: getX
 * Usage: x = getX(any);
 * ---------------------
 * Returns the x component of the value.
 */

double getXGeneric(int size, ...);

/*
 * Function: getY
 * Usage: y = getY(any);
 * ---------------------
 * Returns the y component of the value.
 */

double getYGeneric(int size, ...);

/*
 * Function: getWidth
 * Usage: width = getWidth(any);
 * -----------------------------
 * Returns the width component of the value.
 */

double getWidthGeneric(int size, ...);

/*
 * Function: getHeight
 * Usage: height = getHeight(any);
 * -------------------------------
 * Returns the height component of the value.
 */

double getHeightGeneric(int size, ...);

/*
 * Function: toString
 * Usage: str = toString(arg);
 * ---------------------------
 * Converts the argument to a string, if possible
 */

string toStringGeneric(int size, ...);

/*
 * Function: setVisible
 * Usage: setVisible(arg, flag);
 * -----------------------------
 * Sets whether the argument is visible.
 */

void setVisible(void *arg, bool flag);

/*
 * Function: isVisible
 * Usage: if (isVisible(arg)) . . .
 * --------------------------------
 * Returns <code>true</code> if the argument is visible.
 */

bool isVisible(void *arg);

/*
 * Function: setColor
 * Usage: setColor(arg, color);
 * ----------------------------
 * Sets the color of the argument.
 */

void setColor(void *arg, string color);

/*
 * Function: rotate
 * Usage: rotate(arg, theta);
 * --------------------------
 * Rotates the argument (which is either a <code>GTransform</code> or a
 * <code>GObject</code>) by <code>theta</code> degrees counterclockwise.
 */

void rotate(void *arg, double theta);

/*
 * Function: scale
 * Usage: scale(arg, sx, sy);
 * --------------------------
 * Scales the argument (which is either a <code>GTransform</code> or a
 * <code>GObject</code>) by theta degrees counterclockwise.
 */

void scale(void *arg, double sx, double sy);

/*
 * Friend function: getTypeSizeForType
 * Usage: size = getTypeSizeForType(type);
 * ---------------------------------------
 * Returns the size in bytes for the specified type.
 */

int getTypeSizeForType(string type);

/*
 * Friend function: getCompareFnForType
 * Usage: fn = getCompareFnForType(type);
 * --------------------------------------
 * Returns the comparison function appropriate to the type.
 */

CompareFn getCompareFnForType(string type);

/*
 * Friend function: getFetchFnForType
 * Usage: fn = getFetchFnForType(type);
 * ------------------------------------
 * Returns a function that can fetch an argument of the specified type
 * from a <code>va_list</code> parameter.  The pattern for calling that
 * function is
 *
 *<pre>
 *    fn(args, &arg);
 *</pre>
 *
 * where <code>arg</code> is a <code>GenericType</code> variable.
 */

FetchFn getFetchFnForType(string type);

/*
 * Friend function: getStoreFnForType
 * Usage: fn = getStoreFnForType(type);
 * ------------------------------------
 * Returns a function that can store a generic argument through a
 * destination pointer.  The pattern for calling that function is
 *
 *<pre>
 *    fn(src, dst);
 *</pre>
 *
 * where <code>src</code> is a <code>GenericType</code> variable
 * and <code>dst</code> is a pointer to the destination.
 */

StoreFn getStoreFnForType(string type);

/*
 * Friend function: getToStringFn
 * Usage: fn = getToStringFn(type);
 * --------------------------------
 * Returns a function that can convert a generic value of the specified
 * type to a string.  This function is used primarily for debugging.
 */

ToStringFn getToStringFn(string type);

#endif
