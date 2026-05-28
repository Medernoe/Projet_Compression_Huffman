#ifndef _LISTES_
#define _LISTES_

#include <stdbool.h>
#include "compression.h"
#include "arbrebinaire.h"

typedef struct lsc_cell {
    PArbre tree_node;
    struct lsc_cell* next;
} lsc_cell;

typedef struct lsc{
    lsc_cell* head;
    lsc_cell* tail;
}lsc; 

typedef struct lsc* Plsc;

/*lsc_vide : 
- Entrée : Vide
- Création d'une liste simplement chainée vide
- Sortie : un lsc contenant un pointeur sur un controleur vide*/
lsc *lsc_vide();

/*lsc_est_vide : 
- Entrée : Une liste simplement chainée lsc
- Vérifie si l'adresse dans head est nul ou si elle pointe sur une cellule
- Sortie : True/False selon la présence ou non d'une première cellule*/
bool lsc_est_vide(Plsc p);

/*lsc_insert_head : 
- Entrée : Une liste simplement chainée lsc + un entier
- Creer une cellule avec la valeur value, si la création de la cellule ce passe mal
retourne -1, sinon ajoute cette cellule en entete après head. Retourne 0 si réussi. 
- Sortie : modification d'une lsc avec l'ajout d'une cellule contenant value*/
int lsc_insert_head(Plsc p, PArbre value);


// void ? 
int lsc_insert_tail(Plsc p, PArbre tree_node);

/*lsc_print : 
- Entrée : Une liste simplement chainée lsc 
- Parcours la lsc et ecrit les valeurs de chaque cellule
- Sortie : Aucune (ecrit dans la sortie terminale)*/
void lsc_print(Plsc p);

/*lsc_del_head(lsc *p): 
- Entrée : Une liste simplement chainée lsc 
- Supprime la première cellule de la chaine et raccorde le controleur à la cellule 2 
- Sortie : modification d'une lsc avec suppression de la cellule une*/
int lsc_del_head(Plsc p);

/*lsc_dispose: 
- Entrée : Un pointeur sur une liste simplement chainée lsc
- Supprime la lsc en supprimant l'allocation memoire de chaque cellule
- Sortie : Aucune*/
void lsc_dispose(Plsc* pp);

/*lsc_head_value(lsc *p): 
- Entrée : Une liste simplement chainée lsc 
- Donne le première element de la chaine
- Sortie : PArbre*/
PArbre lsc_head_value(Plsc p);

/*lsc_tail_value(lsc *p): 
- Entrée : Une liste simplement chainée lsc 
- Donne le dernier element de la chaine
- Sortie : PArbre*/
PArbre lsc_tail_value(Plsc p);



#endif
