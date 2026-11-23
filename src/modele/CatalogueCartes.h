#ifndef CODEX_CATALOGUE_CARTES_H
#define CODEX_CATALOGUE_CARTES_H

#include "modele/DonneesCartes.h"

#include <map>
#include <memory>
#include <vector>

namespace codex {

class Carte;
class Objectif;
class FabriqueCartes;
class FabriqueObjectif;

// Patron Singleton : une seule base de cartes pour toute l'application.
// C'est aussi une fabrique : on demande une carte par son identifiant.
class CatalogueCartes
{
public:
    static CatalogueCartes& instance();

    std::shared_ptr<const Carte> creerCarte(int identifiant) const;
    std::shared_ptr<const Objectif> creerObjectif(int identifiant) const;

    const std::vector<int>& idsRessource() const;
    const std::vector<int>& idsDoree() const;
    const std::vector<int>& idsInitiale() const;
    const std::vector<int>& idsObjectif() const;

    int nombreCartes() const;
    int nombreObjectifs() const;

private:
    CatalogueCartes();
    CatalogueCartes(const CatalogueCartes& autre);
    void operator=(const CatalogueCartes& autre);

    std::shared_ptr<const FabriqueObjectif> fabriquePour(TypeObjectif type) const;

    static CatalogueCartes* instanceUnique;

    std::map<int, std::shared_ptr<const Carte> > cartes;
    std::map<int, std::shared_ptr<const Objectif> > objectifs;
    std::map<int, std::shared_ptr<FabriqueCartes> > fabriquesCartes;
    std::map<TypeObjectif, std::shared_ptr<FabriqueObjectif> > fabriquesObjectifs;
    std::vector<int> listeRessource;
    std::vector<int> listeDoree;
    std::vector<int> listeInitiale;
    std::vector<int> listeObjectif;
};

} // namespace codex

#endif // CODEX_CATALOGUE_CARTES_H
