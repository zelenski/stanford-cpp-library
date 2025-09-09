#include "console.h"
#include <iostream>
#include "lexicon.h"
using namespace std;


int main() {
    cout << "Starting main() " << endl;
    Lexicon lex("res/EnglishWords.txt");

    // change init to ctor and separate call is enough to fx
    // Lexicon lex;
    //lex.addWordsFromFile("res/EnglishWords.txt");

    // another minimal fix is to simply have TrieLexicon.h/.cpp in project and recompile

    cout << lex.size() << endl;
    cout << endl << "main() completed." << endl;
    return 0;
}

void test(void) {
    Set<string> suggestions;
}