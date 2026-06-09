#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h> 
#include "compression.h"
#include "arbrebinaire.h"
#include "lsc.h"


int main(void) {
    FreqObject freq = frequency_count("abbraccaddabrra");

    printf("\n Classic Version : \n");

    frequency_print(&freq);

    frequency_order(&freq); 
    
    printf("\n Order Version : \n");
    
    frequency_print(&freq);

    lsc* leaf_order = frequency_to_lsc(&freq); 

    printf("\nlsc version : \n"); 

    lsc_print(leaf_order);

    free(freq.array);

    lsc_dispose(&leaf_order);

    const char* word = "abbraccaddabrra";
    printf("========================================\n");
    printf("CREATION ARBRE HUFFMAN\n");
    printf("Mot a compresser : %s\n\n", word);
    
    PArbre huffman_tree = Huffman_creation(word);

    printf("\n========================================\n");
    printf("PARCOURS ARBRE FINAL \n");

    // Affichage de l'arbre final pour vérifier sa structure
    printf("\nParcours Prefixe (Racine -> Gauche -> Droite) :\n");
    ParcoursPref(huffman_tree);

    printf("\nParcours Infixe (Gauche -> Racine -> Droite) :\n");
    ParcoursInf(huffman_tree);

    return 0;
}
