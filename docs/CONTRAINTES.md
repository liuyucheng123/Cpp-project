# Portee du cours : ce qui est autorise et ce qui ne l'est pas

Le projet n'utilise que des notions explicitement traitees dans le polycopie LO02
(`LO02_Poly_cours_A25.pdf`, 133 pages). Ce fichier sert de preuve de portee : chaque
fonctionnalite utilisee renvoie a la partie du cours correspondante.

## 1. Elements du langage utilises

| Element | Ou dans le projet | Cours |
|---|---|---|
| Classes, attributs, methodes, encapsulation | `src/modele/*` | Ch. 2 (classes, diagramme UML) |
| Constructeurs avec liste d'initialisation | `Joueur`, `CartePlacee`, `Pioche` | Ch. 2 (naissance d'un objet) |
| Destructeur virtuel | `Carte`, `Objectif`, `Vue`, `Saisie` | Ch. 2 (mort d'un objet) |
| Membres et methodes `static` | `CatalogueCartes::instance` | Ch. 2 (membres statiques) |
| Surcharge d'operateurs (`==`, `<`, `<<`) | `Coordonnee`, `Ressource` | Ch. 2 (surcharge des operateurs) |
| Heritage public, redefinition, `override` | `CarteRessource`, `CarteDoree`, `CarteInitiale`, `VueConsole` | Ch. 3 (hierarchies de classes) |
| Methodes virtuelles pures, classes abstraites | `Carte`, `Objectif`, `StrategieIA`, `Vue`, `Saisie` | Ch. 3 (classes abstraites) |
| Polymorphisme et stockage heterogene | `std::vector<std::shared_ptr<const Carte>>` | Ch. 3 (polymorphisme) |
| Exceptions personnalisees (`try/catch/throw`) | `src/noyau/ExceptionJeu.*` | Ch. 2 (detection des erreurs) |
| Pointeurs intelligents | `shared_ptr` partout, `unique_ptr` dans `main` | Ch. 2 (pointeurs intelligents) |
| Modeles de fonctions et de classes (`template`) | `melanger` (patron de fonction) | Ch. 4 (programmation generique) |
| Conteneurs et algorithmes STL | `vector`, `map`, `set`, `std::swap`, iterateurs | Ch. 5 (STL) |
| `std::string`, flux `iostream`/`iomanip` | affichage console, saisies | Ch. 5 (bibliotheque standard) |
| `std::ifstream`/fichiers | `--entree fichier` | Ch. 5 (elements sur les flux) |
| `enum class`, `auto`, `decltype`, `nullptr`, `= default`, `explicit` | tout le projet | Ch. 1 et Ch. 2 |
| Espaces de noms | `namespace codex` | Ch. 1 (espaces de noms) |
| `assert`-libre : harnais maison dans `tests/` | `tests/harnais.h` | Ch. 1 (compilation separee) |

## 2. Elements volontairement NON utilises

Ces elements ne figurent pas dans le polycopie ; ils sont donc interdits par le groupe
et `scripts/check_scope.ps1` les detecte automatiquement :

```
lambda (capture ou non)          std::function             std::optional / std::variant / std::any
std::string_view                 std::filesystem           ranges / concepts / modules / coroutines
structured bindings              if constexpr              expressions de pliage
std::thread / std::async / mutex std::regex                std::chrono / std::format
printf / malloc / new brut       goto                      toute bibliotheque tierce
```

## 3. Consequence : des choix de conception "a l'ancienne"

* Pas de lambda : les strategies et les callbacks sont des **objets** (`StrategieIA`),
  ce qui est justement l'usage du patron Strategie montre en cours.
* Pas de `std::function` : les algorithmes sont appeles par interface virtuelle.
* Pas de `new` brut : toutes les allocations passent par `shared_ptr`/`unique_ptr`.
* Pas de bibliotheque de test : `tests/harnais.h` tient en 40 lignes et n'utilise que
  des `template` et des fonctions `inline`.

## 4. Comment verifier

```powershell
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1   # 0 occurrence attendue
g++ -std=c++14 -Wall -Wextra -pedantic -Isrc src/*.cpp             # 0 avertissement attendu
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc src/*.cpp             # doit aussi compiler
```
