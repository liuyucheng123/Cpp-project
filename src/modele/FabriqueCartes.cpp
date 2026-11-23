#include "modele/FabriqueCartes.h"

#include "modele/CarteDoree.h"
#include "modele/CarteInitiale.h"
#include "modele/CarteRessource.h"
#include "modele/DonneesCartes.h"

namespace codex {

FabriqueCartes::~FabriqueCartes()
{
}

std::shared_ptr<const Carte> FabriqueCarteRessource::fabriquer(const DonneeCarte& donnee) const
{
    std::shared_ptr<const Carte> carte = std::make_shared<CarteRessource>(donnee);
    return carte;
}

std::shared_ptr<const Carte> FabriqueCarteDoree::fabriquer(const DonneeCarte& donnee) const
{
    std::shared_ptr<const Carte> carte = std::make_shared<CarteDoree>(donnee);
    return carte;
}

std::shared_ptr<const Carte> FabriqueCarteInitiale::fabriquer(const DonneeCarte& donnee) const
{
    std::shared_ptr<const Carte> carte = std::make_shared<CarteInitiale>(donnee);
    return carte;
}

} // namespace codex
