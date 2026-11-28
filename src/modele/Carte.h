#ifndef CODEX_CARTE_H
#define CODEX_CARTE_H

#include "noyau/Ressource.h"

#include <map>
#include <string>

namespace codex {

class FaceCarte;

// Informations fournies a une carte au moment ou l'on calcule les points
// qu'elle rapporte en etant posee.
struct ContextePose
{
    int coinsCouverts;                    // nombre de coins d'autres cartes recouverts
    std::map<Ressource, int> ressources;  // ressources visibles APRES la pose

    ContextePose();

    int nombreVisible(Ressource r) const;
};

// Classe abstraite de toutes les cartes du jeu.
class Carte
{
public:
    Carte(int identifiant, Ressource royaume, int categorie);
    virtual ~Carte();

    int id() const;
    Ressource royaume() const;
    int categorie() const;
    bool estDoree() const;
    bool estInitiale() const;

    virtual const FaceCarte& face(bool recto) const = 0;
    virtual std::string nom() const = 0;
    virtual std::string description() const = 0;

    // Les exigences de pose (seules les cartes Doree en ont).
    virtual bool exigencesSatisfaites(const std::map<Ressource, int>& ressources) const;
    virtual std::string texteExigences() const;

    // Patron de methode : la facon de compter les points ne change pas,
    // seules les deux etapes ci-dessous sont specialisees.
    int pointsAuPose(const ContextePose& contexte) const;

    // Nombre imprime sur la banniere, sans appliquer les multiplicateurs
    // (coins recouverts, objets). Sert au mode solo : Tybor ne retient que
    // le nombre de la carte Doree qu'il defausse.
    virtual int pointsImprimes() const;

    // Vrai si la carte porte l'un des objets indiques (coins du recto ou
    // ressources du centre). Sert a comparer une carte Doree aux objectifs.
    bool porteObjet(Ressource objet) const;

protected:
    virtual int pointsFixes() const = 0;
    virtual int pointsVariables(const ContextePose& contexte) const;

private:
    int identifiant;
    Ressource royaumeCarte;
    int categorieCarte;
};

} // namespace codex

#endif // CODEX_CARTE_H
