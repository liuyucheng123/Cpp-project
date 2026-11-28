#ifndef CODEX_PARTIE_H
#define CODEX_PARTIE_H

#include "modele/ActionTour.h"
#include "modele/ConfigurationPartie.h"
#include "modele/Joueur.h"
#include "modele/Pioche.h"
#include "modele/Tybor.h"

#include <memory>
#include <string>
#include <vector>

namespace codex {

class Carte;
class Objectif;

enum class EtatPartie
{
    NON_DEMARREE,
    PREPARATION,
    EN_COURS,
    DERNIER_TOUR,
    TERMINEE
};

struct ResultatJoueur
{
    std::string nom;
    int pointsCartes;
    int pointsObjectifs;
    int total;
    bool ordinateur;

    ResultatJoueur();
};

// Le moteur du jeu : il ne fait aucune entree/sortie.
// Les regles vivent ici, l'affichage et les saisies sont ailleurs.
class Partie
{
public:
    Partie();

    void preparer(const ConfigurationPartie& configuration, GenerateurAleatoire& generateur);

    bool preparationTerminee() const;
    void choisirFaceInitiale(int indiceJoueur, bool recto);
    void choisirObjectifSecret(int indiceJoueur, int indiceCandidat);
    void commencer();

    EtatPartie etat() const;
    bool estTerminee() const;
    bool attentePioche() const;
    int nombreJoueurs() const;
    int joueurCourant() const;
    // Numero du tour en cours (1 au premier tour, +1 quand on revient au premier joueur).
    int numeroTour() const;
    const Joueur& joueur(int indice) const;
    Joueur& joueur(int indice);
    const Pioche& pioche() const;
    std::shared_ptr<const Objectif> objectifCommun(int indice) const;

    // Mode solo officiel : un seul joueur humain, plus Tybor Kwelein.
    bool estModeSolo() const;
    int indiceTybor() const;   // -1 hors mode solo
    const Tybor& adversaire() const;

    void jouerCarte(int indiceMain, Coordonnee origine, bool recto);
    void piocher(SourcePioche source, int indiceEtalee);
    void passerTour();

    // Tour de Tybor (mode solo) : renvoie le journal de son tour.
    std::string jouerTourTybor();

    // Vrai si le joueur peut poser au moins une de ses cartes quelque part.
    bool placementPossible(int indiceJoueur) const;

    const std::vector<ResultatJoueur>& resultats() const;
    int pointsObjectifs(int indiceJoueur) const;
    int gagnant() const;
    bool egalite() const;

private:
    void verifierTour() const;
    void verifierFinDePartie();
    void terminerTour();
    void terminerPartie();

    EtatPartie etatPartie;
    ConfigurationPartie configuration;
    std::vector<Joueur> joueurs;
    Pioche piocheCartes;
    Tybor tybor;
    std::shared_ptr<const Objectif> objectifsCommuns[2];
    std::vector<std::shared_ptr<const Carte> > initialesDisponibles;
    std::vector<int> objectifsDisponibles;
    int indexCourant;
    int compteurTours;
    bool attenteDePioche;
    int toursRestants;
    int passesConsecutifs;
    std::vector<ResultatJoueur> classement;
};

} // namespace codex

#endif // CODEX_PARTIE_H
