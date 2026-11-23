#include "harnais.h"
#include "modele/Carte.h"
#include "modele/Partie.h"
#include "noyau/ExceptionJeu.h"

#include <memory>
#include <string>
#include <vector>

namespace {

void choisirMiseEnPlace(codex::Partie& partie)
{
    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        partie.choisirFaceInitiale(i, true);
        partie.choisirObjectifSecret(i, 0);
    }
}

bool placerUneCarte(codex::Partie& partie)
{
    int indiceJoueur = partie.joueurCourant();
    const codex::Joueur& joueur = partie.joueur(indiceJoueur);
    std::vector<codex::Coordonnee> positions = joueur.zone().positionsCandidates();
    for (int m = 0; m < joueur.nombreCartesEnMain(); ++m) {
        std::shared_ptr<const codex::Carte> carte = joueur.main(m);
        for (int face = 0; face < 2; ++face) {
            for (std::size_t p = 0; p < positions.size(); ++p) {
                try {
                    partie.jouerCarte(m, positions[p], face == 0);
                    return true;
                } catch (const codex::ExceptionJeu&) {
                    continue;
                }
            }
        }
    }
    return false;
}

bool piocherUneCarte(codex::Partie& partie)
{
    for (int indice = 0; indice < 4; ++indice) {
        if (!partie.pioche().etaleeDisponible(indice)) {
            continue;
        }
        try {
            if (indice < 2) {
                partie.piocher(codex::SourcePioche::ETALEE_RESSOURCE, indice);
            } else {
                partie.piocher(codex::SourcePioche::ETALEE_DOREE, indice - 2);
            }
            return true;
        } catch (const codex::ExceptionJeu&) {
            continue;
        }
    }
    if (!partie.pioche().tasVide(false)) {
        partie.piocher(codex::SourcePioche::TAS_RESSOURCE, 0);
        return true;
    }
    if (!partie.pioche().tasVide(true)) {
        partie.piocher(codex::SourcePioche::TAS_DOREE, 0);
        return true;
    }
    return false;
}

} // namespace

void testerPartie()
{
    codex::ConfigurationPartie configuration;
    configuration.graine = 42U;
    configuration.sieges.push_back(codex::ConfigurationJoueur("Alice", false, 0));
    configuration.sieges.push_back(codex::ConfigurationJoueur("Bob", false, 0));

    codex::GenerateurAleatoire generateur(configuration.graine);
    codex::Partie partie;
    partie.preparer(configuration, generateur);

    VERIFIER_EGAL(2, partie.nombreJoueurs(), "deux joueurs");
    VERIFIER_EGAL(3, partie.joueur(0).nombreCartesEnMain(), "trois cartes en main au depart");
    VERIFIER_VRAI(!partie.preparationTerminee(), "la mise en place n'est pas finie");
    VERIFIER_EGAL(0, partie.joueur(0).score(), "score initial nul");
    VERIFIER_VRAI(partie.objectifCommun(0).get() != 0, "un objectif commun est distribue");

    // on ne peut pas jouer avant que la mise en place soit terminee
    bool refus = false;
    try {
        partie.jouerCarte(0, codex::Coordonnee(1, 1), true);
    } catch (const codex::ExceptionJeu&) {
        refus = true;
    }
    VERIFIER_VRAI(refus, "impossible de jouer pendant la mise en place");

    choisirMiseEnPlace(partie);
    VERIFIER_VRAI(partie.preparationTerminee(), "mise en place terminee");
    partie.commencer();
    VERIFIER_VRAI(partie.etat() == codex::EtatPartie::EN_COURS, "la partie est en cours");
    VERIFIER_EGAL(1, partie.joueur(0).zone().nombreCartes(), "la carte initiale est posee");

    // on ne peut pas piocher avant d'avoir pose
    bool refusPioche = false;
    try {
        partie.piocher(codex::SourcePioche::ETALEE_RESSOURCE, 0);
    } catch (const codex::ExceptionJeu&) {
        refusPioche = true;
    }
    VERIFIER_VRAI(refusPioche, "impossible de piocher avant d'avoir pose");

    int tours = 0;
    while (!partie.estTerminee() && tours < 500) {
        ++tours;
        if (!placerUneCarte(partie)) {
            partie.passerTour();
            continue;
        }
        piocherUneCarte(partie);
    }
    VERIFIER_VRAI(partie.estTerminee(), "la partie se termine");
    VERIFIER_VRAI(tours < 500, "la partie ne boucle pas");

    const std::vector<codex::ResultatJoueur>& resultats = partie.resultats();
    VERIFIER_EGAL(2, static_cast<int>(resultats.size()), "deux resultats");
    for (std::size_t i = 0; i < resultats.size(); ++i) {
        VERIFIER_EGAL(resultats[i].total,
                      resultats[i].pointsCartes + resultats[i].pointsObjectifs,
                      "total = cartes + objectifs");
        VERIFIER_VRAI(resultats[i].pointsCartes >= 0, "points de cartes positifs");
        VERIFIER_VRAI(resultats[i].pointsObjectifs >= 0, "points d'objectifs positifs");
    }
    VERIFIER_VRAI(partie.gagnant() >= -1 && partie.gagnant() < 2, "gagnant valide");
}
