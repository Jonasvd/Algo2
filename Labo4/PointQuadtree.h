#ifndef _POINTQUADTREE_H
#define _POINTQUADTREE_H

#include <iostream>
#include <memory>

using namespace std;

template <class X, class Y>
class PointKnoop;

enum Positie { LO, LB, RO, RB };

// ************************************** POINT

template <class X, class Y>
class Point {
public:
	Point(int x, int y) : x(x), y(y) {};
	Positie Compare(const Point& point);
	bool operator==(const Point& other) const;
private:
	X x;
	Y y;
};

template <class X, class Y>
Positie Point<X,Y>::Compare(const Point<X,Y>& point) {
	int a = point.x < x ? 0 : 2; // 0x00 of 0x10
	int b = point.y < y ? 0 : 1; // 0x00 of 0x01
	return static_cast<Positie>(a | b);
}

template <class X, class Y>
bool Point<X,Y>::operator==(const Point<X,Y>& other) const {
	return x == other.x && y == other.y;
}

// ************************************** POINTQUADTREE

template <class X, class Y>
class PointQuadtree : public unique_ptr<PointKnoop<X,Y>> {
	using unique_ptr<PointKnoop<X,Y>>::unique_ptr;
public:
	PointQuadtree() : unique_ptr<PointKnoop<X,Y>>(nullptr) {};
	PointQuadtree(const Point<X,Y>& point) {
		*this = make_unique<PointKnoop<X, Y>>(point);
	}
	PointQuadtree<X, Y>* zoek(const Point<X, Y>& point);
	void voegToe(const Point<X, Y>& point);
	bool pointEquals(const Point<X, Y>& point);
	int diepte();
private:

};

template <class X, class Y>
PointQuadtree<X, Y>* PointQuadtree<X,Y>::zoek(const Point<X, Y>& point) {
	PointQuadtree<X, Y>* huidig = this;
	while (*huidig && !((*huidig)->point == point)) {
		Positie pos = (*huidig)->point.Compare(point);
		huidig = &(*huidig)->kinderen[pos];
	}
	return huidig;
}

// zoekOuder
//if ((*this)->point == point) return nullptr;
//PointQuadtree<X, Y>* ouder = this;
//Positie positie = (*ouder)->point.Compare(point);
//kindboom = (*ouder)->kinderen[positie];
//while (!(kindboom->point == point)) {
//	  ouder = kindboom;
//	  positie = (*ouder)->point.Compare(point);
//	  kindboom == (*ouder)->kinderen[positie];
//}
//return ouder;

template <class X, class Y>
void PointQuadtree<X,Y>::voegToe(const Point<X,Y>& point) {
	PointQuadtree<X, Y>* positie = zoek(point);

	// als hij al bestaat => stoppen
	if ((*positie)) return;

	//positie->reset(new PointKnoop<X,Y>(point));
	*positie = make_unique<PointKnoop<X, Y>>(point);
}

template <class X, class Y>
bool PointQuadtree<X,Y>::pointEquals(const Point<X,Y>& point) {
	return (*this)->point == point;
}

template <class X, class Y>
int PointQuadtree<X, Y>::diepte() {
	if (!*this) return 0;

	int maxdiepte = 0;
	for (int i = 0; i < 4; i++) {
		int diepte = (*this)->kinderen[i].diepte();
		if (diepte > maxdiepte)
			maxdiepte = diepte;
	}
	return maxdiepte + 1;
}

// ************************************* POINTKNOOP

template <class X, class Y>
class PointKnoop {
	friend class PointQuadtree<X, Y>;
	friend class PointQuadtreeTests;
public:
	PointKnoop(const Point<X,Y>& point) : point(point) {};
private:
	Point<X, Y> point;
	PointQuadtree<X,Y> kinderen[4];
};

#endif