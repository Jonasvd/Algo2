#ifndef __ZoekboomMK_H
#define __ZoekboomMK_H
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

Class: ZoekboomMK

beschrijving: Binaire ZoekboomMK waarin geen duplicaatsleutels zijn toegestaan.

***************************************************************************/

template <class Sleutel, class Data>
class ZoekknoopMK;

class Zoekboom14MKTests;

template <class Sleutel, class Data>
class ZoekboomMKIterator;

enum Kleur { rood, zwart };

template <class Sleutel, class Data>
class ZoekboomMK : public unique_ptr<ZoekknoopMK<Sleutel, Data>> {
	//.... constructoren, copy move
	using unique_ptr<ZoekknoopMK<Sleutel, Data>>::unique_ptr;
public:
	// CHANGED
	ZoekboomMK() : unique_ptr<ZoekknoopMK<Sleutel, Data>>(nullptr) {};

	// CHANGED
	ZoekboomMK(const Sleutel& s, const Data& d) {
		*this = make_unique<ZoekknoopMK<Sleutel, Data>>(s, d);
		(*this)->kleur = zwart;
	};

	// ADDED
	ZoekboomMK(const Sleutel& s, const Data& d, const Kleur& k) {
		*this = make_unique<ZoekknoopMK<Sleutel, Data>>(s, d);
		(*this)->kleur = k;
	};
	
	ZoekboomMK(ZoekboomMK<Sleutel, Data>&&) = default;

	ZoekboomMK& operator=(ZoekboomMK<Sleutel, Data>&& other) = default;
	
	ZoekboomMK(const ZoekboomMK<Sleutel, Data>& other) {
		*this = other.kopieer();
	}
	
	ZoekboomMK& operator=(ZoekboomMK<Sleutel, Data>& other) {
		return other.kopieer();
	}
	
	// CHANGED
	ZoekboomMK<Sleutel, Data> kopieer() const {
		// Nieuwe boom aanmaken
		ZoekboomMK<Sleutel, Data> nieuw;
		// Sleutel en Data invullen
		nieuw.reset(new ZoekknoopMK<Sleutel, Data>((*this)->sleutel, (*this)->data));
		// Kleur instellen
		nieuw->wijzigKleur((*this)->kleur);

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
	void inorder(std::function<void(const ZoekknoopMK<Sleutel, Data>&)> bezoek) const;
	void schrijf(ostream& os) const;
	bool repOK() const;
	int geefDiepte();
	bool controleerZwarteDiepte(int& zwarteDiepte) const;
	void maakOnevenwichtig();
	void maakEvenwichtig();

protected:
	//zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
	//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
	//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
	void zoek(const Sleutel& sleutel, ZoekknoopMK<Sleutel, Data>*& ouder, ZoekboomMK<Sleutel, Data>*& plaats);

private:
	void maakEvenwichtig(bool);
};


template <class Sleutel, class Data>
class ZoekknoopMK {
	friend class ZoekboomMK<Sleutel, Data>;
	friend class Zoekboom14MKTests;
	friend class ZoekboomMKIterator<Sleutel, Data>;
public:
	ZoekknoopMK() :ouder(0) {}
	ZoekknoopMK(const Sleutel& sl, const Data& d) :sleutel{ sl }, data(d), ouder(0){};
	ZoekknoopMK(Sleutel&& sl, Data&& d) :sleutel{ move(sl) }, data(move(d)), ouder(0){};

	
	void voegtoe_links(ZoekboomMK<Sleutel, Data>&& boom) {
		links = move(boom);
		links->ouder = this;
	};
	
	void voegtoe_rechts(ZoekboomMK<Sleutel, Data>&& boom) {
		rechts = move(boom);
		rechts->ouder = this;
	};

	
	ZoekboomMK<Sleutel, Data>* getKind(bool linkerkind) {
		return linkerkind ? &links : &rechts;
	};

	// ADDED wijzig kleur methode
	void wijzigKleur(const Kleur& kleur) {
		this->kleur = kleur;
	}

	// ADDED invert kleur methode
	void invertKleur() {
		kleur = kleur == zwart ? rood : zwart;
	}

protected:
	Sleutel sleutel;
	Data data;
	Kleur kleur;
	ZoekknoopMK<Sleutel, Data>* ouder;
	ZoekboomMK<Sleutel, Data> links, rechts;

private:
	
	friend std::ostream& operator<<(std::ostream& os, const ZoekknoopMK& knoop)
	{
		os << "sleutel: " << knoop.sleutel << " data: " << knoop.data << " kleur: " << knoop.kleur;

		return os;
	}
};

template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data) {
	ZoekboomMK<Sleutel, Data>* plaats;
	ZoekknoopMK<Sleutel, Data>* ouder;
	zoek(sleutel, ouder, plaats);
	if (!*plaats) {
		//noot: vanaf C++14 wordt dit
		ZoekboomMK<Sleutel, Data> nieuw = make_unique<ZoekknoopMK<Sleutel, Data> >(sleutel, data);
		//tot dan was het:
		//        ZoekboomMK<Sleutel,Data> nieuw(new ZoekknoopMK<Sleutel,Data>(sleutel,data));
		nieuw->ouder = ouder;
		*plaats = move(nieuw);
	};

};

template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::zoek(const Sleutel& sleutel, ZoekknoopMK<Sleutel, Data>*& ouder, ZoekboomMK<Sleutel, Data>*& plaats) {
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
void ZoekboomMK<Sleutel, Data>::inorder(std::function<void(const ZoekknoopMK<Sleutel, Data>&)> bezoek) const {
	if (*this) {
		(*this)->links.inorder(bezoek);
		bezoek(**this);
		(*this)->rechts.inorder(bezoek);
	};
}

template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::schrijf(ostream& os) const {
	inorder([&os](const ZoekknoopMK<Sleutel, Data>& knoop) {
		os << "(" << knoop.sleutel << " -> " << knoop.data << " (" << (knoop.kleur == zwart ? "Z" : "R") << "))";
		
		os << "\n Ouder: ";
		if (knoop.ouder)
			os << knoop.ouder->sleutel << "(" << (knoop.ouder->kleur == zwart ? "Z" : "R") << ")";
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


template <class Sleutel, class Data>
int ZoekboomMK<Sleutel, Data>::geefDiepte() {
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

// ADDED
template <class Sleutel, class Data>
bool ZoekboomMK<Sleutel, Data>::controleerZwarteDiepte(int& zwarteDiepte) const{
	// als er geen knoop is heb je ook geen diepte
	if (!(*this)) {
		zwarteDiepte = 0;
		return true;
	}

	// geen kinderen
	if (!(*this)->links && !(*this)->rechts) {
		zwarteDiepte = (*this)->kleur == zwart ? 1 : 0; // deze knoop telt wel mee voor de zwarte diepte als hij zwart is
		return true; // er is geen probleem met de onderliggende zwarte diepte
	}

	// als er wel kinderen hebben
	int linkseDiepte = 0;
	int rechtseDiepte = 0;

	bool linksOK = true; // standaard op true (er is ook geen probleem als hij niet zou bestaan
	bool rechtsOK = true;

	if ((*this)->links) {
		linksOK = (*this)->links.controleerZwarteDiepte(linkseDiepte);
	}
	if ((*this)->rechts) {
		rechtsOK = (*this)->rechts.controleerZwarteDiepte(rechtseDiepte);
	}

	if (linkseDiepte != rechtseDiepte) return false;
	
	zwarteDiepte = linkseDiepte + ((*this)->kleur == zwart ? 1 : 0);
	return linksOK && rechtsOK;
}


template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::roteer(bool naarlinks) {
	// TODO check of het wel geroteerd kan worden (heeft kind? brico bestaat?)

	// Als je een brico hebt release je hem, anders niet
	//if((*((*this)->getKind(!naarlinks)))->getKind(naarlinks))
	// Hoeft niet, release kan overweg met nullptr
	ZoekknoopMK<Sleutel, Data>* brico = (*((*this)->getKind(!naarlinks)))->getKind(naarlinks)->release();

	ZoekknoopMK<Sleutel, Data>* hulp = (*this)->getKind(!naarlinks)->release();

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


template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::maakOnevenwichtig() {
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


// richting:
//	true = naar links
//	false = naar rechts
template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::maakEvenwichtig(bool richting) {
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


template <class Sleutel, class Data>
void ZoekboomMK<Sleutel, Data>::maakEvenwichtig() {
	// Het is veel gemakkelijker om een onevenwichtige boom evenwichtig te maken
	this->maakOnevenwichtig(); // Sowieso enkel linker kinderen
	this->maakEvenwichtig(false); // naar rechts roteren
}

template <class Sleutel, class Data>
bool ZoekboomMK<Sleutel, Data>::repOK() const{

	ZoekboomMKIterator<int, string> it(*this);
	Sleutel laatste = (*it).sleutel;
	
	bool hasMore = it.setToNext();
	while (hasMore) {
		// volgorde testen
		if (laatste > (*it).sleutel) return false;
		laatste = (*it).sleutel;

		// ouderpointers testen
		if ((*it).links && (*it).links->ouder != &(*it)) return false;
		if ((*it).rechts && (*it).rechts->ouder != &(*it)) return false;

		// ADDED
		// kleur testen
		// er mogen geen twee rode na elkaar voorkomen
		if ((*it).links && (*it).kleur == rood && (*it).links->kleur == rood) return false;
		if ((*it).rechts && (*it).kleur == rood && (*it).rechts->kleur == rood) return false;

		hasMore = it.setToNext();
	}

	// alles ik ok nu, nu nog de zwarte diepte controleren
	int diepte;
	return (*this).controleerZwarteDiepte(diepte);
}

#endif
