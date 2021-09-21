#include "testing/SimpleTest.h"
#include "strlib.h"
#include "set.h"
#include "map.h"
#include "goptionpane.h"
#include "gwindow.h"
using namespace std;

// miscellaneous bugs I was testing fall 2021

PROVIDED_TEST("Error if empty delimiter to stringSplit") {
    EXPECT_ERROR(stringSplit("Julie was here", ""));
}

PROVIDED_TEST("stringSplit correctly discard leading/repeated delimiters") {
    Vector<string> words;
    words = stringSplit(" apple banana chip    doodle    "," ");
    EXPECT_EQUAL(words.size(), 4);
    words = stringSplit("              "," ");
    EXPECT_EQUAL(words.size(), 0);
}

PROVIDED_TEST("stringToChar correctly handles single whitespace character (no erroenous trim!)") {
    EXPECT_EQUAL(' ', stringToChar(" "));
    EXPECT_EQUAL('\t', stringToChar("\t"));
    EXPECT_EQUAL('\n', stringToChar("\n"));
    EXPECT_ERROR(stringToChar("   a"));
}

PROVIDED_TEST("Error if empty find argument to stringReplace") {
    EXPECT_EQUAL(stringReplace("ababa", "a", "A"),"AbAbA");
    EXPECT_ERROR(stringReplace("ababa", "", "A"));
    string str = "ababa";
    stringReplaceInPlace(str, "a", "A");
    EXPECT_EQUAL(str,"AbAbA");
    EXPECT_ERROR(stringReplaceInPlace(str, "", "A"));
}

struct Person {
    int age;
    string name;
 };

bool operator< (const Person& lhs, const Person& rhs) {
    using namespace stanfordcpplib::collections;
    return compareTo(lhs.age,rhs.age, lhs.name, rhs.name) == -1;
}

PROVIDED_TEST("custom comparator") {
    using namespace stanfordcpplib::collections;
    Vector<Person> v = { {1, "Apple"},
                         {0, "Apple"}, {99, "Apple"},
                         {0, "Zebra"}, {99, "Zebra"},
                         {1, "Aardvark"},  {1, "Zebra"},
                         {1, "Apple"}};

    Set<Person> people;
    for (Person p : v) people.add(p);
    EXPECT_EQUAL(people.size(), 7);
    Person f = people.first();
    EXPECT_EQUAL(f.age, 0);

    Map<Person, int> map;
    for (Person p: people)  map.put(p, p.age);
    f = map.firstKey();
    EXPECT_EQUAL(f.age, 0);
}

PROVIDED_TEST("Dialog font GText") {
     GWindow* window = new GWindow(300, 200);
     window->setTitle("GText");
     window->setLocation(300, 100);
     window->setVisible(true);
    GText *gc = new GText("hello world", 25, 25);
    window->add(gc);
    //GOptionPane::showTextFileDialog("This is the text", "title", 10, 10);
}
