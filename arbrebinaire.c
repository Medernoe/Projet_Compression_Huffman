#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "arbrebinaire.h"

//creer un arbre vide
PArbre ArbreVide(void){
    PArbre A = NULL; 
    return A;
}

//vérifie si l'abre est vide 
bool EstArbreVide(PArbre A){
    //retourne vrai si l'arbre est vide
    return (A == NULL);
}


FreqElement Racine(PArbre A){
    //verifie que l'abre n'est pas vide 
    if (EstArbreVide(A)){
        FreqElement vide = {'\0', 0}; // uniformiser cette element nul avec l'autre précédent 
        return vide;
    }
    //retourne la première valeur 
    return A->value; 
}    

//renvoie le sous arbre gauche de l'arbre
PArbre Gauche(PArbre A){
    if (EstArbreVide(A)){
        return NULL; 
    }
    return A->g;
}     

//renvoie le sous arbre droit de l'arbre
PArbre Droit(PArbre A){
    if (EstArbreVide(A)){
        return NULL;
    }
    return A->d;
}

// renvoie un arbre construit avec FreqElement e comme racine, g son sous arbre gauche et d son sous arbre droit
PArbre Construire(FreqElement e, PArbre g, PArbre d){
    //creer l'arbre
    PArbre A = malloc(sizeof(noeud)); 

    //assigne les valeurs 
    A->value = e; 
    A->g = g; 
    A->d = d; 

    return A;
}

// libere la mémoire de l'arbre
void liberer_arbre(PArbre A){
    if (EstArbreVide(A)) return;
    liberer_arbre(Gauche(A));
    liberer_arbre(Droit(A));
    free(A);
}

