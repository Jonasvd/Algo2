#ifndef __ZOEKBOOM_TESTS_H
#define __ZOEKBOOM_TESTS_H

#include <string>
#include <sstream>

#include "zoekboom14.h"
#include "zoekboom14Iterator.h"

using namespace std;

void print_test_result(string naam_test, bool result, int& failed) {
	if (!result) failed++;
	string res = result ? "OK" : "FAIL !";
	cout << res << "\t" << naam_test << endl;
}
void print_test_result(int& failed) {
	cout << "   " << failed << " tests failed" << endl;
}

class Zoekboom14Tests{
public:
	
	// ************** GENERAL TEST METHODS ********************

	// Zou moeten opgeroepen worden met de wortels van de bomen
	template <class Sleutel, class Data>
	bool areEqual(const Zoekboom<Sleutel,Data>& first, const Zoekboom<Sleutel, Data>& second) {
		// beide wortels leeg of ingevuld?
		if (first->ouder && !second->ouder) return false;

		// sleutel gelijk?
		if (first->sleutel != second->sleutel) return false;
		// data gelijk?
		if (first->data != second->data) return false;

		// beide wel of geen linkerkind?
		if (first->links && !second->links) return false;
		// beide wel of geen rechterkind?
		if (first->rechts && !second->rechts) return false;

		// als beide linkerkind hebben => wijst naar ouder?
		if (first->links && second->links)
			if ((first->links->ouder != first.get()) || (second->links->ouder != second.get())) return false;
		// als beide rechterkind hebben => wijst naar ouder?
		if (first->rechts && second->rechts)
			if ((first->rechts->ouder != first.get()) || (second->rechts->ouder != second.get())) return false;

		// als je hier komt zijn beide knopen waar de bomen naar wijzen ok
		// als je kind(eren) hebt => recursief testen
		bool linksOk = true;
		bool rechtsOk = true;
		if (first->links) linksOk = areEqual(first->links, second->links);
		if (first->rechts) rechtsOk = areEqual(first->rechts, second->rechts);

		// als er een probleem is met één van beide zal linksOk of rechtsOk false zijn
		return linksOk && rechtsOk;
	}

	bool areEqual_test1() {
		Zoekboom<int, string> een(1, "een");
		Zoekboom<int, string> eenKopie(1, "een");
		Zoekboom<int, string> twee(2, "twee");

		return areEqual(een, een)
			&& areEqual(een, eenKopie)
			&& !areEqual(een, twee);
	}
	bool areEqual_test2() {
		Zoekboom<int, string> aveve(1, "Aveve");
		Zoekboom<int, string> brico(2, "Brico");
		Zoekboom<int, string> gamma(3, "Gamma");
		Zoekboom<int, string> wortel(4, "Wortel");
		Zoekboom<int, string> ouderNa(5, "OuderNa");
		ouderNa->voegtoe_links(move(aveve));
		ouderNa->voegtoe_rechts(move(brico));
		wortel->voegtoe_links(move(ouderNa));
		wortel->voegtoe_rechts(move(gamma));

		Zoekboom<int, string> aveve2(1, "Aveve");
		Zoekboom<int, string> brico2(2, "Brico");
		Zoekboom<int, string> gamma2(3, "Gamma");
		Zoekboom<int, string> wortel2(4, "Wortel");
		Zoekboom<int, string> ouderNa2(5, "OuderNa");
		ouderNa2->voegtoe_links(move(aveve2));
		ouderNa2->voegtoe_rechts(move(brico2));
		wortel2->voegtoe_links(move(ouderNa2));
		wortel2->voegtoe_rechts(move(gamma2));

		return areEqual(wortel, wortel2);
	}

	// ************** SPECIFIC TESTS **************************
	bool move1() {
		Zoekboom<int, string> boom3(88, "Koen");
		Zoekboom<int, string> boom4 = move(boom3);
		Zoekboom<int, string> boom4test(88, "Koen");

		return areEqual(boom4, boom4test)
			&& !boom3;
	}
	bool move2() {
		Zoekboom<int, string> boom5(88, "Koen");
		Zoekboom<int, string> boom6;
		boom6 = move(boom5);
		Zoekboom<int, string> boom6test(88, "Koen");

		return areEqual(boom6, boom6test)
			&& !boom5;
	}
	bool copy1() {
		Zoekboom<int, string> g(3, "grandparent");
		Zoekboom<int, string> p(1, "parent");
		Zoekboom<int, string> c(2, "child");
		p->voegtoe_rechts(move(c));
		g->voegtoe_links(move(p));

		Zoekboom<int, string> kopie(g);

		return areEqual(g, kopie) && g && kopie;
	}
	bool copy2() {
		Zoekboom<int, string> g(3, "grandparent");
		Zoekboom<int, string> p(1, "parent");
		Zoekboom<int, string> c(2, "child");
		p->voegtoe_rechts(move(c));
		g->voegtoe_links(move(p));

		Zoekboom<int, string> kopie = g;

		return areEqual(g, kopie) && g && kopie;
	}
	bool geefDiepte1() {
		Zoekboom<int, string> g(3, "grandparent");
		Zoekboom<int, string> p(1, "parent");
		Zoekboom<int, string> c(2, "child");
		p->voegtoe_rechts(move(c));
		g->voegtoe_links(move(p));

		return g.geefDiepte() == 2;
	}
	bool geefDiepte2() {
		Zoekboom<int, string> g(3, "grandparent");

		return g.geefDiepte() == 0;
	}
	bool roteer1() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(3, "b");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		Zoekboom<int, string> a2(1, "a");
		Zoekboom<int, string> b2(3, "b");
		Zoekboom<int, string> c2(2, "c");
		Zoekboom<int, string> g2(5, "g");
		Zoekboom<int, string> p2(4, "p");
		p2->voegtoe_links(move(b2));
		p2->voegtoe_rechts(move(g2));
		c2->voegtoe_links(move(a2));
		c2->voegtoe_rechts(move(p2));

		p.roteer(false);
		return areEqual(p, c2);
	}
	bool roteer2() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(3, "b");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		Zoekboom<int, string> a2(1, "a");
		Zoekboom<int, string> b2(3, "b");
		Zoekboom<int, string> c2(2, "c");
		Zoekboom<int, string> g2(5, "g");
		Zoekboom<int, string> p2(4, "p");
		p2->voegtoe_links(move(b2));
		p2->voegtoe_rechts(move(g2));
		c2->voegtoe_links(move(a2));
		c2->voegtoe_rechts(move(p2));

		c2.roteer(true);
		return areEqual(p, c2);
	}
	bool roteer3() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		Zoekboom<int, string> a2(1, "a");
		Zoekboom<int, string> c2(2, "c");
		Zoekboom<int, string> g2(5, "g");
		Zoekboom<int, string> p2(4, "p");
		p2->voegtoe_rechts(move(g2));
		c2->voegtoe_links(move(a2));
		c2->voegtoe_rechts(move(p2));

		c2.roteer(true);
		return areEqual(p, c2);
	}
	bool roteer4() {
		Zoekboom<int, string> p(1, "p");
		Zoekboom<int, string> c(2, "c");
		p->voegtoe_rechts(move(c));

		Zoekboom<int, string> p2(1, "p");
		Zoekboom<int, string> c2(2, "c");
		c2->voegtoe_links(move(p2));

		p.roteer(true);
		return areEqual(p, c2);
	}
	bool maakEvenwichtig() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(3, "b");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		b->voegtoe_links(move(c));
		p->voegtoe_links(move(b));
		g->voegtoe_links(move(p));

		Zoekboom<int, string> a2(1, "a");
		Zoekboom<int, string> b2(3, "b");
		Zoekboom<int, string> c2(2, "c");
		Zoekboom<int, string> g2(5, "g");
		Zoekboom<int, string> p2(4, "p");
		p2->voegtoe_rechts(move(g2));
		c2->voegtoe_links(move(a2));
		b2->voegtoe_links(move(c2));
		b2->voegtoe_rechts(move(p2));
		g.maakEvenwichtig();

		return areEqual(g, b2);
	}
	bool maakOnevenwichtig() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(3, "b");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		Zoekboom<int, string> a2(1, "a");
		Zoekboom<int, string> b2(3, "b");
		Zoekboom<int, string> c2(2, "c");
		Zoekboom<int, string> g2(5, "g");
		Zoekboom<int, string> p2(4, "p");
		c2->voegtoe_links(move(a2));
		b2->voegtoe_links(move(c2));
		p2->voegtoe_links(move(b2));
		g2->voegtoe_links(move(p2));

		p.maakOnevenwichtig();
		return areEqual(p, g2);
	}
	bool iterator1() { // setToFirst en setToLast
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		b->voegtoe_rechts(move(c));
		b->voegtoe_links(move(a));

		ZoekboomIterator<int, string> it(b);

		stringstream streamFirst;
		stringstream streamLast;
		streamFirst << *(it.setToFirst());
		streamLast << *(it.setToLast());

		return streamFirst.str() == "sleutel: 1 data: a"
			&& streamLast.str() == "sleutel: 3 data: c";
	}
	bool iterator2() { // b als wortel, a en c als kinderen
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		b->voegtoe_rechts(move(c));
		b->voegtoe_links(move(a));

		stringstream stream;

		bool hasMore = true;
		for (ZoekboomIterator<int, string> it(b); hasMore; hasMore = it.setToNext()) {
			stream << *it << endl;
		}

		string expected = "sleutel: 1 data: a\nsleutel: 2 data: b\nsleutel: 3 data: c\n";

		return stream.str() == expected;
	}
	bool iterator3() { // gelinkte lijst met a als wortel
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		b->voegtoe_rechts(move(c));
		a->voegtoe_rechts(move(b));

		stringstream stream;

		bool hasMore = true;
		for (ZoekboomIterator<int, string> it(a); hasMore; hasMore = it.setToNext()) {
			stream << *it << endl;
		}

		string expected = "sleutel: 1 data: a\nsleutel: 2 data: b\nsleutel: 3 data: c\n";

		return stream.str() == expected;
	}
	bool iterator4() { // gelinkte lijst met c als wortel
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		b->voegtoe_links(move(a));
		c->voegtoe_links(move(b));

		stringstream stream;

		bool hasMore = true;
		for (ZoekboomIterator<int, string> it(c); hasMore; hasMore = it.setToNext()) {
			stream << *it << endl;
		}

		string expected = "sleutel: 1 data: a\nsleutel: 2 data: b\nsleutel: 3 data: c\n";

		return stream.str() == expected;
	}
	bool repOK1() {
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(3, "b");
		Zoekboom<int, string> c(2, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		return p.repOK();
	}
	bool repOK2() { // volgorde niet ok
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		return !p.repOK();
	}
	bool repOK3() { // volgorde niet ok
		Zoekboom<int, string> a(1, "a");
		Zoekboom<int, string> b(2, "b");
		Zoekboom<int, string> c(3, "c");
		Zoekboom<int, string> g(5, "g");
		Zoekboom<int, string> p(4, "p");
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		p->links->ouder = NULL;

		return !p.repOK();
	}

	// ************** CALL TESTS ******************************
	void zoekboom14_tests() {
		cout << endl;

		int failed = 0;
		print_test_result("areEqual1", areEqual_test1(), failed);
		print_test_result("areEqual2", areEqual_test2(), failed);
		print_test_result("move1", move1(), failed);
		print_test_result("move2", move2(), failed);
		print_test_result("copy1", copy1(), failed);
		print_test_result("copy2", copy2(), failed);
		print_test_result("geefDiepte1", geefDiepte1(), failed);
		print_test_result("geefDiepte2", geefDiepte2(), failed);
		print_test_result("roteer1", roteer1(), failed);
		print_test_result("roteer2", roteer2(), failed);
		print_test_result("roteer3", roteer3(), failed);
		print_test_result("roteer4", roteer4(), failed);
		print_test_result("maakEvenwichtig", maakEvenwichtig(), failed);
		print_test_result("maakOnevenwichtig", maakOnevenwichtig(), failed);
		print_test_result("iterator1", iterator1(), failed);
		print_test_result("iterator2", iterator2(), failed);
		print_test_result("iterator3", iterator3(), failed);
		print_test_result("iterator4", iterator4(), failed);
		print_test_result("repOK1", repOK1(), failed);
		print_test_result("repOK2", repOK2(), failed);
		print_test_result("repOK3", repOK3(), failed);

		cout << endl;
		print_test_result(failed);
	}

};
#endif