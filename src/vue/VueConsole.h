#ifndef CODEX_VUE_CONSOLE_H
#define CODEX_VUE_CONSOLE_H

#include "vue/Vue.h"

#include <ostream>
#include <string>

namespace codex {

class Carte;
class FaceCarte;
class Joueur;
class Partie;
class ZoneDeJeu;

// Affichage console : la trame generale est toujours la meme, seuls les
// details des cases et des cartes changent (patron de methode).
class VueConsole : public Vue
{
public:
    explicit VueConsole(std::ostream& sortie);
    ~VueConsole();

    void afficherTitre();
    void afficherCarteInitiale(const Joueur& joueur);
    void afficherObjectifs(const Partie& partie);
    void afficherTable(const Partie& partie);
    void afficherZone(const Joueur& joueur);
    void afficherMain(const Joueur& joueur);
    void afficherEtalees(const Partie& partie);
    void afficherResultat(const Partie& partie);
    void afficherMessage(const std::string& message);
    void afficherErreur(const std::string& message);

private:
    void dessinerGrille(const ZoneDeJeu& zone);
    void dessinerListeCartes(const ZoneDeJeu& zone);
    void afficherCarte(const Carte& carte, int indice);

    std::ostream& flux;
};

} // namespace codex

#endif // CODEX_VUE_CONSOLE_H
