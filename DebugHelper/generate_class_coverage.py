#! /usr/bin/env python3
import copy
from typing import List, Optional, Any


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
    'string'
]

basic_values = {
    'bool': ['false', 'true'],
    'char': [f"'{c}'" for c in 'abc'],
    'float': [str(f) for f in [1e-5, 1.37, 3.14159]],
    'int': [str(i) for i in range(3)],
    'string': [f'"{s}"' for s in ['bananas', 'ice cream', 'nachos']]
}

includes = [c.lower() + '.h' for c in containers]


def generate_source_code(depth: int = 1) -> str:
    src = []
    src += [f'#include "{f}"' for f in includes]
    src += ['using namespace std;', '']
    src += ['int main() {']
    src += [
        '\t'
        + ' '.join([str(t), t.varname(), '=', create_init_list_str(t) + ';'])
        for t in generate_all_types(depth)
    ]
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
