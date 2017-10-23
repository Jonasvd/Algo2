#ifndef __FLESSENFABRIEK_H
#define __FLESSENFABRIEK_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

// ************************************* FLESSEN

class Flessen {
public:
	Flessen() = default;
	Flessen(vector<int> grootte, vector<int> aantal);
	Flessen(const Flessen& o) = default;
	void groottes(vector<int>& res) const;
	void aantal(vector<int>& res) const;
	bool haalFlesWeg(int grootte);
	void vulbareVolumes(vector<int>& res);
	int getAantal(int grootte);

	Flessen& operator=(const Flessen& o) = default;
	void schrijf(ostream& out);
private:
	map<int, int> _map;
};

Flessen::Flessen(vector<int> grootte, vector<int> aantal) {
	if (grootte.size() != aantal.size()) {
		cout << "Flessen ctor: grootte en aantal hebben verschillende grootte" << endl;
		return;
	}
	
	for (int i = 0; i < grootte.size(); i++) {
		_map.insert(pair<int,int>(grootte[i], aantal[i]));
	}
}

void Flessen::groottes(vector<int>& res) const{
	for (map<int, int>::const_iterator it = _map.begin(); it != _map.end(); ++it) {
		res.push_back(it->first);
	}
}

void Flessen::aantal(vector<int>& res) const {
	for (map<int, int>::const_iterator it = _map.begin(); it != _map.end(); ++it) {
		res.push_back(it->second);
	}
}

bool Flessen::haalFlesWeg(int grootte) {
	map<int,int>::iterator it = _map.find(grootte);
	if (it == _map.end()) return false; //bevat de grootte niet
	
	if (it->second <= 0) return false; //kan niet meer verkleinen
		
	it->second--;
	return true;
}

int Flessen::getAantal(int grootte) {
	map<int, int>::iterator it = _map.find(grootte);
	if (it == _map.end()) return -1; //bevat de grootte niet

	return it->second;
}

void Flessen::vulbareVolumes(vector<int>& res) {
	for (map<int, int>::const_iterator it = _map.begin(); it != _map.end(); ++it) {
		if (it->second > 0)
			res.push_back(it->first);
	}
}

void Flessen::schrijf(ostream& out) {
	for (map<int, int>::const_iterator it = _map.begin(); it != _map.end(); ++it) {
		out << it->first << "(" << it->second << ") ";
	}
}

// *********************************** PROBLEEM

class Probleem {
public:
	Probleem(int vol, Flessen flssn) : volume(vol), flessen(flssn) {};
	int volume;
	Flessen flessen;
	string str();
	void schrijf(ostream& out);

	Probleem& operator=(const Probleem& o);
};

string Probleem::str() {
	stringstream out;
	flessen.schrijf(out);
	string res = "probleem: " + to_string(volume) + " met flessen: " + out.str();
	return res;
}

void Probleem::schrijf(ostream& out) {
	out << str();
}

Probleem& Probleem::operator=(const Probleem& o) {
	Flessen f = o.flessen;
	return Probleem(o.volume, f);
}

// *********************************** OPLOSSING

class Oplossing {
public:
	Oplossing() : gevonden(false) {};
	Oplossing(Flessen flssn) : flessen(flssn), gevonden(true) {};
	Oplossing(const Oplossing& o) = default;
	Flessen flessen;
	bool gevonden;

	Oplossing& operator=(const Oplossing& o);
	Oplossing& operator=(Oplossing&& o);
};

Oplossing& Oplossing::operator=(const Oplossing& other) {
	cout << "copy oplossing" << endl;
	gevonden = other.gevonden;
	flessen = other.flessen;
	return *this;
}

Oplossing& Oplossing::operator=(Oplossing&& other) {
	gevonden = other.gevonden;
	flessen = move(other.flessen);
	return *this;
}

// *********************************** FABRIEK

class FlessenFabriek {
public:
	FlessenFabriek(vector<int> grootte, vector<int> aantal) : _grootte(grootte), _aantal(aantal) {};
	void schrijf();
	void schrijf(ostream& os);

	Oplossing vulVolume(int volume);
private:
	vector<int> _grootte;
	vector<int> _aantal;
	string printDifference(Flessen& voor, Flessen& na);

	Oplossing zoekOplossing(Probleem& probleem, int& besteOplossing, int huidigNiveau);
	
};

void FlessenFabriek::schrijf() {
	schrijf(cout);
}
void FlessenFabriek::schrijf(ostream& os) {
	os << "FlessenFabriek:" << endl << "  groottes: ";
	for (int i = 0; i < _grootte.size(); i++) {
		os << "\t" << _grootte[i];
	}
	os << endl << "  aantal: ";
	for (int i = 0; i < _aantal.size(); i++) {
		os << "\t" << _aantal[i];
	}
	os << endl;
}

string FlessenFabriek::printDifference(Flessen& voor, Flessen& na) {
	string result;

	vector<int> groottes;
	voor.groottes(groottes);

	for (int i = 0; i < groottes.size(); i++) {
		int v = voor.getAantal(groottes[i]);
		int n = na.getAantal(groottes[i]);
		if (v == -1 || n == -1) return "FOUT IN PRINT DIFFERENCE";
		int diff = v - n;
		result += to_string(diff) + " van " + to_string(groottes[i]) + ", ";
	}
	return result;
}

Oplossing FlessenFabriek::vulVolume(int volume) {
	Flessen flssn(_grootte, _aantal);
	Probleem prob(volume, flssn);
	
	int beste = -1;
	Oplossing oplossing = zoekOplossing(prob, beste, 0);

	/*if (oplossing.gevonden) {
		cout << "Oplossing voor volume " << volume << " in " << beste << " flessen:" << endl;
		cout << "  flessen voor:\t";
		flssn.schrijf(cout);
		cout << endl;
		cout << "  flessen na:\t";
		oplossing.flessen.schrijf(cout);
		cout << endl;
		cout << "  => " << printDifference(flssn, oplossing.flessen) << endl;
	}
	else {
		cout << "Geen oplossing voor volume " << volume << endl;
	}*/
	return move(oplossing);
}

Oplossing FlessenFabriek::zoekOplossing(Probleem& probleem, int& besteOplossing, int huidigNiveau) {
	// Volume onder nul => geen oplossing
	if (probleem.volume < 0) return Oplossing();
	
	// Volume is nul => oplossing gevonden
	if (probleem.volume == 0) {
		besteOplossing = huidigNiveau;
		return Oplossing(probleem.flessen);
	}

	// Haal alle volumes op waar er nog flessen van zijn die kunnen gevuld worden
	vector<int> volumes;
	probleem.flessen.vulbareVolumes(volumes);
	sort(volumes.rbegin(), volumes.rend()); // Reverse sorteren

	// Volume maar geen flessen meer => geen oplossing
	if (probleem.volume > 0 && volumes.size() == 0) return Oplossing();

	// Er is een (voorlopig) beste oplossing en je gebruikt nu al meer flessen dan die => al stoppen
	if (besteOplossing != -1 && huidigNiveau > besteOplossing) return Oplossing();
		
	
	//cout << "(" << huidigNiveau << ") zoek oplossing voor " << probleem.str() << endl;

	// Een (betere) oplossing zoeken
	Oplossing voorlopigBeste;
	for (int i = 0; i < volumes.size(); i++) {

		Probleem deelprobleem = probleem;
		deelprobleem.volume -= volumes[i]; // grootte aftrekken
		deelprobleem.flessen.haalFlesWeg(volumes[i]); // er is dan ook een fles minder van dat volume

		
		int lokaleBesteOplossing = besteOplossing;
		Oplossing opl = zoekOplossing(deelprobleem, lokaleBesteOplossing, huidigNiveau + 1);

		if (opl.gevonden && (besteOplossing == -1 || lokaleBesteOplossing < besteOplossing)) {
			//cout << "betere oplossing gevonden voor " << deelprobleem.str() << endl;
			besteOplossing = lokaleBesteOplossing;
			voorlopigBeste = move(opl);
		}
	}

	// Als er geen oplossing gevonden is zal voorlopigBeste.gevonden == false => gewoon teruggeven
	return move(voorlopigBeste);
}

#endif
