#include "harnais.h"
#include "controleur/SimulationIA.h"
#include "modele/ConfigurationPartie.h"

#include <sstream>

namespace {

codex::ConfigurationPartie configurationAuto(int joueurs, int niveau, unsigned int graine)
{
    codex::ConfigurationPartie configuration;
    configuration.graine = graine;
    for (int i = 0; i < joueurs; ++i) {
        std::ostringstream nom;
        nom << "IA" << (i + 1);
        configuration.sieges.push_back(codex::ConfigurationJoueur(nom.str(), true, niveau));
    }
    return configuration;
}

} // namespace

void testerIA()
{
    int partiesTerminees = 0;
    int meilleurScore = 0;

    for (int joueurs = 1; joueurs <= 4; ++joueurs) {
        for (int niveau = 0; niveau <= 2; ++niveau) {
            codex::ConfigurationPartie configuration =
                configurationAuto(joueurs, niveau, static_cast<unsigned int>(100 + joueurs * 10 + niveau));
            codex::ResumeSimulation resume = codex::jouerPartieAutomatique(configuration);
            VERIFIER_VRAI(resume.terminee, "la partie automatique se termine");
            VERIFIER_EGAL(joueurs, static_cast<int>(resume.totaux.size()), "un total par joueur");
            if (resume.terminee) {
                ++partiesTerminees;
            }
            for (std::size_t i = 0; i < resume.totaux.size(); ++i) {
                VERIFIER_VRAI(resume.totaux[i] >= 0, "score positif");
                VERIFIER_EGAL(resume.totaux[i], resume.pointsCartes[i] + resume.pointsObjectifs[i],
                              "total = cartes + objectifs");
                if (resume.totaux[i] > meilleurScore) {
                    meilleurScore = resume.totaux[i];
                }
            }
            VERIFIER_VRAI(resume.toursJoues > 0, "des tours ont ete joues");
        }
    }
    VERIFIER_EGAL(12, partiesTerminees, "12 parties automatiques terminees");
    VERIFIER_VRAI(meilleurScore >= 20, "au moins une partie atteint 20 points");
}
