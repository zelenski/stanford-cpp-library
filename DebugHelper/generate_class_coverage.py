#! /usr/bin/env python3
#
# Generate C++ code with 106B collection classes of various elem type
# Authored by Jeremy Barenholz & Julie Zelenski

import copy
from typing import List, Optional, Any

TYPEDEFS = '''
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
FIXED = '''
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
    Stack<GridLocation> maze_Stack_Loc = { {1, 2}, {1, 3}, {2, 3}, {2, 4}};
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


containers = [
    'Deque',
    'Grid',
    'HashMap',
    'HashSet',
    'Map',
    'PriorityQueue',
    'Queue',
    'Set',
    'Stack',
    'Vector'
]

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
    'char': [f"'{c}'" for c in 'abc'],
    'float': [str(f) for f in [1e-5, 1.37, 3.14159]],
    'int': [str(i) for i in range(3)],
    'string': [f'"{s}"' for s in ['bananas', 'ice cream', 'nachos']],
    'GridLocation': [f'{{{r}, {c}}}' for r,c in [(0,0), (2,4), (5,3)]],
    'Thing': [f'(Thing){{{len(s)}, "{s}"}}' for s in ['red', 'green', 'blue']]}

includes = [c.lower() + '.h' for c in containers] + ['bits.h']

def generate_source_code(depth: int=1) -> str:
    src = []
    src += [f'#include "{f}"' for f in includes]
    src += ['using namespace std;', '']
    src += TYPEDEFS.split('\n')
    src += ['int main() {']
    src += [
        '\t'
        + ' '.join([str(t), t.varname(), '=', create_init_list_str(t) + ';'])
        for t in generate_all_types(depth)
    ]
    src += FIXED.split('\n')
    src += ['', '\treturn 0;', '}']

    return '\n'.join(src)


def generate_all_types(depth: int = 1) -> List[Type]:
    if depth <= 0:
        return [Type(b) for b in basic_types]

    types = []
    for container in containers:
        for inner in generate_all_types(depth - 1):
            if container.endswith('Map'):
                for key in basic_types:
                    types.append(Type(inner, container, Type(key)))
            else:
                types.append(Type(inner, container))

    return types


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


if __name__ == '__main__':
    print(generate_source_code(depth=1))
