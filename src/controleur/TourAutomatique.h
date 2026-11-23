#ifndef CODEX_TOUR_AUTOMATIQUE_H
#define CODEX_TOUR_AUTOMATIQUE_H

#include <string>

namespace codex {

class Partie;
class StrategieIA;

// Joue un tour complet pour un joueur ordinateur : choix de la carte,
// placement (avec repli sur les placements legaux) puis pioche.
// Renvoie vrai si une carte a ete posee.
// Si "resume" n'est pas nul, il recoit une ligne decrivant l'action jouee
// (carte, position, face, points gagnes) pour que la vue puisse l'afficher :
// le controleur reste seul a parler a la vue, le modele n'imprime rien.
bool jouerTourAutomatique(Partie& partie, int indiceJoueur, StrategieIA& strategie,
                          std::string* resume = nullptr);

} // namespace codex

#endif // CODEX_TOUR_AUTOMATIQUE_H
