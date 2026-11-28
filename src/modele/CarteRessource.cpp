#include "modele/CarteRessource.h"

#include "modele/DonneesCartes.h"

#include <sstream>

namespace codex {
namespace {

FaceCarte construireFace(const DonneeFace& donnee)
{
    FaceCarte face;
    for (int i = 0; i < NB_COINS; ++i) {
        face.fixerCoin(i, donnee.coins[i]);
    }
    for (int i = 0; i < donnee.nbCentres; ++i) {
        face.ajouterCentre(donnee.centres[i]);
    }
    return face;
}

} // namespace

CarteRessource::CarteRessource(const DonneeCarte& donnee)
    : Carte(donnee.id, donnee.royaume, donnee.categorie),
      faceRecto(construireFace(donnee.recto)),
      faceVerso(construireFace(donnee.verso)),
      pointsBanniere(donnee.score)
{
}

const FaceCarte& CarteRessource::face(bool recto) const
{
    if (recto) {
        return faceRecto;
    }
    return faceVerso;
}

std::string CarteRessource::nom() const
{
    std::ostringstream flux;
    flux << "R" << id();
    return flux.str();
}

std::string CarteRessource::description() const
{
    std::ostringstream flux;
    flux << nom() << " (" << nomRessource(royaume()) << ")";
    if (pointsBanniere > 0) {
        flux << " +" << pointsBanniere << " pt";
    }
    return flux.str();
}

int CarteRessource::pointsFixes() const
{
    return pointsBanniere;
}

int CarteRessource::pointsImprimes() const
{
    return pointsBanniere;
}

} // namespace codex
