#ifndef CODEX_PARTIE_H
#define CODEX_PARTIE_H

#include "modele/ActionTour.h"
#include "modele/ConfigurationPartie.h"
#include "modele/Joueur.h"
#include "modele/Pioche.h"

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
    const Joueur& joueur(int indice) const;
    Joueur& joueur(int indice);
    const Pioche& pioche() const;
    std::shared_ptr<const Objectif> objectifCommun(int indice) const;

    void jouerCarte(int indiceMain, Coordonnee origine, bool recto);
    void piocher(SourcePioche source, int indiceEtalee);
    void passerTour();

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
    std::shared_ptr<const Objectif> objectifsCommuns[2];
    std::vector<std::shared_ptr<const Carte> > initialesDisponibles;
    std::vector<int> objectifsDisponibles;
    int indexCourant;
    bool attenteDePioche;
    int toursRestants;
    int passesConsecutifs;
    std::vector<ResultatJoueur> classement;
};

} // namespace codex

#endif // CODEX_PARTIE_H
