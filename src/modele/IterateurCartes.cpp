#include "modele/IterateurCartes.h"

#include "modele/ZoneDeJeu.h"
#include "noyau/ExceptionJeu.h"

namespace codex {

IterateurCartes::IterateurCartes(const std::vector<CartePlacee>& cartesPosees)
    : cartes(&cartesPosees), indice(0)
{
}

void IterateurCartes::premier()
{
    indice = 0;
}

bool IterateurCartes::estFini() const
{
    return indice >= cartes->size();
}

const CartePlacee& IterateurCartes::courant() const
{
    if (estFini()) {
        throw SaisieInvalide("parcours termine : aucune carte courante");
    }
    return (*cartes)[indice];
}

void IterateurCartes::suivant()
{
    if (!estFini()) {
        ++indice;
    }
}

} // namespace codex
