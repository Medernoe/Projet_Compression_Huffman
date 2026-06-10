# Algorithme de compression de Huffman

Projet réalisé dans le cadre de l'UE 2 - Algorithmique et Structures de Données du Master BIMS, Université de Rouen Normandie.

## Sommaire

- [Auteurs du projet](#auteurs-du-projet)
- [Introduction](#introduction)
- [Utilisation des fichiers de test](#utilisation-des-fichiers-de-test)
- [Les fichiers du programme](#les-fichiers-du-programme)
- [Configuration des structures](#configuration-des-structures)
- [Fonctionnement de `hcode`](#fonctionnement-de-hcode)
  - [I. Comptage des fréquences](#i-comptage-des-fréquences)
  - [II. Construction de l'arbre](#ii-construction-de-larbre)
  - [III. Génération du code à partir de l'arbre](#iii-génération-du-code-à-partir-de-larbre)
  - [IV. Codage de l'arbre (en-tête)](#iv-codage-de-larbre-en-tête)
  - [V. Codage du texte (corps)](#v-codage-du-texte-corps)
- [Format du fichier compressé (`.huf`)](#format-du-fichier-compressé-huf)
- [Fonctionnement de `unhcode`](#fonctionnement-de-unhcode)
  - [I. Décodage de l'en-tête et reconstruction de l'arbre](#i-décodage-de-len-tête-et-reconstruction-de-larbre)
  - [II. Décodage du corps](#ii-décodage-du-corps)
- [Choix de conception et justifications](#choix-de-conception-et-justifications)
- [Limites](#limites)
- [Tests](#tests)

------------------------------------------------------------------------------------------------------


## Auteurs du projet

Noé Mederlet : noe.mederlet@univ-rouen.fr
Mehdi Tachekort : mehdi.tachekort@univ-rouen.fr
github : https://github.com/Medernoe/Projet_Compression_Huffman

------------------------------------------------------------------------------------------------------

## Introduction

La compression de **Huffman** a pour objectif, à partir d'une séquence ou d'un texte, de remplacer les caractères les plus fréquents par des codes courts et les caractères les plus rares par des codes longs, au moyen d'un **code préfixe** représenté par un **arbre binaire**.

L'algorithme est codé en langage C et respecte les options de compilation suivantes : 
```
-std=c2x -Wall -Wconversion -Werror -Wextra -Wfatal-errors -Wpedantic -Wwrite-strings -O2
```

Le programme contient deux fichiers principaux : 

- **`hcode`** qui lit le fichier texte d'entrée et écrit sur la sortie standard sa compression sous forme d'une suite de **`0`** et de **`1`**. À la fin, le programme indique le taux de compression du texte.
- **`unhcode`** qui lit le fichier encodé à partir de **`hcode`** (suite de **`0`** et de **`1`**) et écrit sur la sortie standard le fichier décompressé.

Le texte décodé est identique à l'original.

------------------------------------------------------------------------------------------------------

## Utilisation des fichiers de test

```bash
make              # construit hcode et unhcode
./hcode  monTexte.txt > monTexte.huf                                            # compression
./unhcode monTexte.huf > monTexte.dcp                                           # decompression
diff monTexte.txt monTexte.dcp && echo "SÉQUENCE ENCODÉE ET DECODÉE SIMILAIRE"  # vérification
```

``` bash
./hcode  alphabet.txt > alphabet.huf     
./unhcode alphabet.huf > alphabet_decode.dcp
diff alphabet.txt alphabet_decode.dcp && echo "SÉQUENCE ENCODÉE ET DECODÉE SIMILAIRE"
```

``` bash
./hcode  ADN.txt > ADN.huf      
./unhcode ADN.huf > ADN_decode.dcp
diff ADN.txt ADN_decode.dcp && echo "SÉQUENCE ENCODÉE ET DECODÉE SIMILAIRE"
```

Le `Makefile` compile avec les options imposées :
`-std=c2x -Wall -Wconversion -Werror -Wextra -Wfatal-errors -Wpedantic -Wwrite-strings -O2`.

------------------------------------------------------------------------------------------------------

## Les fichiers du programme

- **`hcode`** : Programme d'encodage.
- **`unhcode`** : Programme de décodage.
- **`arbrebinaire`** : Type `PArbre` et fonctions de base sur les arbres binaires (création, accès, parcours, libération).
- **`lsc`** : Liste simplement chaînée (avec contrôleur `head`/`tail`) contenant des arbres ; utilisée pour la construction de Huffman.
- **`compression`** : Comptage des fréquences, tri, et construction de l'arbre de Huffman par la méthode des deux listes.
- **`utils`** : Lecture d'un fichier entier en mémoire (`lire_fichier`), écriture et lecture des 8 bits ASCII d'un caractère (`print_binaire_ascii` / `lire_octet_ascii`).

------------------------------------------------------------------------------------------------------

## Configuration des structures

**`FreqElement`** — un caractère et son nombre d'occurrences :

```c
typedef struct { char letter; int count; } FreqElement;
```

**`FreqObject`** — tableau dynamique de `FreqElement` (l'alphabet observé et ses fréquences).

**`PArbre`** — pointeur sur un noeud d'arbre binaire. Chaque noeud porte un `FreqElement` : pour une **feuille**, `letter` est le caractère et `count` son poids ; pour un **noeud interne**, `letter` vaut `'*'` (marqueur, jamais lu) et `count` est la somme des poids des deux sous-arbres.
Une feuille se reconnaît par `Gauche(A) == NULL && Droit(A) == NULL` (et non par le `'*'`, car le texte pourrait contenir un vrai `*`).

**`Plsc`** — liste simplement chaînée de `PArbre`, avec pointeurs de tête et de queue pour permettre une insertion en queue en temps constant.

------------------------------------------------------------------------------------------------------

## Fonctionnement de `hcode`

L'encodage suit les cinq étapes.

Pour représenter le fonctionnement du programme, toutes les étapes seront illustrées à travers la séquence **`abbraccaddabrra`** présente dans le fichier **`monTexte.txt`**.

### I. Comptage des fréquences
`frequency_count` parcourt le texte et construit le tableau des occurrences de chaque caractère (alphabet inconnu a priori).
```
letter : count 
  a    :   5
  b    :   3
  r    :   3
  c    :   2
  d    :   2
```

### II. Construction de l'arbre

On veut, à chaque étape, sélectionner les deux arbres de poids minimal en temps constant.
Pour cela on maintient **deux listes triées par poids croissant** :

- Une liste des **feuilles** (obtenue en triant les fréquences puis en remplissant la liste de sorte que la tête soit le plus petit poids).
- Une liste des **noeuds internes**, vide au départ.

La fonction `Huffman_recursif` du fichier `compression.c` répète les étapes suivantes :
- **extraire les deux plus petits poids** (forcément parmi les **têtes** des deux listes via `extraire_min`), 
- **fusionner** en un nouveau noeud dont le poids est la somme,
- **ajouter ce noeud en queue** de la liste des noeuds. Ce nouveau noeud a toujours un poids supérieur ou égal aux noeuds déjà créés, donc l'ajout en queue conserve la liste triée sans besoin de re-tri. La récursion s'arrête lorsqu'il ne reste qu'un seul arbre : c'est la racine.

Le plus petit des deux arbres extraits devient le fils **gauche**, le second le fils **droit**.

Voici ci-dessous la structure de l'arbre construit sur la séquence test :
```
            (15:*)
          0/      \1
      (6:*)       (9:*)
     0/   \1     0/    \1
    3:b   3:r  (4:*)    5:a
              0/   \1
             2:c   2:d
```

### III. Génération du code à partir de l'arbre

`construire_codes` parcourt l'arbre en accumulant le chemin depuis la racine (gauche = `0`, droite = `1`). À chaque feuille, le chemin courant est le code du caractère, rangé dans une table `codes[256]` indexée par le code ASCII de la lettre.

```
b = 00     r = 01     a = 11     c = 100    d = 101
```

### IV. Codage de l'arbre (en-tête)

`ecrire_entete` écrit la **forme de l'arbre** par un parcours préfixe :

- noeud interne → un bit `0` ;
- feuille → un bit `1` suivi des **8 bits du code ASCII** de la lettre.

```
noeud interne          -> 0
noeud interne          -> 0
feuille 'b'            -> 1 01100010
feuille 'r'            -> 1 01110010
noeud interne          -> 0
noeud interne          -> 0
feuille 'c'            -> 1 01100011
feuille 'd'            -> 1 01100100
feuille 'a'            -> 1 01100001

en-tete = 0010110001010111001000101100011101100100101100001   (49 bits)
```


### V. Codage du texte (corps)

On parcourt le texte et pour chaque caractère on écrit son code lu dans la table créée à l'étape III.

```
a    b    b    r    a    c     c     a    d     d     a    b    r    r    a
11   00   00   01   11   100   100   11   101   101   11   00   01   01   11

corps = 1100000111100100111011011100010111   (34 bits)
```

------------------------------------------------------------------------------------------------------

## Format du fichier compressé (`.huf`)

```
[ EN-TÊTE : forme de l'arbre ][ CORPS : message encodé ]
```

```
0010110001010111001000101100011101100100101100001 1100000111100100111011011100010111
└───────────── EN-TÊTE : 49 bits ──────────────┘ └──────────── CORPS : 34 bits ─────┘
```

Les deux parties sont **collées, sans séparateur** en une ligne, sans retour à la ligne.
L'en-tête est **auto-délimité** par sa structure avec `0` = noeud, `1` + 8 bits = feuille. Cela permet au décodeur de savoir exactement quand l'arbre est complet (lecture de la dernière feuille). À ce moment, la suite est forcément le corps.

---

## Fonctionnement de `unhcode`

Le décodage est le miroir exact de l'encodage et se fait en deux temps avec un seul indice de lecture qui avance dans la chaîne de bits :

### I. Décodage de l'en-tête et reconstruction de l'arbre

(`lire_arbre`, miroir de `ecrire_entete`) : un `0` crée un noeud interne dont on reconstruit récursivement le fils gauche puis le droit ; un `1` déclenche la lecture des 8 bits suivants pour recréer une feuille. La reconstruction consomme exactement les bits de l'en-tête.

```
0           -> noeud interne (on reconstruit ses deux fils)
0           -> noeud interne
1 01100010  -> feuille : 8 bits =  98 = 'b'
1 01110010  -> feuille : 8 bits = 114 = 'r'
0           -> noeud interne
0           -> noeud interne
1 01100011  -> feuille : 8 bits =  99 = 'c'
1 01100100  -> feuille : 8 bits = 100 = 'd'
1 01100001  -> feuille : 8 bits =  97 = 'a'
            -> arbre complet : 49 bits consommes, le corps commence ici
```

### II. Décodage du corps

On part de la racine de l'arbre recréé à l'étape I et on descend dans l'arbre bit par bit (`0` = gauche, `1` = droite) ; dès qu'on atteint une feuille, on écrit sa lettre et on repart de la racine. La propriété de **code préfixe** garantit qu'il n'y a aucune ambiguïté.

```
corps : 1100000111100100111011011100010111

11   -> 'a'     (racine -> droite -> droite)
00   -> 'b'     (racine -> gauche -> gauche)
00   -> 'b'
01   -> 'r'     (racine -> gauche -> droite)
11   -> 'a'
100  -> 'c'     (racine -> droite -> gauche -> gauche)
100  -> 'c'
11   -> 'a'
101  -> 'd'     (racine -> droite -> gauche -> droite)
101  -> 'd'
11   -> 'a'
00   -> 'b'
01   -> 'r'
01   -> 'r'
11   -> 'a'

texte décodé : abbraccaddabrra
```
---

## Choix de conception et justifications

- **Méthode des deux listes** : Pour construire l'arbre, les valeurs des feuilles et la construction des noeuds sont gérées par 2 listes. Cette méthode ramène la sélection des deux minimums à un test sur les têtes des listes. La `lsc` possède deux contrôleurs : `head` et `tail` ce qui rend l'ajout en queue possible. La complexité finale est O(n).
Une autre solution plus coûteuse consiste à placer toutes les feuilles dans un tableau d'arbres. Le tableau est parcouru à chaque étape, les deux arbres les plus petits sont retirés puis fusionnés et réinsérés sous la forme d'un noeud de valeur la somme des deux arbres. La liste étant entièrement reparcourue à chaque tour, la complexité est de O(n²).
- **Reconnaissance d'une feuille par sa structure** (`Gauche == NULL && Droit == NULL`) plutôt que par le caractère `'*'`. Cela permet de garder la condition toujours valide même avec un changement de design des noeuds.
- **Récursion terminale** pour la construction et la reconstruction de l'arbre : rend le code plus clair et élégant, équivalent à une boucle.
- **En-tête auto-délimité** : Évite de stocker une longueur ou un séparateur.
- **Lecture du fichier en mode binaire (`"rb"`)** : on compresse les octets exacts, ce qui garantit un aller-retour fidèle (un retour à la ligne, par exemple, est conservé tel quel).

---

## Limites

- **Surcoût de l'en-tête** : 9 bits par feuille. Sur un texte avec un alphabet proche de la taille de la séquence (par exemple les 26 lettres de l'alphabet, chacune une fois), l'en-tête domine et le code « compressé » est plus gros que l'original (taux négatif). Huffman n'est rentable que lorsque quelques caractères dominent.
- **Comptage des caractères unique** : Dans ADN.txt, énormément de fragment de plusieurs bases sont répétées en boucle et la compression de Huffman n'en tire pas profit. 

---

## Tests

Le programme a été testé sur des fichiers de tailles et de natures variées : l'exemple du sujet (`abbraccaddabrra`) dans monTexte.txt, l'alphabet entier (`abcdefghijklmnopqrstuvwxyz`) dans alphabet.txt et une séquence d'ADN (`TTACGGCTAGCTTACGGATCAGTACGTTAAGGCCTTAGACGATCG...`) dans ADN.txt pour représenter un cas d'usage biologique.
- Dans le cas de la séquence comprenant toutes les lettres de l'alphabet, la compression est contre-productive (**-84.1 %**), comme décrit dans les limites. 
Chaque lettre n'apparaît qu'une seule fois (taille de la séquence = taille de l'alphabet = 26). L'en-tête coûte 9 bits par feuille, soit 26 × 9 + 25 = 259 bits à lui seul, ce qui dépasse déjà les 208 bits du texte d'origine (26 × 8). Les codes du corps restent pourtant courts (4 à 5 bits par lettre), mais comme chaque lettre n'apparaît qu'une fois, son code court n'est jamais réutilisé : le coût de l'en-tête n'est jamais amorti. 
**La compression de Huffman n'est rentable que lorsque quelques caractères fréquents réutilisent abondamment leur code court.**
- Pour la séquence d'ADN, la compression est très efficace (**71.3 %**) : l'alphabet est court (4 bases) et la séquence longue (31 500 + les retours à la ligne).