#include "Tehnician.h"
#include <iostream>

Tehnician::Tehnician(int id, string nume, string prenume, string cnp, string dataAngajareStr, string oras): Angajat(id, nume, prenume, cnp, dataAngajareStr, oras), val_reparatii_luna(0.0), nr_cereri_active(0){
}

void Tehnician::adauga_competenta(const string& aparat) {
    electronice.push_back(aparat);
}

bool Tehnician::verif_competenta(const string& aparat) const {
    for (int i = 0; i < electronice.size(); i++) {
        if (electronice[i] == aparat) {
            return true;
        }
    }
    return false;
}

void Tehnician::final_rep(double valoareReparatie) {
    this->val_reparatii_luna += valoareReparatie;
    
    if (nr_cereri_active > 0) {
        nr_cereri_active--;
    }
}

bool Tehnician::disponibil() const {
    return nr_cereri_active < 3; 
}

void Tehnician::preia_cerere(int id_cerere) {
    if (disponibil()) {
        nr_cereri_active++;
        cout << "Tehnicianul " << id << " a preluat cererea cu ID " << id_cerere << endl;
    } 
    else {
        cout << "Eroare: Tehnicianul " << id << " are deja 3 cereri active!" << endl;
    }
}

double Tehnician::calc_sal() const {
    double salariuBaza = Angajat::calc_sal();
    double bonus = val_reparatii_luna * 0.02; 

    return salariuBaza + bonus;
}

void Tehnician::afisare() const {
    cout << "--- TEHNICIAN ---" << endl;
    cout << "ID: " << id << endl;
    cout << "Nume: " << nume << " " << prenume << endl;
    cout << "CNP: " << cnp << endl;
    cout << "Salariu curent: " << calc_sal() << " RON" << endl;
    cout << "Competente: ";
    if (electronice.empty()) {
        cout << "Nicio competenta inregistrata.";
    } else {
        for (int i = 0; i < electronice.size(); i++) {
            cout << electronice[i] << ", ";
        }
    }
    cout << endl;
    cout << "Cereri active: " << nr_cereri_active << "/3" << endl;
    cout << "Valoare reparatii luna curenta: " << val_reparatii_luna << " RON" << endl;
    cout << "-----------------" << endl;
}



