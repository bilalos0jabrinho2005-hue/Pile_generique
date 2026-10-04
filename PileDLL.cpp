#include "PileDLL.h"
#include "PileTableau.h"
#include "PilePointeur.h"
#include <cctype>
#include <cstddef>

namespace {
bool estOuverture(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool estFermeture(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool correspond(char ouverture, char fermeture) {
    return (ouverture == '(' && fermeture == ')') ||
           (ouverture == '[' && fermeture == ']') ||
           (ouverture == '{' && fermeture == '}');
}

template <typename Pile>
bool validerAvecPile(const char* expr) {
    if (expr == nullptr)
        return false;

    Pile pile;

    for (std::size_t i = 0; expr[i] != '\0'; ++i) {
        unsigned char uc = static_cast<unsigned char>(expr[i]);
        char c = static_cast<char>(uc);

        if (std::isspace(uc))
            continue;

        if (estOuverture(c)) {
            pile.push(c);
        } else if (estFermeture(c)) {
            if (pile.empty())
                return false;

            if (!correspond(pile.top(), c))
                return false;

            pile.pop();
        }
    }

    return pile.empty();
}
}

bool validerExpressionTableau(const char* expr) {
    return validerAvecPile<PileTableau<char>>(expr);
}

bool validerExpressionPointeur(const char* expr) {
    return validerAvecPile<PilePointeur<char>>(expr);
}
