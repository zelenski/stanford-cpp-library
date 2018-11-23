// This program never terminates.

#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
	cout << "Going to sleep now ..." << endl;
	while (true) {
		sleep(1);
	}
	return 0;   // will never get here
}
