

#include <iostream>
#include <vector>
#include <string>


using namespace std;


vector<int> construireLPS(const string& motif) {
    int m = motif.size();
    vector<int> lps(m, 0);

    int longueur = 0;
    int i = 1;

    while (i < m) {
        if (motif[i] == motif[longueur]) {
            longueur++;
            lps[i] = longueur;
            i++;
        } else {
            if (longueur != 0) {
                longueur = lps[longueur - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

void KMP(const string& texte, const string& motif) {
    if (motif.empty()) {
        cout << "Le motif est vide.\n";
        return;
    }

    vector<int> lps = construireLPS(motif);

    int i = 0; // position dans le texte
    int j = 0; // position dans le motif
    bool trouve = false;

    while (i < (int)texte.size()) {
        if (texte[i] == motif[j]) {
            i++;
            j++;
        }

        if (j == (int)motif.size()) {
            cout << "Motif trouve a la position "
                 << i - j << '\n';

            trouve = true;
            j = lps[j - 1];
        } else if (i < (int)texte.size() &&
                   texte[i] != motif[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    if (!trouve)
        cout << "Motif introuvable.\n";
}

int main() {
    string texte, motif;

    cout << "Donner le texte : ";
    getline(cin, texte);

    cout << "Donner le motif : ";
    getline(cin, motif);

    KMP(texte, motif);

    return 0;
}