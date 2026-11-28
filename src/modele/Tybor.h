#ifndef CODEX_TYBOR_H
#define CODEX_TYBOR_H

#include "noyau/Ressource.h"

#include <map>
#include <string>

namespace codex {

class Carte;
class Joueur;
class Objectif;
class Pioche;

// Adversaire du mode solo officiel : le moine enlumineur Tybor Kwelein.
// Il n'a ni main, ni codex, ni objectif secret : son tour consiste a prendre
// une carte dans chacune des deux colonnes de la rangee visible, puis a
// grossir sa reserve de ressources ou a marquer des points.
class Tybor
{
public:
    // 0 = facile (deux royaumes au depart), 1 = normal (les quatre),
    // 2 = difficile (les quatre, et 4 points d'avance).
    explicit Tybor(int difficulte = 1);

    int difficulte() const;
    int ressourcesDe(Ressource royaume) const;
    int totalRessources() const;
    std::string description() const;

    // Joue un tour complet (deux colonnes) et renvoie le journal de ce tour.
    // Les points gagnes sont ajoutes au joueur qui represente Tybor.
    std::string jouerTour(Pioche& pioche, const Objectif& objectifA, const Objectif& objectifB,
                          Joueur& joueurTybor);

private:
    bool peutPayer(const Carte& carte) const;
    void gagnerRessource(Ressource royaume);
    std::string traiterColonne(Pioche& pioche, int indiceRessource, int indiceDoree,
                               const Objectif& objectifA, const Objectif& objectifB,
                               Joueur& joueurTybor);

    std::map<Ressource, int> ressources;
    int niveauDifficulte;
};

} // namespace codex

#endif // CODEX_TYBOR_H
