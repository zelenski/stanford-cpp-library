# @author Jeremy Barenholtz 2020, created
# @author Julie Zelenski, updated 2020-2023
#
# Debugging helpers for the CS106B classes (Vector, Stack, Set, Map, etc.)
# Some code adopted from stdtypes.py distributed with Qt Creator
from dumper import Children, SubItem
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

    putPairItem(d, i, key, value, 'key', 'value', compact=True)


def add_set_elem(d, i, key, value):
    """Adds an element of a set to the debugger display."""

    # Our Sets stored as map of key,val pairs where val is always true
    # ignore this value and don't display
    d.putSubItem('-', key)


# Currently only used by Vector
def add_indexed_elem(d, i, size, value):
    """Adds an element of an indexed container to the debugger display."""

    d.putSubItem(i, value)


def add_pq_elem(d, i, size, pq_entry):  # pq_entry struct
    """Adds an element of a priority queue to the debugger display."""

    priority = pq_entry['priority']
    value = pq_entry['value']
    putPairItem(d, i, value, priority, 'value', 'priority', compact=False)


def add_stack_elem(d, i, size, value):
    """Adds an element of a stack to the debugger display."""

    if i == size - 1:
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


# This function takes in an extra `ncols` parameter. Look at `qdump__Grid` for
# an example of how you can add parameters to these functions.
def add_grid_elem(d, i, size, value, ncols):
    """Adds an element of a grid to the debugger display."""

    name = f"[{i//ncols}][{i%ncols}]"
    d.putSubItem(name, value)


# The existing Dumper.putPairItem is somewhat broken, we replace with our own
# version that works correctly for our paired collections (Map/HashMap/PriorityQueue)
# The "compact" form uses val1 as top-level summary, makes val2 a single child of val1
# non-compact has blank summary, val1 & val2 are both children of top-level
# only attempts compact if val1 has simple display. Mostly this would translate to having
# no children (and thus not needed expansion) but that info is hard to come by in a reliable
# way. The code below accepts primitive types and makes special case for other types
# know to have simple display (GridLocation/GridLocationRange and std::string)

basic_types = ['std__basic_string', 'std____1__basic_string', 'std____cxx11__basic_string']
STR_TYPES = basic_types + [s.replace('basic_', '') for s in basic_types]

def is_std_string(d, type):
    nsStrippedType = d.stripNamespaceFromType(type.name).replace('::', '__')
    return nsStrippedType in STR_TYPES

def has_simple_display(d, type):
    return type.isSimpleType() or type.name in ['GridLocation', 'GridLocationRange'] or is_std_string(d, type)

def putPairItem(d, i, val1, val2, name1, name2, compact):
    compact = compact and has_simple_display(d, val1.type)
    with SubItem(d, i):
        d.putExpandable()
        if d.isExpanded():
            with Children(d):
                if not compact: d.putSubItem(f"{name1}", val1)
                d.putSubItem(f"{name2}", val2)
        if compact:
            d.putField('key', name1)
            d.putItem(val1)
        else:
            d.putField('key', '...')
            d.putNoType()
            d.putEmptyValue()

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
    vector_or_deque_helper(d, value, elem_fn=partial(add_grid_elem, ncols=cols))


def location_str(value):
    row = value['row'].integer()
    col = value['col'].integer()
    return f"r{row}c{col}"


def put_all_fields(d, value):
    d.putExpandable()  # Show the rest of the fields as usual
    if d.isExpanded():
        with Children(d):
            d.putFields(value)


def qdump__GridLocation(d, value):
    """Display Stanford GridLocation on debugger."""

    makeExpandable = False
    d.putValue(location_str(value))
    if makeExpandable: put_all_fields(d, value)


def qdump__GridLocationRange(d, value):
    """Display Stanford GridLocationRange on debugger."""

    makeExpandable = False
    start = location_str(value['_start'])
    end = location_str(value['_end'])
    d.putValue(f"{start} .. {end}")
    if makeExpandable: put_all_fields(d, value)


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


def qdump__Bit(d, value):
    """Display Huffman Bit on debugger."""
    bit = value['_value'].integer()
    bitstr = "1" if bit else "0"
    d.putValue(bitstr)


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
    """Dumps the internal vector for Vector, Stack, PriorityQueue, and Grid.
       Adapted from qdumpHelper__std__vector__libstdcxx,  qdumpHelper__std__vector__libcxx"""

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
            with Children(d, size, maxNumChild=1000,
                    childType=inner_type):
                for i in d.childRange():
                    q = start + int(i / 8)
                    with SubItem(d, i):
                        # std::vector<bool> stores elements as special
                        # bit-array, so we read each bit and convert from
                        # {0, 1} -> {false, true}
                        val = (int(d.extractPointer(q)) >> (i % 8)) & 1
                        d.putValue(val != 0)
        else:
            d.checkIntType(start)
            d.checkIntType(size)
            addr_base = start
            inner_size = inner_type.size()
            d.putNumChild(size)
            with Children(d, size, inner_type, None, maxNumChild=1000,
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
    """Dumps the deque for containers of bools or Queue for libc++.
       Adapted from qdumpHelper__std__deque__libcxx in stdtypes.py"""

    mptr, mfirst, mbegin, mend, start, size = value.split("pppptt")
    d.check(0 <= size and size <= 1000 * 1000 * 1000)
    d.putItemCount(size)
    if d.isExpanded():
        innerType = value.type[0]
        innerSize = innerType.size()
        ptrSize = d.ptrSize()
        bufsize = (4096 // innerSize) if innerSize < 256 else 16
        with Children(d, size, maxNumChild=1000, childType=innerType):
            for i in d.childRange():
                k, j = divmod(start + i, bufsize)
                base = d.extractPointer(mfirst + k * ptrSize)
                value = d.createValue(base + j * innerSize, innerType)
                elem_fn(d, i, size, value)


def deque_helper_libstd(d, value, elem_fn):
    """Dumps the deque for containers of bools or Queue for libstdc++.
        Adapted from qdumpHelper__std__deque__libstdcxx in stdtypes.py"""

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
        with Children(d, size, maxNumChild=1000, childType=innerType):
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
    """Wrapper for dumping unordered maps."""

    if is_lib_cpp(value):
        unordered_map_helper_libcpp(d, value, elem_fn)
    else:
        unordered_map_helper_libstd(d, value, elem_fn)


def unordered_map_helper_libcpp(d, value, elem_fn):
    """Dumps the unordered_map for HashSet and HashMap for libc++.
       Adapted from qdump__std____1__unordered_map in libcpp_stdtypes.py"""

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

        with Children(d, size, maxNumChild=1000):  # JDZ removed childType=value.type[0],
            for (i, pair) in zip(d.childRange(), traverse_list(curr)):
                elem_fn(d, i, pair[0], pair[1])


def unordered_map_helper_libstd(d, value, elem_fn):
    """Dumps the unordered_map for HashSet and HashMap for libstdc++.
       Adapted from qdump__std__unordered_map in stdtypes.py"""

    try:
        # gcc ~= 4.7
        size = value["_M_element_count"].integer()
        start = value["_M_before_begin"]["_M_nxt"]
    except:
        try:
            # libc++ (Mac)
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
        if d.isMsvcTarget():
            typeCode = 'pp@{%s}@{%s}' % (keyType.name, valueType.name)
            p = d.extractPointer(start)
        else:
            typeCode = 'p@{%s}@{%s}' % (keyType.name, valueType.name)
            p = start.pointer()
        with Children(d, size):
            for i in d.childRange():
                if d.isMsvcTarget():
                    p, _, _, key, _, val = d.split(typeCode, p)
                else:
                    p, _, key, _, val = d.split(typeCode, p)
                elem_fn(d, i, key, val)

def map_helper(d, value, elem_fn):
    """Wrapper for dumping maps."""

    if is_lib_cpp(value):
        map_helper_libcpp(d, value, elem_fn)
    else:
        map_helper_libstd(d, value, elem_fn)


def map_helper_libcpp(d, value, elem_fn):
    """Dumps the internal map for Set or Map for libc++.
       Adapted from qdump__std____1__map in libcpp_stdtypes.py"""

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

        with Children(d, size, maxNumChild=1000):
            for (i, pair) in zip(d.childRange(), in_order_traversal(head)):
                elem_fn(d, i, pair[0], pair[1])


def map_helper_libstd(d, value, elem_fn):
    """Dumps the internal map for Set or Map for libstdc++.
       Adapted from qdump__std__map in stdtypes.py"""

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
def is_lib_cpp(value):
    """Returns whether the class is from libc++."""
    return value.type.name.startswith('std::__1')

