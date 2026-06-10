#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

/*
- lire_fichier
- Input : const char *nom (nom du fichier à lire)
- Output : char* (contenu du fichier sous forme de chaîne de caractères)
- Role : Lit le contenu d'un fichier par allocation dynamique et le stocke dans une chaîne de caractères.
*/
char *lire_fichier(const char *nom);


/*  
- print_binaire_ascii
- Input : char c (caractère à afficher en binaire)
- Output : void (affiche le caractère en binaire sur la sortie standard)
- Role : Affiche les 8 bits ASCII d'un caractère donné. Chaque bit est affiché sous forme de '0' ou '1'. Le bit le plus significatif est affiché en premier.
*/
void print_binaire_ascii(char c);


/**    
- Input : const char *bits (chaîne de bits)
- Output : char (octet lu)
- Role : Lit un octet à partir d'une chaîne de bits et met à jour la position.
 */
char lire_octet_ascii(const char *bits, size_t *pos);

#endif
