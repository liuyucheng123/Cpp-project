#include "harnais.h"

#include <iostream>

void testerCartes();
void testerZone();
void testerObjectifs();
void testerPartie();
void testerIA();

int main()
{
    testerCartes();
    testerZone();
    testerObjectifs();
    testerPartie();
    testerIA();

    std::cout << harnais::compteurVerifications() << " verifications, "
              << harnais::compteurEchecs() << " echec(s)." << std::endl;
    if (harnais::compteurEchecs() > 0) {
        std::cout << "TESTS EN ECHEC" << std::endl;
        return 1;
    }
    std::cout << "TOUS LES TESTS PASSENT" << std::endl;
    return 0;
}
