#ifndef CODEX_ITERATEUR_CARTES_H
#define CODEX_ITERATEUR_CARTES_H

#include <cstddef>
#include <vector>

namespace codex {

struct CartePlacee;

// Patron Iterateur : parcours sequentiel des cartes posees sans exposer
// la structure de donnees interne de ZoneDeJeu.
class IterateurCartes
{
public:
    explicit IterateurCartes(const std::vector<CartePlacee>& cartes);

    void premier();
    bool estFini() const;
    const CartePlacee& courant() const;
    void suivant();

private:
    const std::vector<CartePlacee>* cartes;
    std::size_t indice;
};

} // namespace codex

#endif // CODEX_ITERATEUR_CARTES_H
