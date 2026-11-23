#include "controleur/Saisie.h"

#include <iostream>

namespace codex {

Saisie::~Saisie()
{
}

SaisieClavier::SaisieClavier(std::istream& entreeFlux, std::ostream& sortieFlux)
    : entree(entreeFlux), sortie(sortieFlux)
{
}

std::string SaisieClavier::lireLigne()
{
    std::string ligne;
    sortie << "> ";
    sortie.flush();
    if (!std::getline(entree, ligne)) {
        return "";
    }
    return ligne;
}

bool SaisieClavier::disponible() const
{
    return !entree.eof();
}

AdaptateurFlux::AdaptateurFlux(std::istream& fluxEntree)
    : flux(fluxEntree), termine(false)
{
}

std::string AdaptateurFlux::lireLigne()
{
    std::string ligne;
    if (!std::getline(flux, ligne)) {
        termine = true;
        return "";
    }
    return ligne;
}

bool AdaptateurFlux::disponible() const
{
    return !termine;
}

} // namespace codex
