/*
 * File: dawglexicon.h
 * -------------------
 * This file exports the <code>DawgLexicon</code> class, which is a
 * compact structure for storing a list of words.
 * 
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @version 2018/03/10
 * - added method front
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2017/10/18
 * - fix compiler warnings
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/11
 * - added methods addAll, containsAll,
 *   operators +, +=, *, *= to better match Set/HashSet
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2016/08/04
 * - added operator >>
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - added equals method
 * - added comparison operators ==, !=
 * - added toStlSet method
 * - fixed inclusion of foreach macro to avoid errors
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _dawglexicon_h
#define _dawglexicon_h

#include <initializer_list>
#include <set>
#include <string>

#define INTERNAL_INCLUDE 1
#include "set.h"
#undef INTERNAL_INCLUDE
#define INTERNAL_INCLUDE 1
#include "stack.h"
#undef INTERNAL_INCLUDE

/**
 * This class is used to represent a <b><i>lexicon,</i></b> or word list.
 * The main difference between a lexicon and a dictionary is that
 * a lexicon does not provide any mechanism for storing definitions;
 * the lexicon contains only words, with no associated information.
 * It is therefore similar to a set of strings, but with a more
 * space-efficient internal representation.  The <code>DawgLexicon</code>
 * class supports efficient lookup operations for words and prefixes.
 *
 * Currently the recommended implementation of a lexicon is the
 * <code>Lexicon</code> class, which supports more operations than this one.
 */

#include <cctype>

class DawgLexicon {
public:
    /**
     * Initializes a new empty lexicon.
     */
    DawgLexicon();

    /**
     * Initializes a new lexicon, reading in the contents of the lexicon from
     * the specified data file.
     *
     * The data file must be in one of two formats:
     * (1) a space-efficient precompiled binary format or
     * (2) a text file containing one word per line.
     *
     * The Stanford library distribution includes a binary lexicon file named
     * <code>English.dat</code> containing a list of words in English.  The
     * standard code pattern to initialize that lexicon looks like this:
     *
     *<pre>
     * DawgLexicon english("English.dat");
     *</pre>
     */
    DawgLexicon(std::istream& input);

    /**
     * Initializes a new lexicon, reading in the contents of the lexicon from
     * the specified data file.
     *
     * The data file must be in one of two formats:
     * (1) a space-efficient precompiled binary format (e.g. English.dat) or
     * (2) a text file containing one word per line.
     *
     * @example DawgLexicon english("English.dat");
     */
    DawgLexicon(const std::string& filename);

    /**
     * This constructor uses an initializer list to set up the lexicon.
     * @example DawgLexicon lex {1, 2, 3};
     */
    DawgLexicon(std::initializer_list<std::string> list);

    /**
     * The destructor deallocates any storage associated with the lexicon.
     */
    virtual ~DawgLexicon();
    
    /**
     * Adds the specified word to the lexicon.
     */
    void add(const std::string& word);

    /**
     * Adds all elements of the given other lexicon to this lexicon.
     * Identical in behavior to the += operator.
     */
    DawgLexicon& addAll(const DawgLexicon& lex);

    /**
     * Adds all elements of the given list to this lexicon.
     * Identical in behavior to the += operator.
     */
    DawgLexicon& addAll(std::initializer_list<std::string> list);

    /**
     * Reads the input stream and adds all of its words to the lexicon.
     */
    void addWordsFromFile(std::istream& input);
    
    /**
     * Reads the file and adds all of its words to the lexicon.
     */
    void addWordsFromFile(const std::string& filename);
    
    /**
     * Removes all words from the lexicon.
     */
    void clear();
    
    /**
     * Returns <code>true</code> if <code>word</code> is contained in the
     * lexicon.  In the <code>DawgLexicon</code> class, the case of letters is
     * ignored, so "Zoo" is the same as "ZOO" or "zoo".
     */
    bool contains(const std::string& word) const;

    /**
     * Returns <code>true</code> if every value from the given other lexicon
     * is also found in this lexicon.
     * Equivalent in behavior to isSupersetOf.
     */
    bool containsAll(const DawgLexicon& set2) const;

    /**
     * Returns <code>true</code> if every value from the given list
     * is also found in this lexicon.
     * Equivalent in behavior to isSupersetOf.
     */
    bool containsAll(std::initializer_list<std::string> list) const;

    /**
     * Returns true if any words in the lexicon begin with <code>prefix</code>.
     * Like <code>containsWord</code>, this method ignores the case of letters
     * so that "MO" is a prefix of "monkey" or "Monday".
     */
    bool containsPrefix(const std::string& prefix) const;
    
    /**
     * Compares two lexicons for equality.
     * Returns <code>true</code> if this lexicon contains exactly the same
     * values as the given other lexicon.
     * Identical in behavior to the == operator.
     */
    bool equals(const DawgLexicon& lex2) const;

    /**
     * Returns the first element in alphabetical order from this lexicon (without removing it).
     * This method signals an error if lexicon is empty.
     */
    std::string front() const;

    /**
     * Adds an element to this lexicon, if it was not already there.  This
     * method is exported for compatibility with the STL <code>set</code> class.
     */
    void insert(const std::string& word);

    /**
     * Returns <code>true</code> if the lexicon contains no words.
     */
    bool isEmpty() const;

    /**
     * Returns <code>true</code> if every word of this lexicon is contained in
     * the given other lexicon.
     */
    bool isSubsetOf(const DawgLexicon& lex2) const;

    /**
     * Returns <code>true</code> if every word of this lexicon is contained in
     * the given list.
     */
    bool isSubsetOf(std::initializer_list<std::string> list) const;

    /**
     * Returns <code>true</code> if every word of this lexicon is contained in
     * the given other lexicon.
     * Equivalent in behavior to containsAll.
     */
    bool isSupersetOf(const DawgLexicon& lex2) const;

    /**
     * Returns <code>true</code> if every word of this lexicon is contained in
     * the given list.
     * Equivalent in behavior to containsAll.
     */
    bool isSupersetOf(std::initializer_list<std::string> list) const;

    /**
     * Calls the specified function on each word in the lexicon.
     */
    void mapAll(void (*fn)(std::string)) const;

    /**
     * Calls the specified function on each word in the lexicon.
     */
    void mapAll(void (*fn)(const std::string&)) const;

    /**
     * Calls the specified function on each word in the lexicon.
     */
    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    // implementation note: DawgLexicon does not support removal,
    // so there are no methods remove(), removeAll, retainAll, etc.
    // nor operators -, -=, *=

    /**
     * Returns the number of words contained in the lexicon.
     */
    int size() const;

    /**
     * Converts the lexicon to a printable string representation such as
     * {"a", "b", "c"}.
     * Note that this can be an expensive operation if the lexicon contains
     * a large number of words.
     */
    std::string toString() const;
    
    /**
     * Returns true if the two lexicons have the same elements.
     */
    bool operator ==(const DawgLexicon& lex2) const;

    /**
     * Returns true if the two lexicons do not have the same elements.
     */
    bool operator !=(const DawgLexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons for ordering.
     */
    bool operator <(const DawgLexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons for ordering.
     */
    bool operator <=(const DawgLexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons for ordering.
     */
    bool operator >(const DawgLexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons for ordering.
     */
    bool operator >=(const DawgLexicon& lex2) const;

    /**
     * Returns the union of this lexicon and the given other lexicon,
     * which is the set of words that appear in at least one of the two.
     */
    DawgLexicon operator +(const DawgLexicon& lex2) const;

    /**
     * Returns the union of this lexicon and the given list,
     * which is the set of words that appear in at least one of the two.
     */
    DawgLexicon operator +(std::initializer_list<std::string> list) const;

    /**
     * Returns the union of this lexicon and the given word.
     */
    DawgLexicon operator +(const std::string& word) const;

    /**
     * Adds all of the words from the given lexicon to this one.
     */
    DawgLexicon& operator +=(const DawgLexicon& lex2);

    /**
     * Adds all of the words from the given list to this lexicon.
     */
    DawgLexicon& operator +=(std::initializer_list<std::string> list);

    /**
     * Adds the given word to this lexicon.
     */
    DawgLexicon& operator +=(const std::string& word);

    /**
     * Allows you to add multiple elements to a lexicon.
     * @example lex += "she", "sells", "sea", "shells";
     */
    DawgLexicon& operator ,(const std::string& word);

    // Implementation note: DawgLexicon does not support -, -=, *, and *=
    // operators like the standard Lexicon.  Because of its binary internal
    // format, it is non-trivial to support removal from a DawgLexicon.

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
#ifdef _WIN32
#define LITTLE_ENDIAN 1
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#pragma pack(1)
    struct Edge {
#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
        unsigned long letter:5;
        unsigned long lastEdge:1;
        unsigned long accept:1;
        unsigned long unused:1;
        unsigned long children:24;
#else
        unsigned long children:24;
        unsigned long unused:1;
        unsigned long accept:1;
        unsigned long lastEdge:1;
        unsigned long letter:5;
#endif
    };
#pragma pack()
    Edge* _edges;
    Edge* _start;
    int _edgeCount;
    int _dawgWordsCount;
    Set<std::string> _otherWords;

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
    class iterator : public std::iterator<std::input_iterator_tag, std::string> {
    private:
        const DawgLexicon* lp;
        int index;
        std::string currentDawgPrefix;
        std::string currentSetWord;
        std::string tmpWord;
        Edge* edgePtr;
        Stack<Edge*> stack;
        Set<std::string>::iterator setIterator;
        Set<std::string>::iterator setEnd;

        void advanceToNextWordInDawg();
        void advanceToNextWordInSet();
        void advanceToNextEdge();

    public:
        iterator() : lp(nullptr), index(0), edgePtr(nullptr) {
            /* empty */
        }

        iterator(const DawgLexicon* theLP, bool endFlag) {
            this->lp = theLP;
            if (endFlag) {
                index = lp->size();
            } else {
                index = 0;
                edgePtr = nullptr;
                setIterator = lp->_otherWords.begin();
                setEnd = lp->_otherWords.end();
                currentDawgPrefix = "";
                currentSetWord = "";
                advanceToNextWordInDawg();
                advanceToNextWordInSet();
            }
        }

        iterator(const iterator& it) {
            lp = it.lp;
            index = it.index;
            currentDawgPrefix = it.currentDawgPrefix;
            currentSetWord = it.currentSetWord;
            edgePtr = it.edgePtr;
            stack = it.stack;
            setIterator = it.setIterator;
        }

        iterator& operator ++() {
            if (!edgePtr) {
                advanceToNextWordInSet();
            } else {
                if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
                    advanceToNextWordInDawg();
                } else {
                    advanceToNextWordInSet();
                }
            }
            index++;
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return lp == rhs.lp && index == rhs.index;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        std::string operator *() {
            if (!edgePtr) {
                return currentSetWord;
            }
            if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
                return currentDawgPrefix + lp->ordToChar(edgePtr->letter);
            } else {
                return currentSetWord;
            }
        }

        std::string* operator ->() {
            if (!edgePtr) {
                return &currentSetWord;
            }
            if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
                tmpWord = currentDawgPrefix + lp->ordToChar(edgePtr->letter);
                return &tmpWord;
            } else {
                return &currentSetWord;
            }
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
    Edge* findEdgeForChar(Edge* children, char ch) const;
    Edge* traceToLastEdge(const std::string& s) const;
    void readBinaryFile(std::istream& input);
    void readBinaryFile(const std::string& filename);
    void deepCopy(const DawgLexicon& src);
    int countDawgWords(Edge* _start) const;

    unsigned int charToOrd(char ch) const {
        return ((unsigned int)(tolower(ch) - 'a' + 1));
    }

    char ordToChar(unsigned int ord) const {
        return ((char)(ord - 1 + 'a'));
    }
};

template <typename FunctorType>
void DawgLexicon::mapAll(FunctorType fn) const {
    for (std::string word : *this) {
        fn(word);
    }
}

/**
 * Hashing function for lexicons.
 */
int hashCode(const DawgLexicon& lex);

/**
 * Prints the lexicon to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const DawgLexicon& lex);

/**
 * Reads the contents of a lexicon from the given input stream.
 */
std::istream& operator >>(std::istream& is, DawgLexicon& lex);

#endif // _dawglexicon_h
