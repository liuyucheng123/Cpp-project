#include "modele/Pioche.h"

#include "modele/Carte.h"
#include "modele/CatalogueCartes.h"
#include "modele/ConfigurationPartie.h"
#include "noyau/ExceptionJeu.h"
#include "noyau/Melange.h"

namespace codex {
namespace {

std::vector<std::shared_ptr<const Carte> > construireTas(const std::vector<int>& ids)
{
    std::vector<std::shared_ptr<const Carte> > tas;
    const CatalogueCartes& catalogue = CatalogueCartes::instance();
    for (std::size_t i = 0; i < ids.size(); ++i) {
        tas.push_back(catalogue.creerCarte(ids[i]));
    }
    return tas;
}

} // namespace

Pioche::Pioche()
{
    for (int i = 0; i < 4; ++i) {
        etalees[i] = std::shared_ptr<const Carte>();
    }
}

void Pioche::initialiser(const std::vector<int>& idsRessource, const std::vector<int>& idsDoree,
                         GenerateurAleatoire& generateur)
{
    tasRessource = construireTas(idsRessource);
    tasDoree = construireTas(idsDoree);
    melanger(tasRessource, generateur);
    melanger(tasDoree, generateur);
    for (int i = 0; i < 4; ++i) {
        etalees[i] = std::shared_ptr<const Carte>();
    }
    completerEtalees();
}

int Pioche::tailleTas(bool doree) const
{
    return static_cast<int>(doree ? tasDoree.size() : tasRessource.size());
}

bool Pioche::tasVide(bool doree) const
{
    return tailleTas(doree) == 0;
}

bool Pioche::epuisee() const
{
    return tasRessource.empty() && tasDoree.empty();
}

bool Pioche::plusDeCartesDuTout() const
{
    if (!epuisee()) {
        return false;
    }
    for (int i = 0; i < 4; ++i) {
        if (etalees[i]) {
            return false;
        }
    }
    return true;
}

std::shared_ptr<const Carte> Pioche::etalee(int indice) const
{
    if (indice < 0 || indice >= 4) {
        throw CarteAbsente("indice de carte visible invalide");
    }
    return etalees[indice];
}

bool Pioche::etaleeDisponible(int indice) const
{
    if (indice < 0 || indice >= 4) {
        return false;
    }
    return etalees[indice].get() != 0;
}

void Pioche::completerEtalees()
{
    for (int i = 0; i < 4; ++i) {
        if (etalees[i]) {
            continue;
        }
        bool doree = (i >= 2);
        std::vector<std::shared_ptr<const Carte> >& tas = (doree ? tasDoree : tasRessource);
        if (tas.empty()) {
            continue;
        }
        etalees[i] = tas.back();
        tas.pop_back();
    }
}

std::shared_ptr<const Carte> Pioche::prendreEtalee(int indice)
{
    if (!etaleeDisponible(indice)) {
        throw CarteAbsente("cette carte visible n'est plus disponible");
    }
    std::shared_ptr<const Carte> carte = etalees[indice];
    etalees[indice] = std::shared_ptr<const Carte>();
    return carte;
}

std::shared_ptr<const Carte> Pioche::piocherSurLeTas(bool doree)
{
    std::vector<std::shared_ptr<const Carte> >& tas = (doree ? tasDoree : tasRessource);
    if (tas.empty()) {
        throw CarteAbsente("le tas demande est vide");
    }
    std::shared_ptr<const Carte> carte = tas.back();
    tas.pop_back();
    return carte;
}

int Pioche::cartesRestantes() const
{
    int total = static_cast<int>(tasRessource.size() + tasDoree.size());
    for (int i = 0; i < 4; ++i) {
        if (etalees[i]) {
            ++total;
        }
    }
    return total;
}

} // namespace codex
