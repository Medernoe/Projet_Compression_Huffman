#include <stdio.h>
#include <stdlib.h>

#define TAILLE 1024

// lit tout le fichier dans un tableau qui grandit dynamiquement (par doublement)
char *lire_fichier(const char *nom) {
   // ouvrir le fichier en mode binaire (rb)
   FILE *f = fopen(nom, "rb");         
   if (f == NULL) return NULL;


   size_t capacite = TAILLE;
   size_t k = 0;
   char *contenu = malloc(capacite);
   if (contenu == NULL){
       fprintf(stderr, "Erreur allocation mémoire \n");
       exit(EXIT_FAILURE);
   }
   if (contenu == NULL) { fclose(f); return NULL; }
   // lire le fichier caractère par caractère, en stockant dans contenu[k]
   int c;
   // fgetc retourne un int pour pouvoir signaler EOF (end of file) = -1, d'où le type int de c
   while ((c = fgetc(f)) != EOF) {
       // on garde toujours une place libre pour le '\0' final -> test sur k + 1
       if (k + 1 >= capacite) {
           // Doublement de la capacité
           size_t nouvelle = capacite * 2;
           // Utilisation de realloc pour ajuster la taille du tableau
           char *temp = realloc(contenu, nouvelle);
           if (temp == NULL) {
               free(contenu);
               fclose(f);
               return NULL;
           }
           // Si realloc réussit, on met à jour le pointeur et la capacité
           contenu = temp;
           capacite = nouvelle;
       }
       // Stockage du caractère lu dans le tableau ((char) c pour éviter les warnings de conversion)
       contenu[k] = (char) c;
       k++;
   }
   // Termine la chaîne de caractères avec un '\0' pour indiquer la fin de la chaîne
   contenu[k] = '\0';
   // Une fois le processus terminé, la taille total du tableau est compté
   // Réallocation exact de l'espace mémoire dont le tableau a besoin
   contenu = realloc(contenu, k + 1);
   fclose(f);
   return contenu;
}


void print_binaire_ascii(char c) {
   // Itérer de 7 à 0 pour afficher les bits du caractère, en commençant par le bit le plus significatif
   for (int i = 7; i >= 0; i--)
   // On utilise un masque pour extraire chaque bit du caractère et l'afficher en '0' ou '1'
   // (c >> i) & 1 extrait le i-ème bit de c, et on affiche '1' si ce bit est 1, sinon '0'
       putchar( ((c >> i) & 1) ? '1' : '0' );
}


char lire_octet_ascii(const char *bits, size_t *pos) {
   char c = 0;
   // Itérer 8 fois pour lire les 8 bits d'un octet
   for (int i = 0; i < 8; i++) {
       // Décaler c d'un bit vers la gauche et ajouter le bit courant (converti de '0' ou '1' à 0 ou 1)
       c = (char) ((c << 1) | (bits[*pos] - '0'));
       // Avancer la position pour lire le bit suivant
       (*pos)++;
   }
   return c;
}
