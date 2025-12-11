#include "shared.h"
using namespace std;

void stanford_tree() {
	{
		Map<bool, bool> map_bool_bool = {{false, false}, {true, true}};
		Map<char, bool> map_char_bool = {{'b', false}, {'B', true}};
		Map<double, bool> map_double_bool = {{3.14159, false}, {-0.25, true}};
		Map<int, bool> map_int_bool = {{106, false}, {1891, true}};
		Map<string, bool> map_string_bool = {{"stanford", false}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", true}};
		Map<GridLocation, bool> map_gridlocation_bool = {{{3, 14}, false}};
		Map<Thing, bool> map_thing_bool = {{(Thing){6, "purple"}, false}};
		Map<bool, char> map_bool_char = {{false, 'b'}, {true, 'B'}};
		Map<char, char> map_char_char = {{'b', 'b'}, {'B', 'B'}};
		Map<double, char> map_double_char = {{3.14159, 'b'}, {-0.25, 'B'}};
		Map<int, char> map_int_char = {{106, 'b'}, {1891, 'B'}};
		Map<string, char> map_string_char = {{"stanford", 'b'}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 'B'}};
		Map<GridLocation, char> map_gridlocation_char = {{{3, 14}, 'b'}};
		Map<Thing, char> map_thing_char = {{(Thing){6, "purple"}, 'b'}};
		Map<bool, double> map_bool_double = {{false, 3.14159}, {true, -0.25}};
		Map<char, double> map_char_double = {{'b', 3.14159}, {'B', -0.25}};
		Map<double, double> map_double_double = {{3.14159, 3.14159}, {-0.25, -0.25}};
		Map<int, double> map_int_double = {{106, 3.14159}, {1891, -0.25}};
		Map<string, double> map_string_double = {{"stanford", 3.14159}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", -0.25}};
		Map<GridLocation, double> map_gridlocation_double = {{{3, 14}, 3.14159}};
		Map<Thing, double> map_thing_double = {{(Thing){6, "purple"}, 3.14159}};
		Map<bool, int> map_bool_int = {{false, 106}, {true, 1891}};
		Map<char, int> map_char_int = {{'b', 106}, {'B', 1891}};
		Map<double, int> map_double_int = {{3.14159, 106}, {-0.25, 1891}};
		Map<int, int> map_int_int = {{106, 106}, {1891, 1891}};
		Map<string, int> map_string_int = {{"stanford", 106}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 1891}};
		Map<GridLocation, int> map_gridlocation_int = {{{3, 14}, 106}};
		Map<Thing, int> map_thing_int = {{(Thing){6, "purple"}, 106}};
		Map<bool, string> map_bool_string = {{false, "stanford"}, {true, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<char, string> map_char_string = {{'b', "stanford"}, {'B', "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<double, string> map_double_string = {{3.14159, "stanford"}, {-0.25, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<int, string> map_int_string = {{106, "stanford"}, {1891, "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<string, string> map_string_string = {{"stanford", "stanford"}, {"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}};
		Map<GridLocation, string> map_gridlocation_string = {{{3, 14}, "stanford"}};
		Map<Thing, string> map_thing_string = {{(Thing){6, "purple"}, "stanford"}};
		Map<bool, GridLocation> map_bool_gridlocation = {{false, {3, 14}}};
		Map<char, GridLocation> map_char_gridlocation = {{'b', {3, 14}}};
		Map<double, GridLocation> map_double_gridlocation = {{3.14159, {3, 14}}};
		Map<int, GridLocation> map_int_gridlocation = {{106, {3, 14}}};
		Map<string, GridLocation> map_string_gridlocation = {{"stanford", {3, 14}}};
		Map<GridLocation, GridLocation> map_gridlocation_gridlocation = {{{3, 14}, {3, 14}}};
		Map<Thing, GridLocation> map_thing_gridlocation = {{(Thing){6, "purple"}, {3, 14}}};
		Map<bool, Thing> map_bool_thing = {{false, (Thing){6, "purple"}}};
		Map<char, Thing> map_char_thing = {{'b', (Thing){6, "purple"}}};
		Map<double, Thing> map_double_thing = {{3.14159, (Thing){6, "purple"}}};
		Map<int, Thing> map_int_thing = {{106, (Thing){6, "purple"}}};
		Map<string, Thing> map_string_thing = {{"stanford", (Thing){6, "purple"}}};
		Map<GridLocation, Thing> map_gridlocation_thing = {{{3, 14}, (Thing){6, "purple"}}};
		Map<Thing, Thing> map_thing_thing = {{(Thing){6, "purple"}, (Thing){6, "purple"}}};
		BREAKPOINT;
	}
	{
		Set<bool> set_bool = {false, true};
		Set<char> set_char = {'b', 'B'};
		Set<double> set_double = {3.14159, -0.25};
		Set<int> set_int = {106, 1891};
		Set<string> set_string = {"stanford", "abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		Set<GridLocation> set_gridlocation = {{3, 14}};
		Set<Thing> set_thing = {(Thing){6, "purple"}};
		BREAKPOINT;
	}
}

