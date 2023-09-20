/*
 * File: trielexicon.cpp
 * ---------------------
 * A TrieLexicon is a word list. This TrieLexicon is backed by a data
 * structure called a prefix tree or trie ("try").
 *
 * The DAWG implementation of Lexicon is available as dawglexicon.h/cpp.
 *
 * @version 2018/03/10
 * - added method front
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

#include "trielexicon.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "collections.h"
#include "error.h"
#include "strlib.h"

static bool scrub(std::string& str);

TrieLexicon::TrieLexicon() :
        _root(nullptr),
        _size(0),
        _removeFlag(false) {
    // empty
}

TrieLexicon::TrieLexicon(std::istream& input) :
        _root(nullptr),
        _size(0),
        _removeFlag(false) {
    addWordsFromFile(input);
}

TrieLexicon::TrieLexicon(const std::string& filename) :
        _root(nullptr),
        _size(0),
        _removeFlag(false) {
    addWordsFromFile(filename);
}

TrieLexicon::TrieLexicon(std::initializer_list<std::string> list) :
        _root(nullptr),
        _size(0),
        _removeFlag(false) {
    addAll(list);
}

TrieLexicon::TrieLexicon(const TrieLexicon& src) :
        _root(nullptr),
        _size(0),
        _removeFlag(false) {
    deepCopy(src);
}

TrieLexicon::~TrieLexicon() {
    clear();
}

bool TrieLexicon::add(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return addHelper(_root, scrubbed, /* originalWord */ scrubbed);
}

TrieLexicon& TrieLexicon::addAll(const TrieLexicon& lex) {
    for (const std::string& word : lex) {
        add(word);
    }
    return *this;
}

TrieLexicon& TrieLexicon::addAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        add(word);
    }
    return *this;
}

void TrieLexicon::addWordsFromFile(std::istream& input) {
    std::string line;
    while (getline(input, line)) {
        add(trim(line));
    }
}

void TrieLexicon::addWordsFromFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error("TrieLexicon::addWordsFromFile: Couldn't read from input file " + filename);
    }
    addWordsFromFile(input);
    input.close();
}

std::string TrieLexicon::back() const {
    if (isEmpty()) {
        error("TrieLexicon::back: lexicon is empty");
    }
    return _allWords.last();
}

void TrieLexicon::clear() {
    _size = 0;
    _allWords.clear();
    deleteTree(_root);
    _root = nullptr;
}

bool TrieLexicon::contains(const std::string& word) const {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(_root, scrubbed, /* isPrefix */ false);
}

bool TrieLexicon::containsAll(const TrieLexicon& lex2) const {
    for (const std::string& word : lex2) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool TrieLexicon::containsAll(std::initializer_list<std::string> list) const {
    for (const std::string& word : list) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool TrieLexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(_root, scrubbed, /* isPrefix */ true);
}

bool TrieLexicon::equals(const TrieLexicon& lex2) const {
    return stanfordcpplib::collections::equals(*this, lex2);
}

std::string TrieLexicon::first() const {
    if (isEmpty()) {
        error("TrieLexicon::first: lexicon is empty");
    }
    return _allWords.first();
}

std::string TrieLexicon::front() const {
    if (isEmpty()) {
        error("TrieLexicon::front: lexicon is empty");
    }
    return _allWords.first();
}

void TrieLexicon::insert(const std::string& word) {
    add(word);
}

bool TrieLexicon::isEmpty() const {
    return size() == 0;
}

bool TrieLexicon::isSubsetOf(const TrieLexicon& lex2) const {
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

bool TrieLexicon::isSubsetOf(std::initializer_list<std::string> list) const {
    TrieLexicon lex2(list);
    return isSubsetOf(lex2);
}

bool TrieLexicon::isSupersetOf(const TrieLexicon& lex2) const {
    return containsAll(lex2);
}

bool TrieLexicon::isSupersetOf(std::initializer_list<std::string> list) const {
    return containsAll(list);
}

void TrieLexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : _allWords) {
        fn(word);
    }
}

void TrieLexicon::mapAll(void (*fn)(const std::string&)) const {
    for (std::string word : _allWords) {
        fn(word);
    }
}

bool TrieLexicon::remove(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return removeHelper(_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ false);
}

TrieLexicon& TrieLexicon::removeAll(const TrieLexicon& lex2) {
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

TrieLexicon& TrieLexicon::removeAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        remove(word);
    }
    return *this;
}

bool TrieLexicon::removePrefix(const std::string& prefix) {
    if (prefix.empty()) {
        bool result = !isEmpty();
        clear();
        return result;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }

    return removeHelper(_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ true);
}

TrieLexicon& TrieLexicon::retainAll(const TrieLexicon& lex2) {
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

TrieLexicon& TrieLexicon::retainAll(std::initializer_list<std::string> list) {
    TrieLexicon lex2(list);
    return retainAll(lex2);
}

int TrieLexicon::size() const {
    return _size;
}

std::string TrieLexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

/*
 * Operators
 */
bool TrieLexicon::operator ==(const TrieLexicon& lex2) const {
    return equals(lex2);
}

bool TrieLexicon::operator !=(const TrieLexicon& lex2) const {
    return !equals(lex2);
}

bool TrieLexicon::operator <(const TrieLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) < 0;
}

bool TrieLexicon::operator <=(const TrieLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) <= 0;
}

bool TrieLexicon::operator >(const TrieLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) > 0;
}

bool TrieLexicon::operator >=(const TrieLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) >= 0;
}

TrieLexicon TrieLexicon::operator +(const TrieLexicon& lex2) const {
    TrieLexicon lex = *this;
    lex.addAll(lex2);
    return lex;
}

TrieLexicon TrieLexicon::operator +(std::initializer_list<std::string> list) const {
    TrieLexicon lex = *this;
    lex.addAll(list);
    return lex;
}

TrieLexicon TrieLexicon::operator +(const std::string& word) const {
    TrieLexicon lex = *this;
    lex.add(word);
    return lex;
}

TrieLexicon TrieLexicon::operator *(const TrieLexicon& lex2) const {
    TrieLexicon lex = *this;
    return lex.retainAll(lex2);
}

TrieLexicon TrieLexicon::operator *(std::initializer_list<std::string> list) const {
    TrieLexicon lex = *this;
    return lex.retainAll(list);
}

TrieLexicon TrieLexicon::operator -(const TrieLexicon& lex2) const {
    TrieLexicon lex = *this;
    return lex.removeAll(lex2);
}

TrieLexicon TrieLexicon::operator -(std::initializer_list<std::string> list) const {
    TrieLexicon lex = *this;
    return lex.removeAll(list);
}

TrieLexicon TrieLexicon::operator -(const std::string& word) const {
    TrieLexicon lex = *this;
    lex.remove(word);
    return lex;
}

TrieLexicon& TrieLexicon::operator +=(const TrieLexicon& lex2) {
    return addAll(lex2);
}

TrieLexicon& TrieLexicon::operator +=(std::initializer_list<std::string> list) {
    return addAll(list);
}

TrieLexicon& TrieLexicon::operator +=(const std::string& word) {
    add(word);
    _removeFlag = false;
    return *this;
}

TrieLexicon& TrieLexicon::operator *=(const TrieLexicon& lex2) {
    return retainAll(lex2);
}

TrieLexicon& TrieLexicon::operator *=(std::initializer_list<std::string> list) {
    return retainAll(list);
}

TrieLexicon& TrieLexicon::operator -=(const TrieLexicon& lex2) {
    return removeAll(lex2);
}

TrieLexicon& TrieLexicon::operator -=(std::initializer_list<std::string> list) {
    return removeAll(list);
}

TrieLexicon& TrieLexicon::operator -=(const std::string& word) {
    remove(word);
    _removeFlag = true;
    return *this;
}

/* private helpers implementation */

TrieLexicon& TrieLexicon::operator ,(const std::string& word) {
    if (_removeFlag) {
        remove(word);
    } else {
        add(word);
    }
    return *this;
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool TrieLexicon::addHelper(TrieNode*& node, const std::string& word, const std::string& originalWord) {
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
            _size++;
            _allWords.add(originalWord);
            return true;
        }
    } else {
        // recursive case: chop off first letter, traverse the rest
        return addHelper(node->child(word[0]), word.substr(1), originalWord);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool TrieLexicon::containsHelper(TrieNode* node, const std::string& word, bool isPrefix) const {
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
bool TrieLexicon::removeHelper(TrieNode*& node, const std::string& word, const std::string& originalWord, bool isPrefix) {
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
            _allWords.remove(originalWord);
            _size--;
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
void TrieLexicon::removeSubtreeHelper(TrieNode*& node, const std::string& originalWord) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            removeSubtreeHelper(node->child(letter), originalWord + letter);
        }
        if (node->isWord()) {
            _allWords.remove(originalWord);
            _size--;
        }
        delete node;
        node = nullptr;
    }
}

void TrieLexicon::deepCopy(const TrieLexicon& src) {
    for (std::string word : src._allWords) {
        add(word);
    }
}

void TrieLexicon::deleteTree(TrieNode* node) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            deleteTree(node->child(letter));
        }
        delete node;
    }
}

TrieLexicon& TrieLexicon::operator =(const TrieLexicon& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

std::ostream& operator <<(std::ostream& out, const TrieLexicon& lex) {
    out << lex._allWords;
    return out;
}

std::istream& operator >>(std::istream& is, TrieLexicon& lex) {
    std::string element;
    return stanfordcpplib::collections::readCollection(is, lex, element, /* descriptor */ "TrieLexicon::operator >>");
}

/*
 * Hash function for lexicons.
 */
int hashCode(const TrieLexicon& lex) {
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
