#pragma once
#include <stdexcept>

template <typename T>
class PilePointeur {
private:
    struct Noeud {
        T valeur;
        Noeud* suivant;
        Noeud(const T& v, Noeud* s) : valeur(v), suivant(s) {}
    };

    Noeud* sommet;

public:
    PilePointeur() : sommet(nullptr) {}

    ~PilePointeur() {
        while (!empty())
            pop();
    }

    PilePointeur(const PilePointeur&) = delete;
    PilePointeur& operator=(const PilePointeur&) = delete;

    bool empty() const { return sommet == nullptr; }

    void push(const T& valeur) {
        sommet = new Noeud(valeur, sommet);
    }

    void pop() {
        if (empty())
            throw std::underflow_error("Pile vide");
        Noeud* ancien = sommet;
        sommet = sommet->suivant;
        delete ancien;
    }

    const T& top() const {
        if (empty())
            throw std::underflow_error("Pile vide");
        return sommet->valeur;
    }
};
