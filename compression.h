#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

// -----------------------------------------------------------------------------
// Structures

// Structure pour les occurrences de caractères
typedef struct {
    char letter;
    int count;
} FreqElement;

// FreqObject contient le pointeur vers le tableau de fréquence et la taille de l'alphabet
typedef struct { 
    FreqElement* array; 
    size_t length; 
} FreqObject;

// PFreqObject est un pointeur sur un objet de fréquence
typedef FreqObject* PFreqObject;


struct noeud;
typedef struct noeud* PArbre;

struct lsc;
typedef struct lsc* Plsc;

// -----------------------------------------------------------------------------
// Fonctions

/* frequency_count
- Input : char* word 
- Output : FreqObject
- Role : Parcourt un mot d'alphabet inconnu et construit un tableau d'occurrences des lettres 
*/
FreqObject frequency_count(const char* word); 

/* frequency_order
- Input : PFreqObject freq 
- Role : Trie le tableau dans l'ordre decroissant des occurrences
*/
void frequency_order(PFreqObject freq);

/* frequency_print
- Input : PFreqObject freq 
- Role : Ecrit le tableau des occurences
*/
void frequency_print(PFreqObject freq); 

/* frequency_to_lsc
- Input : PFreqObject freq 
- Output : Plsc 
- Role :  Convertie un tableau de frequence en lsc 
*/
Plsc frequency_to_lsc(PFreqObject freq); 

/* extraire_min
- Input : Plsc leaf_order, Plsc node 
- Output : PArbre 
- Role : Compare les valeurs de d'occurences dans des lsc d'arbre et ressort un arbre de la plus 
petite occurences. 
*/
PArbre extraire_min(Plsc leaf_order, Plsc node);


/* Huffman_recursif
- Input : Plsc leaf_order, Plsc node 
- Output : PArbre 
- Role : construit par reccursité en utilisant extraire min, un arbre de Huffman avec min a gauche
*/
PArbre Huffman_recursif(Plsc leaf_order, Plsc node); 

/* Huffman_creation
- Input :const char* word
- Output : PArbre 
- Role : Appel l'ensemble des fonctions pour construit un arbre de huffman
*/PArbre Huffman_creation(const char* word); 
#endif
