#include <iostream>
#include <stdio.h>
#include "File.h"

using namespace std;

// Constructeur
File::File()
{
    pthread_cond_init(&cond_retire_nouvelle_action_possible, NULL);
    sem_init(&semaphore_file_pleine, 0, MAX_PRODUITS_FILE);
    pthread_mutex_init(&mutex_accessFile, NULL);
}

// Destructeur
File::~File()
{
    pthread_cond_destroy(&cond_retire_nouvelle_action_possible);
    sem_destroy(&semaphore_file_pleine);
    pthread_mutex_destroy(&mutex_accessFile);
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
CodeFile File::Retire(Produit &p)
{
    int nbItemsFile = 0;

    pthread_mutex_lock(&mutex_accessFile);
    nbItemsFile = FileItems.size();
    // Attend si la file n'a plus d'item et que la condition de fermeture n'est pas complétée.
    if ((!flushConsommateurCalled) && nbItemsFile == 0)
    {
        printf("--File::Retire() La file est vide. Allons dormir!\n");
        pthread_cond_wait(&cond_retire_nouvelle_action_possible, &mutex_accessFile);
    }

    // Si la file n'a plus d'item et que la condition de fermeture a été rencontré, alors il faut quitter.
    // Inclue un COURT-CIRCUIT
    if (flushConsommateurCalled && nbItemsFile == 0)
    {
        pthread_mutex_unlock(&mutex_accessFile);
        printf("--File::Retire() La file est termine nous devons quitter.\n");
        return FILE_TERMINEE; // <--- COURT-CIRCUITE
    }

    // Il y a encore des items à consommer.
    p = FileItems.front();
    FileItems.pop();
    nbItemsFile = FileItems.size();
    sem_post(&semaphore_file_pleine);
    pthread_mutex_unlock(&mutex_accessFile);
    printf("--File::Retire()produit avec numero de série %d. Nombre de produits dans la file = %d\n", p.GetNumProduit(), nbItemsFile);
    return FILE_ITEM_VALIDE;
}

/**************************************************************************************
  Méthode CodeFile File::Insere(Produit & p)
    Cette méthode insere un produit dans la file.
    Si la file est pleine, cette méthode doit bloquer.
    
    La fonction retourne :
        FILE_ITEM_VALIDE    si l'item est valide

 **************************************************************************************/
CodeFile File::Insere(Produit &p)
{
    int nbItemsFile = 0;
    // A vous d'ecrire le code!
    sem_wait(&semaphore_file_pleine);
    pthread_mutex_lock(&mutex_accessFile);
    FileItems.push(p);
    nbItemsFile = FileItems.size();
    pthread_cond_signal(&cond_retire_nouvelle_action_possible);
    pthread_mutex_unlock(&mutex_accessFile);
    printf("++File::Insere()produit avec numero de serie %d. Nombre d'items dans la file = %d\n", p.GetNumProduit(), nbItemsFile);
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
int File::FlushConsommateurs(void)
{
    int nbItemsFile = 0;
    pthread_mutex_lock(&mutex_accessFile);
    flushConsommateurCalled = true;
    nbItemsFile = FileItems.size();
    pthread_cond_broadcast(&cond_retire_nouvelle_action_possible);
    pthread_mutex_unlock(&mutex_accessFile);

    return nbItemsFile;
}
