#include "controleur/Menu.h"

#include "controleur/Saisie.h"
#include "noyau/ExceptionJeu.h"
#include "vue/Vue.h"

#include <sstream>

namespace codex {
namespace {

int convertirEntier(const std::string& texte)
{
    std::istringstream flux(texte);
    int valeur = 0;
    flux >> valeur;
    if (flux.fail()) {
        throw SaisieInvalide("ce n'est pas un nombre : " + texte);
    }
    return valeur;
}

} // namespace

Menu::Menu(Vue& vueParam, Saisie& saisieParam) : vue(vueParam), saisie(saisieParam)
{
}

int Menu::demanderEntier(const std::string& question, int minimum, int maximum)
{
    int essais = 0;
    while (essais < 50) {
        ++essais;
        vue.afficherMessage(question);
        std::string ligne = saisie.lireLigne();
        if (ligne.empty() && !saisie.disponible()) {
            throw SaisieInvalide("fin des entrees");
        }
        try {
            int valeur = convertirEntier(ligne);
            if (valeur >= minimum && valeur <= maximum) {
                return valeur;
            }
            std::ostringstream message;
            message << "valeur hors limites : il faut entre " << minimum << " et " << maximum;
            vue.afficherErreur(message.str());
        } catch (const SaisieInvalide& erreur) {
            vue.afficherErreur(erreur.what());
        }
    }
    throw SaisieInvalide("trop d'erreurs de saisie");
}

std::string Menu::demanderTexte(const std::string& question, const std::string& valeurParDefaut)
{
    vue.afficherMessage(question);
    std::string ligne = saisie.lireLigne();
    if (ligne.empty()) {
        return valeurParDefaut;
    }
    return ligne;
}

ConfigurationPartie Menu::demanderConfiguration(unsigned int graineParDefaut)
{
    ConfigurationPartie configuration;
    configuration.graine = graineParDefaut;

    vue.afficherMessage("Nombre de joueurs (1 a 4) :");
    int nombre = demanderEntier("Votre choix :", 1, 4);

    for (int i = 0; i < nombre; ++i) {
        std::ostringstream question;
        question << "Nom du joueur " << (i + 1) << " :";
        std::ostringstream defaut;
        defaut << "Joueur" << (i + 1);
        std::string nom = demanderTexte(question.str(), defaut.str());

        std::ostringstream questionType;
        questionType << "Joueur " << nom << " : 1 = humain, 2 = ordinateur";
        int type = demanderEntier(questionType.str(), 1, 2);

        int niveau = 0;
        if (type == 2) {
            niveau = demanderEntier("Niveau de l'ordinateur : 1 = hasard, 2 = glouton, 3 = objectifs", 1, 3) - 1;
        }
        configuration.sieges.push_back(ConfigurationJoueur(nom, type == 2, niveau));
    }

    int graine = demanderEntier("Graine du hasard (nombre entier positif, 2026 par defaut) :", 1, 1000000000);
    configuration.graine = static_cast<unsigned int>(graine);
    return configuration;
}

} // namespace codex
