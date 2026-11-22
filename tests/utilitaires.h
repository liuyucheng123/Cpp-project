#ifndef CODEX_TESTS_UTILITAIRES_H
#define CODEX_TESTS_UTILITAIRES_H

#include "modele/Carte.h"
#include "modele/DonneesCartes.h"
#include "modele/FabriqueCartes.h"
#include "noyau/Coin.h"

#include <memory>

namespace tests {

// Construit une face sur mesure : pratique pour fabriquer des cartes de test.
inline codex::DonneeFace faceTest(codex::Ressource hg, codex::Ressource hd,
                                  codex::Ressource bg, codex::Ressource bd,
                                  int nbCentres = 0,
                                  codex::Ressource c0 = codex::Ressource::VIDE,
                                  codex::Ressource c1 = codex::Ressource::VIDE,
                                  codex::Ressource c2 = codex::Ressource::VIDE)
{
    codex::DonneeFace face;
    face.coins[0] = hg;
    face.coins[1] = hd;
    face.coins[2] = bg;
    face.coins[3] = bd;
    face.centres[0] = c0;
    face.centres[1] = c1;
    face.centres[2] = c2;
    face.nbCentres = nbCentres;
    return face;
}

inline std::shared_ptr<const codex::Carte> carteTest(int identifiant, codex::Ressource royaume,
                                                     const codex::DonneeFace& recto,
                                                     const codex::DonneeFace& verso)
{
    codex::DonneeCarte donnee;
    donnee.id = identifiant;
    donnee.royaume = royaume;
    donnee.categorie = codex::CATEGORIE_RESSOURCE;
    donnee.recto = recto;
    donnee.verso = verso;
    donnee.score = 0;
    donnee.typeScore = codex::TypeScoreDore::AUCUN;
    donnee.objetScore = codex::Ressource::VIDE;
    for (int i = 0; i < 4; ++i) {
        donnee.exigences[i] = 0;
    }
    codex::FabriqueCarteRessource fabrique;
    return fabrique.fabriquer(donnee);
}

// Carte dont les quatre coins sont vides : elle peut toujours etre posee.
inline std::shared_ptr<const codex::Carte> carteNeutre(int identifiant)
{
    return carteTest(identifiant, codex::Ressource::PLANTE,
                     faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                              codex::Ressource::VIDE, codex::Ressource::VIDE),
                     faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                              codex::Ressource::VIDE, codex::Ressource::VIDE));
}

// Carte dont le coin bas-droit est cache : elle ne peut pas etre recouverte par ce coin.
inline std::shared_ptr<const codex::Carte> carteCoinCache(int identifiant)
{
    return carteTest(identifiant, codex::Ressource::PLANTE,
                     faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                              codex::Ressource::VIDE, codex::Ressource::CACHE),
                     faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                              codex::Ressource::VIDE, codex::Ressource::CACHE));
}

} // namespace tests

#endif // CODEX_TESTS_UTILITAIRES_H
