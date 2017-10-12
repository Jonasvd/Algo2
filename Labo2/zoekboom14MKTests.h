#ifndef __ZoekboomMK_TESTS_H
#define __ZoekboomMK_TESTS_H

#include <string>
#include <sstream>

#include "Zoekboom14MK.h"
#include "Zoekboom14MKIterator.h"

using namespace std;

void print_test_result(string naam_test, bool result, int& failed) {
	if (!result) failed++;
	string res = result ? "OK" : "FAIL !";
	cout << res << "\t" << naam_test << endl;
}
void print_test_result(int& failed) {
	cout << "   " << failed << " tests failed" << endl;
}

class Zoekboom14MKTests{
public:
	
	// ************** GENERAL TEST METHODS ********************

	// Zou moeten opgeroepen worden met de wortels van de bomen
	template <class Sleutel, class Data>
	bool areEqual(const ZoekboomMK<Sleutel, Data>& first, const ZoekboomMK<Sleutel, Data>& second) {
		// beide wortels leeg of ingevuld?
		if (first->ouder && !second->ouder) return false;

		// sleutel gelijk?
		if (first->sleutel != second->sleutel) return false;
		// data gelijk?
		if (first->data != second->data) return false;
		// ADDED
		// kleur gelijk?
		if (first->kleur != second->kleur) return false;

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
		ZoekboomMK<int, string> een(1, "een", rood);
		ZoekboomMK<int, string> eenKopie(1, "een", rood);
		ZoekboomMK<int, string> twee(2, "twee", zwart);
		ZoekboomMK<int, string> tweeAnderKleur(2, "twee", rood);

		return areEqual(een, een)
			&& areEqual(een, eenKopie)
			&& !areEqual(een, twee)
			&& !areEqual(twee, tweeAnderKleur);
	}

	// ************** SPECIFIC TESTS **************************
	bool repOK1() {
		ZoekboomMK<int, string> a(1, "a", rood);
		ZoekboomMK<int, string> b(3, "b", rood);
		ZoekboomMK<int, string> c(2, "c", zwart);
		ZoekboomMK<int, string> g(5, "g", zwart);
		ZoekboomMK<int, string> p(4, "p", zwart);
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		return p.repOK();
	}
	bool repOK2() {
		ZoekboomMK<int, string> a(1, "a", rood);
		ZoekboomMK<int, string> b(3, "b", rood);
		ZoekboomMK<int, string> c(2, "c", rood);
		ZoekboomMK<int, string> g(5, "g", rood);
		ZoekboomMK<int, string> p(4, "p", zwart);
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		return !p.repOK();
	}
	bool controleerZwarteDiepte1() {
		ZoekboomMK<int, string> a(1, "a", rood);
		ZoekboomMK<int, string> b(3, "b", rood);
		ZoekboomMK<int, string> c(2, "c", rood);
		ZoekboomMK<int, string> g(5, "g", rood);
		ZoekboomMK<int, string> p(4, "p", zwart);
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		int diepte;
		return p.controleerZwarteDiepte(diepte) && diepte == 1;
	}
	bool controleerZwarteDiepte2() {
		ZoekboomMK<int, string> a(1, "a", rood);
		ZoekboomMK<int, string> b(3, "b", rood);
		ZoekboomMK<int, string> c(2, "c", rood);
		ZoekboomMK<int, string> g(5, "g", rood);
		ZoekboomMK<int, string> p(4, "p", rood);
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		int diepte;
		return p.controleerZwarteDiepte(diepte) && diepte == 0;
	}
	bool controleerZwarteDiepte3() {
		ZoekboomMK<int, string> a(1, "a", rood);
		ZoekboomMK<int, string> b(3, "b", rood);
		ZoekboomMK<int, string> c(2, "c", zwart);
		ZoekboomMK<int, string> g(5, "g", zwart);
		ZoekboomMK<int, string> p(4, "p", zwart);
		c->voegtoe_links(move(a));
		c->voegtoe_rechts(move(b));
		p->voegtoe_links(move(c));
		p->voegtoe_rechts(move(g));

		int diepte;
		return p.controleerZwarteDiepte(diepte) && diepte == 2;
	}

	// ************** CALL TESTS ******************************
	void Zoekboom14MK_tests() {
		cout << endl;

		int failed = 0;
		print_test_result("areEqual1", areEqual_test1(), failed);
		print_test_result("repOK1", repOK1(), failed);
		print_test_result("repOK2", repOK2(), failed);
		print_test_result("controleerZwarteDiepte1", controleerZwarteDiepte1(), failed);
		print_test_result("controleerZwarteDiepte2", controleerZwarteDiepte2(), failed);
		print_test_result("controleerZwarteDiepte3", controleerZwarteDiepte3(), failed);

		cout << endl;
		print_test_result(failed);
	}

};
#endif