#include "shared.h"
using namespace::std;

void stl() {
    std::vector<int> stl_v = {106,107};
    Vector<int> stanford_v = {106, 107};
    std::unordered_map<int, string> stl_um = {{106, "stanford"}};
    std::map<int, string> stl_m = {{106, "stanford"}};
    HashMap<int, string> stanford_hm = {{106, "stanford"}};
    Map<int, string> stanford_m = {{106, "stanford"}};
    BREAKPOINT;
}

