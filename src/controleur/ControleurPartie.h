#ifndef CODEX_CONTROLEUR_PARTIE_H
#define CODEX_CONTROLEUR_PARTIE_H

#include "modele/Partie.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace codex {

class Saisie;
class StrategieIA;
class Vue;

// Fait le lien entre le joueur (clavier ou ordinateur), l'affichage et le moteur.
// C'est le seul endroit qui attrape les exceptions metier pour les transformer
// en messages comprehensibles.
class ControleurPartie
{
public:
    ControleurPartie(Partie& partie, Vue& vue, Saisie& saisie, GenerateurAleatoire& generateur);

    void executer();

private:
    void phasePreparation();
    void boucleDeJeu();
    void tourHumain();
    void tourOrdinateur();
    void tourTybor();
    void jouerUnTourHumain();
    void poserCarteHumaine();
    void piocherHumain();

    std::vector<Coordonnee> placementsPossibles(const Joueur& joueur,
                                                const std::shared_ptr<const Carte>& carte,
                                                bool recto) const;

    int demanderEntier(const std::string& question, int minimum, int maximum);

    Partie& partie;
    Vue& vue;
    Saisie& saisie;
    GenerateurAleatoire& generateur;
    std::map<int, std::shared_ptr<StrategieIA> > strategies;
};

} // namespace codex

#endif // CODEX_CONTROLEUR_PARTIE_H
