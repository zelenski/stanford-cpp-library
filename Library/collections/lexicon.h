/*
 * File: lexicon.h
 * ---------------
 * This file exports the <code>Lexicon</code> class, which is a
 * compact structure for storing a list of words.
 *
 * @version 2019/04/09
 * - renamed private members with underscore naming scheme for consistency
 * @author Marty Stepp
 * @version 2018/03/10
 * - added methods front, back
 * @version 2016/12/09
 * - added iterator version checking support (implicitly via Set)
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/12
 * - added methods addAll, containsAll, removeAll, retainAll,
 *   operators +, +=, -, -=, *, *= to better match Set/HashSet
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2014/11/13
 * - added comparison operators <, >= etc.
 * - added hashCode function
 * @version 2014/10/10
 * - added equals method, comparison operators ==, !=
 * - fixed inclusion of for-each macro to avoid errors
 */


#ifndef _lexicon_h
#define _lexicon_h

#include <initializer_list>
#include <iterator>
#include <set>
#include <string>

#include "hashcode.h"
#include "set.h"

/**
 * This class is used to represent a <b><i>lexicon,</i></b> or word list.
 * The main difference between a lexicon and a dictionary is that
 * a lexicon does not provide any mechanism for storing definitions;
 * the lexicon contains only words, with no associated information.
 * It is therefore similar to a set of strings, but with a more
 * space-efficient internal representation.  The <code>Lexicon</code>
 * class supports efficient lookup operations for words and prefixes.
 *
 * <p>As an example of the use of the <code>Lexicon</code> class, the
 * following program lists all the two-letter words in the lexicon
 * stored in <code>EnglishWords.dat</code>:
 *
 *<pre>
 *    int main() {
 *       Lexicon english("EnglishWords.dat");
 *       for (string word : english) {
 *          if (word.length() == 2) {
 *             cout << word << endl;
 *          }
 *       }
 *       return 0;
 *    }
 *</pre>
 */

class Lexicon {
public:
    /*
     * Constructor: Lexicon
     * Usage: Lexicon lex;
     *        Lexicon lex(filename);
     * -----------------------------
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
     *    Lexicon english("English.dat");
     *</pre>
     */
    Lexicon();
    Lexicon(std::istream& input);
    Lexicon(const std::string& filename);

    /*
     * This constructor uses an initializer list to set up the lexicon.
     * Usage: Lexicon lex {1, 2, 3};
     */
    Lexicon(std::initializer_list<std::string> list);

    /*
     * Destructor: ~Lexicon
     * --------------------
     * The destructor deallocates any storage associated with the lexicon.
     */
    virtual ~Lexicon();

    /**
     * Adds the specified word to the lexicon, if not already present.
     * The word is converted to lowercase before adding it to the lexicon.
     * If the word contains any non-alphabetic characters (including whitespace),
     * it will not be added. The empty string cannot be added to a lexicon.
     * Returns true if the word was added successfully to the lexicon.
     */
    bool add(const std::string& word);

    /**
     * Adds all elements of the given other lexicon to this lexicon.
     * Returns a reference to this lexicon.
     * Identical in behavior to the += operator.
     */
    Lexicon& addAll(const Lexicon& lex);

    /**
     * Adds all elements of the given initializer list to this lexicon.
     * Returns a reference to this lexicon.
     * Identical in behavior to the += operator.
     */
    Lexicon& addAll(std::initializer_list<std::string> list);

    /**
     * Reads the given input stream and adds all of its words to the lexicon.
     * Each word from the stream is converted to lowercase before adding it.
     */
    void addWordsFromFile(std::istream& input);

    /**
     * Reads the file and adds all of its words to the lexicon.
     * Each word from the file is converted to lowercase before adding it.
     */
    void addWordsFromFile(const std::string& filename);

    /**
     * Returns the last value in the lexicon in alphabetical order.
     * If the set is empty, generates an error.
     */
    std::string back() const;

    /**
     * Removes all words from the lexicon.
     */
    void clear();

    /**
     * Returns <code>true</code> if <code>word</code> is contained in the
     * lexicon.  In the <code>Lexicon</code> class, the case of letters is
     * ignored, so "Zoo" is the same as "ZOO" or "zoo".
     * The empty string cannot be contained in a lexicon, nor can any word
     * containing any non-alphabetic characters such as punctuation or whitespace.
     */
    bool contains(const std::string& word) const;

    /**
     * Returns <code>true</code> if every value from the given other lexicon
     * is also found in this lexicon.
     * Equivalent in behavior to isSupersetOf.
     */
    bool containsAll(const Lexicon& set2) const;

    /**
     * Returns <code>true</code> if every value from the given initializer list
     * is also found in this lexicon.
     * Equivalent in behavior to isSupersetOf.
     */
    bool containsAll(std::initializer_list<std::string> list) const;

    /**
     * Returns true if any words in the lexicon begin with <code>prefix</code>.
     * Like <code>containsWord</code>, this method ignores the case of letters
     * so that "MO" is a prefix of "monkey" or "Monday".
     * The empty string is a prefix of every string, so this method returns
     * true when passed the empty string.
     */
    bool containsPrefix(const std::string& prefix) const;

    /**
     * Compares two lexicons for equality.
     */
    bool equals(const Lexicon& lex2) const;

    /**
     * Returns the first value in the lexicon in alphabetical order.
     * If the set is empty, <code>first</code> generates an error.
     * Equivalent to first.
     */
    std::string first() const;

    /**
     * Returns the first value in the lexicon in alphabetical order.
     * If the set is empty, generates an error.
     * Equivalent to first.
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
     * Returns whether every word of this lexicon is contained in the given set.
     */
    bool isSubsetOf(const Lexicon& lex2) const;

    /**
     * Returns whether every word of this lexicon is contained in the given
     * initializer list.
     */
    bool isSubsetOf(std::initializer_list<std::string> list) const;

    /**
     * Returns whether every word of the given lexicon is contained in this lexicon.
     * Equivalent in behavior to containsAll.
     */
    bool isSupersetOf(const Lexicon& lex2) const;

    /**
     * Returns whether every word of the given list is contained in this lexicon.
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

    /**
     * Removes the specified word from the lexicon, if it was present.
     * Returns true if the word was previously contained in the lexicon;
     * in other words, if a word was removed.
     * The empty string cannot be contained in a lexicon, so passing the
     * empty string to this method returns false.
     */
    bool remove(const std::string& word);

    /**
     * Removes all elements of the given other lexicon from this lexicon.
     * Returns a reference to this lexicon.
     * Identical in behavior to the -= operator.
     */
    Lexicon& removeAll(const Lexicon& lex);

    /**
     * Removes all elements of the given other list from this lexicon.
     * Returns a reference to this lexicon.
     * Identical in behavior to the -= operator.
     */
    Lexicon& removeAll(std::initializer_list<std::string> list);

    /**
     * Removes all words from the lexicon that begin with the given prefix.
     * Returns true if the prefix was previously contained in the lexicon;
     * in other words, if any words were removed.
     * If the empty string is passed, since all words begin with the empty
     * string, all words will be removed and this method will
     * return true if the lexicon was non-empty prior to the call.
     */
    bool removePrefix(const std::string& prefix);

    /**
     * Removes all elements from this lexicon that are not contained in the given
     * other lexicon.
     * Returns a reference to this lexicon.
     * Identical in behavior to the *= operator.
     */
    Lexicon& retainAll(const Lexicon& lex);

    /**
     * Removes all elements from this lexicon that are not contained in the given
     * initializer list.
     * Returns a reference to this lexicon.
     * Identical in behavior to the *= operator.
     */
    Lexicon& retainAll(std::initializer_list<std::string> list);

    /**
     * Returns the number of words contained in the lexicon.
     */
    int size() const;

    /**
     * Converts the lexicon to a printable string representation
     * such as <code>{"a", "b", "c"}</code>.
     * Note that this can be an expensive operation if the lexicon contains
     * a large number of words.
     */
    std::string toString() const;

    /**
     * Returns true if two lexicons have the same elements.
     */
    bool operator ==(const Lexicon& lex2) const;

    /**
     * Returns true if two lexicons do not have the same elements.
     */
    bool operator !=(const Lexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons.
     */
    bool operator <(const Lexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons.
     */
    bool operator <=(const Lexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons.
     */
    bool operator >(const Lexicon& lex2) const;

    /**
     * Relational operators to compare two lexicons.
     */
    bool operator >=(const Lexicon& lex2) const;

    /**
     * Returns the union of lexicons <code>lex1</code> and <code>lex2</code>,
     * which is the set of words that appear in at least one of the two.
     */
    Lexicon operator +(const Lexicon& lex2) const;

    /**
     * Returns the union of this lexicon and the words in the given list,
     * which is the set of words that appear in at least one of the two.
     */
    Lexicon operator +(std::initializer_list<std::string> list) const;

    /**
     * Returns the union of this lexicon and the given word.
     */
    Lexicon operator +(const std::string& word) const;

    /**
     * Returns the intersection of two lexicons,
     * which is the set of all words that appear in both.
     */
    Lexicon operator *(const Lexicon& lex2) const;

    /**
     * Returns the intersection of this lexicons and the given list,
     * which is the set of all words that appear in both.
     */
    Lexicon operator *(std::initializer_list<std::string> list) const;

    /**
     * Returns the difference of two lexicons,
     * which is all of the words that appear in <code>lex1</code> but
     * not <code>lex2</code>.
     * You can also pass an initializer list such as {"a", "b", "c"}.
     * The right hand set can be replaced by a single word, in
     * which case the operator returns a new lexicon formed by removing that word.
     */
    Lexicon operator -(const Lexicon& lex2) const;

    /**
     * Returns the difference of two lexicons,
     * which is all of the words that appear in this lexicon but not in
     * the given initializer list.
     */
    Lexicon operator -(std::initializer_list<std::string> list) const;

    /**
     * Returns a new lexicon formed by removing the given word from this lexicon.
     */
    Lexicon operator -(const std::string& word) const;

    /**
     * Adds all of the words from the given lexicon to this lexicon.
     */
    Lexicon& operator +=(const Lexicon& lex2);

    /**
     * Adds all of the words from the given list to this lexicon.
     */
    Lexicon& operator +=(std::initializer_list<std::string> list);

    /**
     * Adds the specified word to this lexicon.
     */
    Lexicon& operator +=(const std::string& word);

    /**
     * Removes any elements from this lexicon that are not present in
     * <code>lex2</code>.
     */
    Lexicon& operator *=(const Lexicon& lex2);

    /**
     * Removes any elements from this lexicon that are not present in
     * the given initializer list.
     */
    Lexicon& operator *=(std::initializer_list<std::string> list);

    /**
     * Removes all elements in the given lexicon from this lexicon.
     */
    Lexicon& operator -=(const Lexicon& lex2);

    /**
     * Removes all elements in the given list from this lexicon.
     */
    Lexicon& operator -=(std::initializer_list<std::string> list);

    /**
     * Removes the given word from this lexicon.
     */
    Lexicon& operator -=(const std::string& value);

    /**
     * Allows you to add multiple elements to a lexicon.
     *  @example  lex += "she", "sells", "sea", "shells";
     */
    Lexicon& operator ,(const std::string& word);

    /*
     * Additional Lexicon operations
     * -----------------------------
     * In addition to the methods listed in this interface, the Lexicon
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
    struct TrieNode {
    public:
        TrieNode() {
            _isWord = false;
            for (int i = 0; i < 26; i++) {
                _children[i] = nullptr;
            }
        }

        // pre: letter is between 'a' and 'z' in lowercase
        inline TrieNode*& child(char letter) {
            return _children[letter - 'a'];
        }

        inline int childCount() const {
            int count = 0;
            for (int i = 0; i < 26; i++) {
                if (_children[i] != nullptr) {
                    count++;
                }
            }
            return count;
        }

        inline bool isLeaf() const {
            return childCount() == 0;
        }

        inline bool isWord() const {
            return _isWord;
        }

        inline void setWord(bool value) {
            _isWord = value;
        }

    private:
        /* instance variables */
        bool _isWord;
        TrieNode* _children[26];   // 0=a, 1=b, 2=c, ..., 25=z
    };

    /*
     * private helper functions, including
     * recursive helpers to implement public add/contains/remove
     */
    bool addHelper(TrieNode*& node, const std::string& word, const std::string& originalWord);
    bool containsHelper(TrieNode* node, const std::string& word, bool isPrefix) const;
    void deepCopy(const Lexicon& src);
    void deleteTree(TrieNode* node);
    bool removeHelper(TrieNode*& node, const std::string& word, const std::string& originalWord, bool isPrefix);
    void removeSubtreeHelper(TrieNode*& node, const std::string& originalWord);

    friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);
    friend std::istream& operator >>(std::istream& is, Lexicon& lex);

    /* instance variables */
    TrieNode* _root;
    int _size;
    bool _removeFlag;             // flag to differentiate += and -= when used with ,
    Set<std::string> _allWords;   // secondary structure of all words for foreach;
                                  // basically a cop-out so I can loop over words

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
    Lexicon(const Lexicon& src);
    Lexicon& operator =(const Lexicon& src);

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public Set<std::string>::iterator {
    public:
        iterator() : Set<std::string>::iterator() {}
        iterator(const iterator& it) : Set<std::string>::iterator(it) {}
        iterator(const Set<std::string>::iterator& it) : Set<std::string>::iterator(it) {}
    };

    /**
     * Returns an iterator positioned at the first word in the lexicon.
     */
    iterator begin() const {
        return iterator(_allWords.begin());
    }

    /**
     * Returns an iterator positioned at the last word in the lexicon.
     */
    iterator end() const {
        return iterator(_allWords.end());
    }
};

template <typename FunctorType>
void Lexicon::mapAll(FunctorType fn) const {
    for (std::string word : *this) {
        fn(word);
    }
}

/**
 * Hashing function for lexicons.
 */
int hashCode(const Lexicon& l);

/**
 * Prints the lexicon to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const Lexicon& lex);

/**
 * Reads the contents of a lexicon from the given input stream.
 */
std::istream& operator >>(std::istream& is, Lexicon& lex);

#endif // _lexicon_h
