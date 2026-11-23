#include "modele/FabriqueObjectif.h"

#include "modele/DonneesCartes.h"
#include "modele/Objectif.h"

namespace codex {

FabriqueObjectif::~FabriqueObjectif()
{
}

std::shared_ptr<const Objectif> FabriqueObjectifDiagonale::fabriquer(const DonneeObjectif& donnee) const
{
    std::shared_ptr<const Objectif> objectif =
        std::make_shared<ObjectifMotif>(donnee.id, donnee.royaume, donnee.score, true);
    return objectif;
}

std::shared_ptr<const Objectif> FabriqueObjectifEnL::fabriquer(const DonneeObjectif& donnee) const
{
    std::shared_ptr<const Objectif> objectif =
        std::make_shared<ObjectifMotif>(donnee.id, donnee.royaume, donnee.score, false);
    return objectif;
}

std::shared_ptr<const Objectif> FabriqueObjectifSerieRessource::fabriquer(const DonneeObjectif& donnee) const
{
    std::shared_ptr<const Objectif> objectif =
        std::make_shared<ObjectifSerieRessource>(donnee.id, donnee.royaume, donnee.score);
    return objectif;
}

std::shared_ptr<const Objectif> FabriqueObjectifPairesObjet::fabriquer(const DonneeObjectif& donnee) const
{
    std::shared_ptr<const Objectif> objectif =
        std::make_shared<ObjectifPairesObjet>(donnee.id, donnee.objet, donnee.score);
    return objectif;
}

std::shared_ptr<const Objectif> FabriqueObjectifTripletObjets::fabriquer(const DonneeObjectif& donnee) const
{
    std::shared_ptr<const Objectif> objectif =
        std::make_shared<ObjectifTripletObjets>(donnee.id, donnee.score);
    return objectif;
}

} // namespace codex
