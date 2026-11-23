#include "modele/ConfigurationPartie.h"

namespace codex {

ConfigurationJoueur::ConfigurationJoueur()
    : nom("Joueur"), ordinateur(false), niveau(0)
{
}

ConfigurationJoueur::ConfigurationJoueur(const std::string& nomJoueur, bool estOrdinateur, int niveauIA)
    : nom(nomJoueur), ordinateur(estOrdinateur), niveau(niveauIA)
{
}

ConfigurationPartie::ConfigurationPartie() : graine(1U)
{
}

bool ConfigurationPartie::valide() const
{
    return sieges.size() >= 1U && sieges.size() <= 4U;
}

} // namespace codex
