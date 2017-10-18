#ifndef __DNASEQUENTIE_H
#define __DNASEQUENTIE_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// functie om 2D vector uit te schrijven
void schrijf(const vector<vector<int>>& vector) {
	for (int i = 0; i < vector.size(); i++) {
		for (int j = 0; j < vector[i].size(); j++) {
			cout << vector[i][j] << " ";
		}
		cout << endl;
	}
}

int minimum(int a, int b, int c) {
	return min(a, min(b, c));
}

class DNASequentie : public string {
public:
	DNASequentie() {};
	DNASequentie(string s) : string(s) {};
	int d(const string&) const;

};

// code vakkundig geleend van https://en.wikipedia.org/wiki/Levenshtein_distance#Iterative_with_full_matrix
int DNASequentie::d(const string& b) const {
	const int lengte_a = this->length();
	const int lengte_b = b.length();

	// om het iets intuitiever te maken
	const DNASequentie& a = *this;


	// 2D vector aanmaken met allemaal nullen
	vector<vector<int>> d;
	for (int i = 0; i < lengte_a + 1; i++) {
		vector<int> rij(lengte_b + 1, 0);
		d.push_back(rij);
	}

	// eerste rij op 0 1 2 3 ... n instellen
	for (int i = 0; i < d[0].size(); i++) {
		d[0][i] = i;
	}

	// eerste kolom op 0 1 2 3 ... n instellen
	for (int i = 0; i < d.size(); i++) {
		d[i][0] = i;
	}

	// initialisatie is gedaan => het algoritme zelf
	for (int j = 1; j < d[0].size(); j++) { // kolom per kolom VANAF 1
		for (int i = 1; i < d.size(); i++) {
			//int kost = a[i-1] == b[j-1] ? 0 : 1;
			//d[i][j] = minimum(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + kost); // opm: als kost = 0 zal je sowieso d[i-1][j-1] gebruiken, de andere heb je dan niet meer nodig
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = minimum(d[i - 1][j]     + 1,
								  d[i]    [j - 1] + 1, 
								  d[i - 1][j - 1] + 1);
		}
	}

	//schrijf(d);

	return d[lengte_a][lengte_b];
}

#endif