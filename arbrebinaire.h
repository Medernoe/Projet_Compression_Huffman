#ifndef ARBREBINAIRE_H
#define ARBREBINAIRE_H

#include <stdbool.h>
#include "compression.h"


//défini un noeud d'un arbre binaire de recherhe
typedef struct noeud {
    FreqElement value; 
    struct noeud *g;
    struct noeud *d;
} noeud;

typedef struct noeud* PArbre; //pointeur sur un arbre 

// =============================================================================
// fonctions de base

//renvoie un arbre vide
PArbre ArbreVide(void);        

// teste si un arbre est vide
bool EstArbreVide(PArbre); 

//renvoie la valeur de la racine de l'arbre
FreqElement Racine(PArbre);    

//renvoie le sous arbre gauche de l'arbre
PArbre Gauche(PArbre);      

//renvoie le sous arbre droit de l'arbre
PArbre Droit(PArbre);       

// renvoie un arbre construit avec l'FreqElement e comme racine, g son sous arbre gauche et d son sous arbre droit
PArbre Construire(FreqElement e, PArbre g, PArbre d); 

// libere la mémoire de l'arbre
void liberer_arbre(PArbre A);

#endif
