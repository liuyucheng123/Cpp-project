#include "noyau/ExceptionJeu.h"

namespace codex {

ExceptionJeu::ExceptionJeu(const std::string& message) : std::runtime_error(message)
{
}

PlacementInvalide::PlacementInvalide(const std::string& message) : ExceptionJeu(message)
{
}

RessourcesInsuffisantes::RessourcesInsuffisantes(const std::string& message) : ExceptionJeu(message)
{
}

CarteAbsente::CarteAbsente(const std::string& message) : ExceptionJeu(message)
{
}

SaisieInvalide::SaisieInvalide(const std::string& message) : ExceptionJeu(message)
{
}

PartieTerminee::PartieTerminee(const std::string& message) : ExceptionJeu(message)
{
}

IdentifiantInconnu::IdentifiantInconnu(const std::string& message) : ExceptionJeu(message)
{
}

} // namespace codex
