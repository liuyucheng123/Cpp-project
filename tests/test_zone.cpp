#include "harnais.h"
#include "modele/Carte.h"
#include "modele/IterateurCartes.h"
#include "modele/ZoneDeJeu.h"
#include "utilitaires.h"

#include <map>
#include <memory>

void testerZone()
{
    codex::ZoneDeJeu zone;
    VERIFIER_VRAI(zone.estVide(), "une zone neuve est vide");

    std::shared_ptr<const codex::Carte> premiere = tests::carteNeutre(900);
    zone.poserCarteInitiale(premiere, true);
    VERIFIER_EGAL(1, zone.nombreCartes(), "une carte posee");

    std::string cause;
    codex::ZoneDeJeu vide;
    VERIFIER_VRAI(!vide.placementValide(codex::Coordonnee(0, 0), *premiere, true, cause),
                  "impossible de poser sans coin visible recouvert");

    std::shared_ptr<const codex::Carte> deuxieme = tests::carteNeutre(901);
    VERIFIER_VRAI(zone.placementValide(codex::Coordonnee(1, 1), *deuxieme, true, cause),
                  "placement en diagonale accepte");
    VERIFIER_VRAI(!zone.placementValide(codex::Coordonnee(3, 3), *deuxieme, true, cause),
                  "placement isole refuse");

    codex::ZoneDeJeu zoneCachee;
    zoneCachee.poserCarteInitiale(tests::carteCoinCache(902), true);
    VERIFIER_VRAI(!zoneCachee.placementValide(codex::Coordonnee(1, 1), *deuxieme, true, cause),
                  "un coin cache ne peut pas etre recouvert");
    VERIFIER_VRAI(zoneCachee.placementValide(codex::Coordonnee(-1, -1), *deuxieme, true, cause),
                  "un autre coin reste utilisable");

    zone.poser(codex::Coordonnee(1, 1), deuxieme, true, 1);
    VERIFIER_VRAI(!zone.placementValide(codex::Coordonnee(2, 1), *deuxieme, true, cause),
                  "impossible de superposer deux cartes (deux coins de la meme carte)");
    VERIFIER_VRAI(zone.placementValide(codex::Coordonnee(2, 2), *deuxieme, true, cause),
                  "la case en diagonale reste libre");

    codex::ZoneDeJeu deuxCartes;
    deuxCartes.poserCarteInitiale(tests::carteNeutre(903), true);
    deuxCartes.poser(codex::Coordonnee(2, 0), tests::carteNeutre(904), true, 1);
    VERIFIER_VRAI(deuxCartes.placementValide(codex::Coordonnee(1, 1), *deuxieme, true, cause),
                  "couvrir deux coins de deux cartes est legal");
    VERIFIER_EGAL(2, deuxCartes.compterCoinsCouverts(codex::Coordonnee(1, 1)),
                  "deux coins couverts comptes");

    codex::ZoneDeJeu zoneRessources;
    std::shared_ptr<const codex::Carte> carteTroisRessources = tests::carteTest(
        905, codex::Ressource::PLANTE,
        tests::faceTest(codex::Ressource::PLANTE, codex::Ressource::ANIMAL,
                        codex::Ressource::CHAMPIGNON, codex::Ressource::VIDE, 1, codex::Ressource::INSECTE),
        tests::faceTest(codex::Ressource::VIDE, codex::Ressource::VIDE,
                        codex::Ressource::VIDE, codex::Ressource::VIDE));
    zoneRessources.poserCarteInitiale(carteTroisRessources, true);
    std::map<codex::Ressource, int> ressources = zoneRessources.ressourcesVisibles();
    VERIFIER_EGAL(1, ressources[codex::Ressource::PLANTE], "une plante visible");
    VERIFIER_EGAL(1, ressources[codex::Ressource::ANIMAL], "un animal visible");
    VERIFIER_EGAL(1, ressources[codex::Ressource::CHAMPIGNON], "un champignon visible");
    VERIFIER_EGAL(1, ressources[codex::Ressource::INSECTE], "un insecte au centre");

    // la carte (1,-1) recouvre le coin haut-droit (animal) de la carte initiale
    zoneRessources.poser(codex::Coordonnee(1, -1), tests::carteNeutre(906), true, 1);
    ressources = zoneRessources.ressourcesVisibles();
    VERIFIER_EGAL(0, ressources[codex::Ressource::ANIMAL], "le coin recouvert disparait");
    VERIFIER_EGAL(1, ressources[codex::Ressource::PLANTE], "la plante reste visible");

    codex::IterateurCartes iterateur = zoneRessources.iterateur();
    iterateur.premier();
    int compte = 0;
    while (!iterateur.estFini()) {
        ++compte;
        iterateur.suivant();
    }
    VERIFIER_EGAL(2, compte, "deux cartes parcourues par l'iterateur");

    VERIFIER_VRAI(zoneRessources.positionsCandidates().size() >= 8U, "au moins 8 positions candidates");
}
