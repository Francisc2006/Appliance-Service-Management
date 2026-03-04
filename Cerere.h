#pragma once
#include "Electrocasnic.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Cerere{
private:
    const int id;
    shared_ptr<Electrocasnic> aparat; 
    time_t timestamp; 
    int complexitate; 
    
    // de calculat
    int durata_estimata;
    double pret_estimat;
    
    Status_cerere status;
    int id_tehnician_asignat; 

public:
    Cerere(int, shared_ptr<Electrocasnic>, string, int);

    string get_timestamp_str() const;
    int get_id() const;
    shared_ptr<Electrocasnic> get_electrocasnic() const;
    time_t get_timestamp_raw() const;
    int get_complexitate() const;
    int get_durata() const;
    double get_pret() const;
    int get_id_tehnician() const;
    Status_cerere get_status() const;
    
    void asigneaza_tehnician(int);
    void marcheaza_finalizata();
    void scade_timp_ramas(); 

    void afisare() const;
};
