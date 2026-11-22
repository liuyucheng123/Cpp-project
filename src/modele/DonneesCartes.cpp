#include "modele/DonneesCartes.h"

namespace codex {
namespace {

DonneeFace face(Ressource hg, Ressource hd, Ressource bg, Ressource bd,
                int nbCentres, Ressource c0, Ressource c1, Ressource c2)
{
    DonneeFace f;
    f.coins[0] = hg;
    f.coins[1] = hd;
    f.coins[2] = bg;
    f.coins[3] = bd;
    f.centres[0] = c0;
    f.centres[1] = c1;
    f.centres[2] = c2;
    f.nbCentres = nbCentres;
    return f;
}

// Dos d'une carte Ressource ou Doree : quatre coins visibles et vides,
// plus une ressource permanente de son royaume au centre.
DonneeFace dosRessource(Ressource royaume)
{
    return face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE,
                1, royaume, Ressource::VIDE, Ressource::VIDE);
}

const DonneeCarte CARTES[] = {
    // ---- 40 cartes Ressource (identifiants 1 a 40, 10 par royaume) ----
    { 1, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CHAMPIGNON, Ressource::VIDE, Ressource::CHAMPIGNON, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 2, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CHAMPIGNON, Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 3, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::CACHE, Ressource::CHAMPIGNON, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 4, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::CHAMPIGNON, Ressource::VIDE, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 5, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::PLUME, Ressource::PLANTE, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 6, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::ENCRIER, Ressource::CHAMPIGNON, Ressource::CACHE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 7, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CHAMPIGNON, Ressource::INSECTE, Ressource::MANUSCRIT, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 8, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::CHAMPIGNON, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 9, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CHAMPIGNON, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 10, Ressource::CHAMPIGNON, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::VIDE, Ressource::CHAMPIGNON, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 11, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::PLANTE, Ressource::VIDE, Ressource::PLANTE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 12, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::PLANTE, Ressource::PLANTE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 13, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::CACHE, Ressource::PLANTE, Ressource::PLANTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 14, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::PLANTE, Ressource::VIDE, Ressource::PLANTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 15, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::INSECTE, Ressource::PLUME, Ressource::PLANTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 16, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::CHAMPIGNON, Ressource::PLANTE, Ressource::CACHE, Ressource::ENCRIER, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 17, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::MANUSCRIT, Ressource::CACHE, Ressource::PLANTE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 18, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::VIDE, Ressource::PLANTE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 19, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::PLANTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 20, Ressource::PLANTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::PLANTE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 21, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::ANIMAL, Ressource::ANIMAL, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 22, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::VIDE, Ressource::ANIMAL, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 23, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::ANIMAL, Ressource::CACHE, Ressource::ANIMAL, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 24, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::ANIMAL, Ressource::CACHE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 25, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::INSECTE, Ressource::ENCRIER, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 26, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::PLANTE, Ressource::ANIMAL, Ressource::CACHE, Ressource::MANUSCRIT, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 27, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::PLUME, Ressource::CACHE, Ressource::ANIMAL, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 28, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::VIDE, Ressource::ANIMAL, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 29, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 30, Ressource::ANIMAL, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::ANIMAL, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 31, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::INSECTE, Ressource::INSECTE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 32, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::VIDE, Ressource::INSECTE, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 33, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::INSECTE, Ressource::CACHE, Ressource::INSECTE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 34, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::INSECTE, Ressource::CACHE, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 35, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::PLUME, Ressource::ANIMAL, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 36, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::MANUSCRIT, Ressource::INSECTE, Ressource::CACHE, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 37, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::INSECTE, Ressource::PLANTE, Ressource::ENCRIER, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 38, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::INSECTE, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 39, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 40, Ressource::INSECTE, CATEGORIE_RESSOURCE, face(Ressource::CACHE, Ressource::INSECTE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },

    // ---- 40 cartes Doree (41 a 80) ----
    { 41, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, Ressource::PLUME, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::PAR_OBJET, Ressource::PLUME, 0, 1, 2, 0 },
    { 42, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::ENCRIER, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::PAR_OBJET, Ressource::ENCRIER, 1, 0, 2, 0 },
    { 43, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::MANUSCRIT, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 1, TypeScoreDore::PAR_OBJET, Ressource::MANUSCRIT, 0, 0, 2, 1 },
    { 44, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 1, 3, 0 },
    { 45, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 1, 0, 3, 0 },
    { 46, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 0, 3, 1 },
    { 47, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::ENCRIER, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 3, 0 },
    { 48, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::PLUME, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 3, 0 },
    { 49, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::MANUSCRIT, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 3, 0 },
    { 50, Ressource::CHAMPIGNON, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::CHAMPIGNON), 5, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 5, 0 },
    { 51, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::PLUME, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::PAR_OBJET, Ressource::PLUME, 2, 0, 0, 1 },
    { 52, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::MANUSCRIT, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::PAR_OBJET, Ressource::MANUSCRIT, 2, 0, 1, 0 },
    { 53, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::ENCRIER, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 1, TypeScoreDore::PAR_OBJET, Ressource::ENCRIER, 2, 1, 0, 0 },
    { 54, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 3, 0, 0, 1 },
    { 55, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 3, 1, 0, 0 },
    { 56, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 3, 0, 1, 0 },
    { 57, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::PLUME, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 3, 0, 0, 0 },
    { 58, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::MANUSCRIT, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 3, 0, 0, 0 },
    { 59, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::ENCRIER, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 3, 0, 0, 0 },
    { 60, Ressource::PLANTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::PLANTE), 5, TypeScoreDore::STATIQUE, Ressource::VIDE, 5, 0, 0, 0 },
    { 61, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::ENCRIER, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::PAR_OBJET, Ressource::ENCRIER, 0, 2, 0, 1 },
    { 62, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, Ressource::MANUSCRIT, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::PAR_OBJET, Ressource::MANUSCRIT, 1, 2, 0, 0 },
    { 63, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::PLUME, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 1, TypeScoreDore::PAR_OBJET, Ressource::PLUME, 0, 2, 1, 0 },
    { 64, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 3, 0, 1 },
    { 65, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 3, 1, 0 },
    { 66, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 1, 3, 0, 0 },
    { 67, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::MANUSCRIT, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 3, 0, 0 },
    { 68, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::ENCRIER, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 3, 0, 0 },
    { 69, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::CACHE, Ressource::PLUME, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 3, 0, 0 },
    { 70, Ressource::ANIMAL, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::ANIMAL), 5, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 5, 0, 0 },
    { 71, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::PLUME, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::PAR_OBJET, Ressource::PLUME, 1, 0, 0, 2 },
    { 72, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::MANUSCRIT, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::PAR_OBJET, Ressource::MANUSCRIT, 0, 1, 0, 2 },
    { 73, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, Ressource::ENCRIER, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 1, TypeScoreDore::PAR_OBJET, Ressource::ENCRIER, 0, 0, 1, 2 },
    { 74, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 1, 0, 3 },
    { 75, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 1, 0, 0, 3 },
    { 76, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::CACHE, Ressource::VIDE, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 2, TypeScoreDore::PAR_ANGLE, Ressource::VIDE, 0, 0, 1, 3 },
    { 77, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::ENCRIER, Ressource::CACHE, Ressource::VIDE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 0, 3 },
    { 78, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::MANUSCRIT, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 0, 3 },
    { 79, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::CACHE, Ressource::CACHE, Ressource::PLUME, Ressource::VIDE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 3, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 0, 3 },
    { 80, Ressource::INSECTE, CATEGORIE_DOREE, face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), dosRessource(Ressource::INSECTE), 5, TypeScoreDore::STATIQUE, Ressource::VIDE, 0, 0, 0, 5 },

    // ---- 6 cartes Initiales (81 a 86) ----
    { 81, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::CHAMPIGNON, Ressource::PLANTE, Ressource::INSECTE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::VIDE, Ressource::PLANTE, Ressource::INSECTE, Ressource::VIDE, 1, Ressource::INSECTE, Ressource::VIDE, Ressource::VIDE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 82, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::PLANTE, Ressource::ANIMAL, Ressource::CHAMPIGNON, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::ANIMAL, Ressource::VIDE, Ressource::VIDE, Ressource::CHAMPIGNON, 1, Ressource::CHAMPIGNON, Ressource::VIDE, Ressource::VIDE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 83, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::INSECTE, Ressource::ANIMAL, Ressource::CHAMPIGNON, Ressource::PLANTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, 2, Ressource::PLANTE, Ressource::CHAMPIGNON, Ressource::VIDE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 84, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::PLANTE, Ressource::INSECTE, Ressource::ANIMAL, Ressource::CHAMPIGNON, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE, 2, Ressource::ANIMAL, Ressource::INSECTE, Ressource::VIDE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 85, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::INSECTE, Ressource::CHAMPIGNON, Ressource::PLANTE, Ressource::ANIMAL, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 3, Ressource::ANIMAL, Ressource::INSECTE, Ressource::PLANTE), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
    { 86, Ressource::VIDE, CATEGORIE_INITIALE, face(Ressource::CHAMPIGNON, Ressource::ANIMAL, Ressource::PLANTE, Ressource::INSECTE, 0, Ressource::VIDE, Ressource::VIDE, Ressource::VIDE), face(Ressource::VIDE, Ressource::VIDE, Ressource::CACHE, Ressource::CACHE, 3, Ressource::PLANTE, Ressource::ANIMAL, Ressource::CHAMPIGNON), 0, TypeScoreDore::AUCUN, Ressource::VIDE, 0, 0, 0, 0 },
};

const DonneeObjectif OBJECTIFS[] = {
    { 87, TypeObjectif::DIAGONALE, 2, Ressource::CHAMPIGNON, Ressource::VIDE },
    { 88, TypeObjectif::DIAGONALE, 2, Ressource::PLANTE, Ressource::VIDE },
    { 89, TypeObjectif::DIAGONALE, 2, Ressource::ANIMAL, Ressource::VIDE },
    { 90, TypeObjectif::DIAGONALE, 2, Ressource::INSECTE, Ressource::VIDE },
    { 91, TypeObjectif::EN_L, 3, Ressource::CHAMPIGNON, Ressource::VIDE },
    { 92, TypeObjectif::EN_L, 3, Ressource::PLANTE, Ressource::VIDE },
    { 93, TypeObjectif::EN_L, 3, Ressource::ANIMAL, Ressource::VIDE },
    { 94, TypeObjectif::EN_L, 3, Ressource::INSECTE, Ressource::VIDE },
    { 95, TypeObjectif::SERIE_RESSOURCE, 2, Ressource::CHAMPIGNON, Ressource::VIDE },
    { 96, TypeObjectif::SERIE_RESSOURCE, 2, Ressource::PLANTE, Ressource::VIDE },
    { 97, TypeObjectif::SERIE_RESSOURCE, 2, Ressource::ANIMAL, Ressource::VIDE },
    { 98, TypeObjectif::SERIE_RESSOURCE, 2, Ressource::INSECTE, Ressource::VIDE },
    { 99, TypeObjectif::TRIPLET_OBJETS, 3, Ressource::VIDE, Ressource::VIDE },
    { 100, TypeObjectif::PAIRES_OBJET, 2, Ressource::VIDE, Ressource::MANUSCRIT },
    { 101, TypeObjectif::PAIRES_OBJET, 2, Ressource::VIDE, Ressource::ENCRIER },
    { 102, TypeObjectif::PAIRES_OBJET, 2, Ressource::VIDE, Ressource::PLUME },
};

const int NB_CARTES = static_cast<int>(sizeof(CARTES) / sizeof(CARTES[0]));
const int NB_OBJECTIFS = static_cast<int>(sizeof(OBJECTIFS) / sizeof(OBJECTIFS[0]));

} // namespace

const std::vector<DonneeCarte>& donneesCartes()
{
    static std::vector<DonneeCarte> table;
    if (table.empty()) {
        table.assign(CARTES, CARTES + NB_CARTES);
    }
    return table;
}

const std::vector<DonneeObjectif>& donneesObjectifs()
{
    static std::vector<DonneeObjectif> table;
    if (table.empty()) {
        table.assign(OBJECTIFS, OBJECTIFS + NB_OBJECTIFS);
    }
    return table;
}

} // namespace codex
