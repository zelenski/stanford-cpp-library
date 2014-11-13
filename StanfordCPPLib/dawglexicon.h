/*
 * File: dawglexicon.h
 * -------------------
 * This file exports the <code>DawgLexicon</code> class, which is a
 * compact structure for storing a list of words.
 * 
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - added equals method
 * - added comparison operators ==, !=
 * - added toStlSet method
 * - fixed inclusion of foreach macro to avoid errors
 */

#ifndef _dawglexicon_h
#define _dawglexicon_h

#include <set>
#include <string>
#include "set.h"
#include "stack.h"

/*
 * Class: DawgLexicon
 * ------------------
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
 *
 * <p>As an example of the use of the <code>DawgLexicon</code> class, the
 * following program lists all the two-letter words in the lexicon
 * stored in <code>EnglishWords.dat</code>:
 *
 *<pre>
 *    int main() {
 *       DawgLexicon english("EnglishWords.dat");
 *       for (string word : english) {
 *          if (word.length() == 2) {
 *             cout << word << endl;
 *          }
 *       }
 *       return 0;
 *    }
 *</pre>
 */

#include <cctype>

class DawgLexicon {
public:
    /*
     * Constructor: DawgLexicon
     * Usage: DawgLexicon lex;
     *        DawgLexicon lex(filename);
     * ---------------------------------
     * Initializes a new lexicon.  The default constructor creates an empty
     * lexicon.  The second form reads in the contents of the lexicon from
     * the specified data file.  The data file must be in one of two formats:
     * (1) a space-efficient precompiled binary format or (2) a text file
     * containing one word per line.  The Stanford library distribution
     * includes a binary lexicon file named <code>English.dat</code>
     * containing a list of words in English.  The standard code pattern
     * to initialize that lexicon looks like this:
     *
     *<pre>
     *    DawgLexicon english("English.dat");
     *</pre>
     */
    DawgLexicon();
    DawgLexicon(const std::string& filename);

    /*
     * Destructor: ~DawgLexicon
     * ------------------------
     * The destructor deallocates any storage associated with the lexicon.
     */
    virtual ~DawgLexicon();
    
    /*
     * Method: add
     * Usage: lex.add(word);
     * ---------------------
     * Adds the specified word to the lexicon.
     */
    void add(const std::string& word);
    
    /*
     * Method: addWordsFromFile
     * Usage: lex.addWordsFromFile(filename);
     * --------------------------------------
     * Reads the file and adds all of its words to the lexicon.
     */
    void addWordsFromFile(const std::string& filename);
    
    /*
     * Method: clear
     * Usage: lex.clear();
     * -------------------
     * Removes all words from the lexicon.
     */
    void clear();
    
    /*
     * Method: contains
     * Usage: if (lex.contains(word)) ...
     * ----------------------------------
     * Returns <code>true</code> if <code>word</code> is contained in the
     * lexicon.  In the <code>DawgLexicon</code> class, the case of letters is
     * ignored, so "Zoo" is the same as "ZOO" or "zoo".
     */
    bool contains(const std::string& word) const;
    
    /*
     * Method: containsPrefix
     * Usage: if (lex.containsPrefix(prefix)) ...
     * ------------------------------------------
     * Returns true if any words in the lexicon begin with <code>prefix</code>.
     * Like <code>containsWord</code>, this method ignores the case of letters
     * so that "MO" is a prefix of "monkey" or "Monday".
     */
    bool containsPrefix(const std::string& prefix) const;
    
    /*
     * Method: equals
     * Usage: if (lex1.equals(lex2)) ...
     * ---------------------------------
     * Compares two lexicons for equality.
     * Returns <code>true</code> if this lexicon contains exactly the same
     * values as the given other lexicon.
     * Identical in behavior to the == operator.
     */
    bool equals(const DawgLexicon& lex2) const;
    
    /*
     * Method: isEmpty
     * Usage: if (lex.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if the lexicon contains no words.
     */
    bool isEmpty() const;
    
    /*
     * Method: mapAll
     * Usage: lexicon.mapAll(fn);
     * --------------------------
     * Calls the specified function on each word in the lexicon.
     */
    void mapAll(void (*fn)(std::string)) const;
    void mapAll(void (*fn)(const std::string&)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    /*
     * Method: size
     * Usage: int n = lex.size();
     * --------------------------
     * Returns the number of words contained in the lexicon.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = lex.toString();
     * -----------------------------------
     * Converts the lexicon to a printable string representation.
     * Note that this can be an expensive operation if the lexicon contains
     * a large number of words.
     */
    std::string toString() const;
    
    /*
     * Returns an STL set object with the same elements as this lexicon.
     */
    std::set<std::string> toStlSet() const;
    
    /*
     * Operators: ==, !=
     * Usage: if (lex1 == lex2) ...
     * Usage: if (lex1 != lex2) ...
     * ...
     * ----------------------------
     * Relational operators to compare two lexicons to see if they have the same elements.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     */
    bool operator ==(const DawgLexicon& lex2) const;
    bool operator !=(const DawgLexicon& lex2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: if (lex1 <= lex2) ...
     * ...
     * ----------------------------
     * Relational operators to compare two lexicons to see if they have the same elements.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const DawgLexicon& lex2) const;
    bool operator <=(const DawgLexicon& lex2) const;
    bool operator >(const DawgLexicon& lex2) const;
    bool operator >=(const DawgLexicon& lex2) const;

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
    Edge* edges;
    Edge* start;
    int numEdges;
    int numDawgWords;
    Set<std::string> otherWords;

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
        iterator() : lp(NULL), index(0), edgePtr(NULL) {
            /* empty */
        }

        iterator(const DawgLexicon* lp, bool endFlag) {
            this->lp = lp;
            if (endFlag) {
                index = lp->size();
            } else {
                index = 0;
                edgePtr = NULL;
                setIterator = lp->otherWords.begin();
                setEnd = lp->otherWords.end();
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
            if (edgePtr == NULL) {
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
            if (edgePtr == NULL) {
                return currentSetWord;
            }
            if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
                return currentDawgPrefix + lp->ordToChar(edgePtr->letter);
            } else {
                return currentSetWord;
            }
        }

        std::string* operator ->() {
            if (edgePtr == NULL) {
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

    iterator begin() const {
        return iterator(this, false);
    }

    iterator end() const {
        return iterator(this, true);
    }

private:
    Edge* findEdgeForChar(Edge* children, char ch) const;
    Edge* traceToLastEdge(const std::string& s) const;
    void readBinaryFile(const std::string& filename);
    void deepCopy(const DawgLexicon& src);
    int countDawgWords(Edge* start) const;

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

/*
 * hashing function for lexicons
 */
int hashCode(const DawgLexicon& lex);

std::ostream& operator <<(std::ostream& os, const DawgLexicon& lex);

#endif
