
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// ==================== NOEUD ====================

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;

    Noeud(int v) : valeur(v), gauche(nullptr), droite(nullptr) {}
};

// ==================== ARBRE BINAIRE DE RECHERCHE ====================

class Arbre {
private:
    Noeud* racine;

    Noeud* insererRec(Noeud* n, int v, long long& comp) {
        if (n == nullptr)
            return new Noeud(v);

        comp++;

        if (v < n->valeur)
            n->gauche = insererRec(n->gauche, v, comp);
        else if (v > n->valeur)
            n->droite = insererRec(n->droite, v, comp);

        return n;
    }

    bool rechercherRec(Noeud* n, int v, long long& comp) const {
        if (n == nullptr)
            return false;

        comp++;

        if (v == n->valeur)
            return true;

        if (v < n->valeur)
            return rechercherRec(n->gauche, v, comp);

        return rechercherRec(n->droite, v, comp);
    }

    Noeud* minimum(Noeud* n, long long& comp) {
        while (n != nullptr && n->gauche != nullptr) {
            comp++;
            n = n->gauche;
        }
        return n;
    }

    Noeud* supprimerRec(Noeud* n, int v,
                        long long& comp, bool& supprime) {
        if (n == nullptr)
            return nullptr;

        comp++;

        if (v < n->valeur) {
            n->gauche = supprimerRec(n->gauche, v, comp, supprime);
        }
        else if (v > n->valeur) {
            n->droite = supprimerRec(n->droite, v, comp, supprime);
        }
        else {
            supprime = true;

            if (n->gauche == nullptr) {
                Noeud* temp = n->droite;
                delete n;
                return temp;
            }

            if (n->droite == nullptr) {
                Noeud* temp = n->gauche;
                delete n;
                return temp;
            }

            Noeud* successeur = minimum(n->droite, comp);
            n->valeur = successeur->valeur;

            bool supprimeSuccesseur = false;
            n->droite = supprimerRec(
                n->droite, successeur->valeur,
                comp, supprimeSuccesseur
            );
        }

        return n;
    }

    void liberer(Noeud* n) {
        if (n == nullptr)
            return;

        liberer(n->gauche);
        liberer(n->droite);
        delete n;
    }

    void afficherRec(Noeud* n) const {
        if (n == nullptr)
            return;

        afficherRec(n->gauche);
        cout << n->valeur << " ";
        afficherRec(n->droite);
    }

public:
    Arbre() : racine(nullptr) {}

    // Insertion sans doublons, avec comptage des comparaisons
    bool inserer(int v, long long& comp) {
        comp = 0;

        bool existe = rechercherRec(racine, v, comp);

        if (existe)
            return false;

        long long compInsertion = 0;
        racine = insererRec(racine, v, compInsertion);
        comp += compInsertion;

        return true;
    }

    bool rechercher(int v, long long& comp) const {
        comp = 0;
        return rechercherRec(racine, v, comp);
    }

    bool supprimer(int v, long long& comp) {
        comp = 0;
        bool supprime = false;
        racine = supprimerRec(racine, v, comp, supprime);
        return supprime;
    }

    void afficher() const {
        afficherRec(racine);
        cout << '\n';
    }

    ~Arbre() {
        liberer(racine);
    }
};

// ==================== CHARGEMENT DU FICHIER ====================

int chargerFichier(const string& nomFichier, Arbre& arbre,
                   long long& comparaisons,
                   double& tempsInsertionMs) {
    ifstream fichier(nomFichier);

    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir "
             << nomFichier << '\n';
        return -1;
    }

    int valeur;
    int nombresLus = 0;
    int nombresInseres = 0;
    comparaisons = 0;

    // La lecture est effectuee en dehors du chronometrage.
    while (fichier >> valeur) {
        long long compOperation = 0;

        auto debut = steady_clock::now();

        bool insere = arbre.inserer(valeur, compOperation);

        auto fin = steady_clock::now();

        tempsInsertionMs +=
            duration<double, milli>(fin - debut).count();

        comparaisons += compOperation;
        nombresLus++;

        if (insere)
            nombresInseres++;
    }

    if (!fichier.eof()) {
        cerr << "Erreur : le fichier contient une valeur invalide.\n";
        return -1;
    }

    cout << "\n===== CHARGEMENT DU FICHIER =====\n";
    cout << "Nombres lus : " << nombresLus << '\n';
    cout << "Valeurs distinctes inserees : " << nombresInseres << '\n';
    cout << "Doublons ignores : "
         << nombresLus - nombresInseres << '\n';
    cout << "Comparaisons : " << comparaisons << '\n';
    cout << "Temps cumule des insertions : "
         << tempsInsertionMs << " ms\n";

    return nombresLus;
}

// ==================== PROGRAMME PRINCIPAL ====================

int main() {
    Arbre arbre;
    int choix, valeur;

    cout << fixed << setprecision(3);

    long long compChargement = 0;
    double tempsChargement = 0.0;

    // Chargement initial AVANT d'afficher le menu.
    int resultatChargement = chargerFichier(
        "nombres.txt", arbre, compChargement, tempsChargement
    );

    if (resultatChargement == -1) {
        cout << "Verifiez que nombres.txt existe dans le dossier "
             << "de travail du programme.\n";
        return 1;
    }

    do {
        cout << "\n========== MENU ABR ==========\n";
        cout << "1. Inserer un nombre\n";
        cout << "2. Rechercher un nombre\n";
        cout << "3. Supprimer un nombre\n";
        cout << "4. Afficher les valeurs triees\n";
        cout << "0. Quitter\n";
        cout << "Votre choix : ";

        if (!(cin >> choix)) {
            cout << "Saisie invalide.\n";
            break;
        }

        if (choix >= 1 && choix <= 3) {
            cout << "Entrez la valeur : ";

            if (!(cin >> valeur)) {
                cout << "Valeur invalide.\n";
                break;
            }

            long long comparaisons = 0;
            bool resultat = false;

            auto debut = steady_clock::now();

            if (choix == 1)
                resultat = arbre.inserer(valeur, comparaisons);
            else if (choix == 2)
                resultat = arbre.rechercher(valeur, comparaisons);
            else
                resultat = arbre.supprimer(valeur, comparaisons);

            auto fin = steady_clock::now();

            double tempsMicro =
                duration<double, micro>(fin - debut).count();

            cout << "\n----- RESULTAT -----\n";

            if (choix == 1)
                cout << (resultat ? "Insertion effectuee.\n"
                                  : "Valeur deja existante.\n");
            else if (choix == 2)
                cout << (resultat ? "Element trouve.\n"
                                  : "Element absent.\n");
            else
                cout << (resultat ? "Suppression effectuee.\n"
                                  : "Element absent : suppression impossible.\n");

            cout << "Comparaisons : " << comparaisons << '\n';
            cout << "Temps de l'operation : "
                 << tempsMicro << " microsecondes\n";
        }
        else if (choix == 4) {
            cout << "Valeurs dans l'ordre croissant : ";
            arbre.afficher();
        }
        else if (choix != 0) {
            cout << "Choix invalide.\n";
        }

    } while (choix != 0);

    cout << "Fin du programme.\n";
    return 0;
}
