#ifndef ARBREBINAIRE_H
#define ARBREBINAIRE_H

#include <stdbool.h>

typedef int element; //on considerera ici des entiers

typedef struct noeud *PArbre; //pointeur sur un arbre 

// =============================================================================
// fonctions de base

//renvoie un arbre vide
PArbre ArbreVide();        

// teste si un arbre est vide
bool EstArbreVide(PArbre); 

//renvoie la valeur de la racine de l'arbre
element Racine(PArbre);    

//renvoie le sous arbre gauche de l'arbre
PArbre Gauche(PArbre);      

//renvoie le sous arbre droit de l'arbre
PArbre Droit(PArbre);       

// renvoie un arbre construit avec l'element e comme racine, g son sous arbre gauche et d son sous arbre droit
PArbre Construire(element e, PArbre g, PArbre d); 


// =============================================================================
// Les parcours

//affiche les parcours préfixe, infixe et suffixe de l'arbre

void ParcoursPref(PArbre);

void ParcoursInf(PArbre);

void ParcoursSuff(PArbre);


// =============================================================================
//fonctions pour les arbres binaires de recherche (abr)

//insère l'élément dans l'abr et renvoie l'abr obtenu
PArbre Insertion(element, PArbre);

//renvoie vrai si l'élément est présent dans l'abr, faux sinon
bool Recherche(element, PArbre );


// =============================================================================
// Autres fonctions sur les arbres binaires de recherche
//
// Pour les fonctions suivantes on suppose que l'abr pris en entrée est
// non vide.

//retourne l'element max de l'abr 
element Max(PArbre); 

// retourne l'element max de l'abr
PArbre SuppressionMax(PArbre);
 
//supprime la racine de l'abr et renvoie l'abr obtenu
PArbre SuppressionRacine(PArbre);

//supprime l'élément dans l'abr et renvoie l'abr obtenu
PArbre Suppression(element, PArbre);

#endif
