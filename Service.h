#pragma once
#include <vector>
#include <deque>  
#include <list>   
#include <map>
#include <memory>
#include "Angajat.h"
#include "Electrocasnic.h"
#include "Cerere.h"

using namespace std;

//Singleton
class Service {
private:
    Service() {}

    // Datele aplicatiei
    vector<shared_ptr<Angajat>> angajati;
    vector<shared_ptr<Electrocasnic>> catalog_modele;
    map<string, int> statistica_nereparate; //lista aparate care nu pot fi reparate

    deque<shared_ptr<Cerere>> coada_asteptare; // Acces rapid la capete
    list<shared_ptr<Cerere>> cereri_active; // Stergere rapida oriunde
    vector<shared_ptr<Cerere>> istoric_cereri; // vector pt stocare

    
    // Metode private pentru citire
    int get_incarcare_tehnician(int);
    void proceseaza_linie_angajat(stringstream&, int);
    void proceseaza_linie_model(stringstream&, int );
    void proceseaza_linie_cerere(stringstream&, int, int& );

    

public:

    static Service& getInstance();

    Service(const Service&) = delete;
    void operator=(const Service&) = delete;

    //Incarcare scenarii
    void incarca_scenariu(const string&);

    //Gestiune angajati
    void adauga_angajat();
    void sterge_angajat();
    void modifica_angajat();
    void cauta_angajat_cnp();
    void afiseaza_angajati() const;

    //Gestiune electrocasnice
    void adauga_model_suportat();
    void sterge_model_suportat();
    void afiseaza_aparate_reparate() const;
    void afiseaza_aparate_nereparate() const;

    //Procesare cereri
    void ruleaza_simulare(int);

    //Rapoarte
    void raport_top_salarii();
    void raport_tehnician_top_durata();
    void raport_cereri_asteptare();
    

};
