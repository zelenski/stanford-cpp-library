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
#include "compare.h"
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

DawgLexicon::DawgLexicon() {
    edges = start = NULL;
    numEdges = numDawgWords = 0;
}

DawgLexicon::DawgLexicon(std::istream& input) {
    edges = start = NULL;
    numEdges = numDawgWords = 0;
    addWordsFromFile(input);
}

DawgLexicon::DawgLexicon(const std::string& filename) {
    edges = start = NULL;
    numEdges = numDawgWords = 0;
    addWordsFromFile(filename);
}

DawgLexicon::DawgLexicon(const DawgLexicon& src) {
    deepCopy(src);
}

DawgLexicon::~DawgLexicon() {
    if (edges) delete[] edges;
}

void DawgLexicon::add(const std::string& word) {
    std::string copy = word;
    toLowerCaseInPlace(copy);
    if (!contains(copy)) {
        otherWords.add(copy);
    }
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
    edges = start = NULL;
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

bool DawgLexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) return true;
    std::string copy = prefix;
    toLowerCaseInPlace(copy);
    if (traceToLastEdge(copy)) return true;
    for (std::string word : otherWords) {
        if (startsWith(word, copy)) return true;
        if (copy < word) return false;
    }
    return false;
}

bool DawgLexicon::equals(const DawgLexicon& lex2) const {
    // optimization: if literally same lexicon, stop
    if (this == &lex2) {
        return true;
    }
    if (size() != lex2.size()) {
        return false;
    }
    return compare::compare(*this, lex2) == 0;
}

bool DawgLexicon::isEmpty() const {
    return size() == 0;
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
    return compare::compare(*this, lex2) < 0;
}

bool DawgLexicon::operator <=(const DawgLexicon& lex2) const {
    return compare::compare(*this, lex2) <= 0;
}

bool DawgLexicon::operator >(const DawgLexicon& lex2) const {
    return compare::compare(*this, lex2) > 0;
}

bool DawgLexicon::operator >=(const DawgLexicon& lex2) const {
    return compare::compare(*this, lex2) >= 0;
}

/*
 * Private methods
 */

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
    if (src.edges == NULL) {
        edges = NULL;
        start = NULL;
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
 * matches the given char.  Returns NULL if we get to
 * last child without finding a match (thus no such
 * child edge exists).
 */
DawgLexicon::Edge* DawgLexicon::findEdgeForChar(Edge* children, char ch) const {
    Edge* curEdge = children;
    while (true) {
        if (curEdge->letter == charToOrd(ch)) {
            return curEdge;
        }
        if (curEdge->lastEdge) return NULL;
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
 * If a path exists, return last edge; otherwise return NULL.
 */

DawgLexicon::Edge* DawgLexicon::traceToLastEdge(const std::string& s) const {
    if (!start) {
        return NULL;
    }
    Edge* curEdge = findEdgeForChar(start, s[0]);
    int len = (int) s.length();
    for (int i = 1; i < len; i++) {
        if (!curEdge || !curEdge->children) {
            return NULL;
        }
        curEdge = findEdgeForChar(&edges[curEdge->children], s[i]);
    }
    return curEdge;
}

DawgLexicon& DawgLexicon::operator =(const DawgLexicon& src) {
    if (this != &src) {
        if (edges != NULL) {
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
        while (ep != NULL && ep->lastEdge) {
            if (stack.isEmpty()) {
                edgePtr = NULL;
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
    if (edgePtr == NULL) {
        edgePtr = lp->start;
    } else {
        advanceToNextEdge();
    }
    while (edgePtr != NULL && !edgePtr->accept) {
        advanceToNextEdge();
    }
}

std::ostream& operator <<(std::ostream& out, const DawgLexicon& lex) {
    out << "{";
    bool first = true;
    for (std::string word : lex) {
        if (first) {
            first = false;
        } else {
            out << ", ";
        }
        writeGenericValue(out, word, true);
    }
    out << "}";
    return out;
}

int hashCode(const DawgLexicon& lex) {
    int code = hashSeed();
    for (std::string n : lex) {
        code = hashMultiplier() * code + hashCode(n);
    }
    return int(code & hashMask());
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
