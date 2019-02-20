#include <iostream>
#include "File.h"

using namespace std;

// Constructeur
File::File() {
	FlushAppelle = false;
	pthread_mutex_init(&mutexFile,0);
	pthread_cond_init(&CondCons,0);
	pthread_cond_init(&CondProd,0);
	
}

// Destructeur
File::~File() {
	pthread_cond_destroy(&CondCons);
	pthread_cond_destroy(&CondProd);
	pthread_mutex_destroy(&mutexFile);
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
	pthread_mutex_lock(&mutexFile);
	if (FileItems.size()==0)
	{
    		
		cout<<"--File::Retire() La file est vide. Allons dormir!"<<endl;			
		while(FileItems.size()==0 && FlushAppelle==false) pthread_cond_wait(&CondCons,&mutexFile);
		if (FlushAppelle==true && FileItems.size()<1)
		{
			cout<<"--File::Retire() la file termine, nous devons quitter."<<endl;
			pthread_mutex_unlock(&mutexFile);
			return FILE_TERMINEE;
		}
	
	}
	p=FileItems.back();
	FileItems.pop();
	cout<<"--File::Retire() produit avec numero de serie "<< p.GetNumProduit()<<". Nombre de produit dans la file :"<<FileItems.size()<<"."<<endl;
	pthread_cond_signal(&CondProd);
	pthread_mutex_unlock(&mutexFile);
	
	
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
	pthread_mutex_lock(&mutexFile);
	if (FileItems.size()==MAX_PRODUITS_FILE)
	{
		cout<<"++File::Insere() File pleine! Allons dormir!"<<endl;
		while(FileItems.size()==MAX_PRODUITS_FILE) pthread_cond_wait(&CondProd,&mutexFile);
	}

		FileItems.push(p);
    		cout<<"++File::Insere() produit avec numero de serie "<<p.GetNumProduit()<<".Nombre d'items dans la file = "<<FileItems.size()<<" ."<<endl;
		pthread_cond_signal(&CondCons);
		pthread_mutex_unlock(&mutexFile);
	
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
	FlushAppelle =true;
	pthread_cond_broadcast(&CondCons);
	
	return 1;	// Juste pour retourner quelque chose
}
