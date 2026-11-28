#include "modele/CarteDoree.h"

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

CarteDoree::CarteDoree(const DonneeCarte& donnee)
    : Carte(donnee.id, donnee.royaume, donnee.categorie),
      faceRecto(construireFace(donnee.recto)),
      faceVerso(construireFace(donnee.verso)),
      pointsBanniere(donnee.score),
      typeScore(donnee.typeScore),
      objetScore(donnee.objetScore)
{
    const Ressource royaumes[4] = { Ressource::PLANTE, Ressource::ANIMAL,
                                    Ressource::CHAMPIGNON, Ressource::INSECTE };
    for (int i = 0; i < 4; ++i) {
        if (donnee.exigences[i] > 0) {
            exigences[royaumes[i]] = donnee.exigences[i];
        }
    }
}

const FaceCarte& CarteDoree::face(bool recto) const
{
    if (recto) {
        return faceRecto;
    }
    return faceVerso;
}

std::string CarteDoree::nom() const
{
    std::ostringstream flux;
    flux << "D" << id();
    return flux.str();
}

std::string CarteDoree::description() const
{
    std::ostringstream flux;
    flux << nom() << " (" << nomRessource(royaume()) << ")";
    if (typeScore == TypeScoreDore::STATIQUE) {
        flux << " +" << pointsBanniere << " pts";
    } else if (typeScore == TypeScoreDore::PAR_ANGLE) {
        flux << " +" << pointsBanniere << " pts par coin recouvert";
    } else if (typeScore == TypeScoreDore::PAR_OBJET) {
        flux << " +" << pointsBanniere << " pt par " << nomRessource(objetScore);
    }
    flux << " | " << texteExigences();
    return flux.str();
}

bool CarteDoree::exigencesSatisfaites(const std::map<Ressource, int>& ressources) const
{
    std::map<Ressource, int>::const_iterator it = exigences.begin();
    while (it != exigences.end()) {
        std::map<Ressource, int>::const_iterator dispo = ressources.find(it->first);
        int quantite = (dispo == ressources.end()) ? 0 : dispo->second;
        if (quantite < it->second) {
            return false;
        }
        ++it;
    }
    return true;
}

std::string CarteDoree::texteExigences() const
{
    std::ostringstream flux;
    flux << "exige";
    std::map<Ressource, int>::const_iterator it = exigences.begin();
    while (it != exigences.end()) {
        flux << " " << it->second << " " << nomRessource(it->first);
        ++it;
    }
    if (exigences.empty()) {
        flux << " rien";
    }
    return flux.str();
}

int CarteDoree::pointsFixes() const
{
    if (typeScore == TypeScoreDore::STATIQUE) {
        return pointsBanniere;
    }
    return 0;
}

int CarteDoree::pointsImprimes() const
{
    // Le nombre de la banniere, meme pour les cartes "par coin" ou "par objet" :
    // c'est ce nombre que Tybor encaisse en solo, sans les multiplicateurs.
    return pointsBanniere;
}

int CarteDoree::pointsVariables(const ContextePose& contexte) const
{
    if (typeScore == TypeScoreDore::PAR_ANGLE) {
        return pointsBanniere * contexte.coinsCouverts;
    }
    if (typeScore == TypeScoreDore::PAR_OBJET) {
        return pointsBanniere * contexte.nombreVisible(objetScore);
    }
    return 0;
}

} // namespace codex
