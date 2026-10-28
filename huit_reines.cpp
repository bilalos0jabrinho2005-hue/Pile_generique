#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


const int N = 20;


// positions[i] = colonne de la reine sur la ligne i
bool estSure(const vector<int>& positions, int ligne, int colonne) {
    for (int i = 0; i < ligne; i++) {

        // Même colonne
        if (positions[i] == colonne)
            return false;

        if (abs(positions[i] - colonne) == abs(i - ligne))
            return false;
    }

    return true;
}

bool resoudre(vector<int>& positions, int ligne) {
    if (ligne == N)
        return true;

    // Essayer chaque colonne de la ligne courante
    for (int colonne = 0; colonne < N; colonne++) {
        if (estSure(positions, ligne, colonne)) {
            positions[ligne] = colonne;
            // Passer à la ligne suivante
            if (resoudre(positions, ligne + 1))
                return true;
            // Sinon, retour sur trace :
            // essayer une autre colonne
        }
    }

    return false;
}

void afficher(const vector<int>& positions) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (positions[i] == j)
                cout << "D ";
            else
                cout << ". ";
        }
        cout << '\n';
    }
}

int main() {
    vector<int> positions(N, -1);

    if (resoudre(positions, 0)) {
        cout << "Solution trouvee :\n";
        afficher(positions);
    } else {
        cout << "Aucune solution.\n";
    }

    return 0;
}