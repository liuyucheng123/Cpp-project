#include "noyau/Coordonnee.h"

#include <ostream>

namespace codex {

Coordonnee::Coordonnee() : x(0), y(0)
{
}

Coordonnee::Coordonnee(int px, int py) : x(px), y(py)
{
}

bool Coordonnee::operator==(const Coordonnee& autre) const
{
    return x == autre.x && y == autre.y;
}

bool Coordonnee::operator!=(const Coordonnee& autre) const
{
    return !(*this == autre);
}

bool Coordonnee::operator<(const Coordonnee& autre) const
{
    if (y != autre.y) {
        return y < autre.y;
    }
    return x < autre.x;
}

std::ostream& operator<<(std::ostream& os, const Coordonnee& c)
{
    os << "(" << c.x << "," << c.y << ")";
    return os;
}

} // namespace codex
