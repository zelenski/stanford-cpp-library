/*
 * File: lexicon.h
 * ---------------
 * This file exports the <code>Lexicon</code> class, which is a
 * compact structure for storing a list of words.
 */

#ifndef _lexicon_h
#define _lexicon_h

#include <set>
#include <string>
#include "private/foreachpatch.h"
#include "set.h"

/*
 * Class: Lexicon
 * --------------
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
    Lexicon(std::string filename);

    /*
     * Destructor: ~Lexicon
     * --------------------
     * The destructor deallocates any storage associated with the lexicon.
     */
    virtual ~Lexicon();

    /*
     * Method: add
     * Usage: lex.add(word);
     * ---------------------
     * Adds the specified word to the lexicon, if not already present.
     * The word is converted to lowercase and all non-alphabetic characters
     * are stripped from it before adding it to the lexicon.
     * Returns true if the word was not previously contained in the lexicon.
     * The empty string cannot be added to a lexicon.
     */
    bool add(std::string word);

    /*
     * Method: addWordsFromFile
     * Usage: lex.addWordsFromFile(filename);
     * --------------------------------------
     * Reads the file and adds all of its words to the lexicon.
     * Each word from the file is converted to lowercase and all non-alphabetic
     * characters are stripped from it before adding it to the lexicon.
     */
    void addWordsFromFile(std::string filename);

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
     * lexicon.  In the <code>Lexicon</code> class, the case of letters is
     * ignored, so "Zoo" is the same as "ZOO" or "zoo".
     * The empty string cannot be contained in a lexicon.
     */
    bool contains(std::string word) const;

    /*
     * Method: containsPrefix
     * Usage: if (lex.containsPrefix(prefix)) ...
     * ------------------------------------------
     * Returns true if any words in the lexicon begin with <code>prefix</code>.
     * Like <code>containsWord</code>, this method ignores the case of letters
     * so that "MO" is a prefix of "monkey" or "Monday".
     * The empty string is a prefix of every string, so this method returns
     * true when passed the empty string.
     */
    bool containsPrefix(std::string prefix) const;

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
    void mapAll(void (*fn)(const std::string &)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    /*
     * Method: remove
     * Usage: lex.remove(word);
     * ---------------------
     * Removes the specified word from the lexicon, if it was present.
     * Returns true if the word was previously contained in the lexicon;
     * in other words, if a word was removed.
     * The empty string cannot be contained in a lexicon, so passing the
     * empty string to this method returns false.
     */
    bool remove(std::string word);

    /*
     * Method: removePrefix
     * Usage: lex.removePrefix(prefix);
     * ---------------------
     * Removes all words from the lexicon that begin with the given prefix.
     * Returns true if the prefix was previously contained in the lexicon;
     * in other words, if any words were removed.
     * If the empty string is passed, since all words begin with the empty
     * string, all words will be removed and this method will
     * return true if the lexicon was non-empty prior to the call.
     */
    bool removePrefix(std::string prefix);

    /*
     * Method: size
     * Usage: int n = lex.size();
     * --------------------------
     * Returns the number of words contained in the lexicon.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = lexicon.toString();
     * -----------------------------------
     * Converts the lexicon to a printable string representation.
     * Note that this can be an expensive operation if the lexicon contains
     * a large number of words.
     */
    std::string toString() const;

    /*
     * Returns an STL set object with the same elements as this Lexicon.
     */
    std::set<std::string> toStlSet() const;

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
            m_isWord = false;
            for (int i = 0; i < 26; i++) {
                m_children[i] = NULL;
            }
        }

        // pre: letter is between 'a' and 'z' in lowercase
        inline TrieNode*& child(char letter) {
            return m_children[letter - 'a'];
        }

        inline int childCount() {
            int count = 0;
            for (int i = 0; i < 26; i++) {
                if (m_children[i] != NULL) {
                    count++;
                }
            }
            return count;
        }

        inline bool isLeaf() {
            for (int i = 0; i < 26; i++) {
                if (m_children[i] != NULL) {
                    return false;
                }
            }
            return true;
        }

        inline bool isWord() {
            return m_isWord;
        }

        inline void setWord(bool value) {
            m_isWord = value;
        }

    private:
        /* instance variables */
        bool m_isWord;
        TrieNode* m_children[26];   // 0=a, 1=b, 2=c, ..., 25=z
    };

    /*
     * private helper functions, including
     * recursive helpers to implement public add/contains/remove
     */
    bool addHelper(TrieNode*& node, std::string word, const string& originalWord);
    bool containsHelper(TrieNode* node, std::string word, bool isPrefix) const;
    void deepCopy(const Lexicon& src);
    void deleteTree(TrieNode* node);
    void readBinaryFile(std::string filename);
    bool removeHelper(TrieNode*& node, std::string word, const string& originalWord, bool isPrefix);
    void removeSubtreeHelper(TrieNode*& node, const string& originalWord);
    
    friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);
    friend std::istream& operator >>(std::istream& is, Lexicon& lex);

    /* instance variables */
    TrieNode* m_root;
    int m_size;
    Set<string> m_allWords;   // secondary structure of all words for foreach;
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
    class iterator : public Set<string>::iterator {
    public:
        iterator() : Set<string>::iterator() {}
        iterator(const iterator& it) : Set<string>::iterator(it) {}
        iterator(const Set<string>::iterator& it) : Set<string>::iterator(it) {}
    };

    /*
     * Returns an iterator positioned at the first word in the lexicon.
     */
    iterator begin() const {
        return iterator(m_allWords.begin());
    }

    /*
     * Returns an iterator positioned at the last word in the lexicon.
     */
    iterator end() const {
        return iterator(m_allWords.end());
    }
};

template <typename FunctorType>
void Lexicon::mapAll(FunctorType fn) const {
    __foreach__ (std::string word __in__ *this) {
        fn(word);
    }
}

// hashing functions for lexicons;  defined in hashmap.cpp
int hashCode(const Lexicon& l);

/*
 * Prints the lexicon to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const Lexicon& lex);

#endif
