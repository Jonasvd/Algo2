#ifndef _SPIRAAL_H
#define _SPIRAAL_H

#include "PointQuadtree.h"
#include "csv.h"
#include <vector>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Point<double,double> f(int i) {
	return Point<double,double>((10000 - 5 * i)* cos(i*0.25*M_PI), (10000 - 5 * i)* sin(i*0.25*M_PI));
}

void maakGrafiek(int aantalPunten) {
	CsvData grafiek("reeks1", ',');

	PointQuadtree<double, double> boom;
	vector<int> dieptes;

	int aantal = 1;

	for (int i = 0; i < aantalPunten; i++) {
		boom.voegToe(f(i));

		if (i == aantal) {
			int diepte = boom.diepte();
			dieptes.push_back(diepte);
			aantal *= 2;
		}
		if (i % 50 == 0) cout << i << endl;
	}

	grafiek.voegDataToe(dieptes);
}

void maakGrafiek_random(int aantalPunten) {
	CsvData grafiek("reeks2", ',');

	PointQuadtree<double, double> boom;
	vector<int> dieptes;

	int aantal = 1;

	vector<int> randomCijfers;
	for (int j = 0; j < aantalPunten; j++) {
		randomCijfers.push_back(j);
	}
	
	random_shuffle(randomCijfers.begin(), randomCijfers.end());

	for (int i = 0; i < aantalPunten; i++) {
		boom.voegToe(f(randomCijfers[i]));

		if (i == aantal) {
			int diepte = boom.diepte();
			dieptes.push_back(diepte);
			aantal *= 2;
		}
		if (i % 50) cout << i << endl;
	}

	grafiek.voegDataToe(dieptes);
}

#endif
