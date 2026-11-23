#include "modele/Partie.h"

#include "modele/Carte.h"
#include "modele/CatalogueCartes.h"
#include "modele/Objectif.h"
#include "noyau/ExceptionJeu.h"
#include "noyau/Melange.h"

#include <sstream>

namespace codex {
namespace {

const int SCORE_DE_FIN = 20;

} // namespace

ResultatJoueur::ResultatJoueur()
    : nom(""), pointsCartes(0), pointsObjectifs(0), total(0), ordinateur(false)
{
}

Partie::Partie()
    : etatPartie(EtatPartie::NON_DEMARREE), configuration(), joueurs(), piocheCartes(),
      objectifsCommuns(), initialesDisponibles(), objectifsDisponibles(),
      indexCourant(0), compteurTours(1), attenteDePioche(false), toursRestants(0), passesConsecutifs(0), classement()
{
    objectifsCommuns[0] = std::shared_ptr<const Objectif>();
    objectifsCommuns[1] = std::shared_ptr<const Objectif>();
}

void Partie::preparer(const ConfigurationPartie& configurationDemandee, GenerateurAleatoire& generateur)
{
    if (etatPartie != EtatPartie::NON_DEMARREE) {
        throw ExceptionJeu("la partie est deja preparee");
    }
    if (!configurationDemandee.valide()) {
        throw SaisieInvalide("il faut entre 1 et 4 joueurs");
    }

    configuration = configurationDemandee;
    joueurs.clear();

    const CatalogueCartes& catalogue = CatalogueCartes::instance();

    std::vector<int> idsObjectif = catalogue.idsObjectif();
    melanger(idsObjectif, generateur);

    std::vector<int> idsInitiales = catalogue.idsInitiale();
    melanger(idsInitiales, generateur);

    piocheCartes.initialiser(catalogue.idsRessource(), catalogue.idsDoree(), generateur);

    objectifsCommuns[0] = catalogue.creerObjectif(idsObjectif[0]);
    objectifsCommuns[1] = catalogue.creerObjectif(idsObjectif[1]);

    std::size_t prochainObjectif = 2;

    for (std::size_t i = 0; i < configuration.sieges.size(); ++i) {
        const ConfigurationJoueur& siege = configuration.sieges[i];
        Joueur joueur(siege.nom, siege.ordinateur, siege.niveau);
        joueur.fixerCarteInitiale(catalogue.creerCarte(idsInitiales[i]));
        joueur.ajouterEnMain(piocheCartes.piocherSurLeTas(false));
        joueur.ajouterEnMain(piocheCartes.piocherSurLeTas(false));
        joueur.ajouterEnMain(piocheCartes.piocherSurLeTas(true));
        joueur.proposerObjectif(catalogue.creerObjectif(idsObjectif[prochainObjectif]));
        ++prochainObjectif;
        joueur.proposerObjectif(catalogue.creerObjectif(idsObjectif[prochainObjectif]));
        ++prochainObjectif;
        joueurs.push_back(joueur);
    }

    indexCourant = 0;
    compteurTours = 1;
    attenteDePioche = false;
    toursRestants = 0;
    classement.clear();
    etatPartie = EtatPartie::PREPARATION;
}

bool Partie::preparationTerminee() const
{
    for (std::size_t i = 0; i < joueurs.size(); ++i) {
        if (!joueurs[i].faceInitialeChoisie() || !joueurs[i].objectifChoisi()) {
            return false;
        }
    }
    return !joueurs.empty();
}

void Partie::choisirFaceInitiale(int indiceJoueur, bool recto)
{
    if (indiceJoueur < 0 || indiceJoueur >= nombreJoueurs()) {
        throw SaisieInvalide("joueur inconnu");
    }
    if (etatPartie != EtatPartie::PREPARATION) {
        throw ExceptionJeu("le choix de la carte initiale n'est plus possible");
    }
    joueurs[static_cast<std::size_t>(indiceJoueur)].choisirFaceInitiale(recto);
}

void Partie::choisirObjectifSecret(int indiceJoueur, int indiceCandidat)
{
    if (indiceJoueur < 0 || indiceJoueur >= nombreJoueurs()) {
        throw SaisieInvalide("joueur inconnu");
    }
    if (etatPartie != EtatPartie::PREPARATION) {
        throw ExceptionJeu("le choix de l'objectif secret n'est plus possible");
    }
    joueurs[static_cast<std::size_t>(indiceJoueur)].choisirObjectifSecret(indiceCandidat);
}

void Partie::commencer()
{
    if (etatPartie != EtatPartie::PREPARATION) {
        throw ExceptionJeu("la partie ne peut pas commencer maintenant");
    }
    if (!preparationTerminee()) {
        throw ExceptionJeu("tous les joueurs doivent choisir leur carte initiale et leur objectif");
    }
    for (std::size_t i = 0; i < joueurs.size(); ++i) {
        joueurs[i].zone().poserCarteInitiale(joueurs[i].carteInitiale(), joueurs[i].faceInitialeRecto());
    }
    indexCourant = 0;
    compteurTours = 1;
    attenteDePioche = false;
    passesConsecutifs = 0;
    etatPartie = EtatPartie::EN_COURS;
}

EtatPartie Partie::etat() const
{
    return etatPartie;
}

bool Partie::estTerminee() const
{
    return etatPartie == EtatPartie::TERMINEE;
}

bool Partie::attentePioche() const
{
    return attenteDePioche;
}

int Partie::nombreJoueurs() const
{
    return static_cast<int>(joueurs.size());
}

int Partie::joueurCourant() const
{
    return indexCourant;
}

int Partie::numeroTour() const
{
    return compteurTours;
}

const Joueur& Partie::joueur(int indice) const
{
    if (indice < 0 || indice >= nombreJoueurs()) {
        throw SaisieInvalide("joueur inconnu");
    }
    return joueurs[static_cast<std::size_t>(indice)];
}

Joueur& Partie::joueur(int indice)
{
    if (indice < 0 || indice >= nombreJoueurs()) {
        throw SaisieInvalide("joueur inconnu");
    }
    return joueurs[static_cast<std::size_t>(indice)];
}

const Pioche& Partie::pioche() const
{
    return piocheCartes;
}

std::shared_ptr<const Objectif> Partie::objectifCommun(int indice) const
{
    if (indice < 0 || indice > 1) {
        throw SaisieInvalide("indice d'objectif commun invalide");
    }
    return objectifsCommuns[indice];
}

void Partie::verifierTour() const
{
    if (etatPartie == EtatPartie::TERMINEE) {
        throw PartieTerminee("la partie est terminee");
    }
    if (etatPartie != EtatPartie::EN_COURS && etatPartie != EtatPartie::DERNIER_TOUR) {
        throw ExceptionJeu("la partie n'a pas encore commence");
    }
}

void Partie::jouerCarte(int indiceMain, Coordonnee origine, bool recto)
{
    verifierTour();
    if (attenteDePioche) {
        throw PlacementInvalide("il faut d'abord piocher une carte");
    }

    Joueur& actuel = joueurs[static_cast<std::size_t>(indexCourant)];
    std::shared_ptr<const Carte> carte = actuel.main(indiceMain);

    std::string cause;
    if (!actuel.zone().placementValide(origine, *carte, recto, cause)) {
        throw PlacementInvalide(cause);
    }

    std::map<Ressource, int> ressources = actuel.zone().ressourcesVisibles();
    if (recto && !carte->exigencesSatisfaites(ressources)) {
        throw RessourcesInsuffisantes(carte->nom() + " : " + carte->texteExigences());
    }

    int coins = actuel.zone().compterCoinsCouverts(origine);
    actuel.zone().poser(origine, carte, recto, coins);

    ContextePose contexte;
    contexte.coinsCouverts = coins;
    contexte.ressources = actuel.zone().symbolesVisibles();
    int points = carte->pointsAuPose(contexte);
    actuel.ajouterPoints(points);
    actuel.retirerDeMain(indiceMain);

    passesConsecutifs = 0;
    attenteDePioche = true;
}

void Partie::piocher(SourcePioche source, int indiceEtalee)
{
    verifierTour();
    if (!attenteDePioche) {
        throw PlacementInvalide("il faut d'abord poser une carte");
    }

    Joueur& actuel = joueurs[static_cast<std::size_t>(indexCourant)];
    std::shared_ptr<const Carte> carte;

    if (piocheCartes.plusDeCartesDuTout()) {
        attenteDePioche = false;
        verifierFinDePartie();
        terminerTour();
        return;
    }

    if (estEtalee(source)) {
        int indice = indiceEtalee;
        if (!estTasRessource(source)) {
            indice += 2;
        }
        carte = piocheCartes.prendreEtalee(indice);
        piocheCartes.completerEtalees();
    } else {
        bool doree = !estTasRessource(source);
        carte = piocheCartes.piocherSurLeTas(doree);
        piocheCartes.completerEtalees();
    }

    actuel.ajouterEnMain(carte);
    attenteDePioche = false;

    verifierFinDePartie();
    terminerTour();
}

void Partie::passerTour()
{
    verifierTour();
    Joueur& actuel = joueurs[static_cast<std::size_t>(indexCourant)];
    if (actuel.nombreCartesEnMain() != 0 && placementPossible(indexCourant)) {
        throw PlacementInvalide("un joueur qui peut poser une carte doit la poser");
    }
    ++passesConsecutifs;
    attenteDePioche = false;
    if (passesConsecutifs >= nombreJoueurs()) {
        terminerPartie();
        return;
    }
    terminerTour();
}

bool Partie::placementPossible(int indiceJoueur) const
{
    const Joueur& joueurCible = joueur(indiceJoueur);
    std::map<Ressource, int> ressources = joueurCible.zone().ressourcesVisibles();
    std::vector<Coordonnee> positions = joueurCible.zone().positionsCandidates();

    for (int m = 0; m < joueurCible.nombreCartesEnMain(); ++m) {
        std::shared_ptr<const Carte> carte = joueurCible.main(m);
        for (int face = 0; face < 2; ++face) {
            if (face == 0 && !carte->exigencesSatisfaites(ressources)) {
                continue;
            }
            for (std::size_t p = 0; p < positions.size(); ++p) {
                std::string cause;
                if (joueurCible.zone().placementValide(positions[p], *carte, face == 0, cause)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Partie::verifierFinDePartie()
{
    if (etatPartie != EtatPartie::EN_COURS) {
        return;
    }

    bool declenche = piocheCartes.epuisee();
    for (std::size_t i = 0; i < joueurs.size(); ++i) {
        if (joueurs[i].score() >= SCORE_DE_FIN) {
            declenche = true;
        }
    }

    if (declenche) {
        etatPartie = EtatPartie::DERNIER_TOUR;
        int restantManche = nombreJoueurs() - 1 - indexCourant;
        toursRestants = restantManche + nombreJoueurs();
    }
}

void Partie::terminerTour()
{
    if (etatPartie == EtatPartie::DERNIER_TOUR) {
        --toursRestants;
        if (toursRestants <= 0) {
            terminerPartie();
            return;
        }
    }
    int suivant = (indexCourant + 1) % nombreJoueurs();
    if (suivant == 0) {
        ++compteurTours;
    }
    indexCourant = suivant;
}

int Partie::pointsObjectifs(int indiceJoueur) const
{
    if (indiceJoueur < 0 || indiceJoueur >= nombreJoueurs()) {
        throw SaisieInvalide("joueur inconnu");
    }
    const Joueur& cible = joueurs[static_cast<std::size_t>(indiceJoueur)];
    int total = 0;
    total += objectifsCommuns[0]->evaluer(cible.zone());
    total += objectifsCommuns[1]->evaluer(cible.zone());
    total += cible.pointsObjectifSecret();
    return total;
}

void Partie::terminerPartie()
{
    classement.clear();
    for (std::size_t i = 0; i < joueurs.size(); ++i) {
        ResultatJoueur resultat;
        resultat.nom = joueurs[i].nom();
        resultat.ordinateur = joueurs[i].estOrdinateur();
        resultat.pointsCartes = joueurs[i].score();
        resultat.pointsObjectifs = pointsObjectifs(static_cast<int>(i));
        resultat.total = resultat.pointsCartes + resultat.pointsObjectifs;
        classement.push_back(resultat);
    }
    etatPartie = EtatPartie::TERMINEE;
}

const std::vector<ResultatJoueur>& Partie::resultats() const
{
    return classement;
}

int Partie::gagnant() const
{
    if (classement.empty()) {
        return -1;
    }
    std::size_t meilleur = 0;
    for (std::size_t i = 1; i < classement.size(); ++i) {
        if (classement[i].total > classement[meilleur].total) {
            meilleur = i;
        } else if (classement[i].total == classement[meilleur].total
                   && classement[i].pointsObjectifs > classement[meilleur].pointsObjectifs) {
            meilleur = i;
        }
    }
    for (std::size_t i = 0; i < classement.size(); ++i) {
        if (i == meilleur) {
            continue;
        }
        if (classement[i].total == classement[meilleur].total
            && classement[i].pointsObjectifs == classement[meilleur].pointsObjectifs) {
            return -1;
        }
    }
    return static_cast<int>(meilleur);
}

bool Partie::egalite() const
{
    return gagnant() < 0 && !classement.empty();
}

} // namespace codex
