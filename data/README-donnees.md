# Donnees des cartes

`src/modele/DonneesCartes.cpp` contient la table complete du jeu, transcrite une fois pour
toutes en C++ (aucun fichier de donnees n'est lu a l'execution : pas de bibliotheque JSON).

## Contenu

| Famille | Nombre | Identifiants | Remarques |
|---|---|---|---|
| Cartes Ressource | 40 | 1 a 40 | 10 par royaume, dont 12 valent 1 point |
| Cartes Doree | 40 | 41 a 80 | 16 a points fixes (3 ou 5), 12 "par coin", 12 "par objet" |
| Cartes Initiales | 6 | 81 a 86 | recto et verso differents, ressources permanentes au verso |
| Cartes Objectif | 16 | 87 a 102 | 4 diagonales, 4 L, 4 series de ressources, 4 objets |

Chaque carte Ressource et Doree a un dos avec 4 coins visibles vides et une ressource
permanente de son royaume au centre (conforme a la regle du jeu).

## Codage d'un coin

```
P = plante      A = animal      C = champignon     I = insecte
f = plume       j = encrier     s = manuscrit
. = coin visible vide           # = coin absent (jamais recouvrable)
```

## Origine

Les valeurs (coins, exigences, points, objectifs) ont ete relevees sur les cartes du jeu
edite par Bombyx, puis verifiees par les tests `tests/test_cartes.cpp` :
86 cartes, 16 objectifs, 10 cartes par royaume, 12 cartes Ressource a 1 point,
16 cartes Doree a points fixes, 12 "par coin", 12 "par objet".
