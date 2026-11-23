#ifndef CODEX_ZONE_DE_JEU_H
#define CODEX_ZONE_DE_JEU_H

#include "noyau/Coordonnee.h"
#include "noyau/Ressource.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace codex {

class Carte;
class IterateurCartes;

// Une carte posee : sa case haut-gauche, sa face visible et le nombre de coins
// d'autres cartes qu'elle a recouverts en arrivant.
struct CartePlacee
{
    Coordonnee origine;
    std::shared_ptr<const Carte> carte;
    bool recto;
    int coinsCouverts;

    CartePlacee();
    CartePlacee(Coordonnee origineCarte, std::shared_ptr<const Carte> cartePosee,
                bool faceRecto, int nombreCoinsCouverts);
};

// La zone de jeu d'un joueur.
// Modele : une carte occupe un bloc de 2 x 2 cases et ses quatre coins sont
// les quatre cases de ce bloc. Deux cartes placees en diagonale partagent
// exactement une case : c'est ainsi qu'une carte recouvre un coin d'une autre.
class ZoneDeJeu
{
public:
    ZoneDeJeu();

    bool estVide() const;
    int nombreCartes() const;
    void vider();

    static Coordonnee caseDuCoin(Coordonnee origine, int coin);

    bool caseLibre(Coordonnee c) const;
    const CartePlacee* carteSur(Coordonnee c) const;

    // Verifie la regle de pose. Remplit "cause" avec l'explication si le placement est refuse.
    bool placementValide(Coordonnee origine, const Carte& carte, bool recto, std::string& cause) const;

    // Nombre de coins d'autres cartes qui seraient recouverts par une carte posee ici.
    int compterCoinsCouverts(Coordonnee origine) const;

    void poser(Coordonnee origine, std::shared_ptr<const Carte> carte, bool recto, int coinsCouverts);
    void poserCarteInitiale(std::shared_ptr<const Carte> carte, bool recto);

    std::map<Ressource, int> ressourcesVisibles() const;
    std::map<Ressource, int> objetsVisibles() const;
    std::map<Ressource, int> symbolesVisibles() const;
    int nombreVisibles(Ressource r) const;

    // Toutes les cases ou une nouvelle carte pourrait etre posee
    // (cases en diagonale d'une carte deja posee, sans doublon).
    std::vector<Coordonnee> positionsCandidates() const;

    const std::vector<CartePlacee>& cartesPlacees() const;
    IterateurCartes iterateur() const;

    // Boite englobante en cases (bornes incluses).
    void bornes(int& xMin, int& yMin, int& xMax, int& yMax) const;

private:
    std::map<Ressource, int> compterSymbolesVisibles() const;

    std::map<Coordonnee, std::size_t> grille;  // case -> indice dans "liste"
    std::vector<CartePlacee> liste;
};

} // namespace codex

#endif // CODEX_ZONE_DE_JEU_H
