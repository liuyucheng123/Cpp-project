#ifndef CODEX_SAISIE_H
#define CODEX_SAISIE_H

#include <istream>
#include <string>

namespace codex {

// Interface de lecture des reponses du joueur.
class Saisie
{
public:
    virtual ~Saisie();

    virtual std::string lireLigne() = 0;
    virtual bool disponible() const = 0;
};

// Lecture au clavier, avec affichage de l'invite.
class SaisieClavier : public Saisie
{
public:
    SaisieClavier(std::istream& entree, std::ostream& sortie);

    std::string lireLigne();
    bool disponible() const;

private:
    std::istream& entree;
    std::ostream& sortie;
};

// Patron Adaptateur : adapte n'importe quel flux d'entree a l'interface Saisie.
// C'est ce qui permet de rejouer une partie scriptee dans les tests.
class AdaptateurFlux : public Saisie
{
public:
    explicit AdaptateurFlux(std::istream& flux);

    std::string lireLigne();
    bool disponible() const;

private:
    std::istream& flux;
    bool termine;
};

} // namespace codex

#endif // CODEX_SAISIE_H
