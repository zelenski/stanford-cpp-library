// This program produces infinite amounts of output, yipes!

#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
	int i = 0;
	while (true) {
		cout << "LOL infinite output " << i << endl;
		i++;
		// sleep(1);
	}
	return 0;
}
