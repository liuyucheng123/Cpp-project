# Interpretation des regles (et ecarts assumés)

## 1. Geometrie du plateau

Une carte occupe un bloc de 2 x 2 cases ; ses quatre coins sont ces quatre cases.
Deux cartes posees en diagonale partagent exactement une case : c'est le recouvrement
d'un coin. Les cartes ne peuvent donc se toucher que par les coins, comme dans le jeu reel.

Consequences :
* une carte ne peut jamais recouvrir deux coins de la meme carte (ce serait deux cases partagees) ;
* les cases voisines en ligne droite sont interdites (elles partageraient deux cases) ;
* `ZoneDeJeu::positionsCandidates` ne propose que les quatre cases diagonales.

## 2. Pose d'une carte

1. au moins un des quatre coins de la nouvelle carte doit se superposer a un coin **visible**
   (`VIDE` ou portant un symbole) d'une carte deja posee ;
2. aucun coin `CACHE` ne peut etre recouvert ;
3. les quatre cases doivent etre libres ;
4. pour une carte doree jouee **face avant**, les ressources exigees doivent etre visibles
   *avant* la pose. La face arriere (4 coins vides + 1 ressource permanente) est toujours jouable,
   ce qui correspond a la regle "si le recto ne convient pas, on peut jouer la carte face verso".

## 3. Points immediats

* carte Ressource : 0 ou 1 point (banniere) ;
* carte Doree a points fixes : 3 ou 5 points ;
* carte Doree "par coin" : 2 points par coin recouvert par cette carte (1 a 4) ;
* carte Doree "par objet" : 1 point par symbole de l'objet indique visible au moment de la pose
  (le symbole de la carte elle-meme compte), et **pas de recalcul** ensuite.

## 4. Points d'objectifs (fin de partie)

| Type | Regle appliquee | Points |
|---|---|---|
| `DIAGONALE` | 3 cartes du royaume indique alignees en diagonale (deux orientations) | 2 par motif |
| `EN_L` | 3 cartes du royaume indique en L (deux pas diagonaux perpendiculaires, 4 orientations) | 3 par motif |
| `SERIE_RESSOURCE` | 3 symboles du royaume indique visibles | 2 par groupe de 3 |
| `PAIRES_OBJET` | 2 symboles du meme objet visibles | 2 par paire |
| `TRIPLET_OBJETS` | 1 plume + 1 encrier + 1 manuscrit visibles | 3 par triplet |

Une carte ne peut etre comptee qu'une fois **dans un meme objectif** : les motifs sont
selectionnes dans un ordre fixe (haut vers bas puis gauche vers droite) sans reutilisation.
Les trois objectifs (2 communs + 1 secret) sont evalues separement.

## 5. Fin de partie

La fin est declenchee des qu'un joueur atteint 20 points, ou quand les deux tas sont vides.
On termine la manche en cours, chaque joueur joue une derniere fois, puis on ajoute les points
d'objectifs. Le joueur avec le plus de points gagne ; en cas d'egalite, on compare les points
d'objectifs ; si l'egalite persiste, la victoire est partagee.

## 6. Ecarts assumes par rapport au jeu edite

1. **Mode solo** : implemente d'apres la variante officielle "Tybor Kwelein" (un adversaire
   sans main ni codex, qui prend une carte dans chacune des deux colonnes de la rangee
   visible). Deux points restent des interpretations : la correspondance entre les deux
   colonnes et les 4 cartes visibles du programme, et le fait que Tybor encaisse le nombre
   imprime sur une carte doree qu'il defausse (sans appliquer les multiplicateurs).
2. **Cartes esprits** : elles n'appartiennent pas au jeu de base, donc elles ne sont pas
   implementees ; l'ajout d'une famille de cartes est decrit dans `docs/ARCHITECTURE.md`
   (extension 3).
3. **Motif en L** : le motif est interprete comme trois cartes (deux pas diagonaux
   perpendiculaires). Si la carte officielle utilise quatre cartes, il suffit d'ajouter
   une sous-classe d'`Objectif` (voir `docs/ARCHITECTURE.md`, extension 2).
4. **Images** : l'affichage est textuel (les illustrations ne sont pas reproduites),
   ce qui est autorise par le sujet ("seul le fait de pouvoir jouer compte").
