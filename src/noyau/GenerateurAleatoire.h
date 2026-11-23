#ifndef CODEX_GENERATEUR_ALEATOIRE_H
#define CODEX_GENERATEUR_ALEATOIRE_H

namespace codex {

// Generateur congruentiel lineaire : suffisant pour melanger des cartes,
// et surtout reproductible (meme graine => meme partie), ce qui rend les tests possibles.
class GenerateurAleatoire
{
public:
    explicit GenerateurAleatoire(unsigned int graine = 5489U);

    void reinitialiser(unsigned int graine);

    // Renvoie un entier pseudo-aleatoire (toute la plage unsigned int).
    unsigned int suivant();

    // Renvoie un entier dans [0, borneExclue[.
    unsigned int suivant(unsigned int borneExclue);

private:
    unsigned long long etat;
};

} // namespace codex

#endif // CODEX_GENERATEUR_ALEATOIRE_H
