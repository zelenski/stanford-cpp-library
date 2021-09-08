/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "testcases.h"
#include "lexicon.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "assertions.h"
#include "collection-test-common.h"
#include "gtest-marty.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

TEST_CATEGORY(LexiconTests, "Lexicon tests");

TIMED_TEST(LexiconTests, basicTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
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

    Lexicon lex;
    for (std::string word : words) {
        lex.add(word);
    }
    assertEquals("Lexicon size", (int) words.size(), lex.size());

    for (std::string word : words) {
        assertTrue("Lexicon contains " + word, lex.contains(word));
    }

    for (std::string word : badWords) {
        assertFalse("Lexicon contains " + word, lex.contains(word));
    }

    for (std::string word : words) {
        for (int i = 0; i < (int) word.length(); i++) {
            std::string prefix = word.substr(0, i);
            assertTrue("Lexicon containsPrefix " + word, lex.containsPrefix(word));
        }
    }

    for (std::string word : badPrefixes) {
        assertFalse("Lexicon containsPrefix " + word, lex.containsPrefix(word));
    }
}

TIMED_TEST(LexiconTests, compareTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    Lexicon lex;
    lex.add("a");
    lex.add("ab");
    lex.add("bc");
    Lexicon lex2;
    lex2.add("a");
    lex2.add("b");
    lex2.add("c");
    Lexicon lex3;
    compareTestHelper(lex, lex2, "Lexicon", /* compareTo */ -1);
    compareTestHelper(lex2, lex, "Lexicon", /* compareTo */ 1);
    compareTestHelper(lex, lex, "Lexicon", /* compareTo */ 0);

    Set<Lexicon> slex {lex, lex2, lex3};
    assertEqualsString("slex", "{{}, {\"a\", \"ab\", \"bc\"}, {\"a\", \"b\", \"c\"}}", slex.toString());
}

TIMED_TEST(LexiconTests, forEachTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    Lexicon lex;
    lex.add("a");
    lex.add("aba");
    lex.add("cc");
    lex.add("bbb");
    lex.add("c");
    lex.add("bart");
    std::initializer_list<std::string> list {"a", "aba", "bart", "bbb", "c", "cc"};
    assertCollection("foreach Lexicon", list, lex);
}

TIMED_TEST(LexiconTests, frontBackTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    Lexicon lex {"apple", "apricot", "banana", "zebra"};
    assertEqualsString("Lexicon front", "apple", lex.front());
    assertEqualsString("Lexicon back",  "zebra", lex.back());
}

TIMED_TEST(LexiconTests, hashCodeTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    Lexicon lex;
    lex.add("a");
    lex.add("bc");
    assertEqualsInt("hashcode of self lexicon", hashCode(lex), hashCode(lex));

    Lexicon copy = lex;
    assertEqualsInt("hashcode of copy lexicon", hashCode(lex), hashCode(copy));

    Lexicon lex2;   // empty

    // shouldn't add two copies of same lexicon
    HashSet<Lexicon> hashlex {lex, copy, lex2};
    assertEqualsInt("hashset of lexicon size", 2, hashlex.size());
}

TIMED_TEST(LexiconTests, initializerListTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};

    Lexicon lex {"ten", "twenty", "thirty"};
    assertEqualsString("init list Lexicon", "{\"ten\", \"thirty\", \"twenty\"}", lex.toString());
    assertEqualsInt("init list Lexicon size", 3, lex.size());
    assertTrue("init list Lexicon contains ten", lex.contains("ten"));
    assertTrue("init list Lexicon contains twenty", lex.contains("twenty"));
    assertTrue("init list Lexicon contains thirty", lex.contains("thirty"));
    assertFalse("init list Lexicon contains forty", lex.contains("forty"));
    assertFalse("init list Lexicon contains fifty", lex.contains("fifty"));

    lex += {"forty", "fifty"};
    assertEqualsString("after += Lexicon", "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", lex.toString());
    assertEqualsInt("after += Lexicon size", 5, lex.size());
    assertTrue("init list Lexicon contains ten", lex.contains("ten"));
    assertTrue("init list Lexicon contains twenty", lex.contains("twenty"));
    assertTrue("init list Lexicon contains thirty", lex.contains("thirty"));
    assertTrue("init list Lexicon contains forty", lex.contains("forty"));
    assertTrue("init list Lexicon contains fifty", lex.contains("fifty"));
    assertFalse("init list Lexicon contains sixty", lex.contains("sixty"));
    assertFalse("init list Lexicon contains seventy", lex.contains("seventy"));

    Lexicon lex2 = (lex + lexlist);
    assertEqualsString("after += Lexicon", "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", lex.toString());
    assertEqualsInt("after + Lexicon size", 5, lex.size());
    assertTrue("init list Lexicon contains ten", lex.contains("ten"));
    assertTrue("init list Lexicon contains twenty", lex.contains("twenty"));
    assertTrue("init list Lexicon contains thirty", lex.contains("thirty"));
    assertTrue("init list Lexicon contains forty", lex.contains("forty"));
    assertTrue("init list Lexicon contains fifty", lex.contains("fifty"));
    assertFalse("init list Lexicon contains sixty", lex.contains("sixty"));
    assertFalse("init list Lexicon contains seventy", lex.contains("seventy"));

    assertEqualsString("after + Lexicon 2", "{\"fifty\", \"forty\", \"seventy\", \"sixty\", \"ten\", \"thirty\", \"twenty\"}", lex2.toString());
    assertEqualsInt("after + Lexicon 2 size", 7, lex2.size());
    assertTrue("init list Lexicon contains ten", lex2.contains("ten"));
    assertTrue("init list Lexicon contains twenty", lex2.contains("twenty"));
    assertTrue("init list Lexicon contains thirty", lex2.contains("thirty"));
    assertTrue("init list Lexicon contains forty", lex2.contains("forty"));
    assertTrue("init list Lexicon contains fifty", lex2.contains("fifty"));
    assertTrue("init list Lexicon contains sixty", lex2.contains("sixty"));
    assertTrue("init list Lexicon contains seventy", lex2.contains("seventy"));
}

#ifdef SPL_THROW_ON_INVALID_ITERATOR
TIMED_TEST(LexiconTests, iteratorVersionTest_Lexicon, TEST_TIMEOUT_DEFAULT) {
    Lexicon lex {"ten", "twenty", "thirty", "forty"};
    try {
        for (std::string s : lex) {
            if (s.length() % 2 == 0) {
                lex.remove(s);
            }
        }
        assertFail("should not get to end of test; should throw exception before now");
    } catch (ErrorException ex) {
        assertPass("threw exception successfully");
    }
}
#endif // SPL_THROW_ON_INVALID_ITERATOR
