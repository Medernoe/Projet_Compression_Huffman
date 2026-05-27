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


element Racine(PArbre A){
    //verifie que l'abre n'est pas vide 
    if (EstArbreVide(A)){
        return 0;
    }

    //retourne la première valeur 
    return A->value; 
}    


//renvoie le sous arbre gauche de l'arbre
PArbre Gauche(PArbre A){
    if (EstArbreVide(A)){
        return 0;
    }
    return A->g;
}     

//renvoie le sous arbre droit de l'arbre
PArbre Droit(PArbre A){
    if (EstArbreVide(A)){
        return 0;
    }
    return A->d;
}     



// renvoie un arbre construit avec l'element e comme racine, g son sous arbre gauche et d son sous arbre droit
PArbre Construire(element e, PArbre g, PArbre d){
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
        printf("%d\n",Racine(A));
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
        printf("%d\n",Racine(A));
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
        printf("%d\n",Racine(A));
    }
}


//insère l'élément dans l'abr et renvoie l'abr obtenu
PArbre Insertion(element v, PArbre A){
    // arbre vide = nouvel arbre 
    if (EstArbreVide(A)){
        //retourner un arbre construit à partir de l'element
        A = Construire(v, NULL, NULL);
        return A;
    }

    //insertion valeurs inférieur à racine
    if(v < Racine(A)){
        //on recommence sur sous arbre gauche 
        A->g = Insertion(v, Gauche(A)); 
    }
    //insertion valeurs superieur à racine
    else{
        if(v > Racine(A)){
            //on recommence sur sous arbre droit 
            A->d = Insertion(v,Droit(A)); 
        }
    }
    return A; 
}


//renvoie vrai si l'élément est présent dans l'abr, faux sinon
bool Recherche(element v, PArbre A){
    // arbre vide = valeur non présente
    if (EstArbreVide(A)){
        return false;
    }
    
    //recherche valeur est la racine 
    if(v == Racine(A)){
        return true; 
    }
    
    else{
        //recherche valeur plus faible que racine 
        if(v < Racine(A)){
            //on recommence sur sous arbre gauche 
            return Recherche(v, Gauche(A)); 
        }
        else{
            return Recherche(v, Droit(A));
        }
    }
}


//renvoie vrai si l'élément est présent dans l'abr, faux sinon
element Max(PArbre A){
    if (EstArbreVide(A)){
        printf("Arbre vide\n");
        return Racine(A);
    }
    else{
        return Max(Droit(A));
    }
}