/*
 * File: dawglexicon.cpp
 * ---------------------
 * A lexicon is a word list. This lexicon is backed by two separate data
 * structures for storing the words in the list:
 *
 * 1) a DAWG (directed acyclic word graph)
 * 2) a Set<string> of other words.
 *
 * Typically the DAWG is used for a large list read from a file in binary
 * format.  The STL set is for words added piecemeal at runtime.
 *
 * The DAWG idea comes from an article by Appel & Jacobson, CACM May 1988.
 * This lexicon implementation only has the code to load/search the DAWG.
 * The DAWG builder code is quite a bit more intricate, see Julie Zelenski
 * if you need it.
 * 
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2016/08/04
 * - added operator >>
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - removed 'using namespace' statement
 * - added equals method, ==, != operators
 * - fixed inclusion of foreach macro to avoid errors
 * - BUGFIX: operator << now shows "" marks around words to match Lexicon
 */

#include "dawglexicon.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include "collections.h"
#include "error.h"
#include "hashcode.h"
#include "strlib.h"

static uint32_t my_ntohl(uint32_t arg);

/*
 * The DAWG is stored as an array of edges. Each edge is represented by
 * one 32-bit struct.  The 5 "letter" bits indicate the character on this
 * transition (expressed as integer from 1 to 26), the  "accept" bit indicates
 * if you accept after appending that char (current path forms word), and the
 * "lastEdge" bit marks this as the last edge in a sequence of childeren.
 * The bulk of the bits (24) are used for the index within the edge array for
 * the children of this node. The children are laid out contiguously in
 * alphabetical order.  Since we read edges as binary bits from a file in
 * a big-endian format, we have to swap the struct order for little-endian
 * machines.
 */

DawgLexicon::DawgLexicon() :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    // empty
}

DawgLexicon::DawgLexicon(std::istream& input) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addWordsFromFile(input);
}

DawgLexicon::DawgLexicon(const std::string& filename) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addWordsFromFile(filename);
}

DawgLexicon::DawgLexicon(const DawgLexicon& src) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    deepCopy(src);
}

DawgLexicon::DawgLexicon(std::initializer_list<std::string> list) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addAll(list);
}

DawgLexicon::~DawgLexicon() {
    if (edges) {
        delete[] edges;
    }
}

void DawgLexicon::add(const std::string& word) {
    std::string copy = word;
    toLowerCaseInPlace(copy);
    if (!contains(copy)) {
        otherWords.add(copy);
    }
}

DawgLexicon& DawgLexicon::addAll(const DawgLexicon& lex) {
    for (const std::string& word : lex) {
        add(word);
    }
    return *this;
}

DawgLexicon& DawgLexicon::addAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        add(word);
    }
    return *this;
}

/*
 * Check for DAWG in first 4 to identify as special binary format,
 * otherwise assume ASCII, one word per line
 */
void DawgLexicon::addWordsFromFile(std::istream& input) {
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't read input");
    }
    input.read(firstFour, 4);
    if (strncmp(firstFour, expected, 4) == 0) {
        if (otherWords.size() != 0) {
            error("DawgLexicon::addWordsFromFile: Binary files require an empty lexicon");
        }
        readBinaryFile(input);
    } else {
        // plain text file
        input.seekg(0);
        std::string line;
        while (getline(input, line)) {
            add(line);
        }
    }
}

/*
 * Check for DAWG in first 4 to identify as special binary format,
 * otherwise assume ASCII, one word per line
 */
void DawgLexicon::addWordsFromFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    addWordsFromFile(input);
    input.close();
}

void DawgLexicon::clear() {
    if (edges) {
        delete[] edges;
    }
    edges = start = nullptr;
    numEdges = numDawgWords = 0;
    otherWords.clear();
}

bool DawgLexicon::contains(const std::string& word) const {
    std::string copy = word;
    toLowerCaseInPlace(copy);
    Edge* lastEdge = traceToLastEdge(copy);
    if (lastEdge && lastEdge->accept) {
        return true;
    }
    return otherWords.contains(copy);
}

bool DawgLexicon::containsAll(const DawgLexicon& lex2) const {
    for (const std::string& word : lex2) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool DawgLexicon::containsAll(std::initializer_list<std::string> list) const {
    for (const std::string& word : list) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool DawgLexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }
    std::string copy = prefix;
    toLowerCaseInPlace(copy);
    if (traceToLastEdge(copy)) {
        return true;
    }
    for (std::string word : otherWords) {
        if (startsWith(word, copy)) {
            return true;
        }
        if (copy < word) {
            return false;
        }
    }
    return false;
}

bool DawgLexicon::equals(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::equals(*this, lex2);
}

void DawgLexicon::insert(const std::string& word) {
    add(word);
}

bool DawgLexicon::isEmpty() const {
    return size() == 0;
}

bool DawgLexicon::isSubsetOf(const DawgLexicon& lex2) const {
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

bool DawgLexicon::isSubsetOf(std::initializer_list<std::string> list) const {
    DawgLexicon lex2(list);
    return isSubsetOf(lex2);
}

bool DawgLexicon::isSupersetOf(const DawgLexicon& lex2) const {
    return containsAll(lex2);
}

bool DawgLexicon::isSupersetOf(std::initializer_list<std::string> list) const {
    return containsAll(list);
}

void DawgLexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : *this) {
        fn(word);
    }
}

void DawgLexicon::mapAll(void (*fn)(const std::string &)) const {
    for (std::string word : *this) {
        fn(word);
    }
}

int DawgLexicon::size() const {
    return numDawgWords + otherWords.size();
}

std::string DawgLexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::set<std::string> DawgLexicon::toStlSet() const {
    std::set<std::string> result;
    for (std::string word : *this) {
        result.insert(word);
    }
    return result;
}

/*
 * Operators
 */
bool DawgLexicon::operator ==(const DawgLexicon& lex2) const {
    return equals(lex2);
}

bool DawgLexicon::operator !=(const DawgLexicon& lex2) const {
    return !equals(lex2);
}

bool DawgLexicon::operator <(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) < 0;
}

bool DawgLexicon::operator <=(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) <= 0;
}

bool DawgLexicon::operator >(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) > 0;
}

bool DawgLexicon::operator >=(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) >= 0;
}

DawgLexicon DawgLexicon::operator +(const DawgLexicon& lex2) const {
    DawgLexicon lex = *this;
    lex.addAll(lex2);
    return lex;
}

DawgLexicon DawgLexicon::operator +(std::initializer_list<std::string> list) const {
    DawgLexicon lex = *this;
    lex.addAll(list);
    return lex;
}

DawgLexicon DawgLexicon::operator +(const std::string& word) const {
    DawgLexicon lex = *this;
    lex.add(word);
    return lex;
}

DawgLexicon& DawgLexicon::operator +=(const DawgLexicon& lex2) {
    return addAll(lex2);
}

DawgLexicon& DawgLexicon::operator +=(std::initializer_list<std::string> list) {
    return addAll(list);
}

DawgLexicon& DawgLexicon::operator +=(const std::string& word) {
    add(word);
    return *this;
}

/*
 * Private methods and helpers
 */

DawgLexicon& DawgLexicon::operator ,(const std::string& word) {
    add(word);
    return *this;
}

int DawgLexicon::countDawgWords(Edge* ep) const {
    int count = 0;
    while (true) {
        if (ep->accept) count++;
        if (ep->children != 0) {
            count += countDawgWords(&edges[ep->children]);
        }
        if (ep->lastEdge) break;
        ep++;
    }
    return count;
}

void DawgLexicon::deepCopy(const DawgLexicon& src) {
    if (!src.edges) {
        edges = nullptr;
        start = nullptr;
    } else {
        numEdges = src.numEdges;
        edges = new Edge[src.numEdges];
        memcpy(edges, src.edges, sizeof(Edge)*src.numEdges);
        start = edges + (src.start - src.edges);
    }
    numDawgWords = src.numDawgWords;
    otherWords = src.otherWords;
}

/*
 * Implementation notes: findEdgeForChar
 * -------------------------------------
 * Iterate over sequence of children to find one that
 * matches the given char.  Returns nullptr if we get to
 * last child without finding a match (thus no such
 * child edge exists).
 */
DawgLexicon::Edge* DawgLexicon::findEdgeForChar(Edge* children, char ch) const {
    Edge* curEdge = children;
    while (true) {
        if (curEdge->letter == charToOrd(ch)) {
            return curEdge;
        }
        if (curEdge->lastEdge) {
            return nullptr;
        }
        curEdge++;
    }
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */
void DawgLexicon::readBinaryFile(std::istream& input) {
    long startIndex, numBytes;
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't read input");
    }
    input.read(firstFour, 4);
    input.get();
    input >> startIndex;
    input.get();
    input >> numBytes;
    input.get();
    if (input.fail() || strncmp(firstFour, expected, 4) != 0
            || startIndex < 0 || numBytes < 0) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file");
    }
    numEdges = numBytes / sizeof(Edge);
    edges = new Edge[numEdges];
    start = &edges[startIndex];
    input.read((char*) edges, numBytes);
    if (input.fail() && !input.eof()) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file");
    }

#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
    uint32_t *cur = (uint32_t *) edges;
    for (int i = 0; i < numEdges; i++, cur++) {
        *cur = my_ntohl(*cur);
    }
#endif

    numDawgWords = countDawgWords(start);
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */
void DawgLexicon::readBinaryFile(const std::string& filename) {
#ifdef _foreachpatch_h
    std::ifstream input(filename.c_str(), __IOS_IN__ | __IOS_BINARY__);
#else
    std::ifstream input(filename.c_str(), std::ios::in | std::ios::binary);
#endif // _foreachpatch_h
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    readBinaryFile(input);
    input.close();
}

/*
 * Implementation notes: traceToLastEdge
 * -------------------------------------
 * Given a string, trace out path through the DAWG edge-by-edge.
 * If a path exists, return last edge; otherwise return nullptr.
 */

DawgLexicon::Edge* DawgLexicon::traceToLastEdge(const std::string& s) const {
    if (!start) {
        return nullptr;
    }
    Edge* curEdge = findEdgeForChar(start, s[0]);
    int len = (int) s.length();
    for (int i = 1; i < len; i++) {
        if (!curEdge || !curEdge->children) {
            return nullptr;
        }
        curEdge = findEdgeForChar(&edges[curEdge->children], s[i]);
    }
    return curEdge;
}

DawgLexicon& DawgLexicon::operator =(const DawgLexicon& src) {
    if (this != &src) {
        if (edges) {
            delete[] edges;
        }
        deepCopy(src);
    }
    return *this;
}

void DawgLexicon::iterator::advanceToNextWordInSet() {
    if (setIterator == setEnd) {
        currentSetWord = "";
    } else {
        currentSetWord = *setIterator;
        ++setIterator;
    }
}

void DawgLexicon::iterator::advanceToNextEdge() {
    Edge *ep = edgePtr;
    if (ep->children == 0) {
        while (ep && ep->lastEdge) {
            if (stack.isEmpty()) {
                edgePtr = nullptr;
                return;
            } else {
                ep = stack.pop();
                currentDawgPrefix.resize(currentDawgPrefix.length() - 1);
            }
        }
        edgePtr = ep + 1;
    } else {
        stack.push(ep);
        currentDawgPrefix.push_back(lp->ordToChar(ep->letter));
        edgePtr = &lp->edges[ep->children];
    }
}

void DawgLexicon::iterator::advanceToNextWordInDawg() {
    if (!edgePtr) {
        edgePtr = lp->start;
    } else {
        advanceToNextEdge();
    }
    while (edgePtr && !edgePtr->accept) {
        advanceToNextEdge();
    }
}

std::ostream& operator <<(std::ostream& os, const DawgLexicon& lex) {
    return stanfordcpplib::collections::writeCollection(os, lex);
}

std::istream& operator >>(std::istream& is, DawgLexicon& lex) {
    std::string element;
    return stanfordcpplib::collections::readCollection(is, lex, element, /* descriptor */ "DawgLexicon::operator >>");
}

int hashCode(const DawgLexicon& lex) {
    return stanfordcpplib::collections::hashCodeCollection(lex);
}

/*
 * Swaps a 4-byte long from big to little endian byte order
 */
static uint32_t my_ntohl(uint32_t arg) {
    uint32_t result = ((arg & 0xff000000) >> 24) |
            ((arg & 0x00ff0000) >> 8) |
            ((arg & 0x0000ff00) << 8) |
            ((arg & 0x000000ff) << 24);
    return result;
}
