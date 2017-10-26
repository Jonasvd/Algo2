#include "PointQuadtreeTests.h"
#include "Spiraal.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	PointQuadtreeTests tests;
	tests.runTests();

	cout << "grafiek reeks 1 vullen..." << endl;
	maakGrafiek(50000);
	cout << "grafiek reeks 2 vullen..." << endl;
	maakGrafiek_random(50000);

	cout << endl << "DONE" << endl;
	string end;
	cin >> end;
}