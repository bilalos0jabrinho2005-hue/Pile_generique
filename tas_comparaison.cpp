
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>
#include <iomanip>
#include <cstdint>
#include <stdexcept>

using namespace std;
using namespace chrono;

// ============================================
// VERSION 1 : TAS MAXIMUM AVEC VECTOR
// ============================================

class TasVector {
private:
    vector<int> T;

public:

    // Insertion : Sift-Up
    void inserer(int x) {
        T.push_back(x);

        int i = T.size() - 1;

        while (i > 0) {
            int parent = (i - 1) / 2;

            if (T[i] > T[parent]) {
                swap(T[i], T[parent]);
                i = parent;
            }
            else break;
        }
    }

    // Recherche lineaire
    bool rechercher(int x) const {
        for (int val : T) {
            if (val == x)
                return true;
        }
        return false;
    }

    // Parcours direct
    uint64_t parcourir() const {
        uint64_t somme = 0;

        for (int val : T)
            somme += val;

        return somme;
    }

    // Extraction du maximum : Sift-Down
    int extraireMax() {
        if (T.empty())
            throw runtime_error("Tas vide");

        int maximum = T[0];

        T[0] = T.back();
        T.pop_back();

        int i = 0;
        int n = T.size();

        while (true) {
            int gauche = 2 * i + 1;
            int droite = 2 * i + 2;
            int plusGrand = i;

            if (gauche < n && T[gauche] > T[plusGrand])
                plusGrand = gauche;

            if (droite < n && T[droite] > T[plusGrand])
                plusGrand = droite;

            if (plusGrand == i)
                break;

            swap(T[i], T[plusGrand]);
            i = plusGrand;
        }

        return maximum;
    }

    bool vide() const {
        return T.empty();
    }
};

// ============================================
// VERSION 2 : TAS STL priority_queue
// ============================================

// Recherche dans une copie du tas
bool rechercherSTL(priority_queue<int> tas, int x) {
    while (!tas.empty()) {
        if (tas.top() == x)
            return true;

        tas.pop();
    }

    return false;
}

// Parcours dans l'ordre decroissant
uint64_t parcourirSTL(priority_queue<int> tas) {
    uint64_t somme = 0;

    while (!tas.empty()) {
        somme += tas.top();
        tas.pop();
    }

    return somme;
}

// ============================================
// PROGRAMME PRINCIPAL
// ============================================

int main() {

    const int N = 100000;

    // Generation des donnees
    vector<int> donnees(N);

    iota(donnees.begin(), donnees.end(), 1);

    mt19937 generateur(42);
    shuffle(donnees.begin(), donnees.end(), generateur);

    TasVector tasVector;
    priority_queue<int> tasSTL;

    // ========================================
    // 1. INSERTION
    // ========================================

    auto debut = steady_clock::now();

    for (int x : donnees)
        tasVector.inserer(x);

    auto fin = steady_clock::now();

    double tempsInsertionVector =
        duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();

    for (int x : donnees)
        tasSTL.push(x);

    fin = steady_clock::now();

    double tempsInsertionSTL =
        duration<double, milli>(fin - debut).count();

    // ========================================
    // 2. RECHERCHE
    // ========================================

    const int elementRecherche = -1;

    debut = steady_clock::now();

    bool resultatVector =
        tasVector.rechercher(elementRecherche);

    fin = steady_clock::now();

    double tempsRechercheVector =
        duration<double, milli>(fin - debut).count();

    // Copie effectuee AVANT le chronometrage
    priority_queue<int> copieRecherche = tasSTL;

    debut = steady_clock::now();

    bool resultatSTL =
        rechercherSTL(copieRecherche, elementRecherche);

    fin = steady_clock::now();

    double tempsRechercheSTL =
        duration<double, milli>(fin - debut).count();

    // ========================================
    // 3. PARCOURS
    // ========================================

    debut = steady_clock::now();

    uint64_t sommeVector = tasVector.parcourir();

    fin = steady_clock::now();

    double tempsParcoursVector =
        duration<double, milli>(fin - debut).count();

    // Copie avant le chronometrage
    priority_queue<int> copieParcours = tasSTL;

    debut = steady_clock::now();

    uint64_t sommeSTL = parcourirSTL(copieParcours);

    fin = steady_clock::now();

    double tempsParcoursSTL =
        duration<double, milli>(fin - debut).count();

    // ========================================
    // 4. EXTRACTION DE TOUS LES ELEMENTS
    // ========================================

    // Reconstruire deux tas identiques pour cette mesure
    // La construction n'est pas incluse dans le chronometrage.

    TasVector tasExtractionVector;
    priority_queue<int> tasExtractionSTL;

    for (int x : donnees) {
        tasExtractionVector.inserer(x);
        tasExtractionSTL.push(x);
    }

    uint64_t sommeExtractionVector = 0;

    debut = steady_clock::now();

    while (!tasExtractionVector.vide()) {
        sommeExtractionVector +=
            tasExtractionVector.extraireMax();
    }

    fin = steady_clock::now();

    double tempsExtractionVector =
        duration<double, milli>(fin - debut).count();

    uint64_t sommeExtractionSTL = 0;

    debut = steady_clock::now();

    while (!tasExtractionSTL.empty()) {
        sommeExtractionSTL += tasExtractionSTL.top();
        tasExtractionSTL.pop();
    }

    fin = steady_clock::now();

    double tempsExtractionSTL =
        duration<double, milli>(fin - debut).count();

    // ========================================
    // 5. AFFICHAGE DES RESULTATS
    // ========================================

    cout << "\n========== BENCHMARK TAS MAXIMUM ==========\n";

    cout << "Nombre d'elements : " << N << endl;

    cout << fixed << setprecision(3);

    cout << "\n";
    cout << left << setw(22) << "Operation"
         << right << setw(18) << "Vector (ms)"
         << setw(18) << "STL (ms)" << endl;

    cout << string(58, '-') << endl;

    cout << left << setw(22) << "Insertion"
         << right << setw(18) << tempsInsertionVector
         << setw(18) << tempsInsertionSTL << endl;

    cout << left << setw(22) << "Recherche"
         << right << setw(18) << tempsRechercheVector
         << setw(18) << tempsRechercheSTL << endl;

    cout << left << setw(22) << "Parcours"
         << right << setw(18) << tempsParcoursVector
         << setw(18) << tempsParcoursSTL << endl;

    cout << left << setw(22) << "Extraction totale"
         << right << setw(18) << tempsExtractionVector
         << setw(18) << tempsExtractionSTL << endl;

    cout << string(58, '-') << endl;

    // ========================================
    // 6. VERIFICATION
    // ========================================

    cout << "\n========== VERIFICATION ==========\n";

    cout << "Somme parcours Vector : " << sommeVector << endl;
    cout << "Somme parcours STL    : " << sommeSTL << endl;

    cout << "Somme extraction Vector : "
         << sommeExtractionVector << endl;

    cout << "Somme extraction STL    : "
         << sommeExtractionSTL << endl;

    cout << "Recherche Vector : "
         << (resultatVector ? "Trouve" : "Non trouve") << endl;

    cout << "Recherche STL : "
         << (resultatSTL ? "Trouve" : "Non trouve") << endl;

    return 0;
}
