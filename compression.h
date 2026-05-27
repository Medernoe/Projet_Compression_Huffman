#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Déclaration incomplète pour lsc afin d'éviter une inclusion circulaire */
typedef struct lsc lsc;

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

// -----------------------------------------------------------------------------
// Fonctions

/* count_frequency
- Input : char* word 
- Output : FreqObject
- Role : Parcourt un mot d'alphabet inconnu et construit un tableau d'occurrences des lettres 
*/
FreqObject count_frequency(const char* word); 

/* order_frequency
- Input : PFreqObject freq 
- Role : Trie le tableau dans l'ordre croissant des occurrences
*/
void order_frequency(PFreqObject freq);

/* print_frequency
- Input : PFreqObject freq 
- Role : Ecrit le tableau des occurences
*/
void print_frequency(PFreqObject freq); 

/* frequency_to_lsc
- Input : PFreqObject freq 
- Output : lsc* 
- Role :  Convertie un tableau de frequence en lsc 
*/
lsc* frequency_to_lsc(PFreqObject freq); 

#endif