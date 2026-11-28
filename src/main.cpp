#include "controleur/ControleurPartie.h"
#include "controleur/Menu.h"
#include "controleur/Saisie.h"
#include "controleur/SimulationIA.h"
#include "modele/ConfigurationPartie.h"
#include "modele/Partie.h"
#include "noyau/ExceptionJeu.h"
#include "noyau/GenerateurAleatoire.h"
#include "vue/VueConsole.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using namespace codex;

namespace {

bool convertirEntier(const std::string& texte, int& valeur)
{
    std::istringstream flux(texte);
    flux >> valeur;
    return !flux.fail();
}

void afficherAide()
{
    std::cout << "Usage : codex_naturalis [options]" << std::endl;
    std::cout << "  --graine N     graine du hasard (defaut 2026)" << std::endl;
    std::cout << "  --auto N       joue N parties entierement automatiques" << std::endl;
    std::cout << "  --joueurs N    nombre de joueurs du mode auto (1 a 4 ; 1 = duel contre Tybor)"
              << std::endl;
    std::cout << "  --niveau N     niveau des IA du mode auto (1 a 3)" << std::endl;
    std::cout << "  --difficulte N difficulte du mode solo (1 a 3, defaut 2)" << std::endl;
    std::cout << "  --entree F     rejoue les reponses contenues dans le fichier F" << std::endl;
    std::cout << "  --aide         affiche cette aide" << std::endl;
}

} // namespace

int main(int argc, char** argv)
{
    unsigned int graine = 2026U;
    int partiesAutomatiques = 0;
    int joueursAuto = 2;
    int niveauAuto = 2;
    int difficulteSolo = 2;
    std::string fichierEntree;

    for (int i = 1; i < argc; ++i) {
        std::string option(argv[i]);
        int valeur = 0;
        if (option == "--aide") {
            afficherAide();
            return 0;
        } else if (option == "--graine" && i + 1 < argc && convertirEntier(argv[i + 1], valeur)) {
            graine = static_cast<unsigned int>(valeur < 1 ? 1 : valeur);
            ++i;
        } else if (option == "--auto" && i + 1 < argc && convertirEntier(argv[i + 1], valeur)) {
            partiesAutomatiques = valeur;
            ++i;
        } else if (option == "--joueurs" && i + 1 < argc && convertirEntier(argv[i + 1], valeur)) {
            joueursAuto = valeur < 1 ? 1 : (valeur > 4 ? 4 : valeur);
            ++i;
        } else if (option == "--niveau" && i + 1 < argc && convertirEntier(argv[i + 1], valeur)) {
            niveauAuto = valeur < 1 ? 1 : (valeur > 3 ? 3 : valeur);
            ++i;
        } else if (option == "--difficulte" && i + 1 < argc && convertirEntier(argv[i + 1], valeur)) {
            difficulteSolo = valeur < 1 ? 1 : (valeur > 3 ? 3 : valeur);
            ++i;
        } else if (option == "--entree" && i + 1 < argc) {
            fichierEntree = argv[i + 1];
            ++i;
        }
    }

    if (partiesAutomatiques > 0) {
        int totalParties = 0;
        int totalTours = 0;
        for (int numero = 0; numero < partiesAutomatiques; ++numero) {
            ConfigurationPartie configuration;
            configuration.graine = graine + static_cast<unsigned int>(numero) * 101U;
            configuration.difficulteSolo = difficulteSolo - 1;
            for (int siege = 0; siege < joueursAuto; ++siege) {
                std::ostringstream nom;
                nom << "IA" << (siege + 1);
                configuration.sieges.push_back(ConfigurationJoueur(nom.str(), true, niveauAuto - 1));
            }
            ResumeSimulation resume = jouerPartieAutomatique(configuration);
            ++totalParties;
            totalTours += resume.toursJoues;
            std::cout << "Partie " << (numero + 1) << " : scores";
            for (std::size_t i = 0; i < resume.totaux.size(); ++i) {
                std::cout << " " << resume.totaux[i];
            }
            std::cout << " | cartes";
            for (std::size_t i = 0; i < resume.pointsCartes.size(); ++i) {
                std::cout << " " << resume.pointsCartes[i];
            }
            std::cout << " | objectifs";
            for (std::size_t i = 0; i < resume.pointsObjectifs.size(); ++i) {
                std::cout << " " << resume.pointsObjectifs[i];
            }
            std::cout << " | vainqueur " << resume.gagnant << std::endl;
        }
        std::cout << totalParties << " partie(s) terminee(s), " << totalTours << " tours joues." << std::endl;
        return 0;
    }

    VueConsole vue(std::cout);
    std::ifstream fluxFichier;
    std::unique_ptr<Saisie> saisie;
    if (!fichierEntree.empty()) {
        fluxFichier.open(fichierEntree.c_str());
        if (!fluxFichier.is_open()) {
            std::cout << "Impossible d'ouvrir le fichier " << fichierEntree << std::endl;
            return 1;
        }
        saisie = std::make_unique<AdaptateurFlux>(fluxFichier);
    } else {
        saisie = std::make_unique<SaisieClavier>(std::cin, std::cout);
    }
    Menu menu(vue, *saisie);

    ConfigurationPartie configuration = menu.demanderConfiguration(graine);
    GenerateurAleatoire generateur(configuration.graine);
    Partie partie;

    try {
        partie.preparer(configuration, generateur);
    } catch (const ExceptionJeu& erreur) {
        std::cout << "Impossible de preparer la partie : " << erreur.what() << std::endl;
        return 1;
    }

    ControleurPartie controleur(partie, vue, *saisie, generateur);
    try {
        controleur.executer();
    } catch (const ExceptionJeu& erreur) {
        std::cout << "Arret : " << erreur.what() << std::endl;
        return 1;
    }
    return 0;
}
