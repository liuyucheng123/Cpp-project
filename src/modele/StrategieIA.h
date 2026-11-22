#ifndef CODEX_STRATEGIE_IA_H
#define CODEX_STRATEGIE_IA_H

#include "modele/ActionTour.h"
#include "noyau/GenerateurAleatoire.h"

#include <memory>
#include <string>

namespace codex {

class Partie;

// Patron Strategie : chaque niveau de difficulte est un algorithme interchangeable.
// Ajouter une nouvelle IA revient a ajouter une sous-classe, sans toucher au moteur.
class StrategieIA
{
public:
    virtual ~StrategieIA();

    virtual ActionTour choisirAction(const Partie& partie, int indiceJoueur) = 0;
    virtual std::string nom() const = 0;
};

class IAHasard : public StrategieIA
{
public:
    explicit IAHasard(GenerateurAleatoire& generateur);

    ActionTour choisirAction(const Partie& partie, int indiceJoueur);
    std::string nom() const;

private:
    GenerateurAleatoire* generateur;
};

class IAGloutonne : public StrategieIA
{
public:
    explicit IAGloutonne(GenerateurAleatoire& generateur);

    ActionTour choisirAction(const Partie& partie, int indiceJoueur);
    std::string nom() const;

private:
    GenerateurAleatoire* generateur;
};

class IAObjectifs : public StrategieIA
{
public:
    explicit IAObjectifs(GenerateurAleatoire& generateur);

    ActionTour choisirAction(const Partie& partie, int indiceJoueur);
    std::string nom() const;

private:
    GenerateurAleatoire* generateur;
};

// Fabrique simple : le niveau choisi dans le menu donne l'algorithme.
std::shared_ptr<StrategieIA> creerStrategie(int niveau, GenerateurAleatoire& generateur);

} // namespace codex

#endif // CODEX_STRATEGIE_IA_H
