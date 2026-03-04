#include "Cerere.h"

Cerere::Cerere(int id, shared_ptr<Electrocasnic> aparat, string data_str, int complexitate): id(id), aparat(move(aparat)), complexitate(complexitate), status(Status_cerere::in_asteptare), id_tehnician_asignat(-1) {
    this->timestamp = string_to_time_t(data_str);
    
    int vechime = this->aparat->get_vechime();

    // daca aparatul e nou (0 ani), durata ar fi 0.
    if (vechime == 0) 
        vechime = 1;

    durata_estimata = vechime * complexitate;
    pret_estimat = this->aparat->get_pret() * durata_estimata;
}

string Cerere::get_timestamp_str() const {
        return time_t_to_string(timestamp);
}

time_t Cerere::get_timestamp_raw() const { 
    return timestamp; 
}

int Cerere::get_id() const { 
    return id; 
}

shared_ptr<Electrocasnic> Cerere::get_electrocasnic() const { 
    return aparat; 
}

int Cerere::get_complexitate() const { 
    return complexitate; 
}
int Cerere::get_durata() const { 
    return durata_estimata; 
}
double Cerere::get_pret() const { 
    return pret_estimat; 
}
Status_cerere Cerere::get_status() const { 
    return status; 
}
int Cerere::get_id_tehnician() const { 
    return id_tehnician_asignat; 
}

void Cerere::asigneaza_tehnician(int id_tehnician) {
    id_tehnician_asignat = id_tehnician;
    status = Status_cerere::asignata;
}

void Cerere::marcheaza_finalizata() {
    status = Status_cerere::finalizata;
}

void Cerere::scade_timp_ramas() {
    if (durata_estimata > 0) {
        durata_estimata--;
    }
    if (durata_estimata == 0) {
        marcheaza_finalizata();
    }
}

void Cerere::afisare() const {
    cout << "CERERE ID: " << id << " | Data: " << timestamp << endl;
    cout << "Status: ";
    switch(status) {
        case Status_cerere::in_asteptare: cout << "In Asteptare"; break;
        case Status_cerere::asignata: cout << "Asignata (Tehnician " << id_tehnician_asignat << ")"; break;
        case Status_cerere::finalizata: cout << "Finalizata"; break;
        default: cout << "Necunoscut";
    }
    cout << endl;
    
    if (aparat) {
        aparat->afisare();
    }

    cout << "Complexitate: " << complexitate << endl;
    cout << "Durata estimata: " << durata_estimata << " unitati timp" << endl;
    cout << "Cost estimat: " << pret_estimat << " RON" << endl;
    cout << "========================================" << endl;
}

