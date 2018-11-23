/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "dawglexicon.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(DawgLexiconTests, "DawgLexicon tests");

TIMED_TEST(DawgLexiconTests, basicTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::string> words = {
        "a",
        "ab",
        "aab",
        "aaab",
        "aardvark",
        "b",
        "banana"
    };
    std::initializer_list<std::string> badWords = {
        "abb",
        "ad",
        "and",
        "aaardvark",
        "aardvarks",
    };
    std::initializer_list<std::string> badPrefixes = {
        "aaaa",
        "abb",
        "aardvarz",
        "bb",
        "bananas",
        "c",
        "r",
        "z"
    };

    DawgLexicon dawg;
    for (std::string word : words) {
        dawg.add(word);
    }
    assertEquals("DawgLexicon size", (int) words.size(), dawg.size());

    for (std::string word : words) {
        assertTrue("DawgLexicon contains " + word, dawg.contains(word));
    }

    for (std::string word : badWords) {
        assertFalse("DawgLexicon contains " + word, dawg.contains(word));
    }

    for (std::string word : words) {
        for (int i = 0; i < (int) word.length(); i++) {
            std::string prefix = word.substr(0, i);
            assertTrue("DawgLexicon containsPrefix " + word, dawg.containsPrefix(word));
        }
    }

    for (std::string word : badPrefixes) {
        assertFalse("DawgLexicon containsPrefix " + word, dawg.containsPrefix(word));
    }
}

TIMED_TEST(DawgLexiconTests, compareTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    DawgLexicon dawg;
    dawg.add("a");
    dawg.add("b");
    dawg.add("c");
    DawgLexicon dawg2;
    dawg2.add("a");
    dawg2.add("ab");
    dawg2.add("bc");
    DawgLexicon dawg3;
    compareTestHelper(dawg, dawg2, "DawgLexicon", /* compareTo */ 1);
    compareTestHelper(dawg2, dawg, "DawgLexicon", /* compareTo */ -1);
    compareTestHelper(dawg, dawg, "DawgLexicon", /* compareTo */ 0);

    Set<DawgLexicon> sdlex {dawg, dawg2, dawg3};
    assertEqualsString("sdlex", "{{}, {\"a\", \"ab\", \"bc\"}, {\"a\", \"b\", \"c\"}}", sdlex.toString());
}

TIMED_TEST(DawgLexiconTests, forEachTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    DawgLexicon dlex;
    dlex.add("a");
    dlex.add("cc");
    dlex.add("bbb");
    Queue<std::string> expected {"a", "bbb", "cc"};
    for (std::string word : dlex) {
        std::string exp = expected.dequeue();
        assertEqualsString("DawgLexicon foreach", exp, word);
    }
}

TIMED_TEST(DawgLexiconTests, frontBackTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    DawgLexicon dlex {"apple", "apricot", "banana", "zebra"};
    assertEqualsString("DawgLexicon front", "apple", dlex.front());
    // assertEqualsString("DawgLexicon back",  "zebra", dlex.back());
}

TIMED_TEST(DawgLexiconTests, hashCodeTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    DawgLexicon dlex;
    dlex.add("a");
    dlex.add("abc");
    assertEqualsInt("hashcode of self dawglexicon", hashCode(dlex), hashCode(dlex));

    DawgLexicon copy = dlex;
    assertEqualsInt("hashcode of copy dawglexicon", hashCode(dlex), hashCode(copy));

    DawgLexicon dlex2;   // empty

    // shouldn't add two copies of same lexicon
    HashSet<DawgLexicon> hashdawg {dlex, copy, dlex2};
    assertEqualsInt("hashset of dawglexicon size", 2, hashdawg.size());
}

TIMED_TEST(DawgLexiconTests, initializerListTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};

    DawgLexicon dlex {"ten", "twenty", "thirty"};
    assertEqualsString("init list DawgLexicon", "{\"ten\", \"thirty\", \"twenty\"}", dlex.toString());
    assertEqualsInt("init list DawgLexicon size", 3, dlex.size());
    assertTrue("init list DawgLexicon contains ten", dlex.contains("ten"));
    assertTrue("init list DawgLexicon contains twenty", dlex.contains("twenty"));
    assertTrue("init list DawgLexicon contains thirty", dlex.contains("thirty"));
    assertFalse("init list DawgLexicon contains forty", dlex.contains("forty"));
    assertFalse("init list DawgLexicon contains fifty", dlex.contains("fifty"));

    dlex += {"forty", "fifty"};
    assertEqualsString("after += DawgLexicon", "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", dlex.toString());
    assertEqualsInt("after += DawgLexicon size", 5, dlex.size());
    assertTrue("init list DawgLexicon contains ten", dlex.contains("ten"));
    assertTrue("init list DawgLexicon contains twenty", dlex.contains("twenty"));
    assertTrue("init list DawgLexicon contains thirty", dlex.contains("thirty"));
    assertTrue("init list DawgLexicon contains forty", dlex.contains("forty"));
    assertTrue("init list DawgLexicon contains fifty", dlex.contains("fifty"));
    assertFalse("init list DawgLexicon contains sixty", dlex.contains("sixty"));
    assertFalse("init list DawgLexicon contains seventy", dlex.contains("seventy"));

    DawgLexicon dlex2 = (dlex + lexlist);
    assertEqualsString("after += DawgLexicon", "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", dlex.toString());
    assertEqualsInt("after + DawgLexicon size", 5, dlex.size());
    assertTrue("init list DawgLexicon contains ten", dlex.contains("ten"));
    assertTrue("init list DawgLexicon contains twenty", dlex.contains("twenty"));
    assertTrue("init list DawgLexicon contains thirty", dlex.contains("thirty"));
    assertTrue("init list DawgLexicon contains forty", dlex.contains("forty"));
    assertTrue("init list DawgLexicon contains fifty", dlex.contains("fifty"));
    assertFalse("init list DawgLexicon contains sixty", dlex.contains("sixty"));
    assertFalse("init list DawgLexicon contains seventy", dlex.contains("seventy"));

    assertEqualsString("after + DawgLexicon 2", "{\"fifty\", \"forty\", \"seventy\", \"sixty\", \"ten\", \"thirty\", \"twenty\"}", dlex2.toString());
    assertEqualsInt("after + DawgLexicon 2 size", 7, dlex2.size());
    assertTrue("init list DawgLexicon contains ten", dlex2.contains("ten"));
    assertTrue("init list DawgLexicon contains twenty", dlex2.contains("twenty"));
    assertTrue("init list DawgLexicon contains thirty", dlex2.contains("thirty"));
    assertTrue("init list DawgLexicon contains forty", dlex2.contains("forty"));
    assertTrue("init list DawgLexicon contains fifty", dlex2.contains("fifty"));
    assertTrue("init list DawgLexicon contains sixty", dlex2.contains("sixty"));
    assertTrue("init list DawgLexicon contains seventy", dlex2.contains("seventy"));
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(DawgLexiconTests, iteratorVersionTest_DawgLexicon, TEST_TIMEOUT_DEFAULT) {
    DawgLexicon lex {"ten", "twenty", "thirty", "forty"};
    try {
        for (std::string s : lex) {
            if (s.length() % 2 == 0) {
                lex.add("hi");
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR
