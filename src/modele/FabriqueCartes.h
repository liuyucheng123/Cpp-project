#ifndef CODEX_FABRIQUE_CARTES_H
#define CODEX_FABRIQUE_CARTES_H

#include <memory>

namespace codex {

class Carte;
struct DonneeCarte;

// Patron Fabrique (Factory Method) : la creation d'une carte est deleguee
// a une classe dediee, si bien que le catalogue ne connait pas les classes concretes.
class FabriqueCartes
{
public:
    virtual ~FabriqueCartes();
    virtual std::shared_ptr<const Carte> fabriquer(const DonneeCarte& donnee) const = 0;
};

class FabriqueCarteRessource : public FabriqueCartes
{
public:
    std::shared_ptr<const Carte> fabriquer(const DonneeCarte& donnee) const;
};

class FabriqueCarteDoree : public FabriqueCartes
{
public:
    std::shared_ptr<const Carte> fabriquer(const DonneeCarte& donnee) const;
};

class FabriqueCarteInitiale : public FabriqueCartes
{
public:
    std::shared_ptr<const Carte> fabriquer(const DonneeCarte& donnee) const;
};

} // namespace codex

#endif // CODEX_FABRIQUE_CARTES_H
