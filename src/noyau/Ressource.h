#ifndef CODEX_RESSOURCE_H
#define CODEX_RESSOURCE_H

#include <iosfwd>
#include <string>

namespace codex {

// Les quatre royaumes du jeu, les trois objets, plus les deux etats d'un coin.
// VIDE  : coin visible mais sans symbole (il peut etre recouvert).
// CACHE : coin absent (il ne peut jamais etre recouvert).
enum class Ressource
{
    PLANTE,
    ANIMAL,
    CHAMPIGNON,
    INSECTE,
    PLUME,
    ENCRIER,
    MANUSCRIT,
    VIDE,
    CACHE
};

bool estRoyaume(Ressource r);
bool estObjet(Ressource r);
bool coinUtilisable(Ressource r);

char symbole(Ressource r);
std::string nomRessource(Ressource r);

std::ostream& operator<<(std::ostream& os, Ressource r);

} // namespace codex

#endif // CODEX_RESSOURCE_H
