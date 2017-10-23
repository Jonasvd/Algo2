#ifndef _FLESSENFABRIEK_TESTS_H
#define _FLESSENFABRIEK_TESTS_H

#include <iostream>
#include <vector>
#include <sstream>
#include "FlessenFabriek.h"

using namespace std;

bool areEqual(vector<int>& a, vector<int>& b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++){
		if (a[i] != b[i]) return false;
	}
	return true;
}
bool areEqual(Flessen& a, Flessen& b) {
	vector<int> gr_a;
	vector<int> gr_b;
	vector<int> aa_a;
	vector<int> aa_b;
	return areEqual(gr_a, gr_b) && areEqual(aa_a, aa_b);
}
bool areEqual(Oplossing& a, Oplossing& b) {
	if (a.gevonden != b.gevonden) return false;
	return areEqual(a.flessen, b.flessen);
}

class FlessenFabriekTests {
public:
	void runTests();
	void printResults() {
		cout << endl << "   " << _succeeded << " succeeded, " << _failed << " failed" << endl;
	}
private:
	int _succeeded = 0;
	int _failed = 0;
	void assert(bool result) {
		if (result) {
			_succeeded++;
			cout << "Test OK" << endl;
		}
		else {
			_failed++;
			cout << "Test FAILED" << endl;
		}
	}
	void tests();
};

void FlessenFabriekTests::runTests() {
	tests();
	printResults();
}
 // ******************************* TESTS *************************

bool constructor1() {
	vector<int> grootte = { 40, 50, 450, 460 };
	vector<int> aantal = { 10, 10, 10, 10 };
	FlessenFabriek fabriek(grootte, aantal);
	stringstream out;
	fabriek.schrijf(out);

	string expected = "FlessenFabriek:\n  groottes: \t40\t50\t450\t460\n  aantal: \t10\t10\t10\t10\n";

	return out.str() == expected;
}
bool flessen1() {
	vector<int> grootte = { 40, 50, 450, 460 };
	vector<int> aantal = { 10, 20, 30, 0 };
	Flessen flessen1(grootte, aantal);

	bool hw1 = flessen1.haalFlesWeg(50);
	bool hw2 = flessen1.haalFlesWeg(460);

	vector<int> gr;
	flessen1.groottes(gr);
	vector<int> aant;
	flessen1.aantal(aant);

	vector<int> expected_gr = {40, 50, 450, 460};
	vector<int> expected_aant = {10, 19, 30, 0};

	return hw1 && !hw2 && areEqual(gr, expected_gr) && areEqual(aant, expected_aant);
}
bool flessen2() {
	vector<int> grootte = { 40, 50, 450, 460 };
	vector<int> aantal = { 10, 20, 30, 0 };
	Flessen flessen1(grootte, aantal);

	Flessen flessen2 = flessen1;

	flessen1.haalFlesWeg(40);
	flessen2.haalFlesWeg(450);

	vector<int> gr1;
	flessen1.groottes(gr1);
	vector<int> gr2;
	flessen2.groottes(gr2);
	vector<int> aant1;
	flessen1.aantal(aant1);
	vector<int> aant2;
	flessen2.aantal(aant2);

	vector<int> exp_gr = {40, 50, 450, 460};
	vector<int> exp_aant1 = {9, 20, 450, 460};
	vector<int> exp_aant2 = {10, 20, 29, 0};

	return areEqual(gr1, exp_gr) && areEqual(gr2, exp_gr) && areEqual(gr2, exp_gr) && areEqual(aant2, exp_aant2);
}
bool zoekOplossing1() {
	vector<int> grootte = { 40 };
	vector<int> aantal = { 10 };
	FlessenFabriek fabriek(grootte, aantal);

	Oplossing o = fabriek.vulVolume(30);

	return !o.gevonden;
}
bool zoekOplossing2() {
	vector<int> grootte = { 40 };
	vector<int> aantal = { 10 };
	FlessenFabriek fabriek(grootte, aantal);

	Oplossing o = fabriek.vulVolume(160);

	vector<int> exp_aa = { 6 };
	Flessen exp_flessen(grootte, exp_aa);
	Oplossing expected(exp_flessen);

	return areEqual(o, expected);
}
bool zoekOplossing3() {
	vector<int> grootte = { 40, 50 };
	vector<int> aantal = { 10, 10 };
	FlessenFabriek fabriek(grootte, aantal);

	Oplossing o = fabriek.vulVolume(80);

	vector<int> exp_aa = { 8, 10 };
	Flessen exp_flessen(grootte, exp_aa);
	Oplossing expected(exp_flessen);

	return areEqual(o, expected);
}
bool zoekOplossing4() {
	vector<int> grootte = { 90, 40, 10 };
	vector<int> aantal = { 10, 10, 10 };
	FlessenFabriek fabriek(grootte, aantal);

	Oplossing o = fabriek.vulVolume(120);

	vector<int> exp_aa = { 10, 7, 10 };
	Flessen exp_flessen(grootte, exp_aa);
	Oplossing expected(exp_flessen);

	return areEqual(o, expected);
}
bool zoekOplossing5() {
	vector<int> grootte = { 10, 50 };
	vector<int> aantal = { 10, 10 };
	FlessenFabriek fabriek(grootte, aantal);

	Oplossing o = fabriek.vulVolume(100);

	vector<int> exp_aa = { 10, 8 };
	Flessen exp_flessen(grootte, exp_aa);
	Oplossing expected(exp_flessen);

	return areEqual(o, expected);
}

void FlessenFabriekTests::tests() {
	assert(constructor1());
	assert(flessen1());
	assert(flessen2());
	assert(zoekOplossing1());
	assert(zoekOplossing2());
	assert(zoekOplossing3());
	assert(zoekOplossing4());
	assert(zoekOplossing5());
}

#endif