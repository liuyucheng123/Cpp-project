#include "modele/Objectif.h"

#include "modele/Carte.h"
#include "modele/ZoneDeJeu.h"

#include <map>
#include <set>
#include <sstream>

namespace codex {
namespace {

// Compte les motifs formes de trois cases, en prenant les cartes dans un ordre
// fixe (de haut en bas puis de gauche a droite) et sans jamais reutiliser une carte.
int compterMotifs(const std::set<Coordonnee>& cases, bool diagonale, int& motifs)
{
    std::set<Coordonnee> utilisees;
    std::set<Coordonnee>::const_iterator it = cases.begin();
    int total = 0;
    while (it != cases.end()) {
        if (utilisees.find(*it) != utilisees.end()) {
            ++it;
            continue;
        }
        bool trouve = false;
        const int dx[2] = { 1, 1 };
        const int dyDiagonale[2] = { 1, -1 };
        if (diagonale) {
            for (int orientation = 0; orientation < 2 && !trouve; ++orientation) {
                Coordonnee c2(it->x + dx[orientation], it->y + dyDiagonale[orientation]);
                Coordonnee c3(it->x + 2 * dx[orientation], it->y + 2 * dyDiagonale[orientation]);
                if (cases.find(c2) != cases.end() && cases.find(c3) != cases.end()
                    && utilisees.find(c2) == utilisees.end() && utilisees.find(c3) == utilisees.end()) {
                    utilisees.insert(*it);
                    utilisees.insert(c2);
                    utilisees.insert(c3);
                    ++total;
                    trouve = true;
                }
            }
        } else {
            // Les cartes ne se touchent que par les coins : un motif en L
            // est fait de deux pas en diagonale perpendiculaires.
            static const int decalages[4][4] = {
                { 1, 1, 2, 0 },
                { 1, 1, 0, 2 },
                { 1, -1, 2, 0 },
                { 1, -1, 0, -2 }
            };
            for (int orientation = 0; orientation < 4 && !trouve; ++orientation) {
                Coordonnee c2(it->x + decalages[orientation][0], it->y + decalages[orientation][1]);
                Coordonnee c3(it->x + decalages[orientation][2], it->y + decalages[orientation][3]);
                if (cases.find(c2) != cases.end() && cases.find(c3) != cases.end()
                    && utilisees.find(c2) == utilisees.end() && utilisees.find(c3) == utilisees.end()) {
                    utilisees.insert(*it);
                    utilisees.insert(c2);
                    utilisees.insert(c3);
                    ++total;
                    trouve = true;
                }
            }
        }
        ++it;
    }
    motifs = total;
    return total;
}

std::set<Coordonnee> casesDuRoyaume(const ZoneDeJeu& zone, Ressource royaume)
{
    std::set<Coordonnee> cases;
    const std::vector<CartePlacee>& cartes = zone.cartesPlacees();
    for (std::size_t i = 0; i < cartes.size(); ++i) {
        if (cartes[i].carte->royaume() == royaume) {
            cases.insert(cartes[i].origine);
        }
    }
    return cases;
}

} // namespace

Objectif::Objectif(int identifiant) : identifiant(identifiant)
{
}

Objectif::~Objectif()
{
}

int Objectif::id() const
{
    return identifiant;
}

bool Objectif::correspondALaCarte(const Carte& carte) const
{
    (void)carte;
    return false;
}

ObjectifCompose::ObjectifCompose(int identifiant) : Objectif(identifiant)
{
}

void ObjectifCompose::ajouter(std::shared_ptr<const Objectif> objectif)
{
    if (objectif) {
        enfants.push_back(objectif);
    }
}

bool ObjectifCompose::correspondALaCarte(const Carte& carte) const
{
    for (std::size_t i = 0; i < enfants.size(); ++i) {
        if (enfants[i]->correspondALaCarte(carte)) {
            return true;
        }
    }
    return false;
}

int ObjectifCompose::evaluer(const ZoneDeJeu& zone) const
{
    int total = 0;
    for (std::size_t i = 0; i < enfants.size(); ++i) {
        total += enfants[i]->evaluer(zone);
    }
    return total;
}

std::string ObjectifCompose::description() const
{
    std::ostringstream flux;
    flux << "combinaison de " << enfants.size() << " objectifs";
    return flux.str();
}

ObjectifMotif::ObjectifMotif(int identifiant, Ressource royaumeObjectif, int pointsObjectif, bool motifDiagonal)
    : Objectif(identifiant), royaume(royaumeObjectif), points(pointsObjectif), diagonale(motifDiagonal)
{
}

int ObjectifMotif::evaluer(const ZoneDeJeu& zone) const
{
    int motifs = 0;
    compterMotifs(casesDuRoyaume(zone, royaume), diagonale, motifs);
    return motifs * points;
}

std::string ObjectifMotif::description() const
{
    std::ostringstream flux;
    flux << points << " pts par " << (diagonale ? "diagonale" : "L") << " de "
         << nomRessource(royaume);
    return flux.str();
}

bool ObjectifMotif::correspondALaCarte(const Carte& carte) const
{
    return carte.royaume() == royaume;
}

ObjectifSerieRessource::ObjectifSerieRessource(int identifiant, Ressource royaumeObjectif, int pointsObjectif)
    : Objectif(identifiant), royaume(royaumeObjectif), points(pointsObjectif)
{
}

int ObjectifSerieRessource::evaluer(const ZoneDeJeu& zone) const
{
    int symboles = zone.nombreVisibles(royaume);
    return (symboles / 3) * points;
}

std::string ObjectifSerieRessource::description() const
{
    std::ostringstream flux;
    flux << points << " pts par 3 symboles " << nomRessource(royaume);
    return flux.str();
}

bool ObjectifSerieRessource::correspondALaCarte(const Carte& carte) const
{
    return carte.royaume() == royaume;
}

ObjectifPairesObjet::ObjectifPairesObjet(int identifiant, Ressource objetObjectif, int pointsObjectif)
    : Objectif(identifiant), objet(objetObjectif), points(pointsObjectif)
{
}

int ObjectifPairesObjet::evaluer(const ZoneDeJeu& zone) const
{
    int symboles = zone.nombreVisibles(objet);
    return (symboles / 2) * points;
}

std::string ObjectifPairesObjet::description() const
{
    std::ostringstream flux;
    flux << points << " pts par paire de " << nomRessource(objet);
    return flux.str();
}

bool ObjectifPairesObjet::correspondALaCarte(const Carte& carte) const
{
    return carte.porteObjet(objet);
}

ObjectifTripletObjets::ObjectifTripletObjets(int identifiant, int pointsObjectif)
    : Objectif(identifiant), points(pointsObjectif)
{
}

int ObjectifTripletObjets::evaluer(const ZoneDeJeu& zone) const
{
    int plumes = zone.nombreVisibles(Ressource::PLUME);
    int encriers = zone.nombreVisibles(Ressource::ENCRIER);
    int manuscrits = zone.nombreVisibles(Ressource::MANUSCRIT);

    int minimum = plumes;
    if (encriers < minimum) {
        minimum = encriers;
    }
    if (manuscrits < minimum) {
        minimum = manuscrits;
    }
    return minimum * points;
}

std::string ObjectifTripletObjets::description() const
{
    std::ostringstream flux;
    flux << points << " pts par triplet plume + encrier + manuscrit";
    return flux.str();
}

bool ObjectifTripletObjets::correspondALaCarte(const Carte& carte) const
{
    return carte.porteObjet(Ressource::PLUME) || carte.porteObjet(Ressource::ENCRIER)
           || carte.porteObjet(Ressource::MANUSCRIT);
}

} // namespace codex
