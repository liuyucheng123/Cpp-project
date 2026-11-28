#ifndef CODEX_CONFIGURATION_PARTIE_H
#define CODEX_CONFIGURATION_PARTIE_H

#include <string>
#include <vector>

namespace codex {

// Un siege : un nom, et le fait que ce soit un ordinateur ou non.
struct ConfigurationJoueur
{
    std::string nom;
    bool ordinateur;
    int niveau;   // 0 = hasard, 1 = glouton, 2 = objectifs

    ConfigurationJoueur();
    ConfigurationJoueur(const std::string& nomJoueur, bool estOrdinateur, int niveauIA);
};

// Tous les parametres choisis avant de distribuer les cartes.
struct ConfigurationPartie
{
    std::vector<ConfigurationJoueur> sieges;
    unsigned int graine;
    // Un seul siege = mode solo officiel : le joueur affronte Tybor Kwelein.
    int difficulteSolo;   // 0 = facile, 1 = normal, 2 = difficile

    ConfigurationPartie();
    bool valide() const;
    bool modeSolo() const;
};

} // namespace codex

#endif // CODEX_CONFIGURATION_PARTIE_H
