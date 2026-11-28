#ifndef CODEX_CARTE_DOREE_H
#define CODEX_CARTE_DOREE_H

#include "modele/Carte.h"
#include "modele/FaceCarte.h"
#include "modele/DonneesCartes.h"

#include <map>

namespace codex {

// Carte du paquet Doree : elle exige des ressources pour etre posee
// et rapporte ses points de trois facons differentes.
class CarteDoree : public Carte
{
public:
    CarteDoree(const DonneeCarte& donnee);

    const FaceCarte& face(bool recto) const;
    std::string nom() const;
    std::string description() const;

    bool exigencesSatisfaites(const std::map<Ressource, int>& ressources) const;
    std::string texteExigences() const;
    int pointsImprimes() const;

protected:
    int pointsFixes() const;
    int pointsVariables(const ContextePose& contexte) const;

private:
    FaceCarte faceRecto;
    FaceCarte faceVerso;
    int pointsBanniere;
    TypeScoreDore typeScore;
    Ressource objetScore;
    std::map<Ressource, int> exigences;
};

} // namespace codex

#endif // CODEX_CARTE_DOREE_H
