#include "vue/VueConsole.h"

#include "modele/Carte.h"
#include "modele/FaceCarte.h"
#include "modele/IterateurCartes.h"
#include "modele/Joueur.h"
#include "modele/Objectif.h"
#include "modele/Partie.h"
#include "modele/ZoneDeJeu.h"
#include "noyau/Coin.h"

#include <iomanip>
#include <map>
#include <memory>
#include <sstream>

namespace codex {
namespace {

std::string texteCoins(const FaceCarte& face)
{
    std::ostringstream flux;
    flux << "HG=" << symbole(face.coin(COIN_HAUT_GAUCHE))
         << " HD=" << symbole(face.coin(COIN_HAUT_DROIT))
         << " BG=" << symbole(face.coin(COIN_BAS_GAUCHE))
         << " BD=" << symbole(face.coin(COIN_BAS_DROIT));
    return flux.str();
}

std::string texteCentres(const FaceCarte& face)
{
    std::ostringstream flux;
    flux << "[";
    if (face.nombreCentres() == 0) {
        flux << "-";
    }
    for (int i = 0; i < face.nombreCentres(); ++i) {
        if (i > 0) {
            flux << " ";
        }
        flux << nomRessource(face.centre(i));
    }
    flux << "]";
    return flux.str();
}

std::string texteSymboles(const std::map<Ressource, int>& comptes)
{
    std::ostringstream flux;
    bool premier = true;
    std::map<Ressource, int>::const_iterator it = comptes.begin();
    while (it != comptes.end()) {
        if (!premier) {
            flux << ", ";
        }
        flux << nomRessource(it->first) << " " << it->second;
        premier = false;
        ++it;
    }
    if (premier) {
        flux << "aucun";
    }
    return flux.str();
}

std::string separateur(char caractere, int taille)
{
    return std::string(static_cast<std::size_t>(taille), caractere);
}

} // namespace

VueConsole::VueConsole(std::ostream& sortie) : flux(sortie)
{
}

VueConsole::~VueConsole()
{
}

void VueConsole::afficherTitre()
{
    flux << separateur('=', 72) << std::endl;
    flux << "  CODEX NATURALIS - projet LO02 (console)" << std::endl;
    flux << separateur('=', 72) << std::endl;
}

void VueConsole::afficherCarteInitiale(const Joueur& joueur)
{
    std::shared_ptr<const Carte> carte = joueur.carteInitiale();
    if (!carte) {
        return;
    }
    flux << "Carte initiale de " << joueur.nom() << " : " << carte->nom() << std::endl;
    flux << "      recto " << texteCoins(carte->face(true)) << " " << texteCentres(carte->face(true))
         << "   |   verso " << texteCoins(carte->face(false)) << " " << texteCentres(carte->face(false))
         << std::endl;
}

void VueConsole::afficherObjectifs(const Partie& partie)
{
    flux << "Objectifs communs :" << std::endl;
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<const Objectif> objectif = partie.objectifCommun(i);
        flux << "  [" << objectif->id() << "] " << objectif->description() << std::endl;
    }
}

void VueConsole::afficherTable(const Partie& partie)
{
    flux << separateur('-', 72) << std::endl;
    flux << "Scores :";
    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        const Joueur& joueur = partie.joueur(i);
        flux << " " << joueur.nom() << "=" << joueur.score();
    }
    flux << "   |   Pioche : ressources " << partie.pioche().tailleTas(false)
         << ", dorees " << partie.pioche().tailleTas(true) << std::endl;

    const Joueur& actuel = partie.joueur(partie.joueurCourant());
    flux << "Tour " << partie.numeroTour() << " - " << actuel.nom();
    if (actuel.estOrdinateur()) {
        flux << " (ordinateur)";
    }
    flux << "   etat : ";
    if (partie.attentePioche()) {
        flux << "il faut piocher";
    } else {
        flux << "il faut poser une carte";
    }
    if (partie.etat() == EtatPartie::DERNIER_TOUR) {
        flux << " (dernier tour)";
    }
    flux << std::endl;
}

void VueConsole::dessinerGrille(const ZoneDeJeu& zone)
{
    int xMin = 0;
    int yMin = 0;
    int xMax = 0;
    int yMax = 0;
    zone.bornes(xMin, yMin, xMax, yMax);

    flux << "    ";
    for (int x = xMin; x <= xMax; ++x) {
        flux << std::setw(2) << x << " ";
    }
    flux << "  <- x" << std::endl;

    for (int y = yMin; y <= yMax; ++y) {
        flux << std::setw(3) << y << " ";
        for (int x = xMin; x <= xMax; ++x) {
            Coordonnee c(x, y);
            const CartePlacee* posee = zone.carteSur(c);
            char affichage = ' ';
            if (posee != 0) {
                int coin = coinDepuisDecalage(c.x - posee->origine.x, c.y - posee->origine.y);
                affichage = symbole(posee->carte->face(posee->recto).coin(coin));
            }
            flux << " " << affichage << " ";
        }
        flux << std::endl;
    }
}

void VueConsole::dessinerListeCartes(const ZoneDeJeu& zone)
{
    flux << "Cartes posees :" << std::endl;
    IterateurCartes it = zone.iterateur();
    it.premier();
    int numero = 0;
    while (!it.estFini()) {
        const CartePlacee& posee = it.courant();
        ++numero;
        flux << "  " << numero << ") " << posee.carte->nom()
             << " en (" << posee.origine.x << "," << posee.origine.y << ") "
             << (posee.recto ? "recto" : "verso")
             << "  " << texteCoins(posee.carte->face(posee.recto))
             << " " << texteCentres(posee.carte->face(posee.recto));
        if (posee.coinsCouverts > 0) {
            flux << " (a couvert " << posee.coinsCouverts << " coin(s))";
        }
        flux << std::endl;
        it.suivant();
    }
}

void VueConsole::afficherZone(const Joueur& joueur)
{
    flux << "Codex de " << joueur.nom() << " :" << std::endl;
    dessinerGrille(joueur.zone());
    dessinerListeCartes(joueur.zone());

    flux << "Ressources visibles : " << texteSymboles(joueur.zone().ressourcesVisibles()) << std::endl;
    flux << "Objets visibles     : " << texteSymboles(joueur.zone().objetsVisibles()) << std::endl;
    if (joueur.objectifSecret()) {
        flux << "Objectif secret     : [" << joueur.objectifSecret()->id() << "] "
             << joueur.objectifSecret()->description() << std::endl;
    }
}

void VueConsole::afficherCarte(const Carte& carte, int indice)
{
    flux << "  [" << indice << "] " << carte.description() << std::endl;
    flux << "      recto " << texteCoins(carte.face(true)) << " " << texteCentres(carte.face(true))
         << "   |   verso " << texteCoins(carte.face(false)) << " " << texteCentres(carte.face(false))
         << std::endl;
}

void VueConsole::afficherMain(const Joueur& joueur)
{
    flux << "Main de " << joueur.nom() << " :" << std::endl;
    for (int i = 0; i < joueur.nombreCartesEnMain(); ++i) {
        afficherCarte(*joueur.main(i), i + 1);
    }
    if (joueur.nombreCartesEnMain() == 0) {
        flux << "  (main vide)" << std::endl;
    }
}

void VueConsole::afficherEtalees(const Partie& partie)
{
    flux << "Cartes visibles au centre :" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::shared_ptr<const Carte> carte = partie.pioche().etalee(i);
        flux << "  [" << (i + 1) << "] ";
        if (!carte) {
            flux << "(vide)" << std::endl;
            continue;
        }
        flux << (i < 2 ? "ressource " : "doree     ") << carte->description() << std::endl;
        flux << "      recto " << texteCoins(carte->face(true)) << " "
             << texteCentres(carte->face(true))
             << "   |   verso " << texteCoins(carte->face(false)) << " "
             << texteCentres(carte->face(false)) << std::endl;
    }
}

void VueConsole::afficherResultat(const Partie& partie)
{
    flux << separateur('=', 72) << std::endl;
    flux << "FIN DE PARTIE" << std::endl;
    flux << std::setw(4) << "N" << std::setw(18) << "Joueur" << std::setw(10) << "Cartes"
         << std::setw(12) << "Objectifs" << std::setw(10) << "Total" << std::endl;
    const std::vector<ResultatJoueur>& resultats = partie.resultats();
    for (std::size_t i = 0; i < resultats.size(); ++i) {
        flux << std::setw(4) << (i + 1)
             << std::setw(18) << resultats[i].nom
             << std::setw(10) << resultats[i].pointsCartes
             << std::setw(12) << resultats[i].pointsObjectifs
             << std::setw(10) << resultats[i].total << std::endl;
    }
    int gagnant = partie.gagnant();
    if (gagnant < 0) {
        flux << "Resultat : egalite" << std::endl;
    } else {
        flux << "Vainqueur : " << resultats[static_cast<std::size_t>(gagnant)].nom << std::endl;
    }
    flux << separateur('=', 72) << std::endl;
}

void VueConsole::afficherMessage(const std::string& message)
{
    flux << message << std::endl;
}

void VueConsole::afficherErreur(const std::string& message)
{
    flux << "[refuse] " << message << std::endl;
}

} // namespace codex
