#include "controleur/TourAutomatique.h"

#include "modele/Carte.h"
#include "modele/Joueur.h"
#include "modele/Partie.h"
#include "modele/StrategieIA.h"
#include "modele/ZoneDeJeu.h"
#include "noyau/ExceptionJeu.h"

#include <sstream>
#include <string>
#include <vector>

namespace codex {
namespace {

// Construit la ligne affichee apres le coup de l'ordinateur, pour que son
// action soit visible a l'ecran (sinon seul le score change).
void decrirePose(const Joueur& joueur, int scoreAvant, std::string* resume)
{
    if (resume == nullptr || joueur.zone().cartesPlacees().empty()) {
        return;
    }
    const CartePlacee& derniere = joueur.zone().cartesPlacees().back();
    const int gagnes = joueur.score() - scoreAvant;
    std::ostringstream flux;
    flux << joueur.nom() << " joue " << derniere.carte->nom() << " en "
         << derniere.origine << ' ' << (derniere.recto ? "recto" : "verso")
         << " (+" << gagnes << (gagnes > 1 ? " pts)" : " pt)");
    *resume = flux.str();
}

bool placerAvecRepli(Partie& partie, int indiceJoueur, const ActionTour& action)
{
    Joueur& joueur = partie.joueur(indiceJoueur);
    try {
        partie.jouerCarte(action.indiceMain, action.position, action.recto);
        return true;
    } catch (const ExceptionJeu&) {
        // on essaie toutes les combinaisons avant de renoncer
    }

    std::vector<Coordonnee> candidates = joueur.zone().positionsCandidates();
    for (int main = 0; main < joueur.nombreCartesEnMain(); ++main) {
        for (int face = 0; face < 2; ++face) {
            for (std::size_t p = 0; p < candidates.size(); ++p) {
                try {
                    partie.jouerCarte(main, candidates[p], face == 0);
                    return true;
                } catch (const ExceptionJeu&) {
                    // placement illegal : on continue
                }
            }
        }
    }
    return false;
}

void piocherAvecRepli(Partie& partie, const ActionTour& action)
{
    if (!partie.attentePioche()) {
        return;
    }
    try {
        partie.piocher(action.source, action.indiceEtalee);
        return;
    } catch (const ExceptionJeu&) {
        // on essaie les autres sources
    }

    for (int indice = 0; indice < 4; ++indice) {
        if (!partie.pioche().etaleeDisponible(indice)) {
            continue;
        }
        try {
            if (indice < 2) {
                partie.piocher(SourcePioche::ETALEE_RESSOURCE, indice);
            } else {
                partie.piocher(SourcePioche::ETALEE_DOREE, indice - 2);
            }
            return;
        } catch (const ExceptionJeu&) {
            continue;
        }
    }

    for (int essai = 0; essai < 2; ++essai) {
        try {
            if (essai == 0) {
                partie.piocher(SourcePioche::TAS_RESSOURCE, 0);
            } else {
                partie.piocher(SourcePioche::TAS_DOREE, 0);
            }
            return;
        } catch (const ExceptionJeu&) {
            continue;
        }
    }
}

} // namespace

bool jouerTourAutomatique(Partie& partie, int indiceJoueur, StrategieIA& strategie,
                          std::string* resume)
{
    Joueur& joueur = partie.joueur(indiceJoueur);
    if (joueur.nombreCartesEnMain() == 0 || !partie.placementPossible(indiceJoueur)) {
        partie.passerTour();
        return false;
    }

    ActionTour action = strategie.choisirAction(partie, indiceJoueur);
    const int scoreAvant = joueur.score();
    if (!placerAvecRepli(partie, indiceJoueur, action)) {
        partie.passerTour();
        return false;
    }
    decrirePose(joueur, scoreAvant, resume);
    piocherAvecRepli(partie, action);
    return true;
}

} // namespace codex
