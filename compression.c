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
            freq.length++; // On incrémente la longueur, stockée directement dans la structure

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

// Trie bulle inverse pour le tableau de frequence
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
Plsc frequency_to_lsc(PFreqObject freq){
    Plsc leaf_order = lsc_vide();

    if (freq == NULL || freq->array == NULL) {
        printf("Frequencies are empty.\n");
        return leaf_order;
    }
    
    // On boucle pour creer les arbres dans la lsc 
    for (size_t i = 0; i < freq->length; i++) {
        // On utilise pour ne pas déborder du tableau
        PArbre feuille = Construire(freq->array[i], NULL, NULL);
        lsc_insert_head(leaf_order, feuille);
    }
    return leaf_order;
}


// Extrait et supprime le noeud avec la plus petite occurrence parmi les têtes des deux listes
PArbre extraire_min(Plsc leaf_order, Plsc node) {
    PArbre head_leaf = lsc_head_value(leaf_order);
    PArbre head_node = lsc_head_value(node);

    // Si les deux listes sont vides
    if (head_leaf == NULL && head_node == NULL) return NULL;

    // Si leaf_order est vide, on prend dans node
    if (head_leaf == NULL) {
        lsc_del_head(node);
        return head_node;
    }

    // Si node est vide, on prend dans leaf_order
    if (head_node == NULL) {
        lsc_del_head(leaf_order);
        return head_leaf;
    }

    // Si les deux ont des valeurs, on compare les occurrences
    if (Racine(head_leaf).count <= Racine(head_node).count) {
        lsc_del_head(leaf_order);
        return head_leaf;
    } else {
        lsc_del_head(node);
        return head_node;
    }
}

// Fonction récursive de construction de l'arbre
PArbre Huffman_recursif(Plsc leaf_order, Plsc node) {
    // Condition d'arrêt : leaf_order est vide ET il ne reste qu'un seul élément dans node => fin 
    if (lsc_est_vide(leaf_order) && node->head != NULL && node->head->next == NULL) {
        PArbre final_root = lsc_head_value(node);
        lsc_del_head(node); // On nettoie la cellule
        return final_root;
    }
    
    // Condition d'arrêt : le mot n'avait qu'une seule lettre unique
    if (lsc_est_vide(node) && leaf_order->head != NULL && leaf_order->head->next == NULL) {
        PArbre final_root = lsc_head_value(leaf_order);
        lsc_del_head(leaf_order);
        return final_root;
    }

    // On extrait les deux plus petites valeurs (gauche puis droite)
    PArbre left = extraire_min(leaf_order, node);
    PArbre rigth = extraire_min(leaf_order, node);

    // Sécurité en cas de problème de logique
    if (left == NULL || rigth == NULL) return NULL;

    // On crée le nouveau noeud interne
    FreqElement e;
    e.letter = '*'; // Caractère arbitraire pour désigner un noeud interne
    e.count = Racine(left).count + Racine(rigth).count;

    // Le plus petit élément extrait (gauche) va à gauche, le 2ème va à droite
    PArbre new_node = Construire(e, left, rigth);

    // On insère ce nouveau noeud à la fin de la liste des noeuds internes
    lsc_insert_tail(node, new_node);

    // Appel récursif pour continuer la construction
    return Huffman_recursif(leaf_order, node);
}


PArbre Huffman_creation(const char* word) {
    // Si la chaîne est vide
    if (word == NULL || word[0] == '\0') return NULL;

    // Créer la liste de feuilles ordonnées
    FreqObject freq = frequency_count(word);
    frequency_order(&freq); 
    Plsc leaf_order = frequency_to_lsc(&freq);

    // Créer la lsc pour les noeuds internes
    Plsc node = lsc_vide(); 

    // Lancement de la construction récursive
    PArbre huffman_tree = Huffman_recursif(leaf_order, node);

    // Libération de la mémoire des structures temporaires (qui sont maintenant vides)
    free(leaf_order);
    free(node);
    free(freq.array);

    return huffman_tree;
}
