# ############################################################################
# #
# # Copyright (C) 2016 The Qt Company Ltd.
# # Contact: https://www.qt.io/licensing/
# #
# # This file is part of Qt Creator.
# #
# # Commercial License Usage
# # Licensees holding valid commercial Qt licenses may use this file in
# # accordance with the commercial license agreement provided with the
# # Software or, alternatively, in accordance with the terms contained in
# # a written agreement between you and The Qt Company. For licensing terms
# # and conditions see https://www.qt.io/terms-conditions. For further
# # information use the contact form at https://www.qt.io/contact-us.
# #
# # GNU General Public License Usage
# # Alternatively, this file may be used under the terms of the GNU
# # General Public License version 3 as published by the Free Software
# # Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
# # included in the packaging of this file. Please review the following
# # information to ensure the GNU General Public License requirements will
# # be met: https://www.gnu.org/licenses/gpl-3.0.html.
# #
# ############################################################################

# # This is a place to add your own dumpers for testing purposes.
# # Any contents here will be picked up by GDB, LLDB, and CDB based
# # debugging in Qt Creator automatically.

# # NOTE: This file will get overwritten when updating Qt Creator.
# #
# # To add dumpers that don't get overwritten, copy this file here
# # to a safe location outside the Qt Creator installation and
# # make this location known to Qt Creator using the Debugger >
# # Locals & Expressions > Extra Debugging Helpers setting.

# # Example to display a simple type
# # template<typename U, typename V> struct MapNode
# # {
# #     U key;
# #     V data;
# # }
# #
# # def qdump__MapNode(d, value):
# #    d.putValue("This is the value column contents")
# #    d.putNumChild(2)
# #    if d.isExpanded():
# #        with Children(d):
# #            # Compact simple case.
# #            d.putSubItem("key", value["key"])
# #            # Same effect, with more customization possibilities.
# #            with SubItem(d, "data")
# #                d.putItem("data", value["data"])

# # Check http://doc.qt.io/qtcreator/creator-debugging-helpers.html
# # for more details or look at qttypes.py, stdtypes.py, boosttypes.py
# # for more complex examples.

import dumper
from dumper import Children, SubItem

# ######################## Your code below #######################

# @author Jeremy Barenholtz 2020
#
# Debugging helpers for the CS106B classes (Vector, Stack, Set, Map, etc.)
# Some code adopted from stdtypes.py distributed with Qt Creator
# Tweaked some by Julie Zelenski for Fall quarter 2020

from functools import partial

#############################
# Element Display Functions #
#############################

# These functions, named `add_<type>_elem`, control how the elements of each
# class get displayed in the debug window.
#
# You should only adjust these functions if you wish to change the display
# format. To understand the relevant code, fire up the debugger to view your
# desired class, and look at the related add element function here; you should
# be able to see how the code corresponds to what is displayed.


def add_map_elem(d, i, key, value):
    """Adds an element of a map to the debugger display."""

    d.putPairItem(None, (key, value), 'key', 'value')


def add_set_elem(d, i, key, value):
    """Adds an element of a set to the debugger display."""

    # Our Sets stored as map of key,val pairs where val is always true
    # ignore this value and don't display
    d.putSubItem('-', key)


# Currently only used by Vector
def add_indexed_elem(d, i, size, value):
    """Adds an element of an indexed container to the debugger display."""

    d.putSubItem(i, value)


def add_pq_elem(d, i, size, raw):
    """Adds an element of a priority queue to the debugger display."""

    priority = raw['priority']
    value = raw['value']
    d.putPairItem(None, (value, priority), 'value', 'priority')


def add_stack_elem(d, i, size, value):
    """Adds an element of a stack to the debugger display."""

    if size == 1:
        name = 'bottom/top'
    elif i == 0:
        name = 'bottom'
    elif i == size - 1:
        name = 'top'
    else:
        name = '-'

    d.putSubItem(name, value)


def add_queue_elem(d, i, size, value):
    """Adds an element of a queue to the debugger display."""

    if size == 1:
        name = 'front/back'
    elif i == 0:
        name = 'front'
    elif i == size - 1:
        name = 'back'
    else:
        name = '-'

    d.putSubItem(name, value)


# This function takes in an extra `cols` parameter. Look at `qdump__Grid` for
# an example of how you can add parameters to these functions.
def add_grid_elem(d, i, size, value, cols):
    """Adds an element of a grid to the debugger display."""

    name = '[%d, %d]' % (i // cols, i % cols)
    d.putSubItem(name, value)


#####################################
# Stanford Library Dumper Functions #
#####################################

# These functions, named `qdump__<class>`, are called with the underlying data
# for a variable of the corresponding type.
#
# You can change the displayed type with `d.putBetterType('<new_type>')`. See
# `qdump__stanfordcpplib__collections__GenericSet` for an example of that.
# All formatting of the elements should be set using the add element functions
# above.


def qdump__Set(d, value):
    """Display Stanford Set on debugger."""

    # Grab the internal data from Set > MapType > std::map
    value = value['_map']['_elements']
    map_helper(d, value, elem_fn=add_set_elem)


def qdump__stanfordcpplib__collections__GenericSet(d, value):
    """Display Stanford Set or HashSet when dumped as GenericSet (Windows)."""

    # Grab the internal data from the Set > MapType > std::map
    # or std::unordered_map for HashSets

    value = value['_map']['_elements']
    if "unordered" in value.type.name:
        unordered_map_helper(d, value, elem_fn=add_set_elem)
        cls = "HashSet"
    else:
        map_helper(d, value, elem_fn=add_set_elem)
        cls = "Set"

    inner_type = value.type[0].name
    d.putBetterType('%s<%s>' % (cls, inner_type))


def qdump__Stack(d, value):
    """Display Stanford Stack on debugger."""

    # Grab the internal data from Stack > Vector > std::{vector, deque}
    value = value['_elements']['_elements']
    vector_or_deque_helper(d, value, elem_fn=add_stack_elem)


def qdump__Queue(d, value):
    """Display Stanford Queue on debugger."""

    # Grab the internal data from Queue > Deque > std::deque
    value = value['_elements']['_elements']
    deque_helper(d, value, elem_fn=add_queue_elem)


def qdump__PriorityQueue(d, value):
    """Display Stanford PriorityQueue on debugger."""

    # Grab the internal data from PriorityQueue > Vector > std::vector
    value = value['_heap']['_elements']
    vector_helper(d, value, elem_fn=add_pq_elem)


def qdump__Deque(d, value):
    """Display Stanford Deque on debugger."""

    # Grab the internal data from Deque > std::deque
    value = value['_elements']
    deque_helper(d, value, elem_fn=add_queue_elem)


def qdump__Map(d, value):
    """Display Stanford Map on debugger."""

    # Grab the internal data from Map > std::map
    value = value['_elements']
    map_helper(d, value, elem_fn=add_map_elem)


def qdump__Vector(d, value):
    """Display Stanford Vector on debugger."""

    # Grab the internal data from Vector > std::vector
    value = value['_elements']
    vector_or_deque_helper(d, value, elem_fn=add_indexed_elem)


def qdump__Grid(d, value):
    """Display Stanford Grid on debugger."""

    rows = value['_rowCount'].integer()
    cols = value['_columnCount'].integer()

    # Grab the internal data from the Grid > Vector > std::vector
    value = value['_elements']['_elements']
    vector_or_deque_helper(d, value, elem_fn=partial(add_grid_elem, cols=cols))


def qdump__HashMap(d, value):
    """Display Stanford HashMap on debugger."""

    # Grab the internal map from HashMap > std::unordered_map
    value = value['_elements']
    unordered_map_helper(d, value, elem_fn=add_map_elem)


def qdump__HashSet(d, value):
    """Display Stanford HashSet on debugger."""

    # Grab the internal map from HashMap > std::unordered_map
    value = value['_map']['_elements']
    unordered_map_helper(d, value, elem_fn=add_set_elem)


####################
# Helper Functions #
####################


def vector_or_deque_helper(d, value, elem_fn):
    """Wrapper for dumping vectors or deque in the bool case."""

    # If the inner type is bool, container is actually a std::deque, not vector
    inner_type = value.type[0]
    if inner_type.name == 'bool':
        deque_helper(d, value, elem_fn)
    else:
        vector_helper(d, value, elem_fn)


def vector_helper(d, value, elem_fn):
    """Dumps the internal vector for Vector, Stack, PriorityQueue, and Grid."""

    inner_type = value.type[0]
    is_bool = inner_type.name == 'bool'

    # Check if compiled with libstdc++ or libc++
    class_is_lib_cpp = is_lib_cpp(value)

    if is_bool:
        if class_is_lib_cpp:
            start = value["__begin_"].pointer()
            size = value["__size_"].integer()  # JDZ extract integer value
            alloc = size
        else:
            start = value["_M_start"]["_M_p"].pointer()
            soffset = value["_M_start"]["_M_offset"].integer()
            finish = value["_M_finish"]["_M_p"].pointer()
            foffset = value["_M_finish"]["_M_offset"].integer()
            alloc = value["_M_end_of_storage"].pointer()
            size = (finish - start) * 8 + foffset - soffset  # 8 is CHAR_BIT.
    else:
        if class_is_lib_cpp:
            start = value["__begin_"].pointer()
            finish = value["__end_"].pointer()
            alloc = value["__end_cap_"].pointer()
        else:
            start = value["_M_start"].pointer()
            finish = value["_M_finish"].pointer()
            alloc = value["_M_end_of_storage"].pointer()
        size = int((finish - start) / inner_type.size())
        d.check(finish <= alloc)
        if size > 0:
            d.checkPointer(start)
            d.checkPointer(finish)
            d.checkPointer(alloc)

    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        if is_bool:
            if d.isExpanded():
                with dumper.Children(d, size, maxNumChild=10000,
                        childType=inner_type):
                    for i in d.childRange():
                        q = start + int(i / 8)
                        with dumper.SubItem(d, i):
                            # std::vector<bool> stores elements as special
                            # bit-array, so we read each bit and convert from
                            # {0, 1} -> {false, true}
                            val = (int(d.extractPointer(q)) >> (i % 8)) & 1
                            d.putValue(val != 0)
        else:
            max_num_child = 1000 * 1000
            d.checkIntType(start)
            d.checkIntType(size)
            addr_base = start
            inner_size = inner_type.size()
            d.putNumChild(size)
            with dumper.Children(d, size, inner_type, None, max_num_child,
                      addrBase=addr_base, addrStep=inner_size):
                for i in d.childRange():
                    value = d.createValue(addr_base + i * inner_size,
                                          inner_type)
                    elem_fn(d, i, size, value)


def deque_helper(d, value, elem_fn):
    """Wrapper for dumping deques."""

    # Check if compiled with libstdc++ or libc++
    if is_lib_cpp(value):
        deque_helper_libcpp(d, value, elem_fn)
    else:
        deque_helper_libstd(d, value, elem_fn)


def deque_helper_libcpp(d, value, elem_fn):
    """Dumps the deque for containers of bools or Queue for libc++."""

    inner_type = value.type[0]
    inner_size = inner_type.size()
    mptr, mfirst, mbegin, mend, start, size = value.split("pppptt")
    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        ptr_size = d.ptrSize()
        buf_size = (4096 // inner_size) if inner_size < 256 else 16
        with dumper.Children(d, size, maxNumChild=2000, childType=inner_type):
            for i in d.childRange():
                k, j = divmod(start + i, buf_size)
                base = d.extractPointer(mfirst + k * ptr_size)
                value = d.createValue(base + j * inner_size, inner_type)
                elem_fn(d, i, size, value)


def deque_helper_libstd(d, value, elem_fn):
    """Dumps the deque for containers of bools or Queue for libstdc++."""

    inner_type = value.type[0]
    inner_size = inner_type.size()
    buf_size = 1
    if inner_size < 512:
        buf_size = 512 // inner_size

    (mptr, msize, start_cur, start_first, start_last, start_node, finish_cur,
     finish_first, finish_last, finish_node) = value.split("pppppppppp")

    size = buf_size * ((finish_node - start_node) // d.ptrSize() - 1)
    size += (finish_cur - finish_first) // inner_size
    size += (start_last - start_cur) // inner_size

    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        with dumper.Children(d, size, maxNumChild=2000, childType=inner_type):
            pcur = start_cur
            plast = start_last
            pnode = start_node
            for i in d.childRange():
                value = d.createValue(pcur, inner_type)
                elem_fn(d, i, size, value)
                pcur += inner_size
                if pcur == plast:
                    newnode = pnode + d.ptrSize()
                    pfirst = d.extractPointer(newnode)
                    plast = pfirst + buf_size * d.ptrSize()
                    pcur = pfirst
                    pnode = newnode


def unordered_map_helper(d, value, elem_fn):
    """Wrapper for dumping unordered maps."""

    if is_lib_cpp(value):
        unordered_map_helper_libcpp(d, value, elem_fn)
    else:
        unordered_map_helper_libstd(d, value, elem_fn)


def unordered_map_helper_libcpp(d, value, elem_fn):
    """Dumps the unordered_map for HashSet and HashMap for libc++."""

    (size, _) = value["__table_"]["__p2_"].split("pp")
    d.putItemCount(size)

    key_type = value.type[0]
    value_type = value.type[1]
    pair_type = value.type[4][0]

    if d.isExpanded():
        curr = value["__table_"]["__p1_"].split("pp")[0]

        def traverse_list(node):
            while node:
                (next_, _, pad, pair) = d.split("pp@{%s}" % (pair_type.name), node)
                yield pair.split("{%s}@{%s}" % (key_type.name, value_type.name))[::2]
                node = next_

        with dumper.Children(d, size, childType=value.type[0], maxNumChild=1000):
            for (i, pair) in zip(d.childRange(), traverse_list(curr)):
                elem_fn(d, i, pair[0], pair[1])


def unordered_map_helper_libstd(d, value, elem_fn):
    """Dumps the unordered_map for HashSet and HashMap for libstdc++."""

    try:
        # gcc ~= 4.7
        size = value["_M_element_count"].integer()
        start = value["_M_before_begin"]["_M_nxt"]
    except:
        try:
            # libc++ (Mac?)
            size = value["_M_h"]["_M_element_count"].integer()
            start = value["_M_h"]["_M_bbegin"]["_M_node"]["_M_nxt"]
        except:
            try:
                # gcc 4.9.1
                size = value["_M_h"]["_M_element_count"].integer()
                start = value["_M_h"]["_M_before_begin"]["_M_nxt"]
            except:
                # gcc 4.6.2
                size = value["_M_element_count"].integer()
                start = value["_M_buckets"].dereference()
                # FIXME: Pointer-aligned?
                d.putItemCount(size)
                # We don't know where the data is
                d.putNumChild(0)
                return

    d.putItemCount(size)
    if d.isExpanded():
        key_type = value.type[0]
        value_type = value.type[1]
        type_code = 'p@{%s}@{%s}' % (key_type.name, value_type.name)
        p = start.pointer()
        with Children(d, size):
            for i in d.childRange():
                p, pad, key, pad, val = d.split(type_code, p)
                elem_fn(d, i, key, val)


def map_helper(d, value, elem_fn):
    """Wrapper for dumping maps."""

    if is_lib_cpp(value):
        map_helper_libcpp(d, value, elem_fn)
    else:
        map_helper_libstd(d, value, elem_fn)


def map_helper_libcpp(d, value, elem_fn):
    """Dumps the internal map for Set or Map for libc++."""

    try:
        (proxy, head, size) = value.split("ppp")
        d.check(0 <= size and size <= 100 * 1000 * 1000)

    # JEB sometimes there is extra data at the front (?)
    except RuntimeError:
        (junk, proxy, head, size) = value.split("pppp")
        d.check(0 <= size and size <= 100 * 1000 * 1000)

    d.putItemCount(size)

    if d.isExpanded():
        key_type = value.type[0]
        value_type = value.type[1]
        pair_type = value.type[3][0]

        def in_order_traversal(node):
            (left, right, parent,
             color, pad, pair) = d.split("pppB@{%s}" % (pair_type.name), node)

            if left:
                for res in in_order_traversal(left):
                    yield res

            yield pair.split("{%s}@{%s}" %
                             (key_type.name, value_type.name))[::2]

            if right:
                for res in in_order_traversal(right):
                    yield res

        with dumper.Children(d, size, maxNumChild=1000):
            for (i, pair) in zip(d.childRange(), in_order_traversal(head)):
                elem_fn(d, i, pair[0], pair[1])


def map_helper_libstd(d, value, elem_fn):
    """Dumps the internal map for Set or Map for libstdc++."""

    # stuff is actually (color, pad) with 'I@', but we can save cycles
    (compare, stuff, parent, left, right) = value.split('ppppp')
    size = value["_M_t"]["_M_impl"]["_M_node_count"].integer()
    d.check(0 <= size and size <= 100 * 1000 * 1000)
    d.putItemCount(size)

    if d.isExpanded():
        key_type = value.type[0]
        value_type = value.type[1]
        with Children(d, size, maxNumChild=1000):
            node = value["_M_t"]["_M_impl"]["_M_header"]["_M_left"]
            node_size = node.dereference().type.size()
            type_code = "@{%s}@{%s}" % (key_type.name, value_type.name)
            for i in d.childRange():
                (pad1, key, pad2, value) = d.split(type_code, node.pointer()
                                                              + node_size)
                elem_fn(d, i, key, value)

                if node["_M_right"].pointer() == 0:
                    parent = node["_M_parent"]
                    while True:
                        if node.pointer() != parent["_M_right"].pointer():
                            break
                        node = parent
                        parent = parent["_M_parent"]
                    if node["_M_right"] != parent:
                        node = parent
                else:
                    node = node["_M_right"]
                    while True:
                        if node["_M_left"].pointer() == 0:
                            break
                        node = node["_M_left"]


def is_lib_cpp(value):
    """Returns whether the class is from libc++."""

    return value.type.name.startswith('std::__1')
