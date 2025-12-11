#include "shared.h"
using namespace std;

void node_pair() {
    // key align 1,4,8 and val align 1,4,8
    Thing t = {1891, "stanford"};
    GridLocation gl = {3, 14};
    {
        Map<char, char> treenode_11 = { {'B','b'} };
        Map<char, int> treenode_14 = { {'B',106} };
        Map<char, double> treenode_18 = { {'B',0.5} };
        Map<int, char> treenode_41 = { {106,'B'} };
        Map<int, int> treenode_44 = { {106,1891} };
        Map<int, double> treenode_48 = { {106,0.5} };
        Map<double, char> treenode_81 = { {0.5,'B'} };
        Map<double, int> treenode_84 = { {0.5,106} };
        Map<double, double> treenode_88 = { {0.5,0.25} };
        BREAKPOINT;
    }
    {
        HashMap<char, char> hashnode_11 = { {'B','b'} };
        HashMap<char, int> hashnode_14 = { {'B',106} };
        HashMap<char, double> hashnode_18 = { {'B',0.5} };
        HashMap<int, char> hashnode_41 = { {106,'B'} };
        HashMap<int, int> hashnode_44 = { {106,1891} };
        HashMap<int, double> hashnode_48 = { {106,0.5} };
        HashMap<double, char> hashnode_81 = { {0.5,'B'} };
        HashMap<double, int> hashnode_84 = { {0.5,106} };
        HashMap<double, double> hashnode_88 = { {0.5,0.25} };
        BREAKPOINT;
    }
    // struct with align 4,8 as key, again as val
    {
        Map<char, GridLocation> treenode_14 = { {'B',gl} };
        Map<char, Thing> treenode_18 = { {'B',t} };
        Map<int, GridLocation> treenode_44 = { {106,gl} };
        Map<int, Thing> treenode_48 = { {106,t} };
        Map<double, GridLocation> treenode_84 = { {0.5,gl} };
        Map<double, Thing> treenode_88 = { {0.5,t} };

        Map<GridLocation, char> treenode_41 = { {gl,'B'} };
        Map<Thing, char> treenode_81 = { {t,'B'} };
        Map<GridLocation, int> treenode_44s = { {gl,106} };
        Map<Thing, int> treenode_48s = { {t,106} };
        Map<GridLocation, double> treenode_84s = { {gl,0.5} };
        Map<Thing, double> treenode_88s = { {t,0.5} };
        BREAKPOINT;
    }
    {
        HashMap<char, GridLocation> hashnode_14 = { {'B',gl} };
        HashMap<char, Thing> hashnode_18 = { {'B',t} };
        HashMap<int, GridLocation> hashnode_44 = { {106,gl} };
        HashMap<int, Thing> hashnode_48 = { {106,t} };
        HashMap<double, GridLocation> hashnode_84 = { {0.5,gl} };
        HashMap<double, Thing> hashnode_88 = { {0.5,t} };

        HashMap<GridLocation, char> hashnode_41 = { {gl,'B'} };
        HashMap<Thing, char> hashnode_81 = { {t,'B'} };
        HashMap<GridLocation, int> hashnode_44s = { {gl,106} };
        HashMap<Thing, int> hashnode_48s = { {t,106} };
        HashMap<GridLocation, double> hashnode_84s = { {gl,0.5} };
        HashMap<Thing, double> hashnode_88s = { {t,0.5} };
        BREAKPOINT;
    }
}

