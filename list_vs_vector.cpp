#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

int main()
{
    ifstream fichier("mots.txt");

    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir mots.txt" << endl;
        return 1;
    }

    // 1. Lecture du fichier
    vector<string> mots;
    string mot;

    while (fichier >> mot) {
        mots.push_back(mot);
    }

    fichier.close();

    if (mots.empty()) {
        cout << "Le fichier est vide." << endl;
        return 0;
    }

    cout << "Nombre de chaines : " << mots.size() << endl;

    // =========================================
    // TEST 1 : INSERTION A LA FIN
    // =========================================

    vector<string> v;
    list<string> l;

    v.reserve(mots.size());

    auto debut = steady_clock::now();

    for (const string& s : mots)
        v.push_back(s);

    auto fin = steady_clock::now();

    double tempsVectorFin =
        duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();

    for (const string& s : mots)
        l.push_back(s);

    fin = steady_clock::now();

    double tempsListFin =
        duration<double, milli>(fin - debut).count();

    cout << "\n--- Insertion a la fin ---\n";
    cout << "Vector : " << tempsVectorFin << " ms\n";
    cout << "List   : " << tempsListFin << " ms\n";

    // =========================================
    // TEST 2 : INSERTION AU DEBUT
    // =========================================

    vector<string> v2;
    list<string> l2;

    debut = steady_clock::now();

    for (const string& s : mots)
        v2.insert(v2.begin(), s);

    fin = steady_clock::now();

    double tempsVectorDebut =
        duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();

    for (const string& s : mots)
        l2.push_front(s);

    fin = steady_clock::now();

    double tempsListDebut =
        duration<double, milli>(fin - debut).count();

    cout << "\n--- Insertion au debut ---\n";
    cout << "Vector : " << tempsVectorDebut << " ms\n";
    cout << "List   : " << tempsListDebut << " ms\n";

    // Le mot recherche est present dans les donnees
    const string cible = mots[mots.size() / 2];

    cout << "\nMot recherche : " << cible << endl;

    // =========================================
    // TEST 3 : RECHERCHE
    // =========================================

    // Repetition pour rendre la mesure plus stable
    const int repetitions = 100;
    size_t trouvesVector = 0;
    size_t trouvesList = 0;

    debut = steady_clock::now();

    for (int i = 0; i < repetitions; ++i) {
        auto it = find(v.begin(), v.end(), cible);

        if (it != v.end())
            ++trouvesVector;
    }

    fin = steady_clock::now();

    double tempsRechercheVector =
        duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();

    for (int i = 0; i < repetitions; ++i) {
        auto it = find(l.begin(), l.end(), cible);

        if (it != l.end())
            ++trouvesList;
    }

    fin = steady_clock::now();

    double tempsRechercheList =
        duration<double, milli>(fin - debut).count();

    cout << "\n--- Recherche (" << repetitions
         << " repetitions) ---\n";

    cout << "Vector : " << tempsRechercheVector << " ms\n";
    cout << "List   : " << tempsRechercheList << " ms\n";

    // =========================================
    // TEST 4 : SUPPRESSION DE TOUTES LES
    // OCCURRENCES DU MOT CIBLE
    // =========================================

    // Copies hors de la zone chronometree
    vector<string> v3 = v;
    list<string> l3 = l;

    debut = steady_clock::now();

    v3.erase(
        remove(v3.begin(), v3.end(), cible),
        v3.end()
    );

    fin = steady_clock::now();

    double tempsSuppressionVector =
        duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();

    l3.remove(cible);

    fin = steady_clock::now();

    double tempsSuppressionList =
        duration<double, milli>(fin - debut).count();

    cout << "\n--- Suppression de toutes les occurrences ---\n";
    cout << "Vector : " << tempsSuppressionVector << " ms\n";
    cout << "List   : " << tempsSuppressionList << " ms\n";

    
    cout << "\nOccurrences supprimees :\n";
    cout << "Vector : " << v.size() - v3.size() << endl;
    cout << "List   : " << l.size() - l3.size() << endl;

    return 0;
}
