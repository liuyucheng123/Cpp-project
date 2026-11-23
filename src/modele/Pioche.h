#ifndef CODEX_PIOCHE_H
#define CODEX_PIOCHE_H

#include "noyau/GenerateurAleatoire.h"

#include <memory>
#include <vector>

namespace codex {

class Carte;

// Les deux tas de cartes et les quatre cartes visibles au centre de la table.
class Pioche
{
public:
    Pioche();

    void initialiser(const std::vector<int>& idsRessource, const std::vector<int>& idsDoree,
                      GenerateurAleatoire& generateur);

    int tailleTas(bool doree) const;
    bool tasVide(bool doree) const;
    bool epuisee() const;               // les deux tas sont vides
    bool plusDeCartesDuTout() const;    // tas vides et aucune carte visible

    std::shared_ptr<const Carte> etalee(int indice) const;
    bool etaleeDisponible(int indice) const;
    void completerEtalees();

    std::shared_ptr<const Carte> prendreEtalee(int indice);
    std::shared_ptr<const Carte> piocherSurLeTas(bool doree);

    int cartesRestantes() const;

private:
    std::vector<std::shared_ptr<const Carte> > tasRessource;
    std::vector<std::shared_ptr<const Carte> > tasDoree;
    std::shared_ptr<const Carte> etalees[4];   // 0 et 1 : ressources, 2 et 3 : dorees
};

} // namespace codex

#endif // CODEX_PIOCHE_H
