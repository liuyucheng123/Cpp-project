#include "modele/CatalogueCartes.h"

#include "modele/Carte.h"
#include "modele/FabriqueCartes.h"
#include "modele/FabriqueObjectif.h"
#include "modele/Objectif.h"
#include "noyau/ExceptionJeu.h"

#include <sstream>

namespace codex {

CatalogueCartes* CatalogueCartes::instanceUnique = 0;

CatalogueCartes& CatalogueCartes::instance()
{
    if (instanceUnique == 0) {
        instanceUnique = new CatalogueCartes();
    }
    return *instanceUnique;
}

CatalogueCartes::CatalogueCartes()
{
    fabriquesCartes[CATEGORIE_RESSOURCE] = std::make_shared<FabriqueCarteRessource>();
    fabriquesCartes[CATEGORIE_DOREE] = std::make_shared<FabriqueCarteDoree>();
    fabriquesCartes[CATEGORIE_INITIALE] = std::make_shared<FabriqueCarteInitiale>();

    fabriquesObjectifs[TypeObjectif::DIAGONALE] = std::make_shared<FabriqueObjectifDiagonale>();
    fabriquesObjectifs[TypeObjectif::EN_L] = std::make_shared<FabriqueObjectifEnL>();
    fabriquesObjectifs[TypeObjectif::SERIE_RESSOURCE] = std::make_shared<FabriqueObjectifSerieRessource>();
    fabriquesObjectifs[TypeObjectif::PAIRES_OBJET] = std::make_shared<FabriqueObjectifPairesObjet>();
    fabriquesObjectifs[TypeObjectif::TRIPLET_OBJETS] = std::make_shared<FabriqueObjectifTripletObjets>();

    const std::vector<DonneeCarte>& donnees = donneesCartes();
    for (std::size_t i = 0; i < donnees.size(); ++i) {
        const DonneeCarte& donnee = donnees[i];
        std::map<int, std::shared_ptr<FabriqueCartes> >::const_iterator it = fabriquesCartes.find(donnee.categorie);
        if (it == fabriquesCartes.end()) {
            throw IdentifiantInconnu("categorie de carte inconnue");
        }
        std::shared_ptr<const Carte> carte = it->second->fabriquer(donnee);
        cartes[donnee.id] = carte;
        if (donnee.categorie == CATEGORIE_RESSOURCE) {
            listeRessource.push_back(donnee.id);
        } else if (donnee.categorie == CATEGORIE_DOREE) {
            listeDoree.push_back(donnee.id);
        } else {
            listeInitiale.push_back(donnee.id);
        }
    }

    const std::vector<DonneeObjectif>& tableObjectifs = donneesObjectifs();
    for (std::size_t i = 0; i < tableObjectifs.size(); ++i) {
        const DonneeObjectif& donnee = tableObjectifs[i];
        std::shared_ptr<const FabriqueObjectif> fabrique = fabriquePour(donnee.type);
        objectifs[donnee.id] = fabrique->fabriquer(donnee);
        listeObjectif.push_back(donnee.id);
    }
}

CatalogueCartes::CatalogueCartes(const CatalogueCartes& autre)
{
    (void)autre;
    throw IdentifiantInconnu("le catalogue est unique : copie interdite");
}

void CatalogueCartes::operator=(const CatalogueCartes& autre)
{
    (void)autre;
}

std::shared_ptr<const FabriqueObjectif> CatalogueCartes::fabriquePour(TypeObjectif type) const
{
    std::map<TypeObjectif, std::shared_ptr<FabriqueObjectif> >::const_iterator it = fabriquesObjectifs.find(type);
    if (it == fabriquesObjectifs.end()) {
        throw IdentifiantInconnu("type d'objectif inconnu");
    }
    return it->second;
}

std::shared_ptr<const Carte> CatalogueCartes::creerCarte(int identifiant) const
{
    std::map<int, std::shared_ptr<const Carte> >::const_iterator it = cartes.find(identifiant);
    if (it == cartes.end()) {
        std::ostringstream flux;
        flux << "carte inconnue : " << identifiant;
        throw IdentifiantInconnu(flux.str());
    }
    return it->second;
}

std::shared_ptr<const Objectif> CatalogueCartes::creerObjectif(int identifiant) const
{
    std::map<int, std::shared_ptr<const Objectif> >::const_iterator it = objectifs.find(identifiant);
    if (it == objectifs.end()) {
        std::ostringstream flux;
        flux << "objectif inconnu : " << identifiant;
        throw IdentifiantInconnu(flux.str());
    }
    return it->second;
}

const std::vector<int>& CatalogueCartes::idsRessource() const
{
    return listeRessource;
}

const std::vector<int>& CatalogueCartes::idsDoree() const
{
    return listeDoree;
}

const std::vector<int>& CatalogueCartes::idsInitiale() const
{
    return listeInitiale;
}

const std::vector<int>& CatalogueCartes::idsObjectif() const
{
    return listeObjectif;
}

int CatalogueCartes::nombreCartes() const
{
    return static_cast<int>(cartes.size());
}

int CatalogueCartes::nombreObjectifs() const
{
    return static_cast<int>(objectifs.size());
}

} // namespace codex
