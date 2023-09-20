/*
 * File: dawglexicon.h
 * -------------------
 * This file exports the <code>DawgLexicon</code> class, which is a
 * compact structure for storing a list of words.
 */

#pragma once

#include <string>
#include "stack.h"

/**
 * This class is used to represent a <b><i>lexicon,</i></b> or word list.
 * The main difference between a lexicon and a dictionary is that
 * a lexicon does not provide any mechanism for storing definitions;
 * the lexicon contains only words, with no associated information.
 * It is therefore similar to a set of strings, but with a more
 * space-efficient internal representation.  The <code>DawgLexicon</code>
 * class supports efficient lookup operations for words and prefixes.
 */

class DawgLexicon {
public:
    /**
     * Initializes a new lexicon, populated with word list from
     * the EnglishWords data file included with the CS106 library.
     */
    DawgLexicon();

    /**
     * Initializes a new lexicon, reading in the contents of the lexicon from
     * the named data file. The data file format is a space-efficient
     * precompiled binary representation.
     */
    DawgLexicon(const std::string& filename);

    /**
     * The destructor deallocates any storage associated with the lexicon.
     */
    virtual ~DawgLexicon();

    /**
     * Returns <code>true</code> if <code>word</code> is contained in the
     * lexicon.  In the <code>DawgLexicon</code> class, the case of letters is
     * ignored, so "Zoo" is the same as "ZOO" or "zoo".
     */
    bool contains(const std::string& word) const;

    /**
     * Returns true if any words in the lexicon begin with <code>prefix</code>.
     * Like <code>containsWord</code>, this method ignores the case of letters
     * so that "MO" is a prefix of "monkey" or "Monday".
     */
    bool containsPrefix(const std::string& prefix) const;

    /**
     * Returns the number of words contained in the lexicon.
     */
    int size() const;

    /*
     * Additional DawgLexicon operations
     * ---------------------------------
     * In addition to the methods listed in this interface, the DawgLexicon
     * class supports the following operations:
     *
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * All iteration is guaranteed to proceed in alphabetical order.  All
     * words in the lexicon are stored in lowercase.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    struct Edge {
        uint32_t letterOrd:5;
        uint32_t lastChild:1;
        uint32_t isWord:1;
        uint32_t unused:1;
        uint32_t children:24;
    };
    Edge *_edges;
    int _edgeCount, _wordCount, _startIndex;

public:
    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a
     * deep copy, making it possible to pass/return lexicons by value
     * and assign from one lexicon to another.  The entire contents of
     * the lexicon, including all words, are copied.  Making copies is
     * generally avoided because of the expense and thus, lexicons are
     * typically passed by reference.  When a copy is needed, these
     * operations are supported.
     */
    DawgLexicon(const DawgLexicon& src);
    DawgLexicon& operator =(const DawgLexicon& src);

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::string;
        using difference_type = int;
        using pointer = std::string*;
        using reference = std::string&;

    private:
        const DawgLexicon* _lex;
        int _curIndex;
        std::string _curPrefix;
        std::string _tmpWord;
        Edge* _curEdge;
        Stack<Edge*> _pathToCurEdge;

        void advanceToNextWordInDawg();
        void advanceToNextEdge();

    public:
        iterator() : _lex(nullptr), _curIndex(0), _curEdge(nullptr) {
            /* empty */
        }

        iterator(const DawgLexicon* theLP, bool endFlag) {
            this->_lex = theLP;
            if (endFlag) {
                _curIndex = _lex->size();
            } else {
                _curIndex = 0;
                _curEdge = nullptr;
                _curPrefix = "";
                advanceToNextWordInDawg();
            }
        }

        iterator(const iterator& it) {
            _lex = it._lex;
            _curIndex = it._curIndex;
            _curPrefix = it._curPrefix;
            _curEdge = it._curEdge;
            _pathToCurEdge = it._pathToCurEdge;
        }

        iterator& operator ++() {
            advanceToNextWordInDawg();
            _curIndex++;
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return _lex == rhs._lex && _curIndex == rhs._curIndex;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        std::string operator *() {
            return _curPrefix + _lex->ordToChar(_curEdge->letterOrd);
        }

        std::string* operator ->() {
            _tmpWord = _curPrefix + _lex->ordToChar(_curEdge->letterOrd);
            return &_tmpWord;
        }
    };

    /**
     * Returns an iterator positioned at the first word in the lexicon.
     */
    iterator begin() const {
        return iterator(this, /* end */ false);
    }

    /**
     * Returns an iterator positioned at the last word in the lexicon.
     */
    iterator end() const {
        return iterator(this, /* end */ true);
    }

private:
    Edge *edgeForIndex(int index) const;
    Edge* findEdgeForChar(Edge*, char) const;
    Edge* traceToEnd(const std::string&) const;
    void readBinaryFile(const std::string&);
    void deepCopy(const DawgLexicon&);
    void tallyWordCount(Edge*);

    unsigned int charToOrd(char ch) const {
        return ((unsigned int)(tolower(ch) - 'a' + 1));
    }

    char ordToChar(unsigned int ord) const {
        return ((char)(ord - 1 + 'a'));
    }
};


