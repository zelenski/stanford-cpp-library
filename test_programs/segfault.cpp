// This program produces a segfault.

#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
	cout << "Going to segfault now ..." << endl;
	
	int* p = nullptr;
	cout << "oops this will crash" << *p << endl;
	
	return 0;   // will never get here
}
