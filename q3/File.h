#ifndef FILE_H
#define FILE_H

#define MAX_PRODUITS_FILE 10 // Nombre de produit maximum que la file peut contenir
#include <queue>
#include <semaphore.h>
#include <pthread.h>
#include "Produit.h"

// Code retourne par les methodes
enum CodeFile
{
    FILE_TERMINEE,
    FILE_ITEM_VALIDE
};

class File
{
  private:
    // Une file FIFO de la STL
    std::queue<Produit> FileItems;

    // Vous pouvez ajouter ici d'autres variables privées
    bool flushConsommateurCalled;
    sem_t semaphore_file_pleine;
    pthread_cond_t cond_retire_nouvelle_action_possible;
    pthread_mutex_t mutex_accessFile;


  public:
    File();
    ~File();
    CodeFile Retire(Produit &p);
    CodeFile Insere(Produit &p);
    int FlushConsommateurs(void);
};
#endif
//20191H