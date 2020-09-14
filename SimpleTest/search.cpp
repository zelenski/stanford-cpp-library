// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "search.h"
using namespace std;


string clean(string s)
{
    int first = 0,last = s.length()-1;
    while (first <= last) {
        if (ispunct(s[first])) first++;
        else if (ispunct(s[last])) last--;
        else break;
    }
    string result = s.substr(first, last-first+1);
    toLowerCaseInPlace(result);
    for (auto c : result)
        if (isalpha(c)) return result;
    return "";
}

Set<string> tokenize(string text)
{
    Set<string> unique;
    for (auto&t : stringSplit(text, " ")) {
        t = clean(t);
        if (!t.empty())
            unique.add(t);
    }
    return unique;
}

Map<string, Set<string>> readDocs(string dbfile)
{
    Map<string, Set<string>> docs;
    ifstream in;
    string url, contents;
    if (!openFile(in, dbfile))
        error("Could not open file ");

    while(getline(in, url)) {
        getline(in, contents);
        docs[url] = tokenize(contents);
    }
    return docs;
}

Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs)
{
    Map<string, Set<string>> index;

    for (const auto& url : docs) {
        for (const auto& word : docs[url]) {
            index[word].add(url);
        }
    }
    return index;
}


enum {AND, OR, WITHOUT };

Set<string> findQueryMatches(Map<string, Set<string>>& index, string sentence)
{
    Vector<string> terms = stringSplit(sentence, " ");
    Set<string> result, cur;
    for (auto t : terms) {
        int op = OR;
        if (t[0] == '+') {
            op = AND;
            t = t.substr(1);
        } else if (t[0] == '-') {
            op = WITHOUT;
            t = t.substr(1);
        }
        t = clean(t);
        Set<string> cur = index[t];
        switch (op) {
            case AND: result.intersect(cur); break;
            case OR: result.unionWith(cur);break;
            case WITHOUT: result.difference(cur);break;
        }
    }
    return result;
}


void searchEngine(string dbfile)
{
    cout << "Stand by while building index..." << endl;
    auto docs = readDocs(dbfile);
    auto index = buildIndex(docs);
    cout << index.keys() << endl;
    cout << "Indexed " << docs.size() << " pages containing "<< index.size() << " unique terms" << endl;
    while (true) {
        cout << endl;
        string q = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (q.empty()) break;
        Set<string> matches = findQueryMatches(index, q);
        cout << "Found " << matches.size() << " matching pages " << endl;
        cout << matches << endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Tokenize basic case") {
    string text = "hope is what defines humanity";
    Set<string> tokens = {"hope", "is", "what", "defines", "humanity"};
    EXPECT_EQUAL(tokenize(text), tokens);
}
#if 0
PROVIDED_TEST("Tokenize should discard whitespace") {
    string text = " they         might be      giants   ";
    Vector<string> tokens = {"they", "might", "be", "giants"};
    EXPECT_EQUAL(tokenize(text), tokens);
}

PROVIDED_TEST("Tokenize handles punctuation as single-char token") {
    string text = "i'm lovin cs106b!!!";
    Vector<string> tokens = {"i", "'", "m", "lovin", "cs106b", "!", "!", "!"};
    EXPECT_EQUAL(tokenize(text), tokens);
}

PROVIDED_TEST("Tokenize should lower-case tokens") {
    string text = "Mark Tessier-Lavigne";
    Vector<string> tokens = {"mark", "tessier", "-", "lavigne"};
    EXPECT_EQUAL(tokenize(text), tokens);
}
#endif
PROVIDED_TEST("Tokenize should discard whitespace/empty tokens") {
    string text = " they         might be      giants   ";
    Set<string> tokens = {"they", "might", "be", "giants"};
    EXPECT_EQUAL(tokenize(text), tokens);
}

PROVIDED_TEST("Tokenize trims punctuation from start,end of token") {
    string text = "i'm lovin' cs106b!!";
    Set<string> tokens = {"i'm", "lovin", "cs106b"};
    EXPECT_EQUAL(tokenize(text), tokens);
}

PROVIDED_TEST("Tokenize should lower-case tokens") {
    string text = "Mark Tessier-Lavigne";
    Set<string> tokens = {"mark", "tessier-lavigne"};
    EXPECT_EQUAL(tokenize(text), tokens);
}


PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

PROVIDED_TEST("clean") {
    string in = "Apple";
    string t = "apple";
    in = clean(in);
    EXPECT_EQUAL(in, t);
}

// TODO: add your test cases here

