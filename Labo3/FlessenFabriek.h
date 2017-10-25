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
	int totaalAantalFlessen();

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

int Flessen::totaalAantalFlessen() {
	int result = 0;
	for (map<int, int>::const_iterator it = _map.begin(); it != _map.end(); ++it) {
		result += it->second;
	}
	return result;
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

// *********************************** MEMO

class Memo {
public:
	Oplossing vindOplossing(Probleem& probleem);
	void voegToe(Probleem& probleem, Oplossing oplossing);
private:
	map<string, Oplossing> _map;
	string stringify(Probleem& probleem);
};

string Memo::stringify(Probleem& probleem) {
	string res;
	res += to_string(probleem.volume) + "-";
	vector<int> aantal;
	probleem.flessen.aantal(aantal);
	vector<int> groottes;
	probleem.flessen.groottes(groottes);
	for (int i = 0; i < groottes.size(); i++) {
		res += to_string(groottes[i]) + "(" + to_string(aantal[i]) + ")";
	}
	return res;
}

void Memo::voegToe(Probleem& probleem, Oplossing oplossing) {
	string key = stringify(probleem);
	_map.insert(pair<string, Oplossing>(key, oplossing));
}

Oplossing Memo::vindOplossing(Probleem& probleem) {
	string key = stringify(probleem);
	map<string, Oplossing>::const_iterator it = _map.find(key);

	if (it == _map.end()) return Oplossing();

	return it->second;
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

	Oplossing zoekOplossing(Probleem& probleem, int& besteOplossing, int oorspronkelijkAantalFlessen);
	
	Memo memo;
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
	Flessen flessen(_grootte, _aantal);
	Probleem prob(volume, flessen);

	int oorspronkelijkAantalFlessen = flessen.totaalAantalFlessen();

	int beste = -1;
	Oplossing oplossing = zoekOplossing(prob, beste, oorspronkelijkAantalFlessen);
	return oplossing;
}

Oplossing FlessenFabriek::zoekOplossing(Probleem& probleem, int& besteOplossing, int oorspronkelijkAantalFlessen) {
	// Kijken of nog niet opgelost
	Oplossing memoOpl = memo.vindOplossing(probleem);
	if (memoOpl.gevonden) return move(memoOpl);
	
	// Volume onder nul => geen oplossing
	if (probleem.volume < 0) return Oplossing();

	// Er is een (voorlopig) beste oplossing en je gebruikt nu al meer flessen dan die => al stoppen
	int gebruikteFlessen = oorspronkelijkAantalFlessen - probleem.flessen.totaalAantalFlessen();
	if (besteOplossing != -1 && gebruikteFlessen > besteOplossing) return Oplossing();

	// Volume is nul => oplossing gevonden
	if (probleem.volume == 0) return Oplossing(probleem.flessen);

	// Haal alle volumes op waar er nog flessen van zijn die kunnen gevuld worden
	vector<int> volumes;
	probleem.flessen.vulbareVolumes(volumes);
	sort(volumes.rbegin(), volumes.rend()); // Reverse sorteren

	// Volume maar geen flessen meer => geen oplossing
	if (probleem.volume > 0 && volumes.size() == 0) return Oplossing();

	// Een (betere) oplossing zoeken
	Oplossing voorlopigBeste;
	for (int i = 0; i < volumes.size(); i++) {

		Probleem deelprobleem = probleem;
		deelprobleem.volume -= volumes[i]; // grootte aftrekken
		deelprobleem.flessen.haalFlesWeg(volumes[i]); // er is dan ook een fles minder van dat volume

		Oplossing opl = zoekOplossing(deelprobleem, besteOplossing, oorspronkelijkAantalFlessen);
		int gebruikteFlessen = oorspronkelijkAantalFlessen - opl.flessen.totaalAantalFlessen();

		if (opl.gevonden && (!voorlopigBeste.gevonden || besteOplossing == -1 || gebruikteFlessen < besteOplossing)) {
			besteOplossing = gebruikteFlessen;
			voorlopigBeste = move(opl);
		}
	}

	// Als oplossing gevonden => toevoegen aan memo
	if (voorlopigBeste.gevonden) memo.voegToe(probleem, voorlopigBeste);

	// Als er geen oplossing gevonden is zal voorlopigBeste.gevonden == false => gewoon teruggeven
	return move(voorlopigBeste);
}

#endif
