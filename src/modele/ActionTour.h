#ifndef CODEX_ACTION_TOUR_H
#define CODEX_ACTION_TOUR_H

#include "noyau/Coordonnee.h"

namespace codex {

// D'ou vient la carte que l'on pioche.
enum class SourcePioche
{
    ETALEE_RESSOURCE,
    ETALEE_DOREE,
    TAS_RESSOURCE,
    TAS_DOREE
};

bool estEtalee(SourcePioche source);
bool estTasRessource(SourcePioche source);

// Tout ce qu'un joueur (humain ou ordinateur) doit decider pendant son tour :
// la carte a poser, ou la poser, dans quel sens, puis ou piocher.
struct ActionTour
{
    int indiceMain;
    Coordonnee position;
    bool recto;
    SourcePioche source;
    int indiceEtalee;

    ActionTour();
};

} // namespace codex

#endif // CODEX_ACTION_TOUR_H
