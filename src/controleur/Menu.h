#ifndef CODEX_MENU_H
#define CODEX_MENU_H

#include "modele/ConfigurationPartie.h"

#include <string>

namespace codex {

class Saisie;
class Vue;

// Pose les questions de mise en place : nombre de joueurs, noms, humains/ordinateurs.
class Menu
{
public:
    Menu(Vue& vue, Saisie& saisie);

    ConfigurationPartie demanderConfiguration(unsigned int graineParDefaut);

    int demanderEntier(const std::string& question, int minimum, int maximum);
    std::string demanderTexte(const std::string& question, const std::string& valeurParDefaut);

private:
    Vue& vue;
    Saisie& saisie;
};

} // namespace codex

#endif // CODEX_MENU_H
