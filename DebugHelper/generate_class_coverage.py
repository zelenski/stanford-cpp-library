#! /usr/bin/env python3
#
# Generate C++ code with 106B collection classes of various elem type
# Authored by Jeremy Barenholz & Julie Zelenski

import copy
from typing import List, Optional, Any

TYPEDEFS = '''
using namespace std;

#if defined(__APPLE__)
#define BREAKPOINT __builtin_debugtrap()
#elif defined(_WIN32)
#define BREAKPOINT __debugbreak()
#endif

struct Thing {
    int num;
    string label;
};
int hashCode(const Thing& t) {
    return t.num;
}
bool operator ==(const Thing& t1, const Thing& t2) {
    return t1.num == t2.num && t1.label == t2.label;
}
bool operator <(const Thing& t1, const Thing& t2) {
    return stanfordcpplib::collections::compareTo(t1.num, t2.num, t1.label, t2.label) < 0;
}

struct Node {
    char letter;
    Node *left, *right;
};

'''

# manually test combos for certain assignments
ASSIGN_USES = '''
void assign_uses() {
    Node *ptr = new Node {'A', nullptr, nullptr};
    PriorityQueue<Node *> huff_PQ_NodePtr {{4.0, ptr}};
    Vector<Node *> huff_Vector_NodePtr = {ptr, ptr, nullptr, nullptr};
    Set<Node *> huff_Set_NodePtr = {ptr, nullptr};
    Map<char, Node *> huff_Map_NodePtr = {{'a', ptr}, {'b', ptr}, {'c', nullptr}};

    Vector<Bit> huff_Vector_Bit = {0, 1, 1, 1, 0};
    Map<char, Queue<Bit>> huff_Map_Queue_Bit {{'a', {0, 1, 1, 1}}, {'b', {1, 0}}};
    Map<char, Vector<Bit>> huff_Map_Vector_Bit = {{'a', {1, 1, 1, 0}}, {'b', {0, 0}}};
    Map<string, Set<string>> siteIndex_Map_Set =  {{"home", {"the", "end"}}, {"faq", {"what", "why"}}};

    Grid<char> boggle_Grid = {{'a', 'n', 'r'}, {'p', 'e', 't'}, {'c', 'o', 'b'}};
    GridLocationRange redistrict_Range = boggle_Grid.locations();
    GridLocation maze_Loc(2, 4);
    Stack<GridLocation> maze_Stack_Loc = { {1, 2}, {3, 4}, {5, 6}, {7, 8}};
    BREAKPOINT;
}
'''

MAIN_PROGRAM = '''
int main() {
    stanford_linear();
    stanford_hash();
    stanford_tree();
    stanford_other();
    assign_uses();
    return 0;
}
'''

class Type:
    def __init__(
        self,
        inner: 'Type',
        container: Optional['Type'] = None,
        first: Optional['Type'] = None
    ) -> None:
        self.inner = inner
        self.container = container
        self.first = first

    def varname(self) -> str:
        if not self.container and not self.first:
            return f'{self.inner}'
        elif not self.first:
            return f'{self.container}_{self.inner.varname()}'
        else:
            return (f'{self.container}_{self.first.varname()}'
                    f'_{self.inner.varname()}')

    def __str__(self) -> str:
        if not self.container and not self.first:
            return f'{self.inner}'
        elif not self.first:
            return f'{self.container}<{self.inner}>'
        else:
            return f'{self.container}<{self.first}, {self.inner}>'

    def __repr__(self) -> str:
        return f'Type({str(self)})'

    def __eq__(self, other) -> bool:
        return str(self) == str(other)

    def __hash__(self) -> int:
        return hash(str(self))

linear_containers = ['Deque', 'Grid', 'Queue','Stack','Vector', ]
hash_containers = ['HashMap', 'HashSet', ]
tree_containers = ['Map', 'Set', ]
other_containers = [ 'PriorityQueue',]
all_stanford = linear_containers + hash_containers + tree_containers + other_containers

basic_types = [
    'bool',
    'char',
    'float',
    'int',
    'string',
    'GridLocation',
    'Thing'
]

basic_values = {
    'bool': ['false', 'true'],
    'char': [f"'{c}'" for c in 'abcABC'],
    'float': [str(f) for f in [3.14159, 137.0, -.5]],
    'int': [str(i*2 + 1) for i in range(5)],
    'string': [f'"{s}"' for s in ['tree', 'cardinal', 'Stanford', 'abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ']],
    'GridLocation': [f'{{{r}, {c}}}' for r,c in [(6,6), (1,9), (5,3)]],
    'Thing': [f'(Thing){{{len(s)}, "{s}"}}' for s in ['red', 'green', 'blue']]}

def create_init_list_str(type: Type) -> str:
    if type.container == 'Grid':
        n_rows = 3
        inner_parts = ['{' + f"{', '.join(basic_values[type.inner])}" + '}'
                       for i in range(n_rows)]
    elif type.container == 'PriorityQueue' and not type.first:
        new_type = copy.copy(type)
        new_type.first = Type('int')
        return create_init_list_str(new_type)
    elif type.first:
        n = min(len(basic_values[type.inner]), len(basic_values[type.first]))
        inner_parts = [
            '{'
            + f'{basic_values[type.first][i]}, {basic_values[type.inner][i]}'
            + '}'
            for i in range(n)
        ]
    else:
        inner_parts = basic_values[type.inner]

    return '{' + f"{', '.join(inner_parts)}" + '}'

def types_for_container(outer) -> List[Type]:
    types = []
    for inner in [Type(b) for b in basic_types]:
        if outer.endswith('Map'):
            for key in basic_types:
                types.append(Type(inner, outer, Type(key)))
        else:
            types.append(Type(inner, outer))
    return types

def source_for_all(fn_name, containers) -> str:
    lines = [f"void {fn_name}() {{"]
    for outer in containers:
        lines += ['\t{'] + [
        '\t\t'
        + ' '.join([str(t), t.varname(), '=', create_init_list_str(t) + ';'])
        for t in types_for_container(outer)] + ['\t\tBREAKPOINT;\n\t}']
    lines += ["}\n\n"]
    return '\n'.join(lines)

def generate_program() -> str:
    includes = [c.lower() + '.h' for c in all_stanford + ['bits']]
    program = '\n'.join([f'#include "{f}"' for f in includes])
    program += TYPEDEFS
    program += source_for_all("stanford_linear", linear_containers)
    program += source_for_all("stanford_hash", hash_containers)
    program += source_for_all("stanford_tree", tree_containers)
    program += source_for_all("stanford_other", other_containers)
    return program + ASSIGN_USES + MAIN_PROGRAM

if __name__ == '__main__':
    print(generate_program())
