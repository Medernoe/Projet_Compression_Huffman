#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhcode.h"
#include "utils.h"

// reconstruit l'arbre depuis l'en-tete (miroir de ecrire_entete)
// *pos avance dans la chaine de bits
PArbre lire_arbre(const char *bits, size_t *pos) {
    // lire le bit courant pour savoir si c'est une feuille ou un noeud interne
    char b = bits[*pos];
    (*pos)++;
    // Si 1 = c'est une feuille, lire les 8 bits suivants pour reconstituer le caractère
    if (b == '1') {                          
        char c = lire_octet_ascii(bits, pos);
        // construire une feuille avec ce caractère
        FreqElement e = { c, 0 };
        // construire un arbre feuille avec ce caractère et le retourner
        return Construire(e, NULL, NULL);
        // Sinon, c'est un noeud interne : reconstruire récursivement le gauche puis le droit
    } else {
        PArbre g = lire_arbre(bits, pos);
        PArbre d = lire_arbre(bits, pos);
        // construire un noeud interne avec une lettre quelconque (ex: '*') et retourner
        FreqElement e = { '*', 0 };
        return Construire(e, g, d);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier>\n", argv[0]);
        return 1;
    }
    char *bits = lire_fichier(argv[1]);
    if (bits == NULL) {
        fprintf(stderr, "Erreur : impossible de lire '%s'\n", argv[1]);
        return 1;
    }

    size_t pos = 0;

    // 1. reconstruire l'arbre depuis l'en-tete
    PArbre arbre = lire_arbre(bits, &pos);

    // Gère le cas ou l'arbre est reduit a une seule feuille (alphabet taille 1)
    if (Gauche(arbre) == NULL && Droit(arbre) == NULL) {
        while (bits[pos] != '\0') {
            if (bits[pos] == '0' || bits[pos] == '1') {
                putchar(Racine(arbre).letter);   // chaque bit = ce caractere unique
            }
            pos++;
        }
    }
    // Gère le cas du decodage normal (l'arbre a des branches)
    else {
      // decoder le corps en descendant dans l'arbre
      PArbre courant = arbre;
      // lire les bits un par un jusqu'à la fin de la chaine
      while (bits[pos] != '\0') {
        char b = bits[pos];
        pos++;
        // ignorer les bits qui ne sont pas '0' ou '1' (ex: \n a la fin de l'en-tete)
        if (b != '0' && b != '1') continue;
          courant = (b == '0') ? Gauche(courant) : Droit(courant);
          // si on arrive à une feuille, ecrire le caractere et repartir de la racine
          if (Gauche(courant) == NULL && Droit(courant) == NULL) {
            putchar(Racine(courant).letter);
            // repartir de la racine pour le prochain caractere
            courant = arbre;
          }
      }
    }

    liberer_arbre(arbre);
    free(bits);
    return 0;
}
