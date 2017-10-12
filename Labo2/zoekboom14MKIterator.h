#ifndef __ZoekboomMK_ITERATOR_H
#define __ZoekboomMK_ITERATOR_H
#include <cstdlib>
#include <iostream>
#include "Zoekboom14MK.h"

using namespace std;
//iterator overloopt in order (als in "ervanuit gaan dat de waardes kloppen")
template <class Sleutel, class Data>
class ZoekboomMKIterator {
private:
	//de iterator heeft een pointer naar een knoop... vanuit elke knoop kan je naar zijn opvolger wandelen...
	ZoekknoopMK<Sleutel, Data>* first;
	ZoekknoopMK<Sleutel, Data>* current;
	ZoekknoopMK<Sleutel, Data>* last;

	ZoekknoopMK<Sleutel, Data>* zoek_first(const ZoekboomMK<Sleutel, Data>& boom) {
		ZoekknoopMK<Sleutel, Data>* hulp = boom.get();
		// zolang je naar links kunt afdalen is er een knoop die kleiner is en dus eerder komt (vanuit de wortel gezien)
		while (hulp->links) {
			hulp = (hulp->links).get();
		}
		return hulp;
	};

	ZoekknoopMK<Sleutel, Data>* zoek_last(const ZoekboomMK<Sleutel, Data>& boom) {
		ZoekknoopMK<Sleutel, Data>* hulp = boom.get();
		// zolang je naar rechts kunt afdalen is er een knoop die groter is en dus later komt (vanuit de wortel gezien)
		while (hulp->rechts) {
			hulp = (hulp->rechts).get();
		}
		return hulp;
	};
public:
	template <class Sleutel, class Data>
	ZoekboomMKIterator(const ZoekboomMK<Sleutel, Data>& boom) {
		//cout << "constructor ZoekboomMKIterator" << endl;
		first = zoek_first(boom);
		last = zoek_last(boom);
		current = first;
	};
	
	ZoekboomMKIterator<Sleutel, Data>& setToFirst() {
		current = first;
		return *this;
	};
	ZoekboomMKIterator<Sleutel, Data>& setToLast() { 
		current = last;
		return *this;
	};
	
	// operator++ is te ingewikkeld of duurt te lang
	bool setToNext() {
		if (current == last) // als je nu al op de laatste staat is er geen volgende (duuh..)
			return false;

		if (current->rechts) {	//indien rechterkind bestaat, rechterkind is opvolger
			current = zoek_first(current->rechts); //eentje rechts, en dan altijd links...
		}
		else {	//geen rechterkind -> je moet naar boven tot je een "linkerkind was"
			ZoekknoopMK<Sleutel, Data>* huidig = current;

			while (huidig->ouder && huidig->ouder->links.get() != huidig) { //(teken eens... (moet niet)) //eerst controlleren of ouder wel bestaat
				huidig = huidig->ouder;
			}
			current = huidig->ouder;
		}
		return true;
	};

	ZoekknoopMK<Sleutel, Data>& operator *() { //ZoekknoopMK&, anders is het de maal...
		return *current;
	}
};

#endif