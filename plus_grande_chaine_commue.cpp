
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

vector<char> regrouper(vector<char> v)
{
    for(char it1 : v)
    {
        cout<<it1 + it1 * 2<<endl;
    }
}


// Construction d'un tableau de suffixes par doubling
// Le texte contient un symbole 0 unique a la fin.
// Les autres symboles doivent etre strictement positifs.
vector<int> construireSuffixArray(vector<int> s) {
    s.push_back(0);

    int n = (int)s.size();
    int alphabet = max(n, 258);

    vector<int> p(n), c(n), pn(n), cn(n);
    vector<int> cnt(alphabet, 0);

    // Tri initial des symboles
    for (int x : s)
        cnt[x]++;

    for (int i = 1; i < alphabet; i++)
        cnt[i] += cnt[i - 1];

    for (int i = n - 1; i >= 0; i--)
        p[--cnt[s[i]]] = i;

    c[p[0]] = 0;

    for (int i = 1; i < n; i++) {
        c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);
    }

    // Doublement progressif de la longueur des prefixes
    for (int len = 1; len < n; len *= 2) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - len;
            if (pn[i] < 0)
                pn[i] += n;
        }

        fill(cnt.begin(), cnt.end(), 0);

        for (int x : pn)
            cnt[c[x]]++;

        for (int i = 1; i < n; i++)
            cnt[i] += cnt[i - 1];

        for (int i = n - 1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];

        cn[p[0]] = 0;

        for (int i = 1; i < n; i++) {
            pair<int, int> precedent = {
                c[p[i - 1]],
                c[(p[i - 1] + len) % n]
            };

            pair<int, int> courant = {
                c[p[i]],
                c[(p[i] + len) % n]
            };

            cn[p[i]] = cn[p[i - 1]] + (courant != precedent);
        }

        c.swap(cn);

        if (c[p[n - 1]] == n - 1)
            break;
    }

    // Retirer le suffixe du symbole sentinelle
    p.erase(p.begin());

    return p;
}

// Calcul du LCP entre suffixes voisins avec l'algorithme de Kasai
vector<int> construireLCP(
    const vector<int>& texte,
    const vector<int>& sa
) {
    int n = (int)texte.size();

    vector<int> rang(n);
    vector<int> lcp(n, 0);

    for (int i = 0; i < n; i++)
        rang[sa[i]] = i;

    int h = 0;

    for (int i = 0; i < n; i++) {
        int r = rang[i];

        if (r == 0) {
            h = 0;
            continue;
        }

        int j = sa[r - 1];

        while (i + h < n && j + h < n &&
               texte[i + h] == texte[j + h]) {
            h++;
        }

        lcp[r] = h;

        if (h > 0)
            h--;
    }

    return lcp;
}

int main() {
    ifstream fichier("mots.txt");

    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir mots.txt\n";
        return 1;
    }

    vector<string> mots;
    string mot;

    while (fichier >> mot)
        mots.push_back(mot);

    fichier.close();

    if (mots.size() < 2) {
        cout << "Il faut au moins deux mots dans le fichier.\n";
        return 0;
    }

    cout << "Nombre de mots lus : " << mots.size() << '\n';

    // texte : chaque caractere est encode comme valeur positive.
    // Le separateur vaut 1 et chaque caractere vaut 2..257.
    // La sentinelle unique 0 est ajoutee dans construireSuffixArray.
    vector<int> texte;
    vector<int> proprietaire;
    vector<int> debut(mots.size());

    size_t total = 0;

    for (const string& m : mots)
        total += m.size() + 1;

    texte.reserve(total);
    proprietaire.reserve(total);

    for (int id = 0; id < (int)mots.size(); id++) {
        debut[id] = (int)texte.size();

        for (unsigned char c : mots[id]) {
            texte.push_back((int)c + 2);
            proprietaire.push_back(id);
        }

        // Separateur entre les mots
        texte.push_back(1);
        proprietaire.push_back(-1);
    }

    cout << "Construction du tableau de suffixes...\n";

    vector<int> sa = construireSuffixArray(texte);

    cout << "Calcul des sous-chaines communes...\n";

    vector<int> lcp = construireLCP(texte, sa);

    int n = (int)texte.size();

    // Limiter le LCP a la fin des deux mots.
    // Cela empeche les comparaisons de traverser un separateur.
    for (int i = 1; i < n; i++) {
        int pos1 = sa[i - 1];
        int pos2 = sa[i];

        int id1 = proprietaire[pos1];
        int id2 = proprietaire[pos2];

        if (id1 == -1 || id2 == -1) {
            lcp[i] = 0;
            continue;
        }

        int reste1 = debut[id1] + (int)mots[id1].size() - pos1;
        int reste2 = debut[id2] + (int)mots[id2].size() - pos2;

        lcp[i] = min(lcp[i], min(reste1, reste2));

        // Il faut deux mots distincts.
        if (id1 == id2)
            lcp[i] = 0;
    }

    int longueurMax = 0;

    for (int i = 1; i < n; i++)
        longueurMax = max(longueurMax, lcp[i]);

    if (longueurMax == 0) {
        cout << "Aucune sous-chaine non vide commune a deux mots "
             << "distincts n'a ete trouvee.\n";
        return 0;
    }

    // Chaque sous-chaine maximale et une paire de mots temoins.
    map<string, pair<int, int>> solutions;

    int i = 0;

    while (i < n) {
        int j = i;

        // Regrouper les suffixes partageant un prefixe
        // d'au moins longueurMax caracteres.
        while (j + 1 < n && lcp[j + 1] >= longueurMax)
            j++;

        int premierId = -1;
        int deuxiemeId = -1;

        for (int k = i; k <= j; k++) {
            int pos = sa[k];
            int id = proprietaire[pos];

            if (id == -1)
                continue;

            if (premierId == -1) {
                premierId = id;
            } else if (id != premierId) {
                deuxiemeId = id;
                break;
            }
        }

        if (premierId != -1 && deuxiemeId != -1) {
            int pos = sa[i];
            int id = proprietaire[pos];

            if (id != -1) {
                string commun = mots[id].substr(
                    pos - debut[id], longueurMax
                );

                solutions.emplace(
                    commun,
                    make_pair(premierId, deuxiemeId)
                );
            }
        }

        i = j + 1;
    }

    cout << "\n===== RESULTATS =====\n";
    cout << "Longueur maximale : " << longueurMax << '\n';
    cout << "Nombre de sous-chaines maximales : "
         << solutions.size() << "\n\n";

    for (const auto& resultat : solutions) {
        const string& commun = resultat.first;
        int id1 = resultat.second.first;
        int id2 = resultat.second.second;

        cout << "Sous-chaine commune : " << commun << '\n';
        cout << "Mot 1 : " << mots[id1] << '\n';
        cout << "Mot 2 : " << mots[id2] << "\n\n";
    }

    return 0;
}