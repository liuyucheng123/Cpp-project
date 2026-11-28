#include "modele/Joueur.h"

#include "modele/Carte.h"
#include "modele/Objectif.h"
#include "noyau/ExceptionJeu.h"

#include <sstream>

namespace codex {

Joueur::Joueur()
    : nomJoueur("Joueur"), ordinateur(false), niveauIA(0), adversaireTybor(false), zoneJeu(),
      cartesMain(), objectifsCandidats(), objectifRetenu(), initiale(),
      faceChoisie(false), rectoInitial(true), points(0)
{
}

Joueur::Joueur(const std::string& nomJoueurParam, bool estOrdinateur, int niveau,
               bool adversaireSolo)
    : nomJoueur(nomJoueurParam), ordinateur(estOrdinateur), niveauIA(niveau),
      adversaireTybor(adversaireSolo), zoneJeu(),
      cartesMain(), objectifsCandidats(), objectifRetenu(), initiale(),
      faceChoisie(false), rectoInitial(true), points(0)
{
}

const std::string& Joueur::nom() const
{
    return nomJoueur;
}

bool Joueur::estOrdinateur() const
{
    return ordinateur;
}

int Joueur::niveau() const
{
    return niveauIA;
}

bool Joueur::estTybor() const
{
    return adversaireTybor;
}

ZoneDeJeu& Joueur::zone()
{
    return zoneJeu;
}

const ZoneDeJeu& Joueur::zone() const
{
    return zoneJeu;
}

int Joueur::nombreCartesEnMain() const
{
    return static_cast<int>(cartesMain.size());
}

std::shared_ptr<const Carte> Joueur::main(int indice) const
{
    if (indice < 0 || indice >= nombreCartesEnMain()) {
        throw CarteAbsente("indice de carte en main invalide");
    }
    return cartesMain[static_cast<std::size_t>(indice)];
}

void Joueur::ajouterEnMain(std::shared_ptr<const Carte> carte)
{
    if (!carte) {
        throw CarteAbsente("carte vide impossible");
    }
    cartesMain.push_back(carte);
}

std::shared_ptr<const Carte> Joueur::retirerDeMain(int indice)
{
    if (indice < 0 || indice >= nombreCartesEnMain()) {
        throw CarteAbsente("indice de carte en main invalide");
    }
    std::shared_ptr<const Carte> carte = cartesMain[static_cast<std::size_t>(indice)];
    cartesMain.erase(cartesMain.begin() + indice);
    return carte;
}

int Joueur::score() const
{
    return points;
}

void Joueur::ajouterPoints(int pointsGagnes)
{
    if (pointsGagnes < 0) {
        throw ExceptionJeu("un gain de points ne peut pas etre negatif");
    }
    points += pointsGagnes;
}

std::shared_ptr<const Carte> Joueur::carteInitiale() const
{
    return initiale;
}

void Joueur::fixerCarteInitiale(std::shared_ptr<const Carte> carte)
{
    initiale = carte;
}

bool Joueur::faceInitialeChoisie() const
{
    return faceChoisie;
}

bool Joueur::faceInitialeRecto() const
{
    return rectoInitial;
}

void Joueur::choisirFaceInitiale(bool recto)
{
    rectoInitial = recto;
    faceChoisie = true;
}

void Joueur::proposerObjectif(std::shared_ptr<const Objectif> objectif)
{
    if (objectif) {
        objectifsCandidats.push_back(objectif);
    }
}

const std::vector<std::shared_ptr<const Objectif> >& Joueur::candidatsObjectif() const
{
    return objectifsCandidats;
}

std::shared_ptr<const Objectif> Joueur::objectifSecret() const
{
    return objectifRetenu;
}

void Joueur::choisirObjectifSecret(int indiceCandidat)
{
    if (indiceCandidat < 0 || indiceCandidat >= static_cast<int>(objectifsCandidats.size())) {
        throw SaisieInvalide("objectif secret : choix invalide");
    }
    objectifRetenu = objectifsCandidats[static_cast<std::size_t>(indiceCandidat)];
}

bool Joueur::objectifChoisi() const
{
    return objectifRetenu.get() != 0;
}

int Joueur::pointsObjectifSecret() const
{
    if (!objectifRetenu) {
        return 0;
    }
    return objectifRetenu->evaluer(zoneJeu);
}

} // namespace codex
