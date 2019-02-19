#include <iostream>
#include "File.h"

using namespace std;

// Constructeur
File::File() {
	// A vous d'ecrire le code!

}

// Destructeur
File::~File() {
	// A vous d'ecrire le code!
}

/**************************************************************************************
  Méthode CodeFile File::Retire(Produit & p)
    Cette méthode retire un produit de la file.
    Si la file est vide, cette méthode doit bloquer.
    
    La fonction retourne :
        FILE_ITEM_VALIDE    si l'item est valide
        FILE_TERMINEE       si la file est vide et que la file a recu la commande 
                            FlushConsommateurs();
20191H
 **************************************************************************************/
CodeFile File::Retire(Produit & p) {

    	// A vous d'ecrire le code!
	return FILE_ITEM_VALIDE;
}

/**************************************************************************************
  Méthode CodeFile File::Insere(Produit & p)
    Cette méthode insere un produit dans la file.
    Si la file est pleine, cette méthode doit bloquer.
    
    La fonction retourne :
        FILE_ITEM_VALIDE    si l'item est valide

 **************************************************************************************/	
CodeFile File::Insere(Produit & p) {

    	// A vous d'ecrire le code!
	return FILE_ITEM_VALIDE;
}

/***************************************************************************************
  Fonction int File::FlushConsommateurs(void)
    Cette méthode est appelée pour indiquer à la file qu’aucun nouvel item 
    ne sera inséré dans la file à partir de ce moment. Après cet appel, 
    la file continue de fonctionner normalement,
    jusqu’au moment où la file est vide.
    
    La fonction retourne simplement le nombre d'item present dans la file.
20191H
 **************************************************************************************/
int File::FlushConsommateurs(void) {
	// A vous d'ecrire le code!
	return 1;	// Juste pour retourner quelque chose
}
