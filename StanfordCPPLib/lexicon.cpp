/*
 * File: Lexicon.cpp
 * -----------------
 * A Lexicon is a word list. This Lexicon is backed by a data
 * structure called a prefix tree or trie ("try").
 *
 * This is a re-implementation of Lexicon.  Its previous implementation used
 * a pair of structures: a directed acyclic word graph (DAWG) and an STL set.
 * This implementation was discarded because of several reasons:
 *
 * - It relied on binary file formats that were not readable by students.
 * - It did not provide for expected class members like remove.
 * - It had a clunky pair of data structures that had to be searched separately.
 * - It was optimized for space usage over ease of use and maintenance.
 *
 * The original DAWG implementation is retained as dawglexicon.h/cpp.
 */

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "dawglexicon.h"
#include "error.h"
#include "lexicon.h"
#include "strlib.h"
using namespace std;

static bool isDAWGFile(string filename);
static void scrub(string& str);

Lexicon::Lexicon() {
    m_root = NULL;
    m_size = 0;
}

Lexicon::Lexicon(string filename) {
    m_root = NULL;
    m_size = 0;
    addWordsFromFile(filename);
}

Lexicon::Lexicon(const Lexicon& src) {
    m_root = NULL;
    m_size = 0;
    deepCopy(src);
}

Lexicon::~Lexicon() {
    clear();
}

bool Lexicon::add(string word) {
    scrub(word);
    if (word.empty()) {
        return false;
    }
    return addHelper(m_root, word, /* originalWord */ word);
}

void Lexicon::addWordsFromFile(string filename) {
    if (isDAWGFile(filename)) {
        readBinaryFile(filename);
    } else {
        ifstream istr(filename.c_str());
        if (istr.fail()) {
            error("Lexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
        }
        string line;
        while (getline(istr, line)) {
            add(line);
        }
        istr.close();
    }
}

void Lexicon::clear() {
    m_size = 0;
    m_allWords.clear();
    deleteTree(m_root);
    m_root = NULL;
}

bool Lexicon::contains(string word) const {
    scrub(word);
    if (word.empty()) {
        return false;
    } else {
        return containsHelper(m_root, word, /* isPrefix */ false);
    }
}

bool Lexicon::containsPrefix(string prefix) const {
    scrub(prefix);
    if (prefix.empty()) {
        return true;
    } else {
        return containsHelper(m_root, prefix, /* isPrefix */ true);
    }
}

bool Lexicon::isEmpty() const {
    return size() == 0;
}

void Lexicon::mapAll(void (*fn)(string)) const {
    __foreach__ (string word __in__ this->m_allWords) {
        fn(word);
    }
}

void Lexicon::mapAll(void (*fn)(const string &)) const {
    __foreach__ (string word __in__ this->m_allWords) {
        fn(word);
    }
}

bool Lexicon::remove(string word) {
    scrub(word);
    if (word.empty()) {
        return false;
    } else {
        return removeHelper(m_root, word, /* originalWord */ word, /* isPrefix */ false);
    }
}

bool Lexicon::removePrefix(string prefix) {
    scrub(prefix);
    if (prefix.empty()) {
        bool result = !isEmpty();
        clear();
        return result;
    } else {
        return removeHelper(m_root, prefix, /* originalWord */ prefix, /* isPrefix */ true);
    }
}

int Lexicon::size() const {
    return m_size;
}

string Lexicon::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

set<string> Lexicon::toStlSet() const {
    set<string> result;
    __foreach__ (string word __in__ this->m_allWords) {
        result.insert(word);
    }
    return result;
}


/* private helpers implementation */

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::addHelper(TrieNode*& node, string word, const string& originalWord) {
    if (node == NULL) {
        // create nodes all the way down, one for each letter of the word
        node = new TrieNode();
    }

    if (word.empty()) {
        // base case: we have added all of the letters of this word
        if (node->isWord()) {
            return false;   // duplicate word; already present
        } else {
            // new word; add it
            node->setWord(true);
            m_size++;
            m_allWords.add(originalWord);
            return true;
        }
    } else {
        // recursive case: chop off first letter, traverse the rest
        return addHelper(node->child(word[0]), word.substr(1), originalWord);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::containsHelper(TrieNode* node, string word, bool isPrefix) const {
    if (node == NULL) {
        // base case: no pointer down to here, so prefix must not exist
        return false;
    } else if (word.length() == 0) {
        // base case: Found nodes all the way down.
        // If we are looking for a prefix, this means this path IS a prefix,
        // so we should return true.
        // If we are looking for an exact word match rather than a prefix,
        // we must check the isWord flag to see that this word was added
        return (isPrefix ? true : node->isWord());
    } else {
        // recursive case: follow appropriate child pointer for one letter
        return containsHelper(node->child(word[0]), word.substr(1), isPrefix);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::removeHelper(TrieNode*& node, string word, const string& originalWord, bool isPrefix) {
    if (node == NULL) {
        // base case: dead end; this word/prefix must not be contained
        return false;
    } else if (word.empty()) {
        // base case: we have walked all of the letters of this word/prefix
        // and now we must do the removal
        if (isPrefix) {
            // remove this node and all of its descendents
            removeSubtreeHelper(node, originalWord);   // removes from m_allWords, sets m_size
            node = NULL;
        } else {
            // remove / de-word-ify this node only
            if (node->isLeaf()) {
                delete node;
                node = NULL;
            } else {
                if (node->isWord()) {
                    node->setWord(false);
                    m_allWords.remove(originalWord);
                    m_size--;
                }
            }
        }
        return true;
    } else {
        // recursive case: chop off first letter, traverse the rest
        return removeHelper(node->child(word[0]), word.substr(1), originalWord, isPrefix);
    }
}

// remove/free this node and all descendents
void Lexicon::removeSubtreeHelper(TrieNode*& node, const string& originalWord) {
    if (node != NULL) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            removeSubtreeHelper(node->child(letter), originalWord + letter);
        }
        if (node->isWord()) {
            m_allWords.remove(originalWord);
            m_size--;
        }
        delete node;
        node = NULL;
    }
}

void Lexicon::deepCopy(const Lexicon& src) {
    __foreach__ (string word __in__ src.m_allWords) {
        add(word);
    }
}

void Lexicon::deleteTree(TrieNode* node) {
    if (node != NULL) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            deleteTree(node->child(letter));
        }
        delete node;
    }
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(string filename) {
    DawgLexicon ldawg(filename);
    __foreach__ (string word __in__ ldawg) {
        add(word);
    }
}

Lexicon& Lexicon::operator=(const Lexicon& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

std::ostream& operator <<(std::ostream& out, const Lexicon& lex) {
    out << lex.m_allWords;
    return out;
}

std::istream& operator >>(std::istream& is, Lexicon& lex) {
    char ch;
    is >> ch;
    if (ch != '{') {
        error("Lexicon::operator >>: Missing {");
    }
    lex.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            string value;
            readGenericValue(is, value);
            lex.add(value);
            is >> ch;
            if (ch == '}') break;
            if (ch != ',') {
                error(std::string("Lexicon::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

// returns true if the given file (probably) represents a
// binary DAWG lexicon data file
static bool isDAWGFile(string filename) {
    char firstFour[4], expected[] = "DAWG";
    ifstream istr(filename.c_str());
    if (istr.fail()) {
        error(string("Lexicon::addWordsFromFile: Couldn't open lexicon file ") + filename);
    }
    istr.read(firstFour, 4);
    bool result = strncmp(firstFour, expected, 4) == 0;
    istr.close();
    return result;
}

// lowercases the string and also scrubs out non-alphabetic characters,
// such that it can be used as a Lexicon word
static void scrub(string & str) {
    size_t nChars = str.length();
    size_t outIndex = 0;
    for (size_t i = 0; i < nChars; i++) {
        string::value_type ch = tolower(str[i]);
        if (ch >= 'a' && ch <= 'z') {
            str[outIndex++] = ch;
        } // else throw away
    }
    if (outIndex != nChars) {
        str.erase(outIndex, nChars - outIndex);
    }
}
