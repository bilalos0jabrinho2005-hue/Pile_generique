#include <iostream>
#include <string>
#include "PileDLL.h"

void tester(const std::string& expression) {
    bool tableau = validerExpressionTableau(expression.c_str());
    bool pointeur = validerExpressionPointeur(expression.c_str());

    std::cout << "Expression : " << expression << '\n';
    std::cout << "  Tableau dynamique : "
              << (tableau ? "VALIDE" : "INVALIDE") << '\n';
    std::cout << "  Pointeur/liste    : "
              << (pointeur ? "VALIDE" : "INVALIDE") << '\n';
    std::cout << "  Meme resultat     : "
              << (tableau == pointeur ? "OUI" : "NON") << "\n\n";
}




int main() {
    std::cout << "=== TEST DE LA DLL PILE GENERIQUE ===\n\n";

    tester("(10 + 20)");
    tester("((10 + 20) - 5)");
    tester("[10 + (20 * 3)]");
   

    return 0;
}
