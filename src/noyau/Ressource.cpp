#include "noyau/Ressource.h"

#include <ostream>

namespace codex {

bool estRoyaume(Ressource r)
{
    return r == Ressource::PLANTE || r == Ressource::ANIMAL
        || r == Ressource::CHAMPIGNON || r == Ressource::INSECTE;
}

bool estObjet(Ressource r)
{
    return r == Ressource::PLUME || r == Ressource::ENCRIER || r == Ressource::MANUSCRIT;
}

bool coinUtilisable(Ressource r)
{
    return r != Ressource::CACHE;
}

char symbole(Ressource r)
{
    switch (r) {
    case Ressource::PLANTE:     return 'P';
    case Ressource::ANIMAL:     return 'A';
    case Ressource::CHAMPIGNON: return 'C';
    case Ressource::INSECTE:    return 'I';
    case Ressource::PLUME:      return 'f';
    case Ressource::ENCRIER:    return 'j';
    case Ressource::MANUSCRIT:  return 's';
    case Ressource::VIDE:       return '.';
    case Ressource::CACHE:      return '#';
    }
    return '?';
}

std::string nomRessource(Ressource r)
{
    switch (r) {
    case Ressource::PLANTE:     return "plante";
    case Ressource::ANIMAL:     return "animal";
    case Ressource::CHAMPIGNON: return "champignon";
    case Ressource::INSECTE:    return "insecte";
    case Ressource::PLUME:      return "plume";
    case Ressource::ENCRIER:    return "encrier";
    case Ressource::MANUSCRIT:  return "manuscrit";
    case Ressource::VIDE:       return "vide";
    case Ressource::CACHE:      return "cache";
    }
    return "inconnu";
}

std::ostream& operator<<(std::ostream& os, Ressource r)
{
    os << nomRessource(r);
    return os;
}

} // namespace codex
