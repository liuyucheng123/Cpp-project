#ifndef CODEX_MELANGE_H
#define CODEX_MELANGE_H

#include "noyau/GenerateurAleatoire.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace codex {

// Melange de Fisher-Yates : patron de fonction, comme vu dans le chapitre
// "programmation generique". Le generateur est transmis par reference.
template <class T>
void melanger(std::vector<T>& valeurs, GenerateurAleatoire& generateur)
{
    std::size_t i = valeurs.size();
    while (i > 1) {
        std::size_t j = static_cast<std::size_t>(generateur.suivant(static_cast<unsigned int>(i)));
        std::swap(valeurs[i - 1], valeurs[j]);
        --i;
    }
}

} // namespace codex

#endif // CODEX_MELANGE_H
