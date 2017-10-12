#include <iostream>
#include <memory>
#include <string>

#include "Zoekboom14MKTests.h"

using namespace std;

int main()
{
	Zoekboom14MKTests t;
	t.Zoekboom14MK_tests();

	cout << endl << "DONE" << endl;

	string end;
	cin >> end;

	return 0;
}

/*
Kleur uitleg:
het visitor pattern vs iteratoren.
Je hebt een collectie van dingen die je 1 voor 1 wil bezoeken. (bv in de repOK ftie kijken of de ouders wel goed staan)
Het visitor pattern is een typische manier om dit te gaan doen,
Je hebt een container (den boom), de container bevat elementen. En je wil de elementen "bezoeken" dus je zegt tegen de containter
alstublief hier is een functie die jou komt bezoeken. Deze functie kan iets doen met elk van de elementen.
De visitor heeft een functie "behandelknoop" om iets te doen met de knoop. Het is de bedoeling dat de container zorgt dat de visitor
overal op bezoek gaat laten komen.

Het iterator pattern is anders. Er is nogsteeds een container met elementen en de containter heeft een iterator.
Een iterator wijst altijd naar een element. Je zegt nu tegen de container "geef mij een iterator naar het eerste element"
Nu kan je daar mee doen wat je wil. Nu zegt je tegen de iterator, beste iterator verzet jezelf naar het volgende element.
(dit loopt door tot de iterator je verteld dat het gedaan is en er geen elementen meer zijn).

Het verschil is: bij 1 maak je een visitor en heb je er verder niets mee te maken. Bij het tweede kan je veel specifieker zeggen
ik wil "nu" het volgende. (bij de visitor is het de containter die ervoor moet zorgen).
Bij iterator kan je dus ook vroeger gaan stoppen als je een fout tegen komt (dat is gunstig natuurlijk).

Iterator pattern = veel meer controle.
Dit heb je nodig ergens in oefening 1...

*/