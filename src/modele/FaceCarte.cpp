#include "modele/FaceCarte.h"

#include "noyau/ExceptionJeu.h"

namespace codex {

FaceCarte::FaceCarte()
{
    for (int i = 0; i < NB_COINS; ++i) {
        coins[static_cast<std::size_t>(i)] = Ressource::CACHE;
    }
}

Ressource FaceCarte::coin(int indiceCoin) const
{
    if (indiceCoin < 0 || indiceCoin >= NB_COINS) {
        throw IdentifiantInconnu("indice de coin invalide");
    }
    return coins[static_cast<std::size_t>(indiceCoin)];
}

int FaceCarte::nombreCentres() const
{
    return static_cast<int>(centres.size());
}

Ressource FaceCarte::centre(int indice) const
{
    if (indice < 0 || indice >= nombreCentres()) {
        throw IdentifiantInconnu("indice de ressource centrale invalide");
    }
    return centres[static_cast<std::size_t>(indice)];
}

void FaceCarte::ajouterCentre(Ressource r)
{
    centres.push_back(r);
}

void FaceCarte::fixerCoin(int indiceCoin, Ressource r)
{
    if (indiceCoin < 0 || indiceCoin >= NB_COINS) {
        throw IdentifiantInconnu("indice de coin invalide");
    }
    coins[static_cast<std::size_t>(indiceCoin)] = r;
}

} // namespace codex
