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

But, also quickly checked whether I can reproduce the issue here. Neither breaking in a function with that type nor pasting a lookup command of that type resulted in the time you have reported here.
The command I used to check this was:

!qtcreatorcdbext.script print(cdbext.lookupType('std::vector<unsigned __int64,std::allocator<unsigned __int64> >*',0).name())

This can be pasted into the Command field of the Debugger Log which can be enabled under View -> Views 0> Debugger Log while in debug mode.


If you want to further investigate the issue on your side you would need to compile the cdbextension yourself (remember to link against a debug version of python) and set debugPyCdbextModule in <creatorsource>\src\libs\qtcreatorcdbext\pycdbextmodule.h to true (sorry, I have not implemented a buildsystem switch for this). And if that doesn't give you any usable information you might want to attach directly to the cdb and step through the individual functions.


