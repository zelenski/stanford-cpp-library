/*
 * Test file for verifying the Stanford C++ lib collection functionality.
 */

#include "trielexicon.h"
#include "dawglexicon.h"
#include "splversion.h"
#include "SimpleTest.h"

static TrieLexicon *gTrie;

static DawgLexicon loadDawg(std::string name) {
    DawgLexicon dawg(name);
    return dawg;
}
static TrieLexicon loadTrie(std::string name) {
    TrieLexicon trie(name);
    return trie;
}

static void checkDawgAgainst(const DawgLexicon& dawg, const TrieLexicon& trie) {
    EXPECT_EQUAL(dawg.size(), trie.size());

    int dawgCount = 0, trieCount = 0;
    for (auto word : trie) {
        trieCount++;
    }
    for (auto word : dawg) {
        dawgCount++;
    }
    EXPECT_EQUAL(dawgCount, trieCount);

    for (auto word : trie) {
        EXPECT(dawg.contains(word));
    }

    EXPECT(dawg.containsPrefix(""));
    EXPECT(!dawg.contains(""));

    for (auto word : trie) {
        for (int i = 0; i < word.length(); i++) {
            auto prefix = word.substr(0, i);
            EXPECT(dawg.containsPrefix(prefix));
       }
    }

    for (auto word : trie) {
        for (int i = 0; i < word.length(); i++) {
            auto prefix = word.substr(0, i);
            auto other = prefix + "x";
            EXPECT_EQUAL(dawg.containsPrefix(other), trie.containsPrefix(other));
       }
    }
}

PROVIDED_TEST("Load EnglishWords into Trie (slow...) and Dawg (fast!)") {
    TrieLexicon trie;
    TIME_OPERATION(1, trie = loadTrie("res/EnglishWords.txt"));
    gTrie = new TrieLexicon(getLibraryPathForResource("EnglishWords.txt"));
    EXPECT_EQUAL(*gTrie, trie);
    TIME_OPERATION(1, loadDawg("res/dawgle.dat"));
}

PROVIDED_TEST("DawgLexicon, project-local, compare to trie") {
    DawgLexicon dawg("res/dawgle.dat");
    checkDawgAgainst(dawg,  *gTrie);
}

PROVIDED_TEST("DawgLexicon, big-endian data file, compare to trie") {
    DawgLexicon dawg("res/dawgbe.dat");
    checkDawgAgainst(dawg,  *gTrie);
}

PROVIDED_TEST("DawgLexicon, library-istalled, compare to trie") {
    DawgLexicon dawg(getLibraryPathForResource("EnglishWords.dat"));
    checkDawgAgainst(dawg,  *gTrie);
}

PROVIDED_TEST("DawgLexicon, errors in file read") {
    EXPECT_ERROR(loadDawg("NoExist.dat"));
    EXPECT_ERROR(loadDawg(__FILE__));
    EXPECT_ERROR(loadDawg("res/badtag.dat"));
    EXPECT_ERROR(loadDawg("res/badsizeformat.dat"));
    EXPECT_ERROR(loadDawg("res/badstartformat.dat"));
    EXPECT_ERROR(loadDawg("res/badsize.dat"));
    EXPECT_ERROR(loadDawg("res/badstart.dat"));
    EXPECT_ERROR(loadDawg("res/badletter.dat"));
    EXPECT_ERROR(loadDawg("res/badchildren.dat"));
}

PROVIDED_TEST("DawgLexicon, empty") {
    DawgLexicon emptyDawg("");
    TrieLexicon emptyTrie;

    EXPECT_EQUAL(emptyDawg.size(), 0);
    int dawgCount = 0;
    for (auto word : emptyDawg) {
        dawgCount++;
    }
    EXPECT_EQUAL(dawgCount, 0);

    EXPECT_EQUAL(emptyDawg.contains(""), emptyTrie.contains(""));
    EXPECT_EQUAL(emptyDawg.containsPrefix(""), emptyTrie.containsPrefix(""));

    checkDawgAgainst(emptyDawg, emptyTrie);
}
