#include "modele/CarteInitiale.h"

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

CarteInitiale::CarteInitiale(const DonneeCarte& donnee)
    : Carte(donnee.id, donnee.royaume, donnee.categorie),
      faceRecto(construireFace(donnee.recto)),
      faceVerso(construireFace(donnee.verso))
{
}

const FaceCarte& CarteInitiale::face(bool recto) const
{
    if (recto) {
        return faceRecto;
    }
    return faceVerso;
}

std::string CarteInitiale::nom() const
{
    std::ostringstream flux;
    flux << "I" << id();
    return flux.str();
}

std::string CarteInitiale::description() const
{
    std::ostringstream flux;
    flux << nom() << " (initiale)";
    return flux.str();
}

int CarteInitiale::pointsFixes() const
{
    return 0;
}

} // namespace codex
