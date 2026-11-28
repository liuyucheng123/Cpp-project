# Architecture

## 1. Decoupage en couches

```
        +-------------------------------------------------------+
        |  main.cpp : analyse des options, choix du mode        |
        +----------------------------+--------------------------+
                                     |
        +----------------------------v--------------------------+
        |  controleur/  Menu, ControleurPartie, Saisie          |
        |               (AdaptateurFlux), TourAutomatique,       |
        |               SimulationIA                            |
        +----------------------------+--------------------------+
                                     |
        +----------------------------v--------------------------+
        |  vue/  Vue (interface) -- VueConsole                  |
        +----------------------------+--------------------------+
                                     |
        +----------------------------v--------------------------+
        |  modele/  Partie, Joueur, ZoneDeJeu, Pioche, Carte*,  |
        |           Objectif*, StrategieIA, CatalogueCartes     |
        +----------------------------+--------------------------+
                                     |
        +----------------------------v--------------------------+
        |  noyau/  Ressource, Coin, Coordonnee, Generateur,     |
        |          ExceptionJeu, Melange                        |
        +-------------------------------------------------------+

Dependances : main -> controleur -> {vue, modele} -> noyau.
Le modele ne connait ni la console ni les saisies : il ne fait aucun affichage.
```

## 2. Patrons de conception utilises (et pourquoi)

| Patron | Classes | Probleme resolu |
|---|---|---|
| **Template Method** | `Carte::pointsAuPose` (appelle `pointsFixes` + `pointsVariables`), `VueConsole` (trame d'affichage) | le deroulement est ecrit une fois, les variantes sont dans les sous-classes |
| **Strategy** | `StrategieIA` : `IAHasard`, `IAGloutonne`, `IAObjectifs` | changer d'algorithme d'IA sans toucher au moteur ; ajouter une IA = ajouter une classe |
| **Factory Method** | `FabriqueCartes` (`FabriqueCarteRessource`/`Doree`/`Initiale`), `FabriqueObjectif` (5 sous-classes) | isoler la creation des objets de leur utilisation |
| **Singleton** | `CatalogueCartes::instance()` | une seule base de 86 cartes + 16 objectifs pour toute l'application |
| **Iterator** | `IterateurCartes` (`premier`, `estFini`, `courant`, `suivant`) | parcourir la zone sans exposer `std::map` interne |
| **Adapter** | `AdaptateurFlux` : `std::istream` -> `Saisie` | le controleur lit au clavier ou dans un fichier sans changer de code |
| **Composite** | `Objectif` + `ObjectifCompose` | traiter une condition simple et une combinaison de conditions de la meme facon |

## 3. Points d'extension (argumentation du rapport)

1. **Nouvelle IA** : creer `IAExperte : StrategieIA`, l'ajouter dans `creerStrategie`
   (1 ligne) et dans le menu. Aucune modification du moteur.
2. **Nouveau type d'objectif** : creer `ObjectifXXX : Objectif` + `FabriqueObjectifXXX`,
   l'enregistrer dans le catalogue. Le calcul de fin de partie ne change pas.
3. **Nouvelle famille de cartes** (extension du jeu) : ajouter un `categorie` dans
   `DonneesCartes.h`, une classe `CarteXXX : Carte` et sa fabrique. `Partie` n'est pas modifie
   car il ne manipule que des `Carte` abstraites.
4. **Nouvelle presentation** (par exemple graphique) : implementer `Vue`, remplacer
   `VueConsole` dans `main`. Le modele reste identique.
5. **Sauvegarde de partie** : ajouter une `Memento` de `Partie` (non implemente, hors delai).
6. **Nouvel adversaire solo** : le mode officiel Tybor est deja implemente ainsi — classe
   `Tybor` (composition dans `Partie`, pas une `StrategieIA`, car Tybor n'a ni main ni codex)
   et entree `Partie::jouerTourTybor`. Un autre adversaire du meme genre se branche pareil.
