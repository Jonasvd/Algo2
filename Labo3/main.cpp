#include "DNASequentie.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

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
	for (int mens = 0; mens < human.size(); mens++){
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
	// alle chimpansee strings in vector steken
	vector<string> chimp;
	lees_chimpansees(chimp);

	int mens = 0;

	string line;
	ifstream file("human.dna");
	if (file.is_open()) {
		while (getline(file, line)) {
			DNASequentie seq(line);
			int kleinste_mutatie = 9999999999;
			int kleinste_index = 0;

			for (int i = 0; i < chimp.size(); i++) {
				int res = seq.d(chimp[i]);
				if (res < kleinste_mutatie) {
					kleinste_mutatie = res;
					kleinste_index = i;
				}
			}

			cout << mens << " " << kleinste_index << " " << kleinste_mutatie << endl;

			mens++;
		}
		file.close();
	}
	else
		cout << "kan human.dna niet openen" << endl;

}

int main() {
	/*DNASequentie sequentie("sitting");

	cout << sequentie << endl;
	cout << sequentie.d("kitten") << endl;*/

	/*cout << "Menselijk " << mens << " => chimpansee " <<  << endl;
	mens++;*/

	//bereken_kleinste_mutatie();
	
	const clock_t begin_time = clock();

	vector<string> human;
	lees_human(human);
	vector<string> chimp;
	lees_chimpansees(chimp);
	bereken(human, chimp);

	cout << float(clock() - begin_time) / CLOCKS_PER_SEC;

	string s;
	cin >> s;

	return 0;
}