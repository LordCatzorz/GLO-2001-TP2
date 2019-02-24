#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include "Produit.h"

using namespace std;

class ProduitReader
{
  public:
    ProduitReader(std::string readLine)
    {
        // int centsDecimalIndex = readLine.find_last_of('.');
        // const char* cents = readLine.substr(centsDecimalIndex, 2);
        // readLine = readLine.substr(0, centsDecimalIndex -1);
        
        int spaceIndex = readLine.find_last_of(' ');
        const char* intPrice = readLine.substr(spaceIndex, readLine.length() -spaceIndex).c_str();

        readLine = readLine.substr(0, spaceIndex);

        name = readLine;
        price = atof(intPrice);
    }
    std::string GetName(void)
    {
        return this->name;
    }
    float GetPrice(void)
    {
        return this->price;
    }
  private:
    std::string name;
    float price;
};


static std::vector<ProduitReader*> theVector;
static pthread_mutex_t MutexProducteur = PTHREAD_MUTEX_INITIALIZER;


Produit::Produit()
{
    pthread_mutex_lock(&MutexProducteur);
    // Initialise le vecteur de tous les produits à la première lecture.
    if (theVector.size() == 0)
    {
        ifstream fs("./produits.dat");
        string currentLine;
        while (getline(fs, currentLine))
        {
            theVector.push_back(new ProduitReader(currentLine));
        }
    }
    pthread_mutex_unlock(&MutexProducteur);
    this->SetNumProduit(0);
    this->NomProduit = "";
    this->PrixProduit = 0;
}

int Produit::GetNumProduit(void)
{
    return this->NumProduit;
}

void Produit::SetNumProduit(int np)
{
    this->NumProduit = np;
}

string Produit::GetNomProduit(void)
{
    return this->NomProduit;
}

float Produit::GetPrixProduit(void)
{
    return this->PrixProduit;
}

void Produit::PigerProduit(void)
{
    ProduitReader* product = theVector[rand() % theVector.size()];
    this->NomProduit = product->GetName();
    this->PrixProduit = product->GetPrice();
}
//20191H