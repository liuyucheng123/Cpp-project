#ifndef CODEX_VUE_H
#define CODEX_VUE_H

#include <string>

namespace codex {

class Partie;
class Joueur;

// Interface d'affichage. Le modele ne depend jamais de cette classe :
// on peut donc remplacer la console par une autre vue sans le modifier.
class Vue
{
public:
    virtual ~Vue();

    virtual void afficherTitre() = 0;
    virtual void afficherCarteInitiale(const Joueur& joueur) = 0;
    virtual void afficherObjectifs(const Partie& partie) = 0;
    virtual void afficherTable(const Partie& partie) = 0;
    virtual void afficherZone(const Joueur& joueur) = 0;
    virtual void afficherMain(const Joueur& joueur) = 0;
    virtual void afficherEtalees(const Partie& partie) = 0;
    virtual void afficherResultat(const Partie& partie) = 0;
    virtual void afficherMessage(const std::string& message) = 0;
    virtual void afficherErreur(const std::string& message) = 0;
};

} // namespace codex

#endif // CODEX_VUE_H
