#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "compression.h"
#include "lsc.h"
#include "arbrebinaire.h"


// Fonctiond de comptage des occurences 
FreqObject frequency_count(const char* word) {
    // En cas d'erreur, on prépare une structure vide à renvoyer
    FreqObject empty_array = {NULL, 0};

    // Vérifie si le mot existe et n'est pas vide 
    if (word == NULL || word[0] == '\0') {
        printf("Word is empty.\n");
        return empty_array; 
    }

    // Initialisation de l'objet FreqObject
    FreqObject freq;
    freq.array = NULL;
    freq.length = 0;

    for (int i = 0; word[i] != '\0'; i++) {
        char cur = word[i];
        bool unknown = true;

        // Vérification si la lettre a déjà été observée
        for (size_t j = 0; j < freq.length; j++) {
            if (freq.array[j].letter == cur) {
                freq.array[j].count++; // si déja observé alors on ajoute une occurence
                unknown = false;
                break; 
            }
        }

        // Si la lettre n'existe pas dans le tableau de fréquence
        if (unknown) {
            freq.length++; // On incrémente la longueur stockée directement dans la structure

            // On réalloue uniquement le pointeur interne .array
            FreqElement* temp = realloc(freq.array, freq.length * sizeof(FreqElement));
            if (temp == NULL) {
                printf("Memory allocation failed.\n");
                free(freq.array); // Libère ce qui a déjà été alloué
                return empty_array;
            }
            freq.array = temp;

            // Ajout des éléments dans la dernière case
            freq.array[freq.length - 1].letter = cur;
            freq.array[freq.length - 1].count = 1;
        }
    }
    return freq;
}

/* order_frequency
- Input : FreqObject* freq 
- Role : Trie le tableau (trie bulle) dans l'ordre decroissant des occurrences, 
necessaire pour creer une lsc de feuille directement dans l'ordre croissant 
*/
void frequency_order(PFreqObject freq) {
    // Vérifie si le pointeur est NULL, ou le tableau vide / à 1 élément, rien à trier
    if (freq == NULL || freq->array == NULL || freq->length <= 1) {
        return;
    }

    size_t len = freq->length; 

    // Algorithme du tri à bulles
    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = 0; j < len - i - 1; j++) {
            
            // Accès au tableau interne de l'objet : freq->array[j]
            if (freq->array[j].count < freq->array[j + 1].count) {
                
                // Échange des structures FreqElement
                FreqElement temp = freq->array[j];
                freq->array[j] = freq->array[j + 1];
                freq->array[j + 1] = temp;
            }
        }
    }
}


// Ecrit un tableau de frequence 
void frequency_print(PFreqObject freq) {
    // Vérifie si le pointeur est NULL, ou le tableau vide 
    if (freq == NULL || freq->array == NULL) {
        printf("Frequencies are empty.\n");
        return;
    }

    printf("=====================\n");
    printf("letter | count \n");
    size_t i = 0;
    while (i < freq->length) {
        printf("%c|%d\n", freq->array[i].letter, freq->array[i].count);
        i++;
    }
    printf("=====================\n");
}


// Creer une lsc pour les feuilles dans l'ordre croissant  
lsc* frequency_to_lsc(PFreqObject freq){
    lsc* leaf_order = lsc_vide();

    // Vérifie si le pointeur est NULL, ou le tableau vide 
    if (freq == NULL || freq->array == NULL) {
        printf("Frequencies are empty.\n");
        return leaf_order;
    }
    
    for (size_t i = 0; i < freq->length; i++) {
        lsc_insert_head(leaf_order, freq->array[i]);
    }
    return leaf_order;
} 


PArbre Huffman_creation(char* word){
    PFreqObject freq = frequency_count(word);
    freq = frequency_order(&freq); 

    lsc* leaf_order = frequency_to_lsc(&freq); 
    lsc* node_order = lsc_vide; 


    PArbre first_letter = Construire(lsc_head_value(leaf_order), 
                                                    NULL,
                                                    NULL); 

    lsc_del_head(leaf_order); 
    PArbre second_letter = Construire(lsc_head_value(leaf_order), 
                                                    NULL, 
                                                    NULL); 
    
    lsc_insert_head(node_order, Racine(first_letter)->value + Racine(second_letter)->value)  
    PArbre first_node = Construire({Racine(first_letter)->value + Racine(second_letter)->value, NULL}, 
                                    NULL, 
                                    NULL);
    

    for(size_t i = 0, i < freq->length-2, i++){


    }


}