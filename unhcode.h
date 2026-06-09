#ifndef UNHCODE_H
#define UNHCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "arbrebinaire.h"

/* ----------------------------------------------------------------------------
 *  lire_arbre
  Input : une chaine de bits representant l'en-tete d'un texte compresse par hcode
  Output : l'arbre de Huffman reconstruit a partir de l'en-tete
  Role :  A partir de la chaine de bits de l'en-tete, reconstruit l'arbre de Huffman
  qui a servi a compresser le texte. C'est le miroir exact de ecrire_entete, mais en LECTURE.
   - '1' -> feuille : lire les 8 bits suivants pour reconstituer le caractere ;
   - '0' -> noeud interne : reconstruire RECURSIVEMENT le gauche puis le droit
    (meme ordre prefixe que l'ecriture).
 * -------------------------------------------------------------------------- */
PArbre lire_arbre(const char *bits, size_t *pos);

#endif
