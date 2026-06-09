# Compression de Huffman — `hcode` / `unhcode`

Projet d'Algorithmique et Structures de Données (Master BIMS 2, Université de Rouen Normandie).
Implémentation de la compression de **Huffman** en C : remplacer les caractères fréquents par des codes courts et les caractères rares par des codes longs, au travers d'un **code préfixe** représenté par un arbre binaire.

Le projet contient deux programmes :

- **`hcode`** : lit un fichier texte et écrit sur la sortie standard son codage sous forme d'une suite de `0` et de `1` (au format caractère).
À la fin du programme, le taux de compression est indiqué.
- **`unhcode`** : lit un fichier de codage (suite de `0` et de `1`) et écrit sur la sortie standard.
Le texte décodé, identique à l'original.

---

## Compilation et utilisation

```bash
make              # construit hcode et unhcode
./hcode  monTexte.txt > monTexte.huf      # compression
./unhcode monTexte.huf                    # decompression
```
``` bash
./hcode  alphabet.txt > alphabet.huf     
./unhcode alphabet.huf
```
``` bash
./hcode  ADN.txt > ADN.huf      
./unhcode ADN.huf                    
```

Le `Makefile` compile avec les options imposées :
`-std=c2x -Wall -Wconversion -Werror -Wextra -Wfatal-errors -Wpedantic -Wwrite-strings -O2`.

---

## Organisation des fichiers

| Fichier | Rôle |
|---|---|
| `utils.c/.h` | Lecture d'un fichier entier en mémoire (`lire_fichier`) et écriture des 8 bits ASCII d'un caractère (`print_binaire_ascii`). |
| `arbrebinaire.c/.h` | Type `PArbre` et fonctions de base sur les arbres binaires (création, accès, parcours, libération). |
| `lsc.c/.h` | Liste simplement chaînée (avec contrôleur `head`/`tail`) contenant des arbres ; utilisée pour la construction de Huffman. |
| `compression.c/.h` | Comptage des fréquences, tri, et construction de l'arbre de Huffman par la méthode des deux listes. |
| `hcode.c` | Programme d'encodage. |
| `unhcode.c` | Programme de décodage. |

---

## Structures de données

**`FreqElement`** — un caractère et son nombre d'occurrences :

```c
typedef struct { char letter; int count; } FreqElement;
```

**`FreqObject`** — tableau dynamique de `FreqElement` (l'alphabet observé et ses fréquences).

**`PArbre`** — pointeur sur un noeud d'arbre binaire. Chaque noeud porte un `FreqElement` : pour une **feuille**, `letter` est le caractère et `count` son poids ; pour un **noeud interne**, `letter` vaut `'*'` (marqueur, jamais lu) et `count` est la somme des poids des deux sous-arbres.
Une feuille se reconnaît par `Gauche(A) == NULL && Droit(A) == NULL` (et non par le `'*'`, car le texte pourrait contenir un vrai `*`).

**`Plsc`** — liste simplement chaînée de `PArbre`, avec pointeurs de tête et de queue pour permettre une insertion en queue en temps constant.

---

## Phase d'encodage (`hcode`)

L'encodage suit les cinq étapes du sujet.

Pour représenter le fonctionnement des algorithmes, toutes les étapes seront illutré à travers la séquence **`abbraccaddabrra`**.

### 1. Comptage des fréquences
`frequency_count` parcourt le texte et construit le tableau des occurrences de chaque caractère (alphabet inconnu a priori).
```
letter | count 
      a|5
      b|3
      r|3
      c|2
      d|2
```

### 2. Construction de l'arbre — méthode des deux listes
On veut, à chaque étape, sélectionner les deux arbres de poids minimal en temps constant.
Pour cela on maintient **deux listes triées par poids croissant** :

- la liste des **feuilles** (obtenue en triant les fréquences puis en remplissant la liste de sorte que la tête soit le plus petit poids) ;
- la liste des **noeuds internes**, vide au départ.

`Huffman_recursif` répète : extraire les deux plus petits poids (forcément parmi les **têtes** des deux listes — d'où une sélection en O(1) via `extraire_min`), les fusionner en un nouveau noeud dont le poids est la somme, et **ajouter ce noeud en queue** de la liste des noeuds. 
Ce nouveau noeud a toujours un poids supérieur ou égal aux noeuds déjà créés, donc l'ajout en queue conserve la liste triée sans aucun re-tri. La récursion s'arrête quand il ne reste qu'un seul arbre : c'est la racine.

Le plus petit des deux arbres extraits devient le fils **gauche**, le second le fils droit.

```
            (15:*)
          0/      \1
      (6:*)       (9:*)
     0/   \1     0/    \1
    3:r   3:b  (4:*)    5:a
               0/   \1
              2:d   2:c
```

### 3. Génération des codes
`construire_codes` parcourt l'arbre en accumulant le chemin depuis la racine
(gauche = `0`, droite = `1`). À chaque feuille, le chemin courant est le code du
caractère, rangé dans une table `codes[256]` indexée par le code ASCII de la lettre.

```
r = 00     b = 01     a = 11     d = 100    c = 101
```

### 4. Codage de l'arbre (en-tête)
`ecrire_entete` écrit la **forme de l'arbre** par un parcours préfixe :

- noeud interne → un bit `0` ;
- feuille → un bit `1` suivi des **8 bits du code ASCII** de la lettre.

```
noeud interne          -> 0
noeud interne          -> 0
feuille 'r'            -> 1 01110010
feuille 'b'            -> 1 01100010
noeud interne          -> 0
noeud interne          -> 0
feuille 'd'            -> 1 01100100
feuille 'c'            -> 1 01100011
feuille 'a'            -> 1 01100001

en-tete = 0010111001010110001000101100100101100011101100001   (49 bits)
```


### 5. Codage du texte (corps)
On parcourt le texte caractère par caractère et on écrit, pour chacun, son code lu dans
la table.

```
a    b    b    r    a    c     c     a    d     d     a    b    r    r    a
11   01   01   00   11   101   101   11   100   100   11   01   00   00   11

corps = 1101010011101101111001001101000011   (34 bits)
```

---

## Format du fichier compressé (`.huf`)

```
[ EN-TÊTE : forme de l'arbre ][ CORPS : message encodé ]
```

Les deux parties sont **collées, sans séparateur** et sans retour à la ligne final.
Aucun marqueur n'est nécessaire : l'en-tête est **auto-délimité**. Sa grammaire
(`0` = noeud, `1` + 8 bits = feuille) permet au décodeur de savoir exactement quand
l'arbre est complet — à ce moment précis, la suite est forcément le corps.

---

## Phase de décodage (`unhcode`)

Le décodage est le miroir exact de l'encodage et se fait en deux temps avec un seul
indice de lecture qui avance dans la chaîne de bits :

1. **Reconstruction de l'arbre** (`lire_arbre`, miroir de `ecrire_entete`) : un `0` crée
   un noeud interne dont on reconstruit récursivement le fils gauche puis le droit ; un `1`
   déclenche la lecture des 8 bits suivants pour recréer une feuille. La reconstruction
   consomme exactement les bits de l'en-tête.
2. **Décodage du texte** : on part de la racine et on descend dans l'arbre bit par bit
   (`0` = gauche, `1` = droite) ; dès qu'on atteint une feuille, on écrit sa lettre et on
   repart de la racine. La propriété de **code préfixe** garantit qu'il n'y a aucune ambiguïté.

---

## Choix de conception et justifications

- **Méthode des deux listes** : Pour construire l'arbre, les valeurs des feuilles et la construction des noeuds est géré par 2 listes. Cette méthode ramène la sélection des deux minimums à un test sur les têtes des listes. La `lsc` possède deux contrôleurs : `head` et `tail` ce qui rend l'ajout en queue possible. La complexité finale est O(n).
Une autre solution plus couteuse consiste à placer toutes les feuilles dans un tableau d'arbres. Le tableau est parcouru à chaque étape, les deux arbres les plus petits sont retirés puis fusionnés et réinsérés sous la forme d'un noeud de valeur de la somme des deux arbres. La liste étant entièrement reparcouru à chaque tour la complexitée est de O(n²).
- **Reconnaissance d'une feuille par sa structure** (`Gauche == NULL && Droit == NULL`) plutôt que par le caractère `'*'`. Cela permet de garder la condition toujours valide même avec un changement de design des noeuds.
- **Récursion terminale** pour la construction et la reconstruction de l'arbre : rend le code plus clair et élégant, équivalent à une boucle.
- **En-tête auto-délimité** : Évite de stocker une longueur ou un séparateur.
- **Lecture du fichier en mode binaire (`"rb"`)** : on compresse les octets exacts, ce qui garantit un aller-retour fidèle (un retour à la ligne, par exemple, est conservé tel quel).

---

## Limites

- **Surcoût de l'en-tête** : 9 bits par feuille. Sur un texte avec un alphabet proche de la taille de la séquence, (par exemple les 26 lettres de l'alphabet, chacune une fois), l'en-tête domine et le code « compressé » est plus gros que l'original (taux négatif). Huffman n'est rentable que lorsque quelques caractères dominent.
- **Caractère unique** : un fichier ne contenant qu'un seul caractère distinct (`aaaa`) est traité comme un cas particulier (la feuille-racine reçoit le code `0`).

---

## Tests

Le programme a été testé sur des fichiers de tailles et de natures variées : l'exemple du sujet (`abbraccaddabrra`) dans monTexte.txt. L'alphabet entier (`abcdefghijklmnopqrstuvwxyz`) dans alphabet.txt et une séquence d'ADN dans ADN.txt pour représenter un cas d'usage biologique.
- Dans le cas de la séquence comprenant toutes les lettres de l'alphabet, la compression est contre productive (**-84.1%**) comme décrit dans les limites. La taille du texte est égale à la taille de l'alphabet (taille de la séquence = taille de l'alphabet = 26). L'en-tête coûte 9 bits par feuille pour reconstituer l'arbre en plus de l'encodage de chaque lettre de la séquence (4 à 5 bits) et on perd l'utilité de compresser un char de 8 bits. Comme chaque lettre apparait qu'une seule fois, le compression des lettres n'est utilisées qu'une seule fois et le coût de l'en-tête n'est jamais amortis.
- Pour la séquence d'ADN la compression est très efficace (**71.3%**), l'alphabet est court (4 lettres) et la séquence longue (taille 700).