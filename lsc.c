#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lsc.h"

lsc *lsc_vide() {
    lsc *l = malloc(sizeof *l);
    if (l == NULL) {
        fprintf(stderr, "Erreur malloc!\n");
        return NULL;
    }
    l->head = NULL;
    return l;
}

//retourne vrai si la liste est vide
bool lsc_est_vide(lsc *p){
    return (p == NULL) || (p->head == NULL);
}

//permet d'inserer une cellule après head (controleur)
int lsc_insert_head(lsc *p, FreqElement array){
    lsc_cell *cell = malloc(sizeof*cell);
        if (cell==NULL){
        fprintf(stderr, "Erreur malloc!\n"); 
        return -1;
    }
    cell->array=array; 
    cell->next=p->head;
    p->head=cell;
    return 0;
}

//parcour et ecrit une liste 
void lsc_print(lsc *p){
    if (lsc_est_vide(p)) return;

    lsc_cell* pointeur = p->head;
    while (pointeur != NULL) {
        printf("%c|%d\n",
               pointeur->array.letter,
               pointeur->array.count);
        pointeur = pointeur->next;
    }
}

//fonction de suppression de cell en head 
void lsc_del_head(lsc *p){
    if (p->head != NULL) { 
        lsc_cell *pointeur = p->head->next;  
        free(p->head);
        p->head = pointeur;
    }else {
        printf("la liste est vide\n");
    }
}

void lsc_dispose(lsc **pp){
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

FreqElement lsc_head_value(lsc* p){
    if (lsc_est_vide(p)) return {NULL, 0};
    lsc_cell* pointeur = p->head;
    return pointeur->array;
}