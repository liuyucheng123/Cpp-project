#ifndef CODEX_FACE_CARTE_H
#define CODEX_FACE_CARTE_H

#include "noyau/Coin.h"
#include "noyau/Ressource.h"

#include <array>
#include <vector>

namespace codex {

// Une face de carte : quatre coins et les ressources permanentes du centre.
class FaceCarte
{
public:
    FaceCarte();

    Ressource coin(int indiceCoin) const;
    int nombreCentres() const;
    Ressource centre(int indice) const;
    void ajouterCentre(Ressource r);
    void fixerCoin(int indiceCoin, Ressource r);

private:
    std::array<Ressource, NB_COINS> coins;
    std::vector<Ressource> centres;
};

} // namespace codex

#endif // CODEX_FACE_CARTE_H
