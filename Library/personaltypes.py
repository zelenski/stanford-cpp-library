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
# Some code adopted from stdtypes.py ditributed with Qt Creator
# Tweaked some by Julie Zelenski for Fall quarter 2020


# These utility functions are shared across collection types

def add_map_elem(d, i, key, value):
    d.putPairItem(None, (key, value), 'key', 'value')

def add_set_elem(d, i, key, value):
    # our Sets stored as map of key,val pairs where val is always true
    # ignore this value and don't display
    d.putSubItem('-', key)

def add_indexed_elem(d, i, size, value):
    d.putSubItem(i, value)

def add_stack_elem(d, i, size, value):
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
    if size == 1:
        name = 'front/back'
    elif i == 0:
        name = 'front'
    elif i == size - 1:
        name = 'back'
    else:
        name = '-'
    d.putSubItem(name, value)


def qdump__Set(d, value):
    """Display Stanford Set on debugger."""

    # Grab the internal data from the Stanford Set > Stanford MapType > std::map
    value = value['_map']['_elements']
    map_helper(d, value, elem_fn=add_set_elem)


def qdump__stanfordcpplib__collections__GenericSet(d, value):
    """Display Stanford Set or HashSet when dumped as GenericSet (windows)."""

    # Grab the internal data from the Stanford Set > Stanford MapType > std::map
    # or std::unordered_map for HashSets

    value = value['_map']['_elements']
    if "unordered" in value.type.name:
        unordered_map_helper(d, value, elem_fn=add_set_elem)
        cls = "HashSet"
    else:
        map_helper(d, value, elem_fn=add_set_elem)
        cls = "Set"
    innerType = value.type[0].name
    d.putBetterType('%s<%s>' % (cls, innerType))


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

    def add_pq_elem(d, i, size, raw):
        priority = raw['priority']
        value = raw['value']
        d.putPairItem(None, (value, priority), 'value', 'priority')

    vector_helper(d, value, elem_fn=add_pq_elem)


def qdump__Deque(d, value):
    """Display Stanford Deque on debugger."""

    # Grab the internal data from Deque > std::deque
    value = value['_elements']
    deque_helper(d, value, elem_fn=add_queue_elem)


def qdump__Map(d, value):
    """Display Stanford Map on debugger."""

    # Grab the internal data from the Stanford Map > std::map
    value = value['_elements']
    map_helper(d, value, elem_fn=add_map_elem)


def qdump__Vector(d, value):
    """Display Stanford Vector on debugger."""

    # Grab the internal data from the Stanford Vector > std::vector
    value = value['_elements']
    vector_or_deque_helper(d, value, elem_fn=add_indexed_elem)


def qdump__Grid(d, value):
    """Display Stanford Grid on debugger."""

    rows = value['_rowCount'].integer()
    cols = value['_columnCount'].integer()

    # Grab the internal data from the Stanford Grid > Stanford Vector > std::vector
    value = value['_elements']['_elements']

    def add_grid_elem(d, i, size, val):
        name = '[%d, %d]' % (i // cols, i % cols)
        d.putSubItem(name, val)

    vector_or_deque_helper(d, value, elem_fn=add_grid_elem)


def qdump__HashMap(d, value):
    """Display Stanford HashMap on debugger."""

    # Grab the internal map from the Stanford HashMap > std::unordered_map
    value = value['_elements']
    unordered_map_helper(d, value, elem_fn=add_map_elem)


def qdump__HashSet(d, value):
    """Display Stanford HashSet on debugger."""

    # Grab the internal map from the Stanford HashMap > std::unordered_map
    value = value['_map']['_elements']
    unordered_map_helper(d, value, elem_fn=add_set_elem)


def vector_or_deque_helper(d, value, elem_fn):
    # If the inner type is bool, container is actually a std::deque, not vector
    innerType = value.type[0]
    if innerType.name == 'bool':
        deque_helper(d, value, elem_fn)
    else:
        vector_helper(d, value, elem_fn)


def vector_helper(d, value, elem_fn):
    """Dumps the internal vector for Vector, Stack, PriorityQueue, and Grid."""

    innerType = value.type[0]
    isBool = innerType.name == 'bool'

    # Check if compiled with libstdc++ or libc++
    isLibCpp = not value.type.name.startswith('std::vector')

    if isBool:
        if isLibCpp:
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
        if isLibCpp:
            start = value["__begin_"].pointer()
            finish = value["__end_"].pointer()
            alloc = value["__end_cap_"].pointer()
        else:
            start = value["_M_start"].pointer()
            finish = value["_M_finish"].pointer()
            alloc = value["_M_end_of_storage"].pointer()
        size = int((finish - start) / innerType.size())
        d.check(finish <= alloc)
        if size > 0:
            d.checkPointer(start)
            d.checkPointer(finish)
            d.checkPointer(alloc)

    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        if isBool:
            if d.isExpanded():
                with dumper.Children(d, size, maxNumChild=10000, childType=innerType):
                    for i in d.childRange():
                        q = start + int(i / 8)
                        with dumper.SubItem(d, i):
                            # JDZ value is garbage (but this code path unused??)
                            val = (((int(d.extractPointer(q)) >> (i % 8)) & 1) != 0)
                            d.putValue(val)
        else:
            maxNumChild = 1000 * 1000
            d.checkIntType(start)
            d.checkIntType(size)
            addrBase = start
            innerSize = innerType.size()
            d.putNumChild(size)
            with dumper.Children(d, size, innerType, None, maxNumChild,
                      addrBase=addrBase, addrStep=innerSize):
                for i in d.childRange():
                    value = d.createValue(addrBase + i * innerSize, innerType)
                    elem_fn(d, i, size, value)


def deque_helper(d, value, elem_fn):
    # Check if compiled with libstdc++ or libc++
    if value.type.name.startswith('std::deque'):
        deque_helper_libstd(d, value, elem_fn)
    else:
        deque_helper_libcpp(d, value, elem_fn)


def deque_helper_libcpp(d, value, elem_fn):
    """Dumps the internal deque for Stack<bool> or Queue."""

    innerType = value.type[0]
    innerSize = innerType.size()
    mptr, mfirst, mbegin, mend, start, size = value.split("pppptt")
    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        ptrSize = d.ptrSize()
        bufsize = (4096 // innerSize) if innerSize < 256 else 16
        with dumper.Children(d, size, maxNumChild=2000, childType=innerType):
            for i in d.childRange():
                k, j = divmod(start + i, bufsize)
                base = d.extractPointer(mfirst + k * ptrSize)
                value = d.createValue(base + j * innerSize, innerType)
                elem_fn(d, i, size, value)


def deque_helper_libstd(d, value, elem_fn):
    innerType = value.type[0]
    innerSize = innerType.size()
    bufsize = 1
    if innerSize < 512:
        bufsize = 512 // innerSize

    (mapptr, mapsize, startCur, startFirst, startLast, startNode,
     finishCur, finishFirst, finishLast, finishNode) = value.split("pppppppppp")

    size = bufsize * ((finishNode - startNode) // d.ptrSize() - 1)
    size += (finishCur - finishFirst) // innerSize
    size += (startLast - startCur) // innerSize

    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        with dumper.Children(d, size, maxNumChild=2000, childType=innerType):
            pcur = startCur
            plast = startLast
            pnode = startNode
            for i in d.childRange():
                value = d.createValue(pcur, innerType)
                elem_fn(d, i, size, value)
                pcur += innerSize
                if pcur == plast:
                    newnode = pnode + d.ptrSize()
                    pfirst = d.extractPointer(newnode)
                    plast = pfirst + bufsize * d.ptrSize()
                    pcur = pfirst
                    pnode = newnode


def unordered_map_helper(d, value, elem_fn):
    if value.type.name.startswith('std::unordered_map'):
        unordered_map_helper_libstd(d, value, elem_fn)
    else:
        unordered_map_helper_libcpp(d, value, elem_fn)


def unordered_map_helper_libcpp(d, value, elem_fn):
    """Dumps the internal unordered_map for HashSet and HashMap."""

    (size, _) = value["__table_"]["__p2_"].split("pp")
    d.putItemCount(size)

    keyType = value.type[0]
    valueType = value.type[1]
    pairType = value.type[4][0]

    if d.isExpanded():
        curr = value["__table_"]["__p1_"].split("pp")[0]

        def traverse_list(node):
            while node:
                (next_, _, pad, pair) = d.split("pp@{%s}" % (pairType.name), node)
                yield pair.split("{%s}@{%s}" % (keyType.name, valueType.name))[::2]
                node = next_

        with dumper.Children(d, size, childType=value.type[0], maxNumChild=1000):
            for (i, pair) in zip(d.childRange(), traverse_list(curr)):
                elem_fn(d, i, pair[0], pair[1])


def unordered_map_helper_libstd(d, value, elem_fn):
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
        keyType = value.type[0]
        valueType = value.type[1]
        typeCode = 'p@{%s}@{%s}' % (keyType.name, valueType.name)
        p = start.pointer()
        with Children(d, size):
            for i in d.childRange():
                p, pad, key, pad, val = d.split(typeCode, p)
                elem_fn(d, i, key, val)


def map_helper(d, value, elem_fn):
    """Dumps the internal map for Set or Map."""
    if value.type.name.startswith('std::map'):
        map_helper_libstd(d, value, elem_fn)
    else:
        map_helper_libcpp(d, value, elem_fn)


def map_helper_libcpp(d, value, elem_fn):
    try:
        (proxy, head, size) = value.split("ppp")
        d.check(0 <= size and size <= 100 * 1000 * 1000)

    # Sometimes there is extra data at the front. Don't know why at the moment.
    except RuntimeError:
        (junk, proxy, head, size) = value.split("pppp")
        d.check(0 <= size and size <= 100 * 1000 * 1000)

    d.putItemCount(size)

    if d.isExpanded():
        keyType = value.type[0]
        valueType = value.type[1]
        pairType = value.type[3][0]

        def in_order_traversal(node):
            (left, right, parent, color, pad, pair) = d.split("pppB@{%s}" % (pairType.name), node)

            if left:
                for res in in_order_traversal(left):
                    yield res

            yield pair.split("{%s}@{%s}" % (keyType.name, valueType.name))[::2]

            if right:
                for res in in_order_traversal(right):
                    yield res

        with dumper.Children(d, size, maxNumChild=1000):
            for (i, pair) in zip(d.childRange(), in_order_traversal(head)):
                elem_fn(d, i, pair[0], pair[1])


def map_helper_libstd(d, value, elem_fn):
    # stuff is actually (color, pad) with 'I@', but we can save cycles/
    (compare, stuff, parent, left, right) = value.split('ppppp')
    size = value["_M_t"]["_M_impl"]["_M_node_count"].integer()
    d.check(0 <= size and size <= 100 * 1000 * 1000)
    d.putItemCount(size)

    if d.isExpanded():
        keyType = value.type[0]
        valueType = value.type[1]
        with Children(d, size, maxNumChild=1000):
            node = value["_M_t"]["_M_impl"]["_M_header"]["_M_left"]
            nodeSize = node.dereference().type.size()
            typeCode = "@{%s}@{%s}" % (keyType.name, valueType.name)
            for i in d.childRange():
                (pad1, key, pad2, value) = d.split(typeCode, node.pointer() + nodeSize)
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
