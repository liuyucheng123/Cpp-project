#include "modele/StrategieIA.h"

#include "modele/Carte.h"
#include "modele/FaceCarte.h"
#include "modele/IterateurCartes.h"
#include "modele/ZoneDeJeu.h"
#include "modele/Objectif.h"
#include "modele/Partie.h"
#include "noyau/ExceptionJeu.h"

#include <map>
#include <vector>

namespace codex {
namespace {

struct Candidat
{
    int indiceMain;
    Coordonnee position;
    bool recto;
    int points;
    int diversite;
    int gainObjectif;
    int adjacence;

    Candidat();
};

Candidat::Candidat()
    : indiceMain(0), position(), recto(true), points(0), diversite(0), gainObjectif(0), adjacence(0)
{
}

int diversiteDesRessources(const std::map<Ressource, int>& ressources)
{
    int total = 0;
    std::map<Ressource, int>::const_iterator it = ressources.begin();
    while (it != ressources.end()) {
        if (it->second > 0) {
            ++total;
        }
        ++it;
    }
    return total;
}

// Nombre de cartes du meme royaume deja posees en diagonale de la case visee :
// c'est ce qui prepare les motifs (diagonale, L) des objectifs.
int adjacenceMemeRoyaume(const ZoneDeJeu& zone, Coordonnee position, Ressource royaume)
{
    int total = 0;
    const std::vector<CartePlacee>& cartes = zone.cartesPlacees();
    for (std::size_t i = 0; i < cartes.size(); ++i) {
        int dx = cartes[i].origine.x - position.x;
        int dy = cartes[i].origine.y - position.y;
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1 && dx != 0 && dy != 0
            && cartes[i].carte->royaume() == royaume) {
            ++total;
        }
    }
    return total;
}

int evaluationObjectifs(const Partie& partie, int indiceJoueur, const ZoneDeJeu& zone)
{
    const Joueur& joueur = partie.joueur(indiceJoueur);
    int total = 0;
    if (joueur.objectifSecret()) {
        total += joueur.objectifSecret()->evaluer(zone);
    }
    total += partie.objectifCommun(0)->evaluer(zone);
    total += partie.objectifCommun(1)->evaluer(zone);
    return total;
}

std::vector<Candidat> listerCandidats(const Partie& partie, int indiceJoueur, bool avecObjectifs)
{
    const Joueur& joueur = partie.joueur(indiceJoueur);
    std::vector<Coordonnee> positions = joueur.zone().positionsCandidates();
    std::vector<Candidat> liste;
    std::map<Ressource, int> ressources = joueur.zone().ressourcesVisibles();
    int objectifsAvant = 0;
    if (avecObjectifs) {
        objectifsAvant = evaluationObjectifs(partie, indiceJoueur, joueur.zone());
    }

    for (int m = 0; m < joueur.nombreCartesEnMain(); ++m) {
        std::shared_ptr<const Carte> carte = joueur.main(m);
        for (int face = 0; face < 2; ++face) {
            bool recto = (face == 0);
            if (recto && !carte->exigencesSatisfaites(ressources)) {
                continue;
            }
            for (std::size_t p = 0; p < positions.size(); ++p) {
                std::string cause;
                if (!joueur.zone().placementValide(positions[p], *carte, recto, cause)) {
                    continue;
                }
                ZoneDeJeu copie = joueur.zone();
                int coins = copie.compterCoinsCouverts(positions[p]);
                copie.poser(positions[p], carte, recto, coins);
                ContextePose contexte;
                contexte.coinsCouverts = coins;
                contexte.ressources = copie.symbolesVisibles();

                Candidat candidat;
                candidat.indiceMain = m;
                candidat.position = positions[p];
                candidat.recto = recto;
                candidat.points = carte->pointsAuPose(contexte);
                candidat.diversite = diversiteDesRessources(copie.symbolesVisibles());
                candidat.adjacence = adjacenceMemeRoyaume(joueur.zone(), positions[p], carte->royaume());
                if (avecObjectifs) {
                    candidat.gainObjectif = evaluationObjectifs(partie, indiceJoueur, copie) - objectifsAvant;
                }
                liste.push_back(candidat);
            }
        }
    }
    return liste;
}

int noteGloutonne(const Candidat& candidat)
{
    return candidat.points * 10 + candidat.diversite;
}

int noteObjectifs(const Candidat& candidat)
{
    return candidat.points * 10 + candidat.gainObjectif * 6 + candidat.diversite * 2
         + candidat.adjacence * 3;
}

int valeurCarte(const Carte& carte, bool estDoree)
{
    int valeur = estDoree ? 6 : 3;
    const FaceCarte& face = carte.face(true);
    for (int coin = 0; coin < NB_COINS; ++coin) {
        Ressource symbole = face.coin(coin);
        if (symbole == Ressource::VIDE || symbole == Ressource::CACHE) {
            continue;
        }
        valeur += 1;
    }
    return valeur;
}

ActionTour completerAvecPioche(const Partie& partie, ActionTour action, int niveau,
                               GenerateurAleatoire* generateur)
{
    const Pioche& pioche = partie.pioche();
    int meilleur = -1;
    int meilleureValeur = -1000000;

    for (int i = 0; i < 4; ++i) {
        if (!pioche.etaleeDisponible(i)) {
            continue;
        }
        bool doree = (i >= 2);
        int valeur = valeurCarte(*pioche.etalee(i), doree);
        if (niveau == 0) {
            valeur = static_cast<int>(generateur->suivant(1000U));
        }
        if (valeur > meilleureValeur) {
            meilleureValeur = valeur;
            meilleur = i;
        }
    }

    if (meilleur >= 0) {
        if (meilleur < 2) {
            action.source = SourcePioche::ETALEE_RESSOURCE;
            action.indiceEtalee = meilleur;
        } else {
            action.source = SourcePioche::ETALEE_DOREE;
            action.indiceEtalee = meilleur - 2;
        }
        return action;
    }

    if (!pioche.tasVide(false)) {
        action.source = SourcePioche::TAS_RESSOURCE;
        action.indiceEtalee = 0;
    } else if (!pioche.tasVide(true)) {
        action.source = SourcePioche::TAS_DOREE;
        action.indiceEtalee = 0;
    } else {
        action.source = SourcePioche::ETALEE_RESSOURCE;
        action.indiceEtalee = 0;
    }
    return action;
}

} // namespace

StrategieIA::~StrategieIA()
{
}

IAHasard::IAHasard(GenerateurAleatoire& generateurParam) : generateur(&generateurParam)
{
}

std::string IAHasard::nom() const
{
    return "IA hasard";
}

ActionTour IAHasard::choisirAction(const Partie& partie, int indiceJoueur)
{
    ActionTour action;
    std::vector<Candidat> candidats = listerCandidats(partie, indiceJoueur, false);
    if (!candidats.empty()) {
        std::size_t choix = static_cast<std::size_t>(generateur->suivant(static_cast<unsigned int>(candidats.size())));
        action.indiceMain = candidats[choix].indiceMain;
        action.position = candidats[choix].position;
        action.recto = candidats[choix].recto;
    }
    return completerAvecPioche(partie, action, 0, generateur);
}

IAGloutonne::IAGloutonne(GenerateurAleatoire& generateurParam) : generateur(&generateurParam)
{
}

std::string IAGloutonne::nom() const
{
    return "IA gloutonne";
}

ActionTour IAGloutonne::choisirAction(const Partie& partie, int indiceJoueur)
{
    (void)generateur;
    ActionTour action;
    std::vector<Candidat> candidats = listerCandidats(partie, indiceJoueur, false);

    if (!candidats.empty()) {
        std::size_t meilleur = 0;
        int meilleureNote = noteGloutonne(candidats[0]);
        for (std::size_t i = 1; i < candidats.size(); ++i) {
            int note = noteGloutonne(candidats[i]);
            if (note > meilleureNote) {
                meilleureNote = note;
                meilleur = i;
            }
        }
        action.indiceMain = candidats[meilleur].indiceMain;
        action.position = candidats[meilleur].position;
        action.recto = candidats[meilleur].recto;
    }
    return completerAvecPioche(partie, action, 1, generateur);
}

IAObjectifs::IAObjectifs(GenerateurAleatoire& generateurParam) : generateur(&generateurParam)
{
}

std::string IAObjectifs::nom() const
{
    return "IA objectifs";
}

ActionTour IAObjectifs::choisirAction(const Partie& partie, int indiceJoueur)
{
    (void)generateur;
    ActionTour action;
    std::vector<Candidat> candidats = listerCandidats(partie, indiceJoueur, true);

    if (!candidats.empty()) {
        std::size_t meilleur = 0;
        int meilleureNote = noteObjectifs(candidats[0]);
        for (std::size_t i = 1; i < candidats.size(); ++i) {
            int note = noteObjectifs(candidats[i]);
            if (note > meilleureNote) {
                meilleureNote = note;
                meilleur = i;
            }
        }
        action.indiceMain = candidats[meilleur].indiceMain;
        action.position = candidats[meilleur].position;
        action.recto = candidats[meilleur].recto;
    }
    return completerAvecPioche(partie, action, 2, generateur);
}

std::shared_ptr<StrategieIA> creerStrategie(int niveau, GenerateurAleatoire& generateur)
{
    if (niveau <= 0) {
        std::shared_ptr<StrategieIA> strategie = std::make_shared<IAHasard>(generateur);
        return strategie;
    }
    if (niveau == 1) {
        std::shared_ptr<StrategieIA> strategie = std::make_shared<IAGloutonne>(generateur);
        return strategie;
    }
    std::shared_ptr<StrategieIA> strategie = std::make_shared<IAObjectifs>(generateur);
    return strategie;
}

} // namespace codex
