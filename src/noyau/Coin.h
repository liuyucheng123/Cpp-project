#ifndef CODEX_COIN_H
#define CODEX_COIN_H

namespace codex {

// Les quatre coins d'une carte. L'ordre sert aussi d'indice dans les tableaux.
enum Coin
{
    COIN_HAUT_GAUCHE = 0,
    COIN_HAUT_DROIT = 1,
    COIN_BAS_GAUCHE = 2,
    COIN_BAS_DROIT = 3
};

const int NB_COINS = 4;

// Decalage (en cases) du coin par rapport a la case haut-gauche de la carte.
int decalageX(int coin);
int decalageY(int coin);

// Indice du coin designe par un decalage (0 ou 1 dans chaque direction).
int coinDepuisDecalage(int dx, int dy);

} // namespace codex

#endif // CODEX_COIN_H
