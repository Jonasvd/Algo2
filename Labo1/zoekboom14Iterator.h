#ifndef __ZOEKBOOM_ITERATOR_H
#define __ZOEKBOOM_ITERATOR_H
#include <cstdlib>
#include <iostream>
#include "zoekboom14.h"

using namespace std;
//iterator overloopt in order (als in "ervanuit gaan dat de waardes kloppen")
template <class Sleutel, class Data>
class ZoekboomIterator {
private:
	//de iterator heeft een pointer naar een knoop... vanuit elke knoop kan je naar zijn opvolger wandelen...
	Zoekknoop<Sleutel, Data>* first;
	Zoekknoop<Sleutel, Data>* current;
	Zoekknoop<Sleutel, Data>* last;

	Zoekknoop<Sleutel, Data>* zoek_first(const Zoekboom<Sleutel, Data>& boom) {
		Zoekknoop<Sleutel, Data>* hulp = boom.get();
		// zolang je naar links kunt afdalen is er een knoop die kleiner is en dus eerder komt (vanuit de wortel gezien)
		while (hulp->links) {
			hulp = (hulp->links).get();
		}
		return hulp;
	};

	Zoekknoop<Sleutel, Data>* zoek_last(const Zoekboom<Sleutel, Data>& boom) {
		Zoekknoop<Sleutel, Data>* hulp = boom.get();
		// zolang je naar rechts kunt afdalen is er een knoop die groter is en dus later komt (vanuit de wortel gezien)
		while (hulp->rechts) {
			hulp = (hulp->rechts).get();
		}
		return hulp;
	};
public:
	template <class Sleutel, class Data>
	ZoekboomIterator(const Zoekboom<Sleutel, Data>& boom) {
		//cout << "constructor ZoekboomIterator" << endl;
		first = zoek_first(boom);
		last = zoek_last(boom);
		current = first;
	};
	
	ZoekboomIterator<Sleutel, Data>& setToFirst() {
		current = first;
		return *this;
	};
	ZoekboomIterator<Sleutel, Data>& setToLast() { 
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
			Zoekknoop<Sleutel, Data>* huidig = current;

			while (huidig->ouder && huidig->ouder->links.get() != huidig) { //(teken eens... (moet niet)) //eerst controlleren of ouder wel bestaat
				huidig = huidig->ouder;
			}
			current = huidig->ouder;
		}
		return true;
	};

	Zoekknoop<Sleutel, Data>& operator *() { //zoekknoop&, anders is het de maal...
		return *current;
	}
};

#endif