#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lsc.h"
#include "arbrebinaire.h"

// Creer une lsc vide 
Plsc lsc_vide(void) {
    Plsc l = malloc(sizeof *l);
    if (l == NULL) {
        fprintf(stderr, "Erreur malloc!\n");
        return NULL;
    }
    l->head = NULL;
    l->tail = NULL; 
    return l;
}

//retourne vrai si la liste est vide
bool lsc_est_vide(Plsc p){
    return (p == NULL) || (p->head == NULL);
}

//permet d'inserer une cellule en tail (controleur)
int lsc_insert_tail(Plsc p, PArbre tree_node) {
    // verifie que le pointeur est non nul
    if (p == NULL) {
        fprintf(stderr, "Erreur : liste NULL !\n");
        return -1;
    }

    // Alloue la mémoire et creer pour la nouvelle cellule 
    lsc_cell *cell = malloc(sizeof(*cell));
    if (cell == NULL) {
        fprintf(stderr, "Erreur malloc!\n");
        return -1;
    }
    cell->tree_node = tree_node;
    cell->next = NULL; // C'est le dernier élément

    // Si la liste est vide, on l'ajoute en tête
    if (lsc_est_vide(p)) {
        p->head = cell;
        p->tail = cell;
        return 0;
    }

    // Sinon à la fin
    p->tail->next = cell;
    p->tail = cell;
    return 0;
}

//fonction de suppression de cell en head 
int lsc_del_head(Plsc p){
    if (lsc_est_vide(p)) {
        fprintf(stderr, "la liste est vide\n");
        return -1;
    }

    lsc_cell *pointeur = p->head->next;
    free(p->head);
    p->head = pointeur;
    if (p->head == NULL) {
        p->tail = NULL;
    }
    return 0;
}

//fonction de suppression d'une lsc et liberation mémoire  
void lsc_dispose(Plsc *pp){
    if(*pp == NULL){
        printf("La liste n'existe pas");
        return;
    }   
    //*pp = p
    lsc_cell *cell = (*pp)->head;
    while (cell != NULL){
        lsc_cell *pointeur = cell->next; 
        free(cell);
        cell = pointeur;
    }
    //va a l'adresse de *pp et supprime les élements en mémoire
    free(*pp);
    *pp=NULL;
}

// donne l'arbre de head
PArbre lsc_head_value(Plsc p){
    if (lsc_est_vide(p)) return NULL;
    lsc_cell* pointeur = p->head;
    return pointeur->tree_node;
}

// donne l'arbre de tail 
PArbre lsc_tail_value(Plsc p){
    if (lsc_est_vide(p)) return NULL;
    lsc_cell* pointeur = p->tail;
    return pointeur->tree_node;
}

