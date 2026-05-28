#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "arbrebinaire.h"

//creer un arbre vide
PArbre ArbreVide(){
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


//parcour prefixe
void ParcoursPref(PArbre A){
    //si l'arbre à une valeur 
    if (!EstArbreVide(A)){
        //ecrire la valeur 
        printf("%c|%d\n", Racine(A).letter, Racine(A).count);
        // regarde l'arbre gauche 
        ParcoursPref(Gauche(A));
        // regarde l'arbre droit après gauche
        ParcoursPref(Droit(A));
    }
}


//parcour infixe
void ParcoursInf(PArbre A){
    //si l'arbre à une valeur 
    if (!EstArbreVide(A)){
        // regarde l'arbre gauche   
        ParcoursInf(Gauche(A));
        //ecrire la valeur 
        printf("%c|%d\n", Racine(A).letter, Racine(A).count);
        // regarde l'arbre droit après gauche
        ParcoursInf(Droit(A));
    }
}


//parcour sufixe
void ParcoursSuff(PArbre A){
    //si l'arbre à une valeur 
    if (!EstArbreVide(A)){
        // regarde l'arbre gauche   
        ParcoursSuff(Gauche(A));
        // regarde l'arbre droit après gauche
        ParcoursSuff(Droit(A));
        //ecrire la valeur à la fin
        printf("%c|%d\n", Racine(A).letter, Racine(A).count);
    }
}

