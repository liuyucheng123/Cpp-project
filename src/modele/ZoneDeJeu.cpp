#include "modele/ZoneDeJeu.h"

#include "modele/Carte.h"
#include "modele/FaceCarte.h"
#include "modele/IterateurCartes.h"
#include "noyau/Coin.h"
#include "noyau/ExceptionJeu.h"

#include <set>
#include <sstream>

namespace codex {

CartePlacee::CartePlacee()
    : origine(), carte(), recto(true), coinsCouverts(0)
{
}

CartePlacee::CartePlacee(Coordonnee origineCarte, std::shared_ptr<const Carte> cartePosee,
                         bool faceRecto, int nombreCoinsCouverts)
    : origine(origineCarte), carte(cartePosee), recto(faceRecto), coinsCouverts(nombreCoinsCouverts)
{
}

ZoneDeJeu::ZoneDeJeu()
{
}

bool ZoneDeJeu::estVide() const
{
    return liste.empty();
}

int ZoneDeJeu::nombreCartes() const
{
    return static_cast<int>(liste.size());
}

void ZoneDeJeu::vider()
{
    grille.clear();
    liste.clear();
}

Coordonnee ZoneDeJeu::caseDuCoin(Coordonnee origine, int coin)
{
    return Coordonnee(origine.x + decalageX(coin), origine.y + decalageY(coin));
}

bool ZoneDeJeu::caseLibre(Coordonnee c) const
{
    return grille.find(c) == grille.end();
}

const CartePlacee* ZoneDeJeu::carteSur(Coordonnee c) const
{
    std::map<Coordonnee, std::size_t>::const_iterator it = grille.find(c);
    if (it == grille.end()) {
        return 0;
    }
    return &liste[it->second];
}

int ZoneDeJeu::compterCoinsCouverts(Coordonnee origine) const
{
    int total = 0;
    for (int coin = 0; coin < NB_COINS; ++coin) {
        Coordonnee c = caseDuCoin(origine, coin);
        if (!caseLibre(c)) {
            ++total;
        }
    }
    return total;
}

bool ZoneDeJeu::placementValide(Coordonnee origine, const Carte& carte, bool recto,
                                std::string& cause) const
{
    (void)carte;
    (void)recto;

    std::ostringstream explication;
    int representants = 0;
    std::map<std::size_t, int> coinsParCarte;

    for (int coin = 0; coin < NB_COINS; ++coin) {
        Coordonnee c = caseDuCoin(origine, coin);
        if (caseLibre(c)) {
            continue;
        }
        ++representants;

        std::map<Coordonnee, std::size_t>::const_iterator it = grille.find(c);
        std::size_t indice = it->second;
        const CartePlacee& autre = liste[indice];

        coinsParCarte[indice] = coinsParCarte[indice] + 1;

        int coinTouche = coinDepuisDecalage(c.x - autre.origine.x, c.y - autre.origine.y);
        Ressource symbole = autre.carte->face(autre.recto).coin(coinTouche);
        if (!coinUtilisable(symbole)) {
            cause = "le coin vise par la carte " + autre.carte->nom() + " est cache";
            return false;
        }
    }

    if (representants == 0) {
        cause = "la carte doit recouvrir au moins un coin visible d'une carte deja posee";
        return false;
    }

    std::map<std::size_t, int>::const_iterator it = coinsParCarte.begin();
    while (it != coinsParCarte.end()) {
        if (it->second > 1) {
            cause = "la carte recouvrirait deux coins de la meme carte";
            return false;
        }
        ++it;
    }

    return true;
}

void ZoneDeJeu::poser(Coordonnee origine, std::shared_ptr<const Carte> carte, bool recto, int coinsCouverts)
{
    if (!carte) {
        throw PlacementInvalide("aucune carte fournie");
    }
    std::size_t indice = liste.size();
    liste.push_back(CartePlacee(origine, carte, recto, coinsCouverts));
    for (int coin = 0; coin < NB_COINS; ++coin) {
        grille[caseDuCoin(origine, coin)] = indice;
    }
}

void ZoneDeJeu::poserCarteInitiale(std::shared_ptr<const Carte> carte, bool recto)
{
    poser(Coordonnee(0, 0), carte, recto, 0);
}

const std::vector<CartePlacee>& ZoneDeJeu::cartesPlacees() const
{
    return liste;
}

IterateurCartes ZoneDeJeu::iterateur() const
{
    return IterateurCartes(liste);
}

std::map<Ressource, int> ZoneDeJeu::compterSymbolesVisibles() const
{
    std::map<Ressource, int> comptes;

    for (std::size_t i = 0; i < liste.size(); ++i) {
        const CartePlacee& posee = liste[i];
        const FaceCarte& face = posee.carte->face(posee.recto);
        for (int coin = 0; coin < NB_COINS; ++coin) {
            Coordonnee c = caseDuCoin(posee.origine, coin);
            std::map<Coordonnee, std::size_t>::const_iterator it = grille.find(c);
            if (it == grille.end() || it->second != i) {
                continue;  // coin recouvert par une autre carte
            }
            Ressource symbole = face.coin(coin);
            if (symbole == Ressource::VIDE || symbole == Ressource::CACHE) {
                continue;
            }
            comptes[symbole] = comptes[symbole] + 1;
        }
        for (int k = 0; k < face.nombreCentres(); ++k) {
            Ressource symbole = face.centre(k);
            comptes[symbole] = comptes[symbole] + 1;
        }
    }
    return comptes;
}

std::map<Ressource, int> ZoneDeJeu::ressourcesVisibles() const
{
    std::map<Ressource, int> comptes = compterSymbolesVisibles();
    std::map<Ressource, int> resultat;
    std::map<Ressource, int>::const_iterator it = comptes.begin();
    while (it != comptes.end()) {
        if (estRoyaume(it->first)) {
            resultat[it->first] = it->second;
        }
        ++it;
    }
    return resultat;
}

std::map<Ressource, int> ZoneDeJeu::objetsVisibles() const
{
    std::map<Ressource, int> comptes = compterSymbolesVisibles();
    std::map<Ressource, int> resultat;
    std::map<Ressource, int>::const_iterator it = comptes.begin();
    while (it != comptes.end()) {
        if (estObjet(it->first)) {
            resultat[it->first] = it->second;
        }
        ++it;
    }
    return resultat;
}

std::map<Ressource, int> ZoneDeJeu::symbolesVisibles() const
{
    return compterSymbolesVisibles();
}

std::vector<Coordonnee> ZoneDeJeu::positionsCandidates() const
{
    std::set<Coordonnee> candidates;
    for (std::size_t i = 0; i < liste.size(); ++i) {
        for (int dx = -1; dx <= 1; dx += 2) {
            for (int dy = -1; dy <= 1; dy += 2) {
                candidates.insert(Coordonnee(liste[i].origine.x + dx, liste[i].origine.y + dy));
            }
        }
    }
    std::vector<Coordonnee> resultat(candidates.begin(), candidates.end());
    return resultat;
}

int ZoneDeJeu::nombreVisibles(Ressource r) const
{
    std::map<Ressource, int> comptes = compterSymbolesVisibles();
    std::map<Ressource, int>::const_iterator it = comptes.find(r);
    if (it == comptes.end()) {
        return 0;
    }
    return it->second;
}

void ZoneDeJeu::bornes(int& xMin, int& yMin, int& xMax, int& yMax) const
{
    if (liste.empty()) {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
        return;
    }
    xMin = liste[0].origine.x;
    yMin = liste[0].origine.y;
    xMax = liste[0].origine.x + 1;
    yMax = liste[0].origine.y + 1;
    for (std::size_t i = 1; i < liste.size(); ++i) {
        if (liste[i].origine.x < xMin) {
            xMin = liste[i].origine.x;
        }
        if (liste[i].origine.y < yMin) {
            yMin = liste[i].origine.y;
        }
        if (liste[i].origine.x + 1 > xMax) {
            xMax = liste[i].origine.x + 1;
        }
        if (liste[i].origine.y + 1 > yMax) {
            yMax = liste[i].origine.y + 1;
        }
    }
}

} // namespace codex
