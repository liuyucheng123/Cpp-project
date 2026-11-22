#include "harnais.h"
#include "modele/Carte.h"
#include "modele/CatalogueCartes.h"
#include "noyau/ExceptionJeu.h"

#include <map>
#include <memory>

namespace {

bool constructionRefusee(codex::CatalogueCartes& catalogue)
{
    try {
        catalogue.creerCarte(999);
    } catch (const codex::IdentifiantInconnu&) {
        return true;
    }
    return false;
}

bool objectifRefuse(codex::CatalogueCartes& catalogue)
{
    try {
        catalogue.creerObjectif(1);
    } catch (const codex::IdentifiantInconnu&) {
        return true;
    }
    return false;
}

} // namespace

void testerCartes()
{
    codex::CatalogueCartes& catalogue = codex::CatalogueCartes::instance();

    VERIFIER_EGAL(86, catalogue.nombreCartes(), "86 cartes au catalogue (40 + 40 + 6)");
    VERIFIER_EGAL(16, catalogue.nombreObjectifs(), "16 cartes objectif");
    VERIFIER_EGAL(40, static_cast<int>(catalogue.idsRessource().size()), "40 cartes ressource");
    VERIFIER_EGAL(40, static_cast<int>(catalogue.idsDoree().size()), "40 cartes doree");
    VERIFIER_EGAL(6, static_cast<int>(catalogue.idsInitiale().size()), "6 cartes initiales");

    std::map<codex::Ressource, int> royaumes;
    int cartesRessourceAUnPoint = 0;
    for (std::size_t i = 0; i < catalogue.idsRessource().size(); ++i) {
        std::shared_ptr<const codex::Carte> carte = catalogue.creerCarte(catalogue.idsRessource()[i]);
        royaumes[carte->royaume()] = royaumes[carte->royaume()] + 1;
        if (carte->description().find("+1 pt") != std::string::npos) {
            ++cartesRessourceAUnPoint;
        }
    }
    VERIFIER_EGAL(10, royaumes[codex::Ressource::PLANTE], "10 cartes plante");
    VERIFIER_EGAL(10, royaumes[codex::Ressource::ANIMAL], "10 cartes animal");
    VERIFIER_EGAL(10, royaumes[codex::Ressource::CHAMPIGNON], "10 cartes champignon");
    VERIFIER_EGAL(10, royaumes[codex::Ressource::INSECTE], "10 cartes insecte");
    VERIFIER_EGAL(12, cartesRessourceAUnPoint, "12 cartes ressource valent 1 point");

    int doreesStatiques = 0;
    int doreesParAngle = 0;
    int doreesParObjet = 0;
    for (std::size_t i = 0; i < catalogue.idsDoree().size(); ++i) {
        std::shared_ptr<const codex::Carte> carte = catalogue.creerCarte(catalogue.idsDoree()[i]);
        VERIFIER_VRAI(carte->estDoree(), "une carte du paquet doree est doree");
        const std::string description = carte->description();
        if (description.find("par coin recouvert") != std::string::npos) {
            ++doreesParAngle;
        } else if (description.find("par plume") != std::string::npos
                   || description.find("par encrier") != std::string::npos
                   || description.find("par manuscrit") != std::string::npos) {
            ++doreesParObjet;
        } else {
            ++doreesStatiques;
        }
    }
    VERIFIER_EGAL(16, doreesStatiques, "16 cartes doree a points fixes");
    VERIFIER_EGAL(12, doreesParAngle, "12 cartes doree par coin recouvert");
    VERIFIER_EGAL(12, doreesParObjet, "12 cartes doree par objet");

    VERIFIER_EGAL(87, catalogue.idsObjectif()[0], "les objectifs commencent a 87");
    VERIFIER_VRAI(constructionRefusee(catalogue), "carte inconnue refusee");
    VERIFIER_VRAI(objectifRefuse(catalogue), "objectif inconnu refuse");

    // Les exigences d'une carte doree doivent etre coherentes avec son texte.
    std::shared_ptr<const codex::Carte> doree = catalogue.creerCarte(47);
    std::map<codex::Ressource, int> rien;
    VERIFIER_VRAI(!doree->exigencesSatisfaites(rien), "une carte doree exige des ressources");
    std::map<codex::Ressource, int> beaucoup;
    beaucoup[codex::Ressource::PLANTE] = 10;
    beaucoup[codex::Ressource::ANIMAL] = 10;
    beaucoup[codex::Ressource::CHAMPIGNON] = 10;
    beaucoup[codex::Ressource::INSECTE] = 10;
    VERIFIER_VRAI(doree->exigencesSatisfaites(beaucoup), "exigences satisfaites avec assez de ressources");
}
