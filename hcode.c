#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hcode.h"
#include "compression.h"
#include "utils.h"

void ecrire_entete(PArbre A) {
    if (EstArbreVide(A)) return;
    // Si feuille
    if (Gauche(A) == NULL && Droit(A) == NULL) {
        // '1' + 8 bits ASCII de la lettre
        putchar('1');
        print_binaire_ascii(Racine(A).letter);
        // Sinon, naviguer dans le noeud interne (Gauche puis Droit)
    } else {                                        
        putchar('0');
        ecrire_entete(Gauche(A));
        ecrire_entete(Droit(A));
    }
}

void construire_codes(PArbre A, char *chemin, int profondeur, char *codes[256]) {
    if (EstArbreVide(A)) return;

    // Si feuille
    if (Gauche(A) == NULL && Droit(A) == NULL) {
        // cas spécial : si la feuille est aussi la racine, lui donner le code "0"
            if (profondeur == 0) {          
                chemin[0] = '0';
                chemin[1] = '\0';
            } else {
                // terminer le chemin et stocker une copie dans codes[lettre]
                chemin[profondeur] = '\0';
            }
        // lettre = code ASCII de la feuille
        unsigned char lettre = (unsigned char) Racine(A).letter;
        // allocation mémoire pour le code de la lettre (strlen(chemin) + 1 pour le \0)
        codes[lettre] = malloc(strlen(chemin) + 1);
        // copier le chemin dans codes[lettre]
        strcpy(codes[lettre], chemin);
    } else {
        // Sinon naviguer dans le noeud interne (Gauche puis Droit)
        chemin[profondeur] = '0';                           
        construire_codes(Gauche(A), chemin, profondeur + 1, codes);
        chemin[profondeur] = '1';                          
        construire_codes(Droit(A),  chemin, profondeur + 1, codes);
    }
}

int main(int argc, char *argv[]) {
    // 1. un nom de fichier est obligatoire
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier>\n", argv[0]);
        return 1;
    }

    // 2. lecture du texte a compresser
    char *texte = lire_fichier(argv[1]);

    // Gère le cas ou le fichier est vide (texte == NULL) pour éviter les erreurs de segmentation
    if (texte == NULL) {
        fprintf(stderr, "Erreur : impossible de lire '%s'\n", argv[1]);
        return 1;
    }
    // Gère le cas ou le fichier est vide (texte[0] == '\0') pour éviter les erreurs de segmentation
    if (texte[0] == '\0') {
        fprintf(stderr, "Fichier vide : rien a compresser.\n");
        free(texte);
        return 0;
    }


    // 3. construction de l'arbre avec la fonction Huffman_creation qui gère la
    //    frequences, tri, listes de feuilles/noeuds, fusion recursive)
    PArbre arbre = Huffman_creation(texte);

    // 4. table des codes (lettre -> chaine de bits)
    char *codes[256] = {0};      // toutes les cases a NULL
    char chemin[256];            // 256 >= profondeur maximale possible
    construire_codes(arbre, chemin, 0, codes);

    // 5a. EN-TETE : la forme de l'arbre
    ecrire_entete(arbre);

    // 5b. CORPS : chaque caractere du texte est remplace par son code.
    //     bits_corps ne sert qu'a COMPTER (pour le taux) ; c'est fputs qui ecrit.
    size_t bits_corps = 0;
    for (size_t i = 0; texte[i] != '\0'; i++) {
        // le caractère courant est converti en code ASCII (0-255) pour indexer la table des codes
        unsigned char l = (unsigned char) texte[i];
        // ecrire le code de la lettre sur la sortie standard
        fputs(codes[l], stdout);
        // compter le nombre de bits ecrits pour le corps (longueur du code de la lettre)
        bits_corps += strlen(codes[l]);
    }

    // 6. taux de compression sur stderr (pour ne pas écrire .huf).
    // Initialisation de L a 0 avant la boucle pour compter les codes non nuls.
    size_t L = 0;
    // Compter le nombre de codes non nuls dans la table des codes pour calculer L
    for (int i = 0; i < 256; i++) if (codes[i] != NULL) L++;
    // Calcul du taux de compression : (1 - taille_compressee / taille_originale) * 100
    size_t bits_entete = 9 * L + (L - 1);
    // taille_compressee = bits_entete + bits_corps
    size_t total   = bits_entete + bits_corps;
    // taille_originale = strlen(texte) * 8 (bits)
    size_t origine = strlen(texte) * 8;
    // Calcul du taux de compression en pourcentage (double pour éviter la division entière)
    double taux = (1.0 - (double) total / (double) origine) * 100.0;
    // Affichage du taux de compression avec une décimale
    fprintf(stderr, "Taux de compression : %.1f %%\n", taux);

    // 7. liberation memoire
    for (int i = 0; i < 256; i++) free(codes[i]);
    liberer_arbre(arbre);
    free(texte);
    return 0;
}
