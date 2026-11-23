#ifndef CODEX_CARTE_RESSOURCE_H
#define CODEX_CARTE_RESSOURCE_H

#include "modele/Carte.h"
#include "modele/FaceCarte.h"

namespace codex {

struct DonneeCarte;

// Carte du paquet Ressource : deux faces (le dos rapporte une ressource permanente).
class CarteRessource : public Carte
{
public:
    CarteRessource(const DonneeCarte& donnee);

    const FaceCarte& face(bool recto) const;
    std::string nom() const;
    std::string description() const;

protected:
    int pointsFixes() const;

private:
    FaceCarte faceRecto;
    FaceCarte faceVerso;
    int pointsBanniere;
};

} // namespace codex

#endif // CODEX_CARTE_RESSOURCE_H
