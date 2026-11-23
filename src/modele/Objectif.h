#ifndef CODEX_OBJECTIF_H
#define CODEX_OBJECTIF_H

#include "noyau/Ressource.h"

#include <memory>
#include <string>
#include <vector>

namespace codex {

class ZoneDeJeu;

// Objectif de fin de partie. Toutes les conditions partagent la meme
// interface : evaluer() renvoie les points rapportes par la zone du joueur.
class Objectif
{
public:
    explicit Objectif(int identifiant);
    virtual ~Objectif();

    int id() const;
    virtual int evaluer(const ZoneDeJeu& zone) const = 0;
    virtual std::string description() const = 0;

private:
    int identifiant;
};

// Patron Composite : un objectif peut etre la somme de plusieurs conditions.
// Le client traite une condition simple et un groupe de conditions de la meme facon.
class ObjectifCompose : public Objectif
{
public:
    explicit ObjectifCompose(int identifiant);

    void ajouter(std::shared_ptr<const Objectif> objectif);
    int evaluer(const ZoneDeJeu& zone) const;
    std::string description() const;

private:
    std::vector<std::shared_ptr<const Objectif> > enfants;
};

// Motif geometrique : trois cartes du meme royaume en diagonale ou en L.
class ObjectifMotif : public Objectif
{
public:
    ObjectifMotif(int identifiant, Ressource royaume, int points, bool diagonale);

    int evaluer(const ZoneDeJeu& zone) const;
    std::string description() const;

private:
    Ressource royaume;
    int points;
    bool diagonale;
};

// Trois symboles du meme royaume rapportent des points.
class ObjectifSerieRessource : public Objectif
{
public:
    ObjectifSerieRessource(int identifiant, Ressource royaume, int points);

    int evaluer(const ZoneDeJeu& zone) const;
    std::string description() const;

private:
    Ressource royaume;
    int points;
};

// Deux symboles identiques (plume, encrier, manuscrit) rapportent des points.
class ObjectifPairesObjet : public Objectif
{
public:
    ObjectifPairesObjet(int identifiant, Ressource objet, int points);

    int evaluer(const ZoneDeJeu& zone) const;
    std::string description() const;

private:
    Ressource objet;
    int points;
};

// Trois symboles differents (un de chaque) rapportent des points.
class ObjectifTripletObjets : public Objectif
{
public:
    ObjectifTripletObjets(int identifiant, int points);

    int evaluer(const ZoneDeJeu& zone) const;
    std::string description() const;

private:
    int points;
};

} // namespace codex

#endif // CODEX_OBJECTIF_H
