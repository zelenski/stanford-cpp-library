/*
 * File: ref.c
 * Last modified on Mon Jan 14 11:36:55 2008 by eroberts
 * -----------------------------------------------------
 * This file implements the ref.h interface.
 */

#include <stdio.h>
#include "cslib.h"
#include "ref.h"

/*
 * Constant: REF_PASSWORD
 * ----------------------
 * This value is stored in the block to guard against pointer
 * errors.  If the value does not appear, the ref is not valid.
 */

#define REF_PASSWORD 161803398UL

/*
 * Type: RefType
 * -------------
 * This enumeration type defines ref type codes (with the suffix
 * RTC) for all the exported ref types.
 */

typedef enum {
   IntRTC,
   ShortRTC,
   LongRTC,
   FloatRTC,
   DoubleRTC,
   CharRTC,
   BoolRTC,
   UnsignedRTC,
   UnsignedShortRTC,
   UnsignedLongRTC,
   UnsignedCharRTC
} RefType;

/*
 * Type: RefBlock
 * --------------
 * This tagged union type is used to store the ref and is used
 * to allow type-checking on the selector function.  The union
 * value must appear at the beginning of the block to maintain
 * the semantics.
 */

typedef struct {
   union {
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
   } value;
   unsigned long password;
   RefType type;
} RefBlock;

/* Private function prototypes */

static void refTypeCheck(void *ref, RefType type, string fn);

/* Exported entries */

void *newRefInt(int value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.intRep = value;
   ref->password = REF_PASSWORD;
   ref->type = IntRTC;
   return ref;
}

void *newRefShort(short value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.shortRep = value;
   ref->password = REF_PASSWORD;
   ref->type = ShortRTC;
   return ref;
}

void *newRefLong(long value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.longRep = value;
   ref->password = REF_PASSWORD;
   ref->type = LongRTC;
   return ref;
}

void *newRefFloat(float value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.floatRep = value;
   ref->password = REF_PASSWORD;
   ref->type = FloatRTC;
   return ref;
}

void *newRefDouble(double value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.doubleRep = value;
   ref->password = REF_PASSWORD;
   ref->type = DoubleRTC;
   return ref;
}

void *newRefChar(char value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.charRep = value;
   ref->password = REF_PASSWORD;
   ref->type = CharRTC;
   return ref;
}

void *newRefBool(bool value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.boolRep = value;
   ref->password = REF_PASSWORD;
   ref->type = BoolRTC;
   return ref;
}

void *newRefUnsigned(unsigned value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.unsignedRep = value;
   ref->password = REF_PASSWORD;
   ref->type = UnsignedRTC;
   return ref;
}

void *newRefUnsignedShort(unsigned short value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.unsignedShortRep = value;
   ref->password = REF_PASSWORD;
   ref->type = UnsignedShortRTC;
   return ref;
}

void *newRefUnsignedLong(unsigned long value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.unsignedLongRep = value;
   ref->password = REF_PASSWORD;
   ref->type = UnsignedLongRTC;
   return ref;
}

void *newRefUnsignedChar(unsigned char value) {
   RefBlock *ref;

   ref = (RefBlock *) getBlock(sizeof (RefBlock));
   ref->value.unsignedCharRep = value;
   ref->password = REF_PASSWORD;
   ref->type = UnsignedCharRTC;
   return ref;
}

int refToInt(void *ref) {
   refTypeCheck(ref, IntRTC, "refToInt");
   return *((int *) ref);
}

short refToShort(void *ref) {
   refTypeCheck(ref, ShortRTC, "refToShort");
   return *((short *) ref);
}

long refToLong(void *ref) {
   refTypeCheck(ref, LongRTC, "refToLong");
   return *((long *) ref);
}

float refToFloat(void *ref) {
   refTypeCheck(ref, FloatRTC, "refToFloat");
   return *((float *) ref);
}

double refToDouble(void *ref) {
   refTypeCheck(ref, DoubleRTC, "refToDouble");
   return *((double *) ref);
}

char refToChar(void *ref) {
   refTypeCheck(ref, CharRTC, "refToChar");
   return *((char *) ref);
}

bool refToBool(void *ref) {
   refTypeCheck(ref, BoolRTC, "refToBool");
   return *((bool *) ref);
}

unsigned refToUnsigned(void *ref) {
   refTypeCheck(ref, UnsignedRTC, "refToUnsigned");
   return *((unsigned *) ref);
}

unsigned short refToUnsignedShort(void *ref) {
   refTypeCheck(ref, UnsignedShortRTC, "refToUnsignedShort");
   return *((unsigned short *) ref);
}

unsigned long refToUnsignedLong(void *ref) {
   refTypeCheck(ref, UnsignedLongRTC, "refToUnsignedLong");
   return *((unsigned long *) ref);
}

unsigned char refToUnsignedChar(void *ref) {
   refTypeCheck(ref, UnsignedCharRTC, "refToUnsignedChar");
   return *((unsigned char *) ref);
}

static void refTypeCheck(void *ref, RefType type, string fn) {
   RefBlock *rp;

   if (rp == NULL) error("%s: NULL argument", fn);
   rp = (RefBlock *) ref;
   if (rp->password != REF_PASSWORD) {
      error("%s: Illegal ref", fn);
   }
   if (rp->type != type) {
      error("%s: Ref type does not match", fn);
   }
}
