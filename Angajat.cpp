
#include "Angajat.h"
#include "Utils.h"
#include <iostream>
#include <stdexcept> 
#include <string>
#include <ctime>

bool este_cifra(char c) {
    return (c >= '0' && c <= '9');
}

int Angajat::next_ID = 1;

Angajat::Angajat(int id, string nume, string prenume, string cnp, string dataAngajareStr, string oras): id(next_ID++), nume(nume), prenume(prenume), cnp(cnp), oras_dom(oras), data_angajare(string_to_data(dataAngajareStr)){
    
    //Validare lungime nume si prenume
    if (nume.length() < 3 || nume.length() > 30) {
        throw string("Nume - lungime invalida!");
    }

    if (prenume.length() < 3 || prenume.length() > 30) {
        throw string("Prenume - lungime invalida!");
    }

    // Validare CNP
    if (cnp.length() != 13) {
        throw string("CNP - lungime invalida!");
    }

    for(int i = 0; i < cnp.length(); i++) {
        if (!este_cifra(cnp[i])) {
            throw string("CNP-ul contine caractere invalide!");
        }
    }

    //verificare cifra de control
    int cheie[] = {2, 7, 9, 1, 4, 6, 3, 5, 8, 2, 7, 9};
    int s = 0;
    for (int i = 0; i < 12; i++) {
        s = s+ (cnp[i] - '0') * cheie[i];
    }
    
    int rest = s % 11;
    int cifraControl; 
    if (rest == 10) {
        cifraControl = 1;
    }
    else cifraControl = rest;
    
    if (cifraControl != (cnp[12] - '0')) {
        throw string("Eroare: CNP invalid (cifra de control gresita)!");
    }

    // Validare varsta (minim 16 ani la angajare)
    // Extragere anul nasterii din CNP
    int aa = stoi(cnp.substr(1, 2));
    int prefix = cnp[0] - '0';
    int anNastere = (prefix == 1 || prefix == 2) ? (1900 + aa) : (2000 + aa);

    if (prefix == 1 || prefix == 2) {
        anNastere = 1900+ aa;
    }
    else {
        anNastere = 2000 + aa;
    }

    // Verificre diferenta intre anul angajarii si anul nasterii
    if ((this->data_angajare.an - anNastere) < 16) {
        throw string("Angajatul este prea tanar (sub 16 ani la angajare)!");
    }
}

int Angajat::calc_vechime() const {
    Data azi = get_data_curenta();
    int ani = azi.an - data_angajare.an;

    // Verificam daca anul acesta a implinit anul de munca
    if (azi.luna < data_angajare.luna || (azi.luna == data_angajare.luna && azi.zi < data_angajare.zi)) {
        ani--;
    }

    if (ani < 0)
        return 0;
    return ani;
}

int Angajat::get_id() const { 
    return id; 
}
string Angajat::get_nume() const { 
    return nume; 
}
string Angajat::get_prenume() const { 
    return prenume; 
}
string Angajat::get_CNP() const { 
    return cnp; 
}
string Angajat::get_oras() const { 
    return oras_dom; 
}

void Angajat::set_nume(const string& n) { 
    nume = n; 
}
void Angajat::set_prenume(const string& p) { 
    prenume = p; 
}

double Angajat::calc_sal() const {
    double salariu = SALARIU_BAZA;
    int aniVechime = calc_vechime();
    int pachete = aniVechime / ANI_FIDELITATE; 
    
    salariu = salariu + (SALARIU_BAZA * BONUS_FIDELITATE * pachete);

    if (oras_dom != "Bucuresti") {
        salariu = salariu +  PRIMA_TRANSPORT;
    }

    return salariu;
}

