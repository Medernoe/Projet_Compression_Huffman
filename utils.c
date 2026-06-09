#include <stdio.h>
#include <stdlib.h>

// lit tout le fichier d'un coup
char *lire_fichier(const char *nom) {
    FILE *f = fopen(nom, "rb");
    if (f == NULL) return NULL;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return NULL; }
    long taille = ftell(f);
    if (taille < 0) { fclose(f); return NULL; }
    rewind(f);
    char *contenu = malloc((size_t) taille + 1);
    if (contenu == NULL) { fclose(f); return NULL; }
    size_t lus = fread(contenu, 1, (size_t) taille, f);
    contenu[lus] = '\0';
    fclose(f);
    return contenu;
}

void print_binaire_ascii(char c) {
    for (int i = 7; i >= 0; i--)
        putchar( ((c >> i) & 1) ? '1' : '0' );
}

char lire_octet_ascii(const char *bits, size_t *pos) {
    char c = 0;
    for (int i = 0; i < 8; i++) {
        c = (char) ((c << 1) | (bits[*pos] - '0'));
        (*pos)++;
    }
    return c;
}
