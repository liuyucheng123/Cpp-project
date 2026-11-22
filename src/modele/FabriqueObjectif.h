#ifndef CODEX_FABRIQUE_OBJECTIF_H
#define CODEX_FABRIQUE_OBJECTIF_H

#include <memory>

namespace codex {

class Objectif;
struct DonneeObjectif;

// Fabrique abstraite d'objectifs : chaque type d'objectif a sa classe de fabrication.
// Ajouter un nouveau type d'objectif revient a ajouter une sous-classe ici,
// sans modifier le catalogue ni le moteur du jeu.
class FabriqueObjectif
{
public:
    virtual ~FabriqueObjectif();
    virtual std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const = 0;
};

class FabriqueObjectifDiagonale : public FabriqueObjectif
{
public:
    std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const;
};

class FabriqueObjectifEnL : public FabriqueObjectif
{
public:
    std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const;
};

class FabriqueObjectifSerieRessource : public FabriqueObjectif
{
public:
    std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const;
};

class FabriqueObjectifPairesObjet : public FabriqueObjectif
{
public:
    std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const;
};

class FabriqueObjectifTripletObjets : public FabriqueObjectif
{
public:
    std::shared_ptr<const Objectif> fabriquer(const DonneeObjectif& donnee) const;
};

} // namespace codex

#endif // CODEX_FABRIQUE_OBJECTIF_H
