
#include "vector.h"
using namespace std;

int gNum;

void please_no()
{
    string str = "ulie";
    int sum = 0;
    for (int i = 0; i < str.length(); i++)
        sum += str[i];
}
#if 0
int missing_return(int unused)
{
    int uninit;
    gNum = uninit;
}
#endif

const int ignored_qualilfier(int x)
{
    return 5;
}
void empty_body() {
    for (int i = 0; i < 10; i++) ;;
}
#if 0
void readonly_literal()
{
    char *literal = "julie";
    literal[0] = 'k';
    "other"[0] = 'j';
}
#endif

void null_arith()
{
    int *p = (int *)NULL + 4;
    *p = 0;
}

static const int a = 27;
const int b = 42;
struct location { int x; int y;};

int missing_field()
{
    location loc = {5};
    return loc.x;
}


