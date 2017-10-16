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
	void vulmettestboom1(ZoekboomMK<int, string>& w) {
		// zoekboom volgens eerste figuur in opgave
		// virtuele
		ZoekboomMK<int, string> virt(0, "null", zwart);
		// onderste niveau
		ZoekboomMK<int, string> b4(4, "vier", rood);
		//b4->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b4->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		ZoekboomMK<int, string> b8(8, "acht", rood);
		//b8->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b8->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		ZoekboomMK<int, string> b11(11, "elf", zwart);
		//b11->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b11->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		ZoekboomMK<int, string> b13(13, "dertien", zwart);
		//b13->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b13->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		ZoekboomMK<int, string> b16(16, "zestien", zwart);
		//b16->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b16->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		ZoekboomMK<int, string> b20(20, "twintig", zwart);
		//b20->voegtoe_links(move(ZoekboomMK<int, string>(virt)));
		//b20->voegtoe_rechts(move(ZoekboomMK<int, string>(virt)));
		// tweede niveau
		ZoekboomMK<int, string> b2(2, "twee", zwart);
		ZoekboomMK<int, string> b6(6, "zes", zwart);
		b6->voegtoe_links(move(b4));
		b6->voegtoe_rechts(move(b8));
		ZoekboomMK<int, string> b12(12, "twaalf", zwart);
		b12->voegtoe_links(move(b11));
		b12->voegtoe_rechts(move(b13));
		ZoekboomMK<int, string> b18(18, "achttien", zwart);
		b18->voegtoe_links(move(b16));
		b18->voegtoe_rechts(move(b20));
		// eerste niveau
		ZoekboomMK<int, string> b3(3, "drie", zwart);
		b3->voegtoe_links(move(b2));
		b3->voegtoe_rechts(move(b6));
		ZoekboomMK<int, string> b15(15, "vijftien", rood);
		b15->voegtoe_links(move(b12));
		b15->voegtoe_rechts(move(b18));
		// wortel
		ZoekboomMK<int, string> b10(10, "tien", zwart);
		b10->voegtoe_links(move(b3));
		b10->voegtoe_rechts(move(b15));

		w = move(b10);
	}
	bool voegtoe_bottomup1() { // enkel eerste geval 1x
		ZoekboomMK<int, string> boom;
		vulmettestboom1(boom);
		if (!boom.repOK()) return false;

		boom.voegtoe_bottomup(5, "vijf");

		ZoekboomMK<int, string> expected;
		vulmettestboom1(expected);
		ZoekknoopMK<int, string>* zes = (expected->links->rechts).get();
		zes->wijzigKleur(rood);
		zes->links->wijzigKleur(zwart);
		zes->rechts->wijzigKleur(zwart);
		ZoekboomMK<int, string> vijf(5, "vijf", rood);
		zes->links->voegtoe_rechts(move(vijf));

		return areEqual(boom, expected);
	}
	bool voegtoe_bottomup2() { // eerste geval + rode wortel
		ZoekboomMK<int, string> b20(20, "twintig", rood);
		ZoekboomMK<int, string> b10(10, "tien", rood);
		ZoekboomMK<int, string> b15(15, "vijftien", zwart);
		b15->voegtoe_links(move(b10));
		b15->voegtoe_rechts(move(b20));
		b15.voegtoe_bottomup(13, "dertien");

		ZoekboomMK<int, string> e13(13, "dertien", rood);
		ZoekboomMK<int, string> e20(20, "twintig", zwart);
		ZoekboomMK<int, string> e10(10, "tien", zwart);
		e10->voegtoe_rechts(move(e13));
		ZoekboomMK<int, string> e15(15, "vijftien", zwart);
		e15->voegtoe_links(move(e10));
		e15->voegtoe_rechts(move(e20));
		
		return areEqual(b15, e15);
	}
	bool voegtoe_bottomup3() { // tweede geval, op één lijn
		ZoekboomMK<int, string> b9(9, "negen", zwart);
		ZoekboomMK<int, string> b11(11, "elf", rood);
		b11->voegtoe_links(move(b9));
		ZoekboomMK<int, string> b3(3, "drie", zwart);
		ZoekboomMK<int, string> b7(7, "zeven", zwart);
		b7->voegtoe_links(move(b3));
		b7->voegtoe_rechts(move(b11));

		b7.voegtoe_bottomup(17, "zeventien");

		ZoekboomMK<int, string> e17(17, "zeventien", rood);
		ZoekboomMK<int, string> e9(9, "negen", zwart);
		ZoekboomMK<int, string> e11(11, "elf", zwart);
		ZoekboomMK<int, string> e3(3, "drie", zwart);
		ZoekboomMK<int, string> e7(7, "zeven", rood);
		e7->voegtoe_links(move(e3));
		e7->voegtoe_rechts(move(e9));
		e11->voegtoe_links(move(e7));
		e11->voegtoe_rechts(move(e17));

		return areEqual(b7, e11);
	}
	bool voegtoe_bottomup4() { // tweede geval, niet op één lijn
		ZoekboomMK<int, string> b17(17, "zeventien", zwart);
		ZoekboomMK<int, string> b11(11, "elf", rood);
		b11->voegtoe_rechts(move(b17));
		ZoekboomMK<int, string> b3(3, "drie", zwart);
		ZoekboomMK<int, string> b7(7, "zeven", zwart);
		b7->voegtoe_links(move(b3));
		b7->voegtoe_rechts(move(b11));

		b7.voegtoe_bottomup(9, "negen");

		ZoekboomMK<int, string> e17(17, "zeventien", zwart);
		ZoekboomMK<int, string> e11(11, "elf", rood);
		ZoekboomMK<int, string> e9(9, "negen", zwart);
		ZoekboomMK<int, string> e7(7, "zeven", rood);
		ZoekboomMK<int, string> e3(3, "drie", zwart);
		e7->voegtoe_links(move(e3));
		e11->voegtoe_rechts(move(e17));
		e9->voegtoe_links(move(e7));
		e9->voegtoe_rechts(move(e11));

		return areEqual(b7, e9);
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
		print_test_result("voegtoe_bottomup1", voegtoe_bottomup1(), failed);
		print_test_result("voegtoe_bottomup2", voegtoe_bottomup2(), failed);
		print_test_result("voegtoe_bottomup3", voegtoe_bottomup3(), failed);
		print_test_result("voegtoe_bottomup4", voegtoe_bottomup4(), failed);

		cout << endl;
		print_test_result(failed);
	}

};
#endif