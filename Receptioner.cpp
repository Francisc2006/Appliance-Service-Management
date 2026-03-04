#include "Receptioner.h"
#include <iostream>

Receptioner::Receptioner(int id, string nume, string prenume, string cnp, string dataAngajareStr, string oras): Angajat(id, nume, prenume, cnp, dataAngajareStr, oras) {
}

void Receptioner::adauga_cereri(int id_cerere) {
    id_cereri_inreg.push_back(id_cerere);
}

double Receptioner::calc_sal() const {
    return Angajat::calc_sal();
}

void Receptioner::afisare() const {
    cout << "--- RECEPTIONER ---" << endl;
    cout << "ID: " << id << endl;
    cout << "Nume: " << nume << " " << prenume << endl;
    cout << "CNP: " << cnp << endl;
    cout << "Salariu curent: " << calc_sal() << " RON" << endl;
    cout << "Lista cereri inregistrate (ID-uri): ";
    if (id_cereri_inreg.empty()) {
        cout << "Nicio cerere inregistrata.";
    } else {
        for (int i = 0; i < id_cereri_inreg.size(); i++) {
            cout << id_cereri_inreg[i] << " ";
        }
    }
    cout << endl;
    cout << "---------------------" << endl;
}

