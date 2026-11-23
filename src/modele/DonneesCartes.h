#ifndef CODEX_DONNEES_CARTES_H
#define CODEX_DONNEES_CARTES_H

#include "noyau/Ressource.h"

#include <vector>

namespace codex {

// Description d'une face de carte : quatre coins + ressources permanentes du centre.
// L'ordre des coins est HAUT_GAUCHE, HAUT_DROIT, BAS_GAUCHE, BAS_DROIT.
struct DonneeFace
{
    Ressource coins[4];
    Ressource centres[3];
    int nbCentres;
};

// Facon dont une carte Doree rapporte ses points au moment ou elle est posee.
enum class TypeScoreDore
{
    AUCUN,
    STATIQUE,
    PAR_ANGLE,
    PAR_OBJET
};

// Categorie de carte : les trois familles du jeu.
enum
{
    CATEGORIE_RESSOURCE = 0,
    CATEGORIE_DOREE = 1,
    CATEGORIE_INITIALE = 2
};

struct DonneeCarte
{
    int id;
    Ressource royaume;
    int categorie;
    DonneeFace recto;
    DonneeFace verso;
    int score;
    TypeScoreDore typeScore;
    Ressource objetScore;
    int exigences[4];
};

// Objectifs de fin de partie (16 cartes).
enum class TypeObjectif
{
    DIAGONALE,
    EN_L,
    SERIE_RESSOURCE,
    PAIRES_OBJET,
    TRIPLET_OBJETS
};

struct DonneeObjectif
{
    int id;
    TypeObjectif type;
    int score;
    Ressource royaume;
    Ressource objet;
};

const std::vector<DonneeCarte>& donneesCartes();
const std::vector<DonneeObjectif>& donneesObjectifs();

} // namespace codex

#endif // CODEX_DONNEES_CARTES_H
