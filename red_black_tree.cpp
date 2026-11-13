/*

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
*/

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cstdint>
#include <string>

using namespace std;
using namespace chrono;

using Key = uint64_t;
using Clock = high_resolution_clock;

// =====================================================
// 1. IMPLEMENTATION MANUELLE DU RED-BLACK TREE
// =====================================================

class RedBlackTree {

private:

    enum Color { RED, BLACK };

    struct Node {
        Key key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(Key k, Color c)
            : key(k), color(c),
              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* NIL;
    Node* root;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;

        if (x->right != NIL)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    void insertFix(Node* z) {

        while (z->parent->color == RED) {

            if (z->parent == z->parent->parent->left) {

                Node* uncle = z->parent->parent->right;

                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else {

                Node* uncle = z->parent->parent->left;

                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    Node* searchNode(Key key) const {
        Node* current = root;

        while (current != NIL) {
            if (key == current->key)
                return current;

            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }

        return NIL;
    }

    Node* minimum(Node* node) const {
        while (node->left != NIL)
            node = node->left;

        return node;
    }

    void transplant(Node* u, Node* v) {

        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }

    void deleteFix(Node* x) {

        while (x != root && x->color == BLACK) {

            if (x == x->parent->left) {

                Node* w = x->parent->right;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == BLACK &&
                    w->right->color == BLACK) {

                    if (w != NIL)
                        w->color = RED;

                    x = x->parent;
                }
                else {

                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;

                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {

                Node* w = x->parent->left;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                if (w->right->color == BLACK &&
                    w->left->color == BLACK) {

                    if (w != NIL)
                        w->color = RED;

                    x = x->parent;
                }
                else {

                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;

                    rightRotate(x->parent);
                    x = root;
                }
            }
        }

        x->color = BLACK;
    }

    void destroy(Node* node) {
        if (node == NIL)
            return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:

    RedBlackTree() {
        NIL = new Node(0, BLACK);
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;

        root = NIL;
    }

    ~RedBlackTree() {
        destroy(root);
        delete NIL;
    }

    // INSERTION
    bool insert(Key key) {

        Node* z = new Node(key, RED);
        z->left = NIL;
        z->right = NIL;

        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            y = x;

            if (key == x->key) {
                delete z;
                return false;
            }

            if (key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;

        if (y == NIL)
            root = z;
        else if (key < y->key)
            y->left = z;
        else
            y->right = z;

        insertFix(z);

        return true;
    }

    // SEARCH
    bool find(Key key) const {
        return searchNode(key) != NIL;
    }

    // DELETION
    bool erase(Key key) {

        Node* z = searchNode(key);

        if (z == NIL)
            return false;

        Node* y = z;
        Node* x;

        Color originalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        }
        else {

            y = minimum(z->right);
            originalColor = y->color;
            x = y->right;

            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);

            y->left = z->left;
            y->left->parent = y;

            y->color = z->color;
        }

        delete z;

        if (originalColor == BLACK)
            deleteFix(x);

        return true;
    }
};

// =====================================================
// 2. OUTILS DE BENCHMARK
// =====================================================

volatile uint64_t globalChecksum = 0;

struct Results {
    double insertion;
    double searchFound;
    double searchMissing;
    double deletion;
    double searchAfterDeletion;
};

template<typename Function>
double measure(Function operation) {

    auto start = Clock::now();

    operation();

    auto end = Clock::now();

    return duration<double, milli>(end - start).count();
}

// =====================================================
// 3. BENCHMARK GENERIQUE
// =====================================================

template<typename Structure>
Results benchmark(
    Structure& structure,
    const vector<Key>& keys,
    const vector<Key>& missing
) {

    uint64_t checksum = 0;

    Results result{};

    size_t n = keys.size();

    // INSERTION
    result.insertion = measure([&]() {

        for (Key key : keys)
            structure.insert(key);

    });

    // RECHERCHE DES VALEURS EXISTANTES
    result.searchFound = measure([&]() {

        for (Key key : keys)
            checksum += structure.find(key);

    });

    // RECHERCHE DES VALEURS INEXISTANTES
    result.searchMissing = measure([&]() {

        for (Key key : missing)
            checksum += structure.find(key);

    });

    // SUPPRESSION DE 50% DES VALEURS
    result.deletion = measure([&]() {

        for (size_t i = 0; i < n; i += 2)
            checksum += structure.erase(keys[i]);

    });

    // VERIFICATION APRES SUPPRESSION
    result.searchAfterDeletion = measure([&]() {

        for (size_t i = 0; i < n; ++i) {

            bool found = structure.find(keys[i]);

            // Les indices pairs ont ete supprimes.
            bool expected = (i % 2 != 0);

            if (found != expected)
                ++checksum;
        }

    });

    globalChecksum = globalChecksum + checksum;

    return result;
}

// =====================================================
// 4. AFFICHAGE DES RESULTATS
// =====================================================

void printResults(
    const string& name,
    const Results& r,
    size_t n
) {

    cout << "\n" << name << "\n";

    cout << "------------------------------------------\n";

    cout << fixed << setprecision(3);

    cout << "Insertion             : "
         << r.insertion << " ms\n";

    cout << "Recherche (existantes): "
         << r.searchFound << " ms\n";

    cout << "Recherche (absentes)  : "
         << r.searchMissing << " ms\n";

    cout << "Suppression (50%)     : "
         << r.deletion << " ms\n";

    cout << "Recherche apres supp. : "
         << r.searchAfterDeletion << " ms\n";

    double total =
        r.insertion +
        r.searchFound +
        r.searchMissing +
        r.deletion +
        r.searchAfterDeletion;

    cout << "Temps total           : "
         << total << " ms\n";

    cout << "Debit insertion       : "
         << (n / (r.insertion / 1000.0))
         << " operations/s\n";
}

// =====================================================
// 5. PROGRAMME PRINCIPAL
// =====================================================

int main(int argc, char* argv[]) {

    vector<size_t> testSizes = {
        10000,
        100000,
        500000,
        1000000,
        2000000
    };

    // Possibilite de choisir N en argument.
    if (argc > 1) {
        size_t customSize = stoull(argv[1]);
        testSizes = {customSize};
    }

    mt19937_64 generator(123456789);

    cout << "==========================================\n";
    cout << "     BENCHMARK RED-BLACK / MAP / HASHMAP\n";
    cout << "==========================================\n";

    cout << "C++17 - Donnees aleatoires reproductibles\n";

    for (size_t n : testSizes) {

        cout << "\n\n##########################################\n";
        cout << "       TEST AVEC " << n << " ELEMENTS\n";
        cout << "##########################################\n";

        vector<Key> keys(n);
        vector<Key> missing(n);

        // Generation de cles uniques.
        for (size_t i = 0; i < n; ++i)
            keys[i] = static_cast<Key>(i) + 1;

        shuffle(keys.begin(), keys.end(), generator);

        // Cles garanties absentes.
        for (size_t i = 0; i < n; ++i)
            missing[i] = static_cast<Key>(n) + i + 1;

        // -------------------------------------
        // RED BLACK TREE
        // -------------------------------------

        RedBlackTree rbTree;

        Results rbResults = benchmark(
            rbTree, keys, missing
        );

        printResults("RED-BLACK TREE", rbResults, n);

        // -------------------------------------
        // MAP
        // -------------------------------------

        map<Key, int> orderedMap;

        // Adaptateur : map utilise insert/find/erase.
        struct MapAdapter {

            map<Key, int>& data;

            void insert(Key k) {
                data.emplace(k, 1);
            }

            bool find(Key k) const {
                return data.find(k) != data.end();
            }

            bool erase(Key k) {
                return data.erase(k) != 0;
            }

        } mapAdapter{orderedMap};

        Results mapResults = benchmark(
            mapAdapter, keys, missing
        );

        printResults("STD::MAP", mapResults, n);

        // -------------------------------------
        // HASH MAP
        // -------------------------------------

        unordered_map<Key, int> hashMap;

        // Reserve pour limiter les rehashings
        // pendant la phase d'insertion.
        hashMap.max_load_factor(0.75f);
        hashMap.reserve(n);

        struct HashAdapter {

            unordered_map<Key, int>& data;

            void insert(Key k) {
                data.emplace(k, 1);
            }

            bool find(Key k) const {
                return data.find(k) != data.end();
            }

            bool erase(Key k) {
                return data.erase(k) != 0;
            }

        } hashAdapter{hashMap};

        Results hashResults = benchmark(
            hashAdapter, keys, missing
        );

        printResults("STD::UNORDERED_MAP (HASHMAP)",
                     hashResults, n);

        cout << "\n==========================================\n";
        cout << "Fin du test pour N = " << n << "\n";
    }

    cout << "\nChecksum : " << globalChecksum << "\n";

    cout << "\nTous les benchmarks sont termines.\n";

    return 0;
}
