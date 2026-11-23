#ifndef CODEX_CARTE_INITIALE_H
#define CODEX_CARTE_INITIALE_H

#include "modele/Carte.h"
#include "modele/FaceCarte.h"

namespace codex {

struct DonneeCarte;

// Carte de depart : deux faces possibles, ressources permanentes au centre.
class CarteInitiale : public Carte
{
public:
    CarteInitiale(const DonneeCarte& donnee);

    const FaceCarte& face(bool recto) const;
    std::string nom() const;
    std::string description() const;

protected:
    int pointsFixes() const;

private:
    FaceCarte faceRecto;
    FaceCarte faceVerso;
};

} // namespace codex

#endif // CODEX_CARTE_INITIALE_H
