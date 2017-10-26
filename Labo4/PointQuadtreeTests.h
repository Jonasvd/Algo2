#ifndef _POINTQUADTREE_TESTS_H
#define _POINTQUADTREE_TESTS_H

#include "PointQuadtree.h"
#include <iostream>

using namespace std;

class PointQuadtreeTests {
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

void PointQuadtreeTests::runTests() {
	tests();
	printResults();
}

// *************************************** TESTS

bool pointCompare1() {
	Point<int,int> point(5, 7);

	Point<int, int> lb(1, 10);
	if (point.Compare(lb) != LB) return false;

	Point<int, int> lo(1, 2);
	if (point.Compare(lo) != LO) return false;

	Point<int, int> rb(8, 10);
	if (point.Compare(rb) != RB) return false;

	Point<int, int> ro(8, 2);
	if (point.Compare(ro) != RO) return false;

	return true;
}
bool pointEquals() {
	Point<int, int> a(5, 7);
	Point<int, int> b(5, 7);
	Point<int, int> c(5, 8);
	return a == b && !(a == c);
}
bool constructor1() {
	Point<int,int> point(5, 7);
	PointQuadtree<int,int> pqt(point);
	PointQuadtree<int,int> pqt2;

	return pqt && !pqt2;
}
bool zoek1() {
	Point<int, int> point(5, 7);
	PointQuadtree<int, int> pqt(point);
	PointQuadtree<int, int>* qpt_ptr = pqt.zoek(point);

	return qpt_ptr->pointEquals(point);
}
bool zoek2() {
	Point<int, int> point(5, 7);
	PointQuadtree<int, int> pqt;
	PointQuadtree<int, int>* qpt_ptr = pqt.zoek(point);

	return !*qpt_ptr;
}
bool zoek3() {
	Point<int, int> point(5, 7);
	PointQuadtree<int, int> pqt(point);

	Point<int, int> point2(6, 12);
	PointQuadtree<int, int>* qpt_ptr = pqt.zoek(point2);

	return !*qpt_ptr;
}
bool voegToe1() {
	Point<int, int> point(5, 7);
	PointQuadtree<int, int> pqt(point);

	Point<int, int> point2(8, 12);
	pqt.voegToe(point2);

	return pqt.diepte() == 2;
}
bool voegToe2() {
	PointQuadtree<int, int> pqt;

	Point<int, int> point2(8, 12);
	pqt.voegToe(point2);

	return pqt.diepte() == 1;
}

void PointQuadtreeTests::tests() {
	assert(pointCompare1());
	assert(pointEquals());
	assert(constructor1());
	assert(zoek1());
	assert(zoek2());
	assert(zoek3());
	assert(voegToe1());
	assert(voegToe2());

}

#endif

