/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */


#include "lexicon.h"
#include "hashcode.h"
#include "hashset.h"
#include "queue.h"
#include "common.h"
#include "testing/SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>



PROVIDED_TEST("Lexicon, basic") {
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
    EXPECT_EQUAL( (int) words.size(), lex.size());

    for (std::string word : words) {
        EXPECT(lex.contains(word));
    }

    for (std::string word : badWords) {
        EXPECT(!lex.contains(word));
    }

    for (std::string word : words) {
        for (int i = 0; i < (int) word.length(); i++) {
            std::string prefix = word.substr(0, i);
            EXPECT(lex.containsPrefix(word));
        }
    }

    for (std::string word : badPrefixes) {
        EXPECT(!lex.containsPrefix(word));
    }
}

PROVIDED_TEST("Lexicon, compare") {
    Lexicon lex;
    lex.add("a");
    lex.add("ab");
    lex.add("bc");
    Lexicon lex2;
    lex2.add("a");
    lex2.add("b");
    lex2.add("c");
    Lexicon lex3;
    testCompareOperators(lex, lex2, LessThan);
    testCompareOperators(lex2, lex, GreaterThan);
    testCompareOperators(lex, lex, EqualTo);

    Set<Lexicon> slex {lex, lex2, lex3};
    EXPECT_EQUAL( "{{}, {\"a\", \"ab\", \"bc\"}, {\"a\", \"b\", \"c\"}}", slex.toString());
}

PROVIDED_TEST("Lexicon, forEach") {
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


PROVIDED_TEST("Lexicon, hashCode") {
    Lexicon lex;
    lex.add("a");
    lex.add("bc");
    EXPECT_EQUAL( hashCode(lex), hashCode(lex));

    Lexicon copy = lex;
    EXPECT_EQUAL( hashCode(lex), hashCode(copy));

    Lexicon lex2;   // empty

    // shouldn't add two copies of same lexicon
    HashSet<Lexicon> hashlex {lex, copy, lex2};
    EXPECT_EQUAL( 2, hashlex.size());
}

PROVIDED_TEST("Lexicon, initializerList") {
    std::initializer_list<std::string> lexlist = {"sixty", "seventy"};

    Lexicon lex {"ten", "twenty", "thirty"};
    EXPECT_EQUAL( "{\"ten\", \"thirty\", \"twenty\"}", lex.toString());
    EXPECT_EQUAL( 3, lex.size());
    EXPECT( lex.contains("ten"));
    EXPECT( lex.contains("twenty"));
    EXPECT( lex.contains("thirty"));
    EXPECT(! lex.contains("forty"));
    EXPECT(! lex.contains("fifty"));

    lex += {"forty", "fifty"};
    EXPECT_EQUAL( "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", lex.toString());
    EXPECT_EQUAL( 5, lex.size());
    EXPECT( lex.contains("ten"));
    EXPECT( lex.contains("twenty"));
    EXPECT( lex.contains("thirty"));
    EXPECT( lex.contains("forty"));
    EXPECT( lex.contains("fifty"));
    EXPECT(! lex.contains("sixty"));
    EXPECT(! lex.contains("seventy"));

    Lexicon lex2 = (lex + lexlist);
    EXPECT_EQUAL( "{\"fifty\", \"forty\", \"ten\", \"thirty\", \"twenty\"}", lex.toString());
    EXPECT_EQUAL( 5, lex.size());
    EXPECT( lex.contains("ten"));
    EXPECT( lex.contains("twenty"));
    EXPECT( lex.contains("thirty"));
    EXPECT( lex.contains("forty"));
    EXPECT( lex.contains("fifty"));
    EXPECT(! lex.contains("sixty"));
    EXPECT(! lex.contains("seventy"));

    EXPECT_EQUAL( "{\"fifty\", \"forty\", \"seventy\", \"sixty\", \"ten\", \"thirty\", \"twenty\"}", lex2.toString());
    EXPECT_EQUAL( 7, lex2.size());
    EXPECT( lex2.contains("ten"));
    EXPECT( lex2.contains("twenty"));
    EXPECT( lex2.contains("thirty"));
    EXPECT( lex2.contains("forty"));
    EXPECT( lex2.contains("fifty"));
    EXPECT( lex2.contains("sixty"));
    EXPECT( lex2.contains("seventy"));
}

void addDuring(Lexicon& v) { for (auto m : v) v.add("garbageword"); }
void removeDuring(Lexicon& v) { for (auto m : v) v.remove(m); }

PROVIDED_TEST("Lexicon, error on modify during iterate") {
    Lexicon lex {"ten", "twenty", "thirty", "forty"};
   EXPECT_ERROR(addDuring(lex));
   EXPECT_ERROR(removeDuring(lex));
}

