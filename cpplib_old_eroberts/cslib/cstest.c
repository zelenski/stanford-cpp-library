#include <stdio.h>
#include "cslib.h"
#include "strlib.h"
#include "unittest.h"

#define PUBLIC      0
#define PRIVATE     0x1
#define INTERACTIVE 0x2

typedef struct {
   string name;
   proc fn;
   int flags;
} TestEntry;

extern void testBSTModule(void);
extern void testCharSetModule(void);
extern void testExceptionModule(void);
extern void testFilelibModule(void);
extern void testGEventsModule(void);
extern void testGraphModule(void);
extern void testGTransformModule(void);
extern void testGTypesModule(void);
extern void testHashMapModule(void);
extern void testMapModule(void);
extern void testOptionsModule(void);
extern void testPriorityQueueModule(void);
extern void testQueueModule(void);
extern void testRandomModule(void);
extern void testSetModule(void);
extern void testStackModule(void);
extern void testStrbufModule(void);
extern void testStrlibModule(void);
extern void testThreadModule(void);
extern void testTokenScannerModule(void);
extern void testVectorModule(void);

static TestEntry TEST_MODULES[] = {
   { "bst", testBSTModule },
   { "charset", testCharSetModule },
   { "exception", testExceptionModule },
   { "filelib", testFilelibModule },
   { "gevents", testGEventsModule },
   { "graph", testGraphModule },
   { "gtransform", testGTransformModule },
   { "gtypes", testGTypesModule },
   { "hashmap", testHashMapModule },
   { "map", testMapModule },
   { "options", testOptionsModule },
   { "pqueue", testPriorityQueueModule },
   { "queue", testQueueModule },
   { "random", testRandomModule },
   { "set", testSetModule },
   { "stack", testStackModule },
   { "strbuf", testStrbufModule },
   { "strlib", testStrlibModule },
   { "thread", testThreadModule },
   { "tokenscanner", testTokenScannerModule },
   { "vector", testVectorModule },
};

static int N_TEST_MODULES = sizeof TEST_MODULES / sizeof TEST_MODULES[0];

static int findTestModule(string name);

main(int argc, string argv[]) {
   int i, index;
   string arg;
   bool modulesFound;

   modulesFound = false;
   for (i = 1; i < argc; i++) {
      arg = argv[i];
      if (startsWith(arg, "-")) {
         if (stringEqual(arg, "-v") || stringEqual(arg, "-verbose")) {
            setVerboseTestingFlag(true);
         } else {
            error("Unrecognized option %s", arg);
         }
      } else {
         index = findTestModule(arg);
         if (index == -1) {
            error("No module named %s", arg);
         }
         testModule(TEST_MODULES[index].name, TEST_MODULES[index].fn);
         modulesFound = true;
      }
   }
   if (!modulesFound) {
      for (i = 0; i < N_TEST_MODULES; i++) {
         if ((TEST_MODULES[i].flags & (INTERACTIVE | PRIVATE)) == 0) {
            testModule(TEST_MODULES[i].name, TEST_MODULES[i].fn);
         }
      }
   }
   exit(0);
}

static int findTestModule(string name) {
   int i, index;

   index = -1;
   for (i = 0; i < N_TEST_MODULES; i++) {
      if (startsWith(TEST_MODULES[i].name, name)) {
         if (index != -1) return -1;
         index = i;
      }
   }
   return index;
}
