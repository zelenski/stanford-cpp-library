#include <iostream>
#include "console.h"
#include "gridlocation.h"
#include "stack.h"
#include "gwindow.h"
#include "goptionpane.h"
#include "SimpleTest.h"
using namespace std;


void graphics_fixes()
{
    GWindow* window = new GWindow(200, 200);
    window->setTitle("Bug test");
    window->setLocation(50, 50);
    window->setBackground("White");
    window->clear();
    window->setColor("#008F00");
    // fix behavior with fill polygon
    double x = 100, y = 45, w = 15, h = 30;
    for (int i = 0; i < 4; i++) {
        window->fillPolygon({GPoint(x-w, y+h),GPoint(x, y),GPoint(x+w, y+h) } );
        y += h/2;
    }
    window->setVisible(true);
}

void gridlocation_fixes()
{
    // GridLocation used to not be able to properly read from a stream
    // It also had one-arg constructor that was bonus
    GridLocation none;
   // GridLocation one = (1,2);
   // GridLocation one = 1;
    GridLocation one(1, 2);

    Stack<GridLocation> s = {none, one, {3, 4}, {5, 6}};
    Stack<GridLocation> t;

    {
        ostringstream output;
        output << s << endl;
        istringstream input(output.str());
        bool flag = (input >> t) ? true : false;
        cout << "read succcess? " << flag << ", same contents? " << (s == t) << endl;
    }
    {
        Stack<GridLocation> empty;
        ostringstream output;
        output << empty << endl;
        istringstream input(output.str());
        bool flag = (input >> t) ? true : false;
        cout << "read succcess? " << flag << ", same contents? " << (empty == t) << endl;
    }
    {
        Stack<GridLocation> bad;
        istringstream input("{r1c2,r2c3,rc,r6c7");
        bool flag = (input >> bad) ? true : false;
        cout << "read succcess? " << flag << ", stack contains " << bad << endl;
    }
}

int main()
{
    if (!runSimpleTests(SELECTED_TESTS)) {
        graphics_fixes();
        gridlocation_fixes();
    }
    return 0;
}

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

STUDENT_TEST("Dialog font GText") {
    GWindow* window = new GWindow(300, 200);
    window->setTitle("GText");
    window->setLocation(300, 100);
    window->setVisible(true);
    GText *gc = new GText("hello world", 25, 25);
    window->add(gc);
    GOptionPane::showTextFileDialog("This is the text", "title", 10, 10);
}
