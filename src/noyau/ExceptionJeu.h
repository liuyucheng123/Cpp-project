#ifndef CODEX_EXCEPTION_JEU_H
#define CODEX_EXCEPTION_JEU_H

#include <stdexcept>
#include <string>

namespace codex {

// Racine de toutes les erreurs "metier" : le controleur peut les attraper
// en un seul point et afficher un message comprehensible.
class ExceptionJeu : public std::runtime_error
{
public:
    explicit ExceptionJeu(const std::string& message);
};

class PlacementInvalide : public ExceptionJeu
{
public:
    explicit PlacementInvalide(const std::string& message);
};

class RessourcesInsuffisantes : public ExceptionJeu
{
public:
    explicit RessourcesInsuffisantes(const std::string& message);
};

class CarteAbsente : public ExceptionJeu
{
public:
    explicit CarteAbsente(const std::string& message);
};

class SaisieInvalide : public ExceptionJeu
{
public:
    explicit SaisieInvalide(const std::string& message);
};

class PartieTerminee : public ExceptionJeu
{
public:
    explicit PartieTerminee(const std::string& message);
};

class IdentifiantInconnu : public ExceptionJeu
{
public:
    explicit IdentifiantInconnu(const std::string& message);
};

} // namespace codex

#endif // CODEX_EXCEPTION_JEU_H
