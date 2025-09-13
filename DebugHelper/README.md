Use generate.py to generate C++ program to instantiate 106B collections with various element types.
Compile and run under debugger and inspect variables to observe that debug helpers are properly installed and working.

Reminders of useful documentation about the debug pretty printers

- Qt Creator.app/Contents/Resources/debugger/README.txt
- Qt Creator.app/Contents/Resources/debugger/personaltypes.py
- https://doc.qt.io/qtcreator/creator-debugging-helpers.html


stanfordtypes.py
----------------
Linear types (Vector, Deque, Stack, Queue) under the hood are using std::vector or std::deque, same for Grid and PriorityQueue too
These collections tend to either all work or all broken, find the underlying fix will usually corrects all

Tree types (Map, Set)
Keep in mind that Set<T> is actually Map<T,bool>
Dissection of key/value blob is where most of the problems have been

Hash types (HashMap, HashSet)
Keep in mind that HashSet<T> is actually HashMap<T,bool>
Dissection of key/value blob is where most of the problems have been


Bug reports
https://bugreports.qt.io/browse/QTCREATORBUG-18287
From https://forum.qt.io/topic/142639/qt-creator-debugger-is-slow

!qtcreatorcdbext.script print(cdbext.lookupType('std::vector<unsigned __int64,std::allocator<unsigned __int64> >*',0).name())

This can be pasted into the Command field of the Debugger Log which can be enabled under View -> Views 0> Debugger Log while in debug mode.


Fall 2025
---------
In past Qt did not compute the alignment for a struct correctly, just guessed 8.
As of 6.9, it is trying to compute max alignment from fields
but it iterates over get_fields_array() when it should instead b get_members_array()
this causes it to ignore base class members, and for class such as std::string this is deadly.. ugh.
I put in a patch into stanfordtypes.py to do the correct calculation where we depend on alignment
such as extract key/value pair for map and hashmap, this brought map and hashmap back to working.
Needed a followup to also fix alignment for struct which is typedef
