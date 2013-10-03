/*
 * File: main.cpp
 * --------------
 * This file defines a default version of the Main function that takes
 * the argc and argv arguments.  This function must be defined in its
 * own module to ensure that it is loaded only if the client doesn't
 * supply one.
 */

int Main(int, char *[]) {
   extern int Main();
   return Main();
}

