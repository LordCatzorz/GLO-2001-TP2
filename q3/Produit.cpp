#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include "Produit.h"
#include <iostream>

using namespace std;

Produit::Produit() {
	NumProduit = 0;
	NomProduit ="";
	PrixProduit=0.1f;
}

int Produit::GetNumProduit(void) {
	return NumProduit;
}

void Produit::SetNumProduit(int np) {
	NumProduit = np;
}

string Produit::GetNomProduit(void) {
	return NomProduit;
}

float Produit::GetPrixProduit(void) {
	return PrixProduit;
}
	
void Produit::PigerProduit(void) {
	ifstream fichier("produits.dat");
	vector<string> lignesFichier;
	string ligne;
	while (getline(fichier,ligne))
	{
		lignesFichier.push_back(ligne);
	}
	int numeroLigne = rand() % lignesFichier.size();
	ligne = lignesFichier[numeroLigne];
	NomProduit=ligne.substr(0,ligne.find(" "));
	ligne.erase(0,ligne.find(" ")+1);
	PrixProduit=stof(ligne);
}
//20191H