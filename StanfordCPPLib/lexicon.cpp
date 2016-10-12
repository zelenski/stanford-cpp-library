/*
 * File: lexicon.cpp
 * -----------------
 * A Lexicon is a word list. This Lexicon is backed by a data
 * structure called a prefix tree or trie ("try").
 *
 * This is a re-implementation of Lexicon.  Its previous implementation used
 * a pair of structures: a directed acyclic word graph (DAWG) and an STL set.
 * That implementation was discarded because of several reasons:
 *
 * - It relied on binary file formats that were not readable by students.
 * - It did not provide for expected class members like remove.
 * - It had a clunky pair of data structures that had to be searched separately.
 * - It was optimized for space usage over ease of use and maintenance.
 *
 * The original DAWG implementation is retained as dawglexicon.h/cpp.
 * 
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/11
 * - added operators +, +=, -, -=, *, *= to better match Set/HashSet
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - added comparison operators ==, !=
 * - removed 'using namespace' statement
 */

#include "lexicon.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "collections.h"
#include "dawglexicon.h"
#include "error.h"
#include "filelib.h"
#include "hashcode.h"
#include "strlib.h"

static bool scrub(std::string& str);

Lexicon::Lexicon() :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    // empty
}

Lexicon::Lexicon(std::istream& input) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addWordsFromFile(input);
}

Lexicon::Lexicon(const std::string& filename) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addWordsFromFile(filename);
}

Lexicon::Lexicon(std::initializer_list<std::string> list) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addAll(list);
}

Lexicon::Lexicon(const Lexicon& src) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    deepCopy(src);
}

Lexicon::~Lexicon() {
    clear();
}

bool Lexicon::add(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return addHelper(m_root, scrubbed, /* originalWord */ scrubbed);
}

Lexicon& Lexicon::addAll(const Lexicon& lex) {
    for (const std::string& word : lex) {
        add(word);
    }
    return *this;
}

Lexicon& Lexicon::addAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        add(word);
    }
    return *this;
}

void Lexicon::addWordsFromFile(std::istream& input) {
    bool isDAWG = isDAWGFile(input);
    rewindStream(input);
    if (isDAWG) {
        readBinaryFile(input);
    } else {
        if (input.fail()) {
            error("Lexicon::addWordsFromFile: Couldn't read from input");
        }
        std::string line;
        while (getline(input, line)) {
            add(trim(line));
        }
    }
}

void Lexicon::addWordsFromFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error("Lexicon::addWordsFromFile: Couldn't read from input file " + filename);
    }
    addWordsFromFile(input);
    input.close();
}

void Lexicon::clear() {
    m_size = 0;
    m_allWords.clear();
    deleteTree(m_root);
    m_root = nullptr;
}

bool Lexicon::contains(const std::string& word) const {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(m_root, scrubbed, /* isPrefix */ false);
}

bool Lexicon::containsAll(const Lexicon& lex2) const {
    for (const std::string& word : lex2) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool Lexicon::containsAll(std::initializer_list<std::string> list) const {
    for (const std::string& word : list) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool Lexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(m_root, scrubbed, /* isPrefix */ true);
}

bool Lexicon::equals(const Lexicon& lex2) const {
    return stanfordcpplib::collections::equals(*this, lex2);
}

std::string Lexicon::first() const {
    if (isEmpty()) {
        error("Lexicon::first: lexicon is empty");
    }
    return m_allWords.first();
}

void Lexicon::insert(const std::string& word) {
    add(word);
}

bool Lexicon::isEmpty() const {
    return size() == 0;
}

bool Lexicon::isSubsetOf(const Lexicon& lex2) const {
    auto it = begin();
    auto end = this->end();
    while (it != end) {
        if (!lex2.contains(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

bool Lexicon::isSubsetOf(std::initializer_list<std::string> list) const {
    Lexicon lex2(list);
    return isSubsetOf(lex2);
}

bool Lexicon::isSupersetOf(const Lexicon& lex2) const {
    return containsAll(lex2);
}

bool Lexicon::isSupersetOf(std::initializer_list<std::string> list) const {
    return containsAll(list);
}

void Lexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

void Lexicon::mapAll(void (*fn)(const std::string&)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

bool Lexicon::remove(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return removeHelper(m_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ false);
}

Lexicon& Lexicon::removeAll(const Lexicon& lex2) {
    Vector<std::string> toRemove;
    for (const std::string& word : *this) {
        if (lex2.contains(word)) {
            toRemove.add(word);
        }
    }
    for (const std::string& word : toRemove) {
        remove(word);
    }
    return *this;
}

Lexicon& Lexicon::removeAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        remove(word);
    }
    return *this;
}

bool Lexicon::removePrefix(const std::string& prefix) {
    if (prefix.empty()) {
        bool result = !isEmpty();
        clear();
        return result;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }
    
    return removeHelper(m_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ true);
}

Lexicon& Lexicon::retainAll(const Lexicon& lex2) {
    Vector<std::string> toRemove;
    for (const std::string& word : *this) {
        if (!lex2.contains(word)) {
            toRemove.add(word);
        }
    }
    for (const std::string& word : toRemove) {
        remove(word);
    }
    return *this;
}

Lexicon& Lexicon::retainAll(std::initializer_list<std::string> list) {
    Lexicon lex2(list);
    return retainAll(lex2);
}

int Lexicon::size() const {
    return m_size;
}

std::set<std::string> Lexicon::toStlSet() const {
    std::set<std::string> result;
    for (const std::string& word : m_allWords) {
        result.insert(word);
    }
    return result;
}

std::string Lexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

/*
 * Operators
 */
bool Lexicon::operator ==(const Lexicon& lex2) const {
    return equals(lex2);
}

bool Lexicon::operator !=(const Lexicon& lex2) const {
    return !equals(lex2);
}

bool Lexicon::operator <(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) < 0;
}

bool Lexicon::operator <=(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) <= 0;
}

bool Lexicon::operator >(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) > 0;
}

bool Lexicon::operator >=(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) >= 0;
}

Lexicon Lexicon::operator +(const Lexicon& lex2) const {
    Lexicon lex = *this;
    lex.addAll(lex2);
    return lex;
}

Lexicon Lexicon::operator +(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    lex.addAll(list);
    return lex;
}

Lexicon Lexicon::operator +(const std::string& word) const {
    Lexicon lex = *this;
    lex.add(word);
    return lex;
}

Lexicon Lexicon::operator *(const Lexicon& lex2) const {
    Lexicon lex = *this;
    return lex.retainAll(lex2);
}

Lexicon Lexicon::operator *(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    return lex.retainAll(list);
}

Lexicon Lexicon::operator -(const Lexicon& lex2) const {
    Lexicon lex = *this;
    return lex.removeAll(lex2);
}

Lexicon Lexicon::operator -(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    return lex.removeAll(list);
}

Lexicon Lexicon::operator -(const std::string& word) const {
    Lexicon lex = *this;
    lex.remove(word);
    return lex;
}

Lexicon& Lexicon::operator +=(const Lexicon& lex2) {
    return addAll(lex2);
}

Lexicon& Lexicon::operator +=(std::initializer_list<std::string> list) {
    return addAll(list);
}

Lexicon& Lexicon::operator +=(const std::string& word) {
    add(word);
    m_removeFlag = false;
    return *this;
}

Lexicon& Lexicon::operator *=(const Lexicon& lex2) {
    return retainAll(lex2);
}

Lexicon& Lexicon::operator *=(std::initializer_list<std::string> list) {
    return retainAll(list);
}

Lexicon& Lexicon::operator -=(const Lexicon& lex2) {
    return removeAll(lex2);
}

Lexicon& Lexicon::operator -=(std::initializer_list<std::string> list) {
    return removeAll(list);
}

Lexicon& Lexicon::operator -=(const std::string& word) {
    remove(word);
    m_removeFlag = true;
    return *this;
}

/* private helpers implementation */

Lexicon& Lexicon::operator ,(const std::string& word) {
    if (m_removeFlag) {
        remove(word);
    } else {
        add(word);
    }
    return *this;
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::addHelper(TrieNode*& node, const std::string& word, const std::string& originalWord) {
    if (!node) {
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
bool Lexicon::containsHelper(TrieNode* node, const std::string& word, bool isPrefix) const {
    if (!node) {
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
bool Lexicon::removeHelper(TrieNode*& node, const std::string& word, const std::string& originalWord, bool isPrefix) {
    if (!node) {
        // base case: dead end; this word/prefix must not be contained
        return false;
    } else if (word.empty()) {
        // base case: we have walked all of the letters of this word/prefix
        // and now we must do the removal
        if (isPrefix) {
            // remove this node and all of its descendents
            removeSubtreeHelper(node, originalWord);   // removes from m_allWords, sets m_size
            node = nullptr;
        } else {
            // found this word in the lexicon;
            if (node->isLeaf()) {
                // remove this leaf node only
                delete node;
                node = nullptr;
            } else {
                // de-word-ify this node, but leave it because it may
                // still have children that are valid words
                if (node->isWord()) {
                    node->setWord(false);
                }
            }
            m_allWords.remove(originalWord);
            m_size--;
        }
        return true;
    } else {
        // recursive case: chop off first letter, traverse the rest
        bool wasLeaf = node->isLeaf();
        bool result = removeHelper(node->child(word[0]), word.substr(1), originalWord, isPrefix);

        // memory cleanup: if I wasn't a leaf but now am, and am not a word,
        // then I am now unneeded, so remove me too
        if (result && !wasLeaf && node
                && node->isLeaf() && !node->isWord()) {
            delete node;
            node = nullptr;
        }
        return result;
    }
}

// remove/free this node and all descendents
void Lexicon::removeSubtreeHelper(TrieNode*& node, const std::string& originalWord) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            removeSubtreeHelper(node->child(letter), originalWord + letter);
        }
        if (node->isWord()) {
            m_allWords.remove(originalWord);
            m_size--;
        }
        delete node;
        node = nullptr;
    }
}

void Lexicon::deepCopy(const Lexicon& src) {
    for (std::string word : src.m_allWords) {
        add(word);
    }
}

void Lexicon::deleteTree(TrieNode* node) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            deleteTree(node->child(letter));
        }
        delete node;
    }
}

/*
 * Returns true if the given file (probably) represents a
 * binary DAWG lexicon data file.
 */
bool Lexicon::isDAWGFile(std::istream& input) const {
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error(std::string("Lexicon::addWordsFromFile: Couldn't read input"));
    }
    input.read(firstFour, 4);
    bool result = strncmp(firstFour, expected, 4) == 0;
    return result;
}

/*
 * Returns true if the given file (probably) represents a
 * binary DAWG lexicon data file.
 */
bool Lexicon::isDAWGFile(const std::string& filename) const {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error(std::string("Lexicon::addWordsFromFile: Couldn't open lexicon file ") + filename);
    }
    bool result = isDAWGFile(input);
    input.close();
    return result;
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(std::istream& input) {
    DawgLexicon ldawg(input);
    for (std::string word : ldawg) {
        add(word);
    }
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(const std::string& filename) {
    DawgLexicon ldawg(filename);
    for (std::string word : ldawg) {
        add(word);
    }
}

Lexicon& Lexicon::operator =(const Lexicon& src) {
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
    std::string element;
    return stanfordcpplib::collections::readCollection(is, lex, element, /* descriptor */ "Lexicon::operator >>");
}

/*
 * Hash function for lexicons.
 */
int hashCode(const Lexicon& lex) {
    return stanfordcpplib::collections::hashCodeCollection(lex);
}

static bool scrub(std::string& str) {
    size_t nChars = str.length();
    size_t outIndex = 0;
    for (size_t i = 0; i < nChars; i++) {
        std::string::value_type ch = tolower(str[i]);
        if (ch < 'a' || ch > 'z') {
            return false;   // illegal string
        } else {
            str[outIndex] = ch;
            outIndex++;
        }
    }
    if (outIndex != nChars) {
        str.erase(outIndex, nChars - outIndex);
    }
    return true;
}
