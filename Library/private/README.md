JDZ: here is where I am adding notes about library structure/design, 
This is while reorganizing code, fall quarter 2019

__Startup sequence, threads, main()__
Student writes a main() function that appears to them to be the entry point for program, but this is sleight-of-hand. Student code is compiled with a #define to rename main to studentMain and linking with our library supplies a main() that call studentMain() wrapped in necessary startup/teardown code.
- What happens in main wrapper?
    1. establishes the original thread as the "qtgui thread"
    2. initializes QT application
    3. initialize graphical console window (if #include console.h)
    4. create second thread to run studentMain, concurrently run QT application event loop on qtgui thread
    5. shutdown at end of studentMain and/or close console window
- Threads
    A lot of the QT application/graphics interaction has to run on the qtgui thread, see GThread::runOnQtGuiThread for that dispatch


__Static variables (initialization, constructors)__
- No guarantees about order of execution code is run for static initializers. See private/static.h for macros that provide declaration/access to static variable to ensure initializer run exactly once on first use of variable.

