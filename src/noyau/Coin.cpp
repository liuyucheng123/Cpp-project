#include "noyau/Coin.h"

namespace codex {

int decalageX(int coin)
{
    if (coin == COIN_HAUT_DROIT || coin == COIN_BAS_DROIT) {
        return 1;
    }
    return 0;
}

int decalageY(int coin)
{
    if (coin == COIN_BAS_GAUCHE || coin == COIN_BAS_DROIT) {
        return 1;
    }
    return 0;
}

int coinDepuisDecalage(int dx, int dy)
{
    if (dy == 0) {
        return (dx == 0) ? COIN_HAUT_GAUCHE : COIN_HAUT_DROIT;
    }
    return (dx == 0) ? COIN_BAS_GAUCHE : COIN_BAS_DROIT;
}

} // namespace codex
