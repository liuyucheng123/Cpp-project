# Codex Naturalis - projet LO02 (A26)

Adaptation console du jeu de cartes **Codex Naturalis** (Thomas Dupont / Bombyx), realisee
avec les seules notions du cours LO02 : classes, heritage, polymorphisme, classes abstraites,
exceptions, pointeurs intelligents, templates, STL et les patrons de conception vus en cours.

## 快速开始 / Démarrage rapide

```powershell
# Windows, MinGW (GCC) deja installe (fourni avec CLion)
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
.build\codex_naturalis.exe            # partie interactive
.build\codex_naturalis.exe --aide     # options
```

En cas d'ouverture dans CLion : ouvrir le dossier `code`, la configuration CMake est fournie.

## Options de la ligne de commande

| Option | Effet |
|---|---|
| `--graine N` | graine du hasard (defaut 2026) : deux parties avec la meme graine sont identiques |
| `--auto N` | joue N parties entierement automatiques (verification rapide) |
| `--joueurs N` | nombre de joueurs du mode auto (1 a 4) |
| `--niveau N` | niveau des IA du mode auto (1 hasard, 2 glouton, 3 objectifs) |
| `--entree F` | rejoue les reponses ecrites dans le fichier F (demonstration, tests) |
| `--aide` | affiche l'aide |

Exemple de verification complete :

```powershell
.build\codex_naturalis.exe --auto 20 --joueurs 3 --niveau 3 --graine 1
```

## Regles implementees

* 1 a 4 joueurs (les sieges peuvent etre humains ou ordinateurs).
* Mise en place : carte initiale (choix de la face), 3 cartes en main, 2 objectifs communs,
  choix d'un objectif secret parmi 2.
* Tour : poser une carte (en recouvrant au moins un coin visible d'une carte deja posee,
  jamais deux coins de la meme carte), puis piocher (4 cartes visibles ou un des deux tas).
* Cartes doree : ressources exigees pour jouer la face avant ; la face arriere reste jouable.
* Points immediats (banniere, coins recouverts, objets) puis points d'objectifs en fin de partie.
* Fin de partie a 20 points ou quand les deux tas sont vides ; on termine la manche,
  chaque joueur rejoue une derniere fois, puis on ajoute les objectifs.

Les details et les choix d'interpretation sont dans `docs/REGLES_OBJECTIFS.md`.

## Organisation du code

```
src/noyau/       types de base (Ressource, Coordonnee, generateur aleatoire, exceptions)
src/modele/      regles du jeu : cartes, zone, objectifs, pioche, joueurs, moteur (Partie)
src/vue/         affichage (Vue abstraite + VueConsole)
src/controleur/  saisies, menu, controleur de partie, IA automatique, simulation
tests/           tests unitaires sans bibliotheque externe
docs/            contraintes de portee, architecture, regles des objectifs
```

Le modele ne fait aucune entree/sortie : c'est ce qui permet de le tester et de changer
l'affichage sans le modifier.

## Tests

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build.ps1   # compile et lance les tests
.build\codex_tests.exe                                        # tests seuls
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1   # verifie la portee du cours
```

## Portee du cours

Aucune fonctionnalite hors cours n'est utilisee (pas de lambda, pas de `std::function`,
pas de `optional`/`variant`/`filesystem`, pas de bibliotheque externe). La liste complete
et sa correspondance avec les diapositives du cours sont dans `docs/CONTRAINTES.md`.
