/*
 * File: generic.c
 * Last modified on Fri Sep 26 09:34:55 2014 by eroberts
 * -----------------------------------------------------
 * This file implements the generic.h interface.
 */

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "charset.h"
#include "cmpfn.h"
#include "cslib.h"
#include "exception.h"
#include "generic.h"
#include "gevents.h"
#include "gobjects.h"
#include "gtransform.h"
#include "gtypes.h"
#include "hashmap.h"
#include "pqueue.h"
#include "queue.h"
#include "map.h"
#include "ref.h"
#include "set.h"
#include "stack.h"
#include "strbuf.h"
#include "strlib.h"
#include "vector.h"

/* Private function prototypes */
static void intFetchFn(va_list args, GenericType *dst);
static void shortFetchFn(va_list args, GenericType *dst);
static void longFetchFn(va_list args, GenericType *dst);
static void floatFetchFn(va_list args, GenericType *dst);
static void doubleFetchFn(va_list args, GenericType *dst);
static void charFetchFn(va_list args, GenericType *dst);
static void unsignedFetchFn(va_list args, GenericType *dst);
static void unsignedShortFetchFn(va_list args, GenericType *dst);
static void unsignedLongFetchFn(va_list args, GenericType *dst);
static void unsignedCharFetchFn(va_list args, GenericType *dst);
static void pointerFetchFn(va_list args, GenericType *dst);
static void intStoreFn(GenericType src, void *dst);
static void shortStoreFn(GenericType src, void *dst);
static void longStoreFn(GenericType src, void *dst);
static void floatStoreFn(GenericType src, void *dst);
static void doubleStoreFn(GenericType src, void *dst);
static void charStoreFn(GenericType src, void *dst);
static void unsignedStoreFn(GenericType src, void *dst);
static void unsignedShortStoreFn(GenericType src, void *dst);
static void unsignedLongStoreFn(GenericType src, void *dst);
static void unsignedCharStoreFn(GenericType src, void *dst);
static void pointerStoreFn(GenericType src, void *dst);
static string intToStringFn(GenericType any);
static string shortToStringFn(GenericType any);
static string longToStringFn(GenericType any);
static string floatToStringFn(GenericType any);
static string doubleToStringFn(GenericType any);
static string charToStringFn(GenericType any);
static string boolToStringFn(GenericType any);
static string unsignedToStringFn(GenericType any);
static string unsignedShortToStringFn(GenericType any);
static string unsignedLongToStringFn(GenericType any);
static string unsignedCharToStringFn(GenericType any);
static string stringToStringFn(GenericType any);
static string pointerToStringFn(GenericType any);

/* Constants */

#define BUFSIZE 40

/* Exported entries */

int size(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      return sizeVector((Vector) arg);
   } else if (endsWith(type, "PriorityQueue")) {
      return sizePriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      return sizeQueue((Queue) arg);
   } else if (endsWith(type, "CharSet")) {
      return sizeCharSet((CharSet) arg);
   } else if (endsWith(type, "Set")) {
      return sizeSet((Set) arg);
   } else if (endsWith(type, "HashMap")) {
      return sizeHashMap((HashMap) arg);
   } else if (endsWith(type, "Map")) {
      return sizeMap((Map) arg);
   } else if (endsWith(type, "Stack")) {
      return sizeStack((Stack) arg);
   } else if (endsWith(type, "StringBuffer")) {
      return sizeStringBuffer((StringBuffer) arg);
   } else {
      error("size: Unrecognized type %s", type);
      return 0;
   }
}

bool isEmptyGeneric(int size, ...) {
   va_list args;
   void *arg;
   bool result;
   string type;

   va_start(args, size);
   if (size == sizeof(GRectangle)) {
      result = isEmptyGRectangle(va_arg(args, GRectangle));
      va_end(args);
      return result;
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      return isEmptyVector((Vector) arg);
   } else if (endsWith(type, "PriorityQueue")) {
      return isEmptyPriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      return isEmptyQueue((Queue) arg);
   } else if (endsWith(type, "CharSet")) {
      return isEmptyCharSet((CharSet) arg);
   } else if (endsWith(type, "Set")) {
      return isEmptySet((Set) arg);
   } else if (endsWith(type, "HashMap")) {
      return isEmptyHashMap((HashMap) arg);
   } else if (endsWith(type, "Map")) {
      return isEmptyMap((Map) arg);
   } else if (endsWith(type, "Stack")) {
      return isEmptyStack((Stack) arg);
   } else if (endsWith(type, "StringBuffer")) {
      return isEmptyStringBuffer((StringBuffer) arg);
   } else {
      error("isEmpty: Unrecognized type %s", type);
      return false;
   }
}

void clear(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      clearVector((Vector) arg);
   } else if (endsWith(type, "PriorityQueue")) {
      clearPriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      clearQueue((Queue) arg);
   } else if (endsWith(type, "CharSet")) {
      clearCharSet((CharSet) arg);
   } else if (endsWith(type, "Set")) {
      clearSet((Set) arg);
   } else if (endsWith(type, "HashMap")) {
      clearHashMap((HashMap) arg);
   } else if (endsWith(type, "Map")) {
      clearMap((Map) arg);
   } else if (endsWith(type, "Stack")) {
      clearStack((Stack) arg);
   } else if (endsWith(type, "StringBuffer")) {
      clearStringBuffer((StringBuffer) arg);
   } else {
      error("clear: Unrecognized type %s", type);
   }
}

void *clone(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      return cloneVector((Vector) arg);
   } else if (endsWith(type, "PriorityQueue")) {
      return clonePriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      return cloneQueue((Queue) arg);
   } else if (endsWith(type, "CharSet")) {
      return cloneCharSet((CharSet) arg);
   } else if (endsWith(type, "Set")) {
      return cloneSet((Set) arg);
   } else if (endsWith(type, "HashMap")) {
      return cloneHashMap((HashMap) arg);
   } else if (endsWith(type, "Map")) {
      return cloneMap((Map) arg);
   } else if (endsWith(type, "Stack")) {
      return cloneStack((Stack) arg);
   } else {
      error("clone: Unrecognized type %s", type);
      return NULL;
   }
}

void *get(void *arg, ...) {
   string type;
   void *key;
   int index;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      va_start(args, arg);
      index = va_arg(args, int);
      va_end(args);
      return getVector((Vector) arg, index);
   } else if (endsWith(type, "HashMap")) {
      va_start(args, arg);
      key = va_arg(args, string);
      va_end(args);
      return getHashMap((HashMap) arg, key);
   } else if (endsWith(type, "Map")) {
      va_start(args, arg);
      key = va_arg(args, void *);
      va_end(args);
      return getMap((Map) arg, key);
   } else {
      error("get: Unrecognized type %s", type);
      return NULL;
   }
}

void set(void *arg, ...) {
   string type;
   int index;
   void *value;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      va_start(args, arg);
      index = va_arg(args, int);
      value = va_arg(args, void *);
      va_end(args);
      setVector((Vector) arg, index, value);
   } else {
      error("set: Unrecognized type %s", type);
   }
}

void put(void *arg, ...) {
   string type;
   string key;
   void *value;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "HashMap")) {
      va_start(args, arg);
      key = va_arg(args, string);
      value = va_arg(args, void *);
      va_end(args);
      putHashMap((HashMap) arg, key, value);
   } else if (endsWith(type, "Map")) {
      va_start(args, arg);
      key = va_arg(args, void *);
      value = va_arg(args, void *);
      va_end(args);
      putMap((Map) arg, key, value);
   } else {
      error("put: Unrecognized type %s", type);
   }
}

bool containsKey(void *arg, void *key) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "HashMap")) {
      return containsKeyHashMap((HashMap) arg, (string) key);
   } else if (endsWith(type, "Map")) {
      return containsKeyMap((Map) arg, key);
   } else {
      error("containsKey: Unrecognized type %s", type);
      return false;
   }
}

bool containsGeneric(int size, ...) {
   string type;
   void *value, *arg;
   double x, y;
   char ch;
   bool result;
   va_list args;
   GRectangle r;
   GPoint pt;

   va_start(args, size);
   if (size == sizeof(GRectangle)) {
      r = va_arg(args, GRectangle);
      pt = va_arg(args, GPoint);
      va_end(args);
      return containsGRectangle(r, pt);
   }
   arg = va_arg(args, void *);
   type = getBlockType(arg);
   if (endsWith(type, "CharSet")) {
      ch = va_arg(args, int);
      va_end(args);
      return containsCharSet((CharSet) arg, ch);
   } else if (endsWith(type, "Set")) {
      result = containsSetFromArgs((Set) arg, args);
      va_end(args);
      return result;
   } else if (endsWith(type, "GObject")) {
      x = va_arg(args, double);
      y = va_arg(args, double);
      result = containsGObject((GObject) arg, x, y);
      va_end(args);
      return result;
   } else {
      error("contains: Unrecognized type %s", type);
      return false;
   }
}

void add(void *arg, ...) {
   string type;
   char ch;
   void *value;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      va_start(args, arg);
      value = va_arg(args, void *);
      va_end(args);
      addVector((Vector) arg, value);
   } else if (endsWith(type, "CharSet")) {
      va_start(args, arg);
      ch = va_arg(args, int);
      va_end(args);
      addCharSet((CharSet) arg, ch);
   } else if (endsWith(type, "Set")) {
      va_start(args, arg);
      addSetFromArgs((Set) arg, args);
      va_end(args);
   } else if (endsWith(type, "GWindow")) {
      va_start(args, arg);
      addGWindow((GWindow) arg, va_arg(args, GObject));
      va_end(args);
   } else if (endsWith(type, "GCompound")) {
      va_start(args, arg);
      addGCompound((GWindow) arg, va_arg(args, GObject));
      va_end(args);
   } else {
      error("add: Unrecognized type %s", type);
   }
}

void remove(void *arg, ...) {
   GObject gobj;
   string type;
   void *key;
   int index;
   char ch;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "Vector")) {
      va_start(args, arg);
      index = va_arg(args, int);
      va_end(args);
      removeVector((Vector) arg, index);
   } else if (endsWith(type, "CharSet")) {
      va_start(args, arg);
      ch = va_arg(args, int);
      va_end(args);
      removeCharSet((CharSet) arg, ch);
   } else if (endsWith(type, "Set")) {
      va_start(args, arg);
      removeSetFromArgs((Set) arg, args);
      va_end(args);
   } else if (endsWith(type, "HashMap")) {
      va_start(args, arg);
      key = va_arg(args, string);
      va_end(args);
      removeHashMap((HashMap) arg, key);
   } else if (endsWith(type, "Map")) {
      va_start(args, arg);
      key = va_arg(args, void *);
      va_end(args);
      removeMap((Map) arg, key);
   } else if (endsWith(type, "GWindow")) {
      va_start(args, arg);
      gobj = va_arg(args, GObject);
      va_end(args);
      removeGWindow((GWindow) arg, key);
   } else {
      error("remove: Unrecognized type %s", type);
   }
}

/*
 * Implementation notes: enqueue
 * -----------------------------
 * The complexity in this function comes from trying to check for
 * int priority values.  The code reads the priority both as an int
 * and as a double and then sanity-checks the two.
 */

#define DTHRESHOLD 1.0E-100
#define ITHRESHOLD ((unsigned) -1 >> 2)

void enqueue(void *arg, ...) {
   string type;
   void *value;
   double priority;
   int ipriority;
   va_list args;

   type = getBlockType(arg);
   if (endsWith(type, "PriorityQueue")) {
      va_start(args, arg);
      value = va_arg(args, void *);
      priority = va_arg(args, double);
      va_end(args);
      va_start(args, arg);
      va_arg(args, void *);
      ipriority = va_arg(args, int);
      va_end(args);
      if (fabs(priority) < DTHRESHOLD && abs(ipriority) < ITHRESHOLD) {
         priority = ipriority;
      }
      enqueuePriorityQueue((PriorityQueue) arg, value, priority);
   } else if (endsWith(type, "Queue")) {
      va_start(args, arg);
      value = va_arg(args, void *);
      va_end(args);
      enqueueQueue((Queue) arg, value);
   } else {
      error("enqueue: Unrecognized type %s", type);
   }
}

void *dequeue(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "PriorityQueue")) {
      return dequeuePriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      return dequeueQueue((Queue) arg);
   } else {
      error("dequeue: Unrecognized type %s", type);
      return NULL;
   }
}

void *peek(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "PriorityQueue")) {
      return peekPriorityQueue((PriorityQueue) arg);
   } else if (endsWith(type, "Queue")) {
      return peekQueue((Queue) arg);
   } else if (endsWith(type, "Stack")) {
      return peekStack((Stack) arg);
   } else {
      error("peek: Unrecognized type %s", type);
      return NULL;
   }
}

bool equals(void *s1, void *s2) {
   string type;

   type = getBlockType(s1);
   if (endsWith(type, "CharSet")) {
      return equalsCharSet((CharSet) s1, (CharSet) s2);
   } else if (endsWith(type, "Set")) {
      return equalsSet((Set) s1, (Set) s2);
   } else {
      error("equals: Unrecognized type %s", type);
      return false;
   }
}

bool isSubset(void *s1, void *s2) {
   string type;

   type = getBlockType(s1);
   if (endsWith(type, "CharSet")) {
      return isSubsetCharSet((CharSet) s1, (CharSet) s2);
   } else if (endsWith(type, "Set")) {
      return isSubsetSet((Set) s1, (Set) s2);
   } else {
      error("isSubset: Unrecognized type %s", type);
      return false;
   }
}

void *xunion(void *s1, void *s2) {
   string type;

   type = getBlockType(s1);
   if (endsWith(type, "CharSet")) {
      return unionCharSet((CharSet) s1, (CharSet) s2);
   } else if (endsWith(type, "Set")) {
      return unionSet((Set) s1, (Set) s2);
   } else {
      error("union: Unrecognized type %s", type);
      return NULL;
   }
}

void *intersection(void *s1, void *s2) {
   string type;

   type = getBlockType(s1);
   if (endsWith(type, "CharSet")) {
      return intersectionCharSet((CharSet) s1, (CharSet) s2);
   } else if (endsWith(type, "Set")) {
      return intersectionSet((Set) s1, (Set) s2);
   } else {
      error("intersection: Unrecognized type %s", type);
      return NULL;
   }
}

void *setDifference(void *s1, void *s2) {
   string type;

   type = getBlockType(s1);
   if (endsWith(type, "CharSet")) {
      return setDifferenceCharSet((CharSet) s1, (CharSet) s2);
   } else if (endsWith(type, "Set")) {
      return setDifferenceSet((Set) s1, (Set) s2);
   } else {
      error("setDifference: Unrecognized type %s", type);
      return NULL;
   }
}

double getXGeneric(int size, ...) {
   va_list args;
   GPoint pt;
   GRectangle r;
   void *arg;
   string type;

   va_start(args, size);
   if (size == sizeof(GPoint)) {
      pt = va_arg(args, GPoint);
      va_end(args);
      return getXGPoint(pt);
   } else if (size == sizeof(GRectangle)) {
      r = va_arg(args, GRectangle);
      va_end(args);
      return getXGRectangle(r);
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "GEvent")) {
      return getXGEvent((GEvent) arg);
   } else if (endsWith(type, "GObject")) {
      return getXGObject((GObject) arg);
   } else {
      error("getX: Illegal argument type");
      return 0;
   }
}

double getYGeneric(int size, ...) {
   va_list args;
   GPoint pt;
   GRectangle r;
   void *arg;
   string type;

   va_start(args, size);
   if (size == sizeof(GPoint)) {
      pt = va_arg(args, GPoint);
      va_end(args);
      return getYGPoint(pt);
   } else if (size == sizeof(GRectangle)) {
      r = va_arg(args, GRectangle);
      va_end(args);
      return getYGRectangle(r);
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "GEvent")) {
      return getYGEvent((GEvent) arg);
   } else if (endsWith(type, "GObject")) {
      return getYGObject((GObject) arg);
   } else {
      error("getY: Illegal argument type");
      return 0;
   }
}

double getWidthGeneric(int size, ...) {
   va_list args;
   GDimension dim;
   GRectangle r;
   string type;
   void *arg;

   va_start(args, size);
   if (size == sizeof(GDimension)) {
      dim = va_arg(args, GDimension);
      va_end(args);
      return getWidthGDimension(dim);
   } else if (size == sizeof(GRectangle)) {
      r = va_arg(args, GRectangle);
      va_end(args);
      return getWidthGRectangle(r);
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "GWindow")) {
      return getWidthGWindow((GWindow) arg);
   } else if (endsWith(type, "GObject")) {
      return getWidthGObject((GObject) arg);
   } else {
      error("getWidth: Illegal argument type");
      return 0;
   }
}

double getHeightGeneric(int size, ...) {
   va_list args;
   GDimension dim;
   GRectangle r;
   string type;
   void *arg;

   va_start(args, size);
   if (size == sizeof(GDimension)) {
      dim = va_arg(args, GDimension);
      va_end(args);
      return getHeightGDimension(dim);
   } else if (size == sizeof(GRectangle)) {
      r = va_arg(args, GRectangle);
      va_end(args);
      return getHeightGRectangle(r);
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "GWindow")) {
      return getHeightGWindow((GWindow) arg);
   } else if (endsWith(type, "GObject")) {
      return getHeightGObject((GObject) arg);
   } else {
      error("getHeight: Illegal argument type");
      return 0;
   }
}

string toStringGeneric(int size, ...) {
   StringBuffer sb;
   va_list args;
   GPoint pt;
   GRectangle r;
   string type, result;
   void *arg;

   va_start(args, size);
   if (size == sizeof(GPoint)) {
      sb = newStringBuffer();
      pt = va_arg(args, GPoint);
      va_end(args);
      sbprintf(sb, "(%g, %g)", pt.x, pt.y);
      result = copyString(getString(sb));
      freeStringBuffer(sb);
      return result;
   } else if (size == sizeof(GRectangle)) {
      sb = newStringBuffer();
      r = va_arg(args, GRectangle);
      va_end(args);
      sbprintf(sb, "(%g, %g, %g, %g)", r.x, r.y, r.width, r.height);
      result = copyString(getString(sb));
      freeStringBuffer(sb);
      return result;
   }
   arg = va_arg(args, void *);
   va_end(args);
   type = getBlockType(arg);
   if (endsWith(type, "StringBuffer")) {
      return copyString(getString((StringBuffer) arg));
   } else if (endsWith(type, "GTransform")) {
      return toStringGTransform((GTransform) arg);
   } else {
      error("toString: Illegal argument type");
      return NULL;
   }
}

void setVisible(void *arg, bool flag) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      setVisibleGObject((GObject) arg, flag);
   } else if (endsWith(type, "GWindow")) {
      setVisibleGWindow((GWindow) arg, flag);
   } else {
      error("setVisible: Unrecognized type %s", type);
   }
}

bool isVisible(void *arg) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      return isVisibleGObject((GObject) arg);
   } else if (endsWith(type, "GWindow")) {
      return isVisibleGWindow((GWindow) arg);
   } else {
      error("isVisible: Unrecognized type %s", type);
      return false;
   }
}

void setColor(void *arg, string color) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      setColorGObject((GObject) arg, color);
   } else if (endsWith(type, "GWindow")) {
      setColorGWindow((GWindow) arg, color);
   } else {
      error("setColor: Unrecognized type %s", type);
   }
}

void rotate(void *arg, double theta) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      // Fill in
   } else if (endsWith(type, "GTransform")) {
      rotateGTransform((GTransform) arg, theta);
   } else {
      error("rotate: Unrecognized type %s", type);
   }
}

void scale(void *arg, double sx, double sy) {
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      // Fill in
   } else if (endsWith(type, "GTransform")) {
      scaleGTransform((GTransform) arg, sx, sy);
   } else {
      error("scale: Unrecognized type %s", type);
   }
}

int getTypeSizeForType(string type) {
   if (stringEqual(type, "int")) return sizeof(int);
   if (stringEqual(type, "short")) return sizeof(short);
   if (stringEqual(type, "long")) return sizeof(long);
   if (stringEqual(type, "float")) return sizeof(float);
   if (stringEqual(type, "double")) return sizeof(double);
   if (stringEqual(type, "char")) return sizeof(char);
   if (stringEqual(type, "bool")) return sizeof(bool);
   if (stringEqual(type, "unsigned")) return sizeof(unsigned);
   if (stringEqual(type, "unsigned short")) return sizeof(unsigned short);
   if (stringEqual(type, "unsigned long")) return sizeof(unsigned long);
   if (stringEqual(type, "unsigned char")) return sizeof(unsigned char);
   if (stringEqual(type, "char *")) return sizeof(char *);
   if (stringEqual(type, "string")) return sizeof(string);
   return sizeof(void *);
}

CompareFn getCompareFnForType(string type) {
   if (stringEqual(type, "int")) return intCmpFn;
   if (stringEqual(type, "short")) return shortCmpFn;
   if (stringEqual(type, "long")) return longCmpFn;
   if (stringEqual(type, "float")) return floatCmpFn;
   if (stringEqual(type, "double")) return doubleCmpFn;
   if (stringEqual(type, "char")) return charCmpFn;
   if (stringEqual(type, "bool")) return intCmpFn;
   if (stringEqual(type, "unsigned")) return unsignedCmpFn;
   if (stringEqual(type, "unsigned short")) return unsignedShortCmpFn;
   if (stringEqual(type, "unsigned long")) return unsignedLongCmpFn;
   if (stringEqual(type, "unsigned char")) return unsignedCharCmpFn;
   if (stringEqual(type, "char *")) return stringCmpFn;
   if (stringEqual(type, "string")) return stringCmpFn;
   return pointerCmpFn;
}

FetchFn getFetchFnForType(string type) {
   if (stringEqual(type, "int")) return intFetchFn;
   if (stringEqual(type, "short")) return shortFetchFn;
   if (stringEqual(type, "long")) return longFetchFn;
   if (stringEqual(type, "float")) return floatFetchFn;
   if (stringEqual(type, "double")) return doubleFetchFn;
   if (stringEqual(type, "char")) return charFetchFn;
   if (stringEqual(type, "bool")) return intFetchFn;
   if (stringEqual(type, "unsigned")) return unsignedFetchFn;
   if (stringEqual(type, "unsigned short")) return unsignedShortFetchFn;
   if (stringEqual(type, "unsigned long")) return unsignedLongFetchFn;
   if (stringEqual(type, "unsigned char")) return unsignedCharFetchFn;
   return pointerFetchFn;
}

StoreFn getStoreFnForType(string type) {
   if (stringEqual(type, "int")) return intStoreFn;
   if (stringEqual(type, "short")) return shortStoreFn;
   if (stringEqual(type, "long")) return longStoreFn;
   if (stringEqual(type, "float")) return floatStoreFn;
   if (stringEqual(type, "double")) return doubleStoreFn;
   if (stringEqual(type, "char")) return charStoreFn;
   if (stringEqual(type, "bool")) return intStoreFn;
   if (stringEqual(type, "unsigned")) return unsignedStoreFn;
   if (stringEqual(type, "unsigned short")) return unsignedShortStoreFn;
   if (stringEqual(type, "unsigned long")) return unsignedLongStoreFn;
   if (stringEqual(type, "unsigned char")) return unsignedCharStoreFn;
   return pointerStoreFn;
}

ToStringFn getToStringFn(string type) {
   if (stringEqual(type, "int")) return intToStringFn;
   if (stringEqual(type, "short")) return shortToStringFn;
   if (stringEqual(type, "long")) return longToStringFn;
   if (stringEqual(type, "float")) return floatToStringFn;
   if (stringEqual(type, "double")) return doubleToStringFn;
   if (stringEqual(type, "char")) return charToStringFn;
   if (stringEqual(type, "bool")) return boolToStringFn;
   if (stringEqual(type, "unsigned")) return unsignedToStringFn;
   if (stringEqual(type, "unsigned short")) return unsignedShortToStringFn;
   if (stringEqual(type, "unsigned long")) return unsignedLongToStringFn;
   if (stringEqual(type, "unsigned char")) return unsignedCharToStringFn;
   if (stringEqual(type, "char *")) return stringToStringFn;
   if (stringEqual(type, "string")) return stringToStringFn;
   return pointerToStringFn;
}

/* Private functions */

static void intFetchFn(va_list args, GenericType *dst) {
   dst->intRep = va_arg(args, int);
}

static void shortFetchFn(va_list args, GenericType *dst) {
   dst->shortRep = (short) va_arg(args, int);
}

static void longFetchFn(va_list args, GenericType *dst) {
   dst->longRep = va_arg(args, long);
}

static void floatFetchFn(va_list args, GenericType *dst) {
   dst->floatRep = (float) va_arg(args, double);
}

static void doubleFetchFn(va_list args, GenericType *dst) {
   dst->doubleRep = va_arg(args, double);
}

static void charFetchFn(va_list args, GenericType *dst) {
   dst->charRep = (char) va_arg(args, int);
}

static void unsignedFetchFn(va_list args, GenericType *dst) {
   dst->unsignedRep = va_arg(args, unsigned);
}

static void unsignedShortFetchFn(va_list args, GenericType *dst) {
   dst->unsignedShortRep = (unsigned short) va_arg(args, int);
}

static void unsignedLongFetchFn(va_list args, GenericType *dst) {
   dst->unsignedLongRep = va_arg(args, unsigned long);
}

static void unsignedCharFetchFn(va_list args, GenericType *dst) {
   dst->unsignedCharRep = (unsigned char) va_arg(args, int);
}

static void pointerFetchFn(va_list args, GenericType *dst) {
   dst->pointerRep = va_arg(args, void *);
}

static void intStoreFn(GenericType src, void *dst) {
   *((int *) dst) = src.intRep;
}

static void shortStoreFn(GenericType src, void *dst) {
   *((short *) dst) = src.shortRep;
}

static void longStoreFn(GenericType src, void *dst) {
   *((long *) dst) = src.longRep;
}

static void floatStoreFn(GenericType src, void *dst) {
   *((float *) dst) = src.floatRep;
}

static void doubleStoreFn(GenericType src, void *dst) {
   *((double *) dst) = src.doubleRep;
}

static void charStoreFn(GenericType src, void *dst) {
   *((char *) dst) = src.charRep;
}

static void unsignedStoreFn(GenericType src, void *dst) {
   *((unsigned *) dst) = src.unsignedRep;
}

static void unsignedShortStoreFn(GenericType src, void *dst) {
   *((unsigned short *) dst) = src.unsignedShortRep;
}

static void unsignedLongStoreFn(GenericType src, void *dst) {
   *((unsigned long *) dst) = src.unsignedLongRep;
}

static void unsignedCharStoreFn(GenericType src, void *dst) {
   *((unsigned char *) dst) = src.unsignedCharRep;
}

static void pointerStoreFn(GenericType src, void *dst) {
   *((void **) dst) = src.pointerRep;
}

static string intToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%d", any.intRep);
   return copyString(buffer);
}

static string shortToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%d", any.shortRep);
   return copyString(buffer);
}

static string longToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%ldL", any.longRep);
   return copyString(buffer);
}

static string floatToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%g", any.floatRep);
   return copyString(buffer);
}

static string doubleToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%g", any.doubleRep);
   return copyString(buffer);
}

static string charToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%c", any.charRep);
   return copyString(buffer);
}

static string boolToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%s", (any.boolRep) ? "true" :"false");
   return copyString(buffer);
}

static string unsignedToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%u", any.unsignedRep);
   return copyString(buffer);
}

static string unsignedShortToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%u", any.unsignedShortRep);
   return copyString(buffer);
}

static string unsignedLongToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%luL", any.unsignedLongRep);
   return copyString(buffer);
}

static string unsignedCharToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "%c", any.unsignedCharRep);
   return copyString(buffer);
}

static string stringToStringFn(GenericType any) {
   return copyString((string) any.pointerRep);
}

static string pointerToStringFn(GenericType any) {
   char buffer[BUFSIZE];

   sprintf(buffer, "0x%lX", (long) any.pointerRep);
   return copyString(buffer);
}
