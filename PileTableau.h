#pragma once
#include <stdexcept>

template <typename T>
class PileTableau {
private:
    T* elements;
    int sommet;
    int capacite;

public:
    explicit PileTableau(int capaciteInitiale = 100)
        : sommet(-1), capacite(capaciteInitiale) {
        if (capaciteInitiale <= 0)
            throw std::invalid_argument("Capacite invalide");
        elements = new T[capacite];
    }

    ~PileTableau() { delete[] elements; }

    PileTableau(const PileTableau&) = delete;
    PileTableau& operator=(const PileTableau&) = delete;

    bool empty() const { return sommet == -1; }

    void push(const T& valeur) {
        if (sommet + 1 >= capacite)
            throw std::overflow_error("Pile pleine");
        elements[++sommet] = valeur;
    }

    void pop() {
        if (empty())
            throw std::underflow_error("Pile vide");
        --sommet;
    }

    const T& top() const {
        if (empty())
            throw std::underflow_error("Pile vide");
        return elements[sommet];
    }
};
