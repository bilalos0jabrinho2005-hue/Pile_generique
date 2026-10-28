
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

// Estimation de la mémoire occupee par les mots stockes
double memoireVecteurMo(const vector<string>& mots) 
{
    size_t octets = mots.capacity() * sizeof(string);

    for (const string& mot : mots) {
        octets += mot.capacity() + 1;
    }

    return octets / (1024.0 * 1024.0);
}

// Test de sous-sequence avec find
bool contientDansLOrdre(const string& mot1, const string& mot2) {
    auto it = mot1.begin();

    for (char c : mot2) {
        it = find(it, mot1.end(), c);

        if (it == mot1.end())
            return false;

        ++it;
    }

    return true;
}


void afficherResultats(const vector<string>& resultats) {
    cout << resultats.size() << " mot(s) trouve(s)." << endl;

    size_t limite = min<size_t>(resultats.size(), 10);

    for (size_t i = 0; i < limite; i++) {
        cout << " - " << resultats[i] << endl;
    }

    if (resultats.size() > limite) {
        cout << " ... (" << resultats.size() - limite
             << " autres)" << endl;
    }
}

int main() {
    int mode;

    cout << "Mode 1 = charger UNE fois dans un vector, puis chercher\n"
         << "Mode 2 = relire le fichier a chaque recherche\n"
         << "Votre choix : ";

    if (!(cin >> mode) || (mode != 1 && mode != 2)) {
        cerr << "Mode invalide. Choisissez 1 ou 2." << endl;
        return 1;
    }

    vector<string> mots;
    double memApresLecture = 0.0;
    double tempsLecture = 0.0;

    // MODE 1 : chargement unique
    if (mode == 1) {
        ifstream fichier("mots.txt");

        if (!fichier.is_open()) {
            cerr << "Erreur : impossible d'ouvrir mots.txt" << endl;
            return 1;
        }

        mots.reserve(1000000);
        string mot;

        auto t0 = chrono::steady_clock::now();

        while (fichier >> mot) {
            mots.push_back(mot);
        }

        auto t1 = chrono::steady_clock::now();
        fichier.close();

        tempsLecture =
            chrono::duration<double, milli>(t1 - t0).count();

        memApresLecture = memoireVecteurMo(mots);

        cout << "\n" << mots.size() << " mots charges en "
             << tempsLecture << " ms." << endl;

        cout << "Memoire estimee du vector : "
             << fixed << setprecision(2)
             << memApresLecture << " Mo" << endl;
    }

    // BOUCLE DE RECHERCHE
    string recherche;

    while (true) {
        cout << "\nLettres a chercher (0 pour quitter) : ";
        cin >> recherche;
        uint8_t num = 16; 
        if (recherche == "0")
            break;

        vector<string> resultats;
        size_t nbLus = 0;

        if (mode == 1) {
            // Recherche en RAM
            auto t0 = chrono::steady_clock::now();

            for (const string& m : mots) {
                if (contientDansLOrdre(m, recherche)) {
                    resultats.push_back(m);
                }
            }

            auto t1 = chrono::steady_clock::now();

            nbLus = mots.size();

            afficherResultats(resultats);

            cout << "Mots parcourus : " << nbLus << endl;
            cout << "Temps de recherche : "
                 << chrono::duration<double, milli>(t1 - t0).count()
                 << " ms" << endl;

        } else {
            // Lecture et recherche a chaque requete
            ifstream fichier("mots.txt");

            if (!fichier.is_open()) {
                cerr << "Erreur : impossible d'ouvrir mots.txt" << endl;
                return 1;
            }

            string mot;

            auto t0 = chrono::steady_clock::now();

            while (fichier >> mot) {
                nbLus++;

                if (contientDansLOrdre(mot, recherche)) {
                    resultats.push_back(mot);
                }
            }

            auto t1 = chrono::steady_clock::now();
            fichier.close();

            afficherResultats(resultats);

            cout << "Mots lus : " << nbLus << endl;
            cout << "Temps (lecture + recherche) : "
                 << chrono::duration<double, milli>(t1 - t0).count()
                 << " ms" << endl;
        }

        cout << "Memoire estimee des mots stockes : "
             << memoireVecteurMo(mots) << " Mo" << endl;
    }

    // BILAN
    cout << "\n===== BILAN MEMOIRE =====" << endl;
    cout << "sizeof(string) : " << sizeof(string)
         << " octets" << endl;

    if (mode == 1) {
        cout << "Nombre de mots : " << mots.size() << endl;
        cout << "Capacite du vector : " << mots.capacity() << endl;

        cout << "Memoire estimee du vector : "
             << memoireVecteurMo(mots) << " Mo" << endl;
    } else {
        cout << "Mode 2 : les mots ne sont pas conserves dans un vector."
             << endl;
    }

    return 0;
}


