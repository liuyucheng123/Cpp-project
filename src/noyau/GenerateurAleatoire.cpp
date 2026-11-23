#include "noyau/GenerateurAleatoire.h"

namespace codex {

GenerateurAleatoire::GenerateurAleatoire(unsigned int graine)
    : etat(static_cast<unsigned long long>(graine) + 0x9E3779B97F4A7C15ULL)
{
}

void GenerateurAleatoire::reinitialiser(unsigned int graine)
{
    etat = static_cast<unsigned long long>(graine) + 0x9E3779B97F4A7C15ULL;
}

unsigned int GenerateurAleatoire::suivant()
{
    etat = etat * 6364136223846793005ULL + 1442695040888963407ULL;
    return static_cast<unsigned int>(etat >> 33);
}

unsigned int GenerateurAleatoire::suivant(unsigned int borneExclue)
{
    if (borneExclue == 0U) {
        return 0U;
    }
    return suivant() % borneExclue;
}

} // namespace codex
