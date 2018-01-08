# Projet XOR Cipher
Algorithme de chiffrement/déchiffrement et décryptage XOR.
## Installation
Dans un terminal, taper la commande :
```
$ make
```
## Fonctionnalités
Le programme permet de :
- Chiffrer/déchiffrer un fichier quelconque à l'aide d'une clé de chiffrement donnée.
- Disposant d'un texte français chiffré et encodé en Latin-1 :
	- Afficher les caractères possibles pour la clé (ceux permettant d'obtenir un texte déchiffré valide) (mode 1)
	- Afficher la clé la plus probable en effectuant l'analyse fréquentielle des lettres des textes déchiffrés (mode 2)
	- Afficher la clé la plus probable en comptant le nombre de mots des textes déchiffrés présents dans le dictionnaire (mode 3)

## Usage
Le programme s'exécute en ligne de commande de deux manières différentes selon ce que l'on souhaite faire.
### Chiffrement/déchiffrement
Taper la commande :
```
$ ./xorcipher -i input_file -o output_file -k key
```
- `input_file` : nom du fichier à chiffrer/déchiffrer
- `output_file` : nom du fichier de sortie
- `key` : clé de chiffrement

### Décryptage
Taper la commande :
```
$ ./xorcipher -i input_file -m mode [-l key_length]
```
- `input_file` : nom du fichier à chiffrer/déchiffrer
- `mode` : mode choisi
- `key_length` (optionnel) : longueur de la clé
## Tests
Le projet dispose de plusieurs fichiers de test permettant de vérifier son bon fonctionnement.
### Scripts de test
Deux scripts bash sont disponibles pour tester le fonctionnement global du programme : `test_script.sh` et `test_crack.sh`.
### Tests unitaires
Les tests unitaires sont programmés dans le fichier `tests/modules/unit_tests.c`.
#### Exécution
Pour exécuter ces tests, il suffit de les compiler :
```
$ make test
```
Puis de lancer l'exécution :
```
$ ./test
```
#### Ajout de tests
Il est possible d'ajouter des tests dans le fichier `tests/modules/unit_tests.c`. Pour cela, il faut créer une fonction de test sans paramètre et qui ne renvoie rien, par exemple :
```C
void TEST_nom_de_la_fonction_a_tester(void) {
    /* Contenu du test */
}
```
Pour vérifier une assertion, il faut utiliser la macro `ASSERT()`, en passant la condition à tester, par exemple :
```C
ASSERT(resultat == 3);
```
Il faut ensuite ajouter cette fonction de test à la liste des fonctions à tester, qui se trouve dans le `main` :
```C
function functions_list[] = {
    /* ... */
    {"nom_de_la_fonction_à_tester", TEST_nom_de_la_fonction_a_tester},
    /* ... */
};
```
Enfin, il faut ajouter juste avant la fonction, le nom du module qu'elle teste, pour regrouper les tests par module :
```C
function functions_list[] = {
    /* ... */
    {"nom_du_module_testé", 0},
    {"nom_de_la_fonction_à_tester", TEST_nom_de_la_fonction_a_tester},
    /* ... */
};
```
