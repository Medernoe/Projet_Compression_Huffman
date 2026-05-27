#ifndef _LISTES_
#define _LISTES_

#include <stdbool.h>
#include "compression.h"

typedef struct lsc_cell {
    FreqElement array;
    struct lsc_cell *next;
} lsc_cell;

typedef
struct lsc{
    lsc_cell *head;
}lsc; 

/*lsc_vide : 
- Entrée : Vide
- Création d'une liste simplement chainée vide
- Sortie : un lsc contenant un pointeur sur un controleur vide*/
lsc *lsc_vide();

/*lsc_est_vide : 
- Entrée : Une liste simplement chainée lsc
- Vérifie si l'adresse dans head est nul ou si elle pointe sur une cellule
- Sortie : True/False selon la présence ou non d'une première cellule*/
bool lsc_est_vide(lsc *p);

/*lsc_insert_head : 
- Entrée : Une liste simplement chainée lsc + un entier
- Creer une cellule avec la valeur value, si la création de la cellule ce passe mal
retourne -1, sinon ajoute cette cellule en entete après head. Retourne 0 si réussi. 
- Sortie : modification d'une lsc avec l'ajout d'une cellule contenant value*/
int lsc_insert_head(lsc *p, FreqElement value);

/*lsc_print : 
- Entrée : Une liste simplement chainée lsc 
- Parcours la lsc et ecrit les valeurs de chaque cellule
- Sortie : Aucune (ecrit dans la sortie terminale)*/
void lsc_print(lsc *p);

/*lsc_del_head(lsc *p): 
- Entrée : Une liste simplement chainée lsc 
- Supprime la première cellule de la chaine et raccorde le controleur à la cellule 2 
- Sortie : modification d'une lsc avec suppression de la cellule une*/
void lsc_del_head(lsc *p);

/*lsc_dispose: 
- Entrée : Un pointeur sur une liste simplement chainée lsc
- Supprime la lsc en supprimant l'allocation memoire de chaque cellule
- Sortie : Aucune*/
void lsc_dispose(lsc **pp);

/*lsc_head_value(lsc *p): 
- Entrée : Une liste simplement chainée lsc 
- Donne le première element de la chaine
- Sortie : FreqElement*/
FreqElement lsc_head_value(lsc* p);


#endif
