#include "harnais.h"
#include "modele/Carte.h"
#include "modele/Objectif.h"
#include "modele/ZoneDeJeu.h"
#include "utilitaires.h"

#include <memory>

namespace {

std::shared_ptr<const codex::Carte> carteRoyaume(int identifiant, codex::Ressource royaume)
{
    return tests::carteTest(identifiant, royaume,
                            tests::faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                                            codex::Ressource::VIDE, codex::Ressource::VIDE),
                            tests::faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                                            codex::Ressource::VIDE, codex::Ressource::VIDE));
}

std::shared_ptr<const codex::Carte> carteCoins(int identifiant, codex::Ressource hg,
                                               codex::Ressource hd, codex::Ressource bg,
                                               codex::Ressource bd)
{
    return tests::carteTest(identifiant, codex::Ressource::PLANTE,
                            tests::faceTest(hg, hd, bg, bd),
                            tests::faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                                            codex::Ressource::VIDE, codex::Ressource::VIDE));
}

} // namespace

void testerObjectifs()
{
    codex::ZoneDeJeu diagonale;
    diagonale.poser(codex::Coordonnee(0, 0), carteRoyaume(1, codex::Ressource::PLANTE), true, 0);
    diagonale.poser(codex::Coordonnee(1, 1), carteRoyaume(2, codex::Ressource::PLANTE), true, 1);
    diagonale.poser(codex::Coordonnee(2, 2), carteRoyaume(3, codex::Ressource::PLANTE), true, 1);

    codex::ObjectifMotif objectifDiagonale(87, codex::Ressource::PLANTE, 2, true);
    codex::ObjectifMotif objectifL(91, codex::Ressource::PLANTE, 3, false);
    VERIFIER_EGAL(2, objectifDiagonale.evaluer(diagonale), "diagonale de 3 plantes = 2 points");
    VERIFIER_EGAL(0, objectifL.evaluer(diagonale), "aucun L sur une diagonale");

    codex::ZoneDeJeu enL;
    enL.poser(codex::Coordonnee(0, 0), carteRoyaume(4, codex::Ressource::PLANTE), true, 0);
    enL.poser(codex::Coordonnee(1, 1), carteRoyaume(5, codex::Ressource::PLANTE), true, 1);
    enL.poser(codex::Coordonnee(2, 0), carteRoyaume(6, codex::Ressource::PLANTE), true, 1);
    VERIFIER_EGAL(3, objectifL.evaluer(enL), "L de 3 plantes = 3 points");
    VERIFIER_EGAL(0, objectifDiagonale.evaluer(enL), "aucune diagonale sur un L");

    codex::ZoneDeJeu melange;
    melange.poser(codex::Coordonnee(0, 0), carteRoyaume(7, codex::Ressource::PLANTE), true, 0);
    melange.poser(codex::Coordonnee(1, 1), carteRoyaume(8, codex::Ressource::ANIMAL), true, 1);
    melange.poser(codex::Coordonnee(2, 2), carteRoyaume(9, codex::Ressource::PLANTE), true, 1);
    VERIFIER_EGAL(0, objectifDiagonale.evaluer(melange), "pas de diagonale si les royaumes diffèrent");

    codex::ZoneDeJeu serie;
    serie.poser(codex::Coordonnee(0, 0),
                carteCoins(10, codex::Ressource::PLANTE, codex::Ressource::PLANTE,
                           codex::Ressource::PLANTE, codex::Ressource::VIDE),
                true, 0);
    codex::ObjectifSerieRessource objectifSerie(95, codex::Ressource::PLANTE, 2);
    VERIFIER_EGAL(2, objectifSerie.evaluer(serie), "3 symboles plante = 2 points");

    codex::ZoneDeJeu objets;
    objets.poser(codex::Coordonnee(0, 0),
                 carteCoins(11, codex::Ressource::PLUME, codex::Ressource::ENCRIER,
                            codex::Ressource::MANUSCRIT, codex::Ressource::VIDE),
                 true, 0);
    objets.poser(codex::Coordonnee(1, 1),
                 carteCoins(12, codex::Ressource::PLUME, codex::Ressource::VIDE,
                            codex::Ressource::VIDE, codex::Ressource::VIDE),
                 true, 1);

    codex::ObjectifPairesObjet objectifPaires(100, codex::Ressource::PLUME, 2);
    VERIFIER_EGAL(2, objectifPaires.evaluer(objets), "2 plumes = 2 points");
    codex::ObjectifPairesObjet objectifEncriers(101, codex::Ressource::ENCRIER, 2);
    VERIFIER_EGAL(0, objectifEncriers.evaluer(objets), "une seule encrier = 0 point");
    codex::ObjectifTripletObjets objectifTriplet(99, 3);
    VERIFIER_EGAL(3, objectifTriplet.evaluer(objets), "un triplet complet = 3 points");

    // Patron Composite : combiner deux objectifs revient a additionner leurs points.
    codex::ZoneDeJeu zoneComplete;
    zoneComplete.poser(codex::Coordonnee(0, 0),
                       carteCoins(21, codex::Ressource::PLANTE, codex::Ressource::VIDE,
                                  codex::Ressource::VIDE, codex::Ressource::VIDE), true, 0);
    zoneComplete.poser(codex::Coordonnee(1, 1),
                       carteCoins(22, codex::Ressource::PLANTE, codex::Ressource::VIDE,
                                  codex::Ressource::VIDE, codex::Ressource::VIDE), true, 1);
    zoneComplete.poser(codex::Coordonnee(2, 2),
                       carteCoins(23, codex::Ressource::PLANTE, codex::Ressource::VIDE,
                                  codex::Ressource::VIDE, codex::Ressource::VIDE), true, 1);
    VERIFIER_EGAL(2, objectifDiagonale.evaluer(zoneComplete), "diagonale de plantes = 2 points");
    VERIFIER_EGAL(2, objectifSerie.evaluer(zoneComplete), "3 symboles plante = 2 points");

    codex::ObjectifCompose composition(500);
    composition.ajouter(std::make_shared<codex::ObjectifMotif>(
        87, codex::Ressource::PLANTE, 2, true));
    composition.ajouter(std::make_shared<codex::ObjectifSerieRessource>(
        95, codex::Ressource::PLANTE, 2));
    VERIFIER_EGAL(4, composition.evaluer(zoneComplete), "objectif compose = somme des conditions");
}
