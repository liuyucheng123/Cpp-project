#include "controleur/ControleurPartie.h"

#include "controleur/Saisie.h"
#include "controleur/TourAutomatique.h"
#include "modele/Carte.h"
#include "modele/FaceCarte.h"
#include "modele/Objectif.h"
#include "modele/StrategieIA.h"
#include "noyau/ExceptionJeu.h"
#include "vue/Vue.h"

#include <sstream>

namespace codex {
namespace {

int convertirEntier(const std::string& texte)
{
    std::istringstream flux(texte);
    int valeur = 0;
    flux >> valeur;
    if (flux.fail()) {
        throw SaisieInvalide("ce n'est pas un nombre : " + texte);
    }
    return valeur;
}

} // namespace

ControleurPartie::ControleurPartie(Partie& partieJeu, Vue& vueAffichage, Saisie& saisieJoueur,
                                   GenerateurAleatoire& generateurAleatoire)
    : partie(partieJeu), vue(vueAffichage), saisie(saisieJoueur), generateur(generateurAleatoire),
      strategies()
{
    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        const Joueur& joueur = partie.joueur(i);
        if (joueur.estOrdinateur()) {
            strategies[i] = creerStrategie(joueur.niveau(), generateur);
        }
    }
}

int ControleurPartie::demanderEntier(const std::string& question, int minimum, int maximum)
{
    int essais = 0;
    while (essais < 50) {
        ++essais;
        vue.afficherMessage(question);
        std::string ligne = saisie.lireLigne();
        if (ligne.empty() && !saisie.disponible()) {
            throw SaisieInvalide("fin des entrees : la partie s'arrete");
        }
        try {
            int valeur = convertirEntier(ligne);
            if (valeur >= minimum && valeur <= maximum) {
                return valeur;
            }
            std::ostringstream message;
            message << "valeur hors limites : il faut entre " << minimum << " et " << maximum;
            vue.afficherErreur(message.str());
        } catch (const SaisieInvalide& erreur) {
            vue.afficherErreur(erreur.what());
        }
    }
    throw SaisieInvalide("trop d'erreurs de saisie");
}

void ControleurPartie::phasePreparation()
{
    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        const Joueur& joueur = partie.joueur(i);
        std::shared_ptr<const Carte> initiale = joueur.carteInitiale();

        if (joueur.estOrdinateur()) {
            bool recto = (generateur.suivant(2U) == 0U);
            partie.choisirFaceInitiale(i, recto);
            partie.choisirObjectifSecret(i, static_cast<int>(generateur.suivant(2U)));
        } else {
            vue.afficherMessage("--- Mise en place ---");
            (void)initiale;
            vue.afficherCarteInitiale(joueur);
            vue.afficherMessage("  1 = recto, 2 = verso");
            int face = demanderEntier("Votre choix :", 1, 2);
            partie.choisirFaceInitiale(i, face == 1);

            vue.afficherMessage("Objectifs proposes a " + joueur.nom() + " :");
            for (std::size_t k = 0; k < joueur.candidatsObjectif().size(); ++k) {
                std::ostringstream ligne;
                ligne << "  " << (k + 1) << ") [" << joueur.candidatsObjectif()[k]->id() << "] "
                      << joueur.candidatsObjectif()[k]->description();
                vue.afficherMessage(ligne.str());
            }
            int choix = demanderEntier("Objectif secret (1 ou 2) :", 1, 2);
            partie.choisirObjectifSecret(i, choix - 1);
        }
    }
    partie.commencer();
}

std::vector<Coordonnee> ControleurPartie::placementsPossibles(const Joueur& joueur,
                                                             const std::shared_ptr<const Carte>& carte,
                                                             bool recto) const
{
    std::vector<Coordonnee> possibles;
    std::vector<Coordonnee> candidates = joueur.zone().positionsCandidates();
    std::map<Ressource, int> ressources = joueur.zone().ressourcesVisibles();
    if (recto && !carte->exigencesSatisfaites(ressources)) {
        return possibles;
    }
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        std::string cause;
        if (joueur.zone().placementValide(candidates[i], *carte, recto, cause)) {
            possibles.push_back(candidates[i]);
        }
    }
    return possibles;
}

void ControleurPartie::poserCarteHumaine()
{
    Joueur& joueur = partie.joueur(partie.joueurCourant());
    int nombre = joueur.nombreCartesEnMain();

    while (true) {
        int choixCarte = demanderEntier("Carte a poser (1 a " + std::to_string(nombre) + ") :", 1, nombre);
        int optionFace = demanderEntier("Face : 1 = recto, 2 = verso :", 1, 2);
        bool recto = (optionFace == 1);
        std::shared_ptr<const Carte> carte = joueur.main(choixCarte - 1);

        std::map<Ressource, int> ressources = joueur.zone().ressourcesVisibles();
        if (recto && !carte->exigencesSatisfaites(ressources)) {
            vue.afficherErreur(carte->nom() + " n'est pas jouable face doree : " + carte->texteExigences());
            continue;
        }

        std::vector<Coordonnee> possibles = placementsPossibles(joueur, carte, recto);
        if (possibles.empty()) {
            vue.afficherErreur("aucun placement possible avec cette carte et cette face");
            continue;
        }

        vue.afficherMessage("Placements possibles :");
        for (std::size_t i = 0; i < possibles.size(); ++i) {
            std::ostringstream ligne;
            ligne << "  " << (i + 1) << ") (" << possibles[i].x << "," << possibles[i].y << ")";
            vue.afficherMessage(ligne.str());
        }
        int choixPosition = demanderEntier("Choix du placement :", 1, static_cast<int>(possibles.size()));

        try {
            partie.jouerCarte(choixCarte - 1, possibles[static_cast<std::size_t>(choixPosition - 1)], recto);
            return;
        } catch (const ExceptionJeu& erreur) {
            vue.afficherErreur(erreur.what());
        }
    }
}

void ControleurPartie::piocherHumain()
{
    while (true) {
        vue.afficherMessage("Ou piocher ? 1-4 = carte visible, 5 = tas ressource, 6 = tas doree");
        int choix = demanderEntier("Votre choix :", 1, 6);
        try {
            if (choix <= 4) {
                int indice = choix - 1;
                if (indice < 2) {
                    partie.piocher(SourcePioche::ETALEE_RESSOURCE, indice);
                } else {
                    partie.piocher(SourcePioche::ETALEE_DOREE, indice - 2);
                }
            } else if (choix == 5) {
                partie.piocher(SourcePioche::TAS_RESSOURCE, 0);
            } else {
                partie.piocher(SourcePioche::TAS_DOREE, 0);
            }
            return;
        } catch (const ExceptionJeu& erreur) {
            vue.afficherErreur(erreur.what());
        }
    }
}

void ControleurPartie::jouerUnTourHumain()
{
    Joueur& joueur = partie.joueur(partie.joueurCourant());
    vue.afficherObjectifs(partie);
    vue.afficherTable(partie);
    vue.afficherZone(joueur);
    vue.afficherMain(joueur);
    vue.afficherEtalees(partie);

    if (joueur.nombreCartesEnMain() == 0) {
        vue.afficherMessage("Plus de carte en main : le tour est passe.");
        partie.passerTour();
        return;
    }
    if (!partie.placementPossible(partie.joueurCourant())) {
        vue.afficherMessage("Aucun placement possible : le tour est passe.");
        partie.passerTour();
        return;
    }

    poserCarteHumaine();
    piocherHumain();
}

void ControleurPartie::tourOrdinateur()
{
    int indice = partie.joueurCourant();
    Joueur& joueur = partie.joueur(indice);
    vue.afficherTable(partie);
    vue.afficherZone(joueur);

    if (joueur.nombreCartesEnMain() == 0 || !partie.placementPossible(indice)) {
        vue.afficherMessage(joueur.nom() + " passe son tour.");
        partie.passerTour();
        return;
    }

    std::shared_ptr<StrategieIA> strategie = strategies[indice];
    bool pose = jouerTourAutomatique(partie, indice, *strategie);
    if (!pose) {
        vue.afficherMessage(joueur.nom() + " passe son tour.");
    }
}

void ControleurPartie::boucleDeJeu()
{
    int securite = 0;
    while (!partie.estTerminee()) {
        ++securite;
        if (securite > 20000) {
            throw ExceptionJeu("trop de tours : arret de securite");
        }
        int indice = partie.joueurCourant();
        if (partie.joueur(indice).estOrdinateur()) {
            tourOrdinateur();
        } else {
            jouerUnTourHumain();
        }
    }
}

void ControleurPartie::executer()
{
    vue.afficherTitre();
    phasePreparation();
    try {
        boucleDeJeu();
    } catch (const SaisieInvalide&) {
        vue.afficherErreur("entrees terminees : abandon de la partie");
        return;
    }
    vue.afficherResultat(partie);
}

} // namespace codex
