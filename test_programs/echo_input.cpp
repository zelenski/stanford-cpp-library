// This program echoes its stdin to stdout.

#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main() {
	string input;
	while (getline(cin, input)) {
		cout << "read input line: " << input << endl;
	}
	return 0;
}
