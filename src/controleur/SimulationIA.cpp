#include "controleur/SimulationIA.h"

#include "controleur/TourAutomatique.h"
#include "modele/Carte.h"
#include "modele/Partie.h"
#include "modele/StrategieIA.h"
#include "noyau/ExceptionJeu.h"

#include <memory>

namespace codex {

ResumeSimulation::ResumeSimulation()
    : terminee(false), gagnant(-1), totaux(), pointsCartes(), pointsObjectifs(), toursJoues(0)
{
}

ResumeSimulation jouerPartieAutomatique(const ConfigurationPartie& configuration)
{
    ResumeSimulation resume;
    GenerateurAleatoire generateur(configuration.graine);

    Partie partie;
    partie.preparer(configuration, generateur);

    std::vector<std::shared_ptr<GenerateurAleatoire> > generateurs;
    std::vector<std::shared_ptr<StrategieIA> > strategies;

    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        std::shared_ptr<GenerateurAleatoire> generateurIA = std::make_shared<GenerateurAleatoire>(
            configuration.graine + static_cast<unsigned int>(i) * 7919U + 17U);
        generateurs.push_back(generateurIA);
        const Joueur& joueur = partie.joueur(i);
        if (joueur.estTybor()) {
            strategies.push_back(std::shared_ptr<StrategieIA>());
        } else {
            strategies.push_back(creerStrategie(joueur.niveau(), *generateurIA));
        }
    }

    for (int i = 0; i < partie.nombreJoueurs(); ++i) {
        if (partie.joueur(i).estTybor()) {
            continue;
        }
        partie.choisirFaceInitiale(i, generateurs[static_cast<std::size_t>(i)]->suivant(2U) == 0U);
        partie.choisirObjectifSecret(i, static_cast<int>(generateurs[static_cast<std::size_t>(i)]->suivant(2U)));
    }
    partie.commencer();

    int securite = 0;
    while (!partie.estTerminee()) {
        ++securite;
        if (securite > 30000) {
            throw ExceptionJeu("boucle infinie detectee dans la simulation");
        }
        int indice = partie.joueurCourant();
        if (partie.joueur(indice).estTybor()) {
            partie.jouerTourTybor();
        } else {
            jouerTourAutomatique(partie, indice, *strategies[static_cast<std::size_t>(indice)]);
        }
        ++resume.toursJoues;
    }

    resume.terminee = partie.estTerminee();
    resume.gagnant = partie.gagnant();
    const std::vector<ResultatJoueur>& resultats = partie.resultats();
    for (std::size_t i = 0; i < resultats.size(); ++i) {
        resume.totaux.push_back(resultats[i].total);
        resume.pointsCartes.push_back(resultats[i].pointsCartes);
        resume.pointsObjectifs.push_back(resultats[i].pointsObjectifs);
    }
    return resume;
}

} // namespace codex
