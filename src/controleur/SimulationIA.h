#ifndef CODEX_SIMULATION_IA_H
#define CODEX_SIMULATION_IA_H

#include "modele/ConfigurationPartie.h"

#include <vector>

namespace codex {

// Resultat condense d'une partie entierement automatique (servant aux tests
// et au mode "auto" en ligne de commande).
struct ResumeSimulation
{
    bool terminee;
    int gagnant;
    std::vector<int> totaux;
    std::vector<int> pointsCartes;
    std::vector<int> pointsObjectifs;
    int toursJoues;

    ResumeSimulation();
};

ResumeSimulation jouerPartieAutomatique(const ConfigurationPartie& configuration);

} // namespace codex

#endif // CODEX_SIMULATION_IA_H
