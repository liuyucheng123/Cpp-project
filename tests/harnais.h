#ifndef CODEX_HARNAIS_H
#define CODEX_HARNAIS_H

#include <iostream>
#include <string>

// Petit harnais de test : aucune bibliotheque externe, seulement des verifications.
namespace harnais {

inline int& compteurVerifications()
{
    static int valeur = 0;
    return valeur;
}

inline int& compteurEchecs()
{
    static int valeur = 0;
    return valeur;
}

template <class T>
void egal(const T& attendu, const T& obtenu, const std::string& libelle,
          const char* fichier, int ligne)
{
    ++compteurVerifications();
    if (!(attendu == obtenu)) {
        ++compteurEchecs();
        std::cout << "ECHEC " << fichier << ":" << ligne << " " << libelle
                  << " | attendu = " << attendu << " | obtenu = " << obtenu << std::endl;
    }
}

inline void vrai(bool condition, const std::string& libelle, const char* fichier, int ligne)
{
    ++compteurVerifications();
    if (!condition) {
        ++compteurEchecs();
        std::cout << "ECHEC " << fichier << ":" << ligne << " " << libelle << std::endl;
    }
}

} // namespace harnais

#define VERIFIER_EGAL(attendu, obtenu, libelle) \
    harnais::egal((attendu), (obtenu), (libelle), __FILE__, __LINE__)

#define VERIFIER_VRAI(condition, libelle) \
    harnais::vrai((condition), (libelle), __FILE__, __LINE__)

#endif // CODEX_HARNAIS_H
