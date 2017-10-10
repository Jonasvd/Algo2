#ifndef __ZOEKBOOM_H
#define __ZOEKBOOM_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <cassert>
#include <string>
#include <stdexcept>

using std::ostream;
using std::unique_ptr;
using std::cerr;

using namespace std;
/**********************************************************************

Class: Zoekboom

beschrijving: Binaire zoekboom waarin geen duplicaatsleutels zijn toegestaan.

***************************************************************************/

template <class Sleutel, class Data>
class Zoekknoop;

class Zoekboom14Tests;

template <class Sleutel, class Data>
class ZoekboomIterator;

template <class Sleutel, class Data>
class Zoekboom : public unique_ptr<Zoekknoop<Sleutel, Data>> {
	//.... constructoren, copy move
	using unique_ptr<Zoekknoop<Sleutel, Data>>::unique_ptr;
public:
	// ADDED constructor
	Zoekboom() : unique_ptr<Zoekknoop<Sleutel, Data>>(nullptr) {};

	//Zoekboom(const Sleutel& s,const Data& d) : unique_ptr<Zoekknoop<Sleutel,Data>>(new Zoekknoop<Sleutel,Data>(s,d)){ };
	// OF NOG BETER:
	Zoekboom(const Sleutel& s, const Data& d) {
		*this = make_unique<Zoekknoop<Sleutel, Data>>(s, d);
	};

	// ADDED move constructor
	Zoekboom(Zoekboom<Sleutel, Data>&&) = default;

	// ADDED move assignment operator
	/*Zoekboom<Sleutel,Data>& operator=(Zoekboom<Sleutel,Data>&& other){
	cout << "move operator= met knoop = " << *other << endl;
	this->reset(other.release());
	cout << "reset done" << endl;
	return *this;
	}*/
	Zoekboom& operator=(Zoekboom<Sleutel, Data>&& other) = default;

	// ADDED copy constructor
	Zoekboom(const Zoekboom<Sleutel, Data>& other) {
		*this = other.kopieer();
	}

	// ADDED copy assignment operator
	Zoekboom& operator=(Zoekboom<Sleutel, Data>& other) {
		return other.kopieer();
	}

	// ADDED copy method
	Zoekboom<Sleutel, Data> kopieer() const {
		// Nieuwe boom aanmaken
		Zoekboom<Sleutel, Data> nieuw;
		// Sleutel en Data invullen
		nieuw.reset(new Zoekknoop<Sleutel, Data>((*this)->sleutel, (*this)->data));

		// Linkerkind maken als bestaat
		if ((*this)->links) {
			nieuw->links = move((*this)->links.kopieer());
			// Ouder van linkerkind van nieuwe knoop naar nieuwe knoop laten wijzen
			nieuw->links->ouder = nieuw.get();
		}

		// Rechterkind maken als bestaat
		if ((*this)->rechts) {
			nieuw->rechts = move((*this)->rechts.kopieer());
			// Ouder van rechter kind van nieuwe knoop naar nieuwe knoop laten wijzen
			nieuw->rechts->ouder = nieuw.get();
		}

		// Kopie teruggeven
		return nieuw;
	}

	void voegtoe(const Sleutel& sleutel, const Data& data);
	void roteer(bool naarlinks);
	void inorder(std::function<void(const Zoekknoop<Sleutel, Data>&)> bezoek) const;
	void schrijf(ostream& os) const;
	bool repOK() const;
	int geefDiepte();
	void maakOnevenwichtig();
	void maakEvenwichtig();

protected:
	//zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
	//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
	//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
	void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);

private:
	void maakEvenwichtig(bool);
};


template <class Sleutel, class Data>
class Zoekknoop {
	friend class Zoekboom<Sleutel, Data>;
	friend class Zoekboom14Tests;
	friend class ZoekboomIterator<Sleutel, Data>;
public:
	Zoekknoop() :ouder(0) {}
	Zoekknoop(const Sleutel& sl, const Data& d) :sleutel{ sl }, data(d), ouder(0){};
	Zoekknoop(Sleutel&& sl, Data&& d) :sleutel{ move(sl) }, data(move(d)), ouder(0){};

	// ADDED
	void voegtoe_links(Zoekboom<Sleutel, Data>&& boom) {
		//if (this) cout << "This is NULL" << endl;
		//cout << "voeg links toe: " << *boom << endl;
		links = move(boom);
		links->ouder = this;
	};
	// ADDED
	void voegtoe_rechts(Zoekboom<Sleutel, Data>&& boom) {
		//if (this) cout << "This is NULL" << endl;
		//cout << "voeg rechts toe: " << *boom << endl;
		rechts = move(boom);
		rechts->ouder = this;
	};

	// ADDED
	Zoekboom<Sleutel, Data>* getKind(bool linkerkind) {
		return linkerkind ? &links : &rechts;
	}

protected:
	Sleutel sleutel;
	Data data;
	Zoekknoop<Sleutel, Data>* ouder;
	Zoekboom<Sleutel, Data> links, rechts;

private:
	// ADDED << operator
	friend std::ostream& operator<<(std::ostream& os, const Zoekknoop& knoop)
	{
		os << "sleutel: " << knoop.sleutel << " data: " << knoop.data;

		return os;
	}
};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data) {
	Zoekboom<Sleutel, Data>* plaats;
	Zoekknoop<Sleutel, Data>* ouder;
	zoek(sleutel, ouder, plaats);
	if (!*plaats) {
		//noot: vanaf C++14 wordt dit
		Zoekboom<Sleutel, Data> nieuw = make_unique<Zoekknoop<Sleutel, Data> >(sleutel, data);
		//tot dan was het:
		//        Zoekboom<Sleutel,Data> nieuw(new Zoekknoop<Sleutel,Data>(sleutel,data));
		nieuw->ouder = ouder;
		*plaats = move(nieuw);
	};

};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats) {
	plaats = this;
	ouder = 0;
	while (plaats && (*plaats)->sleutel != sleutel) {
		ouder = plaats->get();
		if ((*plaats)->sleutel < sleutel)
			plaats = &(*plaats)->rechts;
		else
			plaats = &(*plaats)->links;
	};
};
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::inorder(std::function<void(const Zoekknoop<Sleutel, Data>&)> bezoek) const {
	if (*this) {
		(*this)->links.inorder(bezoek);
		bezoek(**this);
		(*this)->rechts.inorder(bezoek);
	};
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::schrijf(ostream& os) const {
	inorder([&os](const Zoekknoop<Sleutel, Data>& knoop) {
		os << "(" << knoop.sleutel << " -> " << knoop.data << ")";
		// ADDED print ook ouder uit
		os << "\n Ouder: ";
		if (knoop.ouder)
			os << knoop.ouder->sleutel;
		else
			os << "----- (geen ouder)";
		os << "\n  Linkerkind: ";
		if (knoop.links)
			os << knoop.links->sleutel;
		else
			os << "-----";
		os << "\n  Rechterkind: ";
		if (knoop.rechts)
			os << knoop.rechts->sleutel;
		else
			os << "-----";
		os << "\n";
	});
}

// ADDED geefDiepte
template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte() {
	if (!(*this)) return 0;

	// geen kinderen => diepte is 0
	if (!(*this)->links && !(*this)->rechts) {
		return 0;
	}
	int linkseDiepte = 0;
	int rechtseDiepte = 0;
	// diepte links en rechts opvragen, +1 om deze knoop mee te rekenen
	if ((*this)->links) {
		linkseDiepte = (*this)->links.geefDiepte() + 1;
	}
	if ((*this)->rechts) {
		rechtseDiepte = (*this)->rechts.geefDiepte() + 1;
	}

	// de grootste diepte wint
	return linkseDiepte > rechtseDiepte ? linkseDiepte : rechtseDiepte;
}

// ADDED roteer
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteer(bool naarlinks) {
	// TODO check of het wel geroteerd kan worden (heeft kind? brico bestaat?)

	// Als je een brico hebt release je hem, anders niet
	//if((*((*this)->getKind(!naarlinks)))->getKind(naarlinks))
	// Hoeft niet, release kan overweg met nullptr
	Zoekknoop<Sleutel, Data>* brico = (*((*this)->getKind(!naarlinks)))->getKind(naarlinks)->release();

	Zoekknoop<Sleutel, Data>* hulp = (*this)->getKind(!naarlinks)->release();

	hulp->ouder = (*this)->ouder;

	// Als brico bestaat (door release) voeg je hem toe als kind
	if (brico) {
		(*this)->getKind(!naarlinks)->reset(brico);
		(*((*this)->getKind(!naarlinks)))->ouder = (*this).get();
	}

	hulp->getKind(naarlinks)->reset((*this).release());
	(*(hulp->getKind(naarlinks)))->ouder = hulp;

	(*this).reset(hulp);
}

// ADDED maakOnevenwichtig
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakOnevenwichtig() {
	// Als we een lege boom hebben (dus ook geen knoop) moeten we direct stoppen
	if (!(*this)) return;

	// Zolang we een rechterkind hebben roteren we naar links
	// de bedoeling is om een gelinkte lijst te maken met alle knopen naar links
	while ((*this)->rechts) {
		this->roteer(true);
	}
	// Nu doen we hetzelfde voor ons linkerkind
	if ((*this)->links)
		((*this)->links).maakOnevenwichtig();
}

// ADDED maakEvenwichtig helpfunctie
// richting:
//	true = naar links
//	false = naar rechts
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakEvenwichtig(bool richting) {
	// Tel de diepte
	int aantalKnopen = this->geefDiepte();// +1; // +1 kan ook maar dan voer je een overbodige rotatie uit
	int aantalRotaties = aantalKnopen / 2; // half zoveel keer roteren => onze kinderen zijn even lang (of één knoop verschillend)
										   // we maken eigenlijk van één gelinkte lijst twee gelinkte lijsten van half zo groot

	for (int i = 0; i < aantalRotaties; i++) {
		this->roteer(richting);
	}

	// Nu doen we opnieuw hetzelfde voor de kinderen
	if (aantalRotaties != 0) {
		((*this)->links).maakEvenwichtig(false);
		((*this)->rechts).maakEvenwichtig(true);
	}
}

// ADDED maakEvenwichtig
template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakEvenwichtig() {
	// Het is veel gemakkelijker om een onevenwichtige boom evenwichtig te maken
	this->maakOnevenwichtig(); // Sowieso enkel linker kinderen
	this->maakEvenwichtig(false); // naar rechts roteren
}

template <class Sleutel, class Data>
bool Zoekboom<Sleutel, Data>::repOK() const{

	ZoekboomIterator<int, string> it(*this);
	Sleutel laatste = (*it).sleutel;
	
	bool hasMore = it.setToNext();
	while (hasMore) {
		// volgorde testen
		if (laatste > (*it).sleutel) return false;
		laatste = (*it).sleutel;

		// ouderpointers testen
		if ((*it).links && (*it).links->ouder != &(*it)) return false;
		if ((*it).rechts && (*it).rechts->ouder != &(*it)) return false;

		hasMore = it.setToNext();
	}
	
	return true;
}

#endif
