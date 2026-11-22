#ifndef CODEX_JOUEUR_H
#define CODEX_JOUEUR_H

#include "modele/ZoneDeJeu.h"

#include <memory>
#include <string>
#include <vector>

namespace codex {

class Carte;
class Objectif;

// Un joueur : son nom, sa zone de jeu, sa main, son objectif secret et son score.
class Joueur
{
public:
    Joueur();
    Joueur(const std::string& nomJoueur, bool ordinateur, int niveau);

    const std::string& nom() const;
    bool estOrdinateur() const;
    int niveau() const;

    ZoneDeJeu& zone();
    const ZoneDeJeu& zone() const;

    int nombreCartesEnMain() const;
    std::shared_ptr<const Carte> main(int indice) const;
    void ajouterEnMain(std::shared_ptr<const Carte> carte);
    std::shared_ptr<const Carte> retirerDeMain(int indice);

    int score() const;
    void ajouterPoints(int points);

    std::shared_ptr<const Carte> carteInitiale() const;
    void fixerCarteInitiale(std::shared_ptr<const Carte> carte);
    bool faceInitialeChoisie() const;
    bool faceInitialeRecto() const;
    void choisirFaceInitiale(bool recto);

    void proposerObjectif(std::shared_ptr<const Objectif> objectif);
    const std::vector<std::shared_ptr<const Objectif> >& candidatsObjectif() const;
    std::shared_ptr<const Objectif> objectifSecret() const;
    void choisirObjectifSecret(int indiceCandidat);
    bool objectifChoisi() const;
    int pointsObjectifSecret() const;

private:
    std::string nomJoueur;
    bool ordinateur;
    int niveauIA;
    ZoneDeJeu zoneJeu;
    std::vector<std::shared_ptr<const Carte> > cartesMain;
    std::vector<std::shared_ptr<const Objectif> > objectifsCandidats;
    std::shared_ptr<const Objectif> objectifRetenu;
    std::shared_ptr<const Carte> initiale;
    bool faceChoisie;
    bool rectoInitial;
    int points;
};

} // namespace codex

#endif // CODEX_JOUEUR_H
