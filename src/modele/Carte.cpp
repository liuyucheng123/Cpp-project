#include "modele/Carte.h"

#include "modele/DonneesCartes.h"
#include "modele/FaceCarte.h"
#include "noyau/Coin.h"

namespace codex {

ContextePose::ContextePose() : coinsCouverts(0)
{
}

int ContextePose::nombreVisible(Ressource r) const
{
    std::map<Ressource, int>::const_iterator it = ressources.find(r);
    if (it == ressources.end()) {
        return 0;
    }
    return it->second;
}

Carte::Carte(int identifiant, Ressource royaume, int categorie)
    : identifiant(identifiant), royaumeCarte(royaume), categorieCarte(categorie)
{
}

Carte::~Carte()
{
}

int Carte::id() const
{
    return identifiant;
}

Ressource Carte::royaume() const
{
    return royaumeCarte;
}

int Carte::categorie() const
{
    return categorieCarte;
}

bool Carte::estDoree() const
{
    return categorieCarte == CATEGORIE_DOREE;
}

bool Carte::estInitiale() const
{
    return categorieCarte == CATEGORIE_INITIALE;
}

bool Carte::exigencesSatisfaites(const std::map<Ressource, int>& ressources) const
{
    (void)ressources;
    return true;
}

std::string Carte::texteExigences() const
{
    return "";
}

int Carte::pointsAuPose(const ContextePose& contexte) const
{
    int total = pointsFixes();
    total += pointsVariables(contexte);
    return total;
}

int Carte::pointsImprimes() const
{
    return 0;
}

bool Carte::porteObjet(Ressource objet) const
{
    const FaceCarte& recto = face(true);
    for (int i = 0; i < NB_COINS; ++i) {
        if (recto.coin(i) == objet) {
            return true;
        }
    }
    for (int i = 0; i < recto.nombreCentres(); ++i) {
        if (recto.centre(i) == objet) {
            return true;
        }
    }
    return false;
}

int Carte::pointsVariables(const ContextePose& contexte) const
{
    (void)contexte;
    return 0;
}

} // namespace codex
