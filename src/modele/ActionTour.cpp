#include "modele/ActionTour.h"

namespace codex {

bool estEtalee(SourcePioche source)
{
    return source == SourcePioche::ETALEE_RESSOURCE || source == SourcePioche::ETALEE_DOREE;
}

bool estTasRessource(SourcePioche source)
{
    return source == SourcePioche::ETALEE_RESSOURCE || source == SourcePioche::TAS_RESSOURCE;
}

ActionTour::ActionTour()
    : indiceMain(0), position(), recto(true),
      source(SourcePioche::ETALEE_RESSOURCE), indiceEtalee(0)
{
}

} // namespace codex
