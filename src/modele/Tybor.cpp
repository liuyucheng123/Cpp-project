#include "modele/Tybor.h"

#include "modele/Carte.h"
#include "modele/Joueur.h"
#include "modele/Objectif.h"
#include "modele/Pioche.h"

#include <memory>
#include <sstream>

namespace codex {
namespace {

const Ressource ROYAUMES[4] = { Ressource::PLANTE, Ressource::ANIMAL, Ressource::CHAMPIGNON,
                                Ressource::INSECTE };

// Une carte Doree vaut 5 points de facon permanente : Tybor la garde comme
// ressource au lieu de la defausser, pour ne pas gaspiller une carte utile.
const int VALEUR_CINQ_POINTS = 5;

} // namespace

Tybor::Tybor(int difficulte) : ressources(), niveauDifficulte(difficulte)
{
    if (niveauDifficulte < 0) {
        niveauDifficulte = 0;
    }
    if (niveauDifficulte > 2) {
        niveauDifficulte = 2;
    }
    const int nombreRoyaumes = (niveauDifficulte == 0) ? 2 : 4;
    for (int i = 0; i < nombreRoyaumes; ++i) {
        ressources[ROYAUMES[i]] = 1;
    }
}

int Tybor::difficulte() const
{
    return niveauDifficulte;
}

int Tybor::ressourcesDe(Ressource royaume) const
{
    std::map<Ressource, int>::const_iterator it = ressources.find(royaume);
    if (it == ressources.end()) {
        return 0;
    }
    return it->second;
}

int Tybor::totalRessources() const
{
    int total = 0;
    std::map<Ressource, int>::const_iterator it = ressources.begin();
    while (it != ressources.end()) {
        total += it->second;
        ++it;
    }
    return total;
}

std::string Tybor::description() const
{
    std::ostringstream flux;
    flux << "Tybor Kwelein";
    bool premier = true;
    std::map<Ressource, int>::const_iterator it = ressources.begin();
    while (it != ressources.end()) {
        flux << (premier ? " : " : ", ") << nomRessource(it->first) << " " << it->second;
        premier = false;
        ++it;
    }
    if (premier) {
        flux << " : aucune ressource";
    }
    return flux.str();
}

bool Tybor::peutPayer(const Carte& carte) const
{
    return carte.exigencesSatisfaites(ressources);
}

void Tybor::gagnerRessource(Ressource royaume)
{
    if (!estRoyaume(royaume)) {
        return;
    }
    ressources[royaume] = ressourcesDe(royaume) + 1;
}

std::string Tybor::traiterColonne(Pioche& pioche, int indiceRessource, int indiceDoree,
                                  const Objectif& objectifA, const Objectif& objectifB,
                                  Joueur& joueurTybor)
{
    std::ostringstream journal;
    std::shared_ptr<const Carte> doree = pioche.etalee(indiceDoree);

    if (doree) {
        // Etape 1 : il a les ressources exigees -> il defausse la carte et
        // marque le nombre imprime (sans les multiplicateurs).
        if (peutPayer(*doree)) {
            const int points = doree->pointsImprimes();
            journal << "defausse " << doree->nom() << " (+" << points << " pts)";
            joueurTybor.ajouterPoints(points);
            pioche.prendreEtalee(indiceDoree);
            pioche.completerEtalees();
            return journal.str();
        }
        // Etape 2 : carte a 5 points ou carte qui sert un objectif commun ->
        // il la retourne et l'ajoute a ses ressources.
        if (doree->pointsImprimes() == VALEUR_CINQ_POINTS || objectifA.correspondALaCarte(*doree)
            || objectifB.correspondALaCarte(*doree)) {
            const Ressource royaume = doree->royaume();
            journal << "garde " << doree->nom() << " en ressource " << nomRessource(royaume);
            gagnerRessource(royaume);
            pioche.prendreEtalee(indiceDoree);
            pioche.completerEtalees();
            return journal.str();
        }
    }

    // Etape 3 : il prend la carte Ressource de la colonne.
    std::shared_ptr<const Carte> ressource = pioche.etalee(indiceRessource);
    if (ressource) {
        const Ressource royaume = ressource->royaume();
        journal << "prend " << ressource->nom() << " en ressource " << nomRessource(royaume);
        gagnerRessource(royaume);
        pioche.prendreEtalee(indiceRessource);
        pioche.completerEtalees();
        return journal.str();
    }
    journal << "ne prend rien";
    return journal.str();
}

std::string Tybor::jouerTour(Pioche& pioche, const Objectif& objectifA, const Objectif& objectifB,
                             Joueur& joueurTybor)
{
    // Colonne de gauche : la premiere carte Ressource et la premiere Doree.
    // Colonne de droite : la seconde de chaque paquet.
    const std::string colonneGauche = traiterColonne(pioche, 0, 2, objectifA, objectifB, joueurTybor);
    const std::string colonneDroite = traiterColonne(pioche, 1, 3, objectifA, objectifB, joueurTybor);

    std::ostringstream flux;
    flux << "Tybor joue : " << colonneGauche << " | " << colonneDroite;
    return flux.str();
}

} // namespace codex
