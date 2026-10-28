#include <iostream>
#include <vector>
using namespace std;

int main()
{
    string chaine;
    vector<char> inverse;
    

    cout << "Entrez une chaine : ";
    getline(cin, chaine);

    
    for (string::const_reverse_iterator it = chaine.crbegin();
         it != chaine.crend(); ++it)
    {
        inverse.push_back(*it);
    }

    // Afficher la chaine inversee
    cout << "Chaine inversee : ";

    for (vector<char>::const_iterator it = inverse.cbegin();
         it != inverse.cend(); ++it)
    {
        cout << *it;
    }

    cout << endl;
    return 0;
}
