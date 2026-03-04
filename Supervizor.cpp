#include "Supervizor.h"
#include <iostream>

Supervizor::Supervizor(int id, string nume, string prenume, string cnp, string dataAngajareStr, string oras): Angajat(id, nume, prenume, cnp, dataAngajareStr, oras) {
}

double Supervizor::calc_sal() const {
    double salariuStandard = Angajat::calc_sal();
    double sporConducere = SALARIU_BAZA * 0.20;

    return salariuStandard + sporConducere;
}

void Supervizor::afisare() const {
    cout << "--- SUPERVIZOR ---" << endl;
    cout << "ID: " << id << endl;
    cout << "Nume: " << nume << " " << prenume << endl;
    cout << "CNP: " << cnp << endl;
    cout << "Salariu curent: " << calc_sal() << " RON" << endl;
    cout << "------------------" << endl;
}

