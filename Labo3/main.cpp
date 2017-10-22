#include "DNASequentie.h"
#include "FlessenFabriekTests.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void lees_chimpansees(vector<string>& chimp) {
	string line;
	ifstream file("chimp.dna");
	if (file.is_open()) {
		while (getline(file, line)) {
			chimp.push_back(line);
		}
		file.close();
	}
	else
		cout << "kan chimp.dna niet openen" << endl;
}

void lees_human(vector<string>& human) {
	string line;
	ifstream file("human.dna");
	if (file.is_open()) {
		while (getline(file, line)) {
			human.push_back(line);
		}
		file.close();
	}
	else
		cout << "kan human.dna niet openen" << endl;
}

void bereken(vector<string>& human, vector<string>& chimp) {
	for (int mens = 0; mens < human.size(); mens++) {
		DNASequentie seq(human[mens]);
		int kleinste_mutatie = seq.d(chimp[0]);
		int kleinste_index = 0;
		for (int c = 1; c < chimp.size(); c++) {
			int res = seq.d(chimp[c]);
			if (res < kleinste_mutatie) {
				kleinste_mutatie = res;
				kleinste_index = c;
			}
		}
		cout << mens << " " << kleinste_index << " " << kleinste_mutatie << endl;
	}
}

void bereken_kleinste_mutatie() {
	vector<string> human;
	lees_human(human);
	vector<string> chimp;
	lees_chimpansees(chimp);
	bereken(human, chimp);
}

void mainDNA() {
	bereken_kleinste_mutatie();
}

void mainFlessen() {
	FlessenFabriekTests tests;
	tests.runTests();
}

int main() {

	//mainDNA();
	
	mainFlessen();

	// Hou console open
	string s;
	cin >> s;

	return 0;
}