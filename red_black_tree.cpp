
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

enum Color {
    RED,
    BLACK
};

class RedBlackTree {

private:

    struct Node {
        int data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(int value) {
            data = value;
            color = RED;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }
    };

    Node* root = nullptr;

    // Un pointeur nullptr est considere comme noir
    Color getColor(Node* node) {
        return (node == nullptr) ? BLACK : node->color;
    }

    // Rotation gauche
    void rotateLeft(Node* x) {

        Node* y = x->right;

        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;

        else if (x == x->parent->left)
            x->parent->left = y;

        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Rotation droite
    void rotateRight(Node* x) {

        Node* y = x->left;

        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;

        else if (x == x->parent->right)
            x->parent->right = y;

        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    // Correction apres insertion
    void fixInsert(Node* z) {

        while (z != root &&
               getColor(z->parent) == RED) {

            Node* parent = z->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {

                Node* uncle = grandparent->right;

                // Cas 1 : oncle rouge
                if (getColor(uncle) == RED) {

                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;

                    z = grandparent;
                }

                else {

                    // Cas 2 : triangle gauche-droite
                    if (z == parent->right) {
                        z = parent;
                        rotateLeft(z);
                    }

                    // Cas 3 : ligne gauche-gauche
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;

                    rotateRight(z->parent->parent);
                }
            }

            else {

                Node* uncle = grandparent->left;

                // Cas 1 : oncle rouge
                if (getColor(uncle) == RED) {

                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;

                    z = grandparent;
                }

                else {

                    // Cas 2 : triangle droite-gauche
                    if (z == parent->left) {
                        z = parent;
                        rotateRight(z);
                    }

                    // Cas 3 : ligne droite-droite
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;

                    rotateLeft(z->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    // Recherche interne
    Node* searchNode(int value) const {

        Node* current = root;

        while (current != nullptr) {

            if (value == current->data)
                return current;

            if (value < current->data)
                current = current->left;

            else
                current = current->right;
        }

        return nullptr;
    }

    // Liberation de la memoire
    void destroy(Node* node) {

        if (node == nullptr)
            return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

public:

    ~RedBlackTree() {
        destroy(root);
    }

    // Insertion publique
    // Les doublons sont ignores
    bool insert(int value) {

        Node* z = new Node(value);

        Node* parent = nullptr;
        Node* current = root;

        // Recherche de la position d'insertion
        while (current != nullptr) {

            parent = current;

            if (value == current->data) {
                delete z;
                return false;
            }

            if (value < current->data)
                current = current->left;

            else
                current = current->right;
        }

        z->parent = parent;

        if (parent == nullptr)
            root = z;

        else if (value < parent->data)
            parent->left = z;

        else
            parent->right = z;

        fixInsert(z);

        return true;
    }

    // Recherche publique
    bool search(int value) const {
        return searchNode(value) != nullptr;
    }
};


// Programme principal : lecture en streaming
int main() {

    ifstream file("nombres.txt");

    if (!file.is_open()) {
        cerr << "Erreur : impossible d'ouvrir nombres.txt\n";
        return 1;
    }

    RedBlackTree tree;

    int value;
    long long total = 0;
    long long inserted = 0;

    // Mesure du temps d'insertion
    auto start = high_resolution_clock::now();

    // Lecture et insertion un par un
    while (file >> value) {

        total++;

        if (tree.insert(value))
            inserted++;
    }

    auto end = high_resolution_clock::now();

    file.close();

    double insertionTime =
        duration<double, milli>(end - start).count();

    cout << fixed << setprecision(3);

    cout << "\n===== RESULTATS INSERTION =====\n";

    cout << "Nombre de valeurs lues : "
         << total << endl;

    cout << "Nombre de valeurs inserees : "
         << inserted << endl;

    cout << "Doublons ignores : "
         << total - inserted << endl;

    cout << "Temps d'insertion (ms) : "
         << insertionTime << endl;

    if (insertionTime > 0) {
        cout << "Debit (valeurs/s) : "
             << inserted / (insertionTime / 1000.0)
             << endl;
    }

    // Mesure de recherche
    int recherche;
while(true){

    cout << "\nEntrez une valeur a rechercher : ";
    cin >> recherche;
    
    auto searchStart = high_resolution_clock::now();

    bool found = tree.search(recherche);

    auto searchEnd = high_resolution_clock::now();

    double searchTime =
        duration<double, micro>(searchEnd - searchStart).count();
        
        cout << (found ? "Valeur trouvee" : "Valeur non trouvee")
        << endl;
        
        cout << "Temps de recherche (microsecondes) : "
        << searchTime << endl;
    }

    return 0;
}
