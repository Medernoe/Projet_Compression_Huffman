#ifndef HCODE_H
#define HCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "arbrebinaire.h"
#include "utils.h"

/* ----------------------------------------------------------------------------
// Input : un texte a compresser
// Output : le texte compresse (en binaire) sur la sortie standard
// Role : compresse le texte en utilisant l'algorithme de Huffman.
// 1. construire l'arbre de Huffman a partir du texte
// 2. construire la table de codes a partir de l'arbre
// 3. ecrire l'en-tete (la forme de l'arbre) sur la sortie standard
// 4. ecrire le corps : pour chaque lettre du texte, ecrire son code binaire sur la sortie standard
// 5. afficher le taux de compression
// -------------------------------------------------------------------------- */
void ecrire_entete(PArbre A);

/* ----------------------------------------------------------------------------
 *  construire_codes
 *  Input : un arbre de Huffman construit a partir d'un texte
 *  Output : une table de codes : codes[lettre] = "00", "101"...
 *  Role :  A partir de l'arbre de Huffman, construit la table de codes pour chaque lettre.
 *  1. Parcours de l'arbre : chemin de la racine a chaque feuille = code de la lettre
 *  2. A chaque noeud interne : '0' pour aller a gauche, '1' pour aller a droite
 * -------------------------------------------------------------------------- */
void construire_codes(PArbre A, char *chemin, int profondeur, char *codes[256]);

#endif 
