

    if (trouve)
        cout << "Element trouve !\n";
    else
        cout << "Element absent !\n";

    cout << "\n===== RESULTATS =====\n";
    cout << "Comparaisons insertion : " << compInsertion << '\n';
    cout << "Temps insertion : " << tempsInsertion << " ms\n";
    cout << "Comparaisons recherche : " << compRecherche << '\n';
    cout << "Temps recherche : " << tempsRecherche << " microsecondes\n";

    return 0;
}
