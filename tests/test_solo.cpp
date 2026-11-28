#include "harnais.h"
#include "controleur/SimulationIA.h"
#include "modele/Carte.h"
#include "modele/CatalogueCartes.h"
#include "modele/ConfigurationPartie.h"
#include "modele/Objectif.h"
#include "modele/Partie.h"
#include "modele/Tybor.h"
#include "noyau/ExceptionJeu.h"

#include <memory>
#include <vector>

namespace {

codex::ConfigurationPartie configurationSolo(int difficulte, unsigned int graine)
{
    codex::ConfigurationPartie configuration;
    configuration.graine = graine;
    configuration.difficulteSolo = difficulte;
    configuration.sieges.push_back(codex::ConfigurationJoueur("Alice", false, 0));
    return configuration;
}

// Joue le tour du joueur humain : une carte posable, puis une carte visible.
void jouerTourHumain(codex::Partie& partie)
{
    const codex::Joueur& joueur = partie.joueur(partie.joueurCourant());
    std::vector<codex::Coordonnee> positions = joueur.zone().positionsCandidates();
    for (int m = 0; m < joueur.nombreCartesEnMain(); ++m) {
        std::shared_ptr<const codex::Carte> carte = joueur.main(m);
        for (int face = 0; face < 2; ++face) {
            for (std::size_t p = 0; p < positions.size(); ++p) {
                try {
                    partie.jouerCarte(m, positions[p], face == 0);
                    for (int indice = 0; indice < 4; ++indice) {
                        if (!partie.pioche().etaleeDisponible(indice)) {
                            continue;
                        }
                        if (indice < 2) {
                            partie.piocher(codex::SourcePioche::ETALEE_RESSOURCE, indice);
                        } else {
                            partie.piocher(codex::SourcePioche::ETALEE_DOREE, indice - 2);
                        }
                        return;
                    }
                    return;
                } catch (const codex::ExceptionJeu&) {
                    continue;
                }
            }
        }
    }
}

} // namespace

void testerSolo()
{
    // --- Mise en place : un siege humain plus Tybor ---
    codex::ConfigurationPartie configuration = configurationSolo(1, 2026U);
    codex::GenerateurAleatoire generateur(configuration.graine);
    codex::Partie partie;
    partie.preparer(configuration, generateur);

    VERIFIER_VRAI(partie.estModeSolo(), "un seul siege suffit a ouvrir le mode solo");
    VERIFIER_EGAL(2, partie.nombreJoueurs(), "solo : le joueur et Tybor");
    VERIFIER_VRAI(partie.joueur(1).estTybor(), "le second joueur est Tybor");
    VERIFIER_EGAL(1, partie.indiceTybor(), "Tybor occupe le siege 1");

    // Tybor n'a ni carte initiale, ni main, ni objectif secret.
    VERIFIER_VRAI(partie.joueur(1).carteInitiale().get() == 0, "Tybor n'a pas de carte initiale");
    VERIFIER_EGAL(0, partie.joueur(1).nombreCartesEnMain(), "Tybor n'a pas de main");
    VERIFIER_VRAI(partie.joueur(1).objectifSecret().get() == 0, "Tybor n'a pas d'objectif secret");
    VERIFIER_EGAL(4, partie.adversaire().totalRessources(), "Tybor commence avec 4 ressources");

    bool refusFace = false;
    try {
        partie.choisirFaceInitiale(1, true);
    } catch (const codex::ExceptionJeu&) {
        refusFace = true;
    }
    VERIFIER_VRAI(refusFace, "on ne choisit pas de carte initiale pour Tybor");

    bool refusObjectif = false;
    try {
        partie.choisirObjectifSecret(1, 0);
    } catch (const codex::ExceptionJeu&) {
        refusObjectif = true;
    }
    VERIFIER_VRAI(refusObjectif, "on ne choisit pas d'objectif pour Tybor");

    // La mise en place ne demande que le joueur humain.
    partie.choisirFaceInitiale(0, true);
    partie.choisirObjectifSecret(0, 0);
    VERIFIER_VRAI(partie.preparationTerminee(), "la mise en place est terminee sans Tybor");
    partie.commencer();
    VERIFIER_VRAI(partie.etat() == codex::EtatPartie::EN_COURS, "la partie solo demarre");
    VERIFIER_EGAL(0, partie.joueur(1).zone().nombreCartes(), "Tybor n'a pas de codex");

    // --- Un tour de Tybor, precede du tour humain ---
    jouerTourHumain(partie);
    VERIFIER_EGAL(1, partie.joueurCourant(), "apres le joueur, c'est a Tybor");
    const int ressourcesAvant = partie.adversaire().totalRessources();
    const int scoreAvant = partie.joueur(1).score();
    const std::string journal = partie.jouerTourTybor();
    VERIFIER_VRAI(!journal.empty(), "le tour de Tybor est raconte");
    VERIFIER_VRAI(partie.adversaire().totalRessources() >= ressourcesAvant,
                  "Tybor ne perd jamais de ressource");
    VERIFIER_VRAI(partie.joueur(1).score() >= scoreAvant, "le score de Tybor ne baisse pas");
    VERIFIER_EGAL(0, partie.joueurCourant(), "apres Tybor on revient au joueur humain");
    VERIFIER_EGAL(0, partie.joueur(1).zone().nombreCartes(), "Tybor ne pose aucune carte");

    // --- Difficulte ---
    codex::Partie facile;
    codex::ConfigurationPartie configurationFacile = configurationSolo(0, 7U);
    codex::GenerateurAleatoire generateurFacile(7U);
    facile.preparer(configurationFacile, generateurFacile);
    VERIFIER_EGAL(2, facile.adversaire().totalRessources(), "difficulte facile : 2 ressources");

    codex::Partie difficile;
    codex::ConfigurationPartie configurationDifficile = configurationSolo(2, 11U);
    codex::GenerateurAleatoire generateurDifficile(11U);
    difficile.preparer(configurationDifficile, generateurDifficile);
    VERIFIER_EGAL(4, difficile.adversaire().totalRessources(), "difficulte difficile : 4 ressources");
    VERIFIER_EGAL(4, difficile.joueur(1).score(), "difficulte difficile : 4 points d'avance");

    // --- Les cartes Doree portent bien un nombre imprime ---
    const codex::CatalogueCartes& catalogue = codex::CatalogueCartes::instance();
    bool uneCarteACinqPoints = false;
    bool toutesPositives = true;
    std::size_t cartesAnimal = 0;
    const std::vector<int>& idsDoree = catalogue.idsDoree();
    for (std::size_t i = 0; i < idsDoree.size(); ++i) {
        std::shared_ptr<const codex::Carte> carte = catalogue.creerCarte(idsDoree[i]);
        if (carte->pointsImprimes() <= 0) {
            toutesPositives = false;
        }
        if (carte->pointsImprimes() == 5) {
            uneCarteACinqPoints = true;
        }
        if (carte->royaume() == codex::Ressource::ANIMAL) {
            ++cartesAnimal;
        }
    }
    VERIFIER_VRAI(toutesPositives, "toutes les cartes Doree affichent un nombre de points");
    VERIFIER_VRAI(uneCarteACinqPoints, "il existe des cartes Doree a 5 points");
    VERIFIER_VRAI(cartesAnimal > 0, "il existe des cartes Doree du royaume animal");

    // --- Un objectif correspond a une carte de son royaume ou portant son objet ---
    codex::ObjectifMotif motifAnimal(900, codex::Ressource::ANIMAL, 3, false);
    bool motifReconnu = false;
    bool motifRefuse = false;
    for (std::size_t i = 0; i < idsDoree.size(); ++i) {
        std::shared_ptr<const codex::Carte> carte = catalogue.creerCarte(idsDoree[i]);
        if (carte->royaume() == codex::Ressource::ANIMAL) {
            motifReconnu = motifReconnu || motifAnimal.correspondALaCarte(*carte);
        } else {
            motifRefuse = motifRefuse || !motifAnimal.correspondALaCarte(*carte);
        }
    }
    VERIFIER_VRAI(motifReconnu, "un objectif de royaume reconnait ses cartes");
    VERIFIER_VRAI(motifRefuse, "un objectif de royaume refuse les autres royaumes");

    codex::ObjectifPairesObjet pairesPlume(901, codex::Ressource::PLUME, 2);
    bool plumeTestee = false;
    bool plumeToujoursReconnue = true;
    for (std::size_t i = 0; i < idsDoree.size(); ++i) {
        std::shared_ptr<const codex::Carte> carte = catalogue.creerCarte(idsDoree[i]);
        if (!carte->porteObjet(codex::Ressource::PLUME)) {
            continue;
        }
        plumeTestee = true;
        if (!pairesPlume.correspondALaCarte(*carte)) {
            plumeToujoursReconnue = false;
        }
    }
    VERIFIER_VRAI(plumeTestee, "une carte Doree porte bien une plume");
    VERIFIER_VRAI(plumeToujoursReconnue, "les objectifs d'objet reconnaissent les plumes");

    // --- Une partie solo complete se termine et Tybor ne marque pas d'objectif ---
    codex::ConfigurationPartie configurationAuto = configurationSolo(1, 2026U);
    configurationAuto.sieges[0].ordinateur = true;
    codex::ResumeSimulation resume = codex::jouerPartieAutomatique(configurationAuto);
    VERIFIER_VRAI(resume.terminee, "la partie solo automatique se termine");
    VERIFIER_EGAL(2, static_cast<int>(resume.totaux.size()), "solo : deux lignes de score");
    VERIFIER_EGAL(0, resume.pointsObjectifs[1], "Tybor ne marque aucun point d'objectif");
    VERIFIER_VRAI(resume.totaux[1] >= 0, "le score de Tybor est positif");
}
