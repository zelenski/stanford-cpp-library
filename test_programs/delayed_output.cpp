// This program prints output after a delay.

#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
	sleep(3);
	cout << "Tada delayed output!" << endl;
	return 0;
}
