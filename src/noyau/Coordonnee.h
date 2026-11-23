#ifndef CODEX_COORDONNEE_H
#define CODEX_COORDONNEE_H

#include <iosfwd>

namespace codex {

// Coordonnee d'une case du damier interne (une carte occupe 2 x 2 cases).
struct Coordonnee
{
    int x;
    int y;

    Coordonnee();
    Coordonnee(int px, int py);

    bool operator==(const Coordonnee& autre) const;
    bool operator!=(const Coordonnee& autre) const;
    bool operator<(const Coordonnee& autre) const;
};

std::ostream& operator<<(std::ostream& os, const Coordonnee& c);

} // namespace codex

#endif // CODEX_COORDONNEE_H
