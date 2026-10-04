# PileDLL - Pile générique et validation d'expression

## Contenu
- `PileTableau<T>` : pile générique avec tableau dynamique.
- `PilePointeur<T>` : pile générique avec liste chaînée.
- `PileDLL.dll` : DLL qui expose les deux fonctions de validation.
- `test.cpp` : programme qui utilise la DLL.

## Compilation GCC / MinGW

Créer la DLL :
```bash
g++ -std=c++17 -Wall -Wextra -O2 -shared -DPILEDLL_EXPORTS PileDLL.cpp -o PileDLL.dll -Wl,--out-implib,libPileDLL.dll.a
```

Compiler le test :
```bash
g++ -std=c++17 -Wall -Wextra -O2 test.cpp -L. -lPileDLL -o test.exe
```

Exécuter :
```bash
test.exe
```

La DLL doit être dans le même dossier que `test.exe`.

## Avec Make
```bash
make
test.exe
```

Nettoyage :
```bash
make clean
```

## Concept

```text
test.exe
   |
   v
PileDLL.dll
   |
   +--> validerExpressionTableau()
   |        |
   |        +--> PileTableau<char>
   |             --> tableau dynamique
   |
   +--> validerExpressionPointeur()
            |
            +--> PilePointeur<char>
                 --> liste chaînée
```

La validation vérifie `()`, `[]` et `{}`. Elle vérifie les délimiteurs,
pas toute la grammaire mathématique d'une expression.

## Options GCC importantes
- `-shared` : construit une bibliothèque dynamique.
- `-DPILEDLL_EXPORTS` : active l'exportation des fonctions.
- `-o` : choisit le nom du fichier produit.
- `-L.` : cherche la bibliothèque dans le dossier courant.
- `-lPileDLL` : lie avec `libPileDLL.dll.a`.
- `-Wall -Wextra` : active les avertissements.
- `-std=c++17` : utilise C++17.
