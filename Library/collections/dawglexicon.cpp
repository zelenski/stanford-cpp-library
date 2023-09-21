/*
 * File: dawglexicon.cpp
 * ---------------------
 * A lexicon is a word list. This lexicon implementation is
 * backed by a DAWG (directed acyclic word graph) and loads from a
 * binary file format that is super compact and nearly
 * instantaneous to read. Neat!
 *
 * The DAWG idea comes from an article by Appel & Jacobson, CACM May 1988.
 * This lexicon implementation only has the code to load/search the DAWG.
 * The DAWG builder code is quite a bit more intricate, ask me (Julie Z)
 * if you want to see it or are curious to know more!
 *
 * The DAWG internal structure is an array of edges. Each edge is represented by
 * one 32-bit valued, apportioned as a unsigned bit field:
 *  - letterOrd: 5 bits indicate the character on this transition
 *               (expressed as integer from 1 to 26)
 *  - isWord: 1 this bit indicates whether path to this edge forms word
 *  - lastChild: 1 this bit marks the last edge in a sequence of childeren
 *  - children: 24 remaining bits are index within edge array where children
 *                of this edge start
 *
 * Edge data is stored on disk in binary format. It uses byteswapto correct when
 * endianness of file does not match endianness of system.
 */

#include "dawglexicon.h"
#include <fstream>
#include "splversion.h"
#include "filelib.h"
#include <cstring>

DawgLexicon::DawgLexicon() :
        _edges(nullptr),
        _startIndex(0),
        _edgeCount(0),
        _wordCount(0) {
    readBinaryFile(getLibraryPathForResource("EnglishWords.dat"));
}

DawgLexicon::DawgLexicon(const std::string& filename) :
        _edges(nullptr),
        _startIndex(0),
        _edgeCount(0),
        _wordCount(0) {
    if (!filename.empty()) readBinaryFile(filename);
}

DawgLexicon::DawgLexicon(const DawgLexicon& src) :
        _edges(nullptr),
        _startIndex(0),
        _edgeCount(0),
        _wordCount(0) {
    deepCopy(src);
}

DawgLexicon::~DawgLexicon() {
    if (_edges) {
        delete[] _edges;
    }
}

void DawgLexicon::deepCopy(const DawgLexicon& src) {
    if (!src._edges) {
        _edges = nullptr;
    } else {
        _edgeCount = src._edgeCount;
        _edges = new Edge[src._edgeCount];
        memcpy(_edges, src._edges, sizeof(Edge)*src._edgeCount);
    }
    _startIndex = src._startIndex;
    _wordCount = src._wordCount;
}

DawgLexicon& DawgLexicon::operator =(const DawgLexicon& src) {
    if (this != &src) {
        if (_edges) {
            delete[] _edges;
        }
        deepCopy(src);
    }
    return *this;
}

int DawgLexicon::size() const {
    return _wordCount;
}

void DawgLexicon::tallyWordCount(Edge* cur) {
    if (cur != nullptr) {
        while (true) { // iterate over sequence, recursively explore children
            if (cur->isWord) _wordCount++;
            tallyWordCount(edgeForIndex(cur->children));
            if (cur->lastChild) break;
            cur++;
        }
    }
}

bool DawgLexicon::contains(const std::string& word) const {
    Edge* end = traceToEnd(word);
    return end != nullptr && end->isWord;
}

bool DawgLexicon::containsPrefix(const std::string& prefix) const {
    return prefix.empty() || traceToEnd(prefix) != nullptr;
}

DawgLexicon::Edge* DawgLexicon::edgeForIndex(int index) const {
    if (index == 0 || _edges == nullptr) {
     return nullptr;
    }
    if (index < 0 || index >= _edgeCount) {
        error("DawgLexicon attempt to access edge at index that is out of range!");
    }
    return &_edges[index];
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
    char ordToMatch = tolower(charToOrd(ch));
    Edge* curEdge = children;
    while (true) {
        if (!curEdge) {
            return nullptr;
        }
        if (curEdge->letterOrd == ordToMatch) {
            return curEdge;
        }
        if (curEdge->lastChild) {
            return nullptr;
        }
        curEdge++;
    }
}


/*
 * Implementation notes: traceToEnd
 * --------------------------------
 * Given a string, trace out path through the DAWG edge-by-edge.
 * If path exists, return last edge; otherwise return nullptr.
 */
DawgLexicon::Edge* DawgLexicon::traceToEnd(const std::string& s) const {
    Edge *children = edgeForIndex(_startIndex), *cur = nullptr;
    for (char ch : s) {
        cur = findEdgeForChar(children, ch);
        if (!cur) return nullptr;
        children = edgeForIndex(cur->children);
    }
    return cur;
}

enum Endian { LittleEndian = 0, BigEndian = 1 };

/*
 * Detemrine endianness of this system. (don't rely on
 * compiler flags, just confirm for real!)
 */
static Endian determineSystemEndian() {
    uint32_t val = 0x12345678;
    uint8_t low = *(char *)&val;
    if (low == 0x78) return LittleEndian;
    if (low == 0x12) return BigEndian;
    error("cannot determine endianness");
}

/*
 * Byte swap a 4-byte val from big to little endian (or vice versa)
 */
static uint32_t byteswap(uint32_t arg) {
    uint32_t result = ((arg & 0xff000000) >> 24) |
                      ((arg & 0x00ff0000) >>  8) |
                      ((arg & 0x0000ff00) <<  8) |
                      ((arg & 0x000000ff) << 24);
    return result;
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */
void DawgLexicon::readBinaryFile(const std::string& filename) {
    std::string partial = std::string(__FUNCTION__) + " error reading from " + filename;

    std::ifstream input(filename, std::ios::in | std::ios::binary);
    if (input.fail()) {
         error(std::string(__FUNCTION__) + ": unable to open file " + filename);
    }
    int tagLen = 6;
    char tag[tagLen];
    input.read(tag, tagLen);
    bool expected = strncmp(tag,  "DAWGLE", tagLen) == 0 || strncmp(tag, "DAWGBE", tagLen) == 0;
    if (input.fail() || !expected) {
        error(partial + " (invalid tag)");
    }
    int fileEndian = strncmp("LE", &tag[4], 2) == 0 ? LittleEndian : BigEndian;
    int systemEndian = determineSystemEndian();
    bool needSwap = (systemEndian != fileEndian);

    char bracket[2];
    input >> bracket[0] >> _edgeCount >> bracket[1];
    if (input.fail() ||  bracket[0] != '[' || bracket[1] != ']') {
        error(partial + " (invalid edge count format)");
    }
    input >> bracket[0] >> _startIndex >> bracket[1];
    if (input.fail() ||  bracket[0] != '[' || bracket[1] != ']') {
        error(partial + " (invalid start index format)");
    }

    int totalFileSize = fileSize(filename);
    int numBytesRemaining = totalFileSize - (int)input.tellg();
    if (_edgeCount*sizeof(Edge) != numBytesRemaining) {
        error(partial + " (invalid file size)");
    }
    if ( _startIndex >= _edgeCount) {
        error(partial + " (invalid start index)");
    }

    _edges = new Edge[_edgeCount];
    input.read((char*) _edges, _edgeCount*sizeof(Edge));
    int numRead = input.gcount();
    if ((input.fail() && !input.eof()) || (numRead != _edgeCount*sizeof(Edge))) {
        error(partial + " (invalid edges)");
    }

    uint32_t* cur = reinterpret_cast<uint32_t*>(_edges);
    for (int i = 0; i < _edgeCount; i++) {
        if (needSwap) cur[i] = byteswap(cur[i]);
        if (_edges[i].letterOrd > 26) error(partial + " (invalid edge letter)");
        if (_edges[i].children >= _edgeCount) error(partial + " (invalid edge children)");
    }
    _wordCount = 0;
    tallyWordCount(edgeForIndex(_startIndex));
}


/* * * Iterator support * * */

void DawgLexicon::iterator::advanceToNextEdge() {
    Edge *ep = _curEdge;
    if (ep->children == 0) {
        while (ep && ep->lastChild) {
            if (_pathToCurEdge.isEmpty()) {
                _curEdge = nullptr;
                return;
            } else {
                ep = _pathToCurEdge.pop();
                _curPrefix.resize(_curPrefix.length() - 1);
            }
        }
        _curEdge = ep + 1;
    } else {
        _pathToCurEdge.push(ep);
        _curPrefix.push_back(_lex->ordToChar(ep->letterOrd));
        _curEdge = _lex->edgeForIndex(ep->children);
    }
}

void DawgLexicon::iterator::advanceToNextWordInDawg() {
    if (!_curEdge) {
        _curEdge = _lex->edgeForIndex(_lex->_startIndex);
    } else {
        advanceToNextEdge();
    }
    while (_curEdge && !_curEdge->isWord) {
        advanceToNextEdge();
    }
}
