#include "shared.h"
using namespace std;

void stanford_hash() {
	{
		HashMap<bool, bool> hashmap_bool_bool = {{false, false}, {true, true}};
		HashMap<char, bool> hashmap_char_bool = {{'b', false}, {'B', true}};
		HashMap<double, bool> hashmap_double_bool = {{3.14159, false}, {-0.25, true}};
		HashMap<int, bool> hashmap_int_bool = {{106, false}, {1891, true}};
		HashMap<string, bool> hashmap_string_bool = {{"stanford", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		HashMap<GridLocation, bool> hashmap_gridlocation_bool = {{{3, 14}, false}};
		HashMap<Thing, bool> hashmap_thing_bool = {{(Thing){6, "purple"}, false}};
		HashMap<bool, char> hashmap_bool_char = {{false, 'b'}, {true, 'B'}};
		HashMap<char, char> hashmap_char_char = {{'b', 'b'}, {'B', 'B'}};
		HashMap<double, char> hashmap_double_char = {{3.14159, 'b'}, {-0.25, 'B'}};
		HashMap<int, char> hashmap_int_char = {{106, 'b'}, {1891, 'B'}};
		HashMap<string, char> hashmap_string_char = {{"stanford", 'b'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		HashMap<GridLocation, char> hashmap_gridlocation_char = {{{3, 14}, 'b'}};
		HashMap<Thing, char> hashmap_thing_char = {{(Thing){6, "purple"}, 'b'}};
		HashMap<bool, double> hashmap_bool_double = {{false, 3.14159}, {true, -0.25}};
		HashMap<char, double> hashmap_char_double = {{'b', 3.14159}, {'B', -0.25}};
		HashMap<double, double> hashmap_double_double = {{3.14159, 3.14159}, {-0.25, -0.25}};
		HashMap<int, double> hashmap_int_double = {{106, 3.14159}, {1891, -0.25}};
		HashMap<string, double> hashmap_string_double = {{"stanford", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		HashMap<GridLocation, double> hashmap_gridlocation_double = {{{3, 14}, 3.14159}};
		HashMap<Thing, double> hashmap_thing_double = {{(Thing){6, "purple"}, 3.14159}};
		HashMap<bool, int> hashmap_bool_int = {{false, 106}, {true, 1891}};
		HashMap<char, int> hashmap_char_int = {{'b', 106}, {'B', 1891}};
		HashMap<double, int> hashmap_double_int = {{3.14159, 106}, {-0.25, 1891}};
		HashMap<int, int> hashmap_int_int = {{106, 106}, {1891, 1891}};
		HashMap<string, int> hashmap_string_int = {{"stanford", 106}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 1891}};
		HashMap<GridLocation, int> hashmap_gridlocation_int = {{{3, 14}, 106}};
		HashMap<Thing, int> hashmap_thing_int = {{(Thing){6, "purple"}, 106}};
		HashMap<bool, string> hashmap_bool_string = {{false, "stanford"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<char, string> hashmap_char_string = {{'b', "stanford"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<double, string> hashmap_double_string = {{3.14159, "stanford"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<int, string> hashmap_int_string = {{106, "stanford"}, {1891, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<string, string> hashmap_string_string = {{"stanford", "stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		HashMap<GridLocation, string> hashmap_gridlocation_string = {{{3, 14}, "stanford"}};
		HashMap<Thing, string> hashmap_thing_string = {{(Thing){6, "purple"}, "stanford"}};
		HashMap<bool, GridLocation> hashmap_bool_gridlocation = {{false, {3, 14}}};
		HashMap<char, GridLocation> hashmap_char_gridlocation = {{'b', {3, 14}}};
		HashMap<double, GridLocation> hashmap_double_gridlocation = {{3.14159, {3, 14}}};
		HashMap<int, GridLocation> hashmap_int_gridlocation = {{106, {3, 14}}};
		HashMap<string, GridLocation> hashmap_string_gridlocation = {{"stanford", {3, 14}}};
		HashMap<GridLocation, GridLocation> hashmap_gridlocation_gridlocation = {{{3, 14}, {3, 14}}};
		HashMap<Thing, GridLocation> hashmap_thing_gridlocation = {{(Thing){6, "purple"}, {3, 14}}};
		HashMap<bool, Thing> hashmap_bool_thing = {{false, (Thing){6, "purple"}}};
		HashMap<char, Thing> hashmap_char_thing = {{'b', (Thing){6, "purple"}}};
		HashMap<double, Thing> hashmap_double_thing = {{3.14159, (Thing){6, "purple"}}};
		HashMap<int, Thing> hashmap_int_thing = {{106, (Thing){6, "purple"}}};
		HashMap<string, Thing> hashmap_string_thing = {{"stanford", (Thing){6, "purple"}}};
		HashMap<GridLocation, Thing> hashmap_gridlocation_thing = {{{3, 14}, (Thing){6, "purple"}}};
		HashMap<Thing, Thing> hashmap_thing_thing = {{(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		HashSet<bool> hashset_bool = {false, true};
		HashSet<char> hashset_char = {'b', 'B'};
		HashSet<double> hashset_double = {3.14159, -0.25};
		HashSet<int> hashset_int = {106, 1891};
		HashSet<string> hashset_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		HashSet<GridLocation> hashset_gridlocation = {{3, 14}};
		HashSet<Thing> hashset_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
}

