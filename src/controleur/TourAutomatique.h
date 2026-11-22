#ifndef CODEX_TOUR_AUTOMATIQUE_H
#define CODEX_TOUR_AUTOMATIQUE_H

namespace codex {

class Partie;
class StrategieIA;

// Joue un tour complet pour un joueur ordinateur : choix de la carte,
// placement (avec repli sur les placements legaux) puis pioche.
// Renvoie vrai si une carte a ete posee.
bool jouerTourAutomatique(Partie& partie, int indiceJoueur, StrategieIA& strategie);

} // namespace codex

#endif // CODEX_TOUR_AUTOMATIQUE_H
